#include "QTS.hh"



namespace H2D {


/*
 *   QTS::Node members
 */


QTS::Node::Node(const Data2dim&     aData,
                const rectangle_t&  aBr,
                const uint          aLevel,
                const QTS&          aQTS,
                const bool          a11) // for this one, no cum freq stored
            : _data(aData),
              _br(aBr),
              _cumFreq(aData.total()),
              _sse(0),
              _child(),
              _level(aLevel),
              _nodeId(aQTS.getNodeId()),
              _sizeInBits(2 + (a11 ? 32 : 0)) {
  for(uint i = 0; i < 2; ++i) {
    for(uint j = 0; j < 2; ++j) {
      _child[i][j] = 0;
    }
  }
  if(k_sse == aQTS.kind()) {
    _sse = aData.sse(0, aData.size()); // in paper: sse! 
  } else
  if(k_var == aQTS.kind()) {
    _sse = aData.variance(0, aData.size()); 
  } else
  if(k_card == aQTS.kind()) {
    _sse = aData.total();
  } else {
    assert(0 > 1);
  }
}

QTS::Node::~Node() {
  for(uint i = 0; i < 2; ++i) {
    for(uint j = 0; j < 2; ++j) {
      if(0 != child(i,j)) {
        delete _child[i][j];
      }
    }
  }
}


void
QTS::Node::performSplit(const QTS& aQTS) {
  if(aQTS.trace()) {
    std::cout << "split Node" << nodeId() << '@' << level() << ": "
              << "   w = " << sse()
              << std::endl;
  }

  data2dim_at        lPartition(2,2);
  partitiondescxy_t  lPdXY(br(), 2, 2);
  for(uint i = 0; i < data().size(); ++i) {
    const xyc_t& e = data()[i];
    const uint lIdxX = lPdXY.idxXcorrected(e.x);
    const uint lIdxY = lPdXY.idxYcorrected(e.y);
    if(lIdxX < 2 && lIdxY < 2) {
      lPartition(lIdxX, lIdxY).push_back(e);
    }
  }


  rectangle_t lChildBr;
  for(uint i = 0; i < 2; ++i) {
    for(uint j = 0; j < 2; ++j) {
      if(0 < lPartition(i,j).size()) {
        lPdXY.getRectangle(i, j, lChildBr);
        _child[i][j]  = new Node(lPartition(i,j), lChildBr, level() + 1, aQTS, 2 == (i+j));
      }
      if(aQTS.trace()) {
        std::cout << "   size(" << i << ',' << j << ") = " << lPartition(i,j).size() << std::endl;
      }
    }
  }
}


double
QTS::Node::estimate(const rectangle_t& aQueryRectangle, const rectangle_t& aBr, const QTS& aQTS) const {
  if(aQTS.trace()) {
    std::cout << std::string(2*level(), ' ')
              << "Node::estimate: " << aQueryRectangle << "  " << aBr << std::endl;
  }

  if(aQueryRectangle.contains(aBr)) {
    if(aQTS.trace()) {
      std::cout << std::string(2*level(), ' ')
                << "note: fully contained." << std::endl;
    }
    return cumFreq();
  }

  rectangle_t lIsec;
  lIsec.isec(aQueryRectangle, aBr);
  if(lIsec.hasZeroArea()) {
    if(aQTS.trace()) {
      std::cout << std::string(2*level(), ' ')
                << "note empty intersection." << std::endl;
    }
    return 0;
  }

  if(aQueryRectangle.contains(aBr)) {
    if(aQTS.trace()) {
      std::cout << std::string(2*level(), ' ')
                << "note: fully contained." << std::endl;
    }
    return cumFreq();
  }

  if(isLeaf()) {
    const double lEstimate =  ((lIsec.area() / aBr.area()) * cumFreq());
    if(aQTS.trace()) {
      std::cout << std::string(2*level(), ' ')
                << "note: leaf with cumfreq: " << cumFreq() 
                << " results in estimate " << lEstimate
                << std::endl;
    }
    return lEstimate;
  }

  double lEstimate = 0;

  partitiondescxy_t lPd(aBr, 2, 2);
  rectangle_t       lTile;
  for(uint i = 0; i < 2; ++i) {
    for(uint j = 0; j < 2; ++j) {
      if(0 != child(i, j)) {
        lPd.getRectangle(i, j, lTile);
//	std::cout<<"child: "<<i<<std::endl;
        lEstimate += child(i, j)->estimate(lIsec, lTile, aQTS);
      }
    }
  }

  return lEstimate;
}


uint
QTS::Node::depth() const {
  uint lChildDepth = 0;
  for(uint i = 0; i < 2; ++i) {
    for(uint j = 0; j < 2; ++j) {
      if(0 != child(i, j)) {
        const uint d = child(i, j)->depth();
        if(d > lChildDepth) {
          lChildDepth = 0;
        }
      }
    }
  }
  return 1 + lChildDepth;
}

uint
QTS::Node::noNodes() const {
  uint lRes = 1; // this
  for(uint i = 0; i < 2; ++i) {
    for(uint j = 0; j < 2; ++j) {
      if(0 != child(i, j)) {
        lRes += child(i, j)->noNodes();
      }
    }
  } 
  return lRes;
}



std::ostream&
QTS::Node::print(std::ostream& os) const {
  std::cout << std::string(2 * level(), ' ') 
            << "node br " << br() << " cf " << cumFreq() << std::endl;
  for(uint i = 0; i < 2; ++i) {
    for(uint j = 0; j < 2; ++j) {
      if(0 != child(i, j)) {
        child(i, j)->print(os);
      }
    }
  }
  return os;
}


/* 
 *   QTS members 
 */

QTS::QTS(const Data2dim& aData,
         const kind_t    aKind,
         const uint      aBudget,
         const uint      aPhi,
         const double    aQ,
         const double    aTheta,
         const bool      aTrace) : EstimatorBase2dim(aQ, aTheta),
                                   _outlier(),
                                   _br(),
                                   _kind(aKind),
                                   _budget(aBudget * 8),
                                   _size(0),
                                   _phi(aPhi),
                                   _root(0),
                                   _nodeCount(0),
                                   _trace(aTrace) {
  init(aData);

  // first test [only for longlat.dat]
  if(false) {
    std::cout << std::endl;
    std::cout << "=== first estimate ===" << std::endl;
    rectangle_t lQueryRectangle(80.0, 30.0, 120.0, 40.0);
    double lTrueVal = aData.countWithin(lQueryRectangle);
    double lEstVal  = estimate(lQueryRectangle);
    std::cout << "query rectangle: " << lQueryRectangle << std::endl;
    std::cout << "true val: " << lTrueVal << std::endl;
    std::cout << "est  val: " << lEstVal  << std::endl;
    std::cout << "=== first estimate ===" << std::endl;
    std::cout << std::endl;
  }

}

QTS::~QTS() {
  if(0 != _root) {
    delete _root;
  }
}



void
QTS::init(const Data2dim& aData) {
  Data2dim lRegular;
  if(8 < phi()) {
    aData.split(lRegular, _outlier, phi());
  } else {
    lRegular = aData;
  }

  lRegular.getBoundingRectangle(_br);

  heap_t lHeap;
  _root = new Node(aData, br(), 0, (*this), false);
  lHeap.push(_root);

  uint lTotalSize = root()->sizeInBits();

  if(trace()) {
    std::cout << "before main loop: " << budget() << " <?= " << lTotalSize << std::endl;
  }
  while(budget() > lTotalSize) {
    Node* lTop = lHeap.top();
    lHeap.pop();
    if(7 > lTop->data().size()) { continue; }
    lTop->performSplit(*this);
    for(uint i = 0; i < 2; ++i) {
      for(uint j = 0; j < 2; ++j) {
        Node* lChild = lTop->child(i,j);
        if(0 != lChild) {
          lHeap.push(lChild);
          lTotalSize += lChild->sizeInBits();
        }
      }
    }
    if(trace()) {
      std::cout << "within main loop: " << budget() << " <?= " << lTotalSize << std::endl;
    }
  }

  _size = lTotalSize;

}



double
QTS::estimate(const query_t& lQuery) const {
  rectangle_t r = lQuery.rectangle();
  return estimate(r);
}


double
QTS::estimate(const rectangle_t& r) const {
  double lEstimate = 0;
  if(0 != root()) {
    lEstimate = root()->estimate(r, br(), (*this));
  }
  lEstimate += (double) outlierCount(r);
  return std::max<double>(1.0, lEstimate);
}



uint
QTS::outlierCount(const rectangle_t& r) const {
  uint lRes = 0;
  for(uint i = 0; i < outlier().size(); ++i) {
    const xyc_t& p = outlier()[i];
    if(r.containsHalfOpen(p.x, p.y)) {
      lRes += p.c;
    }
  }
  return lRes;
}


uint
QTS::size() const {
  return (_size / 8);
}

uint
QTS::depth() const {
  if(0 == root()) {
    return 0;
  }
  return root()->depth();
}

uint
QTS::noNodes() const {
  if(0 == root()) {
    return 0;
  }
  return root()->noNodes();
}

std::ostream&
QTS::print(std::ostream& os) const {
  std::cout << "QTS: " << std::endl;
  std::cout << "  tree:" << std::endl;
  _root->print(os);
  return os;
}

std::ostream&
QTS::print_name_param(std::ostream& os) const {
  std::cout << h2d_kind_name(H2D_QTS) << ' '
            << kind() // subkind
            << '('
            << ')'
            << std::endl;
  return os;
} 



} // end namespace



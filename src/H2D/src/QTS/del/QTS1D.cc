#include "QTS1D.hh"



namespace H2D {


/*
 *   QTS::Node members
 */


QTS1D::Node::Node(const Data2dim&     aData,
                const line_t&       aBr,
                const uint          aLevel,
                const QTS1D&          aQTS,
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
      _child[i] = 0;
    
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

QTS1D::Node::~Node() {
  for(uint i = 0; i < 2; ++i) {
      if(0 != child(i)) {
        delete _child[i];
      
    }
  }
}


void
QTS1D::Node::performSplit(const QTS1D& aQTS) {
  if(aQTS.trace()) {
    std::cout << "split Node" << nodeId() << '@' << level() << ": "
              << "   w = " << sse()
              << std::endl;
  }

  data2dim_at      lPartition(2,1);
  partitiondesc_t  lPdX;
  lPdX.set(br()._colLo,br()._colHi, 2);
  if(aQTS.dim()==0){
  for(uint i = 0; i < data().size(); ++i) {
    const xyc_t& e = data()[i];
    const uint lIdxX = lPdX.idxCorrected(e.x);
    if(lIdxX < 2 ) {
      lPartition(lIdxX,0).push_back(e);
    }
  }}
  else {
  for(uint i = 0; i < data().size(); ++i) {
    const xyc_t& e = data()[i];
    const uint lIdxX = lPdX.idxCorrected(e.y);
    if(lIdxX < 2 ) {
      lPartition(lIdxX,0).push_back(e);
    }
  }
  
  
  
  }
  

  line_t lChildBr;
  for(uint i = 0; i < 2; ++i) {
	  if(0 < lPartition(i,0).size()) {
        lChildBr._colLo = lPdX.boundary(i);
	lChildBr._colHi = lPdX.boundary(i+1);
        _child[i]  = new Node(lPartition(i,0), lChildBr, level() + 1, aQTS, 2 == (i+1));
      }
      if(aQTS.trace()) {
        std::cout << "   size(" << i <<  ") = " << lPartition(i,1).size() << std::endl;
      }
    }
  
}


double
QTS1D::Node::estimate(const line_t& aQueryLine, const line_t& aBr, const QTS1D& aQTS) const {
 //std::cout<<"Br lo and hi"<<aBr.colLo()<<"      "<<aBr.colHi()<<std::endl;
 //std::cout<<"q lo and hi"<<aQueryLine.colLo()<<"      "<<aQueryLine.colHi()<<std::endl;
  if(aQueryLine.containsLine(aBr)) {
//	  std::cout<<"contains"<<std::endl;
    return cumFreq();
  }

  line_t lIsec;
  lIsec.lisec(aQueryLine, aBr);
  if(lIsec.hasZeroLength()) {
//std::cout<<"zero length"<<std::endl;
    return 0;
  }


  if(isLeaf()) {
//	  std::cout<<"leaf"<<std::endl;
    const double lEstimate =  ((lIsec.length() / aBr.length()) * cumFreq());
    return lEstimate;
  }

  double lEstimate = 0;
//std::cout<<"inner node"<<std::endl;
  //partitiondesc_t lPdX(aBr._colLo,aBr._colHi, 2);
  partitiondesc_t lPdX;
  lPdX.set(aBr._colLo, aBr._colHi,2);
  line_t       lTile;
  for(uint i = 0; i < 2; ++i) {
      if(0 != child(i)) {
        //lPd.getRectangle(i, j, lTile);
	lTile._colLo = lPdX.boundary(i);
	lTile._colHi = lPdX.boundary(i+1);
//	std::cout<<"child: "<<i<<"   "<<"lTile lo and hi :"<<lTile.colLo()<<"   "<<lTile.colHi()<<"    lisec q with this node:"<<lIsec.colLo()<<"      "<<lIsec.colHi()<<std::endl;

        lEstimate += child(i)->estimate(lIsec, lTile, aQTS);
    }
//      std::cout<<"child: "<<i<<"   "<<"Is leaf?"<<isLeaf()<<"    lTile lo and hi :"<<lTile.colLo()<<"   "<<lTile.colHi()<<"    lisec q with this node:"<<lIsec.colLo()<<"      "<<lIsec.colHi()<<"Estimate till this child   "<<lEstimate<<std::endl;


  }
  //std::cout<<"estimate of node:  "<<lEstimate<<std::endl;
  return lEstimate;
}


uint
QTS1D::Node::depth() const {
  uint lChildDepth = 0;
  for(uint i = 0; i < 2; ++i) {
      if(0 != child(i)) {
        const uint d = child(i)->depth();
        if(d > lChildDepth) {
          lChildDepth = 0;
       	}
    }
  }
  return 1 + lChildDepth;
}

uint
QTS1D::Node::noNodes() const {
  uint lRes = 1; // this
  for(uint i = 0; i < 2; ++i) {
      if(0 != child(i)) {
        lRes += child(i)->noNodes();
      }
  } 
  return lRes;
}



std::ostream&
QTS1D::Node::print(std::ostream& os) const {
  for(uint i = 0; i < 2; ++i) {
      if(0 != child(i)) {
        child(i)->print(os);
      }
  }
  return os;
}


/* 
 *   QTS members 
 */

QTS1D::QTS1D(const Data2dim& aData,
         const kind_t    aKind,
         const uint      aBudget,
         const uint      aPhi,
         const bool      aTrace,
	 const uint      dim) :
                                   _outlier(),
                                   _br(),
                                   _kind(aKind),
                                   _budget(aBudget * 8/2),//devide by dim count
                                   _size(0),
                                   _phi(aPhi),
                                   _root(0),
                                   _nodeCount(0),
                                   _trace(aTrace),
                                   _dim(dim)
{
  init(aData);
  //std::cout<<"begooo ey yar begoo"<<std::endl;
}

QTS1D::~QTS1D() {
  if(0 != _root) {
    delete _root;
  }
}



void
QTS1D::init(const Data2dim& aData) {
  //std::cout<<"resid inja"<<std::endl;
  Data2dim lRegular;
  if(8 < phi()) {
	 // std::cout<<"split inja"<<std::endl;
    aData.split(lRegular, _outlier, phi());
  } else {
    lRegular = aData;
  }

  lRegular.getBoundingLine(_br,dim());

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
    lTop->performSplit(*this);
    for(uint i = 0; i < 2; ++i) {
        Node* lChild = lTop->child(i);
        if(0 != lChild) {
          lHeap.push(lChild);
          lTotalSize += lChild->sizeInBits();
	}
    }
    if(trace()) {
      std::cout << "within main loop: " << budget() << " <?= " << lTotalSize << std::endl;
    }
  }

  _size = lTotalSize;

}





double
QTS1D::estimate(const line_t& r) const {
  double lEstimate = 0;
  if(0 != root()) {
    lEstimate = root()->estimate(r, br(), (*this));
  }
  lEstimate += (double) outlierCount(r);
  return std::max<double>(1.0, lEstimate);
}



uint
QTS1D::outlierCount(const line_t& r) const {
  uint lRes = 0;
  if(dim()==0){
	  for(uint i = 0; i < outlier().size(); ++i) {
    	  const xyc_t& p = outlier()[i];
          if(r.containsValue(p.x)) {
          lRes += p.c;
    }
  }
  }
  else{
	  for(uint i = 0; i < outlier().size(); ++i) {
          const xyc_t& p = outlier()[i];
          if(r.containsValue(p.y)) {
          lRes += p.c;
    }
  }

  }
  return lRes;
}


uint
QTS1D::size() const {
  return (_size / 8);
}

uint
QTS1D::depth() const {
  if(0 == root()) {
    return 0;
  }
  return root()->depth();
}

uint
QTS1D::noNodes() const {
  if(0 == root()) {
    return 0;
  }
  return root()->noNodes();
}

std::ostream&
QTS1D::print(std::ostream& os) const {
  std::cout << "QTS: " << std::endl;
  std::cout << "  tree:" << std::endl;
  _root->print(os);
  return os;
}

 


}
 // end namespace



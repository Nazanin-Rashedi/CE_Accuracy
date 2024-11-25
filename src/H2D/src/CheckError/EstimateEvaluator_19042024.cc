#include "EstimateEvaluator.hh"

namespace H2D {


#define NOPROFILE 12
#define NOTHETA 6
#define NOQERRCLASSES 33
#define NOSELCLASSES 28

EstimateEvaluator::EstimateEvaluator() : _theta(), _aggregates(), _aggregatesNZ(), _qerrProfile(),
                                         _thetaFixed(), _aggrFixed(4), _aggrFixedNZ(4), _countBadFixedNZ(4), _total(0), _qselPercentile(),_allQuerySelPercentiles(),_queryClass(NOTHETA, std::vector<std::vector<uint>>(NOSELCLASSES, std::vector<uint>(NOQERRCLASSES, 0))) {
  _thetaFixed.push_back(500);
  _thetaFixed.push_back(1000);
  _thetaFixed.push_back(5000);
  _thetaFixed.push_back(10000);

  _countBadFixedNZ[0] = 0;
  _countBadFixedNZ[1] = 0;
  _countBadFixedNZ[2] = 0;
  _countBadFixedNZ[3] = 0;
}



EstimateEvaluator::~EstimateEvaluator() {
}


void
EstimateEvaluator::push_back(const uint aTheta) {
  _theta.push_back(aTheta);
  aggr_t lAggr;
  _aggregates.push_back(lAggr);
  _aggregatesNZ.push_back(lAggr);\
  uint_vt lX(NOPROFILE);
  _qerrProfile.emplace_back(lX);
  // uint* lX = new uint[NOPROFILE];
  // memset(lX, 0, NOPROFILE * sizeof(uint));
  // _qerrProfile.push_back(lX);
}

void
EstimateEvaluator::init() {
  
  for(uint i = 0; i < no_theta(); ++i) {
    _aggregates[i].init(0);
    _aggregatesNZ[i].init(0);
  }
  for(uint i = 0; i < 4; ++i) {
    _aggrFixed[i].init(0);
    _aggrFixedNZ[i].init(0);
  }
  for(uint i = 0; i < qerrProfile().size(); ++i) {
    for(uint j = 0; j < qerrProfile(i).size(); ++j) {
      _qerrProfile[i][j] = 0;
    }
  }
  for(uint i = 0; i < _countBadFixedNZ.size(); ++i) {
    _countBadFixedNZ[i] = 0;
  }

  //for(uint i = 0; i < no_theta(); ++i) {
  //  for(uint j = 0; j < 3; ++j) {
  //    _qselPercentile[i][j] = 0;
  //  }
//}

}




/*
 * for each theta[i]:
 * _qerrProfile[i][j]: enthaelt counts:
 * _qerrProfile[i][NOPROFILE-1] : number of overestimates  with qerror >= 3
 * _qerrProfile[i][NOPROFILE-2] : number of underestimates with qerror >= 3
 * _qerrProfile[i][NOPROFILE-3] : number of estimates with qerror >= (NOPROFILE-2)
 * _qerrProfile[i][k]           : number of estimates with qerror <= (k+1), k < NOPROFILE-3
 *
 */

void
EstimateEvaluator::step(const uint aTrueValue, const uint aEstimate) {
  // To avoid infinity, we set zeros to ones.
  const double lQError = q::qerror<double>(0 == aTrueValue ? 1 : aTrueValue, 
                                           0 == aEstimate  ? 1 : aEstimate);

  double qsel = 0.0;
  uint qselPer = 0;
  qsel = 1.0*aTrueValue/(std::max<double>(_total,1)*1.0); 
  //std::cout<<_total<<"    "<<aTrueValue<<"     "<<qsel<<std::endl;
  if(aTrueValue < 2.0) {qselPer = 1;}
  else if (qsel<=0.005) {qselPer = 2;}
  else if (qsel<=0.02) {qselPer = 3;}
  else {qselPer = 4;}
  //std::cout<<_total<<"    "<<aTrueValue<<"     "<<qsel<<"	"<<qselPer<<std::endl;

  ++_allQuerySelPercentiles[qselPer-1];

  const uint lNZ_LIMIT = 1000;

  for(uint i = 0; i < no_theta(); ++i) {
    const bool lOverTheta = ((aTrueValue > theta(i)) || (aEstimate > theta(i)));
    if(lOverTheta) {
      _aggregates[i].step(lQError);
      if(lNZ_LIMIT < aTrueValue) {
        _aggregatesNZ[i].step(lQError);
      }
      uint lIdx = 0;
      if(lQError > 2.0) {
       ++_qselPercentile[i][qselPer-1];
        lIdx = (uint) floor(lQError - 1.00000000001);
        if(aTrueValue > aEstimate) {
          ++_qerrProfile[i][NOPROFILE-2]; // underestimate
        } else {
          ++_qerrProfile[i][NOPROFILE-1]; // overestimate
        }
       } else {
        lIdx = (uint) floor(lQError) - 1;
      }
      if(lQError==2){lIdx = 0;}
      if(lIdx > (NOPROFILE - 3)) {
        lIdx = (NOPROFILE - 3);
      }

  
/*
      if(100 <= aTrueValue || 100 <= aEstimate) {
        ++_qerrProfile[i][NOPROFILE-2];
      }
      if(1000 <= aEstimate || 1000 <= aEstimate) {
        ++_qerrProfile[i][NOPROFILE-1];
      }
*/

      //++_allQuerySelPercentiles[qselPer-1];
      ++_qerrProfile[i][lIdx];
    }

    
  }

  for(uint i = 0; i < 4; ++i) {
    const bool lOverTheta = ((aTrueValue > _thetaFixed[i]) || (aEstimate > _thetaFixed[i]));
    if(lOverTheta) {
      _aggrFixed[i].step(lQError);
      if(_thetaFixed[i] < aTrueValue) { // CHANGED! used to be (lNZ_LIMIT < aTrueValue)
        _aggrFixedNZ[i].step(lQError);
        if(2.0 < lQError) {
          ++_countBadFixedNZ[i];
        }
      }
    }
  }

}


void
EstimateEvaluator::fin() {
  for(uint i = 0; i < no_theta(); ++i) {
    _aggregates[i].fin();
    _aggregatesNZ[i].fin();
  }
  for(uint i = 0; i < 4; ++i) {
    _aggrFixed[i].fin();
    _aggrFixedNZ[i].fin();
  }
}


void
EstimateEvaluator::nstep(const uint aTrueValue, const uint aEstimate) {
  // To avoid infinity, we set zeros to ones.
  const double lQError = q::qerror<double>(0 == aTrueValue ? 1 : aTrueValue, 
                                           0 == aEstimate  ? 1 : aEstimate);
  
  
  uint selClass = selectivity_class(aTrueValue,_total);
  uint qerrClass = qerror_class(lQError);

  for(uint i = 0; i < no_theta(); ++i) {
    const bool lOverTheta = ((aTrueValue > theta(i)) || (aEstimate > theta(i)));
    if(lOverTheta) {
    ++_queryClass[i][selClass][qerrClass];
    }
    else{
    ++_queryClass[i][selClass][0];
    }
      }
    }



void
EstimateEvaluator::step(const uint aTrueValue, const uint aEstimate, std::ostream& fos) {
  // To avoid infinity, we set zeros to ones.
  const double lQError = q::qerror<double>(0 == aTrueValue ? 1 : aTrueValue, 
                                           0 == aEstimate  ? 1 : aEstimate);
  
  fos << aTrueValue << "," << aEstimate ;

  double qsel = 0.0;
  uint qselPer = 0;
  qsel = 1.0*aTrueValue/(std::max<double>(_total,1)*1.0);
  //std::cout<<_total<<"    "<<aTrueValue<<"     "<<qsel<<std::endl;
  if(aTrueValue < 2.0) {qselPer = 1;}
  else if (qsel<=0.005) {qselPer = 2;}
  else if (qsel<=0.02) {qselPer = 3;}
  else {qselPer = 4;}
  ++_allQuerySelPercentiles[qselPer-1];
  //std::cout<<_total<<"    "<<aTrueValue<<"     "<<qsel<<"	"<<qselPer<<std::endl;
  const uint lNZ_LIMIT = 1000;

  for(uint i = 0; i < no_theta(); ++i) {
    const bool lOverTheta = ((aTrueValue > theta(i)) || (aEstimate > theta(i)));
    if(lOverTheta) {
       fos << "," <<lQError;
      _aggregates[i].step(lQError);
      if(lNZ_LIMIT < aTrueValue) {
        _aggregatesNZ[i].step(lQError);
      }
      uint lIdx = 0;
      if(lQError > 2.0) {
        ++_qselPercentile[i][qselPer-1];
        lIdx = (uint) floor(lQError - 1.00000000001);
        if(aTrueValue > aEstimate) {
          ++_qerrProfile[i][NOPROFILE-2]; // underestimate
        } else {
          ++_qerrProfile[i][NOPROFILE-1]; // overestimate
        }
       } else {
        lIdx = (uint) floor(lQError) - 1;
      }
      if(lQError==2){lIdx = 0;}
      if(lIdx > (NOPROFILE - 3)) {
        lIdx = (NOPROFILE - 3);
      }

/*
      if(100 <= aTrueValue || 100 <= aEstimate) {
        ++_qerrProfile[i][NOPROFILE-2];
      }
      if(1000 <= aEstimate || 1000 <= aEstimate) {
        ++_qerrProfile[i][NOPROFILE-1];
      }
*/  //  ++_allQuerySelPercentiles[qselPer-1];
      ++_qerrProfile[i][lIdx];
    }
  
  }
fos<<std::endl;

  for(uint i = 0; i < 4; ++i) {
    const bool lOverTheta = ((aTrueValue > _thetaFixed[i]) || (aEstimate > _thetaFixed[i]));
    if(lOverTheta) {
      _aggrFixed[i].step(lQError);
      if(_thetaFixed[i] < aTrueValue) { // CHANGED! used to be (lNZ_LIMIT < aTrueValue)
        _aggrFixedNZ[i].step(lQError);
        if(2.0 < lQError) {
          ++_countBadFixedNZ[i];
        }
      }
    }
  }

}


void
EstimateEvaluator::dstep(const uint qNum, const uint aTrueValue, const uint aEstimate) {

EstDetail det(qNum,aTrueValue,aEstimate);
det_v.push_back(det);

	
}

std::ostream& 
EstimateEvaluator::dprint(std::ostream& os) const {

for (const EstDetail& det:det_v){
os << det.qNum << " " << det.trueCard << " " << det.estCard << std::endl;
}
return os;
}



std::ostream&
EstimateEvaluator::nprint(std::ostream& os) const {
//uint thetaIdx = 0; //
for (const auto& t : _queryClass) {

	//uint selClassIdx = 0;//
        for (const auto& s : t) {
	//	os << ((uint)std::floor(std::log2(_total)))<<" ";//
	//	os << thetaIdx<<" ";					    //
	//	os << selClassIdx<<" ";//
            for (uint qe = 0; qe < s.size(); qe++) {
		 
                os << s[qe];
                if (qe < s.size() - 1) {
                    std::cout << " ";
                }
            }
	    os<< std::endl;
	  //  selClassIdx++;
        }
//	thetaIdx++;
         os << std::endl;
    }
return os;
}


std::ostream&
EstimateEvaluator::print(std::ostream& os) const {
  os << std::setw(6) << "theta" << ' '
     << std::setw(12) << "count" << ' '
     << std::setw(12) << "min qerr"   << ' '
     << std::setw(12) << "max qerr"   << ' '
     << std::setw(12) << "avg qerr"   << ' ';
  os << "| ";
  for(uint j =0; j < NOPROFILE; ++j) {
    os << ' ' << std::setw(6) << j;
  } 
  os << std::endl;
  for(uint i = 0; i < no_theta(); ++i) {
    os << std::setw(6) << theta(i) << ' '
       << std::setw(12) << (int64_t) _aggregates[i].count() << ' '
       << std::setw(12) << _aggregates[i].min() << ' '
       << std::setw(12) << _aggregates[i].max() << ' '
       << std::setw(12) << _aggregates[i].avg() << ' ';
    os << '|';
    for(uint j = 0; j < NOPROFILE; ++j) {
      os << ' ' << std::setw(6) << _qerrProfile[i][j];
    }
    os << std::endl;
  }
  return os;
}

std::ostream&
EstimateEvaluator::printShort(std::ostream& os) const {
  os << std::setw(6) << "theta" << ' '
     << std::setw(12) << "count" << ' '
     << std::setw(12) << "min qerr"   << ' '
     << std::setw(12) << "max qerr"   << ' '
     << std::setw(12) << "avg qerr"   << ' ';
  os << std::endl;
  for(uint i = 0; i < no_theta(); ++i) {
    os << std::setw(6) << theta(i) << ' '
       << std::setw(12) << (int64_t) _aggregates[i].count() << ' '
       << std::setw(12) << _aggregates[i].min() << ' '
       << std::setw(12) << _aggregates[i].max() << ' '
       << std::setw(12) << _aggregates[i].avg() << ' ';
    os << std::endl;
  }
  os << std::setw(5) << "theta";
  for(uint i = 0; i < NOPROFILE - 3; ++i) {
     os << " q <= " << std::setw(2) << (2 + i);
  }
  os << ' ' << std::setw(7) << "q > 10";
  os << ' ' << std::setw(7) << "under" << ' ' << std::setw(7) << "over" << std::endl;
  for(uint i = 0; i < qerrProfile().size(); ++i) {
     os << std::setw(5) << theta(i);
     for(uint j = 0; j < qerrProfile(i).size(); ++j) {
       os << ' ' << std::setw(7) << qerrProfile(i)[j]<< ' ';
     }
     os << std::endl;
  } 


  return os;
}

std::ostream&
EstimateEvaluator::printHighQerrorSelPercentile(std::ostream& os) const {

os<<"Selectivity of queries with q-error> 2:"<<std::endl;
os<<std::endl;
//NR
  os << std::setw(6) << "theta" << ' '
     << std::setw(18) << "tiny sel" << ' '
     << std::setw(18) << "low sel"   << ' '
     << std::setw(18) << "mid sel"   << ' '
     << std::setw(18) << "high sel"   << ' ';
  os << std::endl;

  os << std::setw(6) << "" << ' '
     << std::setw(18) << "1row" << ' '
     << std::setw(18) << "1row < sel <= 0.005"   << ' '
     << std::setw(18) << " 0.005 < sel <= 0.02"   << ' '
     << std::setw(18) << "sel > 0.02"   << ' ';
  os << std::endl;

  for(uint i = 0; i < no_theta(); i++) {

    os << std::setw(6) << theta(i);
    for (uint j = 0; j < 4; j++){
     os  << std::setw(18) << (int64_t) _qselPercentile[i][j] << ' ';
    }
    os << std::endl;
  }
return os;
}

std::ostream&
EstimateEvaluator::printQuerySelDistr(std::ostream& os) const {
os<<std::endl;
os<<"Distribution of selectivities for 1M queries:"<<std::endl;
os<<std::endl;
//NR
  os << std::setw(20) << "tiny sel" << ' '
     << std::setw(20) << "low sel"   << ' '
     << std::setw(20) << "mid sel"   << ' '
     << std::setw(20) << "high sel"   << ' ';
  os << std::endl;

  os << std::setw(20) << "1row" << ' '
     << std::setw(20) << "1row < sel <= 0.005"   << ' '
     << std::setw(20) << " 0.005 < sel <= 0.02"   << ' '
     << std::setw(20) << "sel > 0.02"   << ' ';
  os << std::endl;

    for (uint j = 0; j < 4; j++){
     os  << std::setw(20) << (int64_t) _allQuerySelPercentiles[j] << ' ';
    }
    os << std::endl;
  os<<std::endl;
return os;
}



std::ostream&
EstimateEvaluator::printAllforOneEst(std::ostream& os) const {
 // os <<aCb.sds()<<","
   //  <<aCb.ds()<<"," 
  os <<_aggregates[0].min() << "," 
     << _aggregates[0].avg() << ","
     << _aggregates[0].max() << ","
     << 1 - (double)(_qerrProfile[1][0])/1000000;
  return os;
}

std::ostream&
EstimateEvaluator::printOneLine(std::ostream& os, const std::string& aLineMarker) const {
  os << aLineMarker << ' ';
  for(uint i = 0; i < no_theta(); ++i) {
    os << " | " << i << ' '
       << theta(i) << ' '
       << (int64_t) _aggregates[i].count() << " | ";
    for(uint j = 0; j < (NOPROFILE - 2); ++j) {
      os << ' ' << _qerrProfile[i][j];
    }
  }
  return os;
}


std::ostream&
EstimateEvaluator::printQErrProfile(std::ostream& os, const std::string& aLineMarker) const {
  os << aLineMarker;
  for(uint i = 0; i < qerrProfile().size(); ++i) {
     os << " | " << i << ' ' << theta(i);
     for(uint j = 0; j < qerrProfile(i).size(); ++j) {
       os << ' ' << qerrProfile(i)[j];
     }
  }
  os << std::endl;
  return os;
}

} // end namespace



#include "ExpResAggr.hh"

ExpResAggr::ExpResAggr()
       : _data(), _syn_no(-1), _min_card_class(0),  _max_card_class(0) {
}

ExpResAggr::ExpResAggr(const uint aSynNo, const uint aMinCardClass, const uint aMaxCardClass)
       : _data(gNoTheta), _syn_no(aSynNo), _min_card_class(aMinCardClass),  _max_card_class(aMaxCardClass) {
}

void
ExpResAggr::init(const uint aSynNo, const uint aMinCardClass, const uint aMaxCardClass) {
  _syn_no = aSynNo;
  _min_card_class = aMinCardClass;
  _max_card_class = aMaxCardClass;
  _data.resize(gNoTheta);
}

void
ExpResAggr::insert(const uint aTheta, const uint aCardClass, const array_t& aArray) {
  if(0 == _data[aTheta].size()) {
    _data[aTheta].resize(no_cc());
  }
  assert(aCardClass >= min_cc());
  assert(aCardClass <= max_cc());
  const uint lIdxCC = aCardClass - min_cc();
  assert(lIdxCC < _data[aTheta].size());
  array_t& a = _data[aTheta][lIdxCC];
  if(nullptr == a.data()) {
    a.resize(gNoSelClass,gNoQErrClass);
    a.mem_init();
  }
  for(uint i = 0; i < gNoSelClass; ++i) {
    for(uint j = 0; j < gNoQErrClass; ++j) {
       a(i,j) += aArray(i,j);
    }
  }
}

void
ExpResAggr::get_profile_max(SynopsisProfile& aSp) const {
  aSp.init(syn_no(), SynopsisProfile::kMax, min_cc(), max_cc());
  for(uint lTheta = 0; lTheta < gNoTheta; ++lTheta) {
    for(uint lCc = 0; lCc < no_cc(); ++lCc) {
      const array_t& a = _data[lTheta][lCc];
      if(nullptr != a.data()) {
        for(uint lSc = 0; lSc < gNoSelClass; ++lSc) {
          uint lMaxQErrClass = 0;
          for(uint lQc = 0; lQc < gNoQErrClass; ++lQc) {
            if(0 < a(lSc,lQc)) {
              lMaxQErrClass = lQc;
            }
          }
          aSp.set(lTheta, lCc, lSc, lMaxQErrClass);
        }
      } else {
        std::cerr << "Warning: ExpResAggr: get_profile_max: _data["
                  << lTheta << "][" << lCc << "] is empty"
                  << std::endl;
      }
    }
  }
}


std::ostream&
ExpResAggr::print(std::ostream& os) const {
  os << gSynopsis[syn_no()] << std::endl;
  os << "  ";
  for(uint i = 0; i < gNoSelClass; ++i) {
    os << ' ' << std::setw(2) << i;
  }
  os << std::endl;
  const uint lTheta = 0;
  for(uint lCc = 0; lCc < no_cc(); ++lCc) {
    const uint lCardClass = lCc + min_cc();
    const array_t& a = _data[lTheta][lCc];
    if(nullptr != a.data()) {
      os << std::setw(2) << lCardClass;
      for(uint lSc = 0; lSc < gNoSelClass; ++lSc) {
        uint lMaxQErrClass = 0;
        for(uint lQc = 0; lQc < gNoQErrClass; ++lQc) {
          if(0 < a(lSc,lQc)) {
            lMaxQErrClass = lQc;
          }
        }
        os << ' ' << std::setw(2) << qerr_class_char(lMaxQErrClass);
      }
      os << std::endl;
    } else {
      std::cerr << "Warning: _data[" << lTheta << "][" << lCc << "] is empty" << std::endl;
    }
  }
  return os;
}

std::ostream&
ExpResAggr::print_rel(std::ostream& os) const {
  // os << "# table R { string syn, int cc, int sc, int theta, int qc, char qcc, int cnt };" << std::endl; 
  for(uint lTheta = 0; lTheta < gNoTheta; ++lTheta) {
    for(uint lCc = 0; lCc < no_cc(); ++lCc) {
      // std::cout << "print_rel: lTheta = " << lTheta << ", cc = " << lCc << std::endl;
      const array_t& a = _data[lTheta][lCc];
      if((nullptr == a.data()) || (0 == a.noRows()) || (0 == a.noCols())) {
        continue;
      }
      for(uint i = 0; i < gNoSelClass; ++i) {
        for(uint j = 0; j < gNoQErrClass; ++j) {
          if(0 != a(i,j)) {
             os << gSynopsis[syn_no()]
                << '|' << (lCc + min_cc())
                << '|' << i
                << '|'
                << lTheta
                << '|' << j
                << '|' << qerr_class_char(j)
                << '|' << a(i,j)
                << std::endl;
          }
        }
      }
    }
  }
  return os;
}



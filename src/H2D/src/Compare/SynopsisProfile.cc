#include "SynopsisProfile.hh"

SynopsisProfile::SynopsisProfile()
                : _data(),
                  _syn_no(0),
                  _kind(kNoKind),
                  _min_card_class(0),
                  _max_card_class(0) {
}

SynopsisProfile::SynopsisProfile(const uint   aSynNo,
                                 const kind_t aKind,
                                 const uint   aMinCardClass,
                                 const uint   aMaxCardClass)
                : _data(),
                  _syn_no(0),
                  _kind(kNoKind),
                  _min_card_class(0),
                  _max_card_class(0) {
  init(aSynNo, aKind, aMinCardClass, aMaxCardClass);
}

void
SynopsisProfile::init(const uint   aSynNo,
                      const kind_t aKind,
                      const uint   aMinCardClass,
                      const uint   aMaxCardClass) {
  _syn_no = aSynNo;
  _kind   = aKind;
  _min_card_class = aMinCardClass;
  _max_card_class = aMaxCardClass;
  _data.resize(gNoTheta);
  for(uint i = 0; i < gNoTheta; ++i) {
    _data[i].resize(no_cc(), no_sc());
    _data[i].mem_init();
  }
}

bool
SynopsisProfile::operator<=(const SynopsisProfile& x) const {
  for(uint lTheta = 0; lTheta < gNoTheta; ++lTheta) {
    for(uint lCc = 0; lCc < no_cc(); ++lCc) {
      for(uint lSc = 0; lSc < no_sc(); ++lSc) {
        if(get(lTheta, lCc, lSc) > x.get(lTheta, lCc, lSc)) {
          return false;
        }
      }
    }
  }
  return true;
}

bool
SynopsisProfile::operator<(const SynopsisProfile& x) const {
  bool lRes = false;
  for(uint lTheta = 0; lTheta < gNoTheta; ++lTheta) {
    for(uint lCc = 0; lCc < no_cc(); ++lCc) {
      for(uint lSc = 0; lSc < no_sc(); ++lSc) {
        if(get(lTheta, lCc, lSc) > x.get(lTheta, lCc, lSc)) {
          return false;
        }
        if(get(lTheta, lCc, lSc) < x.get(lTheta, lCc, lSc)) {
          lRes = true;
        }
      }
    }
  }
  return lRes;
}

std::tuple<bool,uint,uint>
SynopsisProfile::compare_1(const SynopsisProfile& x) const {
 bool lRes = true;
 uint lCnt = 0;
 uint lMxDiff = 0;
 for(uint lTheta = 0; lTheta < gNoTheta; ++lTheta) {
    for(uint lCc = 0; lCc < no_cc(); ++lCc) {
      for(uint lSc = 0; lSc < no_sc(); ++lSc) {
        if(get(lTheta, lCc, lSc) > x.get(lTheta, lCc, lSc)) {
          if(get(lTheta, lCc, lSc) <= (x.get(lTheta, lCc, lSc) + 1)) {
            ++lCnt;
          } else {
            lRes = false;
            lMxDiff = std::max(lMxDiff, get(lTheta, lCc, lSc) - x.get(lTheta, lCc, lSc));
          }
        }
      }
    }
  }
  return {lRes, lCnt, lMxDiff};
}


std::tuple<bool,uint,uint,uint>
SynopsisProfile::compare_n(const SynopsisProfile& x, const uint lTheta) const {
 bool lRes = true;
 uint lbCnt = 0;
 uint bCnt = 0;
 uint lMxDiff = 0;
 //for(uint lTheta = 0; lTheta < gNoTheta; ++lTheta) {
    for(uint lCc = 0; lCc < no_cc(); ++lCc) {
      for(uint lSc = 0; lSc < no_sc(); ++lSc) {
        if(get(lTheta, lCc, lSc) > x.get(lTheta, lCc, lSc)) {
	++bCnt;
          if(get(lTheta, lCc, lSc) <= (x.get(lTheta, lCc, lSc) + 1)) {
            ++lbCnt;
          } else {
            lRes = false;
            lMxDiff = std::max(lMxDiff, get(lTheta, lCc, lSc) - x.get(lTheta, lCc, lSc));
          }
        }
	
      }
    }
 // }
  return {lRes, bCnt, lbCnt, lMxDiff};
}



std::ostream&
SynopsisProfile::print(std::ostream& os) const {
  for(uint lTheta = 0; lTheta < gNoTheta; ++lTheta) {
    print(os, lTheta) << std::endl;
  }
  return os;
}

std::ostream&
SynopsisProfile::print(std::ostream& os, const uint aTheta) const {
  os << gSynopsis[syn_no()] << " (theta = " << aTheta << ')' << std::endl;
  os << "  ";
  for(uint lSc = 0; lSc < no_sc(); ++lSc) {
    os << ' ' << std::setw(2) << lSc; }
  os << std::endl;
  for(uint lCc = 0; lCc < no_cc(); ++lCc) {
    os << std::setw(2) << (lCc + min_cc());
    for(uint lSc = 0; lSc < no_sc(); ++lSc) {
      os << ' ' << std::setw(2) << qerr_class_char(get(aTheta, lCc, lSc));
    }
    os << std::endl;
  }
  return os;
}




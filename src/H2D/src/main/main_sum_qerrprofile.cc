#include "infra/glob_infra_standard_includes.hh"
#include <fstream>
#include <map>
#include <set>

#include "infra/types.hh"

/*
 * reads a file with many q-qerror profile lines adds up the counts
 * and ouputs the percentages of these counts in terms of total number of queries
 * the lines should be produced via EstimateEvaluator::printQErrProfile
 * with line marker as in ProcessQueryFile::run_one_estimator
 * which results in the following format:
 * '!' 'qerr-profile' <sds> <ds> <no-queries> <estimator-name> <estimator-number> 
 * then follow five entries of the form
 * '|' <i> <theta> <cnt_{i,0}> .. <cnt_{i,11}>
 * where <cnt_{i,j}> is from EstimateEvaluator::_qerrProfile
 *
 * here, we aggregate over all cardinality classes
 */

constexpr uint gNoTheta   =  6;
constexpr uint gNoProfile = 12;

typedef std::vector<uint64_vt> uint64_vvt;
// map an estimator kind to a summed up q-error profile
typedef std::map<H2D::H2D_kind_t, uint64_vvt> map_t;
typedef std::map<H2D::H2D_kind_t, uint64_t>   noquery_t;

struct nice {
  nice(const double x) : _x(x) {}
  double _x;
};

std::ostream&
operator<<(std::ostream& os, const nice& x) {
  if(10 < x._x) {
    os << ((int) std::round(x._x));
  } else {
    os << mt::roundXt(x._x);
  }
  return os;
}

void
print_fraction_qerr_gt_10(const map_t& aMap, const uint64_t aTotNoQuery) {
  std::cout << "percentage of queries with q-error $>$ 10" << std::endl;
  std::cout << "Estimator";
  std::cout << " & " << std::setw(11) <<  1;
  std::cout << " &  $\\theta_0$";
  for(uint i = 2; i <= 5; ++i) {
    std::cout << " & $" << i << "\\theta_0$";
  }
  std::cout << std::endl;
  std::cout.precision(6);
  std::cout << std::fixed;
  const double lDiv = (((double) aTotNoQuery) / ((double) 100));
  for(const auto& X: aMap) {
    std::cout << std::setw(9) << H2D::h2d_kind_name(X.first);
    const auto& lQp = X.second;
    for(uint i = 0; i < lQp.size(); ++i) {
      const double lNoQErrGt10 = lQp[i][gNoProfile - 3];
      std::cout << " & " << std::setw(11) << (lNoQErrGt10 / lDiv);
    }
    std::cout << std::endl;
  }
}

void
print_fraction_qerr_gt_lim(const map_t& aMap, const uint64_t aTotNoQuery, const uint aLim) {
  assert(2 <= aLim);
  assert(10 >= aLim);
  std::cout << "percentage of queries with q-error $>$ " << aLim << std::endl;
  std::cout << "Estimator";
  std::cout << " & " << std::setw(11) <<  1;
  std::cout << " &  $\\theta_0$";
  for(uint i = 2; i <= 5; ++i) {
    std::cout << " & $" << i << "\\theta_0$";
  }
  std::cout << std::endl;
  std::cout.precision(6);
  std::cout << std::fixed;
  const uint   lBegin = aLim - 1;
  const uint   lEnd   = gNoProfile - 2;
  const double lDiv   = (((double) aTotNoQuery) / ((double) 100));
  for(const auto& X: aMap) {
    std::cout << std::setw(9) << H2D::h2d_kind_name(X.first);
    const auto& lQp = X.second;
    for(uint i = 0; i < lQp.size(); ++i) {
      uint64_t lNoQErrGtLim = 0;
      for(uint j = lBegin; j < lEnd; ++j) {
        lNoQErrGtLim += lQp[i][j];
      }
      std::cout << " & " << std::setw(11) << (((double) lNoQErrGtLim) / lDiv);
    }
    std::cout << std::endl;
  }
}

bool
read(std::istream& aIs, uint64_vvt& aQErrProfile) {
  char c = 0;
  uint lThetaNo = 0;
  uint lTheta   = 0;
  for(uint i = 0; i < gNoTheta; ++i) {
    aIs >> c >> lThetaNo >> lTheta;
    if(aIs.eof()) { return false; }
    // std::cout << "read: " << c << ' ' << lThetaNo << ' ' << lTheta << std::endl;
    assert('|' == c);
    assert(lThetaNo == i);
    for(uint j = 0; j < gNoProfile; ++j) {
      aIs >> aQErrProfile[i][j];
    }
    if(aIs.eof()) { return false; }
  }
  return true;
}

void
aggregate_profile(uint64_vvt& aQErrProfileAggr, const uint64_vvt& aQErrProfile) {
  if(0 == aQErrProfileAggr.size()) {
    aQErrProfileAggr.resize(gNoTheta);
  }
  for(uint i = 0; i < gNoTheta; ++i) {
    if(0 == aQErrProfileAggr[i].size()) {
      aQErrProfileAggr[i].resize(gNoProfile);
      for(uint j = 0; j < gNoProfile; ++j) {
        aQErrProfileAggr[i][j] = aQErrProfile[i][j];
      }
    } else {
      for(uint j = 0; j < gNoProfile; ++j) {
        aQErrProfileAggr[i][j] += aQErrProfile[i][j];
      }
    }
  }
}

void
test0(const std::string& aFilename) {
  std::ifstream lIs(aFilename);
  if(!lIs) {
    std::cout << "Can't open file '" << aFilename << "'." << std::endl;
    return;
  }

  uint64_vvt lQErrProfile;
  lQErrProfile.resize(gNoTheta);
  for(uint i = 0; i < lQErrProfile.size(); ++i) {
    lQErrProfile[i].resize(gNoProfile);
  }
  map_t lMap;
  char c = 0;
  std::string     lQErrProfileString;
  std::string     lSDS;
  std::string     lDS;
  uint            lCardClass = 0;
  uint64_t        lNoQuery = 0;
  uint64_t        lTotNoQuery = 0;
  std::string     lEstName;
  uint            lEstKindNo;
  uint            lCountGood = 0;
  uint            lCountBad = 0;
  while(!lIs.eof()) {
    lIs >> c >> lQErrProfileString >> lSDS >> lDS >> lCardClass >> lNoQuery >> lEstName >> lEstKindNo;
    // std::cout << "read: " << lSDS << ' ' << lDS << ' ' << lEstName << std::endl;
    if(lIs.eof()) {
      break;
    }
    const H2D::H2D_kind_t lEstKind = (H2D::H2D_kind_t) lEstKindNo;
    if(read(lIs, lQErrProfile)) {
       aggregate_profile(lMap[lEstKind], lQErrProfile);
       lTotNoQuery += lNoQuery;
       ++lCountGood;
    } else {
      ++lCountBad;
    }
  }
  std::cout << "No good: " << lCountGood << std::endl;
  std::cout << "No bad : " << lCountBad  << std::endl;
  print_fraction_qerr_gt_10(lMap, lTotNoQuery);
  for(uint aLim = 2; aLim <= 10; ++aLim) {
    print_fraction_qerr_gt_lim(lMap, lTotNoQuery, aLim);
  }
}

void
print_usage(const int argc, const char* argv[]) {
  std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
}

int
main(const int argc, const char* argv[]) {
  if(2 != argc) {
    print_usage(argc, argv);
    return -1;
  }
  test0(argv[1]);
  return 0;
}


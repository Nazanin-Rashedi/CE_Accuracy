#include "infra/glob_infra_standard_includes.hh"
#include "infra/summaryline.hh"
#include "infra/aggregate.hh"
#include <map>
#include <set>
#include <fstream>

/*
 * reads a file with many summarylines
 * and ouput certain aggregates per estimator
 * the parameters for all estimators must be equal (comparable)
 */


typedef Aggregate<double> aggr_t;

// map an estimator kind to a max of summaryline_t::_max[5]
typedef std::map<H2D::H2D_kind_t, double[5]>  est2sry_mt;
// vector with one entry for every cardinality class in [0,30] (most will be empty anyway)
typedef std::vector<est2sry_mt>        est2sry_vt;

typedef std::set<H2D::H2D_kind_t> kind_st;

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

/*
 *  print maximum q-errors per cardinality class for each synopsis
 */

void
print(const est2sry_vt& X) {
   std::cout << "max q-errors for different theta" << std::endl;
  if(0 == X.size()) { return; }
  const int w = 8;
  uint lMin = 0;
  for(lMin = 1; (0 == X[lMin].size()) && lMin < X.size(); ++lMin);
  if(X.size() <= lMin) { return; }

  kind_st lKinds;
  for(const auto& x : X[lMin]) {
    lKinds.insert(x.first);
  }
  
  std::cout << "cc & k";
  for(const auto& x : X[lMin]) {
    std::cout << " & " << std::setw(w) << h2d_kind_name(x.first);
  }
  std::cout << std::endl;

  for(uint i = 0; i < X.size(); ++i) {
    if(0 == X[i].size()) { continue; }
    std::cout << std::setw(2) << i;
    for(uint k = 0; k < 5; ++k) {
      if(0 < k) {
        std::cout << "   & " << k;
      } else {
        std::cout << " &  ";
      }
      for(const auto& x : X[i]) {
        std::cout << " & " << std::setw(w) << nice(x.second[k]);
      }
      std::cout << " \\\\" << std::endl;
    }
  }
}

// runtime vector of maps
typedef std::map<H2D::H2D_kind_t, aggr_t> est2rt_mt;
typedef std::vector<est2rt_mt> est2rt_vmt;

/*
 *  print construction time or estimation time
 */

void
print(const est2rt_vmt& X, const std::string& aHeader) {
  std::cout << aHeader << std::endl;
  if(0 == X.size()) { return; }

  uint lMin = 0;
  for(lMin = 1; (0 == X[lMin].size()) && lMin < X.size(); ++lMin);
  if(X.size() <= lMin) { return; }

  std::cout << "cc";
  for(const auto& x : X[lMin]) {
    std::cout << " & " << std::setw(8) << h2d_kind_name(x.first);
  }
  std::cout << std::endl;

  std::cout.precision(3);
  std::cout << std::fixed;
  for(uint i = 0; i < X.size(); ++i) {
    if(0 < X[i].size()) {
      std::cout << std::setw(2) << i;
      for(const auto& x : X[i]) {
        std::cout << " & " << std::setw(8) << x.second.avg();
      }
      std::cout << " \\\\" << std::endl;
    }
  }
}

void
test0(const std::string& aFilename) {
  est2rt_vmt lEst2RtC(32); // construction time
  est2rt_vmt lEst2RtQ(32); // estimation   time
  est2sry_vt lEst2QEr(32);

  std::ifstream lIs(aFilename);
  if(!lIs) {
    std::cout << "Can't open file '" << aFilename << "'." << std::endl;
    return;
  }

  uint lCount = 0;
  H2D::summaryline_t lSummaryline;
  while(!lIs.eof()) {
    if(lSummaryline.read(lIs)) {
      const uint lIndex = lSummaryline.card_class();
      const H2D::H2D_kind_t lKind = lSummaryline.kind();
      lEst2RtC[lIndex][lKind].step(lSummaryline._constructionTime);
      lEst2RtQ[lIndex][lKind].step(lSummaryline._checkTimePerQuery);
      if(lEst2QEr[lIndex].end() == lEst2QEr[lIndex].find(lKind)) {
        for(uint i = 0; i < 5; ++i) {
          lEst2QEr[lIndex][lKind][i] = 0;
        }
      }
      for(uint i = 0; i < 5; ++i) {
        lEst2QEr[lIndex][lKind][i] = std::max<double>(lEst2QEr[lIndex][lKind][i], lSummaryline._max[i]);
      }
      ++lCount;
    }
  }
  std::cout << "No good: " << lCount << std::endl;
  print(lEst2RtC, "construction time [s]");
  print(lEst2RtQ, "estimation time [us]");
  print(lEst2QEr);
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


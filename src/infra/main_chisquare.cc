#include "chisquare.hh"
#include <fstream>


struct arg_t {
  std::string _filename;
  uint        _n;
  uint        _m;
  uint        _k; // number of attributes

  arg_t() : _filename(), _n(0), _m(0), _k(0) {}
};

bool
parse_args(arg_t& aArgs, const int argc, const char* argv[]) {
  int lCurr = 1;
  int x = 0;
  char* lCharPtr = 0;
  while(lCurr < argc) {
    if(0 == strcmp("-n", argv[lCurr])) {
      ++lCurr;
      x = strtol(argv[lCurr], &lCharPtr, 10);
      if((lCharPtr == argv[lCurr]) || (0 >= x)) {
        return false;
      }
      ++lCurr;
      aArgs._n = (uint) x;
    } else
    if(0 == strcmp("-m", argv[lCurr])) {
      ++lCurr;
      x = strtol(argv[lCurr], &lCharPtr, 10);
      if((lCharPtr == argv[lCurr]) || (0 >= x)) {
        return false;
      }
      ++lCurr;
      aArgs._m = (uint) x;
    } else
    if(0 == strcmp("-k", argv[lCurr])) {
      ++lCurr;
      x = strtol(argv[lCurr], &lCharPtr, 10);
      if((lCharPtr == argv[lCurr]) || (0 >= x)) {
        return false;
      }
      ++lCurr;
      aArgs._k = (uint) x;
    } else {
      aArgs._filename = std::string(argv[lCurr]);
      ++lCurr;
    }
  }
  return true;
}

void
print_usage(const char* aProgName) {
   std::cout << "usage: " << aProgName << " -n <n> -m <m> -k <k>   <filename>" << std::endl
             << "  -n: number of splits in x-direction" << std::endl
             << "  -m: number of splits in y-direction" << std::endl
             << "  -k: number of attributes in input file" << std::endl
             << std::endl;
}

typedef std::vector<double_vt> double_vvt;

void
read(double_vvt& aData, const uint k, const std::string aFilename) {
  double_vt t(k);
  std::ifstream lIs(aFilename);
  if(!lIs) {
    std::cout << "can't open '" << aFilename << '\'' << std::endl;
  }
  while(lIs) {
    for(uint i = 0; i < k; ++i) {
      lIs >> t[i];
    }
    if(lIs.eof()) {
      break;
    }
    aData.push_back(t);
  }
}

double
get_min(const double_vvt& aData, const uint aAttrNo) {
  double lRes = std::numeric_limits<double>::max();
  for(size_t i = 0; i < aData.size(); ++i) {
    lRes = std::min<double>(lRes, aData[i][aAttrNo]);
  }
  return lRes;
}

double
get_max(const double_vvt& aData, const uint aAttrNo) {
  double lRes = -std::numeric_limits<double>::max();
  for(size_t i = 0; i < aData.size(); ++i) {
    lRes = std::max<double>(lRes, aData[i][aAttrNo]);
  }
  return lRes;
}

void
run0(ChiSquare& aCh, const uint aA1, const uint aA2, const double_vvt aData) {
  std::cout << "ChiSquare for attributes " << aA1 << " and " << aA2 << std::endl;

  aCh.init(get_min(aData, aA1), get_max(aData, aA1),
           get_min(aData, aA2), get_max(aData, aA2));

  for(size_t i = 0; i < aData.size(); ++i) {
    aCh.step(aData[i][aA1], aData[i][aA2]);
  }

  aCh.fin();

  std::cout << "chi-square = " << aCh.get_chi_square() << std::endl;
  std::cout << "max-qerror = " << aCh.get_max_qerror() << std::endl;

  std::cout << "Matrix = " << std::endl;
  // aCh.matrix().print(std::cout, 12) << std::endl;
  const Matrix& M = aCh.matrix();
  for(uint i = 0; i < M.noRows(); ++i) {
    for(uint j = 0; j < M.noCols(); ++j) {
      std::cout << ' ' << std::setw(8) << ((int) M(i,j));
    }
    std::cout << std::endl;
  }

  std::cout << "x-margin = " << aCh.margin_x() << std::endl;
  std::cout << "y-margin = " << aCh.margin_y() << std::endl;
}

void
test0(const uint n, const uint m, const uint k, const std::string& aFilename) {
  double_vvt lData;
  read(lData, k, aFilename);

  if(0 == lData.size()) {
    std::cout << "no data read." << std::endl;
    return;
  }

  std::cout << "|data| = " << lData.size() << std::endl;
  std::cout << "     n = " << n << std::endl
            << "     m = " << m << std::endl
            << "     k = " << k << std::endl
            << std::endl;

  ChiSquare lCh(n, m);
  for(uint i = 0; i < k; ++i) {
    for(uint j = i + 1; j < k; ++j) {
      run0(lCh, i, j, lData);
    }
  }
}

int
main(const int argc, const char* argv[]) {
  arg_t lArgs;
  if(!parse_args(lArgs, argc, argv)) {
    print_usage(argv[0]);
    return -1;
  }

  test0(lArgs._n, lArgs._m, lArgs._k, lArgs._filename);

  return 0;
}





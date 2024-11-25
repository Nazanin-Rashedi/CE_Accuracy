//
//  main.cpp
//  Test001
//
//  Created by Guido Moerkotte on 9/28/12.
//  Copyright (c) 2012 Guido Moerkotte. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <assert.h>

// #include "/Users/moer/src/infra/tmath.hh"
#include "infra/tmath.hh"
#include "infra/q.hh"

typedef unsigned int uint;

inline
uint
fBlockLow(const uint aIndex, const uint aBlockSize, const uint aLevel) {
  uint n = 1;
  // n := aBlockSize^aLevel
  for(uint i = 0; i < aLevel; ++i) {
    n *= aBlockSize;
  }
  return ((aIndex - 1) / n);
}

inline
uint
fBlockHigh(const uint aIndex, const uint aBlockSize, const uint aLevel) {
  uint n = 1;
  // n := aBlockSize^aLevel
  for(uint i = 0; i < aLevel; ++i) {
    n *= aBlockSize;
  }
  return ((aIndex + (n - 1)) / n);
}

void
fFillRandom(uint* x, const uint n) {
  for(uint i = 0; i < n; ++i) {
    x[i] = rand() % 77;
  }
}

void
fPrint(const uint* x, const uint n) {
  uint lSum = 0;
  for(uint i = 0; i < n; ++i) {
    lSum += x[i];
    std::cout << std::setw(3) << i << ' ' 
              << std::setw(3) << x[i] << ' '
              << std::setw(7) << lSum << ' '
              << std::endl;
  }
}

uint
fAccumulate(const uint* x, const uint lb, const uint ub) {
  uint lRes = 0;
  for(uint i = lb; i < ub; ++i) {
    lRes += x[i];
  }
  return lRes;
}

void
test000() {
  uint n = 3; // the size of a package
  uint N = 20; // size of input array to be packaged
  
  uint lb = 5;
  uint ub = 14;
  
  for(uint i = 0; i < N; ++i) {
    const uint lLb1l = fBlockLow(lb, n, 1);
    const uint lLb1h = fBlockHigh(lb, n, 1);
    const uint lUb1l = fBlockLow(ub, n, 1);
    const uint lUb1h = fBlockHigh(ub, n, 1);
    const uint lLb2l = fBlockLow(lb, n, 2);
    const uint lLb2h = fBlockHigh(lb, n, 2);
    const uint lUb2l = fBlockLow(ub, n, 2);
    const uint lUb2h = fBlockHigh(ub, n, 2);
    std::cout << std::setw(3) << i << ' '
              << std::setw(3) << (i / n) << ' '
              << std::setw(3) << (i / (n*n)) << ' '
              << std::setw(3) << ((i == (lLb1l * n)) ? '[' : ((i == (lLb1h * n)) ? ']' : ' '))
              << std::setw(3) << ((i == (lUb1l * n)) ? '[' : ((i == (lUb1h * n)) ? ']' : ' '))
              << std::setw(3) << ((i == (lLb2l * n * n)) ? '[' : ((i == (lLb2h * n * n)) ? ']' : ' '))
              << std::setw(3) << ((i == (lUb2l * n * n)) ? '[' : ((i == (lUb2h * n * n)) ? ']' : ' '))
              << std::endl;
  }
}


/*
 * class MTree
 * for some input array x with aArraySize (n) elements,
 * calculate the sum of aBlockSize consecutive entries and
 * store this sum in the array _arr
 * for the thus calculated sums, calculate for aBlockSize of them
 * their sum and so on until everything is accumulated
 */

class MTree {
  public:
    MTree();
    ~MTree();
  public:
    void init(const uint* x, const uint aArraySize, const uint aBlockSize);
    uint accumulate(const uint* x, // the original input array
                    const uint aLb, // a lower index bound, inclusive
                    const uint aUb  // a upper index bound, inclusive
                   ) const;
  public:
    inline uint blocksize() const { return _blocksize; }
    inline  int noLevels() const { return _noLevels; }
    inline uint arrSize() const { return _arrSize; }
    inline uint startIndex(const uint i) const { return _startIndex[i]; }
    inline uint arr(const uint i) const { return _arr[i]; }
  private:
    uint* _arr;
    uint* _startIndex;
    uint  _n;
    uint  _blocksize;
     int  _noLevels;
    uint  _arrSize;
};

MTree::MTree() : _arr(0), _startIndex(0),
                 _n(0), _blocksize(0), _noLevels(0), _arrSize(0) {
}
MTree::~MTree() {
  if(0 != _arr) { delete _arr; }
  if(0 != _startIndex) { delete _startIndex; }
}

void
MTree::init(const uint* x, const uint aArraySize, const uint aBlockSize) {
  _n = aArraySize;
  _blocksize = aBlockSize;
  
  // calculate the number of levels needed
  _noLevels = 0;
  {
    uint x = 1;
    while(x < _n) {
      x *= blocksize();
      ++_noLevels;
    }
  }
  
  // calculate the array size and the start indexes of the block summaries at each level
  _startIndex = new uint[noLevels() + 1];
  _arrSize = 0;
  uint m = _n; // size of array at level i, starting with 0 = data
  for(int i = 0; i < noLevels(); ++i) {
    _startIndex[i] = _arrSize;
    const uint lNewM = (m + (blocksize() - 1)) / blocksize();
    m = lNewM;
    _arrSize += lNewM;
  }
  _startIndex[noLevels()] = _arrSize;
  
  // allocate array for block summaries
  _arr = new uint[arrSize()];
  
  // print start indices
  std::cout << "start indices: ";
  for(int i = 0; i <= noLevels(); ++i) {
    std::cout << ' ' << _startIndex[i];
  }
  std::cout << std::endl;
  
  // accumulate levelwise
  const uint* lToAccumulate = x;
  uint lIdxIn = 0;
  uint lIdxOut = 0;
  uint lCurrLimit = _n;
  for(int lLevel = 0; lLevel < noLevels(); ++lLevel) {
    const uint lNoBlocks = (startIndex(lLevel + 1) - startIndex(lLevel));
    for(uint lBlockNo = 0; lBlockNo < lNoBlocks; ++lBlockNo) {
      uint lSum = 0;
      for(uint j = 0; j < blocksize() && lIdxIn < lCurrLimit; ++j) {
        lSum += lToAccumulate[lIdxIn++];
      }
      _arr[lIdxOut++] = lSum;
    }
    if(0 == lLevel) {
      lToAccumulate = _arr;
      lIdxIn = 0;
    }
    lCurrLimit = startIndex(lLevel + 1);
  }
  
  // print _arr
  uint bb = blocksize();
  uint lLb = 0;
  std::cout << std::setw(3) << "i" << ' '
            << std::setw(8) << "arr[i]" << ' '
            << std::setw(8) << "accum" << ' '
            << std::setw(3) << "lb" << ' '
            << std::endl;
  for(int lLevel = 0; lLevel < noLevels(); ++lLevel) {
    for(uint i = startIndex(lLevel); i < startIndex(lLevel + 1); ++i) {
      const uint lAcc = fAccumulate(x, lLb, std::min<uint>(_n, lLb + bb));
      std::cout << std::setw(3) << i << ' '
      << std::setw(8) << arr(i) << ' '
      << std::setw(8) << lAcc << ' '
      << std::setw(3) << lLb << ' '
      << std::setw(3) << ((lAcc == arr(i)) ? '.' : 'X')
      << std::endl;
      lLb += bb;
    }
    bb *= blocksize();
    lLb = 0;
  }
  
}

uint 
MTree::accumulate(const uint* x, // the original input array
                  const uint aLb, // a lower index bound, inclusive
                  const uint aUb  // a upper index bound, inclusive
                 ) const {
  const bool lTrace = true;

  if(aLb > aUb) { return 0; }
  uint lRes = 0;
  const uint lUb = std::min<uint>(aUb, _n);

  // shortcut for small intervals
  if((lUb - aLb) < blocksize()) {
    for(uint lIdx = aLb; lIdx <= lUb; ++lIdx) {
      lRes += x[lIdx];
    }
    return lRes;
  }

  // process level -1 (original array)
  // accumulate until start of some block in level 0 (_arr) is reached
  uint lIdx = aLb; // lIdx will always contain an index into original array x
  uint lBlockSizeOneUp = blocksize();
  uint lBeginOneUp = ((0 == lIdx) ? 0 : ((lIdx - 1) / lBlockSizeOneUp) + 1) * lBlockSizeOneUp;
  if(lTrace) {
    std::cout << "  A  : [" << aLb << ',' << lUb << "]  "
              << "lIdx: " << lIdx << ";  "
              << "lBlockSizeOneUp: " << lBlockSizeOneUp << "; "
              << "lBeginOneUp: " << lBeginOneUp << "; "
              << std::endl;
  }
          
  while(lIdx < lBeginOneUp) {
    lRes += x[lIdx];
    ++lIdx;
    if(lIdx > lUb) {
      return lRes;
    }
  }


  // go up until this is no longer possible
  int lLevel  = 0; // current level
  uint lBlockSize = 0; // block size of level lLevel
  uint lIdxArr = 0; // index into _arr for current level
  do {
    lBlockSize = lBlockSizeOneUp;
    lBlockSizeOneUp = lBlockSize * blocksize();
    lBeginOneUp = ((0 == lIdx) ? 0 : ((lIdx - 1) / lBlockSizeOneUp) + 1) * lBlockSizeOneUp;
    lIdxArr = startIndex(lLevel) + (lIdx / lBlockSize);
    if(lTrace) {
      std::cout << "  B  "  << 'L' << lLevel << ' '
                << "lIdx: " << lIdx << ";  "
                << "lBlockSizeOneUp: " << lBlockSizeOneUp << "; "
                << "lBeginOneUp: " << lBeginOneUp << "; "
                << "lIdxArr: " << lIdxArr << "; "
                << std::endl;
    }
    // add up _arr elements at current level until
    // lBeginOnUp is reached
    while(    (lUb >= (lIdx + lBlockSize - 1)) // fail: stop, process current level and go down 
           && (lIdx < lBeginOneUp)) {
      lRes += arr(lIdxArr++);
      lIdx += lBlockSize;
      if(lTrace) {
        std::cout << "      C  lIdx: " << lIdx << ";  "
                  << "lIdxArr: " << lIdxArr << "; "
                  << "lRes: " << lRes << "; "
                  << std::endl;
      }
    }
    assert(lIdx <= (lUb + 1));
    // if((lIdx + 1) == lUb) {
    if(lIdx >  lUb) {
      if(lTrace) {
        std::cout << "    exit D" << std::endl;
      }
      return lRes;
    } 
    if(lUb <= (lIdx + lBlockSize - 1)) {
      if(lTrace) {
        std::cout << "    break E" << std::endl;
      }
      break;
    }
    ++lLevel;
  } while (lLevel < noLevels());

  //  --lLevel; // must decrement one, since lLevel failed while condition

  if(lTrace) {
    std::cout << "  F lIdx: " << lIdx << ";  "
              << "lBlockSizeOneUp: " << lBlockSizeOneUp << "; "
              << "lBeginOneUp: " << lBeginOneUp << "; "
              << "lIdxArr: " << lIdxArr << "; "
              << std::endl;
  }

  // process current level and go down
  while(lIdx <= lUb) {
    // process current level
    if(lTrace) {
      std::cout << "  G lIdx: " << lIdx << ' '
                << "lUb: " << lUb << ' '
                << std::endl;
    }
    while(lUb >= (lIdx + lBlockSize)) {
      if(lTrace) {
        std::cout << "lIdx: " << lIdx << ";  "
                  << "lBlockSizeOneUp: " << lBlockSizeOneUp << "; "
                  << "lBeginOneUp: " << lBeginOneUp << "; "
                  << "lIdxArr: " << lIdxArr << "; "
                  << std::endl;
      }
      lRes += arr(lIdxArr++);
      lIdx += lBlockSize;
    }
    // go one level down
    --lLevel;
    if(0 > lLevel) { break; }
    lBlockSize /= blocksize();
    lIdxArr = startIndex(lLevel) + (lIdx / lBlockSize);
  }

  // process level -1, original data array
  while(lIdx <= lUb) {
    lRes += x[lIdx++];
  }

  return lRes;
}


void
test001() {
  const uint n = 103;
  uint* x = new uint[n];
  fFillRandom(x, n);
  fPrint(x,n);
  
  const uint lTotal = fAccumulate(x, 0, n);
  std::cout << "total: " << lTotal << std::endl;
  const uint lSum1 = fAccumulate(x, 7, 77);
  std::cout << "sum1: " << lSum1 << std::endl;
  
  MTree lMTree;
  lMTree.init(x, n, 5);
  std::cout << "#levels: " << lMTree.noLevels() << std::endl;
  std::cout << "arrsize: " << lMTree.arrSize() << std::endl;


  for(uint i = 0; i < n; ++i) {
    for(uint j = i + 1; j < n; ++j) {
      const uint lAccMtree = lMTree.accumulate(x, i, j);
      const uint lAccDirect = fAccumulate(x, i, j+1); // +1 wg. ub exclusive
      std::cout << std::setw(3) << i << ' '
                << std::setw(3) << j << ' '
                << std::setw(8) << lAccMtree << ' '
                << std::setw(8) << lAccDirect << ' '
                << std::endl;
      assert(lAccMtree == lAccDirect);
    }
  }
}

int main(int argc, const char * argv[])
{
  std::cout << "Hello, My World!\n";
  test000();
  test001();
  return 0;
}


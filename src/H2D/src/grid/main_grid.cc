#include <iostream>
#include <iomanip>
#include <assert.h>

#include "infra/matrix.hh"
#include "grid_tt.hh"

template<class Tgrid>
bool
areEqual(const Tgrid& aGrid, const Matrix& aMatrix) {
  for(uint32_t i = 0; i < aMatrix.noRows(); ++i) {
    for(uint32_t j = 0; j < aMatrix.noCols(); ++j) {
      if(aGrid.get(i,j) != aMatrix(i,j)) {
        return false;
      }
    }
  }
  return true;
}

template<class Tgrid>
void
testGrid(Tgrid& aGrid) {
  const uint32_t p = aGrid.GridSize;

  std::cout << "accessor info: " << std::endl;

  std::cout << "  i" << ' '
            << "  j" << ' '
            << "idx" << ' '
            << "beg" << ' '
            << " lo" << ' '
            << "shift" << ' '
            << std::endl;
  for(uint32_t i = 0; i < p; ++i) {
    for(uint32_t j = 0; j < p; ++j) {
      std::cout << std::setw(3) << i << ' '
                << std::setw(3) << j << ' '
                << std::setw(3) << aGrid.tileIdx(i, j) << ' '
                << std::setw(3) << aGrid.NoTileBits * aGrid.tileIdx(i, j) << ' '
                << std::setw(3) << aGrid.byteIdx(aGrid.tileIdx(i, j)) << ' '
                << std::setw(3) << aGrid.shift(aGrid.tileIdx(i, j)) << ' '
                << std::endl;
    }
  }


  Matrix lMatrix(p,p);

  // const uint32_t lMaxValue = aGrid.Mask;
  const uint32_t lX = aGrid.Mask;

  std::cout << "input is a " << p << 'x' << p << "-grid " 
            << "with " << aGrid.NoTileBits << " bits per entry: "
            << "consumes " << aGrid.NoBytes << " bytes."
            << std::endl;

  uint8_t* lBuffer = new uint8_t[aGrid.NoBytes + 1];

  aGrid.attach(lBuffer);

  const uint32_t lEnd = aGrid.GridSize;
  for(uint32_t i = 0; i < lEnd; ++i) {
    for(uint32_t j = 0; j < lEnd; ++j) {
      uint32_t lVal = 10 * i + j;
      if(lVal > lX) {
        lVal = lX;
      }
      aGrid.set(i, j, lVal);
      lMatrix(i, j) = lVal;
    }
  }

  std::cout << "Grid:" << std::endl;
  aGrid.print(std::cout);
  std::cout << "Matrix:" << std::endl;
  lMatrix.print(std::cout, 3);

  assert(areEqual(aGrid, lMatrix));

  bool lFlip = false;
  for(uint32_t i = 0; i < lEnd; ++i) {
    for(uint32_t j = 0; j < lEnd; ++j) {
      if(lFlip) {
        aGrid.set(i, j, 0);
        lMatrix(i, j) = 0;
      }
      lFlip = !lFlip;
    }
  }

  assert(areEqual(aGrid, lMatrix));

  lFlip = true;
  for(uint32_t i = 0; i < lEnd; ++i) {
    for(uint32_t j = 0; j < lEnd; ++j) {
      if(lFlip) {
        aGrid.set(i, j, lX);
        lMatrix(i, j) = lX;
      }
      lFlip = !lFlip;
    }
  }
  std::cout << "filled grid lX: " << std::endl;
  aGrid.print(std::cout);

  assert(areEqual(aGrid, lMatrix));

  lFlip = false;
  for(uint32_t i = 0; i < lEnd; ++i) {
    for(uint32_t j = 0; j < lEnd; ++j) {
      aGrid.set(i, j, lFlip * lX);
      lMatrix(i, j) = lFlip * lX;
      lFlip = !lFlip;
    }
  }
  std::cout << "filled grid lX: " << std::endl;
  aGrid.print(std::cout);

  assert(areEqual(aGrid, lMatrix));

  lFlip = true;
  for(uint32_t i = 0; i < lEnd; ++i) {
    for(uint32_t j = 0; j < lEnd; ++j) {
      if(lFlip) {
        aGrid.set(i, j, lX);
        lMatrix(i, j) = lX;
      }
      lFlip = !lFlip;
    }
  }
  std::cout << "filled grid lX: " << std::endl;
  aGrid.print(std::cout);

  assert(areEqual(aGrid, lMatrix));

  lFlip = false;
  for(uint32_t i = 0; i < lEnd; ++i) {
    for(uint32_t j = 0; j < lEnd; ++j) {
      if(lFlip) {
        aGrid.set(i, j, 0);
        lMatrix(i, j) = 0;
      }
      lFlip = !lFlip;
    }
  }
  std::cout << "filled grid lX: " << std::endl;
  aGrid.print(std::cout);

  assert(areEqual(aGrid, lMatrix));

  lFlip = false;
  for(uint32_t i = 0; i < lEnd; ++i) {
    for(uint32_t j = 0; j < lEnd; ++j) {
      if(lFlip) {
        aGrid.set(i, j, lX);
        lMatrix(i, j) = lX;
      }
      lFlip = !lFlip;
    }
  }
  std::cout << "filled grid lX: " << std::endl;
  aGrid.print(std::cout);

  assert(areEqual(aGrid, lMatrix));

  aGrid.detach();

  delete[] lBuffer;
}



void
test1() {
  using namespace H2D;

  std::cout << "==========================================================="
            << std::endl << std::endl;

  Grid_TT<5,7> lGrid557;
  testGrid(lGrid557);

  std::cout << "==========================================================="
            << std::endl << std::endl;

  Grid_TT<5,6> lGrid556;
  testGrid(lGrid556);

  std::cout << "==========================================================="
            << std::endl << std::endl;

  Grid_TT<4,6> lGrid446;
  testGrid(lGrid446);


  std::cout << "==========================================================="
            << std::endl << std::endl;

  Grid_TT<4,5> lGrid445;
  testGrid(lGrid445);

  std::cout << "==========================================================="
            << std::endl << std::endl;

  Grid_TT<4,7> lGrid447;
  testGrid(lGrid447);

  std::cout << "==========================================================="
            << std::endl << std::endl;

  Grid_TT<4,3> lGrid443;
  testGrid(lGrid443);

  std::cout << "==========================================================="
            << std::endl << std::endl;

  Grid_TT<3,3> lGrid333;
  testGrid(lGrid333);

  std::cout << "==========================================================="
            << std::endl << std::endl;

  Grid_TT<3,2> lGrid332;
  testGrid(lGrid332);

  std::cout << "==========================================================="
            << std::endl << std::endl;

  Grid_TT<2,5> lGrid225;
  testGrid(lGrid225);

  std::cout << "==========================================================="
            << std::endl << std::endl;

  Grid_TT<2,4> lGrid224;
  testGrid(lGrid224);

  std::cout << "==========================================================="
            << std::endl << std::endl;

  Grid_TT<2,3> lGrid223;
  testGrid(lGrid224);

  std::cout << "==========================================================="
            << std::endl << std::endl;


  Grid_TT<2,2> lGrid222;
  testGrid(lGrid222);

  std::cout << "==========================================================="
            << std::endl << std::endl;


}



int
main() {
  test1();
}


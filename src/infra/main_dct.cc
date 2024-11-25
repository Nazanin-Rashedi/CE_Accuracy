#include <iostream>
#include <iomanip>
#include "matrix.hh"
#include "q.hh"

double
determineQError(const Matrix& M1, const Matrix& M2) {
  if(M1.noRows() != M2.noRows() || M1.noCols() != M2.noCols()) {
    return 1.0/0.0;
  }

  double lRes = 0;
  for(uint i = 0; i < M1.noRows(); ++i) {
    for(uint j = 0; j < M1.noCols(); ++j) {
      lRes = std::max<double>(lRes, q::qerror(std::max<double>(1.0, M1(i,j)), 
                                              std::max<double>(1.0, M2(i,j))));
    }
  }
  return lRes;
}

// returns q-error I (integral based summation)
// and in parameter: aQErrorS: qerror of summation-based estimation
double
fEstimatesOfMatrix(const Matrix& Morig, const Matrix& Mdct, double& aQErrorS) {
  bool lTrace = false;
  if(Morig.noRows() != Mdct.noRows() || Morig.noCols() != Mdct.noCols()) {
    return 1.0/0.0;
  }
  double lMaxQErrorI = 0;
  double lMaxQErrorS = 0;
  std::cout << "estimates produced: " << std::endl;
  for(uint lXlo = 0; lXlo < Morig.noRows(); ++lXlo) {
    for(uint lXhi = lXlo + 1; lXhi <= Morig.noRows(); ++lXhi) {
      for(uint lYlo = 0; lYlo < Morig.noCols(); ++lYlo) {
        for(uint lYhi = lYlo + 1; lYhi <= Morig.noCols(); ++lYhi) {
          const double lTrueVal = Morig.sumAllElements(lXlo, lXhi, lYlo, lYhi);
          const double lEstValS = Mdct.dctEstimateSlow(lXlo, lXhi, lYlo, lYhi);
          const double lEstValI = Mdct.dctEstimate(lXlo, lXhi, lYlo, lYhi);
          if(lTrace) {
            std::cout << "([" << lXlo << "," << lXhi << "],[" << lYlo << "," << lYhi << "]) : "
                      << std::setw(12) << lTrueVal << " =?= "
                      << std::setw(12) << lEstValS << " =?= "
                      << std::setw(12) << lEstValI
                      << std::endl;
          }
          const double lQErrorS = q::qerror(std::max<double>(1.0, lEstValS),
                                            std::max<double>(1.0, lTrueVal));
          if(lQErrorS > lMaxQErrorS) {
            lMaxQErrorS = lQErrorS;
          }
          const double lQErrorI = q::qerror(std::max<double>(1.0, lEstValI),
                                            std::max<double>(1.0, lTrueVal));
          if(lQErrorI > lMaxQErrorI) {
            lMaxQErrorI = lQErrorI;
          }
        }
      }
    }
  }
  aQErrorS = lMaxQErrorS;
  return lMaxQErrorI;
}

void
fGenerateQueryRectangles(const Matrix& M) {
  const double m = (double) M.noRows();
  const double n = (double) M.noCols();

  std::cout << "queries: " << std::endl;

  for(double lRowLo = 0; lRowLo < m; lRowLo += 0.5) {
    for(double lRowHi = lRowHi + 0.5; lRowHi < m; lRowHi += 0.5) {
      for(double lColLo = 0; lColLo < n; lColLo += 0.5) {
        for(double lColHi = lColHi + 0.5; lColHi < n; lColHi += 0.5) {
          const double lEstimate = M.dctEstimateContinuous(lRowLo, lRowHi, lColLo, lColHi);
          std::cout << '['      << lRowHi << ',' << lRowLo 
		     << "] x [" << lColLo << ',' << lColHi << "] : "
                    << lEstimate << std::endl;
        }
      }
    }
  }
  std::cout << std::endl;
}

void
testDct(const Matrix& M1) {
  Matrix M2;

  std::cout << "original matrix" << std::endl;
  M1.print(std::cout, 3);

  M2.setToDiscreteCosineTransform(M1);
  std::cout << "transformed matrix" << std::endl;
  M2.printRoundXX(std::cout, 6);

  // fGenerateQueryRectangles(M2);

  Matrix M2x;
  M2x.setToDiscreteCosineTransformViaC(M1);
  std::cout << "transformed via C matrix" << std::endl;
  M2x.printRoundXX(std::cout, 6);
  std::cout << std::endl;



  Matrix M3;
  M3.setToInverseDiscreteCosineTransform(M2);
  std::cout << "inverse transformed matrix" << std::endl;
  M3.printRoundXX(std::cout, 6);

  Matrix M3x;
  M3x.setToInverseDiscreteCosineTransformViaC(M2x);
  std::cout << "inverse transformed matrix via C" << std::endl;
  M3x.printRoundXX(std::cout, 6);
  std::cout << std::endl;


  Matrix M4(M2);
  // M4.setZeroSmallerThan(0.5); // threshold for  truncation for DCT matrix
  M4.setZeroSmallerThan(1.0); // a little too high
  std::cout << "transformed matrix, truncated" << std::endl;
  M4.printRoundXX(std::cout, 6);

  Matrix M5;
  M5.setToInverseDiscreteCosineTransform(M4);
  M5.setZeroSmallerThan(0.001);
  std::cout << "inverse transformed matrix from truncated transformed matrix" << std::endl;
  M5.printRoundXX(std::cout, 6);

  const double lQError = determineQError(M1, M5);
  std::cout << "q-error within inverse of truncated transformed matrix: " << lQError << std::endl;

  double lMaxQErrorS = 0;
  std::cout << "apply estimates on full transformed matrix" << std::endl;
  const double lMaxQError1 = fEstimatesOfMatrix(M1, M2, lMaxQErrorS);
  std::cout << "maximum q-error integral: " << lMaxQError1 << std::endl;
  std::cout << "maximum q-error  summing: " << lMaxQErrorS << std::endl;

  std::cout << "apply estimates on truncated transformed matrix" << std::endl;
  const double lMaxQError2 = fEstimatesOfMatrix(M1, M4, lMaxQErrorS);
  std::cout << "maximum q-error integral: " << lMaxQError2 << std::endl;
  std::cout << "maximum q-error  summing: " << lMaxQErrorS << std::endl;
  std::cout << std::endl;

}


void
test0() {
  Matrix M1(3, 3,
            10.0, 15.0, 13.0,
            14.0, 20.0, 16.0,
             9.0, 13.0, 11.0);
  testDct(M1);
}

void
test1() {
  Matrix M1(9, 9,
            5.0, 4.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 5.0, 4.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 5.0, 4.0, 1.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 5.0, 4.0, 1.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 5.0, 4.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 5.0, 4.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 5.0, 4.0, 1.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 5.0, 4.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 5.0);
  testDct(M1);
}

void
test2() {
  Matrix M1(9, 9,
            5.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            3.0, 5.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 3.0, 5.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 3.0, 5.0, 3.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 3.0, 5.0, 3.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 3.0, 5.0, 3.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 3.0, 5.0, 3.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 3.0, 5.0, 3.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 3.0, 5.0);
  testDct(M1);
}


void
test3() {
  Matrix M1(9, 9,
            5.0,  3.0,  0.0,  0.0,  0.0,   0.0,  0.0,  3.0,  5.0,
            3.0,  5.0,  3.0,  0.0,  0.0,   0.0,  3.0,  5.0,  3.0,
            0.0,  3.0,  5.0,  3.0,  0.0,   3.0,  5.0,  3.0,  0.0,
            0.0,  0.0,  3.0,  5.0,  6.0,   5.0,  3.0,  0.0,  0.0,
            0.0,  0.0,  0.0,  6.0,  10.0,  6.0,  0.0,  0.0,  0.0,
            0.0,  0.0,  3.0,  5.0,  6.0,   5.0,  3.0,  0.0,  0.0,
            0.0,  3.0,  5.0,  3.0,  0.0,   3.0,  5.0,  3.0,  0.0,
            3.0,  5.0,  3.0,  0.0,  0.0,   0.0,  3.0,  5.0,  3.0,
            5.0,  3.0,  0.0,  0.0,  0.0,   0.0,  0.0,  3.0,  5.0);
  testDct(M1);
}

void
test4() {
  Matrix M(4, 4,
	    1.0,  2.0,  3.0,  4.0,
	    5.0,  6.0,  7.0,  8.0,
	    9.0, 10.0, 11.0, 12.0,
	   13.0, 14.0, 15.0, 16.0);
  Matrix E(4, 4,
	    1.0,  0.0,  0.0,  0.0,
	    0.0,  1.0,  0.0,  0.0,
	    0.0,  0.0,  1.0,  0.0,
	    0.0,  0.0,  0.0,  1.0);
  std::cout << "E = " << std::endl;
  E.print(std::cout, 2);
  std::cout << "M = " << std::endl;
  M.print(std::cout, 2);

  Matrix B;
  B.setToAxB(M,E);
  std::cout << "ME:" << std::endl;
  B.print(std::cout, 2);
  B.setToAtxB(M,E);
  std::cout << "M^tE:" << std::endl;
  B.print(std::cout, 2);
  B.setToAxBt(M, E);
  std::cout << "ME^t:" << std::endl;
  B.print(std::cout, 2);
  B.setToAtxBt(M, E);
  std::cout << "M^tE^t:" << std::endl;
  B.print(std::cout, 2);
  B.setToTransposedOfAtxBt(M, E);
  std::cout << "(M^tE^t)^t:" << std::endl;
  B.print(std::cout, 2);
  
  Matrix C;
  C.setToDiscreteCosineOrthonormalMatrix(4);
  std::cout << "C:" << std::endl;
  C.print(std::cout, 8);

  Matrix CtC;
  CtC.setToAtxB(C, C);
  std::cout << "C^tC = " << std::endl;
  CtC.printRoundXX(std::cout, 8);

  Matrix CCt;
  CCt.setToAxBt(C, C);
  std::cout << "CC^t = " << std::endl;
  CCt.printRoundXX(std::cout, 8);

}



int
main() {
  test0();
  test1();
  test2();
  test3();

  test4();

  return 0;
}


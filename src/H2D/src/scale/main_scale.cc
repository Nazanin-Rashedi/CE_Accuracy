#include <iostream>
#include <iomanip>

#include "scale.hh"

void
fPrintScale_Summary(const q::Scale_S& aScale) {
  std::cout << "Scale(" << aScale.noBits() << ", " 
                        << aScale.maxNo() << ", " 
                        << aScale.limit1() << ", "
                        << aScale.limit2() << ')' 
                        << std::endl
            << "  qerror fuer theta= 0: " << aScale.qerror( 0) << " / "
                                          << aScale.qerrorDouble( 0) << std::endl
            << "  qerror fuer theta=10: " << aScale.qerror(10) << " / "
                                          << aScale.qerrorDouble(10) << std::endl
            << "  qerror fuer theta=15: " << aScale.qerror(15) << " / "
                                          << aScale.qerrorDouble(15) << std::endl
            << "  qerror fuer theta=20: " << aScale.qerror(20) << " / "
                                          << aScale.qerrorDouble(20) << std::endl
            << "  qerror fuer theta=30: " << aScale.qerror(30) << " / "
                                          << aScale.qerrorDouble(30) << std::endl
            << "  qerror fuer theta=40: " << aScale.qerror(40) << " / "
                                          << aScale.qerrorDouble(40) << std::endl
            ;

  if(aScale.maxNo() < 50) {
    aScale.print(std::cout);
  }

}


void
testS() {
  q::Scale_S lScale;


  lScale.autofill_2_1();
  std::cout << "scale_2_1:" << std::endl;
  fPrintScale_Summary(lScale);

  lScale.autofill_2_2();
  std::cout << "scale_2_2:" << std::endl;
  fPrintScale_Summary(lScale);

  lScale.autofill_3_1();
  std::cout << "scale_3_1:" << std::endl;
  fPrintScale_Summary(lScale);

  lScale.autofill_3_2();
  std::cout << "scale_3_2:" << std::endl;
  fPrintScale_Summary(lScale);

  lScale.autofill_3_3();
  std::cout << "scale_3_3:" << std::endl;
  fPrintScale_Summary(lScale);

  lScale.autofill_3_4();
  std::cout << "scale_3_4:" << std::endl;
  fPrintScale_Summary(lScale);

  lScale.autofill_4_1();
  std::cout << "scale_4_1:" << std::endl;
  fPrintScale_Summary(lScale);

  lScale.autofill_4_2();
  std::cout << "scale_4_2:" << std::endl;
  fPrintScale_Summary(lScale);

  lScale.autofill_5_1();
  std::cout << "scale_5_1:" << std::endl;
  fPrintScale_Summary(lScale);

  lScale.autofill_5_2();
  std::cout << "scale_5_2:" << std::endl;
  fPrintScale_Summary(lScale);


}


void
testM(const uint32_t aNoBits, const double aBase) {
  q::ScaleM lScaleM(aNoBits, aBase);
  const uint32_t lMaxCode = lScaleM.maxCode();
  const uint32_t lLimit   = std::min<uint32_t>(lScaleM.limit(), 1000 * 1000);
  std::cout << "no bits: " << aNoBits 
            << ", base: " << lScaleM.base()
            << ", limit: " << lScaleM.limit() << std::endl;
  std::cout << "  decompress max c: " << lScaleM.decompress(lMaxCode) << std::endl;
  std::cout << "  qerror: theta:00: " << lScaleM.qerror( 0, lLimit) << std::endl;
  std::cout << "  qerror: theta:10: " << lScaleM.qerror(10, lLimit) << std::endl;
  std::cout << "  qerror: theta:20: " << lScaleM.qerror(20, lLimit) << std::endl;
  std::cout << "  qerror: theta:30: " << lScaleM.qerror(30, lLimit) << std::endl;
}


void
test6(const double aBase) {
  q::Scale_6 lScale_6(aBase);
  const uint32_t lMaxCode = lScale_6.maxCode();
  std::cout << "no bits: " << lScale_6.noBits() 
            << ", base: " << lScale_6.base() 
            << ", limit: " << lScale_6.limit() 
            << std::endl;
  std::cout << "  decompress max c: " << lScale_6.decompress(lMaxCode) << std::endl;
  std::cout << "  qerror: theta:00: " << lScale_6.qerror( 0, std::min<uint32_t>(1000*1000*10, lScale_6.limit())) << std::endl;
}

void
test2(const uint32_t aNoBits, const double aBase, const uint32_t aEnd, const uint32_t aNoMat) {
  q::Scale_L lScale_L(aNoBits, aBase, aNoMat);
  const uint32_t lEnd = (aEnd <= lScale_L.limit() ? aEnd : lScale_L.limit());
  const double lQError = lScale_L.qerror(1, std::min<uint32_t>(1000*1000*10, lEnd));
  std::cout << "no bits: " << lScale_L.noBits() << ", base: " << lScale_L.base() << std::endl;
  std::cout << "  decompress max u: " << lScale_L.limit() << std::endl;
  std::cout << "  decompress max d: " << lScale_L.limitDouble() << std::endl;
  std::cout << "  qerror: " << lQError << std::endl;
}



void
createTable() {
  std::cout << "\\begin{center}" << std::endl;
  std::cout << "\\begin{tabular}{rrrcrrrrrr}" << std::endl;
  std::cout << "       &        &      & scale/ & \\multicolumn{4}{c}{no bits for} \\\\" << std::endl;
}

void
test_S_3(const uint aVal) {
  const q::Scale_S* lScale[3] = { q::ScaleMgr::instance()->scale_S_3_1(),
                                  q::ScaleMgr::instance()->scale_S_3_2(),
                                  q::ScaleMgr::instance()->scale_S_3_3() };
  std::cout << "test_S_3" << std::endl
            << "i   val  est   code " << std::endl;

  for(uint i = 0; i < 3; ++i) {
    const uint lCode = lScale[i]->compress(aVal);
    const uint lEst  = lScale[i]->decompress(lCode);
    std::cout << (i + 1) << "  "
              << std::setw(4) << aVal << ' '
              << std::setw(4) << lEst << "   "
              << std::setw(4) << lCode << ' '
              << std::endl;
  }
}

void
test_S_4(const uint aVal) {
  const q::Scale_S* lScale[2] = { q::ScaleMgr::instance()->scale_S_4_1(),
                                  q::ScaleMgr::instance()->scale_S_4_2() };
  std::cout << "test_S_4" << std::endl
            << "i   val  est   code " << std::endl;

  for(uint i = 0; i < 2; ++i) {
    const uint lCode = lScale[i]->compress(aVal);
    const uint lEst  = lScale[i]->decompress(lCode);
    std::cout << (i + 1) << "  "
              << std::setw(4) << aVal << ' '
              << std::setw(4) << lEst << "   "
              << std::setw(4) << lCode << ' '
              << std::endl;
  }
}


void
test77(const uint aCode) {
  const q::Scale_L* lScale = q::ScaleMgr::instance()->scale_L_8_2();
  std::cout << std::setw(4) << aCode << ' '
            << std::setw(9) << lScale->decompress(aCode)
            << std::endl;
}


int
main() {
  std::cout << "*** TEST S ***" << std::endl;
  testS();
  std::cout << std::endl;

  std::cout << "*** TEST 6 *** "<< std::endl;

  test6(1.05);
  test6(1.10);
  test6(1.15);
  test6(1.20);
  test6(1.25);
  test6(1.30);
  test6(1.35);
  test6(1.40);
  std::cout << std::endl;

  std::cout << "*** TEST M ***" << std::endl;
  testM(5, 1.05);
  testM(5, 1.10);
  testM(5, 1.15);
  testM(5, 1.20);
  testM(6, 1.05);
  testM(6, 1.10);
  testM(6, 1.15);
  testM(6, 1.20);
  testM(7, 1.05);
  testM(7, 1.10);
  testM(7, 1.15);
  testM(7, 1.20);
  std::cout << std::endl;


  const uint32_t N = 1000 * 1000;

  std::cout << "*** TEST L ***" << std::endl;
  test2(7, 1.200, N, 100);
  test2(7, 1.100, N, 100);
  test2(7, 1.090, N, 100);
  test2(7, 1.085, N, 100);
  test2(7, 1.080, N, 100);
  test2(7, 1.070, N, 100);
  test2(7, 1.060, N, 100);

  test2(8, 1.100, N, 100);
  test2(8, 1.090, N, 100);
  test2(8, 1.085, N, 100);
  test2(8, 1.080, N, 100);
  test2(8, 1.070, N, 100);
  test2(8, 1.060, N, 100);

  test_S_3(100);
  test_S_4( 40);

  test77(54);
  test77(57);

  return 0;
}


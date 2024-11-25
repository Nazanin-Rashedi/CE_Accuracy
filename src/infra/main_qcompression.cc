#include <iostream>
#include <iomanip>
#include <limits>
#include <inttypes.h>

#include "infra/measure.hh"
#include "infra/tmath.hh"
#include "infra/bit_intrinsics.hh"
#include "infra/q.hh"


void
runTestCompress8() {
  
  std::cout << std::setw(2) << "i" << ' '
            << std::setw(12) << "compressed" << ' '
            << std::setw(12) << "decompressed" << ' '
            << std::setw(5) << "bits" << ' '
            << std::setw(5) << "shift" << ' '
            << std::endl;
  
  for(uint i = 0; i < 100; ++i) {
    const uint lToCompress = i;
    std::cout << std::setw(2) << lToCompress << ' ';
    const uint lCode = q::compress8(lToCompress);
    const uint lDecompressed = q::decompress8<uint>(lCode);
    const uint lBits  = (lCode >> 5) & 0x7;
    const uint lShift = (lCode & 0x1F);
    std::cout << std::setw(12) << lCode << ' '
              << std::setw(12) << lDecompressed << ' '
              << std::setw(5) << lBits << ' '
              << std::setw(5) << lShift << ' '
              << std::endl;
       
  }
}


void
runTest() {
  const uint n = 1000*1000*10;

  uint y = 0; 
  {
    double z;
    double lMaxQerror = 0;
    const double lBase = 1.1;
    Measure lMeasure;
    lMeasure.start();
    for(uint i = 1; i <= n; ++i) {
      y = q::encode<double>(i, 1.1, std::numeric_limits<uint>::max());
      z = q::decode<double>(y, 1.1);
      lMaxQerror = std::max<double>(lMaxQerror, q::qerror<double>(i,z));
    }
    lMeasure.stop();
  
    std::cout << "    q::en/decode: " << (lMeasure.mTotalTime() / n) * (1000*1000*1000) << "ns, "
              << "          qerror: " << lMaxQerror << std::endl;
  }

  {
    uint z;
    double lMaxQerror = 0;
    Measure lMeasure;
    lMeasure.start();
    for(uint i = 1; i <= n; ++i) {
      y = q::compress8<uint>(i);
      z = q::decompress8<uint>(y);
      lMaxQerror = std::max<double>(lMaxQerror, q::qerror<double>(i,z));
    }
    lMeasure.stop();
    std::cout << "q::en/decompress: " << (lMeasure.mTotalTime() / n) * (1000*1000*1000) << "ns, "
              << "          qerror: " << lMaxQerror << std::endl;
  }
}

double
runMeasureCompress() {
  const uint n = 1000*1000*10;
  uint y = 0;
  double yy = 0;

  {
    Measure lMeasure1;
    lMeasure1.start(); 
    for(uint i = 1; i <= n; ++i) {
      y |= q::encode<double>(i, 1.1, std::numeric_limits<uint>::max());
    }
    lMeasure1.stop(); 
  
    std::cout << "q::encode: " << (lMeasure1.mTotalTime() / n) * (1000*1000*1000) << " ns "
              << std::endl;
  
    Measure lMeasure2;
    lMeasure2.start(); 
    for(uint i = 1; i <= n; ++i) {
      yy += q::decode<double>(i & ((1 << 8) - 1), 1.1);
    }
    lMeasure2.stop(); 
               
    std::cout << "q::decode: " << (lMeasure2.mTotalTime() / n) * (1000*1000*1000) << " ns "
              << std::endl;
  }


  {
    Measure lMeasure1;
    lMeasure1.start();
    for(uint i = 1; i <= n; ++i) {
      y |= q::compress8<uint>(i);
    }
    lMeasure1.stop();

    std::cout << "q::compress: " << (lMeasure1.mTotalTime() / n) * (1000*1000*1000) << " ns "
              << std::endl;

    Measure lMeasure2;
    lMeasure2.start();
    for(uint i = 1; i <= n; ++i) {
      yy += q::decompress8<uint>(i & 0xFF);
    }
    lMeasure2.stop();

    std::cout << "q::decompress: " << (lMeasure2.mTotalTime() / n) * (1000*1000*1000) << " ns "
              << std::endl;
  }
  return yy+y; 
}


template<uint Tnobitskept, uint Tnoshiftbits>
void
testQCompressB(const uint n) {
  double lMaxQError = 0;
  for(uint i = 0; i < n; ++i) {
    const uint x = q::bqcompress<uint, Tnobitskept, Tnoshiftbits>(i);
    const uint z = q::bqdecompress<uint, Tnobitskept, Tnoshiftbits>(x);
    if(i != 0 || z != 0) {
      lMaxQError = std::max<double>(lMaxQError, q::qerror<double>(i,z));
    }
  }
  std::cout << std::setw(2) << Tnobitskept << ' '
            << std::setw(2) << Tnoshiftbits << ' '
            << std::setw(14) << lMaxQError << ' '
            << std::setw(14) << (pow(2.0, (double) 1.0 / (1 << Tnobitskept)))
            << std::endl;
}

void
test77() {
  std::cout << std::setw(2) << "b" << ' '
            << std::setw(2) << "s" << ' '
            << std::setw(14) << "qerror" << ' '
            << std::setw(14) << "2^-2^k" 
            << std::endl;
  std::cout.precision(12);
  testQCompressB<1,5> (100000);
  testQCompressB<2,5> (100000);
  testQCompressB<3,5> (100000);
  testQCompressB<4,5> (100000);
  testQCompressB<5,5> (100000);
  testQCompressB<6,5> (100000);
  testQCompressB<7,5> (100000);
  testQCompressB<8,5> (100000);
  testQCompressB<9,5> (100000);
  testQCompressB<10,5>(100000);
  testQCompressB<11,5>(100000);
  testQCompressB<12,5>(100000);
  testQCompressB<13,5>(100000);
}

void
test0() {
  const double lBase5a = q::calcBase<double>(1000, 5);
  const double lBase4a = q::calcBase<double>(1000, 4);
  const double lBase4b = q::calcBase<double>( 500, 4);
  const double lBase3a = q::calcBase<double>( 100, 3);
  const double lBase3b = q::calcBase<double>(  50, 3);
  const double lBase2a = q::calcBase<double>(  50, 2);
  const double lBase2b = q::calcBase<double>(  40, 2);
  const double lBase2c = q::calcBase<double>(  30, 2);
  std::cout << "lBase5a: " << lBase5a << "  1000" << std::endl
            << "lBase4a: " << lBase4a << "  1000" << std::endl
            << "lBase4b: " << lBase4b << "   500" << std::endl
            << "lBase3a: " << lBase3a << "   100" << std::endl
            << "lBase3b: " << lBase3b << "    50" << std::endl
            << "lBase2a: " << lBase2a << "    50" << std::endl
            << "lBase2b: " << lBase2b << "    40" << std::endl
            << "lBase2c: " << lBase2c << "    30" << std::endl
            ;
}


int
main() {

  const bool lRunAll = false;
  if(lRunAll) {
    // runTestCompress8();
    runTest();
    const uint y = runMeasureCompress();
    std::cout << "y: " << y << std::endl;
  }

  // test77();
  test0();

  return 0;
}


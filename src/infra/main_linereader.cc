#include "linereader.hh"


void
test0(const char* aFile1, const char* aFile2) {
  int lBufferSize = 99999;
  LineReader lLr1(aFile1, lBufferSize);
  LineReader lLr2(aFile2, lBufferSize);

  if(!lLr1.open()) {
    std::cout << "can't open file1" << std::endl;
  }
  if(!lLr2.open()) {
    std::cout << "can't open file2" << std::endl;
  }

  int lLineCount = 0;
  int lLineLength1 = 0;
  int lLineLength2 = 0;
  while(lLr1.ok() && lLr2.ok()) {
    ++lLineCount;
    lLineLength1 = lLr1.linesize();
    lLineLength2 = lLr2.linesize();
    if(lLineLength1 < lLineLength2) {
      std::cout << lLineCount << ' ' << lLineLength1 << ' ' << lLineLength2 << std::endl;
    }
    lLr1.next();
    lLr2.next();
  }

  if(lLr1.ok() && !lLr2.ok()) {
    std::cout << "file1 longer than file2" << std::endl;
  }
  if(!lLr1.ok() && lLr2.ok()) {
    std::cout << "file2 longer than file2" << std::endl;
  }

  std::cout << "line count = " << lLineCount << std::endl;
  
}


int
main(const int argc, const char* argv[]) {
  if(3 == argc) {
    test0(argv[1], argv[2]);
  }
}


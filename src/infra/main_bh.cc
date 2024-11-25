#include "buckethistogram.hh"


int
main() {
  BucketHistogram lBh(20);

  for(double i = -50.0; i < 50.0; i += 0.1) {
    lBh.insert(i);
  }

  lBh.print(std::cout);

  return 0;
}


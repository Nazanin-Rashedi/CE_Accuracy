#ifndef BUCKETHISTOGRAM_HH
#define BUCKETHISTOGRAM_HH

#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>
#include "types.hh"

/*
 Bucketizing Histogram: buckets: value: [-capacity,+capacity]
 floors double to int and inserts into bucket (++frequency).
 if outside [.,.] bound's frequencies are increased
*/

class BucketHistogram {
  private:
    BucketHistogram(const BucketHistogram&);
    BucketHistogram& operator=(const BucketHistogram&);
  public:
    BucketHistogram(); // sets default aBoundary to 10
    BucketHistogram(int aBoundary);
    ~BucketHistogram();
  public:
    void insert(const double x);
  public:
    inline int boundary() const { return _boundary; }
    inline uint size() const { return (2 * boundary() + 1); } // returns index
    inline uint begin() const { return 0; } // returns index
    inline uint end() const { return size(); } // returns index
    inline int  value(uint idx) const { return (idx - boundary()); } // dereferences index
    inline int  freq(uint idx) const { return (_hist[idx]); }        // dereferences index
    inline double min() const { return _min; }
    inline double max() const { return _max; }
    inline int noinf() const { return _noinf; }
    inline int nonan() const { return _nonan; }
    uint noEntries() const;
  public:
    std::ostream& print(std::ostream& os, bool aPrintPositiveOnly = false) const;
  public:
    static void  printManyRelative(const BucketHistogram* aHistArr, uint aNoHist,
                                   int aPrecision, std::ostream& os);
  private:
    void init();
    void doInsert(const int x);
  private:
    int    _boundary;
    int    _noinf; // number of infty
    int    _nonan; // number of NaN
    double _min;
    double _max;
    uint*  _hist;
};


#endif

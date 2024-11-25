#ifndef EstimatorBase2dim_HH
#define EstimatorBase2dim_HH

#include "types.hh"

namespace H2D {


class EstimatorBase2dim {
  public:
    EstimatorBase2dim() : _q(2), _theta(32) {}
    EstimatorBase2dim(const double aQ, const double aTheta) : _q(aQ), _theta(aTheta) {}
    virtual ~EstimatorBase2dim();
  public:
    inline double q() const { return _q; }
    inline double theta() const { return _theta; }
    inline double minEstimate() const { return _theta; } // minimal estimate returned by all estimators
  public:
    virtual double estimate(const query_t& lQuery) const = 0;
    virtual double estimate(const rectangle_t& r) const = 0;
    virtual void run_prediction(const char* testFileName);
    virtual void train_model(const char* trainFileName , const char* evalFileName);
    virtual void fill_libsvm_trainfiles() const;
    virtual void fill_libsvm_testfiles() const;

 
  public:
     inline void setQ(const double aQ) { _q = aQ; }
     inline void setTheta(const double aTheta) { _theta = aTheta; }
  public:
     static inline uint domValSize() { return _domValSize; }
     static inline uint freqValSize() { return _freqValSize; }
  public:
     virtual uint size() const = 0;
  public:
     virtual std::ostream& print_name_param(std::ostream& os) const = 0;
  private:
    double _q;
    double _theta;
  private:
    static uint _domValSize;  // either 4 or 8 byte to store domain value
    static uint _freqValSize; // typically 4 byte to store frequency
};

} // end namespace

#endif

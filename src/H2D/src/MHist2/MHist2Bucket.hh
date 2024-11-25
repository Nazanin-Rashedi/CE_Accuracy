#ifndef H2D_MHIST2_BUCKET_HH
#define H2D_MHIST2_BUCKET_HH

#include "../infra/types.hh"
#include "../infra/data2dim.hh"


namespace H2D {

class MHist2Bucket {
  public:
    MHist2Bucket(const Data2dim& aData);
    virtual ~MHist2Bucket();
  public:
    inline bool operator==(const MHist2Bucket& x) const { return (xlo() == x.xlo() && ylo() == x.ylo() && xhi() == x.xhi() && yhi() == x.yhi()); }
    inline bool operator<(const MHist2Bucket& x) const { return (xlo() < x.xlo() || (xlo() == x.xlo() && ylo() < x.ylo())); }
    inline bool operator<=(const MHist2Bucket& x) const { return (xlo() < x.xlo() || (xlo() == x.xlo() && ylo() <= x.ylo())); }
    inline bool operator>(const MHist2Bucket& x) const { return (xlo() > x.xlo() || (xlo() == x.xlo() && ylo() > x.ylo())); }
    inline bool operator>=(const MHist2Bucket& x) const { return (xlo() > x.xlo() || (xlo() == x.xlo() && ylo() >= x.ylo())); }
  public:
    static uint size();
  public:
    void estimate(double& aEstimate, const rectangle_t& aRect) const;
  public:
    inline double xlo() const { return _rect.xlo(); }
    inline double ylo() const { return _rect.ylo(); }
    inline double xhi() const { return _rect.xhi(); }
    inline double yhi() const { return _rect.yhi(); }
  private:
    rectangle_t _rect;
    uint _c;
};

} // end namespace


#endif 

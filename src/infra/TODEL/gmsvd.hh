#ifndef GMSVD_HH
#define GMSVD_HH

#include <limits>
#include "infra/matrix.hh"

/*
  zerlegung von A in 
  A = U W V^T
  A^-1 = V [diag(1/w_j)] U^T
  pseudoinvers:
  V [diag(0 == w_j ? 0 : (1/w_j))] U^T

  A MxN
  U MxN orthonormal
  W NxN diagonal, als vektor gespeichert
  V NxN orthonormal
*/

class GMSVD {
  private:
    GMSVD(const GMSVD&) = delete;
    GMSVD& operator=(const GMSVD&) = delete;
  public:
    GMSVD(const Matrix& A, 
          double aEpsilon = std::numeric_limits<double>::epsilon());
    ~GMSVD();
  private:
   inline const struct SVD* svd() const { return _svd; }
   inline struct SVD* svd() { return _svd; }
   inline double eps() const { return _epsilon; }
  public:
    void solve(const Matrix& b, Matrix& x /* out */, double aThreshold = -1.0);
    int rank(double aThreshold = -1.0);
    int nullity(double aThreshold = -1.0);
    double norm() const; // largest singular value
    void get_U(Matrix& U) const;
    void get_W(Matrix& W) const; // vektor Nx1
    void get_V(Matrix& V) const;
    void get_Winv(Matrix& W) const; // get inverse of W  = diag(0 == w_j ? 0 : (1/w_j))
    void get_mpi(Matrix& M) const; // get pseudoinvers (moore penrose inverse)
  private:
    double _epsilon;
    struct SVD* _svd;
};

#endif

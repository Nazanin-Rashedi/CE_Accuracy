#ifndef INFRA_WAVLET_TRANSFORM_NON_STANDARD_2DIM_HH
#define INFRA_WAVLET_TRANSFORM_NON_STANDARD_2DIM_HH

#include <iostream>
#include <vector>
#include <assert.h>

#include "infra/bit_intrinsics.hh"
#include "infra/matrix.hh"
#include "infra/TopK.hh"

/*
 * class WaveletTransformNonStd2dim
 * static functions to calculate wavelet transform
 * A: original (data) input matrix
 * W: matrix of wavelet coefficients
 * both must be 2^m X 2^m
 */

class WaveletTransformNonStd2dim {
  public:
    typedef unsigned int uint;
    struct coeff_t {
      uint i;
      uint j;
      double c; // wavelet coefficient W(i,j)
      coeff_t(const uint aI, const uint aJ, const double aC) : i(aI), j(aJ), c(aC) {}
      bool operator<(const coeff_t& x) const {
        return (std::abs(c) > std::abs(x.c)) || ((std::abs(c) == std::abs(x.c)) && (i < x.i && j < x.j));
      }
    };
    typedef std::vector<coeff_t> coeff_vt;
  public:
    WaveletTransformNonStd2dim();
    ~WaveletTransformNonStd2dim();
  public:
    // A -> W, W is matrix of wavelet coefficients, A is input matrix
    static void transform(Matrix& W, const Matrix& A);

    // W -> A, W is matrix of wavelet coefficients, A is original matrix
    static void inverse(Matrix& A, const Matrix& W);

    // extract top k coefficients
    static void extractTopK(coeff_vt& aCv, const Matrix& W, const uint aK);

    // reconstitute A(k,l) from vector of wavelet coefficients
    // n is the dimension of the original input matrix
    static double inverseCoeff(const coeff_vt& C, const uint k, const uint l, const uint n);
  public:
    static void   normalize(Matrix&);
    static void   denormalize(Matrix&);
    static void   denormalize(coeff_vt&);
    static double normalizeCoeff(const uint i, const uint j, const double c);
    static double denormalizeCoeff(const uint j, const uint i, const double c);
  public:
    inline static uint getLevel(const uint i, const uint j) {
                         return idx_highest_bit_set(i | j | 0x1);
                       }

                /*
                 * wsign(i,j, aSignCase) 
                 * let w(i,j) be a wavelet coeffcient. then let
                 * bi: level bit of wavelet index i in bit number 0
                 * bj: level bit of wavelet index j in bit number 0
                 * sign case: (level) quadrant (00, 01, 10, 11) 
                 *            of number to be reconstructed from wavelet coefficients
                 * then: wsign(i, j, aSignCase) return 1 if '+' and 0 if '-' is the wavelet sign
                 */
    inline static uint wsign(const uint bi, const uint bj, const uint aQuadrant) {
                         const uint a = aQuadrant;
                         const uint b = (bj << 1) | bi;
                         return ( !(a & b) | (3 == (a & b)) );
                       }
  private:
    static void transformSub(Matrix& W, const Matrix& A, const uint w);
    static void inverseSub(Matrix& A, const Matrix& W, const uint w);
};



#endif

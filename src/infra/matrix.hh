#ifndef MATRIX_HH
#define MATRIX_HH

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <assert.h>
#include <vector>
#include <random>

#include "glob_infra_standard_includes.hh"
#include "q.hh"
#include "float_ops.hh"

#define CHECK assert(0 <= i && i < noRows() && 0 <= j && j < noCols());
// #define CHECK

// ...XF procedures maybe faster but are unsafe

class Matrix {
  public:
    typedef std::mt19937 rng32_t;
  public:
    enum format_t {
      kAscii    = 0,
      kMatlab   = 1,
      kLatex    = 2,
      kNoFormat = 3
    };
  public:
    class CholeskyFailed { 
      public:
        CholeskyFailed(const int i, const double d) : _index(i), _sum(d) {}
        inline int    index() const { return _index; }
        inline double sum() const { return _sum; }
      private:
        int    _index;
        double _sum;
    }; // exception class
  public:
    Matrix();
    Matrix(uint n, uint m);
    Matrix(uint n, uint m, double a11 ...); // double, ...: array elements
    Matrix(uint n, uint m, double*);
    Matrix(const Matrix&);
    virtual ~Matrix();
  public:
    inline uint noRows() const { return _noRows; }
    inline uint noCols() const { return _noCols; }
    inline uint size() const { return noRows() * noCols(); }
  public:
    int    countZeroElements() const;
    int    countNonZeroElements() const;
    int    countNegElementsInRow(const int aRowNo, const double aEps /* >= 0 */) const;
    int    countPosElementsInRow(const int aRowNo, const double aEps /* >= 0 */) const;
    double sumAllElements() const;
    double sumAllElements(uint aRowStart, uint aRowEnd, uint aColStart, uint aColEnd) const;
    double minDiag() const; // minimum element on diagonal
    double min() const; // overall min;
    double max() const; // overall max;
    double absMin() const; // minimum of absolute entries
    double absMax() const; // minimum of absolute entries
    double absMinNonZero(const double aEps) const; // minimum of absolute entries, excluding zero
    double prodDiag() const; // product of elements on diagonal
    double minUpperTriag() const; // minimum in upper triagonal matrix (excluding diagonal)
    double maxQDeviationFromSymmetry() const;  // maximum q-error upper/lower triangular
    double prodQDeviationFromSymmetry() const; // product of q-errors upper/lower triangular
    double prodUpperDivProdLower() const; // upper triangular, lower triangular products
  public:
    void rescale(const uint n, const uint m);
    void rescale_no_init(const uint n, const uint m);
    inline void resize(const uint n, const uint m) { rescale(n,m); }
    void setNull();
    void setNullXF();
    void shrinkNoInit(const uint n, const uint m);
    void roundAll(); // round all elements in matrix
  public:
    bool isEinheitsmatrix(const double aEps) const;   // (*this) = I_n
    bool isIdentity(const double aEps = 1E-16) const; // also true for m,n matrices if min(n,m) identity
    bool isIdentityUpTo(const uint aN, const double aEps = 1E-16) const;
    bool isSymmetric(const double aEps = 1E-16) const;
    bool isMinorSymmetric(const double aEps = 1E-16) const; // persymmetric
    bool isTransposedOf(const Matrix& M, const double aEps = 1E-16) const;
    bool isMinorTransposedOf(const Matrix& M, const double aEps = 1E-16) const; // a(i,j)=b(m-i,n-j)
    bool hasZeroOneDiagonal(const double aEps = 1E-16) const;
    bool has_diag_1(const double aEps = 1E-16) const;
    bool hasSameDiagonalAs(const Matrix& M, const double aEps = 1E-16) const;
    bool isZero(const double aEps) const;
    bool hasColSum1(const double aEps) const;
    bool hasZeroBlock(const uint aRowStart, const uint aRowEnd,
                      const uint aColStart, const uint aColEnd,
                      const double aEps) const;
    bool hasBlock(const uint aRow, const uint aCol, const Matrix& A, const double aEps) const;
    bool isBlockMatrixHaupt(const Matrix& A, const Matrix& B,
                            const double aEps) const;
    bool isBlockMatrixNeben(const Matrix& A, const Matrix& B,
                            const double aEps) const;
    void getBlock(Matrix& A, const uint aRowBegin, const uint aRowEnd, const uint aColBegin, const uint aColEnd) const;
  public:
    Matrix& operator=(const Matrix&);
  public:
    inline double& operator()(uint i, uint j) const { CHECK return _m[i * noCols() + j]; };
    inline double& operator()(uint i, uint j)       { CHECK return _m[i * noCols() + j]; };
  public:
    inline const double* data() const { return _m; }
    inline       double* data()       { return _m; }
    inline const double* getArray() const { return _m; }
    inline       double* getArray()       { return _m; }
    inline const double* row(uint i) const { return &(_m[i*noCols()]); }
    inline const double* getPtrToElem(const uint i, const uint j) const { return &(_m[i*noCols() + j]); }
    inline       double* getNonConstPtrToElem(const uint i, const uint j) { return &(_m[i*noCols() + j]); }
  public:
    void perpose(); // transpose along the minor diagonal
  public:
    void copyColumn(const uint aToColumn, const uint aFromColumn);
    void setToSumOf(const Matrix& m1, const Matrix& m2);
    void setToSumWithSecondTransposed(const Matrix& m1, const Matrix& m2);
    void setToDifferenceOf(const Matrix& m1, const Matrix& m2);
    void setToTransposedOf(const Matrix& m);
    void setToPerposedOf(const Matrix& m);
    void setToProductOf(const Matrix& m1, const Matrix& m2);
    void setToProductOfXF(const Matrix& m1, const Matrix& m2); // same as above
    void setToProductWithDiagAsVector(const Matrix& A, const double* v);
    void setToProductWithDiagAsVector(const double* v, const Matrix& A);
    void setToAdiagvAt(const Matrix& M, const double* v);
    void setToProductOfMatrixDiag(const Matrix& A, const Matrix& D); // D is diag  n,1 !!! (als Spaltenvector!!!)
    void setToProductOfDiagMatrix(const Matrix& D, const Matrix& A); // D is diag  n,1 !!! (als Spaltenvector!!!)
    void setToProductWithFirstTransposed(const Matrix& m1, const Matrix& m2);
    void setToProductWithSecondTransposed(const Matrix& m1, const Matrix& m2);
    void setToUUt(const Matrix& U); // U upper triangular calculate UU^t // fast
    void setToShrink2Of(const Matrix& M);
    void setToReverseRows(const Matrix& M);
    void setToReverseCols(const Matrix& M);
    Matrix& setToKroneckerProductOf(const Matrix& A, const Matrix& B);
    Matrix& setToDiag(const double  x, const uint n);
    Matrix& setToDiag(const double* x, const uint n);
    Matrix& setToCounterDiag(const double  x, const uint n);
    Matrix& setToCounterDiag(const double* x, const uint n);
    Matrix& setToAxB(const Matrix& A, const Matrix& B);
    Matrix& setToAtxB(const Matrix& A, const Matrix& B);
    Matrix& setToAxBt(const Matrix& A, const Matrix& B);
    Matrix& setToAtxBt(const Matrix& A, const Matrix& B);
    Matrix& setToTransposedOfAtxBt(const Matrix& A, const Matrix& B);
    void setToInverseOfUpperTriangular(const Matrix& U, const double eps);
    void setToTransposedOfInverseOfUpperTriangular(const Matrix& U, const double eps);
    void setToGInverseOfUpperTriangular(const Matrix& U, const double eps);
    // A symmetric, pos. def. /noneg. def find upper triangular U s.t. A = U'U
    // see Harville p. 237ff, Cholesky decomposition
    void setToSquareRootUpperTriangular(const Matrix& U);
    // void setToTransposedOfSquareRootUpperTriangular(const Matrix& M, const double aEps);
    void setToSquareRootLowerTriangular(const Matrix& L);
    void setToSquareRootLowerTriangular_XF(const Matrix& L, const double aEps);
    void setToSquareRootLowerTriangular_XF(const double aEps);
    void solveSquareRootLowerTriangular(double *x, const double* b) const; // solve Ax=b, after Cholesky Decomp
    void inverseSquareRootLowerTriangular(const Matrix& L); // m must be lower triangular cholesky decomp 
                                                            // of matrix to be inverted
    void inverseSquareRootLowerTriangular_XF(const Matrix& L);
    void setToTransposedOfInverseOfLowerTriangular(const Matrix& L);
    // calculate inverse of symmetric positive definite A using cholesky decomposition
    void setToInverseOfPositiveDefinite(const Matrix& A, const double aEps);
    // LDL decomposition, returns false if some D_j becomes zero
    // M = LDL^t, L lower triangular, D diagonal matrix
    // like cholesky only for symmetric matrices
    bool getLDL(Matrix& L, Matrix& D) const;
    void setToMMt(const Matrix& m); // this = M   * M^t
    void setToMtM(const Matrix& m); // this = M^t * M
    void getAx(double* y, const double* x) const; // y = Ax (this = A)
    void getAx_XF(double* y, const double* x) const; // y = Ax (this = A)
    void getAtx(double* y, const double* x) const; // y = A^tx (this = A)
    void solveUpperTriangular(double* x, const double* b) const;  // solve Ax   = b, A must be upper triangular
    void solveLowerTriangular(double* x, const double* b) const;  // solve Ax   = b, A must be lower triangular
    void solveUpperTriangularT(double* x, const double* b) const; // solve A^tx = b, A must be upper triangular
    void solveLowerTriangularT(double* x, const double* b) const; // solve A^tx = b, A must be lower triangular
    void setAllTo(double x);
    void setAllTo(double x, uint aRowStart, uint aRowEnd, uint aColStart, uint aColEnd);
    // if -eps < a(i,j) < eps then set to zero
    void setZeroSmallerThan(const double eps); // if |a(i,j)| < eps then set to zero
    void setZeroLessThan(const double x);      // if  a(i,j)  < eps
    void lnAll();
    void addToAll(const double x);
    void multiplyAllBy(const double x);
    void divideAllBy(const double x);
    void divideAllBy(const double x, const uint aRowStart, const uint aRowEnd, const uint aColStart, const uint aColEnd);
    void logAll();
    void rowNormalize(); // divide all row elements by row sum
    void colNormalize(); // divide all column elements by column sum
    void setToRowSubmatrix(const Matrix& m, const uint* aRows, uint aNoRows);
    void setToSubmatrix(const Matrix& M, const uint aNoRows, const uint aNoCols);
    // discrete cosine transform (dct)
    // build matrix C (p397)
    Matrix& setToDiscreteCosineOrthonormalMatrix(const uint n);
    Matrix& setToDiscreteCosineTransform(const Matrix& m);
    Matrix& setToInverseDiscreteCosineTransform(const Matrix& m);
    Matrix& setToDiscreteCosineTransformViaC(const Matrix& m);
    Matrix& setToInverseDiscreteCosineTransformViaC(const Matrix& m);
    double  discreteCosineTransformCoefficient(const uint i, const uint j) const;
    double  inverseDiscreteCosineTransformCoefficient(const uint i, const uint j) const;
    double  dctEstimateSlow(const uint aRowLo, const uint aRowHi, 
                            const uint aColLo, const uint aColHi) const;
    double  dctEstimate(const uint aRowLo, const uint aRowHi, 
                        const uint aColLo, const uint aColHi) const;
    double dctEstimateIntegral(const uint aIdx, const uint aMaxIdx, 
		               const uint aLo, const uint aHi) const;
    double dctEstimateContinuous(const double aRowLo, const double aRowHi,
                                 const double aColLo, const double aColHi) const;
    static double dctEstimateIntegralContinuous(const uint aIdx, const uint aMaxIdx, 
                                                const double aLo, const double aHi);
    static inline double dct_k(const uint i) { return ((0 == i) ? (1.0 / sqrt(2.0)) : 1.0); }
  public:
    void setToElementWiseProduct(const Matrix& m1, const Matrix& m2);
    void setToElementWiseQuotient(const Matrix& m1, const Matrix& m2);
    void colSumInclusive(); // sums up colums destructively
    void rowSumInclusive(); // sums up colums destructively
    void colSumExclusive(); // sums up colums destructively
    void rowSumExclusive(); // sums up colums destructively
    void accumulateInclusive(); // in m(k,l): sum of m(i,j) with i <= k, j <= l
    void accumulateExclusive(); // in m(k,l): sum of m(i,j) with i < k, j < l
  public:
    void setToElimRows(const Matrix& A, const uint_vt& aRowsToEliminate); // aRowsToEliminate must be sorted
    void setToElimCols(const Matrix& A, const uint_vt& aColsToEliminate); // aRowsToEliminate must be sorted
    void setToOnlyRows(const Matrix& A, const uint_vt& aRowsToRetain);    // aRowsToRetain must be sorted
    void setToOnlyCols(const Matrix& A, const uint_vt& aColsToRetain);    // aRowsToRetain must be sorted
    void setToReduction(const Matrix& A, const uint_vt& aRows, const uint_vt& aCols);
  public:
    uint convertToHermiteNormalFormUUT(Matrix* aF = 0); // sets this to hermetian normal form, must be unit upper triangular
  public:
    void qr_mgs(Matrix& Q, Matrix& R) const; // modified gram-schmidt qr-zerlegung
  public:
    bool isZeroRow(const uint i, const double aEps) const;
    bool isZeroCol(const uint j, const double aEps) const;
    uint noZeroLoRows() const;
    uint noZeroHiRows() const;
    uint noZeroLoCols() const;
    uint noZeroHiCols() const;
    bool collectNonZeroRowCol(uint_vt& aNonZeroRows, 
                              uint_vt& aNonZeroCols, 
                              const double* x,
                              const double  aEps) const;
  public:
    bool   equalUpTo(const Matrix& M, const double aEps) const;
    bool   equalUpToFirstKRows(const Matrix& M, const uint aK, const double aEps) const;
    bool   isUpperTriangular(const double aEps = 1E-16) const;
    bool   isLowerTriangular(const double aEps = 1E-16) const;
    bool   isDiagonal(const double aEps = 1E-16) const;
    bool   isUnitUpperTriangular(const double aEps = 1E-16) const;
    bool   isDiag01UpperTriangular(const double aEps = 1E-16) const;
    bool   isPermOfHermiteNormalForm(const double aEps = 1E-16) const;
    bool   isHessenbergMatrix(const double Eps = 1E-16) const;
    bool   isHessenbergMatrixTransposed(const double Eps = 1E-16) const;
    bool   isAbsEqual(const Matrix& M, const double aEps = 1E-16) const;
    double qerror(const Matrix& M) const; // max of elementwise q-error
    double abserror(const Matrix& M) const; // max of elementwise abserror
  public:
    double getTrace() const; // sum_i M(i,i)
    double norm_1() const; // ||M||_1
    double norm_infty() const; // ||M||_\infty
    double norm_frobenius() const; // ||M||_F (Frobenius norm)
    double norm_sqrt_dot() const;  // ||M|| := sqrt(A \dot A)
    double innerMatrixProduct(const Matrix&) const; // A \dot B := tr(A'B) (= tr(AB')) (see Harville 58)
  public:
    double oneSidedCoherence() const;
    double twoSidedCoherence() const;
  public:
    void   initAsRotationMatrix(const double aAlpha); // 2-dim rotationsmatrix
  public:
    static void   vecSum(double* x, const double* v1, const double* v2, const uint aSize);
    static double vecDotProd(const double* v1, const double* v2, const uint aSize);
           double colColDotProd(const uint c1, const uint c2) const;
           double rowRowDotProd(const uint r1, const uint r2) const;
           double colRowDotProd(const uint aRow, const uint aCol) const; // n x n only
           double colVecDotProd(const double* v, const uint aColNo) const;
           double rowProdVec(const uint aRowNo, const double* v) const;
           double selfTransposedProdSelf(uint i, uint j, uint aStopXcl) const; // return (A^T A)(i,j)
    static double AB(const Matrix& A, const Matrix& B, uint i, uint j, uint aStopXcl);
    static double IminusAB(const Matrix& A, const Matrix& B, uint i, uint j, uint aStopXcl);
    static void   vecMPI(double* lRes, const double* v, const uint aSize);
           void   setToMoorePenroseInverseOf(const Matrix& A, const double aEpsilon = 1E-11, const bool aTrace = false); // set to Moore-Penrose inverse of A
    static void   productOf(double* aRes, const Matrix& A, const double* v); // aRes = Av 
    static void   productOfXF(double* aRes, const Matrix& A, const double* v); // aRes = Av, slow
    static void   productOfAtV(double* aRes, const Matrix& A, const double* v); // aRes = A^tv,  slow
    static void   productOfAtVXF(double* aRes, const Matrix& A, const double* v); // aRes = A^tv 
    static void   productOfAtVXF01(double* aRes, const Matrix& A, const double* v); // aRes = A^tv 
    static std::ostream& printVector(const double* v, const uint aSize);
    static std::ostream& printVector(std::ostream& os, const double* v, const uint aSize);
    static std::ostream& printVector(std::ostream& os, const double* v, const uint aSize, const int aWidth);
  public:
    void initFromVector(const double* x);
    void initFromStream(std::istream& is, const uint m, const uint n);
    void initFromStream(std::istream& is); // first two numbers must be m and n
    void initRandom(rng32_t& aRng, const uint aMod);
    std::ostream& print(std::ostream& os, const int aFieldWidth, const int aIndent, const format_t aFormat) const;
    std::ostream& print(std::ostream& os, const int aFieldWidth, const int aIndent = 0) const;
    std::ostream& printDown(std::ostream& os, const int aFieldWidth) const;
    std::ostream& printRoundX(std::ostream& os, const int aFieldWidth) const;
    std::ostream& printRoundXX(std::ostream& os, const int aFieldWidth) const;
    std::ostream& printWithRowSum(std::ostream& os, const int aFieldWidth) const;
    std::ostream& printWithRowColSum(std::ostream& os, const int aFieldWidth) const;
    std::ostream& printMatlab(std::ostream& os, const int aFieldWith, const int aIndent = 0) const;
    std::ostream& printLatex(std::ostream& os, const int aFieldWith, const int aIndent) const;
  protected:
    void assign(const Matrix& m);
    int  error(const char*);
  protected:
    uint  _noRows;
    uint  _noCols;
    double* _m;
};

#endif

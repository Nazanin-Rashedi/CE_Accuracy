#ifndef SRC_INFRA_ROUND_HH
#define SRC_INFRA_ROUND_HH

#include <inttypes.h>
#include <stdlib.h>
#include <cmath>
#include <type_traits>

// template wrappers for cmath

namespace mt {

// sign
template <typename T> 
int 
sign(const T val) {
    return (T(0) < val) - (val < T(0));
}

template <typename T>
inline constexpr int
signum(T x, std::false_type is_signed) {
    return T(0) < x;
}

template <typename T>
inline constexpr int
signum(T x, std::true_type is_signed) {
    return (T(0) < x) - (x < T(0));
}

template <typename T>
inline constexpr int
signum(T x) {
    return signum(x, std::is_signed<T>());
}

// round
template<class Tfloat>
inline Tfloat
roundt(const Tfloat);


template<>
inline double
roundt<double>(const double x) {
  return round(x);
}

template<>
inline float
roundt<float>(const float x) {
  return roundf(x);
}

// round X, roundXX, roundXXX

template<class Tfloat>
inline Tfloat
roundXt(const Tfloat x) {
  return (roundt<Tfloat>(x * (Tfloat) 10) / (Tfloat) 10);
}

template<class Tfloat>
inline Tfloat
roundXXt(const Tfloat x) {
  return (roundt<Tfloat>(x * (Tfloat) 100) / (Tfloat) 100);
}

template<class Tfloat>
inline Tfloat
roundXXXt(const Tfloat x) {
  return (roundt<Tfloat>(x * (Tfloat) 1000) / (Tfloat) 1000);
}

template<class Tfloat>
inline Tfloat
roundX4t(const Tfloat x) {
  return (roundt<Tfloat>(x * (Tfloat) 10000) / (Tfloat) 10000);
}

template<class Tfloat>
inline Tfloat
roundX6t(const Tfloat x) {
  return (roundt<Tfloat>(x * (Tfloat) 1000000) / (Tfloat) 100000000);
}

// floor

template<class Tfloat>
inline Tfloat
floort(const Tfloat);


template<>
inline double
floort<double>(const double x) {
  return floor(x);
}

template<>
inline float
floort<float>(const float x) {
  return floorf(x);
}


// abs
template<typename Tnum>
inline Tnum abst(const Tnum);

template<>
inline double abst<double>(const double x) {
  return fabs(x);
}

template<>
inline float abst<float>(const float x) {
  return fabsf(x);
}

template<>
inline int abst<int>(const int x) {
  return abs(x);
}

template<>
inline int64_t abst<int64_t>(const int64_t x) {
  return llabs(x);
}

// ceil

template<class Tfloat>
inline Tfloat
ceilt(const Tfloat);


template<>
inline double
ceilt<double>(const double x) {
  return ceil(x);
}

template<>
inline float
ceilt<float>(const float x) {
  return ceilf(x);
}

// log2

template<class Tfloat>
inline Tfloat
log2t(const Tfloat);


template<>
inline double
log2t<double>(const double x) {
  return log2(x);
}

template<>
inline float
log2t<float>(const float x) {
  return log2f(x);
}

// loge (ln)

template<class Tfloat>
inline Tfloat
logt(const Tfloat);


template<>
inline double
logt<double>(const double x) {
  return log(x);
}

template<>
inline float
logt<float>(const float x) {
  return logf(x);
}

// log10

template<class Tfloat>
inline Tfloat
log10t(const Tfloat);


template<>
inline double
log10t<double>(const double x) {
  return log10(x);
}

template<>
inline float
log10t<float>(const float x) {
  return log10f(x);
}


// log with base 

template<class Tfloat>
inline Tfloat
logbaset(const Tfloat x, const Tfloat base);

template<>
inline float
logbaset<float>(const float x, const float base) {
  return (logf(x) / logf(base));
}

template<>
inline double
logbaset<double>(const double x, const double base) {
  return (log(x) / log(base));
}


// exp

template<class Tfloat>
inline Tfloat
expt(const Tfloat);


template<>
inline double
expt<double>(const double x) {
  return exp(x);
}

template<>
inline float
expt<float>(const float x) {
  return expf(x);
}

// pow

template<class Tfloat>
inline Tfloat
powt(const Tfloat x, const Tfloat y);

template<>
inline double
powt<double>(const double x, const double y) {
  return pow(x, y);
}

template<>
inline float
powt<float>(const float x, const float y) {
  return powf(x, y);
}


template<typename Tfloat>
inline Tfloat
sqrtt(const Tfloat);

template<>
inline double
sqrtt<double>(const double x) {
  return sqrt(x);
}


template<>
inline float
sqrtt<float>(const float x) {
  return sqrtf(x);
}


// sin

template<typename Tfloat>
inline Tfloat
sint(const Tfloat x);

template<>
inline double
sint<double>(const double x) {
  return sin(x);
}

template<>
inline float
sint<float>(const float x) {
  return sinf(x);
}

// cos

template<typename Tfloat>
inline Tfloat
cost(const Tfloat x);

template<>
inline double
cost<double>(const double x) {
  return cos(x);
}

template<>
inline float
cost<float>(const float x) {
  return cosf(x);
}




// gamma

template<typename Tfloat>
inline Tfloat
fLogGamma(const Tfloat x);

template<>
inline double
fLogGamma<double>(const double x) {
  #ifdef __OLD_SAP_
  return gamma(x);
  #else
  return std::lgamma(x);
  #endif
}

template<>
inline float
fLogGamma<float>(const float x) {
  #ifdef __OLD_SAP_
  return gammaf(x);
  #else
  return std::lgammaf(x);
  #endif
}

// almost equality

template<typename Tfloat>
inline bool
equal(const Tfloat x, const Tfloat y, const Tfloat eps) {
   return ((x < y) ? (y - x < eps) : (x - y < eps));
}


// factorial by gamma


template<typename Tfloat>
inline Tfloat
fLogFactorial(const Tfloat n) {
  return mt::fLogGamma<Tfloat>(n + 1.0);
}

template<typename Tfloat>
inline Tfloat
fFactorial(const Tfloat n) {
  return mt::expt<Tfloat>(mt::fLogFactorial<Tfloat>(n));
}

// log factorial via stirling

// binomial

template<typename Tfloat>
inline Tfloat
fBinom(const Tfloat n, const Tfloat k) {
  if(k <= 0.000001) { return 1.0; }
  return mt::expt<Tfloat>(fLogFactorial(n)-(fLogFactorial(k)+fLogFactorial(n-k)));
}

template<typename Tfloat>
inline Tfloat
fBinomGamma(const Tfloat n, const Tfloat k) {
  if(k <= 0.000001) { return 1.0; }
  return mt::expt<Tfloat>((fLogFactorial(n)-fLogFactorial(k))-fLogFactorial(n-k));
}

template<typename Tfloat>
inline Tfloat
fLogBinomGamma(const Tfloat n, const Tfloat k) {
  if(k <= 0.000001) { return 1.0; }
  // return ((fLogFactorial(n)-fLogFactorial(k))-fLogFactorial(n-k));
  return ((fLogFactorial(n) - fLogFactorial(n-k)) - fLogFactorial(k));
}

// using stirling's approximation
// this is only an APPROXIMATION (!)
template<typename Tfloat>
inline Tfloat
fLogBinomStirling(const Tfloat n, const Tfloat k) {
  return (- std::log(std::sqrt(2.0 * M_PI)) 
          + ((n     + 0.5) * std::log(n))
          - ((n - k + 0.5) * std::log(n - k))
          - ((    k + 0.5) * std::log(k)));
}

// this is only an APPROXIMATION (!)
template<typename Tuint>
inline Tuint
fBinomStirling(const Tuint n, const Tuint k) {
  if(0 == k) {
    return 1;
  }
  if(1 == k) {
    return n;
  }
  if(n == k) {
    return 1;
  }
  if(n == (k + 1)) {
    return n;
  }
  return ((Tuint) std::floor(std::exp(fLogBinomStirling<double>((double) n, (double) k))));
}


// for multisets
template<typename Tfloat>
inline Tfloat
fNoMultisets(const Tfloat N, const Tfloat k) {
  return mt::fBinomGamma<Tfloat>(N + k - 1, k);
}

template<typename Tfloat>
inline Tfloat
fNoMultisetsGamma(const Tfloat N, const Tfloat k) {
  return mt::fBinomGamma<Tfloat>(N + k - 1, k);
}

template<typename Tfloat>
inline Tfloat
fLogNoMultisetsGamma(const Tfloat N, const Tfloat k) {
  return mt::fLogBinomGamma<Tfloat>(N + k - 1, k);
}

// calculate/solve quadratic equation
// ax^2 + bx + c   

template<typename Tfloat>
inline Tfloat
calcQuadraticEquation(const Tfloat x, const Tfloat a, const Tfloat b, const Tfloat c);

template<>
inline double
calcQuadraticEquation(const double x, const double a, const double b, const double c) {
  return (a*x*x + b*x + c);
}

template<>
inline float
calcQuadraticEquation(const float x, const float a, const float b, const float c) {
  return (a*x*x + b*x + c);
}

template<typename Tfloat>
inline Tfloat
solveQuadraticEquationPos(const Tfloat a, const Tfloat b, const Tfloat c);

template<>
inline double
solveQuadraticEquationPos(const double a, const double b, const double c) {
  return (-b + sqrtt<double>(b*b - 4*a*c)) / (2 * a);
}

template<>
inline float
solveQuadraticEquationPos(const float a, const float b, const float c) {
  return (-b + sqrtt<float>(b*b - 4*a*c)) / (2 * a);
}

template<typename Tfloat>
inline Tfloat
solveQuadraticEquationNeg(const Tfloat a, const Tfloat b, const Tfloat c);

template<>
inline double
solveQuadraticEquationNeg(const double a, const double b, const double c) {
  return (-b - sqrtt<double>(b*b - 4*a*c)) / (2 * a);
}

template<>
inline float
solveQuadraticEquationNeg(const float a, const float b, const float c) {
  return (-b - sqrtt<float>(b*b - 4*a*c)) / (2 * a);
}



// useful formulas from bqc

/* the following template procedures are different estimates for
 * computing the number of blocks accessed if k records out of n records
 * are accessed and these n records are distributed on m blocks. 
 * Assumptions:
 * 1) Every block contains the same number of records.
 * 2) Every record is accessed with the same probability.
 * Luk \cite{Luk83} and Christodoulakis \cite{Chri84tods} relax these assumptions.
 * IJbema and Blanken \cite{IjBl86} give a formula for all possible distributions,
 * where a distribution is characterized by the number of records stored in a bucket.
 * The distribution (called x in the paper) hence contains m numbers if there are m buckets.
 *
 * here are two example applications:
 *
 * 1) assume indexed access to a relation: 
 *    1) index access results in a number of RIDs.
 *    2) these RIDs are then sorted and then
 *    3) the relation is accessed.
 *    How many page accesses do we have in step 3?
 *
 * 2) assume a index nested loop join:
 *    1) sort first relation on RIDs
 *    2) access the second relation via the index on the join key
 *    How many different records do we access in step 2?
 *    put differently:
 *    How many different key-values are there in the first relation.
 *
 * Another example is computing the costs of a semi-join in a distributed environment (SDD-1-paper)
 *
 * Below we find the first published approximation which is Cardenas formula.
 * Subsequently, Yao showed that Cardenas formula is a little imprecise (since the underlying
 * assumption does not necessarily hold in the database context (see also below)) and suggested
 * his own formula. Since this formula is a little slow
 * Gardy and Nemirovski gave a good approximation which can be computed much faster.
 * All these formulas are iterative formulas.
 * Hence, researchers tried to find closed formulas.
 * Two closed formulas are given below, one by Ijbema and Blanken and the other one by
 * Whang, Wiederhold and Sagalowicz. The former is an approximation of the original formula
 * by line segments whereas the latter is a real closed formula.
 *
 * The difference between Cardenas' formula and Yao's formula is the following assumption:
 * 1) Cardeas assumes that a record can be chosen more than once
 *    That is, in the urn model, we put the record back every time we've drawn it.
 * 2) Yao assumes that a record can be chosen exactly once.
 *    Here, the records are not put back.
 *
 * Drmota, Gardy, and Gittenberger give an overview of urn models in \cite{DrGaGi01}.
 */




/*
 * Cardenas's formula   \cite{Card75}
 * k: number of records accessed
 * n: total number of records
 * m: total number of blocks
 * returns number of blocks to be accessed
 * (actually urn model with replacement)
 */

template<typename Tfloat>
Tfloat
fCardenas(const Tfloat k, const Tfloat n, const Tfloat m) {
  if(k > (n - n/m)) return m;
  if(k <= 1.0) { return k; }
  const Tfloat f = (1 - 1/m);
 
  const Tfloat p = mt::powt<Tfloat>(f, k);
  const Tfloat lRes =  m * (1.0 - p);
  return (lRes > m) ? m : lRes;
}

template<typename Tfloat>
Tfloat
fYaoGamma(const Tfloat k, const Tfloat n, const Tfloat m) {
  return m * (1.0 - exp(fLogBinomGamma(n-(n/m),k) - fLogBinomGamma(n,k)));
}

template<typename Tfloat>
Tfloat
fYaoProbGamma(const Tfloat N, const Tfloat n, const Tfloat k) {
  return (1.0 - exp(fLogBinomGamma(N-n,k) - fLogBinomGamma(N,k)));
}


/* Whang, Wiederhold, Sagalowicz  \cite{WhWiSa83}
 * k: number of records accessed
 * n: total number of records
 * m: total number of buckets
 * in the paper:
 * k: number of records accessed
 * n: total number of records
 * m: total number of buckets
 *
 * Although this is a closed formula, the formula by Gardy and Nemirovski can
 * be computed more efficiently. The problem is with the pow.
 */

template<typename Tfloat>
Tfloat
fWhang(const Tfloat k, const Tfloat n, const Tfloat m) {
  const Tfloat b = n/m;  /* blocking factor */
  if(k > (n - b)) { return m; }
  if(k <= 1.0) { return k; }
  const Tfloat b_hoch_4 = b*b*b*b;
  const Tfloat m_hoch_2 = m*m;
  const Tfloat m_hoch_3 = m_hoch_2 * m;
  const Tfloat mi    = (1.0 - (1.0 / m)); /* (1 - 1/m) */
  const Tfloat mik_1 = pow(mi,k-1.0);     /* (1 - 1/m)^(k-1) */
  const Tfloat mik   = mik_1 * mi;        /* (1 - 1/m)^k     */
  


  const Tfloat lRes =  m * ( (1.0 - mik)
                       + ( (1.0 / (m_hoch_2 * b)) * (k * (k - 1.0)) / 2.0) * mik_1
                       + ( (1.5 / (m_hoch_3 * b_hoch_4)) * (k * (k - 1.0) * (2.0 * k - 1.0)) / 6.0) * mik_1 );
  return (lRes > m) ? m : lRes;
}


/* Dihr and Saharia \cite{DiSa94} give two formulas for parameters
 * n total number of records
 * m total number of pages
 * B blocking factor = \lceil n/m \rceil
 * k number of records to retrieve
 * the first formula gives an upper bound, the second gives a lower bound for Yao's formula.
 */

template<typename Tfloat> Tfloat fDihrLower(const Tfloat k, const Tfloat n, const Tfloat m) {
  const Tfloat B = n/m;
  if(k > (n - B)) return m;
  const Tfloat f = (1.0 - (k / (n - ((B - 1.0) / 2.0))));
  const Tfloat p = pow(f, B);
  const Tfloat lRes = (m * (1 - p));
  return (lRes > m) ? m : lRes;
}

template<typename Tfloat>
Tfloat
fDihrUpper(const Tfloat k, const Tfloat n, const Tfloat m) {
  const Tfloat B = n/m;
  if(k > (n - B)) return m;
  const Tfloat f = ((1.0 - (k/n)) * (1.0 - (k / (n - B + 1.0))));
  const Tfloat p = pow(f, B / 2.0);
  const Tfloat lRes = (m * (1 - p));
  return (lRes > m) ? m : lRes;
}

  
/* 
 * Bernstein et al rough estimate \cite{BeGoWoReRo81}
 */

template<typename Tfloat>
Tfloat
fBernstein(const Tfloat k, const Tfloat n, const Tfloat m) {
  if(k > (n - n/m)) return m;
  Tfloat res = 1.0;
  if(k < (m / 2.0)) {
    res = k; 
  } else
  if(k < (m * 2.0)) { 
    res = (k+m) / 2.0; 
  } else {
    res = m;
  }
  return (res > m) ? m : res;
}



/*
 *  formula of cheung 
 *  used for bags/no distinct values (see bqc)
 */

template<typename Tfloat>
Tfloat
fCheungGamma(const Tfloat k, const Tfloat n, const Tfloat m) {
  const Tfloat B = n/m;
  if(k > (n - B)) return m;
  const Tfloat p = exp(fLogBinomGamma((n - B + k - 1.0), k) - fLogBinomGamma((n + k - 1.0), k));
  return m * ( 1.0 - p );
}

template<typename Tfloat>
Tfloat
fCheungDistinct(const Tfloat N, const Tfloat k) {
  return ((N*k) / (N + k - 1));
}


/*
 * other formulas from chapter "database items, buildings blocks, access paths"
 */

template<typename Tfloat>
Tfloat
fRandomNonUniformProbDistGamma(const Tfloat N, const Tfloat n, const Tfloat k, const Tfloat x) {
  return mt::expt<Tfloat>(fLogBinomGamma(n,x) + fLogBinomGamma(N-n,k-x) - fLogBinomGamma(N,k));
}

template<typename Tfloat>
Tfloat
fSequentialBitProbDistIter(int B, int b, int j) {
  return mt::expt<Tfloat>(fLogBinomGamma(B-j-1, b-1) - fLogBinomGamma(B, b));
}

template<typename Tfloat>
Tfloat
fSequentialBitProbDistGamma(const Tfloat B, const Tfloat b, const Tfloat j) {
  return mt::expt<Tfloat>(fLogBinomGamma(B-j-1, b-1) - fLogBinomGamma(B, b));
}

/* random bitvector: average distance between two successive ones */
template<typename Tfloat>
Tfloat
fSequentialBitAvg(const Tfloat B, const Tfloat b) {
  return ((B - b) / (b + 1.0));
}

/* random bitvector: average number of bits from first bit to last one */
template<typename Tfloat>
Tfloat
fSequentialBitTot(const Tfloat B, const Tfloat b) {
  return ((B*b + b) / (b + 1.0));
}

/* random bitvector: average number of bits from first one to last one */
template<typename Tfloat>
Tfloat
fSequentialBit1Span(const Tfloat B, const Tfloat b) {
  return ((B * b - B + 2.0 * b) / (b + 1.0));
}

/* 
 *    Catalansche Zahlen
 *  fBinom(2n,n)/(n+1) = (2n)!/(n!(n+1)!) 
 */


template<typename Tfloat>
inline Tfloat
fCatalan(const Tfloat n) {
  return fBinomGamma(2 * n, n) / (n + 1);
}

template<typename Tfloat>
inline Tfloat
fCatalan2(const Tfloat n) {
  return fFactorial(2*n) / (fFactorial(n) * (fFactorial(n+1)));
}


/* wege von (0,0) nach (i,j) */
/* Ballot Zahl \cite{CaRoSc71} */
/* s. auch Liebehenschel (Diss, formel 3.7, page 45) */

template<typename Tfloat>
inline Tfloat
fLiebehenschelP(const Tfloat i, const Tfloat j) {
  return ((j+1) * mt::fBinom(i+1, ((i+j)/2) + 1)) / (i+1);
}


/* Liebehenschel Binaere Baeume (s. Diss page 103ff) */
/* q(i,j) Anzahl der Fortsetzungen eines Wegs von (i,j) nach (2n,0) unter
   beruecksichtigung der Markierungen der Segmente ist durch die formel
        q(i,j) = p(2n-i,j) moeglichkeiten |R|^{(2n-i-j)/2}
   gegeben, wobei p wie oben und moeglichkeiten in den Algorithmen verwendet wird.
   moeglichkeiten beinhaltet die anzahl der moeglichkeiten fuer die schliessenden klammern,
   deren korrespondierende oeffnende klammern schon gelesen sind.
   fuer die noch nicht gelesenen klammerpaare gibt es |R| moeglichkeiten.
*/
/* formel fuer q: siehe Liebehenschel diss (formel 3.25 auf seite 78 */

/* implementierung fuer spezialfall moeglichkeiten=1, |R|=1 */
/* siehe auch Liebehenschel Diss seite 106 */

template<typename Tfloat>
inline Tfloat
fLiebehenschelQ(const Tfloat n, const Tfloat i /* open */, const Tfloat j /* close */ ) {
  return mt::fLiebehenschelP(2*n - i, j);
}

template<typename Tfloat>
inline Tfloat
fLiebehenschelQ2(const Tfloat n, const Tfloat i /* open */, const Tfloat j /* close */ ) {
  return ((j+1) * mt::fBinom(2*n-i+1, ((2*n-i+1)/2+1))) / (2*n-i+1);
}


// x some number
// p = 2^a a Mersenne prime (not necessarily)
// return x mod p
template<typename Tuint>
inline Tuint mod_mersenne(const Tuint x, const Tuint p, const Tuint a);

template<>
inline uint32_t 
mod_mersenne(const uint32_t x, const uint32_t p, const uint32_t a) {
  // assert(x < ((((uint32_t) 1) << (2 * a)) - 1));
  const uint32_t r = ((x & p) + (x >> a));
  // if(r >= p) {
  //   r = r - p;
  // }
  return ((r < p) ? r : (r - p));
}

template<>
inline uint64_t 
mod_mersenne(const uint64_t x, const uint64_t p, const uint64_t a) {
  // assert(x < ((((uint64_t) 1) << (2 * a)) - 1));
  const uint64_t r = ((x & p) + (x >> a));
  // if(r >= p) {
  //   r = r - p;
  // }
  return ((r < p) ? r : (r - p));
}



}  // end namespace

#endif

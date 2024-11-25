#include <iostream>
#include <iomanip>
#include <random>
#include <cmath>
#include <string.h>

#include "infra/q.hh"
#include "measure.hh"
#include "matrix.hh"


// constexpr int N = 1000;
// constexpr int M = 1000;
// constexpr int K = 1000;

// for the stupid old sap compiler:
const int N = 1000;
const int M = 2000;
const int K = 1500;

// constexpr int N = 5;

Matrix A0(N,N);
Matrix A1(N,N);
Matrix B(N,N);
Matrix C(N,N);
Matrix D(N,N);

Matrix A0nk(N,K);
Matrix A1nk(N,K);
Matrix Bnm(N,M);
Matrix Bnm01(N,M);
Matrix Cmk(M,K);

double vm[M];
double vn[N];
double vk[K];

double xm1[M];
double xn1[N];
double xk1[K];

double xm2[M];
double xn2[N];
double xk2[K];

double xm3[M];
double xn3[N];
double xk3[K];

void
fInit() {
  std::mt19937 lRng;
  std::uniform_int_distribution<> lCoin;
  
  for(int i = 0; i < N; ++i) {
    for(int j = 0; j < N; ++j) {
      B(i,j) = 1.0 + sin(i*N + j);
      C(i,j) = 1.0 + cos(i*N + j);
    }
  }
  for(int i = 0; i < N; ++i) {
    for(int j = 0; j < M; ++j) {
      Bnm(i,j) = 1.0 + sin(i*N + j);
      Bnm01(i,j) = (0 == lCoin(lRng) ? 0.0L : 1.0L);
    }
  }
  for(int i = 0; i < M; ++i) {
    for(int j = 0; j < K; ++j) {
      Cmk(i,j) = 1.0 + cos(i*M + j);
    }
  }
  for(int i = 0; i < M; ++i) {
    vm[i] = 1.0 + cos(i*M);
  }
  for(int i = 0; i < N; ++i) {
    vn[i] = 1.0 + cos(i*N);
  }
  for(int i = 0; i < K; ++i) {
    vk[i] = 1.0 + cos(i*K);
  }

  
}

// quadratic matrices
bool
fEqual() {
  for(int i = 0; i < N; ++i) {
    for(int j = 0; j < N; ++j) {
      if(A0(i,j) != A1(i,j)) {
        return false;
      }
    }
  }
  return true;
}


double
fQError(const double* x, const double* y, const uint n) {
  double lRes = 0.0;
  for(uint i = 0; i < n; ++i) {
    const double lQerr = q::qerrorT1(*x, *y);
    if(lQerr > lRes) {
      lRes = lQerr;
    }
  }
  return lRes;
}


void
fMult00() {
  Measure lMeasure;
  lMeasure.start();
  A0.setToProductOf(B, C);
  lMeasure.stop();
  std::cout << "mult 00: " << lMeasure.mTotalTime() << std::endl;
}

void
fMult01() {
  Measure lMeasure;
  lMeasure.start();
  A1.setToProductOfXF(B, C);
  lMeasure.stop();
  std::cout << "mult 01: " << lMeasure.mTotalTime() << std::endl;
  if(!fEqual()) {
    std::cout << "matrices differ" << std::endl;
  }
}

// other matrices
void
fMult02() {
  Measure lMeasure;
  lMeasure.start();
  A0nk.setToProductOf(Bnm,Cmk);
  lMeasure.stop();
  std::cout << "mult 02: " << lMeasure.mTotalTime() << std::endl;
}

void
fMult03() {
  Measure lMeasure;
  lMeasure.start();
  A1nk.setToProductOfXF(Bnm,Cmk);
  lMeasure.stop();
  const double lQError = A0nk.qerror(A1nk);
  std::cout << "mult 03: " << lMeasure.mTotalTime() << std::endl;
  if(1 != lQError) {
    std::cout << "matrices differ: qerror: " << lQError << std::endl;
  }
}

void
fVecMult01() {
  double lRuntime1 = 0;
  double lRuntime2 = 0;
  {
    Measure lMeasure;
    lMeasure.start();
    Matrix::productOfAtV(xn1, Bnm, vn);
    lMeasure.stop();
    lRuntime1 = lMeasure.mTotalTime();
  }
  {
    Measure lMeasure;
    lMeasure.start();
    Matrix::productOfAtVXF(xn2, Bnm, vn);
    lMeasure.stop();
    lRuntime2 = lMeasure.mTotalTime();
  }
  const double lQError = fQError(xn1, xn2, N);
  std::cout << "Atv: " << lRuntime1 << "  " << lRuntime2 << std::endl;
  if(1 != lQError) {
    std::cout << "vectors differ: qerror: " << lQError << std::endl;
  }
}

void
fVecMult02() {
  double lRuntime1 = 0;
  double lRuntime2 = 0;
  double lRuntime3 = 0;
  {
    Measure lMeasure;
    lMeasure.start();
    Matrix::productOfAtV(xn1, Bnm01, vn);
    lMeasure.stop();
    lRuntime1 = lMeasure.mTotalTime();
  }
  {
    Measure lMeasure;
    lMeasure.start();
    Matrix::productOfAtVXF(xn2, Bnm01, vn);
    lMeasure.stop();
    lRuntime2 = lMeasure.mTotalTime();
  }
  {
    Measure lMeasure;
    lMeasure.start();
    Matrix::productOfAtVXF01(xn3, Bnm01, vn);
    lMeasure.stop();
    lRuntime3 = lMeasure.mTotalTime();
  }
  const double lQError1 = fQError(xn1, xn2, N);
  const double lQError2 = fQError(xn1, xn3, N);
  std::cout << "Atv: " << lRuntime1 << "  " 
                       << lRuntime2 << "  "
                       << lRuntime3 << "  "
                       << std::endl;
  if(1 != lQError1) {
    std::cout << "vectors differ: qerror1: " << lQError1 << std::endl;
  }
  if(1 != lQError2) {
    std::cout << "vectors differ: qerror2: " << lQError2 << std::endl;
  }
}

int
main() {
  fInit();
  fMult00();
  fMult01();
  // fMult02();
  // fMult03();

  fVecMult01();
  fVecMult02();

  return 0;
}


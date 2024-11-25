#include "FourierMotzkinBasic.hh"
#include "vector_ops.hh"

FourierMotzkinBasic::FourierMotzkinBasic(const bool aTrace)
                    : _width(0), _current(0), _v1(), _v2(), _free(), 
                      _var_eliminated(),
                      _no_vars_eliminated(0),
                      _eps(1e-10), _inconsistent(false), _trace(aTrace) {
}


/*
 *  a(i,0) x_0 + ... + a(i,n) x_n = a(i,n+1)
 *  where n = A.noCols() - 2;
 *  and
 *  x_i >= 0
 */
void
FourierMotzkinBasic::initEq(const Matrix& A) {
  _width = A.noCols();
  clear_all();
  _no_vars_eliminated = 0;

  double* v = 0;
  for(uint i = 0; i < A.noRows(); ++i) {
    v = get_free();
    for(uint j = 0; j < A.noCols(); ++j) {
      v[j] = A(i,j);
    }
    _v1.push_back(v);

    v = get_free();
    for(uint j = 0; j < A.noCols(); ++j) {
      if(0 == A(i,j)) {
        v[j] = 0;
      } else {
        v[j] = -A(i,j);
      }
    }
    _v1.push_back(v);
  }
  for(uint k = 0; k < no_vars(); ++k) {
    v = get_free();
    clear_vec(v);
    v[k] = -1;
    _v1.push_back(v);
  }

  _var_eliminated.resize(no_vars());
  for(uint k = 0; k < no_vars(); ++k) {
    _var_eliminated[k] = false;
  }

  if(trace()) {
    std::cout << "  after initEq:" << std::endl;
    print(std::cout, 3) << std::endl;
  }
}

/*
 *  a(i,0) x_0 + ... + a(i,n) x_n = a(i,n+1)
 *  where n = A.noCols() - 2;
 *  and
 *  x_i >= 0
 */

void
FourierMotzkinBasic::initLeq(const Matrix& A, const bool aWithXgeq0) {
  _width = A.noCols();
  clear_all();
  _no_vars_eliminated = 0;

  double* v = 0;
  for(uint i = 0; i < A.noRows(); ++i) {
    v = get_free();
    for(uint j = 0; j < A.noCols(); ++j) {
      v[j] = A(i,j);
    }
    _v1.push_back(v);
  }

  // add -x_i <= 0 constraints, if requested
  if(aWithXgeq0) {
    for(uint k = 0; k < no_vars(); ++k) {
      v = get_free();
      clear_vec(v);
      v[k] = -1;
      _v1.push_back(v);
    }
  }

  _var_eliminated.resize(no_vars());
  for(uint k = 0; k < no_vars(); ++k) {
    _var_eliminated[k] = false;
  }

  if(trace()) {
    std::cout << "  after initLeq:" << std::endl;
    print(std::cout, 3) << std::endl;
  }
}



bool
FourierMotzkinBasic::run_elimination() {

  if(trace()) {
    std::cout << "run_elimination()" << std::endl;
  }
  uint lNoNP = 0;
  do {
    if(0 >= no_vars_left()) { break; }
    const uint lMinK = min_neg_pos_col(&lNoNP);
    if(trace()) {
      std::cout << "  min k = " << lMinK << ", |N x P| = " << lNoNP << std::endl;
    }
    if(no_vars() <= lMinK) { break; }
    if(!is_consistent()) { break; }
    eliminate_var(lMinK);
    if(0 >= no_vars_left()) { break; }
    if(trace()) {
      std::cout << "  after elim_var(" << lMinK << "):" << std::endl;
      print(std::cout, 3) << std::endl;
    }
  } while(true);

  if(trace()) {
    std::cout << "  consistent         : " << is_consistent() << std::endl;
    std::cout << "  vars eliminated    : " << no_vars_eliminated() << std::endl;
    std::cout << "  vars left          : " << no_vars_left() << std::endl;
  }

  return is_consistent();
}

bool
FourierMotzkinBasic::run_elimination(const uint_vt& aVarVec) {

  if(trace()) {
    std::cout << "run_elimination(" << aVarVec << "):" << std::endl;
  }
  for(const uint lVar : aVarVec) {
    if(no_vars() <= lVar) { break; }
    if(0 >= no_vars_left()) { break; }
    if(!is_consistent()) { break; }
    eliminate_var(lVar);
    if(trace()) {
      std::cout << "  after elim_var(" << lVar << "):" << std::endl;
      print(std::cout, 3) << std::endl;
    }
    if(0 >= no_vars_left()) { break; }
  }

  if(trace()) {
    std::cout << "  consistent         : " << is_consistent() << std::endl;
    std::cout << "  vars eliminated    : " << no_vars_eliminated() << std::endl;
    std::cout << "  vars left          : " << no_vars_left() << std::endl;
  }

  return is_consistent();
}


void
FourierMotzkinBasic::clear_all() {
  for(size_t i = 0; i < _v1.size(); ++i) {
    _free.push_back(_v1[i]);
  }
  _v1.clear();

  for(size_t i = 0; i < _v2.size(); ++i) {
    _free.push_back(_v2[i]);
  }
  _v2.clear();
}

void
FourierMotzkinBasic::clear_other() {
  double_pvt& X = other_nc();
  for(size_t i = 0; i < X.size(); ++i) {
    _free.push_back(X[i]);
  }
  X.clear();
}

size_t
FourierMotzkinBasic::total_memory() const {
  return _v1.size() + _v2.size() + _free.size();
}

double*
FourierMotzkinBasic::get_free() {
  double* lRes = 0;
  if(0 < _free.size()) {
    lRes = _free[_free.size() - 1];
    _free.resize(_free.size() - 1);
  } else {
    lRes = new double[width()];
  }
  return lRes;
}

void
FourierMotzkinBasic::free_all() {
  clear_all();
  for(size_t i = 0; i < _free.size(); ++i) {
    delete[] _free[i];
  }
  _free.clear();
}

const FourierMotzkinBasic::double_pvt& 
FourierMotzkinBasic::current() const {
  if(0 == _current) {
    return _v1;
  }
  return _v2;
}

FourierMotzkinBasic::double_pvt& 
FourierMotzkinBasic::current_nc() {
  if(0 == _current) {
    return _v1;
  }
  return _v2;
}

const FourierMotzkinBasic::double_pvt&
FourierMotzkinBasic::other() const {
  if(0 == _current) {
    return _v2;
  }
  return _v1;
}

FourierMotzkinBasic::double_pvt&
FourierMotzkinBasic::other_nc() {
  if(0 == _current) {
    return _v2;
  }
  return _v1;
}

void
FourierMotzkinBasic::clear_vec(double* v) const {
  for(uint j = 0; j < width(); ++j) {
    v[j] = 0;
  }
}

bool
FourierMotzkinBasic::is_zero(double* v) const {
  bool lRes = true;
  for(uint k = 0; k < no_vars(); ++k) {
    if(0 != v[k]) {
      lRes = false;
      break;
    }
  }
  return lRes;
}

uint
FourierMotzkinBasic::count_neg_pos_col(uint& aNoNeg, uint& aNoPos, const uint k) const {
  const double_pvt& X = current();
  uint lNoNeg = 0;
  uint lNoPos = 0;
  for(size_t i = 0; i < X.size(); ++i) {
    lNoNeg += (0 > X[i][k]);
    lNoPos += (0 < X[i][k]);
  }
  aNoNeg = lNoNeg; 
  aNoPos = lNoPos; 
  return (lNoNeg * lNoPos);
}

uint
FourierMotzkinBasic::min_neg_pos_col(uint* aNpOut) const {
  uint lMinK = width() + 77;
  uint lMinV = std::numeric_limits<uint>::max();
  uint lCurrV = 0;
  uint lNoNeg = 0;
  uint lNoPos = 0;
  for(uint k = 0; k < no_vars(); ++k) {
    lCurrV = count_neg_pos_col(lNoNeg, lNoPos, k);
    if((0 < lCurrV) && (lCurrV < lMinV)) {
      lMinK = k;
      lMinV = lCurrV;
    }
  }
  if(0 != aNpOut) {
    (*aNpOut) = lMinV;
  }
  return lMinK;
}

void
FourierMotzkinBasic::sub_step(double* aOutV, const double* aNegV, const double* aPosV, const uint k) const {
  assert(0 > aNegV[k]);
  assert(0 < aPosV[k]);
  double x;
  for(uint j = 0; j < width(); ++j) {
    x = aPosV[k] * aNegV[j] - aNegV[k] * aPosV[j];
    // if(0 == x)
    if(std::fabs(x) <= eps()) {
     x = 0;
    }
    aOutV[j] = x;
  }
}

void
FourierMotzkinBasic::get_NPZ(uint_vt& N, uint_vt& P, uint_vt& Z, const uint k) const {
  N.clear();
  P.clear();
  Z.clear();
  const double_pvt& X = current();
  for(size_t i = 0; i < X.size(); ++i) {
    if(0 > X[i][k]) { N.push_back(i); } else
    if(0 < X[i][k]) { P.push_back(i); } else
                    { Z.push_back(i); } 
  }
}

void
FourierMotzkinBasic::eliminate_var(const uint k) {
  assert(k < no_vars());
  assert(!var_eliminated(k));

  clear_other();

  if(trace()) {
    std::cout << "  eliminate_var(" << k << ')' << std::endl;
    std::cout << "    #vars left = " << no_vars_left() << std::endl;
    std::cout << "    #ieqs left = " << current().size() << std::endl;
  }

  if(2 == no_vars_left()) {
    eliminate_but_last_var(k);
    return;
  }

  const double_pvt& X = current();
        double_pvt& Y = other_nc();
  uint_vt N; // indices of <0 entries
  uint_vt P; // indices of >0 entries
  uint_vt Z; // indices of =0 entries
  get_NPZ(N, P, Z, k);
  if(trace()) {
    std::cout << "    k = " << k << std::endl;
    std::cout << "    N = " << N << std::endl;
    std::cout << "    P = " << P << std::endl;
    std::cout << "    Z = " << Z << std::endl;
  }

  // just for the case of 3 variables left;
  uint k2 = 0, k3 = 0;
  if(3 == no_vars_left()) { get_other_var_3(k, k2, k3); }


  double* v = 0;

  for(uint i = 0; i < N.size(); ++i) {
    for(uint j = 0; j < P.size(); ++j) {
      v = get_free();
      sub_step(v, X[N[i]], X[P[j]], k);
      if(trace()) {
        // std::cout << "    v[" << i << ',' << j << "] = "; print_vec(std::cout, v, 3)<< std::endl;
      }
      if(!is_zero(v)) {
        if(!is_redundant_row(v)) {
          if(is_consistent_row(v)) {
            if(k2 == k3) {
              // not three variables left at beginning of this call
              Y.push_back(v);
            } else {
              // three variables left at beginning of this call, in v, only 2
              insert_2(Y, v, k2, k3);
            }
            continue;
          } else {
            _inconsistent = true;
            if(trace()) {
              std::cout << "      inconsistent row detected." << std::endl;
            }
          }
        } else {
          // std::cout << "      redundant row detected." << std::endl;
        }
      } else {
        if(0 > v[no_vars()]) {
          _inconsistent = true;
          if(trace()) {
            std::cout << "      inconsistent const row detected." << std::endl;
          }
        }
      }
      _free.push_back(v);
    }
  }

  // copy Z rows
  for(uint i = 0; i < Z.size(); ++i) {
    Y.push_back(X[Z[i]]);
  }

  // move N, P rows to free
  for(uint i = 0; i < N.size(); ++i) { _free.push_back(X[N[i]]); }
  for(uint i = 0; i < P.size(); ++i) { _free.push_back(X[P[i]]); }
  current_nc().clear();
  swap_current();
  ++_no_vars_eliminated;
  _var_eliminated[k] = true;
}

void
FourierMotzkinBasic::eliminate_but_last_var(const uint k) {
  assert(2 == no_vars_left());

  double lLb = -std::numeric_limits<double>::max();
  double lUb = +std::numeric_limits<double>::max();

  const uint k2 = get_other_var(k);

  if(trace()) {
    std::cout << "  eliminate_but_last_var(" << k << ',' << k2 << ')' << std::endl;
  }
  const double_pvt& X = current();
  uint_vt N; // indices of <0 entries
  uint_vt P; // indices of >0 entries
  uint_vt Z; // indices of =0 entries
  get_NPZ(N, P, Z, k);
  if(trace()) {
    std::cout << "    k = " << k << std::endl;
    std::cout << "    N = " << N << std::endl;
    std::cout << "    P = " << P << std::endl;
    std::cout << "    Z = " << Z << std::endl;
  }
  double* v = get_free();

  double c = 0; // coefficient of variable k2
  double b = 0; // b
  double q = 0; // b/c

  for(uint i = 0; i < Z.size(); ++i) {
    c = v[k2];
    b = v[no_vars()];
    if(0 == c) {
      if(0 > b) {
        _inconsistent = true;
        break;
      }
    } else {
      q = b/c;
      if(0 < c) {
        if(q < lUb) {
          lUb = q;
        }
      } else {
        // 0 > c
        if(q > lLb) {
          lLb = q;
        }
      }
    }
  }

  if(trace()) {
    std::cout << "    after processing Z:" << std::endl;
    std::cout << "      lb(" << k2 << ") = " << std::setw(8) << lLb << std::endl;
    std::cout << "      ub(" << k2 << ") = " << std::setw(8) << lUb << std::endl;
  }

  for(uint i = 0; (i < N.size()) && !_inconsistent; ++i) {
    for(uint j = 0; j < P.size(); ++j) {
      sub_step(v, X[N[i]], X[P[j]], k);
      c = v[k2];
      b = v[no_vars()];
      if(0 == c) {
        if(0 > b) {
          _inconsistent = true;
          break;
        }
      } else {
        q = b/c;
        if(0 < c) {
          if(q < lUb) {
            lUb = q;
          }
        } else {
          // 0 > c
          if(q > lLb) {
            lLb = q;
          }
        }
      }
    }
  }

  if(trace()) {
    std::cout << "    after processing N x P:" << std::endl;
    std::cout << "      lb(" << k2 << ") = " << std::setw(8) << lLb << std::endl;
    std::cout << "      ub(" << k2 << ") = " << std::setw(8) << lUb << std::endl;
  }

  if(lLb > lUb) {
    _inconsistent = true;
  }

  if(lUb < 0) {
    _inconsistent = true;
  }


  _var_eliminated[k]  = true;
  _var_eliminated[k2] = true;
  _no_vars_eliminated += 2;
  _free.push_back(v);
}

int  
FourierMotzkinBasic::is_only_pos_or_neg_var_row(const double* v) const {
  int lRc = 77;
  uint lNoPos = 0;
  uint lNoNeg = 0;
  for(uint i = 0; i < no_vars(); ++i) {
    lNoNeg += (0 > v[i]);
    lNoPos += (0 < v[i]);
  }
  if((0 == lNoPos) & (0 <  lNoNeg)) { lRc = -1; } else
  if((0 == lNoNeg) & (0 <  lNoPos)) { lRc = +1; } else
  if((0 == lNoPos) & (0 == lNoNeg)) { lRc = 0; }

  return lRc;
}


// meant to be called if there are exactly two variables left
uint
FourierMotzkinBasic::get_other_var(const uint k) const {
  uint lRes = -1;
  for(uint i = 0; i < no_vars(); ++i) {
    if((i != k) & (!var_eliminated(i))) {
      lRes = i;
      break;
    }
  }
  return lRes;
}

// meant to be calls if there are only three vars left
void
FourierMotzkinBasic::get_other_var_3(const uint k1, uint& k2, uint& k3) const {
  int x = -1;
  int y = -1;
  for(uint i = 0; i < no_vars(); ++i) {
    if((i != k1) & (!var_eliminated(i))) {
      if(0 > x) {
        x = i;
      } else {
        y = i;
        break;
      }
    }
  }
  k2 = x;
  k3 = y;
}

// returns #zero
uint
FourierMotzkinBasic::count_neg_pos_row(uint& aNoNeg, uint& aNoPos, const double* v) const {
  uint lNoZero = 0, lNoNeg = 0, lNoPos = 0;
  for(uint i = 0; i < no_vars(); ++i) {
    lNoZero += (0 == v[i]);
    lNoNeg  += (0 >  v[i]);
    lNoPos  += (0 <  v[i]);
  }
  aNoNeg = lNoNeg;
  aNoPos = lNoPos;
  return lNoZero;
}

bool
FourierMotzkinBasic::is_redundant_row(const double* v) const {
  uint lNoPos = 0, lNoNeg = 0;
  count_neg_pos_row(lNoNeg, lNoPos, v);
  return ((0 == lNoPos) && (0 <= v[no_vars()]));
}

bool 
FourierMotzkinBasic::is_consistent_row(const double* v) const {
  const int lRc = is_only_pos_or_neg_var_row(v);
  bool lRes = true;
  const double b = v[no_vars()];
  switch(lRc) {
    case +1: lRes = (0 <= b); break;
    case  0: lRes = (0 <= b); break;
    default: break;
  }
  return lRes;
}

bool 
FourierMotzkinBasic::all_rows_consistent() const {
  bool lRes = true;
  const uint lNoRows = current().size();
  for(uint i = 0; i < lNoRows; ++i) {
    if(!is_consistent_row(get_row(i))) {
      lRes = false;
      if(trace()) {
        std::cout << "   inconsistent row: " << i << " row = ";
        print_row(std::cout, i, 3) << std::endl;
      } else {
        break;
      }
    }
  }
  return lRes;
}


void
FourierMotzkinBasic::insert_2(double_pvt& Y, double* v, const uint k1, const uint k2) {

  // Y.push_back(v); // tuts
  // return;        

  // subsequent stuff: tuts nicht!


  // remember to free v if not inserted, free those removed from r
  uint lNoRemain     = 0; // remaining v' in Y
  uint lNoEliminated = 0; // eliminated v' in Y
  bool lSubsumed = false; // whether v is subsumed by some v' in Y
  int  lRc       = 0;
  double* v2 = 0;

  const bool lTrace = trace();

  if(lTrace) {
    std::cout << "      insert_2: " << k1 << ',' << k2 << ',' << Y.size() << std::endl;
    std::cout << "        v1 = "; print_vec(std::cout, v, 0) << std::endl;
  }
  uint i = 0;
  for(i = 0; i < Y.size(); ++i) {
    v2 = Y[i];
    if(lTrace) {
      std::cout << "        v2 = "; print_vec(std::cout, v2, 0) << std::endl;
    }
    lRc = check_redundancy_2(v, v2, k1, k2);
    if(-1 == lRc) { 
      lSubsumed = true; 
      if(lTrace) {
        std::cout << "          subsumed!" << std::endl;
      }
      break;
    } else {
      if(+1 == lRc) {
        // v2 is subsumed
        // eliminated v2
        _free.push_back(v2);
        if(lTrace) {
          std::cout << "          eliminated!" << std::endl;
        }
        ++lNoEliminated;
      } else {
        Y[lNoRemain++] = v2;
      }
    }
  }
  // after loop:
  // 0 <= k <= lNoRemain     : filled with non-eliminated
  // lNoRemain < k < i       : voids
  // i < k                   : valid entries

  if(0 < lNoEliminated) {
    for(; i < Y.size(); ++i) {
      Y[lNoRemain++] = Y[i];
    }
    Y.resize(lNoRemain);
  }

  if(lSubsumed) {
    _free.push_back(v);
  } else {
    Y.push_back(v);
  }

  if((0 < lNoEliminated) && lTrace) {
    std::cout << "        subsumed = " << lSubsumed
              << ", no elim = " << lNoEliminated
              << std::endl;
  }
}


int
FourierMotzkinBasic::check_redundancy_2(const double* v1, const double* v2, const uint k1, const uint k2) const {
  if((v1[k1] <= v2[k1]) && (v1[k2] <= v2[k2]) && (v1[no_vars()] >= v2[no_vars()])) {
    return -1; // v1 is redundant;
  }
  if((v1[k1] >= v2[k1]) && (v1[k2] >= v2[k2]) && (v1[no_vars()] <= v2[no_vars()])) {
    return +1; // v2 is redundant;
  }

  // the above could be improved by something like
  // (v2[k1] * v1[no_vars()] <= v1[k1] * v2[no_vars()]) 

  return 0; // no redundancy detected by this simple check
}


std::ostream&
FourierMotzkinBasic::print_vec(std::ostream& os, const double* v, const int aWidth) const {
  for(uint i = 0; i < width(); ++i) {
    os << ' ' << v[i];
  }
  return os;
}

std::ostream&
FourierMotzkinBasic::print_row(std::ostream& os, const uint i, const int aWidth) const {
  return print_vec(os, get_row(i), aWidth);
}


std::ostream&
FourierMotzkinBasic::print(std::ostream& os, const int aWidth) const {
  const double_pvt& X = current();
  for(size_t i = 0; i < X.size(); ++i) {
    double* v = X[i];
    os << "  ";
    for(uint j = 0; j < width(); ++j) {
      os << ' ' << std::setw(aWidth) << v[j];
    }
    if(!is_consistent_row(v)) { os << " X"; } else
    if(is_redundant_row(v)) { os << " *"; }
    os << std::endl;
  }
  uint_vt lNoPos(no_vars());
  uint_vt lNoNeg(no_vars());
  for(size_t i = 0; i < no_vars(); ++i) {
    count_neg_pos_col(lNoNeg[i], lNoPos[i], i);
  }
  os << "  ";
  for(uint i = 0; i < no_vars(); ++i) {
    os << ' ' << std::setw(aWidth) << lNoPos[i];
  }
  os << std::endl;
  os << "  ";
  for(uint i = 0; i < no_vars(); ++i) {
    os << ' ' << std::setw(aWidth) << lNoNeg[i];
  }
  os << std::endl;
  return os;
}




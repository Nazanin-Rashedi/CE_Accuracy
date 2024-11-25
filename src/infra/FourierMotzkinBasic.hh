#ifndef FOURIER_MOTZKIN_BASIC_HH
#define FOURIER_MOTZKIN_BASIC_HH
#pragma once

#include "infra/glob_infra_standard_includes.hh"
#include "matrix.hh"

class FourierMotzkinBasic {
  public:
    typedef std::vector<double*> double_pvt;
  public:
    FourierMotzkinBasic(const bool aTrace);
  public:
    void initEq(const Matrix& A);
    void initLeq(const Matrix& A, const bool aWithXgeq1);
    bool run_elimination();
    bool run_elimination(const uint_vt& aVarVec);
  public:
    inline uint   width() const { return _width; }
    inline uint   no_vars() const { return (width() - 1); }
    inline uint   no_vars_eliminated() const { return _no_vars_eliminated; }
    inline uint   no_vars_left() const { return no_vars() - no_vars_eliminated(); }
    inline double eps() const { return _eps; }
    inline bool   is_consistent() const { return (!_inconsistent); }
    inline bool   trace() const { return _trace; }
    inline bool   var_eliminated(const uint k) const { return _var_eliminated[k]; }
    inline const bool_vt& var_eliminated() const { return _var_eliminated; }
  private:
    double* get_free();
    void    free_all();
    void    clear_other();
    void    clear_all();
  public:
    size_t  total_memory() const; // in number of rows
  private:
    const double_pvt& current() const;
          double_pvt& current_nc();
    const double_pvt& other() const;
          double_pvt& other_nc();
    inline       void     swap_current() { _current ^= 0x1; }
    inline const double*  get_row(const uint i) const { return current()[i]; }
  public:
    void clear_vec(double*) const;
    bool is_zero(double*) const;
    void sub_step(double* aOutV, const double* aNegV, const double* aPosV, const uint k) const;
    void eliminate_var(const uint k);
    void eliminate_but_last_var(const uint k);
    int  is_only_pos_or_neg_var_row(const double* v) const;
    uint count_neg_pos_row(uint& aNoNeg, uint& aNoPos, const double* v) const; // returns #zero
    bool is_redundant_row(const double* v) const;
    bool is_consistent_row(const double* v) const; // assumes all x_k >= 0
    bool all_rows_consistent() const; //  assumes all x_k >= 0
    int  check_redundancy_2(const double* v1, const double* v2, const uint k1, const uint k2) const;
  private:
    // insert v if not redundant with any other v' in Y
    // only two variable coefficients k1 and k2 maybe non-zero
    void insert_2(double_pvt& Y, double* v, const uint k1, const uint k2);
  public:
    uint get_other_var(const uint k) const;
    void get_other_var_3(const uint k1, uint& k2, uint& k3) const;
    uint count_neg_pos_col(uint& aNoNeg, uint& aNoPos, const uint k) const; // returns aNoNeg * aNoPos
    uint min_neg_pos_col(uint* aNpOut = 0) const; // returns column k with count_neg_pos(..,k) = min but > 0
    void get_NPZ(uint_vt& N, uint_vt& P, uint_vt& Z, const uint k) const;
  public:
    std::ostream& print_vec(std::ostream& os, const double* v, const int aWidth) const;
    std::ostream& print_row(std::ostream& os, const uint i, const int aWidth) const;
    std::ostream& print(std::ostream& os, const int aWidth) const;
  private:
    uint        _width;
    uint        _current;
    double_pvt  _v1;
    double_pvt  _v2;
    double_pvt  _free;
    bool_vt     _var_eliminated;
    uint        _no_vars_eliminated;
    double      _eps; // to detect zeros
    bool        _inconsistent; // flag detected inconsistencies here
    bool        _trace;
};


#endif

#ifndef INFRA_DOUBLE_VEC_OPS_HH
#define INFRA_DOUBLE_VEC_OPS_HH


#include "infra/glob_infra_standard_includes.hh"
#include <cmath>
#include "infra/float_ops.hh"

// return x = y ungefaehr
bool   is_equal(const double_vt& x, const double_vt& y, const double eps);

// return \max_i |x_i - y_i|
double max_abs_distance(const double_vt& x, const double_vt& y);

// return \sum x_i
double sum_elements(const double_vt& x);

// x += y
void   add(double_vt& x, const double_vt& y);

// x -= y
void   sub(double_vt& x, const double_vt& y);

// x = -x
void  invert_sign(double_vt& x);

// subtract from each element x[i] > 0 (aSum - sum_{x[i]>0} x_i)/ #>0(x) 
void scale_sum_gt_zero(double_vt& x, const double aSum);

// x *= 1/\sum x_I
void  scale_multiplicatively(double_vt& x);

// x = a + b
void sum(double_vt& x, const double_vt& a, const double_vt& b);

// x = a - b
void diff(double_vt& x, const double_vt& a, const double_vt& b);

//
bool is_zero(const double_vt& x, const double aEps);

// 0 <= x[i]
bool is_positive(const double_vt& x);  

// 0 >= x[i]
bool is_negative(const double_vt& x);  

// x[i] >= c
bool is_geq_c(const double_vt& x, const double c);

// |x[i]| <= aEps
bool fabs_less(const double_vt& x, const double aEps);

// |x[i] - y[i]| <= aEps
bool vec_equal(const double_vt& x, const double_vt& y, const double aEps); 

// max_i |x[i] - y[i]|
double max_fabs_diff(const double_vt& x, const double_vt& y);

// min_i x[i] - y[i]
double min_diff(const double_vt& x, const double_vt& y);

// sum_i |x_i|
double sum_fabs(const double_vt& r); 

// max_i |x_i|
double max_fabs(const double_vt& r); 

// min_i x_i
double vec_min(const double_vt& x);

// max_i x_i
double vec_max(const double_vt& x);

// smallest i: s.t. x_i = min_i x_i
size_t vec_min_idx_min(const double_vt& x);

// smallest i: s.t. x_i = max_i x_i
size_t vec_max_idx_min(const double_vt& x);

// largest i: s.t. x_i = min_i x_i
size_t vec_min_idx_max(const double_vt& x);

// largest i: s.t. x_i = max_i x_i
size_t vec_max_idx_max(const double_vt& x);

// elementwise comparison
bool vec_leq(const double_vt& x, const double_vt& y);
bool vec_geq(const double_vt& x, const double_vt& y);


// sum_i |x_i|
double norm_1(const double_vt& x);

// sum_i (x_i x_i) (ohne sqrt!)
double norm_2(const double_vt& r);

// x = x + (a - b)
void addsub(double_vt& x, const double_vt& a, const double_vt& b);

// count number of elements greater/less than zero
uint countSubZero(const double_vt& x);
uint countEqZero(const double_vt& x, const double aEps);
uint countGtZero(const double_vt& x);
void setZeroSubZero(double_vt& x);

double vec_sum_gt_zero(const double_vt& x, const double lToSub);
void   vec_cut_neg(double_vt& x, const double lToSub);
uint   vec_count_gt_zero(const double_vt& x, const double lToSub);
void   vec_set_zero(double_vt& x);
void   set_zero(double_vt& x, const double aEps = 1e-15);
void   vec_set_to(double_vt& x, const double aVal);
void   vec_add_const(double_vt& x, const double_vt& y, const double c);
void   vec_add_const(double_vt& x, const double c);

void   vec_x_add_cy(double_vt& x, const double_vt& y, const double c); // x = x - cy
void   vec_x_sub_cy(double_vt& x, const double_vt& y, const double c); // x = x - cy

void   get_unit_vector(double_vt& v, const uint aDim, const uint aSize);
double dotprod(const double_vt& v1, const double_vt& v2);

#endif

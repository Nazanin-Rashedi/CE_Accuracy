#ifndef INFRA_FLOAT_OPS_HH
#define INFRA_FLOAT_OPS_HH


template<typename Tfloat>
inline
bool
is_equal(const Tfloat x, const Tfloat y, const Tfloat eps) {
  return ((x <= (y + eps)) && (y <= (x + eps)));
}

#endif

#ifndef INFRA_VARIADIC_HH
#define INFRA_VARIADIC_HH

/*
 *  access n-th type of a variadic template parameter
 */

template<unsigned int N, typename... Ts> using NthTypeOf = typename std::tuple_element<N, std::tuple<Ts...>>::type;

/*
 *  check whether index is in range
 */

template<unsigned int I, typename... Ts>
constexpr bool 
index_ok() {
  return (I < sizeof...(Ts));
}

#endif

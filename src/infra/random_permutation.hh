#ifndef INFRA_RANDOM_PERMUTATION_HH
#define INFRA_RANDOM_PERMUTATION_HH

#include <vector>
#include <random>


// generates a random permutation of given container (array, vector)
template<class Trandgen, class Tcontainer, typename Tuint>
void
random_permutation(Trandgen& aRng, Tcontainer& x, Tuint  aSize) {
  for(Tuint i = aSize - 1; i > 0; --i) {
    std::swap(x[i], x[aRng() % i]);
  }
}


// calls above but assumes Tcontainer has member size
template<class Trandgen, class Tcontainer>
void
random_permutation(Trandgen& aRng, Tcontainer& x) {
  random_permutation<Trandgen, Tcontainer, typename Tcontainer::value_type>(aRng, x, x.size());
}


// generates a vector containing a random
// permutation of the numbers aBegin, .,,, aEnd - 1
template<class Trandgen>
void
random_permutation(Trandgen& aRng,
		   std::vector<unsigned int>& x, 
		   unsigned int aBegin,
		   unsigned int aEnd) {
  x.resize(aEnd - aBegin);
  for(unsigned int i = aBegin; i < aEnd; ++i) {
    x[i] = i;
  }
  random_permutation(aRng, x);
}


#endif

#ifndef INFRA_RANK_PERMUTATION_HH
#define INFRA_RANK_PERMUTATION_HH

/* ranking/unranking of permutations         */
/* with n elements ranging from 0,...,n-1    */



/* implements Myrvold, Ruskey algorithms     */
/* to rank/unrank a permutation              */
/* implements rank1 and unrank1 of the paper */
/* class Tuint is used for a rank and for    */
/* the number of permutations                */
/* permutations are unsigned int arrays      */
/* containing the number 0,...,n-1           */
/* if n is the number of elements considered */

template<class Tuint>
void
ftUnrankPermutation(const Tuint r,         // rank
                    const unsigned int n,  // n
                    unsigned int* pi       // resulting unranked permutation
                   );


/* piInv = \pi^{-1} initialized by ftRankPermutation */
/* the argument pi must contain a permutation of the number 0 - n-1 */
/* must have been initialized when calling ftRankPermutationSub */
/* recursive variant */
/* WARNING: ftRankPermutation modifies its arguments pi and piInv !!! */

template<class Tuint>
Tuint
ftRankPermutation(const unsigned int n, 
                  const unsigned int* pi, 
                        unsigned int* piInv);


// helpers and implementations:


/*
 *  Unranking
 */


template<class Tuint>
void
ftUnrankPermutationSub(Tuint r, const unsigned int n, unsigned int* pi) {
  for(unsigned int i = n; i > 0; --i) {
    unsigned int x = pi[i-1];
    pi[i-1] = pi[r % i];
    pi[r % i] = x;
    r = r / i;
  }
}

template<class Tuint>
void
ftUnrankPermutation(const Tuint r, const unsigned int n, unsigned int* pi) {
  // initialize pi to the identity
  for(unsigned int i = 0; i < n; ++i) {
    pi[i] = i;
  }
  ftUnrankPermutationSub<Tuint>(r, n, pi);
}



/*
 *  Ranking
 */

template<class Tuint>
Tuint
ftRankPermutationSub(const unsigned int n, unsigned int* pi, unsigned int* piInv) {
  Tuint lRes = 0;
  Tuint f = 1;
  // std::cout << " # ";
  for(unsigned int i = n - 1; i > 0; --i) {
    const unsigned int s = pi[i];

    const unsigned int x = pi[i]; // swap
    pi[n-1] = pi[piInv[i]];
    pi[piInv[i]] = x;

    const unsigned int y = piInv[s]; // swap
    piInv[s] = piInv[i];
    piInv[i] = y;

    lRes += (s * f);
    f *= (i+1);
  }
  return lRes;
}

template<class Tuint>
Tuint
ftRankPermutation(const unsigned int n, unsigned int* pi, unsigned int* piInv) {
  // initialize inverse permutation
  for(unsigned int i = 0; i < n; ++i) {
    piInv[pi[i]] = i;
  }
  return ftRankPermutationSub<Tuint>(n, pi, piInv);
}

#endif


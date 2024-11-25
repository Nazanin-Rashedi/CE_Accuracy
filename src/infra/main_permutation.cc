#include "permutation.hh"
#include "ggt.hh"
#include "bit_subsets.hh"

// using namespace perm;


inline bool
implies(const bool a, const bool b) {
  return (!a || b);
}

std::string
to_str(const uint aBv, const uint z) {
  std::string lRes(z, '0');
  for(uint i = 0; i < z; ++i) {
    if(0x1 == (0x1 & (aBv >> i))) {
      lRes[z - 1 - i] = '1';
    }
  }
  return lRes;
}

void
handle_permutation(const perm_t& aPerm) {
  PermutationAnalyzer lAna(aPerm);
  std::cout << aPerm << "  " << lAna << std::endl;
  aPerm.print_props(std::cout, 4);
  const bool lHasSquareRoot1 = aPerm.has_square_root_slow();
  const bool lHasSquareRoot2 = aPerm.has_square_root();

  std::cout << "  has_square_root: " << lHasSquareRoot1 << lHasSquareRoot2 << std::endl;
  assert(lHasSquareRoot1 == lHasSquareRoot2);

  lAna.print(std::cout, 2);

  perm_t lInverse;
  lInverse.set_to_inverse_of(aPerm);
  assert(lInverse.is_inverse_of(aPerm));
  assert(aPerm.is_inverse_of(lInverse));
  // std::cout << "inv(" << aPerm << ") = " << lInverse << std::endl;

  perm_t lReverse;
  lReverse.set_to_reverse_of(aPerm);
  assert(lReverse.is_reverse_of(aPerm));
  assert(aPerm.is_reverse_of(lReverse));
  // std::cout << "rev(" << aPerm << ") = " << lReverse << std::endl;

  perm_t lCmpl(perm_t::kComplement, aPerm);
  // std::cout << "cmpl(" << aPerm << ") = " << lCmpl << std::endl;
  assert(lCmpl.is_complement_of(aPerm));
  assert(aPerm.is_complement_of(lCmpl));
  
  // uint_vt lVec({1,2,3,4});
  // std::cout << lVec << std::endl;
}

void
handle_perm_pair(const perm_t& aPerm1, const perm_t& aPerm2) {
  assert(aPerm1.size() == aPerm2.size());
  std::cout << '[' << aPerm1 << "]  [" << aPerm2 << "]:" << std::endl;
  std::cout << "  equal     : " << aPerm1.is_equal_to(aPerm2) 
                                << aPerm1.is_equal_to(aPerm2) << std::endl
            << "  inverse   : " << aPerm1.is_inverse_of(aPerm2) 
                                << aPerm2.is_inverse_of(aPerm1) << std::endl
            << "  complement: " << aPerm1.is_complement_of(aPerm2)
                                << aPerm2.is_complement_of(aPerm1) << std::endl
            << "  commute   : " << aPerm1.commute(aPerm2) 
                                << aPerm2.commute(aPerm1) << std::endl
            << "  conjugate : " << aPerm1.is_conjugate_of_slow(aPerm2) 
                                << aPerm2.is_conjugate_of_slow(aPerm1) << std::endl
            << "  inv_comm  : " << aPerm1.inverse_commute(aPerm2)
                                << aPerm2.inverse_commute(aPerm1) << std::endl
            ;
  perm_t lSumDirect(aPerm1, aPerm2, perm_t::kSumDirect);
  perm_t lSumSchief(aPerm1, aPerm2, perm_t::kSumSchief);
  std::cout << "  sum_direct: " << lSumDirect << std::endl;
  std::cout << "  sum_schief: " << lSumSchief << std::endl;
}

/*
 *  simple test print properties of a fixed permutation
 */

void
test0() {
  std::cout << "=== test0 ===" << std::endl;
  perm_t lPerm(uint_vt({1, 4, 3, 2, 0}));
  handle_permutation(lPerm);
}

/*
 *  print properties of all permutations in Perm(z)
 */

void
test1(const uint aZend) {
  std::cout << "=== test1 ===" << std::endl;
  for(uint z = 2; z < aZend; ++z) {
    const uint64_t N = factorial64(z);
    perm_t lPerm(z);
    for(uint lRank = 0; lRank < N; ++lRank) {
      ftUnrankPermutation(lRank, z, lPerm.data());
      handle_permutation(lPerm);
    }
  }
}

/*
 * print properties of all pairs of permutations in Perm(z)
 */

void
test2(const uint z) {
  std::cout << "=== test2 ===" << std::endl;
  const uint64_t N = factorial64(z);
  perm_t lPerm1(z);
  perm_t lPerm2(z);
  for(uint lRank1 = 0; lRank1 < N; ++lRank1) {
    ftUnrankPermutation(lRank1, z, lPerm1.data());
    for(uint lRank2 = 0; lRank2 < N; ++lRank2) {
      ftUnrankPermutation(lRank2, z, lPerm2.data());
      handle_perm_pair(lPerm1, lPerm2); 
    }
  }
}

/*
 * print for different z the number of (non-) separable permutations
 */

uint64_t
count_separable(const uint z) {
  const uint64_t N = factorial64(z);
  perm_t lPerm(z);
  uint64_t lCount = 0;
  for(uint lRank = 0; lRank < N; ++lRank) {
    ftUnrankPermutation(lRank, z, lPerm.data());
    if(lPerm.is_separable_low()) {
      ++lCount;
    }
  }
  return lCount;
}

void
test3() {
  for(uint z = 2; z < 10; ++z) {
    const uint64_t N = factorial64(z);
    const uint64_t lSep = count_separable(z);
    const uint64_t lNoSep = N - lSep;
    std::cout << std::setw( 2) << z << ' ' 
              << std::setw(12) << N << ' '
              << std::setw(12) << lSep << ' '
              << std::setw(12) << lNoSep << ' '
              << std::endl;
  }
}

/*
 *  for every permuation print its separability classes
 */

void
print_sepclass(const uint z) {
  perm_t lPermId(z, perm_t::kId);
  assert(lPermId.is_identity());
  perm_t lPermRev(z, perm_t::kRev);
  assert(lPermRev.is_reverse());

  for(PermutationGenerator lGen(z); lGen.isValid(); ++lGen) {
    std::cout << z << ' ' << (*lGen) << "  ";
    for(uint g = 0; g < z - 1; ++g) {
      std::cout << ' ';
      if((*lGen).is_separable_low_at(g)) {
        std::cout << std::setw(2) << g;
      } else {
        std::cout << "  ";
      }
    }
    std::cout << std::endl;
  }
}

void
test4() {
  std::cout << "=== test4 ===" << std::endl;
  std::cout << "separability classes" << std::endl;
  for(uint z = 3; z < 6; ++z) {
    print_sepclass(z);
  }
}


bool
find_uniquifier(perm_t& aU, const uint g, const perm_t& aPerm1, const perm_t& aPerm2) {
  assert(aPerm1.size() == aPerm2.size());
  const uint z = aPerm1.size();
  perm_t lUo1;
  perm_t lUo2;
  int    lSep = -1;
  bool   lRes = false;
  for(PermutationGenerator lGen(z); lGen.isValid(); ++lGen) {
    const perm_t& U = (*lGen);
    lUo1.set_to_concat_of(aPerm1, U);
    lUo2.set_to_concat_of(aPerm2, U);
    if((1 == lUo1.no_separated_from(lUo2, &lSep))) { //  && ((int) g == lSep))
      aU = (*lGen);
      std::cout << "    uniquifier: " << U
                << "  " << lUo1
                << "  " << lUo2
                << std::endl;
    }
  }
  return lRes;
}

bool
find_cspfier(perm_t& aU, const perm_t& aPerm1, const perm_t& aPerm2) {
  assert(aPerm1.size() == aPerm2.size());
  const uint z = aPerm1.size();
  perm_t lUo1;
  perm_t lUo2;
  bool   lRes = false;
  for(PermutationGenerator lGen(z); lGen.isValid(); ++lGen) {
    const perm_t& U = (*lGen);
    lUo1.set_to_concat_of(aPerm1, U);
    lUo2.set_to_concat_of(aPerm2, U);
    if(lUo1.have_common_subset_prefix(lUo2)) {
      aU = (*lGen);
      std::cout << "    cspfier: " << U
                << "  " << lUo1
                << "  " << lUo2
                << std::endl;
    }
  }
  return lRes;
}

void
test5() {
  perm_t lU;
  perm_t lUoP1;
  perm_t lUoP2;
  for(uint z = 3; z < 4; ++z) {
    for(PermutationGenerator lGen1(z); lGen1.isValid(); ++lGen1) {
      for(PermutationGenerator lGen2(z); lGen2.isValid(); ++lGen2) {
        const uint lNoSep = (*lGen1).no_separated_from(*lGen2);
        const bool lCsp = (*lGen1).have_common_subset_prefix(*lGen2);
        if(lCsp) { continue; }
        if(1 == lNoSep) { continue; }
        std::cout << (*lGen1)
                  << "   " << (*lGen2)
                  << "   " << std::setw(2) << lNoSep
                  << "   " << ((*lGen1).have_common_subset_prefix(*lGen2) ? "csp" : "---")
                  << std::endl;
        if(0 == lNoSep) {
          find_cspfier(lU, (*lGen1), (*lGen2));
        } else
        if(1 == lNoSep) {
        } else
        if(1 < lNoSep) {
          for(uint g = 0; g < z - 1; ++g) {
            if(find_uniquifier(lU, g, *lGen1, *lGen2)) {
              lUoP1.set_to_concat_of(lU, *lGen1);
              lUoP2.set_to_concat_of(lU, *lGen2);
              std::cout << (*lGen1)
                        << "   " << (*lGen2)
                        << "   " << lNoSep
                        << "   " << g
                        << "   " << lU
                        << "   " << lUoP1
                        << "   " << lUoP2
                        << "   " << lUoP1.no_separated_from(lUoP2)
                        << std::endl;
            }
          }
        }
      }
    }
  }
}

// test kSep[Id|Rev][Pos|Neg]
void
test6() {
  const uint z = 5;
  for(uint g = 0; g < z; ++g) {
    perm_t lPermSepIdPos(z, g, perm_t::kSepIdPos);
    perm_t lPermSepIdNeg(z, g, perm_t::kSepIdNeg);
    perm_t lPermSepRevPos(z, g, perm_t::kSepRevPos);
    perm_t lPermSepRevNeg(z, g, perm_t::kSepRevNeg);
    assert(lPermSepIdPos.is_valid());
    assert(lPermSepIdNeg.is_valid());
    assert(lPermSepRevPos.is_valid());
    assert(lPermSepRevNeg.is_valid());
    int lSepIdA = -1;
    const uint lNoSepIdA = lPermSepIdPos.no_separated_from(lPermSepIdNeg, &lSepIdA);
    int lSepIdB = -1;
    const uint lNoSepIdB = lPermSepIdNeg.no_separated_from(lPermSepIdPos, &lSepIdB);
    int lSepRevA = -1;
    const uint lNoSepRevA = lPermSepRevPos.no_separated_from(lPermSepRevNeg, &lSepRevA);
    int lSepRevB = -1;
    const uint lNoSepRevB = lPermSepRevNeg.no_separated_from(lPermSepRevPos, &lSepRevB);
    std::cout << z << ' ' << g
              << "  " << lPermSepIdPos 
              << "  " << lPermSepIdNeg
              << "  " << std::setw(2) << lNoSepIdA << ' ' << std::setw(2) << lSepIdA
              << "  " << std::setw(2) << lNoSepIdB << ' ' << std::setw(2) << lSepIdB
              << lPermSepRevPos
              << "  " << lPermSepRevNeg
              << "  " << std::setw(2) << lNoSepRevA << ' ' << std::setw(2) << lSepRevA
              << "  " << std::setw(2) << lNoSepRevB << ' ' << std::setw(2) << lSepRevB
              << std::endl;
  }
}

/*
 *  test perm_t::print_binary_props
 */
void
test7(const uint z) {
  uint64_t lFlags = // perm_t::kIsId             |
                    // perm_t::kIsRev            |
                    perm_t::kIsInvolution     |
                    // perm_t::kIsReducible      |
                    // perm_t::kIsIrreducible    |
                    perm_t::kIsConnected      |
                    // perm_t::kIsDisconnected   |
                    perm_t::kIsDecomposable   |
                    perm_t::kIsIndecomposable |
                    perm_t::kIsDerangement    |
                    perm_t::kIsSelfRevCmpl    |
                    perm_t::kIsSeparableLow;

  for(PermutationGenerator lGen(z); lGen.isValid(); ++lGen) {
    std::cout << (*lGen);
    (*lGen).print_bool_props(std::cout, lFlags) << std::endl;
  }
}


// check kInvRev, kRevInv
void
test8(const uint z, const bool aTrace) {
  bool lRes = true;
  for(PermutationGenerator lGen(z); lGen.isValid(); ++lGen) {
    const perm_t& pi = (*lGen);
    const perm_t  pi_inv(perm_t::kInverse, pi);
    const perm_t  pi_rev(perm_t::kReverse, pi);
    const perm_t  pi_inv_rev(perm_t::kInverse, pi_rev);
    const perm_t  pi_rev_inv(perm_t::kReverse, pi_inv);
    const perm_t  pi_inv_rev_x(perm_t::kInvRev, pi);
    const perm_t  pi_rev_inv_x(perm_t::kRevInv, pi);
    const bool lOk = (pi_inv_rev == pi_inv_rev_x) && (pi_rev_inv == pi_rev_inv_x);
    if(aTrace || !lOk) {
      std::cout << pi
                << "  " << pi_inv
                << "  " << pi_rev
                << ". " << pi_inv_rev
                << "  " << pi_inv_rev_x
                << ". " << pi_rev_inv
                << "  " << pi_rev_inv_x
                << std::endl;
    }
    assert(pi_inv_rev == pi_inv_rev_x);
    assert(pi_rev_inv == pi_rev_inv_x);
    lRes = lRes && lOk;
  }
  std::cout << "test8: " << z << ": " << (lRes ? "ok" : "BAD") << std::endl;
}

// check kConcRevInv, kConcInvRev
void
test9(const uint z, const bool aTrace) {
  bool lRes = true;
  for(PermutationGenerator lGen(z); lGen.isValid(); ++lGen) {
    const perm_t& pi = (*lGen);
    const perm_t  pi_inv(perm_t::kInverse, pi);
    const perm_t  pi_rev(perm_t::kReverse, pi);
    const perm_t  pi_inv_rev(pi_rev, pi_inv, perm_t::kConcat);
    const perm_t  pi_rev_inv(pi_inv, pi_rev, perm_t::kConcat);
    const perm_t  pi_inv_rev_x(perm_t::kConcInvRev, pi);
    const perm_t  pi_rev_inv_x(perm_t::kConcRevInv, pi);
    const bool lOk = (pi_inv_rev == pi_inv_rev_x) && (pi_rev_inv == pi_rev_inv_x);
    if(aTrace || !lOk) {
      std::cout << pi
                << ". " << pi_inv
                << "  " << pi_rev
                << ". " << pi_inv_rev
                << "  " << pi_inv_rev_x
                << ". " << pi_rev_inv
                << "  " << pi_rev_inv_x
                << std::endl;
    }
    // assert(pi_inv_rev == pi_inv_rev_x);
    // assert(pi_rev_inv == pi_rev_inv_x);
    lRes = lRes && lOk;
  }
  std::cout << "test9: " << z << ": " << (lRes ? "ok" : "BAD") << std::endl;
}

// test perm_t::sum_diff() und perm_t::sum_diff(pi)
void
test10(const uint aZend, const bool aTrace) {
  std::cout << "=== test10 ===" << std::endl;
/*
  uint64_t lProps = perm_t::kIsInvolution     |
                    perm_t::kIsReducible      |
                    perm_t::kIsSeparableLow   |
                    perm_t::kIsSeparableHi    |
                    perm_t::kIsConnected      |
                    perm_t::kIsDerangement    |
                    perm_t::kIsSelfRevCmpl    |
                    perm_t::kMaximizesSumDist ;
*/
  for(uint z = 2; z < aZend; ++z) {
    perm_t lPermId(z, perm_t::kId);
    perm_t lPermRev(z, perm_t::kRev);
    for(PermutationGenerator lGen(z); lGen.isValid(); ++lGen) {
      const perm_t& lPerm = (*lGen);
      const uint lSumDist    = lPerm.sum_dist();
      const uint lSumDistId  = lPerm.sum_dist(lPermId);
      const uint lSumDistRev = lPerm.sum_dist(lPermRev);
      if(aTrace) {
        std::cout << lPermId
                  << "  " << lPermRev
                  << "  " << lPerm
                  << "  " << std::setw(3) << lSumDist
                  << "  " << std::setw(3) << lSumDistId
                  << "  " << std::setw(3) << lSumDistRev
                  << std::endl;
      }
    }
  }
}

void
test11(const uint aZend, const bool aTrace) {
  std::cout << "=== test11 ===" << std::endl;
  perm_t lSumDirect;
  perm_t lSumSchief;
  for(uint z = 2; z < aZend; ++z) {
    // perm_t lPermRev(z, perm_t::kRev);
    uint lDistSum    = 0;
    uint lDistSumLag = 0;
    for(PermutationGenerator lGen1(z); lGen1.isValid(); ++lGen1) {
      const perm_t& lPerm1 = (*lGen1);
      for(PermutationGenerator lGen2(z); lGen2.isValid(); ++lGen2) {
        const perm_t& lPerm2 = (*lGen2);
        perm_t lSumDirect(lPerm1, lPerm2, perm_t::kSumDirect);
        perm_t lSumSchief(lPerm1, lPerm2, perm_t::kSumSchief);
        const uint lDist12 = lPerm1.sum_dist(lPerm2);
        lDistSumLag = lDistSum;
        lDistSum    = lSumDirect.sum_dist(lSumSchief);
        if(aTrace) {
          std::cout << lPerm1
                    << "  " << lPerm2
                    << "  " << lSumDirect
                    << "  " << lSumSchief
                    << "  " << std::setw(3) << lDist12
                    << "  " << std::setw(3) << lDistSum
                    << "  " << std::endl;
        }
        if(0 < lDistSumLag) {
          assert(lDistSum == lDistSumLag);
        }
      }
    }
    std::cout << std::setw(2) << z
              << "  " << std::setw(8) << lDistSum
              << std::endl;
  }
}
void
test11a(const uint aZend, const bool aTrace) {
  std::cout << "=== test11a ===" << std::endl;
  perm_t lSumDirect;
  perm_t lSumSchief;
  for(uint z = 2; z < aZend; ++z) {
    perm_t lPermRev(z, perm_t::kRev);
    uint lDistSum    = 0;
    uint lDistSumLag = 0;
      const perm_t& lPerm1 = lPermRev;
      for(PermutationGenerator lGen2(z); lGen2.isValid(); ++lGen2) {
        const perm_t& lPerm2 = (*lGen2);
        perm_t lSumDirect(lPerm1, lPerm2, perm_t::kSumDirect);
        perm_t lSumSchief(lPerm1, lPerm2, perm_t::kSumSchief);
        const uint lDist12 = lPerm1.sum_dist(lPerm2);
        lDistSumLag = lDistSum;
        lDistSum    = lSumDirect.sum_dist(lSumSchief);
        if(aTrace) {
          std::cout << lPerm1
                    << "  " << lPerm2
                    << "  " << lSumDirect
                    << "  " << lSumSchief
                    << "  " << std::setw(3) << lDist12
                    << "  " << std::setw(3) << lDistSum
                    << "  " << std::endl;
        }
        if(0 < lDistSumLag) {
          assert(lDistSum == lDistSumLag);
        }
      }
    std::cout << std::setw(2) << z
              << "  " << std::setw(8) << lDistSum
              << std::endl;
  }
}

uint64_t
no_sep_with_duplicates(const uint z) {
  uint64_t lRes = 0;
  for(uint g = 1; g < z; ++g) {
    lRes += factorial64(g) * factorial64(z - g);
  }
  return lRes;
}

uint64_t
A136128(const uint z) {
  uint64_t lRes = 0;
  for(uint g = 0; g < z; ++g) {
    lRes += factorial64(g) * factorial64(z - g);
  }
  return lRes;
}

// A003149   a(n) = Sum_{k=0..n} k!(n-k)!
uint64_t
A003149(const uint z) {
  uint64_t lRes = 0;
  for(uint g = 0; g <= z; ++g) {
    lRes += factorial64(g) * factorial64(z - g);
  }
  return lRes;
}

int
sum_z1(const perm_t aPerm, const int z1, const int z2) {
  int lRes = 0;
  for(int i = 0; i < z1; ++i) {
    lRes += std::abs((int) aPerm(i) - z1 - z2 + 1 + i);
  }
  return lRes;
}

int
sum_z2(const perm_t aPerm, const int z1, const int z2) {
  int lRes = 0;
  for(int i = 0; i < z2; ++i) {
    lRes += std::abs((int) aPerm(i) + z1 - z2 + 1 + i);
  }
  return lRes;
}

void
test11b(const uint aZend, const bool aTrace) {
  std::cout << "=== test11b ===" << std::endl;
  perm_t lSumDirect;
  perm_t lSumSchief;
  for(uint z = 1; z < aZend; ++z) {
    perm_t lPermRev(z, perm_t::kRev);
    uint lMaxDirect = 0;
    uint lNoMaxDirect = 0;
    uint lCount = 0;
    for(uint g = 1; g < z; ++g) {
      const uint z1 = g;
      const uint z2 = z - z1;
      perm_t lPermId1(z1, perm_t::kId);
      perm_t lPermId2(z2, perm_t::kId);
      perm_t lPermRev1(z1, perm_t::kRev);
      perm_t lPermRev2(z2, perm_t::kRev);
      for(PermutationGenerator lGen1(z1); lGen1.isValid(); ++lGen1) {
        const perm_t& lPerm1 = (*lGen1);
        for(PermutationGenerator lGen2(z2); lGen2.isValid(); ++lGen2) {
          const perm_t& lPerm2 = (*lGen2);
          ++lCount;
          perm_t lPerm1uId2(lPerm1, lPermId2, perm_t::kSumDirect);
          perm_t lPermId1u2(lPermId1, lPerm2, perm_t::kSumDirect);
          perm_t lPermConc12(lPerm1uId2, lPermId1u2, perm_t::kConcat);
          perm_t lPermConc21(lPermId1u2, lPerm1uId2, perm_t::kConcat);
          perm_t lPermSumDirect(lPerm1, lPerm2, perm_t::kSumDirect);
          perm_t lPermSumSchief(lPerm1, lPerm2, perm_t::kSumSchief);

          const uint lDist1 = lPerm1.sum_dist(lPermRev1); 
          const uint lDist2 = lPerm2.sum_dist(lPermRev2); 
          const uint lDist3 = lPermSumDirect.sum_dist(lPermRev); // *decomp* delta(pi1 + pi2, pi_rev)
          const uint lDist4 = lPermSumSchief.sum_dist(lPermRev);
          const uint lDist5 = lPerm1uId2.sum_dist(lPermRev);
          const uint lDist6 = lPermId1u2.sum_dist(lPermRev);

          const uint lSumZ1 = sum_z1(lPerm1, z1, z2);
          const uint lSumZ2 = sum_z2(lPerm2, z1, z2);

          if(lDist3 > lMaxDirect) {
            lNoMaxDirect = 1;
            lMaxDirect = lDist3;
          } else
          if(lDist3 == lMaxDirect) {
            ++lNoMaxDirect;
          }
          if(aTrace) {
            std::cout << lPerm1
                      // << "  " << lPerm1uId2
                      << "  "  << lPerm2
                      // << "  " << lPermId1u2
                      << "  "  << lPermSumDirect
                      << "  "  << lPermSumSchief
                      << "   " << std::setw(3) << lDist1   // delta(pi1, pi_rev^z1)
                      << "   " << std::setw(3) << lDist2   // delta(pi2, pi_rev^z2)
                      << " | " << std::setw(3) << lDist3   // delta(pi1+pi2, pi_rev^z)
                      // << "   " << std::setw(3) << lDist4
                      << " | " << std::setw(3) << lDist5
                      << "   " << std::setw(3) << lDist6
                      // << "  "  << (lPermSumDirect.is_separated_from(lPermRev) ? "sep" : "---")
                      // << "  "  << (lPermSumSchief.is_separated_from(lPermRev) ? "sep" : "---")
                      // << "  "  << (lPermSumDirect.have_common_subset_prefix(lPermRev) ? "csp" : "---")
                      // << "  "  << (lPermSumSchief.have_common_subset_prefix(lPermRev) ? "csp" : "---")
                      << " | " << std::setw(2) << lSumZ1
                      << ' '   << std::setw(2) << lSumZ2
                      << ' '   << std::setw(3) << (lSumZ1 + lSumZ2)
                      // << " | " << std::setw(3) << lDistK1
                      // << "  "  << std::setw(3) << lDistK2
                      << " | " << perm_t::max_sum_dist(z)
                      << std::endl;
            // std::cout << "  " << lPerm1uId2
            //           << "  " << lPermId1u2
            //           << "  " << lPermConc12
            //           << "  " << lPermConc21
            //           << std::endl;

          }
          assert(lPermSumDirect == lPermConc12);
          assert(lPermSumDirect == lPermConc21);
          assert(lDist1 <= std::min(lDist3, lDist4));
          assert(lDist2 <= std::min(lDist3, lDist4));
          assert(lDist3 == (lSumZ1 + lSumZ2));
          assert(lDist3 <= std::min(lDist5, lDist6));
          assert(lDist3 == std::min(lDist5, lDist6)); // this is the true surprise
        }
      }
    }
    std::cout << std::setw(2) << z
              << "  "  << std::setw( 8) << lCount
              << "  "  << std::setw( 8) << lNoMaxDirect
              << "  "  << std::setw( 8) << lMaxDirect   // A007590: (n) = floor(n^2/2). 
              << " | " << std::setw(10) << no_sep_with_duplicates(z)
              << " | " << std::setw(10) << A136128(z)
              << " | " << std::setw(10) << A003149(z)
              << std::endl;
  }
}

uint_vt gPrimes({2,3,5,7,11,13,17});

// test multiply

void
mul(const uint a, const perm_t& aPerm, const perm_t& aTau1, const perm_t& aTau2, const bool aTrace) {
  perm_t lPerm = aPerm;
  lPerm.multiply_by(a);
  const uint lD11 = aPerm.sum_dist(aTau1);
  const uint lD12 = lPerm.sum_dist(aTau1);
  const uint lD21 = aPerm.sum_dist(aTau2);
  const uint lD22 = lPerm.sum_dist(aTau2);
  if(aTrace) {
    std::cout << std::setw(2) << a << " * " << aPerm
              << " = " << lPerm
              << " | " << lD11
              << "  "  << lD12
              << " | " << lD21
              << "  "  << lD22
              << " | " << aTau1
              << "  "  << aTau2
              << std::endl;
  }
}

void
test12(const uint aZend, const bool aTrace) {
  std::cout << "=== test12 ===" << std::endl;
  perm_t lPerm;
  for(uint z = 3; z < aZend; ++z) {
    perm_t lPermId(z, perm_t::kId);
    perm_t lPermRev(z, perm_t::kRev);
    perm_t lPerm(z);
    for(PermutationGenerator lGen(z); lGen.isValid(); ++lGen) {
      for(uint a = 2; a < aZend; ++a) {
        if(1 != ggt(a, z)) { continue; }
        mul(a, (*lGen), lPermId, lPermRev, aTrace);
      }
      for(uint i = 0; i < gPrimes.size(); ++i) {
        const uint a = gPrimes[i];
        if(1 != ggt(a, z)) { continue; }
        mul(a, (*lGen), lPermId, lPermRev, aTrace);
      }
    }
  }
}

/*
 *  #pi: cmpl(pi) = rev(pi)
 */

bool
cmpl_eq_rev(const perm_t& aPerm, const bool aTrace) {
  perm_t lPermRev(perm_t::kReverse, aPerm);
  perm_t lPermCmpl(perm_t::kComplement, aPerm);
  const bool lRes = (lPermRev == lPermCmpl);
  if(aTrace) {
    std::cout << aPerm
              << "  " << lPermRev
              << "  " << lPermCmpl
              << "  " << (lRes ? " eq" : "neq")
              << std::endl;
  }
  return lRes;
}

// A000165 vs A241122 vs here
// 1, 2, 8, 48, 384, 3840, 46080, 645120, 10321920,  185794560, 3715891200,  81749606400
//    2, 8, 48, 384, 3840, 46080, 645120, 10321920, 2786918400, 2229534720, 735746457600
//    2  8  48  384  3840  46080, 645120,

void
test13(const uint aZend, const bool aTrace) {
  std::cout << "=== test13 ===" << std::endl;
  const uint lInc = (aTrace ? 1 : 2);
  for(uint z = 2; z < aZend; z += lInc) {
    uint lCount = 0;
    for(PermutationGenerator lGen(z); lGen.isValid(); ++lGen) {
      const bool lV1 = (*lGen).is_self_revcmpl();
      // const bool lV2 = cmpl_eq_rev(*lGen, aTrace);
      // assert(lV1 == lV2);
      lCount += lV1;
    }
    std::cout << std::setw(2) << z
              << "  " << std::setw(8) << lCount
              << std::endl;
  }
}

void
test14(const uint aZend, const bool aTrace) {
  std::cout << "=== test14 ===" << std::endl;
  uint lCount[2][2];
  uint lCountSep = 0;
  for(uint z = 2; z < aZend; ++z) {
    lCount[0][0] = 0;
    lCount[0][1] = 0;
    lCount[1][0] = 0;
    lCount[1][1] = 0;
    lCountSep    = 0;
    for(PermutationGenerator lGen(z); lGen.isValid(); ++lGen) {
      const perm_t& lPerm = (*lGen);
      const bool    lSepL = lPerm.is_separable_low();
      const bool    lSepH = lPerm.is_separable_hi();
      const bool    lSep  = lPerm.is_separable();
      const pset_t  lSepP = lPerm.get_separation_points();
      ++(lCount[lSepL][lSepH]);
      lCountSep += lSep;
      if(aTrace) {
        std::cout << lPerm
                  << "  " << (lSepL ? "sepL" : "----") 
                  << "  " << (lSepH ? "sepH" : "----") 
                  << "  " << (lSep  ? "sep" : "---") 
                  << "  " << to_str(lSepP, z)
                  << std::endl;
      }
      assert(implies(lSepL, !lSepH));
      assert(implies(lSepH, !lSepL));
      assert(lSep == (lSepL || lSepH));
      for(BvMemberIdxAsc<pset_t> lIter(lSepP); lIter.isValid(); ++lIter) {
        const uint g = (*lIter);
        if(aTrace) {
          std::cout << "g = " << g << "  " << lPerm.is_separable_at(g)
                    << std::endl;
        }
        assert(lPerm.is_separable_at(g) || lPerm.is_separable_at(z - 2 - g));
        assert(lPerm.is_separable_low_at(g) || lPerm.is_separable_hi_at(g));
      }
    }
    std::cout << std::setw(2) << z
              << "  " << std::setw(8) << lCount[0][0]
              << "  " << std::setw(8) << lCount[0][1]
              << "  " << std::setw(8) << lCount[1][0]
              << "  " << std::setw(8) << lCount[1][1]
              << "  " << std::setw(8) << lCountSep
              << std::endl;
    assert(0 == lCount[1][1]);
    assert(lCount[0][1] + lCount[1][0] == lCountSep);
  }
}

void
test15(const uint aZend, const bool aTrace) {
  const uint64_t lPropSet = 0xFFFF;
  for(uint z = 3; z < aZend; ++z) {
    perm_t lPermRev(z, perm_t::kRev);
    for(PermutationGenerator lGen(z); lGen.isValid(); ++lGen) {
      const perm_t& lPerm = (*lGen);
      const bool lCsp = lPerm.have_common_subset_prefix(lPermRev);
      const bool lSep = lPerm.is_separated_from(lPermRev);
      perm_t lRevOfPerm(perm_t::kReverse, lPerm);
      perm_t lCmplOfPerm(perm_t::kComplement, lPerm);
      if(false ||
         lCsp  || 
         lSep  ||
         false) {
        continue;
      }
      if(aTrace) {
        std::cout << lPerm
                  << "  " << lPermRev
                  << "  " << (lCsp ? "CSP" : "---")
                  << "  " << (lSep ? "SEP" : "---")
                  << "  " << lPerm.sum_dist(lPermRev)
                  << std::endl;
        std::cout << "   " << lPerm; lPerm.print_bool_props(std::cout, lPropSet) << std::endl;
        std::cout << "   " << lRevOfPerm;  lRevOfPerm.print_bool_props(std::cout, lPropSet) << std::endl;
        std::cout << "   " << lCmplOfPerm; lCmplOfPerm.print_bool_props(std::cout, lPropSet) << std::endl;
      }
    }
  }
}

void
test16(const uint aZend, const bool aTrace) {
  std::cout << " z"
            << "  " << std::setw(12) << "ico_pi_rev"
            << std::endl;
  for(uint z = 1; z < aZend; ++z) {
    perm_t lPermRev(z, perm_t::kRev);
    uint lCount = 0; // A000085: Number of self-inverse permutations on n letters (involutions)
    for(PermutationGenerator lGen(z); lGen.isValid(); ++lGen) {
      const perm_t& lPerm = (*lGen);
      lCount += lPerm.inverse_commute(lPermRev);
    }
    std::cout << std::setw(2) << z
              << "  " << std::setw(12) << lCount
              << std::endl;
  }
}

void
test17(const uint aZend, const bool aTrace) {
  std::cout << "=== test17 ===" << std::endl;
  for(uint z = 2; z < aZend; ++z) {
    for(PermutationGenerator lGen(z); lGen.isValid(); ++lGen) {
      const perm_t& lPerm = (*lGen);
      const perm_t  lPermM2F(perm_t::kMove2Front, lPerm, 0);
      std::cout << lPerm << "  " << lPermM2F << std::endl;
      assert(lPermM2F.is_valid());
    }
  }
}
        
      
int
main() {
  // test0();
  // test1(8);
  // test2(3);
  // test2(4);
  // test3();
  // test4();
  // test5();
  // test6();
  // test7(5);
  // test8(3, true);
  // test8(4, false);
  // test8(5, false);
  // test8(6, false);
  // test9(2, true);
  // test9(3, true);
  // test9(4, true);
  // test9(2, false);
  // test9(3, false);
  // test9(4, false);
  // test9(5, false);
  // test9(6, false);
  // test9(7, false);
  // test10(5, true);    // test perm_t::sum_diff() und perm_t::sum_diff(pi)
  // test11( 5, true);   // test sum_direct, sum_schief
  // test11( 7, false);  // test sum_direct, sum_schief
  // test11a(12, false); // test sum_direct, sum_schief: A001105: a(n) = 2*n^2
  // test11b( 7, true);  // test sum_direct, sum_schief
  // test11b(10, false); // test sum_direct, sum_schief
  // test12(4,true);     // test multiply permutation by a with ggt(a,z) = 1
  // test13(5,true);
  // test13(10,false);
  // test14(6, true);    // test sepLo, sepHi, sep
  // test14(10, false);  // test sepLo, sepHi, sep
  // test15(6, true);    // look at permutations !csp(pi,pi_rev) and !sep(pi, pi_rev)
  //  test16(12, false);  // for pi_rev count #pi with inverse_commute(pi, pi_rev)
  test17(6, true);  // check kMove2Front
  return 0;
}


#include "permutation.hh"
#include "infra/bit_intrinsics.hh"
#include "infra/rank_permutation.hh"
#include "infra/ggt.hh"

// namespace perm {

/*
 *  members perm_t
 */

perm_t::perm_t(const uint z, const kind_t aKind) : _vec(z) {
  switch(aKind) {
    case kId:  init_id(z);
               break;
    case kRev: init_rev(z);
               break;
    default:   break;
  }
}

perm_t::perm_t(const uint z, const kind_t aKind, const uint i, const uint j) : _vec(z) {
  assert(kTransposition == aKind);
  assert(i < z);
  assert(j < z);
  init_id(z);
  _vec[i] = j;
  _vec[j] = i;
}

perm_t::perm_t(const uint z, const uint g, const kind_t aKind) : _vec() {
  switch(aKind) {
    case kSepIdPos:  init_sep_id_pos(z, g);
                     break;
    case kSepIdNeg:  init_sep_id_neg(z, g);
                     break;
    case kSepRevPos: init_sep_rev_pos(z, g);
                     break;
    case kSepRevNeg: init_sep_rev_neg(z, g);
                     break;
    default: assert(0 == 1); break;
  }
}

perm_t::perm_t(const unop_t aUnop, const perm_t& aPerm) : _vec() {
  switch(aUnop) {
    case kCopy:        _vec = aPerm.vec();
                       break;
    case kInverse:     set_to_inverse_of(aPerm);
                       break;
    case kReverse:     set_to_reverse_of(aPerm);
                       break;
    case kComplement:  set_to_complement_of(aPerm);
                       break;
    case kInvRev:      set_to_inverse_of_reverse_of(aPerm);
                       break;
    case kRevInv:      set_to_reverse_of_inverse_of(aPerm);
                       break;
    case kConcInvRev:  set_to_conc_of_inverse_and_reverse_of(aPerm);
                       break;
    case kConcRevInv:  set_to_conc_of_reverse_and_inverse_of(aPerm);
                       break;
    default: break;
  }
}

perm_t::perm_t(const unop_t aUnop, const perm_t& aPerm, const uint aVal) : _vec() {
  const uint z = aPerm.size();

  assert(aUnop == kMove2Front);
  assert(aVal < z);
  if(0 == aPerm[0]) {
    _vec = aPerm._vec;
    return;
  }
  _vec.resize(z);
  _vec[0] = 0;
  uint j = 1;
  for(uint i = 0; i < z; ++i) {
    if(0 != aPerm[i]) {
      _vec[j++] = aPerm[i];
    }
  }
}

perm_t::perm_t(const unop_t aUnOp, const perm_t& aPerm, const uint i, const uint j) : _vec() {
  const uint z = aPerm.size();
  assert(i < z);
  assert(j < z);
  assert(aUnOp == kSwap);
  _vec = aPerm.vec();
  std::swap(_vec[i], _vec[j]);
}


perm_t::perm_t(const perm_t& aPerm1, const perm_t& aPerm2, const binop_t aBinop) 
       : _vec() {
  switch(aBinop) {
    case kConcat:    set_to_concat_of(aPerm1, aPerm2);
                     break;
    case kSumDirect: set_to_sum_direct_of(aPerm1, aPerm2);
                     break;
    case kSumSchief: set_to_sum_schief_of(aPerm1, aPerm2);
                     break;
    default: assert(0 == 1);
  }
}

perm_t::perm_t(const uint64_t aRank, const uint z) : _vec(z) {
  ftUnrankPermutation<uint64_t>(aRank, z, _vec.data());
}

perm_t&
perm_t::init_id(const uint z) {
  _vec.resize(z);
  for(uint i = 0; i < size(); ++i) {
    _vec[i] = i;
  }
  return (*this);
}

perm_t&
perm_t::init_rev(const uint z) {
  _vec.resize(z);
  for(uint i = 0; i < z; ++i) {
    _vec[i] = z - 1 - i;
  }
  return (*this);
}

perm_t&
perm_t::init_vec(const uint_vt& aVec) {
  _vec = aVec;
  return (*this);
}

// 0...g | g+1 ... z-1  (= pi_id)
perm_t&
perm_t::init_sep_id_pos(const uint z, const uint g) {
  assert(g < z);
  return init_id(z);
}

// g+1...z-1 | 0...g
// (z - 1) - (g + 1) + 1 = z - 1 - g - 1 + 1 = z - g - 1
perm_t&
perm_t::init_sep_id_neg(const uint z, const uint g) {
  assert(g < z);
  if(g == z - 1) {
    return init_id(z);
   }
  _vec.resize(z);
  for(uint i = 0; i < z; ++i) {
    _vec[(z - g - 1 + i) % z] = i;
  }
  return (*this);
}

// g ... 0 | z-1 ... g+1
perm_t&
perm_t::init_sep_rev_pos(const uint z, const uint g) {
  assert(g < z);
  if(g == z - 1) {
    return init_rev(z);
  } 
  _vec.resize(z);
  for(uint i = 0; i <= g; ++i) {
    _vec[i] = g - i;
  }
  for(uint i = 1; i < z - g; ++i) {
    _vec[g + i] = z - i;
  }
  return (*this);
}

// z-1 ... g+1 | g-1 ... 0  = pi_rev
perm_t&
perm_t::init_sep_rev_neg(const uint z, const uint g) {
  assert(g < z);
  init_rev(z);
  return (*this);
}

perm_t&
perm_t::unrank(const uint64_t aRank, const uint z) {
  if(size() != z) {
    resize(z);
  }
  ftUnrankPermutation<uint64_t>(aRank, z, _vec.data());
  return (*this);
}


uint
perm_t::inverse_of(const uint aVal) const {
  for(uint i = 0; i < size(); ++i) {
    if(val(i) == aVal) {
      return i;
    }
  }
  return -1;
}


bool
perm_t::is_identity() const {
  for(uint i = 0; i < size(); ++i) {
    if(_vec[i] != i) {
      return false;
    }
  }
  return true;
}

bool
perm_t::is_reverse() const {
  uint z = size() - 1;
  for(uint i = 0; i < size(); ++i, --z) {
    if(_vec[i] != z) {
      return false;
    }
  }
  return true;
}

bool
perm_t::is_involution() const {
  for(uint i = 0; i < size(); ++i) {
    if(_vec[_vec[i]] != i) {
      return false;
    }
  }
  return true;
}

bool
perm_t::is_reducible() const {
  return (0 < no_fixpoint());
}

bool
perm_t::is_irreducible() const {
  return (0 == no_fixpoint());
}

bool
perm_t::is_connected() const {
  return (!is_disconnected());
}

// exists interval I = [a,b], |I| => 2
// pi(I) is also interval
bool
perm_t::is_disconnected() const {
  uint lSet = 0;
  for(uint i = 1; i < size(); ++i) {
    lSet = ((uint) 1 << val(i - 1)) | ((uint) 1 << val(i));
    if(is_block_010(lSet)) {
      return true;
    }
  }
  return false;
}

/*
 * decomposable: exists k such that pi(i) > pi(j) for all i <= k < j
 */

bool
perm_t::is_decomposable() const {
  bool lRes = false;
  const uint z = size();
  for(uint k = 0; k < z - 1; ++k) {
    bool lSuccess = true;
    for(uint i = 0; (i <= k) && lSuccess; ++i) {
      for(uint j = k + 1; j < z; ++j) {
        if(val(i) < val(j)) {
          lSuccess = false;
          break;
        }
      }
    }
    if(lSuccess) {
      lRes = true;
      break;
    }
  }
  return lRes;
}

bool
perm_t::is_indecomposable() const {
  return (!is_decomposable());
}

bool
perm_t::is_derangement() const {
  return (0 == no_fixpoint());
}

bool
perm_t::is_self_revcmpl() const {
  const uint z = size();
  for(uint i = 0; i < z; ++i) {
    if((z - 1) != (val(i) + val(z - 1 - i))) {
      return false;
    }
  }
  return true;
}

bool
perm_t::is_separable_low_at(const uint g) const {
  for(uint i = 0; i <= g; ++i) {
    if(g < _vec[i]) {
      return false;
    }
  }
  return true;
}

bool
perm_t::is_separable_low() const {
  for(uint g = 0; g < size() - 1; ++g) {
    if(is_separable_low_at(g)) {
      return true;
    }
  }
  return false;
}

bool
perm_t::is_separable_hi_at(const uint g) const {
  const uint z = size();
  const uint lBegin = z - 1 - g;
  for(uint i = lBegin; i < z; ++i) {
    if(g < _vec[i]) {
      return false;
    }
  }
  return true;
}

bool
perm_t::is_separable_hi() const {
  for(uint g = 0; g < size() - 1; ++g) {
    if(is_separable_hi_at(g)) {
      return true;
    }
  }
  return false;
}

bool
perm_t::is_separable_at(const uint g) const {
  const uint z = size();
  assert(g < z - 1);
  return (is_separable_low_at(g) || is_separable_hi_at(g));
}

bool
perm_t::is_separable() const {
  return (0 != get_separation_points());
}

pset_t
perm_t::get_separation_points() const {
  assert(size() < (8 * sizeof(pset_t)));
  const uint z = size();
  const pset_t lOne = 1;
        pset_t lMask = 0;   // first i bits set
        pset_t lPsetL  = 0; // bits pi(0) .. pi(i) are set
        pset_t lPsetH  = 0; // bits pi(z - 1 - 0) .. pi(z - 1 - i) are set
        pset_t lRes   = 0;
  for(uint i = 0; i < z - 1; ++i) {
    lMask   |= (lOne << i);
    lPsetL  |= (lOne << val(i));
    lPsetH  |= (lOne << val(z - 1 - i));
    if((lPsetL == lMask) || (lPsetH == lMask)) {
      lRes |= (lOne << i);
    }
  }
  return lRes;
}

bool
perm_t::maximizes_sum_dist() const {
  return (sum_dist() == max_sum_dist(size()));
}

bool
perm_t::has_square_root_slow() const {
  const uint z = size();
  for(PermutationGenerator lGen(z); lGen.isValid(); ++lGen) {
    perm_t lPerm(*lGen, *lGen, kConcat);
    if((*this) == lPerm) {
      return true;
    }
  }
  return false;
}

bool
perm_t::has_square_root() const {
  PermutationAnalyzer lAna(*this);
  return lAna.has_square_root();
}

uint
perm_t::no_fixpoint() const {
  uint lRes = 0;
  for(uint i = 0; i < size(); ++i) {
    if(_vec[i] == i) {
      ++lRes;
    }
  }
  return lRes;
}

uint
perm_t::no_inversion() const {
  uint lRes = 0;
  for(uint i = 0; i < size(); ++i) {
    for(uint j = i + 1; j < size(); ++j) {
      lRes += (_vec[i] > _vec[j]);
    }
  }
  return lRes;
}

int
perm_t::sign() const {
  const uint lNoInv = no_inversion();
  return ((0x0 == (lNoInv & 0x1)) ? +1 : -1);
}

uint
perm_t::degree() const {
  perm_t lPerm1(*this);
  perm_t lPerm2(size());
  uint lRes = 0;
  while(!lPerm1.is_identity()) {
    lPerm2.set_to_concat_of(*this, lPerm1);
    lPerm1 = lPerm2;
    ++lRes;
  }

  return lRes;
}

uint
perm_t::sum_dist() const {
  uint lRes = 0;
  for(uint i = 0; i < size(); ++i) {
    lRes += std::abs( ((int) val(i)) - ((int) i));
  }
  return lRes;
}

uint
perm_t::sum_dist(const perm_t& aPerm) const {
  uint lRes = 0;
  for(uint i = 0; i < size(); ++i) {
    lRes += std::abs( ((int) val(i)) - ((int) aPerm.val(i)) );
  }
  return lRes;
}

uint
perm_t::kendall_tau_distance(const perm_t& aPerm) const {
  assert(size() == aPerm.size());
  const uint z = size();
  uint lRes = 0;
  for(uint i = 0; i < z; ++i) {
    for(uint j = i + 1; j < z; ++j) {
      if(val(i) < val(j) && aPerm(i) > aPerm(j)) {
        ++lRes;
      } else
      if(val(i) > val(j) && aPerm(i) < aPerm(j)) {
        ++lRes;
      }
    }
  }
  return lRes;
}

pset_t
perm_t::all() const {
  return ((((pset_t) 1) << size()) - 1);
}

// \sum_{i=0}^{aEnd-1} 2^{pi(i)}
pset_t
perm_t::pset(const uint aEnd) const {
  pset_t lRes = 0;
  for(uint i = 0; i < aEnd; ++i) {
    lRes |= (1 << _vec[i]);
  }
  return lRes;
}

uint
perm_t::inflection_point(const uint64_t aIdx) const {
  const uint z = size();
  int      lRes = -1;
  uint64_t lSet =  0;
  for(uint k = 0; k < z; ++k) {
    lSet |= (1 << val(k));
    if(!isSubsetOf(lSet, aIdx)) {
      lRes = k;
      break;
    }
  }
  if(0 > lRes) {
    lRes = z - 1;
  }
  return lRes;
}

// a number in Ret(z) is given by its two zero positions
uint
perm_t::inflection_point(const uint aZeroPos1, const uint aZeroPos2) const {
  uint i = 0;
  for(; i < size(); ++i) {
    if((val(i) == aZeroPos1) || (val(i) == aZeroPos2)) {
      break;
    }
  }
  return i;
}

bool
perm_t::is_equal_to(const perm_t& aPerm) const {
  if(size() != aPerm.size()) {
    return false;
  }
  for(uint i =  0; i < size(); ++i) {
    if(_vec[i] != aPerm[i]) {
      return false;
    }
  }
  return true;
}

bool
perm_t::disjoint(const perm_t& aPerm) const {
  assert(size() == aPerm.size());
  bool lRes = true;
  for(uint i = 0; i < size(); ++i) {
     if(!((i == (*this)[i]) || (i == aPerm[i]))) {
       lRes = false;
       break;
     }
  }
  return lRes;
}

bool
perm_t::is_inverse_of(const perm_t& aPerm) const {
  if(aPerm.size() != size()) {
    return false;
  }
  for(uint i = 0; i < size(); ++i) {
    if(i != _vec[aPerm[i]]) {
      return false;
    }
  }
  return true;
}

bool
perm_t::is_reverse_of(const perm_t& aPerm) const {
  if(aPerm.size() != size()) {
    return false;
  }
  const uint z = size();
  for(uint i = 0; i < aPerm.size(); ++i) {
    if(val(z - 1 - i) != aPerm[i]) {
      return false;
    }
  }
  return true;
}

bool
perm_t::is_complement_of(const perm_t& aPerm) const {
  if(aPerm.size() != size()) {
    return false;
  }
  const uint z = size();
  for(uint i = 0; i < aPerm.size(); ++i) {
    if((z - 1) != (val(i) + aPerm(i))) {
      return false;
    }
  }
  return true;
}

bool
perm_t::commute(const perm_t& aPerm) const {
  if(size() != aPerm.size()) {
    return false;
  }
  for(uint i = 0; i < size(); ++i) {
    if(_vec[aPerm[i]] != aPerm[_vec[i]]) {
      return false;
    }
  }
  return true;
}

bool
perm_t::inverse_commute(const perm_t& aPerm) const {
  const perm_t& pi_1 = (*this);
  const perm_t& pi_2 =  aPerm;
  const perm_t  pi_1_inv(kInverse, pi_1);
  const perm_t  pi_2_inv(kInverse, pi_2);
  const perm_t  pi_1inv2(pi_2, pi_1_inv, kConcat);
  const perm_t  pi_2inv1(pi_1, pi_2_inv, kConcat);
  return (pi_1inv2 == pi_2inv1);
}

bool
perm_t::is_conjugate_of_slow(const perm_t& aPerm) const {
  assert(size() == aPerm.size());
  const uint z = size();
  const uint64_t N = factorial64(z);
  perm_t lPerm(z);
  perm_t lInv(z);
  perm_t lConc1(z);
  perm_t lConc2(z);
  for(uint lRank = 0; lRank < N; ++lRank) {
    ftUnrankPermutation(lRank, z, lPerm.data());
    lConc1.set_to_concat_of(lPerm, aPerm);
    lInv.set_to_inverse_of(lPerm);
    lConc2.set_to_concat_of(lConc1, lInv);
    if(is_equal_to(lConc2)) {
      return true;
    }
  }
  return false;
}

bool
perm_t::have_common_subset_prefix(const perm_t& aPerm) const {
  pset_t lRelset1 = 0;
  pset_t lRelset2 = 0;
  assert(size() == aPerm.size());
  const uint l = size() - 1;
  for(uint i = 0; i < l; ++i) {
    lRelset1 |= (1 << (*this)[i]);
    lRelset2 |= (1 <<   aPerm[i]);
    if(lRelset1 == lRelset2) {
      return true;
    }
  }
  return false;
}

bool
perm_t::is_separated_from_at(const uint g, const perm_t& aPerm) const {
  assert(size() == aPerm.size());
  const uint z = size();
  assert(0 < z);
  assert(g < z);

  const uint lSet1 = pset(g+1);
  const uint lSet2 = (1 << z) - 1 - aPerm.pset(z - g - 1);
  return (lSet1 == lSet2);
/*
  pset_t lSet11 = 0; // lower half of this
  pset_t lSet12 = 0; // upper half of this
  pset_t lSet21 = 0; // lower half of aPerm
  pset_t lSet22 = 0; // upper half of aPerm

  for(uint i = 0; i <= g; ++i) {
    lSet11 |= (1 << (*this)[i]);
    lSet22 |= (1 << aPerm[z - g - 1 + i]);
  }
  for(uint i = g + 1; i < z; ++i) {
    lSet12 |= (1 << (*this)[i]);
    lSet21 |= (1 << aPerm[i - g - 1]);
  }
  return ((lSet11 == lSet22) && (lSet12 == lSet21));
*/

}

bool
perm_t::is_separated_from(const perm_t& aPerm) const {
  assert(size() == aPerm.size());
  const uint z = size();
  for(uint g = 0; g < z - 1; ++g) {
    if(is_separated_from_at(g, aPerm)) {
      return true;
    }
  }
  return false;
}

/*
 * returns |{g | sep(g,pi,pi')}|
 * if this is 1, aSepPointOut is set to the only separation point that exists
 */

uint
perm_t::no_separated_from(const perm_t& aPerm, int* aSepPointOut) const {
  assert(size() == aPerm.size());
  const uint z = size();
  uint lRes = 0;
  int lSepPointOut = 0;
  for(uint g = 0; g < z - 1; ++g) {
    if(is_separated_from_at(g, aPerm)) {
      ++lRes;
      lSepPointOut = g;
    }
  }
  if(nullptr != aSepPointOut) {
    if(1 == lRes) {
      (*aSepPointOut) = lSepPointOut;
    } else {
      (*aSepPointOut) = -1;
    }
  }
  return lRes;
}

uint
perm_t::get_all_separators(const perm_t& aPerm) const {
  assert(size() == aPerm.size());
  const uint z = size();
  uint lRes = 0;
  for(uint g = 0; g < z - 1; ++g) {
    if(is_separated_from_at(g, aPerm)) {
      lRes |= ((uint) 1) << g;
    }
  }
  return lRes;
}

bool
perm_t::is_separated_from_at_new(const uint g, const perm_t& aPerm) const {
  assert(size() == aPerm.size());
  const uint z = size();
  assert(0 < z);
  assert(0 < g);
  assert(g < z);
  const uint lSet1 = pset(g);
  const uint lSet2 = (1 << z) - 1 - aPerm.pset(z - g);
  return (lSet1 == lSet2);
}

uint
perm_t::no_common(const perm_t& aPerm) const {
  assert(size() == aPerm.size());
  const uint z = size();
  uint lRes = 0;
  for(uint i = 0; i < z; ++i) {
     lRes += (val(i) == aPerm.val(i));
  }
  return lRes;
}

perm_t&
perm_t::set_to_inverse_of(const perm_t& aPerm) {
  const uint z = aPerm.size();
  _vec.resize(z);
  for(uint i = 0; i < z; ++i) {
    _vec[aPerm(i)] = i;
  }
  return (*this);
}

perm_t&
perm_t::set_to_reverse_of(const perm_t& aPerm) {
  const uint z = aPerm.size();
  _vec.resize(z);
  for(uint i = 0; i < z; ++i) {
    _vec[z - 1 - i] = aPerm[i];
  }
  return (*this);
}

perm_t&
perm_t::set_to_inverse_of_reverse_of(const perm_t& aPerm) {
  const uint z = aPerm.size();
  _vec.resize(z);
  for(uint i = 0; i < z; ++i) {
    _vec[aPerm[i]] = z - 1 - aPerm[i];
  }
  return (*this);
}

perm_t&
perm_t::set_to_reverse_of_inverse_of(const perm_t& aPerm) {
  const uint z = aPerm.size();
  _vec.resize(z);
  for(uint i = 0; i < z; ++i) {
    _vec[z - 1 - aPerm(i)] = i;
  }
  return (*this);
}

// rev(pi) o inv(pi)
perm_t&
perm_t::set_to_conc_of_reverse_and_inverse_of(const perm_t& aPerm) {
  const uint z = aPerm.size();
  _vec.resize(z);
  for(uint i = 0; i < z; ++i) {
    _vec[aPerm[i]] = aPerm[z - 1 - i];
  }
  return (*this);
}

//  inv(pi) o rev(pi) = pi_rev
perm_t&
perm_t::set_to_conc_of_inverse_and_reverse_of(const perm_t& aPerm) {
  init_rev(aPerm.size());
  return (*this);
}

perm_t&
perm_t::set_to_complement_of(const perm_t& aPerm) {
  const uint z = aPerm.size();
  _vec.resize(z);
  for(uint i = 0; i < z; ++i) {
    _vec[i] = z - 1 - aPerm[i];
  }
  return(*this);
}

perm_t&
perm_t::set_to_concat_of(const perm_t& aPerm1, const perm_t& aPerm2) {
  assert(aPerm1.size() == aPerm2.size());
  const uint z = aPerm1.size();
  _vec.resize(z);
  for(uint i = 0; i < z; ++i) {
    _vec[i] = aPerm2[aPerm1[i]];
  }
  return (*this);
}

perm_t&
perm_t::set_to_sum_direct_of(const perm_t& aPerm1, const perm_t& aPerm2) {
  const uint z1 = aPerm1.size();
  const uint z2 = aPerm2.size();
  _vec.resize(z1 + z2);
  for(uint i = 0; i < z1; ++i) {
    _vec[i] = aPerm1[i];
  }
  for(uint i = 0; i < z2; ++i) {
    _vec[z1 + i] = z1 + aPerm2[i];
  }
  return (*this);
}

perm_t&
perm_t::set_to_sum_schief_of(const perm_t& aPerm1, const perm_t& aPerm2) {
  const uint z1 = aPerm1.size();
  const uint z2 = aPerm2.size();
  _vec.resize(z1 + z2);
  for(uint i = 0; i < z1; ++i) {
    _vec[i] = z2 + aPerm1[i];
  }
  for(uint i = 0; i < z2; ++i) {
    _vec[z1 + i] = aPerm2[i];
  }
  return (*this);
}

perm_t&
perm_t::multiply_by(const uint a) {
  const uint z = size();
  assert(1 == ggt(a,z));
  for(uint i = 0; i < z; ++i) {
    _vec[i] = (a * _vec[i]) % z;
  }
  assert(is_valid());
  return (*this);
}

perm_t&
perm_t::insert(const uint aPos, const perm_t& aPerm) {
  const uint z = aPerm.size();
  resize(z + 1);
  for(uint i = 0; i < aPos; ++i) {
    _vec[i] = aPerm[i];
  }
  _vec[aPos] = z;
  for(uint i = aPos + 1; i <= z; ++i) {
    _vec[i] = aPerm[i - 1];
  }
  return (*this);
}

perm_t&
perm_t::prepend(const perm_t& aPerm) {
  const uint z = aPerm.size();
  resize(z + 1);
  _vec[0] = z;
  for(uint i = 1; i <= z; ++i) {
    _vec[i] = aPerm[i-1];
  }
  return (*this);
}

perm_t&
perm_t::append(const perm_t& aPerm) {
  const uint z = aPerm.size();
  resize(z + 1);
  for(uint i = 0; i < z; ++i) {
    _vec[i] = aPerm[i];
  }
  _vec[z] = z;
  return (*this);
}

perm_t&
perm_t::swap(const uint i, const uint j) {
  assert(i < size());
  assert(j < size());
  std::swap(_vec[i], _vec[j]);
  return (*this);
}

void
perm_t::resize(const uint z) {
  _vec.resize(z);
}

bool
perm_t::is_valid() const {
  return (all() == pset(size()));
}

uint
perm_t::max_sum_dist(const uint z) {
  return ((z * z) / 2);
}

std::ostream&
perm_t::print(std::ostream& os) const {
  os << '[';
  bool lFirst = true;
  for(uint i = 0; i < size(); ++i) {
    if(!lFirst) {
      os << ' ';
    }
    os << val(i);
    lFirst = false;
  }
  os << ']';
  return os;
}

std::ostream&
operator<<(std::ostream& os, const perm_t& aPerm) {
  return aPerm.print(os);
}

std::ostream&
perm_t::print_props(std::ostream& os, const int aIndent) const {
  std::string lIndent(aIndent, ' ');
  os << lIndent << "is_identity    : " << std::setw(4) << is_identity() << std::endl
     << lIndent << "is_reverse     : " << std::setw(4) << is_reverse() << std::endl
     << lIndent << "is_involution  : " << std::setw(4) << is_involution() << std::endl
     << lIndent << "is_separable_L : " << std::setw(4) << is_separable_low() << std::endl
     << lIndent << "is_separable_H : " << std::setw(4) << is_separable_hi() << std::endl
     << lIndent << "is_irreducible : " << std::setw(4) << is_irreducible() << std::endl
     << lIndent << "is_connected   : " << std::setw(4) << is_connected() << std::endl
     << lIndent << "is_decomposable: " << std::setw(4) << is_decomposable() << std::endl
     << lIndent << "is_self_revcmpl: " << std::setw(4) << is_self_revcmpl() << std::endl
     << lIndent << "no_fixpoint    : " << std::setw(4) << no_fixpoint() << std::endl
     << lIndent << "no_inversion   : " << std::setw(4) << no_inversion() << std::endl
     << lIndent << "maxim_sum_dist : " << std::setw(4) << maximizes_sum_dist() << std::endl
     << lIndent << "sum_dist       : " << std::setw(4) << sum_dist() << std::endl
     << lIndent << "sign           : " << std::setw(4) << sign() << std::endl
     << lIndent << "degree         : " << std::setw(4) << degree() << std::endl
     ;
  return os;
}

std::ostream&
perm_t::print_bool_props(std::ostream& os, const uint64_t aPropSet) const {
  if(0 != (aPropSet & kIsId)) {
    os << ' ' << (is_identity() ? "id" : "--");
  }
  if(0 != (aPropSet & kIsRev)) {
    os << ' ' << (is_reverse() ? "rev" : "---");
  }
  if(0 != (aPropSet & kIsInvolution)) {
    os << ' ' << (is_involution() ? "inv" : "---");
  }
  if(0 != (aPropSet & kIsReducible)) {
    os << ' ' << (is_reducible() ? "red" : "---");
  }
  if(0 != (aPropSet & kIsIrreducible)) {
    os << ' ' << (is_irreducible() ? "ird" : "---");
  }
  if(0 != (aPropSet & kIsConnected)) {
    os << ' ' << (is_connected() ? "con" : "---");
  }
  if(0 != (aPropSet & kIsDisconnected)) {
    os << ' ' << (is_disconnected() ? "dcn" : "---");
  }
  if(0 != (aPropSet & kIsDecomposable)) {
    os << ' ' << (is_decomposable() ? "dcmp" : "----");
  }
  if(0 != (aPropSet & kIsIndecomposable)) {
    os << ' ' << (is_indecomposable() ? "idcp" : "----");
  }
  if(0 != (aPropSet & kIsDerangement)) {
    os << ' ' << (is_derangement() ? "nfx" : "---");
  }
  if(0 != (aPropSet & kIsSelfRevCmpl)) {
    os << ' ' << (is_self_revcmpl() ? "src" : "---");
  }
  if(0 != (aPropSet & kIsSeparableLow)) {
    os << ' ' << (is_separable_low() ? "sepL" : "----");
  }
  if(0 != (aPropSet & kIsSeparableHi)) {
    os << ' ' << (is_separable_hi() ? "sepH" : "----");
  }
  if(0 != (aPropSet & kIsSeparable)) {
    os << ' ' << (is_separable_hi() ? "sep" : "---");
  }
  if(0 != (aPropSet & kMaximizesSumDist)) {
    os << ' ' << (maximizes_sum_dist() ? "mxsd" : "----");
  }
  return os;
}


/*
 *  members of PermutationGenerator
 */

PermutationGenerator::PermutationGenerator(const uint z)
                     : _z(z), _N(factorial64(z)), _rank(0), _perm(z) {
  ftUnrankPermutation(_rank, _z, _perm.data()); 
}

PermutationGenerator&
PermutationGenerator::operator++() {
  ++_rank;
  if(_rank < _N) {
    ftUnrankPermutation(_rank, _z, _perm.data());
  }
  return (*this);
}

/*
 * members PermutationAnalyzer
 */

PermutationAnalyzer::PermutationAnalyzer() 
                    : _z(0), _perm(), _prop(),
                      _cycles(), _cycle_type_1(), _cycle_type_2() {
}

PermutationAnalyzer::PermutationAnalyzer(const perm_t& aPerm) 
                    : _z(aPerm.size()), _perm(aPerm), _prop(),
                      _cycles(), _cycle_type_1(), _cycle_type_2() {
  analyze_1(aPerm);
}


void
PermutationAnalyzer::analyze_1(const perm_t& aPerm) {
  _z    = aPerm.size();
  _perm = aPerm;
  _prop._no_fixpoint = aPerm.no_fixpoint();
  calc_cycles();
}

void
PermutationAnalyzer::calc_cycles() {
  pset_t lSet = 0;
  uint_vt lCycle;
  for(int i = z() - 1; i >= 0; --i) {
    if(test_bit<pset_t>(lSet, i)) {
      continue;
    }
    const uint lStop = i;
    uint lCurr = i;
    do {
      lSet |= (1 << lCurr);
      lCycle.push_back(lCurr);
      lCurr = perm(lCurr);
    } while(lStop != lCurr);
    _cycles.emplace_back(lCycle);
    lCycle.clear();
  }
  // reverse order in _cycles
  // standard form: 
  // 1) every cycle contains largest element first
  // 2) cycles are listed in increasing order of the first element
  for(uint i = 0; i < _cycles.size() / 2; ++i) {
    std::swap(_cycles[i], _cycles[_cycles.size() - 1 - i]);
  }
  // init cycle_type_1
  _cycle_type_1.resize(z());
  for(uint i = 0; i < z(); ++i) {
    _cycle_type_1[i] = 0;
  }
  for(uint i = 0; i < _cycles.size(); ++i) {
    ++(_cycle_type_1[_cycles[i].size() - 1]);
  }
  // init cycle_type_2
  _cycle_type_2.resize(_cycles.size());
  for(uint i = 0; i < _cycles.size(); ++i) {
    _cycle_type_2[i] = _cycles[i].size();
  }
}

uint
PermutationAnalyzer::max_cycle_length() const {
  uint lRes = 0;
  for(uint i = 0; i < no_cycles(); ++i) {
    if(cycle(i).size() > lRes) {
      lRes = cycle(i).size();
    }
  }
  return lRes;
}

uint
PermutationAnalyzer::no_cycles_of_length(const uint aLen) const {
  uint lRes = 0;
  for(uint i = 0; i < no_cycles(); ++i) {
    if(aLen == cycle(i).size()) {
      ++lRes;
    }
  }
  return lRes;
}

/*
 *  Mikos Bona: Combinatorics of Permutations: Lemma 3.66, p 112:
 *  pi has square root if and only if
 *  its unique decomposition into the product of distinct cycles
 *  contains
 *  for every even number of cycles of each even cycle length
 */

bool
PermutationAnalyzer::has_square_root() const {
  const uint lMaxCycleLength = max_cycle_length();
  for(uint lCycleLength = 2; lCycleLength <= lMaxCycleLength; lCycleLength += 2) {
    if(0x1 == (0x1 & no_cycles_of_length(lCycleLength))) {
      return false;
    }
  }
  return true;
}

std::ostream&
PermutationAnalyzer::print(std::ostream& os, const int aIndent) const {
  std::string lIndent(aIndent, ' ');
  std::cout << lIndent << "cycles:"; print_cycles(os) << std::endl;
  std::cout << lIndent << "cycle_type_1: " << _cycle_type_1 << std::endl;
  std::cout << lIndent << "cycle_type_2: " << _cycle_type_2 << std::endl;
  return os;
}

std::ostream&
PermutationAnalyzer::print_cycles(std::ostream& os) const {
  for(uint i = 0; i < _cycles.size(); ++i) {
    // const std::vector<uint>& lCycle = _cycles[i];
    // os << lCycle;
    // os << ' ' << '(' << lCycle << ')';
    os << '(' << _cycles[i] << ')';
    // uint_vt lX({1,2,3,4});
    // os << lX;
  }
  return os;
}

std::ostream&
PermutationAnalyzer::print_short(std::ostream& os) const {
  os << prop();
  return os;
}

std::ostream&
PermutationAnalyzer::prop_t::print_short(std::ostream& os) const {
  os << no_fixpoint();
  return os;
}

std::ostream&
operator<<(std::ostream& os, const PermutationAnalyzer& aPermAna) {
  return aPermAna.print_short(os);
}

std::ostream& 
operator<<(std::ostream& os, const PermutationAnalyzer::prop_t& aProp) {
  return aProp.print_short(os);
}

// factorial
uint64_t
factorial64(uint n) {
  uint64_t lRes = 1;
  for(uint i = 2; i <= n; ++i) {
    lRes *= i;
  }
  return lRes;
}

// concatenate (p2 \circ p1)(i) = p2(p1(i))
perm_t 
concat(const perm_t& aPerm1, const perm_t& aPerm2) {
  assert(aPerm1.size() == aPerm2.size());
  const uint z = aPerm1.size();
  perm_t lPermOut(z);
  for(uint i = 0; i < z; ++i) {
    lPermOut[i] = aPerm2[aPerm1[i]];
  }
  return lPermOut;
}

// } // end namespace


#ifndef PERMUTATION_HH
#define PERMUTATION_HH

#include "infra/glob_infra_standard_includes.hh"
#include "infra/rank_permutation.hh"
#include "infra/vector_ops.hh"

/*
 * class PermutationAnalyzer
 * analyzes permutation of {0,\ldots,z-1}
 * for some given z < 32.
 *
 * additionally there are some functions useful functions on permutations
 */

// namespace perm {

typedef uint64_t pset_t;

class perm_t {
  public:
    enum kind_t {
      kId            = 0, // identity
      kRev           = 1, // reverse permutation
      kSepIdPos      = 2, // 0   ... g   | g+1 ... z-1  (= pi_id)
      kSepIdNeg      = 3, // 0   ... g-1 | z-1 ... g+1
      kSepRevPos     = 4, // 0   ... g-1 | z-1 ... g+1
      kSepRevNeg     = 5, // z-1 ... g+1 | g-1 ... 0    (= pi_rev)
      kTransposition = 6, // 0 ... j .. i .. z-1
      kNoKind        = 7  // not initialized
    };
    enum unop_t {
      kCopy       =  0,
      kInverse    =  1,
      kReverse    =  2,
      kComplement =  3, // pi'(i) = [z-1 - pi(0), ... , z-1 - pi(n)]
      kInvRev     =  4, // inv(rev(pi))
      kRevInv     =  5, // rev(inv(pi))
      kConcRevInv =  6, // rev(pi) o inv(pi)
      kConcInvRev =  7, // inv(pi) o rev(pi) (= pi_rev)
      kSwap       =  8,
      kMove2Front =  9,
      kNoUnOp     = 10
    };
    enum binop_t {
      kConcat    = 0,
      kSumDirect = 1,
      kSumSchief = 2,
      kNoBinop   = 3
    };
    // boolean property flags
    enum prop_t {
      kIsId             = (((uint64_t) 1) <<  0),
      kIsRev            = (((uint64_t) 1) <<  1),
      kIsInvolution     = (((uint64_t) 1) <<  2),
      kIsReducible      = (((uint64_t) 1) <<  3),
      kIsIrreducible    = (((uint64_t) 1) <<  4), // 0 == no_fixpoint()
      kIsConnected      = (((uint64_t) 1) <<  5),
      kIsDisconnected   = (((uint64_t) 1) <<  6),
      kIsDecomposable   = (((uint64_t) 1) <<  7),
      kIsIndecomposable = (((uint64_t) 1) <<  8),
      kIsDerangement    = (((uint64_t) 1) <<  9), // 0 == no_fixpoint()
      kIsSelfRevCmpl    = (((uint64_t) 1) << 10), // pi(i) + pi(z-1-i) = z-1
      kIsSeparableLow   = (((uint64_t) 1) << 11), 
      kIsSeparableHi    = (((uint64_t) 1) << 12), 
      kIsSeparable      = (((uint64_t) 1) << 13), 
      kMaximizesSumDist = (((uint64_t) 1) << 14),
    };
  public:
    perm_t() : _vec() {}
    perm_t(const uint z) : _vec(z) {}
    perm_t(const uint z, const kind_t aKind);
    perm_t(const uint z, const kind_t aKind, const uint i, const uint j); // transposition
    perm_t(const uint z, const uint g, const kind_t aKind); // for aKind = kSepPos, kSepNeg
    perm_t(const perm_t& aPerm) : _vec(aPerm.vec()) {}
    perm_t(const uint_vt& aVec) : _vec(aVec) {}
    perm_t(const unop_t aUnOp, const perm_t& aPerm);
    perm_t(const unop_t aUnop, const perm_t& aPerm, const uint aVal); // for kMove2Front
    perm_t(const unop_t aUnOp, const perm_t& aPerm, const uint i, const uint j); // for kSwap
    // perm_t(const unop_t aUnOp, const uint i, const uint j, const perm_t& aPerm); // for kSwap
    perm_t(const perm_t& aPerm1, const perm_t& aPerm2, const binop_t aBinop);
    perm_t(const uint64_t aRank, const uint z);
  public:
    perm_t& init_id(const uint z);
    perm_t& init_rev(const uint z);
    perm_t& init_vec(const uint_vt& aVec);
    perm_t& init_sep_id_pos(const uint z, const uint g);
    perm_t& init_sep_id_neg(const uint z, const uint g);
    perm_t& init_sep_rev_pos(const uint z, const uint g);
    perm_t& init_sep_rev_neg(const uint z, const uint g);
    perm_t& unrank(const uint64_t aRank, const uint z);
  public:
    inline const uint_vt& vec() const { return _vec; }
    inline uint  size() const { return _vec.size(); }
    inline uint  val(const uint i) const { return _vec[i]; }
           uint  inverse_of(const uint aVal) const;  // pi^{-1}(i)
    inline uint  operator()(const uint i) const { return _vec[i]; }
    inline uint& operator()(const uint i)       { return _vec[i]; }
    inline uint  operator[](const uint i) const { return _vec[i]; }
    inline uint& operator[](const uint i)       { return _vec[i]; }
    inline const uint* data() const { return _vec.data(); }
    inline       uint* data()       { return _vec.data(); }
  public:
    bool    is_identity() const;
    bool    is_reverse() const;
    bool    is_involution() const; // pi \circ pi = id
    bool    is_reducible() const;
    bool    is_irreducible() const;
    bool    is_connected() const;
    bool    is_disconnected() const;
    bool    is_indecomposable() const;
    bool    is_decomposable() const;
    bool    is_derangement() const;
    bool    is_self_revcmpl() const;
    bool    is_separable_low_at(const uint g) const;
    bool    is_separable_low() const;
    bool    is_separable_hi_at(const uint g) const;
    bool    is_separable_hi() const;
    bool    is_separable_at(const uint g) const;
    bool    is_separable() const;
    bool    maximizes_sum_dist() const;
    bool    has_square_root_slow() const; // exists pi': pi' o pi' = pi
    bool    has_square_root() const; // exists pi': pi' o pi' = pi
    pset_t  get_separation_points() const; // as bitvector
    uint    no_fixpoint() const;
    uint    no_inversion() const;
    int     sign() const;
    uint    degree() const; // min_k pi^k = id
    uint    sum_dist() const; // sum_i |pi(i) - i|
    uint    sum_dist(const perm_t& aPerm) const; // sum_i |pi(i) - pi'(i)|
    uint    kendall_tau_distance(const perm_t& aPerm) const;
    pset_t  all() const;
    pset_t  pset(const uint aEnd) const;
    uint    inflection_point(const uint64_t aIdx) const;
    uint    inflection_point(const uint aZeroPos1, const uint aZeroPos2) const;
  public:
    bool is_equal_to(const perm_t& aPerm) const;
    inline bool operator==(const perm_t& aPerm) const { return is_equal_to(aPerm); }
    bool disjoint(const perm_t& aPerm) const;
    bool is_inverse_of(const perm_t& aPerm) const;
    bool is_reverse_of(const perm_t& aPerm) const;
    bool is_complement_of(const perm_t& aPerm) const;
    bool commute(const perm_t& aPerm) const;
    bool inverse_commute(const perm_t& aPerm) const; // pi_1^{-1} o pi2 = pi_2^{-1} pi_1
    bool is_conjugate_of_slow(const perm_t& aPerm) const; // exists pi_3 : pi_1 = pi_3 \circ pi_2 \circ pi_3^-1
    bool have_common_subset_prefix(const perm_t& aPerm) const; // csp
    bool is_separated_from_at(const uint g, const perm_t& aPerm) const; // sep(g,pi,pi') old definition
    bool is_separated_from(const perm_t& aPerm) const;
    uint no_separated_from(const perm_t&, int* aSepPointOut = 0) const;
    uint get_all_separators(const perm_t&) const;
    bool is_separated_from_at_new(const uint g, const perm_t& aPerm) const; // sep(g,pi,pi') new definition
    uint no_common(const perm_t&) const;
  public:
    perm_t& set_to_inverse_of(const perm_t& aPerm); 
    perm_t& set_to_reverse_of(const perm_t& aPerm);
    perm_t& set_to_inverse_of_reverse_of(const perm_t& aPerm);
    perm_t& set_to_reverse_of_inverse_of(const perm_t& aPerm);
    perm_t& set_to_conc_of_inverse_and_reverse_of(const perm_t& aPerm);
    perm_t& set_to_conc_of_reverse_and_inverse_of(const perm_t& aPerm);
    perm_t& set_to_complement_of(const perm_t& aPerm);
    perm_t& set_to_concat_of(const perm_t& aPerm1, const perm_t& aPerm2); // p2 \circ p1
    perm_t& set_to_sum_direct_of(const perm_t& aPerm1, const perm_t& aPerm2); // direct sum
    perm_t& set_to_sum_schief_of(const perm_t& aPerm1, const perm_t& aPerm2); // skew sum
  public:
    perm_t& multiply_by(const uint a);
    perm_t& insert(const uint aPos, const perm_t& aPerm);
    perm_t& prepend(const perm_t& aPerm);
    perm_t& append(const perm_t& aPerm);
    perm_t& swap(const uint i, const uint j);
  public:
    void resize(const uint z);
    bool is_valid() const; // for check purposes
  public:
    static uint max_sum_dist(const uint z);
  public:
    std::ostream& print(std::ostream&) const;
    std::ostream& print_props(std::ostream&, const int aIndent) const;
    std::ostream& print_bool_props(std::ostream&, const uint64_t aPropSet) const;
  private:
    uint_vt _vec;
};

class PermutationGenerator {
  public:
    PermutationGenerator(const uint z);
  public:
    inline bool isValid() const { return _rank < _N; }
    inline const perm_t& operator*() const { return _perm; }
    PermutationGenerator& operator++();
  private:
    const uint     _z;
    const uint64_t _N;
          uint64_t _rank;
          perm_t   _perm;
};

class PermutationAnalyzer {
  public:
    struct prop_t {
      uint _no_fixpoint;

      inline uint no_fixpoint() const { return _no_fixpoint; }
      std::ostream& print_short(std::ostream& os) const;
    };
  public:
    PermutationAnalyzer();
    PermutationAnalyzer(const perm_t& aPerm);
  public:
    void analyze_1(const perm_t& aPerm);
  public:
    void calc_cycles();
  public:
    inline       uint     z() const { return _z; }
    inline const perm_t&  perm() const { return _perm; }
    inline       uint     perm(const uint i) const { return _perm(i); }
    inline const prop_t&  prop() const { return _prop; }
    inline       uint     no_cycles() const { return _cycles.size(); }
    inline const uint_vt& cycle(const uint i) const { return _cycles[i]; }
  public:
    uint max_cycle_length() const;
    uint no_cycles_of_length(const uint n) const;
    bool has_square_root() const;
  public:
    std::ostream& print(std::ostream& os, const int aIndent) const;
    std::ostream& print_cycles(std::ostream& os) const;
    std::ostream& print_short(std::ostream& os) const;
  private:
    uint     _z;
    perm_t   _perm;
    prop_t   _prop;
    uint_vvt _cycles; // in standard form
    uint_vt  _cycle_type_1; // [i] = number of cycles of length i   // page 2 middle
    uint_vt  _cycle_type_2; // contains all the sizes of the cycles // page 2 bottom
};


uint64_t factorial64(uint n);  // n!
perm_t   concat(const perm_t& aPerm1, const perm_t& aPerm2); // concatenate p1 \circ p2


std::ostream& operator<<(std::ostream& os, const perm_t&);
std::ostream& operator<<(std::ostream& os, const PermutationAnalyzer& aPermAna);
std::ostream& operator<<(std::ostream& os, const PermutationAnalyzer::prop_t& aProp);

// } // end namespace

#endif

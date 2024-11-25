#ifndef SRC_INFRA_BIT_SUBSETS_PDEP_HH
#define SRC_INFRA_BIT_SUBSETS_PDEP_HH

// USAGE:
// for(BvTrueNonEmptySubsetsPdep<uint32_t x(S); x.isValid(); ++x) {
//      uint32_t S1 = *x; // extract subset
// }

// only works on processors with bmi2 

#ifdef __BMI2__

// this one generates S \subset X, S \neq \emptyset
template<class Tuint>
class BvTrueNonEmptySubsetsPdep {
  public:
    BvTrueNonEmptySubsetsPdep(const Tuint x) : _set(x), 
                                               _end((((Tuint) 1) << number_of_bits_set<Tuint>(x)) - 1),
                                               _idx(1),
                                               _subset(bit_distribute<Tuint>(_idx, _set)) {}
  public:
        inline void init(Tuint x) {
                      _set = x;
                      _end = ((((Tuint) 1) << number_of_bits_set<Tuint>(x)) - 1);
                      _idx = 1;
                      _subset = bit_distribute<Tuint>(_idx, _set); 
                    }
        inline void reset() { 
                      _end = ((((Tuint) 1) << number_of_bits_set<Tuint>(_set)) - 1);
                      _idx = 1;
                      _subset = bit_distribute<Tuint>(_idx, _set); 
                    }
        inline BvTrueNonEmptySubsetsPdep& operator++() { 
                                            ++_idx;
                                            _subset = bit_distribute<Tuint>(_idx, _set);
                                            return (*this); 
                                          }
        inline bool isValid() const { return (_idx < _end); }
        inline Tuint operator*() { return _subset; }
        inline Tuint get() const { return _subset; }
        inline Tuint getFullSet() const { return _set; }
        inline Tuint idx() const { return _idx; }
  private:
    Tuint _set;
    Tuint _end;
    Tuint _idx;
    Tuint _subset;
};

template<class Tuint>
class BvNonEmptySubsetsPdep {
  public:
    BvNonEmptySubsetsPdep(const Tuint x) : _set(x),
                                           _end((((Tuint) 1) << number_of_bits_set<Tuint>(x))),
                                           _idx(1),
                                           _subset(bit_distribute<Tuint>(_idx, _set)) {}
  public:
        inline void init(Tuint x) {
                      _set = x;
                      _end = ((((Tuint) 1) << number_of_bits_set<Tuint>(x)));
                      _idx = 1;
                      _subset = bit_distribute<Tuint>(_idx, _set);
                    }
        inline void reset() { 
                      _end = ((((Tuint) 1) << number_of_bits_set<Tuint>(_set)));
                      _idx = 1;
                      _subset = bit_distribute<Tuint>(_idx, _set);
                    }
        inline BvNonEmptySubsetsPdep& operator++() { 
                                        ++_idx;
                                        _subset = bit_distribute<Tuint>(_idx, _set);
                                        return (*this);
                                      }
        inline bool isValid() const { return (_idx < _end); }
        inline Tuint operator*() { return _subset; }
        inline Tuint get() const { return _subset; }
        inline Tuint getFullSet() const { return _set; }
        inline Tuint idx() const { return _idx; }
  private:
    Tuint _set;
    Tuint _end;
    Tuint _idx;
    Tuint _subset;

};

// this one generates S \subset X, S \neq X
template<class Tuint>
class BvTrueSubsetsPdep {
  public:
    BvTrueSubsetsPdep(const Tuint x) : _set(x),
                                       _end((((Tuint) 1) << number_of_bits_set<Tuint>(x)) - 1),
                                       _idx(0),
                                       _subset(bit_distribute<Tuint>(_idx, _set)) {}
  public:
        inline void init(Tuint x) {
                      _set = x;
                      _end = ((((Tuint) 1) << number_of_bits_set<Tuint>(x)) - 1);
                      _idx = 0;
                      _subset = bit_distribute<Tuint>(_idx, _set);
                    }
        inline void reset() { 
                      _end = ((((Tuint) 1) << number_of_bits_set<Tuint>(_set)) - 1);
                      _idx = 0;
                      _subset = bit_distribute<Tuint>(_idx, _set);
                    }
        inline BvTrueSubsetsPdep& operator++() { 
                                     ++_idx;
                                     _subset = bit_distribute<Tuint>(_idx, _set);
                                     return (*this);
                                   }
        inline bool isValid() const { return (_idx < _end); }
        inline Tuint operator*() { return _subset; }
        inline Tuint get() const { return _subset; }
        inline Tuint getFullSet() const { return _set; }
        inline Tuint idx() const { return _idx; }
  private:
    Tuint _set;
    Tuint _end;
    Tuint _idx;
    Tuint _subset;
};


// this one generates all subsets, including the full set and the empty set
template<class Tuint>
class BvAllSubsetsPdep {
  public:
    BvAllSubsetsPdep(const Tuint x) : _set(x),
                                      _end((((Tuint) 1) << number_of_bits_set<Tuint>(x))),
                                      _idx(0),
                                      _subset(bit_distribute<Tuint>(_idx, _set)) {}
  public:
        inline void init(Tuint x) {
                      _set = x;
                      _end = ((((Tuint) 1) << number_of_bits_set<Tuint>(x)));
                      _idx = 0;
                      _subset = bit_distribute<Tuint>(_idx, _set);
                    }
        inline void reset() { 
                      _end = ((((Tuint) 1) << number_of_bits_set<Tuint>(_set)));
                      _idx = 0;
                      _subset = bit_distribute<Tuint>(_idx, _set);
                    }
        inline BvAllSubsetsPdep& operator++() { 
                                   ++_idx;
                                   _subset = bit_distribute<Tuint>(_idx, _set);
                                   return (*this);
                                 }
        inline bool isValid() const { return (_idx < _end); }
        inline Tuint operator*() { return _subset; }
        inline Tuint get() const { return _subset; }
        inline Tuint getFullSet() const { return _set; }
        inline Tuint idx() const { return _idx; }
  private:
    Tuint _set;
    Tuint _end;
    Tuint _idx;
    Tuint _subset;
};

#endif

#endif

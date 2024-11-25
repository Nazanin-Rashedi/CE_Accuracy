#ifndef INFRA_STRING_DICTIONARY_HASHED_HH
#define INFRA_STRING_DICTIONARY_HASHED_HH

#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <inttypes.h>
#include "infra/simpleexception.hh"
#include "infra/simplecharcontainer.hh"

class StringDictionaryHashed {
  public:
    class StrEq {
      public:
        bool operator()(const char* x, const char*y) const {
               return (0 == strcmp(x,y));
             }
    };
    class StrHash {
      public:
        uint64_t operator()(const char* s) const { // hashpjw
                   int64_t h = 0;
                   int64_t g;
                   for(const char* p = s; *p != '\0'; p++) {
                     h = (h << 4) + (*p);
                     if((g = h & 0xf00000000000000L)) {
                       h ^= (g >> 56);
                       h ^= g;
                     }
                   }
                   return h;
                 }
    };
  public:
    struct entry_t {
             entry_t() : _cptr(0), _mult(0) {}
               inline const char* cptr() const { return _cptr; }
               inline uint64_t    mult() const { return _mult; }
               char*    _cptr; // character pointer to string in simple char container
               uint64_t _mult; // multiplicity of string
           };
    typedef std::unordered_map<const char*, uint64_t, StrHash, StrEq> hmap_t; // maps char* to index in vector
    typedef std::vector<entry_t> str_vt;
  public:
    StringDictionaryHashed(const uint64_t aExpectedNoEntries, const uint32_t aChunkSize);
    ~StringDictionaryHashed();
  public:
    uint64_t insert(const char* aBegin, const char* aEnd);
    uint64_t get_idx(const char* aBegin, const char* aEnd);  // if necessary: inserts
    bool     contains(const char* aBegin) const; // must be 0-terminated
    const entry_t* lookup(const char* aBegin) const;   // returns  0 if not found, must be 0-terminated
    int64_t        lookup_id(const char* aBegin) const; // returns  -1 if not found, must be 0-terminated
  public:
    inline const entry_t&   get_entry(const uint64_t aIdx) const { return _idx2entry[aIdx]; }
    inline const char*      get_string(const uint64_t aIdx) const { return _idx2entry[aIdx].cptr(); }
  private:
    entry_t* lookup_nc(const char* aBegin);   // returns  0 if not found, must be 0-terminated
  public:
    inline uint64_t size() const { return _idx2entry.size(); }
    inline uint64_t noDistinctValues() const { return _idx2entry.size(); }
    inline uint64_t cardinality() const { return _count; }
    inline uint64_t multiplicity(const uint64_t aIdx) const { return _idx2entry[aIdx].mult(); }
  public:
    std::ostream& print(std::ostream& os) const;
  private:
    SimpleCharContainer _container;
    hmap_t              _str2idx;
    str_vt              _idx2entry;
    uint64_t            _count; // counts inserts
};


#endif

#include "StringDictionaryHashed.hh"

StringDictionaryHashed::StringDictionaryHashed(const uint64_t aExpectedNoEntries, const uint32_t aLog2ChunkSize)
                 : _container(aLog2ChunkSize),
                   _str2idx(aExpectedNoEntries),
                   _idx2entry(aExpectedNoEntries),
                   _count(0) {
  _str2idx.clear();
  _idx2entry.clear();
}

StringDictionaryHashed::~StringDictionaryHashed() {}

uint64_t
StringDictionaryHashed::insert(const char* aBegin, const char* aEnd) {
  // allow for empty strings

  // if(aEnd >= aBegin) {
  //   throw SimpEx1("empty string inserted");
  // }

  // do not allow strings with negative length
  if(aEnd < aBegin) {
    throw SimpEx1("end < begin of string");
  }

  // check if string fits into one chunk
  if(aEnd > aBegin) {
    if((uint64_t) (aEnd - aBegin) > _container.chunkSize()) {
      std::cerr << "chunk size: " << _container.chunkSize() << std::endl;
      std::cerr << "string size: " << (uint64_t) (aEnd - aBegin) << std::endl;
      throw SimpEx1("string too long");
    }
  }

  // dirty: enforce null-terminatedness
  char* x = (char*) aEnd;
  char  c = (*x);
  (*x) = '\0';


  // std::cout << "to insert: " << aBegin << " : ";

  hmap_t::iterator lIter = _str2idx.find(aBegin);
  uint64_t lRes = 0;
  if(_str2idx.end() == lIter) {
    // std::cout << "new" << " : ";
    entry_t lEntry;
    lEntry._cptr = (char*) _container.insert(aBegin, aEnd);
    lEntry._mult = 1;
    // std::cout << lEntry._cptr << " : " << StrHash()(lEntry._cptr) << " : " ;
    const uint64_t lIdx = _idx2entry.size();
    // std::cout << lIdx << " : " << _str2idx.size() << " : " << _idx2entry.size() << std::endl;
    _str2idx.insert(std::pair<const char*, uint64_t>(lEntry._cptr, lIdx));
    _idx2entry.push_back(lEntry);
    lRes = lIdx;
  } else {
    const uint64_t lIdx = (*lIter).second;
    entry_t& e = _idx2entry[lIdx];
    ++(e._mult);
    lRes = lIdx;
    // std::cout << "old" << "  " << lIdx << std::endl;
  }
  (*x) = c;
  ++_count;
  return lRes;
}


StringDictionaryHashed::entry_t*
StringDictionaryHashed::lookup_nc(const char* aBegin) {
  hmap_t::const_iterator lIter = _str2idx.find(aBegin);
  if(_str2idx.end() == lIter) {
    return 0;
  }
  return &(_idx2entry[(*lIter).second]);
}

int64_t
StringDictionaryHashed::lookup_id(const char* aBegin) const {
  hmap_t::const_iterator lIter = _str2idx.find(aBegin);
  if(_str2idx.end() == lIter) {
    return (uint64_t) -1;
  }
  return (int64_t) ((*lIter).second);
}

uint64_t
StringDictionaryHashed::get_idx(const char* aBegin, const char* aEnd) {
  return insert(aBegin, aEnd);
}


const StringDictionaryHashed::entry_t*
StringDictionaryHashed::lookup(const char* aBegin) const {
  hmap_t::const_iterator lIter = _str2idx.find(aBegin);
  if(_str2idx.end() == lIter) {
    return 0;
  }
  return &(_idx2entry[(*lIter).second]);
}


bool
StringDictionaryHashed::contains(const char* aBegin) const {
  return (_str2idx.end() != _str2idx.find(aBegin));
}


std::ostream&
StringDictionaryHashed::print(std::ostream& os) const {
  std::cout << "_idx2entry: " << std::endl;
  for(uint64_t i = 0; i < _idx2entry.size(); ++i) {
    os << std::setw(4) << i << " : "
       << std::setw(4) << _idx2entry[i]._mult <<  " : "
       << std::setw(4) << _idx2entry[i]._cptr << std::endl;
  }

  std::cout << "_str2idx: " << std::endl;
  for(hmap_t::const_iterator lIter = _str2idx.begin(); _str2idx.end() != lIter; ++lIter) {
    const entry_t& e = _idx2entry[(*lIter).second];
    os << std::setw(4) << (*lIter).second  << " : "
       << std::setw(4) << e._mult <<  " : "
       << std::setw(4) << e._cptr << std::endl;
  }
  os << std::endl;

  return os;
}



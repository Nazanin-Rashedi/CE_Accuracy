#ifndef MIN_MAX_WITH_AND_WITHOUT_BRANCH_HH
#define MIN_MAX_WITH_AND_WITHOUT_BRANCH_HH


//  in: x,y
// out: x=min(x,y), y = max(x,y)

template<class T>
inline void
minmaxbranch(T& x, T& y) {
  if(x > y) {
    const T z = x;
    x = y;
    y = z;
  }
}


// tuts nur fuer int
// und ist eh langsamer (ca. 30%)

template<class T>
inline void
minmaxnobranch(T& x, T& y) {
  const T s = x + y;
  const T d = (x - y) & 0x7FFFFFFF;
  x = (s - d) / 2;
  y = (s + d) / 2;
}



#endif

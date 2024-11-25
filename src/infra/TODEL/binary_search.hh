#ifndef INFRA_BINARY_SEARCH_HH
#define INFRA_BINARY_SEARCH_HH



// returns index idx, caller must still check whether arr[idx] contains key
// could also add a return parameter and make binary_search it bool

template<typename Telem, typename Tidx>
Tidx
binary_search(const Telem& key, const Telem* arr, const Tidx n) {
  Tidx lo  = 0;
  Tidx hi  = n - 1;
  Tidx mid = 0;
  while(lo <= hi) {
    mid = (lo + hi) / 2;
    // std::cout << "     in loop: lo/mid/hi:  " << lo << '/' << mid << '/' << hi << std::endl;
    if(arr[mid] < key) {
      lo = mid + 1;
    } else
    if(arr[mid] > key) {
      hi = mid - 1;
    } else {
      break;
    }
  }
  // const bool lFound = (lo <= hi);
  // std::cout << "  after loop: lo/mid/hi:  " << lo << '/' << mid << '/' << hi 
  //           << "  " << (lFound ? "found" : "not found") << std::endl;

  return mid;
}


#endif


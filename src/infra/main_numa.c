#include <stdio.h>
#include <numa.h>


void
test0() {
  int lNumaAvailable = numa_available();
  printf("numa available: %d\n", lNumaAvailable);

  if(-1 == lNumaAvailable) {
    printf("no numa support available.\n");
    return;
  }


  int lNoNumaNodes = numa_num_configured_nodes();
  printf("#numa nodes: %d\n", lNoNumaNodes);
  int lNoNumaCpus = numa_num_configured_cpus();
  printf("#numa cpus : %d\n", lNoNumaCpus);

  struct bitmask* lNumaAllCpus = numa_all_cpus_ptr;
  
}



int
main() {
  test0();
  return 0;
}


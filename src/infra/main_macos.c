#include <stdio.h>
#include <inttypes.h>
#include <sys/types.h>

/*
 *  sysctl on linux is deprecated and no longer supported
 *  use /proc/sys instead
 */

// /Library/Developer/CommandLineTools/SDKs/MacOSX11.1.sdk/usr/include/sys/sysctl.h
#include <sys/sysctl.h>

// /Library/Developer/CommandLineTools/SDKs/MacOSX11.1.sdk/usr/include/mach/machine.h
// CPUFAMILY_ARM_FIRESTORM_ICESTORM for 'Apple M1'

#include <mach/machine.h>

void
test0() {
  printf("=== test0 ===\n");
  int    lPageSize = 0; 
  size_t lSizeOfInt = sizeof(lPageSize);
  int    lName[2];
  lName[0] = CTL_HW;
  lName[1] = HW_PAGESIZE;
  int lRc = sysctl(lName, 2, &lPageSize, &lSizeOfInt, 0, 0);
  if(lRc) {
    printf("  something went wrong(pagesize): %d\n", lRc);
  }
  printf("  page size = %d\n", lPageSize);
  char   lBuff[100];
  size_t lBuffSize = 100;
  lName[1] = HW_MACHINE;
  lRc = sysctl(lName, 2, lBuff, &lBuffSize, 0, 0);
  if(lRc) {
    printf("  something went wrong(machine): %d\n", lRc);
  } else {
    printf("  machine = %s\n", lBuff);
  }
  lName[1] = HW_MODEL;
  lBuffSize = 100;
  lRc = sysctl(lName, 2, lBuff, &lBuffSize, 0, 0);
  if(lRc) {
    printf("  something went wrong(model): %d\n", lRc);
  } else {
    printf("  model = %s\n", lBuff);
  }
  lName[1] = HW_NCPU;
  int lNoHwThread = 0;
  lSizeOfInt = sizeof(int);
  lRc = sysctl(lName, 2, &lNoHwThread, &lSizeOfInt, 0, 0);
  if(lRc) {
    printf("  something went wrong(nproc): %d\n", lRc);
  }
  printf("  nohwthread = %d\n", lNoHwThread);
  lName[1] = HW_BYTEORDER; 
  int lByteOrder = -1;
  lSizeOfInt = sizeof(int);
  lRc = sysctl(lName, 2, &lByteOrder, &lSizeOfInt, 0, 0);
  if(lRc) {
    printf("  something went wrong(byteorder): %d\n", lRc);
  }
  printf("  byteorder = %d\n", lByteOrder);

  lName[1] = HW_PHYSMEM; 
  uint64_t lTotMem = 0;
  lSizeOfInt = sizeof(uint64_t);
  lRc = sysctl(lName, 2, &lTotMem, &lSizeOfInt, 0, 0);
  if(lRc) {
    printf("  something went wrong(physmem): %d\n", lRc);
  }
  printf("  tot mem = %llu\n", lTotMem);

  lName[1] = HW_MACHINE_ARCH;
  lBuffSize = 100;
  lRc = sysctl(lName, 2, lBuff, &lBuffSize, 0, 0);
  if(lRc) {
    printf("  something went wrong(arch): %d\n", lRc);
  } else {
    printf("  machine arch = %s\n", lBuff);
  }

  lName[1] = HW_BUS_FREQ;
  int lBusFreq = -1;
  lSizeOfInt = sizeof(int);
  lRc = sysctl(lName, 2, &lBusFreq, &lSizeOfInt, 0, 0);
  if(lRc) {
    printf("  something went wrong (busfreq): %d\n", lRc);
  } else {
    printf("  bus freq = %d\n", lBusFreq);
  }
  
  lName[1] = HW_CPU_FREQ;
  uint32_t lCpuFreq = -1;
  lSizeOfInt = sizeof(int);
  lRc = sysctl(lName, 2, &lCpuFreq, &lSizeOfInt, 0, 0);
  if(lRc) {
    printf("  something went wrong (cpufreq): %d\n", lRc);
  } else {
    printf("  cpu freq = %u\n", lCpuFreq);
  }
  
  lName[1] = HW_CACHELINE;
  uint32_t lCacheLineSize = -1;
  lSizeOfInt = sizeof(int);
  lRc = sysctl(lName, 2, &lCacheLineSize, &lSizeOfInt, 0, 0);
  if(lRc) {
    printf("  something went wrong (cachelinesize): %d\n", lRc);
  } else {
    printf("  cacheline size = %u\n", lCacheLineSize);
  }

  lName[1] = HW_L1ICACHESIZE;
  uint32_t lCache_L1i = 0;
  lSizeOfInt = sizeof(int);
  lRc = sysctl(lName, 2, &lCache_L1i, &lSizeOfInt, 0, 0);
  if(lRc) {
    printf("  something went wrong(L1i): %d\n", lRc);
  } else {
    printf("  L1i size = %u\n", lCache_L1i);
  }

  lName[1] = HW_L1DCACHESIZE;
  uint32_t lCache_L1d = 0;
  lSizeOfInt = sizeof(int);
  lRc = sysctl(lName, 2, &lCache_L1d, &lSizeOfInt, 0, 0);
  if(lRc) {
    printf("  something went wrong(L1d): %d\n", lRc);
  } else {
    printf("  L1d size = %u\n", lCache_L1d);
  }

  lName[1] = HW_L2CACHESIZE;
  uint32_t lCache_L2 = 0;
  lSizeOfInt = sizeof(int);
  lRc = sysctl(lName, 2, &lCache_L2, &lSizeOfInt, 0, 0);
  if(lRc) {
    printf("  something went wrong(L2): %d\n", lRc);
  } else {
    printf("  L2  size = %u\n", lCache_L2);
  }

  lName[1] = HW_L3CACHESIZE;
  uint32_t lCache_L3 = 0;
  lSizeOfInt = sizeof(int);
  lRc = sysctl(lName, 2, &lCache_L3, &lSizeOfInt, 0, 0);
  if(lRc) {
    printf("  something went wrong(L3): %d\n", lRc);
  } else {
    printf("  L3  size = %u\n", lCache_L3);
  }

  lName[1] = HW_MEMSIZE;
  uint64_t lMemSize = 0;
  lSizeOfInt = sizeof(uint64_t);
  lRc = sysctl(lName, 2, &lMemSize, &lSizeOfInt, 0, 0);
  if(lRc) {
    printf("  something went wrong(memsize): %d\n", lRc);
  } else {
    printf("  MM  size = %llu\n", lMemSize);
  }

  lName[1] = HW_AVAILCPU;
  uint32_t lAvailCpu = 0;
  lSizeOfInt = sizeof(uint64_t);
  lRc = sysctl(lName, 2, &lAvailCpu, &lSizeOfInt, 0, 0);
  if(lRc) {
    printf("  something went wrong(availcpu): %d\n", lRc);
  } else {
    printf("  Avail CPU = %u\n", lAvailCpu);
  }

}

void
test1() {
  printf("=== test1 ===\n");
  int    lPageSize = 0;
  size_t lSizeOfInt = sizeof(lPageSize);
  int lRc = sysctlbyname("hw.pagesize", &lPageSize, &lSizeOfInt, 0, 0);
  if(lRc) {
    printf("  something went wrong: %d\n", lRc);
  }
  printf("  page size = %d\n", lPageSize);

  uint64_t lCpuFreqCurr = 0;
  uint64_t lCpuFreqMax  = 0;
  uint64_t lCpuFreqMin  = 0;
  lSizeOfInt = sizeof(uint64_t);
  lRc = sysctlbyname("hw.cpufrequency", &lCpuFreqCurr, &lSizeOfInt, 0, 0);
  if(lRc) {
    printf("  something went wrong(cpufreq): %d\n", lRc);
  }
  printf("  cpu freq curr = %llu (B=%zu)\n", lCpuFreqCurr, lSizeOfInt);
  lSizeOfInt = sizeof(uint64_t);
  lRc = sysctlbyname("hw.cpufrequency_min", &lCpuFreqMin, &lSizeOfInt, 0, 0);
  if(lRc) {
    printf("  something went wrong(cpufreqmin): %d\n", lRc);
  }
  printf("  cpu freq min  = %llu (B=%zu)\n", lCpuFreqMin, lSizeOfInt);
  lSizeOfInt = sizeof(uint64_t);
  lRc = sysctlbyname("hw.cpufrequency_max", &lCpuFreqMax, &lSizeOfInt, 0, 0);
  if(lRc) {
    printf("  something went wrong(cpufreqmax): %d\n", lRc);
  }
  printf("  cpu freq max  = %llu (B=%zu)\n", lCpuFreqMax, lSizeOfInt);

  int lHwCpuFamily = -1;
  lSizeOfInt = sizeof(int);
  lRc = sysctlbyname("hw.cpufamily", &lHwCpuFamily, &lSizeOfInt, 0, 0);
  if(lRc) {
    printf("  something weng wrong(cpufam): %d\n", lRc);
  }
  printf("  cpu family = 0x%08x (%d)\n", lHwCpuFamily, lHwCpuFamily);

  int lHwCpuType = -1;
  lSizeOfInt = sizeof(int);
  lRc = sysctlbyname("hw.cputype", &lHwCpuType, &lSizeOfInt, 0, 0);
  if(lRc) {
    printf("  something went wrong(cputype): %d\n", lRc);
  }
  printf("  cpu     type = 0x%08x (%d)\n", lHwCpuType, lHwCpuType);

  int lHwCpuSubType = -1;
  lSizeOfInt = sizeof(int);
  lRc = sysctlbyname("hw.cpusubtype", &lHwCpuSubType, &lSizeOfInt, 0, 0);
  if(lRc) {
    printf("  something went wrong(cpusubtype): %d\n", lRc);
  }
  printf("  cpu sub type = 0x%08x (%d) \n", lHwCpuSubType, lHwCpuSubType);

  int lHwCpuThreadType = -1;
  lSizeOfInt = sizeof(int);
  lRc = sysctlbyname("hw.cputhreadtype", &lHwCpuThreadType, &lSizeOfInt, 0, 0);
  if(lRc) {
    printf("  something went wrong(cputhreadtype): %d\n", lRc);
  }
  printf("  cpu thread type = %d \n", lHwCpuThreadType);

  
  int lHwNoProcessorPackages = -1;
  lSizeOfInt = sizeof(int);
  lRc = sysctlbyname("hw.packages", &lHwNoProcessorPackages, &lSizeOfInt, 0, 0);
  if(lRc) {
    printf("  something went wrong(packages): %d\n", lRc);
  }
  printf("  no cpu packages = %d \n", lHwNoProcessorPackages);
 
  int lHwNoPhysCpu = 0; 
  lSizeOfInt = sizeof(int);
  lRc = sysctlbyname("hw.physicalcpu", &lHwNoPhysCpu, &lSizeOfInt, 0, 0);
  if(lRc) {
    printf("  something went wrong(physcpu): %d\n", lRc);
  }
  printf("  no cpu phys = %d \n", lHwNoPhysCpu);

  int lHwNoLogCpu  = 0; 
  lSizeOfInt = sizeof(int);
  lRc = sysctlbyname("hw.logicalcpu", &lHwNoLogCpu, &lSizeOfInt, 0, 0);
  if(lRc) {
    printf("  something went wrong(logcpu): %d\n", lRc);
  }
  printf("  no cpu log  = %d \n", lHwNoLogCpu);

  size_t lBuffSize = 99;
  char lBuff[100];
  lRc = sysctlbyname("machdep.cpu.brand_string", lBuff, &lBuffSize, 0, 0);
  if(lRc) {
    printf("  something went wrong(cpubrandstring): %d\n", lRc);
  }
  printf("  cpu brand string = %s\n", lBuff);  
} 

int
main() {
  test0();
  test1();
  return 0;
}


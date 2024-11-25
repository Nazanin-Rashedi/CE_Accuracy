#include <stdio.h>
#include <math.h>

#include <cpuid.h>

void cpuid(int *a, int *b, int *c, int *d)
{
  int keeper;
  __asm__ __volatile__ (" mov &#37;5, %%eax;"
                        " mov %6, %%ecx;"
                        " mov %%ebx, %0;"
                        " cpuid;"
                        " mov %%eax, %1;"
                        " mov %%ebx, %2;"
                        " mov %%ecx, %3;"
                        " mov %%edx, %4;"
                        " mov %7, %%ebx"
                      : "=r" (keeper), "=a" (*a), "=r" (*b), "=c" (*c), "=d" (*d)   // output
                      : "a" (*a), "c" (*c), "r" (keeper) );                         // input
}

 
 
int bit_check(int *x, int bit)
{
   if (((*x >> bit) & 0x01) == 0)
      return 0;      /* bit ain't set */
   else
      return 1;      /* bit is set */
}
 
 
int read_val(int *reg, int startbit, int endbit)
{
  int i,val=0;
 
  for (i=0; i<=(endbit-startbit); i++)
  {
    if ( bit_check(reg,startbit+i) == 1 ) val=val+pow(2,i);
  }
  return val;
}
 
 
int main()
{
  int a=0,b=0,c=0,d=0;
  int maxinstr=0,maxextinstr=0;
  int vendor=0;
  int cores=1,threads=1;
 
  /* MAX CPUID INSTRUCTION LEVELS */
  a=0x80000000; cpuid(&a,&b,&c,&d);
  maxextinstr=a;
  a=0; cpuid(&a,&b,&c,&d);
  maxinstr=a;
  vendor=b; /* Registers EBX, EDX and ECX hold the vendorstring now, we compare EBX to see if it's 'Genu' or 'Auth'
               because the vendorstring for Intel is 'GenuineIntel' and for AMD it's 'AuthenticAMD' */
 
  /* CPU threads (logical CPUs) for Intel and AMD */
  if ( maxinstr >= 1 )
  {
    a=1; cpuid(&a,&b,&c,&d);
    threads=read_val(&b,16,23);
  }
 
  if ( vendor == 0x756E6547 ) /* Cores for Intel */
  {
    if ( maxinstr >= 4 )
    {
      a=4; c=0; cpuid(&a,&b,&c,&d);
      cores=read_val(&a,26,31)+1;
    }
  }
  else if ( vendor == 0x68747541 ) /* Cores for AMD */
  {
    if ( maxextinstr >= 0x80000008 )
    {
      a=0x80000008; cpuid(&a,&b,&c,&d);
      cores=read_val(&c,0,7)+1;
    }
  }
 
  if ( threads >= 2 )
  {
    if ( threads > cores ) printf("Hyper-Threading CPU (%i threads, %i core(s))\n",threads,cores);
    else                   printf("Multi-core CPU (%i threads, %i cores)\n",threads,cores);
  }
  else printf("Single-core CPU without hyper-threading (1 thread, 1 core)\n");
 
  return 0;
}


# put into .bashrc:
# export CPUARCH=`uname -m`
# or 
# export CPUARCH=`echo $MACHTYPE | sed -e 's/-.*//'`
# and adapt makefile to work on Raspi, XU4
#

ifeq (${CPUARCH}, armv7l)
  AFLAGS = -mcpu=cortex-a15 -mtune=cortex-a15 -mfloat-abi=hard -mfpu=neon 
  ASFLAGS = -mcpu=cortex-a15 -mfloat-abi=hard -mfpu=neon 
else
  AFLAGS = 
endif

ifeq (${CPUARCH}, aarch64)
  ifeq (${UNS}, "Darwin")
    AFLAGS = -mtune=native
  else
    # AFLAGS = -march=native -mtune=native
    AFLAGS = -mtune=native
  endif
endif

ifeq (${OSKERNEL}, MacOS)
  ifneq (${CPUARCH}, aarch64)
    MSKINCL = /Users/guidomoerkotte/src3/mosek/9.2/tools/platform/osx64x86/h
    MSKLIBDIR = /Users/guidomoerkotte/src3/mosek/9.2/tools/platform/osx64x86/bin
  endif
endif

STDFLAGS = -fPIC 

CPSTDFLAG = -std=c++20

# standard flags and Warning Flags for C++
ifeq (${HOST}, wdfl32187349a)
  STDFLAGS = -fPIC -D__OLD_SAP
  CPSTDFLAG = -std=c++11
  CCSTDFLAG = -std=c99
else
  ifeq (${OSKERNEL}, MacOS)
    # STDFLAGS = -fPIC 
    STDFLAGS = -DMACOS
    CPWFLAGS = -Wall -Weffc++ 
    CPSTDFLAG = -std=c++17
    CCSTDFLAG = -std=c99
  endif
endif

ifeq (${HOST}, apollo)
  WITH_MOSEK = true
  MSKINCL = /opt/mosek/9.1/tools/platform/linux64x86/h
  MSKLIBDIR = /opt/mosek/9.1/tools/platform/linux64x86/bin
  # MSKINCL = /opt/mosek/6/tools/platform/linux64x86/h
  # MSKLIBDIR = /opt/mosek/6/tools/platform/linux64x86/bin
endif

ifeq (${HOST}, fyndhorn)
  WITH_MOSEK = true
  MSKINCL = /opt/mosek/9.2/tools/platform/linux64x86/h
  MSKLIBDIR = /opt/mosek/9.2/tools/platform/linux64x86/bin
  # MSKINCL = /opt/mosek/6/tools/platform/linux64x86/h
  # MSKLIBDIR = /opt/mosek/6/tools/platform/linux64x86/bin
endif

ifeq (${HOST}, pluto)
  WITH_MOSEK = true
  MSKINCL   = /opt/mosek/9.2/tools/platform/linux64x86/h
  MSKLIBDIR = /opt/mosek/9.2/tools/platform/linux64x86/bin
endif

ifeq (${HOST}, adarkar)
  WITH_MOSEK = true
  MSKINCL   = /opt/mosek/9.2/tools/platform/linux64x86/h
  MSKLIBDIR = /opt/mosek/9.2/tools/platform/linux64x86/bin
endif

ifeq (${HOST}, kelsinko)
  WITH_MOSEK = true
  MSKINCL   = /opt/mosek/9.2/tools/platform/linux64x86/h
  MSKLIBDIR = /opt/mosek/9.2/tools/platform/linux64x86/bin
endif

ifeq (${HOST}, erebor)
  WITH_MOSEK = true
  MSKINCL   = /opt/mosek/9.2/tools/platform/linux64x86/h
  MSKLIBDIR = /opt/mosek/9.2/tools/platform/linux64x86/bin
endif

ifeq (${HOST}, pi3-hiwi1)
  WITH_MOSEK = true
  MSKINCL   = /opt/mosek/9.2/tools/platform/linux64x86/h
  MSKLIBDIR = /opt/mosek/9.2/tools/platform/linux64x86/bin
endif

# warning flags for C, C++
CCWFLAGS = -Wall 
CPWFLAGS = -Wall -Weffc++ 

ifdef DEBUG
  DBFLAGS = -DDEBUG
else
  DBFLAGS =
endif

ifdef PROFILE
GFLAGS = -pg -O2
else
  ifdef DEBUG
    GFLAGS = -g
  else
    GFLAGS = -O3
  endif
endif

ifdef VTUNE
GFLAGS = -g -O3
endif

CFLAGS  = ${GFLAGS} ${AFLAGS} ${STDFLAGS} ${CPSTDFLAG} ${CPWFLAGS} ${DBFLAGS}
CPFLAGS = ${GFLAGS} ${AFLAGS} ${STDFLAGS} ${CPSTDFLAG} ${CPWFLAGS} ${DBFLAGS}
CCFLAGS = ${GFLAGS} ${AFLAGS} ${STDFLAGS} ${CCSTDFLAG} ${CCWFLAGS} ${DBFLAGS}

CFLAGSNW = ${GFLAGS} ${AFLAGS} ${STDFLAGS} ${CPSTDFLAG}             ${DBFLAGS}

# CPPFLAGS = ${GFLAGS} ${AFLAGS} ${STDFLAGS}  -std=c++17 ${CPWFLAGS} ${DBFLAGS}
CPPFLAGS = ${GFLAGS} ${AFLAGS} ${STDFLAGS} ${CPSTDFLAG} ${CPWFLAGS} ${DBFLAGS}


ifeq (${HOST}, wdfl32187349a)
  CPPC = g++
  CC = g++
  ICPC = g++
else
  CPPC = g++
  CC = g++
  ifeq (${CPUARCH}, x86_64)
    # ICPC = icpc
    # ICPC_COMPATIBILITY_FLAG = -wd3802
    ICPC = g++
  else
    ICPC = g++
  endif
endif

ifeq (${CPUARCH}, arm64)
  NATIVE = 
else
  ifeq (${CPUARCH}, x86_64)
    NATIVE = -march=native -mtune=native
  endif
endif

ifeq (${CPUARCH}, x86_64)
  CFSSE = -msse4.2 
  CFMAVX = -mavx
  ifeq (${HOST}, wdfl32187349a)
    BMI =
    CFSIMD = $(NATIVE) -mavx -msse4.2
  else
    ifeq (${HOST}, centaurus)
      BMI =
      CFSIMD = $(NATIVE) -mavx -msse4.2
    else
      BMI = -mbmi2
      CFSIMD = $(NATIVE) -mavx2
    endif
  endif
else
  # arm
  CFSSE = 
  CFMAVX = 
  BMI = 
  CFSIMD =
endif

ifeq (${SUP_AVX512_PCNT}, yes)
  CFSIMD = -mavx512f
endif

CCC = gcc
GCC = gcc

CPC = g++

SRCDIR = ${SRCBASEDIR}

ifdef ODI
  OBJBASEDIR = ${TOPDIR}/obj/${CPUARCH}
else
  ifdef HDI
    OBJBASEDIR = ${TOPDIR}/obj/${HOST}
  else
    OBJBASEDIR = ${SRCBASEDIR}
  endif
endif

# LPSOLVE
LPDIR = ${TOPDIR}/src3/lp_solve/lpsolve55/bin/ux64
LPINCL  = -I${TOPDIR}/src3/lp_solve_5.5
LPLIB = -llpsolve55



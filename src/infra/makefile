include ${SRCBASEDIR}/inc.mk

CINCL = -I. -I..

CFAVX = -mavx

  OBJDIR = $(HOME)/LearnedVSTraditionalCE/src/infra
  vpath .o $(HOME)/LearnedVSTraditionalCE/src

OFSA = BitvectorLong.o \
       buckethistogram.o \
       print_test_header.o \
       random_string.o \
       geocoord.o \
       datejd.o \
       terminal_color.o \
       linereader.o \
       linereaderWithEdit.o \
       commandreader.o \
       temperature.o \
       timestamp.o \
       matrix.o \
       StringDictionaryHashed.o \
       CardEstEval.o \
       Linterpol.o \
       simpleexception.o \
       simplecharcontainer.o \
       like.o \
       FourierMotzkinBasic.o \
       unionfind.o \
       prime_table_1_1.o \
       lageparameter.o \
       stimestamp.o \
       stime.o \
       AoAlloc.o \
       WaveletTransformNonStd2dim.o \
       cstring_hash.o \
       double_vec_ops.o \
       chisquare.o \
       newton.o \
       FukushimaLambertW.o \
       cfixedsize_container.o \
       cvarsize_container.o \
       cstring_container.o \
       cstring_dictionary.o \
       permutation.o \
       cmeasure.o \
       crusage.o \
       fun_mod_mersenne.o \
       gmsvd.o \

ifneq (${CPUARCH}, armv7l)
  OSFB = CrystalClock.o
endif

ifeq (${OSKERNEL}, Linux)
  OFSC = bind_to_hw_thread.o \
         cbind_to_hw_thread.o \
         system.o 
endif

OFS = $(OFSA) $(OFSB) $(OFSC)

# OBJALL = $(addprefix $(OBJDIR)/,$(OBS))

BFSA = main_random_string \
       main_fa_date \
       main_fa_time \
       main_prime \
       main_no_primes \
       main_prime_table \
       main_terminal_color \
       main_bh \
       main_bitset_iter \
       main_larson \
       main_multmatvec1 \
       main_multmatvec2 \
       main_multmatvec3 \
       main_multmatvec4 \
       main_multmatvec5 \
       main_matrix \
       main_vp \
       main_Ax \
       main_designmatrix \
       main_ao_alloc \
       main_qr \
       main_permutation \
       main_uval \
       main_mod_mersenne \
       main_tabhash \
       main_chisquare \
       main_csc \
       main_rankperm \
       main_invut \
       main_tableau \
       main_shiftedexpdist \
       main_genrandvec \
       main_linereader \
       main_zipf \
       main_system \

ifneq (${CPUARCH}, armv7l)
  BFSB = main_crystal_clock 
endif

ifeq (${OSKERNEL}, Linux) 
  BFSLINUX = get_device_model \
             main_system 
endif


BFS = $(BFSA) $(BFSB) $(BFSLINUX)

# BINALL = $(addprefix $(OBJDIR),$(BFS))


# special requires SSE, BMI2 etc. (intel only)

NEEDSSIMD = main_multmatvec6 \
            gmstrstr16.o \
            gmstrstr16_42.o \
            main_pdep \
            main_crc32 \

# check existance of mosek installation

NEEDSMOSEK = mosekenv.o 

ifeq ($(CPUARCH), x86_64)
  ifeq ($(HOST), wdfl32187349a)
    AFS = $(OFS) $(BFS) $(NEEDSSIMD) 
  else
    ifeq ($(OSKERNEL), Linux)
      ifeq (${SUPP_MOSEK}, yes)
        AFS = $(OFS) $(BFS) $(NEEDSSIMD) $(NEEDSMOSEK)
      else
        AFS = $(OFS) $(BFS) $(NEEDSSIMD) 
      endif
    else
      AFS = $(OFS) $(BFS) $(NEEDSSIMD) 
    endif
  endif
else
  AFS = $(OFS) $(BFS)
endif

ALL = $(addprefix $(OBJDIR)/,$(AFS))

all : $(ALL)
	@ echo "build for ${CPUARCH} in ${OBJDIR}"
	@ echo "SUPP_MOSEK ${SUPP_MOSEK}"


$(OBJDIR)/main_fa_date : $(OBJDIR)/main_fa_date.o $(OBJDIR)/fa_date.o
	$(CC) -o $@ $^

$(OBJDIR)/main_fa_date.o : main_fa_date.cc fa_date.hh
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_fa_date.cc

$(OBJDIR)/main_fa_time : $(OBJDIR)/main_fa_time.o $(OBJDIR)/fa_date.o $(OBJDIR)/fa_time.o $(OBJDIR)/fa_timestamp.o
	$(CC) -o $@ $^

$(OBJDIR)/main_fa_time.o : main_fa_time.cc fa_time.hh
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_fa_time.cc

$(OBJDIR)/main_crystal_clock : $(OBJDIR)/main_crystal_clock.o $(OBJDIR)/CrystalClock.o $(OBJDIR)/cmeasure.o $(OBJDIR)/cbind_to_hw_thread.o
	$(CC) -o $@ $^

$(OBJDIR)/main_crystal_clock.o : main_crystal_clock.cc CrystalClock.hh cmeasure.h cbind_to_hw_thread.h
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_crystal_clock.cc

$(OBJDIR)/main_rotmatrix : $(OBJDIR)/main_rotmatrix.o $(OBJDIR)/matrix.o 
	$(CC) -o $@ $^

$(OBJDIR)/main_rotmatrix.o : main_rotmatrix.cc matrix.hh
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_rotmatrix.cc

$(OBJDIR)/main_qr : $(OBJDIR)/main_qr.o $(OBJDIR)/matrix.o
	$(CC) -g -o $@ $^

$(OBJDIR)/main_qr.o : main_qr.cc matrix.hh
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_qr.cc

$(OBJDIR)/main_bh : $(OBJDIR)/main_bh.o $(OBJDIR)/buckethistogram.o
	$(CC) -o $@ $^

$(OBJDIR)/main_bh.o : main_bh.cc buckethistogram.hh
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_bh.cc

$(OBJDIR)/main_larson : $(OBJDIR)/main_larson.o 
	$(CC) -o $@ $^

$(OBJDIR)/main_larson.o : main_larson.cc Larson.hh
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_larson.cc

$(OBJDIR)/main_bitset_iter : $(OBJDIR)/main_bitset_iter.o 
	$(CC) -o $@ $^

$(OBJDIR)/main_bitset_iter.o : main_bitset_iter.cc bitvectorsmall.hh bit_intrinsics.hh bit_subsets.hh
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_bitset_iter.cc


$(OBJDIR)/main_designmatrix : $(OBJDIR)/main_designmatrix.o $(OBJDIR)/gmsvd.o $(OBJDIR)/matrix.o $(OBJDIR)/double_vec_ops.o
	$(CC) -o $@ $^

$(OBJDIR)/main_designmatrix.o : main_designmatrix.cc matrix.hh
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_designmatrix.cc


$(OBJDIR)/main_matrix : $(OBJDIR)/main_matrix.o $(OBJDIR)/matrix.o
	$(CC) -o $@ $^

$(OBJDIR)/main_matrix.o : main_matrix.cc matrix.hh
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_matrix.cc

$(OBJDIR)/main_vp : $(OBJDIR)/main_vp.o $(OBJDIR)/cmeasure.o
	$(CC) -o $@ $^

$(OBJDIR)/main_vp.o : main_vp.cc vp_ops.hh cmeasure.h
	$(CC) $(CFLAGS) $(NATIVE) $(CINCL) -c -o $@ main_vp.cc


$(OBJDIR)/main_invut : $(OBJDIR)/main_invut.o $(OBJDIR)/matrix.o $(OBJDIR)/cmeasure.o
	$(CC) -o $@ $^

$(OBJDIR)/main_invut.o : main_invut.cc matrix.hh
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_invut.cc

$(OBJDIR)/main_terminal_color : $(OBJDIR)/main_terminal_color.o $(OBJDIR)/terminal_color.o
	$(CCC) -o $@ $^

$(OBJDIR)/main_terminal_color.o : main_terminal_color.c terminal_color.h
	$(CCC) -O3 $(CINCL) -c -o $@ main_terminal_color.c


$(OBJDIR)/main_Ax : $(OBJDIR)/main_Ax.o $(OBJDIR)/matrix.o $(OBJDIR)/double_vec_ops.o $(OBJDIR)/cmeasure.o
	$(CC) -o $@ $^

$(OBJDIR)/main_Ax.o : main_Ax.cc matrix.hh double_vec_ops.hh cmeasure.h 
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_Ax.cc

$(OBJDIR)/main_multmatvec1 : $(OBJDIR)/main_multmatvec1.o $(OBJDIR)/matrix.o
	$(CC) -o $@ $^

$(OBJDIR)/main_multmatvec1.o : main_multmatvec1.cc matrix.hh 
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_multmatvec1.cc

$(OBJDIR)/main_multmatvec2 : $(OBJDIR)/main_multmatvec2.o $(OBJDIR)/matrix.o
	$(CC) -o $@ $^

$(OBJDIR)/main_multmatvec2.o : main_multmatvec2.cc matrix.hh
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_multmatvec2.cc

$(OBJDIR)/main_multmatvec3 : $(OBJDIR)/main_multmatvec3.o $(OBJDIR)/matrix.o
	$(CC) -o $@ $^

$(OBJDIR)/main_multmatvec3.o : main_multmatvec3.cc matrix.hh
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_multmatvec3.cc

$(OBJDIR)/main_multmatvec4 : $(OBJDIR)/main_multmatvec4.o $(OBJDIR)/matrix.o
	$(CC) -o $@ $^

$(OBJDIR)/main_multmatvec4.o : main_multmatvec4.cc matrix.hh
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_multmatvec4.cc

$(OBJDIR)/main_multmatvec5 : $(OBJDIR)/main_multmatvec5.o $(OBJDIR)/matrix.o
	$(CC) -o $@ $^

$(OBJDIR)/main_multmatvec5.o : main_multmatvec5.cc matrix.hh
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_multmatvec5.cc

$(OBJDIR)/main_multmatvec6 : $(OBJDIR)/main_multmatvec6.o $(OBJDIR)/matrix.o
	$(CC) -o $@ $^

$(OBJDIR)/main_multmatvec6.o : main_multmatvec6.cc matrix.hh
	$(CC) $(CFAVX) $(CFLAGS) $(CINCL) -c -o $@ main_multmatvec6.cc


$(OBJDIR)/main_random_string : $(OBJDIR)/main_random_string.o $(OBJDIR)/random_string.o
	$(CC) -o $@ $^

$(OBJDIR)/main_random_string.o : main_random_string.cc random_string.hh
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_random_string.cc

$(OBJDIR)/main_prime : $(OBJDIR)/main_prime.o
	$(CC) -o $@ $^

$(OBJDIR)/main_prime.o : main_prime.cc 
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_prime.cc

$(OBJDIR)/main_no_primes : $(OBJDIR)/main_no_primes.o $(OBJBASEDIR)/infra/FukushimaLambertW.o $(OBJBASEDIR)/infra/cmeasure.o
	$(CC) -o $@ $^

$(OBJDIR)/main_no_primes.o : main_no_primes.cc FukushimaLambertW.hh glob_infra_standard_includes.hh cmeasure.h
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_no_primes.cc

$(OBJDIR)/main_prime_table : $(OBJDIR)/main_prime_table.o
	$(CC) -o $@ $^

$(OBJDIR)/main_prime_table.o : main_prime_table.cc 
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_prime_table.cc

$(OBJDIR)/main_system : $(OBJDIR)/main_system.o $(OBJDIR)/system.o $(OBJDIR)/cbind_to_hw_thread.o
	$(CC) -o $@ $^

$(OBJDIR)/main_system.o : main_system.cc system.hh
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_system.cc

$(OBJDIR)/main_uval : $(OBJDIR)/main_uval.o 
	$(CC) -o $@ $^

$(OBJDIR)/main_uval.o : main_uval.cc uval.hh
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_uval.cc

$(OBJDIR)/main_tabhash : $(OBJDIR)/main_tabhash.o 
	$(CC) -o $@ $^

$(OBJDIR)/main_tabhash.o : main_tabhash.cc TabulationHash.hh
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_tabhash.cc

$(OBJDIR)/main_chisquare : $(OBJDIR)/main_chisquare.o $(OBJDIR)/chisquare.o $(OBJDIR)/matrix.o
	$(CC) -o $@ $^

$(OBJDIR)/main_chisquare.o : main_chisquare.cc chisquare.hh matrix.hh
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_chisquare.cc


$(OBJDIR)/main_pdep : $(OBJDIR)/main_pdep.o 
	$(CC) -o $@ $^

$(OBJDIR)/main_pdep.o : main_pdep.cc bit_intrinsics.hh
	$(CC) $(CFLAGS) -mbmi2 $(CINCL) -c -o $@ main_pdep.cc

$(OBJDIR)/main_mod_mersenne : $(OBJDIR)/main_mod_mersenne.o $(OBJDIR)/fun_mod_mersenne.o $(OBJDIR)/cmeasure.o $(OBJDIR)/cbind_to_hw_thread.o
	$(CC) -o $@ $^

$(OBJDIR)/main_mod_mersenne.o : main_mod_mersenne.cc tmath.hh measure.hh cmeasure.h
	$(CC) -c $(CFLAGS) $(CINCL) -o $@ main_mod_mersenne.cc

$(OBJDIR)/cmeasure.o : cmeasure.c cmeasure.h
	gcc -O3 -c -std=c99 -fPIC -o $@ cmeasure.c

$(OBJDIR)/cbind_to_hw_thread.o : cbind_to_hw_thread.c cbind_to_hw_thread.h
	gcc -O3 -c $(CINCL) -o $@ cbind_to_hw_thread.c

$(OBJDIR)/cstring_hash.o : cstring_hash.c cstring_hash.h
	gcc -O3 -c -Wall $(CINCL) -std=c99 -o $@ cstring_hash.c

$(OBJDIR)/get_device_model : get_device_model.c
	gcc $(CCFLAGS) -Wall -o $@ $^

$(OBJDIR)/main_rankperm : main_rankperm.cc rank_permutation.hh
	g++ $(CFLAGS) -Wall -o $@ main_rankperm.cc

$(OBJDIR)/main_ao_alloc : $(OBJDIR)/main_ao_alloc.o $(OBJDIR)/AoAlloc.o
	g++ -o $@ $^

$(OBJDIR)/main_ao_alloc.o : main_ao_alloc.cc AoAlloc.hh
	g++ -c $(CFLAGS) $(CINCL) -o $@ main_ao_alloc.cc

$(OBJDIR)/main_tableau : $(OBJDIR)/main_tableau.o $(OBJDIR)/FourierMotzkinBasic.o $(OBJDIR)/matrix.o 
	g++ -o $@ $^

$(OBJDIR)/main_tableau.o : main_tableau.cc LpGr.cc FourierMotzkinBasic.hh matrix.hh double_vec_ops.hh
	g++ $(CFLAGS) $(CINCL) -c -o $@ main_tableau.cc

$(OBJDIR)/main_crc32 : $(OBJDIR)/main_crc32.o 
	g++ -o $@ $^

$(OBJDIR)/main_crc32.o : main_crc32.cc hasht.hh
	g++ $(CFLAGS) $(CINCL) $(CFSIMD) -c -o $@ main_crc32.cc

$(OBJDIR)/main_csc : $(OBJDIR)/main_csc.o $(OBJDIR)/cfixedsize_container.o $(OBJDIR)/cvarsize_container.o $(OBJDIR)/cstring_container.o $(OBJDIR)/cstring_dictionary.o
	gcc -o $@ $^

$(OBJDIR)/main_csc.o : main_csc.c cfixedsize_container.h cvarsize_container.h cstring_container.h cstring_dictionary.h
	gcc $(CCFLAGS) $(CINCL) -c -o $@ main_csc.c

$(OBJDIR)/cstring_dictionary.o : cstring_container.h cfixedsize_container.h

$(OBJDIR)/main_shiftedexpdist : $(OBJDIR)/main_shiftedexpdist.o
	$(CC) -o $@ $^

$(OBJDIR)/main_shiftedexpdist.o : main_shiftedexpdist.cc bag_tt.hh ShiftedExponentialDistribution.hh
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_shiftedexpdist.cc

$(OBJDIR)/main_genrandvec : $(OBJDIR)/main_genrandvec.o $(OBJDIR)/GenRandIntVec.o $(OBJDIR)/cmeasure.o
	$(CC) -o $@ $^

$(OBJDIR)/main_genrandvec.o : main_genrandvec.cc GenRandIntVec.hh zipf_distribution.hh
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_genrandvec.cc


$(OBJDIR)/main_zipf : $(OBJDIR)/main_zipf.o 
	$(CC) -o $@ $^

$(OBJDIR)/main_zipf.o : main_zipf.cc zipf_distribution.hh
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_zipf.cc


$(OBJDIR)/main_permutation : $(OBJDIR)/main_permutation.o $(OBJDIR)/permutation.o
	$(CC) -o $@ $^

$(OBJDIR)/main_permutation.o : main_permutation.cc permutation.hh
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_permutation.cc

$(OBJDIR)/main_linereader : $(OBJDIR)/main_linereader.o $(OBJDIR)/linereader.o $(OBJDIR)/simpleexception.o $(OBJDIR)/stimestamp.o $(OBJDIR)/stime.o $(OBJDIR)/datejd.o
	$(CC) -o $@ $^

$(OBJDIR)/main_linereader.o : main_linereader.cc linereader.hh
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_linereader.cc

$(OBJDIR)/main_perf : $(OBJDIR)/main_perf.o  $(OBJDIR)/perf_wrapper.o $(OBJDIR)/cbind_to_hw_thread.o
	$(CCC) $(CCFLAGS) $(CINCL) -o $@ $^

$(OBJDIR)/main_perf.o : main_perf.c perf_wrapper.h
	$(CCC) $(CCFLAGS) $(CINCL) -c -o $@ main_perf.c

$(OBJDIR)/main_perf_wrapper : $(OBJDIR)/main_perf_wrapper.o  $(OBJDIR)/perf_wrapper.o $(OBJDIR)/CrystalClock.o $(OBJDIR)/cbind_to_hw_thread.o $(OBJDIR)/cmeasure.o
	$(CC) $(CFLAGS) $(CINCL) -o $@ $^


$(OBJDIR)/main_perf_wrapper.o : main_perf_wrapper.cc perf_wrapper.h
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_perf_wrapper.cc


# SPECIAL NEEDS MOSEK
$(OBJDIR)/mosekenv.o : mosekenv.cc mosekenv.hh
	$(CC) $(CFLAGS) $(CINCL) -c -I${MSKINCL} -o $@ mosekenv.cc


# SPECIAL NEEDS SIMD

$(OBJDIR)/gmstrstr16.o : gmstrstr16.cc gmstrstr16.hh
	$(ICPC) $(ICPC_COMPATIBILITY_FLAG)  $(CFLAGS)  $(CINCL) $(CFSSE) -c -o $@ gmstrstr16.cc

$(OBJDIR)/gmstrstr16_42.o : gmstrstr16_42.cc gmstrstr16_42.hh
	$(ICPC)  $(ICPC_COMPATIBILITY_FLAG) $(CFLAGS) $(CINCL) $(CFSSE)  -c -o $@ gmstrstr16_42.cc


$(OBJDIR)/matrix.o : matrix.cc matrix.hh  vp_ops.hh
	$(CPC) $(CFLAGS) $(NATIVE) $(CINCL) -c -o $@ matrix.cc

# GENERAL

$(OBJDIR)/%.o : %.cc %.hh
	@ echo "general c++ rule"
	$(CPC) $(CFLAGS) $(CINCL) $(NATIVE) -c -o $@ $*.cc

$(OBJDIR)/%.o : %.c %.h
	@ echo "general c rule"
	$(CCC) $(CCFLAGS) $(CINCL) $(NATIVE) -c -o $@ $*.c

.PHONY: clean

clean :
	cd $(OBJDIR); rm -f *.o *.gch a.out $(ALL)

include ${SRCBASEDIR}/fin.mk

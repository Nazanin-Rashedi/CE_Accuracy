include ${HOME}/LearnedVSTraditionalCE/src/inc.mk

OBJDIR = $(HOME)/LearnedVSTraditionalCE/src/H2D/src/CheckError

vpath .o $(OBJDIR):$(OBJBASEDIR)

INCL = -I.. -I../.. -I../../..

VPATH = .:..:../..:../../..

HDRINFRA = infra/argbase.hh \
           infra/matrix.hh \

HDRX = CheckErrorUtil.hh \
       TestRegular2dim.hh \
       TestFixed2dim.hh \
       TestRandom2dim.hh \
       EstimateEvaluator.hh \

OFSX = CheckErrorUtil.o \
       TestRegular2dim.o \
       TestFixed2dim.o \
       TestRandom2dim.o \
       EstimateEvaluator.o \


HDRZ = infra/cb.hh \
       infra/util.hh \
       infra/types.hh \
       infra/data2dim.hh \
       infra/summaryline.hh \
       infra/RegularPartitioning2dim.hh \

OBJZ = $(addprefix $(OBJDIR)/, $(OFSX))

all : $(OBJZ)


$(OBJDIR)/CheckErrorUtil.o : CheckErrorUtil.cc CheckErrorUtil.hh $(HDRX) $(HDRY) $(HDRZ) 
	$(CC) -c $(CFLAGS) $(INCL) -o $@ CheckErrorUtil.cc

$(OBJDIR)/TestRegular2dim.o : TestRegular2dim.cc TestRegular2dim.hh EstimateEvaluator.hh $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(INCL) -o $@ TestRegular2dim.cc

$(OBJDIR)/TestFixed2dim.o : TestFixed2dim.cc TestFixed2dim.hh EstimateEvaluator.hh $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(INCL) -o $@ TestFixed2dim.cc

$(OBJDIR)/TestRandom2dim.o : TestRandom2dim.cc TestRandom2dim.hh EstimateEvaluator.hh $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(INCL) -o $@ TestRandom2dim.cc

$(OBJDIR)/EstimateEvaluator.o : EstimateEvaluator.cc EstimateEvaluator.cc
	$(CC) -c $(CFLAGS) $(INCL) -o $@ EstimateEvaluator.cc

.PHONY : clean

clean :
	rm -f *.o a.out 

include ${SRCBASEDIR}/fin.mk


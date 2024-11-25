include ${SRCBASEDIR}/inc.mk

H2DIR  = $(HOME)/LearnedVSTraditionalCE/src/H2D/src
OBJDIR = $(HOME)/LearnedVSTraditionalCE/src/H2D/src/Sampling

INCL = -I.. -I../.. -I../../..

VPATH = .:..:../..:../../..


HDRINFRA = infra/types.hh \
           infra/data2dim.hh \
           infra/summaryline.hh \
           infra/EstimatorBase.hh \
	   infra/glob_infra_standard_includes.hh \
	   infra/tmath.hh \
	   infra/linfun.hh \
	   infra/newton_tt.hh \
	   FukushimaLambertW.hh \

OFS = Sample2dim.o \


AFS = $(OFS)

ALL = $(addprefix $(OBJDIR)/, $(AFS))

all: $(ALL)


$(OBJDIR)/Sample2dim.o : Sample2dim.cc Sample2dim.hh 
	$(CC) -c $(CFLAGS) $(INCL) -o $@ Sample2dim.cc



clean :
	cd $(OBJDIR)
	rm -f *.o a.out $(ALL)

include ${SRCBASEDIR}/fin.mk


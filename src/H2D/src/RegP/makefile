include ${SRCBASEDIR}/inc.mk

H2DIR  = $(HOME)/LearnedVSTraditionalCE/src/H2D/src
OBJDIR = $(HOME)/LearnedVSTraditionalCE/src/H2D/src/RegP

INCL = -I.. -I../.. -I../../..

VPATH = .:..:../..:../../..


HDRINFRA = infra/types.hh \
           infra/summaryline.hh \
           infra/RegularPartitioning2dim.hh \
           infra/EstimatorBase2dim.hh \
           infra/HighlyFrequentTile.hh \


OFS = RegPEstimator.o HFTEstimator.o

AFS = $(OFS)

ALL = $(addprefix $(OBJDIR)/, $(AFS))

all: $(ALL)

$(OBJDIR)/HFTEstimator.o : HFTEstimator.cc HFTEstimator.hh $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(INCL) -o $@ HFTEstimator.cc

$(OBJDIR)/RegPEstimator.o : RegPEstimator.cc RegPEstimator.hh $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(INCL) -o $@ RegPEstimator.cc

clean :
	cd $(OBJDIR)
	rm -f *.o a.out $(ALL)

include ${SRCBASEDIR}/fin.mk

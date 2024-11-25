include ${SRCBASEDIR}/inc.mk

H2DIR  = $(HOME)/LearnedVSTraditionalCE/src
OBJDIR = $(H2DIR)/H2D/src/MHist2

INCL = -I.. -I../.. -I../../..

VPATH = .:..:../..:../../..

HDRINFRA = infra/types.hh \
           infra/summaryline.hh \
           infra/RegularPartitioning2dim.hh \
           infra/EstimatorBase2dim.hh \

OFS = MHist2Bucket.o \
      MHist2.o 

AFS = $(OFS)

ALL = $(addprefix $(OBJDIR)/, $(AFS))

all: $(ALL)

$(OBJDIR)/MHist2.o : MHist2.cc MHist2.hh $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(INCL) -o $@ MHist2.cc

$(OBJDIR)/MHist2Bucket.o : MHist2Bucket.cc MHist2Bucket.hh $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(INCL) -o $@ MHist2Bucket.cc


clean :
	cd $(OBJDIR)
	rm -f *.o a.out $(ALL)


include ${SRCBASEDIR}/fin.mk

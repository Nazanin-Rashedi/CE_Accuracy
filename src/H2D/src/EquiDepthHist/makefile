include ${HOME}/LearnedVSTraditionalCE/src/inc.mk

OBJDIR = $(HOME)/LearnedVSTraditionalCE/src/H2D/src/EquiDepthHist

vpath .o $(OBJDIR):$(OBJBASEDIR)


INCL = -I.. -I../.. -I../../..

VPATH = .:..:../..:../../..

HDRINFRA = infra/types.hh \
           infra/summaryline.hh \
           infra/RegularPartitioning2dim.hh \
           infra/EstimatorBase2dim.hh \

OFS = EqDepHist.o 

OBJ = $(addprefix $(OBJDIR)/, $(OFS))

ALL = $(OBJ)

all : $(ALL)

$(OBJDIR)/EqDepHist.o : EqDepHist.cc EqDepHist.hh $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(INCL) -o $@ EqDepHist.cc

clean :
	rm -f *.o a.out 

include ${SRCBASEDIR}/fin.mk


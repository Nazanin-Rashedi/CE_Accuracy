include ${SRCBASEDIR}/inc.mk

H2DIR  = $(HOME)/LearnedVSTraditionalCE/src
OBJDIR = $(H2DIR)/H2D/src/OneDEqDepHist

vpath .o $(OBJDIR):$(OBJBASEDIR)

INCL = -I.. -I../.. -I../../..

VPATH = .:..:../..:../../..

HDRINFRA = infra/types.hh \
           infra/summaryline.hh \
           infra/data2dim.hh \
           infra/EstimatorBase2dim.hh \

OFS = OneDEqDepHist.o 
OBJ = $(addprefix $(OBJDIR)/, $(OFS))

ALL = $(OBJ)

all : $(ALL)

$(OBJDIR)/OneDEqDepHist.o : OneDEqDepHist.cc OneDEqDepHist.hh $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(INCL) -o $@ OneDEqDepHist.cc

clean :
	cd $(OBJDIR)
	rm -f *.o a.out 

include ${SRCBASEDIR}/fin.mk

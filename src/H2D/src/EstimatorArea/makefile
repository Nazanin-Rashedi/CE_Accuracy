include ${SRCBASEDIR}/inc.mk

H2DIR  = $(HOME)/LearnedVSTraditionalCE/src/H2D/src
OBJDIR = $(HOME)/LearnedVSTraditionalCE/src/H2D/src/EstimatorArea

vpath .o $(OBJDIR):$(OBJBASEDIR)

INCL = -I.. -I../.. -I../../..

VPATH = .:..:../..:../../..

HDRINFRA = infra/types.hh \
           infra/summaryline.hh \
           infra/data2dim.hh \
           infra/EstimatorBase2dim.hh \

OFS = EstimatorArea.o 
OBJ = $(addprefix $(OBJDIR)/, $(OFS))

ALL = $(OBJ)

all : $(ALL)

$(OBJDIR)/EstimatorArea.o : EstimatorArea.cc EstimatorArea.hh $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(INCL) -o $@ EstimatorArea.cc

clean :
	cd $(OBJDIR)
	rm -f *.o a.out 

include ${SRCBASEDIR}/fin.mk

include ${SRCBASEDIR}/inc.mk

H2DIR  = $(HOME)/LearnedVSTraditionalCE/src/H2D/src
OBJDIR = $(HOME)/LearnedVSTraditionalCE/src/H2D/src/QTS

INCL = -I.. -I../.. -I../../..

VPATH = .:..:../..:../../..

HDRINFRAG = infra/matrix.hh

OFSINFRAG = infra/matrix.o

OBJINFRAG = $(addprefix $(OBJBASEDIR)/, $(OFSINFRAG))


HDRINFRA = infra/types.hh \
           infra/summaryline.hh \
           infra/RegularPartitioning2dim.hh \
           infra/EstimatorBase2dim.hh \

OFSINFRA = infra/EstimatorBase2dim.o \
           infra/RegularPartitioning2dim.o \
           infra/data2dim.o \
           infra/types.o \

OBJINFRA = $(addprefix $(H2DIR)/, $(OFSINFRA))

OFS = QTS.o \
      IQTS.o \

BFS = main_iqts

AFS = $(OFS) $(BFS)

ALL = $(addprefix $(OBJDIR)/, $(AFS))

all : $(ALL)

$(OBJDIR)/main_iqts : $(OBJDIR)/main_iqts.o $(OBJDIR)/IQTS.o $(OBJINFRA) $(OBJINFRAG)
	$(CC)  $(CFLAGS) -o $@ $^

$(OBJDIR)/main_iqts.o : main_iqts.cc IQTS.hh $(HDRINFRA) $(HDRINFRAG)
	$(CC) -c $(CFLAGS) $(INCL) -o $@ main_iqts.cc


$(OBJDIR)/IQTS.o : IQTS.cc IQTS.hh $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(INCL) -o $@ IQTS.cc

$(OBJDIR)/QTS.o : QTS.cc QTS.hh $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(INCL) -o $@ QTS.cc

clean :
	cd $(OBJDIR)
	rm -f *.o a.out $(ALL)

include ${SRCBASEDIR}/fin.mk


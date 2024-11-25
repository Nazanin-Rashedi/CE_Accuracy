include ${SRCBASEDIR}/inc.mk

H2DIR  = $(HOME)/LearnedVSTraditionalCE/src/H2D/src
OBJDIR = $(HOME)/LearnedVSTraditionalCE/src/H2D/src/scale

vpath .o $(OBJDIR):$(OBJBASEDIR)

INCL = -I.. -I../.. -I../../..

VPATH = .:..:../..:../../..

HDRINFRAG = infra/q.hh \

HDRINFRA = infra/types.hh \
           infra/summaryline.hh \
           infra/RegularPartitioning2dim.hh \
           infra/EstimatorBase2dim.hh \
           infra/array_tt.hh \
           infra/numarray_tt.hh \

OFSINFRA = infra/EstimatorBase2dim.o \
           infra/RegularPartitioning2dim.o \
           infra/data2dim.o \
           infra/types.o \

OFS = scale.o

BFS = main_scale

AFS = $(OFS) $(BFS)

ALL = $(addprefix $(OBJDIR)/, $(AFS))

all : $(ALL)

$(OBJDIR)/main_scale : $(OBJDIR)/main_scale.o $(OBJDIR)/scale.o
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/main_scale.o : main_scale.cc scale.hh $(HDRINFRA) $(HDRINFRAG)
	$(CC) $(CFLAGS) -c $(INCL) -o $@ main_scale.cc


$(OBJDIR)/scale.o : scale.cc scale.hh $(HDRINFRA) $(HDRINFRAG)
	$(CC) -c $(CFLAGS) $(INCL) -o $@ scale.cc

clean :
	cd $(OBJDIR)
	rm -f *.o a.out $(ALL)

include ${SRCBASEDIR}/fin.mk


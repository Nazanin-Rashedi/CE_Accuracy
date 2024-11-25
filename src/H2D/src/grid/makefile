include ${HOME}/LearnedVSTraditionalCE/src/inc.mk

OBJDIR = $(HOME)/LearnedVSTraditionalCE/src/H2D/src/grid
H2DIR  = $(HOME)/LearnedVSTraditionalCE/src/H2D/src

vpath .o $(OBJDIR):$(OBJBASEDIR)

INCL = -I. -I.. -I../.. -I../../.. -I../../../../
VPATH = .:..:../..:../../..

HDRINFRA = infra/matrix.hh

OFSINFRA = infra/matrix.o

OBJINFRA = $(addprefix $(OBJBASEDIR)/, $(OFSINFRA))

AFS = grid.o

BFS = main_grid

AFS = $(OFS) $(BFS)

ALL = $(addprefix $(OBJDIR)/, $(AFS))

all : $(ALL)


$(OBJDIR)/main_grid : $(OBJDIR)/main_grid.o $(OBJDIR)/grid.o $(OBJINFRA)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/main_grid.o : main_grid.cc grid.hh $(HDRINFRA)
	$(CC) $(CFLAGS) $(INCL) -c -o $@ main_grid.cc


$(OBJDIR)/grid.o : grid.cc grid.hh
	$(CC) $(CFLAGS) -c -o $@ grid.cc



clean:
	cd $(OBJDIR)
	rm -f a.out *.o $(ALL)

include ${SRCBASEDIR}/fin.mk


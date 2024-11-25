include ${SRCBASEDIR}/inc.mk

CINCL = -I. -I..

OBJDIR = $(OBJBASEDIR)/Hilbert

vpath .o $(OBJBASEDIR):$(OBJBASEDIR)/infra

HDRINFRA = infra/bitvectorsmall.hh \
           infra/measure.hh \
           infra/aggregate.hh \

OFSINFRA = infra/measure.o \

OBJINFRA = $(addprefix $(OBJDIR)/,$(OBJINFRA))

OFS = Hilbert.o \

OBJ = $(addprefix $(OBJDIR)/,$(OFS))


BFS = main_cilbert \
      main_hilbert \
      main_zcurve \

AFS = $(OFS) $(BFS)

ALL = $(addprefix $(OBJDIR)/,$(AFS))

all : $(ALL)
	@ echo build for ${CPUARCH}


$(OBJDIR)/main_cilbert : $(OBJDIR)/main_cilbert.o $(OBJ)
	$(CC) -o $@ $^

$(OBJDIR)/main_cilbert.o : main_cilbert.cc 
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_cilbert.cc

$(OBJDIR)/main_hilbert : $(OBJDIR)/main_hilbert.o $(OBJ)
	$(CC) -o $@ $^

$(OBJDIR)/main_hilbert.o : main_hilbert.cc 
	$(CC) $(CFLAGS) $(CINCL) -c -o $@ main_hilbert.cc


$(OBJDIR)/main_zcurve : $(OBJDIR)/main_zcurve.o $(OBJ)
	$(CC) -o $@ $^

$(OBJDIR)/main_zcurve.o : main_zcurve.cc 
	$(CC) $(CFLAGS) $(CINCL) -c -march=native -o $@ main_zcurve.cc

# GENERAL

$(OBJDIR)/%.o : %.cc %.hh
	$(CC) $(CPFLAGS) $(CINCL) -c -o $@ $*.cc

$(OBJDIR)/%.o : %.c %.h
	$(CCC) $(CCFLAGS) $(CINCL) -c -o $@ $*.cc

# PHONY

.PHONY: clean

clean :
	cd $(OBJDIR); rm -f *.o *.gch a.out $(ALL)

include ${SRCBASEDIR}/fin.mk

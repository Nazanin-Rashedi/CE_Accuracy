include ${HOME}/LearnedVSTraditionalCE/src/inc.mk


CINCL = -I.. -I../.. -I../../..
OBJDIR = $(HOME)/LearnedVSTraditionalCE/src/H2D/src/Compare
vpath .o ${HOME}/LearnedVSTraditionalCE/src
VPATH = .:..:../..:../../..

OFSINFRA = linereader.o \
           stimestamp.o \
           stime.o \
           datejd.o \
           simpleexception.o \
           cmeasure.o \

OBJINFRA =  $(addprefix $(OBJBASEDIR)/infra/, $(OFSINFRA))

# source files, object files, dependency files
SRC = $(sort $(wildcard *.cc))
OFS = $(SRC:%.cc=%.o)
DEP = $(SRC:%.cc=%.d)
DEPX = $(addprefix $(OBJDIR)/, $(DEP))

# binary executables
BFS = main_prepare

# everything
AFS = $(OFS) $(BFS)

ALL = $(addprefix $(OBJDIR)/, $(AFS))

all : $(ALL)
	@ echo "build for ${CPUARCH}"

-include $(DEPX)

# EXECUTABLES

$(OBJDIR)/main_prepare : $(ALL) $(OBJINFRA)
	$(CC) -o $@ $^


# GENERAL

$(OBJDIR)/%.o : %.cc
	@ echo "default rule:"
	$(CC) $(CFLAGS) $(CINCL) $(NATIVE) -MMD -c -o $@ $*.cc

# PHONY

.PHONY: clean

clean :
	rm -f *.o *.d *.gch a.out $(ALL)

include ${SRCBASEDIR}/fin.mk

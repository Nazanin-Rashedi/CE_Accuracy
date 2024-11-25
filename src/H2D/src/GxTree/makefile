include ${SRCBASEDIR}/inc.mk

H2DIR  = $(HOME)/LearnedVSTraditionalCE/src/H2D/src
OBJDIR = $(HOME)/LearnedVSTraditionalCE/src/H2D/src/GxTree


INCL = -I.. -I../.. -I../../..

VPATH = .:..:../..:../../..

HDRINFRAG = infra/matrix.hh

OFSINFRAG = infra/matrix.o

OBJINFRAG = $(addprefix $(OBJBASEDIR)/, $(OFSINFRAG))

HDRINFRA = infra/types.hh \
           infra/summaryline.hh \
           infra/RegularPartitioning2dim.hh \
           infra/EstimatorBase2dim.hh \
           infra/array_tt.hh \
           infra/numarray_tt.hh \
           infra/grid_tt.hh \

OFSINFRA = infra/EstimatorBase2dim.o \
           infra/RegularPartitioning2dim.o \
           infra/data2dim.o \
           infra/types.o \

OBJINFRA = $(addprefix $(H2DIR)/, $(OFSINFRA))

NODEHDR = Gxt_Node_G.hh \
          Gxt_Node_L.hh \
          Gxt_Node_M.hh \
          Gxt_Node_S.hh \

NODEOFS = Gxt_Node_G.o \
          Gxt_Node_L.o \
          Gxt_Node_M.o \
          Gxt_Node_S.o \

NODEOBJ = $(addprefix $(OBJDIR)/, $(NODEOFS))

HDRLOC = $(NODEHDR) \
         estimate_t.hh \
         encode_t.hh \

OFSM = CheckError/CheckErrorUtil.o \
       CheckError/EstimateEvaluator.o \
       CheckError/TestFixed2dim.o \
       CheckError/TestRandom2dim.o \
       CheckError/TestRegular2dim.o \
       infra/summaryline.o \
       infra/cb.o \
       infra/util.o \

OBJM = $(addprefix $(H2DIR)/, $(OFSM))

OFS = GxTree.o \
      GxtTypes.o \

OBJ = $(addprefix $(OBJDIR)/, $(OFS))

BFS = main \
      main_brqr \

AFS = $(OFS) $(BFS) $(NODEOFS)

ALL = $(addprefix $(OBJDIR)/, $(AFS))

all : $(ALL)

$(OBJDIR)/main : $(OBJDIR)/main.o $(OBJ) $(H2DIR)/scale/scale.o $(NODEOBJ) $(OBJM) $(OBJINFRA) $(OBJINFRAG)
	$(CC)  $(CFLAGS) -o $@ $^

$(OBJDIR)/main.o : main.cc GxTree.hh GxtTypes.hh $(HDRLOC) $(HDRINFRA) 
	$(CC) -c $(CFLAGS) $(INCL) -o $@ main.cc


$(OBJDIR)/main_brqr : $(OBJDIR)/main_brqr.o $(OBJINFRA) $(OBJINFRAG)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJDIR)/main_brqr.o : main_brqr.cc ../infra/types.hh
	$(CC) -c $(CFLAGS) $(INCL) -o $@ main_brqr.cc


$(OBJDIR)/GxTree.o : GxTree.cc GxTree.hh GxtTypes.hh $(HDRLOC) $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(INCL) -o $@ GxTree.cc

$(OBJDIR)/Gxt_Node_G.o : Gxt_Node_G.cc Gxt_Node_G.hh GxtTypes.hh estimate_t.hh $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(INCL) -o $@ Gxt_Node_G.cc

$(OBJDIR)/Gxt_Node_L.o : Gxt_Node_L.cc Gxt_Node_L.hh GxtTypes.hh estimate_t.hh $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(INCL) -o $@ Gxt_Node_L.cc

$(OBJDIR)/Gxt_Node_M.o : Gxt_Node_M.cc Gxt_Node_M.hh GxtTypes.hh estimate_t.hh $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(INCL) -o $@ Gxt_Node_M.cc

$(OBJDIR)/Gxt_Node_S.o : Gxt_Node_S.cc Gxt_Node_S.hh GxtTypes.hh estimate_t.hh $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(INCL) -o $@ Gxt_Node_S.cc

$(OBJDIR)/GxtTypes.o : GxtTypes.cc GxtTypes.hh $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(INCL) -o $@ GxtTypes.cc



# GENERAL

$(OBJDIR)/%.o : %.cc %.hh
	@ echo "default rule"
	$(CPPC) -c $(CINCL) $(NATIVE) $(CPFLAGS) $(CFSIMD) -o $@ $*.cc



clean :
	cd $(OBJDIR)
	rm -f *.o a.out $(ALL)

include ${SRCBASEDIR}/fin.mk

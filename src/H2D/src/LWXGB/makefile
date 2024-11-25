include ${SRCBASEDIR}/inc.mk

H2DIR  = $(HOME)/LearnedVSTraditionalCE/src/H2D/src
OBJDIR = $(HOME)/LearnedVSTraditionalCE/src/H2D/src/LWXGB

INCL = -I.. -I../.. -I../../.. -I../../../..

VPATH = .:..:../..:../../..


HDRINFRA = infra/types.hh \
           infra/cb.hh \
           infra/EstimatorBase2dim.hh \
           infra/data2dim.hh \
           XGBoost/XGBEstimator.hh \
           EstimatorArea/EstimatorArea.hh" \
	   XGBoost/XGBEstimator.hh \


OFS = LWXGB.o 

AFS = $(OFS)

ALL = $(addprefix $(OBJDIR)/, $(AFS))

all : $(ALL)


$(OBJDIR)/LWXGB.o : LWXGB.cpp LWXGB.hh 
	$(CC) -c $(CFLAGS)-fopenmp $(INCL) -o $@ LWXGB.cpp

clean :
	cd $(OBJDIR)
	rm -f *.o a.out 

include ${SRCBASEDIR}/fin.mk


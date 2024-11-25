include ${SRCBASEDIR}/inc.mk

H2DIR  = $(HOME)/LearnedVSTraditionalCE/src/H2D/src
OBJDIR = /$(HOME)/LearnedVSTraditionalCE/src/H2D/src/EXGB

INCL = -I.. -I../.. -I../../.. -I../../../..

VPATH = .:..:../..:../../..


HDRINFRA = infra/types.hh \
           infra/cb.hh \
           infra/EstimatorBase2dim.hh \
           infra/data2dim.hh \
           XGBoost/XGBEstimator.hh \
           EstimatorArea/EstimatorArea.hh" \
	   XGBoost/XGBEstimator.hh \


OFS = EXGB.o 

AFS = $(OFS)

ALL = $(addprefix $(OBJDIR)/, $(AFS))

all : $(ALL)


$(OBJDIR)/EXGB.o : EXGB.cpp EXGB.hh 
	$(CC) -c $(CFLAGS) $(INCL) -o $@ EXGB.cpp

clean :
	cd $(OBJDIR)
	rm -f *.o a.out 

include ${SRCBASEDIR}/fin.mk


include ${SRCBASEDIR}/inc.mk

H2DIR  = $(HOME)/LearnedVSTraditionalCE/src/H2D/src
OBJDIR = $(HOME)/LearnedVSTraditionalCE/src/H2D/src/XGBoost

INCL = -I.. -I../.. -I../../.. -I../../../..

VPATH = .:..:../..:../../..


HDRINFRA = infra/types.hh \
           infra/cb.hh \
           infra/EstimatorBase2dim.hh \
           infra/data2dim.hh \

OFS = XGBEstimator.o 

AFS = $(OFS)

ALL = $(addprefix $(OBJDIR)/, $(AFS))

all : $(ALL)


$(OBJDIR)/XGBEstimator.o : XGBEstimator.cpp XGBEstimator.hh 
	$(CC) -c $(CFLAGS) $(INCL) -o $@ XGBEstimator.cpp

clean :
	cd $(OBJDIR)
	rm -f *.o a.out 

include ${SRCBASEDIR}/fin.mk


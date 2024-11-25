include ${SRCBASEDIR}/inc.mk

OBJDIR = $(OBJBASEDIR)/H2D/src/infra
vpath .o $(OBJDIR):$(OBJBASEDIR)

INCL = -I.. -I../.. -I../../..

VPATH = .:..:../..:../../../

HDRINFRAG = infra/argbase.hh \
            infra/matrix.hh \
            infra/tmath.hh \
            infra/aggregate.hh \
            infra/q.hh \
            infra/tmath.hh \


OBJINFRAG = $(OBJBASEDIR)/infra/matrix.o \
            $(OBJBASEDIR)/infra/cmeasure.o \


HDRZ = cb.hh \
       util.hh \
       types.hh \
       data2dim.hh \
       EstimatorBase2dim.hh \


OFSZ = cb.o \
       util.o \
       types.o \
       HighlyFrequentTile.o \
       data2dim.o \
       EstimatorBase2dim.o \
       RegularPartitioning2dim.o \
       summaryline.o \
       
OBJZ = $(addprefix $(OBJDIR)/, $(OFSZ))

all : $(OBJZ)


$(OBJDIR)/RegularPartitioning2dim.o : RegularPartitioning2dim.cc RegularPartitioning2dim.hh $(HDRINFRAG)
	$(CC) -c $(CFLAGS) $(INCL) -o $@ RegularPartitioning2dim.cc


$(OBJDIR)/EstimatorBase2dim.o : EstimatorBase2dim.cc EstimatorBase2dim.hh types.hh
	$(CC) -c $(CFLAGS) $(INCL) -o $@ EstimatorBase2dim.cc

$(OBJDIR)/data2dim.o : data2dim.cc data2dim.cc
	$(CC) -c $(CFLAGS) $(INCL) -o $@ data2dim.cc

$(OBJDIR)/summaryline.o : summaryline.cc summaryline.hh $(OBJINFRA)
	$(CC) -c $(CFLAGS) $(INCL) -o $@ summaryline.cc

$(OBJDIR)/cb.o : cb.cc cb.hh
	$(CC) -c $(CFLAGS) $(INCL) -o $@ cb.cc

$(OBJDIR)/util.o : util.cc util.hh types.hh 
	$(CC) -c $(CFLAGS) $(INCL) -o $@ util.cc

$(OBJDIR)/HighlyFrequentTile.o : HighlyFrequentTile.cc HighlyFrequentTile.hh types.hh
	$(CC) -c $(CFLAGS) $(INCL) -o $@ HighlyFrequentTile.cc

$(OBJDIR)/types.o : types.cc types.hh
	$(CC) -c $(CFLAGS) $(INCL) -o $@ types.cc

$(OBJDIR)/matrix.o : matrix.cc matrix.hh
	$(CC) -c $(CFLAGS) $(INCL) -o $@ matrix.cc

.PHONY : clean

clean :
	cd $(OBJDIR)
	rm -f *.o a.out 


include ${SRCBASEDIR}/fin.mk


include ${SRCBASEDIR}/inc.mk

H2DIR  = $(HOME)/LearnedVSTraditionalCE/src/H2D/src
OBJDIR = $(HOME)/LearnedVSTraditionalCE/src/H2D/src/main

CINCL = -I.. -I../.. -I../../..
VPATH = .:..:../..:../../..

LIBDIR = -L $(TOPDIR)/xgboost/lib/
LIBS   = xgboost

LDINCL = -L.. -L../..

HDRINFRAG = infra/argbase.hh \
            infra/matrix.hh \
            infra/cmeasure.h \
            infra/FukushimaLambertW.hh \

OFSINFRAG =  infra/WaveletTransformNonStd2dim.o \
             infra/matrix.o \
             infra/cmeasure.o \
             infra/FukushimaLambertW.o \

OBJINFRAG = $(addprefix $(OBJBASEDIR)/, $(OFSINFRAG))
         

HDRX = CheckError/CheckErrorUtil.hh \
       CheckError/TestRegular2dim.hh \
       CheckError/TestFixed2dim.hh \
       CheckError/TestRandom2dim.hh \
       CheckError/EstimateEvaluator.hh \

OFSX = CheckError/CheckErrorUtil.o \
       CheckError/TestRegular2dim.o \
       CheckError/TestFixed2dim.o \
       CheckError/TestRandom2dim.o \
       CheckError/EstimateEvaluator.o \

OBJX = $(addprefix $(H2DIR)/, $(OFSX))

HDRY = EstimatorArea/EstimatorArea.hh \
       Sampling/Sample2dim.hh \
       XGBoost/XGBEstimator.hh \
       EXGB/EXGB.hh \
       LWXGB/LWXGB.hh \
       EquiDepthHist/EqDepHist.hh \
       QTS/QTS.hh \
       QTS/IQTS.hh \
       GxTree/GxTree.hh \
       scale/scale.hh \
       OneDEqDepHist/OneDEqDepHist.hh


OFSY = EstimatorArea/EstimatorArea.o \
       Sampling/Sample2dim.o \
       XGBoost/XGBEstimator.o \
       EXGB/EXGB.o \
       LWXGB/LWXGB.o \
       RegP/RegPEstimator.o \
       RegP/HFTEstimator.o \
       EquiDepthHist/EqDepHist.o \
       MHist2/MHist2.o \
       MHist2/MHist2Bucket.o \
       QTS/QTS.o \
       QTS/IQTS.o \
       GxTree/GxTree.o \
       GxTree/Gxt_Node_G.o \
       GxTree/Gxt_Node_L.o \
       GxTree/Gxt_Node_M.o \
       GxTree/Gxt_Node_S.o \
       GxTree/GxtTypes.o \
       scale/scale.o \
       OneDEqDepHist/OneDEqDepHist.o

OBJY = $(addprefix $(H2DIR)/, $(OFSY))

HDRZ = infra/cb.hh \
       infra/util.hh \
       infra/types.hh \
       infra/data2dim.hh \
       infra/summaryline.hh \
       infra/RegularPartitioning2dim.hh \
       infra/EstimatorBase2dim.hh \
       infra/cmeasure.h \


OFSZ = infra/RegularPartitioning2dim.o \
       infra/EstimatorBase2dim.o \
       infra/summaryline.o \
       infra/data2dim.o \
       infra/cb.o \
       infra/util.o \
       infra/HighlyFrequentTile.o \
       infra/types.o \

OBJZ = $(addprefix $(H2DIR)/, $(OFSZ))

 
BFS = main_queryset_estimates \

AFS = $(BFS)

ALL = $(addprefix $(OBJDIR)/, $(AFS))

all : $(ALL)

bin : all
	cp main_analyze     ../../bin
	cp main_regp        ../../bin
	cp main_correlation ../../bin
	cp main_select      ../../bin
	cp main_convdic     ../../bin
	cp main_total       ../../bin


$(OBJDIR)/main_ZHist : $(OBJDIR)/main_ZHist.o $(OBJX) $(OBJY) $(OBJZ) $(OBJDIR)/arg.o $(OBJINFRAG)
	$(CC) -o $@ $^

$(OBJDIR)/main_ZHist.o : main_ZHist.cc $(HDRX) $(HDRY) $(HDRZ) $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(CINCL) -o $@ main_ZHist.cc


$(OBJDIR)/main_correlation : $(OBJDIR)/main_correlation.o $(OBJX) $(OBJY) $(OBJZ) $(OBJDIR)/arg.o $(OBJINFRAG)
	$(CC) -o $@ $^

$(OBJDIR)/main_correlation.o : main_correlation.cc $(HDRX) $(HDRY) $(HDRZ) $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(CINCL) -o $@ main_correlation.cc

$(OBJDIR)/main_select : $(OBJDIR)/main_select.o $(OBJX) $(OBJY) $(OBJZ) $(OBJDIR)/arg.o $(OBJINFRAG)
	$(CC) -o $@ $^

$(OBJDIR)/main_select.o : main_select.cc $(HDRX) $(HDRY) $(HDRZ) $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(CINCL) -o $@ main_select.cc

$(OBJDIR)/main_total : $(OBJDIR)/main_total.o $(OBJX) $(OBJY) $(OBJZ) $(OBJDIR)/arg.o $(OBJINFRAG)
	$(CC) -o $@ $^

$(OBJDIR)/main_total.o : main_total.cc $(HDRX) $(HDRY) $(HDRZ) $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(CINCL) -o $@ main_total.cc

$(OBJDIR)/main_stat : $(OBJDIR)/main_stat.o $(OBJX) $(OBJY) $(OBJZ) $(OBJDIR)/arg.o $(OBJINFRAG)
	$(CC) -o $@ $^

$(OBJDIR)/main_stat.o : main_stat.cc $(HDRX) $(HDRY) $(HDRZ) $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(CINCL) -o $@ main_stat.cc



$(OBJDIR)/main_hft : $(OBJDIR)/main_hft.o $(OBJX) $(OBJY) $(OBJZ) $(OBJDIR)/arg.o $(OBJINFRAG)
	$(CC) -o $@ $^

$(OBJDIR)/main_hft.o : main_hft.cc $(HDRX) $(HDRY) $(HDRZ) $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(CINCL) -o $@ main_hft.cc

$(OBJDIR)/main_regp : $(OBJDIR)/main_regp.o $(OBJX) $(OBJY) $(OBJZ) $(OBJDIR)/arg.o $(OBJINFRAG)
	$(CC) -o $@ $^

$(OBJDIR)/main_regp.o : main_regp.cc $(HDRX) $(HDRY) $(HDRZ) $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(CINCL) -o $@ main_regp.cc


$(OBJDIR)/main_convdic : $(OBJDIR)/main_convdic.o $(OBJDIR)/arg.o $(OBJX) $(OBJY) $(OBJZ) $(OBJINFRAG)
	$(CC) -o $@ $^

$(OBJDIR)/main_convdic.o : main_convdic.cc $(HDRX) $(HDRY) $(HDRZ) $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(CINCL) -o $@ main_convdic.cc

$(OBJDIR)/main_discretize : $(OBJDIR)/main_discretize.o $(OBJZ) $(OBJDIR)/arg.o $(OBJINFRAG)
	$(CC) -o $@ $^

$(OBJDIR)/main_discretize.o : main_discretize.cc  $(HDRZ) $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(CINCL) -o $@ main_discretize.cc 


#$(OBJDIR)/main_analyze : $(OBJDIR)/main_analyze.o $(OBJDIR)/arg.o $(OBJX) $(OBJY) $(OBJZ) $(OBJINFRAG)
#	$(CC) $(LDINCL) -o $@ $^

#$(OBJDIR)/main_analyze.o : main_analyze.cc $(HDRX) $(HDRY) $(HDRZ) $(HDRINFRA)
#	$(CC) -c $(CFLAGS) $(CINCL) -o $@ main_analyze.cc


$(OBJDIR)/main_ana11 : $(OBJDIR)/main_ana11.o $(OBJDIR)/arg.o $(OBJDIR)/fparamloop.o $(OBJDIR)/fprocess.o $(OBJX) $(OBJY) $(OBJZ) $(OBJINFRAG)
	$(CC) $(LDINCL) -o $@ $^

$(OBJDIR)/main_ana11.o : main_ana11.cc fparamloop.hh fprocess.hh $(HDRX) $(HDRY) $(HDRZ) $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(CINCL) -o $@ main_ana11.cc

$(OBJDIR)/main_gen_query : $(OBJDIR)/main_gen_query.o $(OBJDIR)/arg.o $(OBJX) $(OBJY) $(OBJZ) $(OBJINFRAG)
	$(CC) $(LDINCL) -o $@ $^

$(OBJDIR)/main_gen_query.o : main_gen_query.cc $(HDRX) $(HDRY) $(HDRZ) $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(CINCL) -o $@ main_gen_query.cc


$(OBJDIR)/main_ana2dim : $(OBJDIR)/main_ana2dim.o $(OBJX) $(OBJY) $(OBJZ) $(OBJDIR)/arg.o $(OBJINFRAG)
	$(CC) -o $@ $^

$(OBJDIR)/main_ana2dim.o : main_ana2dim.cc $(HDRX) $(HDRY) $(HDRZ) $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(CINCL) -o $@ main_ana2dim.cc

$(OBJDIR)/fparamloop.o : fparamloop.cc fparamloop.hh fprocess.hh $(HDRX) $(HDRY) $(HDRZ) infra/cb.hh $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(CINCL) -o $@ fparamloop.cc

$(OBJDIR)/main_dataset_stats : $(OBJDIR)/main_dataset_stats.o $(H2DIR)/infra/data2dim.o
	$(CC) -o $@ $^

$(OBJDIR)/main_dataset_stats.o : main_dataset_stats.cc $(HDRZ) $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(CINCL) -o $@ main_dataset_stats.cc

$(OBJDIR)/main_budget : $(OBJDIR)/main_budget.o 
	$(CC) -o $@ $^

$(OBJDIR)/main_budget.o : main_budget.cc $(HDRZ) $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(CINCL) -o $@ main_budget.cc

$(OBJDIR)/main_queryset_estimates : $(OBJDIR)/main_queryset_estimates.o $(OBJDIR)/arg.o $(OBJDIR)/process_query_file.o $(OBJX) $(OBJY) $(OBJZ) $(OBJINFRAG)
	$(CC) $(LDINCL) $(LIBDIR) -fopenmp -o $@  $^ -l xgboost

$(OBJDIR)/main_queryset_estimates.o : main_queryset_estimates.cc process_query_file.hh $(HDRX) $(HDRY) $(HDRZ) $(HDRINFRA)
	$(CC) -c $(CFLAGS)-fopenmp $(CINCL) -o $@ main_queryset_estimates.cc

$(OBJDIR)/main_sumsum : $(OBJDIR)/main_sumsum.o $(OBJZ) $(OBJINFRAG)
	$(CC) -o $@ $^

$(OBJDIR)/main_sumsum.o : main_sumsum.cc $(HDRINFRA) $(HDRINFRAG)
	$(CC) -c $(CFLAGS) $(CINCL) -o $@ main_sumsum.cc

$(OBJDIR)/main_sum_qerrprofile : $(OBJDIR)/main_sum_qerrprofile.o $(OBJZ) $(OBJINFRAG)
	$(CC) -o $@ $^

$(OBJDIR)/main_sum_qerrprofile.o : main_sum_qerrprofile.cc $(HDRINFRA) $(HDRINFRAG)
	$(CC) -c $(CFLAGS) $(CINCL) -o $@ main_sum_qerrprofile.cc



$(OBJDIR)/process_query_file.o : process_query_file.cc process_query_file.hh $(HDRX) $(HDRY) $(HDRZ) infra/cb.hh $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(CINCL) -o $@ process_query_file.cc

$(OBJDIR)/fprocess.o : fprocess.cc fprocess.hh $(HDRX) $(HDRY) $(HDRZ) infra/cb.hh $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(CINCL) -o $@ fprocess.cc

$(OBJDIR)/arg.o : arg.cc arg.hh infra/cb.hh $(HDRINFRA)
	$(CC) -c $(CFLAGS) $(CINCL) -o $@ arg.cc

clean :
	rm -f *.o a.out 

include ${SRCBASEDIR}/fin.mk


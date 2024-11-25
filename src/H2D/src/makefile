
SYSDIRS = infra scale CheckError Sampling \
          EstimatorConst EstimatorArea RegP GridTree EquiDepthHist EquiDepthHist2 \
          DiscreteCosineTransform Wavelet OneDEqDepHist \
          NonoTree GenHist Phased HiRed MHist MHist2 QTS FlexTree GxTree ZHist XGBoost EXGB LWXGB

all:
	for i in $(SYSDIRS); do \
          (cd $$i; $(MAKE) all); \
        done; \
	(cd main; $(MAKE) );

odi:
	for i in $(SYSDIRS); do \
          (cd $$i; $(MAKE) ODI=xxx); \
        done; \
	(cd main; $(MAKE) ODI=xxx);

hdi:
	for i in $(SYSDIRS); do \
          (cd $$i; $(MAKE) HDI=xxx); \
        done; \
	(cd main; $(MAKE) HDI=xxx);

bin: 
	for i in $(SYSDIRS); do \
          (cd $$i; $(MAKE) bin); \
        done; \
	(cd main; $(MAKE) bin);


clean:
	for i in $(SYSDIRS); do \
          (cd $$i; $(MAKE) clean); \
        done; \
	(cd main; $(MAKE) clean);



#ifndef H2D_MAIN_FPARAMLOOP_HH
#define H2D_MAIN_FPARAMLOOP_HH

#include <iostream>
#include <iomanip>
#include <fstream>

#include <string.h>
#include <string>
#include <vector>

#include "infra/argbase.hh"
#include "infra/aggregate.hh"
#include "infra/matrix.hh"
#include "infra/measure.hh"
#include "infra/tmath.hh"

#include "infra/types.hh"
#include "infra/cb.hh"
#include "infra/util.hh"
#include "infra/data2dim.hh"
#include "infra/summaryline.hh"
#include "infra/RegularPartitioning2dim.hh"

#include "EstimatorConst/EstimatorConst.hh"
#include "EstimatorArea/EstimatorArea.hh"
#include "RegP/RegPEstimator.hh"
#include "RegP/HFTEstimator.hh"
#include "Sampling/Sample2dim.hh"
#include "GridTree/GridTree.hh"
#include "EquiDepthHist/EqDepHist.hh"
#include "EquiDepthHist2/HTree2dim.hh"
#include "DiscreteCosineTransform/DiscreteCosineTransform.hh"
#include "DiscreteCosineTransform/DiscreteCosineTransformOnCumFreq.hh"
#include "Wavelet/WaveletNonStdEstimator.hh"
#include "NonoTree/NonoTree.hh"
#include "GenHist/GenHist.hh"
#include "Phased/Phased.hh"
#include "HiRed/HiRed.hh"
#include "MHist/MHist.hh"
#include "MHist2/MHist2.hh"
#include "QTS/QTS.hh"
#include "QTS/IQTS.hh"
#include "FlexTree/FlexTree.hh"
#include "GxTree/GxTree.hh"

#include "CheckError/CheckErrorUtil.hh"
#include "CheckError/TestRandom2dim.hh"
#include "CheckError/TestRegular2dim.hh"

#include "arg.hh"
#include "fprocess.hh"


void fParamLoopEstConst(const H2D::Data2dim& aData, H2D::Cb& aCb);
void fParamLoopEstArea(const H2D::Data2dim& aData, H2D::Cb& aCb);
void fParamLoopGenHist(const H2D::Data2dim& aData, H2D::Cb& aCb);
void fParamLoopNonoTree(const H2D::Data2dim& aData, H2D::Cb& aCb);
void fParamLoopQTS(const H2D::Data2dim& aData, H2D::Cb& aCb);
void fParamLoopIQTS(const H2D::Data2dim& aData, H2D::Cb& aCb);
void fParamLoopFlexTree(const H2D::Data2dim& aData, H2D::Cb& aCb);
void fParamLoopGxTree(const H2D::Data2dim& aData, H2D::Cb& aCb);
void fParamLoopGridTree(const H2D::Data2dim& aData, H2D::Cb& aCb);
void fParamLoopEquiDepthHist(const H2D::Data2dim& aData, H2D::Cb& aCb);
void fParamLoopEquiDepthHist2(const H2D::Data2dim& aData, H2D::Cb& aCb);
void fParamLoopPhased(const H2D::Data2dim& aData, H2D::Cb& aCb);
void fParamLoopDct(const H2D::Data2dim& aData, H2D::Cb& aCb);
void fParamLoopDctCf(const H2D::Data2dim& aData, H2D::Cb& aCb);
void fParamLoopWaveletNonStd(const H2D::Data2dim& aData, H2D::Cb& aCb);
void fParamLoopRegP(const H2D::Data2dim& aData, H2D::Cb& aCb);
void fParamLoopHft(const H2D::Data2dim& aData, H2D::Cb& aCb);
void fParamLoopHiRed(const H2D::Data2dim& aData, H2D::Cb& aCb);
void fParamLoopMHist(const H2D::Data2dim& aData, H2D::Cb& aCb);
void fParamLoopMHist2(const H2D::Data2dim& aData, H2D::Cb& aCb);
void fParamLoopSample(const H2D::Data2dim& aData, H2D::Cb& aCb);
void fParamLoopZHist(const H2D::Data2dim& aData, H2D::Cb& aCb);



#endif

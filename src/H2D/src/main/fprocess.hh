#ifndef H2D_MAIN_FPROCESS_HH
#define H2D_MAIN_FPROCESS_HH

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
#include "ZHist/ZHist.hh"

#include "CheckError/CheckErrorUtil.hh"
#include "CheckError/TestRandom2dim.hh"
#include "CheckError/TestRegular2dim.hh"

#include "arg.hh"


void fRunQuery(const H2D::Data2dim& aData, const H2D::Cb& aCb);

void fProcessEstConst(const H2D::Data2dim& aData, const H2D::Cb& aCb);
void fProcessEstArea(const H2D::Data2dim& aData, const H2D::Cb& aCb);
void fProcessGenHist(const H2D::Data2dim& aData, const H2D::Cb& aCb);
void fProcessNonoTree(const H2D::Data2dim& aData, const H2D::Cb& aCb);
void fProcessQTS(const H2D::Data2dim& aData, const H2D::Cb& aCb);
void fProcessIQTS(const H2D::Data2dim& aData, const H2D::Cb& aCb);
void fProcessFlexTree(const H2D::Data2dim& aData, const H2D::Cb& aCb);
void fProcessGxTree(const H2D::Data2dim& aData, const H2D::Cb& aCb);
void fProcessGridTree(const H2D::Data2dim& aData, const H2D::Cb& aCb);
void fProcessEquiDepthHist(const H2D::Data2dim& aData, const H2D::Cb& aCb);
void fProcessEquiDepthHist2(const H2D::Data2dim& aData, const H2D::Cb& aCb);
void fProcessPhased(const H2D::Data2dim& aData, const H2D::Cb& aCb);
void fProcessDct(const H2D::Data2dim& aData, const H2D::Cb& aCb);
void fProcessDctCf(const H2D::Data2dim& aData, const H2D::Cb& aCb);
void fProcessWaveletNonStd(const H2D::Data2dim& aData, const H2D::Cb& aCb);
void fProcessRegP(const H2D::Data2dim& aData, const H2D::Cb& aCb);
void fProcessZHist(const H2D::Data2dim& aData, const H2D::Cb& aCb);
void fProcessHft(const H2D::Data2dim& aData, const H2D::Cb& aCb);
void fProcessHiRed(const H2D::Data2dim& aData, const H2D::Cb& aCb);
void fProcessMHist(const H2D::Data2dim& aData, const H2D::Cb& aCb);
void fProcessMHist2(const H2D::Data2dim& aData, const H2D::Cb& aCb);
void fProcessSample(const H2D::Data2dim& aData, const H2D::Cb& aCb);

#endif

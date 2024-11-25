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


void
fRunQuery(const H2D::Data2dim& aData,
          const H2D::Cb&       aCb) {
  char lBuffer[256];
  if(0 == aCb.query().size()) {
    std::cerr << "W no query given to fRunQuery" << std::endl;
    return;
  }
  H2D::rectangle_t lQueryRectangle;
  if(!lQueryRectangle.setTo(aCb.query().c_str())) {
    std::cerr << "query rectangle not parsable." << std::endl;
    return;
  }


  H2D::GxTree*    lGxt    = 0;
  H2D::GxTreeItp* lGxtItp = 0;


  if(aCb.gxtree()) {
      const bool lCheckEncoding = true;
      lGxt = new H2D::GxTree(aData, (H2D::GxTree::gx_kind_t) aCb.kind(), aCb.budget(),
                     aCb.leafRefinement(), aCb.lrf(),
                     aCb.minimumNodeTotal(),
                     aCb.phi(), aCb.q(), aCb.theta(), aCb.trace());
      lGxt->encode(lCheckEncoding);
      lGxt->printEncodingInfo(std::cout);
      lGxtItp = new H2D::GxTreeItp(lGxt->outlier(), lGxt->encoding(), aCb.trace());
      lGxt->_gxtitp = lGxtItp;
      std::cout << std::endl;
      std::cout << "Gx-Tree:" << std::endl;
      std::cout << "       q: " << lGxt->q() << std::endl;
      std::cout << "   theta: " << lGxt->theta() << std::endl;
      std::cout << "    kind: " << lGxt->kind() << std::endl;
      std::cout << "   depth: " << lGxt->depth() << std::endl;
      std::cout << "   #node: " << lGxt->noNodes() << std::endl;
      std::cout << " minleaf: " << lGxt->minLeafCumFreq() << std::endl;
      std::cout << " maxleaf: " << lGxt->maxLeafCumFreq() << std::endl;
      std::cout << "    size: " << lGxt->calcSize() << std::endl;
      std::cout << "#outlier: " << lGxt->noOutlier() << std::endl;
      std::cout << " #points: " << aData.size() << std::endl;
      std::cout << "max freq: " << aData.maxFrequency() << std::endl;
      std::cout << "tree theta: " << lGxt->theta() << std::endl;
      std::cout << "itp  theta: " << lGxtItp->theta() << std::endl;
      
      std::cout << std::endl;
  }

  bool lDoAnotherQuery = false;
  do {
    const double lTrueCard = aData.countWithin(lQueryRectangle);
    if(aCb.estConst()) {
    } 
    if(aCb.estArea()) {
    }
    if(aCb.regp()) {
    }
    if(aCb.eqd()) {
    }
    if(aCb.eqd2()) {
    }
    if(aCb.dct()) {
    }
    if(aCb.dctcf()) {
    }
    if(aCb.nono()) {
    }
    if(aCb.genhist()) {
    }
    if(aCb.waveletNonStd()) {
    }
    if(aCb.hft()) {
    }
    if(aCb.phased()) {
    }
    if(aCb.hired()) {
    }
    if(aCb.mhist()) {
    }
    if(aCb.mhist2()) {
    }
    if(aCb.qts()) {
    }
    if(aCb.iqts()) {
    }
    if(aCb.flextree()) {
    }
    if(aCb.gxtree()) {
      const double lEstimateTree = lGxt->estimate(lQueryRectangle);
      const double lEstimateItp  = lGxtItp->estimate(lQueryRectangle);
      const double lQErrorTree   = q::qerrorT1<double>(lEstimateTree, lTrueCard);
      const double lQErrorItp    = q::qerrorT1<double>(lEstimateItp , lTrueCard);
      std::cout << "query rectangle   = " << lQueryRectangle << std::endl;
      std::cout << "tree  estimate    = " << lEstimateTree   << std::endl;
      std::cout << "itp   estimate    = " << lEstimateItp    << std::endl;
      std::cout << "true  cardinality = " << lTrueCard       << std::endl;
      std::cout << "tree    q-error   = " << lQErrorTree     << std::endl;
      std::cout << "itp     q-error   = " << lQErrorItp      << std::endl;
    }
    if(aCb.gridtree()) {
    }
    if(0 < aCb.sampleSize()) {
    }

    char x = 'y';
    while('y' == x) {
      std::cout << "another query? [y/n]: ";
      std::cout.flush();
      std::cin >> x;
      if('y' == x || '[' == x) {
        if('[' != x) {
          std::cout << "query rectangle: ";
          std::cout.flush();
        }
        char c = x;
        while('[' != c) {
          std::cin >> c;
        }
        lBuffer[0] = c;
        std::cin.getline(lBuffer + 1, 250);
        if(lQueryRectangle.setTo(lBuffer)) {
          lDoAnotherQuery = true;
          x = 'n';
        } else {
          std::cout << "invalid rectangle." << std::endl;
        }
      } else {
        lDoAnotherQuery = false;
      }
    }
  }
  while(lDoAnotherQuery);
}


void
fProcessEstConst(const H2D::Data2dim& aData,
                 const H2D::Cb&       aCb) {
  const double lTotal = (double) aData.total();
  const double lTheta[5] = { (lTotal / 100),
                             (lTotal /  50),
                             (lTotal /  33),
                             (lTotal /  20),
                             (lTotal /  10) };
  double lQErrorMax[5] = { 0, 0, 0, 0, 0 };
  double lQErrorAvg[5] = { 0, 0, 0, 0, 0 };

  H2D::summaryline_t lSummaryLine;
  lSummaryLine._card = aData.total();
  lSummaryLine._noDv = aData.size();
  lSummaryLine._nout = 0;
  lSummaryLine._budget = aCb.budget();
  lSummaryLine._size = 4;
  lSummaryLine._phi  = 0;
  lSummaryLine._theta = aCb.theta();
  lSummaryLine._kind  = H2D::H2D_EST_CONST;
  lSummaryLine._subkind = 0;
  lSummaryLine._filename = std::string(H2D::gbasename(aCb.filename().c_str()));
  
  for(uint i = 0; i < 5; ++i) {
    Measure lMeasure;
    lMeasure.start();
    H2D::EstimatorConst lEstimator(aData, lTheta[i]);
    lMeasure.stop();
    lSummaryLine._constructionTime = lMeasure.mCpuTime();
    if(aCb.shortErrorCheck()) {
      fAnalyzeEstimator(aData, lEstimator, aCb, lSummaryLine, true, true);
    } else
    if(aCb.checkError()) {
      fAnalyzeEstimator(aData, lEstimator, aCb, lSummaryLine);
    }
    lQErrorMax[i] = lSummaryLine._max[i];
    lQErrorAvg[i] = lSummaryLine._avg[i];
  }

  for(uint i = 0; i < 5; ++i) {
    lSummaryLine._max[i] = lQErrorMax[i];
    lSummaryLine._avg[i] = lQErrorAvg[i];
  }

  std::cout << lSummaryLine << std::endl;

}


void
fProcessEstArea(const H2D::Data2dim& aData,
                const H2D::Cb&       aCb) {
  const double lTotal = (double) aData.total();
  const double lTheta[5] = { (lTotal / 100),
                             (lTotal /  50),
                             (lTotal /  33),
                             (lTotal /  20),
                             (lTotal /  10) };
  double lQError[5] = { 0, 0, 0, 0, 0 };

  H2D::summaryline_t lSummaryLine;
  lSummaryLine._card = aData.total();
  lSummaryLine._noDv = aData.size();
  lSummaryLine._nout = 0;
  lSummaryLine._budget = aCb.budget();
  lSummaryLine._size = 4;
  lSummaryLine._phi  = 0;
  lSummaryLine._theta = aCb.theta();
  lSummaryLine._kind  = H2D::H2D_EST_AREA;
  lSummaryLine._subkind = 0;
  lSummaryLine._filename = std::string(H2D::gbasename(aCb.filename().c_str()));

  for(uint i = 0; i < 5; ++i) {
    Measure lMeasure;
    lMeasure.start();
    H2D::EstimatorArea lEstimator(aData, lTheta[i]);
    lMeasure.stop();
    lSummaryLine._constructionTime = lMeasure.mCpuTime();
    if(aCb.shortErrorCheck()) {
      fAnalyzeEstimator(aData, lEstimator, aCb, lSummaryLine, true, true);
    } else
    if(aCb.checkError()) {
      fAnalyzeEstimator(aData, lEstimator, aCb, lSummaryLine);
    }
    lQError[i] = lSummaryLine._max[i];
  }

  for(uint i = 0; i < 5; ++i) {
    lSummaryLine._max[i] = lQError[i];
  }

  std::cout << lSummaryLine << std::endl;

}





void
fProcessGenHist(const H2D::Data2dim& aData,
                const H2D::Cb&       aCb) {
  Measure lMeasure;
  lMeasure.start();
  H2D::GenHist lGenHist(aData, 
                        (1 == aCb.kind() ? H2D::GenHist::k_prime : H2D::GenHist::k_original),
                        aCb.alpha(), 
                        aCb.delta(),
                        aCb.nx(), 
                        aCb.ny(), 
                        aCb.phi(), 
                        aCb.q(), 
                        aCb.trace());
  lMeasure.stop();

  std::cout << "GenHist : " << std::endl;
  std::cout << "    kind: " << lGenHist.kind() << std::endl;
  std::cout << "    size: " << lGenHist.size() << std::endl;
  std::cout << "#outlier: " << lGenHist.noOutlier() << std::endl;
  std::cout << "#buckets: " << lGenHist.noBuckets() << std::endl;
  std::cout << " #points: " << aData.size() << std::endl;
  std::cout << "max freq: " << aData.maxFrequency() << std::endl;

  if(aCb.print()) {
    lGenHist.print(std::cout);
  }
  std::cout << "no       buckets        : " << lGenHist.noBuckets() << std::endl;
  std::cout << "no small buckets (<=  5): " << lGenHist.noSmallBuckets( 5) << std::endl;
  std::cout << "no small buckets (<= 10): " << lGenHist.noSmallBuckets(10) << std::endl;
  std::cout << "no small buckets (<= 20): " << lGenHist.noSmallBuckets(20) << std::endl;

  double lConstructionTime = lMeasure.mCpuTime();

  H2D::summaryline_t lSummaryLine;
  lSummaryLine._card = aData.total();
  lSummaryLine._noDv = aData.size();
  lSummaryLine._nout = lGenHist.noOutlier();
  lSummaryLine._budget = aCb.budget();
  lSummaryLine._size = lGenHist.size();
  lSummaryLine._phi  = lGenHist.phi();
  lSummaryLine._theta = aCb.theta();
  lSummaryLine._constructionTime = lConstructionTime;
  lSummaryLine._kind = H2D::H2D_GENHIST;
  lSummaryLine._subkind = lGenHist.kind();
  lSummaryLine._filename = std::string(H2D::gbasename(aCb.filename().c_str()));
  lSummaryLine._param._genhist._alpha = lGenHist.alpha();
  lSummaryLine._param._genhist._delta = lGenHist.delta();
  lSummaryLine._param._genhist._nx = lGenHist.nx();
  lSummaryLine._param._genhist._ny = lGenHist.ny();
  lSummaryLine._param._genhist._q = lGenHist.q();
  lSummaryLine._param._genhist._noBuckets = lGenHist.noBuckets();

  std::cout << "hist kind: " << h2d_kind_name(lSummaryLine._kind) << std::endl;

  const bool lShort = aCb.shortErrorCheck();
  if(lShort) {
    fAnalyzeEstimator(aData, lGenHist, aCb, lSummaryLine, true, true);
  } else {
    fAnalyzeEstimator(aData, lGenHist, aCb, lSummaryLine);
  }

  std::cout << lSummaryLine << std::endl;

}

void
fProcessNonoTree(const H2D::Data2dim& aData,
                 const H2D::Cb&       aCb) {
  Measure lMeasure;
  lMeasure.start();
  H2D::NonoTree lNonoTree(aData, aCb.kind(), aCb.nx(), aCb.ny(), aCb.delta(), aCb.kappa(), 
                          aCb.shrinkage(),
                          aCb.phi(), aCb.q(), aCb.theta(), aCb.trace());
  lMeasure.stop();

  std::cout << "Nono-Tree " << std::endl;
  std::cout << "    kind: " << lNonoTree.kind() << std::endl;
  std::cout << "   depth: " << lNonoTree.depth() << std::endl;
  std::cout << "   #node: " << lNonoTree.noNodes() << std::endl;
  std::cout << "    size: " << lNonoTree.size() << std::endl;
  std::cout << "#outlier: " << lNonoTree.noOutlier() << std::endl;
  std::cout << " #points: " << aData.size() << std::endl;
  std::cout << "max freq: " << aData.maxFrequency() << std::endl;

  if(aCb.print()) {
    lNonoTree.print(std::cout);
  }

  double lConstructionTime = lMeasure.mCpuTime();

  H2D::double_vt lShrinkageFactors;
  lNonoTree.getShrinkage(lShrinkageFactors);
  std::sort(lShrinkageFactors.begin(), lShrinkageFactors.end());
  std::cout << std::endl << "--- begin shrinkage factors ---" << std::endl;
  for(uint i = 0; i < lShrinkageFactors.size(); ++i) {
    std::cout << lShrinkageFactors[i] << ' ';
  }
  std::cout << std::endl << "--- end shrinkage factors ---" << std::endl;

  H2D::summaryline_t lSummaryLine;
  lSummaryLine._card = aData.total();
  lSummaryLine._noDv = aData.size();
  lSummaryLine._nout = lNonoTree.noOutlier();
  lSummaryLine._budget = aCb.budget();
  lSummaryLine._size = lNonoTree.size();
  lSummaryLine._phi  = lNonoTree.phi();
  lSummaryLine._theta = aCb.theta();
  lSummaryLine._constructionTime = lConstructionTime;
  lSummaryLine._kind = H2D::H2D_NONOTREE;
  lSummaryLine._subkind = lNonoTree.kind();
  lSummaryLine._filename = std::string(H2D::gbasename(aCb.filename().c_str()));
  lSummaryLine._param._nono._nx    = lNonoTree.nx();
  lSummaryLine._param._nono._ny    = lNonoTree.ny();
  lSummaryLine._param._nono._kappa = lNonoTree.kappa();
  lSummaryLine._param._nono._shrinkage = aCb.shrinkage();
  lSummaryLine._param._nono._depth = lNonoTree.depth();
  lSummaryLine._param._nono._noNodes = lNonoTree.noNodes();

  std::cout << "hist kind: " << h2d_kind_name(lSummaryLine._kind) << std::endl;

  const bool lShort = aCb.shortErrorCheck();
  if(lShort) {
    fAnalyzeEstimator(aData, lNonoTree, aCb, lSummaryLine, true, true);
  } else {
    fAnalyzeEstimator(aData, lNonoTree, aCb, lSummaryLine);
  }

  std::cout << lSummaryLine << std::endl;

}


void
fProcessQTS(const H2D::Data2dim& aData,
            const H2D::Cb&       aCb) {
  Measure lMeasure;
  lMeasure.start();
  H2D::QTS lQTS(aData, (H2D::QTS::kind_t) aCb.kind(), aCb.budget(), aCb.phi(), aCb.q(), aCb.theta(), aCb.trace());
  lMeasure.stop();

  std::cout << "QTS:" << std::endl;
  std::cout << "    kind: " << lQTS.kind() << std::endl;
  std::cout << "   depth: " << lQTS.depth() << std::endl;
  std::cout << "   #node: " << lQTS.noNodes() << std::endl;
  std::cout << "    size: " << lQTS.size() << std::endl;
  std::cout << "#outlier: " << lQTS.noOutlier() << std::endl;
  std::cout << " #points: " << aData.size() << std::endl;
  std::cout << "max freq: " << aData.maxFrequency() << std::endl;

  if(aCb.print()) {
    lQTS.print(std::cout);
  }

  double lConstructionTime = lMeasure.mCpuTime();

  H2D::summaryline_t lSummaryLine;
  lSummaryLine._card = aData.total();
  lSummaryLine._noDv = aData.size();
  lSummaryLine._nout = lQTS.noOutlier();
  lSummaryLine._budget = aCb.budget();
  lSummaryLine._size = lQTS.size();
  lSummaryLine._phi  = lQTS.phi();
  lSummaryLine._theta = aCb.theta();
  lSummaryLine._constructionTime = lConstructionTime;
  lSummaryLine._kind = H2D::H2D_QTS;
  lSummaryLine._subkind = lQTS.kind();
  lSummaryLine._filename = std::string(H2D::gbasename(aCb.filename().c_str()));
  lSummaryLine._param._qts._depth   = lQTS.depth();
  lSummaryLine._param._qts._noNodes = lQTS.noNodes();

  std::cout << "hist kind: " << h2d_kind_name(lSummaryLine._kind) << std::endl;

  const bool lShort = aCb.shortErrorCheck();
  if(lShort) {
    fAnalyzeEstimator(aData, lQTS, aCb, lSummaryLine, true, true);
  } else {
    fAnalyzeEstimator(aData, lQTS, aCb, lSummaryLine);
  }

  std::cout << lSummaryLine << std::endl;

}


void
fProcessIQTS(const H2D::Data2dim& aData,
             const H2D::Cb&       aCb) {
  Measure lMeasure;
  lMeasure.start();
  H2D::IQTS lIQTS(aData, (H2D::IQTS::kind_t) aCb.kind(), aCb.budget(), aCb.phi(), aCb.q(), aCb.theta(), aCb.trace());
  lMeasure.stop();

  std::cout << "IQTS:" << std::endl;
  std::cout << "       q: " << lIQTS.q() << std::endl;
  std::cout << "   theta: " << lIQTS.theta() << std::endl;
  std::cout << "    kind: " << lIQTS.kind() << std::endl;
  std::cout << "   depth: " << lIQTS.depth() << std::endl;
  std::cout << "   #node: " << lIQTS.noNodes() << std::endl;
  std::cout << "    size: " << lIQTS.size() << std::endl;
  std::cout << "#outlier: " << lIQTS.noOutlier() << std::endl;
  std::cout << " #points: " << aData.size() << std::endl;
  std::cout << "max freq: " << aData.maxFrequency() << std::endl;

  if(aCb.print()) {
    lIQTS.print(std::cout);
  }

  double lConstructionTime = lMeasure.mCpuTime();

  H2D::summaryline_t lSummaryLine;
  lSummaryLine._card = aData.total();
  lSummaryLine._noDv = aData.size();
  lSummaryLine._nout = lIQTS.noOutlier();
  lSummaryLine._budget = aCb.budget();
  lSummaryLine._size = lIQTS.size();
  lSummaryLine._phi  = lIQTS.phi();
  lSummaryLine._theta = aCb.theta();
  lSummaryLine._constructionTime = lConstructionTime;
  lSummaryLine._kind = H2D::H2D_IQTS;
  lSummaryLine._subkind = lIQTS.kind();
  lSummaryLine._filename = std::string(H2D::gbasename(aCb.filename().c_str()));
  lSummaryLine._param._iqts._depth   = lIQTS.depth();
  lSummaryLine._param._iqts._noNodes = lIQTS.noNodes();
  lSummaryLine._param._iqts._noNotRefined = lIQTS.noNotRefined();
  lSummaryLine._param._iqts._noLT23 = lIQTS.noLT23();
  lSummaryLine._param._iqts._noLT24 = lIQTS.noLT24();
  lSummaryLine._param._iqts._noLT2p = lIQTS.noLT2p();

  std::cout << "hist kind: " << h2d_kind_name(lSummaryLine._kind) << std::endl;

  const bool lShort = aCb.shortErrorCheck();
  if(lShort) {
    fAnalyzeEstimator(aData, lIQTS, aCb, lSummaryLine, true, true);
  } else {
    fAnalyzeEstimator(aData, lIQTS, aCb, lSummaryLine);
  }
  std::cout << lSummaryLine << std::endl;
}

void
fProcessFlexTree(const H2D::Data2dim& aData,
                 const H2D::Cb&       aCb) {
  Measure lMeasure;
  lMeasure.start();
  H2D::FlexTree lFxt(aData, (H2D::FlexTree::kind_t) aCb.kind(), aCb.budget(), aCb.kappa(), aCb.phi(), aCb.q(), aCb.theta(), aCb.trace());
  lMeasure.stop();

  std::cout << "FlexTree:" << std::endl;
  std::cout << "       q: " << lFxt.q() << std::endl;
  std::cout << "   theta: " << lFxt.theta() << std::endl;
  std::cout << "    kind: " << lFxt.kind() << std::endl;
  std::cout << "   depth: " << lFxt.depth() << std::endl;
  std::cout << "   #node: " << lFxt.noNodes() << std::endl;
  std::cout << " minleaf: " << lFxt.minLeafCumFreq() << std::endl;
  std::cout << " maxleaf: " << lFxt.maxLeafCumFreq() << std::endl;
  std::cout << "    size: " << lFxt.calcSize() << std::endl;
  std::cout << "#outlier: " << lFxt.noOutlier() << std::endl;
  std::cout << " #points: " << aData.size() << std::endl;
  std::cout << "max freq: " << aData.maxFrequency() << std::endl;

  if(aCb.print()) {
    lFxt.print(std::cout);
  }

  double lConstructionTime = lMeasure.mCpuTime();

  H2D::summaryline_t lSummaryLine;
  lSummaryLine._card = aData.total();
  lSummaryLine._noDv = aData.size();
  lSummaryLine._nout = lFxt.noOutlier();
  lSummaryLine._budget = aCb.budget();
  lSummaryLine._size = lFxt.size();
  lSummaryLine._phi  = lFxt.phi();
  lSummaryLine._theta = aCb.theta();
  lSummaryLine._constructionTime = lConstructionTime;
  lSummaryLine._kind = H2D::H2D_FLEXTREE;
  lSummaryLine._subkind = lFxt.kind();
  lSummaryLine._filename = std::string(H2D::gbasename(aCb.filename().c_str()));
  lSummaryLine._param._flextree._precisionLevel  = lFxt.precisionLevel();
  lSummaryLine._param._flextree._depth    = lFxt.depth();
  lSummaryLine._param._flextree._noNodes  = lFxt.noNodes();
  lSummaryLine._param._flextree._noGNodes = lFxt.noGNodes();
  lSummaryLine._param._flextree._minSplit   = lFxt.minSplit();
  lSummaryLine._param._flextree._maxUnsplit = lFxt.maxUnsplit();

  std::cout << "hist kind: " << h2d_kind_name(lSummaryLine._kind) << std::endl;

  const bool lShort = aCb.shortErrorCheck();
  if(lShort) {
    fAnalyzeEstimator(aData, lFxt, aCb, lSummaryLine, true, true);
  } else {
    fAnalyzeEstimator(aData, lFxt, aCb, lSummaryLine);
  }
  std::cout << lSummaryLine << std::endl;
}


void
fProcessGxTree(const H2D::Data2dim& aData,
               const H2D::Cb&       aCb) {
  bool lCheckEncoding = true;

  Measure lMeasureT;
  Measure lMeasureE;
  lMeasureE.start();
  lMeasureT.start();
  H2D::GxTree lGxt(aData, (H2D::GxTree::gx_kind_t) aCb.kind(), aCb.budget(), 
                   aCb.leafRefinement(), aCb.lrf(),
                   aCb.minimumNodeTotal(), 
                   aCb.phi(), aCb.q(), aCb.theta(), aCb.trace());
  lMeasureT.stop();
  lGxt.encode(lCheckEncoding);
  lMeasureE.stop();

  lGxt.printEncodingInfo(std::cout);
  H2D::GxTreeItp lGxtItp(lGxt.outlier(), lGxt.encoding(), false);
  lGxtItp.setTheta(aCb.theta());
  lGxtItp.setQ(aCb.q());
  
  std::cout << "Gx-Tree:" << std::endl;
  std::cout << "       q: " << lGxt.q() << std::endl;
  std::cout << "   theta: " << lGxt.theta() << std::endl;
  std::cout << "    kind: " << lGxt.kind() << std::endl;
  std::cout << "   depth: " << lGxt.depth() << std::endl;
  std::cout << "   #node: " << lGxt.noNodes() << std::endl;
  std::cout << " minleaf: " << lGxt.minLeafCumFreq() << std::endl;
  std::cout << " maxleaf: " << lGxt.maxLeafCumFreq() << std::endl;
  std::cout << "    size: " << lGxt.calcSize() << std::endl;
  std::cout << "#outlier: " << lGxt.noOutlier() << std::endl;
  std::cout << " #points: " << aData.size() << std::endl;
  std::cout << "max freq: " << aData.maxFrequency() << std::endl;

  if(aCb.print()) {
    lGxt.print(std::cout);
  }

  H2D::summaryline_t lSummaryLineE;
  lSummaryLineE._card = aData.total();
  lSummaryLineE._noDv = aData.size();
  lSummaryLineE._nout = lGxt.noOutlier();
  lSummaryLineE._budget = aCb.budget();
  lSummaryLineE._size = lGxt.size();
  lSummaryLineE._phi  = lGxt.phi();
  lSummaryLineE._theta = aCb.theta();
  lSummaryLineE._constructionTime = lMeasureE.mCpuTime();
  lSummaryLineE._kind = H2D::H2D_GXTREE;
  lSummaryLineE._subkind = lGxt.kind();
  lSummaryLineE._filename = std::string(H2D::gbasename(aCb.filename().c_str()));
  lSummaryLineE._param._gxtree._leafRefinement = lGxt.leafRefinement();
  lSummaryLineE._param._gxtree._lrf            = lGxt.lrf();
  lSummaryLineE._param._gxtree._minimumNodeTotal  = lGxt.minimumNodeTotal();
  lSummaryLineE._param._gxtree._depth    = lGxt.depth();
  lSummaryLineE._param._gxtree._noNodes  = lGxt.noNodes();
  lSummaryLineE._param._gxtree._noGNodes = lGxt.noGNodes();
  lSummaryLineE._param._gxtree._minSplit   = lGxt.minSplit();
  lSummaryLineE._param._gxtree._maxUnsplit = lGxt.maxUnsplit();
  lSummaryLineE._param._gxtree._encoded = 'E';

  H2D::summaryline_t lSummaryLineT = lSummaryLineE;
  lSummaryLineT._param._gxtree._encoded = 'T';
  lSummaryLineT._constructionTime = lMeasureT.mCpuTime();

  std::cout << "hist kind: " << h2d_kind_name(lSummaryLineE._kind) << std::endl;

  const bool lShort = aCb.shortErrorCheck();
  if(lShort) {
    lGxt._gxtitp = &lGxtItp;
    fAnalyzeEstimator(aData, lGxtItp, aCb, lSummaryLineE, true, true);
    fAnalyzeEstimator(aData, lGxt   , aCb, lSummaryLineT, true, true);
  } else {
    if(true || aCb.trace()) {
      std::cout << "### analyzing lGxtItp" << std::endl;
    }
    fAnalyzeEstimator(aData, lGxtItp, aCb, lSummaryLineE);
    if(aCb.debug()) {
      if(true || aCb.trace()) {
        std::cout << "### analyzing lGxt (original tree)" << std::endl;
      }
      lGxt._gxtitp = &lGxtItp;
      fAnalyzeEstimator(aData, lGxt, aCb, lSummaryLineT);
    }
  }
  if(aCb.debug()) {
    std::cout << lSummaryLineT << std::endl;
  }
  std::cout << lSummaryLineE << std::endl;
}


void
fProcessGridTree(const H2D::Data2dim& aData,
                 const H2D::Cb&       aCb) {
  const H2D::GridTree::kind_t lKind = (H2D::GridTree::kind_t) aCb.kind();

  Measure lMeasure;
  lMeasure.start();

  H2D::GridTree lGta(aData, 
                     lKind,
                     aCb.delta(), aCb.kappa(), aCb.phi(), 
                     aCb.q(), aCb.theta(), aCb.trace());
  lMeasure.stop();

  std::cout << "GridTree:" << std::endl;
  std::cout << "       q: " << lGta.q() << std::endl;
  std::cout << "   theta: " << lGta.theta() << std::endl;
  std::cout << "    kind: " << lKind << std::endl;
  std::cout << "   depth: " << lGta.depth() << std::endl;
  std::cout << "   #node: " << lGta.noNodes() << std::endl;
  std::cout << "    size: " << lGta.size() << std::endl;
  std::cout << "#outlier: " << lGta.noOutlier() << std::endl;
  std::cout << " #points: " << aData.size() << std::endl;
  std::cout << "max freq: " << aData.maxFrequency() << std::endl;

  if(aCb.print()) {
    lGta.print(std::cout, 4);
  }

  double lConstructionTime = lMeasure.mCpuTime();

  H2D::summaryline_t lSummaryLine;
  lSummaryLine._card = aData.total();
  lSummaryLine._noDv = aData.size();
  lSummaryLine._nout = lGta.noOutlier();
  lSummaryLine._budget = aCb.budget();
  lSummaryLine._size = lGta.size();
  lSummaryLine._phi  = lGta.phi();
  lSummaryLine._theta = aCb.theta();
  lSummaryLine._constructionTime = lConstructionTime;
  lSummaryLine._kind = H2D::H2D_GridTreeAuto;
  lSummaryLine._subkind = lKind;
  lSummaryLine._filename = std::string(H2D::gbasename(aCb.filename().c_str()));
  lSummaryLine._param._gt._nx      = lGta.nx();
  lSummaryLine._param._gt._ny      = lGta.ny();
  lSummaryLine._param._gt._delta   = lGta.delta();
  lSummaryLine._param._gt._kappa   = lGta.kappa();
  lSummaryLine._param._gt._noZ     = lGta.noClassZ();
  lSummaryLine._param._gt._noS     = lGta.noClassS();
  lSummaryLine._param._gt._noM     = lGta.noClassM();
  lSummaryLine._param._gt._noL     = lGta.noClassL();
  lSummaryLine._param._gt._depth   = lGta.depth();
  lSummaryLine._param._gt._noNodes = lGta.noNodes();
 

  std::cout << "hist kind: " << h2d_kind_name(lSummaryLine._kind) << std::endl;

  const bool lShort = aCb.shortErrorCheck();
  if(lShort) {
    fAnalyzeEstimator(aData, lGta, aCb, lSummaryLine, true, true);
  } else {
    fAnalyzeEstimator(aData, lGta, aCb, lSummaryLine);
  }
  std::cout << lSummaryLine << std::endl;
}




void
fProcessEquiDepthHist(const H2D::Data2dim& aData,
                      const H2D::Cb&       aCb) {
  std::cout << "TRACE: " << aCb.trace() << ", "
            << "PRINT: " << aCb.print() << std::endl;

  // if budget is given, we must infer nx and ny
  // we always assume nx = ny

  uint lNx = aCb.nx();
  uint lNy = aCb.ny();
  
  if((0 < aCb.budget()) && ((0 == aCb.nx()) || (0 == aCb.ny()))) {
    // budget given in bytes
    // we have to distinguish by kind
    // const uint lEntrySize = 2 * 4 + 4; // 2 * domValSize() + freqValSize()
    // lEntrySize = 12 bytes
    double lDNx = 0;
    if(0 == aCb.kind()) {
      // k_simple
      // size = 4 + 4 + nx() * lEntrySize + nx() * ny() * lEntrySize
      // s = 8 + 12x + 12xx
      // zu loesen: 0 = (8-s) + 12x + 12xx
      lDNx = mt::solveQuadraticEquationPos<double>(12, 12, 8 - (int) aCb.budget());
    } else {
      // k_matrix
      // size = 8 + 12*x + (12+rx*ry)*x^2
      // this one should not be applied!!!
      const double s = 12 + aCb.rx() * aCb.ry();
      lDNx = mt::solveQuadraticEquationPos<double>(s, 12, 8 - (int) aCb.budget());
    }
    const int lNxLo = (int) mt::floort(lDNx);
    const int lNxHi = (int) mt::ceilt(lDNx);
    if( (lDNx - (double) (lNxLo * lNxLo))
        <=
        ((double) (lNxHi * lNxHi) - lDNx)) {
      lNx = lNy = lNxLo;
    } else {
      lNx = lNy = lNxHi;
    }
    if(true || aCb.trace()) {
      std::cout << "calculated from budget = " << aCb.budget() 
                << " we have nx = ny = " << lNx << std::endl;
    }
  }


  Measure lMeasure;
  lMeasure.start();
  H2D::EqDepHist lEqDepHist(aData, 
                            ((1 == aCb.kind()) ? H2D::EqDepHist::k_matrix 
                                               : H2D::EqDepHist::k_simple),
                            // aCb.nx(), aCb.ny(), 
                            lNx, lNy,
                            aCb.rx(), aCb.ry(),
                            aCb.phi(), aCb.q(), aCb.theta(), aCb.trace());
  lMeasure.stop();

  if(aCb.print()) {
    lEqDepHist.print(std::cout);
  }

  double lConstructionTime = lMeasure.mCpuTime();

  H2D::summaryline_t lSummaryLine;
  lSummaryLine._card = aData.total();
  lSummaryLine._noDv = aData.size();
  lSummaryLine._nout = lEqDepHist.noOutlier();
  lSummaryLine._budget = aCb.budget();
  lSummaryLine._size = lEqDepHist.size();
  lSummaryLine._phi  = lEqDepHist.phi();
  lSummaryLine._theta = aCb.theta();
  lSummaryLine._constructionTime = lConstructionTime;
  lSummaryLine._kind = H2D::H2D_EquiDepth;
  lSummaryLine._subkind = aCb.kind();
  lSummaryLine._filename = std::string(H2D::gbasename(aCb.filename().c_str()));
  lSummaryLine._param._eqdh._nx = lEqDepHist.nx();
  lSummaryLine._param._eqdh._ny = lEqDepHist.ny();
  lSummaryLine._param._eqdh._rx = aCb.rx();
  lSummaryLine._param._eqdh._ry = aCb.ry();

  std::cout << "hist kind: " << h2d_kind_name(lSummaryLine._kind) << std::endl;

  fAnalyzeEstimator(aData, lEqDepHist, aCb, lSummaryLine);

  std::cout << lSummaryLine << std::endl;
}



void
fProcessEquiDepthHist2(const H2D::Data2dim& aData,
                       const H2D::Cb&       aCb) {
  std::cout << "TRACE: " << aCb.trace() << ", "
            << "PRINT: " << aCb.print() << std::endl;

  // if budget is given, we must infer nx and ny
  // we always assume nx = ny

  uint lNx = aCb.nx();
  uint lNy = aCb.ny();
  
  if((0 < aCb.budget()) && ((0 == aCb.nx()) || (0 == aCb.ny()))) {
    double lDNx = 0;
    // k_simple
    // size = 8 * nx + 8 * ny * nx
    // s = 8 + 12x + 12xx
    // zu loesen: 0 = (8-s) + 8*x + 16^2
    lDNx = mt::solveQuadraticEquationPos<double>(16, 8, 8 - (int) aCb.budget());
    const int lNxLo = (int) mt::floort(lDNx);
    const int lNxHi = (int) mt::ceilt(lDNx);
    if( (lDNx - (double) (lNxLo * lNxLo))
        <=
        ((double) (lNxHi * lNxHi) - lDNx)) {
      lNx = lNy = lNxLo;
    } else {
      lNx = lNy = lNxHi;
    }
    if(true || aCb.trace()) {
      std::cout << "calculated from budget = " << aCb.budget() 
                << " we have nx = ny = " << lNx << std::endl;
    }
  }

  const bool lUniformScheme = (0 == aCb.kind());

  Measure lMeasure;
  lMeasure.start();
  H2D::HTree::HTree2dim lEqDepHist(lNx, lNy, aData, lUniformScheme);
  lMeasure.stop();

  // if(aCb.print()) {
  //   lEqDepHist.print(std::cout);
  // }

  double lConstructionTime = lMeasure.mCpuTime();

  H2D::summaryline_t lSummaryLine;
  lSummaryLine._card = aData.total();
  lSummaryLine._noDv = aData.size();
  lSummaryLine._nout = 0; // lEqDepHist.noOutlier();
  lSummaryLine._budget = aCb.budget();
  lSummaryLine._size = lEqDepHist.size();
  lSummaryLine._phi  = aCb.phi();
  lSummaryLine._theta = aCb.theta();
  lSummaryLine._constructionTime = lConstructionTime;
  lSummaryLine._kind = H2D::H2D_EquiDepth2;
  lSummaryLine._subkind = 0;
  lSummaryLine._filename = std::string(H2D::gbasename(aCb.filename().c_str()));
  lSummaryLine._param._eqdh._nx = lNx;
  lSummaryLine._param._eqdh._ny = lNy;
  lSummaryLine._param._eqdh._rx = 0;
  lSummaryLine._param._eqdh._ry = 0;

  std::cout << "hist kind: " << h2d_kind_name(lSummaryLine._kind) << std::endl;

  fAnalyzeEstimator(aData, lEqDepHist, aCb, lSummaryLine);

  std::cout << lSummaryLine << std::endl;
}




void
fProcessPhased(const H2D::Data2dim& aData,
               const H2D::Cb&       aCb) {
  std::cout << "TRACE: " << aCb.trace() << ", "
            << "PRINT: " << aCb.print() << std::endl;


  // if budget is given, we must infer nx and ny
  // we always assume nx = ny

  uint lNx = aCb.nx();
  uint lNy = aCb.ny();

  if((0 < aCb.budget()) && ((0 == aCb.nx()) || (0 == aCb.ny()))) {
    // budget given in bytes
    // we have to distinguish by kind
    // const uint lEntrySize = 2 * 4 + 4; // 2 * domValSize() + freqValSize()
    // lEntrySize = 12 bytes
    double lDNx = 0;
    if(0 == (aCb.kind() & H2D::Phased::k_withmatrix)) {
      // k_simple
      // size = 4 + 4 + nx() * lEntrySize + nx() * ny() * lEntrySize
      // s = 8 + 12x + 12xx
      // zu loesen: 0 = (8-s) + 12x + 12xx
      lDNx = mt::solveQuadraticEquationPos<double>(12, 12, 8 - (int) aCb.budget());
    } else {
      // k_matrix
      // size = 8 + 12*x + (12+rx*ry)*x^2
      // this one should not be applied!!!
      const double s = 12 + aCb.rx() * aCb.ry();
      lDNx = mt::solveQuadraticEquationPos<double>(s, 12, 8 - (int) aCb.budget());
    }
    const int lNxLo = (int) mt::floort(lDNx);
    const int lNxHi = (int) mt::ceilt(lDNx);
    if( (lDNx - (double) (lNxLo * lNxLo))
        <=
        ((double) (lNxHi * lNxHi) - lDNx)) {
      lNx = lNy = lNxLo;
    } else {
      lNx = lNy = lNxHi;
    }
    if(true || aCb.trace()) {
      std::cout << "calculated from budget = " << aCb.budget()
                << " we have nx = ny = " << lNx << std::endl;
    }
  }



  Measure lMeasure;
  lMeasure.start();
  H2D::Phased lPhased(aData,
                      (H2D::Phased::kind_t) aCb.kind(),
                      // aCb.nx(), aCb.ny(),
                      lNx, lNy,
                      aCb.rx(), aCb.ry(),
                      aCb.phi(), aCb.q(), aCb.theta(), aCb.trace());
  lMeasure.stop();

  if(aCb.print()) {
    lPhased.print(std::cout);
  }

  double lConstructionTime = lMeasure.mCpuTime();

  H2D::summaryline_t lSummaryLine;
  lSummaryLine._card = aData.total();
  lSummaryLine._noDv = aData.size();
  lSummaryLine._nout = lPhased.noOutlier();
  lSummaryLine._budget = aCb.budget();
  lSummaryLine._size = lPhased.size();
  lSummaryLine._phi  = lPhased.phi();
  lSummaryLine._theta = aCb.theta();
  lSummaryLine._constructionTime = lConstructionTime;
  lSummaryLine._kind = H2D::H2D_PHASED;
  lSummaryLine._subkind = aCb.kind();
  lSummaryLine._filename = std::string(H2D::gbasename(aCb.filename().c_str()));
  lSummaryLine._param._eqdh._nx = lPhased.nx();
  lSummaryLine._param._eqdh._ny = lPhased.ny();
  lSummaryLine._param._eqdh._rx = aCb.rx();
  lSummaryLine._param._eqdh._ry = aCb.ry();

  std::cout << "hist kind: " << h2d_kind_name(lSummaryLine._kind) << std::endl;

  fAnalyzeEstimator(aData, lPhased, aCb, lSummaryLine);

  std::cout << lSummaryLine << std::endl;
}




void
fProcessDct(const H2D::Data2dim& aData,
            const H2D::Cb&       aCb) {
  std::cout << "Discrete Cosine Transform on Frequency Matrix" << std::endl;
  std::cout << "TRACE: " << aCb.trace() << ", "
            << "PRINT: " << aCb.print() << std::endl;

  uint lNoCoeff = aCb.noCoeff();
  if((0 == aCb.noCoeff()) && (0 < aCb.budget())) {
    lNoCoeff = (aCb.budget() / 6); // 1B row idx, 1B col idx, 4B coeff
    if(0 == lNoCoeff) {
      lNoCoeff = 1;
    }
  }


  Measure lMeasure;
  lMeasure.start();
  H2D::DiscreteCosineTransform lDiscreteCosineTransform(aData, 
                                                        aCb.nx(), 
                                                        aCb.ny(), 
                                                        aCb.phi(), 
                                                        lNoCoeff,
                                                        aCb.zeta(),
                                                        aCb.q(), 
                                                        aCb.theta(), 
                                                        aCb.materialize(),
                                                        aCb.trace());
  lMeasure.stop();

  if(aCb.print()) {
    lDiscreteCosineTransform.print(std::cout);
  }

  double lConstructionTime = lMeasure.mCpuTime();

  H2D::summaryline_t lSummaryLine;
  lSummaryLine._card = aData.total();
  lSummaryLine._noDv = aData.size();
  lSummaryLine._nout = lDiscreteCosineTransform.noOutlier();
  lSummaryLine._budget = aCb.budget();
  lSummaryLine._size = lDiscreteCosineTransform.size();
  lSummaryLine._phi  = lDiscreteCosineTransform.phi();
  lSummaryLine._theta = aCb.theta();
  lSummaryLine._constructionTime = lConstructionTime;
  lSummaryLine._kind = H2D::H2D_DiscreteCosineTransform;
  lSummaryLine._subkind = 0;
  lSummaryLine._filename = std::string(H2D::gbasename(aCb.filename().c_str()));
  lSummaryLine._param._dct._nx = lDiscreteCosineTransform.nx();
  lSummaryLine._param._dct._ny = lDiscreteCosineTransform.ny();
  lSummaryLine._param._dct._noCoeff = lDiscreteCosineTransform.noCoeff();
  lSummaryLine._param._dct._zeta = aCb.zeta();

  std::cout << "hist kind: " << h2d_kind_name(lSummaryLine._kind) << std::endl;
  std::cout << "Construction time: " << lConstructionTime << std::endl;
  std::cout << "    #coefficients: " << lDiscreteCosineTransform.noCoeff() << std::endl;
  const bool lShort = aCb.shortErrorCheck();
  if(lShort) {
    fAnalyzeEstimator(aData, lDiscreteCosineTransform, aCb, lSummaryLine, true, true);
  } else {
    fAnalyzeEstimator(aData, lDiscreteCosineTransform, aCb, lSummaryLine);
  }

  std::cout << lSummaryLine << std::endl;

}


void
fProcessDctCf(const H2D::Data2dim& aData,
              const H2D::Cb&       aCb) {
  std::cout << "Discrete Cosine Transform on Cumulated Frequency Matrix" << std::endl;
  std::cout << "TRACE: " << aCb.trace() << ", "
            << "PRINT: " << aCb.print() << std::endl;

  uint lNoCoeff = aCb.noCoeff();
  if((0 == aCb.noCoeff()) && (0 < aCb.budget())) {
    lNoCoeff = (aCb.budget() / 6); // 1B row idx, 1B col idx, 4B coeff
    if(0 == lNoCoeff) {
      lNoCoeff = 1;
    }
  }

  Measure lMeasure;
  lMeasure.start();
  H2D::DiscreteCosineTransformOnCumFreq lDctCf(aData, 
                                               aCb.nx(), 
                                               aCb.ny(), 
                                               aCb.phi(),
                                               lNoCoeff,
                                               aCb.zeta(),
                                               aCb.q(), 
                                               aCb.theta(), 
                                               aCb.materialize(),
                                               aCb.trace());
  lMeasure.stop();

  if(aCb.print()) {
    lDctCf.print(std::cout);
  }

  double lConstructionTime = lMeasure.mCpuTime();

  H2D::summaryline_t lSummaryLine;
  lSummaryLine._card = aData.total();
  lSummaryLine._noDv = aData.size();
  lSummaryLine._nout = lDctCf.noOutlier();
  lSummaryLine._budget = aCb.budget();
  lSummaryLine._size = lDctCf.size();
  lSummaryLine._phi  = lDctCf.phi();
  lSummaryLine._theta = aCb.theta();
  lSummaryLine._constructionTime = lConstructionTime;
  lSummaryLine._kind = H2D::H2D_DiscreteCosineTransformOnCumFreq;
  lSummaryLine._subkind = 0;
  lSummaryLine._filename = std::string(H2D::gbasename(aCb.filename().c_str()));
  lSummaryLine._param._dct._nx = lDctCf.nx();
  lSummaryLine._param._dct._ny = lDctCf.ny();
  lSummaryLine._param._dct._noCoeff = lDctCf.noCoeff();
  lSummaryLine._param._dct._zeta = aCb.zeta();

  std::cout << "hist kind: " << h2d_kind_name(lSummaryLine._kind) << std::endl;
  std::cout << "Construction time: " << lConstructionTime << std::endl;
  std::cout << "    #coefficients: " << lDctCf.noCoeff() << std::endl;
  const bool lShort = aCb.shortErrorCheck();
  if(lShort) {
    fAnalyzeEstimator(aData, lDctCf, aCb, lSummaryLine, true, true);
  } else {
    fAnalyzeEstimator(aData, lDctCf, aCb, lSummaryLine);
  }

  std::cout << lSummaryLine << std::endl;

}

void
fProcessWaveletNonStd(const H2D::Data2dim& aData,
                      const H2D::Cb&       aCb) {

  uint lNoCoeff = aCb.noCoeff();
  if((0 == aCb.noCoeff()) && (0 < aCb.budget())) {
    lNoCoeff = (aCb.budget() / 6); // 1B row idx, 1B col idx, 4B coeff
    if(0 == lNoCoeff) {
      lNoCoeff = 1;
    }
  }

  Measure lMeasure;
  lMeasure.start();
  H2D::WaveletNonStdEstimator lWest(aData,
                                    aCb.nx(),
                                    lNoCoeff,
                                    aCb.kind(),
                                    aCb.alpha(),
                                    aCb.phi(),
                                    aCb.q(),
                                    aCb.theta(),
                                    aCb.materialize(),
                                    aCb.trace());
  lMeasure.stop();

  std::cout << "WvltNonStd: " << std::endl;
  std::cout << "      kind: " << lWest.kind() << std::endl;
  std::cout << "      size: " << lWest.size() << std::endl;
  std::cout << "  #outlier: " << lWest.noOutlier() << std::endl;
  std::cout << "   #coeffs: " << lWest.coeffs().size() << std::endl;
  std::cout << "   #points: " << aData.size() << std::endl;
  std::cout << "  max freq: " << aData.maxFrequency() << std::endl;

  if(aCb.print()) {
    lWest.print(std::cout);
  }

  double lConstructionTime = lMeasure.mCpuTime();

  H2D::summaryline_t lSummaryLine;
  lSummaryLine._card = aData.total();
  lSummaryLine._noDv = aData.size();
  lSummaryLine._nout = lWest.noOutlier();
  lSummaryLine._budget = aCb.budget();
  lSummaryLine._size = lWest.size();
  lSummaryLine._phi  = lWest.phi();
  lSummaryLine._theta = aCb.theta();
  lSummaryLine._constructionTime = lConstructionTime;
  lSummaryLine._kind = H2D::H2D_WAVELET_NONSTD;
  lSummaryLine._subkind = lWest.kind();
  lSummaryLine._filename = std::string(H2D::gbasename(aCb.filename().c_str()));
  lSummaryLine._param._waveletnonstd._n       = lWest.n();
  lSummaryLine._param._waveletnonstd._noCoeff = lWest.noCoeff();
  lSummaryLine._param._waveletnonstd._alpha   = lWest.alpha();

  std::cout << "hist kind: " << h2d_kind_name(lSummaryLine._kind) 
                             << " [" << lWest.kind() << ']'
            << std::endl;

  const bool lShort = aCb.shortErrorCheck();
  if(lShort) {
    fAnalyzeEstimator(aData, lWest, aCb, lSummaryLine, true, true);
  } else {
    fAnalyzeEstimator(aData, lWest, aCb, lSummaryLine);
  }

  std::cout << lSummaryLine << std::endl;

}

void
fProcessRegP(const H2D::Data2dim& aData,
             const H2D::Cb&       aCb) {
  uint nx = aCb.nx();
  uint ny = aCb.ny();
  if((1 >= nx) || (1 >= ny)) {
    if(0 == aCb.budget()) {
      // auto
      const uint s = aData.total();
      nx = ny = (uint) floor(sqrt((8 * s) / 100)); // 1% of data size
      const uint lLim = (8*s)/100;
      while((nx * ny) < lLim) {
        ++nx;
      }
      
      if(2 >= nx) {
        nx = ny = 2;
      }
    } else {
      // from budget
      nx = ny = (uint) mt::floort<double>(mt::sqrtt<double>(aCb.budget()));
    }
  }

  Measure lMeasure;
  lMeasure.start();
  H2D::RegPEstimator  lRegP(aData, nx, ny, aCb.eps(), aCb.phi(), aCb.q(), aCb.theta());
  lMeasure.stop();

  std::cout << "#max cum frequency in tile: " << lRegP.maxEntry() << std::endl;
  std::cout << "#    zero: " << lRegP.noZero() << std::endl;
  std::cout << "#non-zero: " << lRegP.noNonZero() << std::endl;

  if(aCb.print()) {
    lRegP.print(std::cout);
  }
  const double lConstructionTime = lMeasure.mCpuTime();

  H2D::summaryline_t lSummaryLine;
  lSummaryLine._card = aData.total();
  lSummaryLine._noDv = aData.size();
  lSummaryLine._nout = lRegP.noOutlier();
  lSummaryLine._budget = aCb.budget();
  lSummaryLine._size = lRegP.size();
  lSummaryLine._phi  = aCb.phi();
  lSummaryLine._theta = aCb.theta();
  lSummaryLine._constructionTime = lConstructionTime;
  lSummaryLine._kind = H2D::H2D_RegPart;
  lSummaryLine._subkind = 0;
  lSummaryLine._filename = std::string(H2D::gbasename(aCb.filename().c_str()));
  lSummaryLine._param._regp._nx = nx;
  lSummaryLine._param._regp._ny = ny;
  lSummaryLine._param._regp._maxfreq = lRegP.maxEntry();

  fAnalyzeEstimator(aData, lRegP, aCb, lSummaryLine);

  std::cout << lSummaryLine << std::endl;
}

void
fProcessHft(const H2D::Data2dim& aData,
            const H2D::Cb&       aCb) {
  Measure lMeasure;
  lMeasure.start();
  H2D::HFTEstimator lHft(aData, aCb.logHi(), aCb.logLo(), aCb.kappa(), aCb.phi(), aCb.q(), aCb.theta(), aCb.trace());
  lMeasure.stop();

  if(aCb.print()) {
    std::cout << "fProcessHft: lHft: " << std::endl;
    lHft.print(std::cout);
    std::cout << std::endl;
  }
  const double lConstructionTime = lMeasure.mCpuTime();

  H2D::summaryline_t lSummaryLine;
  lSummaryLine._card = aData.total();
  lSummaryLine._noDv = aData.size();
  lSummaryLine._nout = lHft.noOutlier();
  lSummaryLine._budget = aCb.budget();
  lSummaryLine._size = lHft.size();
  lSummaryLine._phi  = aCb.phi();
  lSummaryLine._theta = aCb.theta();
  lSummaryLine._constructionTime = lConstructionTime;
  lSummaryLine._kind = H2D::H2D_HFT;
  lSummaryLine._subkind = 0;
  lSummaryLine._filename = std::string(H2D::gbasename(aCb.filename().c_str()));
  lSummaryLine._param._hft._logHi = aCb.logHi();
  lSummaryLine._param._hft._logLo = aCb.logLo();
  lSummaryLine._param._hft._kappa = aCb.kappa();

  fAnalyzeEstimator(aData, lHft, aCb, lSummaryLine);

  std::cout << lSummaryLine << std::endl;
}


void
fProcessHiRed(const H2D::Data2dim& aData,
              const H2D::Cb&       aCb) {
  Measure lMeasure;
  lMeasure.start();
  H2D::HiRed lHiRed(aData, aCb.kind(), aCb.eps(), aCb.maxLevel(), aCb.phi(), aCb.q(), aCb.theta(), aCb.trace());
  lMeasure.stop();

  if(aCb.print()) {
    std::cout << "fProcessHiRed: lHiRed: " << std::endl;
    lHiRed.print(std::cout);
    std::cout << std::endl;
  }
  const double lConstructionTime = lMeasure.mCpuTime();

  H2D::summaryline_t lSummaryLine;
  lSummaryLine._card = aData.total();
  lSummaryLine._noDv = aData.size();
  lSummaryLine._nout = lHiRed.noOutlier();
  lSummaryLine._budget = aCb.budget();
  lSummaryLine._size = lHiRed.size();
  lSummaryLine._phi  = aCb.phi();
  lSummaryLine._theta = aCb.theta();
  lSummaryLine._constructionTime = lConstructionTime;
  lSummaryLine._kind = H2D::H2D_HIRED;
  lSummaryLine._subkind = 0;
  lSummaryLine._filename = std::string(H2D::gbasename(aCb.filename().c_str()));
  lSummaryLine._param._hired._eps = aCb.eps();
  lSummaryLine._param._hired._maxLevel = aCb.maxLevel();
  lSummaryLine._param._hired._noNodes = lHiRed.noNodes();
  lSummaryLine._param._hired._depth = lHiRed.depth();

  fAnalyzeEstimator(aData, lHiRed, aCb, lSummaryLine);

  std::cout << lSummaryLine << std::endl;
}



void
fProcessMHist(const H2D::Data2dim& aData,
              const H2D::Cb&       aCb) {

  uint lNoBuckets = aCb.noBuckets();
  if((0 < aCb.budget()) && (0 == aCb.noBuckets())) {
    // size():
    const uint lBucketSize = 4 * 4 + 4;
    // return buckets().size() * lBucketSize;
    lNoBuckets = aCb.budget() / lBucketSize;
  }

  Measure lMeasure;
  lMeasure.start();
  H2D::MHist lMHist(aData, (H2D::MHist::kind_t) aCb.kind(), 
                    // aCb.noBuckets(), 
                    lNoBuckets,
                    aCb.phi(), aCb.q(), aCb.theta(), aCb.trace());
  lMeasure.stop();

  if(aCb.print()) {
    std::cout << "fProcessMHist: lMHist: " << std::endl;
    lMHist.print(std::cout);
    std::cout << std::endl;
  }
  const double lConstructionTime = lMeasure.mCpuTime();

  H2D::summaryline_t lSummaryLine;
  lSummaryLine._card = aData.total();
  lSummaryLine._noDv = aData.size();
  lSummaryLine._nout = lMHist.noOutlier();
  lSummaryLine._budget = aCb.budget();
  lSummaryLine._size = lMHist.size();
  lSummaryLine._phi  = aCb.phi();
  lSummaryLine._theta = aCb.theta();
  lSummaryLine._constructionTime = lConstructionTime;
  lSummaryLine._kind = H2D::H2D_MHIST;
  lSummaryLine._subkind = aCb.kind();
  lSummaryLine._filename = std::string(H2D::gbasename(aCb.filename().c_str()));
  lSummaryLine._param._hired._eps = aCb.eps();
  lSummaryLine._param._hired._maxLevel = aCb.noBuckets();

  fAnalyzeEstimator(aData, lMHist, aCb, lSummaryLine);

  std::cout << lSummaryLine << std::endl;
}

void
fProcessMHist2(const H2D::Data2dim& aData,
               const H2D::Cb&       aCb) {

  uint lNoBuckets = aCb.noBuckets();
  if((0 < aCb.budget()) && (0 == aCb.noBuckets())) {
    // size():
    const uint lBucketSize = H2D::MHist2Bucket::size(); 
    lNoBuckets = aCb.budget() / lBucketSize;
  }

  H2D::MHist2::kind_t lKind = H2D::MHist2::k_invalid;

  switch(aCb.kind()) {
    case 1:  lKind = H2D::MHist2::k_maxDiffSpread;
             break;
    case 2:  lKind = H2D::MHist2::k_maxSpread;
             break;
    case 3:  lKind = H2D::MHist2::k_maxDiffCount;
             break;
    case 4:  lKind = H2D::MHist2::k_maxDiffArea;
             break;
    case 5:  lKind = H2D::MHist2::k_minVariance;
             break;
    default: assert(0 == 1);
  }

  Measure lMeasure;
  lMeasure.start();
  H2D::MHist2 lMHist(lNoBuckets, lKind, aData); 
  lMeasure.stop();

  // if(aCb.print()) {
  //   std::cout << "fProcessMHist2: lMHist2: " << std::endl;
  //   lMHist.print(std::cout);
  //   std::cout << std::endl;
  // }

  const double lConstructionTime = lMeasure.mCpuTime();

  H2D::summaryline_t lSummaryLine;
  lSummaryLine._card = aData.total();
  lSummaryLine._noDv = aData.size();
  lSummaryLine._nout = 0; // lMHist.noOutlier();
  lSummaryLine._budget = aCb.budget();
  lSummaryLine._size = lMHist.size();
  lSummaryLine._phi  = aCb.phi();
  lSummaryLine._theta = aCb.theta();
  lSummaryLine._constructionTime = lConstructionTime;
  lSummaryLine._kind = H2D::H2D_MHIST2;
  lSummaryLine._subkind = aCb.kind();
  lSummaryLine._filename = std::string(H2D::gbasename(aCb.filename().c_str()));
  lSummaryLine._param._hired._eps = aCb.eps();
  lSummaryLine._param._hired._maxLevel = aCb.noBuckets();

  fAnalyzeEstimator(aData, lMHist, aCb, lSummaryLine);

  std::cout << lSummaryLine << std::endl;
}




void
fProcessSample(const H2D::Data2dim& aData,
               const H2D::Cb&       aCb) {
  uint lSampleSize = aCb.sampleSize();


  if((0 == lSampleSize) && (0 < aCb.budget())) {
    lSampleSize = (aCb.budget() / (4+4));
  }

  std::cout << "Sample size: " << lSampleSize << std::endl;

  Measure lMeasure;
  lMeasure.start();
  H2D::Sample2dim lSample(aData, lSampleSize, aCb.q(), aCb.theta(),
                          aCb.trace());
  lMeasure.stop();

  const double lConstructionTime = lMeasure.mCpuTime();

  H2D::summaryline_t lSummaryLine;
  lSummaryLine._card = aData.total();
  lSummaryLine._noDv = aData.size();
  lSummaryLine._nout = lSample.noOutlier();
  lSummaryLine._budget = aCb.budget();
  lSummaryLine._size = lSample.size();
  lSummaryLine._phi  = aCb.phi();
  lSummaryLine._theta = aCb.theta();
  lSummaryLine._constructionTime = lConstructionTime;
  lSummaryLine._kind = H2D::H2D_Sampling;
  lSummaryLine._subkind = 0;
  lSummaryLine._filename = std::string(H2D::gbasename(aCb.filename().c_str()));
  lSummaryLine._param._sampling._noSamples = lSampleSize;

  fAnalyzeEstimator(aData, lSample, aCb, lSummaryLine);

  std::cout << lSummaryLine << std::endl;

}



void
fProcessFixedNxNy(const H2D::Data2dim& aData, const H2D::GridTree::kind_t aKind, const H2D::Cb& aCb) {

  std::cout << "name of file: " << aCb.filename() << std::endl;
  std::cout << "is hist file: " << aCb.isHistFile() << std::endl;
  std::cout << "delta: " << aCb.delta() << ", kappa: " << aCb.kappa() << std::endl;

  // std::cout << "total: " << lRPD.total() << std::endl;
  // std::cout << "first: " << aData[0] << std::endl;
  // std::cout << "last : " << aData.last() << std::endl;

  H2D::RegularPartitioning2dim lRPD(aCb.nx(), aCb.ny(), aData);

  std::cout << "card : " << aData.size() << " (=!= " << lRPD.total() << ")" << std::endl;
  std::cout << "partitioning: regular: " << aCb.nx() << 'x' << aCb.ny() << std::endl;

  lRPD.printAnalysis(std::cout, aCb.delta(), aCb.fieldWidth());

  // std::cout << "Simple Tree fixed nx,ny: " << std::endl;
  H2D::GridTree lGridTreeFixed;
  H2D::GridTree::param_t lGTParam;
  Measure lMeasureConstructionTime;
  lMeasureConstructionTime.start();
  lGridTreeFixed.init(aData, aCb.nx(), aCb.ny(), aCb.delta(), aCb.kappa(), aCb.phi(),
                      aKind, lGTParam, aCb.q(), aCb.theta());
  lMeasureConstructionTime.stop();
  const double lConstructionTime = lMeasureConstructionTime.mCpuTime();
  std::cout << "construction time for GridTree of kind " << H2D::GridTree::name(aKind) << ": " 
            << lConstructionTime
            << std::endl;
  std::cout << "number of outliers: " << lGridTreeFixed.outlier().size() << std::endl;
  if(!lGridTreeFixed.ok()) {
    std::cout << "#GridTree not ok!" << std::endl;
    std::cerr << "#GridTree not ok!" << std::endl;
    return;
  }

  // lGridTreeFixed.print(std::cout, 3);
  // std::cout << "size: " << lGridTreeFixed.size() << std::endl;
  // std::cout << "Estimate2: " << lGridTreeFixed.estimate(lQueryRectangle) << std::endl;

  // char lKindChar = '0' + (char) aKind;
  // if(aKind > 9) {
  //   lKindChar = 'A' + (char) (aKind - 10);
  // }

  H2D::summaryline_t lSummaryLine;
  lSummaryLine._card = aData.total();
  lSummaryLine._noDv = aData.size();
  lSummaryLine._nout = lGridTreeFixed.noOutlier();
  lSummaryLine._budget = aCb.budget();
  lSummaryLine._size = lGridTreeFixed.size();
  lSummaryLine._phi  = aCb.phi();
  lSummaryLine._theta = aCb.theta();
  lSummaryLine._constructionTime = lConstructionTime;
  lSummaryLine._kind = H2D::H2D_GridTreeFixed;
  lSummaryLine._subkind = aKind;
  lSummaryLine._filename = std::string(H2D::gbasename(aCb.filename().c_str()));
  lSummaryLine._param._gt._nx = lGridTreeFixed.nx();
  lSummaryLine._param._gt._ny = lGridTreeFixed.ny();
  lSummaryLine._param._gt._delta = aCb.delta();
  lSummaryLine._param._gt._kappa = aCb.kappa();
  lSummaryLine._param._gt._noZ   = lGridTreeFixed.noClassZ();
  lSummaryLine._param._gt._noS   = lGridTreeFixed.noClassS();
  lSummaryLine._param._gt._noM   = lGridTreeFixed.noClassM();
  lSummaryLine._param._gt._noL   = lGridTreeFixed.noClassL();
  lSummaryLine._param._gt._depth = lGridTreeFixed.depth();
  lSummaryLine._param._gt._noNodes = lGridTreeFixed.noNodes();

  fAnalyzeEstimator(aData, lGridTreeFixed, aCb, lSummaryLine);

  std::cout << lSummaryLine << std::endl;

  if(aCb.printDot()) {
    std::cout << "=== dot ===" << std::endl;
    lGridTreeFixed.printDot(std::cout, 4);
  }

}


void
fGenerateGridTreeKindSpecific(const H2D::Data2dim& aData, const H2D::Cb& aCb) {
  H2D::GridTree::param_t lParam;
  H2D::GridTree::kind_t  lKind = H2D::GridTree::k_fixed2x2;
  switch(aCb.kind()) {
    case  0: lKind = H2D::GridTree::k_fixed2x2;
             break;
    case  1: lKind = H2D::GridTree::k_fixed3x3;
             break;
    case  2: lKind = H2D::GridTree::k_fixed4x4;
             break;
    case  3: lKind = H2D::GridTree::k_fixed5x5;
             break;
    case  4: lKind = H2D::GridTree::k_fixed7x7;
             break;
    case  5: lKind = H2D::GridTree::k_autosize;
             break;
    case  6: lKind = H2D::GridTree::k_maxsize;
             break;
    case  7: lKind = H2D::GridTree::k_maxdistortion0;
             break;
    case  8: lKind = H2D::GridTree::k_maxdistortionS;
             break;
    case  9: lKind = H2D::GridTree::k_mindistortionS;
             break;
    case 10: lKind = H2D::GridTree::k_minqerr0;
             break;
    case 11: lKind = H2D::GridTree::k_minqerr1;
    default: break;
  }
  Measure lMeasureConstructionTime;

  lMeasureConstructionTime.start();
  H2D::GridTree lGridTree(aData, aCb.delta(), aCb.kappa(), aCb.phi(), lKind, lParam, aCb.q(), aCb.theta());
  lMeasureConstructionTime.stop();

  const double lConstructionTime = lMeasureConstructionTime.mCpuTime();

  std::cout << "construction time for GridTree of kind " << H2D::GridTree::name(lKind) << ": " 
            << lConstructionTime
            << std::endl;

  std::cout << "number of outliers: " << lGridTree.outlier().size() << std::endl;

  H2D::summaryline_t lSummaryLine;
  lSummaryLine._card = aData.total();
  lSummaryLine._noDv = aData.size();
  lSummaryLine._nout = lGridTree.noOutlier();
  lSummaryLine._budget = aCb.budget();
  lSummaryLine._size = lGridTree.size();
  lSummaryLine._phi  = aCb.phi();
  lSummaryLine._theta = aCb.theta();
  lSummaryLine._constructionTime = lConstructionTime;
  lSummaryLine._kind = H2D::H2D_GridTreeAuto;
  lSummaryLine._subkind = lKind;
  lSummaryLine._filename = std::string(H2D::gbasename(aCb.filename().c_str()));
  lSummaryLine._param._gt._nx = lGridTree.nx();
  lSummaryLine._param._gt._ny = lGridTree.ny();
  lSummaryLine._param._gt._delta = aCb.delta();
  lSummaryLine._param._gt._kappa = aCb.kappa();
  lSummaryLine._param._gt._noZ   = lGridTree.noClassZ();
  lSummaryLine._param._gt._noS   = lGridTree.noClassS();
  lSummaryLine._param._gt._noM   = lGridTree.noClassM();
  lSummaryLine._param._gt._noL   = lGridTree.noClassL();
  lSummaryLine._param._gt._depth = lGridTree.depth();

  fAnalyzeEstimator(aData, lGridTree, aCb, lSummaryLine);
  std::cout << lSummaryLine << std::endl;
}


void
fGenerateBestGridTree(const H2D::Data2dim& aData, const H2D::Cb& aCb) {
  H2D::GridTree::param_t lParam;
  H2D::GridTree::kind_t  lKind = H2D::GridTree::k_fixed2x2;
  switch(aCb.kind()) {
    case  0: lKind = H2D::GridTree::k_fixed2x2;
             break;
    case  1: lKind = H2D::GridTree::k_fixed3x3;
             break;
    case  2: lKind = H2D::GridTree::k_fixed4x4;
             break;
    case  3: lKind = H2D::GridTree::k_fixed5x5;
             break;
    case  4: lKind = H2D::GridTree::k_fixed7x7;
             break;
    case  5: lKind = H2D::GridTree::k_autosize;
             break;
    case  6: lKind = H2D::GridTree::k_maxsize;
             break;
    case  7: lKind = H2D::GridTree::k_maxdistortion0;
             break;
    case  8: lKind = H2D::GridTree::k_maxdistortionS;
             break;
    case  9: lKind = H2D::GridTree::k_mindistortionS;
             break;
    case 10: lKind = H2D::GridTree::k_minqerr0;
             break;
    case 11: lKind = H2D::GridTree::k_minqerr1;
    default: break;
  }
  Measure lMeasureConstructionTime;

  lMeasureConstructionTime.start();

  uint lBestNx = 0;
  uint lBestNy = 0;
  double lBestQError = std::numeric_limits<double>::max();
  const uint lTotal = aData.total();
  const uint lTheta = lTotal / 20; // theta = 5% of total

  for(uint lNx = 2; lNx < 8; ++lNx) {
    for(uint lNy = 2; lNy < 8; ++lNy) {
       std::cout << "test " << lNx << 'x' << lNy << std::endl;
       H2D::GridTree lGridTree;
       lGridTree.init(aData, lNx, lNy, aCb.delta(), aCb.kappa(), aCb.phi(),
                      lKind, lParam, aCb.q(), aCb.theta());
       H2D::EstimateEvaluator lEstimateEvaluator;
       lEstimateEvaluator.push_back(lTheta);
       fCheckError(aData, lGridTree, lEstimateEvaluator, aCb, true);
       const double lQError = lEstimateEvaluator.aggregate(0).max();
       std::cout << "best::trial: " << lNx << 'x' << lNy << " : qerror: " << lQError << std::endl;
       if(lQError < lBestQError) {
         lBestQError = lQError;
         lBestNx = lNx;
         lBestNy = lNy;
       }
    }
  }

  std::cout << "best: " << lBestNx << 'x' << lBestNy << " q-error: " << lBestQError << std::endl;

  H2D::GridTree lGridTree;
  lGridTree.init(aData, lBestNx, lBestNy, aCb.delta(), aCb.kappa(), aCb.phi(),
                 lKind, lParam, aCb.q(), aCb.theta());

  lMeasureConstructionTime.stop();

  const double lConstructionTime = lMeasureConstructionTime.mCpuTime();

  std::cout << "construction time for GridTree of kind " << H2D::GridTree::name(lKind) << ": " 
            << lConstructionTime
            << std::endl;

  std::cout << "number of outliers: " << lGridTree.outlier().size() << std::endl;

  H2D::summaryline_t lSummaryLine;
  lSummaryLine._card = aData.total();
  lSummaryLine._noDv = aData.size();
  lSummaryLine._nout = lGridTree.noOutlier();
  lSummaryLine._budget = aCb.budget();
  lSummaryLine._size = lGridTree.size();
  lSummaryLine._phi  = aCb.phi();
  lSummaryLine._theta = aCb.theta();
  lSummaryLine._constructionTime = lConstructionTime;
  lSummaryLine._kind = H2D::H2D_GridTreeBest;
  lSummaryLine._subkind = lKind;
  lSummaryLine._filename = std::string(H2D::gbasename(aCb.filename().c_str()));
  lSummaryLine._param._gt._nx = lGridTree.nx();
  lSummaryLine._param._gt._ny = lGridTree.ny();
  lSummaryLine._param._gt._delta = aCb.delta();
  lSummaryLine._param._gt._kappa = aCb.kappa();
  lSummaryLine._param._gt._noZ   = lGridTree.noClassZ();
  lSummaryLine._param._gt._noS   = lGridTree.noClassS();
  lSummaryLine._param._gt._noM   = lGridTree.noClassM();
  lSummaryLine._param._gt._noL   = lGridTree.noClassL();
  lSummaryLine._param._gt._depth = lGridTree.depth();

  fAnalyzeEstimator(aData, lGridTree, aCb, lSummaryLine);
  std::cout << lSummaryLine << std::endl;

}



int
main(const int argc, const char* argv[]) {
  H2D::Cb lCb;
  argdesc_vt lArgDesc;
  construct_arg_desc(lArgDesc);

  if(!parse_args<H2D::Cb>(1, argc, argv, lArgDesc, lCb)) {
    std::cerr << "error while parsing arguments." << std::endl;
    return -1;
  }
  if(lCb.help()) {
    print_usage(std::cout, argv[0], lArgDesc);
    return 0;
  }
  if(0 == lCb.filename().size()) {
    std::cerr << "no file name given." << std::endl;
    return -1;
  }

  H2D::Data2dim lData;

  if(lCb.isHistFile()) {
    lData.readHistFile(lCb.filename());
  } else {
    lData.readValueFile(lCb.filename());
  }


  // if(lData.maxFrequency() < lCb.phi()) {
  //   std::cout << "NOTE: max frequency < phi" << std::endl;
  //   std::cerr << "NOTE: max frequency < phi" << std::endl;
  // }

  const double lTotal = (double) lData.total();

  if(100.0 > lTotal) {
    std::cerr << "data file too small: total = " << lTotal << std::endl;
    return -1;
  }

  if(1 >= lCb.theta()) {
    lCb.theta(0.01 * lTotal);
  }

  if(0 < lCb.query().size()) {
    fRunQuery(lData, lCb);
    return 0;
  }

  if(lCb.estConst()) {
    fProcessEstConst(lData, lCb);
  } else
  if(lCb.estArea()) {
    fProcessEstArea(lData, lCb);
  } else
  if(lCb.regp()) {
    fProcessRegP(lData, lCb);
  } else
  if(lCb.eqd()) {
    fProcessEquiDepthHist(lData, lCb);
  } else
  if(lCb.eqd2()) {
    fProcessEquiDepthHist2(lData, lCb);
  } else
  if(lCb.dct()) {
    fProcessDct(lData, lCb);
  } else
  if(lCb.dctcf()) {
    fProcessDctCf(lData, lCb);
  } else
  if(lCb.nono()) {
    fProcessNonoTree(lData, lCb);
  } else
  if(lCb.genhist()) {
    fProcessGenHist(lData, lCb);
  } else
  if(lCb.waveletNonStd()) {
    fProcessWaveletNonStd(lData, lCb);
  } else
  if(lCb.hft()) {
    fProcessHft(lData,lCb);
  } else
  if(lCb.phased()) {
    fProcessPhased(lData, lCb);
  } else
  if(lCb.hired()) {
    fProcessHiRed(lData, lCb);
  } else
  if(lCb.mhist()) {
    fProcessMHist(lData, lCb);
  } else
  if(lCb.mhist2()) {
    fProcessMHist2(lData, lCb);
  } else
  if(lCb.qts()) {
    fProcessQTS(lData, lCb);
  } else
  if(lCb.iqts()) {
    fProcessIQTS(lData, lCb);
  } else
  if(lCb.flextree()) {
    fProcessFlexTree(lData, lCb);
  } else
  if(lCb.gxtree()) {
    fProcessGxTree(lData, lCb);
  } else
  if(lCb.gridtree()) {
    fProcessGridTree(lData, lCb);
  } else
  if(0 <= lCb.sampleSize()) {
    fProcessSample(lData, lCb);
  } else
  if(0 > lCb.kind() && lCb.allGridTrees()) {
    for(int lKind = 0; lKind < H2D::GridTree::k_nok; ++lKind) {
      for(uint nx = 1; nx <= 20; ++nx) {
        for(uint ny = 1; ny <= 20; ++ny) {
          lCb.nx(nx);
          lCb.ny(ny);
          fProcessFixedNxNy(lData, (H2D::GridTree::kind_t) lKind, lCb);
        }
      }
    }
  } else
  if(0 <= lCb.kind() && lCb.allGridTrees()) {
    for(uint nx = 2; nx <= 7; ++nx) {
      for(uint ny = 2; ny <= 7; ++ny) {
        lCb.nx(nx);
        lCb.ny(ny);
        fProcessFixedNxNy(lData, (H2D::GridTree::kind_t) lCb.kind(), lCb);
      }
    }
  } else {
    if(-1 > lCb.kind()) {
      for(uint lKind = 0; lKind < H2D::GridTree::k_nok; ++lKind) {
        lCb.kind(lKind);
        fGenerateGridTreeKindSpecific(lData, lCb);
      }
    } else {
      if(1 < lCb.nx() && 1 < lCb.ny()) {
        fProcessFixedNxNy(lData, (H2D::GridTree::kind_t) lCb.kind(), lCb);
      } else {
        if(lCb.bestGridTree()) {
          fGenerateBestGridTree(lData, lCb);
        } else {
          fGenerateGridTreeKindSpecific(lData, lCb);
        }
      }
    }
  }

  return 0;
}




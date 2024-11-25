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
#include "ZHist/ZHist.hh"
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
#include "fparamloop.hh"

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
    fParamLoopEstConst(lData, lCb);
  } else
  if(lCb.estArea()) {
    fParamLoopEstArea(lData, lCb);
  } else
  if(lCb.regp()) {
    fParamLoopRegP(lData, lCb);
  } else
  if(lCb.eqd()) {
    fParamLoopEquiDepthHist(lData, lCb);
  } else
  if(lCb.eqd2()) {
    fParamLoopEquiDepthHist2(lData, lCb);
  } else
  if(lCb.dct()) {
    fParamLoopDct(lData, lCb);
  } else
  if(lCb.dctcf()) {
    fParamLoopDctCf(lData, lCb);
  } else
  if(lCb.nono()) {
    fParamLoopNonoTree(lData, lCb);
  } else
  if(lCb.genhist()) {
    fParamLoopGenHist(lData, lCb);
  } else
  if(lCb.waveletNonStd()) {
    fParamLoopWaveletNonStd(lData, lCb);
  } else
  if(lCb.zhist()) {
    fParamLoopZHist(lData, lCb);
  } else
  if(lCb.hft()) {
    fParamLoopHft(lData,lCb);
  } else
  if(lCb.phased()) {
    fParamLoopPhased(lData, lCb);
  } else
  if(lCb.hired()) {
    fParamLoopHiRed(lData, lCb);
  } else
  if(lCb.mhist()) {
    fParamLoopMHist(lData, lCb);
  } else
  if(lCb.mhist2()) {
    fParamLoopMHist2(lData, lCb);
  } else
  if(lCb.qts()) {
    fParamLoopQTS(lData, lCb);
  } else
  if(lCb.iqts()) {
    fParamLoopIQTS(lData, lCb);
  } else
  if(lCb.flextree()) {
    fParamLoopFlexTree(lData, lCb);
  } else
  if(lCb.gxtree()) {
    fParamLoopGxTree(lData, lCb);
  } else
  if(lCb.gridtree()) {
    fParamLoopGridTree(lData, lCb);
  } else
  if(lCb.sampling()) {
    fParamLoopSample(lData, lCb);
  } else {
    std::cerr << "EE unknown histogram kind" << std::endl;
    std::cout << "EE unknown histogram kind" << std::endl;
  }

  std::cout << "##FIN" << std::endl;
  return 0;
}




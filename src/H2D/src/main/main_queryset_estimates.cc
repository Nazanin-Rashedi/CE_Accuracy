#include <iostream>
#include <iomanip>
#include <fstream>

#include <string.h>
#include <string>
#include <vector>
#include <filesystem>

#include "infra/argbase.hh"
#include "infra/aggregate.hh"
#include "infra/matrix.hh"
#include "infra/measure.hh"
#include "infra/tmath.hh"

#include "infra/types.hh"
#include "infra/cb.hh"
#include "infra/util.hh"

/*
#include "infra/data2dim.hh"
#include "infra/summaryline.hh"
#include "infra/RegularPartitioning2dim.hh"
*/

/*
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
#include "XGBoost/XGBEstimator.hh"
*/

#include "arg.hh"

#include "process_query_file.hh"

/*
 *  run estimators on an already generated query set
 *  --sds set of data sets
 *  --ds  data set out of the given SDS
 *  must exist 
 *  - <ds>.hist file with input data in form of a histogram
 *  - <ds>.qu   file with generated queries
 */



int
main(const int argc, const char* argv[]) {
  H2D::Cb lCb;
  argdesc_vt lArgDesc;
  construct_arg_desc(lArgDesc);

  if(!parse_args<H2D::Cb>(1, argc, argv, lArgDesc, lCb)) {
    std::cerr << "error while parsing arguments." << std::endl;
    return -1;
  }

  const std::string gDirIn  = lCb.inDir();
  const std::string gDirOut = lCb.outDir();

  if(lCb.help()) {
    print_usage(std::cout, argv[0], lArgDesc);
    return 0;
  }
  if(0 == lCb.sds().size()) {
    std::cerr << "no set of data sets given." << std::endl;
    return -1;
  }

  if(0 == lCb.ds().size()) {
    std::cerr << "no data set given." << std::endl;
    return -1;
  }

  if(!std::filesystem::exists(gDirIn) || !std::filesystem::is_directory(gDirIn)) {
    std::cout << "not a valid directory: '" << gDirIn << std::endl;
    return -1;
  }

  if(!std::filesystem::exists(gDirOut) || !std::filesystem::is_directory(gDirOut)) {
    std::cout << "not a valid directory: '" << gDirOut << std::endl;
    return -1;
  }

  const std::string lFilebase = gDirIn + '/' + lCb.sds()  + '/' + lCb.ds();
  const std::string lFilenameData = lFilebase + ".hist";
  const std::string lFilenameQuery = lFilebase + ".qu";

  if(!std::filesystem::exists(lFilenameData) || !std::filesystem::is_regular_file(lFilenameData)) {
    std::cout << "Not a valid file: '" << lFilenameData << "'." << std::endl;
    return -1;
  }
  if(!std::filesystem::exists(lFilenameQuery) || !std::filesystem::is_regular_file(lFilenameQuery)) {
    std::cout << "Not a valid file: '" << lFilenameQuery << "'." << std::endl;
    return -1;
  }


  std::filesystem::path lDirRes(gDirOut);
  lDirRes /= lCb.sds();
  if(!std::filesystem::exists(lDirRes) || !std::filesystem::is_directory(lDirRes)) {
    std::cout << "is not a valid directory: '" << lDirRes << "'." << std::endl;
    return -1;
  }

  H2D::ProcessQueryFile lProc(gDirIn, gDirOut, lCb);
  lProc.run(lCb);
 
//  std::cout << "# FIN" << std::endl;
  std::cout<<std::endl;
  return 0;
}




#include <iostream>
#include <fstream>
#include <iomanip>

#include "infra/bitvectorsmall.hh"
#include "infra/matrix.hh"
#include "infra/measure.hh"

#include "infra/cb.hh"
#include "infra/summaryline.hh"

#include "../CheckError/CheckErrorUtil.hh"

#include "GxTree.hh"
#include "Gxt_Node_G.hh"
#include "Gxt_Node_L.hh"
#include "Gxt_Node_M.hh"

void runGxTree(H2D::Cb& aCb);


void
test1(const H2D::GxTree::gx_kind_t aGxKind, 
      const uint                   aBudget,
      const int                    aLeafRefinement,
      const bool                   aToPrintTree,
      const bool                   aTrace) {
  std::cout << "*** TEST 1 ***" << std::endl;

  H2D::Data2dim lData;
  lData.readHistFile("/home/rashedi/src/H2D/in/longlat.hist");
  assert(0 < lData.size());

  H2D::GxTree lGxTree(lData,
                      aGxKind,
                      aBudget,
                      aLeafRefinement,
                      0.9,
                      0, // minimum node total
                      0,
                      2.0,
                      130,
                      aTrace);

  std::cout << "no nodes: " << lGxTree.noNodes() << std::endl
            << "no leafs: " << lGxTree.noLeafNodes() << std::endl
            << "no inner: " << lGxTree.noInnerNodes() << std::endl
            << "   depth: " << lGxTree.depth() << std::endl
            << "    size: " << lGxTree.size() << std::endl
            << "calcsize: " << lGxTree.calcSize() << std::endl
            << "  budget: " << lGxTree.budget() << std::endl
            << " leafRef: " << lGxTree.leafRefinement() << std::endl
            << "MinNodTot: " << lGxTree.minimumNodeTotal() << std::endl
            << "min leaf cumfreq: " << lGxTree.root()->minLeafCumFreq() << std::endl
            << "max leaf cumfreq: " << lGxTree.root()->maxLeafCumFreq() << std::endl
            << "max unsplit freq: " << lGxTree.maxUnsplit() << std::endl
            << "min   split freq: " << lGxTree.minSplit() << std::endl
            << std::endl;

  if(aToPrintTree) {
    // to print or not to print
    lGxTree.print(std::cout);
    std::ofstream lDotFile("zGxTree.dot");  
    lGxTree.printDot(lDotFile);
  }



  std::cout<< "**********************" << std::endl
           << "*** START ENCODING ***" << std::endl
           << "**********************" << std::endl;

  bool lCheckEncoding = true;
  lGxTree.trace(aTrace);
  lGxTree.encode(lCheckEncoding);


  H2D::GxTreeItp lGxTreeItp(lGxTree.outlier(), lGxTree.encoding(), true);
  std::cout << std::endl;
  std::cout << "=== first estimate from encoding ===" << std::endl;
  H2D::rectangle_t lQueryRectangle(80.0, 30.0, 120.0, 40.0);
  double lTrueVal = lData.countWithin(lQueryRectangle);
  double lEstVal  = lGxTreeItp.estimate(lQueryRectangle);
  std::cout << "query rectangle: " << lQueryRectangle << std::endl;
  std::cout << "true val: " << lTrueVal << std::endl;
  std::cout << "est  val: " << lEstVal  << std::endl;
  std::cout << "=== first estimate from encoding ===" << std::endl;
  std::cout << std::endl;
 
}


void
test2(const H2D::GxTree::gx_kind_t aGxKind, 
      const uint                   aBudget,
      const int                    aLeafRefinement,
      const bool                   aShort,
      const bool                   aToPrintTree,
      const bool                   aTrace) {
  std::cout << "*** TEST 2 ***" << std::endl;
  H2D::Cb lCb;
  lCb.filename("longlat");
  lCb.gridtree(true);
  lCb.budget(aBudget);
  lCb.kind(aGxKind);
  lCb.leafRefinement(aLeafRefinement);
  lCb.kappa(0);
  lCb.phi(0);
  // lCb.q();
  // lCb.theta();
  lCb.minimumNodeTotal(0);
  lCb.trace(aTrace);
  lCb.checkError(true);
  lCb.printBadFlag(true);
  lCb.printBadQ(100.0);
  lCb.printBadTheta(100.0);

  runGxTree(lCb);
}

void
runGxTree(H2D::Cb& aCb) {
  H2D::Data2dim lData;
  lData.readHistFile(aCb.filename());
  assert(0 < lData.size());

  const bool lCheckEncoding = true;

  Measure lMeasure;
  lMeasure.start();

  H2D::GxTree lGxtRaw(lData, (H2D::GxTree::gx_kind_t) aCb.kind(), aCb.budget(),
                      aCb.leafRefinement(), aCb.lrf(),
                      aCb.minimumNodeTotal(), 
                      aCb.phi(), aCb.q(), aCb.theta(), aCb.trace());
  lGxtRaw.encode(lCheckEncoding);
  H2D::GxTreeItp lGxtItp(lGxtRaw.outlier(), lGxtRaw.encoding(), false);

  lMeasure.stop();

  const double lConstructionTime = lMeasure.mCpuTime();

  std::cout << "Gx-Tree:" << std::endl;
  lGxtRaw.printParameters(std::cout);
  std::cout << "  budget: " << lGxtRaw.budget() << std::endl;
  std::cout << "       q: " << lGxtRaw.q() << std::endl;
  std::cout << "   theta: " << lGxtRaw.theta() << std::endl;
  std::cout << "    kind: " << lGxtRaw.kind() << std::endl;
  std::cout << "   depth: " << lGxtRaw.depth() << std::endl;
  std::cout << "   #node: " << lGxtRaw.noNodes() << std::endl;
  std::cout << " minleaf: " << lGxtRaw.minLeafCumFreq() << std::endl;
  std::cout << " maxleaf: " << lGxtRaw.maxLeafCumFreq() << std::endl;
  std::cout << "    size: " << lGxtRaw.calcSize() << std::endl;
  std::cout << "#outlier: " << lGxtRaw.noOutlier() << std::endl;
  std::cout << " #points: " << lData.size() << std::endl;
  std::cout << "max freq: " << lData.maxFrequency() << std::endl;
  std::cout << "constr. time: " << lConstructionTime << std::endl;

  std::cout << "GxtItp:" << std::endl;
  lGxtItp.printNodeTypes(std::cout);


  if(aCb.print()) {
    lGxtRaw.print(std::cout);
    lGxtItp.print(std::cout);
  }

  H2D::summaryline_t lSummaryLineRaw;
  lSummaryLineRaw._card = lData.total();
  lSummaryLineRaw._noDv = lData.size();
  lSummaryLineRaw._nout = lGxtRaw.noOutlier();
  lSummaryLineRaw._budget  = lGxtRaw.budget();
  lSummaryLineRaw._size = lGxtRaw.size();
  lSummaryLineRaw._phi  = lGxtRaw.phi();
  lSummaryLineRaw._theta = aCb.theta();
  lSummaryLineRaw._constructionTime = lConstructionTime;
  lSummaryLineRaw._kind = H2D::H2D_GXTREE;
  lSummaryLineRaw._subkind = lGxtRaw.kind();
  lSummaryLineRaw._filename = std::string(H2D::gbasename(aCb.filename().c_str()));
  lSummaryLineRaw._param._gxtree._leafRefinement = aCb.leafRefinement();
  lSummaryLineRaw._param._gxtree._depth    = lGxtRaw.depth();
  lSummaryLineRaw._param._gxtree._noNodes  = lGxtRaw.noNodes();
  lSummaryLineRaw._param._gxtree._noGNodes = lGxtRaw.noGNodes();
  lSummaryLineRaw._param._gxtree._minSplit   = lGxtRaw.minSplit();
  lSummaryLineRaw._param._gxtree._maxUnsplit = lGxtRaw.maxUnsplit();
  lSummaryLineRaw._param._gxtree._encoded = 'T';

  H2D::summaryline_t lSummaryLineItp;
  lSummaryLineItp._card = lData.total();
  lSummaryLineItp._noDv = lData.size();
  lSummaryLineItp._nout = lGxtRaw.noOutlier();
  lSummaryLineItp._budget  = lGxtRaw.budget();
  lSummaryLineItp._size = lGxtRaw.size();
  lSummaryLineItp._phi  = lGxtRaw.phi();
  lSummaryLineItp._theta = aCb.theta();
  lSummaryLineItp._constructionTime = lConstructionTime;
  lSummaryLineItp._kind = H2D::H2D_GXTREE;
  lSummaryLineItp._subkind = lGxtRaw.kind();
  lSummaryLineItp._filename = std::string(H2D::gbasename(aCb.filename().c_str()));
  lSummaryLineRaw._param._gxtree._leafRefinement = aCb.leafRefinement();
  lSummaryLineItp._param._gxtree._depth    = lGxtRaw.depth();
  lSummaryLineItp._param._gxtree._noNodes  = lGxtRaw.noNodes();
  lSummaryLineItp._param._gxtree._noGNodes = lGxtRaw.noGNodes();
  lSummaryLineItp._param._gxtree._minSplit   = lGxtRaw.minSplit();
  lSummaryLineItp._param._gxtree._maxUnsplit = lGxtRaw.maxUnsplit();
  lSummaryLineItp._param._gxtree._encoded = 'E';

  lGxtRaw._gxtitp = &lGxtItp;  // at every node the estimates of Raw, Itp are compared


  std::cout << "runGxTree:  aCb.checkError() == " << (aCb.checkError() ? "true" : "false") << std::endl;

  aCb.trace(false);

  if(aCb.shortErrorCheck()) { 
    std::cout << "CHECK SHORT " << aCb.checkError() << std::endl;
    fAnalyzeEstimator(lData, lGxtRaw, aCb, lSummaryLineRaw, true, true); 
    fAnalyzeEstimator(lData, lGxtItp, aCb, lSummaryLineItp, true, true); 
  } else { 
    std::cout << "CHECK LONG " << aCb.checkError() << std::endl;
    fAnalyzeEstimator(lData, lGxtRaw, aCb, lSummaryLineRaw);
    fAnalyzeEstimator(lData, lGxtItp, aCb, lSummaryLineItp);
  }

  std::cout << "summary line for uncompressed GxTree: " << std::endl;
  std::cout << lSummaryLineRaw << std::endl;
  std::cout << "summary line for   compressed GxTree: " << std::endl;
  std::cout << lSummaryLineItp << std::endl;
  
}


void
test3(const H2D::GxTree::gx_kind_t aGxKind) {
  std::cout << "*** TEST 3 ***" << std::endl;

  const uint lBudgets[36] = {200, 300, 400, 500, 600, 700, 800, 900, 
                             1000, 1100, 1200, 1300, 1400, 1500, 1600,
                             1700, 1800, 1900, 2000, 2100, 2200, 2300,
                             2400, 2500, 2600, 2700, 2800, 2900, 3000,
                             3100, 3200, 3300, 3400, 3500, 3600, 3700};

  for(int lLeafRef = 0; lLeafRef < 4; ++lLeafRef) {
    for(uint i = 0; i < 36; ++i) {
    std::cout << "@@@ TEST 3:  "
              << "gx kind = " << aGxKind << "; "
              << "budget = "  << lBudgets[i] << "; "
              << "leaf refinement = " << lLeafRef
              << std::endl;
      test2(aGxKind, 
            lBudgets[i],
            lLeafRef,
            false, // short check of estimation (fewer queries)
            false, // print tree
            false  // trace
           );
    }
  }
}

void
test4() {
  std::cout << "*** TEST 4 ***" << std::endl;
}

void
test5() {

}

void
test8() {
  std::cout << "*** TEST 8 ***" << std::endl;
}

void
runQuery(const char* aQuery, 
         const int   aLeafRefinement,
         const H2D::Cb& aCb) {

  std::cout << "query rectangle in: " << aQuery << std::endl;

  H2D::rectangle_t lQueryRectangle;
  if(!lQueryRectangle.setTo(aQuery)) {
    std::cerr << "can't parse rectangle " << aQuery << std::endl;
    return;
  }
  
  std::cout << "producing estimates for query rectangle " << lQueryRectangle << std::endl;

  H2D::Data2dim lData;
  lData.readHistFile(aCb.filename());
  assert(0 < lData.size());

  H2D::GxTree lGxtRaw(lData, (H2D::GxTree::gx_kind_t) aCb.kind(), aCb.budget(),
                      aLeafRefinement, aCb.lrf(),
                      aCb.minimumNodeTotal(), 
                      aCb.phi(), aCb.q(), aCb.theta(), aCb.trace());
  lGxtRaw.encode(true);
  H2D::GxTreeItp lGxtItp(lGxtRaw.outlier(), lGxtRaw.encoding(), true);


  std::cout << std::endl;
  std::cout << "GxTree:" << std::endl;
  lGxtRaw.print(std::cout);
  std::cout << std::endl;
  std::cout << "GxTree Encoded:" << std::endl;
  lGxtItp.print(std::cout);
  std::cout << std::endl;

  lGxtRaw._gxtitp = &lGxtItp; // check at every node

  std::cout << "Estimate from GxTree (before encoding):" << std::endl;
  const double lEstRaw = std::max<double>(1.0, lGxtRaw.estimate(lQueryRectangle));

  std::cout << std::endl;
  std::cout << "Estimate from GxTreeItp (from encoded tree):" << std::endl;
  const double lEstItp = std::max<double>(1.0, lGxtItp.estimate(lQueryRectangle));
  const double lTruVal = std::max<double>(1.0, lData.countWithin(lQueryRectangle));

  std::cout << std::endl << "Results:" << std::endl;

  std::cout << "est gxt raw = " << lEstRaw << std::endl
            << "est gxt itp = " << lEstItp << std::endl
            << "true result = " << lTruVal << std::endl
            << "q-err raw/res = " << q::qerrorT1(lEstRaw, lTruVal) << std::endl
            << "q-err itp/res = " << q::qerrorT1(lEstItp, lTruVal) << std::endl
            << "q-err raw/itp = " << q::qerrorT1(lEstRaw, lEstItp) << std::endl
            << std::endl;

  assert(2.0 > q::qerrorT1(lEstRaw, lEstItp));
}


void
fPrintHeaderSizes() {
  std::cout << "sizeof(|G|) = " << (8*sizeof(H2D::Gxt::Node_G_Generic)) << std::endl;
  std::cout << "sizeof(|L|) = " << (8*sizeof(H2D::Gxt::Node_L_Generic)) << std::endl;
  std::cout << "sizeof(|M|) = " << (8*sizeof(H2D::Gxt::Node_M_Generic)) << std::endl;
  std::cout << "sizeof(|S|) = " << (8*sizeof(H2D::Gxt::Node_S_Generic)) << std::endl;
}

int
main(const int argc, const char* argv[]) {


  std::cout << "argc: " << argc << std::endl;

  fPrintHeaderSizes();

/*
  test1(H2D::GxTree::K_Gxxx,  500,  false);
  test1(H2D::GxTree::K_Gxxx,  800,  false);
  test1(H2D::GxTree::K_Gxxx, 1000,  false);
  test1(H2D::GxTree::K_Gxxx, 1500,  false);
  test1(H2D::GxTree::K_Gxxx, 2000,  false);
  test1(H2D::GxTree::K_Gxxx, 2500,  false);
  test1(H2D::GxTree::K_Gxxx, 3000,  false);
  test1(H2D::GxTree::K_Gxxx, 3500,  false);
*/

  const bool lCheckError     = true;
  const bool lCheckShort     = false;
  const bool lPrintTree      = true;
  const int  lLeafRefinement = 0;
  const uint lBudget         = 8000; 


  assert(224 == (sizeof(H2D::Gxt::Node_G_Generic) * 8));
  assert(128 == (sizeof(H2D::Gxt::Node_L_Generic) * 8));
  assert( 64 == (sizeof(H2D::Gxt::Node_M_Generic) * 8));
  // assert( 32 == (sizeof(H2D::Gxt::Node_S_Generic) * 8));

  if(3 == argc) {
    H2D::Cb lCb;
    lCb.filename(argv[2]);
    lCb.gridtree(true);
    lCb.kind(H2D::GxTree::K_GLMS);
    lCb.kappa(0);
    lCb.phi(0);
    // lCb.q();
    // lCb.theta();
    lCb.trace(true);
    lCb.checkError(lCheckError);
    lCb.shortErrorCheck(lCheckShort);
    lCb.print(lPrintTree);
    lCb.printBadFlag(true);
    lCb.printBadQ(100.0);
    lCb.printBadTheta(200.0);
    lCb.budget(lBudget);

    runQuery(argv[1], lLeafRefinement, lCb);
  }
  if(2 == argc) {
    H2D::Cb lCb;
    lCb.filename(argv[1]);
    lCb.gridtree(true);
    lCb.kind(H2D::GxTree::K_GLMS);
    lCb.kappa(0);
    lCb.phi(0);
    // lCb.q();
    // lCb.theta();
    lCb.trace(false);
    lCb.checkError(lCheckError);
    lCb.shortErrorCheck(lCheckShort);
    lCb.printBadFlag(true);
    lCb.printBadQ(100.0);
    lCb.printBadTheta(200.0);
    lCb.budget(lBudget);
  
    runGxTree(lCb); 

    std::cout << "FIN 2 == argc" << std::endl;

  } else {

    // test2(H2D::GxTree::K_Gxxx, lBudget,  
    //       lLeafRefinement, lShort, lPrintTree, false);
    // test2(H2D::GxTree::K_GLxx, lBudget,  
    //       lLeafRefinement, lShort, lPrintTree, false);
    // test2(H2D::GxTree::K_GLMx, lBudget,  
    //       lLeafRefinement, lShort, lPrintTree, lTrace);

    
    test3(H2D::GxTree::K_GLMS);
    test3(H2D::GxTree::K_GLMx);
    test3(H2D::GxTree::K_GLxx);
    test3(H2D::GxTree::K_Gxxx);
  }

  // test1(H2D::GxTree::K_GLxx, 500,  lLeafRefinement, lTrace);
  // test1(H2D::GxTree::K_GLMx, 500,  lLeafRefinement, lTrace);
  // test1(H2D::GxTree::K_GLMS, 500,  lLeafRefinement, lTrace);


  // test1(H2D::GxTree::K_GLxx,  lLeafRefinement, true);

  return 0;
}




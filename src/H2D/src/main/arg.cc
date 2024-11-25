#include "arg.hh"

using namespace H2D;

void
construct_arg_desc(argdesc_vt& x) {
  typedef argdesc_t<Cb, int>         iarg_t;
  typedef argdesc_t<Cb, H2D::uint>   uarg_t;
  // typedef argdesc_t<Cb, float>       farg_t;
  typedef argdesc_t<Cb, double>      darg_t;
  typedef argdesc_t<Cb, bool>        barg_t;
  typedef argdesc_t<Cb, std::string> sarg_t;

  x.push_back( new barg_t("--help", false, &Cb::help, "print this message" ) );
  x.push_back( new barg_t("--hist", false, &Cb::isHistFile, "input file is histogram (x, y, count)") );
  x.push_back( new sarg_t("--file", "", &Cb::filename, "input file for 2-dim data") );

  x.push_back(new uarg_t("--no-query", 100, &Cb::no_query, "number of queries to generate") );
  x.push_back(new sarg_t("--file-query", "", &Cb::filename_query, "file name for generated queries") );

  x.push_back(new sarg_t("--sds", "", &Cb::sds, "name of set of data sets (directory name)"));
  x.push_back(new sarg_t("--ds",  "", &Cb::ds,  "name of data set (file name without suffix .hist)"));
  x.push_back(new sarg_t("--inDir",  "", &Cb::inDir,  "Dir path that includes sds directories"));
  x.push_back(new sarg_t("--outDir",  "", &Cb::outDir,  "Dir path for output results and XGB models"));
  x.push_back(new sarg_t("--trainQDir",  "", &Cb::trainQDir,  "Dir path for .qu_t100k training queries"));
  x.push_back(new sarg_t("--testQDir",  "", &Cb::testQDir,  "Dir path for .qu_a tst queries"));

  x.push_back( new uarg_t("--dim", 0, &Cb::dim, "dim for NR partition, 0:x and 1:y") );
  x.push_back( new uarg_t("--nx", 11, &Cb::nx, "number of partitions in x-direction") );
  x.push_back( new uarg_t("--ny", 11, &Cb::ny, "number of partitions in y-direction") );
  x.push_back( new uarg_t("--rx",  5, &Cb::rx, "number of subpartitions in x-direction") );
  x.push_back( new uarg_t("--ry",  5, &Cb::ry, "number of subpartitions in y-direction") );
  x.push_back( new iarg_t("-w", 4, &Cb::fieldWidth, "field width for row vectors") );
  x.push_back( new darg_t("--dmax", 0.0, &Cb::dmax, "maximal allowed distortion for class M") );
  x.push_back( new uarg_t("--delta", 32, &Cb::delta, "delta for determining class 0,S,M,L") );
  x.push_back( new uarg_t("--kappa", 512, &Cb::kappa, "kappa for determining class M,L") );
  x.push_back( new uarg_t("--phi", 250, &Cb::phi, "phi for determining (highly) frequent values") );
  x.push_back( new uarg_t("--min-node-total", 0, &Cb::minimumNodeTotal, "minimum total of a node (Gxt)") );
  x.push_back( new darg_t("--zeta", 0.0, &Cb::zeta, "zeta") );
  x.push_back( new darg_t("--alpha", 0.0, &Cb::alpha, "alpha") );
  x.push_back( new uarg_t("--precision", 3, &Cb::precision, "precision (e.g. used for FlexTree)") );
  x.push_back( new barg_t("--with-errors", false, &Cb::withErrors, "analyze errors of regular grids") );
  x.push_back( new barg_t("--check-error", false, &Cb::checkError, "check error by running millions of queries") );
  x.push_back( new barg_t("--check-error-short", false, &Cb::shortErrorCheck, "quick error check by running thousands of queries") );
  x.push_back( new iarg_t("--kind", -1, &Cb::kind, "(sub) kind of Histogram to be constructed") );
  x.push_back( new uarg_t("--no-coeff", 0, &Cb::noCoeff, "number of coefficients, e.g., for dct") );
  x.push_back( new uarg_t("--max-level", 6, &Cb::maxLevel, "maximal level (depth) for tree histograms") );//NR was 0 changed for HiRed
  x.push_back( new uarg_t("--no-buckets", 0, &Cb::noBuckets, "number of buckets") );
  x.push_back( new uarg_t("--budget", 0, &Cb::budget, "space budget in number of bytes") );
  x.push_back( new barg_t("--best-gt",    false, &Cb::bestGridTree, "determine best grid tree by trying all pxo") );
  x.push_back( new barg_t("--all-trees", false, &Cb::allGridTrees, "generate all grid trees for all pxo") );

  x.push_back( new iarg_t("--sample", -1, &Cb::sampleSize, "do sampling with size <n>") );

  x.push_back( new barg_t("--est-const", false, &Cb::estConst, "constant estimator") );
  x.push_back( new barg_t("--est-area", false, &Cb::estArea, "area-based estimator") );
  x.push_back( new barg_t("--regp", false, &Cb::regp, "regular partitioning") );
  x.push_back( new barg_t("--hft" , false, &Cb::hft, "keep highly frequent tiles of grids with multiple resolutions") );
  x.push_back( new barg_t("--vih", false, &Cb::vih, "VI-Histogram") );
  x.push_back( new barg_t("--eqd", false, &Cb::eqd, "Equi-Depth-Histogram") );
  x.push_back( new barg_t("--eqd2", false, &Cb::eqd2, "Equi-Depth-Histogram (SLubi)") );
  x.push_back( new barg_t("--nono", false, &Cb::nono, "Nono-Tree") );
  x.push_back( new barg_t("--genhist", false, &Cb::genhist, "GenHist") );
  x.push_back( new barg_t("--wn", false, &Cb::waveletNonStd, "Wavelet (non-standard)") );
  x.push_back( new barg_t("--zhist", false, &Cb::zhist, "ZHist (Z-Curves)") );
  x.push_back( new barg_t("--phased", false, &Cb::phased, "Phased") );
  x.push_back( new barg_t("--hired", false, &Cb::hired, "HiRed") );
  x.push_back( new barg_t("--mhist", false, &Cb::mhist, "MHist") );
  x.push_back( new barg_t("--mhist2", false, &Cb::mhist2, "MHist2 (SLubi)") );
  x.push_back( new barg_t("--qts", false, &Cb::qts, "QTS (Buccafurri et al 2003)") );
  x.push_back( new barg_t("--iqts", false, &Cb::iqts, "IQTS (Buccafurri et al 2003)") );
  x.push_back( new barg_t("--fxt", false, &Cb::flextree, "FlexTree") );
  x.push_back( new barg_t("--gxt", false, &Cb::gxtree, "GxTree") );
  x.push_back( new barg_t("--sampling", false, &Cb::sampling, "Sampling") );
  x.push_back( new barg_t("--SAO", false, &Cb::sampleAlphaOmega, "SampleAlphaOmega") );
  
  x.push_back( new darg_t("--epsAlphaOmega", 0.00001, &Cb::epsAlphaOmega, "epsAlphaOmega"));
  x.push_back( new barg_t("--xgb", false, &Cb::xgb, "XGBoost") );
  x.push_back( new barg_t("--exgb", false, &Cb::exgb, "Enriched XGBoost") );
  x.push_back( new barg_t("--lwxgb", false, &Cb::lwxgb, "Enriched EBO Minsel AVI XGBoost") );
  x.push_back( new barg_t("--nxgb", false, &Cb::nxgb, "XGBoost with Sampling and 1D QTS selectivities") );
  x.push_back( new barg_t("--indep", false, &Cb::indep, "independance assupmtion with 1D QTS selectivities") );
  x.push_back( new barg_t("--nreqd", false, &Cb::nreqd, "independance assupmtion with 1D Equidepth histograms by NR") );

  x.push_back( new uarg_t("--xgb-num-trees", 500,&Cb::xgb_num_trees,"number of trees for xb"));
  x.push_back( new uarg_t("--xgb-num-train-queries", 20000,&Cb::xgb_num_train_queries,"number of queries to be taken for training xgb"));

  x.push_back( new barg_t("--gta", false, &Cb::gridtree, "GridTree") );

  x.push_back( new iarg_t("--leaf-refinement", 0, &Cb::leafRefinement, "leaf refinement level") );
  x.push_back( new darg_t("--lrf", 0.9, &Cb::lrf, "leaf refinement factor (0 < lrf <= 1)") );
  x.push_back( new iarg_t("--theta", -1, &Cb::theta, "theta") );
  x.push_back( new darg_t("--eps", 70, &Cb::eps, "epsilon") );//NR was -1 changed for HiRed
  x.push_back( new darg_t("--q", -1, &Cb::q, "q") );
  x.push_back( new barg_t("--mat", false, &Cb::materialize, "allow materialization for faster estimation") );
  x.push_back( new barg_t("--print-dot", false, &Cb::printDot, "print grid tree with dot" ) );
  x.push_back( new barg_t("--print-regp", false, &Cb::printRegP, "print regular partitioning") );
  x.push_back( new darg_t("--print-bad-q", 0.0, &Cb::printBadQ, "print bad rectangles (qmax)") );
  x.push_back( new darg_t("--print-bad-theta", 0.0, &Cb::printBadTheta, "print bad rectangles (theta)") );
  x.push_back( new barg_t("--print", false, &Cb::print, "print histogram") );
  x.push_back( new barg_t("--debug", false, &Cb::debug, "debug GxTree") );
  x.push_back( new barg_t("--trace", false, &Cb::trace, "trace histogram construction/estimation") );
  x.push_back( new sarg_t("--query", "", &Cb::query, "a single query to be run (with GxTree)") );
  x.push_back( new barg_t("--dct", false, &Cb::dct, "discrete cosine transform") );
  x.push_back( new barg_t("--dctcf", false, &Cb::dctcf, "discrete cosine transform on cumulated frequency matrix") );
  x.push_back( new darg_t("--shrinkage", 0.0, &Cb::shrinkage, "upper bound on shrinkage for optional br") );
  x.push_back( new iarg_t("--logHi", 0, &Cb::logHi, "log2(nx) = log2(ny) of finest grained grid") );
  x.push_back( new iarg_t("--logLo", 0, &Cb::logLo, "log2(nx) = log2(ny) of coarsest grained grid") );

  x.push_back( new iarg_t("--lowlim", 0, &Cb::lowLim, "files with total less than lowlim are ignored") );
}



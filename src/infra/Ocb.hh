#ifndef HIST_K_DIM_CB_HH
#define HIST_K_DIM_CB_HH

#include <string>

#include "types.hh"

namespace H2D {

class Cb {
  public:
    typedef unsigned int uint;
  public:
    Cb();
  public:

    inline const std::string& filename() const { return _filename; }
    inline void  filename(const std::string& x) { _filename = x; }

    inline bool isHistFile() const { return _isHistFile; }
    inline void isHistFile(const bool& x) { _isHistFile = x; }

    inline uint no_query() const { return _no_query; }
           void no_query(const uint&);

    inline const std::string& filename_query() const { return _filename_query; }
           void filename_query(const std::string&);

    inline const std::string& sds() const { return _sds; }
    inline const std::string& ds()  const { return _ds; }
    inline void sds(const std::string& aSDS) { _sds = aSDS; }
    inline void ds(const std::string&  aDS ) { _ds  = aDS;  }

 
    inline uint nx() const { return _nx; }
    inline uint ny() const { return _ny; }
    inline void nx(const uint& x) { _nx = x; }
    inline void ny(const uint& y) { _ny = y; }

    inline uint rx() const { return _rx; }
    inline uint ry() const { return _ry; }
    inline void rx(const uint& x) { _rx = x; }
    inline void ry(const uint& y) { _ry = y; }

    inline int  fieldWidth() const { return _fieldWidth; }
    inline void fieldWidth(const int& x) { _fieldWidth = x; }

    inline double dmax() const { return _dmax; }
    inline void   dmax(const double& x) { _dmax = x; }

    inline uint   delta() const { return _delta; }
    inline void   delta(const uint& x) { _delta = x; }

    inline uint   kappa() const { return _kappa; }
    inline void   kappa(const uint& x) { _kappa = x; }

    inline uint   phi() const { return _phi; }
    inline void   phi(const uint& x) { _phi = x; }

    inline uint   minimumNodeTotal() const { return _minimumNodeTotal; }
    inline void   minimumNodeTotal(const uint& x) { _minimumNodeTotal = x; }

    inline bool   withErrors() const { return _withErrors; }
    inline void   withErrors(const bool& x) { _withErrors = x; }

    inline bool help() const { return _help; }
    inline void help(const bool& x) { _help = x; }

    inline int  kind() const { return _kind; }
    inline void kind(const int& x) { _kind = x; }

    inline bool bestGridTree() const { return _bestGridTree; }
    inline void bestGridTree(const bool& x) { _bestGridTree = x; }

    inline bool allGridTrees() const { return _allGridTrees; }
    inline void allGridTrees(const bool& x) { _allGridTrees = x; }

    inline bool checkError() const { return _checkError; }
    inline void checkError(const bool& x) { _checkError = x; }

    inline bool shortErrorCheck() const { return _shortErrorCheck; }
    inline void shortErrorCheck(const bool& x) { _shortErrorCheck = x; }

    inline int  sampleSize() const { return _sampleSize; }
    inline void sampleSize(const int& x) { _sampleSize = x; }

    inline bool estConst() const { return _estConst; }
    inline void estConst(const bool& x) { _estConst = x; }

    inline bool estArea() const { return _estArea; }
    inline void estArea(const bool& x) { _estArea = x; }

    inline bool regp() const { return _regp; }
    inline void regp(const bool& x) { _regp = x; }

    inline bool eqd() const { return _eqd; }
    inline void eqd(const bool& x) { _eqd = x; }

    inline bool eqd2() const { return _eqd2; }
    inline void eqd2(const bool& x) { _eqd2 = x; }


    inline bool dct() const { return _dct; }
    inline void dct(const bool& x) { _dct = x; }

    inline bool dctcf() const { return _dctcf; }
    inline void dctcf(const bool& x) { _dctcf = x; }

    inline bool nono() const { return _nono; }
    inline void nono(const bool& x) { _nono = x; }

    inline bool genhist() const { return _genhist; }
    inline void genhist(const bool& x) { _genhist = x; }

    inline bool waveletNonStd() const { return _waveletNonStd; }
    inline void waveletNonStd(const bool& x) { _waveletNonStd = x; }

    inline bool zhist() const { return _zhist; }
    inline void zhist(const bool& x) { _zhist = x; }

    inline bool hft() const { return _hft; }
    inline void hft(const bool& x) { _hft = x; }

    inline bool phased() const { return _phased; }
    inline void phased(const bool& x) { _phased = x; }

    inline bool hired() const { return _hired; }
    inline void hired(const bool& x) { _hired = x; }

    inline bool mhist() const { return _mhist; }
    inline void mhist(const bool& x) { _mhist = x; }

    inline bool mhist2() const { return _mhist2; }
    inline void mhist2(const bool& x) { _mhist2 = x; }

    inline bool qts() const { return _qts; }
    inline void qts(const bool& x) { _qts = x; }

    inline bool iqts() const { return _iqts; }
    inline void iqts(const bool& x) { _iqts = x; }

    inline bool flextree() const { return _flextree; }
    inline void flextree(const bool& x) { _flextree = x; }

    inline bool gxtree() const { return _gxtree; }
    inline void gxtree(const bool& x) { _gxtree = x; }

    inline bool sampling() const { return _sampling; }
    inline void sampling(const bool& x) { _sampling = x; }

    inline bool sampleAlphaOmega() const { return _sampleAlphaOmega; }
    inline void sampleAlphaOmega(const bool& x) { _sampleAlphaOmega = x; }


    inline double epsAlphaOmega() const { return _epsAlphaOmega; }
    inline void epsAlphaOmega(const double& x) { _epsAlphaOmega = x; }




    inline bool xgb() const { return _xgb; }
    inline void xgb(const bool& x) { _xgb = x; }

    inline bool exgb() const { return _exgb; }
    inline void exgb(const bool& x) { _exgb = x; }
    
    inline bool indep() const { return _indep; }
    inline void indep(const bool& x) { _indep = x; }

    inline bool nreqd() const { return _nreqd; }
    inline void nreqd(const bool& x) { _nreqd = x; }

    inline bool eboxgb() const { return _eboxgb; }
    inline void eboxgb(const bool& x) { _eboxgb = x; }

    inline bool nxgb() const { return _nxgb; }
    inline void nxgb(const bool& x) { _nxgb = x; }


    inline uint xgb_num_trees() const { return _xgb_num_trees; }
    inline void xgb_num_trees(const uint& x) { _xgb_num_trees = x; }

    inline uint xgb_num_train_queries() const { return _xgb_num_train_queries; }
    inline void xgb_num_train_queries(const uint& x) { _xgb_num_train_queries = x; }


    inline bool gridtree() const { return _gridtree; }
    inline void gridtree(const bool& x) { _gridtree = x; }


    inline bool vih() const { return _vih; }
    inline void vih(const bool& x) { _vih = x; }

    inline double  q() const { return _q; }
    inline void    q(const double& x) { _q = x; }

    inline int  theta() const { return _theta; }
    inline void theta(const int& x) { _theta = x; }

    inline double zeta() const { return _zeta; }
    inline void   zeta(const double& x) { _zeta = x; }

    inline double alpha() const { return _alpha; }
    inline void   alpha(const double& x) { _alpha = x; }

    inline uint   precision() const { return _precision; }
    inline void   precision(const uint& x) { _precision = x; }


    inline double eps() const { return _eps; }
    inline void   eps(const double& x) { _eps = x; }

    inline uint   maxLevel() const { return _maxLevel; }
    inline void   maxLevel(const uint& x) { _maxLevel = x; }

    inline uint   dim() const { return _dim; }
    inline void   dim(const uint& x) { _dim = x; }


    inline uint noCoeff() const { return _noCoeff; }
    inline void noCoeff(const uint& x) { _noCoeff = x; }

    inline uint noBuckets() const { return _noBuckets; }
    inline void noBuckets(const uint& x) { _noBuckets = x; }

    inline uint budget() const { return _budget; }
    inline void budget(const uint& x) { _budget = x; }

    inline double shrinkage() const { return _shrinkage; }
    inline void   shrinkage(const double& x) { _shrinkage = x; }

    inline int   leafRefinement() const { return _leafRefinement; }
    inline void  leafRefinement(const int& x) { _leafRefinement = x; }

    inline double lrf() const { return _lrf; }
    inline void   lrf(const double& x) { _lrf = x; }

    inline int   logHi() const { return _logHi; }
    inline void  logHi(const int& x) { _logHi = x; }

    inline int   logLo() const { return _logLo; }
    inline void  logLo(const int& x) { _logLo = x; }

    inline bool materialize() const { return _materialize; }
    inline void materialize(const bool& x) { _materialize = x; }

    inline int   lowLim() const { return _lowLim; }
    inline void  lowLim(const int& x) { _lowLim = x; }

    inline bool printDot() const { return _printDot; }
    inline void printDot(const bool& x) { _printDot = x; }

    inline bool printRegP() const { return _printRegP; }
    inline void printRegP(const bool& x) { _printRegP = x; }

    // queries with very bad estimates can be written to output stream
    // flag determines whether to do so
    // Q determines the lower limit of the q-error for a bad query
    inline const badquery_t& printBadQueries() const { return _printBadQueries; }

    inline bool printBadFlag() const { return _printBadQueries.flag(); }
    inline void printBadFlag(const bool& x) { _printBadQueries._flag = x; }

    inline double printBadQ() const { return _printBadQueries._qmax; }
    inline void   printBadQ(const double& aQ) { 
                    _printBadQueries._flag = true; 
                    _printBadQueries._qmax = aQ; 
                  }

    inline double printBadTheta() const { return _printBadQueries.theta(); }
    inline void   printBadTheta(const double& aTheta) { 
                    _printBadQueries._flag  = true; 
                    _printBadQueries._theta = aTheta; 
                  }


    inline bool print() const { return _print; }
    inline void print(const bool& x) { _print = x; }

    inline bool debug() const { return _debug; }
    inline void debug(const bool& x) { _debug = x; }

    inline bool trace() const { return _trace; }
    inline void trace(const bool& x) { _trace = x; }

    inline std::string query() const { return _query; }
    inline void query(const std::string& x) { _query = x; }

  private:
    std::string _filename;
    bool        _isHistFile; // x, y, count (hist file) vs. x, y (value (only) file)
    uint        _no_query; // number of queries to generate (main_gen_query)
    std::string _filename_query; // filename to write queries to
    std::string _sds;            // name of set of data sets (directory name)
    std::string _ds;             // name of data set (filename without suffix .hist)
    uint        _nx; // number of partitions in x direction
    uint        _ny; // number of partitions in y direction
    uint        _rx; // number of subpartitionings in x direction, e.g., for EqDepth
    uint        _ry; // number of subpartitionings in y direction, e.g., for EqDepth
    int         _fieldWidth; // field width for printing matrices
    double      _dmax; // maximum distortion allows in class M tiles with no 4x4 subdivision
    uint        _delta; // determines class of tile
    uint        _kappa; // determines class of tile
    uint        _phi; // determines class of tile
    uint        _minimumNodeTotal; // for GxTree: min cum freq to allow for refinement of a tile
    int         _kind;  // kind: 0, 1, 2, ... see GridTree::kind_t
    int         _sampleSize; // > 0: do sampling with _sampleSize data points
    bool        _estConst;   // constant estimator, returns theta
    bool        _estArea;    // area-based estimator
    bool        _regp;       // regular partitioning estimates
    bool        _eqd;        // Equi-Depth-Histogram
    bool        _eqd2;       // Equi-Depth-Histogram, impl lubienetzki
    bool        _dct;        // discrete cosine transform
    bool        _dctcf;      // discrete cosine transform on cumulated frequency matrix
    bool        _nono;       // Nono-Tree
    bool        _genhist;    // GenHist
    bool        _waveletNonStd; // non-standard wavelet transform
    bool        _zhist;      // ZHist, ZCurves
    bool        _vih;        // VI-Histogram
    bool        _hft;        // store high frequency tiles only, avg for rest
    bool        _phased;     // Phased
    bool        _hired;      // HiRed
    bool        _mhist;      // MHist
    bool        _mhist2;     // MHist, impl lubienetzki
    bool        _qts;        // QTS (quadtree summary)
    bool        _iqts;       // IQTS (indexed quadtree summary)
    bool        _flextree;   // FlexTree
    bool        _gxtree;     // GxTree
    bool        _sampling;   // Sampling
    bool        _sampleAlphaOmega;  //sampleAlphaOmega	
    double      _epsAlphaOmega;        //eps for AlphaOmega Sampling		   
    bool        _gridtree;   // GridTree
    double      _q;          // q_{max}
    int         _theta;      // \theta
    double      _zeta;       // some double number, e.g., for dct relative truncation heuristics
    double      _alpha;      // some double number, e.g., for GenHist
    uint        _precision;  // e.g. for FlexTree
    double      _eps;        // some double number, e.g., for ignoring estimates
    uint        _maxLevel;   // max level, e.g., for HiRed
    uint        _noCoeff;    // number of coefficients to retain, e.g. for dct
    uint        _noBuckets;  // number of coefficients to retain, e.g. for dct
    uint        _budget;     // space budget in bytes
    int         _leafRefinement; // level of leaf refinemement for GxTree
    double      _lrf;        // leaf refinement factor (0 < lrf <= 1.0)
    double      _shrinkage;  // upper bound on shrinkage for optional br in nono tree
    int         _logHi;      // log2(nx)=log2(ny) of highest resolution grid
    int         _logLo;      // log2(nx)=log2(ny) of lowest resolution grid
    bool        _allGridTrees; // generate all grid trees for all pxo grids
    bool        _bestGridTree; // look for best grid tree pxo
    bool        _checkError;
    bool        _shortErrorCheck;
    bool        _withErrors;
    bool        _materialize; // if many queries are run, estimators are allowed to materialize something 
    int         _lowLim;
    bool        _printDot;
    bool        _printRegP;
    badquery_t  _printBadQueries;
    bool        _print; // print any histogram
    bool        _debug; // debug GxTree (runs restimates on Tree)
    bool        _trace; // trace any histogram
    std::string _query; // a single query to be run
    bool        _help;
    bool        _xgb;        // XGBoost  
    uint	_xgb_num_trees;//num of XGBoost trees
    uint	_xgb_num_train_queries;//_xgb_num_train_queries
    bool        _exgb; // enriched XGboost
    bool        _eboxgb; // enriched ebo minsel avi XGboost
    bool        _nxgb; //enriched xgb with QTS1D selectivities
    bool        _indep;// independance
    bool        _nreqd; //indep with 1D eqi depth hists by NR
    uint        _dim;
};


} // end namespace


#endif

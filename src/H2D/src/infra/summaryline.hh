#ifndef SUMMARYLINE_HH
#define SUMMARYLINE_HH

#include <iostream>
#include <iomanip>
#include <string>
#include <sys/utsname.h>

#include "types.hh"

namespace H2D {


/*
 * Hierarchie of summary lines
 * Base class: summaryline_t keeps track of input and output
 * subclasses keep track of method specific parameters
 */


class summaryline_t {
  public:
    // Equi-Width Histogram, aka Regular Partitioning
    struct regp_param_t { 
             uint _nx; // number of partitions in x direction
             uint _ny; // number of partitions in y direction
             uint _maxfreq; // maximum frequency found in a tile
           };

    // Equi-Depth Histogram
    struct eqdh_param_t {
             uint _nx; // number of partitions in x direction
             uint _ny; // number of partitions in y direction
             uint _rx; // for k_matrix: number of subpartitions in x direction
             uint _ry; // for k_matrix: number of subpartitions in y direction
           };

    // Sampling
    struct sample_param_t {
             uint _noSamples; // number of samples taken
           };

    // Grid Tree
    struct gt_param_t {
             uint   _nx; // number of partitions in x direction
             uint   _ny; // number of partitions in y direction

             uint   _delta; // border class S/M tiles
             uint   _kappa; // border class M/L tiles

             uint   _noZ; // number of class Z tiles (empty tiles)
             uint   _noS; // number of class S tiles
             uint   _noM; // number of class M tiles
             uint   _noL; // number of class L tiles
             uint   _depth; // depth of grid tree
             uint   _noNodes; // number of nodes
           };
    // Discrete Cosine Transform
    struct dct_param_t {
             uint   _nx;
             uint   _ny;
             uint   _noCoeff;
             double _zeta;
           };

    // Nono-Tree parameters
    struct nono_param_t {
             uint   _nx;
             uint   _ny;
             uint   _kappa;
             double _shrinkage;
             uint   _depth;
             uint   _noNodes;
           };
   
    // GenHist parameters
    struct genhist_param_t {
             uint _nx;
             uint _ny;
             double _alpha;
             uint   _delta;
             double _q;
             uint _noBuckets;
    };
    // Wavelet, non-standard
    struct waveletnonstd_param_t {
             uint _n;
             uint _noCoeff;
             uint _alpha;
    };
    // ZHist
    struct zhist_param_t {
       uint _n;
       uint _nxy;
       uint _noBuckets;
    };
    // Highly Frequent Tile
    struct hft_param_t {
      int  _logHi;
      int  _logLo;
      uint _kappa;

    };
    // Phased
    struct phased_param_t {
      uint _nx;
      uint _ny;
      uint _rx; // for k_matrix: number of subpartitions in x direction
      uint _ry; // for k_matrix: number of subpartitions in y direction
    };
    // HiRed
    struct hired_param_t {
     double _eps;
     uint   _maxLevel;
     uint   _noNodes;
     uint   _depth;
    };
    // MHist
    struct mhist_param_t {
      uint _noBuckets;
    };
    // QTS
    struct qts_param_t {
      uint _depth;
      uint _noNodes;
    };
    // IQTS
    struct iqts_param_t {
      uint _depth;
      uint _noNodes;
      uint _noNotRefined;
      uint _noLT23;
      uint _noLT24;
      uint _noLT2p;
    };
    // FlexTree
    struct flextree_param_t {
      uint _precisionLevel;
      uint _depth;
      uint _noNodes;
      uint _noGNodes;
      uint _minSplit;   // card of smallest split node
      uint _maxUnsplit; // card. of largest unsplit node
    };
    // GxTree
    struct gxtree_param_t {
      int    _leafRefinement;
      double _lrf;
      uint   _minimumNodeTotal;
      uint   _depth;
      uint   _noNodes;
      uint   _noGNodes;
      uint   _minSplit;   // card of smallest split node
      uint   _maxUnsplit; // card. of largest unsplit node
      char   _encoded;    // 'E' if from encoded subtree, 'T' if from tree
    };

    // union of all parameter structs 
    union param_t {
            regp_param_t          _regp;
            eqdh_param_t          _eqdh;
            sample_param_t        _sampling;
            gt_param_t            _gt;
            dct_param_t           _dct;
            nono_param_t          _nono;
            genhist_param_t       _genhist;
            waveletnonstd_param_t _waveletnonstd;
            zhist_param_t         _zhist;
            hft_param_t           _hft;
            phased_param_t        _phased;
            hired_param_t         _hired;
            mhist_param_t         _mhist;
            qts_param_t           _qts;
            iqts_param_t          _iqts;
            flextree_param_t      _flextree;
            gxtree_param_t        _gxtree;
          };
     
  public:
    summaryline_t();
    virtual ~summaryline_t();
  public:
    virtual std::ostream&  print(std::ostream& os, const std::string& aLineMarker) const;
  public:
    inline static double roundX(const double x) {
                           return (round(x * 10.0) / 10.0);
                         }
    inline static double roundXX(const double x) {
                           return (round(x * 100.0) / 100.0);
                         }
    inline static double roundXXX(const double x) {
                           return (round(x * 1000.0) / 1000.0);
                         }
    inline void setCheckTime(const double aCheckTime, const uint aNoQueries) {
                  _checkTimePerQuery = (aCheckTime * 1000.0 * 1000.0) / aNoQueries;
                }
  public:
    inline uint       card_class() const { return ((uint) std::floor(std::log2(_card))); }
    inline H2D_kind_t kind() const { return _kind; }
  public:
    bool read(std::istream& aIs);
  public:
    uint         _card;  // cardinality of original relation, i.e. total of input histogram
    uint         _noDv;  // number of distinct values in histogram in input file _filename
    uint         _budget; // given input budget, does not apply to all histograms
    uint         _size;  // size of histogram/datastructure, e.g., GridTree
    uint         _phi;   // boundary for outlier (is outlier its frequency exceeds phi)
    uint         _theta; // theta used for minEstimate, set to 1% of original relation size
    uint         _nout;  // number of outliers
    double       _constructionTime; // time to construct histogram/datastructure [s]
    double       _xgb_predTime;
    double       _checkTimePerQuery; // in mues (microseconds)
    double       _max[5];   // maximum q-error for 5 different thetas (e.g. 1%, 2%, 3%, 5%, 10%)
    double       _maxNZ[5];  // maximum q-error for 5 different thetas (e.g. 1%, 2%, 3%, 5%, 10%), lTrueVal < 1
    double       _avg[5];   // average q-error for 5 different thetas (e.g. 1%, 2%, 3%, 5%, 10%)
    double       _maxFixed[4]; // for fix theta's e.g. 1000, 5000, 10000, 20000
    double       _maxFixedNZ[4]; // for fix theta's e.g. 1000, 5000, 10000, 20000, lTrueVal < 1
    uint         _cntBadFixedNZ[4]; // count number of bad queries (e.g. estimate's qerror > 2 or > 10)
    H2D_kind_t   _kind;     // kind of histogram/datastructure, see types.hh
    int          _subkind;  // subkind (e.g. for GridTree::kind_t, maybe stored here);
    std::string  _filename; // input file name
    std::string  _hostname;
    param_t      _param;
};

std::ostream& operator<<(std::ostream&, const summaryline_t&);


} // end namespace

 
#endif

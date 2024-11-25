#ifndef H2D_I_GxTree_HH
#define H2D_I_GxTree_HH

#include <vector>
#include <algorithm>

#include "infra/PairingHeap.hpp"
#include "infra/array_tt.hh"
#include "infra/numarray_tt.hh"
#include "infra/bitvectorsmall.hh"


#include "infra/types.hh"
#include "infra/util.hh"
#include "infra/data2dim.hh"
#include "infra/RegularPartitioning2dim.hh"
#include "infra/EstimatorBase2dim.hh"
#include "infra/summaryline.hh"

#include "../scale/scale.hh"

#include "GxtTypes.hh"

#include "Gxt_Node_G.hh"
#include "Gxt_Node_M.hh"
#include "Gxt_Node_L.hh"
#include "Gxt_Node_S.hh"

/*
 * GxTree
 * new version of GxTree
 */

// general abbreviations:
// Mc, MC: micro child(ren)
// TR: tile rectangle
// BR, Br: bounding rectangle (true bounding rectangle)
// brd: bounding rectangle discretized
//  1. directly after calcNeedsBr:
//     reconstructed bounding rectangle from encoding

namespace H2D {


class GxTree : public EstimatorBase2dim {
  public:
    typedef numarray_tt<uint> uint_at;
  public:
    // class for nodes of the GxTree. GxTree itself only holds a pointer to the root
    class Node;
    // construction uses a heap of unsplit tiles 'sorted' by the tile frequencies
    struct heapentry_t {
      Node* _node;
      uint  _i;
      uint  _j;
    };
    class CMPNode {
      public:
        bool operator()(const heapentry_t& x, const heapentry_t& y) const {
               return (x._node->cumFreq(x._i, x._j) < y._node->cumFreq(y._i, y._j));
             }
    };

    typedef PairingHeap<heapentry_t, CMPNode> heap_t;
  public:
    // to experiment with restricted types of the GxTree by disallowing certain types of nodes
    enum gx_kind_t {
      K_Gxxx   = 0, // only G nodes
      K_GLxx   = 1, // only G and L nodes
      K_GLMx   = 2, // only G, L, and M nodes
      K_GLMS   = 3, // all nodes
      K_NO_KIND = 4
    };

    // node types = node sizes, must be aligned with _nodeSizes
    typedef Gxt::node_type_t node_type_t;
  public:
    // mm matrices store tile frequencies for different kinds of regular partitionings
    struct mm2_t {
      uint_at _2x2;
      uint_at _4x4;
      uint_at _8x8;
      uint_at _16x16;
      bool    _filled;

      mm2_t() : _2x2(2,2), _4x4(4,4), _8x8(8,8), _16x16(16,16), _filled(false) {}

      std::ostream& print(std::ostream& os) const;
    };

    struct mm3_t {
      uint_at _3x3;
      uint_at _6x6;
      uint_at _9x9;
      uint_at _12x12;
      bool    _filled;

      mm3_t() : _3x3(3,3), _6x6(6,6), _9x9(9,9), _12x12(12,12), _filled(false) {}

      std::ostream& print(std::ostream& os) const;
    };

    struct mm5_t {
      uint_at _5x5;
      uint_at _10x10;
      uint_at _20x20;
      bool    _filled;

      mm5_t() : _5x5(5,5), _10x10(10,10), _20x20(20,20), _filled(false) {}

      std::ostream& print(std::ostream& os) const;
    };

    // struct containg the result of the analysis of the current data points
    // it is used to decide which kind of node is used.
    struct anares_t {
       node_type_t  _nodeType;
       uint         _gridType;  // G_pXp: stores p // mainly for printing
       bool         _brDoesFit; // can node type decided upon hold a BR?
                                // if it becomes an inner node?
       bool         _leafEnforced; // sometimes nodes must become leafs (see M.I.B2)
   
       anares_t() : _nodeType(Gxt::N_NO_TYPE), _gridType(0), _brDoesFit(false), _leafEnforced(false) {}
 
       inline node_type_t nodeType() const { return _nodeType; }
       inline uint        gridType() const { return _gridType; }
       inline bool        brDoesFit() const { return _brDoesFit; }

       std::ostream& print(std::ostream& os) const;
    };

/*
    // stubs to be eliminated
    struct Gxt__Node_L_Generic {
    };
    struct Gxt__Node_M_Generic {
    };
    struct Gxt__Node_S_Generic {
    };
*/


    // struct representing the complete encoding of a GxTree 
    struct encoding_t {
      rectangle_t          _topBr; // br of total input data set
      uint                 _no[4]; // number of nodes for each kind (indexed by node_type_t)
      Gxt::Node_G_Generic* _G; // ptr to beginning of array containg the G-Nodes
      Gxt::Node_L_Generic* _L; // ptr to beginning of array containg the L-Nodes
      Gxt::Node_M_Generic* _M; // ptr to beginning of array containg the M-Nodes
      Gxt::Node_S_Generic* _S; // ptr to beginning of array containg the S-Nodes
      node_type_t          _rootType; // type of root node, its index must be 0
      gx_kind_t            _gxKind;   // what kind of nodes where allowed during construction
      encoding_t() : _topBr(), _no(), 
                     _G(0), _L(0), _M(0), _S(0),
                     _rootType(Gxt::N_NO_TYPE),
                     _gxKind(K_NO_KIND) {}
    };

    // struct used as argument to prepareEncoding
    struct encode_arg_t {
      uint _childCount[4];   // count number of children
    };

    // struct embedded in GxTree::Node to cluster all encoding related information together
    // _child*   is only used for inner nodes
    // _tileFreq is only used for leaf  nodes
    struct node_encoding_t {
      uint          _childBaseIdx[4]; // index of first child in according array
      uint          _childCount[4];   // number of children of each node_type_t
      uint_at       _childIdx;        // indexes of child nodes, specific for node_type_t
      uint_at       _tileFreq;        // tile frequencies, only used for leaf nodes
      Gxt::NodeType _nodeType;        // detailed node type of child (see GxtTypes.hh)
      uint          _brKind;          // 00: no BR, 01: 16x16, 10: 32x32, 11: 64x64
      uint16_t      _brCode;          // 4 numbers in 4 bits each
      uint          _myIdx;
      bool          _isLeaf;

      node_encoding_t() : _childBaseIdx(), _childCount(), _childIdx(), _tileFreq(),
                          _nodeType(), _brKind(), _brCode(), _myIdx(0), _isLeaf(false) {
        for(uint i = 0; i < 4; ++i) {
          _childBaseIdx[i] = 0;
          _childCount[i] = 0;
        }
      }

      inline Gxt::NodeType nodeType() const { return _nodeType; }

      inline bool     isLeaf() const { return _isLeaf; }
      inline bool     hasBr() const { return (0 != _brKind); }
      inline uint     brKind() const { return _brKind; }
      inline uint16_t brCode() const { return _brCode; }
      inline uint     myIdx() const { return _myIdx; }
             uint     noChildPtr() const;
    };

  public:

    class Node {
      public:
        typedef array_tt<Data2dim> data2dim_at;
        typedef array_tt<Node*> node_at;
        typedef array_tt<RegularPartitioning2dim> regp_at;
      private:
        friend class GxTree;
        Node(const Node&);
        Node& operator=(const Node&);
      public:
        Node(const Data2dim&    aData, 
             const rectangle_t& aTr, // bouding tile rectangle from parent
             const uint         aLevel,
             const Node*        aParent,
             const GxTree&      aGxTree);
        ~Node();
      public:
        inline const Data2dim&    data() const { return _data; } // input data
        inline const rectangle_t& tr() const { return _tr; } // tile rectangle
        inline const rectangle_t& br() const { return _br; } // bounding rectangle, reconstructed
        inline const rectangle_t& brd() const { return _brd; }
        inline const rectangle_t& brdd() const { return _brdd; }
        inline       double       cumFreq() const { return _cumFreq; }
        inline       double       cumFreq(const uint i, const uint j) const { 
                                    return regpPxO(i, j); 
                                  }
        inline       uint         level() const { return _level; }
        inline       uint         nodeId() const { return _nodeId; }
        inline const anares_t&    anares() const { return _anares; }
        inline       node_type_t  nodeType() const { return anares().nodeType(); }
        inline       uint         nx() const { return _regpPxO.noRows(); } // P = nx = cnx
        inline       uint         ny() const { return _regpPxO.noCols(); } // O = ny = cny
        inline       uint         cnx() const { return _childrenPxO.noRows(); }
        inline       uint         cny() const { return _childrenPxO.noCols(); }
        inline const Node*        child(const uint i, const uint j) const { return _childrenPxO(i, j); }
        inline       Node*        child(const uint i, const uint j) { return _childrenPxO(i, j); }
        inline       bool         isUnsplit(const uint i, const uint j) const { return (0 == child(i,j)); }
        inline       bool         isSplit(const uint i, const uint j) const { return (0 != child(i,j)); }
        inline       uint         size() const { return Gxt::nodeSize(nodeType()); }  // this node
                     uint         calcSize() const; // recursively, in bytes
                     bool         isLeaf() const;
                     uint         noChildren() const;

        inline const RegularPartitioning2dim& tr_regp16x16() const { return _tr_regp16x16; }
        inline       double       tr_regp16x16(const uint i, const uint j) const {
                                    return _tr_regp16x16(i,j);
                                  }

        inline const RegularPartitioning2dim& br_regp16x16() const { return _br_regp16x16; }
        inline       double       br_regp16x16(const uint i, const uint j) const {
                                    return _br_regp16x16(i,j);
                                  }

        inline const RegularPartitioning2dim& regpPxO() const { return _regpPxO; }
        inline       double       regpPxO(const uint i, const uint j) const {
                                    return _regpPxO(i,j);
                                  }
        inline const Data2dim&    dataPxO(const uint i, const uint j) const {
                                    return _dataPxO(i, j);
                                  }

      public:
        uint32_t   calcNeedsBr(const RegularPartitioning2dim& aRegP16x16, 
                               const rectangle_t* aTileRectangle,
                                     rectangle_t* aBrNewOut) const;
        void       reconstructBr(      rectangle_t&       aBrOut,
                                 const partitiondescxy_t& aPd // for 16x16 tile partition
                                ) const;
        inline bool     needsBr() const { return (0 != _brCode); }
        inline uint32_t brCode() const { return _brCode; }
        inline int      hasBr() const { return _hasBr; } // -1: unknown, 0: no, 1: yes
      public:
        uint   depth() const;
        uint   noNodes() const;
        uint   noLeafNodes() const;
        uint   noInnerNodes() const;
        uint   noGNodes() const;
        uint   noLNodes() const;
        double minLeafCumFreq() const;
        double maxLeafCumFreq() const;
      public:
        void  init(const Data2dim& aData, const rectangle_t& aTr, const Node* aParent, const GxTree& aGxTree);
        bool  initG(const Data2dim& aData, const anares_t& aAnaRes, const GxTree& aGxTree);
        bool  initL(const Data2dim& aData, const anares_t& aAnaRes, const GxTree& aGxTree);
        bool  initM(const Data2dim& aData, const anares_t& aAnaRes, const GxTree& aGxTree);
        bool  initS(const Data2dim& aData, const anares_t& aAnaRes, const GxTree& aGxTree);
        Node* expandChild(const uint i, const uint j, const GxTree& aGxTree);
        void  insertChildrenIntoHeap(heap_t& aHeap, const GxTree& aGxTree);
        // prepare calls distribute data and clearChildren
        void  prepare(const Data2dim& aData, const uint aNx, const uint aNy);
        // distributeData distributes data in aData to aDataArr
        void  distributeData(const Data2dim& aData, 
                             const uint aNx,
                             const uint aNy,
                             data2dim_at& aDataArr);
        // clearChildren sets child pointers to zero
        void  clearChildren();
      public:
        void analyze(anares_t& aAnaRes, const Node* aParent, const GxTree& aGxTree);
        uint maxUnsplitTileFrequency() const;
      public:
        double estimate(const rectangle_t& aQueryRectangle, 
                        const rectangle_t& aTileRectangle,
                        const GxTree&    aGxTree) const;
        double estimateSub(const rectangle_t& aQueryRectangle, 
                           const rectangle_t& aTileRectangle,
                           const GxTree&      aGxTree) const;
      public:
        inline double shrinkageBrTr() const { return (br().area() / tr().area()); }
        inline double shrinkageBrdTr() const { return (brd().area() / tr().area()); }
      public:
        double maxQError(const RegularPartitioning2dim& aTru,
                         const RegularPartitioning2dim& aEst) const;
        double qerrOfAvg(const uint_at& aMatrix) const;
        double qerrIncrease() const; // needs _mm2, _mm5

      private:
        static void fill_mm2(      mm2_t& aMM2,
                             const RegularPartitioning2dim& aRegP16x16,
                             const GxTree&); 
               void fill_mm3(      mm3_t&       aMM3,
                             const rectangle_t& aBrOrTr,
                             const Data2dim&    aData,
                             const GxTree&      aGxTree); 
               // fill_mm5 always uses brd()
               void fill_mm5(      mm5_t&    aMM5,
                             const Data2dim& aData,
                             const GxTree&   aGxTree); 

      private:
        void prepareEncoding(encode_arg_t&, 
                             const uint aMyIdx, 
                             const GxTree&);
        void performEncoding(encoding_t&, const bool aCheckEncoding, const GxTree&);
        // can be called after peroformEncoding (for debugging):
        void printEncodingInfo(std::ostream& os, const encoding_t&) const;
  
      private:
        void determineDetailedNodeType(const GxTree&); // called only in prepareEncoding
        void performEncoding_G(encoding_t&, const bool aCheckEncoding, const GxTree&);
        void performEncoding_L(encoding_t&, const bool aCheckEncoding, const GxTree&);
        void performEncoding_M(encoding_t&, const bool aCheckEncoding, const GxTree&);
        void performEncoding_S(encoding_t&, const bool aCheckEncoding, const GxTree&);
      public:
        static inline bool maxQErrOk(const double x, const double y, const double d) {
                             return ((x * d >= y) && (y * d) >= x);
                           }

      public:
        std::ostream& print(std::ostream& os) const;
        // the aRegP in the following function is used as a reference to calculate the max qerror
        void          printPxO(std::ostream& os, const uint aNx, const uint aNy, const RegularPartitioning2dim* aRegP = 0) const;
        void          printThePxO(std::ostream& os) const;
        std::ostream& printDot(std::ostream& os, const int aParentId) const;
      private:
        const Data2dim&  _data;       // data points
        rectangle_t _tr;   // tile rectangle, derived from parents grid 
        rectangle_t _br;   // bounding rectangle, only for testing
        rectangle_t _brd;  // bounding rectangle, 16x16 discretized
        rectangle_t _brdd; // _brd or _tr, depending on whether BR is needed/fits
        double      _cumFreq;    // number of points contained in _tr
        RegularPartitioning2dim _tr_regp16x16; // on tile rectangle
        RegularPartitioning2dim _br_regp16x16; // on bounding rectangle

        mm2_t _tr_mm2; // build on tile rectangle
        mm2_t _br_mm2; // build on bounding rectangle, if needed
        mm3_t _mm3;    // 
        mm5_t _mm5;    // 


        RegularPartitioning2dim _regpPxO; // final decision: PxO grid
        data2dim_at             _dataPxO; // final decision: PxO grid
        node_at                 _childrenPxO;  // pointers too child nodes (only used during construction), PxO

        uint        _level;      // level of node (only used for nicer output)
        uint        _nodeId;     // id of node (only used for nicer output)
        uint32_t    _brCode;     // encoding of bounding rectangle, == 0 if no br is needed (tile doesn't shrink)
        int         _hasBr; // -1: unknown, 0: no, 1: yes
        anares_t    _anares;
        node_encoding_t _myEncoding;
    };

  private:
    GxTree(const GxTree&);
    GxTree& operator=(const GxTree&);
  public:
    GxTree(const Data2dim& aData,
           const gx_kind_t aGxKind,
           const uint      aBudget, // in number of bytes
           const int       aLeafRefinement,
           const double    aLrf, // leaf refinenement factor (0 < lrf <= 1)
           const uint      aMinimumNodeTotal,
           const uint      aPhi,
           const double    aQ,
           const double    aTheta,
           const bool      aTrace);
    virtual ~GxTree();
  public:
    void init(const Data2dim& aData);
  public:
    inline const rectangle_t& br() const { return _br; }
    inline gx_kind_t          gxKind() const { return _gxKind; }
    inline gx_kind_t          kind() const { return _gxKind; }
    inline const Data2dim&    outlier() const { return _outlier; }
    inline uint               noOutlier() const { return _outlier.size(); }
    inline uint               budget()  const { return _budget; } // in number of bits
    inline  int               leafRefinement() const { return _leafRefinement; }
    inline double             lrf() const { return _lrf; } // leaf refinement factor (0 < lrf <= 1)
    inline uint               minimumNodeTotal() const { return _minimumNodeTotal; }
    inline uint               phi() const { return _phi; }
    inline const Node*        root() const { return _root; }
    inline bool               trace() const { return _trace; }
    inline void               trace(const bool x) { _trace = x; }
  public:
    // get a new unique node ID
    inline uint               getNodeId() const { return (_nodeCount++); }
  public:
    virtual double estimate(const rectangle_t& r) const;
    virtual double estimate(const query_t& lQuery) const;
  public:
    uint   outlierCount(const rectangle_t& r) const;
    virtual uint size() const override; // in number of bytes
    uint   calcSize() const; // in number of bytes
    uint   depth() const;
    uint   noNodes() const;
    uint   noLeafNodes() const;
    uint   noInnerNodes() const;
    uint   noGNodes() const;
    uint   noLNodes() const;
    double minLeafCumFreq() const;
    double maxLeafCumFreq() const;
  public:
    inline uint     minSplit() const { return _minSplit; }
    inline uint     maxUnsplit() const { return _maxUnsplit; }
  public:
    // aRefineLeafs: use leaf grids other than inner grid
    void encode(const bool aCheckEncoding);
    inline const encoding_t& encoding() const { return _encoding; }
  public:
    inline void incNoG() const { ++_nodeCountG; }
    inline void incNoL() const { ++_nodeCountL; }
    inline void incNoM() const { ++_nodeCountM; }
    inline void incNoS() const { ++_nodeCountS; }
  public:
    virtual std::ostream& print_name_param(std::ostream& os) const;
            std::ostream& print(std::ostream& os) const;
            std::ostream& printDot(std::ostream& os) const;
            std::ostream& printParameters(std::ostream& os) const;
            std::ostream& printEncodingInfo(std::ostream& os) const;
  private:
    Data2dim     _outlier;
    rectangle_t  _br; // bounding rectangle
    gx_kind_t    _gxKind;
    uint         _budget; // in number of bytes
     int         _leafRefinement; // allowed kinds of leaf nodes A, or A,B or A,B,C...
    double       _lrf; // leaf refinement factor
    uint         _minimumNodeTotal; // only tiles with at least this number of elements (possibly) refined
    uint         _size;   // in number of bytes
    const uint   _phi;
    Node*        _root;
    mutable uint _nodeCount;
    mutable uint _nodeCountG;
    mutable uint _nodeCountL;
    mutable uint _nodeCountM;
    mutable uint _nodeCountS;
    bool         _trace;
    uint         _minSplit;    // after construction: minimum cum freq of all tiles split
    uint         _maxUnsplit;  // after construction: maximum cum freq of all unsplit tiles
    encoding_t   _encoding;
  public:
    const GxTreeItp*   _gxtitp; // for test purposes only
};

// GxTreeInterpreter
// interpretes encoded GxTree
// to provide an estimate

class GxTreeItp : public EstimatorBase2dim {
  public:
    typedef GxTree::encoding_t encoding_t;
    typedef Gxt::node_type_t   node_type_t;
    typedef double (GxTreeItp::*estfun_t)(const rectangle_t& aQueryRectangle,
                                          const rectangle_t& aTileRectangle,
                                          const uint         aIdx,
                                          const uint         aLevel) const;
  public:
    GxTreeItp(const Data2dim&   aOutlier,
              const encoding_t& aGxTreeEncoding, 
              const bool        aTrace);
    virtual ~GxTreeItp();
  public:
    virtual double estimate(const rectangle_t& aQueryRectangle) const;
    virtual double estimate(const query_t& lQuery) const;

  public: 
    // only used by estimate templates in estimate_t.hh
    // and by GxTree::Node::estimate for testing purposes
    double estimate(const rectangle_t& aQueryRectangle,
                    const rectangle_t& aTileRectangle,
                    const uint         aIdx,
                    const node_type_t  aNodeType,
                    const uint         aLevel) const;
  public:
    inline const encoding_t& encoding() const { return _encoding; }
  private:
    double estimateG(const rectangle_t& aQueryRectangle,
                     const rectangle_t& aTileRectangle,
                     const uint         aIdx,
                     const uint         aLevel) const;
    double estimateL(const rectangle_t& aQueryRectangle,
                     const rectangle_t& aTileRectangle,
                     const uint         aIdx,
                     const uint         aLevel) const;
    double estimateM(const rectangle_t& aQueryRectangle,
                     const rectangle_t& aTileRectangle,
                     const uint         aIdx,
                     const uint         aLevel) const;
    double estimateS(const rectangle_t& aQueryRectangle,
                     const rectangle_t& aTileRectangle,
                     const uint         aIdx,
                     const uint         aLevel) const;
  public:
    inline const Data2dim& outlier() const { return _outlier; }
    inline uint            noOutlier() const { return _outlier.size(); }
           uint            outlierCount(const rectangle_t& r) const;
  public:
    virtual uint size() const override; // returns always 0
  public:
    inline const uint no_G() const { return _encoding._no[Gxt::N_G]; }
    inline const uint no_L() const { return _encoding._no[Gxt::N_L]; }
    inline const uint no_M() const { return _encoding._no[Gxt::N_M]; }
    inline const uint no_S() const { return _encoding._no[Gxt::N_S]; }
    inline const Gxt::Node_G_Generic* get_G() const { return _encoding._G; }
    inline const Gxt::Node_L_Generic* get_L() const { return _encoding._L; }
    inline const Gxt::Node_M_Generic* get_M() const { return _encoding._M; }
    inline const Gxt::Node_S_Generic* get_S() const { return _encoding._S; }
    inline const Gxt::Node_G_Generic& get_G(const uint i) const { return _encoding._G[i]; }
    inline const Gxt::Node_L_Generic& get_L(const uint i) const { return _encoding._L[i]; }
    inline const Gxt::Node_M_Generic& get_M(const uint i) const { return _encoding._M[i]; }
    inline const Gxt::Node_S_Generic& get_S(const uint i) const { return _encoding._S[i]; }
  public:
    inline bool trace() const { return _trace; }
    inline void trace(const bool x) { _trace = x; }
  public:
    virtual std::ostream& print_name_param(std::ostream& os) const;
            std::ostream& print(std::ostream& os) const;
            std::ostream& printNodeTypes(std::ostream& os) const;
  private:
    Data2dim          _outlier;
    const encoding_t& _encoding;
    bool              _trace;
  private:
    static estfun_t   _estfun[4];
};


} // end namspace


#endif

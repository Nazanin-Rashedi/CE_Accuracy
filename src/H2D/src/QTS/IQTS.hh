#ifndef H2D_I_IQTS_HH
#define H2D_I_IQTS_HH

#include <vector>
#include <algorithm>
#include <inttypes.h>

#include "infra/PairingHeap.hpp"
#include "infra/array_tt.hh"
#include "infra/bitvectorsmall.hh"

#include "infra/types.hh"
#include "infra/util.hh"
#include "infra/data2dim.hh"
#include "infra/RegularPartitioning2dim.hh"
#include "infra/EstimatorBase2dim.hh"
#include "infra/summaryline.hh"


/*
 * IQTS/IIQTS
 * Buccafurri, Furfaro, Sacca, Sirangelo 2003
 */

namespace H2D {


class IQTS : public EstimatorBase2dim {
  public:
    enum kind_t {
      k_l2_sse  = 0, // priority queue via max SSE
      k_lq_sse  = 1, // priority queue via max SSE
      k_l2_var  = 2, // via max variance
      k_lq_var  = 3, // via max variance
      k_l2_card = 4, // via max cardinality
      k_lq_card = 5, // via max cardinality
      k_nokind = 3
    };
    inline bool useL2() const { return (0x1 == (kind() & 0x1)); }
    inline bool useLQ() const { return (0x0 == (kind() & 0x1)); }

    class Node {
      private:
        friend class IQTS;
        Node(const Node&);
        Node& operator=(const Node&);
        typedef array_tt<Data2dim> data2dim_at;
      public:
        enum kind_LT_t {
               k_lt_23 = 0,
               k_lt_24 = 1,
               k_lt_2p = 2,
               k_lt_no_lt = 3
             };
        static inline kind_LT_t getKindLT(const uint64_t lCode) {
                                  if(0x1 == ((lCode >> 63) & 0x1)) {
                                    return k_lt_24;
                                  } else
                                  if(0x0 == ((lCode >> 61) & 0x3)) {
                                    return k_lt_23;
                                  } else {
                                    return k_lt_2p;
                                  }
                                }
        inline kind_LT_t getKindLT() const {
                           return getKindLT(code());
                         }
      public:
        Node(const Data2dim&    aData, 
             const rectangle_t& aBr,
             const uint         aLevel,
             const IQTS&        aIQTS,
             const bool         a11);
        ~Node();
      public:
        inline const Data2dim&    data() const { return _data; }
        inline const rectangle_t& br() const { return _br; }
        inline       double       cumFreq() const { return _cumFreq; }
        inline       double       sse() const { return _sse; }
        inline       uint         level() const { return _level; }
        inline       uint         nodeId() const { return _nodeId; }
        inline const Node*        child(const uint i, const uint j) const { return _child[i][j]; }
        inline       Node*        child(const uint i, const uint j) { return _child[i][j]; }
        inline       uint         sizeInBits() const { return _sizeInBits; }
        inline       bool         isLeaf() const { return (0 == _child[0][0] &&
                                                           0 == _child[0][1] &&
                                                           0 == _child[1][0] &&
                                                           0 == _child[1][1]); }
        inline       bool         hasRefinement() const { return _hasRefinement; }
        inline       uint64_t     code() const { return _code; }
      public:
        uint depth() const;
        uint noNodes() const;
        uint noNotRefined() const;
        uint noLT23() const;
        uint noLT24() const;
        uint noLT2p() const;
      public:
        void performSplit(const IQTS& aIQTS);
        void prepareEncoding(const Data2dim& aData, const rectangle_t aBr, const IQTS& aIQTS);
      public:
        static double errorL2(const Matrix& M8x8Tru, const double aAvg);
        static double errorLQ(const Matrix& M8x8Tru, const double aAvg, const double aTheta);
        static double errorL2_4x4(const Matrix& M8x8Tru, const Matrix& M4x4Est);
        static double errorLQ_4x4(const Matrix& M8x8Tru, const Matrix& M4x4Est, const double aTheta);
        static double errorL2_8x8(const Matrix& M8x8Tru, const Matrix& M8x8Est);
        static double errorLQ_8x8(const Matrix& M8x8Tru, const Matrix& M8x8Est, const double aTheta);
      public:
        double estimate(const rectangle_t& aQueryRectangle, 
                        const rectangle_t& aBoundingRectangle,
                        const IQTS&         aIQTS) const;
        double estimateLT23(const rectangle_t& aQueryRectangle, 
                            const rectangle_t& aBoundingRectangle,
                            const IQTS&         aIQTS) const;
        double estimateLT24(const rectangle_t& aQueryRectangle, 
                            const rectangle_t& aBoundingRectangle,
                            const IQTS&         aIQTS) const;
      public:
        // for 2/3-LT: matrix must be 4x4 matrix
        static uint64_t encode23LT(const Matrix& M, const uint aStartRowId, const uint aStartColId);
        static bool     decode23LT(const uint64_t aCode, const double aTotal,
                                   Matrix& M, const uint aStartRowId, const uint aStartColId);
        // for 2/4-LT: matrix must be 8x8 matrix
        static uint64_t encode24LT(const Matrix& M, const uint aQEnc);
        static bool     decode24LT(const uint64_t aCode, const double aTotal, Matrix& M);
      public:
        static inline uint64_t encode(double z, double n, uint aNoBits) {
                                  if(0 >= n) {
                                    return (uint64_t) 0LL;
                                  }
                                  const uint64_t lRes = round((z/n) * (double) ((1 << aNoBits) - 1));
                                  if(0 != (lRes & ~((1 << aNoBits) - 1)) ) {
                                    std::cout << "ENCODE(" << z << ", " << n << ", " << aNoBits 
                                              << ") --> " << Bitvector64(lRes) << std::endl;
                                  }
                                  return lRes;
                               }
        static inline double   decode(uint64_t z, double aTotal, uint aNoBits) {
                                  const uint64_t m = ((1 << aNoBits) - 1);
                                  return round(((double) (z & m)  / ((double) m)) * aTotal);
                               }
        static uint64_t encode2x2(const double f00, const double f01, const double f10, const double f11,
                                  const double aTotal, const uint aNoBits1, const uint aNoBits2);
        static void     decode2x2(const uint64_t aCode, const double aTotal, const uint aNoBits1, const uint aNoBits2,
                                  double& f00, double& f01, double& f10, double& f11);

        static uint64_t encode2x2(const double f00, const double f01, const double f10, const double f11,
                                  const uint aNoBits1, const uint aNoBits2);
        static uint64_t encode2x2(const Matrix& M, const uint aStartRowId, const uint aStartColId,
                                  const uint aNoBits1, const uint aNoBits2);

        static void     decode2x2(const uint64_t aCode, const double aTotal, const uint aNoBits1, const uint aNoBits2,
                                  Matrix& M, const uint aStartRowId, const uint aStartColId);

        static uint64_t encode4x4(const Matrix& M, const uint aStartRowId, const uint aStartColId,
                                  const uint aNoBits01, const uint aNoBits02,
                                  const uint aNoBits11, const uint aNoBits12);
        static void     decode4x4(const uint64_t aCode, const double aTotal,
                                  Matrix& M, const uint aStartRowId, const uint aStartColId,
                                  const uint aNoBits01, const uint aNoBits02,
                                  const uint aNoBits11, const uint aNoBits12);
      public:
        std::ostream& print(std::ostream& os) const;
      private:
        Data2dim    _data;    // data points
        rectangle_t _br;      // bounding rectangle
        double      _cumFreq; // number of points contained in _br
        double      _sse;     // sse 
        Node*       _child[2][2]; // quadtree!
        uint        _level;
        uint        _nodeId;
        uint        _sizeInBits; // in bits
        bool        _hasRefinement;
        uint64_t    _code;
    };

    class CMPNode {
      public:
        bool operator()(const Node* x, const Node* y) const { return (x->sse() < y->sse()); }
    };

    typedef PairingHeap<Node*, CMPNode> heap_t;
  private:
    IQTS(const IQTS&);
    IQTS& operator=(const IQTS&);
  public:
    IQTS(const Data2dim& aData,
         const kind_t    aKind,
         const uint      aBudget, // in number of bytes
         const uint      aPhi,
         const double    aQ,
         const double    aTheta,
         const bool      aTrace);
    virtual ~IQTS();
  public:
    void init(const Data2dim& aData);
  public:
    inline const rectangle_t& br() const { return _br; }
    inline kind_t             kind() const { return _kind; }
    inline const Data2dim&    outlier() const { return _outlier; }
    inline uint               noOutlier() const { return _outlier.size(); }
    inline uint               budget()  const { return _budget; } // in number of bits
    inline uint               phi() const { return _phi; }
    inline const Node*        root() const { return _root; }
    inline bool               trace() const { return _trace; }
    inline void               trace(const bool x) { _trace = x; }
    inline uint               getNodeId() const { return (_nodeCount++); }
  public:
    virtual double estimate(const rectangle_t& r) const;
    virtual double estimate(const query_t& lQuery) const;
  public:
    uint   outlierCount(const rectangle_t& r) const;
    uint   size() const; // in number of bytes
    uint   depth() const;
    uint   noNodes() const;
    uint   noNotRefined() const;
    uint   noLT23() const;
    uint   noLT24() const;
    uint   noLT2p() const;

  public:
    virtual std::ostream& print_name_param(std::ostream& os) const;
            std::ostream& print(std::ostream& os) const;
  private:
    Data2dim     _outlier;
    rectangle_t  _br; // bounding rectangle
    kind_t       _kind;
    uint         _budget; // in number of bits 
    uint         _size;   // in number of bits
    const uint   _phi;
    Node*        _root;
    mutable uint _nodeCount;
    bool         _trace;
};


} // end namspace


#endif

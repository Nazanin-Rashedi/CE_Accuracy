#ifndef H2D_QTS1D_HH
#define H2D_QTS1D_HH

#include <vector>
#include <algorithm>

#include "infra/PairingHeap.hpp"
#include "infra/array_tt.hh"


#include "infra/types.hh"
#include "infra/util.hh"
#include "infra/data2dim.hh"
#include "infra/RegularPartitioning2dim.hh"
#include "infra/EstimatorBase2dim.hh"
#include "infra/summaryline.hh"


/*
 * QTS/IQTS
 * Buccafurri, Furfaro, Sacca, Sirangelo 2003
 */

namespace H2D {


class QTS1D {
  public:
    enum kind_t {
      k_sse  = 0, // priority queue via max SSE
      k_var  = 1, // via max variance
      k_card = 2, // via max cardinality
      k_nokind = 3
    };
    class Node {
      private:
        friend class QTS1D;
        Node(const Node&);
        Node& operator=(const Node&);
        typedef array_tt<Data2dim> data2dim_at;
      public:
        Node(//const std::vector<double> aColumn,
	     const Data2dim&    aData, 
             const line_t&      aLine,
             const uint         aLevel,
             const QTS1D&         aQTS,
             const bool         a11);
        ~Node();
      public:
        inline const Data2dim&    data() const { return _data; }
        inline const line_t& br() const { return _br; }
        inline       double       cumFreq() const { return _cumFreq; }
        inline       double       sse() const { return _sse; }
        inline       uint         level() const { return _level; }
        inline       uint         nodeId() const { return _nodeId; }
        inline const Node*        child(const uint i) const { return _child[i]; }
        inline       Node*        child(const uint i) { return _child[i]; }
        inline       uint         sizeInBits() const { return _sizeInBits; }
        inline       bool         isLeaf() const { return (0 == _child[0] &&
                                                           0 == _child[1]); }
      public:
        uint depth() const;
        uint noNodes() const;
      public:
        void performSplit(const QTS1D& aQTS);
      public:
        double estimate(const line_t& aQueryLine, 
                        const line_t& aBoundingLine,
                        const QTS1D&         aQTS) const;
      public:
        std::ostream& print(std::ostream& os) const;
      private:
        Data2dim    _data;    // data points
        line_t      _br;      // bounding rectangle
        double      _cumFreq; // number of points contained in _br
        double      _sse;     // sse 
        Node*       _child[2]; // quadtree!
        uint        _level;
        uint        _nodeId;
        uint        _sizeInBits; // in bits
    };

    class CMPNode {
      public:
        bool operator()(const Node* x, const Node* y) const { return (x->sse() < y->sse()); }
    };

    typedef PairingHeap<Node*, CMPNode> heap_t;
  private:
    QTS1D(const QTS1D&);
    QTS1D& operator=(const QTS1D&);
  public:
    QTS1D(const Data2dim& aData,
        const kind_t    aKind,
        const uint      aBudget, // in number of bytes
        const uint      aPhi,
        const bool      aTrace,
	const uint      dim);
    virtual ~QTS1D();
  public:
    void init(const Data2dim& aData);
  public:
    inline const line_t& br() const { return _br; }
    inline const uint dim() const{ return _dim; }
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
    //virtual double estimate(const rectangle_t& r) const;
    double estimate(const line_t& lQuery) const;
  public:
    uint   outlierCount(const line_t& r) const;
    uint   depth() const;
    uint   noNodes() const;
  public:
    uint size() const; // in number of bytes
  public:
    //virtual std::ostream& print_name_param(std::ostream& os) const;
            std::ostream& print(std::ostream& os) const;
  private:
    Data2dim     _outlier;
    line_t       _br; // bounding rectangle
    kind_t       _kind;
    uint         _budget; // in number of bits 
    uint         _size;   // in number of bits
    const uint   _phi;
    Node*        _root;
    mutable uint _nodeCount;
    bool         _trace;
    uint         _dim;
};


} // end namspace


#endif

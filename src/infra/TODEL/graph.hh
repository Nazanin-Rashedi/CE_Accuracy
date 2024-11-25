#ifndef INFRA_GRAPH_HH
#define INFRA_GRAPH_HH

#include <iostream>
#include <vector>

#include "infra/simpledictionary_a.hh"

/*
 * SGraph
 * implements a simple graph
 * nodes are mapped to integers via a simple dictionary
 * edges contain two integers and a load
 */

template<class Tnode, class Tedgeload>
class SGraph {
  public:
    typedef SimpleDictionary<Tnode> node_dic_t;
    struct edgebase_t { unsigned int _from, _to; };
    typedef std::vector<edgebase_t> edgebase_vt;
    class Edge;
    class Node;
    typedef std::vector<Edge*> edge_vt;
    typedef typename edge_vt::const_iterator edge_cit;
  public:
    SGraph();
    ~SGraph();
  public:
    inline unsigned int noNodes() const { return _nodes.size(); }
    inline unsigned int noEdges() const { return _edges.size(); }
    inline const Node& node(unsigned int i) const { return (_nodes[i]); }
    inline       Node& node(unsigned int i)       { return (_nodes[i]); }
    inline const Edge& edge(unsigned int i) const { return (_edges[i]); }
    inline       Edge& edge(unsigned int i)       { return (_edges[i]); }
  public:
    void insert(Edge*);
  public:
    static SGraph* read(std::istream& is);
    std::ostream&      write(std::ostream&) const;
    std::ostream&      print(std::ostream&) const;
  private:
    node_dic_t   _nodes;
    edge_vt      _edges;
};


template<class Tnodeload, class Tedgeload>
class SGraph<Tnodeload,Tedgeload>::Edge {
  friend class SGraph<Tnodeload,Tedgeload>;
  private:
    Edge(unsigned int aNo, unsigned int aFrom, unsigned int aTo);
    Edge(unsigned int aNo, unsigned int aFrom, unsigned int aTo, const Tedgeload& x);
  public:
    Edge();
    Edge(unsigned int aFrom, unsigned int aTo);
    Edge(unsigned int aFrom, unsigned int aTo, const Tedgeload& x);
    ~Edge();
  public:
    inline unsigned int from() const { return _from; }
    inline unsigned int to() const { return _to; }
    inline const Tnodeload& load() const { return _load; }
    inline       Tnodeload& load()       { return _load; }
    inline unsigned int other(const unsigned int i) const { return ((i == _from) ? _to : _from); }
  private:
    unsigned int  _from;
    unsigned int  _to;
  public:
    Tedgeload     _load;
};

template<class Tnodeload, class Tedgeload>
class SGraph<Tnodeload,Tedgeload>::Node {
  friend class SGraph<Tnodeload,Tedgeload>;
  private:
    Node(unsigned int aNo, const Tnodeload&);
  public:
    Node(); 
    Node(const Tnodeload&);
    ~Node();
  public:
    inline unsigned int no() const { return _no; }
    inline const Tnodeload& load() const { return _load; }
    inline       Tnodeload& load()       { return _load; }
  public:
    inline const edgelist_t& edgesAll() const { return _edgesAll; }
    inline const edgelist_t& edgesBefore() const { return _edgesBefore; }
    inline const edgelist_t& edgesAfter() const { return _edgesAfter; }
    
  private:
    unsigned int _no;
    edgelist_t   _edgesAll;
    edgelist_t   _edgesBefore;
    edgelist_t   _edgesAfter;
    Tnodeload    _load;
};


template<class Tnodeload, class Tedgeload>
SGraph<Tnodeload,Tedgeload>::Edge::Edge() : _no(0), _from(0), _to(0), _load() {}

template<class Tnodeload, class Tedgeload>
SGraph<Tnodeload,Tedgeload>::Edge::Edge(unsigned int aNo, 
                                       unsigned int aFrom, unsigned int aTo, 
                                       const Tedgeload& aInlet) 
                 : _no(aNo), _from(aFrom), _to(aTo), _load(aInlet) {}

template<class Tnodeload, class Tedgeload>
SGraph<Tnodeload,Tedgeload>::Edge::~Edge() {}

template<class Tnodeload, class Tedgeload>
SGraph<Tnodeload,Tedgeload>::Node::Node() : _no(0), _load() {}

template<class Tnodeload, class Tedgeload>
SGraph<Tnodeload,Tedgeload>::Node::Node(unsigned int aNo, const Tnodeload& aInlet) 
                           : _no(aNo), _load(aInlet) {}

template<class Tnodeload, class Tedgeload>
SGraph<Tnodeload,Tedgeload>::Node::~Node() {}


template<class Tnodeload, class Tedgeload>
SGraph<Tnodeload,Tedgeload>::SGraph() 
                           : _noNodes(0), _noEdges(0), _nodes(0), _edges(0) {
  _nodes = new Node[aNoNodes];
}

template<class Tnodeload, class Tedgeload>
SGraph<Tnodeload,Tedgeload>::SGraph(unsigned int aNoNodes) 
                           : _noNodes(aNoNodes), _noEdges(0), _nodes(0), _edges(0) {
  _nodes = new Node[aNoNodes];
}

template<class Tnodeload, class Tedgeload>
SGraph<Tnodeload,Tedgeload>::SGraph(unsigned int aNoNodes, const edgebaselist_t& aEdgeList) 
                           : _noNodes(aNoNodes), _noEdges(0), _nodes(0), _edges(0) {
  _nodes = new Node[noNodes()];
  _noEdges = aEdgeList.size();
  _edges = new Edge[noEdges()];
  unsigned int i = 0;
  for(typename edgebaselist_t::const_iterator iter = aEdgeList.begin(); iter != aEdgeList.end(); ++iter) {
    _edges[i]._no   = i;
    _edges[i]._from = (*iter)._from;
    _edges[i]._to   = (*iter)._to;
    ++i;
  }
}

template<class Tnodeload, class Tedgeload>
SGraph<Tnodeload,Tedgeload>::SGraph(unsigned int aNoNodes, const edgelist_t& aEdgeList)
                           : _noNodes(aNoNodes), _noEdges(0), _nodes(0), _edges(0) {
  _nodes = new Node[noNodes()];
  _noEdges = aEdgeList.size();
  _edges = new Edge[noEdges()];
  unsigned int i = 0;
  for(typename edgelist_t::const_iterator iter = aEdgeList.begin(); iter != aEdgeList.end(); ++iter) {
    _edges[i]._no    = i;
    _edges[i]._from  = (*iter)->_from;
    _edges[i]._to    = (*iter)->_to;
    _edges[i]._load = (*iter)->_load;
    ++i;
  }
}



template<class Tnodeload, class Tedgeload>
SGraph<Tnodeload,Tedgeload>::~SGraph() {
  if(0 != _nodes) { delete[] _nodes; }
  if(0 != _edges) { delete[] _edges; }
}


/*
 Format: (graph (nodes [k x]+) (edges [i j y]+) )
 k: node number in [0,n-1] ordered
 x: node load
 i: node number
 j: node number != i
 y: edge load
*/


template<class Tnodeload, class Tedgeload>
SGraph<Tnodeload,Tedgeload>*
SGraph<Tnodeload,Tedgeload>::read(std::istream& is) {
  char        lChar;
  std::string lStr;
  int         lNodeNo;
  Tedgeload   lTedgeload;
  int         lNodeFrom;
  int         lNodeTo;
  Tnodeload   lTnodeload;
  int         i;

  std::list<Node> lNodeList;
  std::list<Edge> lEdgeList;
  SGraph* lQg = 0;

  is.setf(std::ios_base::skipws);

  is >> lChar;
  if('(' != lChar) { return 0; }
  is >> lStr;
  if(lStr != "graph") { return 0; }

  is >> lChar;
  if('(' != lChar) { return 0; }
  is >> lStr;
  if(lStr != "nodes") { return 0; }

  is >> lChar;
  i = 0;
  while(')' != lChar) {
    is.putback(lChar);
    is >> lNodeNo >> lTnodeload;
    if(i != lNodeNo) { return 0; }
    lNodeList.push_back(Node(lNodeNo, lTnodeload));
    ++i;
    is >> lChar;
  }
 
  is >> lChar;
  if('(' != lChar) { return 0; }
  is >> lStr;
  if(lStr != "edges") { return 0; }
  is >> lChar;
  i = 0;
  while(')' != lChar) {
    is.putback(lChar);
    is >> lNodeFrom >> lNodeTo >> lTedgeload;
    lEdgeList.push_back(Edge(i, lNodeFrom, lNodeTo, lTedgeload));
    ++i;
    is >> lChar;
  }

  is >> lChar;
  if(')' != lChar) { return 0; }

  lQg = new SGraph();
  lQg->_noNodes = lNodeList.size();
  lQg->_noEdges = lEdgeList.size();
  lQg->_nodes   = new Node[lQg->noNodes()];
  lQg->_edges   = new Edge[lQg->noEdges()];

  for(typename std::list<Node>::iterator iter = lNodeList.begin(); iter != lNodeList.end(); ++iter) {
    lQg->_nodes[(*iter).no()]._no      = (*iter).no();
    lQg->_nodes[(*iter).no()]._cardinality = (*iter).load();
  }

  for(typename std::list<Edge>::iterator iter = lEdgeList.begin(); iter != lEdgeList.end(); ++iter) {
    Edge& lEdge = (lQg->_edges[(*iter).no()]);
    lEdge._no      = (*iter).no();
    lEdge._from    = (*iter).from();
    lEdge._to      = (*iter).to();
    lEdge._load = (*iter).load();
    lQg->insert(&(lQg->_edges[(*iter).no()]));
  }
  return lQg;
}

template<class Tnodeload, class Tedgeload>
void
SGraph<Tnodeload,Tedgeload>::insert(Edge* aEdge) {
  _nodes[aEdge->from()]._edgesAll.push_back(aEdge);
  _nodes[aEdge->to()]._edgesAll.push_back(aEdge);
  if(aEdge->from() < aEdge->to()) {
    _nodes[aEdge->from()]._edgesAfter.push_back(aEdge);
    _nodes[aEdge->to()]._edgesBefore.push_back(aEdge);
  } else {
    _nodes[aEdge->from()]._edgesBefore.push_back(aEdge);
    _nodes[aEdge->to()]._edgesAfter.push_back(aEdge);
  }
}

template<class Tnodeload, class Tedgeload>
std::ostream&
SGraph<Tnodeload,Tedgeload>::write(std::ostream& os) const {
  os << "(graph (nodes ";
  for(int i = 0; i < noNodes(); ++i) {
    os << node(i).no() << ' ' << node(i).load() << ' ';
  }
  os << ")" << std::endl;
  os << "(edges " << std::endl;
  for(int i = 0; i < noEdges(); ++i) {
    os << ' ' 
       << edge(i).from() << ' ' 
       << edge(i).to() << ' ' 
       << edge(i).load() 
       << std::endl;
  }
  os << "))" << std::endl;
  return os;
}

template<class Tnodeload, class Tedgeload>
std::ostream&
SGraph<Tnodeload,Tedgeload>::print(std::ostream& os) const {
  os << "[QG: " << std::endl;
  for(int i = 0; i < noNodes(); ++i) {
    const Node& lNode = node(i);
    os << " " << lNode.no() << " (" << lNode.load() << ") :" << std::endl;
    for(edge_cit iter = lNode._edgesAll.begin(); iter != lNode._edgesAll.end(); ++iter) {
      os << "    " 
         << (*iter)->no() << ": "
         << (*iter)->from() << ' ' 
         << (*iter)->to() << ' '
         << (*iter)->load() << ' '
         << std::endl;
    }
    os << "  B ";
    for(edge_cit iter = lNode._edgesBefore.begin(); iter != lNode._edgesBefore.end(); ++iter) {
      os << ((lNode.no()  == (*iter)->from()) ?
             (*iter)->to() : (*iter)->from()) << ' ';
    }
    os << std::endl;
    os << "  A ";
    for(edge_cit iter = lNode._edgesAfter.begin(); iter != lNode._edgesAfter.end(); ++iter) {
      os << ((lNode.no()  == (*iter)->from()) ?
             (*iter)->to() : (*iter)->from()) << ' ';
    }
    os << std::endl;

  }
  os << "]" << std::endl;
  return os;
}

#endif


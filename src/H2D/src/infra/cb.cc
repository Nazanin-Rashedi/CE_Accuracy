#include "cb.hh"

namespace H2D {


Cb::Cb() : _filename(), _isHistFile(false), 
           _no_query(0), _filename_query(),
           _sds(), _ds(),_inDir(),_outDir(),_trainQDir(),_testQDir(),
           _nx(0), _ny(0), 
           _rx(5), _ry(5), 
           _fieldWidth(4), 
           _dmax(0.0),
           _delta(0),
           _kappa(0),
           _phi(0),
           _minimumNodeTotal(0),
           _kind(-1), 
           _sampleSize(-1),
           _estConst(false),
           _estArea(false),
           _regp(false),
           _eqd(false),
           _eqd2(false),
           _dct(false),
           _dctcf(false),
           _nono(false),
           _genhist(false),
           _waveletNonStd(false),
           _zhist(false),
           _vih(false),
           _hft(false),
           _phased(false),
           _hired(false),
           _mhist(false),
           _mhist2(false),
           _qts(false),
           _iqts(false),
           _flextree(false),
           _gxtree(false),
           _sampling(false),
	   _sampleAlphaOmega(false),
	   _epsAlphaOmega(0.00001),
           _gridtree(false),
           _q(2),
           _theta(-1),
           _zeta(0.0),
           _alpha(0.0),
           _precision(3),
           _eps(-1.0),
           _maxLevel(0),
           _noCoeff(0),
           _noBuckets(0),
           _budget(0),
           _leafRefinement(0),
           _lrf(0.9),
           _shrinkage(1.0),
           _logHi(0),
           _logLo(0),
           _allGridTrees(false),
           _bestGridTree(false),
           _checkError(false),
           _shortErrorCheck(false),
           _withErrors(false),
           _materialize(false),
           _lowLim(0),
           _printDot(false),
           _printRegP(false),
           _printBadQueries(),
           _print(false),
           _debug(false),
           _trace(false),
           _query(),
           _help(false),
           _xgb(false),
           _xgb_num_trees(100),
           _xgb_num_train_queries(10000),
           _exgb(false),
           _lwxgb(false),
	   _nxgb(false),
	   _indep(false),
	   _nreqd(false),
	   _dim(0)
 {}

void
Cb::no_query(const uint& x) {
  _no_query = x;
}

void
Cb::filename_query(const std::string& x) {
  _filename_query = x;
}

} // end namespace




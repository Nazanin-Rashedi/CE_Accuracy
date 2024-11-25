#include "summaryline.hh"

namespace H2D {


summaryline_t::summaryline_t() : _card(0),
                                 _noDv(0),
                                 _budget(0),
                                 _size(0),
                                 _phi(0),
                                 _theta(0),
                                 _nout(0),
                                 _constructionTime(0),
                                 _xgb_predTime(0),
                                _checkTimePerQuery(0),
                                 _max(),
                                 _avg(),
                                 _maxFixed(),
                                 _maxFixedNZ(),
                                 _cntBadFixedNZ(),
                                 _kind(H2D_NO_KIND),
                                 _subkind(0),
                                 _filename(),
                                 _hostname(),
                                 _param() {
  utsname x;
  const int lRetCode = uname(&x);
  if(0 == lRetCode) {
    _hostname = std::string(x.nodename);
  } else {
    _hostname = std::string("UNKNOWNHOST");
  }
  _cntBadFixedNZ[0] = 0;
  _cntBadFixedNZ[1] = 0;
  _cntBadFixedNZ[2] = 0;
  _cntBadFixedNZ[3] = 0;
}


summaryline_t::~summaryline_t() {
}

               
std::ostream&
summaryline_t::print(std::ostream& os, const std::string& aLineMarker) const {
  os << aLineMarker  << ' '
     << std::setw(2) << card_class() << ' '
     << std::setw(6) << _card << ' '
     << std::setw(6) << _noDv << ' '
     << std::setw(5) << _nout << ' '
     << std::setw(6) << _budget << ' '
     << std::setw(6) << _size << ' '
     << std::setw(8) << _constructionTime << ' '
     << std::setw(5) << roundX(_max[0]) << ' '  // maximal q-error
     << std::setw(5) << roundX(_max[1]) << ' '
     << std::setw(5) << roundX(_max[2]) << ' '
     << std::setw(5) << roundX(_max[3]) << ' '
     << std::setw(5) << roundX(_max[4]) << ' '
     << std::setw(5) << roundX(_maxNZ[0]) << ' '
     << std::setw(5) << roundX(_maxNZ[1]) << ' '
     << std::setw(5) << roundX(_maxNZ[2]) << ' '
     << std::setw(5) << roundX(_maxNZ[3]) << ' '
     << std::setw(5) << roundX(_maxNZ[4]) << ' '
     << std::setw(5) << roundXXX(_avg[0]) << ' '
     << std::setw(5) << roundXXX(_avg[1]) << ' '
     << std::setw(5) << roundXXX(_avg[2]) << ' '
     << std::setw(5) << roundXXX(_avg[3]) << ' '
     << std::setw(5) << roundXXX(_avg[4]) << ' '
     << std::setw(5) << roundX(_maxFixed[0]) << ' '
     << std::setw(5) << roundX(_maxFixed[1]) << ' '
     << std::setw(5) << roundX(_maxFixed[2]) << ' '
     << std::setw(5) << roundX(_maxFixed[3]) << ' '
     << std::setw(5) << roundX(_maxFixedNZ[0]) << ' '
     << std::setw(5) << roundX(_maxFixedNZ[1]) << ' '
     << std::setw(5) << roundX(_maxFixedNZ[2]) << ' '
     << std::setw(5) << roundX(_maxFixedNZ[3]) << ' '
     << std::setw(5) << roundX(_cntBadFixedNZ[0]) << ' '
     << std::setw(5) << roundX(_cntBadFixedNZ[1]) << ' '
     << std::setw(5) << roundX(_cntBadFixedNZ[2]) << ' '
     << std::setw(5) << roundX(_cntBadFixedNZ[3]) << ' '
     << std::setw(5) << h2d_kind_name(_kind) << ' '
     << std::setw(2) << _kind << ' '
     << std::setw(2) << _subkind << ' '
     << std::setw(4) << _phi     << ' '
     << std::setw(4) << _theta   << ' '
     << std::setw(8) << _checkTimePerQuery << ' '
     << std::setw(8) << _xgb_predTime << ' '
    
     << _filename << ' ';
  double lNoTiles = 0;
  switch(_kind) {
    case H2D_EST_CONST:
    case H2D_EST_AREA:
         break;
    case H2D_RegPart:    
        os << ' ' << _param._regp._nx << ' ' << _param._regp._ny << ' ' << _param._regp._maxfreq;
        break;
    case H2D_EquiDepth: 
    case H2D_EquiDepth2: 
        os << ' ' << _param._eqdh._nx << ' ' << _param._eqdh._ny;
        // if(1 == _subkind) {
          os << ' ' << _param._eqdh._rx << ' ' << _param._eqdh._ry;
        // }
        break;
    case H2D_Sampling:
        os << ' ' << _param._sampling._noSamples;
        break;
    case H2D_GridTreeFixed:
    case H2D_GridTreeAuto:
    case H2D_GridTreeBest:
        lNoTiles = (double) (_param._gt._nx * _param._gt._ny);
        os << std::setw(3) << _param._gt._delta << ' '
           << std::setw(2) << _param._gt._kappa << ' '
           << std::setw(4) << _param._gt._noZ << ' '
           << std::setw(4) << roundX(((double) _param._gt._noZ / lNoTiles) * (double) 100.0) << ' '
           << std::setw(4) << _param._gt._noS   << ' '
           << std::setw(4) << _param._gt._noM   << ' '
           << std::setw(4) << _param._gt._noL   << ' '
           << std::setw(2) << _param._gt._nx << ' '
           << std::setw(2) << _param._gt._ny << ' '
           << std::setw(2) << _param._gt._depth << ' '
           << std::setw(2) << _param._gt._noNodes << ' '
           ;
        break;
    case H2D_DiscreteCosineTransform:
    case H2D_DiscreteCosineTransformOnCumFreq:
         os << ' ' << _param._dct._nx 
            << ' ' << _param._dct._ny 
            << ' ' << _param._dct._noCoeff 
            << ' ' << _param._dct._zeta;
         break;
    case H2D_NONOTREE:
         os << ' ' << _param._nono._nx
            << ' ' << _param._nono._ny
            << ' ' << _param._nono._kappa
            << ' ' << _param._nono._shrinkage
            << ' ' << _param._nono._depth
            << ' ' << _param._nono._noNodes  
            ;
         break;
    case H2D_GENHIST:
         os << ' ' << _param._genhist._nx 
            << ' ' << _param._genhist._ny
            << ' ' << _param._genhist._alpha
            << ' ' << _param._genhist._delta 
            << ' ' << _param._genhist._q
            << ' ' << _param._genhist._noBuckets;
         break;
    case H2D_WAVELET_NONSTD:
         os << ' ' << _param._waveletnonstd._n
            << ' ' << _param._waveletnonstd._noCoeff
            << ' ' << _param._waveletnonstd._alpha;
         break;
    case H2D_HFT:
         os << ' ' << _param._hft._logHi
            << ' ' << _param._hft._logLo
            << ' ' << _param._hft._kappa;
         break;
    case H2D_PHASED:
         os << ' ' << _param._phased._nx << ' ' << _param._phased._ny
            << ' ' << _param._phased._rx << ' ' << _param._phased._ry;
         break;
    case H2D_HIRED:
         os << ' ' << _param._hired._eps << ' ' << _param._hired._maxLevel << ' ' 
                   << _param._hired._noNodes << ' ' << _param._hired._depth;
         break;
    case H2D_MHIST:
    case H2D_MHIST2:
         os << ' ' << _param._mhist._noBuckets;
         break;
    case H2D_QTS:
         os << ' ' << _param._qts._depth 
            << ' ' << _param._qts._noNodes;
         break;
    case H2D_IQTS:
         os << ' ' << _param._iqts._depth 
            << ' ' << _param._iqts._noNodes
            << ' ' << _param._iqts._noNotRefined << ' ' << _param._iqts._noLT23
            << ' ' << _param._iqts._noLT24 << ' ' << _param._iqts._noLT2p;
         break;
    case H2D_FLEXTREE:
         os << ' ' << _param._flextree._precisionLevel
            << ' ' << _param._flextree._depth 
            << ' ' << _param._flextree._noNodes
            << ' ' << _param._flextree._noGNodes
            << ' ' << _param._flextree._minSplit
            << ' ' << _param._flextree._maxUnsplit;
         break;
    case H2D_GXTREE:
         os << ' ' << (char) ('A' + (_param._gxtree._leafRefinement))
            << ' ' << _param._gxtree._lrf
            << ' ' << _param._gxtree._minimumNodeTotal
            << ' ' << _param._gxtree._depth
            << ' ' << _param._gxtree._noNodes
            << ' ' << _param._gxtree._noGNodes
            << ' ' << _param._gxtree._minSplit
            << ' ' << _param._gxtree._maxUnsplit
            << ' ' << _param._gxtree._encoded;
         break;
    case H2D_ZHist:
         os << ' ' << _param._zhist._n
            << ' ' << _param._zhist._nxy
            << ' ' << _param._zhist._noBuckets;
         break;
    case H2D_VIHIST:
    default:        
        os        << "ERROR: UNKNOWN HISTOGRAM_KIND";
        std::cout << "ERROR: UNKNOWN HISTOGRAM_KIND: " << _kind << std::endl;
        std::cerr << "ERROR: UNKNOWN HISTOGRAM_KIND: " << _kind << std::endl;
  };
  os << ' ' << _hostname << std::endl;
  return os;
}

bool
summaryline_t::read(std::istream& aIs) {
  char c = 0;
  aIs >> c;
  if('@' != c) {
    return false;
  }
  uint u = 0; // cardinality class, ignore
  double d = 0;
  aIs >> u >> _card >> _noDv >> _nout >> _budget >> _size >> _constructionTime;
  if(aIs.eof()) { return false; }
  for(uint i = 0; i < 5; ++i) {
    aIs >> _max[i];
  }
  for(uint i = 0; i < 5; ++i) {
    aIs >> _maxNZ[i];
  }
  for(uint i = 0; i < 5; ++i) {
    aIs >> _avg[i];
  }
  for(uint i = 0; i < 4; ++i) {
    aIs >> _maxFixed[i];
  }
  for(uint i = 0; i < 4; ++i) {
    aIs >> _maxFixedNZ[i];
  }
  for(uint i = 0; i < 4; ++i) {
    aIs >> _cntBadFixedNZ[i];
  }
  std::string lEstName; // ignore
  int         lKind = 0;
  aIs >> lEstName >> lKind >> _subkind >> _phi >> _theta >> _checkTimePerQuery >> _filename;
  _kind = (H2D_kind_t) lKind;
  switch(_kind) {
    case H2D_EST_CONST:
         break;
    case H2D_EST_AREA:
         break;
    case H2D_RegPart:
        aIs >> _param._regp._nx 
            >> _param._regp._ny 
            >> _param._regp._maxfreq;
        break;
    case H2D_EquiDepth:
    case H2D_EquiDepth2:
        aIs >> _param._eqdh._nx 
            >> _param._eqdh._ny
            >> _param._eqdh._rx 
            >> _param._eqdh._ry;
        break;
    case H2D_Sampling:
        aIs >> _param._sampling._noSamples;
        break;
    case H2D_GridTreeFixed:
    case H2D_GridTreeAuto:
    case H2D_GridTreeBest:
        aIs >> _param._gt._delta
            >> _param._gt._kappa
            >> _param._gt._noZ
            >> d
            >> _param._gt._noS
            >> _param._gt._noM
            >> _param._gt._noL
            >> _param._gt._nx
            >> _param._gt._ny
            >> _param._gt._depth
            >> _param._gt._noNodes
           ;
        break;
    case H2D_DiscreteCosineTransform:
    case H2D_DiscreteCosineTransformOnCumFreq:
         aIs >> _param._dct._nx
             >> _param._dct._ny
             >> _param._dct._noCoeff
             >> _param._dct._zeta;
         break;
    case H2D_NONOTREE:
         aIs >> _param._nono._nx
             >> _param._nono._ny
             >> _param._nono._kappa
             >> _param._nono._shrinkage
             >> _param._nono._depth
             >> _param._nono._noNodes
            ;
         break;
    case H2D_GENHIST:
         aIs >> _param._genhist._nx
             >> _param._genhist._ny
             >> _param._genhist._alpha
             >> _param._genhist._delta
             >> _param._genhist._q
             >> _param._genhist._noBuckets;
         break;
    case H2D_WAVELET_NONSTD:
         aIs >> _param._waveletnonstd._n
             >> _param._waveletnonstd._noCoeff
             >> _param._waveletnonstd._alpha;
         break;
    case H2D_HFT:
         aIs >> _param._hft._logHi
             >> _param._hft._logLo
             >> _param._hft._kappa;
         break;
    case H2D_PHASED:
         aIs >> _param._phased._nx
             >> _param._phased._ny
             >> _param._phased._rx
             >> _param._phased._ry;
         break;
    case H2D_HIRED:
         aIs >> _param._hired._eps
             >> _param._hired._maxLevel
             >> _param._hired._noNodes
             >> _param._hired._depth;
         break;
    case H2D_MHIST:
    case H2D_MHIST2:
         aIs >> _param._mhist._noBuckets;
         break;
    case H2D_QTS:
         aIs >> _param._qts._depth
             >> _param._qts._noNodes;
         break;
    case H2D_IQTS:
         aIs >> _param._iqts._depth
             >> _param._iqts._noNodes
             >> _param._iqts._noNotRefined 
             >> _param._iqts._noLT23
             >> _param._iqts._noLT24 
             >> _param._iqts._noLT2p;
         break;
    case H2D_FLEXTREE:
         aIs >> _param._flextree._precisionLevel
             >> _param._flextree._depth
             >> _param._flextree._noNodes
             >> _param._flextree._noGNodes
             >> _param._flextree._minSplit
             >> _param._flextree._maxUnsplit;
         break;
    case H2D_GXTREE:
         aIs >> c;
         _param._gxtree._leafRefinement = (c - 'A');
         aIs >> _param._gxtree._lrf
             >> _param._gxtree._minimumNodeTotal
             >> _param._gxtree._depth
             >> _param._gxtree._noNodes
             >> _param._gxtree._noGNodes
             >> _param._gxtree._minSplit
             >> _param._gxtree._maxUnsplit
             >> _param._gxtree._encoded;
         break;
    case H2D_ZHist:
         aIs >> _param._zhist._n
             >> _param._zhist._nxy
             >> _param._zhist._noBuckets;
         break;
    case H2D_VIHIST:
    default:
        std::cout << "ERROR: UNKNOWN HISTOGRAM_KIND: " << _kind << std::endl;
        std::cerr << "ERROR: UNKNOWN HISTOGRAM_KIND: " << _kind << std::endl;
        return false;
  };
  aIs >>  _hostname;
  return (!aIs.eof());
}



std::ostream& 
operator<<(std::ostream& os, const summaryline_t& x) {
  return x.print(os, "@");
}


} // end namespace




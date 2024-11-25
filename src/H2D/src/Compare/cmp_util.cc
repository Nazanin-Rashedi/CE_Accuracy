#include "cmp_util.hh"


//const std::string gExpDirName      = "/home/rashedi/H2D/out";
//const std::string gRelDirName      = "/global/rashedi/H2D/rel";
const std::string gDsDescFileName  = "ds_stats.rel";
const std::string gRelFileRaw      = "res_raw.rel";
const std::string gRelFileAggr     = "res_aggr.rel";


const string_vt gSDS = {
  "earthquake",
  "OpenStreetMap",
  "tiger"
};

const string_vt gSynopsis = {
  "eqd",
  "est-area",
  "gxt",
  "iqts_3", 
  "qts_0",
  "mhist2_4", 
  "qts_2",
  "regp", 
  "sampling", 
  "exgb3003",
 // "nreqd",
 // "SAO",
  //"SAO_0",
  "eboxgb1616",
  //"xgb",
  //"exgb",
  //"nxgb",
  "postgredef",
  "postgres",
  "postgis",
  "deepDBBudget",
  "mscn"
 // "xgb.rev",
 // "exgb.rev",
//  "eboxgb.rev",
 // "nxgb.rev",
 // "mscn"
  
};

uint
get_no(const std::string& s, const string_vt& S) {
  for(uint i = 0; i < S.size(); ++i) {
    if(s == S[i]) {
      return i;
    }
  }
  return -1;
} 

char
qerr_class_char(const uint aQErrClass) {
  char lRes = 'X';
  if(10 > aQErrClass) {
    lRes = '0' + aQErrClass;
  } else 
  if(32 > aQErrClass) {
    lRes = 'A' + (aQErrClass - 10);
  }
  return lRes;
}

/*
 * partition filename into its components
 * return codes:
 * +1: the filename does not match *.out
 * -1: the filename does end with *.out but is not partionable (bad error)
 *  0: ok
 */
int 
fn_partition(fn_t& aFn, const std::string& aFilename) {
  const size_t lSuff = aFilename.rfind(".out");
  if((aFilename.size() - 4) != lSuff) {
  //  std::cout << "Error: bad suffix: " << aFilename.size() << " " << aFilename << std::endl;
    return +1;
  }
  const char* x = aFilename.c_str();
  const char* b = x;
  const char* e = 0;
  const char* n = 0;
        char* p = 0;
  aFn._no = strtol(b, &p, 10);
  e = p;
  if(e == b) { return -1; }
  if('_' != *e) { return -1; }

  ++e;
  b = e;
  while('_' != *e) { ++e; }
  aFn._sds = std::string(b, e);// e is _ before ds
//  std::cout<<aFn._sds<<std::endl;
  if('_' != *e) { return -1; }
  ++e;
  b = e; // b is start of ds
  
  /* Guido's Original Code
  e = aFilename.c_str() + (aFilename.size() - 4);
  x = e;
  while('_' != *x) --x;
  ++x;
  aFn._syn = std::string(x, e);
  e = x - 1;
  aFn._ds = std::string(b, e);
  */

  //NR changed the code to cover varients
  e = x + (aFilename.size() - 3);
  while('_' != *e) { --e; }  //last _
  ++e;
  if(std::isdigit(*e)) { 
  //std::cout<<*e<<std::endl;	  
  e-=2;
  while('_'!= *e) { --e;}// e is _ before syn

  ++e;
  n = e; // p is start of syn
  aFn._syn = std::string(n,x+aFilename.size()-4);
  aFn._ds = std::string(b, n-1);
  }
  else{ 
  while('_'!=*e) {--e;} //e is _ before syn
  aFn._syn = std::string(e+1,x+aFilename.size()-4);
  aFn._ds = std::string(b,e);
  
  }

  //std::cout<<aFn._ds<<std::endl;
  //std::cout<<aFn._syn<<std::endl;
 // aFn._ds = std::string(b, e);
 // ++e;
 // b = e; //
  
  //while('.' != *e) { ++e; }
  //
  //aFn._syn = std::string(b, e);
  //if( *e != '.'){
  //++e;
  //b = e;
  //aFn._var = std::string(b,b+1);
 // }
  //else{
  //aFn._var = " ";
  //}
  /*++e;
  b = e;
  aFn._var = std::string(b,b+1);
  */
  return 0;
}
/*
 *  eat non-digit characters from a given input stream
 */

void
eat_non_digit(std::istream& aIs) {
 aIs >> std::ws;
 char c = aIs.peek();
 if(std::isdigit(c)) {
   return;
 }
 aIs >> c;
 aIs >> std::ws; 
} 

/*  
 *  read one experiment result file in Nazanin-Format
 *  and store it in a vector (size = gNoTheta) of arrays
 *  of size gNoSelClass X gNoQErrClass
 */ 

bool
read_exp_res_file(array_vt& aArrVec, const std::string& aFilename, const bool aTrace) {
  aArrVec.resize(gNoTheta);
  for(uint k = 0; k < gNoTheta; ++k) {
    aArrVec[k].resize(gNoSelClass,gNoQErrClass); // <== needs to be changed if there are more qerror classes
    aArrVec[k].mem_init();
  }
  std::ifstream lIs(aFilename);
  if(!lIs) {
    std::cout << "Error: could not open file '" << aFilename << "'." << std::endl;
    return false;
  }
  
  // char c = 0;
  for(uint k = 0; k < gNoTheta; ++k) {
    for(uint i = 0; i < gNoSelClass; ++i) {
      for(uint j = 0; j < gNoQErrClass; ++j) {
        #ifdef PARSE_WITH_CHECK
        if(0 < j) {
          lIs >> c;
          if(',' != c) {
            std::cout << "Error: ',' expected: " << k << ',' << i << ',' << j << std::endl;
            return false;
          }
        }
        #else
        eat_non_digit(lIs);
        #endif
        lIs >> aArrVec[k](i,j);
      }
    }
    if(aTrace) {
      std::cout << "Matrix " << k << std::endl;
      aArrVec[k].print(std::cout, " ", "", 6) << std::endl;
    }
  }
  return true;
}



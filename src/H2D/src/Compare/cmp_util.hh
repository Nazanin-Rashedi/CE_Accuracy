#ifndef H2D_COMPARE_CMP_UTIL_HH
#define H2D_COMPARE_CMP_UTIL_HH
#pragma once

#include "infra/glob_infra_standard_includes.hh"
#include "infra/array_tt.hh"
#include "infra/linereader.hh"
extern "C" {
  #include "infra/cmeasure.h"
}

#include "H2D/src/infra/log_classes.hh"

#include <set>
#include <fstream>
#include <filesystem>

using string_vt = std::vector<std::string>;
using array_t   = array_tt<uint64_t>;
using array_vt  = std::vector<array_t>;

//extern const std::string gExpDirName;      // top directory for experimental results
extern const std::string gDsDescFileName;  // DS statistics file
//extern const std::string gRelDirName;      // result directory for relations produced
extern const std::string gRelFileRaw;      // filename for file containing raw (unaggregated) data
extern const std::string gRelFileAggr;     // filename for aggregated data (group by cc, sc)

constexpr uint gNoCardClass = 30;
constexpr uint gNoSelClass  = 28;
constexpr uint gNoQErrClass = 33;
constexpr uint gNoTheta     =  6;

extern const string_vt gSDS;
extern const string_vt gSynopsis;


// get index of string in vector
uint get_no(const std::string& s, const string_vt& S);

// convert integer query class to character: hex and if > 15: 'X'
char qerr_class_char(const uint aQErrClass);

/*
 * struct fn_t
 * partitioned file name
 */

struct fn_t {
  uint        _no;
  std::string _sds;
  std::string _ds;
  std::string _syn;
  std::string _var;
  fn_t() : _no(0), _sds(), _ds(), _syn(), _var() {}
  inline int no() const { return _no; }
  inline const std::string& sds() const { return _sds; }
  inline const std::string& ds()  const { return _ds;  }
  inline const std::string& syn() const { return _syn; }
  inline const std::string& var() const { return _var; }
};

int fn_partition(fn_t& aFn, const std::string& aFilename);

/*
 *  eat non-digit characters from a given input stream
 */

void eat_non_digit(std::istream& aIs);

/*  
 *  read one experiment result file in Nazanin-Format
 *  and store it in a vector (size = gNoTheta) of arrays
 *  of size gNoSelClass X gNoQErrClass
 */ 

bool read_exp_res_file(array_vt& aArrVec, const std::string& aFilename, const bool aTrace);


#endif

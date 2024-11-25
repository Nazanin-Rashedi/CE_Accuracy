#include <iostream>
#include <iomanip>
#include <fstream>
  
#include <string.h>
#include <string> 
#include <vector>
#include <filesystem>
    
#include "infra/argbase.hh"
#include "infra/aggregate.hh"
#include "infra/matrix.hh"
#include "infra/measure.hh"
#include "infra/tmath.hh"
#include "infra/types.hh"
#include "infra/cb.hh"
#include "infra/util.hh"
#include "infra/data2dim.hh"
#include "infra/summaryline.hh"
#include "infra/RegularPartitioning2dim.hh"
#include "EstimatorArea/EstimatorArea.hh"
#include "RegP/RegPEstimator.hh"
#include "RegP/HFTEstimator.hh"
#include "Sampling/Sample2dim.hh"
#include "EquiDepthHist/EqDepHist.hh"
#include "MHist2/MHist2.hh"
#include "QTS/QTS.hh"
#include "QTS/IQTS.hh"
#include "GxTree/GxTree.hh"
#include "CheckError/EstimateEvaluator.hh"
#include "XGBoost/XGBEstimator.hh"
#include "EXGB/EXGB.hh"
#include "LWXGB/LWXGB.hh"
#include "OneDEqDepHist/OneDEqDepHist.hh"
#include <random>


namespace H2D {

class ProcessQueryFile {
  public:
    ProcessQueryFile(const std::string& aDirIn,
                     const std::string& aDirOut,
                           Cb&          aCb);
  public:
    bool run(const Cb& aCb);
  private:
    bool init(const std::string& aDirIn,
              const std::string& aDirOut,
                    Cb&          aCb);
    bool run_all_estimator(      summaryline_t& aSummaryline,
                           const Cb&            aCb);
    bool run_one_estimator(      summaryline_t& aSummaryline,
                           const H2D_kind_t     aEstKind, 
                           const Cb&            aCb);
    bool fin(const Cb& aCb);
  public:
    inline const std::string& filebase() const { return _filebase; }
    inline const std::string& dir_in() const { return _dir_in; }

    inline const Data2dim&    data()     const { return _data; }
    inline const query_vt&    query()    const { return _query; }
    inline const query_vt&    trainQuery() const {return _trainQuery;}
    inline double construction_time_s() { return _construction_time_s; }
    inline double avg_query_time_us() { return _avg_query_time_us; }
    inline bool   trace() const { return _trace; }
  private:
    bool read_data_file(const std::string&  aFilename);
    bool read_query_file(const std::string& aFilename);
    bool read_train_query_file(const std::string& aFilename);
    int generate_train_query(std::ostream& os, const H2D::Data2dim& aData, const H2D::Cb& aCb, uint no_query);
    int generate_train_query_2000(const H2D::Data2dim& aData, const H2D::Cb& aCb, uint no_query);
    EstimatorBase2dim* new_estimator(      summaryline_t& aSummaryline,
                                     const H2D_kind_t     aEstKind,
                                     const Cb&            aCb);
  private:
    std::string       _dir_in;
    std::string       _dir_out;
    std::string       _filebase; // complete filename without suffix .hist or .qu
    Data2dim          _data;     // the contents of the data file (histogram format)
    query_vt          _query;    // the contents of the query file (all queries to be tested)
    EstimateEvaluator _esteval;
    double            _construction_time_s;
    double            _avg_query_time_us;
    bool              _trace;
    query_vt          _trainQuery;
friend class XGBEstimator;
};



} // end namespace

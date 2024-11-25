#include "DsDesc.hh"
#include "ExpResAggr.hh"
#include <iostream>
#include <string>

// Declare the paths as global variables

void
test2(const bool aGenRelRaw, const bool aGenRelAggr, const bool aRunCompare, const std::string& gExpDirName, const std::string& gRelDirName) {

  // read file containing statistcs for each DS (data set)
  DsDesc lDsDesc;
  if(!lDsDesc.read(gRelDirName + '/' + gDsDescFileName)) {
    std::cout << "Error while reading file '" << gDsDescFileName << "'." << std::endl;
  }

  std::cout<<gDsDescFileName<<std::endl;
  const uint lMinCC = lDsDesc.min_card_class();
  const uint lMaxCC = lDsDesc.max_card_class();

  std::cout << "  min card class: " << lMinCC << std::endl;
  std::cout << "  max card class: " << lMaxCC << std::endl;

  // init result aggregator/accumulator
  exp_res_aggr_vt lExpAggrVec(gSynopsis.size()); // main thing to fill: accumulate all result files
  for(uint i = 0; i < lExpAggrVec.size(); ++i) {
    lExpAggrVec[i].init(i, lMinCC, lMaxCC);
  }

  std::ofstream lOsRaw;
  if(aGenRelRaw) {
    const std::string lFilenameRelRaw = gRelDirName + '/' + gRelFileRaw;
    lOsRaw.open(lFilenameRelRaw);
    if(!lOsRaw) {
       std::cout << "can't open '" << lFilenameRelRaw << "'." << std::endl;
       return;
    }
    lOsRaw << "# table H2dRaw { "
           << "string sds, string ds, int cc, int sc, "
           << "string syn, int syn_id, "
           << "int theta, int qc, char qcc, int cnt"
           << "};" << std::endl;
  }

  uint lCountFile = 0;
  uint lCountGood = 0;
  std::filesystem::path lResDir(gExpDirName);
  array_vt lArrVec;
  cmeasure_t lMeas;
  cmeasure_start(&lMeas);
  using uint_st = std::set<uint>;
  uint_st lBadSynopsis; // to minimize error messages
  for(uint i = 0; i < gSDS.size(); ++i) {
    std::cout << "SDS = " << gSDS[i] << std::endl;
    std::filesystem::path lSdsDir = lResDir / gSDS[i];
    std::cout << "  dir = " << lSdsDir << std::endl;
    if(!std::filesystem::is_directory(lSdsDir)) {
      std::cout << "  sds-dir is not a directory" << std::endl;
      return;
    }
    for(auto const& lFile : std::filesystem::directory_iterator(lSdsDir)) {
      if(!lFile.is_regular_file()) {
        std::cout << "    file = " << lFile << std::endl;
        std::cout << "    ^not a regular file" << std::endl;
      }
      fn_t lFn;
      const int lRc = fn_partition(lFn, lFile.path().filename());
      if(+1 == lRc) {
        //std::cout << "    file = " << lFile << std::endl;
	//std::cout << "    ^no real output file" << std::endl;
      } else
      if(-1 == lRc) {
       // std::cout << "    file = " << lFile << std::endl;
        //std::cout << "    ^skip" << std::endl;
      } else {
        ++lCountFile;
        // std::cout << "    ^real result data file" << std::endl;
        if("mhist" == lFn.syn()) { continue; }

        const uint lSynNo = get_no(lFn.syn(), gSynopsis);
        if(lSynNo >= gSynopsis.size()) {
          if(lBadSynopsis.end() == lBadSynopsis.find(lSynNo)) {
            std::cout << "    WARNING: BAD synopsis: '" << lFn.syn() << "'." << std::endl;
            lBadSynopsis.insert(lSynNo);
          }
          continue;
        }
        assert(lSynNo < lExpAggrVec.size());

        if(!read_exp_res_file(lArrVec, lFile.path(), false)) {
          std::cout << "    ERROR: file = '" << lFile << "' not readable" << std::endl;
          continue;
        }

        const DsDesc::tuple_t* lTuple = lDsDesc.find(lFn.sds(), lFn.ds());
        if(nullptr == lTuple) {
          std::cout << "    file = " << lFile << std::endl;
          std::cout << "    sds = " << lFn.sds() << ", ds = " << lFn.ds() << std::endl;
        }
        assert(nullptr != lTuple);

        // insert into lExpAggrVec
        const uint lCardClass = lTuple->card_class();
        for(uint lTheta = 0; lTheta < gNoTheta; ++lTheta) {
          lExpAggrVec[lSynNo].insert(lTheta, lCardClass, lArrVec[i]);
        }
        if(aGenRelRaw) {
          // write to raw output file
          for(uint lTheta = 0; lTheta < gNoTheta; ++lTheta) {
            for(uint i = 0; i < gNoSelClass; ++i) {
              for(uint j = 0; j < gNoQErrClass; ++j) {
                const uint64_t lCnt = lArrVec[lTheta](i,j);
                if(0 < lCnt) {
                  lOsRaw << lTuple->sds() << '|'
                         << lTuple->ds()  << '|'
                         << lTuple->card_class() << '|'
                         << i << '|'
                         << gSynopsis[lSynNo] << '|'
                         << lSynNo << '|'
			// << lFn.var() << '|'
                         << lTheta << '|'
                         << j << '|'
                         << qerr_class_char(j) << '|'
                         << lCnt
                         << std::endl;
                }
              }
            }
          }
        }
        ++lCountGood;
      }
    }
  }
  cmeasure_stop(&lMeas);

  std::cout << "#files = " << lCountFile << std::endl;
  std::cout << "#good  = " << lCountGood << std::endl;
  std::cout << "#runtime = " << cmeasure_total_s(&lMeas) << " [s]" << std::endl;

  // print aggregated result
  
  for(uint i = 0; i < lExpAggrVec.size(); ++i) {
    lExpAggrVec[i].print(std::cout) << std::endl;
  }

  // write aggregated results flattened to file
  if(aGenRelAggr) {
    const std::string lFilenameRelAggr = gRelDirName + '/' + gRelFileAggr;
    std::ofstream lOsAggr(lFilenameRelAggr);
    if(!lOsAggr) {
      std::cout << "Can't open file '" << lFilenameRelAggr << "'." << std::endl;
      return;
    }
    lOsAggr << "# table R { string syn, int cc, int sc, int theta, int qc, char qcc, int cnt };" << std::endl; 
    for(uint i = 0; i < lExpAggrVec.size(); ++i) {
      if("mhist" == gSynopsis[lExpAggrVec[i].syn_no()]) { continue; }
      lExpAggrVec[i].print_rel(lOsAggr);
    }
  }

  if(!aRunCompare) { return; }

  // produce synopsis profiles
  using synprof_vt = std::vector<SynopsisProfile>;
  synprof_vt lSynProf(gSynopsis.size());
  for(uint i = 0; i < gSynopsis.size(); ++i) {
    lExpAggrVec[i].get_profile_max(lSynProf[i]);
  }
  for(uint i = 0; i < gSynopsis.size(); ++i) {
    lSynProf[i].print(std::cout);
  }

  // compare synopsis profiles
  std::cout << "<=>" << std::endl;
  for(uint i = 0; i < gSynopsis.size(); ++i) {
    if("mhist" == gSynopsis[i]) { continue; }
    for(uint j = i + 1; j < gSynopsis.size(); ++j) {
      if("mhist" == gSynopsis[j]) { continue; }
      if(lSynProf[i] <= lSynProf[j]) {
        std::cout << gSynopsis[i] << ' ';
        std::cout << "<=";
        std::cout << ' ' << gSynopsis[j] << std::endl;
      } else
      if(lSynProf[j] <= lSynProf[i]) {
        std::cout << gSynopsis[j] << ' ';
        std::cout << "<=";
        std::cout << ' ' << gSynopsis[i] << std::endl;
      } else {
        // std::cout << gSynopsis[i] << ' ';
        // std::cout << "<>";
        // std::cout << ' ' << gSynopsis[j] << std::endl;
      }
      
    }
  }

  std::cout << "<>" << std::endl;
  for(uint i = 0; i < gSynopsis.size(); ++i) {
    if("mhist" == gSynopsis[i]) { continue; }
    for(uint j = i + 1; j < gSynopsis.size(); ++j) {
      if("mhist" == gSynopsis[j]) { continue; }
      if(lSynProf[i] < lSynProf[j]) {
        std::cout << gSynopsis[i] << ' ';
        std::cout << "<";
        std::cout << ' ' << gSynopsis[j] << std::endl;
      } else
      if(lSynProf[j] < lSynProf[i]) {
        std::cout << gSynopsis[j] << ' ';
        std::cout << "<";
        std::cout << ' ' << gSynopsis[i] << std::endl;
      } else {
        // std::cout << gSynopsis[i] << ' ';
        // std::cout << "<>";
        // std::cout << ' ' << gSynopsis[j] << std::endl;
      }
    }
  }

  std::cout << "....compare_n_theta0...." << std::endl;
  for(uint i = 0; i < gSynopsis.size(); ++i) {
    if("mhist" == gSynopsis[i]) { continue; } 
    for(uint j = i + 1; j < gSynopsis.size(); ++j) {
      if("mhist" == gSynopsis[j]) { continue; }
      auto [lRes1, bCnt1, lbCnt1, lMx1] = lSynProf[i].compare_n(lSynProf[j],0);
      auto [lRes2, bCnt2, lbCnt2, lMx2] = lSynProf[j].compare_n(lSynProf[i],0);
      std::cout << std::setw(8) << gSynopsis[i] 
                << " cmp_t0 " 
                << std::setw(8) << gSynopsis[j] << ": "
                << (lRes1 ? "yes" : "no ") << ' '
                << std::setw(4) << bCnt1 << ' '
	        << std::setw(4) << lbCnt1 << ' '
		
                << std::setw(2) << lMx1 << " ; "
                << (lRes2 ? "yes" : "no ") << ' '
                << std::setw(4) << bCnt2 << ' '
		<< std::setw(4) << lbCnt2 << ' '
                << std::setw(2) << lMx2
                << std::endl;
    }
  }

  std::cout << "....compare_n_theta1...." << std::endl;
  for(uint i = 0; i < gSynopsis.size(); ++i) {
    if("mhist" == gSynopsis[i]) { continue; } 
    for(uint j = i + 1; j < gSynopsis.size(); ++j) {
      if("mhist" == gSynopsis[j]) { continue; }
      auto [lRes1, bCnt1, lbCnt1, lMx1] = lSynProf[i].compare_n(lSynProf[j],1);
      auto [lRes2, bCnt2, lbCnt2, lMx2] = lSynProf[j].compare_n(lSynProf[i],1);
      std::cout << std::setw(8) << gSynopsis[i] 
                << " cmp_t1 " 
                << std::setw(8) << gSynopsis[j] << ": "
                << (lRes1 ? "yes" : "no ") << ' '
                << std::setw(4) << bCnt1 << ' '
	        << std::setw(4) << lbCnt1 << ' '
		
                << std::setw(2) << lMx1 << " ; "
                << (lRes2 ? "yes" : "no ") << ' '
                << std::setw(4) << bCnt2 << ' '
		<< std::setw(4) << lbCnt2 << ' '
                << std::setw(2) << lMx2
                << std::endl;
    }
  }





  std::cout << "....compare_n_theta2...." << std::endl;
  for(uint i = 0; i < gSynopsis.size(); ++i) {
    if("mhist" == gSynopsis[i]) { continue; } 
    for(uint j = i + 1; j < gSynopsis.size(); ++j) {
      if("mhist" == gSynopsis[j]) { continue; }
      auto [lRes1, bCnt1, lbCnt1, lMx1] = lSynProf[i].compare_n(lSynProf[j],2);
      auto [lRes2, bCnt2, lbCnt2, lMx2] = lSynProf[j].compare_n(lSynProf[i],2);
      std::cout << std::setw(8) << gSynopsis[i] 
                << " cmp_t2 " 
                << std::setw(8) << gSynopsis[j] << ": "
                << (lRes1 ? "yes" : "no ") << ' '
                << std::setw(4) << bCnt1 << ' '
	        << std::setw(4) << lbCnt1 << ' '
		
                << std::setw(2) << lMx1 << " ; "
                << (lRes2 ? "yes" : "no ") << ' '
                << std::setw(4) << bCnt2 << ' '
		<< std::setw(4) << lbCnt2 << ' '
                << std::setw(2) << lMx2
                << std::endl;
    }
  }




  std::cout << "....compare_n_theta3...." << std::endl;
  for(uint i = 0; i < gSynopsis.size(); ++i) {
    if("mhist" == gSynopsis[i]) { continue; } 
    for(uint j = i + 1; j < gSynopsis.size(); ++j) {
      if("mhist" == gSynopsis[j]) { continue; }
      auto [lRes1, bCnt1, lbCnt1, lMx1] = lSynProf[i].compare_n(lSynProf[j],3);
      auto [lRes2, bCnt2, lbCnt2, lMx2] = lSynProf[j].compare_n(lSynProf[i],3);
      std::cout << std::setw(8) << gSynopsis[i] 
                << " cmp_t3 " 
                << std::setw(8) << gSynopsis[j] << ": "
                << (lRes1 ? "yes" : "no ") << ' '
                << std::setw(4) << bCnt1 << ' '
	        << std::setw(4) << lbCnt1 << ' '
		
                << std::setw(2) << lMx1 << " ; "
                << (lRes2 ? "yes" : "no ") << ' '
                << std::setw(4) << bCnt2 << ' '
		<< std::setw(4) << lbCnt2 << ' '
                << std::setw(2) << lMx2
                << std::endl;
    }
  }



  std::cout << "....compare_n_theta4...." << std::endl;
  for(uint i = 0; i < gSynopsis.size(); ++i) {
    if("mhist" == gSynopsis[i]) { continue; } 
    for(uint j = i + 1; j < gSynopsis.size(); ++j) {
      if("mhist" == gSynopsis[j]) { continue; }
      auto [lRes1, bCnt1, lbCnt1, lMx1] = lSynProf[i].compare_n(lSynProf[j],4);
      auto [lRes2, bCnt2, lbCnt2, lMx2] = lSynProf[j].compare_n(lSynProf[i],4);
      std::cout << std::setw(8) << gSynopsis[i] 
                << " cmp_t4 " 
                << std::setw(8) << gSynopsis[j] << ": "
                << (lRes1 ? "yes" : "no ") << ' '
                << std::setw(4) << bCnt1 << ' '
	        << std::setw(4) << lbCnt1 << ' '
		
                << std::setw(2) << lMx1 << " ; "
                << (lRes2 ? "yes" : "no ") << ' '
                << std::setw(4) << bCnt2 << ' '
		<< std::setw(4) << lbCnt2 << ' '
                << std::setw(2) << lMx2
                << std::endl;
    }
  }



  std::cout << "....compare_n_theta5...." << std::endl;
  for(uint i = 0; i < gSynopsis.size(); ++i) {
    if("mhist" == gSynopsis[i]) { continue; } 
    for(uint j = i + 1; j < gSynopsis.size(); ++j) {
      if("mhist" == gSynopsis[j]) { continue; }
      auto [lRes1, bCnt1, lbCnt1, lMx1] = lSynProf[i].compare_n(lSynProf[j],5);
      auto [lRes2, bCnt2, lbCnt2, lMx2] = lSynProf[j].compare_n(lSynProf[i],5);
      std::cout << std::setw(8) << gSynopsis[i] 
                << " cmp_t5 " 
                << std::setw(8) << gSynopsis[j] << ": "
                << (lRes1 ? "yes" : "no ") << ' '
                << std::setw(4) << bCnt1 << ' '
	        << std::setw(4) << lbCnt1 << ' '
		
                << std::setw(2) << lMx1 << " ; "
                << (lRes2 ? "yes" : "no ") << ' '
                << std::setw(4) << bCnt2 << ' '
		<< std::setw(4) << lbCnt2 << ' '
                << std::setw(2) << lMx2
                << std::endl;
    }
  }
}
int main(int argc, char* argv[]) {
    // Check if the user provided the required arguments
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <Experiment Directory> <Release Directory>" << std::endl;
        return 1; // Exit with an error code
    }

    // Assign the paths from command-line arguments (explicit conversion)
    const std::string gExpDirName = std::string(argv[1]);
    const std::string gRelDirName = std::string(argv[2]);

    // Display the paths to confirm they are set correctly
    std::cout << "Experiment Directory: " << gExpDirName << std::endl;
    std::cout << "Release Directory: " << gRelDirName << std::endl;
    const bool lGenRelRaw  = true;
    const bool lGenRelAggr = true;
    const bool lRunCompare = true;
//  test0();
//  test1();
  test2(lGenRelRaw, lGenRelAggr, lRunCompare, gExpDirName, gRelDirName);

    // Your main program logic goes here
    return 0;
}




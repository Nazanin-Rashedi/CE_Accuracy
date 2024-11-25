#include "infra/glob_infra_standard_includes.hh"
#include "infra/tmath.hh"
#include "infra/data2dim.hh"
#include <filesystem>

typedef std::vector<std::string> string_vt;

const std::string gH2D = "/home/moer/H2D/in";

const string_vt gSDS = {
  "earthquake",
  "OpenStreetMap",
  "tiger"
};


void
test0() {
  std::filesystem::path lDirTop(gH2D);
  std::filesystem::path lDirSDS;
  uint_vt lHistogram(30);
  for(const auto& lSDS : gSDS) {
    lDirSDS  = lDirTop;
    lDirSDS /= lSDS;
    for(const auto& lIter : std::filesystem::directory_iterator(lDirSDS)) {
      std::filesystem::path lDS = lIter.path();
      if(".hist" == lDS.extension()) {
        H2D::Data2dim lData;
        lData.readHistFile(lDS);
        const uint lLog2Size = std::floor(std::log2(lData.total()));
        std::cout << lData.total() << ' '
                  << lData.size() << ' '
                  << lLog2Size << ' '
                  << lData.maxFrequency() << ' '
                  << lSDS << ' '
                  << lDS.stem() << ' '
                  // << lDS.extension()
                  << std::endl;
        ++lHistogram[lLog2Size];
      }
    }
  }
  std::cout << "Histogram of log2(sizes):" << std::endl;
  for(uint i = 0; i < lHistogram.size(); ++i) {
    std::cout << std::setw(2) << i << ' '
              << std::setw(8) << lHistogram[i]
              << std::endl;
  }
}


int
main() {
  test0();
  return 0;
}

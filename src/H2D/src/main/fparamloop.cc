#include "fparamloop.hh"


const uint gPlNoSizes = 8;
const uint gPlNoSizesSlow = 5;
const uint gPlSizes[gPlNoSizes] = { 1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000 };

// const uint gPlNoNXY = 4;
// const uint gPlNXY[gPlNoNXY] = {64, 128, 256, 512};
const uint gPlNoNXY = 3;
const uint gPlNXY[gPlNoNXY] = {64, 128, 256};

void 
fParamLoopEstConst(const H2D::Data2dim& aData, H2D::Cb& aCb) {
  fProcessEstConst(aData, aCb);
}

void 
fParamLoopEstArea(const H2D::Data2dim& aData, H2D::Cb& aCb) {
  fProcessEstArea(aData, aCb);
}

void 
fParamLoopGenHist(const H2D::Data2dim& aData, H2D::Cb& aCb) {
}

void 
fParamLoopNonoTree(const H2D::Data2dim& aData, H2D::Cb& aCb) {
}

void 
fParamLoopQTS(const H2D::Data2dim& aData, H2D::Cb& aCb) {
}

void 
fParamLoopIQTS(const H2D::Data2dim& aData, H2D::Cb& aCb) {
  for(uint i = 0; i < gPlNoSizes; ++i) {
    aCb.budget(gPlSizes[i]);
    aCb.kind(0);
    fProcessIQTS(aData, aCb);
    aCb.kind(5);
    fProcessIQTS(aData, aCb);
  }
}

void 
fParamLoopFlexTree(const H2D::Data2dim& aData, H2D::Cb& aCb) {
}

void 
fParamLoopGxTree(const H2D::Data2dim& aData, H2D::Cb& aCb) {
  for(uint i = 0; i < gPlNoSizes; ++i) {
    aCb.budget(gPlSizes[i]);
    for(uint k = 0; k < 4; ++k) {
      aCb.kind(k);
      aCb.leafRefinement(0);
      aCb.lrf(0.0);
      fProcessGxTree(aData, aCb);
      aCb.leafRefinement(2);
      aCb.lrf(0.7);
      fProcessGxTree(aData, aCb);
      aCb.lrf(0.8);
      fProcessGxTree(aData, aCb);
      aCb.lrf(0.9);
      fProcessGxTree(aData, aCb);
      aCb.lrf(1.0);
      fProcessGxTree(aData, aCb);
    }
  }
}

void 
fParamLoopGridTree(const H2D::Data2dim& aData, H2D::Cb& aCb) {
}

void 
fParamLoopEquiDepthHist(const H2D::Data2dim& aData, H2D::Cb& aCb) {
  for(uint i = 0; i < gPlNoSizes; ++i) {
    aCb.budget(gPlSizes[i]);
    aCb.kind(0);
    fProcessEquiDepthHist(aData, aCb);
  }
}

void 
fParamLoopEquiDepthHist2(const H2D::Data2dim& aData, H2D::Cb& aCb) {
  for(uint i = 0; i < gPlNoSizes; ++i) {
    aCb.budget(gPlSizes[i]);
    aCb.kind(0);
    fProcessEquiDepthHist2(aData, aCb);
  }
}

void 
fParamLoopPhased(const H2D::Data2dim& aData, H2D::Cb& aCb) {
  for(uint i = 0; i < gPlNoSizes; ++i) {
    aCb.budget(gPlSizes[i]);
    aCb.kind(0);
    fProcessPhased(aData, aCb);
  }
}

void 
fParamLoopDct(const H2D::Data2dim& aData, H2D::Cb& aCb) {
  aCb.kind(0);
  for(uint i = 0; i < gPlNoSizesSlow; ++i) {
    aCb.budget(gPlSizes[i]);
    for(uint j = 0; j < gPlNoNXY; ++j) {
      aCb.nx(gPlNXY[j]);
      aCb.ny(gPlNXY[j]);
      fProcessDct(aData, aCb);
    }
  }
}

void 
fParamLoopDctCf(const H2D::Data2dim& aData, H2D::Cb& aCb) {
}

void 
fParamLoopWaveletNonStd(const H2D::Data2dim& aData, H2D::Cb& aCb) {
  aCb.kind(0);
  for(uint i = 0; i < gPlNoSizesSlow; ++i) {
    aCb.budget(gPlSizes[i]);
    for(uint j = 0; j < gPlNoNXY; ++j) {
      aCb.nx(gPlNXY[j]);
      aCb.ny(gPlNXY[j]);
      fProcessWaveletNonStd(aData, aCb);
    }
  }
}

void 
fParamLoopRegP(const H2D::Data2dim& aData, H2D::Cb& aCb) {
  aCb.kind(0);
  for(uint i = 0; i < gPlNoSizes; ++i) {
    aCb.budget(gPlSizes[i]);
    for(uint j = 0; j < gPlNoNXY; ++j) {
      aCb.nx(gPlNXY[j]);
      aCb.ny(gPlNXY[j]); 
      fProcessRegP(aData, aCb);
    }
  }
}

void
fParamLoopZHist(const H2D::Data2dim& aData, H2D::Cb& aCb) {
  aCb.kind(0);
  for(uint i = 0; i < gPlNoSizesSlow; ++i) {
    aCb.budget(gPlSizes[i]);
    for(uint j = 0; j < gPlNoNXY; ++j) {
      aCb.nx(gPlNXY[j]);
      aCb.ny(gPlNXY[j]);
      fProcessZHist(aData, aCb);
    }
  }
}

void 
fParamLoopHft(const H2D::Data2dim& aData, H2D::Cb& aCb) {
}

void 
fParamLoopHiRed(const H2D::Data2dim& aData, H2D::Cb& aCb) {
}

void 
fParamLoopMHist(const H2D::Data2dim& aData, H2D::Cb& aCb) {
}

void 
fParamLoopMHist2(const H2D::Data2dim& aData, H2D::Cb& aCb) {
  for(uint i = 0; i < gPlNoSizes; ++i) {
    aCb.budget(gPlSizes[i]);
    aCb.kind(4);
    fProcessMHist2(aData, aCb);
    aCb.kind(5);
    fProcessMHist2(aData, aCb);
  }
}

void 
fParamLoopSample(const H2D::Data2dim& aData, H2D::Cb& aCb)  {
  aCb.kind(0);
  aCb.sampleSize(0);
  for(uint i = 0; i < gPlNoSizes; ++i) {
    aCb.budget(gPlSizes[i]);
    fProcessSample(aData, aCb);
  }
}





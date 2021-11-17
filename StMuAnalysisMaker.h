//#ifndef StMuAnalysisMaker_def
//#define StMuAnalysisMaker_def
#include "StMaker.h"
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"


class TFile;
class TH1D;
class TH2D;
class StMuDst;
class StMuEvent;
class StMuTrack;
class StMuMcTrack;

#define NumCentClass 9

class StMuAnalysisMaker : public StMaker {

 public:
  StMuAnalysisMaker(); //StMuDstMaker* maker);
  virtual ~StMuAnalysisMaker();
  Int_t Init();
  Int_t Make();
  Int_t Finish();
  void  Clear(Option_t *option="");
	
  void SetOutputFileName(TString name) {mOutputFileName = name;}
  void SetParticleType(char* PartName){mPartType = PartName; }

 private:

  //	StMuDstMaker* mMuDstMaker;
  TH1D* hSpectraMC[NumCentClass];
  TH1D* hSpectraRC[NumCentClass];
  TH1D* hEfficienty[NumCentClass];

  TH1D *hNoCutVertexZ;
  TH2D *hNoCutVertexXY;
  TH1D *hNoCutRefMult;
  TH1D *hNoCutNFitPoints;
  TH1D *hNoCutNFitPtOverPossPt;
  TH1D *hNoCutNFitdEdx;
  TH1D *hNoCutDCAz;
  TH1D *hNoCutDCAmagn;
  TH2D *hNoCutDCAxy;

  TH1D *hCutVertexZ;
  TH2D *hCutVertexXY;
  TH1D *hCutRefMult;
  TH1D *hCutNFitPoints;
  TH1D *hCutNFitPtOverPossPt;
  TH1D *hCutNFitdEdx;
  TH1D *hCutDCAz;
  TH1D *hCutDCAmagn;
  TH2D *hCutDCAxy;

  TFile* outputFile;
  UInt_t mEventsProcessed;
  TString mOutputFileName;
  char* mPartType;

  ClassDef(StMuAnalysisMaker,0)
};
//#endif

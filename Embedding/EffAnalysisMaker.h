#ifndef SimpleAnalysisMaker_def
#define SimpleAnalysisMaker_def
#include "StMaker.h"
#include "TString.h"
#include "TH1D.h"
#include "TH2D.h"



class StMuDstMaker;
class TFile;
class TH1F;

#define NumCentClass 9
#define NumEta 27
#define EtaMin -1.35
#define EtaMax 1.35

class EfficientyAnalysisMaker : public StMaker
{
private:
	StMuDstMaker* mMuDstMaker;

	TH1D* hSpectraMC[NumCentClass][NumEta];
	TH1D* hSpectraRC[NumCentClass][NumEta];
	TH1D* hEfficienty[NumCentClass][NumEta];

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
	Int_t mPartIndex;
public:
	EfficientyAnalysisMaker(StMuDstMaker* maker);
	virtual ~EfficientyAnalysisMaker();
	Int_t Init();
	Int_t Make();
	Int_t Finish();

	void SetOutputFileName(TString name) {mOutputFileName = name;}
	void SetParticleIndex(Int_t index){mPartIndex = index; }
	ClassDef(EfficientyAnalysisMaker,1)
};
#endif

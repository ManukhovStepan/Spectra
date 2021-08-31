// C++ headers

#include <iostream>
#include <string>

// ROOT headers
#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TString.h"
#include "TVector3.h"
#include "TMath.h"
#include "TObjArray.h"

Int_t starLibs = gROOT->Macro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
Int_t picoLib  = gSystem->Load("StPicoEvent");
Int_t picoLib1 = gSystem->Load("StRefMultCorr");

const Char_t *defaultOutFile="27GeV_spectra.root";

using namespace std;

int Analysis27GeV(const Char_t *inFile = "/star/data01/pwg/manukhov/27GeV/*.picoDst.root",   const Char_t *outFileName=defaultOutFile)
{

// Open input and output files --------------------------------------------------------------------------------
	StPicoDstReader *picoReader = new StPicoDstReader(inFile);
	picoReader->Init();
	TFile ofile(outFileName,"RECREATE");
	TObjArray l(0);  // TObjArray is required to write correctly in the file 

// Initializing some variables --------------------------------------------------------------------------------
	Int_t cent9, runid, NumBins = 90;
	Double_t RefMult, Vz, Vx, Vy, nHitsOverPoss;
	Double_t posYields = 0, negYields = 0, dEta, dPt;

// Initializing histograms ------------------------------------------------------------------------------------
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

	TString hPos_names[9];
	TH1D *hPosSpectra[9];
	TString hNeg_names[9];
	TH1D *hNegSpectra[9];

// Defining histograms ----------------------------------------------------------------------------------------
	hNoCutVertexZ 		= new TH1D("hNoCutVertexZ","Vertex Z destribution, no cuts",20, -50., 50.);
	hNoCutVertexXY 		= new TH2D("hNoCutVertexXY","Vertex XY destribution, no cuts",40, -0.5, 0.5, 40, -0.5, 0.5);
	hNoCutRefMult		= new TH1D("hNoCutRefMult","Reference multiplicity destribution, no cuts", 60, 0., 500.);
	hNoCutNFitPoints	= new TH1D("hNoCutFitPoints", "Number of fit points destribution, no cuts", 50, 0., 50.);
	hNoCutNFitPtOverPossPt	= new TH1D("hNoCutFitPtOverPossPt", "Number of fit points over the number of possible fit points destribution, no cuts", 50, 0., 1.1);
	hNoCutNFitdEdx		= new TH1D("hNoCutNFitdEdx", "Number of dE/dx points destribution, no cuts", 50, 0., 50.);
	hNoCutDCAz		= new TH1D("hNoCutDCAz", "DCA Z destribution, no cuts", 50, -1.5, 1.5);
	hNoCutDCAxy		= new TH2D("hNoCutDCAxy", "DCA XY destribution, no cuts", 50, -5, 5, 50, -5, 5);
	hNoCutDCAmagn		= new TH1D("hNoCutDCAmagn", "DCA magnitude destribution, no cuts", 50, 0.,2);

	hCutVertexZ 		= new TH1D("hCutVertexZ","Vertex Z destribution, with cuts",20, -50., 50.);
	hCutVertexXY 		= new TH2D("hCutVertexXY","Vertex XY destribution, with cuts",40, -0.5, 0.5, 40, -0.5, 0.5);
	hCutRefMult		= new TH1D("hCutRefMult","Reference multiplicity destribution, with cuts", 60, 0., 500.);
	hCutNFitPoints		= new TH1D("hCutFitPoints", "Number of fit points destribution, with cuts", 50, 0., 50.);
	hCutNFitPtOverPossPt	= new TH1D("hCutFitPtOverPossPt", "Number of fit points over the number of possible fit points destribution, with cuts", 50, 0., 1.1);
	hCutNFitdEdx		= new TH1D("hCutNFitdEdx", "Number of dE/dx points destribution, with cuts", 50, 0., 50.);
	hCutDCAz		= new TH1D("hCutDCAz", "DCA Z destribution, with cuts", 50, -1.5, 1.5);
	hCutDCAxy		= new TH2D("hCutDCAxy", "DCA XY destribution, with cuts", 50, -5, 5, 50, -5, 5);
	hCutDCAmagn		= new TH1D("hCutDCAmagn", "DCA magnitude destribution, with cuts", 50, 0.,2);

	for(Int_t i = 0; i < 9; i++)
	{
		hPos_names[i] 	= "hPosSpectra_";
		hPos_names[i]  += i;
		hPosSpectra[i] 		= new TH1D(hPos_names[i],"Pos d^{2}N/2piP_{T}dP_{T}deta distribution",NumBins, 0.01, 9.);
		hNeg_names[i] 	= "hNegSpectra_";
		hNeg_names[i]  += i;
		hNegSpectra[i] 		= new TH1D(hNeg_names[i],"Neg d^{2}N/2piP_{T}dP_{T}deta distribution",NumBins, 0.01, 9.);
	};

// Drawing settings of histograms -----------------------------------------------------------------------------
	hNoCutVertexZ->SetFillColor(kBlue);
	hNoCutVertexZ->SetOption("bar2");
	hNoCutRefMult->SetFillColor(kBlue);
	hNoCutRefMult->SetOption("bar2");
	hNoCutVertexXY->SetOption("COLZ");
	hNoCutNFitPoints->SetFillColor(kBlue);
	hNoCutNFitPoints->SetOption("bar2");
	hNoCutNFitPtOverPossPt->SetFillColor(kBlue);
	hNoCutNFitPtOverPossPt->SetOption("bar2");
	hNoCutNFitdEdx->SetFillColor(kBlue);
	hNoCutNFitdEdx->SetOption("bar2");
	hNoCutDCAz->SetFillColor(kBlue);
	hNoCutDCAz->SetOption("bar2");
	hNoCutDCAmagn->SetFillColor(kBlue);
	hNoCutDCAmagn->SetOption("bar2");
	hNoCutDCAxy->SetOption("COLZ");

	hCutVertexZ->SetFillColor(kBlue);
	hCutVertexZ->SetOption("bar2");
	hCutRefMult->SetFillColor(kBlue);
	hCutRefMult->SetOption("bar2");
	hCutVertexXY->SetOption("COLZ");
	hCutNFitPoints->SetFillColor(kBlue);
	hCutNFitPoints->SetOption("bar2");
	hCutNFitPtOverPossPt->SetFillColor(kBlue);
	hCutNFitPtOverPossPt->SetOption("bar2");
	hCutNFitdEdx->SetFillColor(kBlue);
	hCutNFitdEdx->SetOption("bar2");
	hCutDCAz->SetFillColor(kBlue);
	hCutDCAz->SetOption("bar2");
	hCutDCAmagn->SetFillColor(kBlue);
	hCutDCAmagn->SetOption("bar2");
	hCutDCAxy->SetOption("COLZ");

// Setting the status of branches -----------------------------------------------------------------------------
	picoReader->SetStatus("*",0);
	picoReader->SetStatus("Event*", 1);
	picoReader->SetStatus("Track*", 1);

// Reading by events ------------------------------------------------------------------------------------------
	Long64_t events2read = picoReader->chain()->GetEntries();
	//if (events2read > 5000) events2read = 5000;
	for(Long64_t iEvent=0; iEvent<events2read; iEvent++) {
		cout << "Working on event #[" << (iEvent+1) << "/" << events2read << "]" << "\r";
		Bool_t readEvent = picoReader->readPicoEvent(iEvent);
		StPicoDst *dst = picoReader->picoDst();
		StPicoEvent *event = dst->event();
		if(	!event->isTrigger(610001)		&&
			!event->isTrigger(610011)		&&
			!event->isTrigger(610021)		&&
			!event->isTrigger(610031)		&&
			!event->isTrigger(610041)		&&
			!event->isTrigger(610051)		) continue; 

		runid = event->runId();
		RefMult = event->refMult();
		Vz = event->primaryVertex().Z();
		Vx = event->primaryVertex().X();
		Vy = event->primaryVertex().Y();
		Double_t rV = sqrt(pow((event->primaryVertex().X() - 0.099),2) + pow((event->primaryVertex().Y() + 0.173),2));

// Centrality definition, accounting for badruns --------------------------------------------------------------
		StRefMultCorr* refmultCorrUtil = CentralityMaker::instance()->getRefMultCorr();
		refmultCorrUtil-> init(event->runId());
		refmultCorrUtil-> initEvent(RefMult, event->primaryVertex().Z(), event->ZDCx());
		cent9 = refmultCorrUtil->getCentralityBin9();
		Bool_t isBadRun = refmultCorrUtil->isBadRun(runid);    
		if(isBadRun) 	continue;
		if(cent9<0) 	continue;

// Filling histograms before event cuts -----------------------------------------------------------------------
		hNoCutRefMult ->Fill(RefMult);
		hNoCutVertexZ ->Fill(event->primaryVertex().Z());
		hNoCutVertexXY->Fill(event->primaryVertex().X(), event->primaryVertex().Y());

// Event cuts -------------------------------------------------------------------------------------------------
		if(abs(event->primaryVertex().Z())>30.) continue;
		if(rV>0.25)				continue;

// Filling histograms after event cuts ------------------------------------------------------------------------
		hCutRefMult ->Fill(RefMult);
		hCutVertexZ ->Fill(event->primaryVertex().Z());
		hCutVertexXY->Fill(event->primaryVertex().X(), event->primaryVertex().Y());

// Reading by tracks ------------------------------------------------------------------------------------------
		for(Int_t iTrk=0; iTrk<dst->numberOfTracks(); iTrk++) {
			StPicoTrack *picoTrack = dst->track(iTrk);
			if(!picoTrack) 			continue;
			if(!picoTrack->isPrimary())	continue;
			nHitsOverPoss = (Double_t)((Double_t)(picoTrack->nHitsFit()) / (Double_t)(picoTrack->nHitsMax()));

// Filling histograms before track cuts -----------------------------------------------------------------------
			hNoCutNFitPoints->Fill(picoTrack->nHitsFit());
			hNoCutNFitPtOverPossPt->Fill((Double_t)nHitsOverPoss);
			hNoCutNFitdEdx->Fill(picoTrack->nHitsDedx());
			hNoCutDCAz->Fill(picoTrack->gDCA(event->primaryVertex()).Z());
			hNoCutDCAxy->Fill(picoTrack->gDCA(event->primaryVertex()).X(), picoTrack->gDCA(event->primaryVertex()).Y());
			hNoCutDCAmagn->Fill(picoTrack->gDCA(event->primaryVertex()).Mag());

// Track cuts -------------------------------------------------------------------------------------------------
			if(picoTrack->gDCA(event->primaryVertex()).Mag()>1.) 	continue;
			if(picoTrack->nHitsFit()<15.) 				continue;
			if(nHitsOverPoss<0.24) 					continue;
			if(picoTrack->nHitsDedx()<13.)  			continue; // (?)
			if(picoTrack->pMom().Pt()<0.2) 				continue;
		//	if((picoTrack->gMom().Pt() / picoTrack->pMom().Pt())>0.7) continue;
		//	if((picoTrack->gMom().Pt() / picoTrack->pMom().Pt())<1.4285) continue;

// Filling histograms after track cuts ------------------------------------------------------------------------
			hCutNFitPoints->Fill(picoTrack->nHitsFit());
			hCutNFitPtOverPossPt->Fill((Double_t)nHitsOverPoss);
			hCutNFitdEdx->Fill(picoTrack->nHitsDedx());
			hCutDCAz->Fill(picoTrack->gDCA(event->primaryVertex()).Z());
			hCutDCAxy->Fill(picoTrack->gDCA(event->primaryVertex()).X(), picoTrack->gDCA(event->primaryVertex()).Y());
			hCutDCAmagn->Fill(picoTrack->gDCA(event->primaryVertex()).Mag());

// Filling pT spectra -----------------------------------------------------------------------------------------
			dEta = 1;
			dPt = (hPosSpectra[0]->GetBinCenter(hPosSpectra[0]->GetNbinsX()) - hPosSpectra[0]->GetBinCenter(0)) / hPosSpectra[0]->GetNbinsX();
			if(picoTrack->charge()>0) {
				posYields = 1./((2.*3.14159) *(picoTrack ->pMom().Pt())*dPt*dEta);
				hPosSpectra[cent9]-> Fill(picoTrack ->pMom().Pt(), posYields);
			}
			if(picoTrack->charge()<0) {
				negYields = 1./((2.*3.14159) *(picoTrack ->pMom().Pt())*dPt*dEta);
				hNegSpectra[cent9]-> Fill(picoTrack ->pMom().Pt(), negYields);
			}
		} 	// end iTrack
	} 		// end iEvent

// Saving histograms to a file --------------------------------------------------------------------------------
	for(Int_t j = 0; j<9; j++) {   
		l.Add(hPosSpectra[j]); // l is an element of the TObjArray class 
		l.Add(hNegSpectra[j]);
	}
     
	l.Write();

	hNoCutRefMult->Write();
	hNoCutVertexZ->Write();
	hNoCutVertexXY->Write();
	hNoCutNFitPoints->Write();
	hNoCutNFitPtOverPossPt->Write();
	hNoCutNFitdEdx->Write();
	hNoCutDCAz->Write();
	hNoCutDCAxy->Write();
	hNoCutDCAmagn->Write();

	hCutRefMult->Write();
	hCutVertexZ->Write();
	hCutVertexXY->Write();
	hCutNFitPoints->Write();
	hCutNFitPtOverPossPt->Write();
	hCutNFitdEdx->Write();
	hCutDCAz->Write();
	hCutDCAxy->Write();
	hCutDCAmagn->Write();

	ofile.Close();

// Removing pointers ------------------------------------------------------------------------------------------
	l.Delete();
	delete hRefMult;
	delete hVertexZ;
	delete hVertexXY;
	delete hNFitPoints;
	delete hNFitPtOverPossPt;
	delete hNFitdEdx;
	delete hDCAz;
	delete hDCAxy;
	delete hDCAmagn;
	delete hGPtOverPPt;
	delete hNFitPVsEta2D;
	delete [] hPosSpectra;
	delete [] hNegSpectra;

// End of the program -----------------------------------------------------------------------------------------
	picoReader->Clear();
	picoReader->Finish();
	return 0;
}



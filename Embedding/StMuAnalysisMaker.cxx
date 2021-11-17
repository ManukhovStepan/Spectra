#include "TMath.h"
#include "TFile.h"
#include "StMessMgr.h"
#include "TH1.h"
#include "TH2.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StMuDSTMaker/COMMON/StMuMcTrack.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuAnalysisMaker.h"

ClassImp(StMuAnalysisMaker)

StMuAnalysisMaker::StMuAnalysisMaker(/* StMuDstMaker* maker */) : StMaker() {
  for ( Int_t i = 0 ; i < NumCentClass ; i++ ) {
      hSpectraMC[i] = NULL;
      hSpectraRC[i] = NULL;
      hEfficienty[i]= NULL;
  }
  //	mMuDstMaker = maker;
  outputFile = NULL;
  mEventsProcessed =  0;
  mOutputFileName = "";
  mPartType = "nan";
}

StMuAnalysisMaker::~StMuAnalysisMaker() { }

Int_t StMuAnalysisMaker::Init() {
  if(mOutputFileName == "" ) mOutputFileName = "muAnalysis.root";
  outputFile = new TFile( mOutputFileName, "recreate");

  std::cout << "Init";

  // Initializing histograms ------------------------------------------------------------------------------------
  const Int_t nbins = 90;
  for ( Int_t i = 0 ; i < NumCentClass ; i++ ) {
      TString hTitlesRC 	= Form("hSpectraRC[%d]",i);	
      TString hNamesRC 	= Form("Reconstructed spectra, centrality index: %d, particle: %s", i, mPartType);	
      hSpectraRC[i] 	= new TH1D( hTitlesRC, hNamesRC, nbins, 0.01, 9.);
      TString hTitlesMC 	= Form("hSpectraMC[%d]",i);	
      TString hNamesMC 	= Form("Monte-Carlo spectra, centrality index: %d, particle: %s", i, mPartType);	
      hSpectraMC[i] 	= new TH1D( hTitlesMC, hNamesMC, nbins, 0.01, 9.);
      TString hTitlesEff 	= Form("hEfficienty[%d]",i);	
      TString hNamesEff 	= Form("Efficienty destribution, centrality index: %d, particle index: %s", i, mPartType);	
      hEfficienty[i] 	= new TH1D( hTitlesEff, hNamesEff, nbins, 0.01, 9.);		
						
      hSpectraRC[i]->SetFillColor(kBlue);
      hSpectraRC[i]->SetOption("bar2");
      hSpectraMC[i]->SetFillColor(kBlue);
      hSpectraMC[i]->SetOption("bar2");
      hEfficienty[i]->SetFillColor(kBlue);
      hEfficienty[i]->SetOption("bar2");
  }

  hNoCutVertexZ 			= new TH1D("hNoCutVertexZ","Vertex Z destribution, no cuts",20, -50., 50.);
  hNoCutVertexXY 			= new TH2D("hNoCutVertexXY","Vertex XY destribution, no cuts",40, -0.5, 0.5, 40, -0.5, 0.5);
  hNoCutRefMult			= new TH1D("hNoCutRefMult","Reference multiplicity destribution, no cuts", 60, 0., 500.);
  hNoCutNFitPoints		= new TH1D("hNoCutFitPoints", "Number of fit points destribution, no cuts", 50, 0., 50.);
  hNoCutNFitPtOverPossPt		= new TH1D("hNoCutFitPtOverPossPt", "Number of fit points over the number of possible fit points destribution, no cuts", 50, 0., 1.1);
  hNoCutNFitdEdx			= new TH1D("hNoCutNFitdEdx", "Number of dE/dx points destribution, no cuts", 50, 0., 50.);
  hNoCutDCAz			= new TH1D("hNoCutDCAz", "DCA Z destribution, no cuts", 50, -1.5, 1.5);
  hNoCutDCAxy			= new TH2D("hNoCutDCAxy", "DCA XY destribution, no cuts", 50, -5, 5, 50, -5, 5);
  hNoCutDCAmagn			= new TH1D("hNoCutDCAmagn", "DCA magnitude destribution, no cuts", 50, 0.,2);

  hCutVertexZ 			= new TH1D("hCutVertexZ","Vertex Z destribution, with cuts",20, -50., 50.);
  hCutVertexXY 			= new TH2D("hCutVertexXY","Vertex XY destribution, with cuts",40, -0.5, 0.5, 40, -0.5, 0.5);
  hCutRefMult			= new TH1D("hCutRefMult","Reference multiplicity destribution, with cuts", 60, 0., 500.);
  hCutNFitPoints			= new TH1D("hCutFitPoints", "Number of fit points destribution, with cuts", 50, 0., 50.);
  hCutNFitPtOverPossPt		= new TH1D("hCutFitPtOverPossPt", "Number of fit points over the number of possible fit points destribution, with cuts", 50, 0., 1.1);
  hCutNFitdEdx			= new TH1D("hCutNFitdEdx", "Number of dE/dx points destribution, with cuts", 50, 0., 50.);
  hCutDCAz			= new TH1D("hCutDCAz", "DCA Z destribution, with cuts", 50, -1.5, 1.5);
  hCutDCAxy			= new TH2D("hCutDCAxy", "DCA XY destribution, with cuts", 50, -5, 5, 50, -5, 5);
  hCutDCAmagn			= new TH1D("hCutDCAmagn", "DCA magnitude destribution, with cuts", 50, 0.,2);

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

  std::cout << "\t[DONE]" << endl;;
  return StMaker::Init();	
}

void StMuAnalysisMaker::Clear(Option_t *opt) {
  std::cout << "Clear";
  StMaker::Clear();
  std::cout << "\t[DONE]\n";
}

Int_t StMuAnalysisMaker::Finish() {
  std::cout << "EffAnalysisMaker::Finish()\n";
  if(!outputFile->IsOpen()){
    std::cout << "File is broken... Can't make or can't read TFile " << std::endl;
  }

  // Saving histograms to a file --------------------------------------------------------------------------------
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
	
  // Saving main histograms into this folders ----------------------------------------------------------------------

  for ( Int_t i = 0 ; i < NumCentClass ; i++ ) {
      hSpectraRC[i]->Write();
      hSpectraMC[i]->Write();
      hEfficienty[i]->Write();

  }

  outputFile->Write();
  outputFile->Close();

  std::cout << "Total Events Processed in DstMaker " << mEventsProcessed << endl ;
  return kStOk;
}

Int_t StMuAnalysisMaker::Make() {  
  std::cout << "Call to StMuAnalysisMaker::Make()" << std::endl;
  std::cout << "Processing Event: " << mEventsProcessed + 1 << std::endl;

  // Defining dst, event, mcTracks etc. -------------------------------------------------------------------------
  StMuDst* mu;
  mu = (StMuDst*) GetInputDS("MuDst");
  if (!mu){
    gMessMgr->Warning() << "StMuAnalysisMaker::Make : No MuDst" << endm;
    return kStOK;        // if no event, we're done
  }

  //  Check StMuEvent branch
  StMuEvent* muEvent;
  muEvent = (StMuEvent*) mu->event();
  if(!muEvent){
    std::cout << "Error: No MuDst Event!    Ptr: " << muEvent << std::endl;
    return kStOK;
  }
  mu->setVertexIndex(0);
  TClonesArray *mcTracks = mu->mcArray(MCTrack);
  int nPrimaryTracks = mu->primaryTracks()->GetEntries();
  int nMcTracks = mcTracks->GetEntriesFast();
	
  Int_t RefMult = muEvent->refMult();
  Double_t Vz = muEvent->primaryVertexPosition().z();
  Double_t Vx = muEvent->primaryVertexPosition().x();
  Double_t Vy = muEvent->primaryVertexPosition().y();
  Double_t Vr = sqrt(pow(Vx,2) + pow(Vy,2));


  // Centrality definition -------------------------------------------------------------------------------------
  Int_t cent9;
  if 		(RefMult > 295) 	{cent9 = 0;}
  else if 	(RefMult > 246) 	{cent9 = 1;}
  else if 	(RefMult > 168) 	{cent9 = 2;}
  else if 	(RefMult > 113) 	{cent9 = 3;}
  else if 	(RefMult > 72) 	{cent9 = 4;}
  else if 	(RefMult > 44) 	{cent9 = 5;}
  else if 	(RefMult > 25) 	{cent9 = 6;}
  else if 	(RefMult > 13) 	{cent9 = 7;}
  else if 	(RefMult > 6) 		{cent9 = 8;}
  else 						{cent9 = -1;}


  // Filling histograms before event cuts -----------------------------------------------------------------------
  hNoCutRefMult->Fill(RefMult);
  hNoCutVertexZ->Fill(Vz);
  hNoCutVertexXY->Fill(Vx, Vy);

  // Event cuts -------------------------------------------------------------------------------------------------
  if((abs(Vz)<30.) && (Vr<2.0) && (cent9!=-1) ){

    // Filling histograms after event cuts ------------------------------------------------------------------------
    hCutRefMult->Fill(RefMult);
    hCutVertexZ->Fill(Vz);
    hCutVertexXY->Fill(Vx, Vy);

    // Reading by reconstructed tracks ----------------------------------------------------------------------------
    for(int trackIndex = 0; trackIndex < nPrimaryTracks; trackIndex++){
      StMuTrack *muTrack = (StMuTrack*) mu->primaryTracks(trackIndex);
      if(!muTrack) continue;

      // Filling histograms before track cuts -----------------------------------------------------------------------
      hNoCutNFitPoints->Fill(muTrack->nHitsFit());
      hNoCutNFitPtOverPossPt->Fill(muTrack->nHitsPoss());
      hNoCutNFitdEdx->Fill(muTrack->nHitsDedx());
      hNoCutDCAz->Fill(muTrack->dcaGlobal().z());
      hNoCutDCAxy->Fill(muTrack->dcaGlobal().x(), muTrack->dcaGlobal().y());
      hNoCutDCAmagn->Fill(muTrack->dcaGlobal().mag());

      // Track cuts -------------------------------------------------------------------------------------------------
      if(muTrack->dcaGlobal().mag()>1.) 	continue;
      if(muTrack->nHitsFit()<15.) 			continue;
      if(muTrack->nHitsPoss()<0.24) 		continue;
	 if(abs(muTrack->eta())>0.5)			continue;
      //	if(muTrack->nHitsDedx()<13.)  	continue;
      if(muTrack->pt()<0.2) 				continue;

      // Filling histograms after track cuts ------------------------------------------------------------------------
      hCutNFitPoints->Fill(muTrack->nHitsFit());
      hCutNFitPtOverPossPt->Fill(muTrack->nHitsPoss());
      hCutNFitdEdx->Fill(muTrack->nHitsDedx());
      hCutDCAz->Fill(muTrack->dcaGlobal().z());
      hCutDCAxy->Fill(muTrack->dcaGlobal().x(), muTrack->dcaGlobal().y());
      hCutDCAmagn->Fill(muTrack->dcaGlobal().mag());

	 // Filling pT reconstructed spectra --------------------------------------------------------------------------
      hSpectraRC[cent9]->Fill(muTrack->pt());


    } // end trackIndex

    // Reading by Monte-Carlo tracks ----------------------------------------------------------------------------
    for (int mcTrackIndex = 0; mcTrackIndex < nMcTracks; ++mcTrackIndex){
      StMuMcTrack *mcTrack = (StMuMcTrack *) mcTracks->UncheckedAt(mcTrackIndex);
      if(!mcTrack) continue;

      // Filling pT Monte-Carlo spectra --------------------------------------------------------------------------*/
      hSpectraMC[cent9]->Fill(mcTrack->pT());
    }

  } // end event cuts

  std::cout << "Make has been finished\n";
  mEventsProcessed++; 
  return kStOK; 
   
}

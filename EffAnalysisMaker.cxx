

#include "TMath.h"
#include "StMuDSTMaker/COMMON/StMuTrack.h"
#include "StMuDSTMaker/COMMON/StMuMcTrack.h"
#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "EffAnalysisMaker.h"


ClassImp(EffAnalysisMaker)

EffAnalysisMaker::EffAnalysisMaker( StMuDstMaker* maker ) : StMaker()
	{
		for ( Int_t i = 0 ; i < NumCentClass ; i++ ) {
			for ( Int_t j = 0 ; j < NumEta ; j++ ) {	
				hSpectraMC[i][j] = NULL;
				hSpectraRC[i][j] = NULL;
				hEfficienty[i][j]= NULL;
			} 	
		}
		mMuDstMaker = maker;
		outputFile = NULL;
		mEventsProcessed =  0;
		mOutputFileName = "";
		mPartIndex = -1;
	}
//#############################################################################################################
//------------------------------------------    Init()   ------------------------------------------------------
//#############################################################################################################
Int_t EffAnalysisMaker::Init()
	{
		if(mOutputFileName == "" ) mOutputFileName = "muAnalysis.root";
		outputFile = new TFile( mOutputFileName, "recreate");

// Initializing histograms ------------------------------------------------------------------------------------
		const Int_t nbins = 90;
		for ( Int_t i = 0 ; i < NumCentClass ; i++ ) {
			for ( Int_t j = 0 ; j < NumEta ; j++ ) {
				TString hTitlesRC 	= "hSpectraRC[" + i + "][" + j + "]";	
				TString hNamesRC 	= "Reconstructed spectra, centrality index: " + i + ", eta index: " + j + ", particle index: " + mPartIndex;	
				hSpectraRC[i][j] 	= new TH1D( hTitlesRC, hNamesMC, nbins, 0.01, 9.);
				TString hTitlesMC 	= "hSpectraMC[" + i + "][" + j + "]";	
				TString hNamesMC	= "Monte-Carlo spectra, centrality index: " + i + ", eta index: " + j + ", particle index: " + mPartIndex;	
				hSpectraRC[i][j] 	= new TH1D( hTitlesMC, hNamesMC, nbins, 0.01, 9.);
				TString hTitlesEff 	= "hEfficienty[" + i + "][" + j + "]";	
				TString hNamesEff 	= "Efficienty destribution, centrality index: " + i + ", eta index: " + j + ", particle index: " + mPartIndex;	
				hEfficienty[i][j] 	= new TH1D( hTitlesEff, hNamesEff, nbins, 0.01, 9.);

				hSpectraRC[i][j]->SetFillColor(kBlue);
				hSpectraRC[i][j]->SetOption("bar2");
				hSpectraMC[i][j]->SetFillColor(kBlue);
				hSpectraMC[i][j]->SetOption("bar2");
				hEfficienty[i][j]->SetFillColor(kBlue);
				hEfficienty[i][j]->SetOption("bar2");
			}
		}

		hNoCutVertexZ 			= new TH1D("hNoCutVertexZ","Vertex Z destribution, no cuts",20, -50., 50.);
		hNoCutVertexXY 		= new TH2D("hNoCutVertexXY","Vertex XY destribution, no cuts",40, -0.5, 0.5, 40, -0.5, 0.5);
		hNoCutRefMult			= new TH1D("hNoCutRefMult","Reference multiplicity destribution, no cuts", 60, 0., 500.);
		hNoCutNFitPoints		= new TH1D("hNoCutFitPoints", "Number of fit points destribution, no cuts", 50, 0., 50.);
		hNoCutNFitPtOverPossPt	= new TH1D("hNoCutFitPtOverPossPt", "Number of fit points over the number of possible fit points destribution, no cuts", 50, 0., 1.1);
		hNoCutNFitdEdx			= new TH1D("hNoCutNFitdEdx", "Number of dE/dx points destribution, no cuts", 50, 0., 50.);
		hNoCutDCAz			= new TH1D("hNoCutDCAz", "DCA Z destribution, no cuts", 50, -1.5, 1.5);
		hNoCutDCAxy			= new TH2D("hNoCutDCAxy", "DCA XY destribution, no cuts", 50, -5, 5, 50, -5, 5);
		hNoCutDCAmagn			= new TH1D("hNoCutDCAmagn", "DCA magnitude destribution, no cuts", 50, 0.,2);

		hCutVertexZ 			= new TH1D("hCutVertexZ","Vertex Z destribution, with cuts",20, -50., 50.);
		hCutVertexXY 			= new TH2D("hCutVertexXY","Vertex XY destribution, with cuts",40, -0.5, 0.5, 40, -0.5, 0.5);
		hCutRefMult			= new TH1D("hCutRefMult","Reference multiplicity destribution, with cuts", 60, 0., 500.);
		hCutNFitPoints			= new TH1D("hCutFitPoints", "Number of fit points destribution, with cuts", 50, 0., 50.);
		hCutNFitPtOverPossPt	= new TH1D("hCutFitPtOverPossPt", "Number of fit points over the number of possible fit points destribution, with cuts", 50, 0., 1.1);
		hCutNFitdEdx			= new TH1D("hCutNFitdEdx", "Number of dE/dx points destribution, with cuts", 50, 0., 50.);
		hCutDCAz				= new TH1D("hCutDCAz", "DCA Z destribution, with cuts", 50, -1.5, 1.5);
		hCutDCAxy				= new TH2D("hCutDCAxy", "DCA XY destribution, with cuts", 50, -5, 5, 50, -5, 5);
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
	
		return kStOK ;
	}

//#############################################################################################################
//------------------------------------------    Finish()   ----------------------------------------------------
//#############################################################################################################
Int_t EffAnalysisMaker::Finish()
	{
		cout << "EffAnalysisMaker::Finish()\n";
		if(!outputFile->IsOpen()){
			cout << "File is broken... Can't make or can't read TFile " << endl;
		}
		/*TObjArray l(0);  // TObjArray is required to write correctly in the file 
		for ( Int_t i = 0 ; i < NumCentClass ; i++ ) {
			for ( Int_t j = 0 ; j < NumEta ; j++ ) {  
				l.Add(hSpectraRC[i][j]); 
				l.Add(hSpectraMC[i][j]);
				l.Add(hEfficienty[i][j]);
			}
		}
     
		l.Write();*/

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
	
// Creating folders to main histograms ---------------------------------------------------------------------------
		outputFile->mkdir("RecoHistograms");
		outputFile->mkdir("MCHistograms");
		outputFile->mkdir("EfficiencyHistograms");

// Saving main histograms into this folders ----------------------------------------------------------------------
		outputFile->cd();
		outputFile->cd("RecoHistograms");
		for ( Int_t i = 0 ; i < NumCentClass ; i++ ) {
			for ( Int_t j = 0 ; j < NumEta ; j++ ) { 
				hSpectraRC[i][j]->Write();
				hSpectraMC[i][j]->Write();
				hEfficienty[i][j]->Write();
			}
   		}

		outputFile->cd();
		outputFile->cd("MCHistograms");
		for ( Int_t i = 0 ; i < NumCentClass ; i++ ) {
			for ( Int_t j = 0 ; j < NumEta ; j++ ) { 
				hSpectraRC[i][j]->Write();
				hSpectraMC[i][j]->Write();
				hEfficienty[i][j]->Write();
			}
   		}

		outputFile->cd();
		outputFile->cd("EfficiencyHistograms");
		for ( Int_t i = 0 ; i < NumCentClass ; i++ ) {
			for ( Int_t j = 0 ; j < NumEta ; j++ ) { 
				hSpectraRC[i][j]->Write();
				hSpectraMC[i][j]->Write();
				hEfficienty[i][j]->Write();
			}
   		}

		outputFile->Write();
		outputFile->Close();

		cout << "Total Events Processed in DstMaker " << mEventsProcessed << endl ;
		return kStOk;
	}
//#############################################################################################################
//------------------------------------------    Make()   ------------------------------------------------------
//#############################################################################################################

Int_t EffAnalysisMaker::Make() 
	{  
		cout << "Call to StMuAnalysisMaker::Make()" << endl;
		cout << "Processing Event : " << mEventsProcessed + 1 << endl;

// Defining dst, event, mcTracks etc. -------------------------------------------------------------------------
		StMuDst* mu 			= mMuDstMaker -> muDst();
		StMuEvent* muEvent      =  mMuDstMaker -> muDst()-> event(); 
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
		else continue; 


// Filling histograms before event cuts -----------------------------------------------------------------------
		hNoCutRefMult->Fill(RefMult);
		hNoCutVertexZ->Fill(Vz);
		hNoCutVertexXY->Fill(Vx, Vy);

// Event cuts -------------------------------------------------------------------------------------------------
		if(abs(Vz)>30.) 	continue;
		if(Vr>2.0)		continue;

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
			if(muTrack->nHitsFit()<15.) 		continue;
			if(muTrack->nHitsPoss()<0.24) 	continue;
		//	if(muTrack->nHitsDedx()<13.)  	continue; // (?)
			if(muTrack->pt()<0.2) 			continue;

// Filling histograms after track cuts ------------------------------------------------------------------------
			hCutNFitPoints->Fill(muTrack->nHitsFit());
			hCutNFitPtOverPossPt->Fill(muTrack->nHitsPoss());
			hCutNFitdEdx->Fill(muTrack->nHitsDedx());
			hCutDCAz->Fill(muTrack->dcaGlobal().z());
			hCutDCAxy->Fill(muTrack->dcaGlobal().x(), muTrack->dcaGlobal().y());
			hCutDCAmagn->Fill(muTrack->dcaGlobal().mag());

// Finding nEtaBin as a function of Eta: y(x) = kx + b -------------------------------------------------------
			Int_t nEtaBin;
			Double_t k;
			Double_t b;		
			k = (double)(NumEta-1.)/((double)(EtaMax - EtaMin));
			b = (double)((NumEta-1.)*(-EtaMin))/(double)(EtaMax - EtaMin);
			nEtaBin = (int)(k*(muTrack->eta())+b); 
/* ------------------------------------------------------------------------------------------------------------
	For this analisys it means

	Eta	    Eta+dEta	NbinsEta
	-1.35	-1.25	0
	-1.25	-1.15	1
	-1.15	-1.05	2
	-1.05	-0.95	3
	-0.95	-0.85	4
	-0.85	-0.75	5
	-0.75	-0.65	6
	-0.65	-0.55	7
	-0.55	-0.45	8
	-0.45	-0.35	9
	-0.35	-0.25	10
	-0.25	-0.15	11
	-0.15	-0.05	12
	-0.05	0.05		13
	0.05		0.15		14
	0.15		0.25		15
	0.25		0.35		16
	0.35		0.45		17
	0.45		0.55		18
	0.55		0.65		19
	0.65		0.75		20
	0.75		0.85		21
	0.85		0.95		22
	0.95		1.05		23
	1.05		1.15		24
	1.15		1.25		25
	1.25		1.35		26
	
	In general state formules for k, b comes from 
	y(xMin) = 0 	= k*xMin + b
	y(xMax) = N-1 	= k*xMax + b       (N-1 because the countdown starts from zero)
	
	N = NumEta				

// Filling pT reconstructed spectra --------------------------------------------------------------------------*/
		hSpectraRC[cent9][nEtaBin]->Fill(muTrack->pt());


 		} // end trackIndex

// Reading by Monte-Carlo tracks ----------------------------------------------------------------------------
		for (int mcTrackIndex = 0; mcTrackIndex < nMcTracks; ++mcTrackIndex){
			StMuMcTrack *mcTrack = (StMuMcTrack *) mcTracks->UncheckedAt(mcTrackIndex);
			if(!mcTrack) continue;

// Finding nEtaBin as a function of Eta: y(x) = kx + b -------------------------------------------------------
			Int_t nEtaBin;
			Double_t k;
			Double_t b;		
			k = (double)(NumEta-1.)/((double)(EtaMax - EtaMin));
			b = (double)((NumEta-1.)*(-EtaMin))/(double)(EtaMax - EtaMin);
			nEtaBin = (int)(k*(mcTrack->eta())+b); 

// Filling pT Monte-Carlo spectra --------------------------------------------------------------------------*/
		hSpectraMC[cent9][nEtaBin]->Fill(muTrack->pt());
		}
 
 
		mEventsProcessed++; 
		return kStOK; 
   
	}

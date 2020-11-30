// This is needed for calling standalone classes (not needed on RACF)
#define _VANILLA_ROOT_

// C++ headers
#include <iostream>

// ROOT headers
#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TVector3.h"
#include "TMath.h"


// PicoDst headers
#include "/home/manukhov/DUBNA/STAR/StPicoEvent/StPicoDstReader.h"
#include "/home/manukhov/DUBNA/STAR/StPicoEvent/StPicoDst.h"
#include "/home/manukhov/DUBNA/STAR/StPicoEvent/StPicoEvent.h"
#include "/home/manukhov/DUBNA/STAR/StPicoEvent/StPicoTrack.h"
#include "/home/manukhov/DUBNA/STAR/StPicoEvent/StPicoBTofHit.h"
#include "/home/manukhov/DUBNA/STAR/StPicoEvent/StPicoBTowHit.h"
#include "/home/manukhov/DUBNA/STAR/StPicoEvent/StPicoEmcTrigger.h"
#include "/home/manukhov/DUBNA/STAR/StPicoEvent/StPicoBTofPidTraits.h"
#include "/home/manukhov/DUBNA/STAR/StPicoEvent/StPicoTrackCovMatrix.h"

#if ROOT_VERSION_CODE >= ROOT_VERSION(6,0,0) 
R__LOAD_LIBRARY(/home/manukhov/DUBNA/STAR/StPicoEvent/libStPicoDst.so)
#endif

using namespace std;

 // Bose-Einstein fitting function 

Double_t fitBE(Double_t *x, Double_t *par) 
{
   Double_t arg = 0;
   if (par[1] != 0) arg = x[0]/par[1];

   Double_t fitval = par[0] * 1./(TMath::Exp(arg)  -  1.) + par[2];
   return fitval;
}

int NormalCuts()
{
 // Инициализирую вспомогательные переменные, открываю файл
  TFile ofile("Cuts.root","RECREATE");
  StPicoDstReader *picoReader = new StPicoDstReader("/home/manukhov/DUBNA/STAR/st_physics*.picoDst.root");
  picoReader->Init();
  

 //  Инициализирую гистограммы и графики
  TH1D *hVertexZ;
  TH2D *hVertexXY;
  TH1D *hRefMult;
  TH1D *hNFitPoints;
  TH1D *hNFitPtOverPossPt;
  TH1D *hNFitdEdx;
  TH1D *hDCAz;
  TH1D *hDCAmagn;
  TH2D *hDCAxy;
  TH2D *hGPtOverPPt;
  TH2D *hNFitPVsEta2D;
  TH2D *hSpectraVsEta2D;
  TH2D *hSpectraVsNFitP2D;
  TString hHitsTPCvsEta_names[10];
  TString hHitsTPCvsEta_titles[10];
  TH1D *hHitsTPCvsEta[10];
  TString hSpectraVsEta_names[10];
  TString hSpectraVsEta_titles[10];
  TH1D *hSpectraVsEta[10];
  TMultiGraph* hHitsTPCvsEtaAll = new TMultiGraph();
  TMultiGraph* hSpectraVsEtaAll = new TMultiGraph();

  hVertexZ 		= new TH1D("Vertex Z","Vertex Z destribution",20, -50., 50.);
  hVertexXY 		= new TH2D("Vertex XY","Vertex XY destribution",40, -0.5, 0.5, 40, -0.5, 0.5);
  hRefMult		= new TH1D("RefMult","Reference multiplicity destribution", 60, 0., 450.);
  hNFitPoints		= new TH1D("nFitPoints", "Number of fit points destribution", 50, 0., 50.);
  hNFitPtOverPossPt	= new TH1D("nFitPtOverPossPt", "Number of fit points over the number of possible fit points destribution", 50, 0., 1.1);
  hNFitdEdx		= new TH1D("hNFitdEdx", "Number of dE/dx points destribution", 50, 0., 50.);
  hDCAz			= new TH1D("hDCAz", "DCA Z destribution", 50, -1.5, 1.5);
  hDCAxy		= new TH2D("hDCAxy", "DCA XY destribution", 50, -1.5, 1.5, 50, -1.5, 1.5);
  hDCAmagn		= new TH1D("hDCAmagn", "DCA magnitude destribution", 50, 0.,2);
  hGPtOverPPt		= new TH2D("hGPtOverPPt", "Global track vs. primary track P_{T} destribution", 50, 0., 1.7, 50, 0., 1.7);
  hNFitPVsEta2D		= new TH2D("hNFitPVsEta2D", "Number of fit points vs. pseudo rapidity", 50, -1.5, 1.5, 50, 0., 50.);
  hSpectraVsEta2D	= new TH2D("hSpectraVsEta2D", "d^{2}N/2piP_{T}dP_{T}dEta distribution vs. pseudo rapidity", 20, -1.5, 1.5, 30, 0.2, 1.9);
  hSpectraVsNFitP2D	= new TH2D("hSpectraVsNFitP2D", "d^{2}N/2piP_{T}dP_{T}dEta distribution vs. number of fit points", 50, 0., 50., 30, 0.2, 1.9);
  for(Int_t i = 0; i < 10; i++)
  {
  	hHitsTPCvsEta_names[i] 	= "hHitsTPCvsEta_" + to_string(i+1);
        hHitsTPCvsEta_titles[i] = "Eta = " + to_string(0.5 + i*0.1);
  	hHitsTPCvsEta[i] 		= new TH1D(hHitsTPCvsEta_names[i],hHitsTPCvsEta_titles[i],15, 25., 40.);
	hHitsTPCvsEta[i]->SetMarkerStyle(21);
	hHitsTPCvsEta[i]->SetFillColor(i+1);

  	hSpectraVsEta_names[i] 	= "hSpectraVsEta" + to_string(i+1);
        hSpectraVsEta_titles[i] = "d^{2}N/2piP_{T}dP_{T}dEta distribution, 0-5 percent centrality, Eta = " + to_string(0.5 + i*0.1);
  	hSpectraVsEta[i] 		= new TH1D(hSpectraVsEta_names[i],hSpectraVsEta_titles[i],20, 0.2, 1.5);
	hSpectraVsEta[i]->SetMarkerStyle(21);
	hSpectraVsEta[i]->SetMarkerColor(i+1);
  };
  hHitsTPCvsEtaAll->SetTitle("TPC hits destribution; Number of TPC hits; Number of tracks");
  hSpectraVsEtaAll->SetTitle("d^{2}N/2piP_{T}dP_{T}dEta distribution, 0-5 percent centralit; P_{T}, GeV/c; d^{2}N}/2piP_{T}dP_{T}dEta");

    hVertexZ->SetFillColor(kBlue);
    hVertexZ->SetOption("bar2");
    hRefMult->SetFillColor(kBlue);
    hRefMult->SetOption("bar2");
    hNFitPVsEta2D->SetOption("COLZ");
    hSpectraVsEta2D->SetOption("COLZ");
    hSpectraVsNFitP2D->SetOption("COLZ");
    hVertexXY->SetOption("COLZ");
    hNFitPoints->SetFillColor(kBlue);
    hNFitPoints->SetOption("bar2");
    hNFitPtOverPossPt->SetFillColor(kBlue);
    hNFitPtOverPossPt->SetOption("bar2");
    hNFitdEdx->SetFillColor(kBlue);
    hNFitdEdx->SetOption("bar2");
    hDCAz->SetFillColor(kBlue);
    hDCAz->SetOption("bar2");
    hDCAmagn->SetFillColor(kBlue);
    hDCAmagn->SetOption("bar2");
    hDCAxy->SetOption("COLZ");
    hGPtOverPPt->SetOption("COLZ");

 // Начинаю считывание по событиям
  picoReader->SetStatus("Event*", 1);
  Long64_t events2read = picoReader->chain()->GetEntries();
  //if (events2read > 5000) events2read = 5000;
   for(Long64_t iEvent=0; iEvent<events2read; iEvent++) {
    cout << "Working on event #[" << (iEvent+1) << "/" << events2read << "]" << "\r";
    Bool_t readEvent = picoReader->readPicoEvent(iEvent);
    StPicoDst *dst = picoReader->picoDst();
    StPicoEvent *event = dst->event();


    Double_t RefMult = event->refMult();
    Double_t Vz = event->primaryVertex().Z();
    Double_t Vx = event->primaryVertex().X();
    Double_t Vy = event->primaryVertex().Y();

    hRefMult->Fill(RefMult);   
    hVertexZ->Fill(Vz);
    hVertexXY->Fill(Vx, Vy);



 // Счтиываю каждый трэк
    for(Int_t iTrk=0; iTrk<dst->numberOfTracks(); iTrk++) {
    StPicoTrack *picoTrack = dst->track(iTrk);
    if(!picoTrack) continue;

    Double_t nFitP = picoTrack->nHitsFit();
    Double_t nFitPOver = nFitP/47.;
    Double_t ndEdx = picoTrack->nHitsDedx();
    Double_t DCAx = picoTrack->gDCA(event->primaryVertex()).X();
    Double_t DCAy = picoTrack->gDCA(event->primaryVertex()).Y();
    Double_t DCAz = picoTrack->gDCA(event->primaryVertex()).Z();
    Double_t DCAmagn = picoTrack->gDCA(event->primaryVertex()).Mag();
    Double_t gPt = picoTrack->gMom().Pt();
    Double_t pPt = picoTrack->pMom().Pt();
    Double_t Eta = picoTrack->pMom().Eta();
    Double_t absEta = abs(picoTrack->pMom().Eta());
    Double_t Yields = 1./((2.*3.14159) *(picoTrack ->pMom().Pt()));
    
   // cout << Eta << "\r";
    hNFitPoints->Fill(nFitP);
    hNFitPtOverPossPt->Fill(nFitPOver);
    hNFitdEdx->Fill(ndEdx);
    hDCAz->Fill(DCAz);
    hDCAxy->Fill(DCAx, DCAy);
    hDCAmagn->Fill(DCAmagn);
    hGPtOverPPt->Fill(gPt, pPt);
    if(absEta > 0.01)
    hNFitPVsEta2D->Fill(Eta, nFitP);
    hSpectraVsEta2D->Fill(Eta, picoTrack ->pMom().Pt(), Yields);
    hSpectraVsNFitP2D->Fill(nFitP, picoTrack ->pMom().Pt(), Yields);

    for(Int_t i = 0; i<10; i++) {
	Double_t X = 0.5 + 0.1*i;
	if(absEta < X) {
	hHitsTPCvsEta[i]->Fill(nFitP);
	if(RefMult > 295) {
	hSpectraVsEta[i]->Fill(Yields);
	}
	}
    }


  
  } // end for(Int_t iTrk=0; iTrk<dst->numberOfTracks(); iTrk++) 
    

} // end  for(Long64_t iEvent=0; iEvent<events2read; iEvent++)


  for(Int_t i = 0; i<10; i++) {
	TGraphErrors* gr = new TGraphErrors(hHitsTPCvsEta[i]);
	TGraphErrors* gr1 = new TGraphErrors(hSpectraVsEta[i]);
	gr->SetMarkerStyle(21);
	gr->SetMarkerColor(i+1);
	gr->SetFillColor(i+1);
	gr1->SetMarkerStyle(21);
	gr1->SetMarkerColor(i+1);
	gr1->SetFillColor(i+1);
	hHitsTPCvsEtaAll->Add(gr, "AP");
	hSpectraVsEtaAll->Add(gr1, "AP");
}

  for(Int_t i = 0; i<10; i++) {

   TF1 *func = new TF1("fitBE",fitBE,0,2,3);
   func->SetParameters(1000,1,0.1);
   func->SetParNames("Constant","Sigma","Background");
   hSpectraVsEta[i]->Fit("fitBE","r");
}

  ofile.GetList()->Add(hHitsTPCvsEtaAll);
  ofile.GetList()->Add(hSpectraVsEtaAll);
  ofile.Write();
  ofile.Close();

picoReader->Finish();



return 0;
}


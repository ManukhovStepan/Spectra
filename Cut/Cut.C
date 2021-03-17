// C++ headers

#include <iostream>
#include <string>

// ROOT headers
#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TVector3.h"
#include "TMath.h"


Int_t starLibs = gROOT->Macro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
Int_t picoLib = gSystem->Load("StPicoEvent");

const Char_t *defaultOutFile="Cutting.root";

using namespace std;

 // Bose-Einstein fitting function 


int Cutting(const Char_t *inFile = "star/data56/reco/production_14p5GeV_2019/ReversedFullField/P19ic/2019/118/20118006/st_physics*.picoDst.root",
  const Char_t *outFileName=defaultOutFile)
  
{

    TFile ofile(outFileName,"RECREATE");
    TTree Track("Track","Track");
    TTree Event("Event","Event");


    Double_t RefMult;
    Double_t Vz;
    Double_t Vx;
    Double_t Vy;

    Double_t nFitP;
    Double_t nFitPOver;
    Double_t ndEdx;
    Double_t DCAx;
    Double_t DCAy;
    Double_t DCAz;
    Double_t DCAmagn;
    Double_t gPt;
    Double_t pPt;
    Double_t Eta;
    Double_t absEta;
    Double_t Yields;
// C++ headers

#include <iostream>
#include <string>

// ROOT headers
#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TVector3.h"
#include "TMath.h"


Int_t starLibs = gROOT->Macro("$ST
  Track.Branch("nFitP",&nFitP,"nFitP/D");
  Track.Branch("nFitPOver",&nFitPOver,"nFitPOver/D");
  Track.Branch("ndEdx",&ndEdx,"ndEdx/D");
  Track.Branch("DCAx",&DCAx,"DCAx/D");
  Track.Branch("DCAy",&DCAy,"DCAy/D");
  Track.Branch("DCAz",&DCAz,"DCAz/D");
  Track.Branch("Yields",&Yields,"Yields/D");
  Track.Branch("DCAmagn",&DCAmagn,"DCAmagn/D");
  Track.Branch("gPt",&gPt,"gPt/D");
  Track.Branch("pPt",&pPt,"pPt/D");
  Track.Branch("Eta",&Eta,"Eta/D");
  Track.Branch("absEta",&absEta,"absEta/D");

  Event.Branch("RefMult",&RefMult,"RefMult/D");
  Event.Branch("Vz",&Vz,"Vz/D");
  Event.Branch("Vx",&Vx,"Vx/D");
  Event.Branch("Vy",&Vy,"Vy/D");

 // Инициализирую вспомогательные переменные, открываю файл
  StPicoDstReader *picoReader = new StPicoDstReader(inFile);
  picoReader->Init();







 // Начинаю считывание по событиям
  picoReader->SetStatus("Event*", 1);
  Long64_t events2read = picoReader->chain()->GetEntries();
  //if (events2read > 5000) events2read = 5000;
   for(Long64_t iEvent=0; iEvent<events2read; iEvent++) {
    cout << "Working on event #[" << (iEvent+1) << "/" << events2read << "]" << "\r";
    Bool_t readEvent = picoReader->readPicoEvent(iEvent);
    StPicoDst *dst = picoReader->picoDst();
    StPicoEvent *event = dst->event();


     RefMult = event->refMult();
     Vz = event->primaryVertex().Z();
     Vx = event->primaryVertex().X();
     Vy = event->primaryVertex().Y();

//    hRefMult->Fill(RefMult);
//    hVertexZ->Fill(Vz);
//    hVertexXY->Fill(Vx, Vy);



 // Счтиываю каждый трэк
    for(Int_t iTrk=0; iTrk<dst->numberOfTracks(); iTrk++) {
    StPicoTrack *picoTrack = dst->track(iTrk);
    if(!picoTrack) continue;

     nFitP = picoTrack->nHitsFit();
     nFitPOver = nFitP/(picoTrack->nHitsMax());
     ndEdx = picoTrack->nHitsDedx();
     DCAx = picoTrack->gDCA(event->primaryVertex()).X();
     DCAy = picoTrack->gDCA(event->primaryVertex()).Y();
     DCAz = picoTrack->gDCA(event->primaryVertex()).Z();
     DCAmagn = picoTrack->gDCA(event->primaryVertex()).Mag();
     gPt = picoTrack->gMom().Pt();
     pPt = picoTrack->pMom().Pt();
     Eta = picoTrack->pMom().Eta();
     absEta = abs(picoTrack->pMom().Eta());
     Yields = 1./((2.*3.14159) *(picoTrack ->pMom().Pt()));
    


 Track.Fill();
  
  } // end for(Int_t iTrk=0; iTrk<dst->numberOfTracks(); iTrk++) 
    
Event.Fill();
} // end  for(Long64_t iEvent=0; iEvent<events2read; iEvent++)

ofile.Write();
ofile.Close();



picoReader->Finish();
 return 0;
}


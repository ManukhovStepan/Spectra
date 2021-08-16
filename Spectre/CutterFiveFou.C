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
#include "TCanvas.h"
#include "TVector3.h"
#include "TMath.h"

Int_t starLibs = gROOT->Macro("$STAR/StRoot/StMuDSTMaker/COMMON/macros/loadSharedLibraries.C");
Int_t picoLib  = gSystem->Load("StPicoEvent");
Int_t picoLib1 = gSystem->Load("StRefMultCorr");

const Char_t *defaultOutFile="Cutting.root";

using namespace std;

int CutterFiveFou(const Char_t *inFile = "star/data56/reco/production_14p5GeV_2019/ReversedFullField/P19ic/2019/118/20118006/st_physics*.picoDst.root",   const Char_t *outFileName=defaultOutFile)
{
    // Инициализирую вспомогательные переменные, открываю файл
     StPicoDstReader *picoReader = new StPicoDstReader(inFile);
     picoReader->Init();

    TFile ofile(outFileName,"RECREATE");
    

     Int_t cent9;
     Int_t runid;
     
    Double_t RefMult;
    Double_t Vz;
    Double_t Vx;
    Double_t Vy;

    Double_t nFitPOver; //!!!
    Double_t Yields;

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
    TH2D *hNFitPVsEta2D; ///!!
 //   TH2D *hSpectraVsEta2D; //!!
  //  TH2D *hSpectraVsNFitP2D; //!!

    hVertexZ 		= new TH1D("Vertex Z","Vertex Z destribution",20, -50., 50.);
    hVertexXY 		= new TH2D("Vertex XY","Vertex XY destribution",40, -0.5, 0.5, 40, -0.5, 0.5);
    hRefMult		= new TH1D("RefMult","Reference multiplicity destribution", 60, 0., 500.);
    hNFitPoints		= new TH1D("nFitPoints", "Number of fit points destribution", 50, 0., 50.);
    hNFitPtOverPossPt	= new TH1D("nFitPtOverPossPt", "Number of fit points over the number of possible fit points destribution", 50, 0., 1.1);
    hNFitdEdx		= new TH1D("hNFitdEdx", "Number of dE/dx points destribution", 50, 0., 50.);
    hDCAz			= new TH1D("hDCAz", "DCA Z destribution", 50, -1.5, 1.5);
    hDCAxy		= new TH2D("hDCAxy", "DCA XY destribution", 50, -5, 5, 50, -5, 5);
    hDCAmagn		= new TH1D("hDCAmagn", "DCA magnitude destribution", 50, 0.,2);
    hGPtOverPPt		= new TH2D("hGPtOverPPt", "Global track vs. primary track P_{T} destribution", 50, 0.01, 4, 50, 0.01, 4);
    hNFitPVsEta2D		= new TH2D("hNFitPVsEta2D", "Number of fit points vs. pseudo rapidity", 50, -1.5, 1.5, 50, 0., 50.); //!!
//    hSpectraVsEta2D	= new TH2D("hSpectraVsEta2D", "d^{2}N/2piP_{T}dP_{T}dEta distribution vs. pseudo rapidity", 20, -1.5, 1.5, 30, 0.2, 1.9);
//    hSpectraVsNFitP2D	= new TH2D("hSpectraVsNFitP2D", "d^{2}N/2piP_{T}dP_{T}dEta distribution vs. number of fit points", 50, 0., 50., 30, 0.2, 1.9);

    hVertexZ->SetFillColor(kBlue);
    hVertexZ->SetOption("bar2");
    hRefMult->SetFillColor(kBlue);
    hRefMult->SetOption("bar2");
    hNFitPVsEta2D->SetOption("COLZ");
//    hSpectraVsEta2D->SetOption("COLZ");
//    hSpectraVsNFitP2D->SetOption("COLZ");
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

    Double_t posYields=0;
    Double_t negYields=0;
    Double_t events2average = 0.; //!!
    Int_t NumBins = 180;
    TString hPos_names[9];
    TH1D *hPosSpectra[9];
    TString hNeg_names[9];
    TH1D *hNegSpectra[9];

     TObjArray l(0);
     for(Int_t i = 0; i < 9; i++)
     {
           hPos_names[i] 	= "hPosSpectra_";
           hPos_names[i]  += i+1;
           hPosSpectra[i] 		= new TH1D(hPos_names[i],"Pos d^{2}N/2piP_{T}dP_{T}deta distribution",NumBins, 0.01, 9.);
           hNeg_names[i] 	= "hNegSpectra_";
           hNeg_names[i]  += i+1;
           hNegSpectra[i] 		= new TH1D(hNeg_names[i],"Neg d^{2}N/2piP_{T}dP_{T}deta distribution",NumBins, 0.01, 9.);
      };

 // Начинаю считывание по событиям
  picoReader->SetStatus("Event*", 1); //!!
  Long64_t events2read = picoReader->chain()->GetEntries();
  //if (events2read > 5000) events2read = 5000;
   for(Long64_t iEvent=0; iEvent<events2read; iEvent++) {
    cout << "Working on event #[" << (iEvent+1) << "/" << events2read << "]" << "\r";
    Bool_t readEvent = picoReader->readPicoEvent(iEvent);
    StPicoDst *dst = picoReader->picoDst();
    StPicoEvent *event = dst->event();

     runid = event->runId();
     RefMult = event->refMult();
     Vz = event->primaryVertex().Z();
     Vx = event->primaryVertex().X();
     Vy = event->primaryVertex().Y();

     Double_t rV = sqrt(pow((event->primaryVertex().X() - 0.099),2) + pow((event->primaryVertex().Y() + 0.173),2));
   // Centrality definition-------------------------------------------------------------

     StRefMultCorr* refmultCorrUtil = CentralityMaker::instance()->getRefMultCorr();
     refmultCorrUtil-> init(event->runId());
     refmultCorrUtil-> initEvent(RefMult, event->primaryVertex().Z(), event->ZDCx());
     cent9 = refmultCorrUtil->getCentralityBin9();
    
     Bool_t isBadRun = refmultCorrUtil->isBadRun(runid);    
     if(isBadRun)
     continue;
     
if (cent9 < 0) 
 continue;


 // Счтиываю каждый трэк
    for(Int_t iTrk=0; iTrk<dst->numberOfTracks(); iTrk++) {
    StPicoTrack *picoTrack = dst->track(iTrk);
    if(!picoTrack) continue;

    if((picoTrack->nHitsFit() < 50) &&
      (picoTrack->nHitsDedx() < 50) &&
      (abs(picoTrack->gDCA(event->primaryVertex()).Z()) < 1.5) &&
      (abs(picoTrack->gDCA(event->primaryVertex()).X()) < 5) &&
      (abs(picoTrack->gDCA(event->primaryVertex()).Y()) < 5) &&
      (picoTrack->gDCA(event->primaryVertex()).Mag() < 2) &&
      (picoTrack->gMom().Pt() < 4) &&
      (picoTrack->pMom().Pt() < 4)) //!!!
      {
    nFitPOver = ((Double_t)(picoTrack->nHitsFit()) / (Double_t)(picoTrack->nHitsMax())); //!!
    
    hNFitPoints->Fill(picoTrack->nHitsFit());
    hNFitPtOverPossPt->Fill((Double_t)nFitPOver);
    hNFitdEdx->Fill(picoTrack->nHitsDedx());
    hDCAz->Fill(picoTrack->gDCA(event->primaryVertex()).Z());
    hDCAxy->Fill(picoTrack->gDCA(event->primaryVertex()).X(), picoTrack->gDCA(event->primaryVertex()).Y());
    hDCAmagn->Fill(picoTrack->gDCA(event->primaryVertex()).Mag());
    hGPtOverPPt->Fill(picoTrack->gMom().Pt(), picoTrack->pMom().Pt());
    if( abs(picoTrack->pMom().Eta())> 0.005) //!!!
    hNFitPVsEta2D->Fill(picoTrack->pMom().Eta(),picoTrack->nHitsFit());
    }

//    hSpectraVsEta2D->Fill(picoTrack->pMom().Eta(), picoTrack->pMom().Pt(), Yields);
//    hSpectraVsNFitP2D->Fill(picoTrack->nHitsFit(), picoTrack->pMom().Pt(), Yields);

    if(   	 (picoTrack->isPrimary())    &&
            (abs(event->primaryVertex().Z())<30.) 	&&
            (rV<0.25)                      &&  //!!
            (abs(picoTrack->pMom().Eta())<0.5) &&
            (picoTrack->gDCA(event->primaryVertex()).Mag()<1.) &&
            (picoTrack->nHitsFit()>15.) &&
           // (((picoTrack->nHitsFit())/(picoTrack->nHitsMax()))>0.24) && //!!
            (picoTrack->nHitsDedx()>13.) && //!!
            (picoTrack->pMom().Pt()>0.2) &&
            ((picoTrack->gMom().Pt() / picoTrack->pMom().Pt())>0.7) &&
            ((picoTrack->gMom().Pt() / picoTrack->pMom().Pt())<1.4285) )  {



          // Заполняю (2piPt)^-1*d2N/dPtdy гистограммы----------------------------------------------
             if(picoTrack->charge()>0)
             {
             posYields = 1./((2.*3.14159) *(picoTrack ->pMom().Pt()));
             hPosSpectra[cent9]-> Fill(picoTrack ->pMom().Pt(), posYields);

             }
             if(picoTrack->charge()<0)
             {
             negYields = 1./((2.*3.14159) *(picoTrack ->pMom().Pt()));
             hNegSpectra[cent9]-> Fill(picoTrack ->pMom().Pt(), negYields);
             }
}
}
 /* end for(Int_t iTrk=0; iTrk<dst->numberOfTracks(); iTrk++)
 */
hRefMult ->Fill(RefMult);
hVertexZ ->Fill(event->primaryVertex().Z());
hVertexXY->Fill(event->primaryVertex().X(), event->primaryVertex().Y());

} // end  for(Long64_t iEvent=0; iEvent<events2read; iEvent++)

       for(Int_t j = 0; j<9; j++)
     {

       l.Add(hPosSpectra[j]);
       l.Add(hNegSpectra[j]);
     }
     
      l.Write();
      hRefMult->Write();
      hVertexZ->Write();
      hVertexXY->Write();
      hNFitPoints->Write();
      hNFitPtOverPossPt->Write();
      hNFitdEdx->Write();
      hDCAz->Write();
      hDCAxy->Write();
      hDCAmagn->Write();
      hGPtOverPPt->Write();
      hNFitPVsEta2D->Write(); //!!
     ofile.Close();
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
     //delete [] hPosSpectra;
     //delete [] hNegSpectra;
     picoReader->Clear();
picoReader->Finish();
 return 0;
}


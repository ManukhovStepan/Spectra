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
#include "TH1F.h"
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

int TryAnalisys()
{
 // Инициализирую вспомогательные переменные, открываю файл
  TFile ofile("Spectra.root","RECREATE");
  StPicoDstReader *picoReader = new StPicoDstReader("/home/manukhov/DUBNA/STAR/st_physics*.picoDst.root");
  picoReader->Init();
  Int_t cent9;
  Int_t RefMult;
  Double_t posYields = 0;
  Double_t negYields = 0;
  Double_t NumCollCentPos = 0;
  Double_t NumCollPerPos = 0;
  Double_t NumCollCentNeg = 0;
  Double_t NumCollPerNeg = 0;
  Int_t NumCentPos = 0;
  Int_t NumCentNeg = 0;
  Int_t NumPerPos = 0;
  Int_t NumPerNeg = 0;
  Double_t RcpPos[10];
  Double_t RcpNeg[10];  
  Double_t RcpPosErr[10];
  Double_t RcpNegErr[10];  
  Double_t Pt[10], PtErr[10];
  Int_t NumBins = 20;
  Double_t events2average = 0.;

 //  Инициализирую гистограммы и графики
  TString hPos_names[9];
  TH1D *hPosYields[9];
  TString hNeg_names[9];
  TH1D *hNegYields[9];
  TGraphErrors *hPosRcp;
  TGraphErrors *hNegRcp;
  TMultiGraph* SpectraPos = new TMultiGraph();
  TMultiGraph* SpectraNeg = new TMultiGraph();
  for(Int_t i = 0; i < 9; i++)
  {
  	hPos_names[i] 	= "hPos_" + to_string(i+1);
  	hPosYields[i] 		= new TH1D(hPos_names[i],"Pos d^{2}N}/2piP_{T}dP_{T}deta distribution",NumBins, 0., 2.);
  	hNeg_names[i] 	= "hNeg_" + to_string(i+1);
  	hNegYields[i] 		= new TH1D(hNeg_names[i],"Neg d^{2}N/2piP_{T}dP_{T}deta distribution",NumBins, 0., 2.);
  };
  SpectraPos->SetTitle("Positive d^{2}N/2piP_{T}dP_{T}deta spectra; P_{T}, GeV/c; d^{2}N/2piP_{T}dP_{T}deta");
  SpectraNeg->SetTitle("Negative d^{2}N/2piP_{T}dP_{T}deta spectra; P_{T}, GeV/c; d^{2}N/2piP_{T}dP_{T}deta");

 // Начинаю считывание по событиям
  picoReader->SetStatus("Event*", 1);
  Long64_t events2read = picoReader->chain()->GetEntries();
  //if (events2read > 2000) events2read = 15000;
   for(Long64_t iEvent=0; iEvent<events2read; iEvent++) {
    cout << "Working on event #[" << (iEvent+1) << "/" << events2read << "]" << "\r";
    Bool_t readEvent = picoReader->readPicoEvent(iEvent);
    StPicoDst *dst = picoReader->picoDst();
    StPicoEvent *event = dst->event();
  // Сut-ы на события (event selection)
    Double_t rV = sqrt(pow((event->primaryVertex().X() - 0.099),2) + pow((event->primaryVertex().Y() + 0.173),2));
    if(		(abs(event->primaryVertex().Z())<50.) 	&&
		(rV<0.24)			
		
							)
 { 

    events2average += 1.; 

  // Определяю центральности
    RefMult = event->refMult();
    if      (RefMult > 295) {cent9 = 0;} 
    else if (RefMult > 246) {cent9 = 1;} 
    else if (RefMult > 168) {cent9 = 2;} 
    else if (RefMult > 113) {cent9 = 3;} 
    else if (RefMult > 72)  {cent9 = 4;} 
    else if (RefMult > 44)  {cent9 = 5;} 
    else if (RefMult > 25)  {cent9 = 6;} 
    else if (RefMult > 13)  {cent9 = 7;} 
    else if (RefMult > 6)   {cent9 = 8;} 

 // Счтиываю каждый трэк
    for(Int_t iTrk=0; iTrk<dst->numberOfTracks(); iTrk++) {
    StPicoTrack *picoTrack = dst->track(iTrk);
    if(!picoTrack) continue;

 // Сut-ы на трэки (track selection)
  //  if(picoTrack->isPrimary())  {
    if(  	(abs(picoTrack->pMom().Eta())<0.5) &&
		(picoTrack->gDCA(event->primaryVertex()).Mag()<1.) &&
		(picoTrack->nHitsFit()>16.) &&
		(((picoTrack->nHitsFit())/47.)>0.52) &&
		(picoTrack->nHitsDedx()>14.) &&
		(picoTrack->pMom().Pt()>0.2) &&
		((picoTrack->gMom().Pt() / picoTrack->pMom().Pt())>0.7) && 
		((picoTrack->gMom().Pt() / picoTrack->pMom().Pt())<1.4285) )  {   
				
 // Заполняю (2piPt)^-1*d2N/dPtdy гистограммы
    if(picoTrack->charge()>0)
    {
    posYields = 1./((2.*3.14159) *(picoTrack ->pMom().Pt()));
    hPosYields[cent9]-> Fill(picoTrack ->pMom().Pt(), posYields);
    }
    if(picoTrack->charge()<0)
    {
    negYields = 1./((2.*3.14159) *(picoTrack ->pMom().Pt()));
    hNegYields[cent9]-> Fill(picoTrack ->pMom().Pt(), negYields);
    }

  // Записываю сколько центральных и перефирийных столковений произошло
    if((cent9 < 1) && (picoTrack->charge()>0)) {
    NumCentPos += 1;
    }
    if((cent9 > 6) && (picoTrack->charge()>0)){
    NumPerPos += 1;
    }
    if((cent9 < 1) && (picoTrack->charge()<0)) {
    NumCentNeg += 1;
    }
    if((cent9 > 6) && (picoTrack->charge()<0)) {
    NumPerNeg += 1;    
    }
   

    /* cout << endl;   // для проверки катов
   cout << event->primaryVertex().Z() << endl;
   cout << picoTrack->pMom().Eta() << endl;
   cout << picoTrack->gDCA(event->primaryVertex()).Mag() << endl;
   cout << picoTrack->nHitsFit() << endl;
   cout << picoTrack->nHitsDedx() << endl;
   cout << picoTrack->pMom().Pt() << endl;*/
  

  } // end if (cuts)
//  } // end if (track is primary)
  } // end for(Int_t iTrk=0; iTrk<dst->numberOfTracks(); iTrk++) 
    



  } // end if (vertex is good)
} // end  for(Long64_t iEvent=0; iEvent<events2read; iEvent++)


 // Заполняю гистограммы (графики) для фактора ядерной модификации
    NumCollCentPos = NumCentPos / events2average;
    NumCollPerPos = NumPerPos / events2average;
    NumCollCentNeg = NumCentNeg / events2average;
    NumCollPerNeg = NumPerNeg / events2average;  
  

  /*   cout << endl;
     cout << NumCollCentPos << endl;   
     cout << NumCollPerPos << endl;   
     cout << NumCollCentNeg << endl;   
     cout << NumCollPerNeg << endl;   */


   Double_t x1, x2, y1, y2, err1c, err1p, err2c, err2p;
   for(Int_t i = 3; i<13; i++) {
   Pt[i-3] = 0.1*i - 0.05;
   PtErr[i-3]=0.015;
   x1 = hPosYields[0]->GetBinContent(i);
   y1 = hPosYields[7]->GetBinContent(i) + hPosYields[8]->GetBinContent(i);
   x2 = hNegYields[0]->GetBinContent(i);
   y2 = hNegYields[7]->GetBinContent(i) + hNegYields[8]->GetBinContent(i);
   err1c = hPosYields[0]->GetBinError(i);
   err1p = hPosYields[7]->GetBinError(i);
   err2c = hPosYields[0]->GetBinError(i);
   err2p = hPosYields[7]->GetBinError(i);

   if (y1 == 0) RcpPos[i-3] = 0;
   RcpPos[i-3] = (NumCollPerPos * x1) / (NumCollCentPos * y1);
   if (y2 == 0) RcpNeg[i-3] = 0;
   RcpNeg[i-3] = (NumCollPerPos * x2) / (NumCollCentPos * y2);
   RcpPosErr[i-3] = RcpPos[i-3] * sqrt( (err1c/x1)*(err1c/x1) + (err1p/y1)*(err1p/y1) );
   RcpNegErr[i-3] = RcpNeg[i-3] * sqrt( pow((err2c/x2),2) + pow((err2p/y2),2) );
 // cout << RcpPos[i-1] << endl;
 // cout << RcpNeg[i-1] << endl;
   }


  Int_t Num = 10;
  hPosRcp = new TGraphErrors(Num, Pt, RcpPos, PtErr, RcpPosErr);
  hNegRcp = new TGraphErrors(Num, Pt, RcpNeg, PtErr, RcpNegErr);
  hPosRcp->SetTitle("R_{CP} positive destribution; P_{T}, GeV/c; R_{CP}");
  hNegRcp->SetTitle("R_{CP} negative destribution; P_{T}, GeV/c; R_{CP}");
  hPosRcp->SetMarkerStyle(21);
  hNegRcp->SetMarkerStyle(21);
 // hPosRcp->Draw("AP");
 // hNegRcp->Draw("AP");
 /* TMultiGraph* RCP = new TMultiGraph();
  RCP->SetTitle("Positive and negative RCP spectra; P_{T}, GeV/c; Rcp");
  RCP->Add(hPosRcp, "AP");
  RCP->Add(hNegRcp, "AP");*/
 // ofile.GetList()->Add(hPosRcp, "AP");
 // ofile.GetList()->Add(hNegRcp, "AP");

  TMultiGraph* toSave1 = new TMultiGraph();
  toSave1->SetTitle("R_{CP} positive destribution; P_{T}, GeV/c; R_{CP}");
  toSave1->Add(hPosRcp, "AP");
  TMultiGraph* toSave2 = new TMultiGraph();
  toSave2->SetTitle("R_{CP} negative destribution; P_{T}, GeV/c; R_{CP}");
  toSave2->Add(hNegRcp, "AP");
  ofile.GetList()->Add(toSave1);
  ofile.GetList()->Add(toSave2);

  // Объединяю 2piPt)^-1*d2N/dPtdy спектры в один график 
   Double_t g1[20]; 
   Double_t g1err[20];
   Double_t g2[20];	
   Double_t g2err[20];	
   Double_t x[20];		
   Double_t xerr[20];	

  for(Int_t j = 0; j<9; j++) {
   for(Int_t i = 0; i<NumBins; i++) {
   x[i] = 0.1*i - 0.05;
   xerr[i]=0.025;
   g1[i] = hPosYields[j]->GetBinContent(i);
   g2[i] = hNegYields[j]->GetBinContent(i);
   g1err[i] = hPosYields[j]->GetBinError(i);
   g2err[i] = hPosYields[j]->GetBinError(i);
   if (j == 0) {
   g1[i] = g1[i]*3;
   g2[i] = g2[i]*3;
   }
   if (j == 1) {
   g1[i] = g1[i]*3;
   g2[i] = g2[i]*3;

   }
   }
   TGraphErrors* pos = new TGraphErrors(NumBins, x, g1, xerr, g1err);
   pos->SetMarkerStyle(21);
   pos->SetMarkerColor(j+1);
   TString name = "Au+Au centrality class:" + to_string(j);
   pos->SetTitle(name);
   SpectraPos->Add(pos, "AP");
   TGraphErrors* neg = new TGraphErrors(NumBins, x, g2, xerr, g2err);
   neg->SetMarkerStyle(21);
   neg->SetMarkerColor(j+1);
   neg->SetTitle(name);
   SpectraNeg->Add(neg, "AP");
   }


  ofile.GetList()->Add(SpectraPos);
  ofile.GetList()->Add(SpectraNeg);  

  ofile.Write();
  ofile.Close();

picoReader->Finish();



return 0;
}


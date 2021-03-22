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


int main()//const Char_t *outFileName=defaultOutFile)
{
TFile ofile("54GeV-Tot2.root","RECREATE");

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
//Double_t Charge;

TString hPos_names[9];
TString hNeg_names[9];
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
     //TH1D *CHARGE;

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
     hNFitPVsEta2D		= new TH2D("hNFitPVsEta2D", "Number of fit points vs. pseudo rapidity", 50, -1.5, 1.5, 50, 0., 50.);
     hSpectraVsEta2D	= new TH2D("hSpectraVsEta2D", "d^{2}N/2piP_{T}dP_{T}dEta distribution vs. pseudo rapidity", 20, -1.5, 1.5, 30, 0.2, 1.9);
     hSpectraVsNFitP2D	= new TH2D("hSpectraVsNFitP2D", "d^{2}N/2piP_{T}dP_{T}dEta distribution vs. number of fit points", 50, 0., 50., 30, 0.2, 1.9);
     //CHARGE		= new TH1D("Charge","Charge destribution",10, -1.5, 1.5);


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

 TChain chain("Track");
  chain.Add("/home/alisher/54Out/54-2/*.root");

 chain.SetBranchAddress("nFitP",&nFitP);
 chain.SetBranchAddress("nFitPOver",&nFitPOver);
 chain.SetBranchAddress("ndEdx",&ndEdx);
 chain.SetBranchAddress("DCAx",&DCAx);
 chain.SetBranchAddress("DCAy",&DCAy);
 chain.SetBranchAddress("DCAz",&DCAz);
 chain.SetBranchAddress("Yields",&Yields);
 chain.SetBranchAddress("DCAmagn",&DCAmagn);
 chain.SetBranchAddress("gPt",&gPt);
 chain.SetBranchAddress("pPt",&pPt);
 chain.SetBranchAddress("Eta",&Eta);
 chain.SetBranchAddress("absEta",&absEta);
// chain.SetBranchAddress("Charge",&Charge);

 TChain ch("Event");
  ch.Add("/home/alisher/54Out/54-2/*.root");
 ch.SetBranchAddress("RefMult",&RefMult);
 ch.SetBranchAddress("Vz",&Vz);
 ch.SetBranchAddress("Vx",&Vx);
 ch.SetBranchAddress("Vy",&Vy);

//----------------------------------------------------------------------
 Long64_t tentries = chain.GetEntries();

   for (Long64_t i=0;i<tentries;i++)
       {
         chain.GetEntry(i);

        // cout << Eta << "\r";
         hNFitPoints->Fill(nFitP);
         hNFitPtOverPossPt->Fill(nFitPOver);
         hNFitdEdx->Fill(ndEdx);
         hDCAz->Fill(DCAz);
         hDCAxy->Fill(DCAx, DCAy); 
         hDCAmagn->Fill(DCAmagn);
         hGPtOverPPt->Fill(gPt, pPt);
         if(absEta > 0.005)
         hNFitPVsEta2D->Fill(Eta, nFitP);
         hSpectraVsEta2D->Fill(Eta, pPt, Yields);
         hSpectraVsNFitP2D->Fill(nFitP, pPt, Yields);
        // CHARGE->Fill(Charge);
       }

   Long64_t eentries = ch.GetEntries();

   for (Long64_t i=0;i<eentries;i++)
       {
        ch.GetEntry(i);

        hRefMult->Fill(RefMult);
        hVertexZ->Fill(Vz);
        hVertexXY->Fill(Vx, Vy);
   }
     ofile.Write();
     ofile.Close();
   return 0;
}

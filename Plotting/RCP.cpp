#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <TString.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TMultiGraph.h>
#include <TGraphErrors.h>

// The input of the program is a file containing the non idenfied spectra of the 
// transverse momentum at 27GeV, which are called hPosSpectra_0, hPosSpectra_1, 
// hPosSpectra_2 and so on. Based on them, the nuclear modification 
// factor (RCP) is built. <Npart> and <Ncoll> are taken from the article 
// https://www.star.bnl.gov/protected/common/common2011/centrality/27GeV/index.html

using namespace std;


int RCP(Int_t mode = 1, TString FileName = "/home/manukhov/Dubna/files/st_27GeV.root")
{
//Getting information from the file --------------------------------------------------------------------------------
	TFile* File = new TFile(FileName, "READ");
	TH1D *hPos60_80;
	TH1D *hNeg60_80;
	File->GetObject("hPos60_80", hPos60_80);
	File->GetObject("hNeg60_80", hNeg60_80);   
	TH1D *hPos0_5;
	TH1D *hNeg0_5;
	File->GetObject("hPosSpectraRebin_8", hPos0_5);
	File->GetObject("hNegSpectraRebin_8", hNeg0_5);   
	TH1D* hCent9;
	File->GetObject("hNormCent9", hCent9);   
	
	Double_t NEvent0_5 = hCent9->GetBinContent(10);
	Double_t NEvent60_80 = hCent9->GetBinContent(2) + hCent9->GetBinContent(3);
	
	Int_t NumBins = hPos0_5->GetNbinsX();	
	
// Normalization by Nevent -------------------------------------------------------------------------------------------
cout << "here1" << endl;
	for(Int_t i = 0; i < NumBins; i++) {
		hPos0_5->SetBinContent(i, (hPos0_5->GetBinContent(i)		/  NEvent0_5  ));
		hPos0_5->SetBinError(i, (hPos0_5->GetBinError(i)		/  NEvent0_5  ));
		hNeg0_5->SetBinContent(i, (hNeg0_5->GetBinContent(i)		/  NEvent0_5  ));
		hNeg0_5->SetBinError(i, (hNeg0_5->GetBinError(i)		/  NEvent0_5  ));	
	
		hPos60_80->SetBinContent(i, (hPos60_80->GetBinContent(i)	/  NEvent60_80  ));
		hPos60_80->SetBinError(i, (hPos60_80->GetBinError(i)		/  NEvent60_80  ));
		hNeg60_80->SetBinContent(i, (hNeg60_80->GetBinContent(i)	/  NEvent60_80  ));
		hNeg60_80->SetBinError(i, (hNeg60_80->GetBinError(i)		/  NEvent60_80  ));
	}
	
/* 27 GeV number of binary collisions -------------------------------------------------------------------------------
	Double_t NcentPos = 841.47851, NcentErr = 28.39811;
	Double_t NperPos = 19.97044  , NperErr = 8.56423;
	Double_t NcentNeg = 841.47851;
	Double_t NperNeg = 19.97044;*/
	
// 19 GeV number of binary collisions -------------------------------------------------------------------------------
	Double_t NcentPos = 799.84113, NcentErr = 27.37286;
	Double_t NperPos = 18.91901 , NperErr = 6.86227;
	Double_t NcentNeg = 799.84113;
	Double_t NperNeg = 18.91901;



// Creating RCP histo -------------------------------------------------------------------------------------------------
	TH1D *hPosRCP;
	TH1D *hNegRCP;
	hPosRCP = (TH1D*)hPos0_5->Clone("PosRCP");
	hNegRCP = (TH1D*)hNeg0_5->Clone("NegRCP");
	hPosRCP->Divide(hPos60_80);
	hNegRCP->Divide(hNeg60_80);
	
// Normalization by <Ncoll> -------------------------------------------------------------------------------------------
	for(Int_t i = 0; i < NumBins; i++) {
		hPosRCP->SetBinContent(i, 	(hPosRCP->GetBinContent(i)*	NperPos/NcentPos));
		hPosRCP->SetBinError(  i, 	(hPosRCP->GetBinError(i)  *	NperPos/NcentPos));
		hNegRCP->SetBinContent(i, 	(hNegRCP->GetBinContent(i)*	NperNeg/NcentNeg));
		hNegRCP->SetBinError(  i, 	(hNegRCP->GetBinError(i)  *	NperNeg/NcentNeg));	
	}
	
// Drawing --------------------------------------------------------------------------------------------------------------

	hPosRCP->SetMarkerColor(2);
	hNegRCP->SetMarkerColor(4);
	hPosRCP->SetMarkerStyle(21);
	hNegRCP->SetMarkerStyle(21);
	TCanvas *c1 = new TCanvas();
	c1->SetTitle("R_{CP} destribution");	
	if(mode == 1) {
/*
		TH1D* hEmpty;
		hEmpty = (TH1D*)hPos0_5->Clone("PosRCP");
		for(Int_t i = 0; i < NumBins; i++) {
			hEmpty->SetBinContent(i,0);
			hEmpty->SetBinError(  i,0);
		}
		hEmpty->SetTitle("R_{CP} destribution, 27 GeV");
		hEmpty->Draw();
*/	
		hPosRCP->SetTitle("Positive R_{CP}");
		hPosRCP->SetXTitle("P_{T}, GeV/c");
		hPosRCP->SetYTitle("R_{CP}, [0-5/60-80]");
		hPosRCP->SetAxisRange(0.,2, "Y");
		hPosRCP->SetAxisRange(0.2,5, "X");	
		hPosRCP->Draw("e1p");
	
		hNegRCP->SetTitle("Negative R_{CP}");
		hNegRCP->SetAxisRange(0.,3.5, "Y");
		hNegRCP->SetAxisRange(0.2,5, "X");
		hNegRCP->Draw("same e1p");
		c1->BuildLegend();
	}
// Semisumm of RCPs -----------------------------------------------------------------------------------------------------
	if (mode == 2) {
	TH1D* hSpectra0_5;
	TH1D* hSpectra60_80;
//	hSpectra0_5->Add(hPos0_5, hNeg0_5);
//	hSpectra60_80->Add(hPos60_80, hNeg60_80);
	hSpectra0_5 = (TH1D*)hPos0_5->Clone("hSpectra0_5");
	hSpectra0_5->Add(hNeg0_5);
	hSpectra60_80 = (TH1D*)hPos60_80->Clone("hSpectra60_80");
	hSpectra60_80->Add(hNeg60_80);
	TH1D* hRCPaverage;
	hRCPaverage = (TH1D*)hSpectra0_5->Clone("RCP");
	hRCPaverage->Divide(hSpectra60_80);
	for(Int_t i = 0; i < NumBins; i++) {
		hRCPaverage->SetBinContent(i, 	(hRCPaverage->GetBinContent(i)*	NperPos/NcentPos));
		hRCPaverage->SetBinError(  i, 	(hRCPaverage->GetBinError(i)  *	NperPos/NcentPos));
	}		
	hRCPaverage->SetMarkerStyle(21);
	hRCPaverage->SetTitle("Average R_{CP} destribution, 27 GeV");
	hRCPaverage->SetXTitle("P_{T}, GeV/c");
	hRCPaverage->SetYTitle("R_{CP}, [0-5/60-80]");
	hRCPaverage->SetAxisRange(0.,2, "Y");
	hRCPaverage->SetAxisRange(0.2,6, "X");	
	hRCPaverage->Draw("e1p");
	gStyle->SetOptStat(0);
	c1->SetTitle("R_{CP} destribution");	
	c1->SetTicks();
	c1->SetGridx();
	c1->SetGridy();
	}
	
	gStyle->SetOptStat(0);
	c1->SetTicks();
	c1->SetGridx();
	c1->SetGridy();
	return 0;
}

#include <iostream>
#include <string>
#include <TString.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TMultiGraph.h>
#include <TGraphErrors.h>

using namespace std;

int RCPpos(TString FileName = "/home/manukhov/DUBNA/ROOT_FILES/AuAu27GeVOut.root")
{
// Grtting gistograms from file
	TCanvas *c1 = new TCanvas();
	c1->SetTitle("R_{CP} destribution");
	TFile* File = new TFile(FileName, "READ");
	TString hPos_names[9];
	TH1D *hPosSpectra[9];
	TString hNeg_names[9];
	TH1D *hNegSpectra[9];
	for(Int_t i = 0; i < 9; i++)
	{
		hPos_names[i] 	= "hPosSpectra_";
		hPos_names[i]  += i;
		File->GetObject(hPos_names[i], hPosSpectra[8-i]);
		hNeg_names[i] 	= "hNegSpectra_";
		hNeg_names[i]  += i;
		File->GetObject(hNeg_names[i], hNegSpectra[8-i]);
	};

// Find bins: Pt(BinA) = 0.2, Pt(BinB) = 4; -----------------------------------------------------------------------
	Double_t BinA = 0, BinB = hPosSpectra[0]->GetNbinsX();
	for(Int_t i = 0; i < hPosSpectra[0]->GetNbinsX(); i++)
	{
		if (hPosSpectra[0]->GetBinCenter(i) < 0.2)
			BinA += 1; 
		if (hPosSpectra[0]->GetBinCenter(hPosSpectra[0]->GetNbinsX() - i) > 4)
			BinB -= 1;
	};
	Int_t NumBins = BinB - BinA + 1; 

// Creating and filling RCP histogram -----------------------------------------------------------------------------
	TH1D *hPosRCP;
	TH1D *hNegRCP;
	hPosRCP	= new TH1D("PosRCP","R_{CP} positive destribution 27 GeV; P_{T}, GeV/c; R_{CP}",NumBins, 0.2, 4);
	hNegRCP = new TH1D("NegRCP","R_{CP} negative destribution; P_{T}, GeV/c; R_{CP}",NumBins, 0.2, 4);
	Double_t Ncent = 841.47851, NerrCent = 26.67759;
	Double_t Nper = 19.97044 , NerrPer = 8.56423;

	for(Int_t i = 0; i < NumBins; i++) {


		Double_t ContCentPos = hPosSpectra[0]->GetBinContent(i+BinA);
		Double_t ContPerPos = hPosSpectra[7]->GetBinContent(i+BinA) + hPosSpectra[8]->GetBinContent(i+BinA);
		Double_t ContCentNeg = hNegSpectra[0]->GetBinContent(i+BinA);
		Double_t ContPerNeg = hNegSpectra[7]->GetBinContent(i+BinA) + hNegSpectra[8]->GetBinContent(i+BinA);
		hPosRCP->SetBinContent(i, (Nper*ContCentPos)/(Ncent*ContPerPos));
		hNegRCP->SetBinContent(i, (Nper*ContCentNeg)/(Ncent*ContPerNeg));


		Double_t ErrCentPos = hPosSpectra[0]->GetBinError(i+BinA);
		Double_t ErrPerPos = sqrt(pow((hPosSpectra[7]->GetBinError(i+BinA)),2) + pow((hPosSpectra[8]->GetBinError(i+BinA)),2));
		Double_t ErrCentNeg = hNegSpectra[0]->GetBinError(i+BinA);
		Double_t ErrPerNeg = sqrt(pow((hNegSpectra[7]->GetBinError(i+BinA)),2) + pow((hNegSpectra[8]->GetBinError(i+BinA)),2));
		hPosRCP->SetBinError(i, (Nper*ContCentPos)/(Ncent*ContPerPos) * sqrt(pow((ErrCentPos/ContCentPos),2)+pow((ErrPerPos/ContPerPos),2)/*+pow((NerrCent/Ncent),2)+pow((NerrPer/Nper),2)*/));
		hNegRCP->SetBinError(i, (Nper*ContCentNeg)/(Ncent*ContPerNeg) * sqrt(pow((ErrCentNeg/ContCentNeg),2)+pow((ErrPerNeg/ContPerNeg),2)/*+pow((NerrCent/Ncent),2)+pow((NerrPer/Nper),2)*/));            
	}

// Drqwing historgams --------------------------------------------------------------------------------------------
	hPosRCP->SetMarkerColor(1);
	hNegRCP->SetMarkerColor(1);
	hPosRCP->SetMarkerStyle(36);
	hNegRCP->SetMarkerStyle(36);
	hPosRCP->SetAxisRange(0.01,0.5, "Y");
	hPosRCP->SetAxisRange(0.2,4, "X");
	hPosRCP->Draw("e1p");
	gStyle->SetOptStat(0);
	c1->SetTicks();
	c1->SetGridx();
	c1->SetGridy();
	c1->Update();

	return 0;
}

#include <iostream>
#include <string>
#include <TString.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TMultiGraph.h>
#include <TGraphErrors.h>

// The input of the program is a file containing the non idenfied spectra of the 
// transverse momentum at 27GeV, which are called hPosSpectra_1, hPosSpectra_2, 
// hPosSpectra_3 and so on. Based on them, the nuclear modification 
// factor (RCP) is built. <Npart> and <Ncoll> are taken from the article 
// https://www.star.bnl.gov/protected/common/common2011/centrality/27GeV/index.html

using namespace std;


int RCP_27GeV_pos(TString FileName = "/home/manukhov/DUBNA/ROOT_FILES/Total-100Mill(1).root")
{
    TCanvas *c1 = new TCanvas();
    c1->SetTitle("R_{CP} destribution");
    TFile* File = new TFile(FileName, "READ");
    TString hPos_names[9];
    TH1D *hPosSpectra[9];
    TString hNeg_names[9];
    TH1D *hNegSpectra[9];

// Getting histograms from the file

    for(Int_t i = 0; i < 9; i++)
    {
          hPos_names[i] 	= "hPosSpectra_";
          hPos_names[i]  += i+1;
          File->GetObject(hPos_names[i], hPosSpectra[8-i]);
          hNeg_names[i] 	= "hNegSpectra_";
          hNeg_names[i]  += i+1;
          File->GetObject(hNeg_names[i], hNegSpectra[8-i]);
    };


    // Find bins: Pt(BinA) = 0.2, Pt(BinB) = 4; 
    Double_t BinA = 0, BinB = hPosSpectra[0]->GetNbinsX();
    for(Int_t i = 0; i < hPosSpectra[0]->GetNbinsX(); i++)
    {
	 if (hPosSpectra[0]->GetBinCenter(i) < 0.2)
	 BinA += 1; 
	 if (hPosSpectra[0]->GetBinCenter(hPosSpectra[0]->GetNbinsX() - i) > 4)
         BinB -= 1;
    };
    Int_t NumBins = BinB - BinA + 1; 

    Double_t Norm[9] = {0,0,0,0,0,0,0,0,0};
    for(Int_t j = 0; j < 9; j++) {
    for(Int_t i = 3; i < hPosSpectra[j]->GetNbinsX(); i++) {
        Norm[j] += hPosSpectra[j]->GetBinContent(i);
    }
    }

    for(Int_t j = 0; j < 9; j++) {
    for(Int_t i = 0; i < hPosSpectra[j]->GetNbinsX(); i++) {
	hPosSpectra[j]->SetBinContent(i, (hPosSpectra[j]->GetBinContent(i)/Norm[j]));
	hPosSpectra[j]->SetBinError(i, (hPosSpectra[j]->GetBinError(i)/Norm[j]));
    }
    }

   /* Double_t edges[NumBins + 1];
    for(Int_t i = 0; i < NumBins+1; i++) {
        edges[i] = -0.8 + pow(2,log(2.8)*i/(NumBins*log(2)) );
    } */
    TH1D *hPosRCP;
    TH1D *hNegRCP;
    hPosRCP	= new TH1D("PosRCP","R_{CP} positive destribution 27 GeV; P_{T}, GeV/c; R_{CP}",NumBins, 0.2, 4);
    hNegRCP 	= new TH1D("NegRCP","R_{CP} negative destribution; P_{T}, GeV/c; R_{CP}",NumBins, 0.2, 4);



    Double_t NcentPos = 1.4, NcentErr = 26.67759;
    Double_t NperPos = 1  , NperErr = 8.56423;
    Double_t NcentNeg = 761.08736;
    Double_t NperNeg = 19.97044;


    for(Int_t i = 0; i < NumBins; i++) {
        Double_t ContCentPos = hPosSpectra[0]->GetBinContent(i+BinA);
        Double_t ContPerPos = (Norm[7]*hPosSpectra[7]->GetBinContent(i+BinA) + Norm[8]*hPosSpectra[8]->GetBinContent(i+BinA))/(Norm[7]+Norm[8]);
        Double_t ContCentNeg = hNegSpectra[0]->GetBinContent(i+BinA);
        Double_t ContPerNeg = (Norm[7]*hNegSpectra[7]->GetBinContent(i+BinA) + Norm[8]*hNegSpectra[8]->GetBinContent(i+BinA))/(Norm[7]+Norm[8]);
        hPosRCP->SetBinContent(i, (NperPos*ContCentPos)/(NcentPos*ContPerPos));
        hNegRCP->SetBinContent(i, (NperNeg*ContCentNeg)/(NcentNeg*ContPerNeg));
        Double_t ErrCentPos = hPosSpectra[0]->GetBinError(i+BinA);
        Double_t ErrPerPos = sqrt(pow((hPosSpectra[7]->GetBinError(i+BinA)),2) + pow((hPosSpectra[8]->GetBinError(i+BinA)),2));
        Double_t ErrCentNeg = hNegSpectra[0]->GetBinError(i+BinA);
        Double_t ErrPerNeg = sqrt(pow((hNegSpectra[7]->GetBinError(i+BinA)),2) + pow((hNegSpectra[8]->GetBinError(i+BinA)),2));
        hPosRCP->SetBinError(i, (NperPos*ContCentPos)/(NcentPos*ContPerPos) * sqrt(pow((ErrCentPos/ContCentPos),2)+pow((ErrPerPos/ContPerPos),2)));
        hNegRCP->SetBinError(i, (NperNeg*ContCentNeg)/(NcentNeg*ContPerNeg) * sqrt(pow((ErrCentNeg/ContCentNeg),2)+pow((ErrPerNeg/ContPerNeg),2)));
    }

    hPosRCP->SetMarkerColor(1);
    hNegRCP->SetMarkerColor(1);
    hPosRCP->SetMarkerStyle(36);
    hNegRCP->SetMarkerStyle(21);
    hPosRCP->SetAxisRange(0.1,10, "Y");
    hPosRCP->SetAxisRange(0.2,4, "X");
    hPosRCP->Draw("e1p");


  /*      hPosRCP->SetAxisRange(0.2,2, "Y");
    hPosRCP->SetAxisRange(0.2,8, "X");
    hPosSpectra[0]->SetAxisRange(0.008,100000000, "Y");
    hPosSpectra[0]->SetAxisRange(0.3,9, "X");
    hNegSpectra[0]->SetAxisRange(0.04,100000000, "Y");
    hNegSpectra[0]->SetAxisRange(0.3,9, "X");*/

    c1->BuildLegend();
    c1->SetTicks();
    c1->SetGridx();
    c1->SetGridy();
    c1->Update();
    return 0;
}

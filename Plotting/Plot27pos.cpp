#include <iostream>
#include <string>
#include <TString.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TMultiGraph.h>
#include <TGraphErrors.h>


using namespace std;

int Plot27pos()
{
    TCanvas *c1 = new TCanvas();
    c1->SetTitle("Positive d^{2}N/2piP_{T}dP_{T}dEta spectra");
    TFile* File = new TFile("/home/manukhov/DUBNA/ROOT_FILES/Total-100Mill(1).root", "READ");
    TString hPos_names[9];
    TH1D *hPosSpectra[9];
    TString hNeg_names[9];
    TH1D *hNegSpectra[9];
    for(Int_t i = 0; i < 9; i++)
    {
          hPos_names[i] 	= "hPosSpectra_";
          hPos_names[i]  += i+1;
          File->GetObject(hPos_names[i], hPosSpectra[8-i]);
          hNeg_names[i] 	= "hNegSpectra_";
          hNeg_names[i]  += i+1;
          File->GetObject(hNeg_names[i], hNegSpectra[8-i]);
    };
   // hNegSpectra[0]->Add(hNegSpectra[0], );
  //  hNegSpectra[1]->Add(hNegSpectra[1], 10);

   // Double_t cont = hNegSpectra[0]->GetBinContent(10);
cout << "i am here" << endl;
    Double_t Norm[9] = {0,0,0,0,0,0,0,0,0};
    for(Int_t j = 0; j < 9; j++) {
    for(Int_t i = 3; i < hPosSpectra[j]->GetNbinsX(); i++) {
        Norm[j] += hPosSpectra[j]->GetBinContent(i);
    }
//cout << "i am here " << Norm[j] << endl;
    }
//cout << "i am here " << Norm[0] << endl;/*
/*
    for(Int_t j = 0; j < 9; j++) {
    for(Int_t i = 0; i < hPosSpectra[j]->GetNbinsX(); i++) {
	hPosSpectra[j]->SetBinContent(i, (hPosSpectra[j]->GetBinContent(i)/Norm[j]));
	hPosSpectra[j]->SetBinError(i, (hPosSpectra[j]->GetBinError(i)/Norm[j]));
	hPosSpectra[j]->SetBinContent(hPosSpectra[j]->GetNbinsX(), 0);
	hPosSpectra[j]->SetBinError(hPosSpectra[j]->GetNbinsX(), 0);
    }
    }*/

    for(Int_t j = 0; j < 9; j++) {
    for(Int_t i = 0; i < hPosSpectra[j]->GetNbinsX(); i++) {
	hPosSpectra[j]->SetBinContent(i, (hPosSpectra[j]->GetBinContent(i)/pow(10,j)));
	hPosSpectra[j]->SetBinError(i, (hPosSpectra[j]->GetBinError(i)/pow(10,j)));
    }
    }

    TString centralitynames[9];
    centralitynames[0] = "0-5%"; centralitynames[1] = "5-10%/10"; centralitynames[2] = "10-20%/10^{2}"; centralitynames[3] = "20-30%/10^{3}";
    centralitynames[4] = "30-40%/10^{4}"; centralitynames[5] = "40-50%/10^{5}"; centralitynames[6] = "50-60%/10^{6}"; centralitynames[7] = "60-70%/10^{7}";
    centralitynames[8] = "70-80%/10^{8}";

    hPosSpectra[0]->SetMarkerColor(1);
    hNegSpectra[0]->SetMarkerColor(1);
    hPosSpectra[0]->SetMarkerStyle(21);
    hNegSpectra[0]->SetMarkerStyle(21);
    hPosSpectra[0]->Draw("e1p");
  //  hNegSpectra[0]->Draw("e1p");
    TString name = "Centrality: " + centralitynames[0];
    hPosSpectra[0]->SetTitle(name);
    hNegSpectra[0]->SetTitle(name);
    hPosSpectra[0]->SetXTitle("P_{T}, GeV/c");
    hPosSpectra[0]->SetYTitle("d^{2}N/2piP_{T}dP_{T}dEta spectra");
    hNegSpectra[0]->SetXTitle("P_{T}, GeV/c");
    hNegSpectra[0]->SetYTitle("d^{2}N/2piP_{T}dP_{T}dEta spectra");

    hPosSpectra[0]->SetAxisRange(0.00000000001,1000000, "Y");
    hPosSpectra[0]->SetAxisRange(0.3, 10, "X");
    hNegSpectra[0]->SetAxisRange(0.07,300000, "Y");
    hNegSpectra[0]->SetAxisRange(0.3, 3, "X");

    for(Int_t i = 1; i < 9; i++)
    {
        hPosSpectra[i]->Draw("SAME e1p");
      //  hNegSpectra[i]->Draw("SAME e1p");
        //hPosSpectra[i]->SetMarkerStyle(106+i);
        hNegSpectra[i]->SetMarkerStyle(21);
        hPosSpectra[i]->SetMarkerColor(i+1);
        hNegSpectra[i]->SetMarkerColor(i+1);


        TString name = "Centrality: " + centralitynames[i];
        hPosSpectra[i]->SetTitle(name);
        hNegSpectra[i]->SetTitle(name);

    }
        hPosSpectra[0]->SetMarkerStyle(20);
        hPosSpectra[1]->SetMarkerStyle(21);
        hPosSpectra[2]->SetMarkerStyle(22);
        hPosSpectra[3]->SetMarkerStyle(23);
        hPosSpectra[4]->SetMarkerStyle(29);
        hPosSpectra[5]->SetMarkerStyle(33);
        hPosSpectra[6]->SetMarkerStyle(34);
        hPosSpectra[7]->SetMarkerStyle(45);
        hPosSpectra[8]->SetMarkerStyle(43);
    for(Int_t i = 0; i < 9; i++)
    {
	hPosSpectra[i]->SetMarkerSize(1.5);
    }

    c1->BuildLegend();
    c1->SetTicks();
    c1->SetLogy();
    c1->SetGridx();
    c1->SetGridy();
    c1->Update();
    return 0;
}

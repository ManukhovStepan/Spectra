#include <iostream>
#include <string>
#include <TString.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TMultiGraph.h>
#include <TGraphErrors.h>


using namespace std;

int main()
{
    TCanvas *c1 = new TCanvas();
    c1->SetTitle("Positive d^{2}N/2piP_{T}dP_{T}dEta spectra");
    TFile* File = new TFile("/home/manukhov/DUBNA/ROOT_FILES/SpecTotal-27.root", "READ");
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


    for(Int_t i = 0; i < hPosSpectra[0]->GetNbinsX(); i++) {
        Double_t cont_pos = hPosSpectra[0]->GetBinContent(i);
        cont_pos = cont_pos*4;
        hPosSpectra[0]->SetBinContent(i, cont_pos);
        cont_pos = hPosSpectra[1]->GetBinContent(i);
        cont_pos = cont_pos*3;
        hPosSpectra[1]->SetBinContent(i, cont_pos);
      //  Double_t proverka = hPosSpectra[0]->GetBinContent(i);
      //  cout << proverka << endl;
        Double_t cont_neg = hNegSpectra[0]->GetBinContent(i);
        cont_neg = cont_neg*8;
        hNegSpectra[0]->SetBinContent(i, cont_neg);
        cont_neg = hNegSpectra[1]->GetBinContent(i);
        cont_neg = cont_neg*3;
        hNegSpectra[1]->SetBinContent(i, cont_neg);
    }

    TString centralitynames[9];
    centralitynames[0] = "0-5% x4"; centralitynames[1] = "5-10% x3"; centralitynames[2] = "10-20%"; centralitynames[3] = "20-30%";
    centralitynames[4] = "30-40%"; centralitynames[5] = "40-50%"; centralitynames[6] = "50-60%"; centralitynames[7] = "60-80%";
    centralitynames[8] = "80-100%";

    hPosSpectra[0]->SetMarkerColor(1);
    hNegSpectra[0]->SetMarkerColor(1);
    hPosSpectra[0]->SetMarkerStyle(21);
    hNegSpectra[0]->SetMarkerStyle(21);
    hPosSpectra[0]->Draw("e1p");
  //  hNegSpectra[0]->Draw("SAME e1p");
    TString name = "Au+Au centrality class: " + centralitynames[0];
    hPosSpectra[0]->SetTitle(name);
    hNegSpectra[0]->SetTitle(name);
    hPosSpectra[0]->SetXTitle("P_{T}, GeV/c");
    hPosSpectra[0]->SetYTitle("d^{2}N/2piP_{T}dP_{T}dEta spectra");
    hNegSpectra[0]->SetXTitle("P_{T}, GeV/c");
    hNegSpectra[0]->SetYTitle("d^{2}N/2piP_{T}dP_{T}dEta spectra");

    hPosSpectra[0]->SetAxisRange(0.008,100000000, "Y");
    hPosSpectra[0]->SetAxisRange(0.3,9, "X");
    hNegSpectra[0]->SetAxisRange(0.04,100000000, "Y");
    hNegSpectra[0]->SetAxisRange(0.3,9, "X");

    for(Int_t i = 1; i < 9; i++)
    {
        hPosSpectra[i]->Draw("SAME e1p");
     //   hNegSpectra[i]->Draw("SAME e1p");
        hPosSpectra[i]->SetMarkerStyle(21);
        hNegSpectra[i]->SetMarkerStyle(21);
        hPosSpectra[i]->SetMarkerColor(i+1);
        hNegSpectra[i]->SetMarkerColor(i+1);


        TString name = "Au+Au centrality class: " + centralitynames[i];
        hPosSpectra[i]->SetTitle(name);
        hNegSpectra[i]->SetTitle(name);

    }

    c1->BuildLegend();
    c1->SetTicks();
    c1->SetLogy();
    c1->SetGridx();
    c1->SetGridy();
    c1->Update();
    return 0;
}

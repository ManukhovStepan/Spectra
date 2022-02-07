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
    TFile* File = new TFile("/home/manukhov/DUBNA/ROOT_FILES/19GeV.root", "READ");
    TString hPos_names[9];
    TH1D *hPosSpectra[9];
    TString hNeg_names[9];
    TH1D *hNegSpectra[9];
    for(Int_t i = 0; i < 9; i++)
    {
          hPos_names[i] 	= "hPosSpectra_";
          hPos_names[i]  += i;
          File->GetObject(hPos_names[i], hPosSpectra[i]);
          hNeg_names[i] 	= "hNegSpectra_";
          hNeg_names[i]  += i;
          File->GetObject(hNeg_names[i], hNegSpectra[i]);
    };

	const Int_t NBINS = 35;
	Double_t edges[NBINS + 1] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.75, 3.0, 3.5, 4.0, 4.5, 5.0, 5.5, 6.0, 6.5, 7.0};
	TString hPos_namesUneven[9];
	TH1D *hPosSpectraUneven[9];
	TString hNeg_namesUneven[9];
	TH1D *hNegSpectraUneven[9];
	for(Int_t i = 0; i < 9; i++)
	{
		hPos_namesUneven[i] 	= "hPosSpectraUneven_";
		hPos_namesUneven[i]  += i;
		hPosSpectraUneven[i] = new TH1D(hPos_namesUneven[i], hPos_namesUneven[i], NBINS, edges);
		hNeg_namesUneven[i] 	= "hNegSpectraUneven_";
		hNeg_namesUneven[i]  += i;
		hNegSpectraUneven[i] = new TH1D(hNeg_namesUneven[i], hNeg_namesUneven[i], NBINS, edges);
	};

//cout << "1" << endl;
	for(Int_t j = 0; j < 9; j++) {
	for(Int_t i = 0; i < 25; i++) {
		hPosSpectraUneven[j]->SetBinContent(i, (hPosSpectra[j]->GetBinContent(i))  );
		hPosSpectraUneven[j]->SetBinError(i, (hPosSpectra[j]->GetBinError(i))  );
//cout << "2" << endl;
		hNegSpectraUneven[j]->SetBinContent(i, (hNegSpectra[j]->GetBinContent(i))  );
		hNegSpectraUneven[j]->SetBinError(i, (hNegSpectra[j]->GetBinError(i))  );
    }
		Double_t meanPos25 = (	hPosSpectra[j]->GetBinContent(25)*hPosSpectra[j]->GetBinCenter(25) 
						+ 	hPosSpectra[j]->GetBinContent(26)*hPosSpectra[j]->GetBinCenter(26)
						+0.5*hPosSpectra[j]->GetBinContent(27)*hPosSpectra[j]->GetBinCenter(27))/(2.5*hPosSpectraUneven[j]->GetBinCenter(25));
		Double_t sigmaPos25 = sqrt( 	  pow(hPosSpectra[j]->GetBinError(25),2) 
							+ 	  pow(hPosSpectra[j]->GetBinError(26),2) 
							+ 0.25*pow(hPosSpectra[j]->GetBinError(27),2) )/2.5;
		hPosSpectraUneven[j]->SetBinContent(25, meanPos25  );
		hPosSpectraUneven[j]->SetBinError(25, sigmaPos25  );

		Double_t meanNeg25 = (	hNegSpectra[j]->GetBinContent(25)*hNegSpectra[j]->GetBinCenter(25) 
						+ 	hNegSpectra[j]->GetBinContent(26)*hNegSpectra[j]->GetBinCenter(26)
						+0.5*hNegSpectra[j]->GetBinContent(27)*hNegSpectra[j]->GetBinCenter(27))/(2.5*hNegSpectraUneven[j]->GetBinCenter(25));
		Double_t sigmaNeg25 = sqrt( 	  pow(hNegSpectra[j]->GetBinError(25),2) 
							+ 	  pow(hNegSpectra[j]->GetBinError(26),2) 
							+ 0.25*pow(hNegSpectra[j]->GetBinError(27),2) )/2.5;
		hNegSpectraUneven[j]->SetBinContent(25, meanNeg25  );
		hNegSpectraUneven[j]->SetBinError(25,  sigmaNeg25 );



		Double_t meanPos26 = (	hPosSpectra[j]->GetBinContent(29)*hPosSpectra[j]->GetBinCenter(29)
						+ 	hPosSpectra[j]->GetBinContent(28)*hPosSpectra[j]->GetBinCenter(28)
						+0.5*hPosSpectra[j]->GetBinContent(27)*hPosSpectra[j]->GetBinCenter(27))/(2.5*hPosSpectraUneven[j]->GetBinCenter(26));
		Double_t sigmaPos26 = sqrt( 	  pow(hPosSpectra[j]->GetBinError(29),2) 
							+ 	  pow(hPosSpectra[j]->GetBinError(28),2) 
							+ 0.25*pow(hPosSpectra[j]->GetBinError(27),2) )/2.5;
		hPosSpectraUneven[j]->SetBinContent(26, meanPos26  );
		hPosSpectraUneven[j]->SetBinError(26, sigmaPos26  );

		Double_t meanNeg26 = (	hNegSpectra[j]->GetBinContent(29)*hNegSpectra[j]->GetBinCenter(29) 
						+ 	hNegSpectra[j]->GetBinContent(28)*hNegSpectra[j]->GetBinCenter(28)
						+0.5*hNegSpectra[j]->GetBinContent(27)*hNegSpectra[j]->GetBinCenter(27))/(2.5*hNegSpectraUneven[j]->GetBinCenter(26));
		Double_t sigmaNeg26 = sqrt( 	  pow(hNegSpectra[j]->GetBinError(29),2) 
							+ 	  pow(hNegSpectra[j]->GetBinError(28),2) 
							+ 0.25*pow(hNegSpectra[j]->GetBinError(27),2) )/2.5;
		hNegSpectraUneven[j]->SetBinContent(26, meanNeg26  );
		hNegSpectraUneven[j]->SetBinError(26,  sigmaNeg26 );
	
	Int_t k = 0;
	for(Int_t i = 27; i < 35; i++) {
	//	cout << "4" << endl;
		Double_t meanPos = (	hPosSpectra[j]->GetBinContent(i + k + 3)*hPosSpectra[j]->GetBinCenter(i + k + 3) 
						  + 	hPosSpectra[j]->GetBinContent(i + k + 4)*hPosSpectra[j]->GetBinCenter(i + k + 4) 
						  +	hPosSpectra[j]->GetBinContent(i + k + 5)*hPosSpectra[j]->GetBinCenter(i + k + 5) 
						  + 	hPosSpectra[j]->GetBinContent(i + k + 6)*hPosSpectra[j]->GetBinCenter(i + k + 6) 
						  +	hPosSpectra[j]->GetBinContent(i + k + 7)*hPosSpectra[j]->GetBinCenter(i + k + 7))/(5.0*hPosSpectraUneven[j]->GetBinCenter(i));
		Double_t sigmaPos = sqrt(pow(hPosSpectra[j]->GetBinError(i + k + 3),2)
						  + 	pow(hPosSpectra[j]->GetBinError(i + k + 4),2)
						  +	pow(hPosSpectra[j]->GetBinError(i + k + 5),2)
						  + 	pow(hPosSpectra[j]->GetBinError(i + k + 6),2)
						  +	pow(hPosSpectra[j]->GetBinError(i + k + 7),2)) /5.0;
		//cout << "meanPos " << meanPos << endl;
		hPosSpectraUneven[j]->SetBinContent(i, meanPos );
		hPosSpectraUneven[j]->SetBinError(i, sigmaPos );

		Double_t meanNeg = (	hNegSpectra[j]->GetBinContent(i + k + 3)*hNegSpectra[j]->GetBinCenter(i + k + 3) 
						  + 	hNegSpectra[j]->GetBinContent(i + k + 4)*hNegSpectra[j]->GetBinCenter(i + k + 4) 
						  +	hNegSpectra[j]->GetBinContent(i + k + 5)*hNegSpectra[j]->GetBinCenter(i + k + 5) 
						  + 	hNegSpectra[j]->GetBinContent(i + k + 6)*hNegSpectra[j]->GetBinCenter(i + k + 6) 
						  +	hNegSpectra[j]->GetBinContent(i + k + 7)*hNegSpectra[j]->GetBinCenter(i + k + 7))/(5.0*hNegSpectraUneven[j]->GetBinCenter(i));
		Double_t sigmaNeg = sqrt(pow(hNegSpectra[j]->GetBinError(i + k + 3),2)
						  + 	pow(hNegSpectra[j]->GetBinError(i + k + 4),2)
						  +	pow(hNegSpectra[j]->GetBinError(i + k + 5),2)
						  + 	pow(hNegSpectra[j]->GetBinError(i + k + 6),2)
						  +	pow(hNegSpectra[j]->GetBinError(i + k + 7),2)) /5.0;
		hNegSpectraUneven[j]->SetBinContent(i, meanNeg  );
		hNegSpectraUneven[j]->SetBinError(i, sigmaNeg  );
		k+=4;
    }
    }

    for(Int_t j = 0; j < 9; j++) {
    for(Int_t i = 0; i < hPosSpectra[j]->GetNbinsX(); i++) {
	hPosSpectra[j]->SetBinContent(i, (hPosSpectra[j]->GetBinContent(i)/pow(3,j)));
	hPosSpectra[j]->SetBinError(i, (hPosSpectra[j]->GetBinError(i)/pow(3,j)));

	hNegSpectra[j]->SetBinContent(i, (hNegSpectra[j]->GetBinContent(i)/pow(3,j)));
	hNegSpectra[j]->SetBinError(i, (hNegSpectra[j]->GetBinError(i)/pow(3,j)));
    }
    }
  /*  for(Int_t i = 0; i < hNegSpectra[0]->GetNbinsX(); i++) {
	hNegSpectra[0]->SetBinContent(i, (hNegSpectra[0]->GetBinContent(i)*3));
	hNegSpectra[0]->SetBinError(i, (hNegSpectra[0]->GetBinError(i)*3));
    }*/
//cout << "5" << endl;
    TString centralitynames[9];
    centralitynames[0] = "0-5%"; centralitynames[1] = "5-10%/3"; centralitynames[2] = "10-20%/3^{2}"; centralitynames[3] = "20-30%/3^{3}";
    centralitynames[4] = "30-40%/3^{4}"; centralitynames[5] = "40-50%/3^{5}"; centralitynames[6] = "50-60%/3^{6}"; centralitynames[7] = "60-70%/3^{7}";
    centralitynames[8] = "70-80%/3^{8}";



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

    hPosSpectra[0]->SetAxisRange(0.000000005,10000000, "Y");
    hPosSpectra[0]->SetAxisRange(0.1, 8.5, "X");
    hNegSpectra[0]->SetAxisRange(0.000000005,50000000, "Y");
    hNegSpectra[0]->SetAxisRange(0.1, 8.5, "X");

    for(Int_t i = 1; i < 9; i++)
    {
        hPosSpectra[i]->Draw("SAME e1p");
       // hNegSpectra[i]->Draw("SAME e1p");
        //hPosSpectra[i]->SetMarkerStyle(106+i);
       // hNegSpectra[i]->SetMarkerStyle(21);
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

        hNegSpectra[0]->SetMarkerStyle(20);
        hNegSpectra[1]->SetMarkerStyle(21);
        hNegSpectra[2]->SetMarkerStyle(22);
        hNegSpectra[3]->SetMarkerStyle(23);
        hNegSpectra[4]->SetMarkerStyle(29);
        hNegSpectra[5]->SetMarkerStyle(33);
        hNegSpectra[6]->SetMarkerStyle(34);
        hNegSpectra[7]->SetMarkerStyle(45);
        hNegSpectra[8]->SetMarkerStyle(43);
    for(Int_t i = 0; i < 9; i++)
    {
	hPosSpectra[i]->SetMarkerSize(1.5);
	hNegSpectra[i]->SetMarkerSize(1.5);
    }

/*
//cout << "6" << endl;
    hPosSpectraUneven[0]->SetMarkerColor(1);
    hNegSpectraUneven[0]->SetMarkerColor(1);
    hPosSpectraUneven[0]->SetMarkerStyle(21);
    hNegSpectraUneven[0]->SetMarkerStyle(21);
    hPosSpectraUneven[0]->Draw("e1p");
  //  hNegSpectra[0]->Draw("e1p");
    TString name = "Centrality: " + centralitynames[0];
    hPosSpectraUneven[0]->SetTitle(name);
    hNegSpectraUneven[0]->SetTitle(name);
    hPosSpectraUneven[0]->SetXTitle("P_{T}, GeV/c");
    hPosSpectraUneven[0]->SetYTitle("d^{2}N/2piP_{T}dP_{T}dEta spectra");
    hNegSpectraUneven[0]->SetXTitle("P_{T}, GeV/c");
    hNegSpectraUneven[0]->SetYTitle("d^{2}N/2piP_{T}dP_{T}dEta spectra");

    hPosSpectraUneven[0]->SetAxisRange(0.00000001,10000000, "Y");
    hPosSpectraUneven[0]->SetAxisRange(0, 7., "X");
    hNegSpectraUneven[0]->SetAxisRange(0.00000001,10000000, "Y");
    hNegSpectraUneven[0]->SetAxisRange(0, 7., "X");

    for(Int_t i = 1; i < 9; i++)
    {
        hPosSpectraUneven[i]->Draw("SAME e1p");
       // hNegSpectra[i]->Draw("SAME e1p");
        //hPosSpectra[i]->SetMarkerStyle(106+i);
       // hNegSpectra[i]->SetMarkerStyle(21);
        hPosSpectraUneven[i]->SetMarkerColor(i+1);
        hNegSpectraUneven[i]->SetMarkerColor(i+1);


        TString name = "Centrality: " + centralitynames[i];
        hPosSpectraUneven[i]->SetTitle(name);
        hNegSpectraUneven[i]->SetTitle(name);

    }
        hPosSpectraUneven[0]->SetMarkerStyle(20);
        hPosSpectraUneven[1]->SetMarkerStyle(21);
        hPosSpectraUneven[2]->SetMarkerStyle(22);
        hPosSpectraUneven[3]->SetMarkerStyle(23);
        hPosSpectraUneven[4]->SetMarkerStyle(29);
        hPosSpectraUneven[5]->SetMarkerStyle(33);
        hPosSpectraUneven[6]->SetMarkerStyle(34);
        hPosSpectraUneven[7]->SetMarkerStyle(45);
        hPosSpectraUneven[8]->SetMarkerStyle(43);

        hNegSpectraUneven[0]->SetMarkerStyle(20);
        hNegSpectraUneven[1]->SetMarkerStyle(21);
        hNegSpectraUneven[2]->SetMarkerStyle(22);
        hNegSpectraUneven[3]->SetMarkerStyle(23);
        hNegSpectraUneven[4]->SetMarkerStyle(29);
        hNegSpectraUneven[5]->SetMarkerStyle(33);
        hNegSpectraUneven[6]->SetMarkerStyle(34);
        hNegSpectraUneven[7]->SetMarkerStyle(45);
        hNegSpectraUneven[8]->SetMarkerStyle(43);
    for(Int_t i = 0; i < 9; i++)
    {
	hNegSpectraUneven[i]->SetMarkerSize(1.5);
	hNegSpectraUneven[i]->SetMarkerSize(1.5);
    }*/


    c1->BuildLegend();
    c1->SetTicks();
    c1->SetLogy();
    c1->SetGridx();
    c1->SetGridy();
    c1->Update();
    return 0;
}

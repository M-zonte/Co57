//
//  2-LinearitaMCA.cpp
//  Co57
//
//  Created by Matteo Capitani on 20/11/23.
//

#include <iostream>
#include "Co57/Libs/UtilityLib.h"
#include "Co57/Libs/UtilityLib.cc"
#include "Co57/Libs/Co57Lib.h"
#include "Co57/Libs/Co57Lib.cc"
#include "TSpectrum.h"

using namespace std;


int main(int argc, char ** argv) {
    
    clock_t t_begin;
    t_begin = clock();
    
    TString input_filename = "/Users/matteocapitani/Desktop/Università/Magistrale/Laboratorio di Misure Nucleari/Co57/Data/2-LinearitaMCA/MCAlinearityREAL.txt";
    TString output_filename = "/Users/matteocapitani/Desktop/Università/Magistrale/Laboratorio di Misure Nucleari/Co57/Output/2-Linearita.root";
    TString voltage_filename = "/Users/matteocapitani/Desktop/Università/Magistrale/Laboratorio di Misure Nucleari/Co57/Data/2-LinearitaMCA/voltage.txt";
    
    vector<vector<double>> data = readN957<double>(input_filename);
    vector<pair<double, double>> voltage = readPAIR<double>(voltage_filename);
    vector<double> media;
    vector<double> sigma;
    
    TH1D *MCA_Linearity = new TH1D("MCA Linearity", "MCA Linearity", data.at(0).size(), 0, 8192);

    for (int i=0; i< data.at(0).size(); i++) {
        MCA_Linearity->SetBinContent(i, data.at(2).at(i));
    }
    
    int peakFound = MCA_Linearity->ShowPeaks(1, "noMarkov", 0.4);
    TList *functions = MCA_Linearity->GetListOfFunctions();
    TPolyMarker *pm = (TPolyMarker*)functions->FindObject("TPolyMarker");
    
    TFile *f = new TFile(output_filename, "RECREATE");
    f->cd();
    
    for (int i=0; i<peakFound; i++) {
        TF1 *fit = new TF1("Peak", "gaus(0)", pm->GetX()[i]-15, pm->GetX()[i]+15);
        TFitResultPtr *fitPtr = new TFitResultPtr (MCA_Linearity->Fit(fit, "SRLQ+"));
        media.push_back(fit->GetParameter(1));
        sigma.push_back(fit->GetParameter(2));
    }
    
    MCA_Linearity->Write();
    
    sort(media.begin(), media.end());
    sort(sigma.begin(), sigma.end());
    
    TApplication theApp("theApp", &argc, argv);
    TCanvas *c = new TCanvas();
    c->cd();
    TGraphErrors *MCA_analysis = new TGraphErrors();
    MCA_analysis->TNamed::SetNameTitle("MCA Analysis", "MCA Linearity Analysis");
    MCA_analysis->GetXaxis()->SetTitle("Volt (V)");
    MCA_analysis->GetYaxis()->SetTitle("Channel");
    
    for (int i=0; i<voltage.size(); i++) {
        MCA_analysis->SetPoint(i, voltage.at(i).first, media.at(i));
        MCA_analysis->SetPointError(i, voltage.at(i).second, sigma.at(i));
    }
    MCA_analysis->SetMarkerStyle(20);
    MCA_analysis->SetMarkerSize(0.7);
    TF1 *fit = new TF1("Linearity", "pol1(0)", -0.3, 10);
    gStyle->SetOptFit(1111);
    TFitResultPtr *fitPtr = new TFitResultPtr (MCA_analysis->Fit(fit, "SR+"));
    MCA_analysis->Draw("AP");
    MCA_analysis->Write();
    
    f->Close();
    //theApp.Run();
    t_begin = clock() - t_begin;
    cout << "Time Elapsed: " << (1.*t_begin/CLOCKS_PER_SEC) << " s" << endl;
    return 0;
}

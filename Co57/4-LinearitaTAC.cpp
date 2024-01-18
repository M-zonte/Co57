//
//  4-LinearitaTAC.cpp
//  Co57
//
//  Created by Matteo Capitani on 02/01/24.
//

#include <iostream>
#include "Libs/UtilityLib.h"
#include "Libs/UtilityLib.cc"
#include "Libs/Co57Lib.h"
#include "Libs/Co57Lib.cc"
using namespace std;

int main(int argc, char ** argv) {
    
    clock_t t_begin;
    t_begin = clock();
    
    TString input_filename = "/Users/matteocapitani/Desktop/Università/4 Anno/Laboratorio di Misure Nucleari/Co57/Data/4-LinearitàTAC/SpettroTAC.txt";
    TString output_filename = "/Users/matteocapitani/Desktop/Università/4 Anno/Laboratorio di Misure Nucleari/Co57/Output/2-Linearita.root";
    TString time_filename = "/Users/matteocapitani/Desktop/Università/4 Anno/Laboratorio di Misure Nucleari/Co57/Data/4-LinearitàTAC/TempiTAC.txt";
    
    vector<vector<double>> data = readN957<double>(input_filename);
    vector<pair<double, double>> time = readPAIR<double>(time_filename);
    vector<double> mean;
    vector<double> sigma;

    TH1D *TAC_Linearity = new TH1D("TAC Linearity", "TAC Linearity", data.at(2).size(), 0, 8192);

    TFile *f = new TFile(output_filename, "UPDATE");
    f->cd();
    for (int i=0; i<data.at(2).size(); i++) {
        TAC_Linearity->SetBinContent(i, data.at(2).at(i));
    }
    
    int peakFound = TAC_Linearity->ShowPeaks(1, "noMarkov", 0.2);
    TList *functions = TAC_Linearity->GetListOfFunctions();
    TPolyMarker *pm = (TPolyMarker*)functions->FindObject("TPolyMarker");
    
    for (int i=0; i<peakFound; i++) {
        TF1 *fit = new TF1("Peak", "gaus(0)", pm->GetX()[i]-15, pm->GetX()[i]+15);
        TFitResultPtr *fitPtr = new TFitResultPtr (TAC_Linearity->Fit(fit, "SRLQ+"));
        mean.push_back(fit->GetParameter(1));
        sigma.push_back(fit->GetParameter(2));
    }
    TAC_Linearity->Write();
    
    sort(mean.begin(), mean.end());
    sort(sigma.begin(), sigma.end());
    
    TApplication theApp("theApp", &argc, argv);
    TCanvas *c = new TCanvas();
    c->cd();
    TGraphErrors *TAC_Analysis = new TGraphErrors();
    TAC_Analysis->TNamed::SetNameTitle("TAC Analysis", "TAC Linearity Analysis");
    TAC_Analysis->GetXaxis()->SetTitle("Time (ns)");
    TAC_Analysis->GetYaxis()->SetTitle("Channel");
    
    for (int i=0; i<time.size(); i++) {
        TAC_Analysis->SetPoint(i, time.at(i).first, mean.at(i));
        TAC_Analysis->SetPointError(i, time.at(i).second, sigma.at(i));
    }
    TAC_Analysis->SetMarkerStyle(20);
    TAC_Analysis->SetMarkerSize(0.7);
    TF1 *fit = new TF1("Linearity", "pol1(0)", 0, 500);
    gStyle->SetOptFit(1111);
    TFitResultPtr *fitPtr = new TFitResultPtr (TAC_Analysis->Fit(fit, "SR+"));
    TAC_Analysis->Draw("AP");
    TAC_Analysis->Write();
    
    
    
    //Risoluzione Temporale
    double time_per_ch = 1./fit->GetParameter(1);
    double FWHM = 2*sqrt(2*M_LN2) * media(sigma);
    cout << "Time Resolution: " << FWHM * time_per_ch << " ns" << endl;
    
    t_begin = clock() - t_begin;
    cout << "Time Elapsed: " << (1.*t_begin/CLOCKS_PER_SEC) << " s" << endl;
    
    f->Close();
    theApp.Run();
    
    return 0;
}

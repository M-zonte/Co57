//
//  2-LinearitaMCA.cpp
//  Co57
//
//  Created by Matteo Capitani on 20/11/23.
//

#include <iostream>
#include "Libs/UtilityLib.h"
#include "Libs/UtilityLib.cc"
#include "Libs/Co57Lib.h"
#include "Libs/Co57Lib.cc"
#include "TSpectrum.h"

using namespace std;


int main(int argc, char ** argv) {
    
    clock_t t_begin;
    t_begin = clock();
    
    TString input_filename = "/Users/matteocapitani/Desktop/Università/4 Anno/Laboratorio di Misure Nucleari/Co57/Data/2-LinearitaMCA/mca_linearityGOOD.txt";
    TString output_filename = "/Users/matteocapitani/Desktop/Università/4 Anno/Laboratorio di Misure Nucleari/Co57/Output/2-LinearitaMCA.root";
    int maxVoltage = 5;
    
    vector<vector<double>> data = readN957<double>(input_filename);
    vector<double> media;
    vector<double> sigma;
    
    TH1D *MCA_Linearity = new TH1D("MCA Linearity", "MCA Linearity", data.at(0).size(), 0, 8192);
    for (int i=0; i< data.at(0).size(); i++) {
        MCA_Linearity->SetBinContent(i, data.at(2).at(i));
    }
    
    int peakFound = MCA_Linearity->ShowPeaks();
    TList *functions = MCA_Linearity->GetListOfFunctions();
    TPolyMarker *pm = (TPolyMarker*)functions->FindObject("TPolyMarker");
    
    TFile *f = new TFile(output_filename, "RECREATE");
    f->cd();
    
    for (int i=1; i<=maxVoltage; i++) {
        int peakPosition = 760*i;
        TF1 *fit = new TF1("Peak", "gaus(0)", pm->GetX()[i-1]-10, pm->GetX()[i-1]+10);
        TFitResultPtr *fitPtr = new TFitResultPtr (MCA_Linearity->Fit(fit, "SRLQ+"));
        media.push_back(fit->GetParameter(1));
        sigma.push_back(fit->GetParameter(2));
    }
    
    MCA_Linearity->Write();

    sort(media.begin(), media.end());
    sort(sigma.begin(), sigma.end());
    TGraphErrors *MCA_analysis = new TGraphErrors();
    for (int i=0; i<maxVoltage; i++) {
        MCA_analysis->SetPoint(i, i+1, media.at(i));
        MCA_analysis->SetPointError(0.03*i, sigma.at(i));
    }
    MCA_analysis->SetMarkerStyle(20);
    TF1 *fit = new TF1("Linearity", "pol1(0)", 0, 8);
    TFitResultPtr *fitPtr = new TFitResultPtr (MCA_analysis->Fit(fit, "SR+"));
    
    MCA_analysis->Write();
    f->Close();
    
    t_begin = clock() - t_begin;
    cout << "Time Elapsed: " << (1.*t_begin/CLOCKS_PER_SEC) << " s" << endl;
    return 0;
}

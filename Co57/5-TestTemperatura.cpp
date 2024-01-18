//
//  5-TestTemperatura.cpp
//  Co57
//
//  Created by Matteo Capitani on 10/01/24.
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
    
    TString filename = "/Users/matteocapitani/Desktop/Università/4 Anno/Laboratorio di Misure Nucleari/Co57/Data/5-TestTemperatura/Spettri/";
    TString output_filename = "/Users/matteocapitani/Desktop/Università/4 Anno/Laboratorio di Misure Nucleari/Co57/5-TestTemperatura.root";
    
    vector<double> media;
    vector<double> sigma;
    vector<TDatime> date;
    
    
    TFile *f = new TFile(output_filename, "RECREATE");
    f->mkdir("Spettri");
    f->cd("Spettri");
    
    for (int i=1; i<=850; i++) {
        TString name = filename + to_string(i) + ".txt";
        vector<vector<double>> data = readN957<double>(name);
        date.push_back(readDATE(name));
        cout << i << "\t" << date.at(i-1).GetDate() << "\t" << date.at(i-1).GetTime() <<  endl;
        TString histo_name = "spectrum" + to_string(i) + "\t" + date.at(i-1).AsSQLString();
        TH1D *spectrum = new TH1D(histo_name, histo_name, data.at(0).size(), 0, 8192);
        for (int j=0; j<data.at(0).size(); j++) {
            spectrum->SetBinContent(j, data.at(2).at(j));
        }
        //spectrum->SetBins(8192, data.at(1).at(0), data.at(1).at(8191));
        TF1 *peak = new TF1("Peak", "gaus(0)", 400, 1050);
        TFitResultPtr *fitPtr = new TFitResultPtr (spectrum->Fit(peak, "SRLQ+"));
        media.push_back(peak->GetParameter(1));
        sigma.push_back(peak->GetParError(1));
        spectrum->Write();
    }
    
    TGraphErrors *temp_analysis = new TGraphErrors();
    
    for (int i=1; i<=850; i++) {
        UInt_t d_time = date.at(i-1).Convert();
        cout << d_time << endl;
        temp_analysis->SetPoint(i, d_time, media.at(i-1));
        temp_analysis->SetPointError(i, 0, sigma.at(i-1));
    }
    
    temp_analysis->TNamed::SetNameTitle("Temperature Analysis", "Temperature Analysis");
    temp_analysis->GetXaxis()->SetTitle("Date");
    temp_analysis->GetXaxis()->SetTimeDisplay(1);
    gStyle->SetTimeOffset(0);
    
    temp_analysis->GetXaxis()->SetLimits(date.at(0).Convert()-60*60, date.at(849).Convert()+60*60);
    temp_analysis->SetMarkerStyle(3);

    temp_analysis->GetYaxis()->SetTitle("Energy");
    f->cd("/");
    temp_analysis->Write();
    
    f->Close();
    t_begin = clock() - t_begin;
    cout << "Time Elapsed: " << (1.*t_begin/CLOCKS_PER_SEC) << " s" << endl;
    
    return 0;
}

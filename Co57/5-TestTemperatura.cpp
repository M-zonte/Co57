//
//  5-TestTemperatura.cpp
//  Co57
//
//  Created by Matteo Capitani on 10/01/24.
//

#include <iostream>
#include "Co57/Libs/UtilityLib.h"
#include "Co57/Libs/UtilityLib.cc"
#include "Co57/Libs/Co57Lib.h"
#include "Co57/Libs/Co57Lib.cc"


using namespace std;

int main(int argc, char ** argv) {
    
    clock_t t_begin;
    t_begin = clock();
    
    TString filename = "/Users/matteocapitani/Desktop/Università/Magistrale/Laboratorio di Misure Nucleari/Co57/Data/5-TestTemperatura/Spettri/";
    TString output_filename = "/Users/matteocapitani/Desktop/Università/Magistrale/Laboratorio di Misure Nucleari/Co57/5-TestTemperatura.root";
    
    
    vector<double> media;
    vector<double> sigma;
    vector<TDatime> date;
    
    
    TFile *f = new TFile(output_filename, "RECREATE");
    f->mkdir("Spettri");
    f->cd("Spettri");
    
    //ANALISI SPETTRI
    
    for (int i=1; i<=850; i++) {
        TString name = filename + to_string(i) + ".txt";
        vector<vector<double>> data = readN957<double>(name);
        date.push_back(readDATE(name));
        cout << i << "\t" << date.at(i-1).AsString() << endl;
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
    //DRIFT DEL PICCO CON TEMPERATURA
    TGraphErrors *temp_analysis = new TGraphErrors();
    TCanvas *c1 = new TCanvas;
    c1->cd();
    c1->SetGrid();
    for (int i=1; i<=850; i++) {
        UInt_t d_time = date.at(i-1).Convert();
        temp_analysis->SetPoint(i, d_time, media.at(i-1));
        temp_analysis->SetPointError(i, 0, sigma.at(i-1));
    }
    
    temp_analysis->TNamed::SetNameTitle("Temperature Analysis", "Temperature Analysis");
    temp_analysis->GetXaxis()->SetTitle("Date");
    temp_analysis->GetXaxis()->SetTimeDisplay(1);
    gStyle->SetTimeOffset(0);
    
    temp_analysis->GetXaxis()->SetLimits(date.at(0).Convert()-60*60, date.at(849).Convert()+60*60);
    
    //temp_analysis->SetMarkerSize(10);

    temp_analysis->GetYaxis()->SetTitle("Channel");
    f->cd("/");
    temp_analysis->SetLineColor(kViolet);
    temp_analysis->SetLineWidth(2);
    temp_analysis->Draw("ALZ"); //SISTEMARE FA CACARE
    c1->Write();
    
    //VARIAZIONE DELLA TEMPERATURA
    TString temp_filename = "/Users/matteocapitani/Desktop/Università/Magistrale/Laboratorio di Misure Nucleari/Co57/Data/5-TestTemperatura/temperatura23-24.txt";
    vector<pair<TDatime, float>> Temperature = readTemperature(temp_filename);
    
    TGraph *temp_variation = new TGraph();
    for (int i=0; i<Temperature.size(); i++) {
        UInt_t d = Temperature.at(i).first.Convert();
        cout << Temperature.at(i).first.AsSQLString() << endl;
        temp_variation->SetPoint(i+1, d, Temperature.at(i).second);
    }
    
    temp_variation->TNamed::SetNameTitle("Temperature Variation", "Temperature Variation");
    temp_variation->GetXaxis()->SetTitle("Date");
    temp_variation->GetXaxis()->SetTimeDisplay(1);
    gStyle->SetTimeOffset(0);
    
    temp_variation->GetXaxis()->SetLimits(Temperature.at(0).first.Convert()-60*60, Temperature.at(5470).first.Convert()+60*60);

    temp_variation->GetYaxis()->SetTitle("Temperature");
    temp_variation->Write();
    
    //CONFRONTO TRA SPETTRO CALDO E FREDDO
    
    vector<vector<double>> dataCold = readN957<double>(filename + "3.txt");
    vector<vector<double>> dataWarm = readN957<double>(filename + "847.txt");
    
    TH1D *spectrumCold = new TH1D ("Cold", "", dataCold.at(0).size(), 0, 8192);
    TH1D *spectrumWarm = new TH1D ("Warm", "", dataWarm.at(0).size(), 0, 8192);
    
    for (int i=0; i<dataCold.at(0).size(); i++) {
        spectrumCold->SetBinContent(dataCold.at(0).at(i), dataCold.at(2).at(i));
        spectrumWarm->SetBinContent(dataWarm.at(0).at(i), dataWarm.at(2).at(i));
    }
    spectrumCold->SetLineColor(kBlue);
    spectrumCold->SetLineWidth(2.5);
    spectrumWarm->SetLineColor(kRed);
    spectrumWarm->SetLineWidth(2.5);
    
    
    auto *hs = new THStack("hs", "Peak Drifting");

    spectrumCold->SetTitle("Cold: 22/12/23 17:23");
    spectrumWarm->SetTitle("Warm: 09/01/24 07:49");
    hs->Add(spectrumCold);
    hs->Add(spectrumWarm);
    hs->Draw("AFC NOSTACK");
    gPad->BuildLegend();
    hs->Write();
    
    f->Close();
    t_begin = clock() - t_begin;
    cout << "Time Elapsed: " << (1.*t_begin/CLOCKS_PER_SEC) << " s" << endl;
    
    return 0;
}

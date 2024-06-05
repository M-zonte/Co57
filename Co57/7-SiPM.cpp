//
//  7-SiPM.cpp
//  Co57
//
//  Created by Matteo Capitani on 28/05/24.
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
    
    TString output_filename = "/Users/matteocapitani/Desktop/Università/Magistrale/Laboratorio di Misure Nucleari/Co57/Output/7-SiPM.root";
    TString input_14 = "/Users/matteocapitani/Desktop/Università/Magistrale/Laboratorio di Misure Nucleari/Co57/Data/7-SiPM/Spettro energetico sipm 14 kev good.txt";
    TString input_122 = "/Users/matteocapitani/Desktop/Università/Magistrale/Laboratorio di Misure Nucleari/Co57/Data/7-SiPM/Spettro energetico sipm 122 kev good.txt";
    
    
    TFile *f = new TFile(output_filename, "RECREATE");
    f->cd();
    
    //SPETTRO ENERGETICO AD ALTA STATISTICA
    vector<vector<int>> data14 = readN957<int>(input_14);
    vector<vector<int>> data122 = readN957<int>(input_122);

    TString name14 = "SiPM 1";
    TString name122 = "SiPM 2";
    
    gStyle->SetOptFit(1111);
    gStyle->SetOptStat("n"); //only print the histo name
    TH1D *spectrum14 = new TH1D(name14, name14, data14.at(0).size(), 0, 8192);
    TH1D *spectrum122 = new TH1D(name122, name122, data122.at(0).size(), 0, 8192);
    
    spectrum14->GetXaxis()->SetTitle("Channel");
    spectrum14->GetYaxis()->SetTitle("Counts");
    spectrum122->GetXaxis()->SetTitle("Channel");
    spectrum122->GetYaxis()->SetTitle("Counts");
    
    for (int i=0; i<data14.at(0).size(); i++){
        spectrum14->SetBinContent(data14.at(1).at(i), data14.at(2).at(i));
        spectrum14->SetBinError(data14.at(1).at(i), sqrt(data14.at(2).at(i)));
        spectrum122->SetBinContent(data122.at(1).at(i), data122.at(2).at(i));
        spectrum122->SetBinError(data122.at(1).at(i), sqrt(data122.at(2).at(i)));

    }
    TF1 *fit14 = new TF1("14keV", "gaus(0)+[3]", 300, 800);
    TF1 *fit122Escape = new TF1 ("122keV+escape", "gaus(0)+gaus(3)+gaus(6)+[9]", 1800, 4900);
    
    //Fit spettro SiPM 1
    //Parametri picco 14 keV
    double par14[4] = {100E3, 500, 100, 1000};
    fit14->SetParameters(par14);
    fit14->SetParNames("Amp 14 keV", "#mu 14 keV", "#sigma 14 keV", "Background");
    
    //Parametri triplo picco escape+122+136
    fit122Escape->SetParLimits(1, 2300, 2700);
    fit122Escape->SetParLimits(2, 100, 700);
    fit122Escape->SetParLimits(4, 3400, 3700);
    fit122Escape->SetParLimits(5, 100, 700);
    fit122Escape->SetParLimits(6, 0.05*500E3, 0.15*500E3); //forzo il picco del 136 ad essere al massimo il 20% del 122
    fit122Escape->SetParLimits(7, 3900, 4600);
    fit122Escape->SetParLimits(8, 100, 1000);
    fit122Escape->SetParNames("Amp escape", "#mu escape", "#sigma escape", "Amp 122 keV", "#mu 122 keV", "#sigma 122 keV", "Amp 136 keV", "#mu 136 keV", "#sigma 136", "Background");
    
    TCanvas *c1 = new TCanvas();
    c1->cd();
    c1->SetGrid();
    TFitResultPtr *FitPtr14 = new TFitResultPtr(spectrum14->Fit(fit14, "SRB+"));
    TFitResultPtr *FitPtr122 = new TFitResultPtr(spectrum14->Fit(fit122Escape, "SRB+"));
    
    //Disegno spettro 1
    double par122[9];
    fit122Escape->GetParameters(par122);
    TF1 *drawEscape = new TF1("escape", "gaus(0)", 1800, 3700);
    drawEscape->SetLineColor(kBlue);
    TF1 *draw122 = new TF1("122 keV", "gaus(0)", 2500, 5000);
    draw122->SetLineColor(kGreen);
    TF1 *draw136 = new TF1("136 keV", "gaus(0)", 2500, 5500);
    draw136->SetLineColor(kViolet);
    drawEscape->SetParameters(par122);
    draw122->SetParameters(&par122[3]);
    draw136->SetParameters(&par122[6]);
    drawEscape->Draw("Same");
    draw122->Draw("Same");
    draw136->Draw("Same");
    gROOT->GetListOfCanvases()->Draw();
    c1->Write();

    //Parametri triplo picco 2
    fit122Escape->SetParLimits(1, 1000, 1800);
    fit122Escape->SetParLimits(2, 100, 700);
    fit122Escape->SetParLimits(4, 2400, 3300);
    fit122Escape->SetParLimits(5, 100, 700);
    fit122Escape->SetParLimits(6, 0.05*170E3, 0.15*170E3); //forzo il picco del 136 ad essere al massimo il 20% del 122
    fit122Escape->SetParLimits(7, 3800, 4600);
    fit122Escape->SetParLimits(8, 100, 1000);
    
    //Disegno spettro 2
    TCanvas *c2 = new TCanvas();
    c2->cd();
    c2->SetGrid();
    fit122Escape->SetRange(1000, 5000);
    drawEscape->SetRange(500, 2900);
    draw122->SetRange(1000, 4500);
    draw136->SetRange(2000, 5700);
    TFitResultPtr *FitPtr122_136 = new TFitResultPtr(spectrum122->Fit(fit122Escape, "SRB+"));
    fit122Escape->GetParameters(par122);
    drawEscape->SetParameters(par122);
    draw122->SetParameters(&par122[3]);
    draw136->SetParameters(&par122[6]);
    drawEscape->Draw("Same");
    draw122->Draw("Same");
    draw136->Draw("Same");
    gROOT->GetListOfCanvases()->Draw();
    c2->Write();

    
    
    //RISOLUZIONE DEL SIPM AL VARIARE DELL'OVERVOLTAGE
    
    //Definizioni nomi e variabili
    TString input_other = "/Users/matteocapitani/Desktop/Università/Magistrale/Laboratorio di Misure Nucleari/Co57/Data/7-SiPM/Risoluzione/";
    TString V_name[7] = {"54.2", "54.8", "55.1", "55.4", "56.0", "56.6", "57.0"};
    vector<double> Voltage;
    vector<double> resolution14;
    vector<double> resolution122;
    vector<double> errResolution14;
    vector<double> errResolution122;
    vector<vector<int>> data;
    
    for (int i=0; i<7; i++) {
        TString name = input_other + V_name[i] + ".txt";
        data = readN957<int>(name);
        Voltage.push_back(V_name[i].Atof());
        
        name = "Energy Spectrum (OverVoltage = " + V_name[i] + ")";
        TH1D *spectrum = new TH1D(name, name, data.at(0).size(), 0, 8192);
        for (int j=0; j<data.at(0).size(); j++) {
            spectrum->SetBinContent(data.at(0).at(j), data.at(2).at(j));
            spectrum->SetBinError(data.at(0).at(j), sqrt(data.at(2).at(j)));
        }
        fit14->SetRange(125+50*i, 450+150*i);
        fit122Escape->SetRange(800+250*i, 3000+700*i);
        double par[3] = {2500, static_cast<double>(200+100*i), 100};
        fit14->SetParameters(par);
        fit122Escape->SetParLimits(1, 900+300*i, 1300+400*i);
        fit122Escape->SetParLimits(2, 100, 700);
        fit122Escape->SetParLimits(4, 1600+500*i, 2300+500*i);
        fit122Escape->SetParLimits(5, 100, 700);
        fit122Escape->SetParLimits(6, 100, 400E3);
        fit122Escape->SetParLimits(7, 1800+500*i, 2500+600*i);
        fit122Escape->SetParLimits(8, 100, 1000);
        fit122Escape->SetParNames("Amp escape", "#mu escape", "#sigma escape", "Amp 122 keV", "#mu 122 keV", "#sigma 122 keV", "Amp 136 keV", "#mu 136 keV", "#sigma 136", "Background");
        
        
        TFitResultPtr *fitPtr14 = new TFitResultPtr(spectrum->Fit(fit14, "SRBQ+"));
        TFitResultPtr *fitPtr122 = new TFitResultPtr(spectrum->Fit(fit122Escape, "SRBQ+"));
        
        spectrum->Write();
        resolution14.push_back(2.35*fit14->GetParameter(2)/fit14->GetParameter(1));
        resolution122.push_back(2.35*fit122Escape->GetParameter(5)/fit122Escape->GetParameter(4));
        errResolution14.push_back(sumQuad(fit14->GetParError(2)/fit14->GetParameter(2), fit14->GetParError(1)/fit14->GetParameter(1)) * resolution14.at(i));
        errResolution122.push_back(sumQuad(fit122Escape->GetParError(5)/fit122Escape->GetParameter(5), fit122Escape->GetParError(4)/fit122Escape->GetParameter(4)) * resolution122.at(i));
    }
    
    for (int i=0; i<7; i++) {
        cout << V_name[i] << " V" << endl;
        cout << "14keV:" << "\t" << resolution14.at(i) << " ± " << errResolution14.at(i) << endl;
        cout << "122keV:" << "\t" << resolution122.at(i) << " ± " << errResolution122.at(i) << endl;
    }
    
    TGraphErrors *res14 = new TGraphErrors();
    TGraphErrors *res122 = new TGraphErrors();
    
    TApplication theApp("theApp", &argc, argv);

    // il primo punto del 14 non sta fittando giusto e il 56.0 non è veramente il 56.0 ma 56.6 duplicato (da riprendere)
    for (int i=0; i<7; i++) {
        res14->SetPoint(i, Voltage.at(i), fabs(resolution14.at(i)));
        res14->SetPointError(i, 0.1, errResolution14.at(i));
        res122->SetPoint(i, Voltage.at(i), fabs(resolution122.at(i)));
        res122->SetPointError(i, 0.1, errResolution122.at(i));
    }
    res14->SetTitle("Energy Resolution @ 14 keV");
    res14->GetXaxis()->SetTitle("Voltage (V)");
    res14->GetYaxis()->SetTitle("Resolution");
    res122->SetTitle("Energy Resolution @ 122 keV");
    res122->GetXaxis()->SetTitle("Voltage (V)");
    res122->GetYaxis()->SetTitle("Resolution");
    
    res14->Write();
    res122->Write();
    //theApp.Run();
    
    f->Close();
    
    t_begin = clock() - t_begin;
    cout << "Time Elapsed: " << (1.*t_begin/CLOCKS_PER_SEC) << " s" << endl;
    return 0;
}

//  Sorgente Co57 #79
//  TensioneLavoroNaI.cpp
//  Co57
//
//  Created by Matteo Capitani on 10/11/23.
//

#include <iostream>
#include "Libs/myRootLib.h"
#include "Libs/UtilityLib.h"
#include "Libs/Co57Lib.h"
using namespace std;


int main(int argc, char ** argv) {
    
    clock_t t_begin;
    t_begin = clock();
    
    TString folder_filepath = "/Users/matteocapitani/Desktop/Università/4 Anno/Laboratorio di Misure Nucleari/Co57/Data/1-TensioneLavoroNaI/";
    TString outputSpectrum = "/Users/matteocapitani/Desktop/Università/4 Anno/Laboratorio di Misure Nucleari/Co57/Output/1-TensioneLavoroNaI.root";
    vector<vector<vector<int>>> data;
    vector<int> times;
    vector<double> voltage;
    
    
    //lettura dei dati
    
    for (int StartVoltage = 700; StartVoltage <= 1250; StartVoltage += 50) {
        TString data_filepath = folder_filepath + to_string(StartVoltage) + ".txt";
        vector<vector<int>> voltage_dataset = readN957<int>(data_filepath);
        
        times.push_back(readTIME(data_filepath));
        data.push_back(voltage_dataset);
        voltage.push_back(StartVoltage);
    }

    TFile *f = new TFile(outputSpectrum, "RECREATE");
    f->cd();
    
    
    // media, sigma
    vector<pair<double, double>> par_14keV;
    vector<pair<double, double>> par_doublepeak1;
    vector<pair<double, double>> par_doublepeak2;
    vector<pair<double, double>> err_14KeV;
    vector<pair<double, double>> err_doublepeak1;
    vector<pair<double, double>> err_doublepeak2;

    
    //creazione degli spettri e fit
    //ATTENZIONE! Tutti i parametri numerici sono stati attentamente calibrati per far convergere bene il fit quindi NON TOCCARE
    
    for (int i=0; i<data.size(); i++) {
        TString name = "Bias Voltage: " + to_string(700 + i*50) + " Volt";
        TH1D *spectrum = new TH1D(name, name, data.at(i).at(0).size(), 0, 8192);
        //elimino tutti i punti minori di una certa soglia
        for (int binCount = 0; binCount < spectrum->GetNbinsX(); binCount++) {
            if (data.at(i).at(0).at(binCount) < 200) {
                spectrum->SetBinContent(binCount, 0);
            } else {
                spectrum->SetBinContent(binCount, data.at(i).at(2).at(binCount));
            }
        }
        //noFondo(spectrum, times.at(i));
        int maxPeak = spectrum->GetMaximumBin();
        TF1 *peak_14keV = new TF1("14 keV", "gaus(0)", maxPeak-300, maxPeak+300);
        TF1 *double_peak = new TF1("double peak", "gaus(0) + gaus(3)", 2900, 7000);
        
        peak_14keV->SetLineColor(kRed);
        double_peak->SetLineColor(kOrange);
        
        peak_14keV->SetParameter(0, VAmpG(spectrum, peak_14keV));
        peak_14keV->SetParameter(1, VMG(peak_14keV));
        peak_14keV->SetParameter(2, VSG(peak_14keV));
        
        double_peak->SetParameter(0, VAmpG(spectrum, double_peak));
        double_peak->SetParameter(1, VMG(double_peak)-1000);
        double_peak->SetParameter(2, VSG(double_peak));
        double_peak->SetParameter(3, VAmpG(spectrum, double_peak));
        double_peak->SetParameter(4, VMG(double_peak)+1000);
        double_peak->SetParameter(5, VSG(double_peak));
        

        
        TFitResultPtr *fitPtr_14 = new TFitResultPtr (spectrum->Fit(peak_14keV, "SRLQN"));
        TFitResultPtr *fitPtr_double = new TFitResultPtr (spectrum->Fit(double_peak, "SRLQN"));
        //con il primo fit trovo una prima stima dei parametri con il quale ridefinisco il range del TF1 con il quale fitto una seconda volta
        peak_14keV->SetRange(peak_14keV->GetParameter(1)-peak_14keV->GetParameter(2)*1.5, peak_14keV->GetParameter(1)+peak_14keV->GetParameter(2)*2.2);
        double_peak->SetRange(double_peak->GetParameter(1)-double_peak->GetParameter(2)*1.5, double_peak->GetParameter(4)+double_peak->GetParameter(5)*2.2);
        
        
        *fitPtr_14 = TFitResultPtr(spectrum->Fit(peak_14keV, "SERLQ+"));
        *fitPtr_double = TFitResultPtr(spectrum->Fit(double_peak, "SERLQ+"));
        
        par_14keV.push_back(make_pair(peak_14keV->GetParameter(1), peak_14keV->GetParameter(2)));
        par_doublepeak1.push_back(make_pair(double_peak->GetParameter(1), double_peak->GetParameter(2)));
        par_doublepeak2.push_back(make_pair(double_peak->GetParameter(4), double_peak->GetParameter(5)));
        
        err_14KeV.push_back(make_pair(peak_14keV->GetParError(1), peak_14keV->GetParError(2)));
        err_doublepeak1.push_back(make_pair(double_peak->GetParError(1), double_peak->GetParError(2)));
        err_doublepeak2.push_back(make_pair(double_peak->GetParError(4), double_peak->GetParError(5)));
        
        spectrum->Write();
    }
    
    //Posizione di ogni picco con relativa sigma
    
    cout << "14 keV peak" << endl;
    for (int i=0; i<data.size(); i++) {
        cout << voltage.at(i) << ":\t" << par_14keV.at(i).first << "\t" << par_14keV.at(i).second << endl;
    }
    cout << endl;
    cout << "iodine peak" << endl;
    for (int i=0; i<data.size(); i++) {
        cout << voltage.at(i) << ":\t" << par_doublepeak1.at(i).first << "\t" << par_doublepeak1.at(i).second << endl;
    }
    cout << endl;
    cout << "122-136 keV peak" << endl;
    for (int i=0; i<data.size(); i++) {
        cout << voltage.at(i) << ":\t" << par_doublepeak2.at(i).first << "\t" << par_doublepeak2.at(i).second << endl;
    }
    cout << endl;
    
    
    //risoluzione energetica: FWHM/media
    
    vector<vector<double>> resolution(3);
    vector<vector<double>> err_resolution(3);
    for (int i=0; i<data.size(); i++) {
        resolution.at(0).push_back(par_14keV.at(i).second * 2*sqrt(2*M_LN2) / par_14keV.at(i).first);
        resolution.at(1).push_back(par_doublepeak1.at(i).second * 2*sqrt(2*M_LN2) / par_doublepeak1.at(i).first);
        resolution.at(2).push_back(par_doublepeak2.at(i).second * 2*sqrt(2*M_LN2) / par_doublepeak2.at(i).first);
        //non sono sicuro che la stima degli errori si faccia così, per ora la lascio
        err_resolution.at(0).push_back(sumQuad(err_14KeV.at(i).first/par_14keV.at(i).first, err_14KeV.at(i).second)/par_14keV.at(i).second);
        err_resolution.at(1).push_back(sumQuad(err_doublepeak1.at(i).first/par_doublepeak1.at(i).first, err_doublepeak1.at(i).second/par_doublepeak1.at(i).second));
        err_resolution.at(2).push_back(sumQuad(err_doublepeak2.at(i).first/par_doublepeak2.at(i).first, err_doublepeak2.at(i).second/par_doublepeak2.at(i).second));

    }
    
    TGraphErrors *analysis_14keV = new TGraphErrors(data.size());
    TGraphErrors *analysis_1peak = new TGraphErrors(data.size());
    TGraphErrors *analysis_2peak = new TGraphErrors(data.size());
    
    analysis_14keV->SetTitle("Optimization bias voltage: 14 keV peak");
    analysis_1peak->SetTitle("Optimization bias voltage: 1st peak");
    analysis_2peak->SetTitle("Optimization bias voltage: 2nd peak");
    
    for (int i=0; i<data.size(); i++) {
        analysis_14keV->SetPoint(i, voltage.at(i), resolution.at(0).at(i));
        analysis_14keV->SetPointError(i, err_HVPowerSupply, 2*sqrt(2*M_LN2) * err_resolution.at(0).at(i) * resolution.at(0).at(i));

        analysis_1peak->SetPoint(i, voltage.at(i), resolution.at(1).at(i));
        analysis_1peak->SetPointError(i, err_HVPowerSupply, 2*sqrt(2*M_LN2) * err_resolution.at(1).at(i) * resolution.at(1).at(i));

        analysis_2peak->SetPoint(i, voltage.at(i), resolution.at(2).at(i));
        analysis_2peak->SetPointError(i, err_HVPowerSupply, 2*sqrt(2*M_LN2) * err_resolution.at(2).at(i) * resolution.at(2).at(i));
    }
    
    analysis_14keV->Write();
    analysis_1peak->Write();
    analysis_2peak->Write();
    
    
    
    f->Close();
    
    t_begin = clock() - t_begin;
    cout << "Time Elapsed: " << (1.*t_begin/CLOCKS_PER_SEC) << " s" << endl;
    
    return 0;
}

//  Sorgente Co57 #79
//  TensioneLavoroNaI.cpp
//  Co57
//
//  Created by Matteo Capitani on 10/11/23.
//

#include <iostream>
#include "Co57/Libs/UtilityLib.h"
#include "Co57/Libs/UtilityLib.cc"
#include "Co57/Libs/Co57Lib.h"
#include "Co57/Libs/Co57Lib.cc"
using namespace std;

const int Detector_switch = 0;

int main(int argc, char ** argv) {
    
    clock_t t_begin;
    t_begin = clock();
    
    TString folder_filepath = "/Users/matteocapitani/Desktop/Università/Magistrale/Laboratorio di Misure Nucleari/Co57/Data/1-TensioneLavoroNaI/detector";
    TString outputSpectrum = "/Users/matteocapitani/Desktop/Università/Magistrale/Laboratorio di Misure Nucleari/Co57/Output/1-TensioneLavoroNaI.root";
    vector<vector<vector<int>>> data;
    vector<int> times;
    vector<double> voltage;
    
    TFile *f = new TFile(outputSpectrum, "RECREATE");
    f->cd();
    
    if (Detector_switch != 2) {
        //DETECTOR 1
        //lettura dei dati
        cout << "DETECTOR 1" << endl;
        
        for (int StartVoltage = 700; StartVoltage <= 1250; StartVoltage += 50) {
            TString data_filepath = folder_filepath + "1/" + to_string(StartVoltage) + ".txt";
            vector<vector<int>> voltage_dataset = readN957<int>(data_filepath);
            
            times.push_back(readTIME(data_filepath));
            data.push_back(voltage_dataset);
            voltage.push_back(StartVoltage);
        }
        
        
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
            TString name = "Bias Voltage detector 1: " + to_string(700 + i*50) + " Volt";
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
        
        analysis_14keV->TNamed::SetNameTitle("Optimization bias voltage detector 1: 14 keV peak", "Optimization bias voltage detector 1: 14 keV peak");
        analysis_1peak->TNamed::SetNameTitle("Optimization bias voltage detector 1: 1st peak", "Optimization bias voltage detector 1: 1st peak");
        analysis_2peak->TNamed::SetNameTitle("Optimization bias voltage detector 1: 2nd peak", "Optimization bias voltage detector 1: 2nd peak");
        
        
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
    }
    
    
    if (Detector_switch != 1) {
        //DETECTOR 2
        cout << "DETECTOR 2" << endl;
        
        //Pulisco i vector per riutilizzarli con i nuovi dati
        times.clear();
        data.clear();
        voltage.clear();
        
        //Lettura dei dati
        
        for (int StartVoltage = 700; StartVoltage <= 1100; StartVoltage += 50) {
            TString data_filepath = folder_filepath + "2/" + to_string(StartVoltage) + ".txt";
            vector<vector<int>> voltage_dataset = readN957<int>(data_filepath);
            
            //times.push_back(readTIME(data_filepath));
            data.push_back(voltage_dataset);
            voltage.push_back(StartVoltage);
        }
        
        vector<pair<double, double>> par_122keV;
        vector<pair<double, double>> err_122keV;
        
        //creazione degli spettri e fit
        
        for (int i=0; i<data.size(); i++) {
            TString name = "Bias Voltage detector 2: " + to_string(700 + i*50) + " Volt";
            TH1D *spectrum = new TH1D(name, name, data.at(i).at(0).size(), 0, 8192);
            //elimino tutti i punti minori di una certa soglia
            for (int binCount = 0; binCount < spectrum->GetNbinsX(); binCount++) {
                if (data.at(i).at(0).at(binCount) < 200 || data.at(i).at(0).at(binCount) > 8060) {
                    spectrum->SetBinContent(binCount, 0);
                } else {
                    spectrum->SetBinContent(binCount, data.at(i).at(2).at(binCount));
                }
            }
            //noFondo(spectrum, times.at(i));
            int maxPeak = spectrum->GetMaximumBin();
            
            TF1 *double_peak = new TF1("122+136", "gaus(0) + pol2(3)", maxPeak-maxPeak/3, maxPeak+maxPeak/2);
            
            double_peak->SetParameter(0, VAmpG(spectrum, double_peak));
            double_peak->SetParameter(1, VMG(double_peak));
            double_peak->SetParameter(2, VSG(double_peak));
            
            TFitResultPtr *fitPtr_double = new TFitResultPtr (spectrum->Fit(double_peak, "SRLQ+"));
            gStyle->SetOptFit(1111);
            
            
            par_122keV.push_back(make_pair(double_peak->GetParameter(1), double_peak->GetParameter(2)));
            err_122keV.push_back(make_pair(double_peak->GetParError(1), double_peak->GetParError(2)));
            
            spectrum->Write();
        }
        
        vector<double> resolution;
        vector<double> err_resolution;
        for (int i=0; i<data.size(); i++) {
            resolution.push_back(par_122keV.at(i).second * 2*sqrt(2*M_LN2) / par_122keV.at(i).first);
            //non sono sicuro che la stima degli errori si faccia così, per ora la lascio
            err_resolution.push_back(sumQuad(err_122keV.at(i).first/par_122keV.at(i).first, err_122keV.at(i).second)/par_122keV.at(i).second);
        }
        
        TGraphErrors *analysis = new TGraphErrors(data.size());
        analysis->TNamed::SetNameTitle("Optimization bias voltage detector 2: 122-136 keV peak", "Optimization bias voltage detector 2: 122-136 keV peak");
        
        for (int i=0; i<resolution.size(); i++) {
            analysis->SetPoint(i, voltage.at(i), resolution.at(i));
            analysis->SetPointError(i, err_HVPowerSupply, err_resolution.at(i)); //l'errore sul voltaggio non è quello del power supply ad alta tensione ma quello sulla vitina del rivelatore che però è nell'ordine del paio di Volt
        }
        
        analysis->Write();
    }
    
    
    
    
    
    
    
    f->Close();
    
    t_begin = clock() - t_begin;
    cout << "Time Elapsed: " << (1.*t_begin/CLOCKS_PER_SEC) << " s" << endl;
    
    return 0;
}

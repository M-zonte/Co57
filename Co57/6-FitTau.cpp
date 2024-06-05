//
//  6-FitTau.cpp
//  Co57
//
//  Created by Matteo Capitani on 28/02/24.
//

#include <stdio.h>
#include <iostream>
#include "Co57/Libs/UtilityLib.h"
#include "Co57/Libs/UtilityLib.cc"
#include "Co57/Libs/Co57Lib.h"
#include "Co57/Libs/Co57Lib.cc"


using namespace std;

int main(int argc, char ** argv) {
    
    clock_t t_begin;
    t_begin = clock();
    
    TString filename = "/Users/matteocapitani/Downloads/pocaStat.txt";
    TString output_filename = "/Users/matteocapitani/Desktop/Università/Magistrale/Laboratorio di Misure Nucleari/Co57/Output/6-FitTau.root";
    
    TFile *f = new TFile(output_filename, "RECREATE");
    f->cd();
    vector<vector<double>> data = readN957<double>(filename);
    
    TH1D *FitTau = new TH1D("FitTau", "FitTau", data.at(2).size(), 0, 8192);

    for (int i=0; i<data.at(2).size(); i++) {
        FitTau->SetBinContent(i, data.at(2).at(i));
    }
    
    TApplication theApp("theApp", &argc, argv);
    TCanvas *c = new TCanvas();
    c->cd();
    

    TF1 *fit = new TF1("FitTau", convolution, 300, 1500, 2);
    gStyle->SetOptFit(1111);
    /*
    fit->SetParName(0,"k");
    fit->SetParName(1,"#tau");
    fit->SetParName(2,"A");
    fit->SetParName(3,"#mu");
    fit->SetParName(4,"#sigma");
    fit->SetParameter(0, 300);
    //fit->SetParameter(1, 200);
    fit->SetParameter(2, 10);
    fit->SetParameter(3, 2000);
    fit->SetParameter(4, 400);
    fit->SetParameter(5, 15);
    */
    TFitResultPtr *fitPtr = new TFitResultPtr (FitTau->Fit(fit, "SRLQ+"));
   
    cout << "tau: " << -1./(4*fit->GetParameter(1)) << " ns" << endl;

    
    FitTau->Draw();
    
    theApp.Run();
    
    t_begin = clock() - t_begin;
    cout << "Time Elapsed: " << (1.*t_begin/CLOCKS_PER_SEC) << " s" << endl;
    
    return 0;
}

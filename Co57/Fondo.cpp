//
//  Fondo.cpp
//  Co57
//
//  Created by Matteo Capitani on 20/11/23.
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
    
    TString output_filename = "/Users/matteocapitani/Desktop/Università/Magistrale/Laboratorio di Misure Nucleari/Co57/Output/fondo_norm.txt";
    TString Root_filename = "/Users/matteocapitani/Desktop/Università/Magistrale/Laboratorio di Misure Nucleari/Co57/Output/fondo.root";
    
    vector<vector<double>> data = readN957<double>("/Users/matteocapitani/Desktop/Università/Magistrale/Laboratorio di Misure Nucleari/Co57/Data/fondo2gg.txt");
    int time_fondo = readTIME("/Users/matteocapitani/Desktop/Università/Magistrale/Laboratorio di Misure Nucleari/Co57/Data/fondo2gg.txt");
    
    ofstream output(output_filename);
    for (int i=0; i<data.at(0).size(); i++) {
        output << data.at(0).at(i) << "\t" << data.at(1).at(i) << "\t" << static_cast<double>(data.at(2).at(i)/time_fondo) << endl;
    }
    TFile *f = new TFile(Root_filename, "UPDATE");
    f->cd();
    TH1D *Fondo = new TH1D("Fondo", "Fondo", data.at(0).size(), 0, 8192);
    for (int i=0; i< data.at(0).size(); i++) {
        Fondo->SetBinContent(i, data.at(2).at(i));
    }
    
    Fondo->Write();
    f->Close();
    t_begin = clock() - t_begin;
    cout << "Time Elapsed: " << (1.*t_begin/CLOCKS_PER_SEC) << " s" << endl;
    return 0;
}

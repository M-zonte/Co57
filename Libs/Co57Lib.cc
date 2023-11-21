//
//  Co57Lib.cpp
//  Co57
//
//  Created by Matteo Capitani on 13/11/23.
//

#include "Libs/Co57Lib.h"

const double err_HVPowerSupply = 2.; //errore sul setting del voltaggio del power supply ad alta tensione

// funzione per estrarre i dati dai file di testo del programma di lettura N957 di CAEN
template <typename myType>
vector<vector<myType>> readN957 (TString filepath)
{
    //check di esistenza del file
    fstream file;
    file.open (filepath, ios::in);
    if (file.good() != true)
    { cout << "READING FAILED" << endl; exit(EXIT_FAILURE); }
    
    //eliminazione del testo all'inizio del file
    while (file.eof() != true) {
        if (file.peek() != '0') {
            file.ignore(256, '\n');
        } else {
            break;
        }
    }
    vector<vector<myType>> data(3);
    
    //lettura delle tre colonne del file (canale - energia - conteggi)
    while (file.eof() != true) {
        myType n = 0.;
        for(int i=0; i<3; i++) {
            file >> n;
            data.at(i).push_back(n);
        }
    }
    
    data.at(0).pop_back(); //per rimuovere l'ultima riga che legge non so perchè
    data.at(1).pop_back();
    data.at(2).pop_back();
    
    file.close();
    return data;
    
}

int readTIME (TString filepath)
{
    //check di esistenza del file
    fstream file;
    file.open (filepath, ios::in);
    if (file.good() != true)
    { cout << "READING FAILED" << endl; exit(EXIT_FAILURE); }
    
    //estrazione della riga che ci interessa
    while (file.eof() != true) {
        if (file.peek() != 'L') {
            file.ignore(256, '\n');
        } else {
            break;
        }
    }
    
    string s;
    getline(file, s);
    file.close();
    s.erase(s.begin(), s.begin()+10); //Il valore del tempo inizia 10 caratteri dopo l'inizio della riga
    
    return atoi(s.c_str());
}

int readEVENTS (TString filepath)
{
    //check di esistenza del file
    fstream file;
    file.open (filepath, ios::in);
    if (file.good() != true)
    { cout << "READING FAILED" << endl; exit(EXIT_FAILURE); }
    
    //estrazione della riga che ci interessa
    while (file.eof() != true) {
        if (file.peek() != 'N') {
            file.ignore(256, '\n');
        } else {
            break;
        }
    }
    
    string s;
    getline(file, s);
    file.close();
    s.erase(s.begin(), s.begin()+10); //Il valore del tempo inizia 10 caratteri dopo l'inizio della riga
    
    return atoi(s.c_str());
}

TH1* noFondo(TH1 *h, int time)
{
    vector<vector<double>> fondo = readN957<double>("/Users/matteocapitani/Desktop/Università/4 Anno/Laboratorio di Misure Nucleari/Co57/Output/fondo_norm.txt");
    for (int i=0; i<h->GetNbinsX(); i++) {
        double bin_content = h->GetBinContent(i);
        h->SetBinContent(i, bin_content - fondo.at(2).at(i)*time);
    }
    return h;
}

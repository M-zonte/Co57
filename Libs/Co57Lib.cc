//
//  Co57Lib.cpp
//  Co57
//
//  Created by Matteo Capitani on 13/11/23.
//

#include "/Users/matteocapitani/Desktop/Università/4 Anno/Laboratorio di Misure Nucleari/Co57/Libs/Co57Lib.h"

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
    char isZero = NULL;
    do {
        isZero = file.peek();
        file.ignore(256, '\n');
        isZero = file.peek();
    } while (isZero != '0' || file.eof() == true);
    
    vector<vector<myType>> data(3);
    
    //lettura delle tre colonne del file (canale - energia - conteggi)
    while (file.eof() != true)
    {
        myType n = 0.;
        if (file.eof() != true)
        {
            for(int i=0; i<3; i++){
                file >> n;
                data.at(i).push_back(n);
            }
        }
    }
    data.at(0).pop_back(); //per rimuovere l'ultima riga che legge non so perchè
    data.at(1).pop_back();
    data.at(2).pop_back();

    return data;
    
}

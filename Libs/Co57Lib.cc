//
//  Co57Lib.cpp
//  Co57
//
//  Created by Matteo Capitani on 13/11/23.
//

#include "Co57/Libs/Co57Lib.h"

const double err_HVPowerSupply = 2.; //errore sul setting del voltaggio del power supply ad alta tensione

// funzione per estrarre i dati dai file di testo del programma di lettura N957 di CAEN
template <typename myType>
vector<vector<myType>> readN957 (TString filepath)
{
    //check di esistenza del file
    fstream file;
    file.open (filepath, ios::in);
    if (file.good() != true)
    { cout << "READING FAILED" << endl; exit(EXIT_FAILURE);}
    
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

vector<pair<TDatime, float>> readTemperature (TString filepath)
{
    //check di esistenza del file
    fstream file;
    file.open (filepath, ios::in);
    if (file.good() != true)
    { cout << "READING FAILED" << endl; exit(EXIT_FAILURE);}
    
    vector<pair<TDatime, float>> data;
    
    //salta la prima riga
    string first_line;
    getline(file, first_line);
    
    string line;
    //estrazione dei dati
    while (getline(file, line)) {
        vector<string> row;
        stringstream lineStream(line);
        string cell;

        while (getline(lineStream, cell, ',')) {
            row.push_back(cell);
        }
        //replace_all(row.at(1), "/", "-"); non serve perchè lo faccio già dentro convertDateFormat
        string date = convertDateFormat(row.at(1));
        TDatime d(date.c_str());
        float Temperature = atof(row.at(2).c_str());
        pair<TDatime, float> p(d, Temperature);
        data.push_back(p);
    }
    
    return data;
}

string convertDateFormat(const string& date) {
    // Assicurati che la data abbia il formato corretto "dd/mm/yyyy hh:mm:ss"
    if (date.size() != 19 || date[2] != '/' || date[5] != '/' || date[10] != ' ' || date[13] != ':' || date[16] != ':') {
        cerr << "Formato data non valido" << std::endl;
        return "";
    }

    // Estrai il giorno, il mese, l'anno, l'ora, il minuto e i secondi dalla stringa
    string day = date.substr(0, 2);
    string month = date.substr(3, 2);
    string year = date.substr(6, 4);
    string hour = date.substr(11, 2);
    string minute = date.substr(14, 2);
    string second = date.substr(17, 2);

    // Combina anno, mese e giorno nel formato "yyyy-mm-dd"
    string formattedDate = year + "-" + month + "-" + day + " " + hour + ":" + minute + ":" + second;

    return formattedDate;
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

TDatime readDATE (TString filepath)
{
    //check di esistenza del file
    fstream file;
    file.open (filepath, ios::in);
    if (file.good() != true)
    { cout << "READING FAILED" << endl; exit(EXIT_FAILURE); }
    
    //estrazione della riga che ci interessa
    while (file.eof() != true) {
        if (file.peek() != 'I') {
            file.ignore(256, '\n');
        } else {
            break;
        }
    }
    
    string s;
    getline(file, s);
    file.close();
    s.erase(s.begin(), s.begin()+15); //Il valore del tempo inizia 10 caratteri dopo l'inizio della riga
    vector<int> date; // day-month-year-hour-minute-second
    for (int i=0; i<6; i++) {
        string subs =  s.substr(i*3, 2);
        //cout << subs << endl;
        date.push_back(atoi(subs.c_str()));
    }
    
    TDatime d(date.at(2)+2000, date.at(1), date.at(0), date.at(3), date.at(4), date.at(5));
    return d;
}

TH1* noFondo(TH1 *h, int time)
{
    vector<vector<double>> fondo = readN957<double>("/Users/matteocapitani/Desktop/Università/Magistrale/Laboratorio di Misure Nucleari/Co57/Output/fondo_norm.txt");
    for (int i=0; i<h->GetNbinsX(); i++) {
        double bin_content = h->GetBinContent(i);
        h->SetBinContent(i, bin_content - fondo.at(2).at(i)*time);
    }
    return h;
}

double convolution(double *x, double *par)
{
    /*
    double k = par[0];
    double N = par[1];
    double mu = par[2];
    double sigma = par[3];
    double tau = par[4];

    double A = (mu - x[0])/tau;
    double B = sigma*sigma/(2*tau*tau);

    double Num = (x[0] - mu - sigma*sigma/tau);
    double Den = (sqrt(2)*sigma);

    double esp = exp(A + B);
    double quadra = (1 + erf(Num / Den));
    return k + N*esp*quadra;
     */
    double esp = exp(par[0]+par[1]*x[0]);
    //double gaus = par[2]*exp(-0.5* ((x[0]-par[3])/par[4]) * (x[0]-par[3])/par[4]);
    return esp;
}

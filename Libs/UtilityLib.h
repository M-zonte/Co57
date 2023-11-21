//
//  UtilityLib.h
//
//
//  Created by Matteo Capitani on 03/05/22.
//

#ifndef UtilityLib_hpp
#define UtilityLib_hpp

#include <stdio.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>
#include <map>

#include "TCanvas.h"
#include "TH1F.h"
#include "TH1I.h"
#include "TApplication.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TF1.h"
#include "TFitResult.h"
#include "TFitResultPtr.h"
#include "TStyle.h"
#include "TAttText.h"
#include "TLine.h"          // Allows to draw lines
#include "TAttLine.h"       // Allows to change graphic attributes of lines
#include "TAttFill.h"       // Allows to color areas in a canvas
#include "TMultiGraph.h"
#include "TFormula.h"
#include "TDatime.h"
#include "v5/TFormula.h"
#include "TMath.h"
//#include "TGeoSystemOfUnits.h"
#include "TGeoPhysicalConstants.h"
#include "TTree.h"
#include "TFile.h"
#include "TH2F.h"
#include "TMarker.h"
#include "TLatex.h"
#include "TPolyMarker.h"



using namespace std;
using namespace TGeoUnit;

double RAD (double);
double gaussiana (double, double, double);
long long int fattoriale (int);

double Max(double, double);
double Min(double, double);

template <typename myType>
double media (vector<myType>);
template <typename myType>
double sigma (vector<myType>);

double rand_range(double, double);

template <typename myType>
vector<vector<myType>> invMAT (vector<vector<myType>>);

double sec (double);
double csc (double);
double cot (double);

double sumQuad (double, double);
double compTest (double, double, double);

double ERRprod (double, double, double, double);
double ERRdiv (double, double, double, double);

Double_t retta(Double_t *, Double_t *);
Double_t rettaAB(Double_t *, Double_t *);
Double_t sinusoide(Double_t *, Double_t*);
Double_t gaussiana_root(Double_t *, Double_t *);
Double_t ENC_V(Double_t *, Double_t *);
TString FindRemove(TString, char);

double VAmpG(TH1 *, TF1 *);
double VMG(TF1 *);
double VSG(TF1 *);

template <typename myType>
vector<myType> readNUM (TString); //per la lettura da file di numeri
vector<TString> readCHAR (TString); //per la lettura da file di stringhe di testo
template <typename myType>
vector<vector<myType>> readRANGE (TString); //per la lettura dei range da file (probabilmente posso modificare leggiNUM per fare sta cosa ma non ho tutta questa voglia)
template <typename myType>
vector<pair<myType, myType>> readPAIR (TString);

template <typename myType>
vector<vector<myType>> readMultiData(TString, char); //per la lettura di N istogrammi in un unico file, delimitati da un carattere
TH1I quickDraw (TString, TString); //per plottare rapidamente un istogramma (non funziona ancora)

//OPERAZIONI TRA VECTOR
template <typename myType>
vector<myType> operator+ (vector<myType>, vector<myType>);//somma di un vettore entrata per entrata
template <typename myType>
vector<myType> operator- (vector<myType>, vector<myType>);//sottrazione di un vettore entrata per entrata
template <typename myType>
vector<int> operator- (vector<myType>, vector<int>);
template <typename myType>
vector<int> operator- (vector<int>, vector<myType>);
template <typename myType>
vector<myType> operator* (vector<myType>, vector<myType>);//moltiplicazione di un vettore entrata per entrata
template <typename myType>
vector<myType> operator* (myType, vector<myType>); //moltiplicazione di un vettore per uno scalare
template <typename myType>
vector<myType> operator/ (vector<myType>, myType);//divisione di un vettore per uno scalare diverso da 0
template <typename myType>
void printvec(vector<myType>, char opt = 'C');
template <typename myType>
void printvec(vector<pair<myType, myType>>, TString = "");
template <typename myType>
void printMultiVec(vector<vector<myType>> vec);

template <typename myType>
vector<myType> constructARRAY(myType *);

template <typename myType>
vector<int> findvec(vector<myType>, TString);

vector<TDatime> fillDate(const int, TDatime, const int); //per riempire un vettore di n posti con delle date a partire da una data passata come argomento incrementate tutte le volte di un numero di secondi dato da interval

// Libera la memoria allocata da oggetti contenuti in vector
template <typename myType>
void Libera (vector<myType>);

// Setta la griglia nei TCanvas
void SetMyGrid ();


#endif /* UtilityLib_h */

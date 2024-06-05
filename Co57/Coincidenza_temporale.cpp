
//------ ROOT lib
#include "TROOT.h"
#include "TStyle.h"
#include "TString.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TStyle.h"
#include "TF1.h"
#include "TH1F.h"
#include "TH1.h"
#include "TFrame.h"
#include "TLegend.h"

//------ c++ lib
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace std;


double funzione(double *x, double *par)
{	
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
}


int main() 
{
	// Controlla la posizione della legenda ed il tipo di entrate
	gStyle->SetOptStat("");
	gStyle->SetOptFit(1111);
	gStyle->SetStatX(0.95);
	gStyle->SetStatY(0.92);	
	gStyle->SetFitFormat("6.5g");

	// dichiarazioni
	ifstream input_file;	
		
	int n = 8192;		// numero canali
	double ti = 39.6654;	// tempo iniziale
	double tf = 1910.96;	// tempo finale

	double channel;
	double merda;	
	double z;		// conteggi

	TH1F* h1 = new TH1F("","",8192,ti,tf);
	input_file.open ("Raccolta_3.dat",ios::in);
	for(int i=0;i<n;i++) {
		input_file >> channel;
		input_file >> merda;
		input_file >> z;
 		
		h1->AddBinContent(i+1,z);
	} 
	input_file.close();

	TF1* f1 = new TF1("Funzione 1",funzione,100,1800,5);
	f1->SetLineColor(kBlack);
	f1->SetParName(0,"k");						// quota (C)
	f1->SetParName(1,"N");		f1->SetParameter(1,4000);	// coeff normalizz (A)
	f1->SetParName(2,"#mu");	f1->SetParameter(2,500);
	f1->SetParName(3,"#sigma");	f1->SetParameter(3,200);
	f1->SetParName(4,"#tau");	f1->SetParameter(4,200);
	h1->Fit("Funzione 1","Q","",100,1800);		

	h1->SetMaximum(5000);
	h1->SetFillColor(kOrange-3);
	h1->SetLineColor(kOrange+10);

	TCanvas* c1 = new TCanvas;	
	h1->Draw();	
	c1->SetGrid();

	// colore del frame
	c1->Update();
	c1->GetFrame()->SetFillColor(19);
	c1->Modified();

	c1->Print("Coincidenza_temporale.png","png");

	return 0;
}

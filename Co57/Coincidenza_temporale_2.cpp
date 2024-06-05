
// c++ -o Coincidenza_temporale `root-config --glibs --cflags` Coincidenza_temporale.cpp



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
#include "TLine.h"

//------ c++ lib
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace std;

// convoluzione gaussiana * esponenziale negativo
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

// propagazione degli errori (da controllare)
// controllare normalizzazione gauss <-> erf
double propagazione(double *par, double *epar, double x)
{
	double k = par[0];
	double N = par[1];
	double mu = par[2];
	double sigma = par[3];
	double tau = par[4];

	double ek = epar[0];
	double eN = epar[1];
	double emu = epar[2];
	double esigma = epar[3];
	double etau = epar[4];

	double A = (mu - x)/tau;
	double B = sigma*sigma/(2*tau*tau);

	double Num = (x - mu - sigma*sigma/tau);
	double Den = (sqrt(2)*sigma);

	double esp = exp(A + B);
	double quadra = (1 + erf(Num / Den));

	double Gauss = exp(-Num*Num/Den/Den);

	double Dermu_esp = esp/tau;
	double Dersigma_esp = esp*sigma/tau/tau;
	double Dertau_esp = esp*((x-mu)/tau/tau-sigma*sigma/tau/tau/tau);
	double Dermu_quadra = Gauss * (-1/sqrt(2)/sigma);
	double Dersigma_quadra = Gauss * (-(x-mu)/sqrt(2)/sigma/sigma - 1/tau/sqrt(2));
	double Dertau_quadra = Gauss * (sigma/sqrt(2)/tau/tau);

	double Derk = 1;
	double DerN = esp*quadra;
	double Dermu = Dermu_esp*quadra + Dermu_quadra*esp;
	double Dersigma = Dersigma_esp*quadra + Dersigma_quadra*esp;
	double Dertau = Dertau_esp*quadra + Dertau_quadra*esp;

	double sk = Derk*Derk*ek*ek;
	double sN = DerN*DerN*eN*eN;	
	double smu = Dermu*Dermu*emu*emu;
	double ssigma = Dersigma*Dersigma*esigma*esigma;
	double stau = Dertau*Dertau*etau*etau;

	return sqrt(sk + sN + smu + ssigma + stau);	

}


int main() 
{
	// Controlla la posizione della legenda ed il tipo di entrate
	gStyle->SetOptStat("");
	gStyle->SetOptFit(1111);
	gStyle->SetStatX(0.95);
	gStyle->SetStatY(0.92);	
	gStyle->SetFitFormat("6.5g");

// DICHIARAZIONI =====================================================

	ifstream input_file;	
		
	int n =8192;	// numero canali
	
	// calibrazione temporale daFreddo
	double ti = 39.6654;	// tempo iniziale
	double tf = 1910.96;	// tempo finale
	
	// range interpolazione
	int tmin = 120;
	int tmax = 1000;

	// range visivo
	int t_range_min = ti;
	int t_range_max = tf;

	// nomi file: faccio in modo che il nome dei png sia lo stesso del file txt
	// e che vengano stampati nella stessa cartella
	TString percorso = ("SpettroDefinitivo");
	TString file_txt = percorso + (".txt");
	TString file_png = percorso + (".png");
   	TString scarti_png = percorso + ("_scarti.png");


// LETTURA DATI =====================================================
	
	double channel;
	double merda;	
	double z;		// conteggi
	double conteggi=0;

	TH1F* h1 = new TH1F("","",n,ti,tf); //8192
	input_file.open (file_txt,ios::in);
	for(int i=0;i<n;i++) {
		input_file >> channel;
		input_file >> merda;
		input_file >> z;
		conteggi +=z;  		

		h1->AddBinContent(i+1,z);
	} 
	input_file.close();


// REBIN ============================================================

	int num_bin = (int) sqrt(conteggi);		// numero dei bin (errore della distribuzione)
	int rebin = (int) n / num_bin;			// coefficente che vuole .Rebin
		
//	TH1F* h2 = (TH1F*) h1->Rebin(4);					// metodo 1
	TH1F* h2 = dynamic_cast<TH1F*> (h1->Rebin(rebin));	// metodo 2


// INTERPOLAZIONE ===================================================

	TF1* f1 = new TF1("Funzione 1",funzione,tmin,tmax,5);
	f1->SetLineColor(kBlack);
	f1->SetParName(0,"k");		f1->SetParameter(0,0);		// quota (C)
	f1->SetParName(1,"N");		f1->SetParameter(1,8000);	// coeff normalizz (A)
	f1->SetParName(2,"#mu");	f1->SetParameter(2,400);	// media gaussiana
	f1->SetParName(3,"#sigma");	f1->SetParameter(3,90);		// dev standard gaussiana
	f1->SetParName(4,"#tau");	f1->SetParameter(4,141.5);	// tempo caratteristico
	h2->Fit("Funzione 1","Q","",tmin,tmax);	
/*
	TF1* f2 = new TF1("Funzione 2","gaus(0)",tmin,tmax);
	f2->SetLineColor(kBlue);
//	f2->SetParName(0,"k");		f2->SetParameter(0,71.077);		// quota (C)
//	f2->SetParName(1,"N");		f2->SetParameter(1,3954);	// coeff normalizz (A)
//	f2->SetParName(2,"#mu");	f2->SetParameter(2,399.69);
//	f2->SetParName(3,"#sigma");	f2->SetParameter(3,50);
//	f2->SetParName(4,"#tau");	f2->SetParameter(4,140);
	h2->Fit("Funzione 2","Q","",tmin,580);	
*/		


// ARTE E STAMPA ============================================
	
	h2->GetXaxis()->SetRangeUser(t_range_min,t_range_max);
//	h2->SetMaximum(40);
	h2->SetFillColor(kOrange-3);
	h2->SetLineColor(kOrange+10);	

	TCanvas* c1 = new TCanvas;	
	h2->Draw();
//	f2->Draw("SAME");
	c1->SetGrid();

	// colore del frame
	c1->Update();
	c1->GetFrame()->SetFillColor(19);
	c1->Modified();

	c1->Print(file_png,"png");


// SCARTI ===================================================

	double f1_eval;		// y della funzione
	double h2_eval;		// y dell'istogramma
	double scarto;		// scarto pesato
	double scarto_dir;	// scarto diretto
	double dt = (tf - ti)/ num_bin;	// larghezza bin
		
	double vett_vuoto[num_bin];		// vettore vuoto da mettere in TGraphError
		
	TGraphErrors* g1 = new TGraphErrors(num_bin,vett_vuoto,vett_vuoto,NULL,NULL);
	g1->SetMaximum(+1.1);
	g1->SetMinimum(-1.1);
		
	TString titolo_g1 = ("Scarti");
	g1->SetTitle(titolo_g1);
	g1->GetXaxis()->SetTitle("tempo [ns]");
	g1->GetYaxis()->SetTitle("scarti");

	// calcolo errore 
	double parametri[5];
	double parerr[5];
	
	parametri[0] = f1->GetParameter(0);
	parametri[1] = f1->GetParameter(1);
	parametri[2] = f1->GetParameter(2);
	parametri[3] = f1->GetParameter(3);
	parametri[4] = f1->GetParameter(4);

	parerr[0] = f1->GetParError(0);
	parerr[1] = f1->GetParError(1);
	parerr[2] = f1->GetParError(2);
	parerr[3] = f1->GetParError(3);
	parerr[4] = f1->GetParError(4);

	double errore;

	// calcolo skewness e kurtosis, commento in fondo *
	double sum2 = 0;
	double sum3 = 0;
	double sum4 = 0;
	double num_bin_scarti=0;
	
	// per ogni bin calcolo lo scarto associato
	for(int i=0; i< num_bin; i++){

		double t1 = ti + dt*(i + .5);	// valore temporale del bin

		// per rimanere nel range della funzione interpolante metto l'if
		if(t1 >= tmin && t1 <= tmax){	

			f1_eval = h2->GetFunction("Funzione 1")->Eval(t1);			
			h2_eval = h2->GetBinContent(i + 1);					
			errore = sqrt(pow(propagazione(parametri,parerr,t1),2) + conteggi);
			scarto_dir = (h2_eval - f1_eval);
			scarto = scarto_dir/errore;

			sum3 += pow(scarto_dir,3);
			sum4 += pow(scarto_dir,4);
			num_bin_scarti ++;
					
		} else {

			scarto = 0;

		}
		g1->SetPoint(i,t1,scarto);
	}

	g1->GetXaxis()->SetLimits(t_range_min,t_range_max);
		
	// Stampa ----------------------------------------------------------------------
	
	// inserisco una linea verticale corrispondente a mu
	TLine* linea_mu = new TLine(parametri[2],1,parametri[2],-1);
	linea_mu->SetLineColor(kRed); 

	// inserisco una linea verticale corrispondente al picco
	TLine* linea_picco = new TLine(f1->GetMaximumX(),1,f1->GetMaximumX(),-1);
	linea_picco->SetLineColor(kBlue); 


	// non riesce a mettere il colore e nemmeno il grid
	// regolare le proporzioni dei due grafici
	// regolare i titoli
	// regolare la scala dell'asse y, è possibile diminuire il numero delle tacche

	TCanvas* c2 = new TCanvas;
	c2->SetGrid();
	c2->Divide(1,2);
	c2->cd(1);	h1->Draw();	
	c2->cd(2);	g1->Draw();	
				linea_picco->Draw("same");
				linea_mu->Draw("same");

	// colore del frame
	c2->Update();
	c2->GetFrame()->SetFillColor(19);
	c2->Modified();

	c2->Print(scarti_png,"png");

	// skewness e kurtosis
	double skew = sum3/num_bin_scarti/pow(errore,3);
	double kurt = sum4/num_bin_scarti/pow(errore,4);
	cout << "Skewness = " << skew << endl;
	cout << "Kurtosis = " << kurt << endl;

	return 0;
}



// * normalmente questi valori si calcolano sugli scarti delle x,
//   ma io me ne sbatto il topocazzo e lo faccio sugli scarti delle y.
//   Definirla una manovra brutale è dir poco, bisogna andare a vedere bene le sigma 


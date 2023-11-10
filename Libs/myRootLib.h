/**************************************************************************************************************
                                 Libreria di supporto per oggetti di ROOT
**************************************************************************************************************/
// Created by Giacomo Fracasso
// Matteo Capitanis' UtilityLib is needed; its path must be specified here:
#ifndef myRootLib_h
#define myRootLib_h

#include "/Users/matteocapitani/Desktop/Università/3 Anno/Lab Nucleare👨🏼‍🔬/DataAnalysis/Libs/UtilityLib.h"
/*
// Std libs ----------------
#include <stdio.h>
#include <iostream>
#include <ostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <string>
#include <ctime>
#include <algorithm>

// Root Libs ---------------
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
*/

using namespace std;

// Matteo Capitanis' UtilityLib is needed; #include directive's at line 6 of .cc file
// These following functions are required by ComptonFunctHelp method:
/* |1. double VAmpG(TH1F *, TF1 *);  |2. double VMG(TF1 *);  |3. double VSG(TF1 *); */

/*******************************************************
 *                    D2HISTO CLASS                    *
 *******************************************************/

class D2histo
{
    /*      MEMBERS      */ 
private:
    TCanvas*           m_c;
    TPad*              m_p;
    TString      m_formula;
    unsigned int  m_Nbin=0; 

public:
    TH1*               m_h;
    TF1*               m_f;
    
    /*    CONSTRUCTORS   */

    // Default Constructor: declares nullptr objects
    D2histo ();
    // Personalized Constructor: every object has to already exist
    D2histo (TH1*, TF1*, TPad*, TCanvas*, TString form = "", int N_bin = 0);
    // Copy Constructor
    D2histo (const D2histo & orig);

    /*********************************************
     *            Compton Constructor            *
     * Requires:                                 *
     * - vector containing content per bin       *
     * - formula for TF1 object                  *
     * - optional: title for root objects        *
     *********************************************/
    template <typename myType>
    D2histo (vector<myType> data, TString formula, TString title = "");
    
    // Destructor
    ~D2histo ();
    
    /*      METHODS      */
    // BUILDERS ---------------------------------------------------------------------------
    // histoMaker's admitted options are: "bins" & "entries"
    template <typename myType>
    void histoMaker       (vector<myType>, TString, TString title = "");
    // Sets various options to an existing TF1, including fit range
    void gausFitHelper (double, double, char);
    // Sets the same title to all members of D2histo objects
    void SetMyTitle  (TString title = "Title", TString xAxis = "Bin", TString yAxis = "Counts", char color = '0');

    // DRAWERS ----------------------------------------------------------------------------
    // Manages D2histo's drawing and fit with his TF1 member
    TFitResultPtr MyDraw  ();
    // Overload for multiple fits (max 4 functions):
    vector<TFitResultPtr *> MyDraw (vector<TF1 *>);
    void elimPoint(float);

    /*  Friends  */
    // Functions from Capitani's Libraries that need acces to private members
    friend double VAmpG (TH1F *, TF1 *);
    friend double VMG   (TF1 *);
    friend double VSG   (TF1 *);
    // Class Friends
    friend TH1;
    friend TPad;
    friend TF1;
    friend TCanvas;
    friend TFrame;

};

#endif /* myRootLib_h */


class tStudent
{
public:
    /*   MEMBERS    */
    double      val_teo;
    double      val_mis;
    double      err_teo;
    double      err_mis;
    double      m_t;
    double      m_pro;
    double      x_min;
    double      x_max;
    TF1*        m_f_t;
    TF1*        m_f_m;
    TPad*       m_pad;
    TCanvas*    m_c;

    /* CONSTRUCTORS */

    // Default
    tStudent ();
    // Main
    tStudent (Double_t val_teo, Double_t err_teo, Double_t val_mis, Double_t err_mis);

    /*   METHODS    */
    void 
    ComparisonGraph (TString title = "0", TString xAxis = "0", TString yAxis = "0");
    Double_t
    MyTStudent      ();


};

// Function that colors the intersecated? area of two TF1
TGraph* IntersectionColor (TF1* f1, TF1* f2, double x_min, double x_max, Int_t COLOR = kGreen+1);

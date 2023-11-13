/**************************************************************************************************************
                                 Libreria di supporto per oggetti di ROOT
**************************************************************************************************************/

#include "/Users/matteocapitani/Desktop/Università/4 Anno/Laboratorio di Misure Nucleari/Co57/Libs/myRootLib.h"


/*******************************************************
 *             D2HISTO CLASS IMPLEMENTATION            *
 *******************************************************
 -------------------- CONSTRUCTORS ---------------------
 *******************************************************/

// Default Constructor. No pre-existing items are needed, no new items are allocated.
D2histo::D2histo ():
    m_c  (nullptr),
    m_h  (nullptr),
    m_f  (nullptr),
    m_p  (nullptr),
    m_formula (""),
    m_Nbin     (0)
{}
// Personalized Constructor. Every item must already exist.
D2histo::D2histo (TH1* h, TF1* f, TPad* p, TCanvas* c, TString form, int N_bin):
    m_c       (c),
    m_h       (h),
    m_f       (f),
    m_p       (p),
    m_formula (form),
    m_Nbin    (N_bin)
{}
// Copy Constructor
D2histo::D2histo (const D2histo & orig):
    m_c       (orig.m_c),
    m_h       (orig.m_h),
    m_f       (orig.m_f),
    m_p       (orig.m_p),
    m_formula (orig.m_formula),
    m_Nbin    (orig.m_Nbin)
{}
// MAIN Compton Constructor
template <typename myType>
D2histo::D2histo (vector<myType> data, TString formula, TString title):
    m_Nbin    (static_cast<uint> (data.size())),
    m_formula (formula),
    m_c       (new TCanvas),
    m_p       (new TPad),
    m_f       (new TF1  (title, formula))
{
    if (typeid(myType) == typeid(int)){
        this->m_h = new TH1I (title, title, m_Nbin, 0, m_Nbin);
    } else if (typeid(myType) == typeid(float)) {
        this->m_h = new TH1F (title, title, m_Nbin, 0, m_Nbin);
    } else if (typeid(myType) == typeid(double)) {
        this->m_h = new TH1D(title, title, m_Nbin, 0, m_Nbin);
    } else {
        cout << "UNSUPPORTED TYPE OF DATA! DEFAULT HISTOGRAM IS CREATED (double)" << endl;
        this->m_h = new TH1D(title, title, m_Nbin, 0, m_Nbin);
    }
    // Automatically fills the Histogram
    for (int j=0; j<m_Nbin; ++j)
    {
        // In case Saturated points have to be removed:
        // first 100 bins must be set to zero manually (before filling)
        m_h->SetBinContent(j, data.at(j));
    }
}

// Default DESTRUCTOR
D2histo::~D2histo ()
{
    delete m_c;
    delete m_h;
    delete m_p;
    delete m_f;
}

/*******************************************************
----------------------- METHODS ------------------------
*******************************************************/


// Fills the histogram, the object must have been created by default constructor
// Two options to build histograms from scratch are avaiable:
// - from bin contents
// - from entries
template <typename myType>
void
D2histo::histoMaker (vector<myType> data, TString option, TString title)
{
    int flag = 0;
    // First, "option" has to be verified being an admittable keyword
    if       (option == "B")    {flag = 1;}
    else if  (option == "E") {flag = 2;}
    else
    {
        cout << "ERROR: D2histo::histoMaker() permitted options are «B» for bins and «E» for entries" << endl;
        exit(EXIT_FAILURE);
    }

    // Next step is making sure no histogram was already allocated
    if (m_h != nullptr)
    {
        cout << "WARNING: D2histo object altready contains a histogram." << endl;
        cout << "To avoid this warning make sure the object was created by default constructor:" << endl;
        cout << "D2histo::D2histo(); m_h will be overwritten." << endl;
        cout << "Are you sure to continue? (y/n)" << endl;
        char check;
        cin >> check;
        if(check == 'n') {
            exit(EXIT_FAILURE);
        }
    }

    m_h->Reset();
    delete m_h;

    switch (flag)
    {
        case 1:
        /* First option: from bin contents */
        // Size of data is required to be the number of bins
        m_Nbin = static_cast<unsigned int> (data.size());
        
        if (typeid(myType) == typeid(int)){
            this->m_h = new TH1I (title, title, m_Nbin, 0, m_Nbin);
        } else if (typeid(myType) == typeid(float)) {
            this->m_h = new TH1F (title, title, m_Nbin, 0, m_Nbin);
        } else if (typeid(myType) == typeid(double)) {
            this->m_h = new TH1D(title, title, m_Nbin, 0, m_Nbin);
        } else {
            cout << "UNSUPPORTED TYPE OF DATA! DEFAULT HISTOGRAM IS CREATED (double)" << endl;
            this->m_h = new TH1D(title, title, m_Nbin, 0, m_Nbin);
        }
        // Filling
        for (int j=0; j<m_Nbin; ++j)
        {
            // Saturated points need to be removed: 
            // first 100 bins must be set to zero manually (before filling)
            m_h->SetBinContent(j, data.at(j));
        }
        break;

        case 2:
        /* Second option: from entries   */
        // Size of data will be interpreted as number of entries
        uint N_ent = static_cast<uint> (data.size());
        // Number of bins is set as square root of N_ent
        m_Nbin  = static_cast<int> ( sqrt(N_ent) );

        if (typeid(myType) == typeid(int)){
            this->m_h = new TH1I (title, title, m_Nbin, 0, m_Nbin);
        } else if (typeid(myType) == typeid(float)) {
            this->m_h = new TH1F (title, title, m_Nbin, 0, m_Nbin);
        } else if (typeid(myType) == typeid(double)) {
            this->m_h = new TH1D(title, title, m_Nbin, 0, m_Nbin);
        } else {
            cout << "UNSUPPORTED TYPE OF DATA! DEFAULT HISTOGRAM IS CREATED (double)" << endl;
            this->m_h = new TH1D(title, title, m_Nbin, 0, m_Nbin);
        }
        // Filling
        for (int j=0; j<N_ent; ++j)
        {
            // Saturated points need to be removed: 
            // first 100 bins must be set to zero manually (before filling)
            m_h->Fill(j, data.at(j));
        }
    }

    return ;
}

// Builds the desired function; parameters are set with some basic setting (see UtilittyLib.cc), using option R specifies the range
void 
D2histo::gausFitHelper (double fitMin = 0, double fitMax = 0, char opt = '0')
{
    if (opt == 'R' || opt == 'r'){
        m_f->SetRange(fitMin, fitMax);
    } else {
        m_f->SetRange(m_h->GetMinimum(), m_h->GetMaximum());
    }
    // Fit & Graphic Options
    m_f->SetParName(0, "Amp");
    m_f->SetParName(1, "#mu");
    m_f->SetParName(2, "#sigma");

    m_f->SetParameter(0, VAmpG(m_h, m_f));
    m_f->SetParameter(1, VMG  (m_f));
    m_f->SetParameter(2, VSG  (m_f));
    m_f->SetLineWidth(4);

    return;
}

// Sets universal titles according to what's required by compton exp
void 
D2histo::SetMyTitle (TString title, TString xAxis, TString yAxis, char color)
{
    static int h_counter = 0;
    TString help = "#bf{";
    help += title;
    help += "}";
    m_c->SetTitle(title);
    m_f->SetTitle(title);
    m_h->SetTitle(help);

    m_h->FitPanel();
    m_h->GetXaxis()->SetTitle(xAxis);
    m_h->GetYaxis()->SetTitle(yAxis);
    if (color == 'C') {
        m_h->SetLineColor(kBlue+h_counter);
    } else {
        m_h->SetLineColor(kBlue);
    }

    m_h->SetTitleFont(60, "xy");
    m_h->SetTitleFont(22, "t");
    gStyle->SetTitleFont(52, "t");

    ++h_counter;
    return ;
}

// Draws histogram, fitting it with the TF1 member m_f
TFitResultPtr
D2histo::MyDraw ()
{
    // Selects current Canvas
    m_c->cd();
    // Sets Default Canvas' Grid
    m_p->Draw();
    m_p->cd();
    m_p->SetGrid();
    m_p->SetFillStyle(4000);
    // Draws & Fits
    m_h->Draw();
    gStyle->SetOptFit(1112);
    TFitResultPtr* pt_res = new TFitResultPtr ( m_h->Fit(m_f, "SR+") );
    
    m_c->Modified();
    m_c->Update();

    return *pt_res;
}
// Draws histogram, fitting it with given TF1 max 4 
vector<TFitResultPtr *> 
D2histo::MyDraw (vector <TF1 *> gr_fit)
{
    // Selects current Canvas
    m_c->cd();
    // Sets Default Canvas' Grid
    m_p->Draw();
    m_p->cd();
    m_p->SetGrid();
    m_p->SetFillStyle(4000);
    // Draws
    m_h->Draw();
    gStyle->SetOptFit(1112);

    // Fits with every TF1, collecting TFitResultPtrs in a vector
    vector<TFitResultPtr *> v_pt_res;
    for (int i=0; i<gr_fit.size(); ++i)
    {
        v_pt_res.push_back( new TFitResultPtr ( m_h->Fit(gr_fit.at(i), "SR+") ) );
    }
    
    m_c->Modified();
    m_c->Update();

    return v_pt_res;


}
//per eliminare sistematicamente tutti i punti che non centrano niente, prende il massimo del fit, lo moltiplica per 5 ed mette a 0 tutti i punti con più count (adesso che ci penso questo si potrebbe fare con la derivata, se sale troppo rapidamente sono punti da togliere mmmh...)
void D2histo::elimPoint (float scale = 5.) //questo è proprio brutto, perchè gli chiedo di mettere il range due volte ma adesso non ho voglia di pensare come farlo meglio
{
    //m_f->SetRange(f_min, f_max);
    float max = VAmpG(m_h, m_f) * scale;
    
    for (uint i=0; i<m_Nbin; ++i)
    {
        if (m_h->GetBinContent(i) > max)
        {
            m_h->SetBinContent(i, 0);
        }
    }
    return;
}

/********   t-STUDENT TEST   ********/
// Between two values: expected & measured
// m: measured | t: expected (by Theory)

tStudent::tStudent ():
    val_teo (0),
    val_mis (0),
    err_teo (0),
    err_mis (0),
    x_min   (0),
    x_max   (0),
    m_t     (0),
    m_pro   (0),
    m_c     (nullptr),
    m_f_t   (nullptr),
    m_f_m   (nullptr),
    m_pad   (nullptr)
{}

// Il grafico viene centrato intorno al valore teorico
tStudent::tStudent (Double_t val_teo, Double_t err_teo, Double_t val_mis, Double_t err_mis):
    val_teo (val_teo),
    val_mis (val_mis),
    err_teo (err_teo),
    err_mis (err_mis),
    m_t     (0),
    m_pro   (0),
    m_c     (new TCanvas),
    m_pad   (new TPad)
{
    // Produzione degli estremi dei TF1
    // Basato sulla distanza tra le ascisse dei valori
    double delta_x = fabs(val_teo-val_mis);
    x_min = val_teo-3*delta_x;
    x_max = val_teo+3*delta_x;
    /* Basato sull'errore del valore teorico
    double err_max = err_teo;
    if (err_mis > err_teo) err_max = err_mis;
    x_min = val_teo-4*err_max;
    x_max = val_teo+4*err_max;
    */
    m_f_t = new TF1 ("", "gausn(0)", x_min, x_max);
    m_f_m = new TF1 ("", "gausn(0)", x_min, x_max);
}

void 
tStudent::ComparisonGraph (TString title, TString xAxis, TString yAxis)
{
    static int t_counter = 0;
    /* 1 */ /* Disegno del grafico di sovrapposizione delle gaussiane dei valori */
    // Argomenti richiesti:
    /* - valore teorico ed errore 
     * - valore misurato e errore
     * - titolo del grafico       */
    // Set dei parametri
    m_f_t->SetParName  (0, "Media");
    m_f_t->SetParameter("Media", val_teo);
    m_f_t->SetParName  (1, "DevSt");
    m_f_t->SetParameter("DevSt", err_teo);
    
    m_f_m->SetParName  (0, "Media");
    m_f_m->SetParameter("Media", val_mis);
    m_f_m->SetParName  (1, "DevSt");
    m_f_m->SetParameter("DevSt", err_mis);
    // Grafico e titoli
    m_c->cd();
    m_c->SetTitle(title);
    // Sets Default Canvas' Grid
    m_pad->Draw();
    m_pad->cd();
    m_pad->SetGrid();
    m_pad->SetFillStyle(4000);
    // Opzioni Grafiche
    m_f_t ->SetLineColor(kAzure+t_counter);
    m_f_t ->SetLineWidth(4);
    m_f_m ->SetLineColor(kRed+t_counter);
    m_f_m ->SetLineWidth(4);
    gStyle->SetTitleFont(52, "t");

    // Produzione del Titolo
    title += ";"; 
    title += xAxis;
    title += ";";
    title += yAxis;
    m_f_t->SetTitle(title);

    // Disegno delle Funzioni
    m_f_t->Draw();
    m_f_m->Draw("SAME");

    // Intersezione delle funzioni
    TGraph* g = IntersectionColor(m_f_m, m_f_t, x_min, x_max);
    g->Draw("fSAME"); // F che sta per fill

    // Per la Legenda
    TString teo = "Valore Teorico";
    TString mis = "Valore Misurato";
    /*
    teo += (val_teo); teo += (" " += xAxis);
    mis += (val_mis); mis += (" " += xAxis);
    */
    TLegend* leg1 = new TLegend(0.62,0.77,0.92,0.92); // Angolo alto a DX
    leg1->SetHeader("#bf{Legenda}", "C");
    leg1->AddEntry (m_f_t, teo, "lcp");
    leg1->AddEntry (m_f_m, mis, "lcp");
    leg1->AddEntry (g, "Intersezione", "lcp");
    leg1->Draw();

    // Linee che connettono i massimi delle gaussiane all'asse x
    // TLine Constructor:
    // TLine (x1, y1, x2, y2) where (x1,y1) & (x2,y2) are coordinates of the points to connect
    TLine* L1 = new TLine (val_teo, m_f_t->GetMaximum(), val_teo, 0);
    TLine* L2 = new TLine (val_mis, m_f_m->GetMaximum(), val_mis, 0);

    L1->SetLineStyle      (kDashed); // Elements enumerated in TAttLine class
    L2->SetLineStyle      (kDashed); // kSolid = 1 | kDashed = 2 | kDotted = 3 
    L1->SetLineColorAlpha (kAzure+t_counter, 0.9); // (color, opacity)
    L2->SetLineColorAlpha (kRed  +t_counter, 0.9);

    L1->Draw();
    L2->Draw();

    m_c->Modified();
    m_c->Update();

    /* 2 */ /* Calcolo T e P */ /* DA VERIFICARE */ /* DA METTERE NEL METODO: MyTStudent*/
    // Argomenti richiesti:
    /* - numero gradi di libertà 
     * -  */
    m_t   = fabs(val_mis - val_teo) / sqrt(err_teo*err_teo + err_mis *err_mis);
    m_pro = erfc(m_t); // CONTROLLARE COME SI FA LA PROB.

    // Stampa a schermo dei risultati
    cout << "\nt-Student = " << m_t << "    " << "prob = " << m_pro << endl;
    
    ++t_counter;

}

// Colorare l'area tra due TF1
TGraph* IntersectionColor (TF1* f1, TF1* f2, double x_min, double x_max, Int_t COLOR)
{
    // Returned TGraph will have to be Drawed with option "f" to allow fill color
    // Selects a bunch of points from the two TF1, choicing the minimum one between f1(x) and f2(x) 
    vector<double> points;

    Int_t N = Max(f1->GetNpx(), f2->GetNpx());
    TGraph* g = new TGraph (N);

    double dx = fabs(x_max-x_min)/N; // Increment variable
    
    for (Int_t i=0; i<N; ++i) 
    {
        double n_dx = dx*i,
               val1 = f1->Eval (n_dx),
               val2 = f2->Eval (n_dx);

        if   (val1 < val2) g->SetPoint(i, n_dx, val1);
        else               g->SetPoint(i, n_dx, val2); 
    }

    g->SetMarkerSize(0);
    g->SetLineStyle (kDashed);
    g->SetLineColorAlpha (COLOR, 0.9);
    g->SetLineWidth (2);
    g->SetFillColorAlpha (COLOR, 0.9);
    g->SetFillStyle (3002);

    return g;    
}

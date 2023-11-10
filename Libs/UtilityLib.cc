//
//  UtilityLib.cc
//
//
//  Created by Matteo Capitani on 03/05/22.
//  test

#include "/Users/matteocapitani/Desktop/Università/Tesi/AnalisiDati/TesiTriennale/Libs/UtilityLib.h"

using namespace std;

double RAD(double x)
{
    return (x*M_PI)/180.;
}

double gaussiana (double media, double sigma, double x)
{
    return 1./sqrt(2*M_PI*sigma*sigma) * exp(-(x-media)*(x-media)/2*sigma*sigma);
}

long long int fattoriale (int n)
{
    if (n <= 1) {
        return 1;
    }
        long long int f = n;
        for (int i=n-1; i>1; --i) {
            f *= i;
    }
    return f;
}

double Max(double x, double y)
{
    if (x>y) {
        return x;
    } else {
        return y;
    }
}

double Min(double x, double y)
{
    if (x<y) {
        return x;
    } else {
        return y;
    }
}

template <typename myType>
double media (vector<myType> data)
{
    double sum = 0;
    for (int i=0; i<data.size(); ++i){
        sum += data.at(i);
    }
    double n = 1.*sum/data.size();
    
    return n;
}

template <typename myType>
double sigma (vector<myType> data)
{
    double sumQ = 0;
    for (int i=0; i<data.size(); ++i){
        sumQ += data.at(i)*data.at(i);
    }
    return 1.*(sqrt((sumQ/data.size() - media(data)*media(data))));
}

double rand_range(double xMin, double xMax)
{
    return xMin + (xMax - xMin) * rand () / static_cast<double> (RAND_MAX) ;
}


template <typename myType>
vector<vector<myType>> invMAT (vector<vector<myType>> data)
{
    vector<vector<myType>> inv;
    
    int maxSize = 0;
    for (int i=0; i<data.size(); ++i)
    {
        if (maxSize < data.at(i).size())
        {
            maxSize = data.at(i).size();
        }
    }
    for (int i=0; i<maxSize; ++i)
    {
        vector<myType> tmp;
        for(int j=0; j<data.size(); ++j)
        {
            tmp.push_back(data.at(j).at(i));
        }
        inv.push_back(tmp);
    }
    return inv;
}


double sec (double x)
{
    return 1./sin(x);
}

double csc (double x)
{
    return 1./cos(x);
}

double cot (double x)
{
    return cos(x)/sin(x);
}

double sumQuad (double A, double B)
{
    return A*A + B*B;
}

double compTest (double media, double teorico, double sigma)
{
    return abs((media - teorico)/sigma);
}


//funzione per calcolare l'errore sul prodotto
double ERRprod (double mis1, double err1, double mis2, double err2)
{
    return (mis1*mis2) * sqrt(pow(err1/mis1, 2) + pow(err2/mis2, 2));
}
//funzione per calcolare l'errore sul rapporto
double ERRdiv (double mis1, double err1, double mis2, double err2)
{
    return (mis1/mis2) * sqrt(pow(err1/mis1, 2) + pow(err2/mis2, 2));
}


Double_t rettaAB(Double_t *x, Double_t *par)
{
    return par[0]+par[1]*x[0];
}
//retta passante da 0: y=Ax
Double_t retta(Double_t *x, Double_t *par)
{
    return par[0]*x[0];
}
//funzione sinusoidale A*sin(wx+phi)+k
Double_t sinusoide (Double_t *x, Double_t *par)
{
    return (par[0]*sin((par[1]*x[0])+par[2]))+par[3];
}
//gaussiana
Double_t gaussiana_root (Double_t *x, Double_t *par)
{
    return 1./(sqrt(2*M_PI*par[1]*par[1])) * exp(-(x[0]-par[0])*(x[0]-par[0])/(2*par[1]*par[1]));
}
//ENC^2 per il voltaggio
Double_t ENC_V (Double_t *x, Double_t *par)
{
    return sqrt(par[0]*(1/x[0]) + par[1]*x[0] + par[2]);
}
//funzione per rimuovere un carattere da una stringa (potrei passarli per ref così cambia il valore della stringa)
TString FindRemove(TString s1, char c)
{
    vector<char> cloneString;
    for (int j=0; j<s1.Sizeof(); ++j) {
        if (s1[j] != c && s1[j] != '\0' ) {
            cloneString.push_back(s1[j]);
        }
    }
    TString s;
    for (int j=0; j<cloneString.size(); ++j) {
        s.Append(cloneString.at(j));
    }
    return s;
}

TString FindRemove(TString s1, TString c)
{
    vector<char> cloneString;
    for (int j=0; j<s1.Sizeof(); ++j) {
        if (s1[j] != c && s1[j] != '\0' ) {
            cloneString.push_back(s1[j]);
        }
    }
    TString s;
    for (int j=0; j<cloneString.size(); ++j) {
        s.Append(cloneString.at(j));
    }
    return s;
}
template <typename myType>
vector<myType> readNUM (TString filepath)
{
    vector<myType> dati;

    // Legge i dati e li caccia in un vettore
    fstream bin_file;
    bin_file.open (filepath, ios::in);
    
    if (bin_file.good() != true)
    { cout << "READING FAILED" << endl; exit(EXIT_FAILURE); }

    while (bin_file.eof() != true)
    {
        myType n = 0;
        if (bin_file.eof() != true)
        {
            bin_file >> n;
            dati.push_back(n);
        }
    }

    bin_file.close();

    return dati;
}

//probabilmente posso farlo con i template ma non ho voglia di perderci 4 ore per capire come
vector<TString> readCHAR (TString filepath)
{
    vector<TString> dati;

    // Legge i dati e li caccia in un vettore
    fstream file;
    file.open (filepath, ios::in);
    if (file.good() != true)
    { cout << "READING FAILED" << endl; exit(EXIT_FAILURE); }

    while (file.eof() != true)
    {
        TString s;
        s.ReadToDelim(file, '\n');
        dati.push_back(s);
    }

    file.close();

    return dati;
}
template <typename myType>
vector<vector<myType>> readRANGE (TString filepath)
{
    vector<vector<myType>> data(2);

    fstream file;
    file.open (filepath, ios::in);
    if (file.good() != true)
    { cout << "READING FAILED" << endl; exit(EXIT_FAILURE); }

    while (file.eof() != true)
    {
        myType n = 0.;
        if (file.eof() != true)
        {
            for(int i=0; i<2; i++){
                file >> n;
                data.at(i).push_back(n);
            }
        }
    }
    data.at(0).pop_back(); //per rimuovere l'ultima riga che legge non so perchè
    data.at(1).pop_back();

    return data;
}

template <typename myType>
vector<pair<myType, myType>> readPAIR (TString filepath)
{
    vector<pair<myType, myType>> data;

    fstream file;
    file.open (filepath, ios::in);
    if (file.good() != true)
    { cout << "READING FAILED" << endl; exit(EXIT_FAILURE); }

    while (file.eof() != true)
    {
        pair<myType, myType> n;
        if (file.eof() != true)
        {
            file >> n.first;
            file >> n.second;
            data.push_back(n);
        }
    }
    data.pop_back();
    return data;
}

template <typename myType>
vector<vector<myType>> readMultiData(TString filepath, char c)
{
    vector<vector<myType>> data;
    
    fstream file;
    file.open (filepath, ios::in);
    if (file.good() != true)
    { cout << "READING FAILED" << endl; exit(EXIT_FAILURE); }
    
    //int count = 0;
    while (file.eof() != true)
    {
        int i=0;
        TString n = nullptr;
        vector<int> bin_int;
        
        //cout << n << endl;
        while(n != c && file.eof() != true) {
            file >> n;
            if (n != c && file.eof() != true) {
                bin_int.push_back(n.Atoi());
                ++i;
            }
        };
        
        //cout << i << endl;
        for (int j=0; j<i; ++j){
        }
        data.push_back(bin_int);
        //++count;
        //cout << count << endl;
    }
    //cout << file.eof() << endl;
    //cout << data.at(count).at(4);
    return data;
}


//piccola funzione per calcolare velocemente il punto per settare il parametro della media di un fit gaussiano facendo la media del range (Valor Medio Gaussiana)
double VMG (TF1 *f)
{
    return f->GetXmin() + (f->GetXmax() - f->GetXmin())/2 ;
}
//piccola funzione per calcolare velocemente il punto per settare il parametro dell'ampiezza di un fit gaussiano trovando la y corrispondente alla media del range (Valore Ampiezza Gaussiana)
double VAmpG (TH1 *h, TF1 *f)
{
    return h->GetBinContent(VMG(f)) + 1 ; //il +1 serve perchè se abbiamo pochi count c'è il rischio che il bin corrispondente al valor medio sia in realtà vuoto
}
//piccola funzione per calcolare velocemente il punto per settare il parametro della sigma di un fit gaussiano assumendo che il range sia circa 5sigma (Valore Sigma Gaussiana)
double VSG (TF1 *f)
{
    return (f->GetXmax() - f->GetXmin())/5 ;
}


TH1I quickDraw (TString filepath, TString title)
{
    fstream file;
    file.open (filepath, ios::in);
    if (file.good() != true)
    { cout << "READING FAILED" << endl; exit(EXIT_FAILURE); }
    
    vector<double> point = readNUM<double>(filepath);
    
    TCanvas c1;
    TH1I gr(title, title, 8192, 0, 1580.33); //A+B*8192
    
    TPad *grid1  = new TPad("grid", "", 0,0,1,1);
    grid1->Draw();
    grid1->cd();
    grid1->SetGrid();
    grid1->SetFillStyle(4000);
    
    for (int i=0; i<point.size(); ++i)
    {
        gr.SetBinContent(i+1, point.at(i));
    }
    
    gr.GetXaxis()->SetTitle("Energy (keV)");
    gr.GetYaxis()->SetTitle("Counts");
    gr.SetLineColor(kBlue);
    gr.SetTitle(title);
    
    return gr;
}


template <typename myType>
vector<myType> operator+ (vector<myType> X, vector<myType> Y)
{
    vector<myType> sum;
    int N = static_cast<int> (X.size());
    int M = static_cast<int> (Y.size());
    // sum = N > M ? X : Y;  SUPER FANCYYYYYY
    if (N >= M ) {
        N = M;
        sum = X;
        X = Y;
    } else {
    sum = Y;
        
    }
    for (int i=0; i<N; ++i)
    {
        sum.at(i) += X.at(i);
        
    }
    return sum;
}

template <typename myType>
vector<myType> operator- (vector<myType> X, vector<myType> Y)
{
    vector<myType> sub;
    
    if (Y.size() > X.size()) {
        X.resize(Y.size(), 0);
    } else {
        Y.resize(X.size(), 0);
    }
    sub = X;
    for (int i=0; i<X.size(); ++i)
    {
        sub.at(i) -= Y.at(i);
    }
    return sub;
}
//overloading dell'operatore - per fare la differenza tra un tipo e un intero (probabilmente dovrò farlo anche con gli altri)
template <typename myType>
vector<int> operator- (vector<myType> X, vector<int> Y)
{
    vector<int> sub;
    
    if (Y.size() > X.size()) {
        X.resize(Y.size(), 0);
    } else {
        Y.resize(X.size(), 0);
    }
    
    for (int i=0; i<X.size(); ++i)
    {
        double pf = 0;
        double pi = 0;
        pf = modf(X.at(i), &pi);
        if (pf < 0.5){
            sub.push_back(pi - Y.at(i));
        } else {
            sub.push_back(pi+1 - Y.at(i));
        }
    }
}

template <typename myType>
vector<int> operator- (vector<int> X, vector<myType> Y)
{
    vector<int> sub;
    
    if (Y.size() > X.size()) {
        X.resize(Y.size(), 0);
    } else {
        Y.resize(X.size(), 0);
    }
    
    for (int i=0; i<X.size(); ++i)
    {
        double pf = 0;
        double pi = 0;
        pf = modf(Y.at(i), &pi);
        
        if (pf < 0.5){
            sub.push_back(X.at(i) - pi);
        } else {
            sub.push_back(X.at(i) - pi+1);
        }
    }
    return sub;
}

template <typename myType>
vector<myType> operator* (vector<myType> X, vector<myType> Y)
{
    vector<myType> mol;
    int N = static_cast<int> (X.size());
    int M = static_cast<int> (Y.size());
    // sum = N > M ? X : Y;  SUPER FANCYYYYYY
    if (N >= M ) {
        N = M;
        mol = X;
        X = Y;
    } else {
    mol = Y;
    }
    for (int i=0; i<N; ++i)
    {
        mol.at(i)*= X.at(i);
        
    }
    return mol;
}

template <typename myType>
vector<myType> operator* (myType K, vector<myType> X)
{
    vector<myType> mol = X;
    int N = static_cast<int> (X.size());
    for (int i=0; i<N; ++i)
    {
        mol.at(i) *= K;
    }
    return mol;
}

template <typename myType>
vector<myType> operator/ (vector<myType> X, myType K)
{
    if (K == 0) {
        cout << "DIVISION BY 0 NOT ALLOWED!!" << endl;
        return X;
    }
    vector<myType> div = X;
    int N = static_cast<int> (X.size());
    for (int i=0; i<N; ++i)
    {
        div.at(i)/= K;
        
    }
    return div;
}
//C per stamparlo in colonna, L in linea, altro per mettere uno spazio tra un valore e l'altro
template <typename myType>
void printvec(vector<myType> vec, char opt)
{
    if (opt == 'C' || opt == 'c')
    {
        for (int i=0; i<vec.size(); ++i)
        {
            cout << vec.at(i) << endl;
        }
    } else if (opt == 'L' || opt == 'l')
    {
        for (int i=0; i<vec.size(); ++i)
        {
            cout << vec.at(i) << "\t";
        }
    } else {
        for (int i=0; i<vec.size(); ++i)
        {
            cout << vec.at(i) << " ";
        }
    }
    return ;
}
template <typename myType>
void printvec(vector<pair<myType, myType>> vec, vector<TString> str)
{
    for (int i=0; i<vec.size(); i++){
        cout << str.at(i) << ":\t" << vec.at(i).first << "\t" << vec.at(i).second << endl;
        }
    return ;
}

template <typename myType>
void printMultiVec(vector<vector<myType>> vec)
{
    vector<vector<myType>> output = invMAT(vec);
    
    for (int i=0; i<output.size(); ++i)
    {
        for (int j=0; j<output.at(i).size(); ++j)
        {
            cout << output.at(i).at(j) << "\t";
        }
        cout << endl;
    }
    return ;
}

//non capisco perchè non funzioni se templo anche "key"
template <typename myType>
vector<int> findvec(vector<myType> vec, TString key)
{
    vector<int> found;
    for (int i=0; i<vec.size(); i++)
    {
        if (vec.at(i) == key)
        {
            found.push_back(i);
        }
    }
    if (found.size() == 0)
    {
        cerr << "not found" << endl;
        exit(-1);
    }
    return found;
}


vector<TDatime> fillDate(const int size, TDatime startDate, const int interval)
{
    vector<TDatime> D;
    for (int i=0; i<size; ++i){
        D.push_back(startDate.Convert() + interval*i);
    }
    return D;
}

// Funzione che setta la griglia del TCanvas; vediamo se funziona
void SetMyGrid ()
{
    TPad *grid  = new TPad("grid", "", 0,0,1,1);
    grid->Draw();
    grid->cd();
    grid->SetGrid();
    grid->SetFillStyle(4000);

}

// Libera la memoria allocata da oggetti contenuti in vector
template <typename myType>
void Libera (vector<myType> vec)
{
    int N = static_cast<int> ( vec.size() );
    for (int i=N-1; i>=0; i--)
    {
        delete ( vec.at(i) );
    }
    vec.clear();
}

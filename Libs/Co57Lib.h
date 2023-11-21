//
//  Co57Lib.hpp
//  Co57
//
//  Created by Matteo Capitani on 13/11/23.
//

#ifndef Co57Lib_h
#define Co57Lib_h

#include "Libs/UtilityLib.h"

using namespace std;

extern const double err_HVPowerSupply;

template <typename myType>
vector<vector<myType>> readN957 (TString);

int readTIME (TString);
int readEVENTS (TString);

TH1* noFondo(TH1 *, int);

#endif /* Co57Lib_hpp */


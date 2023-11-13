//  Sorgente Co57 #79
//  TensioneLavoroNaI.cpp
//  Co57
//
//  Created by Matteo Capitani on 10/11/23.
//

#include <iostream>
#include "/Users/matteocapitani/Desktop/Università/4 Anno/Laboratorio di Misure Nucleari/Co57/Libs/myRootLib.cc"
#include "/Users/matteocapitani/Desktop/Università/4 Anno/Laboratorio di Misure Nucleari/Co57/Libs/UtilityLib.cc"
#include "/Users/matteocapitani/Desktop/Università/4 Anno/Laboratorio di Misure Nucleari/Co57/Libs/Co57Lib.cc"
using namespace std;


int main(int argc, char ** argv) {
    
    vector<vector<int>> data = readN957<int>("/Users/matteocapitani/Desktop/Università/4 Anno/Laboratorio di Misure Nucleari/Co57/Data/1-TensioneLavoroNaI/1250Volt-5min.txt");
    cout << data.at(0).size();
    
    return 0;
}

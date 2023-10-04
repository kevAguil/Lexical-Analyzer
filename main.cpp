//
//  main.cpp
//  PA1_K_Aguilar
//
//  Created by Kevin Aguilar on 2/16/23.
//

#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <sstream>
#include <map>
#include <regex>
#include <istream>
#include "lex.h"
#include <list>
#include <algorithm>

using namespace std;

int main(int argc, const char * argv[]) {
    int numOfLines = 0;
    int lineNumber = 0;
    ifstream file;
    LexItem tok;
    
    // No Specified Input File Name Case
    if(argc < 2){
        cout << "NO SPECIFIED INPUT FILE."<< endl;
        exit(0);
    }
    file.open(argv[1]);
    string fileName(argv[1]);
    
    // Cannot Open The File Case
    if(file.fail()){
        cout << "CANNOT OPEN THE FILE " << fileName << endl;
        exit(0);
    }
    
    // File is Empty Case
    if(file.peek() == std::ifstream::traits_type::eof()){
        cout << "Lines: " << numOfLines << endl;
        cout << "Empty File." << endl;
        exit(0);
    }
    
    // More than one File Name is Given Case
    for (int i=2; i<argc; i++){
        if(argv[i][0] != '-'){
            cout << "ONLY ONE FILE NAME IS ALLOWED." << endl;
            exit(0);
        }
    }
    
    // Checks for Arguments given
    bool vflag {false};
    bool identflag {false};
    bool sconstflag {false};
    bool nconstflag {false};
    
    // Checks Arguments Given
    for(int i=2; i<argc; i++){
        string argument = argv[i];
        if(argument.compare("-v") == 0){
            vflag = true;
            continue;
            }
 
        if(argument.compare("-ident") == 0){
            identflag = true;
            continue;
            }
            
        if(argument.compare("-nconst") == 0){
            nconstflag = true;
            continue;
            }
            
        if(argument.compare("-sconst") == 0){
            sconstflag = true;
            continue;
        }
        else{
            cout << "UNRECOGNIZED FLAG {" << argument <<'}' <<  endl;
            exit(0);
        }
        
    }

    // Collects Tokens
    int totalTokens = 0;
    int numOfIdentifiers = 0;
    int numOfNumbers = 0;
    int numOfStrings = 0;
    list <string> identList;
    list <string> stringList;
    list <string> numbersList;
    // Collects Token
    while((tok = getNextToken(file, lineNumber)) != DONE) {
        totalTokens++;
        if(tok.operator==(ERR)){
            cout << tok;
            exit(0);
        }
        
        if(vflag){
            cout << tok;
        }
        
        if(tok.operator==(IDENT) || tok.operator==(SIDENT) || tok.operator==(NIDENT) ){
            if(find(identList.begin(),identList.end(), tok.GetLexeme()) == identList.end()){
                numOfIdentifiers++;
                identList.push_back(tok.GetLexeme());
            }
        }
        if(tok.operator==(SCONST)){
            if(find(stringList.begin(), stringList.end(), tok.GetLexeme()) == stringList.end()){
                numOfStrings++;
                stringList.push_back(tok.GetLexeme());
            }
            
        }
        if(tok.operator==(ICONST) || tok.operator==(RCONST)){
            numOfNumbers++;
            numbersList.push_back(tok.GetLexeme());
            
        }
        

    }
    
    cout << "\nLines: " << lineNumber << endl;
    cout << "Total Tokens: " << totalTokens << endl;
    cout << "Identifiers: " << numOfIdentifiers << endl;
    cout << "Numbers: " << numOfNumbers << endl;
    cout << "Strings: " << numOfStrings << endl;
    
    if(identflag){
        cout << "IDENTIFIERS:" << endl;
        identList.sort();
        list <string> :: iterator iter;
        for(iter = identList.begin();iter != identList.end(); iter++){
            if(iter == identList.begin()){
                cout << *iter;
                continue;
            }
            cout << ", " << *iter;
        }
        cout << endl;
    }
    
    if(nconstflag){
        list <float> floatlist;
        cout << "NUMBERS:" << endl;
        list <string> :: iterator iter;
        for( iter = numbersList.begin(); iter != numbersList.end(); iter++){
            floatlist.push_back(stof(*iter));
        }
        floatlist.sort();
        list <float> :: iterator numIter;
        for( numIter = floatlist.begin(); numIter != floatlist.end(); numIter++){
            cout << *numIter << endl;
        }
        
    }
    
    if(sconstflag){
        cout << "STRINGS:" << endl;
        stringList.sort();
        list <string> :: iterator iter;
        for(iter = stringList.begin();iter != stringList.end(); iter++){
            cout << '\''<< *iter << '\''<< endl;

        }
    }
    
    return 0;
}

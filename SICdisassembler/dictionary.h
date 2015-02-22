#ifndef DICTIONARY_H_INCLUDED
#define DICTIONARY_H_INCLUDED
#include <map>
#include <cstdlib>
#include <string.h>
#include <fstream>
using namespace std;
typedef map <string, pair<string,char> > dictionary;
dictionary buildDictionary(){
    dictionary optable;
    ifstream in;
    in.open("opcode");
    while(in!=NULL){
        string s1,s2;
        char s3;
        in>>s1>>s2>>s3;
        optable[s2] = make_pair(s1,s3);
        //cout<<optable[s2].first<<endl;
    }
    in.close();
    return optable;
}

#endif // DICTIONARY_H_INCLUDED

#include <iostream>
#include "disassemblerPass.h"
#include <string.h>
#include <map>
#include <fstream>
using namespace std;
void headerRecord(string s){
    proglength = base10(s.substr(13,6));
    start = base10(s.substr(7,6));
    cout<<" \t"<<s.substr(7,6)<<"\t"<<s.substr(1,6)<<"\t"<<"START\t"<<s.substr(7,6)<<endl;
}
void textRecord(string s){
    firstPass(s);
}
string endRecord(string s){
    code[getAddress(s.substr(1,6))]->label = "LBL" + tostring(lastLabel);
    lastLabel++;
    return " \t \tEND\t"+code[getAddress(s.substr(1,6))]->label;
}
void printCode(){
    for(map <string,instruction*>::iterator it = code.begin();it!=code.end();++it){
        cout<<it->second->objcode<<"\t\t"<<it->second->loc<<"\t"<<it->second->label<<"\t"<<it->second->mnemonic<<"\t"<<it->second->target<<endl;
    }
}
void readObjectCode(string objectfile){
    ifstream in;
    in.open(objectfile.c_str());
    string endrec;
    while(in!=0){
        string s;
        getline(in,s);
        if(s[0]=='H')headerRecord(s);
        if(s[0]=='T')textRecord(s);
        if(s[0]=='E')endrec = endRecord(s);
    }
    secondPass();
    printCode();
    cout<<endrec<<endl;
    in.close();
}
int main(int argc, char** argv)
{
    optable = buildTable();
    readObjectCode(argv[1]);
    return 0;
}

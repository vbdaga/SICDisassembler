#include <iostream>
#include "./dictionary.h"
#include "./hexsystem.h"
#include <string.h>
#include <map>
#include <fstream>
#include <sstream>
using namespace std;
dictionary optable;
struct lineOfCode{
    string loc,label,mnemonic,target,objcode;
    lineOfCode(string s1,string s2,string s3,string s4,string s5){
        loc = s1;
        label = s2;
        mnemonic = s3;
        target = s4;
        objcode = s5;
    }
};
map <string , lineOfCode* > code;
int lastLabel=1;
int proglength;
int start;
string tostring(int x){
    stringstream s;
    s<<x;
    return s.str();
}
void headerRecord(string s){
    proglength = strtol(s.substr(13,6).c_str(),NULL,16);
    start = strtol(s.substr(7,6).c_str(),NULL,16);
    cout<<s.substr(7,6)<<"\t"<<s.substr(1,6)<<"\t"<<"START\t"<<s.substr(7,6)<<endl;
}
string getAddress(string s){
    int i=1;
    for(;i<s.size();i++){
        if(s[i]!='0')break;
    }
    //cout<<s.substr(i,7-i)<<endl;
    return s.substr(i,7-i);
}
void textRecord(string s){
    hex16 addr(getAddress(s));
    hex16 three("3");
    int lenOfCode = 2*strtol(s.substr(7,2).c_str(),NULL,16);
    //cout<<s<<endl;
    for(int i=9;i<9+lenOfCode;){
        string curr = s.substr(i,6);
        //if refrenced earlier
        if(code[addr.value]!=0){

            string tem = "";
            //check for word or byte
            if(code[addr.value]->mnemonic != ""){

                code[addr.value]->loc = addr.value;
                map <string , lineOfCode* > :: iterator it = code.find(addr.value);
                //check for it == code.end()
                it++;
                if(it==code.end()){
                    //cout<<addr.value<<endl;
                    int temsize = start + proglength - strtol(addr.value.c_str(),NULL,16);
                    if(temsize == 3 && curr[0]=='0' && curr[1]=='0'){
                        code[addr.value]->mnemonic = "WORD";
                        code[addr.value]->target = tostring(strtol(curr.c_str(),NULL,16));
                        code[addr.value]->objcode = curr;
                        i+=6;
                        addr = addr + three;
                    }
                    else{
                        //cout<<addr.value<<endl;
                        int length = temsize;
                        length*=2;
                        curr = s.substr(i,length);
                        code[addr.value]->mnemonic = "BYTE";
                        code[addr.value]->target = "X'"+curr+"'";
                        code[addr.value]->objcode = curr;
                        i+=length;
                        //addr = addr + diff;
                    }
                    break;
                }
                hex16 next(it->first);
                hex16 diff = next - addr;
                if(diff.value == "3" && curr[0]=='0' && curr[1]=='0'){
                    code[addr.value]->mnemonic = "WORD";
                    code[addr.value]->target = tostring(strtol(curr.c_str(),NULL,16));
                    code[addr.value]->objcode = curr;
                    i+=6;
                    addr = addr + three;
                }
                else{
                    //cout<<addr.value<<endl;
                    int length = diff.base10();
                    length*=2;
                    curr = s.substr(i,length);
                    code[addr.value]->mnemonic = "BYTE";
                    code[addr.value]->target = "X'"+curr+"'";
                    code[addr.value]->objcode = curr;
                    i+=length;
                    addr = addr + diff;
                }
            }
            //instruction is subroutine
            else{
                if(strtol(curr.substr(2,1).c_str(),NULL,16)>=8){
                    curr[2] = strtol(curr.substr(2,1).c_str(),NULL,16) - 8 +'0';
                }
                code[addr.value]->loc = addr.value;
                code[addr.value]->mnemonic = optable[curr.substr(0,2)].first;
                if(strtol(curr.substr(2,1).c_str(),NULL,16)>=8){
                    curr[2] = strtol(curr.substr(2,1).c_str(),NULL,16) - 8 +'0';
                }
                code[addr.value]->target = curr.substr(2,4);
                code[addr.value]->objcode = curr;
                if(optable[curr.substr(0,2)].second == 's'){
                    if(code[curr.substr(2,4)]==0){
                        code[curr.substr(2,4)] = new lineOfCode(curr.substr(2,4),"LBL" + tostring(lastLabel),"","","");
                        lastLabel++;
                    }
                }
                else{
                    if(code[curr.substr(2,4)]==0){
                        code[curr.substr(2,4)] = new lineOfCode(curr.substr(2,4),"","RESB","","");
                    }
                }
                i+=6;
                addr = addr + three;
            }
        }
        //if new instruction without previous reference
        else{

            if(optable[curr.substr(0,2)].second == 'p'){
                code[addr.value] = new lineOfCode(addr.value,"",optable[curr.substr(0,2)].first,"",curr);
                i+=6;
                addr = addr + three;
                continue;
            }
            if(strtol(curr.substr(2,1).c_str(),NULL,16)>=8){
                curr[2] = strtol(curr.substr(2,1).c_str(),NULL,16) - 8 +'0';
            }
            //cout<<optable[curr.substr(0,2)].first<<endl;
            code[addr.value] = new lineOfCode(addr.value,"",optable[curr.substr(0,2)].first,curr.substr(2,4),curr);
            if(optable[curr.substr(0,2)].second == 's'){
                if(code[curr.substr(2,4)]==0){
                    code[curr.substr(2,4)] = new lineOfCode(curr.substr(2,4),"LBL" + tostring(lastLabel),"","","");
                    lastLabel++;
                }
                else{
                    code[curr.substr(2,4)]->label = "LBL" + tostring(lastLabel);
                    lastLabel++;
                }
            }
            else{
                if(code[curr.substr(2,4)]==0){
                    code[curr.substr(2,4)] = new lineOfCode(curr.substr(2,4),"","RESB","","");
                }
            }
            i+=6;
            addr = addr + three;
        }
    }
    //cout<<addr.value<<endl;
}
void secondPass(){
    map < string , lineOfCode* > :: iterator it = code.begin();
    for(;it!=code.end();it++){
        if(it->second->mnemonic == "RESB"){
            map < string , lineOfCode* > :: iterator it1 = it;
            it1++;
            if(it1==code.end()){
                //fill this
                int temsize = start + proglength - strtol(it->first.c_str(),NULL,16);
                it->second->target = tostring(temsize);
                break;
            }
            hex16 curr(it->first);
            hex16 next(it1->first);
            hex16 diff = next - curr;
            int len = strtol(diff.value.c_str(),NULL,16);
            it->second->target = tostring(len);
        }
        else if(optable[it->second->objcode.substr(0,2)].second == 's'){
            it->second->target = code[it->second->target]->label;
        }
    }
}
string endRecord(string s){
    code[getAddress(s.substr(1,6))]->label = "LBL" + tostring(lastLabel);
    lastLabel++;
    return " \t \tEND\t"+code[getAddress(s.substr(1,6))]->label;
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
    for(map <string,lineOfCode*>::iterator it = code.begin();it!=code.end();++it){
        cout<<it->second->loc<<"\t"<<it->second->label<<"\t"<<it->second->mnemonic<<"\t"<<it->second->target<<"\t"<<it->second->objcode<<endl;
    }
    cout<<endrec<<endl;
    in.close();

}
int main(int argc, char** argv)
{
    optable = buildDictionary();
    readObjectCode(argv[1]);
    return 0;
}

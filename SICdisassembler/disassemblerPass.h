#ifndef DISASSEMBLERPASS_H_INCLUDED
#define DISASSEMBLERPASS_H_INCLUDED
#include "./dictionary.h"
#include "./numsystem.h"
#include "instruction.h"
typedef map <string , instruction* > lineOfCode;
table optable;
lineOfCode code;
int proglength;
int start;
int lastLabel=1;
void firstPass(string s){
    hexadecimal address(getAddress(s));
    hexadecimal three("3");
    int lenOfCode = 2*base10(s.substr(7,2));
    for(int i=9;i<9+lenOfCode;){
        string curr = s.substr(i,6);

        //if address referenced earlier
        if(code[address.value]!=0){
            string tem = "";

            //check for word or byte
            if(code[address.value]->mnemonic != ""){
                code[address.value]->loc = address.value;
                map <string , instruction* > :: iterator it = code.find(address.value);
                it++;

                //if we are at the last address
                if(it==code.end()){
                    int temsize = start + proglength - base10(address.value);
                    if(temsize == 3 && curr[0]=='0' && curr[1]=='0'){
                        code[address.value]->mnemonic = "WORD";
                        code[address.value]->target = tostring(base10(curr));
                        code[address.value]->objcode = curr;
                        i+=6;
                        address = address + three;
                    }
                    else{
                        int length = temsize;
                        length*=2;
                        curr = s.substr(i,length);
                        code[address.value]->mnemonic = "BYTE";
                        code[address.value]->target = "X'"+curr+"'";
                        code[address.value]->objcode = curr;
                        i+=length;
                    }
                    break;
                }

                hexadecimal next(it->first);
                hexadecimal diff = next - address;
                if(diff.value == "3" && curr[0]=='0' && curr[1]=='0'){
                    code[address.value]->mnemonic = "WORD";
                    code[address.value]->target = tostring(base10(curr));
                    code[address.value]->objcode = curr;
                    i+=6;
                    address = address + three;
                }
                else{
                    int length = diff.base10();
                    length*=2;
                    curr = s.substr(i,length);
                    code[address.value]->mnemonic = "BYTE";
                    code[address.value]->target = "X'"+curr+"'";
                    code[address.value]->objcode = curr;
                    i+=length;
                    address = address + diff;
                }
            }

            //instruction is subroutine
            else{
                code[address.value]->objcode = curr;

                //check for indexed addressing
                if(base10(curr.substr(2,1))>=8){
                    curr[2] = base10(curr.substr(2,1)) - 8 +'0';
                }
                code[address.value]->loc = address.value;
                code[address.value]->mnemonic = optable[curr.substr(0,2)].first;
                code[address.value]->target = curr.substr(2,4);

                //check for type of address
                if(optable[curr.substr(0,2)].second == 's'){
                    if(code[curr.substr(2,4)]==0){
                        code[curr.substr(2,4)] = new instruction(curr.substr(2,4),"LBL" + tostring(lastLabel),"","","");
                        lastLabel++;
                    }
                }
                else{
                    if(code[curr.substr(2,4)]==0){
                        code[curr.substr(2,4)] = new instruction(curr.substr(2,4),"","RESB","","");
                    }
                }
                i+=6;
                address = address + three;
            }
        }

        //if new instruction without previous reference
        else{

            //if instruction is RSUB
            if(optable[curr.substr(0,2)].second == 'p'){
                code[address.value] = new instruction(address.value,"",optable[curr.substr(0,2)].first,"",curr);
                i+=6;
                address = address + three;
                continue;
            }

            code[address.value] = new instruction(address.value,"",optable[curr.substr(0,2)].first,curr.substr(2,4),curr);
            if(base10(curr.substr(2,1))>=8){
                curr[2] = base10(curr.substr(2,1)) - 8 +'0';
            }
            code[address.value]->target = curr.substr(2,4);

            //check for type of target address
            if(optable[curr.substr(0,2)].second == 's'){
                if(code[curr.substr(2,4)]==0){
                    code[curr.substr(2,4)] = new instruction(curr.substr(2,4),"LBL" + tostring(lastLabel),"","","");
                    lastLabel++;
                }
                else{
                    code[curr.substr(2,4)]->label = "LBL" + tostring(lastLabel);
                    lastLabel++;
                }
            }
            else{
                if(code[curr.substr(2,4)]==0){
                    code[curr.substr(2,4)] = new instruction(curr.substr(2,4),"","RESB","","");
                }
            }
            i+=6;
            address = address + three;
        }
    }
}
void secondPass(){
    map < string , instruction* > :: iterator it = code.begin();
    for(;it!=code.end();it++){

        //if address is marked as RSEB
        if(it->second->mnemonic == "RESB"){
            map < string , instruction* > :: iterator it1 = it;
            it1++;
            if(it1==code.end()){
                //fill this
                int temsize = start + proglength - base10(it->first);
                it->second->target = tostring(temsize);
                break;
            }
            hexadecimal curr(it->first);
            hexadecimal next(it1->first);
            hexadecimal diff = next - curr;
            int len = base10(diff.value);
            it->second->target = tostring(len);
        }
        //update label for instructions
        else if(optable[it->second->objcode.substr(0,2)].second == 's'){
            it->second->target = code[it->second->target]->label;
        }
        else if(base10(it->second->objcode.substr(2,1))>=8){
            it->second->target+=" , X";
        }
    }
}
#endif // DISASSEMBLERPASS_H_INCLUDED

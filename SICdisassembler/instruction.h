#ifndef INSTRUCTION_H_INCLUDED
#define INSTRUCTION_H_INCLUDED

struct instruction{
    string loc,label,mnemonic,target,objcode;
    instruction(string s1,string s2,string s3,string s4,string s5){
        loc = s1;
        label = s2;
        mnemonic = s3;
        target = s4;
        objcode = s5;
    }
};
string getAddress(string s){
    int i=1;
    for(;i<s.size();i++){
        if(s[i]!='0')break;
    }
    //cout<<s.substr(i,7-i)<<endl;
    return s.substr(i,7-i);
}
#endif // INSTRUCTION_H_INCLUDED

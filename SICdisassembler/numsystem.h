#ifndef HEXSYSTEM_H_INCLUDED
#define HEXSYSTEM_H_INCLUDED
#include <map>
#include <string.h>
#include <sstream>
#include <cstdlib>
using namespace std;
struct hexadecimal{
    string value;
    hexadecimal(string s){
        value = s;
    }
    hexadecimal operator + (const hexadecimal& b){
        string sum;
        int first = strtol(this->value.c_str(),NULL,16);
        int second = strtol(b.value.c_str(),NULL,16);
        int s = first +second;
        sum = "";
        while(s>0){
            int x = s%16;
            string tem = "0";
            if(x<10){
                tem[0] = tem[0]+x;
                sum = tem+sum;
            }
            else{
                tem[0] = 'A' + x -10;
                sum = tem+sum;
            }
            s=s/16;
        }
        hexadecimal c(sum);
        return c;
    }
    hexadecimal operator - (const hexadecimal& b){
        string sum;
        int first = strtol(this->value.c_str(),NULL,16);
        int second = strtol(b.value.c_str(),NULL,16);
        int s = first - second;
        sum = "";
        while(s>0){
            int x = s%16;
            string tem = "0";
            if(x<10){
                tem[0] = tem[0]+x;
                sum = tem+sum;
            }
            else{
                tem[0] = 'A' + x -10;
                sum = tem+sum;
            }
            s=s/16;
        }
        hexadecimal c(sum);
        return c;
    }
    int base10(){
        return strtol(this->value.c_str(),NULL,16);
    }
};
int base10(string s){
    return strtol(s.c_str(),NULL,16);
}
string tostring(int x){
    stringstream s;
    s<<x;
    return s.str();
}

#endif // HEXSYSTEM_H_INCLUDED

#ifndef HEXSYSTEM_H_INCLUDED
#define HEXSYSTEM_H_INCLUDED
#include <map>
#include <string.h>
#include <sstream>
#include <cstdlib>
using namespace std;
struct hex16{
    string value;
    hex16(string s){
        value = s;
    }
    hex16 operator + (const hex16& b){
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
        hex16 c(sum);
        return c;
    }
    hex16 operator - (const hex16& b){
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
        hex16 c(sum);
        return c;
    }
    int base10(){
        return strtol(this->value.c_str(),NULL,16);
    }
};


#endif // HEXSYSTEM_H_INCLUDED

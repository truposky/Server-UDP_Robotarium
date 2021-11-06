#include <iostream>
#include <stdlib.h>
#include <cstring> 
#include <stdio.h>
using namespace std;
int main(){


    char array[256];


    double wD=20.023;
    double wI=20.004;
    char del=',';
    sprintf(array,"%lf",wD);
    sprintf(array+sizeof(wD),"%c",del);
    sprintf(array+sizeof(wD)+sizeof(del),"%lf",wI);
    cout<<array<<endl;
    cout<<sizeof(wD)+sizeof(wI)+sizeof(del)<<endl;
    return 0;
}
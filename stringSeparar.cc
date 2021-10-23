#include <iostream>
#include <string>
#include <sstream>
#include <bits/stdc++.h>
using namespace std;

const int PORT = 4242;
const int MAXDATASIZE = 256; //numero de bytes que se pueden recibir
const int HEADER_LEN = sizeof(unsigned short)*3;
const int MAXROBOTS = 4;
struct appdata{

        unsigned short id; //identificador
        unsigned short op; //codigo de operacion
        unsigned short len; //longitud de datos

        char data [MAXDATASIZE-HEADER_LEN];//datos
        //nota¡ actualizar char data a string o puntero para que sea mas versatil.


};
struct appdata operation_send;

int main(){
    string del=":";
    string data = "10:200";
    string s=data;
     int cont=0;
    int start = 0;
    int end = data.find(del);
    cout<<"end "<<end<<endl;
    string word;
    while (end != -1) {
        
        word=data.substr(start, end - start);
       operation_send.op=stoul(word,nullptr,0);
        cout<<"palabra1:"<<word<<endl;
        cout<<"data "<<data<<endl;
       
     
        start = end + del.size();
        cout<<"start: "<<start<<endl;
        end = data.find(del, start);
        cout<<"end "<<end<<endl;
        cout<<del.size()<<endl;
    cont++;
    }
     word=data.substr(start, end - start);
        strcpy(operation_send.data,word.c_str());
        cout<<"palabra"<<word<<endl;
    return 0;
}
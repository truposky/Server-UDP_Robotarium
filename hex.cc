#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>


using namespace std;
// function to convert decimal to hexadecimal
void decToHexa(int n)
{
    // char array to store hexadecimal number
    char hexaDeciNum[100];
 
    // counter for hexadecimal number array
    int i = 0;
    while (n != 0) {
        // temporary variable to store remainder
        int temp = 0;
 
        // storing remainder in temp variable.
        temp = n % 16;
 
        // check if temp < 10
        if (temp < 10) {
            hexaDeciNum[i] = temp + 48;
            i++;
        }
        else {
            hexaDeciNum[i] = temp + 55;
            i++;
        }
 
        n = n / 16;
    }
 
    // printing hexadecimal number array in reverse order
    for (int j = i - 1; j >= 0; j--)
        cout << hexaDeciNum[j];
}

int main()
{
    unsigned short value,value2;
    stringstream ss;
    cout<<"escribe numero:";
    cin>>hex>>value;
    cin>>value2;    
    cout<<"ha escrito :"<<value<<endl;
    cout<<sizeof(value)<<endl;
    ss<<hex<<value2;
    string res ( ss.str() );
    cout<<res<<endl;
}
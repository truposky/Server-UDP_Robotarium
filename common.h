#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include <pthread.h>
#include <errno.h>

const int PORT = 4242;
const int MAXDATASIZE = 256; //numero de bytes que se pueden recibir
const int HEADER_LEN = sizeof(unsigned short)*3;

//direcciones IP del servidor y de los distintos robots
const char IP_SERVER[] = "192.168.2.10";
const char arduino1[] ="192.168.2.5";
const char arduino2[] ="192.168.2.6";
const char arduino3[] ="192.168.2.7";
const char arduino4[] ="192.168.2.8";
const char arduino5[] ="192.168.2.9";


struct appdata{

        unsigned short op; //codigo de operacion
        unsigned short id; //identificador
        unsigned short len; //longitud de datos

        char data [MAXDATASIZE-HEADER_LEN];//datos


};

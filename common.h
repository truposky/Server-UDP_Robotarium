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
#include <fcntl.h>
#include <pthread.h>
#include <errno.h>

const int PORT = 4242;
const int MAXDATASIZE = 256; //numero de bytes que se pueden recibir
const int HEADER_LEN = sizeof(unsigned short)*3;
const int MAXROBOTS = 4;
const char IP_SERVER[] = "192.168.2.2";
/*//direcciones IP del servidor y de los distintos robots

const char arduino1[] ="192.168.2.5";
const char arduino2[] ="192.168.2.6";
const char arduino3[] ="192.168.2.7";
const char arduino4[] ="192.168.2.8";
const char arduino5[] ="192.168.2.9";
*/

struct appdata{

        unsigned short id; //identificador
        unsigned short op; //codigo de operacion
        unsigned short len; //longitud de datos

        char data [MAXDATASIZE-HEADER_LEN];//datos


};
//operacion error
#define OP_ERROR            0xFF
//operaciones requeridas por servidor
#define OP_SALUDO           0x00
#define OP_MOVE_FORWARD     0x01
#define OP_MOVE_BACKWARD    0x02
#define OP_MOVE_RIGHT       0x03
#define OP_MOVE_LEFT        0x04
//operaciones requeridas por cliente
#define OP_WAIT_INSTRUCTION 0x99
//OP_SALUDO

struct appdata *operation;//message received of operation 
struct appdata resultado;
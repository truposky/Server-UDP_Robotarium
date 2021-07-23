
//#include "common.h"
#include "robot.hh"
using namespace std;

//UDP parameters
int sockfd;
struct sockaddr_in my_addr;
struct sockaddr_in robot_addr;

char buf[MAXDATASIZE];//this buf is de content of the message
socklen_t sin_size;
enum {r1, r2, r3, r4,r5};
     //definition of robots
    Robot robot1,robot2,robot3,robot4;


//prototypes
int clasification (char* client_addr);
void comRobot1();
void comRobot2();
void comRobot3();
void comRobot4();

int main (){
 

   
    int numbytes;
    int error;
    int cont=0;//count for know the different robot operative
    bool condicion=true;
  

    //Se crea el socket
    if((sockfd=socket(AF_INET,SOCK_DGRAM,0))==-1){
        perror("socket");
        exit(1);

    }
             // set timeout
    struct timeval timeout;
         timeout.tv_sec = 10;//sec
         timeout.tv_usec = 0;//microsecond
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1) {
        perror("setsockopt failed:");
    }
    my_addr.sin_family=AF_INET;//familia: ordenacion de bytes de la maquina
    my_addr.sin_port=htons(PORT);//puerto:ordenacion de bytes de la red
    my_addr.sin_addr.s_addr=inet_addr(IP_SERVER); //IP: ordenacion de bytes de la red
    memset(&(my_addr.sin_zero),'\0',8); //pone a cero el resto de la estructura

    if(bind(sockfd,(struct sockaddr*) &my_addr,sizeof(struct sockaddr_in))==-1){//asigna el socket a un puerto local

        perror("bind");
        exit(1);
    }

    //loop
    while(condicion){
        
        cout<<"(Servidor) esperando mensajes [puerto local]"<<ntohs(my_addr.sin_port)<<endl;

        //se recibe mensaje de cliente

        memset(buf,'\0',MAXDATASIZE);//se pone a cero el buffer
        sin_size=sizeof(struct sockaddr_in);
       //saludo inicial de los distintos robots
        if((numbytes=recvfrom(sockfd,buf,MAXDATASIZE,0,(struct sockaddr*)&robot_addr, &sin_size))==-1){
            
            perror("recvfrom");
            if(cont >= MAXROBOTS){
            close (sockfd);// se cierra el socket para la comunicacion inicial
            condicion=false;
            }
            else{
                char respuesta;
                cout<<"faltan "<<MAXROBOTS-cont<<" por estar operativos, desea continuar o esperar (y) (n)"<<endl;
                cin>>respuesta;
                if(respuesta=='y'){
                    condicion=true;
                }
                else{
                     condicion=false;   
                }
            }
        }
        else{//si detecta la conexion o un mensaje se responde y se envia una respuesta en consecuencia
            cont++;//un robot mas conectado
            cout<<"(servidor) mensaje recibido de IP:"<<inet_ntoa(robot_addr.sin_addr)<<" puerto ["<<ntohs(robot_addr.sin_port)<<"] longitud["<<numbytes<<"]"<<endl;
            cout<<"(cliente) "<<buf<<endl;

            operation=( struct appdata*)&buf;
            if((numbytes< HEADER_LEN) || (numbytes != operation->len+HEADER_LEN) ){

                cout<<"(servidor) unidad de datos incompleta\n";
                continue;
            }
            else{
                
                cout<<"(servidor) id "<<operation->id;
                cout<<" operacion solicitada [op 0x]"<<operation->op;
                cout<<" contenido "<<operation->data<<endl;
            }

            /* relaiza operacion solicitada por el cliente */
            error=0;
            resultado.id=operation->id;//id

            switch (operation->op){
                case OP_SALUDO:
                    resultado.op=OP_SALUDO;
                    strcpy(resultado.data, "Saludo recibido");
                    resultado.len =strlen(resultado.data);
                break;
                case OP_WAIT_INSTRUCTION:
                    cout<<"esperando instruccion, escriba operacion y datos si es necesario"<<endl;
                    cin>>resultado.op>>resultado.data;
                break;
                default: /*operacion desconocida*/
                    resultado.op=OP_ERROR;
                    strcpy(resultado.data, "operacion desconocida");
                    resultado.len =strlen(resultado.data);
                    error=1;
                    break;
            }
            if((numbytes=sendto(sockfd,(char*) &resultado,resultado.len + HEADER_LEN,0,(struct sockaddr*)&robot_addr,sizeof(struct sockaddr_in)))==-1){
                perror("recv");
                continue;
            }
            else{
                cout<<"(servidor) mensaje enviado al cliente"<<endl;
            }
        }



    }

    //una vez que se saben los robots operativos se procede a enviar las corrrespondientes instrucciones
    while(1){
                    //primero se elige al robot al que se desea mandar una determinada instruccion
                cout<<"(0)robot1"<<endl<<"(1)robot2"<<endl<<"(2)robot3"<<endl<<"(3)robot4"<<endl;
                cout<<"eliga el robot con el cual desea comunicarse"<<endl;
                cin>>cont;

                switch (cont)
                {
                case r1 :
                    comRobot1();
                    break;
                
                case r2 :
                    comRobot2();
                    break;

                
                case r3 :
                    comRobot3();
                    break;

                
                case r4 :
                    comRobot4();
                    break;
                    
                default:
                    break;
                
            }
           



        }
}





/*int clasification (char* client_addr){
    int state;
   

    if (!(strcmp(arduino1,client_addr))){
        state=robot1;
    }
    else if(!(strcmp(arduino2,client_addr))){
        state=robot2;
    }
    else if(!(strcmp(arduino3,client_addr))){
        state=robot3;
    }
    else if(!(strcmp(arduino4,client_addr))){
        state=robot4;
    }
    else if(!(strcmp(arduino5,client_addr))){
        state=robot5;
        
    }        

    return state;
}
*/

    
void comRobot1() {

            //Se crea el socket
            if((sockfd=socket(AF_INET,SOCK_DGRAM,0))==-1){
                perror("socket");
                
            }
            /*se convierte de string a char*/
            const char *char_ip=robot1.ip.c_str();
            /*---------------------*/

            robot_addr.sin_family=AF_INET;//familia: ordenacion de bytes de la maquina
            robot_addr.sin_port=htons(robot1.port);//puerto:ordenacion de bytes de la red
            robot_addr.sin_addr.s_addr=inet_addr(char_ip); //IP: ordenacion de bytes de la red
            memset(&(robot_addr.sin_zero),'\0',8); //pone a cero el resto de la estructura
            memset (buf, '\0', MAXDATASIZE); /* Pone a cero el buffer inicialmente */
}

void comRobot2(){

            //Se crea el socket
            if((sockfd=socket(AF_INET,SOCK_DGRAM,0))==-1){
                perror("socket");
              
            }
            /*se convierte de string a char*/
            const char *char_ip=robot2.ip.c_str();
            /*---------------------*/

            robot_addr.sin_family=AF_INET;//familia: ordenacion de bytes de la maquina
            robot_addr.sin_port=htons(robot2.port);//puerto:ordenacion de bytes de la red
            robot_addr.sin_addr.s_addr=inet_addr(char_ip); //IP: ordenacion de bytes de la red
            memset(&(robot_addr.sin_zero),'\0',8); //pone a cero el resto de la estructura
            memset (buf, '\0', MAXDATASIZE); /* Pone a cero el buffer inicialmente */
}

void comRobot3(){

            //Se crea el socket
            if((sockfd=socket(AF_INET,SOCK_DGRAM,0))==-1){
                perror("socket");
                
            }
            /*se convierte de string a char*/
           const char *char_ip=robot3.ip.c_str();

            robot_addr.sin_family=AF_INET;//familia: ordenacion de bytes de la maquina
            robot_addr.sin_port=htons(robot3.port);//puerto:ordenacion de bytes de la red
            robot_addr.sin_addr.s_addr=inet_addr(char_ip); //IP: ordenacion de bytes de la red
            memset(&(robot_addr.sin_zero),'\0',8); //pone a cero el resto de la estructura
            memset (buf, '\0', MAXDATASIZE); /* Pone a cero el buffer inicialmente */
}

void comRobot4(){

            //Se crea el socket
            if((sockfd=socket(AF_INET,SOCK_DGRAM,0))==-1){
                perror("socket");
            }
            
            const char *char_ip=robot4.ip.c_str();

            robot_addr.sin_family=AF_INET;//familia: ordenacion de bytes de la maquina
            robot_addr.sin_port=htons(robot4.port);//puerto:ordenacion de bytes de la red
            robot_addr.sin_addr.s_addr=inet_addr(char_ip); //IP: ordenacion de bytes de la red
            memset(&(robot_addr.sin_zero),'\0',8); //pone a cero el resto de la estructura
            memset (buf, '\0', MAXDATASIZE); /* Pone a cero el buffer inicialmente */
}
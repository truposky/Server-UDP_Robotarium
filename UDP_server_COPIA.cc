
#include "common.h"
using namespace std;




enum {robot1, robot2, robot3, robot4,robot5};
  


//prototypes
int clasification (char* client_addr);


int main (){

    int sockfd;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    char buf[MAXDATASIZE];
    int numbytes;

    int error;
    int cont;
    socklen_t sin_size;

    //Se crea el socket
    if((sockfd=socket(AF_INET,SOCK_DGRAM,0))==-1){
        perror("socket");
        exit(1);

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
    while(1){

        cout<<"(Servidor) esperando mensajes [puerto local]"<<ntohs(my_addr.sin_port)<<endl;

        //se recibe mensaje de cliente

        memset(buf,'\0',MAXDATASIZE);//se pone a cero el buffer
        sin_size=sizeof(struct sockaddr_in);
        if((numbytes=recvfrom(sockfd,buf,MAXDATASIZE,0,(struct sockaddr*)&their_addr, &sin_size))==-1){

            perror("recvfrom");
            continue;//se vuelve al principio del loop
        }
        cout<<"(servidor) mensaje recibido de IP:"<<inet_ntoa(their_addr.sin_addr)<<" puerto ["<<ntohs(their_addr.sin_port)<<"] longitud["<<numbytes<<"]"<<endl;
        cout<<"(cliente) "<<buf<<endl;




    }
}




int clasification (char* client_addr){
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


    

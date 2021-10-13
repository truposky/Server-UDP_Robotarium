/*
** listener.c -- a datagram sockets "server" demo
*/
#include "common.hh"
void SetupRobots();
using namespace tinyxml2;
using namespace std;



#define MYPORT "4950"   // the port users will be connecting to
#define MAXBUFLEN 255
// get sockaddr, IPv4 or IPv6:
 char buf[MAXBUFLEN];

//----prototipos-----//
int comRobot1();
void tokenize(string data, string del);

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
enum {r1, r2, r3, r4,r5};
     //definition of robots
Robot robot1,robot2,robot3,robot4;
int main(void)
{
    SetupRobots();
    bool condicion=true;
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes,cont=0;
    struct sockaddr_storage their_addr; // connector's address information
  
    socklen_t addr_len;
    char s[INET_ADDRSTRLEN];
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE; // use my IP
    if ((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) 
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }
              // set timeout
    struct timeval timeout;
         timeout.tv_sec = 10;//sec
         timeout.tv_usec = 0;//microsecond
    
    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next)
     {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }
        if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1) {
            perror("setsockopt failed:");
        }
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("listener: bind");
            continue;
        }
        break;
    }
    if (p == NULL) 
    {
        fprintf(stderr, "listener: failed to bind socket\n");
        return 2;
    }
  
    while(condicion)
    {
        printf("listener: waiting to recvfrom...\n");
        addr_len = sizeof their_addr;
        memset(buf,'\0',MAXBUFLEN);//se pone a cero el buffer
        if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,(struct sockaddr *)&their_addr, &addr_len)) == -1) 
        {
            perror("recvfrom");
             if(cont >= MAXROBOTS)
            {
               // close (sockfd);// se cierra el socket para la comunicacion inicial
                condicion=false;
            }
            else
            {

                char respuesta;
                cout<<"faltan "<<MAXROBOTS-cont<<" por estar operativos, desea esperar (y) (n)"<<endl;
                cin>>respuesta;
                if(respuesta=='y'){
                    condicion=true;
                }
                else{
                     condicion=false;
                }
            }
        }
        else
        {
         //si detecta la conexion o un mensaje se responde y se envia una respuesta en consecuencia
            cont++;//un robot mas conectado
             char hoststr[NI_MAXHOST];
            char portstr[NI_MAXSERV];

            int rc = getnameinfo((struct sockaddr *)&their_addr,  addr_len, hoststr, sizeof(hoststr), portstr, sizeof(portstr), NI_NUMERICHOST | NI_NUMERICSERV);
            cout<<"(servidor) mensaje recibido de IP:"<<hoststr
            <<" puerto ["<<portstr<<"] longitud["<<numbytes<<"]"<<endl;
            buf[numbytes] = '\0';
            cout<<"(cliente) "<<buf<<endl;

            operation_recv=( struct appdata*)&buf;
            if((numbytes< HEADER_LEN) || (numbytes != operation_recv->len+HEADER_LEN) )
            {
                cont--;
                if(cont<=0)cont=0;
                cout<<"(servidor) unidad de datos incompleta\n";
                continue;

            }
            else
            {

                cout<<"(servidor) id "<<operation_recv->id;
                cout<<" operacion solicitada [op 0x]"<<operation_recv->op;
                cout<<" contenido "<<operation_recv->data<<endl;


            }
            
   
        }
    }
    freeaddrinfo(servinfo);
    close(sockfd);
    //ahora se empieza a comunicar el ordenador con los diferentes robots.
    
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
                  //  comRobot2();
                    break;

                
                case r3 :
                 //   comRobot3();
                    break;

                
                case r4 :
                  //  comRobot4();
                    break;
                    
                default:
                    break;
                
            }
           



        }
    return 0;
}

void SetupRobots()
{
    // Read the sample.xml file
    XMLDocument Robotdoc;
    Robotdoc.LoadFile( "robots_info.xml" );

    XMLNode* Robotarium =Robotdoc.FirstChild();
    XMLElement *robot=Robotarium->FirstChildElement("robot");
    int i=0;
    while(robot !=NULL)
    {
        
        XMLElement *robotChild=robot->FirstChildElement("ID");
        int ID;
        robotChild->QueryIntText(&ID);
        cout<<"ID:"<<ID<<endl;
    
         robotChild=robot->FirstChildElement("IP");
         const char* ip=robotChild->GetText();
         string ss=ip;
         cout<<"ip:"<<ip<<endl;

        robotChild=robot->FirstChildElement("PORT");
        const char* port=robotChild->GetText();
        string p=port;
        cout<<"puerto:"<<p<<endl;
        
        robot=robot->NextSiblingElement("robot"); 
        switch (i)
        {
            case 0:
                robot1.SetupRobotData(ID,ss,p);
                break;
            case 1:
                robot2.SetupRobotData(ID,ss,p);
                break;
            case 2:
                robot3.SetupRobotData(ID,ss,p);
                break;
             case 3:
                robot4.SetupRobotData(ID,ss,p);
                break;

        }       
        i++;   
    }
}

int comRobot1(){
    int id;
    string ip,port;
    robot1.SetupConection(id,ip,port);
    //se crea el socket y se establece la comunicación
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    struct sockaddr_storage robot_addr;
    
    socklen_t addr_len = sizeof robot_addr;


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    const char *ipRobot=ip.c_str();
    const char *portRobot=port.c_str();
    
    if ((rv = getaddrinfo(ipRobot, portRobot, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
    }
    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype,
    p->ai_protocol)) == -1) {
    perror("talker: socket");
    continue;
    }
    break;
    }
    if (p == NULL) {
        fprintf(stderr, "talker: failed to create socket\n");
        return 2;
    }
    memset (buf, '\0', MAXBUFLEN); /* Pone a cero el buffer inicialmente */
    //aqui se indica la operacion que se desea realizar
    operation_send.id=id;//se asigna el id del robot1
    string data;
    string delimiter=":";
    cout<<"ingrese operacion"<<endl;//se debe ingresar la operacion y los correspondientes datos
    //por ahora todo junto.
    cin.ignore();
    getline(cin,data);
    tokenize( data,":");
    

    cout<<"mensaje enviado :id "<<operation_send.id<<" op: "<<dec<<operation_send.op<<endl;
    cout<<"datos "<<operation_send.data<<endl;
    if ((numbytes = sendto(sockfd, &operation_send, operation_send.len+HEADER_LEN, 0,p->ai_addr, p->ai_addrlen)) == -1) {
        perror("talker: sendto");
        exit(1);
    }

    cout<<"mensaje enviado"<<endl;

    if((numbytes=recvfrom(sockfd,buf,MAXBUFLEN-1,0,(struct sockaddr*)&robot_addr, &addr_len))==-1){
    }
    operation_recv=( struct appdata*)&buf;
    if((numbytes< HEADER_LEN) || (numbytes != operation_recv->len+HEADER_LEN) ){

        cout<<"(servidor) unidad de datos incompleta\n";
    }
    else{
        
        cout<<"(servidor) id "<<operation_recv->id;
        cout<<" operacion solicitada [op 0x]"<<operation_recv->op;
        cout<<" contenido "<<operation_recv->data<<endl;
    }
    /* relaiza operacion solicitada por el cliente */
    
    operation_send.id=operation_recv->id;//id

    switch (operation_recv->op){
        case OP_SALUDO:
            operation_send.op=OP_SALUDO;
            strcpy(operation_send.data, "Saludo recibido");
            operation_send.len =strlen(operation_send.data);
        break;
        case OP_WAIT_INSTRUCTION:
            cout<<"esperando instruccion, escriba operacion y datos si es necesario"<<endl;
            
            cin>>operation_send.op>>operation_send.data;
        break;
        case OP_MESSAGE:
            cout<<"(robot1)"<<buf<<endl;
        default: /*operacion desconocida*/
            operation_send.op=OP_ERROR;
            strcpy(operation_send.data, "operacion desconocida");
            operation_send.len =strlen(operation_send.data);
            
            break;
    }
    freeaddrinfo(servinfo);
    
    close(sockfd);
    return 0;
    


}

void tokenize(string data, string del)//sirve para separa la entrada string.
{
    int cont=0;
    int start = 0;
    int end = data.find(del);
    string word;
    while (end != -1) {
        
        
         if(cont==0)
        {   
            word=data.substr(start, end - start);
            operation_send.op=stoul(word,nullptr,0);
            start = end + del.size();
            end = data.find(del, start);
            
        }
    
       
        cont++;
    }
     word=data.substr(start, end - start);
    strcpy(operation_send.data,word.c_str());
  
   
   
}

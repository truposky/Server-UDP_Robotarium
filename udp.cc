/*
** listener.c -- a datagram sockets "server" demo
*/
#include "common.hh"
void SetupRobots();
using namespace tinyxml2;
using namespace std;


#define HELLOPORT "4241"
#define MYPORT "4442"   // the port users will be connecting to
#define MAXBUFLEN 255
// get sockaddr, IPv4 or IPv6:
 char buf[MAXDATASIZE];

//----prototipos-----//
int comRobot(int id,string ip,string port);
void tokenize(const string s, char c,vector<string>& v);
void concatenateChar(char c, char *word);
void operationSend();
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
    string message;//se usara para separa el mensaje en las correspondientes partes
    int cont=0;
    //se declaran las variables para definir los parametros de comunicacion del robot
    int id;
    string ip,port;

    while(1){//main loop

        /* //primero se elige al robot al que se desea mandar una determinada instruccion
        cout<<"(0)robot1"<<endl<<"(1)robot2"<<endl<<"(2)robot3"<<endl<<"(3)robot4"<<endl;
        cout<<"eliga el robot con el cual desea comunicarse"<<endl;
        cin>>cont;

        switch (cont)
        {
        case r1 :
            
            robot1.SetupConection(id,ip,port);
            break;
        
        case r2 :
            robot2.SetupConection(id,ip,port);
            break;

        
        case r3 :
            robot3.SetupConection(id,ip,port);
            break;

        
        case r4 :
            robot4.SetupConection(id,ip,port);
            break;
            
        default:
            break;
        
        }*/
         robot1.SetupConection(id,ip,port);
        comRobot(id,ip,port);
        
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

int comRobot(int id,string ip,string port){
    
    //se crea el socket y se establece la comunicación
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    struct sockaddr_storage robot_addr;
    cout<<"puert Robot:"<<port<<endl;
    socklen_t addr_len = sizeof robot_addr;


    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // set to AF_INET to force IPv4
    hints.ai_socktype = SOCK_DGRAM;
   //hints.ai_flags = IPPROTO_UDP; 

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
    memset (buf, '\0', MAXDATASIZE); /* Pone a cero el buffer inicialmente */
    //aqui se indica la operacion que se desea realizar
    operation_send.id=id;//se asigna el id del robot1
    string data;
    string delimiter=":";
    cout<<"elige operacion"<<endl;//se debe ingresar la operacion y los correspondientes datos
    operationSend();//se elige la operacion a enviar
    if(operation_send.op != OP_SALUDO && operation_send.op != OP_VEL_ROBOT){
        //se ingresa la informacion correspondiente a la operacion elegida.
        double velRuedaDerecha;
        double velRuedaIzquierda;
        cout<<"ingresa datos: ";
        cin.ignore();
        cin>>velRuedaDerecha;
        cin.ignore();
        cin>>velRuedaIzquierda;
        cout<<velRuedaDerecha<<","<<velRuedaIzquierda<<endl;
        doubleToBytes(velRuedaDerecha, &operation_send.data[0]);
        doubleToBytes(velRuedaIzquierda, &operation_send.data[8]);
        operation_send.len = sizeof (operation_send.data);
    }
    if ((numbytes = sendto(sockfd,(char *) &operation_send, operation_send.len+HEADER_LEN, 0,p->ai_addr, p->ai_addrlen)) == -1) {
        perror("talker: sendto");
        exit(1);
    }

    cout<<"mensaje enviado"<<endl;

   /* if((numbytes=recvfrom(sockfd,buf,MAXBUFLEN-1,0,(struct sockaddr*)&robot_addr, &addr_len))==-1){
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
    // relaiza operacion solicitada por el cliente 

    switch (operation_recv->op){
        case OP_SALUDO:
            cout<<" contenido "<<operation_recv->data<<endl;
        break;
        case OP_MESSAGE_RECIVE:
            cout<<" contenido "<<operation_recv->data<<endl;
        break;
        case OP_VEL_ROBOT:
            vector<double> speed;
            speed[0] = bytesToDouble(&operation_recv->data[0]);
            speed[1] = bytesToDouble(&operation_recv->data[8]);
            cout<<"velocidad rueda derecha: "<<speed[0]<<endl;
            cout<<"velocidad rueda izquierda: "<<speed[1]<<endl;
            break;

    }*/
    
    freeaddrinfo(servinfo);
    
    close(sockfd);
    
    return 0;
    


}
void tokenize(const string s, char c,vector<string>& v)//sirve para separa la entrada string.
{
   string::size_type i = 0;
   string::size_type j = s.find(c);

   while (j != string::npos) 
   {
      v.push_back(s.substr(i, j-i));
      i = ++j;
      j = s.find(c, j);

      if (j == string::npos)
         v.push_back(s.substr(i, s.length()));
   }
}

void concatenateChar(char c, char *word){
    char cadenaTemporal[2];
    cadenaTemporal[0] = c;
    cadenaTemporal[1] = '\0';
    strcat(word, cadenaTemporal);

}
void operationSend(){
    int value;
    cout<<"(0) SALUDO"<<endl;
    cout<<"(1) MOVE_WHEEL"<<endl;
    cout<<"(2) STOP_WHEEL"<<endl;
    cout<<"(3) MESSAGE"<<endl;
    cout<<"(4) TELEMETRY"<<endl;
    cout<<"(5) VEL_wheel"<<endl;
    cin>>value;
    switch (value)
    {

        case 0:
            operation_send.op=OP_SALUDO;
            break;
        case 1:
            operation_send.op=OP_MOVE_WHEEL;
            break;
        case 2:
            operation_send.op=OP_STOP_WHEEL;
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            operation_send.op=OP_VEL_ROBOT;
            
            break;
    }

    
}

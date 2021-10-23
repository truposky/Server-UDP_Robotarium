#include <iostream>
#include <fstream>
#include <vector>
#include "tinyxml2.cpp"
using namespace std;
using namespace tinyxml2;

int main(void){
     // Read the sample.xml file
    XMLDocument Robotdoc;
    Robotdoc.LoadFile( "robots_info.xml" );

    XMLNode* Robotarium =Robotdoc.FirstChild();
    XMLElement *robot=Robotarium->FirstChildElement("robot");

    while(robot !=NULL)
    {
        XMLElement *robotChild=robot->FirstChildElement("ID");
        int ID;
        robotChild->QueryIntText(&ID);
        cout<<ID<<endl;
    
         robotChild=robot->FirstChildElement("IP");
         const char* ip=robotChild->GetText();
         string ss=ip;
         cout<<"ip:"<<ip<<endl;

        robotChild=robot->FirstChildElement("UDP");
        int port;
        robotChild->QueryIntText(&port);
        cout<<"puerto:"<<port<<endl;
       
        robot=robot->NextSiblingElement("robot");    
    }

    return 0;
}
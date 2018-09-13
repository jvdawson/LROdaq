#include "messagelistener.h"
#include <iostream>


#define PORT 62000
//62000 des messages AMC
//messagelistener -- listens to all AMC cards (clashes?)...
messagelistener::messagelistener(char *my_addr):client(PORT,my_addr)
{
  std::cout<<"messagelistener"<<std::endl;
  fout.open("log/AMC.log");
}
messagelistener::~messagelistener()
{
  fout.close();
}

////////////////////////////////////////////////////////////////////
void messagelistener::read()
{
  unsigned char buffer[548]={'0'};
  //need new read - where does message come from!!
  int valread = cread(buffer,548); //remove addr and port from client.h..
  if(valread>0)
    {
      std::cout<<buffer<<std::endl; //WANT TO SEND THIS SOMEWHERE ELSE
      //compile new message
      fout<<buffer<<std::endl;
      //PORT NUMBER?
    //SYSLOG AND/OR PHYSICAL LOG AND/OR WEB?
      //      for(int i=0;i<1472;i++)std::cout<<buffer[i]<<" ";
      // std::cout<<std::endl;
      //logger command - syslog
    }else{
    //raise exception..
    std::cout<<"disconnect?"<<std::endl;
  }

}

#include "messagelistener.h"
#include <iostream>

#define PORT 62000
//62000 des messages AMC
//messagelistener -- listens to all AMC cards...
messagelistener::messagelistener(char *my_addr):client(PORT,my_addr)
{
  std::cout<<"messagelistener"<<std::endl;
}
messagelistener::~messagelistener()
{

}

////////////////////////////////////////////////////////////////////
void messagelistener::read()
{
  unsigned char buffer[548]={'0'};
  int valread = cread(buffer,548); //remove addr and port from client.h..
  if(valread>0)
    {
      std::cout<<buffer<<std::endl; //WANT TO SEND THIS SOMEWHERE ELSE
      //SYSLOG AND/OR PHYSICAL LOG AND/OR WEB?
      //      for(int i=0;i<1472;i++)std::cout<<buffer[i]<<" ";
      // std::cout<<std::endl;
    }else{
    //raise exception..
    std::cout<<"disconnect?"<<std::endl;
  }

}

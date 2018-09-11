#include "messagelistener.h"
#include <iostream>

#define PORT 62000
//62000 des messages AMC
//messagelistener -- listens to all AMC cards...
messagelistener::messagelistener(char *my_addr):client(PORT,my_addr)
{

}
messagelistener::~messagelistener()
{

}

////////////////////////////////////////////////////////////////////
void messagelistener::read()
{
  unsigned char buffer[1024];
  int valread = cread(buffer); //remove addr and port from client.h..
  if(valread>0)
    {
      std::cout<<buffer<<std::endl; //WANT TO SEND THIS SOMEWHERE ELSE
      //SYSLOG AND/OR PHYSICAL LOG AND/OR WEB?

    }else{
    //raise exception..
    std::cout<<"disconnect?"<<std::endl;
  }

}

#include "client.h"
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>

bool debug=true;
#define daqcomputer "172.16.4.1"
#define daqport 50325
#define daqDport 65000

client::~client()
{
  PORT=0;
  sock=0;


}

 
client::client(int p, char addr[])
{
  PORT =p;
  sock=0;
  init(addr);
}
void client::init(char addr[])
{
  struct sockaddr_in serv_addr;
  //  struct sockaddr_in address;

  memset(&serv_addr, '0', sizeof(serv_addr));
  bool res=false;
  if(debug)std::cout<<"create socket"<<std::endl;
  res = create_socket();
  if(debug)std::cout<<"connect socket"<<std::endl;
  res = connect_socket(serv_addr, addr);
  if(debug)std::cout<<"init done"<<std::endl;
  
}
bool client::create_socket()
{
  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
      printf("\n Socket creation error \n");
      return false;
    }
  //RAISE ERROR

  return true;
}

bool client::connect_socket(struct sockaddr_in serv_addr,char addr[])
{
  serv_addr.sin_family = AF_INET;
  if(PORT>10000){ serv_addr.sin_port = htons(daqDport);}else{
  serv_addr.sin_port = htons(daqport);//PORT
  }
  serv_addr.sin_addr.s_addr = inet_addr(daqcomputer);//THIS IS THIS IP..
 

  if (bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {    
    std::cout<<"Connection error"<<std::endl;
       return false;
    }
  if(debug){std::cout<<"Bound"<<std::endl;}

  //assign new value to connect to
  serv_addr.sin_addr.s_addr = inet_addr(addr);
  serv_addr.sin_port = htons(PORT);

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
      printf("\nConnection Failed \n");
      return false;
    }
  return true;
}

bool client::client_send(unsigned char buffer[], int blen)
{
  if(debug)std::cout<<"client_send "<<std::endl;
  if(sizeof(blen>0)){
    //write out what is being sent for debugging
    if(debug)
      {std::cout<<"Sending:";
	for(int i=0;i<blen;i++)
	  {
	    std::cout<<std::hex<<unsigned(buffer[i])<<" ";
	  }
	std::cout<<std::endl;
      }

    send(sock, buffer,blen,0);
  return true;
    }
    return false;
}
bool client::client_read(unsigned char buffer[]) //how much to read?
{
  std::cout<<sizeof(buffer)<<std::endl;
  int valread = read(sock, buffer, sizeof(buffer));
  if(debug){
    std::cout<<"valread "<<valread<<std::endl;
    for(int i=0;i<valread;i++)
      { //this is passed as a char, but will be decoded as unsigned char 
	std::cout<<std::hex<<unsigned(buffer[i])<<" ";
      }
    std::cout<<std::endl;
  }
  return true;
}

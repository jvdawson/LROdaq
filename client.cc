#include "client.h"
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <sys/time.h>

//NEED TO ADD ERROR HANDLING...

bool debug=false;//true;
//#define daqcomputer "172.16.4.1"
//#define daqport 50325
//#define daqDport 65000

client::~client()
{

  sock=0;


}



client::client(int my_p, char *my_addr)
{
  sock=0;
  std::cout<<"client: "<<my_p<<" "<<my_addr<<std::endl;
  init(my_p,my_addr);
}
bool client::init(int PORT, char *addr)
{

  struct sockaddr_in serv_addr;

  memset(&serv_addr, '0', sizeof(serv_addr));

  if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
      printf("\n Socket creation error \n");
      return false;
    }
  //RAISE ERROR

  // PORT is port on my machine to send/receive data
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  serv_addr.sin_addr.s_addr = inet_addr(addr);//THIS IS MY IP..
 

  if (bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {    
    std::cout<<"Connection error"<<std::endl;
       return false;
    }
  if(debug){std::cout<<"Bound"<<std::endl;}

  /*struct timeval timeout={2,0}; //set timeout for 2 seconds
  setsockopt(sock,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeout,sizeof(struct timeval));
  */ //timeout on socket


}

bool client::csend(unsigned char buffer[], int blen, char *addr, int p)
{
  struct sockaddr_in dest_addr;
  dest_addr.sin_family = AF_INET;                                           
  dest_addr.sin_port = htons(p);                                            
  dest_addr.sin_addr.s_addr = inet_addr(addr);//THIS IS THIS IP..               
  
  
  if(debug)std::cout<<"client_send "<<std::endl;
  if(sizeof(blen>0)){
    //write out what is being sent for debugging
    if(debug)
      {std::cout<<"Sending:";
	for(int i=0;i<blen;i=i+2)
	  {
	    std::cout<<std::hex<<unsigned(buffer[i])<<unsigned(buffer[i+1])<<" ";
	  }
	std::cout<<std::endl;
      }

    ssize_t thissize = sendto(sock,buffer,blen, 0,(struct sockaddr *) &dest_addr, sizeof(dest_addr));
    // ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
    //                  const struct sockaddr *dest_addr, socklen_t addrlen);
    //    send(sock, buffer,blen,0);
  return true;
    }
    return false;
}
int client::cread(unsigned char buffer[], int buflen) 
{//buflen: 1472
  //strlen?

  //  int length = recvfrom( sock, buffer, sizeof(buffer), 0, NULL, NULL);
  int length = recv(sock,buffer,buflen,0);
  
  
  std::cout<<std::dec<<length<<":"<< sizeof(buffer)<<":"<<buffer<<std::endl;

  if(debug){
    std::cout<<"length "<<length<<std::endl;
    for(int i=0;i<length;i++)
      { //this is passed as a char, but will be decoded as unsigned char 
	std::cout<<std::hex<<unsigned(buffer[i])<<" ";
      }
    std::cout<<std::endl;
  }
  
  return length;
}

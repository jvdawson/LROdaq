#include "server.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>

//how to connect to read data...
//this is TCP/IP or UDP?

bool debug=true;
//JUST TO TEST...
server::~server()
{
  PORT=0;
  sock=0;


}

 
server::server(int p)
{

  PORT =p;
  sock=0;
  init();
}
void server::init()
{
  struct sockaddr_in serv_addr;
  

  memset(&serv_addr, '0', sizeof(serv_addr));
  bool res=false;
  if(debug)std::cout<<"create socket"<<std::endl;
  res = create_socket();
  if(debug)std::cout<<"attach socket"<<std::endl;
  res=attach_socket();
  if(debug)std::cout<<"connect socket"<<std::endl;
  res = connect_socket(serv_addr);
  if(debug)std::cout<<"init done"<<std::endl;
  
}
bool server::create_socket()
{
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {//SOCK_DGRAM? -- SOCK_STREAM [previous]
      //SOCK_DGRAM SOCK_SEQPACKET-- doesn't work
      printf("\n Socket creation error \n");
      return false;
    }
  //RAISE ERROR

  return true;
}
bool server::attach_socket()
{
  int opt=1; //??
  //SO_REUSEPORT?
  // Forcefully attaching socket to the port 8080
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR ,//| SO_REUSEPORT,
		 &opt, sizeof(opt)))
    {
      perror("setsockopt");
      return false;
    }
  return true;
}
bool server::connect_socket(struct sockaddr_in serv_addr)
{
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  int addrlen = sizeof(serv_addr);
 

  if (bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {    
    std::cout<<"Connection error"<<std::endl;
       return false;
    }
  if(debug){std::cout<<"Bound"<<std::endl;}

  if(listen(sock,3)<0){ //??
    std::cout<<"Listen error"<<std::endl;
    return false;
  }

  if ((new_socket = accept(sock, (struct sockaddr *)&serv_addr, 
			   (socklen_t*)&addrlen))<0)
    {
      std::cout<<"Accept error"<<std::endl;
      return false;
    }


  return true;
}

bool server::server_send(unsigned char buffer[], int blen)
{
  if(debug)std::cout<<"server_send "<<std::endl;
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
bool server::server_read(unsigned char buffer[]) //how much to read?
{ //need to return the number of bytes read... (more useful)
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

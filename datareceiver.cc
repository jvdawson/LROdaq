#include <unistd.h>
#include <stdio.h>
//don't need all of these...
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>
//for select?
#include <sys/time.h>
#include <sys/types.h>
#include "datareceiver.h"


//data sent to udpport (my_p)
datareceiver::datareceiver()
{ //pass in pipe fd
  
};
datareceiver::~datareceiver()
{
  //  if(mythread){delete mythread;}

}
////////////////////
void datareceiver::AddReceiver(int port, char *addr)
{
  //make the thread and add to list?
  //call function datareceiver(port,addr)
  //NEED to add a queue or a pipe(?, or directions pipe to writer end?)

  //just testing for now
  client testclient(port,addr);
  unsigned char buffer[64000];
  int valread = cread(buffer); //remove addr and port from client.h..
  if(valread>0)
    {
      std::cout<<buffer<<std::endl; //WANT TO SEND THIS SOMEWHERE ELSE
    }
}
void datareceiver::start()
{

  //  mythread=new thread(myreceiver);

}
void datareceiver::myreceiver(int port, char *addr)
{ //and pipe to write out to?
  client thisclient(port,addr); //datacommunication
  //need socket (private -- need function)

  fd_set rfds,wfds; //read, error, write
  struct timeval tv; //maybe want to alter timeout during running?
  int retval;

  FD_ZERO(&rfds);
  FD_SET(thisclient.GetSock(), &rfds); //serverfd new_socket
  int nmax = sock;
  tv.tv_sec = 2;
  tv.tv_usec = 0;
  bool status=false;
  //NEED also pipe to mother...to stop thread
  unsigned char buffer[4096];//WHAT IS MAXIMUM SIZE?

  //BUFFER NEEDS TO BE HERE..
  //is pipe ready to writing (should check)
  //check queue too.. and exceptions
  while(-1)
    {
      FD_ZERO(&rfds);
      FD_SET(thisclient.GetSock(), &rfds); 
     
      nmax = thisclient.GetSock()+1;
     

        //TIME OUT 
      tv.tv_sec = TIMEOUT_s;
      tv.tv_usec = TIMEOUT_us;
 
      retval = select(nmax+1, &rfds, NULL, NULL, &tv);
      if (FD_ISSET(thisclient.GetSock(), &rfds)) {
            std::cout<<"data "<<std::endl;
            // handle data on this connection
            for(int i=0;i<sizeof(buffer);i++){buffer[i]=0;}
            status =  thisclient.cread(buffer);
	    //valread = read(*it, buffer, 1024);
            if(status){
	      std::cout<<sizeof(buffer)<<std::endl;
	      //decode data -- do something with it..write? QUEUE PIPE TO WRITER?
	    }
         
      }


    }
}

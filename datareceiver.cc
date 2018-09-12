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
  std::cout<<"datareceiver"<<std::endl;
  if (pipe(pipefd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

};
datareceiver::~datareceiver()
{
  //  if(mythread){delete mythread;}
  for (std::vector<int>::iterator it = receivers.begin(); it != receivers.end(); ++it) {
    pthread_join(*it, NULL);
  }
  //remove threads from vector?
}
////////////////////
struct threadinfo {
  card mycard;
  int commpipefd[2];
  int datapipefd[2];
};

static void *myreceiver(void *arg)//doesn't have to be a member function of class datareceiver...
{ //with void * could pass in other objects, like pipe to mother code?
  threadinfo *tinfo = (threadinfo)*arg;
  //tinfo. like in man pthread_create

  // card* mycard = (card*)arg;
  //now do something with my card...
  //communication pipe
  close(commpipefd[1]); //close writing to comm pipe
  close(datapipefd[0]); //close reading to data pipe

  // --
  fd_set rfds,wfds; //read, error, write
  struct timeval tv; //maybe want to alter timeout during running?
  int retval;

  FD_ZERO(&rfds);
  FD_SET(mycard->GetDataSocket(), &rfds); //serverfd new_socket
  int nmax = mycard->GetDataSocket();
  tv.tv_sec = 2;
  tv.tv_usec = 0;

  //REQUEST DATA?
  mycard->DataReadRequest();
 while(-1)
    {
      FD_ZERO(&rfds);
      FD_SET(mycard->GetDataSocket(), &rfds); 
     
      nmax = mycard->GetDataSocket()+1;
     

        //TIME OUT 
      tv.tv_sec = TIMEOUT_s;
      tv.tv_usec = TIMEOUT_us;
 
      retval = select(nmax+1, &rfds, NULL, NULL, &tv);
      if (FD_ISSET(mycard.GetDataSocket(), &rfds)) {
            std::cout<<"data "<<std::endl;
            // handle data on this connection
	    mycard->ReadData(); //where does data go? - pipe it to write thread?
         
      }


    }
 ///////////////////////////////

}

void datareceiver::AddReceiver(card mycard)
{
  //pthread or thread?
  pthread_t mynewthread;
  pthread_create(&mynewthread, NULL, myreceiver, &mycard); //ok?
  receivers.push_back(mynewthread); //???
  //  receivers.push_back(pthread_create( &thread1, NULL, print_message_function, (void*) message1);mycard); //ok?

}
/*void datareceiver::AddReceiver(int port, char *addr)
{//need to remove receiver from card function[dcomm]

  /*  //make the thread and add to list?
  //call function datareceiver(port,addr)
  //NEED to add a queue or a pipe(?, or directions pipe to writer end?)

  //just testing for now
  client testclient(port,addr);
  unsigned char buffer[64000];
  int valread = testclient.cread(buffer,64000); //remove addr and port from client.h..
  if(valread>0)
    {
      std::cout<<buffer<<std::endl; //WANT TO SEND THIS SOMEWHERE ELSE
    }
  

}
*/

/*void datareceiver::myreceiver(int port, char *addr)
{ //and pipe to write out to?
  client thisclient(port,addr); //datacommunication
  //need socket (private -- need function)

  fd_set rfds,wfds; //read, error, write
  struct timeval tv; //maybe want to alter timeout during running?
  int retval;

  FD_ZERO(&rfds);
  FD_SET(thisclient.GetSock(), &rfds); //serverfd new_socket
  int nmax = thisclient.GetSock();
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
            status =  thisclient.cread(buffer,4096);
	    //valread = read(*it, buffer, 1024);
            if(status){
	      std::cout<<sizeof(buffer)<<std::endl;
	      //decode data -- do something with it..write? QUEUE PIPE TO WRITER?
	    }
         
      }


    }
}
*/

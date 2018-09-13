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


};
datareceiver::~datareceiver()
{
  //  if(mythread){delete mythread;}
  /* for (std::vector<struct threadinfo>::iterator it = receivers.begin(); it != receivers.end(); ++it) {
    pthread_join((*it).thread_id, NULL);
    }*/ //should empty vector...
  //remove threads from vector?
}
////////////////////
void datareceiver::Stop()
{
  int retval;
  char buf[]="EXIT";
  for (std::vector<struct threadinfo>::iterator it = receivers.begin(); it != receivers.end(); ++it) {
    retval = write( (*it).commpipefd[1], buf,strlen(buf));
  }

  for (std::vector<struct threadinfo>::iterator it = receivers.begin(); it != receivers.end(); ++it) {
    pthread_join((*it).thread_id, NULL);
  }
}

static void *myreceiver(void *arg)
{ 
  threadinfo *tinfo = (threadinfo*)arg;
 
  close(tinfo->commpipefd[1]); //close writing to comm pipe
  close(tinfo->datapipefd[0]); //close reading to data pipe

  // --
  fd_set rfds,wfds; //read, error, write
  struct timeval tv; //maybe want to alter timeout during running?
  int retval,nmax,cblen;
  char commbuffer[1024];
  
  //timeouts
   //REQUEST DATA? -- could be at request of comm pipe?
  tinfo->mycard->Data_ReadRequest();
  while(-1)
    {
      FD_ZERO(&rfds);
      FD_ZERO(&wfds);
      FD_SET(tinfo->mycard->GetDataSocket(), &rfds); 
      FD_SET(tinfo->commpipefd[0], &rfds);//read from pipe
      FD_SET(tinfo->datapipefd[1], &wfds);//write to pipe
      nmax = tinfo->mycard->GetDataSocket();
      if(tinfo->commpipefd[0]>nmax){nmax = tinfo->commpipefd[0];}
      if(tinfo->datapipefd[0]>nmax){nmax = tinfo->datapipefd[0];}
      tv.tv_sec = 2;//TIMEOUTs (change whilst running?)
      tv.tv_usec = 0;

      //if select on write, it will always be OK, no timeout...?  
      retval = select(nmax+1, &rfds, NULL, NULL, &tv);
      if (retval == -1){
        perror("select()");        
      }  else if (retval){
	//DATA ON CARD...
	if (FD_ISSET(tinfo->mycard->GetDataSocket(), &rfds)) {
            std::cout<<"data "<<std::endl;
            // handle data on this connection
	    tinfo->mycard->ReadData(); //where does data go? - pipe it to write thread   
	}
	//COMM PIPE...
	if (FD_ISSET(tinfo->commpipefd[0], &rfds)) {
	  std::cout<<"comm pipe "<<std::endl;
	  cblen = read(tinfo->commpipefd[0], &commbuffer, 1024); //max 1024
	  std::cout<<"thread: "<<tinfo->thread_id<<" length: "<<cblen<<" data: "<<commbuffer<<std::endl;//depending on the command
	  if(strcmp(commbuffer,"EXIT")==0)
	    {
	      std::cout<<"leave while loop"<<std::endl;
	      break;
	    }

	}


      }


    }
 ///////////////////////////////
  std::cout<<"End..."<<tinfo->thread_id<<std::endl;
}

void datareceiver::AddReceiver(card mycard)
{
  //pthread or thread?
  struct threadinfo tinfo;
  //pipes to go!!
  if (pipe(tinfo.commpipefd) == -1) {
    perror("comm pipe");
    exit(EXIT_FAILURE);
  }
 if (pipe(tinfo.datapipefd) == -1) {
    perror("comm pipe");
    exit(EXIT_FAILURE);
  }

  tinfo.thread_id = receivers.size(); 
  pthread_create(&tinfo.thread_id, NULL, myreceiver, &tinfo); //ok?
  receivers.push_back(tinfo); //???
  // close(tinfo->commpipefd[0]); //unused reading end
  //  close(tinfo->datapipefd[]); //unused writing end


  //close ends of pipe?
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

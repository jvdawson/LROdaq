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
#include <fstream>
#include "datareceiver.h"


//data sent to udpport (my_p)
datareceiver::datareceiver()
{ //pass in pipe fd
  std::cout<<"datareceiver constructed"<<std::endl;
  writerinfo.thread_id=0; 

};
datareceiver::~datareceiver()
{

}
////////////////////
long datareceiver::GetNumberofEvents()
{
  //ask writer function how many events it has received and written?
 char buf[]="NUMB\0"; //NEED TERMINATOR!!!!!
 char snum[8]={0};
 
 int retval;
 if(writerinfo.thread_id!=0)
    {
      std::cout<<"ask number of events from writer "<<std::endl;
      retval = write( writerinfo.commpipefd[1], buf, strlen(buf));
      retval = read(writerinfo.trigpipefd[0],snum,strlen(snum));
      std::cout<<"received from writer:"<<snum<<" "<<atol(snum)<<" "<<retval<<std::endl;
    }
 return atol(snum);
}
void datareceiver::Stop()
{
  std::cout<<"datareceiver::Stop"<<std::endl;
  int retval;
  char buf[]="EXIT\0"; //NEED TERMINATOR!!!!!
  for (std::vector<struct threadinfo>::iterator it = receivers.begin(); it != receivers.end(); ++it) {
    retval = write( (*it).commpipefd[1], buf,strlen(buf));
  }

  for (std::vector<struct threadinfo>::iterator it = receivers.begin(); it != receivers.end(); ++it) {
    pthread_join((*it).thread_id, NULL);
  }
  std::cout<<"writeinfothread_id"<<writerinfo.thread_id<<std::endl;
  //stop all data readers then stop data writer...
  if(writerinfo.thread_id!=0)
    {
      std::cout<<"send exit to writer "<<std::endl;
      retval = write( writerinfo.commpipefd[1], buf, strlen(buf));
      pthread_join( writerinfo.thread_id, NULL); //do I need that?
    }
}

static void *myreceiver(void *arg)
{
  std::cout<<"a new thread..."<<arg<<std::endl;
  struct threadinfo *tinfo = (struct threadinfo*) arg;
  std::cout<<"socket "<<tinfo<<" "<<tinfo->mycard<<" "<<tinfo->mycard->GetDataSocket()<<std::endl;                
  // -- I should close ends of the pipes that I won't use
  // -- for some reason this is causing me problems...
  // 
  // --
  fd_set rfds,wfds; //read, error, write
  struct timeval tv; //maybe want to alter timeout during running?
  int retval,nmax,cblen;
  char commbuffer[4];
  
  //timeouts
   //REQUEST DATA? -- could be at request of comm pipe?
  tinfo->mycard->Data_ReadRequest();
  while(-1)
    {
      std::cout<<"loop"<<std::endl;
      FD_ZERO(&rfds);
      FD_ZERO(&wfds);
      std::cout<<"FD_SET"<<std::endl;
      std::cout<<tinfo->mycard->GetDataSocket()<<std::endl;
      FD_SET(tinfo->mycard->GetDataSocket(), &rfds);
      std::cout<<"pipes"<<std::endl;
      FD_SET(tinfo->commpipefd[0], &rfds);//read from pipe
      FD_SET(tinfo->datapipefd[1], &wfds);//write to pipe
      nmax = tinfo->mycard->GetDataSocket();
      if(tinfo->commpipefd[0]>nmax){nmax = tinfo->commpipefd[0];}
      if(tinfo->datapipefd[1]>nmax){nmax = tinfo->datapipefd[1];}
      std::cout<<nmax<<std::endl;
      
      tv.tv_sec = 2;//TIMEOUTs (change whilst running?)
      tv.tv_usec = 0;

      //if select on write, it will always be OK, no timeout...?
      std::cout<<"select.."<<std::endl;
      retval = select(nmax+1, &rfds, NULL, NULL, &tv);
      if (retval == -1){
        perror("select()");        
      }  else if (retval){
	std::cout<<"selector returns "<<retval<<std::endl;
	//DATA ON CARD...
	if (FD_ISSET(tinfo->mycard->GetDataSocket(), &rfds)) {
            std::cout<<"data from card..."<<std::endl;
            // handle data on this connection
	    tinfo->mycard->ReadData(); //where does data go? - pipe it to write thread
	    //check if event is complete?
	    if(tinfo->mycard->isEventComplete()){
	    //do I check if I can write or do I block?
	    //cblen = write(tinfo->datapipefd[1], tinfo->mycard->databuffer, tinfo->mycard->datalength);
	      std::cout<<"event is complete - write to pipe "<<std::endl;
	      tinfo->mycard->WriteToPipe(&tinfo->datapipefd[1]);
	    }
	}
	//COMM PIPE...
	if (FD_ISSET(tinfo->commpipefd[0], &rfds)) {
	  std::cout<<"comm pipe "<<std::endl;
	  cblen = read(tinfo->commpipefd[0], &commbuffer, 1024); //max 1024
	  std::cout<<"thread: "<<tinfo->thread_id<<" length: "<<cblen<<" data\
: "<<commbuffer<<std::endl;//depending on the command 
	  if(cblen>0){
	    std::cout<<"thread: "<<tinfo->thread_id<<" length: "<<cblen<<" data: "<<commbuffer<<std::endl;//depending on the command
	    if(strcmp(commbuffer,"EXIT")==0)
	    {
	      std::cout<<"leave while loop"<<std::endl;
	      break;
	    }
	  }else{
	    sleep(1);
	  }
	}


      }


    }
 ///////////////////////////////
  std::cout<<"End..."<<tinfo->thread_id<<std::endl;
}

void datareceiver::AddReceiver(card *mycard)
{
  std::cout<<"Add receiver "<<mycard->GetDataSocket()<<std::endl;
  
  struct threadinfo *tinfo=new threadinfo();
  //pipes to go!!
  if (pipe(tinfo->commpipefd) == -1) {
    perror("comm pipe");
    exit(EXIT_FAILURE);
  }
  if (pipe(tinfo->datapipefd) == -1) {
    perror("comm pipe");
    exit(EXIT_FAILURE);
  }
  writerinfo.datasockets.push_back(tinfo->datapipefd[0]);//just one end of pipe?
  writerinfo.bsize = mycard->maxlength; 
  tinfo->mycard= mycard;
  //here is still OK
  std::cout<<"socket "<<tinfo->mycard->GetDataSocket()<<std::endl;
  tinfo->thread_id = receivers.size();
  std::cout<<"create thread "<<tinfo<<" "<<tinfo->mycard<<" "<<&mycard<<std::endl;
  if(pthread_create(&tinfo->thread_id, NULL, myreceiver, tinfo)!=0)
    {
      perror("pthread create");
    }
  
  std::cout<<"push back"<<std::endl;
  receivers.push_back(*tinfo); //???
  // close(tinfo->commpipefd[0]); //unused reading end
  //  close(tinfo->datapipefd[]); //unused writing end

}
//////////////////////////////////////////////////////////

static void *mywriter(void *arg)
{//filename needs to be modified...
  std::cout<<"writer thread..."<<arg<<std::endl;


  std::ofstream bout("data.bin", std::ios::binary);

  struct writerthreadinfo *tinfo = (struct writerthreadinfo*) arg;
  
  // --
  fd_set rfds,wfds; //read, error, write
  struct timeval tv; //maybe want to alter timeout during running?
  int retval,nmax,cblen;
  
  char commbuffer[4]={0};
  char ocommbuffer[64]={0};

  char *databuffer=new char[tinfo->bsize];//char or unsignedchar?
  int nevents=0;
  int halfbuffer=0;
  
  while(-1)
    {
      std::cout<<"writer loop"<<std::endl;
      FD_ZERO(&rfds);
      FD_ZERO(&wfds);
      nmax=0;
      std::cout<<"FD_SET"<<std::endl;
      for (std::vector<int>::iterator it = tinfo->datasockets.begin(); it != tinfo->datasockets.end(); ++it) {
	FD_SET( (*it), &rfds);
	if( (*it)>nmax)nmax=(*it);
      }
      std::cout<<"pipe"<<std::endl;
      FD_SET(tinfo->commpipefd[0], &rfds);//read from pipe
      if(tinfo->commpipefd[0]>nmax){nmax = tinfo->commpipefd[0];}
      //      FD_SET(tinfo->trigpipefd[1], &wfds);//write to pipe                      
      // if(tinfo->commpipefd[0]>nmax){nmax = tinfo->commpipefd[0];}


      std::cout<<nmax<<std::endl;
      
      tv.tv_sec = 2;//TIMEOUTs (change whilst running?)
      tv.tv_usec = 0;

      //if select on write, it will always be OK, no timeout...?
      std::cout<<"select.."<<std::endl;
      retval = select(nmax+1, &rfds, NULL, NULL, &tv);
      if (retval == -1){
        perror("select()");        
      } else{
	std::cout<<"selector returns "<<retval<<std::endl;
	//DATA send down pipe?
	for (std::vector<int>::iterator it = tinfo->datasockets.begin(); it != tinfo->datasockets.end(); ++it) {
	  
	  if( FD_ISSET( (*it), & rfds))
	    {//want to read pipe and write to file...
	      halfbuffer=tinfo->bsize/2;
	      std::cout<<"try to read from pipe: "<<halfbuffer<<std::endl;
	      cblen = read((*it), databuffer, halfbuffer);
	      std::cout<<"read "<<cblen<<" from pipe, requested "<<tinfo->bsize<<std::endl;
	      cblen = read((*it), &databuffer[halfbuffer], halfbuffer);
	      std::cout<<"read "<<cblen<<" from pipe, requested "<<tinfo->bsize<<std::endl;
	      //write an identifier or header?
	      bout.write((char*)databuffer,tinfo->bsize);
	      nevents++; //could how many events have written
	    }
	}	
	
      }
	//COMM PIPE...
	if (FD_ISSET(tinfo->commpipefd[0], &rfds)) {
	  std::cout<<"comm pipe "<<std::endl;
	  cblen = read(tinfo->commpipefd[0], &commbuffer, 4); //max 1024
	  std::cout<<"writer thread: "<<" length: "<<cblen<<" data\
: "<<commbuffer<<std::endl;//depending on the command 
	  if(cblen>0){
	    if(strcmp(commbuffer,"EXIT")==0)
	      {
		std::cout<<"leave while loop"<<std::endl;
		break;
	      }
	     if(strcmp(commbuffer,"NUMB")==0)
              {
                std::cout<<"request number of events: "<<nevents<<std::endl;
		sprintf(ocommbuffer,"%d\0",nevents);
		std::cout<<ocommbuffer<<std::endl;
		write(tinfo->trigpipefd[1],ocommbuffer,strlen(ocommbuffer));
              }


	  }else{//A problem??
	    sleep(1);
	  }



      }


    }
  
  bout.close();
}
void datareceiver::CreateWriter()
{
  if (pipe(writerinfo.commpipefd) == -1) {
    perror("Writer comm pipe");
    exit(EXIT_FAILURE);
  }
  if (pipe(writerinfo.trigpipefd) == -1) {
    perror("Writer trig pipe");
    exit(EXIT_FAILURE);
  }

  //////
 std::cout<<"create writer thread "<<std::endl;
  if(pthread_create(&writerinfo.thread_id, NULL, mywriter, &writerinfo)!=0)
    {
      perror("pthread create");
    }
  


}

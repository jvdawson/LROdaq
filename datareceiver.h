#ifndef DATARECEIVER
#define DATARECEIVER

#include <pthread.h>
#include <vector>
#include "client.h"
//SIMPLER METHOD, USE POSIX THREADS
//LISTENER IS A FUNCTION
//HOLDS ARRAY of Posxthreads which call listener
//HOLDS ARRAY of client for reading socket
//GUESS SHOULD BE ABLE TO CREATE AND DESTROY THREADS easier (without pipe)
//WILL HOLD QUEUES (or pipes) for each one
//MASTER WRITER THREAD? [or all write to a pipe directly to master write thread?]

class datareceiver
{



 public:
  datareceiver();
  ~datareceiver();

  //could add directly to thread list here... with port and addr
  void AddReceiver(int port, char *addr);


  void start();

 private:
  double TIMEOUT_s, TIMEOUT_us;
  void receiver(int port, char *addr );//loop, select, read

  //vector<pthread_t> recevierlist;
  //VECTOR of clients and push? 
  //  vector<client> clientlist;
  
  //QUEUE or pipe for data Nqueues?


};
#endif

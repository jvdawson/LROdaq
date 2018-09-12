#ifndef DATARECEIVER
#define DATARECEIVER

#include <pthread.h>
#include <vector>
#include "client.h"
#include "card.h"
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
  //  void AddReceiver(int port, char *addr);
  void AddReceiver(card mycard);



 private:
  double TIMEOUT_s, TIMEOUT_us;
  // void myreceiver(int port, char *addr );//loop, select, read
  // void* myreceiver(void* mycard); //read request? and then select?

  //vector<pthread_t> recevierlist;
  //VECTOR of clients and push? 
  //  vector<client> clientlist;
  std::vector<pthread_t> receivers;
  //need list of pipes comm pipe and data pipes
  //QUEUE or pipe for data Nqueues?
  int pipefd[2]; //way to contact thread

};

#endif

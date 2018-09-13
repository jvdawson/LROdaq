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

struct threadinfo {
  pthread_t thread_id;
  int       thread_num; 
  card* mycard;
  int commpipefd[2];
  int datapipefd[2];
};

class datareceiver
{



 public:
  datareceiver();
  ~datareceiver();

  //could add directly to thread list here... with port and addr
  //  void AddReceiver(int port, char *addr);
  void AddReceiver(card mycard);
  void Stop();


 private:

  std::vector<struct threadinfo> receivers;


};

#endif

#ifndef MESSAGELISTENER
#define MESSAGELISTENER

#include "client.h"

class messagelistener: public client
{

 public:
 messagelistener(char *my_addr);
  ~messagelistener();

  int get_socket(); //TO USE WITH GLOBAL SELECTOR

  void read();
  //redirect service? syslogger? html?

 private:
  //thread?


};
#endif

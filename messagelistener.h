#ifndef MESSAGELISTENER
#define MESSAGELISTENER
#include <fstream>
#include "client.h"

class messagelistener: public client
{

 public:
 messagelistener(char *my_addr);
  ~messagelistener();

  int get_socket(){return sock;}

  void read();
  //redirect service? syslogger? html?

 private:
  std::ofstream fout;

  

};
#endif

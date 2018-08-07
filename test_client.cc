#include "client.h"
#include <iostream>
#include <string.h>
int main(void)
{
  client myclient(8080,"127.0.0.1");
  char buffer[2048];
  strcpy(buffer,"Hello message");
  bool res = myclient.client_send(buffer);//buffer is clipped to "Hello message"
 
  char bufferout[2048];//this buffer has to be larger or equal to incoming...
 
  res = myclient.client_read(bufferout);
  std::cout<<buffer<<std::endl;
  return 0;
}

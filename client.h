#ifndef CLIENT
#define CLIENT
#include <iostream>
class client
{

 public:
  client(int my_p, char *my_addr);//127.0.0.1??
  ~client();
  //to work with ? selector?
  

  bool csend(unsigned char buffer[], int blen, char *addr, int p);
  int cread(unsigned char buffer[], int blen);
  int GetSock(){
    std::cout<<"client: getsock: "<<sock<<std::endl;
    return sock;}

 protected:




  int sock;
  
 private:


  bool init(int p, char *addr);


    
};
#endif 

#ifndef CLIENT
#define CLIENT

class client
{

 public:
  client(int my_p, char *my_addr);//127.0.0.1??
  ~client();
  //to work with ? selector?
  

  bool csend(unsigned char buffer[], int blen, char *addr, int p);
  int cread(unsigned char buffer[]);

 protected:




  int sock;
  
 private:
  
  bool init(int p, char *addr);


    
};
#endif 


class client
{

 public:
  client(int my_p, char *my_addr);//127.0.0.1??
  ~client();
  //to work with ? selector?
  

  bool csend(unsigned char buffer[], int blen, char *addr, int p);
  bool cread(unsigned char buffer[], char *addr, int p);

 protected:


 private:

  int sock;

  
  bool init(int p, char *addr);


    
};

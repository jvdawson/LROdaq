
class client
{

 public:
  client(int default_p, char default_addr[], int p,char addr[]);
  ~client();
  //to work with ? selector?
  

  bool client_send(unsigned char buffer[], int blen);
  bool client_read(unsigned char buffer[]);

 protected:


 private:
  int PORT;
  int DEFAULT_PORT;
  int sock;

  
  void init(char default_addr[], char addr[]);
  bool create_socket();
  bool connect_socket(struct sockaddr_in serv_addr,char default_addr[], char addr[]);

};

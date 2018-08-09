
class server
{

 public:
  server(int p);
  ~server();
  //to work with ? selector?
  

  bool server_send(unsigned char buffer[], int blen);
  bool server_read(unsigned char buffer[]);

 protected:


 private:
  int PORT;
  int sock;
  int new_socket;
  
  void init();
  bool create_socket();
  bool connect_socket(struct sockaddr_in serv_addr);//point-to-point I thought?
  bool attach_socket();
};

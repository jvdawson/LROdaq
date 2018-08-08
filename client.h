//#include <stdio.h>
//#include <unistd.h>
//#include <arpa/inet.h>
//#include <sys/socket.h>
//#include <stdlib.h>
//#include <netinet/in.h>
//#include <string.h>

class client
{

 public:
  client(int p,char addr[]);
  ~client();
  //to work with ? selector?
  

 protected:

  bool client_send(unsigned char buffer[], int blen);
  bool client_read(unsigned char buffer[]);


 private:
  int PORT;
  int sock;
  
  void init(char addr[]);
  bool create_socket();
  bool connect_socket(struct sockaddr_in serv_addr,char addr[]);

};

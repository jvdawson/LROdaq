// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <iostream>
#include <string.h>
//for select?
#include <sys/time.h>
#include <sys/types.h>
//
#include <vector>

#define PORT 6086
int main(int argc, char const *argv[])
{
  char *ready_message="ready";
  char *running_message="running";
  char *error_message="error";

  int server_fd, new_socket, valread;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[1024] = {0};

      
  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
      perror("socket failed");
      exit(EXIT_FAILURE);
    }
  //SO_REUSEPORT?
  // Forcefully attaching socket to the port 8080
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR ,//| SO_REUSEPORT,
		 &opt, sizeof(opt)))
    {
      perror("setsockopt");
      exit(EXIT_FAILURE);
    }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons( PORT );
      
  // Forcefully attaching socket to the port 8080
  if (bind(server_fd, (struct sockaddr *)&address, 
	   sizeof(address))<0)
    {
      perror("bind failed");
      exit(EXIT_FAILURE);
    }
  if (listen(server_fd, 3) < 0)
    {
      perror("listen");
      exit(EXIT_FAILURE);
    }
  /*  if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
			   (socklen_t*)&addrlen))<0)
    {
      perror("accept");
      exit(EXIT_FAILURE);
    }else{
    printf("connection accepted %d\n",new_socket);
    }*/

  //OK THIS IS WORKING --
  //frame work -- python -> sends start acq, stop acq signals [and potential other] to daq daemon.. TCP/IP or UDP?
  //what about other connections? someone else uses webpage?
  /*In order to be notified of incoming connections on a  socket,  you  can
       use  select(2)  or  poll(2).  A readable event will be delivered when a
       new connection is attempted and you may then call  accept()  to  get  a
       socket  for  that connection.  Alternatively, you can set the socket to
       deliver SIGIO when activity occurs  on  a  socket;  see  socket(7)  for
       details.*/
  //what about disconnections?
  
  ////////////////////////////////////////////////////////////
  fd_set rfds,wfds; //read, error, write
  struct timeval tv;
  int retval;
  int nmax = server_fd;
  FD_ZERO(&rfds);
  FD_SET(server_fd, &rfds); //serverfd new_socket
  

  tv.tv_sec = 20;
  tv.tv_usec = 0;
  std::vector<int> connected;
 
  //NEED loop, [task?], need to accept new connections, and disconnect..  
  while(true) //BREAK OUT
    {
     
      FD_ZERO(&rfds);
      FD_SET(server_fd, &rfds); //to listen for new connections
     
      nmax = server_fd+1;
      std::cout<<"nmax : "<<nmax<<std::endl;
        for (std::vector<int>::iterator it = connected.begin(); it != connected.end(); ++it) {

        FD_SET(*it, &rfds);
        if (*it >= nmax) nmax = *it + 1;
      }
      std::cout<<"nmax : "<<nmax<<std::endl;
      //TIME OUT 
      tv.tv_sec = 20;
      tv.tv_usec = 0;
 
      retval = select(nmax+1, &rfds, NULL, NULL, &tv);
 
      if (retval == -1){
	perror("select()");
	
      }  else if (retval){
	printf("Data is available now. %d \n",retval);
	if(FD_ISSET(server_fd, &rfds)){ //create new socket..
	  if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
				   (socklen_t*)&addrlen))<0)
	    {	      
	      perror("accept");
	      exit(EXIT_FAILURE);
	    }else{
	    printf("connection accepted %d\n",new_socket);
	    connected.push_back(new_socket);//just need to push back? 
	  }
	}//NOW CHECK ALL CONNECTIONS

	
	//what about disconnections?? -- NEED TO CHECK ERRORS..
	for (std::vector<int>::iterator it = connected.begin(); it != connected.end(); ++it) {

	  if (FD_ISSET(*it, &rfds)) {
	    std::cout<<"data on "<<*it<<std::endl;
	    // handle data on this connection
	    for(int i=0;i<1024;i++){buffer[i]=0;}
	    valread = read(*it, buffer, 1024);
	    if(valread>0){
	    std::cout<<valread<<" "<<buffer<<std::endl;
	    if(strcmp(buffer,"status")==0)
	      {
		std::cout<<"client asks for status"<<std::endl;
		send(*it,ready_message, strlen(ready_message),0 );
	      }else if(strcmp(buffer,"start")==0)
	      {
		std::cout<<"client asks to start acquisition"<<std::endl;

		//do something and change state -- assume all OK for now

		send(*it,running_message, strlen(running_message),0 );
	      }else if(strcmp(buffer,"stop")==0)
	      {
		std::cout<<"client asks to stop acquisition"<<std::endl;

		//do something and change state --assume all OK for now

		send(*it,ready_message, strlen(ready_message),0 );
	      }else{
	      std::cout<<"command unknown"<<std::endl;
	    }


	    //	    for(int i=0;i<valread;i++){ std::cout<<std::hex<<unsigned(buffer[i])<<" ";}printf("\n");
	    }else{
	    
	    std::cout<<"socket disconnected"<<std::endl; //remove from connected
	    connected.erase(it);
	   
	    break;
	    }
	  } 
	}


      } else{
	printf("No data within twenty seconds.\n");
      }
      sleep(1); //delay necessary?
    }


  //////////////////////////////////////////////////////////
  /*
  valread = read( new_socket , buffer, 1024);
  printf("Elements %d\n",valread);

  for(int i=0;i<valread;i++){ std::cout<<std::hex<<unsigned(buffer[i])<<" ";}printf("\n");

  send(new_socket , hello , strlen(hello) , 0 );
  printf("Hello message sent\n");*/

  return 0;
}

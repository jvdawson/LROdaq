#include <netinet/in.h>
#include <net/if.h>             //where if_nameindex() was hiding
#include <stdio.h>      //for ioctl() arg
#include <sys/ioctl.h>
#include <cstring>
//thanks linus
#define NIPQUAD(addr)   ((unsigned char *)&addr)[0], \
                        ((unsigned char *)&addr)[1], \
                        ((unsigned char *)&addr)[2], \
                        ((unsigned char *)&addr)[3]

#define ID 172

//int get_ip(char myip[]);

/*int main(int argc, char **argv)
{
  char myip[100];
  int i=get_ip(myip);
  printf("%s\n",myip);
  return 0;
}
*/

int get_ip(char myip[])
{

  
  int sock = 0;
        struct ifreq ifreq;
        struct sockaddr_in *saptr = NULL;
        struct if_nameindex *iflist = NULL, *listsave = NULL;

#ifndef SIOCGIFADDR
        printf("get a newer kernel buddy!\n");
        return 1;
#endif
        //need a socket for ioctl()
        if( (sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
                perror("socket");
                return 1;
        }

        //returns pointer to dynamically allocated list of structs
        iflist = listsave = if_nameindex();

        //walk thru the array returned and query for each
        //interface's address
        for(iflist; *(char *)iflist != 0; iflist++){
                //copy in the interface name to look up address of
                strncpy(ifreq.ifr_name, iflist->if_name, IF_NAMESIZE);

		if(strncmp(ifreq.ifr_name,"eth",3)==0){
		  //get the address for this interface
		  if(ioctl(sock, SIOCGIFADDR, &ifreq) != 0){
		    perror("ioctl");
		    return 1;
		  }
		  
		  //print out the address
		  saptr = (struct sockaddr_in *)&ifreq.ifr_addr;
		  
		  printf( "%-5s  ::  %3d.%3d.%3d.%3d\n",
			  ifreq.ifr_name,
			  NIPQUAD(saptr->sin_addr.s_addr) );
		  
		  if(((unsigned char *)&saptr->sin_addr.s_addr)[0] == ID){
		  
		    sprintf(myip,"%d.%d.%d.%d",NIPQUAD(saptr->sin_addr.s_addr));
		    //		  strcpy(myip, saptr->sin_addr.s_addr);
		  }
		}}
        //free the dynamic memory kernel allocated for us
        if_freenameindex(listsave);
        return 0;
}

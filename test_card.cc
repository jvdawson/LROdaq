#include "card.h"
#include "messagelistener.h"
#include "datareceiver.h"
 #include <unistd.h>
#include <iostream>
#include <fstream>
//file the card
//take some data
//write to file
//ignore messages for now...
//typedef std::basic_ofstream<unsigned char, std::char_traits<unsigned char> > uofstream;

int main(void)
{//127.0.0.1
  card mycard("172.16.4.13");
  messagelistener messenger("172.16.4.1");
  datareceiver mydata;
  
  bool res=false;
  res = mycard.isReady(); //Looks OK
  /*   bool card::SetControlRegisters(uint16_t STrace_Num_Trig,
			       bool Sstart_enable, 
			       bool Srem_log_msg_enable, 
			       bool Ssoft_reboot,
			       uint32_t Spre_trig_samples,
			       uint32_t Supdport, //not sure
			       uint32_t Snbevents)*/
  //Snbevents -- is the number of events to read..
  
  //in progress
  int nevents=5;
  std::ofstream fbin("data.bin",std::ios::binary);
  res = mycard.SetControlRegisters(4,1,1,0,31,nevents);//turned off messenger..
   
  //  messenger.read();
  // messenger.read();
  // messenger.read();



  //  mycard.Data_ReadRequest();
  mydata.AddReceiver(mycard);

  for(int i=0;i<10;i++)
    {
      messenger.read();
    }
  
  mydata.Stop();
  /*    for(int i=0;i<nevents;i++)
      {
	
	mycard.ReadData();
	std::cout<<"read event "<<i<<" "<<mycard.datalength<<std::endl;
        
	fbin.write((char*)mycard.databuffer,mycard.datalength);
	
	std::cout<<std::endl;
	}*/
 
    fbin.close();
    
return 0;
}

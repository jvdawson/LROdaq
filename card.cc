#include "card.h"
#include <string.h>
#include <iostream>


card::card(char addr[]):client(8080,addr)
{
  //325
  Trace_Num_Trig=1000;
  start_enable =false;
  rem_log_msg_enable=false;
  soft_reboot=false;
  udpport = 325;
  nbevents = 1000;

}
card::~card()
{


}

//SETS all control registers and writes them
bool card::SetControlRegisters(uint16_t STrace_Num_Trig,
			   bool Sstart_enable, 
			   bool Srem_log_msg_enable, 
			   bool Ssoft_reboot,
			   uint32_t Spre_trig_samples,
			   uint32_t Supdport, //not sure
			   uint32_t Snbevents)
{
  Trace_Num_Trig=STrace_Num_Trig;
  start_enable= Sstart_enable;
  rem_log_msg_enable = Srem_log_msg_enable;
		
  soft_reboot = Ssoft_reboot;
  pre_trig_samples = Spre_trig_samples;
  udpport = Supdport; //not sure
  nbevents= Snbevents;

  send_control_registers();
}

bool card::isReady()
{//PROBLEM... with sizes?
  //array of 20 is U32 ->type cast to U8 [array size is 20*4 U8
  //3 is U16 ->type cast to U8
  unsigned char sbuffer[83];
  for(int i=0;i<82;i++)sbuffer[i]='0';
  //  memset(&sbuffer, '0', sizeof(sbuffer));
  sbuffer[1]='3'; //BIG ENDIAN ?
  //do I need \n?
  sbuffer[82]='\n';
  client_send((char*)&sbuffer);

  //read
  unsigned char rbuffer[1024]; //need to be longer than value read
  client_read((char*)&rbuffer);
  //find ABCD
  std::cout<<" "<<rbuffer<<std::endl;
  if (rbuffer[0]==0xAB && rbuffer[1]==0xCD)
    {
      std::cout<<"found!"<<std::endl;
      return true;
    }
  return false;
}
////////////////////////////////////////////////////////

//testing 
//BIGENDIAN..
//
bool card::send_control_registers()
{
  //values are held in card?
  //1 U16.. 4 U32
  //pre_trig_samples U32 (4 bytes) -> U8 [100] [00000064]
  uint16_t header = 0x2;
  char sheader[2];
  memcpy(sheader, &header, sizeof(header));

  //hmm this isn't working..
  // sheader= (char*) &header;
  uint32_t buffer[4];
 //  char sbuffer[4*4]=&buffer[0]; 

  buffer[3] = pre_trig_samples;
  buffer[2] = nbevents;
  //nb events (100) ->  [00000064]

  //result C81

  //TRACE NUM 200 + bits for start enable, rem_log_msg_en, softreboot
  //start_enable 
  //rem_log_msg_enable -- shift 1
  //soft_reboot -- shift 2
  //OR
  buffer[1] = (start_enable | (rem_log_msg_enable<<1) | (soft_reboot<<2) | Trace_Num_Trig<<4); //0000 0C81
  
  //destination udp port? 5000? or 325?
  buffer[0] = udpport; 
 
  char sbuffer[4*4];

  memcpy(sbuffer, &buffer, sizeof(buffer));
 
  char stotal[4*4 + 2];
  strcat(stotal,sheader);
  strcat(stotal,sbuffer);

  //concat these together, convert to string..
  client_send(stotal);



}

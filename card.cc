#include "card.h"
#include <string.h>
#include <iostream>


card::card(char addr[],int p):client(p,addr)
{
  //325
  Trace_Num_Trig=200;
  start_enable =true;
  rem_log_msg_enable=false;
  soft_reboot=false;
  udpport = 325;
  nbevents = 100;

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
{//PROBLEM... here...
  //array of 20 is U32 ->type cast to U8 [array size is 20*4 U8
  //3 is U16 ->type cast to U8
  unsigned char sbuffer[82]={0}; 
  sbuffer[1]=0x3;

  client_send((char*)&sbuffer, 82);


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
uint16_t swap16(uint16_t num)
{
  uint16_t swapped = (num>>8) | (num<<8);
  return swapped;
}
uint32_t swap(uint32_t num)
{
  uint32_t swapped= ((num>>24)&0xff) | // move byte 3 to byte 0
    ((num<<8)&0xff0000) | // move byte 1 to byte 2
    ((num>>8)&0xff00) | // move byte 2 to byte 1
    ((num<<24)&0xff000000); // byte 0 to byte 3
 
  return swapped;
}
bool card::send_control_registers()
{
  //values are held in card?
  //1 U16.. 4 U32
  //pre_trig_samples U32 (4 bytes) -> U8 [100] [00000064]
 
  unsigned char sheader[2]={0,0x2};

  uint32_t buffer[4]={0};

  buffer[3] = swap(pre_trig_samples);
  buffer[2] = swap(nbevents);
  //nb events (100) ->  [00000064]

  //result C81 --OK

  //TRACE NUM 200 + bits for start enable, rem_log_msg_en, softreboot
  //start_enable 
  //rem_log_msg_enable -- shift 1
  //soft_reboot -- shift 2
  //OR
  buffer[1] = swap(start_enable | (rem_log_msg_enable<<1) | (soft_reboot<<2) | Trace_Num_Trig<<4); //0000 0C81
  
  //destination udp port? 5000? or 325?
  buffer[0] = swap(udpport); 
  //  for(int i=0;i<4;i++)std::cout<<std::hex<<unsigned(buffer[i])<<" ";
  //std::cout<<std::endl; OK

  unsigned char sbuffer[16];
  memcpy(sbuffer,&buffer,16);

  
  for(int i=0;i<4*4;i++)std::cout<<std::hex<<unsigned(sbuffer[i])<<" ";
  std::cout<<std::endl;//OK

  // unsigned char stotal[4*4 + 2];
  // strcat(stotal,sbuffer);
  // strcat(stotal,sheader);

  //concat these together, convert to string..
  client_send((char*)sbuffer,4*4);



}
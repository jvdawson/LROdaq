#include "card.h"
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>




card::card(char *addr, int nchannels)
{

  Trace_Num_Trig=200;
  start_enable =false;
  rem_log_msg_enable=false;
  soft_reboot=false;
  udpport = 5050; //5000;//?? -- needs to be configured..65000
  nbevents = 100;
  //172.16.4.1 daq
  char daq_ip[100];
  if(get_ip(daq_ip)!=0)
    {
      perror("failed to get LRODAQ ip address ");
    }
  std::cout<<"comm "<<std::endl;
  comm = new client(50325,  (char*)daq_ip);
  
  card_address=new char[sizeof(addr)];
  strcpy(card_address, addr);//?

  std::cout<<"dcomm"<<std::endl;
  dcomm = new client(udpport, (char*)daq_ip);

  //1024 samples/event/ch
  //2 bytes per sample
  //64 channels -- to configure

 
  dindex=0;//empty...
  maxlength=1024*2*nchannels;
  databuffer=new unsigned char[maxlength];
 
}
card::~card()
{
  delete comm;
  delete dcomm;
  delete databuffer;

}

//SETS all control registers and writes them
bool card::SetControlRegisters(uint16_t STrace_Num_Trig,
			       bool Sstart_enable, 
			       bool Srem_log_msg_enable, 
			       bool Ssoft_reboot,
			       uint32_t Spre_trig_samples,
			       //			       uint32_t Supdport, //not sure
			       uint32_t Snbevents)
{
  Trace_Num_Trig=STrace_Num_Trig;
  start_enable= Sstart_enable;
  rem_log_msg_enable = Srem_log_msg_enable;
		
  soft_reboot = Ssoft_reboot;
  pre_trig_samples = Spre_trig_samples;
  //FIXED!  udpport = Supdport; //not sure
  nbevents= Snbevents;

  send_control_registers();
}

bool card::isReady()
{
  //array of 20 is U32 ->type cast to U8 [array size is 20*4 U8
  //3 is U16 ->type cast to U8
  unsigned char sbuffer[82]={0}; 
  sbuffer[1]=0x3;
  bool res=false;
  res = comm->csend(sbuffer, 82, card_address, 325);


  unsigned char rbuffer[1024]; //need to be longer than value read
  int length;
  length = comm->cread(rbuffer,1024);
  for(int i=0;i<length;i++)
    {
      std::cout<<std::hex<<unsigned(rbuffer[i])<<" ";
    }
  std::cout<<std::dec<<std::endl;
  //find ABCD
  if (rbuffer[4]==0xAB && rbuffer[5]==0xCD)
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
  buffer[1] = swap(start_enable | (rem_log_msg_enable<<1) | (soft_reboot<<2) | Trace_Num_Trig<<4); //0000 0C81
  buffer[0] = swap(udpport); //??

  unsigned char sbuffer[16];
  memcpy(sbuffer,&buffer,16);

  std::cout<<"CONTROL REGISTERS REQUESTED"<<std::endl;
  for(int i=0;i<4*4;i++)std::cout<<std::hex<<unsigned(sbuffer[i])<<" ";
  std::cout<<std::endl;//OK

   unsigned char stotal[4*4 + 2];
   for(int i=0;i<2;i++)
     {
       stotal[i]=sheader[i];
     }
   for(int i=0;i<4*4;i++)
     {
       stotal[i+2]=sbuffer[i];
     }
  //concat these together, convert to string..
   bool res = comm->csend(stotal,4*4+2, card_address, 325);
}
/////////////////////////////////////////////////
void card::Data_ReadRequest()
{
  std::cout<<"Data Read Request"<<std::endl;
  //TIMEOUT? 20 U32_T
  //which port to use?
  unsigned char sbuffer[80]={0};//send ACK to port 64000 of AMC
  int res = comm->csend(sbuffer,80,card_address,64000);
  unsigned char obuffer[4]={0}; //according to Cyril it should send back the config.. but where? I only see on messenger...
  res = comm->cread(obuffer,4);
  std::cout<<obuffer<<std::endl;
  
}
int card::GetDataSocket(){
  return dcomm->GetSock();
}

void card::ReadData()
{
  std::cout<<"Read Data"<<std::endl;
  int packetsize=0;
  //datalength
  packetsize = dcomm->cread(&databuffer[dindex],maxlength);
  std::cout<<packetsize<<" "<<dindex<<std::endl;
  if(dindex+packetsize>maxlength)
    {
      perror("total event size greater than maximum");
    }
  dindex=dindex+packetsize; //move along index..

  //I expect first UDP 1472, second of 572 * nchannels....
  //there's a risk of blocking (what if a packet is missing?)
  
  
}
bool card::isEventComplete()
{
  if(dindex==maxlength)
    {
      return true;
    }
  return false;
}
void card::WriteToPipe(int *pipefd)
{
  //write the data to the pipe! it's too big to write in one go
  //max is 64kB or so
  int nindex = maxlength/2;
  std::cout<<"write to pipe : "<<maxlength/2<<std::endl;
  int cblen = write(*pipefd, databuffer, nindex);
  std::cout<<"wrote "<<cblen<<" to pipe "<<nindex<<std::endl;
  cblen = write(*pipefd, &databuffer[nindex], nindex);
  std::cout<<"wrote "<<cblen<<" to pipe "<<nindex<<std::endl;
 
  dindex=0;
}

#ifndef CARD
#define CARD
//#include "server.h"
#include "client.h"
#include "getipaddress.h"

#include <stdint.h> //: uint8_t, uint16_t, uint32_t, and uint64_t

class card
{
 public:
  card(char *addr, int nchannels);
  ~card();

  bool SetControlRegisters(uint16_t STrace_Num_Trig,
			   bool Sstart_enable, 
			   bool Srem_log_msg_enable, 
			   bool Ssoft_reboot,
			   uint32_t Spre_trig_samples,
			   uint32_t Snbevents);
  bool isReady();
  void Data_ReadRequest();
  void ReadData();
  int GetDataSocket();

  //1024 samples/event/ch
  //2 bytes per sample
  //64 channels -- to configure
  bool isEventComplete();
  void WriteToPipe(int *pipefd);

  unsigned char *databuffer;
  int dindex;
  int maxlength;
  char daq_ip[100];
 protected:
 
  bool send_control_registers();


 private:

  
  //these need to be set, function to set
  uint16_t Trace_Num_Trig;
  bool start_enable, rem_log_msg_enable, soft_reboot;
  uint32_t pre_trig_samples;
  uint32_t udpport;
  uint32_t nbevents;

  //
  client *dcomm;
  client *comm;
  char *card_address;
};
#endif

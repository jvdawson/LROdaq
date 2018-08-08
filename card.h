#include "client.h"
#include <stdint.h> //: uint8_t, uint16_t, uint32_t, and uint64_t

class card: protected client
{
 public:
  card(char addr[], int p);
  ~card();

  bool SetControlRegisters(uint16_t STrace_Num_Trig,
			   bool Sstart_enable, 
			   bool Srem_log_msg_enable, 
			   bool Ssoft_reboot,
			   uint32_t Spre_trig_samples,
			   uint32_t Supdport, //not sure
			   uint32_t Snbevents);
  bool isReady();

 protected:

  bool send_control_registers();


 private:

  //these need to be set, function to set
  uint16_t Trace_Num_Trig;
  bool start_enable, rem_log_msg_enable, soft_reboot;
  uint32_t pre_trig_samples;
  uint32_t udpport;
  uint32_t nbevents;

};
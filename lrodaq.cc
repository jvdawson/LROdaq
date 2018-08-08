#include "card.h"
//testing version for real...
int main(void)
{
  card mycard("172.16.4.13",325);
  bool res=false;
  res = mycard.isReady(); 
  /* bool card::SetControlRegisters(uint16_t STrace_Num_Trig,
			       bool Sstart_enable, 
			       bool Srem_log_msg_enable, 
			       bool Ssoft_reboot,
			       uint32_t Spre_trig_samples,
			       uint32_t Supdport, //not sure
			       uint32_t Snbevents)
  */
  //in progress 
  //  res = mycard.SetControlRegisters(200,1,0,0,100,325,100);
  return 0;
}

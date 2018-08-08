#include "card.h"

int main(void)
{
  card mycard("127.0.0.1",8080);
  bool res=false;
  //  res = mycard.isReady(); //Looks OK
  /* bool card::SetControlRegisters(uint16_t STrace_Num_Trig,
			       bool Sstart_enable, 
			       bool Srem_log_msg_enable, 
			       bool Ssoft_reboot,
			       uint32_t Spre_trig_samples,
			       uint32_t Supdport, //not sure
			       uint32_t Snbevents)
  */
  //in progress 
  res = mycard.SetControlRegisters(200,1,0,0,100,325,100);
  return 0;
}

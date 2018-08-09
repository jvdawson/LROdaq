#include "card.h"
//NEED ERROR HANDLING and timeouts!!
//testing version for real...
int main(void)
{

  card mycard((char*)"172.16.4.13");//172.16.4.13
  bool res=false;



 
  res = mycard.isReady();
  
  /* bool card::SetControlRegisters(uint16_t STrace_Num_Trig,
			       bool Sstart_enable, 
			       bool Srem_log_msg_enable, 
			       bool Ssoft_reboot,
			       uint32_t Spre_trig_samples,
			       uint32_t Snbevents)
  */
  //in progress 
  //  res = mycard.SetControlRegisters(200,0,0,0,100,100);

  res = mycard.SetControlRegisters(200,1,0,0,0,200);


  mycard.Data_ReadRequest();

  res = mycard.SetControlRegisters(0,0,0,0,0,00);


  return 0;
}

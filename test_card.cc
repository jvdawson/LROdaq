#include "card.h"
#include "messagelistener.h"
//#include "datareceiver.h"
 #include <unistd.h>
int main(void)
{//127.0.0.1
  card mycard("172.16.4.13");
  //  messagelistener messenger("172.16.4.1");
  // datareceiver mydata;
  
  bool res=false;
  //  res = mycard.isReady(); //Looks OK
  /*   bool card::SetControlRegisters(uint16_t STrace_Num_Trig,
			       bool Sstart_enable, 
			       bool Srem_log_msg_enable, 
			       bool Ssoft_reboot,
			       uint32_t Spre_trig_samples,
			       uint32_t Supdport, //not sure
			       uint32_t Snbevents)*/
  //Snbevents -- is the number of events to read..
  
  //in progress 
   res = mycard.SetControlRegisters(4,1,1,0,31,5);
   
   //   messenger.read();
   // messenger.read();
   //messenger.read();



   mycard.Data_ReadRequest();
   //   mydata.AddReceiver(65000, "172.16.4.1");
   res = mycard.SetControlRegisters(10,0,0,0,15,0);

return 0;
}

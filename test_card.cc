#include "card.h"

int main(void)
{
  card mycard("127.0.0.1");
  
  bool res = mycard.isReady();
  //bool res = mycard.SetControlRegisters(100,1,0,0,1000,325,1000);
  return 0;
}

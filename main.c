#include "Control.h"
#include "eeprom.h"
sControl Control;
sControl* pC = &Control;

int main()
{
	Control_Conf();
	Outputs_Conf();
	Inputs_Conf();
	COM_Conf();
	LED3_ON;
	delay_ms(500);
	LED3_OFF;
  EE_Init();
	Control_ReadRulesFromEeprom();
	while(1)
	{
		LED1_TOG;
		Rule_Act();
		COM_SendToHost();
		delay_ms(100);
	}
}

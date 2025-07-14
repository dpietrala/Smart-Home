#include "Control.h"
extern sControl* pC;
extern uint16_t VirtAddVarTab[NB_OF_VAR];
extern uint16_t VarDataTab[NB_OF_VAR];
static void RCC_SystemInit(void)
{
	uint32_t PLL_M=8, PLL_N=336, PLL_P=2, PLL_Q=7;
	RCC->CR |= RCC_CR_HSEON;								//W³¹czenie HSE
	while(!(RCC->CR & RCC_CR_HSERDY));						//czekamy a¿ HSE bêdzie gotowy
	RCC->CFGR = RCC_CFGR_PPRE2_DIV2 | RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_HPRE_DIV1;
	RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1)-1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) | (PLL_Q << 24);
	FLASH->ACR |= FLASH_ACR_ICEN |FLASH_ACR_DCEN |FLASH_ACR_LATENCY_5WS;
	RCC->CR |= RCC_CR_PLLON;								//w³¹czenie PLL
	while(!(RCC->CR & RCC_CR_PLLRDY));						//czekanie a¿ PLL gotowa
	RCC->CFGR |= RCC_CFGR_SW_PLL;							//PLL jako Ÿród³o dla SYSCLK
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL); //czekanie a¿ PLL bedzie gotowe jako SYSCLK
}
static void RCC_Conf(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN |
									RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIOFEN |
									RCC_AHB1ENR_GPIOGEN | RCC_AHB1ENR_GPIOHEN | RCC_AHB1ENR_GPIOIEN |
									RCC_AHB1ENR_DMA1EN 	| RCC_AHB1ENR_DMA2EN  | RCC_AHB1ENR_BKPSRAMEN;
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM3EN | RCC_APB1ENR_TIM4EN | RCC_APB1ENR_SPI3EN |
									RCC_APB1ENR_TIM5EN | RCC_APB1ENR_TIM6EN | RCC_APB1ENR_TIM7EN | RCC_APB1ENR_PWREN;
	
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_USART6EN | RCC_APB2ENR_SPI1EN |
									RCC_APB2ENR_TIM1EN | RCC_APB2ENR_TIM8EN;
}
static void Led_Conf(void)
{
	//dISCOVERY
	GPIOD->MODER 	|= GPIO_MODER_MODER12_0 | GPIO_MODER_MODER13_0 | GPIO_MODER_MODER14_0 | GPIO_MODER_MODER15_0;
	GPIOD->PUPDR 	|= GPIO_PUPDR_PUPDR12_0 | GPIO_PUPDR_PUPDR13_0 | GPIO_PUPDR_PUPDR14_0 | GPIO_PUPDR_PUPDR15_0;
	
//	GPIOE->MODER 	|= GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0;
//	GPIOE->PUPDR 	|= GPIO_PUPDR_PUPDR2_0 | GPIO_PUPDR_PUPDR3_0 | GPIO_PUPDR_PUPDR4_0 | GPIO_PUPDR_PUPDR5_0;
}
static void Control_StructConf(void)
{
	pC->Status.times = 0;
	pC->Status.work = On;
	for(uint16_t i=0;i<MAXINPUT;i++)
	{
		pC->Inputs[i].forced = Off;
		pC->Inputs[i].numclick = 0;
		pC->Inputs[i].state = InOff;
		pC->Inputs[i].tempstate = InTempOff;
		pC->Inputs[i].tempstateprev = InTempOff;
		pC->Inputs[i].timeon = 0;
		pC->Inputs[i].timeoff = 0;
	}
	for(uint16_t i=0;i<MAXOUTPUT;i++)
	{
		pC->Outputs[i].state = OutOff;
		pC->Outputs[i].newstate = OutOff;
		pC->Outputs[i].time = 0;
	}
	pC->rulecount = 0;
	
//	uint16_t index=0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 1;
//	pC->Rules[index].instate = InOne;
//	pC->Rules[index].outnum = 1;
//	pC->Rules[index].outstate = OutTog;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 1;
//	pC->Rules[index].instate = InTwo;
//	pC->Rules[index].outnum = 1;
//	pC->Rules[index].outstate = OutTime;
//	pC->Rules[index++].time = 2000;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 1;
//	pC->Rules[index].instate = InLong;
//	pC->Rules[index].outnum = 1;
//	pC->Rules[index].outstate = OutOff;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 0;
//	pC->Rules[index].instate = InOne;
//	pC->Rules[index].outnum = CableOut_11;
//	pC->Rules[index].outstate = OutTog;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 0;
//	pC->Rules[index].instate = InLong;
//	pC->Rules[index].outnum = CableOut_11;
//	pC->Rules[index].outstate = OutOff;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 0;
//	pC->Rules[index].instate = InLong;
//	pC->Rules[index].outnum = CableOut_12;
//	pC->Rules[index].outstate = OutOff;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 0;
//	pC->Rules[index].instate = InLong;
//	pC->Rules[index].outnum = CableOut_13;
//	pC->Rules[index].outstate = OutOff;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 0;
//	pC->Rules[index].instate = InTwo;
//	pC->Rules[index].outnum = CableOut_11;
//	pC->Rules[index].outstate = OutTog;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 0;
//	pC->Rules[index].instate = InTwo;
//	pC->Rules[index].outnum = CableOut_12;
//	pC->Rules[index].outstate = OutTog;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 0;
//	pC->Rules[index].instate = InTwo;
//	pC->Rules[index].outnum = CableOut_13;
//	pC->Rules[index].outstate = OutTog;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 1;
//	pC->Rules[index].instate = InOne;
//	pC->Rules[index].outnum = CableOut_78;
//	pC->Rules[index].outstate = OutTog;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 2;
//	pC->Rules[index].instate = InOne;
//	pC->Rules[index].outnum = CableOut_14;
//	pC->Rules[index].outstate = OutTog;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 2;
//	pC->Rules[index].instate = InTwo;
//	pC->Rules[index].outnum = CableOut_14;
//	pC->Rules[index].outstate = OutTime;
//	pC->Rules[index++].time = 10*1000;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 3;
//	pC->Rules[index].instate = InOne;
//	pC->Rules[index].outnum = CableOut_15;
//	pC->Rules[index].outstate = OutTog;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 3;
//	pC->Rules[index].instate = InTwo;
//	pC->Rules[index].outnum = CableOut_14;
//	pC->Rules[index].outstate = OutTog;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 3;
//	pC->Rules[index].instate = InTwo;
//	pC->Rules[index].outnum = CableOut_15;
//	pC->Rules[index].outstate = OutTog;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 4;
//	pC->Rules[index].instate = InOne;
//	pC->Rules[index].outnum = CableOut_12;
//	pC->Rules[index].outstate = OutTog;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 4;
//	pC->Rules[index].instate = InOne;
//	pC->Rules[index].outnum = CableOut_13;
//	pC->Rules[index].outstate = OutTog;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 4;
//	pC->Rules[index].instate = InTwo;
//	pC->Rules[index].outnum = CableOut_11;
//	pC->Rules[index].outstate = OutTog;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 4;
//	pC->Rules[index].instate = InTwo;
//	pC->Rules[index].outnum = CableOut_12;
//	pC->Rules[index].outstate = OutTog;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 4;
//	pC->Rules[index].instate = InTwo;
//	pC->Rules[index].outnum = CableOut_13;
//	pC->Rules[index].outstate = OutTog;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 5;
//	pC->Rules[index].instate = InOne;
//	pC->Rules[index].outnum = CableOut_11;
//	pC->Rules[index].outstate = OutTog;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 5;
//	pC->Rules[index].instate = InTwo;
//	pC->Rules[index].outnum = CableOut_11;
//	pC->Rules[index].outstate = OutTog;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 5;
//	pC->Rules[index].instate = InTwo;
//	pC->Rules[index].outnum = CableOut_12;
//	pC->Rules[index].outstate = OutTog;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 5;
//	pC->Rules[index].instate = InTwo;
//	pC->Rules[index].outnum = CableOut_13;
//	pC->Rules[index].outstate = OutTog;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 6;
//	pC->Rules[index].instate = InOne;
//	pC->Rules[index].outnum = CableOut_16;
//	pC->Rules[index].outstate = OutTog;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 7;
//	pC->Rules[index].instate = InOne;
//	pC->Rules[index].outnum = CableOut_17;
//	pC->Rules[index].outstate = OutTog;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 8;
//	pC->Rules[index].instate = InOne;
//	pC->Rules[index].outnum = CableOut_21;
//	pC->Rules[index].outstate = OutTog;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 8;
//	pC->Rules[index].instate = InLong;
//	pC->Rules[index].outnum = CableOut_23;
//	pC->Rules[index].outstate = OutTog;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 9;
//	pC->Rules[index].instate = InOne;
//	pC->Rules[index].outnum = CableOut_22;
//	pC->Rules[index].outstate = OutTog;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 9;
//	pC->Rules[index].instate = InLong;
//	pC->Rules[index].outnum = CableOut_23;
//	pC->Rules[index].outstate = OutTog;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 10;
//	pC->Rules[index].instate = InOne;
//	pC->Rules[index].outnum = CableOut_20;
//	pC->Rules[index].outstate = OutTog;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 3;
//	pC->Rules[index].instate = InTwo;
//	pC->Rules[index].outnum = CableOut_78;
//	pC->Rules[index].outstate = OutTime;
//	pC->Rules[index++].time = 5*60*60*1000;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 3;
//	pC->Rules[index].instate = InTwo;
//	pC->Rules[index].outnum = CableOut_79;
//	pC->Rules[index].outstate = OutTime;
//	pC->Rules[index++].time = 5*60*60*1000;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 3;
//	pC->Rules[index].instate = InTwo;
//	pC->Rules[index].outnum = CableOut_80;
//	pC->Rules[index].outstate = OutTime;
//	pC->Rules[index++].time = 5*60*60*1000;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 3;
//	pC->Rules[index].instate = InTwo;
//	pC->Rules[index].outnum = CableOut_81;
//	pC->Rules[index].outstate = OutTime;
//	pC->Rules[index++].time = 5*60*60*1000;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 3;
//	pC->Rules[index].instate = InTwo;
//	pC->Rules[index].outnum = CableOut_82;
//	pC->Rules[index].outstate = OutTime;
//	pC->Rules[index++].time = 5*60*60*1000;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 3;
//	pC->Rules[index].instate = InTwo;
//	pC->Rules[index].outnum = CableOut_83;
//	pC->Rules[index].outstate = OutTime;
//	pC->Rules[index++].time = 5*60*60*1000;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 3;
//	pC->Rules[index].instate = InTwo;
//	pC->Rules[index].outnum = CableOut_84;
//	pC->Rules[index].outstate = OutTime;
//	pC->Rules[index++].time = 5*60*60*1000;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 3;
//	pC->Rules[index].instate = InLong;
//	pC->Rules[index].outnum = CableOut_78;
//	pC->Rules[index].outstate = OutOff;
//	pC->Rules[index++].time = 0;

//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 3;
//	pC->Rules[index].instate = InLong;
//	pC->Rules[index].outnum = CableOut_79;
//	pC->Rules[index].outstate = OutOff;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 3;
//	pC->Rules[index].instate = InLong;
//	pC->Rules[index].outnum = CableOut_80;
//	pC->Rules[index].outstate = OutOff;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 3;
//	pC->Rules[index].instate = InLong;
//	pC->Rules[index].outnum = CableOut_81;
//	pC->Rules[index].outstate = OutOff;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 3;
//	pC->Rules[index].instate = InLong;
//	pC->Rules[index].outnum = CableOut_82;
//	pC->Rules[index].outstate = OutOff;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 3;
//	pC->Rules[index].instate = InLong;
//	pC->Rules[index].outnum = CableOut_83;
//	pC->Rules[index].outstate = OutOff;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 3;
//	pC->Rules[index].instate = InLong;
//	pC->Rules[index].outnum = CableOut_84;
//	pC->Rules[index].outstate = OutOff;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 11;
//	pC->Rules[index].instate = InOne;
//	pC->Rules[index].outnum = CableOut_18;
//	pC->Rules[index].outstate = OutTog;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 11;
//	pC->Rules[index].instate = InTwo;
//	pC->Rules[index].outnum = CableOut_19;
//	pC->Rules[index].outstate = OutTog;
//	pC->Rules[index++].time = 0;
//	
//	pC->Rules[index].active = On;
//	pC->Rules[index].innum = 11;
//	pC->Rules[index].instate = InTwo;
//	pC->Rules[index].outnum = CableOut_18;
//	pC->Rules[index].outstate = OutOff;
//	pC->Rules[index++].time = 0;
}
static void Control_IWDGEnable(uint16_t time_ms)
{
	RCC->CSR |= RCC_CSR_LSION;
	while((RCC->CSR & RCC_CSR_LSIRDY) == 0){;}
  //Zezwolenie na zapis do rejestrów IWDG_PR oraz IWDG_RLR
  IWDG->KR = 0x5555;
  //Ustawienie taktowania, 32kHz/32 = 1024Hz
	IWDG->PR = 0x03;
  //Ustawienie wartosci poczatkowej
  IWDG->RLR = time_ms;
  //Przeladowanie wybranej konfiguracji
  IWDG->KR = 0xaaaa;
  //Wlaczenie watchdoga oraz sygnalu taktujacego LSI
  IWDG->KR = 0xcccc;
}
static void Control_TimConf(void)
{
	TIM6->PSC = 84-1;
	TIM6->ARR = 50000-1;	//Przerwanie co 50 ms
	TIM6->DIER |= TIM_DIER_UIE;
	TIM6->CR1 |= TIM_CR1_CEN;
	NVIC_EnableIRQ(TIM6_DAC_IRQn);
}
static void Control_BackupConf(void)
{
	PWR->CR = PWR_CR_DBP;
	PWR->CSR = PWR_CSR_BRE;
	while (!( PWR->CSR & PWR_CSR_BRR));
}
void Control_WriteBackup(void)
{
	volatile int32_t * const p2 = (int32_t *)BACKUPADDRESS;
	uint32_t index = 0;
	for(uint32_t i=0;i<MAXOUTPUT;i++)
	{
		*(p2 + index++) = (uint32_t)pC->Outputs[i].state;
	}
}
void Control_ReadBackup(void)
{
	volatile int32_t * const p2 = (int32_t *)BACKUPADDRESS;
	uint32_t index = 0;
	for(uint32_t i=0;i<MAXOUTPUT;i++)
	{
		pC->Outputs[i].state = (eOutstate)(*(p2 + index++));
	}
}
void Control_WriteRulesToEeprom(void)
{
	for(uint16_t i=0;i<NB_OF_VAR;i++)
		VirtAddVarTab[i] = i;
	
	uint32_t index = 0;
	VarDataTab[index++] = pC->rulecount;
	for(uint16_t i=0;i<pC->rulecount;i++)
	{
		VarDataTab[index++] = pC->Rules[i].innum;
		VarDataTab[index++] = pC->Rules[i].instate;
		VarDataTab[index++] = pC->Rules[i].outnum;
		VarDataTab[index++] = pC->Rules[i].outstate;
		VarDataTab[index++] = pC->Rules[i].enablenum;
		VarDataTab[index++] = pC->Rules[i].time_h;
		VarDataTab[index++] = pC->Rules[i].time_m;
		VarDataTab[index++] = pC->Rules[i].time_s;
	}
	
	for(uint16_t i=0;i<index;i++)
	{
		Control_IWDGReload();
		EE_WriteVariable(VirtAddVarTab[i], VarDataTab[i]);
	}
}
void Control_ReadRulesFromEeprom(void)
{
	for(uint16_t i=0;i<NB_OF_VAR;i++)
		VirtAddVarTab[i] = i;
	
	EE_ReadVariable(VirtAddVarTab[0], &VarDataTab[0]);
	pC->rulecount = VarDataTab[0];
	uint32_t index = 1;
	for(uint16_t i=0;i<8*pC->rulecount;i++)
	{
		Control_IWDGReload();
		EE_ReadVariable(VirtAddVarTab[index], &VarDataTab[index]);
		index++;
	}
	index = 1;
	for(uint16_t i=0;i<pC->rulecount;i++)
	{
		pC->Rules[i].innum = VarDataTab[index++];
		pC->Rules[i].instate = (eInstate)(VarDataTab[index++]);
		pC->Rules[i].outnum = VarDataTab[index++];
		pC->Rules[i].outstate = (eOutstate)(VarDataTab[index++]);
		pC->Rules[i].enablenum = VarDataTab[index++];
		pC->Rules[i].time_h = VarDataTab[index++];
		pC->Rules[i].time_m = VarDataTab[index++];
		pC->Rules[i].time_s = VarDataTab[index++];
		
		pC->Rules[i].time = 1000*(3600*(uint32_t)pC->Rules[i].time_h + 60*(uint32_t)pC->Rules[i].time_m + (uint32_t)pC->Rules[i].time_s);
	}
}
void Control_Conf(void)
{
	RCC_SystemInit();
	RCC_Conf();
	SysTick_Config(168000);
	Led_Conf();
	Control_TimConf();
	Control_BackupConf();
	Control_StructConf();
	Control_ReadBackup();
	Control_IWDGEnable(100);
}
void delay_ms(uint32_t ms)
{
	pC->Status.times = 0;
	while(pC->Status.times < ms);
}
void Control_IWDGReload(void)
{
	IWDG->KR = 0xaaaa;
}
void SysTick_Handler(void)
{
	Control_IWDGReload();
	pC->Status.times++;
	pC->Com.timeout++;
	if(pC->Com.timeout >= COMTIMEMAX)
	{
		ClearStr(pC->Com.bufread, BUF_LEN);
		DMA2_Stream2->CR 		&= ~DMA_SxCR_EN;
		DMA2->LIFCR 				|= DMA_LIFCR_CTCIF2;
		DMA2_Stream2->CR 		|= DMA_SxCR_EN;
		DMA2_Stream1->CR 		&= ~DMA_SxCR_EN;
		DMA2->LIFCR 				|= DMA_LIFCR_CTCIF1;
		DMA2_Stream1->CR 		|= DMA_SxCR_EN;
		pC->Com.timeout = 0;
	}
	
	for(uint8_t i=0;i<MAXINPUT;i++)
	{
		if(pC->Inputs[i].tempstate == InTempOn)
			pC->Inputs[i].timeon++;
		if(pC->Inputs[i].tempstate == InTempOff)
			pC->Inputs[i].timeoff++;
	}
	for(uint8_t i=0;i<MAXOUTPUT;i++)
		if(pC->Outputs[i].state == OutTime)
			pC->Outputs[i].time++;
		else
			pC->Outputs[i].time = 0;
}
void TIM6_DAC_IRQHandler(void)
{
	if((TIM6->SR & TIM_SR_UIF) != RESET)
	{
		Control_WriteBackup();
		Outputs_Write();
		TIM6->SR &= ~TIM_SR_UIF;
	}
}

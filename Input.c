#include "Input.h"
extern sControl* pC;
void Inputs_Conf(void)
{
	//MUX1A, MUX1B, MUX1C
	GPIOB->MODER |= GPIO_MODER_MODER11_0 | GPIO_MODER_MODER10_0;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD11_1 | GPIO_PUPDR_PUPD10_1;
	GPIOE->MODER |= GPIO_MODER_MODER15_0;
	GPIOE->PUPDR |= GPIO_PUPDR_PUPD15_1;
	
	//S11, S12, S13, S14
	GPIOE->PUPDR |= GPIO_PUPDR_PUPD14_0 | GPIO_PUPDR_PUPD13_0 | GPIO_PUPDR_PUPD12_0 | GPIO_PUPDR_PUPD11_0;
	//S21, S22, S23, S24
	GPIOE->PUPDR |= GPIO_PUPDR_PUPD10_0 | GPIO_PUPDR_PUPD9_0 | GPIO_PUPDR_PUPD8_0 | GPIO_PUPDR_PUPD7_0;
	//S31, S32, S33, S34
	GPIOB->PUPDR |= GPIO_PUPDR_PUPD2_0 | GPIO_PUPDR_PUPD1_0 | GPIO_PUPDR_PUPD0_0;
	GPIOC->PUPDR |= GPIO_PUPDR_PUPD5_0;
	//S41, S42, S43, S44
	GPIOC->PUPDR |= GPIO_PUPDR_PUPD4_0;
	GPIOA->PUPDR |= GPIO_PUPDR_PUPD7_0 | GPIO_PUPDR_PUPD6_0 | GPIO_PUPDR_PUPD5_0;
	
	TIM7->PSC = 840-1;
	TIM7->ARR = 5000-1;	//Przerwanie co 5 ms
	TIM7->DIER |= TIM_DIER_UIE;
	TIM7->CR1 |= TIM_CR1_CEN;
	NVIC_EnableIRQ(TIM7_IRQn);
}
void Inputs_SelectChannel(uint8_t ch)
{
	switch (ch)
	{
		case 6:	{MUX1C_HIGH; MUX1B_HIGH; MUX1A_HIGH; break;}
		case 5:	{MUX1C_HIGH; MUX1B_HIGH; MUX1A_LOW; break;}
		case 4:	{MUX1C_HIGH; MUX1B_LOW; MUX1A_HIGH; break;}
		case 7:	{MUX1C_HIGH; MUX1B_LOW; MUX1A_LOW; break;}
		case 0:	{MUX1C_LOW; MUX1B_HIGH; MUX1A_HIGH; break;}
		case 3:	{MUX1C_LOW; MUX1B_HIGH; MUX1A_LOW; break;}
		case 1:	{MUX1C_LOW; MUX1B_LOW; MUX1A_HIGH; break;}
		case 2:	{MUX1C_LOW; MUX1B_LOW; MUX1A_LOW; break;}
		default: break;
	}
}
static void Inputs_ChangeTempState(uint8_t num, eIntempstate tempstate)
{
	if(pC->Inputs[num].forced == On)
		return;
	
	pC->Inputs[num].tempstateprev = pC->Inputs[num].tempstate;
	pC->Inputs[num].tempstate = tempstate;
	
	if(pC->Inputs[num].tempstateprev == InTempOff && pC->Inputs[num].tempstate == InTempOn)
	{
		pC->Inputs[num].numclick++;
		pC->Inputs[num].timeoff = 0;
	}
	else if(pC->Inputs[num].tempstate == InTempOff)
	{
		pC->Inputs[num].timeon = 0;
	}
	
	if(pC->Inputs[num].timeoff >= INPUTTIMEOFFMAX)
	{
		pC->Inputs[num].timeoff = 0;
		if(pC->Inputs[num].numclick == 1)
		{
			pC->Inputs[num].state = InOne;
			pC->Inputs[num].numclick = 0;
		}
		else if(pC->Inputs[num].numclick >= 2)
		{
			pC->Inputs[num].state = InTwo;
			pC->Inputs[num].numclick = 0;
		}
	}
	
	if(pC->Inputs[num].timeon >= INPUTTIMEONMAX)
	{
		pC->Inputs[num].timeon = 0;
		pC->Inputs[num].state = InLong;
		pC->Inputs[num].numclick = 0;
	}
}
static void Inputs_CatchInputsFromAllMuxes(uint8_t ch)
{
	if((INS11_PORT->IDR & INS11_IDRPIN) == RESET)		Inputs_ChangeTempState(0+ch+8*3, InTempOn);
	else																						Inputs_ChangeTempState(0+ch+8*3, InTempOff);
	if((INS12_PORT->IDR & INS12_IDRPIN) == RESET)		Inputs_ChangeTempState(0+ch+8*2, InTempOn);
	else																						Inputs_ChangeTempState(0+ch+8*2, InTempOff);
	if((INS13_PORT->IDR & INS13_IDRPIN) == RESET)		Inputs_ChangeTempState(0+ch+8*1, InTempOn);
	else																						Inputs_ChangeTempState(0+ch+8*1, InTempOff);
	if((INS14_PORT->IDR & INS14_IDRPIN) == RESET)		Inputs_ChangeTempState(0+ch+8*0, InTempOn);
	else																						Inputs_ChangeTempState(0+ch+8*0, InTempOff);
	
	if((INS21_PORT->IDR & INS21_IDRPIN) == RESET)		Inputs_ChangeTempState(32+ch+8*3, InTempOn);
	else																						Inputs_ChangeTempState(32+ch+8*3, InTempOff);
	if((INS22_PORT->IDR & INS22_IDRPIN) == RESET)		Inputs_ChangeTempState(32+ch+8*2, InTempOn);
	else																						Inputs_ChangeTempState(32+ch+8*2, InTempOff);
	if((INS23_PORT->IDR & INS23_IDRPIN) == RESET)		Inputs_ChangeTempState(32+ch+8*1, InTempOn);
	else																						Inputs_ChangeTempState(32+ch+8*1, InTempOff);
	if((INS24_PORT->IDR & INS24_IDRPIN) == RESET)		Inputs_ChangeTempState(32+ch+8*0, InTempOn);
	else																						Inputs_ChangeTempState(32+ch+8*0, InTempOff);
	
	if((INS31_PORT->IDR & INS31_IDRPIN) == RESET)		Inputs_ChangeTempState(64+ch+8*3, InTempOn);
	else																						Inputs_ChangeTempState(64+ch+8*3, InTempOff);
	if((INS32_PORT->IDR & INS32_IDRPIN) == RESET)		Inputs_ChangeTempState(64+ch+8*2, InTempOn);
	else																						Inputs_ChangeTempState(64+ch+8*2, InTempOff);
	if((INS33_PORT->IDR & INS33_IDRPIN) == RESET)		Inputs_ChangeTempState(64+ch+8*1, InTempOn);
	else																						Inputs_ChangeTempState(64+ch+8*1, InTempOff);
	if((INS34_PORT->IDR & INS34_IDRPIN) == RESET)		Inputs_ChangeTempState(64+ch+8*0, InTempOn);
	else																						Inputs_ChangeTempState(64+ch+8*0, InTempOff);
	
	if((INS41_PORT->IDR & INS41_IDRPIN) == RESET)		Inputs_ChangeTempState(96+ch+8*3, InTempOn);
	else																						Inputs_ChangeTempState(96+ch+8*3, InTempOff);
	if((INS42_PORT->IDR & INS42_IDRPIN) == RESET)		Inputs_ChangeTempState(96+ch+8*2, InTempOn);
	else																						Inputs_ChangeTempState(96+ch+8*2, InTempOff);
	if((INS43_PORT->IDR & INS43_IDRPIN) == RESET)		Inputs_ChangeTempState(96+ch+8*1, InTempOn);
	else																						Inputs_ChangeTempState(96+ch+8*1, InTempOff);
	if((INS44_PORT->IDR & INS44_IDRPIN) == RESET)		Inputs_ChangeTempState(96+ch+8*0, InTempOn);
	else																						Inputs_ChangeTempState(96+ch+8*0, InTempOff);
}
static void Inputs_ReadInputs(void)
{
	for(uint8_t i=0;i<8;i++)
	{
		for(uint32_t j=0;j<2000;j++)
			Inputs_SelectChannel(i);
		Inputs_CatchInputsFromAllMuxes(i);
	}
}
void TIM7_IRQHandler(void)
{
	if((TIM7->SR & TIM_SR_UIF) != RESET)
	{
		Inputs_ReadInputs();
		TIM7->SR &= ~TIM_SR_UIF;
	}
}

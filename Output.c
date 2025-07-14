#include "Output.h"
extern sControl* pC;
void Outputs_AllSlavesHigh(void)
{
	OUTCS0_HIGH;
	OUTCS1_HIGH;
	OUTCS2_HIGH;
	OUTCS3_HIGH;
	OUTCS4_HIGH;
	OUTCS5_HIGH;
	OUTCS6_HIGH;
	OUTCS7_HIGH;
	OUTCS8_HIGH;
	OUTCS9_HIGH;
}
uint8_t Outputs_SendByte(uint8_t data)
{
	while (!(SPI3->SR & SPI_SR_TXE)){;}
	SPI3->DR = data;
	while ((SPI3->SR & SPI_SR_BSY)){;}
	while (!(SPI3->SR & SPI_SR_RXNE)){;}
	return SPI3->DR;
}
void Outputs_SelectSlave(uint8_t n)
{
	Outputs_AllSlavesHigh();
	switch(n)
	{
		case 0:		OUTCS0_LOW;	break;
		case 1:		OUTCS1_LOW;	break;
		case 2:		OUTCS2_LOW;	break;
		case 3:		OUTCS3_LOW;	break;
		case 4:		OUTCS4_LOW;	break;
		case 5:		OUTCS5_LOW;	break;
		case 6:		OUTCS6_LOW;	break;
		case 7:		OUTCS7_LOW;	break;
		case 8:		OUTCS8_LOW;	break;
		case 9:		OUTCS9_LOW;	break;
		default:							break;
	}
}
static void Outputs_WriteToOneRegOfOneSlave(uint8_t slave, uint8_t reg, uint8_t value)
{
	Outputs_SelectSlave(slave);
	Outputs_SendByte(0x40);
	Outputs_SendByte(reg);
	Outputs_SendByte(value);
	Outputs_AllSlavesHigh();
}
void Outputs_Conf(void)
{
	GPIOD->MODER |= GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0 | 
									GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0;
	GPIOD->PUPDR |= GPIO_PUPDR_PUPDR0_0 | GPIO_PUPDR_PUPDR1_0 | GPIO_PUPDR_PUPDR2_0 | GPIO_PUPDR_PUPDR3_0 |
									GPIO_PUPDR_PUPDR4_0 | GPIO_PUPDR_PUPDR5_0 | GPIO_PUPDR_PUPDR6_0 | GPIO_PUPDR_PUPDR7_0;
	GPIOB->MODER |= GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0;
	GPIOB->PUPDR |= GPIO_PUPDR_PUPDR3_0 | GPIO_PUPDR_PUPDR4_0;
	Outputs_AllSlavesHigh();
	
	GPIOC->MODER		|= GPIO_MODER_MODER10_1 | GPIO_MODER_MODER11_1 | GPIO_MODER_MODER12_1;
	GPIOC->OSPEEDR	|= GPIO_OSPEEDER_OSPEEDR10_1 | GPIO_OSPEEDER_OSPEEDR11_1 | GPIO_OSPEEDER_OSPEEDR12_1;
	GPIOC->PUPDR		|= GPIO_PUPDR_PUPDR10_0 | GPIO_PUPDR_PUPDR11_0 | GPIO_PUPDR_PUPDR12_0;
	GPIOC->AFR[1]		|= 0x00066600;
	SPI3->CR1				|= SPI_CR1_BR_1 | SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_MSTR | SPI_CR1_SPE;
	
	for(uint8_t i=0;i<(MAXOUTPUT/8);i++)
		Outputs_WriteToOneRegOfOneSlave(i, MCP_IODIR, 0x00);
}
static void Outputs_ChangeState(void)
{
	for(uint8_t i=0;i<MAXOUTPUT;i++)
	{
		switch(pC->Outputs[i].newstate)
		{
			case OutNull:
				break;
			case OutOn:
				pC->Outputs[i].state = OutOn;
				break;
			case OutOff:
				pC->Outputs[i].state = OutOff;
				break;
			case OutTog:
				if(pC->Outputs[i].state == OutOn || pC->Outputs[i].state == OutTime)
					pC->Outputs[i].state = OutOff;
				else if(pC->Outputs[i].state == OutOff)
					pC->Outputs[i].state = OutOn;
				break;
			case OutTime:
				pC->Outputs[i].state = OutTime;
				break;
			}
		if(pC->Outputs[i].state == OutTime)
			if(pC->Outputs[i].time >= pC->Outputs[i].timemax)
				pC->Outputs[i].state = OutOff;
			
		pC->Outputs[i].newstate = OutNull;
	}
}
void Outputs_Write(void)
{
	Outputs_ChangeState();
	uint8_t value = 0;
	for(int i=0;i<(MAXOUTPUT/8);i++)
	{
		value = 0;
		for(int j=7;j>=0;j--)
		{
			value = value<<1;
			if(pC->Outputs[8*i+j].state == OutOn || pC->Outputs[8*i+j].state == OutTime)
				value += 0;	//Wlaczenie swiatla
			else if(pC->Outputs[8*i+j].state == OutOff)
				value += 1;	//Wylaczenie swiatla
		}
		Outputs_WriteToOneRegOfOneSlave(i, MCP_OLAT, value);
	}
}
void Rule_Act(void)
{
	sRule r;
	for(int i=0;i<pC->rulecount;i++)
	{
		if(pC->Rules[i].enablenum >= 64)
		{
			pC->Rules[i].enable = On;
		}
		else
		{
			if(pC->Inputs[pC->Rules[i].enablenum].tempstate == InTempOn)
			{
				pC->Rules[i].enable = On;
			}
			else
			{
				pC->Rules[i].enable = Off;
			}
		}
	}
	
	for(uint16_t i=0;i<pC->rulecount;i++)
	{
		r = pC->Rules[i];
		if(r.enable == On)
		{
			if(r.instate == InForce0 && pC->Inputs[r.innum].tempstate == InTempOff)
			{
				pC->Outputs[r.outnum].newstate = r.outstate;
				pC->Outputs[r.outnum].timemax = r.time;
			}
			else if(r.instate == InForce1 && pC->Inputs[r.innum].tempstate == InTempOn)
			{
				pC->Outputs[r.outnum].newstate = r.outstate;
				pC->Outputs[r.outnum].timemax = r.time;
			}
			else if(pC->Inputs[r.innum].state == r.instate)
			{
				pC->Outputs[r.outnum].newstate = r.outstate;
				pC->Outputs[r.outnum].timemax = r.time;
			}
		}
	}
	for(uint16_t i=0;i<MAXINPUT;i++)
		if(pC->Inputs[i].forced == Off)
			pC->Inputs[i].state = InOff;
}

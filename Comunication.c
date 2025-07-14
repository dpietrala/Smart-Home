#include "Comunication.h"
extern sControl* pC;
void COM_Conf(void)
{
	DMA2_Stream2->PAR 	= (uint32_t)&USART1->DR;
  DMA2_Stream2->M0AR 	= (uint32_t)pC->Com.bufread;
  DMA2_Stream2->NDTR 	= (uint16_t)BUF_LEN;
  DMA2_Stream2->CR 		|= DMA_SxCR_PL | DMA_SxCR_MINC | DMA_SxCR_CHSEL_2 | DMA_SxCR_EN;
  	
  DMA2_Stream7->PAR 	= (uint32_t)&USART1->DR;
  DMA2_Stream7->M0AR 	= (uint32_t)pC->Com.bufwrite;
  DMA2_Stream7->NDTR 	= (uint16_t)BUF_LEN;
  DMA2_Stream7->CR 		|= DMA_SxCR_PL | DMA_SxCR_MINC | DMA_SxCR_DIR_0 | DMA_SxCR_CHSEL_0;
  	
  GPIOA->MODER |= GPIO_MODER_MODE9_1 | GPIO_MODER_MODE10_1;
  GPIOA->PUPDR |= GPIO_PUPDR_PUPD9_0 | GPIO_PUPDR_PUPD10_0;
  GPIOA->AFR[1] |= 0x00000770;
  USART1->BRR = 84000000/115200;
  USART1->CR3 |= USART_CR3_DMAR | USART_CR3_DMAT;
  USART1->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_IDLEIE | USART_CR1_UE;
	NVIC_EnableIRQ(USART1_IRQn);
	
	
	DMA2_Stream1->PAR 	= (uint32_t)&USART6->DR;
  DMA2_Stream1->M0AR 	= (uint32_t)pC->Com.bufread;
  DMA2_Stream1->NDTR 	= (uint16_t)BUF_LEN;
  DMA2_Stream1->CR 		|= DMA_SxCR_PL | DMA_SxCR_MINC | DMA_SxCR_CHSEL_2 | DMA_SxCR_CHSEL_0 | DMA_SxCR_EN;
  	
  DMA2_Stream6->PAR 	= (uint32_t)&USART6->DR;
  DMA2_Stream6->M0AR 	= (uint32_t)pC->Com.bufwrite;
  DMA2_Stream6->NDTR 	= (uint16_t)BUF_LEN;
  DMA2_Stream6->CR 		|= DMA_SxCR_PL | DMA_SxCR_MINC | DMA_SxCR_DIR_0 | DMA_SxCR_CHSEL_2 | DMA_SxCR_CHSEL_0;
  	
  GPIOC->MODER |= GPIO_MODER_MODE6_1 | GPIO_MODER_MODE7_1;
  GPIOC->PUPDR |= GPIO_PUPDR_PUPD6_0 | GPIO_PUPDR_PUPD7_0;
  GPIOC->AFR[0] |= 0x88000000;
  USART6->BRR = 84000000/115200;
  USART6->CR3 |= USART_CR3_DMAR | USART_CR3_DMAT;
  USART6->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_IDLEIE | USART_CR1_UE;
	NVIC_EnableIRQ(USART6_IRQn);
}
void ClearStr(uint8_t* str, uint32_t l)
{
	for(uint32_t i=0;i<l;i++)
		str[i] = 0;
}
uint16_t crc16(uint8_t* packet, uint32_t nBytes)
{
	uint16_t crc = 0;
	for(uint32_t byte = 0; byte < nBytes; byte++)
	{
		crc = crc ^ ((uint16_t)packet[byte] << 8);
		for (uint8_t bit = 0; bit < 8; bit++)
			if(crc & 0x8000) 	crc = (crc << 1) ^ 0x1021;
			else							crc = crc << 1;
	}
	return crc;
}
static void COM_ReadSetOutputs(uint8_t *buf)
{
	uint16_t crc1 = crc16(buf, 6);
	uint16_t crc2 = (uint16_t)(buf[6]<<8) + (uint16_t)buf[7];
	if(crc1 == crc2)
	{
		uint16_t num = buf[2];
		eOutstate newstate = (eOutstate)buf[3];
		uint32_t time = 1000 * (((uint16_t)buf[4]<<8) + ((uint16_t)buf[5]<<0));
		pC->Outputs[num].newstate = newstate;
		pC->Outputs[num].timemax = time;
	}
	ClearStr(buf, BUF_LEN);
	DMA2_Stream2->CR 		&= ~DMA_SxCR_EN;
	DMA2->LIFCR 				|= DMA_LIFCR_CTCIF2;
  DMA2_Stream2->CR 		|= DMA_SxCR_EN;
	
	DMA2_Stream1->CR 		&= ~DMA_SxCR_EN;
	DMA2->LIFCR 				|= DMA_LIFCR_CTCIF1;
  DMA2_Stream1->CR 		|= DMA_SxCR_EN;
	
	pC->Com.timeout = 0;
}
static void COM_ReadSetInputs(uint8_t *buf)
{
	uint16_t crc1 = crc16(buf, 4);
	uint16_t crc2 = (uint16_t)(buf[4]<<8) + (uint16_t)buf[5];
	if(crc1 == crc2)
	{
		uint16_t num = buf[2];
		eInstate newstate = (eInstate)buf[3];
		if(newstate == InForce0)
		{
			pC->Inputs[num].state = InForce0;
			pC->Inputs[num].tempstate = InTempOff;
			pC->Inputs[num].forced = On;
		}
		else if(newstate == InForce1)
		{
			pC->Inputs[num].state = InForce1;
			pC->Inputs[num].tempstate = InTempOn;
			pC->Inputs[num].forced = On;
		}
		else if(newstate == InReleaseForces)
		{
			pC->Inputs[num].state = InOff;
			pC->Inputs[num].forced = Off;
		}
		else
		{
			pC->Inputs[num].state = newstate;
			pC->Inputs[num].forced = Off;
		}
	}
	ClearStr(buf, BUF_LEN);
	DMA2_Stream2->CR 		&= ~DMA_SxCR_EN;
	DMA2->LIFCR 				|= DMA_LIFCR_CTCIF2;
  DMA2_Stream2->CR 		|= DMA_SxCR_EN;
	
	DMA2_Stream1->CR 		&= ~DMA_SxCR_EN;
	DMA2->LIFCR 				|= DMA_LIFCR_CTCIF1;
  DMA2_Stream1->CR 		|= DMA_SxCR_EN;
	
	pC->Com.timeout = 0;
}
static void COM_ReadNewRules(uint8_t *buf)
{
	uint16_t numrules = ((uint16_t)buf[2]<<8) + (uint16_t)buf[3];
	uint32_t numbytes = 8 * numrules + 4;
	uint16_t crc1 = crc16(buf, numbytes);
	uint16_t crc2 = ((uint16_t)buf[numbytes]<<8) + (uint16_t)buf[numbytes+1];
	if(crc1 == crc2)
	{
		if(numrules > MAXRULES)
			numrules = MAXRULES;
		pC->rulecount = numrules;
		for(uint16_t i=0;i<pC->rulecount;i++)
		{
			pC->Rules[i].innum = buf[8*i+4+0];
			pC->Rules[i].instate = (eInstate)(buf[8*i+4+1] + 2);
			pC->Rules[i].outnum = buf[8*i+4+2];
			pC->Rules[i].outstate = (eOutstate)(buf[8*i+4+3] + 1);
			pC->Rules[i].enablenum = buf[8*i+4+4];
			pC->Rules[i].time_h = buf[8*i+4+5];
			pC->Rules[i].time_m = buf[8*i+4+6];
			pC->Rules[i].time_s = buf[8*i+4+7];
			pC->Rules[i].time = 1000*(3600*(uint32_t)pC->Rules[i].time_h + 60*(uint32_t)pC->Rules[i].time_m + (uint32_t)pC->Rules[i].time_s);
		}
		Control_WriteRulesToEeprom();
		LED2_TOG;
		
		ClearStr(buf, BUF_LEN);
		DMA2_Stream2->CR 		&= ~DMA_SxCR_EN;
		DMA2->LIFCR 				|= DMA_LIFCR_CTCIF2;
		DMA2_Stream2->CR 		|= DMA_SxCR_EN;
		
		DMA2_Stream1->CR 		&= ~DMA_SxCR_EN;
		DMA2->LIFCR 				|= DMA_LIFCR_CTCIF1;
		DMA2_Stream1->CR 		|= DMA_SxCR_EN;
		
		pC->Com.timeout = 0;
		pC->Com.recnewrules = On;
	}
}
static void COM_SendOutputs(void)
{
	uint8_t* buf = pC->Com.bufwrite;
	ClearStr(buf, BUF_LEN);
	uint8_t index = 0;
	buf[index++] = Frametype_Header;
	buf[index++] = Frametype_SendOutputs;
	buf[index++] = MAXOUTPUT;
	for(uint8_t i=0;i<MAXOUTPUT;i++)
		buf[index++] = pC->Outputs[i].state;
	uint16_t crc = crc16(buf, MAXOUTPUT+3);
	buf[index++] = crc>>8;
	buf[index++] = crc>>0;
	
	DMA2_Stream7->CR &= ~DMA_SxCR_EN;
	DMA2->HIFCR |= DMA_HIFCR_CTCIF7;
	DMA2_Stream7->NDTR 	= (uint16_t)index;
	DMA2_Stream7->CR |= DMA_SxCR_EN;
	
	DMA2_Stream6->CR &= ~DMA_SxCR_EN;
	DMA2->HIFCR |= DMA_HIFCR_CTCIF6;
	DMA2_Stream6->NDTR 	= (uint16_t)index;
	DMA2_Stream6->CR |= DMA_SxCR_EN;
}
static void COM_SendNewRulesConfirmation(void)
{
	uint8_t* buf = pC->Com.bufwrite;
	ClearStr(buf, BUF_LEN);
	buf[0] = Frametype_Header;
	buf[1] = Frametype_NewRulesConfirmation;
	uint16_t crc = crc16(buf, 2);
	buf[2] = crc>>8;
	buf[3] = crc>>0;
	
	DMA2_Stream7->CR &= ~DMA_SxCR_EN;
	DMA2->HIFCR |= DMA_HIFCR_CTCIF7;
	DMA2_Stream7->NDTR 	= (uint16_t)4;
	DMA2_Stream7->CR |= DMA_SxCR_EN;
	
	DMA2_Stream6->CR &= ~DMA_SxCR_EN;
	DMA2->HIFCR |= DMA_HIFCR_CTCIF6;
	DMA2_Stream6->NDTR 	= (uint16_t)4;
	DMA2_Stream6->CR |= DMA_SxCR_EN;
}
void COM_SendToHost(void)
{
	if(pC->Com.recnewrules == On)
	{
		COM_SendNewRulesConfirmation();
		pC->Com.recnewrules = Off;
	}
	else
	{
		COM_SendOutputs();
	}
}
static void COM_ReadFromHost(void)
{
	LED4_TOG;
	uint8_t* buf = pC->Com.bufread;
	if(buf[0] == Frametype_Header && buf[1] == Frametype_ReadSetOutputs)
		COM_ReadSetOutputs(buf);
	else if(buf[0] == Frametype_Header && buf[1] == Frametype_ReadSetInputs)
		COM_ReadSetInputs(buf);
	else if(buf[0] == Frametype_Header && buf[1] == Frametype_ReadNewRules)
		COM_ReadNewRules(buf);
	else if(buf[0] == Frametype_Header && buf[1] == Frametype_GetOutputs)
		COM_SendOutputs();
}
//----------- Przerwania --------------------------------------------
void USART1_IRQHandler(void)
{
	if((USART1->SR & USART_SR_IDLE) != RESET) 
	{
		char c = USART1->DR;
		COM_ReadFromHost();
	}
}
void USART6_IRQHandler(void)
{
	if((USART6->SR & USART_SR_IDLE) != RESET) 
	{
		char c = USART6->DR;
		COM_ReadFromHost();
	}
}

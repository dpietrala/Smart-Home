#ifndef	_CONTROL
#define _CONTROL

typedef enum {Off =	0, On =	1} eOnOff;
typedef enum {InTempOff =	0, InTempOn = 1} eIntempstate;
typedef enum {InNull=0, InOff=1, InOne=2, InTwo=3, InLong=4, InForce0 = 5, InForce1 = 6, InReleaseForces = 7} eInstate;
typedef enum {OutNull = 0, OutOff=1, OutOn=2, OutTog=3, OutTime=4} eOutstate;
typedef enum 
{
	Frametype_null = 0,
	Frametype_ReadSetOutputs = 1,
	Frametype_ReadSetInputs = 2,
	Frametype_SendOutputs = 3,
	Frametype_ReadNewRules = 4,
	Frametype_GetOutputs = 5,
	Frametype_NewRulesConfirmation = 6,
	Frametype_Header = 155,
}eFrameType;

#include "stm32f4xx.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Input.h"
#include "Output.h"
#include "Comunication.h"
#include "eeprom.h"

#define BACKUPADDRESS			0x40024008
#define MAXINPUT					128
#define MAXOUTPUT					80
#define MAXRULES					990
#define BUF_LEN						9000
#define COMTIMEMAX				3000
#define INPUTTIMEOFFMAX		200
#define INPUTTIMEONMAX		600

typedef struct
{
	uint32_t	times;
	eOnOff		work;
}sStatus;
typedef struct
{
	uint8_t		bufread[BUF_LEN];
	uint8_t		bufwrite[BUF_LEN];
	uint32_t	timeout;
	eOnOff		recnewrules;
}sCom;
typedef struct
{
	eOnOff				forced;
	uint32_t			timeon;
	uint32_t			timeoff;
	uint8_t				numclick;
	eIntempstate	tempstate;
	eIntempstate	tempstateprev;
	eInstate			state;
}sInput;
typedef struct
{
	uint32_t			time;
	uint32_t			timemax;
	eOutstate 		state;
	eOutstate 		newstate;
}sOutput;
typedef struct
{
	eOnOff				enable;
	uint8_t				enablenum;
	uint8_t 			innum;
	eInstate			instate;
	uint8_t				outnum;
	eOutstate			outstate;
	uint8_t				time_h;
	uint8_t				time_m;
	uint8_t				time_s;
	uint32_t			time;
}sRule;
typedef struct
{
	sStatus		Status;
	sCom			Com;
	sInput		Inputs[MAXINPUT];
	sOutput		Outputs[MAXOUTPUT];
	sRule			Rules[MAXRULES];
	uint32_t	rulecount;
}sControl;

//Discovery
#define LED_PORT		GPIOD
#define LED1_PIN		GPIO_ODR_ODR_12
#define LED2_PIN		GPIO_ODR_ODR_13
#define LED3_PIN		GPIO_ODR_ODR_14
#define LED4_PIN		GPIO_ODR_ODR_15

//#define LED_PORT		GPIOE
//#define LED1_PIN		GPIO_ODR_ODR_3
//#define LED2_PIN		GPIO_ODR_ODR_2
//#define LED3_PIN		GPIO_ODR_ODR_5
//#define LED4_PIN		GPIO_ODR_ODR_4

#define LED1_ON			LED_PORT->ODR |= LED1_PIN;
#define LED1_OFF		LED_PORT->ODR &= ~LED1_PIN;
#define LED1_TOG		LED_PORT->ODR ^= LED1_PIN;
#define LED2_ON			LED_PORT->ODR |= LED2_PIN;
#define LED2_OFF		LED_PORT->ODR &= ~LED2_PIN;
#define LED2_TOG		LED_PORT->ODR ^= LED2_PIN;
#define LED3_ON			LED_PORT->ODR |= LED3_PIN;
#define LED3_OFF		LED_PORT->ODR &= ~LED3_PIN;
#define LED3_TOG		LED_PORT->ODR ^= LED3_PIN;
#define LED4_ON			LED_PORT->ODR |= LED4_PIN;
#define LED4_OFF		LED_PORT->ODR &= ~LED4_PIN;
#define LED4_TOG		LED_PORT->ODR ^= LED4_PIN;
#define LEDALL_ON		LED_PORT->ODR |= LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN;
#define LEDALL_OFF	LED_PORT->ODR &= ~LED1_PIN & ~LED2_PIN & ~LED3_PIN & ~LED4_PIN;
#define LEDALL_TOG	LED_PORT->ODR ^= LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN;


void Control_Conf(void);
void delay_ms(uint32_t ms);
void Control_WriteBackup(void);
void Control_ReadBackup(void);
void Control_IWDGReload(void);
void Control_WriteRulesToEeprom(void);
void Control_ReadRulesFromEeprom(void);

#endif

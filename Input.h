#ifndef _INPUT
#define _INPUT
#include "Control.h"

//------- Definiowanie makr do sterowania muxami ---------------------------------------------
#define MUX1A_LOW			GPIOB->ODR &= ~GPIO_ODR_ODR_11
#define MUX1A_HIGH		GPIOB->ODR |= GPIO_ODR_ODR_11
#define MUX1B_LOW			GPIOB->ODR &= ~GPIO_ODR_ODR_10
#define MUX1B_HIGH		GPIOB->ODR |= GPIO_ODR_ODR_10
#define MUX1C_LOW			GPIOE->ODR &= ~GPIO_ODR_ODR_15
#define MUX1C_HIGH		GPIOE->ODR |= GPIO_ODR_ODR_15

//------- Definiowanie makr do odczytu wejsc ---------------------------------------------
#define INS11_PORT		GPIOE
#define INS11_IDRPIN	GPIO_IDR_IDR_14
#define INS12_PORT		GPIOE
#define INS12_IDRPIN	GPIO_IDR_IDR_13
#define INS13_PORT		GPIOE
#define INS13_IDRPIN	GPIO_IDR_IDR_12
#define INS14_PORT		GPIOE
#define INS14_IDRPIN	GPIO_IDR_IDR_11

#define INS21_PORT		GPIOE
#define INS21_IDRPIN	GPIO_IDR_IDR_10
#define INS22_PORT		GPIOE
#define INS22_IDRPIN	GPIO_IDR_IDR_9
#define INS23_PORT		GPIOE
#define INS23_IDRPIN	GPIO_IDR_IDR_8
#define INS24_PORT		GPIOE
#define INS24_IDRPIN	GPIO_IDR_IDR_7

#define INS31_PORT		GPIOB
#define INS31_IDRPIN	GPIO_IDR_IDR_2
#define INS32_PORT		GPIOB
#define INS32_IDRPIN	GPIO_IDR_IDR_1
#define INS33_PORT		GPIOB
#define INS33_IDRPIN	GPIO_IDR_IDR_0
#define INS34_PORT		GPIOC
#define INS34_IDRPIN	GPIO_IDR_IDR_5

#define INS41_PORT		GPIOC
#define INS41_IDRPIN	GPIO_IDR_IDR_4
#define INS42_PORT		GPIOA
#define INS42_IDRPIN	GPIO_IDR_IDR_7
#define INS43_PORT		GPIOA
#define INS43_IDRPIN	GPIO_IDR_IDR_6
#define INS44_PORT		GPIOA
#define INS44_IDRPIN	GPIO_IDR_IDR_5

void Inputs_Conf(void);
void Inputs_SelectChannel(uint8_t ch);

#endif

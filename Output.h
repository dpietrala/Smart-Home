#ifndef _OUTPUT
#define _OUTPUT
#include "Control.h"

//------- Definiowanie makr do sterownia Chip select dla SPI2 ---------------------------------------------
#define OUTCS0_LOW	GPIOD->ODR &= ~GPIO_ODR_ODR_0
#define OUTCS0_HIGH	GPIOD->ODR |= GPIO_ODR_ODR_0
#define OUTCS1_LOW	GPIOD->ODR &= ~GPIO_ODR_ODR_1
#define OUTCS1_HIGH	GPIOD->ODR |= GPIO_ODR_ODR_1
#define OUTCS2_LOW	GPIOD->ODR &= ~GPIO_ODR_ODR_2
#define OUTCS2_HIGH	GPIOD->ODR |= GPIO_ODR_ODR_2
#define OUTCS3_LOW	GPIOD->ODR &= ~GPIO_ODR_ODR_3
#define OUTCS3_HIGH	GPIOD->ODR |= GPIO_ODR_ODR_3
#define OUTCS4_LOW	GPIOD->ODR &= ~GPIO_ODR_ODR_4
#define OUTCS4_HIGH	GPIOD->ODR |= GPIO_ODR_ODR_4
#define OUTCS5_LOW	GPIOD->ODR &= ~GPIO_ODR_ODR_5
#define OUTCS5_HIGH	GPIOD->ODR |= GPIO_ODR_ODR_5
#define OUTCS6_LOW	GPIOD->ODR &= ~GPIO_ODR_ODR_6
#define OUTCS6_HIGH	GPIOD->ODR |= GPIO_ODR_ODR_6
#define OUTCS7_LOW	GPIOD->ODR &= ~GPIO_ODR_ODR_7
#define OUTCS7_HIGH	GPIOD->ODR |= GPIO_ODR_ODR_7
#define OUTCS8_LOW	GPIOB->ODR &= ~GPIO_ODR_ODR_3
#define OUTCS8_HIGH	GPIOB->ODR |= GPIO_ODR_ODR_3
#define OUTCS9_LOW	GPIOB->ODR &= ~GPIO_ODR_ODR_4
#define OUTCS9_HIGH	GPIOB->ODR |= GPIO_ODR_ODR_4

//------- Definiowanie adresów rejestrów w ekspanderze MCP ---------------------------------------------
#define MCP_IODIR			0x00
#define MCP_IPOL			0x01
#define MCP_GPINTEN		0x02
#define MCP_DEFVAL		0x03
#define MCP_INTCON		0x04
#define MCP_IOCON			0x05
#define MCP_GPPU			0x06
#define MCP_INTF			0x07
#define MCP_INTCAP		0x08
#define MCP_GPIO			0x09
#define MCP_OLAT			0x0a

//------- Definiowanie gdzie sa podlaczone poszczególne numery wyjsc do poszczególnych numerów kabli --
//Out 0
#define CableOut_11 0
#define CableOut_12 1
#define CableOut_13 2
#define CableOut_14 3
#define CableOut_15 4
#define CableOut_16 5
#define CableOut_17 6
//#define CableOut_ 7
//Out 1
#define CableOut_18 8
#define CableOut_19 9
#define CableOut_20 10
#define CableOut_21 11
#define CableOut_22 12
#define CableOut_23 13
//#define CableOut_ 14
//#define CableOut_ 15
//Out 2
#define CableOut_71 16
#define CableOut_72 17
#define CableOut_73 18
#define CableOut_74 19
#define CableOut_75 20
#define CableOut_76 21
#define CableOut_77 22
//#define CableOut_ 23
//Out 3
#define CableOut_78 24
#define CableOut_79 25
#define CableOut_80 26
#define CableOut_81 27
#define CableOut_82 28
#define CableOut_83 29
#define CableOut_84 30
//#define CableOut_ 31
//Out 4
#define CableOut_32 32
#define CableOut_33 33
#define CableOut_34 34
#define CableOut_35 35
#define CableOut_36 36
#define CableOut_37 37
#define CableOut_38 38
#define CableOut_39 39
//Out 5
#define CableOut_40 40
#define CableOut_41 41
#define CableOut_65 42
#define CableOut_66 43
#define CableOut_67 44
#define CableOut_68 45
#define CableOut_69 46
#define CableOut_70 47
//Out 6
#define CableOut_42 48
#define CableOut_43 49
#define CableOut_44 50
#define CableOut_45 51
#define CableOut_46 52
#define CableOut_47 53
#define CableOut_48 54
#define CableOut_49 55
//Out 7
#define CableOut_50 56
#define CableOut_51 57
#define CableOut_52 58
#define CableOut_54 59
#define CableOut_55 60
#define CableOut_56 61
#define CableOut_57 62
//#define CableOut_ 63
//Out 8
#define CableOut_58 64
#define CableOut_59 65
#define CableOut_60 66
#define CableOut_61 67
#define CableOut_62 68
#define CableOut_63 69
#define CableOut_64 70
//#define CableOut_ 71
//Out 9
#define CableOut_86 72
#define CableOut_87 73
#define CableOut_88 74
#define CableOut_92 75
#define CableOut_93 76
#define CableOut_94 77
#define CableOut_95 78
#define CableOut_96 79



void Outputs_Conf(void);
void Outputs_Write(void);
uint8_t Outputs_SendByte(uint8_t data);
void Outputs_SelectSlave(uint8_t n);
void Outputs_AllSlavesHigh(void);
void Rule_Act(void);

#endif

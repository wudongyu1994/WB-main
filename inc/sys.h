#ifndef SYS_H
#define SYS_H

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <io430.h>
#include <in430.h>

//延时函数，IAR自带，经常使用到
#define CPU_F ((double)8000000)   //外部高频晶振8MHZ
//#define CPU_F ((double)32768)   //外部低频晶振32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0)) 

//自定义数据结构，方便使用
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long
#define u8 unsigned char
#define u16  unsigned int
#define u32 unsigned long

//***********************************************************************
//                   系统时钟初始化，外部8M晶振
//			时钟默认配置：SMCLK=MCLK=8Mhz，ACLK=32768Hz
//***********************************************************************
void Clock_Init(void);
//***********************************************************************
//                   系统时钟初始化，内部RC晶振
//***********************************************************************
void Clock_Init_Inc(void);
//***********************************************************************
//                   系统时钟初始化，外部32.768K晶振
//***********************************************************************
void Clock_Init_Ex32768(void);
//***********************************************************************
//               MSP430内部看门狗初始化
//***********************************************************************
void WDT_Init(void);

#endif












#ifndef SYS_H
#define SYS_H

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <io430.h>
#include <in430.h>

//��ʱ������IAR�Դ�������ʹ�õ�
#define CPU_F ((double)8000000)   //�ⲿ��Ƶ����8MHZ
//#define CPU_F ((double)32768)   //�ⲿ��Ƶ����32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0)) 
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0)) 

//�Զ������ݽṹ������ʹ��
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long
#define u8 unsigned char
#define u16  unsigned int
#define u32 unsigned long

//***********************************************************************
//                   ϵͳʱ�ӳ�ʼ�����ⲿ8M����
//			ʱ��Ĭ�����ã�SMCLK=MCLK=8Mhz��ACLK=32768Hz
//***********************************************************************
void Clock_Init(void);
//***********************************************************************
//                   ϵͳʱ�ӳ�ʼ�����ڲ�RC����
//***********************************************************************
void Clock_Init_Inc(void);
//***********************************************************************
//                   ϵͳʱ�ӳ�ʼ�����ⲿ32.768K����
//***********************************************************************
void Clock_Init_Ex32768(void);
//***********************************************************************
//               MSP430�ڲ����Ź���ʼ��
//***********************************************************************
void WDT_Init(void);

#endif












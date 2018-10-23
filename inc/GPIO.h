#ifndef GPIO_H
#define GPIO_H

#include "sys.h"

/*/KEY为P4.6口
#define KEY_DIR			P4DIR_bit.P6     //KEY 方向寄存器
#define KEY_IFG			P4IFG_bit.P6     //KEY 中断标志位寄存器
#define KEY_IES			P4IES_bit.P6     //KEY 触发方式寄存器，上升沿或下降沿
#define KEY_IE			P4IE_bit.P6      //KEY 中断使能寄存器
#define KEY_IN			P4IN_bit.P6      //KEY 输入寄存器*/

//KEY为P2.3口
#define KEY_DIR			P2DIR_bit.P4     //KEY 方向寄存器
#define KEY_IFG			P2IFG_bit.P4     //KEY 中断标志位寄存器
#define KEY_IES			P2IES_bit.P4     //KEY 触发方式寄存器，上升沿或下降沿
#define KEY_IE			P2IE_bit.P4      //KEY 中断使能寄存器
#define KEY_IN			P2IN_bit.P4      //KEY 输入寄存器*/
//GATE_STATE---P1.3
#define GATE_STATE_DIR	P1DIR_bit.P3	
#define GATE_STATE_IN		P1IN_bit.P3
#define GATE_STATE_IFG	P1IFG_bit.P3
#define GATE_STATE_IES	P1IES_bit.P3
#define GATE_STATE_IE		P1IE_bit.P3

//GATE_CTL---P1.2
#define GATE_CTL_DIR		P1DIR_bit.P2
#define GATE_CTL_OUT		P1OUT_bit.P2

#define LED_DIR 	P6DIR_bit.P1
#define LED_OUT 	P6OUT_bit.P1

void IO_Init(void);

#endif
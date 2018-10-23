#ifndef GPIO_H
#define GPIO_H

#include "sys.h"

/*/KEYΪP4.6��
#define KEY_DIR			P4DIR_bit.P6     //KEY ����Ĵ���
#define KEY_IFG			P4IFG_bit.P6     //KEY �жϱ�־λ�Ĵ���
#define KEY_IES			P4IES_bit.P6     //KEY ������ʽ�Ĵ����������ػ��½���
#define KEY_IE			P4IE_bit.P6      //KEY �ж�ʹ�ܼĴ���
#define KEY_IN			P4IN_bit.P6      //KEY ����Ĵ���*/

//KEYΪP2.3��
#define KEY_DIR			P2DIR_bit.P4     //KEY ����Ĵ���
#define KEY_IFG			P2IFG_bit.P4     //KEY �жϱ�־λ�Ĵ���
#define KEY_IES			P2IES_bit.P4     //KEY ������ʽ�Ĵ����������ػ��½���
#define KEY_IE			P2IE_bit.P4      //KEY �ж�ʹ�ܼĴ���
#define KEY_IN			P2IN_bit.P4      //KEY ����Ĵ���*/
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
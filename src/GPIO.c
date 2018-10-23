#include "GPIO.h"

void IO_Init(void)
{
	//��״̬��ʼ��
	GATE_STATE_DIR=0;		//0-input;1-output
	//�ſ��Ƴ�ʼ��
	GATE_CTL_DIR=1;			//0-input;1-output
	GATE_CTL_OUT=0;			//1-open gate;0---close Gate
	
	//�˹�������ʼ��
	KEY_DIR=0;	//0-input;1-output
	KEY_IFG=0;	//clear flag
	KEY_IES=1;	//trig side: 0-up;1-down
	KEY_IE=1;		//enable interrupt*/
	
	//��״̬����ʼ��
	GATE_STATE_DIR=0;	//0-input;1-output
	GATE_STATE_IFG=0;	//clear flag
	GATE_STATE_IES=1;	//trig side: 0-up;1-down
	GATE_STATE_IE=1;	//1-enable interrupt;0-unable
	
	LED_DIR=1;		//0-input;1-output
	LED_OUT=0;		//0-led OFF;1-led ON
}
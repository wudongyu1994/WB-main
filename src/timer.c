#include "timer.h"

void TimerAInit(void)
{
  	TACCTL0 = CCIE;					//Capture/compare interrupt enable.
    TACTL = TASSEL_1 + ID_3;         //ʱ��ԴACLK,8��Ƶ
    CCR0 = 20480;             //Ŀ��ֵ����TA0R�ﵽCCR0ʱ�����жϡ�TA��ʱ5s
    TACTL |= MC_0;           //STOPֹͣ��ʱ��
}
void TimerBInit(void)
{
  	TBCCTL0 = CCIE;					//Capture/compare interrupt enable.
    TBCTL = TBSSEL_1 + ID_3;         //ʱ��ԴACLK��8��Ƶ
    TBCCR0 = 40960;           //Ŀ��ֵ����TB0R�ﵽCCR0ʱ�����жϡ�TB��ʱ10s
    TBCTL |= MC_0;           //STOPֹͣ��ʱ��
}
void ON_TimerA(void)
{
  	TA0R =0;           			//�����ʱ��������
    TA0CTL |=  MC_1;    //������ʱ��ʱ����up to MODE��ʱ��ԴACLK
}
void OFF_TimerA(void)
{
	TACTL &= ~(MC1+MC0);
}
void ON_TimerB(void)
{
	TBR =0;           //�����ʱ��������
	TBCTL |=  MC_1;    //������ʱ��ʱ��
}
void OFF_TimerB(void)
{
	TBCTL &= ~(MC1+MC0);    //�رն�ʱ��
}
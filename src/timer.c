#include "timer.h"

void TimerAInit(void)
{
  	TACCTL0 = CCIE;					//Capture/compare interrupt enable.
    TACTL = TASSEL_1 + ID_3;         //时钟源ACLK,8分频
    CCR0 = 20480;             //目标值，当TA0R达到CCR0时产生中断。TA定时5s
    TACTL |= MC_0;           //STOP停止定时器
}
void TimerBInit(void)
{
  	TBCCTL0 = CCIE;					//Capture/compare interrupt enable.
    TBCTL = TBSSEL_1 + ID_3;         //时钟源ACLK，8分频
    TBCCR0 = 40960;           //目标值，当TB0R达到CCR0时产生中断。TB定时10s
    TBCTL |= MC_0;           //STOP停止定时器
}
void ON_TimerA(void)
{
  	TA0R =0;           			//清除定时器计数器
    TA0CTL |=  MC_1;    //开启超时定时器，up to MODE，时钟源ACLK
}
void OFF_TimerA(void)
{
	TACTL &= ~(MC1+MC0);
}
void ON_TimerB(void)
{
	TBR =0;           //清除定时器计数器
	TBCTL |=  MC_1;    //开启超时定时器
}
void OFF_TimerB(void)
{
	TBCTL &= ~(MC1+MC0);    //关闭定时器
}
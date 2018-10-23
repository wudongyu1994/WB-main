#include "adc.h"

void ADC_Init(void)
{
  /*P6SEL_bit.P6=1;		//ADC MODE
  ADC12CTL0|= ADC12ON + SHT0_2 + REF2_5V + REFON; //ADC电源控制开，16个CLK，内部基准2.5V
  ADC12CTL1|= ADC12SSEL_2 +SHP + CSTARTADD_0;            //SMCLK做时钟源
  ADC12MCTL0= SREF_1 + INCH_6;                     //参考控制位及通道选择，这里选择通道0
  //ADC12IE|= 0x01;                                 //中断允许
  //ADC12CTL0|= ENC;   */                             
  
  P6SEL_bit.P4=1;		//ADC MODE;battery
  P6SEL_bit.P6=1;		//ADC MODE;Liquid
  
  ADC12CTL0=ADC12ON+ SHT0_15+ MSC;	//打开ADC,设置采样时间，多次转换
  ADC12CTL1=SHP+ CONSEQ_1+ ADC12SSEL_3+ CSTARTADD_0;	//使用采样定时器;多通道单次转换;ADC内部时钟源;起始地址0
  ADC12MCTL0 |=INCH_4;				//通道选择
  ADC12MCTL1 |=INCH_6+EOS;			//最后一个通道需要标记EOS
  
}

void ADC_ON(void)
{
  ADC12CTL0 |=ENC+ADC12SC;			//使能转换;开始转换
  while(!(ADC12IFG & 0x0002));      //等待ADC12MCTL1结束转换
}
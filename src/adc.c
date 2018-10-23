#include "adc.h"

void ADC_Init(void)
{
  /*P6SEL_bit.P6=1;		//ADC MODE
  ADC12CTL0|= ADC12ON + SHT0_2 + REF2_5V + REFON; //ADC��Դ���ƿ���16��CLK���ڲ���׼2.5V
  ADC12CTL1|= ADC12SSEL_2 +SHP + CSTARTADD_0;            //SMCLK��ʱ��Դ
  ADC12MCTL0= SREF_1 + INCH_6;                     //�ο�����λ��ͨ��ѡ������ѡ��ͨ��0
  //ADC12IE|= 0x01;                                 //�ж�����
  //ADC12CTL0|= ENC;   */                             
  
  P6SEL_bit.P4=1;		//ADC MODE;battery
  P6SEL_bit.P6=1;		//ADC MODE;Liquid
  
  ADC12CTL0=ADC12ON+ SHT0_15+ MSC;	//��ADC,���ò���ʱ�䣬���ת��
  ADC12CTL1=SHP+ CONSEQ_1+ ADC12SSEL_3+ CSTARTADD_0;	//ʹ�ò�����ʱ��;��ͨ������ת��;ADC�ڲ�ʱ��Դ;��ʼ��ַ0
  ADC12MCTL0 |=INCH_4;				//ͨ��ѡ��
  ADC12MCTL1 |=INCH_6+EOS;			//���һ��ͨ����Ҫ���EOS
  
}

void ADC_ON(void)
{
  ADC12CTL0 |=ENC+ADC12SC;			//ʹ��ת��;��ʼת��
  while(!(ADC12IFG & 0x0002));      //�ȴ�ADC12MCTL1����ת��
}
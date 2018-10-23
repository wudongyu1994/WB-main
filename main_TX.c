/*################################################
�����߼��ĵ�

����ʱ���������ߣ�mcuҲ���ߡ�

һ������״̬�ĸı䣬���ϱ��������ݡ�5s��ʱ����---IO���жϣ����ȼ�0
�˹����°�������10s��ʱ�䣬��������ģʽ��5s��ʱ�رա�---IO���жϣ����ȼ�1
ÿ��5s�ϱ�һ�����ݡ�---��ʱ���жϣ����ȼ�2

�������ݰ�������״̬��Һλ��������

#1-��˯��ģʽ�������ģʽ��
	SX1276Write( REG_LR_HOPPERIOD,0x91 );//ѡһ���ò����ļĴ���������֤����Ȼ�޷�����
	LSD_RF_RXmode();
#2-����˯��ģʽ��
	LSD_RF_Sleepmode();
#3-��˯��ģʽ���뷢��ģʽ��
	G_LoRaConfig.PayloadLength = 12;	//ѡ���ͳ���
	LSD_RF_SendPacket(TXbuffer);			//ѡ��������
	LSD_RF_Sleepmode();								//���������̽���˯��ģʽ

################################################*/
#include "sys.h"
#include "led.h"
#include "GPIO.h"
#include "timer.h"
#include "adc.h"
#include "flash.h"
#include "sx1276-f149-Hal.h"
#include "LSD_RF_SX1276.h"
#include "LSD_RF_APPrf.h"

#define DEV_ID	0x89
#define NET_ID	0x02
//===================================�������===================================================
u8 liquid,power;
bool flag_rxmode=false;
extern S_LoRaConfig G_LoRaConfig;
u8 GateBuffer[12]={0x80,7,7,2,0x55,0x66,0x77,0x89,0,0,0x4B,0x81};	//state of the Gate
u8 LqdBuffer[12]={0x80,7,7,2,0x55,0x66,0x77,0x89,1,10,0xD6,0x81};	//liquid data
u8 PwrBuffer[12]={0x80,7,7,2,0x55,0x66,0x77,0x89,3,20,0x82,0x81};	//power data
uint8_t TXbuffer[30]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29};
uint8_t Rxbuffer[64];
//===================================��������===================================================
void mySend(u8 cmd,u8 flag,u8 data)
{
	u8 sum=0,i;
	
	TXbuffer[0]=0x80;
	TXbuffer[1]=cmd;
	TXbuffer[2]=0x07;
	TXbuffer[3]=NET_ID;
	TXbuffer[4]=0x55;
	TXbuffer[5]=0x66;
	TXbuffer[6]=0x77;
	TXbuffer[7]=DEV_ID;
	TXbuffer[8]=flag;
	TXbuffer[9]=data;
	for(i=1;i<10;i++)
		sum+=TXbuffer[i];
	TXbuffer[10]=sum;
	TXbuffer[11]=0x81;
	G_LoRaConfig.PayloadLength = 12;
	LSD_RF_SendPacket(TXbuffer);
	LSD_RF_Sleepmode();
	delay_ms(10);
}

void main( void )
{
	WDT_Init();
	Clock_Init();
	IO_Init();
    FlashInit();
    LED_OUT=1;
	delay_ms(2000);

    //���ø�������
	G_LoRaConfig.LoRa_Freq = 470000000;   //����Ƶ��470MHz
	G_LoRaConfig.BandWidth = BW125KHZ;    //BW = 125KHz
	G_LoRaConfig.SpreadingFactor = SF09;  //SF = 9
	G_LoRaConfig.CodingRate = CR_4_6;     //CR = 4/6
	G_LoRaConfig.PowerCfig = 15;          //19��dBm
	G_LoRaConfig.MaxPowerOn = true;
	G_LoRaConfig.CRCON = true;            //CRC����
	G_LoRaConfig.ExplicitHeaderOn = true; //Header����
	G_LoRaConfig.PayloadLength = 30;      //���ݰ�����*/
	if(SX127x_init()!=NORMAL) 
        ((void (*)())0xFFFE)();   //����ģ���ʼ��ʧ�ܸ�λ
	
	TimerAInit();
	ON_TimerA();
	//TimerBInit();
	//ON_TimerB();
	ADC_Init();
	LSD_RF_Sleepmode();
    /*if(FlashReadChar(ADDR_FLASH)!=DATA_FLASH){
        FlashWriteChar(ADDR_FLASH,DATA_FLASH);
        delay_ms(100);
        //WDTCTL=0;
         ((void (*)())0xFFFE)();
    }else{
        FlashWriteChar(ADDR_FLASH,0xff);
    }    */
    
  	LED_OUT=0;
    
	_EINT();                              //�����ж�
	while(1)
	{
        delay_ms(3000);
	    //GATE_CTL_OUT=!GATE_CTL_OUT;
        /*FlashWriteChar(ADDR_FLASH,DATA_FLASH);
        a=FlashReadChar(ADDR_FLASH);
        a++;
        LPM0;*/
        //LPM3;
		//delay_ms(1000);
		//LED_OUT=!LED_OUT;
	}
}
////////////////////////////////////////////////////////////////////////////////
// �������� : PORT1 interrupt,���߽��������ж����
////////////////////////////////////////////////////////////////////////////////
#pragma vector = PORT1_VECTOR
__interrupt void port1_isr(void)
{
	u8 i,sum=0;
	static u8 rxLen=0;
	if(DIO0_IFG)  			//���LoRa�յ����ݣ�������ж��ڴ�
	{
		DIO0_IFG = 0;                                  //���DIO0�жϱ�־λ
		LSD_RF_RxPacket(Rxbuffer);   //�������ݰ�
		rxLen=G_LoRaConfig.PayloadLength;
    //code here:
		
		if(Rxbuffer[0]==0x80 && Rxbuffer[rxLen-1]==0x81)
		{
			if(Rxbuffer[2]==rxLen-5 && Rxbuffer[1]==0x55)
			{
				if(Rxbuffer[3]==NET_ID && Rxbuffer[4]==0x55 && Rxbuffer[5]==0x66 
					 && Rxbuffer[6]==0x77 && Rxbuffer[7]==DEV_ID)
				{
					for(i=1;i<rxLen-2;i++)
						sum+=Rxbuffer[i];
					if(sum==Rxbuffer[rxLen-2])
					{
						//GATE_CTL_OUT=!GATE_CTL_OUT;
						if(Rxbuffer[8]==0)
							GATE_CTL_OUT=!Rxbuffer[9];		//1-open gate;0---close Gate
					}
				}
			}
		}
		LSD_RF_Sleepmode();
		SX1276Write( REG_LR_HOPPERIOD,0x91 );//ѡһ���ò����ļĴ���������֤
		LSD_RF_RXmode();
	}
	else if(GATE_STATE_IFG)	//����ű��򿪣�������ж��ڴ�
	{
		delay_ms(20);
		if(!GATE_STATE_IN)
		{
			if(flag_rxmode)	//�ڽ���ģʽ��˵���Ѿ������˹���ť
			{
				flag_rxmode=false;
				LED_OUT=0;
				LSD_RF_Sleepmode();
				mySend(0X05,0x00,GATE_STATE_IN);	//�������ݰ�
				delay_ms(300);
				mySend(0X05,0x00,GATE_STATE_IN);	//�������ݰ�
				delay_ms(300);
				mySend(0X05,0x00,GATE_STATE_IN);	//�������ݰ�
				ON_TimerA();
			}
			else						//��ͨģʽ
			{
				TA0R =0;           			//�����ʱ��������
				ADC_ON();
				liquid=(u8)(ADC12MEM1>>4);
				mySend(0X07,0x00,GATE_STATE_IN);	
				delay_ms(10);
				mySend(0X07,0x01,liquid);
			}
		}
		
	}
	P1IFG=0;
}
////////////////////////////////////////////////////////////////////////////////
// �������� : PORT2 interrupt,��Ϊ�����ж����
////////////////////////////////////////////////////////////////////////////////
#pragma vector = PORT2_VECTOR
__interrupt void port2_isr(void)
{
	if(KEY_IFG)                                      //�ж��Ƿ���DIO0������ж�
	{
		delay_ms(40);
		if(!KEY_IN && (!flag_rxmode))
		{
            
                //send pack
                mySend(0X07,0x04,0);
                OFF_TimerA();
                SX1276Write( REG_LR_HOPPERIOD,0x91 );//ѡһ���ò����ļĴ���������֤
                LSD_RF_RXmode();
                flag_rxmode=true;
                LED_OUT=1;
                delay_ms(100);
                LED_OUT=0;
                delay_ms(100);
                LED_OUT=1;
                delay_ms(100);
            
		}
        delay_ms(2000);
        if(!KEY_IN){
            LED_OUT=0;
            delay_ms(300);
            LED_OUT=1;
            delay_ms(300);
            LED_OUT=0;
            delay_ms(300);
            LED_OUT=1;
            delay_ms(300);
            LED_OUT=0;
            delay_ms(300);
            ((void (*)())0xFFFE)();
        }
	}
	P2IFG=0;
}
////////////////////////////////////////////////////////////////////////////////
// �������� : timerA0 interrupt,5s time-up
////////////////////////////////////////////////////////////////////////////////
#pragma vector = TIMERA0_VECTOR
__interrupt void TimerA0_ISR()
{
    LED_OUT=!LED_OUT;
	ADC_ON();
	liquid=(u8)(ADC12MEM1>>4);
	mySend(0X07,0x00,GATE_STATE_IN);
	delay_ms(10);
	mySend(0X07,0x01,liquid);
	if(!flag_rxmode)
	    GATE_CTL_OUT=0;			//1-open gate;0---close Gate
}
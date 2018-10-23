/*################################################
程序逻辑的的

正常时间天线休眠，mcu也休眠。

一旦有门状态的改变，则上报所有数据。5s定时归零---IO口中断，优先级0
人工按下按键，有10s钟时间，开启接收模式。5s定时关闭。---IO口中断，优先级1
每隔5s上报一次数据。---定时器中断，优先级2

所有数据包括：门状态，液位，电量。

#1-从睡眠模式进入接收模式：
	SX1276Write( REG_LR_HOPPERIOD,0x91 );//选一个用不到的寄存器来做验证，不然无法接收
	LSD_RF_RXmode();
#2-进入睡眠模式：
	LSD_RF_Sleepmode();
#3-从睡眠模式进入发送模式：
	G_LoRaConfig.PayloadLength = 12;	//选择发送长度
	LSD_RF_SendPacket(TXbuffer);			//选择发送数组
	LSD_RF_Sleepmode();								//发送完立刻进入睡眠模式

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
//===================================定义变量===================================================
u8 liquid,power;
bool flag_rxmode=false;
extern S_LoRaConfig G_LoRaConfig;
u8 GateBuffer[12]={0x80,7,7,2,0x55,0x66,0x77,0x89,0,0,0x4B,0x81};	//state of the Gate
u8 LqdBuffer[12]={0x80,7,7,2,0x55,0x66,0x77,0x89,1,10,0xD6,0x81};	//liquid data
u8 PwrBuffer[12]={0x80,7,7,2,0x55,0x66,0x77,0x89,3,20,0x82,0x81};	//power data
uint8_t TXbuffer[30]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29};
uint8_t Rxbuffer[64];
//===================================函数声明===================================================
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

    //配置各个参数
	G_LoRaConfig.LoRa_Freq = 470000000;   //中心频点470MHz
	G_LoRaConfig.BandWidth = BW125KHZ;    //BW = 125KHz
	G_LoRaConfig.SpreadingFactor = SF09;  //SF = 9
	G_LoRaConfig.CodingRate = CR_4_6;     //CR = 4/6
	G_LoRaConfig.PowerCfig = 15;          //19±dBm
	G_LoRaConfig.MaxPowerOn = true;
	G_LoRaConfig.CRCON = true;            //CRC开启
	G_LoRaConfig.ExplicitHeaderOn = true; //Header开启
	G_LoRaConfig.PayloadLength = 30;      //数据包长度*/
	if(SX127x_init()!=NORMAL) 
        ((void (*)())0xFFFE)();   //无线模块初始化失败复位
	
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
    
	_EINT();                              //开总中断
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
// 功能描述 : PORT1 interrupt,无线接收数据中断入口
////////////////////////////////////////////////////////////////////////////////
#pragma vector = PORT1_VECTOR
__interrupt void port1_isr(void)
{
	u8 i,sum=0;
	static u8 rxLen=0;
	if(DIO0_IFG)  			//如果LoRa收到数据，则产生中断在此
	{
		DIO0_IFG = 0;                                  //清除DIO0中断标志位
		LSD_RF_RxPacket(Rxbuffer);   //接收数据包
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
		SX1276Write( REG_LR_HOPPERIOD,0x91 );//选一个用不到的寄存器来做验证
		LSD_RF_RXmode();
	}
	else if(GATE_STATE_IFG)	//如果门被打开，则产生中断在此
	{
		delay_ms(20);
		if(!GATE_STATE_IN)
		{
			if(flag_rxmode)	//在接收模式，说明已经触发人工按钮
			{
				flag_rxmode=false;
				LED_OUT=0;
				LSD_RF_Sleepmode();
				mySend(0X05,0x00,GATE_STATE_IN);	//反馈数据包
				delay_ms(300);
				mySend(0X05,0x00,GATE_STATE_IN);	//反馈数据包
				delay_ms(300);
				mySend(0X05,0x00,GATE_STATE_IN);	//反馈数据包
				ON_TimerA();
			}
			else						//普通模式
			{
				TA0R =0;           			//清除定时器计数器
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
// 功能描述 : PORT2 interrupt,人为按键中断入口
////////////////////////////////////////////////////////////////////////////////
#pragma vector = PORT2_VECTOR
__interrupt void port2_isr(void)
{
	if(KEY_IFG)                                      //判断是否是DIO0引起的中断
	{
		delay_ms(40);
		if(!KEY_IN && (!flag_rxmode))
		{
            
                //send pack
                mySend(0X07,0x04,0);
                OFF_TimerA();
                SX1276Write( REG_LR_HOPPERIOD,0x91 );//选一个用不到的寄存器来做验证
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
// 功能描述 : timerA0 interrupt,5s time-up
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
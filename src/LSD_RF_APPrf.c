#include "LSD_RF_APPrf.h"

////////////////////////////////////////////////////////////////////////////////
// �������� : RF�������ݰ�
// ������� : uint8_t*data����������ָ��
// ���ز��� : ��
// ˵��     : ���ݷ�����ɺ�DIO0�ӵ͵�ƽ��ɸߵ�ƽ��ÿ�ε��ô˺��������Զ��Ƚ�DIO0��Ϊ�ͣ��ȴ��ߵ�ƽ
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_SendPacket(uint8_t*cbuf)
{
    unsigned long int j=16777215;                   //��ʱ�ã��û���Ҫ����ʵ�����������
    SX_DIO0_DIR=0;                                  //����Ϊ����
    DIO0_IFG = 0;                                   //���DIO0��־λ
    DIO0_IES = 0;                                   //����DIO0�����ش�����ʽ 
    DIO0_IE = 0;                                    //��ֹDIO0�ж�
    SX1276_TxPacket(cbuf);                    //��������
    while((!DIO0_IFG)&&j)j--;                               //�ȴ�GDIO0��ƽΪ��
    DIO0_IFG = 0;                                   //����жϱ�־λ
}
////////////////////////////////////////////////////////////////////////////////
// �������� : RF�������״̬
// ������� : 
// ���ز��� : ��
// ˵��     : ����������ɺ�DIO0�ӵ͵�ƽ��ɸߵ�ƽ��
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_RXmode()
{
    Rx_mode();                      //RF���ջ��л���RXģʽ
    SX_DIO0_DIR = 0;                     //������
    DIO0_IFG = 0;                        //���DIO0�жϱ�־λ
    DIO0_IES = 0;                        //����DIO0�����ش�����ʽ
    DIO0_IE = 1;                         //ʹ��DIO0�ж�
}

////////////////////////////////////////////////////////////////////////////////
// �������� : RF����Sleep״̬
// ������� : ��
// ���ز��� : ��
// ˵��     : ��
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_Sleepmode(void)
{
    SX1276Write( REG_LR_OPMODE, 0x80|RFLR_OPMODE_STANDBY );     
    SX1276Write(REG_LR_IRQFLAGS,0xff);                           
    SX1276Write( REG_LR_OPMODE, 0x80|RFLR_OPMODE_SLEEP );       
    /*PA_RX_OUT = 0;           //PA_RX ��ʼ�����Ϊ0
    PA_TX_OUT = 0; */          //PA_TX ��ʼ�����Ϊ0   Ŀ���ǽ��ʹ�������
}
////////////////////////////////////////////////////////////////////////////////
// �������� : RF  CAD��ʼ��
// ������� : ��
// ���ز��� : ��
// ˵��     : DIO1--CADDetected    DIO3---CADDone
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_CADinit(void)
{
    SX1276Write( REG_LR_OPMODE, 0x80|RFLR_OPMODE_STANDBY );  
    SX1276Write( REG_LR_PREAMBLEMSB,0xf0);
    SX1276Write( REG_LR_PREAMBLELSB,0xff);
    SX1276Write( REG_LR_IRQFLAGSMASK,\
	~(RFLR_IRQFLAGS_CADDONE|RFLR_IRQFLAGS_CADDETECTED));
    //
    SX1276Write( REG_LR_DIOMAPPING1,\
	RFLR_DIOMAPPING1_DIO3_00 | RFLR_DIOMAPPING1_DIO1_10);
    SX1276WriteRxTx(false);     //set RF switch to RX path
    SX1276Write(REG_LR_IRQFLAGS,0xff);      
}
////////////////////////////////////////////////////////////////////////////////
// �������� : RF����CAD�������ŵ����һ��
// ������� : ��
// ���ز��� : ��
// ˵��     :   ����ʱ��ԼΪ(2^SF+32)/BW
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_CAD_Sample(void)
{
    SX1276WriteRxTx(false);     //set RF switch to RX path
    SX1276Write( REG_LR_OPMODE, 0x80|RFLR_OPMODE_STANDBY );  
    SX1276Write( REG_LR_OPMODE, 0x80|RFLR_OPMODE_CAD ); 
}
////////////////////////////////////////////////////////////////////////////////
// �������� : WOR��ʼ��
// ������� : ��
// ���ز��� : ��
// ˵��     :   DIO1 :�����ж�   DIO3��CAD��ʱ�жϣ�Ҳ������Ϊ�ǽ��ռ���жϣ�
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_WORInit(void)
{
    LSD_RF_CADinit();        //CAD���ܳ�ʼ��
    //CADDoneʹ��
    SX_DIO3_DIR=0;                       //������
    DIO3_IFG = 0;                        //���DIO0�жϱ�־λ
    DIO3_IES = 0;                        //����DIO0�����ش�����ʽ
    DIO3_IE = 1;                         //ʹ��DIO0�ж�
    //CADDetectedʹ��
    SX_DIO1_DIR=0;                       //������
    DIO1_IFG = 0;                        //���DIO0�жϱ�־λ
    DIO1_IES = 0;                        //����DIO0�����ش�����ʽ
    DIO1_IE = 1;                         //ʹ��DIO0�ж�
    //�ر�DIO0�����ж�ʹ��
    DIO0_IE = 0;                         //ʹ��DIO0�ж�
}
/*///////////////////////////////////////////////////////////////////////////////
// �������� : ִ��WOR����
// ������� : uint8_t cclen  0������˯�ߡ�1������CAD���ģʽ
// ���ز��� : ��
// ˵��     :   
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_WOR_Execute(uint8_t cclen)
{
    switch(cclen)
    {
        case 0:   //����˯��
                  LSD_RF_Sleepmode();      //����˯��ģʽ
                  ON_Sleep_Timerout();     //����˯�߳�ʱ��ʱ��
                  break;
        case 1:   //����CAD���ģʽ
                  OFF_Sleep_Timerout();    //�ر�˯�߳�ʱ��ʱ��
                  LSD_RF_CAD_Sample();     //����CADһ�� 
                  
                  break;
        default:  break;
    }
}*/
////////////////////////////////////////////////////////////////////////////////
// �������� : WOR��RX
// ������� : ��
// ���ز��� : ��
// ˵��     : �˳�WOR������RXģʽ��ǰ��preamble��Ȼ�����������ֵ��
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_WOR_Exit()
{
    OFF_TimerA();
    SX1276Write( REG_LR_OPMODE, 0x80|RFLR_OPMODE_STANDBY );      
    SX1276Write( REG_LR_PAYLOADLENGTH,G_LoRaConfig.PayloadLength);                    
    SX1276Write( REG_LR_IRQFLAGSMASK, ~(RFLR_IRQFLAGS_RXDONE)); 
    SX1276Write( REG_LR_DIOMAPPING1, RFLR_DIOMAPPING1_DIO0_00 );
    SX1276WriteRxTx(false);                                     //set RF switch to RX path
    SX1276Write( REG_LR_FIFOADDRPTR,0x00);                      
    SX1276Write(REG_LR_IRQFLAGS,0xff);                          
    SX1276Write( REG_LR_OPMODE, 0x80|RFLR_OPMODE_RECEIVER );   
    SX_DIO0_DIR = 0;                     //������
    DIO0_IFG = 0;                        //���DIO0�жϱ�־λ
    DIO0_IES = 0;                        //����DIO0�����ش�����ʽ
    DIO0_IE = 1;                         //ʹ��DIO0�ж�
    
    DIO1_IE = 0;                         //��ֹDIO1
    DIO3_IE = 0;                         //��ֹDIO3
}

////////////////////////////////////////////////////////////////////////////////
// �������� : RF���ͻ��Ѱ�
// ������� : uint8_t*data����������ָ��,ǰ������
// ���ز��� : ��
// ˵��     : 
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_Awake(uint8_t*cbuf,uint16_t Preamble_Length)
{
    SX_DIO0_DIR = 0;
    DIO0_IFG = 0;                                   //���DIO0��־λ
    DIO0_IES = 0;                                   //����DIO0�����ش�����ʽ 
    DIO0_IE = 0;                                    //��ֹDIO0�ж�
    
    SX1276Write( REG_LR_OPMODE, 0x80|RFLR_OPMODE_STANDBY );      
    SX1276Write( REG_LR_PAYLOADLENGTH,G_LoRaConfig.PayloadLength);                    
    SX1276WriteRxTx(true);                                      
    SX1276Write( REG_LR_FIFOADDRPTR,0x80);                      
    SX1276WriteBuffer(REG_LR_FIFO,cbuf,G_LoRaConfig.PayloadLength);                   
    SX1276Write(REG_LR_IRQFLAGS,0xff);                          
    SX1276Write( REG_LR_IRQFLAGSMASK, ~(RFLR_IRQFLAGS_TXDONE));  
    SX1276Write( REG_LR_DIOMAPPING1, RFLR_DIOMAPPING1_DIO0_01 );
    SX1276Write( REG_LR_PREAMBLEMSB,(uint8_t)(Preamble_Length>>8));//set preamble length
    SX1276Write( REG_LR_PREAMBLELSB,(uint8_t)Preamble_Length);//set preamble length
    SX1276Write( REG_LR_OPMODE, 0x80|RFLR_OPMODE_TRANSMITTER );  
    while((!DIO0_IFG));                                         //�ȴ�GDIO0��ƽΪ��
 
    DIO0_IFG = 0;                                               //����жϱ�־λ
    //�����껽�����ݰ��󣬽�ǰ��ʱ��Ļ�����Ĭ��ֵ��
    SX1276Write( REG_LR_OPMODE, 0x80|RFLR_OPMODE_STANDBY );     
    SX1276Write( REG_LR_PREAMBLEMSB,0);//set preamble length
    SX1276Write( REG_LR_PREAMBLELSB,10);//set preamble length
}
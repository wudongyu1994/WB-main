#include "LSD_RF_SX1276.h"

//===================================�������===================================================
float G_BandWidthKHz = 500.0;//���ؼ���Symbol����ʹ��
float G_TsXms = 1.024;//1.024ms
S_LoRaConfig G_LoRaConfig = {
    470000000,
    BW500KHZ,
    SF08,
    CR_4_5,
    15,
    true,
    true,
    true,
    64,
};

bool LoRaConfig_Check()
{
    if((G_LoRaConfig.LoRa_Freq<137000000)||(G_LoRaConfig.LoRa_Freq>525000000))
        return false;
    G_LoRaConfig.BandWidth = (t_BandWidth)(G_LoRaConfig.BandWidth&0xF0);
    if(G_LoRaConfig.BandWidth>BW500KHZ)
        return false;
    //����BandWidth
    switch(G_LoRaConfig.BandWidth){
    case BW500KHZ:G_BandWidthKHz = 500.0;break;
    case BW250KHZ:G_BandWidthKHz = 250.0;break;
    case BW125KHZ:G_BandWidthKHz = 125.0;break;
    case BW62_50KHZ:G_BandWidthKHz = 62.5;break;
    case BW41_66KHZ:G_BandWidthKHz = 41.66;break;
    case BW31_25KHZ:G_BandWidthKHz = 31.25;break;
    case BW20_83KHZ:G_BandWidthKHz = 20.83;break;
    case BW15_62KHZ:G_BandWidthKHz = 15.62;break;
    case BW10_41KHZ:G_BandWidthKHz = 10.41;break;
    case BW7_81KHZ:G_BandWidthKHz = 7.81;break;
    }
    G_LoRaConfig.SpreadingFactor = (t_SpreadingFactor)(G_LoRaConfig.SpreadingFactor&0xF0);
    if((G_LoRaConfig.SpreadingFactor>SF12)||(G_LoRaConfig.SpreadingFactor<SF06))
        return false;
    //����LoRa��Ԫ���ڣ���λms
    G_TsXms = (2<<((G_LoRaConfig.SpreadingFactor>>4)-1))/G_BandWidthKHz;
    
    G_LoRaConfig.CodingRate = (t_CodingRate)(G_LoRaConfig.CodingRate&0x0E);
    if((G_LoRaConfig.CodingRate>CR_4_8)||(G_LoRaConfig.CodingRate<CR_4_5))
        return false;
    if(G_LoRaConfig.PowerCfig>15)
        return false;
    if(G_LoRaConfig.PayloadLength>127)
        return false;
    return true;
}
//===================================�Ӻ���===================================================
////////////////////////////////////////////////////////////////////////////////
// �������� : RF��ʼ��
// ������� : tSX127xInitPara initPara  ��������Init_LoRa_0_8K, Init_LoRa_4_8K , Init_LoRa_10k,
// ���ز��� : tSX127xError              ����ö������
// ˵��     :  ��ʼ��ʱ���ŵ���ʼ��Ĭ��Ϊ0�ŵ�
////////////////////////////////////////////////////////////////////////////////

tSX127xError SX127x_init()
{
  	uint8_t test = 0;
    if(false==LoRaConfig_Check())   //��������������
    {
        return PARAMETER_INVALID;  //��������
    }
    SX1276InitIo();                // PAIO�ڳ�ʼ��         
    SX1276Reset();                 //��λRF
	do
	{
		 test=SX1276SPISetup();
	}
    while(!test);              //SPI��ʼ��
    
    //�л���LoRamode��standby״̬
    SX1276Write( REG_LR_OPMODE, RFLR_OPMODE_SLEEP );            
    SX1276Write( REG_LR_OPMODE, 0x80|RFLR_OPMODE_SLEEP );       
    SX1276Write( REG_LR_OPMODE, 0x80|RFLR_OPMODE_STANDBY );
    
    /*------------------------------------------------
    SPI ��֤                   */  
    
    SX1276Write( REG_LR_HOPPERIOD,0x91 );//ѡһ���ò����ļĴ���������֤
    SX1276Read( REG_LR_HOPPERIOD,&test);
	if(test!=0x91)
        return SPI_READCHECK_WRONG;  
	
	/*SX1276Write( 0x01, 0 );            
    SX1276Write( 0x01, 0x80|0 );       
    SX1276Write( 0x01, 0x80|1 );
	
	
    SX1276Write( 0x24,0x91 );
	SX1276Read( 0x24,&test);
	if(test==0x91)	P6OUT=0XFF;
	SX1276Write( 0x24,0x88 );
    SX1276Read( 0x24,&test);
    if(test==0x90)  P6OUT=0XFF;           
	while(1);*/

             
    
    //Frequency Configuration 
    LSD_RF_FreqSet();    //����Ƶ��
    
    //PA Configuration  
    LSD_RF_PoutSet();
    SX1276Write( REG_LR_PARAMP,RFLR_PARAMP_0100_US);  
    //  ��PA Ramp��ʱ�䣬����û�LDO���ܿ������������������������ʵ�����PA Rampʱ��
    //  �����Rampʱ����̳�����LDO������ʱ������ֽ���TX��ϵͳ����Ϊ�������������RF�źŲ����ֵ�����
    SX1276Write( REG_LR_OCP,0x20|RFLR_OCP_TRIM_240_MA);//�������ر��� Over Current Protection
    
    //PayloadLength ��ʼ��             
    SX1276Write( REG_LR_PAYLOADLENGTH,G_LoRaConfig.PayloadLength); 
    //ע�⣬��ͷģʽ��Implicit Header��ʱ��������ǰ�涨���շ�˫����PL
    
    //BW��CR��Header���ޣ���ʼ��        
    SX1276Write( REG_LR_MODEMCONFIG1,\
        (((uint8_t)G_LoRaConfig.BandWidth)|((uint8_t)G_LoRaConfig.CodingRate))|(\
            (true==G_LoRaConfig.ExplicitHeaderOn)?0x00:0x01));
    
    //SF��CRC��ʼ��                 
    SX1276Write( REG_LR_MODEMCONFIG2,\
        ((uint8_t)G_LoRaConfig.SpreadingFactor)|(\
            (true==G_LoRaConfig.CRCON)?0x04:0x00));
    if(SF06==G_LoRaConfig.SpreadingFactor){   //����SF = 6����Ҫ�����ú�����
        uint8_t temp = 0;
        SX1276Read( 0x31,&temp);
        SX1276Write(  0x31,(temp& 0xF8)|0x05);
        SX1276Write(  0x37,0x0C); 
    }
    
    //�������Ż������Ƿ�����AutoAGCĬ�Ͽ���    
    SX1276Write( REG_LR_MODEMCONFIG3,((G_TsXms>16.0)?\
      RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_ON:RFLR_MODEMCONFIG3_LOWDATARATEOPTIMIZE_OFF\
          )|RFLR_MODEMCONFIG3_AGCAUTO_ON);
    return NORMAL;
}
////////////////////////////////////////////////////////////////////////////////
// �������� : RF�������ݰ�
// ������� : uint8_t*data����������ָ��
// ���ز��� : ��
// ˵��     : ����Ϊ�����ǣ� preamble�Ļ�Ĭ��ֵ
////////////////////////////////////////////////////////////////////////////////
void SX1276_TxPacket(uint8_t*data)
{
    
    SX1276Write( REG_LR_OPMODE, 0x80|RFLR_OPMODE_STANDBY );      
    SX1276Write( REG_LR_PREAMBLEMSB,0);
    SX1276Write( REG_LR_PREAMBLELSB,10);
    SX1276Write( REG_LR_PAYLOADLENGTH,G_LoRaConfig.PayloadLength);                    
    SX1276WriteRxTx(true);                                      
    SX1276Write( REG_LR_FIFOADDRPTR,0x80);                      
    SX1276WriteBuffer(REG_LR_FIFO,data,G_LoRaConfig.PayloadLength);                   
    SX1276Write(REG_LR_IRQFLAGS,0xff);                          
    SX1276Write( REG_LR_IRQFLAGSMASK, ~(RFLR_IRQFLAGS_TXDONE)); 
    SX1276Write( REG_LR_DIOMAPPING1, RFLR_DIOMAPPING1_DIO0_01 );
    SX1276Write( REG_LR_OPMODE, 0x80|RFLR_OPMODE_TRANSMITTER );  
}
////////////////////////////////////////////////////////////////////////////////
// �������� : RF�������״̬
// ������� : 
// ���ز��� : ��
// ˵��     : ������պ�preamble���û�Ĭ��ֵΪ
////////////////////////////////////////////////////////////////////////////////
void Rx_mode()
{
    SX1276Write( REG_LR_OPMODE, 0x80|RFLR_OPMODE_STANDBY );      
    SX1276Write( REG_LR_PREAMBLEMSB,0);
    SX1276Write( REG_LR_PREAMBLELSB,10);
    SX1276Write( REG_LR_PAYLOADLENGTH,G_LoRaConfig.PayloadLength);
    SX1276Write( REG_LR_IRQFLAGSMASK, ~(RFLR_IRQFLAGS_RXDONE)); 
    SX1276Write( REG_LR_DIOMAPPING1, RFLR_DIOMAPPING1_DIO0_00 );
    SX1276WriteRxTx(false);                                     
    SX1276Write( REG_LR_FIFOADDRPTR,0x00);                      
    SX1276Write(REG_LR_IRQFLAGS,0xff);                          
    SX1276Write( REG_LR_OPMODE, 0x80|RFLR_OPMODE_RECEIVER );       
}
////////////////////////////////////////////////////////////////////////////////
// �������� : RF�������ݰ�
// ������� : uint8_t*cbuf��������ָ��
// ���ز��� : ��
// ˵��     : 
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_RxPacket(uint8_t*cbuf)
{
    if(true==G_LoRaConfig.ExplicitHeaderOn){
        //��ͷ��ô�ӼĴ����ж���������G_LoRaConfig���ó��ȶ�ȡFIFO
        SX1276Read(REG_LR_NBRXBYTES,&G_LoRaConfig.PayloadLength);
        SX1276Write( REG_LR_FIFOADDRPTR,0x00);
    }
    SX1276ReadFifo(cbuf,G_LoRaConfig.PayloadLength);              
    SX1276Write(REG_LR_IRQFLAGS,0xff);        
}
////////////////////////////////////////////////////////////////////////////////
// �������� : RF����SLEEP״̬
// ������� : ��
// ���ز��� : ��
// ˵��     : 
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_SleepMode(void)
{
    SX1276Write( REG_LR_OPMODE, 0x80|RFLR_OPMODE_STANDBY );      
    SX1276Write( REG_LR_OPMODE, 0x80|RFLR_OPMODE_SLEEP );     
}
////////////////////////////////////////////////////////////////////////////////
// �������� : RF����standby״̬
// ������� : ��
// ���ز��� : ��
// ˵��     : 
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_StandbyMode(void)
{
    SX1276Write( REG_LR_OPMODE, 0x80|RFLR_OPMODE_STANDBY );       
}
////////////////////////////////////////////////////////////////////////////////
// �������� : RF����Ƶ��
// ������� : ��
// ���ز��� : ��
// ˵��     : 
////////////////////////////////////////////////////////////////////////////////
tSX127xError LSD_RF_FreqSet()
{
    
    if((G_LoRaConfig.LoRa_Freq>525000000)||(G_LoRaConfig.LoRa_Freq<137000000))
        return PARAMETER_INVALID;
    uint32_t freq_reg = (uint32_t)(G_LoRaConfig.LoRa_Freq/FREQ_STEP);
    uint8_t test_FRFMSB = 0,test_FRFMID=0,test_FRFLSB=0;
    LSD_RF_StandbyMode();
    // FREQ = 474.6MHz
    SX1276Write( REG_LR_FRFMSB, (uint8_t)(freq_reg>>16));//Carrier Freq 470M
    SX1276Write( REG_LR_FRFMID, (uint8_t)(freq_reg>>8) );
    SX1276Write( REG_LR_FRFLSB, (uint8_t)(freq_reg) );
    
    SX1276Read(REG_LR_FRFMSB,&test_FRFMSB);
    SX1276Read(REG_LR_FRFMID,&test_FRFMID);
    SX1276Read(REG_LR_FRFLSB,&test_FRFLSB);
    
    if(test_FRFMSB != (uint8_t)(freq_reg>>16))
        return SPI_READCHECK_WRONG;
    if(test_FRFMID != (uint8_t)(freq_reg>>8))
        return SPI_READCHECK_WRONG;
    if(test_FRFLSB != (uint8_t)(freq_reg))
        return SPI_READCHECK_WRONG;
    return NORMAL;
}
////////////////////////////////////////////////////////////////////////////////
// �������� : RF���ù���
// ������� : ��
// ���ز��� : ��
// ˵��     : 
////////////////////////////////////////////////////////////////////////////////
tSX127xError LSD_RF_PoutSet()
{
    if(G_LoRaConfig.PowerCfig>15)
        return PARAMETER_INVALID;
    LSD_RF_StandbyMode();
    SX1276Write( REG_LR_PACONFIG, 0xf0|G_LoRaConfig.PowerCfig);
    uint8_t test = 0;
    SX1276Read(REG_LR_PACONFIG,&test);
    if((0xf0|G_LoRaConfig.PowerCfig)!=test)
        return SPI_READCHECK_WRONG;
    if(true==G_LoRaConfig.MaxPowerOn)
        SX1276Write( REG_LR_PADAC, 0x80|RFLR_PADAC_20DBM_ON );  
    else
        SX1276Write( REG_LR_PADAC, 0x80|RFLR_PADAC_20DBM_OFF );
    return NORMAL;
}

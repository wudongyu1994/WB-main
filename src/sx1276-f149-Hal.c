#include "sx1276-f149-Hal.h"

////////////////////////////////////////////////////////////////////////////////
// �������� : RF��ʼ��IO��
// ������� : ��
// ���ز��� : ��
// ˵��     : 
////////////////////////////////////////////////////////////////////////////////
void SX1276InitIo( void )
{
    /*PA_TX_DIR = 1;           //PA_TX ��Ϊ���
    PA_RX_DIR = 1;           //PA_RX ��Ϊ���
    PA_RX_OUT = 0;           //PA_RX ��ʼ�����Ϊ0
    PA_TX_OUT = 1; */          //PA_TX ��ʼ�����Ϊ1
    SXRESET_SET = 1;
}
////////////////////////////////////////////////////////////////////////////////
// �������� : RF  SPI���ó�ʼ��
// ������� : ��
// ���ز��� : ��
// ˵��     : �˳�ʼ��Ϊ430Ӳ��SPI��ʼ��
////////////////////////////////////////////////////////////////////////////////
uint8_t SX1276SPISetup(void)
{
	SPI_NSS_DIR = 1;
    SPI_NSS_OUT = 1;         // /CS disable
	SPI_PSEL |= SPI_SIMO_BIT+SPI_SOMI_BIT+SPI_UCLK_BIT;
    SPI_PDIR |= SPI_SIMO_BIT+SPI_UCLK_BIT;
    SPI_PDIR &= ~SPI_SOMI_BIT;
	
	U0CTL =MM + SYNC + SWRST +CHAR ;
	U0TCTL=/*CKPL + */SSEL0 +STC ;
	U0BR1=0;
	U0BR0=0X07;
	U0MCTL = 0;
	ME1 = USPIE0;
	U0CTL &= ~SWRST;            // Initialize USART state machine
	//IE1 |= URXIE0 + UTXIE0;    // Enable USART0 RX interrupt
	return 1;
}
////////////////////////////////////////////////////////////////////////////////
// �������� : RF   ��λ
// ������� : ��
// ���ز��� : ��
// ˵��     :
////////////////////////////////////////////////////////////////////////////////
void SX1276Reset()
{
    SXRESET_SET = 1;       //Ӳ����λIO�ڷ��������
    SXRESET_OUT = 0;       //Ӳ����λIO�����0
    delay_ms(1);           //��ʱ
    SXRESET_OUT = 1;       //���Ϊ1
    delay_ms(15);
}
////////////////////////////////////////////////////////////////////////////////
// �������� : RF  ��Ĵ�����ַ������������
// ������� : uint8_t addr,�Ĵ�����ַ uint8_t *buffer,��������ָ�� uint8_t sizeָ�볤��
// ���ز��� : ��
// ˵��     :
////////////////////////////////////////////////////////////////////////////////
void SX1276WriteBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
	uint8_t i;  
	
    SPI_NSS_OUT = 0;
    IFG1 &= ~URXIFG0;                     // Clear flag(
    U0TXBUF = (addr | 0x80);              // Send address  
    while (!(IFG1&URXIFG0));              // Wait for TX to finish
    for( i = 0; i < size; i++ )
    { 
        IFG1 &= ~URXIFG0;					// Clear flag(
        U0TXBUF = buffer[i];                // Send data
        while (!(IFG1&URXIFG0));            // Wait for TX to finish
    }
    SPI_NSS_OUT = 1;
}
////////////////////////////////////////////////////////////////////////////////
// �������� : RF  ��Ĵ�����ַ����������
// ������� : uint8_t addr,�Ĵ�����ַ uint8_t *buffer,�洢����ָ�� uint8_t sizeҪ���ĳ���
// ���ز��� : ���ݷ��ص�*buffer��
// ˵��     :
////////////////////////////////////////////////////////////////////////////////
void SX1276ReadBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
    uint8_t i;
    
	SPI_NSS_OUT = 0;
    IFG1 &= ~URXIFG0;                       // Clear flag
    U0TXBUF = (addr & 0x7F);                // Send address 
    while (!(IFG1&URXIFG0));                // Wait for end of addr byte TX
    IFG1 &= ~URXIFG0;                       // Clear flag
    for( i = 0; i < size; i++ )
    {   
        U0TXBUF = 0;                          //Initiate next data RX
        while (!(IFG1&URXIFG0));              // Wait for RX to finish
        buffer[i] = U0RXBUF;                  // Store data from last data RX
        									  //��ȡU0RXBUF��IFG�Զ�Reset 
    }  
    SPI_NSS_OUT = 1;
}
////////////////////////////////////////////////////////////////////////////////
// �������� : RF  ��Ĵ�����ַд1�ֽ�����
// ������� : uint8_t addr,�Ĵ�����ַ uint8_t data����
// ���ز��� : 
// ˵��     :
////////////////////////////////////////////////////////////////////////////////
void SX1276Write( uint8_t addr, uint8_t data )
{
    SX1276WriteBuffer( addr, &data, 1 );
}
////////////////////////////////////////////////////////////////////////////////
// �������� : RF  ��Ĵ�����ַ��1�ֽ�����
// ������� : uint8_t addr,�Ĵ�����ַ uint8_t *data�����ݴ洢��ַ
// ���ز��� : 
// ˵��     :
////////////////////////////////////////////////////////////////////////////////
void SX1276Read( uint8_t addr, uint8_t *data )
{
    SX1276ReadBuffer( addr, data, 1 );
}
////////////////////////////////////////////////////////////////////////////////
// �������� : RF  ��FIFOд����
// ������� : uint8_t *buffer,����ָ�� uint8_t size����
// ���ز��� : 
// ˵��     :
////////////////////////////////////////////////////////////////////////////////
void SX1276WriteFifo( uint8_t *buffer, uint8_t size )
{
    SX1276WriteBuffer( 0, buffer, size );
}
////////////////////////////////////////////////////////////////////////////////
// �������� : RF  ��FIFO������
// ������� : uint8_t *buffer,����ָ�� uint8_t size����
// ���ز��� : uint8_t *buffer �洢��ȡ����
// ˵��     :
////////////////////////////////////////////////////////////////////////////////
void SX1276ReadFifo( uint8_t *buffer, uint8_t size )
{
    SX1276ReadBuffer( 0, buffer, size );
}
////////////////////////////////////////////////////////////////////////////////
// �������� : RF  TX/RX��PA�л�
// ������� :  bool txEnable  �л��߼�
// ���ز��� : ��
// ˵��     :�棺��ΪTX���٣���ΪRX   ΪӲ������PA����IO��
////////////////////////////////////////////////////////////////////////////////
void SX1276WriteRxTx( bool txEnable )
{
    /*if( txEnable != 0 )       //���Ϊ�棬ΪTX
    {
        PA_RX_OUT = 0;        //PA_RXΪ0
        PA_TX_OUT = 1;        //PA_TXΪ1
    }
    else  //Ϊ�٣�ΪRX
    {
        PA_RX_OUT = 1;        //PA_RXΪ1
        PA_TX_OUT = 0;         //PA_TXΪ0
    }*/
}
////////////////////////////////////////////////////////////////////////////////

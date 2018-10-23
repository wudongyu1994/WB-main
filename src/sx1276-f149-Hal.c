#include "sx1276-f149-Hal.h"

////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RF初始化IO口
// 输入参数 : 无
// 返回参数 : 无
// 说明     : 
////////////////////////////////////////////////////////////////////////////////
void SX1276InitIo( void )
{
    /*PA_TX_DIR = 1;           //PA_TX 作为输出
    PA_RX_DIR = 1;           //PA_RX 作为输出
    PA_RX_OUT = 0;           //PA_RX 初始化输出为0
    PA_TX_OUT = 1; */          //PA_TX 初始化输出为1
    SXRESET_SET = 1;
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RF  SPI设置初始化
// 输入参数 : 无
// 返回参数 : 无
// 说明     : 此初始化为430硬件SPI初始化
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
// 功能描述 : RF   复位
// 输入参数 : 无
// 返回参数 : 无
// 说明     :
////////////////////////////////////////////////////////////////////////////////
void SX1276Reset()
{
    SXRESET_SET = 1;       //硬件复位IO口方向做输出
    SXRESET_OUT = 0;       //硬件复位IO口输出0
    delay_ms(1);           //延时
    SXRESET_OUT = 1;       //输出为1
    delay_ms(15);
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RF  向寄存器地址连续发送数据
// 输入参数 : uint8_t addr,寄存器地址 uint8_t *buffer,发送数组指针 uint8_t size指针长度
// 返回参数 : 无
// 说明     :
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
// 功能描述 : RF  向寄存器地址连续读数据
// 输入参数 : uint8_t addr,寄存器地址 uint8_t *buffer,存储数组指针 uint8_t size要读的长度
// 返回参数 : 数据返回到*buffer中
// 说明     :
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
        									  //读取U0RXBUF后，IFG自动Reset 
    }  
    SPI_NSS_OUT = 1;
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RF  向寄存器地址写1字节数据
// 输入参数 : uint8_t addr,寄存器地址 uint8_t data数据
// 返回参数 : 
// 说明     :
////////////////////////////////////////////////////////////////////////////////
void SX1276Write( uint8_t addr, uint8_t data )
{
    SX1276WriteBuffer( addr, &data, 1 );
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RF  向寄存器地址读1字节数据
// 输入参数 : uint8_t addr,寄存器地址 uint8_t *data读数据存储地址
// 返回参数 : 
// 说明     :
////////////////////////////////////////////////////////////////////////////////
void SX1276Read( uint8_t addr, uint8_t *data )
{
    SX1276ReadBuffer( addr, data, 1 );
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RF  向FIFO写数据
// 输入参数 : uint8_t *buffer,数组指针 uint8_t size长度
// 返回参数 : 
// 说明     :
////////////////////////////////////////////////////////////////////////////////
void SX1276WriteFifo( uint8_t *buffer, uint8_t size )
{
    SX1276WriteBuffer( 0, buffer, size );
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RF  向FIFO读数据
// 输入参数 : uint8_t *buffer,数组指针 uint8_t size长度
// 返回参数 : uint8_t *buffer 存储读取内容
// 说明     :
////////////////////////////////////////////////////////////////////////////////
void SX1276ReadFifo( uint8_t *buffer, uint8_t size )
{
    SX1276ReadBuffer( 0, buffer, size );
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : RF  TX/RX的PA切换
// 输入参数 :  bool txEnable  切换逻辑
// 返回参数 : 无
// 说明     :真：作为TX。假：作为RX   为硬件两个PA控制IO口
////////////////////////////////////////////////////////////////////////////////
void SX1276WriteRxTx( bool txEnable )
{
    /*if( txEnable != 0 )       //如果为真，为TX
    {
        PA_RX_OUT = 0;        //PA_RX为0
        PA_TX_OUT = 1;        //PA_TX为1
    }
    else  //为假，为RX
    {
        PA_RX_OUT = 1;        //PA_RX为1
        PA_TX_OUT = 0;         //PA_TX为0
    }*/
}
////////////////////////////////////////////////////////////////////////////////

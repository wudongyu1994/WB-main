#ifndef __SX1276_F149_HAL_H__
#define __SX1276_F149_HAL_H__

#include "sys.h"

//===================================RF硬件IO口定义===================================================
//SX1276 DIO pins  I/O definitions
//DIO0为P1.1口
#define SX_DIO0_DIR         P1DIR_bit.P1     //DIO0 方向寄存器   
#define DIO0_IFG            P1IFG_bit.P1     //DIO0 中断标志位寄存器
#define DIO0_IES            P1IES_bit.P1     //DIO0 触发方式寄存器，上升沿或下降沿
#define DIO0_IE             P1IE_bit.P1      //DIO0 中断使能寄存器
#define DIO0_IN             P1IN_bit.P1      //DIO0 输入寄存器
//DIO1为P2.0口
#define SX_DIO1_DIR         P2DIR_bit.P0     //DIO1 方向寄存器
#define DIO1_IFG            P2IFG_bit.P0     //DIO0 中断标志位寄存器
#define DIO1_IES            P2IES_bit.P0     //DIO0 触发方式寄存器，上升沿或下降沿
#define DIO1_IE             P2IE_bit.P0      //DIO0 中断使能寄存器
#define DIO1_IN             P2IN_bit.P0      //DIO1 输入寄存器
//DIO3为P1.0口
#define SX_DIO3_DIR         P1DIR_bit.P0     //DIO3 方向寄存器
#define DIO3_IFG            P1IFG_bit.P0     //DIO0 中断标志位寄存器
#define DIO3_IES            P1IES_bit.P0     //DIO0 触发方式寄存器，上升沿或下降沿
#define DIO3_IE             P1IE_bit.P0      //DIO0 中断使能寄存器
#define DIO3_IN             P1IN_bit.P0      //DIO3 输入寄存器

//SX1276 SPI I/O definitions
#define  SPI_NSS_DIR        P3DIR_bit.P0    //片选
#define  SPI_NSS_OUT        P3OUT_bit.P0

#define  SPI_PSEL           P3SEL
#define  SPI_PDIR           P3DIR
#define  SPI_SIMO_BIT       BIT1
#define  SPI_SOMI_BIT       BIT2
#define  SPI_UCLK_BIT       BIT3
#define  SPI_SOMI_BIT_R     P3IN_bit.P2

//SX1276 RESET I/O definitions
#define  SXRESET_SET        P2DIR_bit.P1
#define  SXRESET_OUT        P2OUT_bit.P1

//===================================宏定义===================================================
// RXTX pin control see errata note
#define RXTX( txEnable )                            SX1276WriteRxTx( txEnable );

// DIO state read functions mapping

#define DIO0                                        SX1276ReadDio0( )
#define DIO1                                        SX1276ReadDio1( )
#define DIO2                                        SX1276ReadDio2( )
#define DIO3                                        SX1276ReadDio3( )
#define DIO4                                        SX1276ReadDio4( )
#define DIO5                                        SX1276ReadDio5( )
//0,1,3 needed

#define SX1276ReadDio0( )   DIO0_IN
#define SX1276ReadDio1( )   DIO1_IN
#define SX1276ReadDio2( )   1         //未使用
#define SX1276ReadDio3( )   DIO3_IN
#define SX1276ReadDio4( )   1         //未使用

typedef enum
{
    RADIO_RESET_OFF,
    RADIO_RESET_ON,
}tRadioResetState;

//===================================SPI函数声明===================================================
void SX1276InitIo( void );
uint8_t SX1276SPISetup(void);
void SX1276WriteBuffer( uint8_t addr, uint8_t *buffer, uint8_t size );
void SX1276ReadBuffer( uint8_t addr, uint8_t *buffer, uint8_t size );
void SX1276Write( uint8_t addr, uint8_t data );
void SX1276Read( uint8_t addr, uint8_t *data );
void SX1276WriteFifo( uint8_t *buffer, uint8_t size );
void SX1276ReadFifo( uint8_t *buffer, uint8_t size );
void SX1276Reset();
void SX1276WriteRxTx( bool txEnable );

#endif //__SX1276_HAL_H__
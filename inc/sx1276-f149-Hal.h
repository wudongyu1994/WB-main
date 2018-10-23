#ifndef __SX1276_F149_HAL_H__
#define __SX1276_F149_HAL_H__

#include "sys.h"

//===================================RFӲ��IO�ڶ���===================================================
//SX1276 DIO pins  I/O definitions
//DIO0ΪP1.1��
#define SX_DIO0_DIR         P1DIR_bit.P1     //DIO0 ����Ĵ���   
#define DIO0_IFG            P1IFG_bit.P1     //DIO0 �жϱ�־λ�Ĵ���
#define DIO0_IES            P1IES_bit.P1     //DIO0 ������ʽ�Ĵ����������ػ��½���
#define DIO0_IE             P1IE_bit.P1      //DIO0 �ж�ʹ�ܼĴ���
#define DIO0_IN             P1IN_bit.P1      //DIO0 ����Ĵ���
//DIO1ΪP2.0��
#define SX_DIO1_DIR         P2DIR_bit.P0     //DIO1 ����Ĵ���
#define DIO1_IFG            P2IFG_bit.P0     //DIO0 �жϱ�־λ�Ĵ���
#define DIO1_IES            P2IES_bit.P0     //DIO0 ������ʽ�Ĵ����������ػ��½���
#define DIO1_IE             P2IE_bit.P0      //DIO0 �ж�ʹ�ܼĴ���
#define DIO1_IN             P2IN_bit.P0      //DIO1 ����Ĵ���
//DIO3ΪP1.0��
#define SX_DIO3_DIR         P1DIR_bit.P0     //DIO3 ����Ĵ���
#define DIO3_IFG            P1IFG_bit.P0     //DIO0 �жϱ�־λ�Ĵ���
#define DIO3_IES            P1IES_bit.P0     //DIO0 ������ʽ�Ĵ����������ػ��½���
#define DIO3_IE             P1IE_bit.P0      //DIO0 �ж�ʹ�ܼĴ���
#define DIO3_IN             P1IN_bit.P0      //DIO3 ����Ĵ���

//SX1276 SPI I/O definitions
#define  SPI_NSS_DIR        P3DIR_bit.P0    //Ƭѡ
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

//===================================�궨��===================================================
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
#define SX1276ReadDio2( )   1         //δʹ��
#define SX1276ReadDio3( )   DIO3_IN
#define SX1276ReadDio4( )   1         //δʹ��

typedef enum
{
    RADIO_RESET_OFF,
    RADIO_RESET_ON,
}tRadioResetState;

//===================================SPI��������===================================================
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
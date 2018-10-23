#ifndef LED_H
#define LED_H

#include "io430.h"
#include "in430.h"

//小4152
#define LEDOUT      P4OUT
#define LEDDIR      P4DIR

#define LED0        (BIT0)
#define LED1        (BIT1)
#define LED2        (BIT2)
#define LED3        (BIT3)
#define LED4        (BIT4)
#define LED5        (BIT5)
#define LED6        (BIT6)
#define LED7        (BIT7)

#define LEDALL      (0x3f)
#define LEDENAB(X)  LEDDIR |= ((X)&LEDALL)
#define LEDDISA(X)  LEDDIR &= ~((X)&LEDALL)
#define LEDONBIT(X)    LEDOUT &= ~((X)&LEDALL)
#define LEDONBAND(X)   LEDOUT = ~((X)&LEDALL) //Band - 每次都控制3个灯， Bit 只控制X置位
#define LEDOFFBIT(X)   LEDOUT |= ((X)&LEDALL)
#define LEDOFFBAND(X)  LEDOUT = ((X)&LEDALL)
#define LEDTOG(X)   LEDOUT ^= ((X)&LEDALL)  //与1异或等效于取反



#endif
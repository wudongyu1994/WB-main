#ifndef FLASH_H
#define FLASH_H

#include "sys.h"
#define ADDR_FLASH  0X3000
#define DATA_FLASH  0X83

void WaitForEnable();
void FlashInit();
void FlashWriteChar(unsigned int addr,char Data);
char FlashReadChar(unsigned int Addr);

#endif
#include "flash.h"

void WaitForEnable()
{
    while((FCTL3 & BUSY) == BUSY);      //Busy
}

void FlashInit()
{
    FCTL2 = FWKEY + FSSEL_2 + FN1;          // д╛хо SMCLK/3 =333KHz 
}

void FlashWriteChar(unsigned int addr,char Data)
{
    char *FlashPtr = (char *)addr;              // Segment A pointer
    FCTL1 = FWKEY + WRT;                        // Set WRT bit for write operation
    FCTL3 = FWKEY;                              // Clear Lock bit
    _DINT();
    *FlashPtr = Data;                           // Save Data
    WaitForEnable();                            //Busy
    _EINT();
    FCTL1 = FWKEY;                              // Clear WRT bit
    FCTL3 = FWKEY + LOCK;                       // Set LOCK bit
}

char FlashReadChar(unsigned int Addr)
{ 
    char Data;
    char *FlashPtr = (char *) Addr; 
    Data = *FlashPtr;
    return(Data);
}
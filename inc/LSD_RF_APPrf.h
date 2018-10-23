#ifndef LSD_RF_APPrf_H
#define LSD_RF_APPrf_H

#include "sys.h"
#include "sx1276-LoRa.h"
#include "sx1276-f149-Hal.h"
#include "LSD_RF_SX1276.h"
#include "timer.h"

void LSD_RF_SendPacket(uint8_t*cbuf);
void LSD_RF_RXmode();
void LSD_RF_Sleepmode(void);
void LSD_RF_CADinit(void);
void LSD_RF_CAD_Sample(void);
void LSD_RF_WORInit(void);
void LSD_RF_WOR_Execute(uint8_t cclen);
void LSD_RF_WOR_Exit();
void LSD_RF_Awake(uint8_t*cbuf,uint16_t Preamble_Length);
#endif
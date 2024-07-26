#include <stdio.h>
#include "Debug_BootTimeLog.h"
#include "HAL_Uart.h"
#include "CmicM.h"

uint16 G_BOOT_TIME_CNT = 1;
BOOTTIMESTR BOOT_TIME;

void CmicM_debug_BootTimeLog(bool final, bool start, uint16 step, ADK_LOG_FUNCTION api ){
    if(final == TRUE){
        if(start == TRUE){
            BOOT_TIME[0].timestamp[0] = TickerGetTimestamp();
            BOOT_TIME[0].STEP = step;
        }
        else{
            BOOT_TIME[0].timestamp[1] = TickerGetTimestamp();
            BOOT_TIME[0].timestamp[2] = BOOT_TIME[0].timestamp[1] - BOOT_TIME[0].timestamp[0];  //result in msec
            BOOT_TIME[0].DURATION = ((float)BOOT_TIME[0].timestamp[2]) / 1000;                  //convert to sec
            BOOT_TIME[0].API = api;
        }
    }
    else{
        if(start == TRUE){
            BOOT_TIME[G_BOOT_TIME_CNT].timestamp[0] = TickerGetTimestamp();
            BOOT_TIME[G_BOOT_TIME_CNT].STEP = step;
        }
        else{
            BOOT_TIME[G_BOOT_TIME_CNT].timestamp[1] = TickerGetTimestamp();
            BOOT_TIME[G_BOOT_TIME_CNT].timestamp[2] = BOOT_TIME[G_BOOT_TIME_CNT].timestamp[1] - BOOT_TIME[G_BOOT_TIME_CNT].timestamp[0];
            BOOT_TIME[G_BOOT_TIME_CNT].DURATION = ((float)BOOT_TIME[G_BOOT_TIME_CNT].timestamp[2]) / 1000;
            BOOT_TIME[G_BOOT_TIME_CNT].API = api;
            G_BOOT_TIME_CNT++;
        }
    }
}
///////////////////////////////////////////////////////////
//  HfsiRam.h
//  Implementation of the HFSI Data
//  Created on:      2015/11/09 15:41:54
//  Original author: Auto Generate By Excel
///////////////////////////////////////////////////////////

#ifndef HfsiRam_h
#define HfsiRam_h

#include "FinisherCommon.h"

#define HFSI_EXCHANGE_HISTORY_NUM 3

typedef struct {
    UL currentCounter;
    UL lifeSpec;
    UL lastCounter[HFSI_EXCHANGE_HISTORY_NUM];
    UC exchangeReason[HFSI_EXCHANGE_HISTORY_NUM];
    UC dummy;
} HFSIRamData;

extern HFSIRamData hfsiRamArea[];

#endif // #ifndef HfsiRam_h

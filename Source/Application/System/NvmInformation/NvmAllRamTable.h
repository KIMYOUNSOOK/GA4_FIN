///////////////////////////////////////////////////////////
//  NvmAllRamTable.h
//  Implementation of the NVM Data
//  Created on:      2016/05/18 10:09:23
//  Original author: Auto Generate By Excel
///////////////////////////////////////////////////////////

#ifndef NvmAllRamTable_h
#define NvmAllRamTable_h

#include "FinisherCommon.h"

typedef struct {
    UC* address;
}NVMRamData;

extern const NVMRamData nvmRamTable[];
extern const US NVM_RAM_TABLE_SIZE;

#endif // #ifndef NvmAllRamTable_h

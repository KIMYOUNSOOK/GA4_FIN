///////////////////////////////////////////////////////////
//  NvmAutoInitialize.h
//  Implementation of the NVM Data
//  Created on:      2016/05/18 10:09:23
//  Original author: Auto Generate By Excel
///////////////////////////////////////////////////////////

#ifndef NvmAutoInitialize_h
#define NvmAutoInitialize_h

#include "FinisherCommon.h"
#include "ENvmID.h"

typedef struct {
    US major;
    US minor;
    ENvmID id;
}NVMInitializeData;

extern const NVMInitializeData nvmAutoInitializeData[];

#endif // #ifndef NvmAutoInitialize_h

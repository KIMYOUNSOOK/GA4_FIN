///////////////////////////////////////////////////////////
//  HfsiAutoInitialize.h
//  Implementation of the HFSI Data
//  Created on:      2015/11/09 15:41:54
//  Original author: Auto Generate By Excel
///////////////////////////////////////////////////////////

#ifndef HfsiAutoInitialize_h
#define HfsiAutoInitialize_h

#include "FinisherCommon.h"
#include "EHfsiItemID.h"

typedef struct {
    US major;
    US minor;
    EHfsiItemID id;
} HFSIInitializeData;

extern const HFSIInitializeData hfsiAutoInitializeData[];

#endif // #ifndef HfsiAutoInitialize_h

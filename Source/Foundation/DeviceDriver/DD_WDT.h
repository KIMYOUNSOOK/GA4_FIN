// File Name: DD_WDT.h
// Description:
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 13.10.15
// Revision:  000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+--------------------------------------------------------------------------------------
// 13.10.15 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev001 Base)
// Revision 000

#ifndef DD_WDT_H
#define DD_WDT_H

#include "DD_Event.h"

void DD_WDT_Notify(DD_EventID eventId, UC* pMsg);
void DD_WDT_Start(void);
void DD_WDT_Stop(void);
void DD_WDT_Clear(void);

#endif

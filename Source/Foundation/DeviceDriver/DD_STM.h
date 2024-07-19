// File Name: DD_STM.h
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
// 13.10.15 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev005 Base)
// Revision 000

#ifndef DD_STM_H
#define DD_STM_H

#include "DD_Event.h"
#include "DD_INT.h"

#define DD_STM_NO_TIMER					0xFF
#define DD_STM_NO_DMA					0xFF

#define DD_STM_RESULT_OK				0
#define DD_STM_RESULT_NG				(-1)

typedef struct{
	UC profileID;
	UC startIndex;
	UC endIndex;
} DD_STM_PROFILE_TYPE;

typedef enum{
//	DD_STM_PROF_ID_STACKER_UPDOWN,
//	DD_STM_PROF_ID_STACKER_PRF_1,  
	DD_STM_PROF_ID_STACKER_PRF_2,//V3.3.92 Stacker Profile Added..
	DD_STM_PROF_ID_EJCT_BELT_PRF_1,
	DD_STM_PROF_ID_EJCT_BELT_PRF_2,
	DD_STM_PROF_ID_EJCT_BELT_PRF_3,
	DD_STM_PROF_ID_EJCT_BELT_PRF_EJECT,
	DD_STM_PROF_ID_TAMPER_NORMAL_HI,
	DD_STM_PROF_ID_TAMPER_NORMAL_MID,
	DD_STM_PROF_ID_TAMPER_NORMAL_LOW,
	DD_STM_PROF_ID_TAMPER_INITIALIZE,
	DD_STM_PROF_ID_NUM
} DD_STM_PROF_ID;

typedef enum{
	DD_STM_ID_STACKER,			// Stacker Motor
	DD_STM_ID_EJECT_BELT,		// Eject Belt Motor
	DD_STM_ID_RIGHT_TAMPER,		// Right Tamper Motor
	DD_STM_ID_LEFT_TAMPER, 		// Left Tamper Motor
	DD_STM_ID_NUM
} DD_STM_ID;

// EventLoopìoò^ä÷êî
void DD_STM_Notify(DD_EventID eventId, UC* pMsg);

// ÉAÉvÉäíÒãüAPI
void DD_STM_Init(void);
void DD_STM_SetProfile(DD_STM_PROF_ID profileID, US* pProfile);
void DD_STM_SetProfileID(DD_STM_ID motorID, DD_STM_PROF_ID profileID);
void DD_STM_StartClock(DD_STM_ID motorID, US targetIndex, void (*func)(SS result));
void DD_STM_ChangeClock(DD_STM_ID motorID, US targetIndex, void (*func)(SS result));
void DD_STM_MoveBySetPulse(DD_STM_ID motorID, US targetIndex, US totalPulseCnt, void (*func)(SS result));
void DD_STM_SlowDownStop(DD_STM_ID motorID, void (*func)(SS result));
void DD_STM_ShutDownStop(DD_STM_ID motorID);
void DD_STM_StartCountClock(DD_STM_ID motorID);
US DD_STM_StopCountClock(DD_STM_ID motorID);
void DD_STM_AppointPulseTiming(DD_STM_ID motorID, US pulseCnt, void (*func)(SS result));

#endif
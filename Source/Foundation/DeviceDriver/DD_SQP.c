// File Name: DD_SQP.c
// Description:
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 13.09.11
// Revision:  010
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+--------------------------------------------------------------------------------------
// 12.08.05 | Nishizawa Mitsuyuki | Created this file.
// Revision 001

#define DD_SQP_C

#include "Common.h"
#include "DD_Common.h"
#include "DD_TAU.h"
#include "DD_Event.h"
#include "LIB_Lock.h"
#include "DD_SQP.h"
#include "DD_ResourceAssign.h"

#define DD_SQP_BASE_CLOCK_FREQ		1000000				// [×1Hz]

//============================================================================= 
// Function Name : DD_SQP_Init
// Description   : タイマチャネルの設定初期化
// Parameter     : void
// Return        : void
// Note          : -
//=============================================================================
void DD_SQP_Init(void)
{
	UC motorIndex = 0;
	
	for (motorIndex = 0; motorIndex < DD_SQP_ID_NUM; motorIndex++) {
		DD_TAU_InitByChannel(gDD_SQP_CtrlTable[motorIndex].TAU_CH, DD_TAU_MODE_SQUARE_PULSE_ID, DD_TAU_ACTIVE_HIGH_ID);
	}
}

//============================================================================= 
// Function Name : DD_SQP_SetFrequency
// Description   : 周波数とDUTYの設定
// Parameter     : motorID		"モータID"
//               : frequency	"周波数[×0.1Hz]"
// Return        : void
// Note          : -
//=============================================================================
void DD_SQP_SetFrequency(UC motorID, UL frequency)
{
	UL tdrVal = ((DD_SQP_BASE_CLOCK_FREQ * 10 / frequency) / 2) - 1;
	
	DD_TAU_SetTDR(gDD_SQP_CtrlTable[motorID].TAU_CH, (US)tdrVal);
}

//============================================================================= 
// Function Name : DD_SQP_Start
// Description   : Pwm出力の開始	
// Parameter     : 
// Return        : 
// Note          : 
//=============================================================================
void DD_SQP_Start(UC motorID)
{
	DD_TAU_Start(gDD_SQP_CtrlTable[motorID].TAU_CH);
}

//============================================================================= 
// Function Name : DD_SQP_Stop
// Description   : Pwm出力の停止
// Parameter     : motorID
// Return        : 
// Note          : 
//=============================================================================
void DD_SQP_Stop(UC motorID)
{
	DD_TAU_Stop(gDD_SQP_CtrlTable[motorID].TAU_CH);
}

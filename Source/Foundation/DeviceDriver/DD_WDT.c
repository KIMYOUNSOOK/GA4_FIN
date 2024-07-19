// File Name: DD_WDT.c
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

#include "Common.h"
#include "DD_WDT.h"
#include "DD_DIO.h"

//============================================================================= 
// Function Name : WDT_Start
// Description   : WatchDogTimerの周期Reset処理開始
// Parameter     : -
// Return        : -
// Note          : -
//=============================================================================
void DD_WDT_Start(void)
{
	DD_EVT_Put(evStart_ID, evuDD_WDT_ID, 0, 0);
}

//============================================================================= 
// Function Name : WDT_Stop
// Description   : WatchDogTimerの周期Reset処理停止
// Parameter     : -
// Return        : -
// Note          : 用途未定
//=============================================================================
void DD_WDT_Stop(void)
{
	DD_EVT_Put(evStop_ID, evuDD_WDT_ID, 0, 0);
}


//============================================================================= 
// Function Name : WDT_Clear
// Description   : WDTをリセットし、Digital Output信号をトグル出力する
// Parameter     : -
// Return        : -
// Note          : -
//=============================================================================
void DD_WDT_Clear(void)
{
	static UC toggle = 0;
    WDTE = 0xACU;								// Timer Reset
	
	if (toggle == 0) {
		DD_DIO_SetOutput(DD_DO_WDT_CLR, ON);	// WDT Toggle
		toggle = 1;
	}
	else {
		DD_DIO_SetOutput(DD_DO_WDT_CLR, OFF);	// WDT Toggle
		toggle = 0;
	}
}

//============================================================================= 
// Function Name : WDT_Notify
// Description   : Event通知関数
// Parameter     : DD_EventID eventId : EventのID値
//				   UC* pMsg	   : データポインタ
// Return        : -
// Note          : -
//=============================================================================
void DD_WDT_Notify(DD_EventID eventId, UC* pMsg)
{
	switch (eventId) {
	case evStart_ID:
	case evTimeout_ID:
		DD_WDT_Clear();
		DD_EVT_SetTimer(50, evuDD_WDT_ID, tmDD_WDT_ID, 0);
		break;
	case evStop_ID:
		DD_EVT_CancelTimer(tmDD_WDT_ID);
		break;
	default:
		break;
	}
}


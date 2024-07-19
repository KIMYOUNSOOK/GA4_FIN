// File Name: DD_Event.h
// Description:
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 15.03.27
// Revision:  001
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+--------------------------------------------------------------------------------------
// 13.10.15 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev003 Base)
// Revision 000
// 15.01.28 | Atsushi Morikawa    | Log Function Addition
// 13.03.27 | Hoshino Yuichi      | PARポーリング処理削除
// Revision 001

#ifndef LIB_EVENT_H
#define LIB_EVENT_H

typedef enum{
	evuDD_STM_ID,
	evuDD_WDT_ID,
	evuDD_DIO_ID,
	evuDD_COM_ParPacketReceiver_ID,
	evuDD_COM_ParPacketSender_ID,
	evuDD_COM_ParDataReceiver_ID,
	evuDD_COM_ParDataSender_ID,
	evuDD_LOG_ID,
	evuDD_NVM_ID,
#ifdef DEBUG
	evuDebug_ID,
#endif
	evuNone_ID,
	evuTypeLast_ID		// これより上に追加すること
} DD_EventUserID;

// Eventは追加しないこと
typedef enum{
	evGoIdle_ID,
	evGoStop_ID,
	evGoComplete_ID,
	evGoFail_ID,
	evGoCancel_ID,
	evGoStart_ID,
	evGoNext_ID, 	
	// evGo**イベントはこれより上に追加すること
	
	evTimeout_ID,
	evStart_ID,
	evStop_ID,
	evComplete_ID,
	evRead_ID,
	evWrite_ID,
	evSend_ID,
	evReceive_ID,
	evAcknowledge_ID,
	evSync_ID,
	
	evCancel_ID,
	evReset_ID,
	evFail_ID,
	evErr_ID,
	
	evNoEvent_ID,	// Eventはこれより上に追加すること
} DD_EventID;

typedef enum {
	tmMSP_LOC_ID,
//	tmStepMot0_ID,
//	tmStepMot1_ID,
//	tmStepMot2_ID,
//	tmStepMot3_ID,
//	tmStepMot4_ID,
	tmDD_WDT_ID,
	tmDD_NVM_ID,
	tmDD_COM_ParPacketSender_ID,
//	tmParDataSender_ID,
	tmDD_COMParSciPacketReceiver_ID,
//	tmParDataReceiver_ID,
	tmDD_COM_ParPolling_ID,
#ifdef DEBUG
	tmDebug_ID,
#endif

	timerLast_ID
} DD_EventTimerID;

// 外部参照関数のプロトタイプ宣言
void DD_EVT_Init(void);
callt void DD_EVT_Receive(void);
callt void DD_EVT_Put(DD_EventID eventID, DD_EventUserID userID, UC *pMsg, SS size);
void DD_EVT_SetTimer(US time, DD_EventUserID userID, DD_EventTimerID eventTimerID, void* arg);
void DD_EVT_CancelTimer(DD_EventTimerID eventTimerID);
UL DD_EVT_GetCurrentTime();

// MSPに提供するAPI(その他は使用不可)
void DD_EVT_EntryCallback(void (*func)());

#endif

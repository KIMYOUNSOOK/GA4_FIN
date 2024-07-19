// File Name: DD_Event.c
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
// 13.10.15 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev006 Base)
// Revision 000
// 15.01.28 | Atsushi Morikawa    | Log Function Addition
// 13.03.27 | Hoshino Yuichi      | 外付けフラッシュ対応
// 13.03.27 | Hoshino Yuichi      | PARポーリング処理削除
// Revision 001
// 15.04.01	| Shin.KS			  | 10ms周期処理を1ms周期処理変更(Interruptを使用しないSensor処理のため)
// Revision 002(TBD)

#include "Common.h"
#include "LIB_Service.h"
#include "DD_ResourceAssign.h"
#include "DD_Event.h"
#include "DD_INT.h"
#include "DD_STM.h"
#include "DD_DIO.h"
#include "DD_WDT.h"
#include "DD_TAU.h"

#include "DD_COM_ParPacketSender.h"
#include "DD_COM_ParDataSender.h"
#include "DD_COM_ParPacketReceiver.h"
#include "DD_COM_ParDataReceiver.h"

#include "DD_LOG.h"
#include "DD_NVM.h"

#ifdef DEBUG
void Debug_Notify(DD_EventID eventID, void* arg);
#endif

// ◆ Event
typedef struct  {
	DD_EventID eventID;
	DD_EventUserID userID;
	UC* pMsg;
} DD_EVT_TYPE;

static UL timerCnt1ms = 0;

// Notify関数を追加する場合には"Event.h"の"enum EventUserID"に
// EventUser_IDを追加し、Notify()とEventUser_IDの並びを同じにすること
void (* const notifyFunc[evuTypeLast_ID])(DD_EventID, UC*) = {
	DD_STM_Notify,
	DD_WDT_Notify,
	DD_DIO_Notify,
	DD_COM_ParPacketReceiverNotify,
	DD_COM_ParPacketSenderNotify,
	DD_COM_ParDataReceiverNotify,
	DD_COM_ParDataSenderNotify,
	DD_LOG_Notify,
	DD_NVM_Notify,
#ifdef DEBUG
//	Debug_Notify
#endif
} ;

typedef enum {
	tsUnused_ID,		// 未使用状態
	tsTick_ID,			// 計測中
	tsCanceled_ID,		// キャンセル済み
	tsTimeout_ID		// タイムアウト中（イベントキューに登録済み）
} DD_EVT_TIMER_STATE;
// ◆ EventTimer
typedef struct{
	DD_EventUserID userID;
	UC eventTimerID;
	DD_EVT_TIMER_STATE state;
	UC lapCount;		// 複数Timeoutした場合に後優先を実現するためのカウンタ（現状複数Timeoutイベントがキューに存在することがあり得るため）
	US count;
	void* arg;
} DD_EVT_TIMER_TYPE;

static DD_EVT_TIMER_TYPE timerQueue[timerLast_ID];
void   (*TMRFUNC)();     															 	/* MSP 1msカウント関数へのポインタ     */

static void DD_EVT_Timeout(void* arg);

#ifdef DEBUG
US dbgCnt = 0;
#endif

//=============================================================================
// Function Name : DD_EVT_Receive
// Description   : EventQueueからEventを取り出し宛先へEventを通知する
// Parameter     : -
// Return        : -
// Note          : MainLoopからCallされる
//=============================================================================
callt void DD_EVT_Receive(void)
{
	UC* pMsg;
	DD_EVT_TYPE* pEvent;

	while ( LIB_rcv_mbf(eMbfDdHigh, &pMsg) > 0 ) {										// High PriorityのEventを取り出し終えるまで
		pEvent = (DD_EVT_TYPE*)pMsg;
		notifyFunc[(UC)(pEvent->userID)](pEvent->eventID, pEvent->pMsg);
	}

	if ( LIB_rcv_mbf(eMbfDdLow, &pMsg) > 0 ) {											// Low PriorityのEvent取り出し実施
		pEvent = (DD_EVT_TYPE*)pMsg;
		if ((pEvent->eventID == evTimeout_ID)) {										// Timeout後(EnQueue後)にEvent_CancelTimerした場合
			timerQueue[*(pEvent->pMsg)].lapCount --;
			if ((timerQueue[*(pEvent->pMsg)].state == tsCanceled_ID)
		     || (timerQueue[*(pEvent->pMsg)].lapCount > 0)) {							// キャンセル済みの場合、もしくは１つのTimer IDに対して複数Timeoutが存在する場合
			 																			// ※１つのTimer IDに対してTimeoutは１つのみ有効である（後優先）
				return ;
			}
		}
		if ( ((UC)pEvent->userID) < evuTypeLast_ID ) {
			notifyFunc[(UC)(pEvent->userID)](pEvent->eventID, pEvent->pMsg);
		}
	}
}

//=============================================================================
// Function Name : DD_EVT_Put
// Description   : EventをEventQueue登録する
// Parameter     : eventID : EventのID
//               : userID  : 宛先のEventUserのID
//               : pMsg    : 宛先に通知するデータの先頭ポインタ
//               : size    : 宛先に通知するデータのサイズ
// Return        : -
// Note          : 呼び出し頻度が高いためにcalltを宣言して実行速度を向上
//=============================================================================
callt void DD_EVT_Put(DD_EventID eventID, DD_EventUserID userID, UC *pMsg, SS size)
{
	DD_EVT_TYPE event;

	event.eventID	= eventID;
	event.userID	= userID;
	event.pMsg		= pMsg;
	size += 2;										// eventIDとuserID分加算

	if ( eventID > evGoNext_ID ) {					// Low Priorityの場合
		LIB_snd_mbf(eMbfDdLow, (UC*)&event, sizeof(event));
	}
	else {											// High Priorityの場合
		LIB_snd_mbf(eMbfDdHigh, (UC*)&event, sizeof(event));
	}
}

//=============================================================================
// Function Name : DD_EVT_SetTimer
// Description   : TimerTableにタイマを登録する
// Parameter     : time          : 設定時間
//               : userID        : timeoutの通知先のEventUserのID(自EventUserのID)
//               : eventTimerID  : timerのID(Timeout時にはNotify関数の引数に設定される)
// Return        : -
// Note          : 重複セットは後優先
//=============================================================================
void DD_EVT_SetTimer(US time, DD_EventUserID userID, DD_EventTimerID eventTimerID, void *arg)
{
	DD_EVT_TIMER_TYPE* pTimer = &timerQueue[eventTimerID];
	UC lockKey = 0;

	DD_EVT_CancelTimer(eventTimerID);				// 後優先とするため前のタイマを停止

	lockKey = LIB_GetKey();
	LIB_Lock();
	pTimer->count = time;
	pTimer->arg = arg;

	if ( pTimer->count == 0 ) {
		pTimer->count = 1;
	}

	pTimer->eventTimerID = eventTimerID;
	pTimer->userID = userID;
	pTimer->state = tsTick_ID;
	pTimer->lapCount ++;

	LIB_Unlock(lockKey);
}

//=============================================================================
// Function Name : DD_EVT_CancelTimer
// Description   : 設定したTimerをキャンセルする
// Parameter     : eventTimerID : timerのID
// Return        : -
// Note          : コールした時に既にTimeoutしていてEventQueueにTimeoutイベントが積まれていても
//               : そのTimeoutイベントが通知されないように工作する
//=============================================================================
void DD_EVT_CancelTimer(DD_EventTimerID eventTimerID)
{
	DD_EVT_TYPE* pEvent;
	DD_EVT_TIMER_TYPE* pTimer = &timerQueue[eventTimerID];
	UC lockKey = 0;

	lockKey = LIB_GetKey();
	LIB_Lock();

	if (pTimer->state == tsTick_ID) {
		pTimer->state = tsCanceled_ID;
		pTimer->count = 0;
		pTimer->lapCount--;
	}
	else if (pTimer->state == tsTimeout_ID) {
		pTimer->state = tsCanceled_ID;
	}

	LIB_Unlock(lockKey);

}

//=============================================================================
// Function Name : DD_EVT_Init
// Description   :
// Parameter     :
// Return        :
// Note          :
//=============================================================================
void DD_EVT_Init(void)
{
	UC index = 0;

	DD_TAU_EntryCallback(DD_SOFTWARE_TIMER_CH, DD_EVT_Timeout, 0);
	TMRFUNC = NULL;

	// TimerQueueの初期化
	for ( index = 0; index < timerLast_ID; index++ ) {
		timerQueue[index].count = 0;
		timerQueue[index].lapCount = 0;
	}
}

//=============================================================================
// Function Name : DD_EVT_GetCurrentTime
// Description   :
// Parameter     :
// Return        :
// Note          :
//=============================================================================
UL DD_EVT_GetCurrentTime(void)
{
	return timerCnt1ms;
}

//=============================================================================
// Function Name : DD_EVT_Timeout
// Description   : -
// Parameter     : -
// Return        : -
// Note          : Timer割込み内からコールされる
//=============================================================================
static void DD_EVT_Timeout(void* arg)
{
	UC id;
	static UC cnt = 0;
	DD_EVT_TIMER_TYPE* pTimer = &timerQueue[0];

	timerCnt1ms++;

	for ( id = 0; id < timerLast_ID; id++ ) {
		if (pTimer->state == tsTick_ID) {
			if ( (pTimer->count -= 1) == 0 ) {
				pTimer->state = tsTimeout_ID;
				DD_EVT_Put(evTimeout_ID, pTimer->userID, (UC*)&(pTimer->eventTimerID), 1);
			}
		}
		pTimer++;
	}

	// 10ms周期処理を1ms周期処理変更(Interruptを使用しないSensor処理のため), By Shin.KS
	// if ( ++cnt == 10 ) {
	//	DD_EVT_Put(evStart_ID, evuDD_DIO_ID, 0, 0);
	//	cnt = 0;
	// }
	DD_EVT_Put(evStart_ID, evuDD_DIO_ID, 0, 0);


	if ( TMRFUNC != NULL ) {														// MSPのイベントタイマーカウント処理を実行
		TMRFUNC();
	}
}

//=============================================================================
// Function Name : DD_EVT_EntryCallback
// Description   : コールバック関数の登録
// Parameter     :
// Return        :
// Note          :
//=============================================================================
void DD_EVT_EntryCallback(void (*func)())
{
	TMRFUNC = func ;
}

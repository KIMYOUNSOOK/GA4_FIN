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
// 13.03.27 | Hoshino Yuichi      | �O�t���t���b�V���Ή�
// 13.03.27 | Hoshino Yuichi      | PAR�|�[�����O�����폜
// Revision 001
// 15.04.01	| Shin.KS			  | 10ms����������1ms���������ύX(Interrupt���g�p���Ȃ�Sensor�����̂���)
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

// �� Event
typedef struct  {
	DD_EventID eventID;
	DD_EventUserID userID;
	UC* pMsg;
	
} DD_EVT_TYPE;

static UL timerCnt1ms = 0;

// Notify�֐���ǉ�����ꍇ�ɂ�"Event.h"��"enum EventUserID"��
// EventUser_ID��ǉ����ANotify()��EventUser_ID�̕��т𓯂��ɂ��邱��
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
	tsUnused_ID,		// ���g�p���
	tsTick_ID,			// �v����
	tsCanceled_ID,		// �L�����Z���ς�
	tsTimeout_ID		// �^�C���A�E�g���i�C�x���g�L���[�ɓo�^�ς݁j
} DD_EVT_TIMER_STATE;
// �� EventTimer
typedef struct{
	DD_EventUserID userID;
	UC eventTimerID;
	DD_EVT_TIMER_STATE state;
	UC lapCount;		// ����Timeout�����ꍇ�Ɍ�D����������邽�߂̃J�E���^�i���󕡐�Timeout�C�x���g���L���[�ɑ��݂��邱�Ƃ����蓾�邽�߁j
	US count;
	void* arg;
} DD_EVT_TIMER_TYPE;

static DD_EVT_TIMER_TYPE timerQueue[timerLast_ID];
void   (*TMRFUNC)();     															 	/* MSP 1ms�J�E���g�֐��ւ̃|�C���^     */

static void DD_EVT_Timeout(void* arg);

#ifdef DEBUG
US dbgCnt = 0;
#endif

//=============================================================================
// Function Name : DD_EVT_Receive
// Description   : EventQueue����Event�����o�������Event��ʒm����
// Parameter     : -
// Return        : -
// Note          : MainLoop����Call�����
//=============================================================================
callt void DD_EVT_Receive(void)
{
	UC* pMsg;
	DD_EVT_TYPE* pEvent;

	while ( LIB_rcv_mbf(eMbfDdHigh, &pMsg) > 0 ) {										// High Priority��Event�����o���I����܂�
		pEvent = (DD_EVT_TYPE*)pMsg;
		notifyFunc[(UC)(pEvent->userID)](pEvent->eventID, pEvent->pMsg);
	}

	if ( LIB_rcv_mbf(eMbfDdLow, &pMsg) > 0 ) {											// Low Priority��Event���o�����{
		pEvent = (DD_EVT_TYPE*)pMsg;
		if ((pEvent->eventID == evTimeout_ID)) {										// Timeout��(EnQueue��)��Event_CancelTimer�����ꍇ
			timerQueue[*(pEvent->pMsg)].lapCount --;
			if ((timerQueue[*(pEvent->pMsg)].state == tsCanceled_ID)
		     || (timerQueue[*(pEvent->pMsg)].lapCount > 0)) {							// �L�����Z���ς݂̏ꍇ�A�������͂P��Timer ID�ɑ΂��ĕ���Timeout�����݂���ꍇ
			 																			// ���P��Timer ID�ɑ΂���Timeout�͂P�̂ݗL���ł���i��D��j
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
// Description   : Event��EventQueue�o�^����
// Parameter     : eventID : Event��ID
//               : userID  : �����EventUser��ID
//               : pMsg    : ����ɒʒm����f�[�^�̐擪�|�C���^
//               : size    : ����ɒʒm����f�[�^�̃T�C�Y
// Return        : -
// Note          : �Ăяo���p�x���������߂�callt��錾���Ď��s���x������
//=============================================================================
callt void DD_EVT_Put(DD_EventID eventID, DD_EventUserID userID, UC *pMsg, SS size)
{
	DD_EVT_TYPE event;

	event.eventID	= eventID;
	event.userID	= userID;
	event.pMsg		= pMsg;
	size += 2;										// eventID��userID�����Z

	if ( eventID > evGoNext_ID ) {					// Low Priority�̏ꍇ
		LIB_snd_mbf(eMbfDdLow, (UC*)&event, sizeof(event));
	}
	else {											// High Priority�̏ꍇ
		LIB_snd_mbf(eMbfDdHigh, (UC*)&event, sizeof(event));
	}
}

//=============================================================================
// Function Name : DD_EVT_SetTimer
// Description   : TimerTable�Ƀ^�C�}��o�^����
// Parameter     : time          : �ݒ莞��
//               : userID        : timeout�̒ʒm���EventUser��ID(��EventUser��ID)
//               : eventTimerID  : timer��ID(Timeout���ɂ�Notify�֐��̈����ɐݒ肳���)
// Return        : -
// Note          : �d���Z�b�g�͌�D��
//=============================================================================
void DD_EVT_SetTimer(US time, DD_EventUserID userID, DD_EventTimerID eventTimerID, void *arg)
{
	DD_EVT_TIMER_TYPE* pTimer = &timerQueue[eventTimerID];
	UC lockKey = 0;

	DD_EVT_CancelTimer(eventTimerID);				// ��D��Ƃ��邽�ߑO�̃^�C�}���~

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
// Description   : �ݒ肵��Timer���L�����Z������
// Parameter     : eventTimerID : timer��ID
// Return        : -
// Note          : �R�[���������Ɋ���Timeout���Ă���EventQueue��Timeout�C�x���g���ς܂�Ă��Ă�
//               : ����Timeout�C�x���g���ʒm����Ȃ��悤�ɍH�삷��
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

	// TimerQueue�̏�����
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
// Note          : Timer�����ݓ�����R�[�������
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

	// 10ms����������1ms���������ύX(Interrupt���g�p���Ȃ�Sensor�����̂���), By Shin.KS
	// if ( ++cnt == 10 ) {
	//	DD_EVT_Put(evStart_ID, evuDD_DIO_ID, 0, 0);
	//	cnt = 0;
	// }
	DD_EVT_Put(evStart_ID, evuDD_DIO_ID, 0, 0);


	if ( TMRFUNC != NULL ) {														// MSP�̃C�x���g�^�C�}�[�J�E���g���������s
		TMRFUNC();
	}
}

//=============================================================================
// Function Name : DD_EVT_EntryCallback
// Description   : �R�[���o�b�N�֐��̓o�^
// Parameter     :
// Return        :
// Note          :
//=============================================================================
void DD_EVT_EntryCallback(void (*func)())
{
	TMRFUNC = func ;
}

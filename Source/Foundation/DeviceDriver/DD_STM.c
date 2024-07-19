// File Name: DD_STM.c
// Description:
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 13.10.31
// Revision:  000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+--------------------------------------------------------------------------------------
// 13.10.31 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev015 Base)
// Revision 000

#define DD_STM_C

#include "Common.h"
#include "DD_Common.h"
#include "DD_TAU.h"
#include "DD_DMA.h"
#include "DD_Event.h"
#include "LIB_Lock.h"
#include "DD_STM.h"
#include "DD_ResourceAssign.h"

#if 0
#ifndef STARTER_KIT
#define DD_STM_PROF_STEP_MAX			128
#define DD_STM_PROF_NUM_MAX				8
#else
#define DD_STM_PROF_STEP_MAX			32
#define DD_STM_PROF_NUM_MAX				4
#endif
#else
#define DD_STM_PROF_STEP_MAX			512


#endif

#ifndef STARTER_KIT
#define DD_STM_PROF_CONST_STEP_SIZE		16
#else
#define DD_STM_PROF_CONST_STEP_SIZE		8
#endif

#define DD_STM_NOT_COUNT_CLOCK			0
#define DD_STM_COUNT_CLOCK				1
#define DD_STM_PULSE_TIMING				2

#define DD_STM_BASE_CLOCK_FREQ			1000000				// [�~1Hz]

typedef void (*DD_VOID_FUNC_SS_TYPE)(SS);

// Motor State
typedef enum{
	stIdle_ID,
	stChangePulseWait_ID,
	stChangeEntry_ID,
	stChangeExecute_ID,
	stChangeAnswer_ID,
	stConstant_ID,
	stPTPulseWait_ID,
	stPTEntry_ID,
	stPTExec_ID,
	stPTExecPulseWait_ID,
	stPTJudge_ID,
	stPTAnswer_ID,
	stStopPulseWait_ID,
	stStopEntry_ID,
	stStopExecute_ID,
	stStopExecPulseWait_ID,
	stStopAnswer_ID,
	stSPPulseWait_ID,
	stSPChangeEntry_ID,
	stSPChange_ID,
	stSPChangePulseWait_ID,
	stSPConstEntry_ID,
	stSPConstant_ID,
	stSPConstPulseWait_ID,
	stSPConstJudge_ID,
	stSPStopEntry_ID,
	stSPStop_ID,
	stSPStopPulseWait_ID,
	stSPAnswer_ID,
	stShutDown_ID,
} DD_STM_StateID;

#define DD_STM_RECEIVE_DATA_NUM	4

typedef struct{
	US 						targetIndex;
	US 						totalPulseCnt;
	DD_VOID_FUNC_SS_TYPE	func;
} DD_STM_ReceiveData;

typedef struct{
	UC						controlType;
	US						countStartIndex;
	US						clockCount;
	US						remain;
	DD_VOID_FUNC_SS_TYPE	func;
} DD_STM_CountClockData;

typedef struct{
	DD_STM_StateID			state;

	DD_STM_PROF_ID			profileID;
	UC						lastReceiveNo;
	DD_STM_ReceiveData		receiveData;
	DD_STM_ReceiveData		entryData;

	US						nowIndex;
	US						targetIndex;
	US 						spChangePulseCnt;
	US						spConstPulseCnt;
	US						constTable[DD_STM_PROF_CONST_STEP_SIZE];
	US						spSlowDownPulseCnt;
	SS						spResult;
	DD_VOID_FUNC_SS_TYPE	func;
	DD_STM_CountClockData	count;
} DD_STM_MotorManager;

DD_STM_MotorManager gDD_STM_MotorManager[DD_STM_ID_NUM];

typedef struct {
	DD_STM_PROF_ID	profileID;
	US				idleIndex;
	US				slowUpDownThresh;											// Slow-Up,Slow-Down�C���f�b�N�X��臒l(臒l�����FSlow-Up�C���f�b�N�X�A臒l�ȏ�FSlow-Down�C���f�b�N�X)
	US				profEndIndex;
} DD_STM_Profile;
DD_STM_Profile gDD_STM_Profile[DD_STM_PROF_ID_NUM];

US gDD_STM_ProfileTable[DD_STM_PROF_STEP_MAX * 2];								// High��ԁALow��Ԃ������߁~2���Ă���
US gDD_STM_ConvertIndexTable[DD_STM_PROF_STEP_MAX];


// �� Prototype(Used Only in DD_STM.c)
// ��ԑJ�ڃC�x���g����
static void stIdle(DD_STM_ID motorID);
static void stChangePulseWait(DD_STM_ID motorID);
static void stChangeEntry(DD_STM_ID motorID);
static void stChangeExecute(DD_STM_ID motorID);
static void stChangeAnswer(DD_STM_ID motorID);
static void stConstant(DD_STM_ID motorID);
static void stStopPulseWait(DD_STM_ID motorID);
static void stStopEntry(DD_STM_ID motorID);
static void stStopExecute(DD_STM_ID motorID);
static void stStopExecPulseWait(DD_STM_ID motorID);
static void stStopAnswer(DD_STM_ID motorID);
static void stSPPulseWait(DD_STM_ID motorID);
static void stSPChangeEntry(DD_STM_ID motorID);
static void stSPChange(DD_STM_ID motorID);
static void stSPChangePulseWait(DD_STM_ID motorID);
static void stSPConstEntry(DD_STM_ID motorID);
static void stSPConstant(DD_STM_ID motorID);
static void stSPConstPulseWait(DD_STM_ID motorID);
static void stSPConstJudge(DD_STM_ID motorID);
static void stSPStopEntry(DD_STM_ID motorID);
static void stSPStop(DD_STM_ID motorID);
static void stSPStopPulseWait(DD_STM_ID motorID);
static void stSPAnswer(DD_STM_ID motorID);
static void stShutDown(DD_STM_ID motorID);
static void stPTPulseWait(DD_STM_ID motorID);
static void stPTEntry(DD_STM_ID motorID);
static void stPTExec(DD_STM_ID motorID);
static void stPTExecPulseWait(DD_STM_ID motorID);
static void stPTJudge(DD_STM_ID motorID);
static void stPTAnswer(DD_STM_ID motorID);

// �v���t�@�C��ID�Z�b�g
static void SetProfileIDExec(DD_STM_ID motorID, DD_STM_PROF_ID profileID);
// �J�E���g�N���b�N�f�[�^
static void SetCountStartIndex(DD_STM_ID motorID);

// �N���b�N����
static bool SetChangeClockData(DD_STM_ID motorID);
static bool SetStopClockData(DD_STM_ID motorID);

// ���荞�݃R�[���o�b�N
static void TransferEndCallback(void* arg);
static void TimeoutCallback(void* arg);

//============================================================================= 
// Function Name : DD_STM_Init
// Description   : �X�e�b�s���O���[�^�̏�����
// Parameter     : -
// Return        : -
// Note          : �����p�����[�^�A�g�p����TAU�ADMA�̃��W�X�^������������
//=============================================================================
void DD_STM_Init(void)
{
	UC motorID, profID;
	US loopCnt;

	// ���[�^�֘A�f�[�^�̏�����
	for( motorID = 0; motorID < DD_STM_ID_NUM; motorID++ ){
		// TAU�ADMA�̃��W�X�^�ݒ�A�R�[���o�b�N�ݒ�
		DD_TAU_EntryCallback(gDD_STM_CtrlTable[motorID].timerCH, TimeoutCallback, (void*)motorID);
		DD_TAU_InitByChannel(gDD_STM_CtrlTable[motorID].timerCH, DD_TAU_MODE_SQUARE_PULSE_ID, DD_TAU_ACTIVE_HIGH_ID);

		DD_DMA_EntryCallback(gDD_STM_CtrlTable[motorID].dmacCH, TransferEndCallback, (void*)motorID);
		DD_DMA_InitByChannel(gDD_STM_CtrlTable[motorID].dmacCH, DD_DMA_TRIGGER_TYPE_TM, gDD_STM_CtrlTable[motorID].timerCH);

		// gDD_STM_MotorManager�̏�����
		gDD_STM_MotorManager[motorID].state = stIdle_ID;
		gDD_STM_MotorManager[motorID].profileID = DD_STM_PROF_ID_NUM;
		gDD_STM_MotorManager[motorID].receiveData.targetIndex = 0;
		gDD_STM_MotorManager[motorID].receiveData.totalPulseCnt = 0;
		gDD_STM_MotorManager[motorID].receiveData.func = NULL;
		gDD_STM_MotorManager[motorID].entryData.targetIndex = 0;
		gDD_STM_MotorManager[motorID].entryData.totalPulseCnt = 0;
		gDD_STM_MotorManager[motorID].entryData.func = NULL;
		gDD_STM_MotorManager[motorID].nowIndex = 0;
		gDD_STM_MotorManager[motorID].targetIndex = 0;
		gDD_STM_MotorManager[motorID].spChangePulseCnt = 0;
		gDD_STM_MotorManager[motorID].spConstPulseCnt = 0;
		for( loopCnt = 0; loopCnt < DD_STM_PROF_CONST_STEP_SIZE; loopCnt++ ){
			gDD_STM_MotorManager[motorID].constTable[loopCnt] = 0;
		}
		gDD_STM_MotorManager[motorID].spSlowDownPulseCnt = 0;
		gDD_STM_MotorManager[motorID].spResult = DD_STM_RESULT_OK;
		gDD_STM_MotorManager[motorID].func = NULL;
	}

	// �v���t�@�C���f�[�^�̏�����
	for( profID = 0; profID < DD_STM_PROF_ID_NUM; profID++ ){
		gDD_STM_Profile[profID].profileID = DD_STM_PROF_ID_NUM;
		gDD_STM_Profile[profID].idleIndex = 0;
		gDD_STM_Profile[profID].slowUpDownThresh = 0;
		gDD_STM_Profile[profID].profEndIndex = 0;
	}

	// �v���t�@�C���e�[�u���̏�����
	for( loopCnt = 0; loopCnt < (DD_STM_PROF_STEP_MAX * 2); loopCnt++ ){
		gDD_STM_ProfileTable[loopCnt] = 0;
	}

	// �C���f�b�N�X�ϊ��e�[�u���̏�����
	for( loopCnt = 0; loopCnt < DD_STM_PROF_STEP_MAX; loopCnt++ ){
		gDD_STM_ConvertIndexTable[loopCnt] = 0;
	}
}

//============================================================================= 
// Function Name : DD_STM_SetProfile
// Description   : �v���t�@�C���ݒ�
// Parameter     : profileID	"�v���t�@�C���e�[�u����ID"
//               : pProfile		"�v���t�@�C���e�[�u���̃|�C���^"
// Return        : -
// Note          : -
//=============================================================================
void DD_STM_SetProfile(DD_STM_PROF_ID profileID, US* pProfile)
{
	static US profIndex = 0;
	US freqData;
	US lastProfData = 0;
	US loopIndex, searchIndex;
	US* pProfAdd = pProfile;

	if( profileID >= DD_STM_PROF_ID_NUM ){
		return;																	// ��`����Ă��Ȃ�profileID���w�肳��Ă���(Irregular Case)
	}

	if( gDD_STM_Profile[profileID].profileID == profileID ){
		return;																	// 1�x�v���t�@�C���ݒ肪����Ă���(Irregular Case)
	}

	for( loopIndex = 0; *pProfAdd != 0; loopIndex++, pProfAdd++ ){
	}

	if( (profIndex + loopIndex * 2)  > (DD_STM_PROF_STEP_MAX * 2) ){
		return;																	// ProfileTable��RAM�e�ʂ𒴂���(Irregular Case)
	}

	// �v���t�@�C���e�[�u���Ƀ^�C�}�l�f�[�^���Z�b�g
	// Idle���Z�b�g
	gDD_STM_Profile[profileID].profileID = profileID;
	gDD_STM_Profile[profileID].idleIndex = profIndex;

	gDD_STM_ProfileTable[profIndex] = 0;										// Low��Ԃ̐ݒ�
	profIndex++;

	gDD_STM_ProfileTable[profIndex] = 0;										// High��Ԃ̐ݒ�
	profIndex++;

	// Slow-Up�C���f�b�N�X�����Z�b�g
	for( ; *pProfile >= lastProfData; pProfile++ ){
		freqData = (US)(((UL)(DD_STM_BASE_CLOCK_FREQ * 10) / (UL)(*pProfile)) / 2) - 1 ;

		gDD_STM_ProfileTable[profIndex] = freqData;								// Low��Ԃ̐ݒ�
		profIndex++;

		gDD_STM_ProfileTable[profIndex] = freqData;								// High��Ԃ̐ݒ�
		profIndex++;

		lastProfData = *pProfile;
	}

	gDD_STM_Profile[profileID].slowUpDownThresh = profIndex;

	// Slow-Down�C���f�b�N�X�����Z�b�g
	for( ; *pProfile != 0; pProfile++ ){
		freqData = (US)(((UL)(DD_STM_BASE_CLOCK_FREQ * 10) / (UL)(*pProfile)) / 2) - 1 ;

		gDD_STM_ProfileTable[profIndex] = freqData;								// Low��Ԃ̐ݒ�
		profIndex++;

		gDD_STM_ProfileTable[profIndex] = freqData;								// High��Ԃ̐ݒ�
		profIndex++;
	}

	gDD_STM_Profile[profileID].profEndIndex = profIndex - 2;

	// �C���f�b�N�X�ϊ��e�[�u���ɃC���f�b�N�X�f�[�^���Z�b�g
	// Idle�C���f�b�N�X�̕ϊ��C���f�b�N�X���Z�b�g
	gDD_STM_ConvertIndexTable[gDD_STM_Profile[profileID].idleIndex / 2] = gDD_STM_Profile[profileID].idleIndex;

	// Slow-Up��Slow-Down�̕ϊ��C���f�b�N�X���Z�b�g
	for( loopIndex = gDD_STM_Profile[profileID].idleIndex + 2; loopIndex <= gDD_STM_Profile[profileID].slowUpDownThresh; loopIndex += 2 ){
		for( searchIndex = gDD_STM_Profile[profileID].slowUpDownThresh; searchIndex <= gDD_STM_Profile[profileID].profEndIndex; searchIndex += 2 ){
			if( gDD_STM_ProfileTable[searchIndex] > gDD_STM_ProfileTable[loopIndex] ){
				gDD_STM_ConvertIndexTable[loopIndex / 2] = searchIndex;
				break;
			}
			else{
				/* No Action */
			}
		}

		if( searchIndex > gDD_STM_Profile[profileID].profEndIndex ){			// �ϊ��Ώۂ�Slow-Down�C���f�b�N�X��������Ȃ��Ƃ�
			gDD_STM_ConvertIndexTable[loopIndex / 2] = gDD_STM_Profile[profileID].idleIndex;	//idleIndex��ݒ肷��
		}
	}

	// Slow-Down��Slow-Up�̕ϊ��C���f�b�N�X���Z�b�g
	for( loopIndex = gDD_STM_Profile[profileID].slowUpDownThresh; loopIndex <= gDD_STM_Profile[profileID].profEndIndex; loopIndex += 2 ){
		for( searchIndex = gDD_STM_Profile[profileID].idleIndex + 2; searchIndex <= (gDD_STM_Profile[profileID].slowUpDownThresh - 2); searchIndex += 2 ){
			if( gDD_STM_ProfileTable[searchIndex] < gDD_STM_ProfileTable[loopIndex] ){		// �ō����g����Slow-Up�C���f�b�N�X�ɐݒ肳��Ă���̂ŁA�K���T���͐�������
				gDD_STM_ConvertIndexTable[loopIndex / 2] = searchIndex;
				break;
			}
			else{
				/* No Action */
			}
		}
	}
}

//============================================================================= 
// Function Name : DD_STM_SetProfileID
// Description   : �v���t�@�C���ݒ�
// Parameter     : motorID		"���[�^�[ID"
// 				 : profileID	"�v���t�@�C���e�[�u����ID"
// Return        : -
// Note          : -
//=============================================================================
void DD_STM_SetProfileID(DD_STM_ID motorID, DD_STM_PROF_ID profileID)
{
	UL eventData = (UL)motorID | ((UL)(profileID) << 8);

	DD_EVT_Put(evReceive_ID, evuDD_STM_ID, (UC*)(eventData), 0);
}

//============================================================================= 
// Function Name : DD_STM_StartClock
// Description   : �N���b�N�o�͊J�n
// Parameter     : motorID		"���[�^��ID"
//               : targetIndex	"�ڕW�C���f�b�N�X"
//               : *func		"�R�[���o�b�N�֐��|�C���^"
// Return        : -
// Note          : -
//=============================================================================
void DD_STM_StartClock(DD_STM_ID motorID, US targetIndex, void (*func)(SS result))
{
	UL eventData;

	gDD_STM_MotorManager[motorID].receiveData.targetIndex	 = targetIndex;
	gDD_STM_MotorManager[motorID].receiveData.func			 = func;

	gDD_STM_MotorManager[motorID].lastReceiveNo++;

	eventData = (UL)motorID | ((UL)(gDD_STM_MotorManager[motorID].lastReceiveNo) << 8);

	DD_EVT_Put(evWrite_ID, evuDD_STM_ID, (UC*)(eventData), 0);
}

//============================================================================= 
// Function Name : DD_STM_ChangeClock
// Description   : �N���b�N�o�͕ύX
// Parameter     : motorID		"���[�^��ID"
//               : targetIndex	"�ڕW�C���f�b�N�X"
//               : *func		"�R�[���o�b�N�֐��|�C���^"
// Return        : -
// Note          : -
//=============================================================================
void DD_STM_ChangeClock(DD_STM_ID motorID, US targetIndex, void (*func)(SS result))
{
	UL eventData;

	gDD_STM_MotorManager[motorID].receiveData.targetIndex	 = targetIndex;
	gDD_STM_MotorManager[motorID].receiveData.func			 = func;

	gDD_STM_MotorManager[motorID].lastReceiveNo++;

	eventData = (UL)motorID | ((UL)(gDD_STM_MotorManager[motorID].lastReceiveNo) << 8);

	DD_EVT_Put(evWrite_ID, evuDD_STM_ID, (UC*)(eventData), 0);
}

//============================================================================= 
// Function Name : DD_STM_MoveBySetPulse
// Description   : �w��p���X���N���b�N�o��
// Parameter     : motorID		"���[�^��ID"
//               : targetIndex	"�ڕW�C���f�b�N�X"
//               : *func		"�R�[���o�b�N�֐��|�C���^"
// Return        : -
// Note          : -
//=============================================================================
void DD_STM_MoveBySetPulse(DD_STM_ID motorID, US targetIndex, US totalPulseCnt, void (*func)(SS result))
{
	UL eventData;

	gDD_STM_MotorManager[motorID].receiveData.targetIndex	 = targetIndex;
	gDD_STM_MotorManager[motorID].receiveData.totalPulseCnt	 = totalPulseCnt;
	gDD_STM_MotorManager[motorID].receiveData.func			 = func;

	gDD_STM_MotorManager[motorID].lastReceiveNo++;

	eventData = (UL)motorID | ((UL)(gDD_STM_MotorManager[motorID].lastReceiveNo) << 8);

	DD_EVT_Put(evStart_ID, evuDD_STM_ID, (UC*)(eventData), 0);
}

//============================================================================= 
// Function Name : DD_STM_SlowDownStop
// Description   : �N���b�N��~
// Parameter     : motorID		"���[�^��ID"
//               : targetIndex	"�ڕW�C���f�b�N�X"
//               : *func		"�R�[���o�b�N�֐��|�C���^"
// Return        : -
// Note          : -
//=============================================================================
void DD_STM_SlowDownStop(DD_STM_ID motorID, void (*func)(SS result))
{
	UL eventData;

	gDD_STM_MotorManager[motorID].receiveData.func = func;

	gDD_STM_MotorManager[motorID].lastReceiveNo++;

	eventData = (UL)motorID | ((UL)(gDD_STM_MotorManager[motorID].lastReceiveNo) << 8);

	DD_EVT_Put(evStop_ID, evuDD_STM_ID, (UC*)(eventData), 0);
}

//============================================================================= 
// Function Name : DD_STM_ShutDownStop
// Description   : �N���b�N�ً}��~
// Parameter     : motorID		"���[�^��ID"
// Return        : -
// Note          : �R�[���^�C�~���O��ShutDown���������{����
//=============================================================================
void DD_STM_ShutDownStop(DD_STM_ID motorID)
{
	DD_EVT_Put(evCancel_ID, evuDD_STM_ID, (UC*)(motorID), 0);
}

//============================================================================= 
// Function Name : DD_STM_StartCountClock
// Description   : �o�̓N���b�N���J�E���g(Start)
// Parameter     : motorID		"���[�^��ID"
// Return        : -
// Note          : -
//=============================================================================
void DD_STM_StartCountClock(DD_STM_ID motorID)
{
	if( gDD_STM_MotorManager[motorID].count.controlType == DD_STM_NOT_COUNT_CLOCK ){
		gDD_STM_MotorManager[motorID].count.controlType = DD_STM_COUNT_CLOCK;
		gDD_STM_MotorManager[motorID].count.countStartIndex = 0xFFFF;

		DD_EVT_Put(evRead_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
	else{																		// ���łɃJ�E���g�N���b�N�@�\���g���Ă���ꍇ
		/* No Action */
	}
}

//============================================================================= 
// Function Name : DD_STM_StopCountClock
// Description   : �o�̓N���b�N���J�E���g(Stop)
// Parameter     : motorID		"���[�^��ID"
// Return        : -
// Note          : -
//=============================================================================
US DD_STM_StopCountClock(DD_STM_ID motorID)
{
	US count, remainCnt, nowIndex;

	switch( gDD_STM_MotorManager[motorID].state ){
	case stChangePulseWait_ID:
	case stChangeEntry_ID:
	case stChangeExecute_ID:
	case stChangeAnswer_ID:
	case stConstant_ID:
		remainCnt = DD_DMA_GetRemainTransferCnt(gDD_STM_CtrlTable[motorID].dmacCH);
		nowIndex = gDD_STM_MotorManager[motorID].targetIndex - ((remainCnt / 2) * 2);
		count = (nowIndex - gDD_STM_MotorManager[motorID].count.countStartIndex) / 2;
		break;

	case stPTPulseWait_ID:
		count = gDD_STM_MotorManager[motorID].count.clockCount;
		break;

	case stPTEntry_ID:
		count = gDD_STM_MotorManager[motorID].count.clockCount / 2;
		break;

	case stPTExec_ID:
	case stPTExecPulseWait_ID:
		remainCnt = DD_DMA_GetRemainTransferCnt(gDD_STM_CtrlTable[motorID].dmacCH);
		count = gDD_STM_MotorManager[motorID].count.clockCount / 2 + (DD_STM_PROF_CONST_STEP_SIZE - 1 - remainCnt) / 2;
		break;

	case stPTJudge_ID:
		count = gDD_STM_MotorManager[motorID].count.clockCount / 2;
		break;

	case stStopPulseWait_ID:
		count = gDD_STM_MotorManager[motorID].count.clockCount / 2;
		break;

	case stStopEntry_ID:
		count = gDD_STM_MotorManager[motorID].count.clockCount / 2;
		break;

	case stStopExecute_ID:
	case stStopExecPulseWait_ID:
		remainCnt = DD_DMA_GetRemainTransferCnt(gDD_STM_CtrlTable[motorID].dmacCH);
		nowIndex = gDD_STM_MotorManager[motorID].targetIndex - ((remainCnt / 2) * 2);
		count = (nowIndex - gDD_STM_MotorManager[motorID].nowIndex) / 2;
		break;

	case stStopAnswer_ID:
	case stIdle_ID:
		count = gDD_STM_MotorManager[motorID].count.clockCount / 2;
		break;

	default:
		count = 0;
		break;
	}

	gDD_STM_MotorManager[motorID].count.controlType = DD_STM_NOT_COUNT_CLOCK;
	DD_EVT_Put(evReset_ID, evuDD_STM_ID, (UC*)(motorID), 0);

	return count;
}

//============================================================================= 
// Function Name : DD_STM_AppointPulseTiming
// Description   : �w��p���X�^�C�~���O
// Parameter     : motorID		"���[�^��ID"
// Return        : -
// Note          : -
//=============================================================================
void DD_STM_AppointPulseTiming(DD_STM_ID motorID, US pulseCnt, void (*func)(SS result))
{
	if( gDD_STM_MotorManager[motorID].count.controlType == DD_STM_NOT_COUNT_CLOCK ){
		gDD_STM_MotorManager[motorID].count.controlType = DD_STM_PULSE_TIMING;
		gDD_STM_MotorManager[motorID].count.func = func;
		gDD_STM_MotorManager[motorID].count.remain = pulseCnt * 2;

		DD_EVT_Put(evRead_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
	else{																		// ���łɃJ�E���g�N���b�N�@�\���g���Ă���ꍇ
		/* No Action */
	}
}

//============================================================================= 
// Function Name : DD_STM_Notify
// Description   : Event�ʒm�֐�
// Parameter     : eventID 	"�C�x���gID"
//               : pMsg		"�f�[�^�|�C���^"
//				 : (0-7bit:morotID�A8-15bit:para1�A16-23bit:para2�A24-31bit:para3)
// Return        : -
// Note          : EventLoop����R�[�������
//=============================================================================
void DD_STM_Notify(DD_EventID eventID, UC* pMsg)
{
	UC key;
	DD_STM_ID motorID = (DD_STM_ID)((UL)(pMsg) & 0x000000FF);
	UC para1 = (UC)(((UL)(pMsg) & 0x0000FF00) >> 8);

	switch( eventID ){
	case evReceive_ID:
		switch( gDD_STM_MotorManager[motorID].state ){
		case stIdle_ID:															// �N���b�N��~����state�̏ꍇ
		case stSPAnswer_ID:
		case stStopAnswer_ID:
		case stShutDown_ID:
			SetProfileIDExec(motorID, (DD_STM_PROF_ID)para1 );
			break;

		default:																// �N���b�N�o�͒���state�̏ꍇ
			/* No Action */
			break;
		}
		break;

	case evWrite_ID:
		key = LIB_GetKey();
		LIB_Lock();																// ���荞�݋֎~

		if( para1 == gDD_STM_MotorManager[motorID].lastReceiveNo ){				// �C�x���g�Ǘ�No.�Ǝ�M�f�[�^�Ǘ�No.����v����Ƃ�
			gDD_STM_MotorManager[motorID].entryData = gDD_STM_MotorManager[motorID].receiveData;
			LIB_Unlock(key);													// ���荞�݋���
		}
		else{																	// �C�x���g�Ǘ�No.�Ǝ�M�f�[�^�Ǘ�No.����v���Ȃ��Ƃ�
			LIB_Unlock(key);													// ���荞�݋���
			break;																// �C�x���g�������I������
		}

		if( gDD_STM_MotorManager[motorID].profileID == DD_STM_PROF_ID_NUM ){	// �v���t�@�C��ID�ݒ肪�����{�̂Ƃ�
			break;																// �C�x���g�������I������
		}

		switch( gDD_STM_MotorManager[motorID].state ){
		case stIdle_ID:															// �N���b�N��~����state�̏ꍇ
		case stStopAnswer_ID:
		case stSPAnswer_ID:
		case stShutDown_ID:
			stChangeEntry(motorID);
			break;

		case stChangePulseWait_ID:												// �N���b�N�o�͒���state�̏ꍇ
		case stChangeEntry_ID:
		case stChangeExecute_ID:
		case stChangeAnswer_ID:
		case stConstant_ID:
		case stPTPulseWait_ID:
		case stPTEntry_ID:
		case stPTExec_ID:
		case stPTExecPulseWait_ID:
		case stPTJudge_ID:
		case stPTAnswer_ID:
		case stStopPulseWait_ID:
		case stStopEntry_ID:
		case stStopExecute_ID:
		case stStopExecPulseWait_ID:
		case stSPPulseWait_ID:
		case stSPChangeEntry_ID:
		case stSPChange_ID:
		case stSPChangePulseWait_ID:
		case stSPConstEntry_ID:
		case stSPConstant_ID:
		case stSPConstPulseWait_ID:
		case stSPConstJudge_ID:
		case stSPStopEntry_ID:
		case stSPStop_ID:
		case stSPStopPulseWait_ID:
			stChangePulseWait(motorID);
			break;

		default:																// �o�J�悯
			/* No Action */
			break;
		}
		break;

	case evStart_ID:
		key = LIB_GetKey();
		LIB_Lock();																// ���荞�݋֎~

		if( para1 == gDD_STM_MotorManager[motorID].lastReceiveNo ){				// �C�x���g�Ǘ�No.�Ǝ�M�f�[�^�Ǘ�No.����v����Ƃ�
			gDD_STM_MotorManager[motorID].entryData = gDD_STM_MotorManager[motorID].receiveData;
			LIB_Unlock(key);													// ���荞�݋���
		}
		else{																	// �C�x���g�Ǘ�No.�Ǝ�M�f�[�^�Ǘ�No.����v���Ȃ��Ƃ�
			LIB_Unlock(key);													// ���荞�݋���
			break;																// �C�x���g�������I������
		}

		if( gDD_STM_MotorManager[motorID].profileID == DD_STM_PROF_ID_NUM ){	// �v���t�@�C��ID�ݒ肪�����{�̂Ƃ�
			break;																// �C�x���g�������I������
		}

		switch( gDD_STM_MotorManager[motorID].state ){
		case stIdle_ID:															// �N���b�N��~����state�̏ꍇ
		case stStopAnswer_ID:
		case stSPAnswer_ID:
		case stShutDown_ID:
			stSPChangeEntry(motorID);
			break;

		case stChangePulseWait_ID:												// �N���b�N�o�͒���state�̏ꍇ
		case stChangeEntry_ID:
		case stChangeExecute_ID:
		case stChangeAnswer_ID:
		case stConstant_ID:
		case stPTPulseWait_ID:
		case stPTEntry_ID:
		case stPTExec_ID:
		case stPTExecPulseWait_ID:
		case stPTJudge_ID:
		case stPTAnswer_ID:
		case stStopPulseWait_ID:
		case stStopEntry_ID:
		case stStopExecute_ID:
		case stStopExecPulseWait_ID:
		case stSPPulseWait_ID:
		case stSPChangeEntry_ID:
		case stSPChange_ID:
		case stSPChangePulseWait_ID:
		case stSPConstEntry_ID:
		case stSPConstant_ID:
		case stSPConstPulseWait_ID:
		case stSPConstJudge_ID:
		case stSPStopEntry_ID:
		case stSPStop_ID:
		case stSPStopPulseWait_ID:
			stSPPulseWait(motorID);
			break;

		default:																// �o�J�悯
			/* No Action */
			break;
		}
		break;

	case evRead_ID:
		if( gDD_STM_MotorManager[motorID].count.controlType == DD_STM_COUNT_CLOCK ){
			switch( gDD_STM_MotorManager[motorID].state ){
			case stChangePulseWait_ID:
			case stChangeEntry_ID:
			case stChangeExecute_ID:
				SetCountStartIndex(motorID);
				break;

			case stChangeAnswer_ID:
			case stConstant_ID:
				SetCountStartIndex(motorID);
				stPTPulseWait(motorID);
				break;

			default:
				/* No Action */
				break;
			}
		}
		else if( gDD_STM_MotorManager[motorID].count.controlType == DD_STM_PULSE_TIMING ){
			switch( gDD_STM_MotorManager[motorID].state ){
			case stChangeAnswer_ID:
			case stConstant_ID:
				stPTPulseWait(motorID);
				break;

			default:
				/* No Action */
				break;
			}
		}
		break;

	case evGoComplete_ID:
		switch( gDD_STM_MotorManager[motorID].state ){
		case stChangePulseWait_ID:
			stChangeEntry(motorID);
			break;

		case stStopPulseWait_ID:
			stStopEntry(motorID);
			break;

		case stStopExecPulseWait_ID:
			stStopAnswer(motorID);
			break;

		case stSPPulseWait_ID:
			stSPChangeEntry(motorID);
			break;

		case stSPChangePulseWait_ID:
			stSPConstEntry(motorID);
			break;

		case stSPConstPulseWait_ID:
			stSPConstJudge(motorID);
			break;

		case stSPStopPulseWait_ID:
			stSPAnswer(motorID);
			break;

		case stPTPulseWait_ID:
			stPTEntry(motorID);
			break;

		case stPTExecPulseWait_ID:
			stPTJudge(motorID);
			break;

		default:
			/* No Action */
			break;
		}
		break;

	case evGoCancel_ID:
		switch( gDD_STM_MotorManager[motorID].state ){
		case stChangeEntry_ID:
			stChangeAnswer(motorID);
			break;

		case stStopEntry_ID:
			stStopAnswer(motorID);
			break;

		case stSPConstEntry_ID:
			stSPStopEntry(motorID);
			break;

		case stSPStopEntry_ID:
			stSPAnswer(motorID);
			break;

		case stPTEntry_ID:
			stPTAnswer(motorID);
			break;

		default:
			/* No Action */
			break;
		}
		break;

	case evGoNext_ID:
		switch( gDD_STM_MotorManager[motorID].state ){
		case stChangeEntry_ID:
			stChangeExecute(motorID);
			break;

		case stStopEntry_ID:
			stStopExecute(motorID);
			break;

		case stSPChangeEntry_ID:
			stSPChange(motorID);
			break;

		case stSPConstEntry_ID:
			stSPConstant(motorID);
			break;

		case stSPConstJudge_ID:
			stSPStopEntry(motorID);
			break;

		case stSPStopEntry_ID:
			stSPStop(motorID);
			break;

		case stPTEntry_ID:
			stPTExec(motorID);
			break;

		case stPTJudge_ID:
			stPTAnswer(motorID);
			break;

		default:
			/* No Action */
			break;
		}
		break;

	case evSend_ID:
		switch( gDD_STM_MotorManager[motorID].state ){
		case stChangeExecute_ID:
			stChangeAnswer(motorID);
			break;

		case stStopExecute_ID:
			stStopExecPulseWait(motorID);
			break;

		case stSPChange_ID:
			stSPChangePulseWait(motorID);
			break;

		case stSPConstant_ID:
			stSPConstPulseWait(motorID);
			break;

		case stSPStop_ID:
			stSPStopPulseWait(motorID);
			break;

		case stPTExec_ID:
			stPTExecPulseWait(motorID);
			break;

		default:
			/* No Action */
			break;
		}
		break;

	case evComplete_ID:
		switch( gDD_STM_MotorManager[motorID].state ){
		case stChangeAnswer_ID:
			stConstant(motorID);
			break;

		case stStopAnswer_ID:
		case stSPAnswer_ID:
		case stShutDown_ID:
			stIdle(motorID);
			break;

		default:
			/* No Action */
			break;
		}
		break;

	case evStop_ID:
		key = LIB_GetKey();
		LIB_Lock();																// ���荞�݋֎~

		if( para1 == gDD_STM_MotorManager[motorID].lastReceiveNo ){				// �C�x���g�Ǘ�No.�Ǝ�M�f�[�^�Ǘ�No.����v����Ƃ�
			gDD_STM_MotorManager[motorID].entryData = gDD_STM_MotorManager[motorID].receiveData;
			LIB_Unlock(key);													// ���荞�݋���
		}
		else{																	// �C�x���g�Ǘ�No.�Ǝ�M�f�[�^�Ǘ�No.����v���Ȃ��Ƃ�
			LIB_Unlock(key);													// ���荞�݋���
			break;																// �C�x���g�������I������
		}

		switch( gDD_STM_MotorManager[motorID].state ){
		case stChangePulseWait_ID:
		case stChangeEntry_ID:
		case stChangeExecute_ID:
		case stChangeAnswer_ID:
		case stConstant_ID:
		case stPTPulseWait_ID:
		case stPTEntry_ID:
		case stPTExec_ID:
		case stPTExecPulseWait_ID:
		case stPTJudge_ID:
		case stPTAnswer_ID:
		case stSPPulseWait_ID:
		case stSPChangeEntry_ID:
		case stSPChange_ID:
		case stSPChangePulseWait_ID:
		case stSPConstEntry_ID:
		case stSPConstant_ID:
		case stSPConstPulseWait_ID:
		case stSPConstJudge_ID:
		case stSPStopEntry_ID:
		case stSPStop_ID:
		case stSPStopPulseWait_ID:
			stStopPulseWait(motorID);
			break;

		default:																// stIdle_ID, stStopPulseWait_ID, stStopEntry_ID, stStopExecute_ID, stStopExecPulseWait_ID, stStopAnswer_ID, stSPAnswer_ID, stShutDown_ID
			/* No Action */
			break;
		}
		break;

	case evCancel_ID:
		switch( gDD_STM_MotorManager[motorID].state ){
		case stChangePulseWait_ID:
		case stChangeEntry_ID:
		case stChangeExecute_ID:
		case stChangeAnswer_ID:
		case stConstant_ID:
		case stPTPulseWait_ID:
		case stPTEntry_ID:
		case stPTExec_ID:
		case stPTExecPulseWait_ID:
		case stPTJudge_ID:
		case stPTAnswer_ID:
		case stStopPulseWait_ID:
		case stStopEntry_ID:
		case stStopExecute_ID:
		case stStopExecPulseWait_ID:
		case stStopAnswer_ID:
		case stSPPulseWait_ID:
		case stSPChangeEntry_ID:
		case stSPChange_ID:
		case stSPChangePulseWait_ID:
		case stSPConstEntry_ID:
		case stSPConstant_ID:
		case stSPConstPulseWait_ID:
		case stSPConstJudge_ID:
		case stSPStopEntry_ID:
		case stSPStop_ID:
		case stSPStopPulseWait_ID:
		case stSPAnswer_ID:
			stShutDown(motorID);
			break;

		default:																// stIdle_ID,stShutDown_ID
			break;
		}
		break;

	case evGoStart_ID:
		switch( gDD_STM_MotorManager[motorID].state ){
		case stSPChangeEntry_ID:
 		case stSPConstJudge_ID:
			stSPConstEntry(motorID);
			break;

		case stPTJudge_ID:
			stPTEntry(motorID);
			break;

		default:
			/* No Action */
			break;
		}
		break;

	case evReset_ID:
		switch( gDD_STM_MotorManager[motorID].state ){
		case stPTPulseWait_ID:
		case stPTEntry_ID:
		case stPTExec_ID:
		case stPTExecPulseWait_ID:
		case stPTJudge_ID:
			DD_DMA_Stop(gDD_STM_CtrlTable[motorID].dmacCH);						// DMA�]�����~
			stConstant(motorID);
			break;

		default:
			break;
		}

	default:
		/* No Action */
		break;
	}
}

//============================================================================= 
// Function Name : stIdle
// Description   : stIdle_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stIdle(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stIdle_ID;
}

//============================================================================= 
// Function Name : stChangePulseWait
// Description   : stChangePulseWait_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stChangePulseWait(DD_STM_ID motorID)
{
	US remainCnt;

	gDD_STM_MotorManager[motorID].state = stChangePulseWait_ID;

	DD_DMA_Stop(gDD_STM_CtrlTable[motorID].dmacCH);								// ���쒆��DMA�]�����~
	remainCnt = DD_DMA_GetRemainTransferCnt(gDD_STM_CtrlTable[motorID].dmacCH);// �c��]���񐔂��擾

	if( gDD_STM_MotorManager[motorID].nowIndex != gDD_STM_MotorManager[motorID].targetIndex ){	// SPConst��DMA�]�����͍X�V���Ȃ�
		gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_MotorManager[motorID].targetIndex - ((remainCnt / 2) * 2);
	}

	DD_TAU_IntEnable(gDD_STM_CtrlTable[motorID].timerCH);						// �w�胂�[�^�̃^�C�}���荞�݋���
}

//============================================================================= 
// Function Name : stChangeEntry
// Description   : stChangeEntry_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stChangeEntry(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stChangeEntry_ID;

	gDD_STM_MotorManager[motorID].targetIndex = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex + 2 +
												gDD_STM_MotorManager[motorID].entryData.targetIndex * 2;
	gDD_STM_MotorManager[motorID].func = gDD_STM_MotorManager[motorID].entryData.func;

	if( SetChangeClockData(motorID)  == true ){								// �N���b�N�ύX����
		DD_EVT_Put(evGoNext_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
	else{																	// �N���b�N�ύX�Ȃ�
		DD_EVT_Put(evGoCancel_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
}

//============================================================================= 
// Function Name : stChangeExecute
// Description   : stChangeExecute_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stChangeExecute(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stChangeExecute_ID;
}

//============================================================================= 
// Function Name : stChangeAnswer
// Description   : stChangeAnswer_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stChangeAnswer(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stChangeAnswer_ID;

	gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_MotorManager[motorID].targetIndex;

	if( gDD_STM_MotorManager[motorID].func != NULL ){
		gDD_STM_MotorManager[motorID].func(DD_STM_RESULT_OK);
	}

	if( gDD_STM_MotorManager[motorID].count.controlType == DD_STM_NOT_COUNT_CLOCK ){	// �葬���ɏo�̓N���b�N�����J�E���g���Ȃ��ꍇ
		DD_EVT_Put(evComplete_ID, evuDD_STM_ID, (UC*)motorID, 0);
	}
	else{																		// �葬���ɏo�̓N���b�N�����J�E���g����ꍇ
		DD_EVT_Put(evRead_ID, evuDD_STM_ID, (UC*)motorID, 0);
	}
}

//============================================================================= 
// Function Name : stConstant
// Description   : stConstant_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stConstant(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stConstant_ID;
}

//============================================================================= 
// Function Name : stStopPulseWait
// Description   : stStopPulseWait_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stStopPulseWait(DD_STM_ID motorID)
{
	US remainCnt, lastState;


	gDD_STM_MotorManager[motorID].state = stStopPulseWait_ID;

	DD_DMA_Stop(gDD_STM_CtrlTable[motorID].dmacCH);								// ���쒆��DMA�]�����~
	remainCnt = DD_DMA_GetRemainTransferCnt(gDD_STM_CtrlTable[motorID].dmacCH);// �c��]���񐔂��擾

	if( gDD_STM_MotorManager[motorID].nowIndex != gDD_STM_MotorManager[motorID].targetIndex ){	// SPConst��DMA�]�����͍X�V���Ȃ�
		gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_MotorManager[motorID].targetIndex - ((remainCnt / 2) * 2);
	}

	if( gDD_STM_MotorManager[motorID].count.controlType == DD_STM_COUNT_CLOCK ){
		if( (lastState == stPTExec_ID) || (lastState == stPTExecPulseWait_ID) ){
			gDD_STM_MotorManager[motorID].count.clockCount += ((DD_STM_PROF_CONST_STEP_SIZE - 1 - remainCnt) / 2) * 2;	// DMA�]���c��
		}
	}

	DD_TAU_IntEnable(gDD_STM_CtrlTable[motorID].timerCH);						// �w�胂�[�^�̃^�C�}���荞�݋���
}

//============================================================================= 
// Function Name : stStopEntry
// Description   : stStopEntry_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stStopEntry(DD_STM_ID motorID)
{
	UL eventData;

	gDD_STM_MotorManager[motorID].state = stStopEntry_ID;

	gDD_STM_MotorManager[motorID].targetIndex = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].profEndIndex;
	gDD_STM_MotorManager[motorID].func = gDD_STM_MotorManager[motorID].entryData.func;

	if( gDD_STM_MotorManager[motorID].count.controlType == DD_STM_COUNT_CLOCK ){
		gDD_STM_MotorManager[motorID].count.clockCount += 2;
	}

	if( SetStopClockData(motorID)  == true ){								// �N���b�N�ύX�Ȃ�
		DD_EVT_Put(evGoNext_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
	else{																	// �N���b�N�ύX�Ȃ�
		DD_EVT_Put(evGoCancel_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
}

//============================================================================= 
// Function Name : stStopExecute
// Description   : stStopExecute_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stStopExecute(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stStopExecute_ID;
}

//============================================================================= 
// Function Name : stStopExecPulseWait
// Description   : stStopExecPulseWait_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stStopExecPulseWait(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stStopExecPulseWait_ID;

	gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_MotorManager[motorID].targetIndex;

	DD_TAU_IntEnable(gDD_STM_CtrlTable[motorID].timerCH);						// �w�胂�[�^�̃^�C�}���荞�݋���
}

//============================================================================= 
// Function Name : stStopAnswer
// Description   : stStopAnswer_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stStopAnswer(DD_STM_ID motorID)
{
	US nowIndex = gDD_STM_MotorManager[motorID].nowIndex;
	
	gDD_STM_MotorManager[motorID].state = stStopAnswer_ID;

	DD_TAU_Stop(gDD_STM_CtrlTable[motorID].timerCH);

	gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex;

	if( gDD_STM_MotorManager[motorID].func != NULL ){
		gDD_STM_MotorManager[motorID].func(DD_STM_RESULT_OK);
	}

	if( gDD_STM_MotorManager[motorID].count.controlType == DD_STM_COUNT_CLOCK ){
		gDD_STM_MotorManager[motorID].count.clockCount += (gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].profEndIndex - nowIndex);
	}

	DD_EVT_Put(evComplete_ID, evuDD_STM_ID, (UC*)motorID, 0);
}

//============================================================================= 
// Function Name : stSPPulseWait
// Description   : stSPPulseWait_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stSPPulseWait(DD_STM_ID motorID)
{
	US remainCnt;

	gDD_STM_MotorManager[motorID].state = stSPPulseWait_ID;

	DD_DMA_Stop(gDD_STM_CtrlTable[motorID].dmacCH);								// ���쒆��DMA�]�����~
	remainCnt = DD_DMA_GetRemainTransferCnt(gDD_STM_CtrlTable[motorID].dmacCH);// �c��]���񐔂��擾

	if( gDD_STM_MotorManager[motorID].nowIndex != gDD_STM_MotorManager[motorID].targetIndex ){	// SPConst��DMA�]�����͍X�V���Ȃ�
		gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_MotorManager[motorID].targetIndex - ((remainCnt / 2) * 2);
	}

	DD_TAU_IntEnable(gDD_STM_CtrlTable[motorID].timerCH);						// �w�胂�[�^�̃^�C�}���荞�݋���
}

//============================================================================= 
// Function Name : stSPChangeEntry
// Description   : stSPChangeEntry_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stSPChangeEntry(DD_STM_ID motorID)
{
	US nowIndex, targetIndex, indexConvPlsCnt, changePulseCnt, slowDownStartIndex, slowDownPulseCnt, totalPulseCnt;
	UC loopCnt;

	gDD_STM_MotorManager[motorID].state = stSPChangeEntry_ID;

	targetIndex = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex + 2 +
				  gDD_STM_MotorManager[motorID].entryData.targetIndex * 2;
	totalPulseCnt = gDD_STM_MotorManager[motorID].entryData.totalPulseCnt * 2;
	gDD_STM_MotorManager[motorID].func = gDD_STM_MotorManager[motorID].entryData.func;

	nowIndex = gDD_STM_MotorManager[motorID].nowIndex;

	if( (totalPulseCnt <= 2) &&	(nowIndex == gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex) ){	// �N���b�N��~��Ԃ����1Pulse�o��
		gDD_STM_MotorManager[motorID].targetIndex = nowIndex + 2;
		gDD_STM_MotorManager[motorID].spChangePulseCnt = 2;
		gDD_STM_MotorManager[motorID].spConstPulseCnt = 0;
		gDD_STM_MotorManager[motorID].spSlowDownPulseCnt = 0;
		gDD_STM_MotorManager[motorID].spResult = DD_STM_RESULT_OK;
	}
	else{
		// �C���f�b�N�X�̕ϊ�
		if( ((targetIndex < gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].slowUpDownThresh) &&
			 (nowIndex < gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].slowUpDownThresh)) ||
			((targetIndex >= gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].slowUpDownThresh) &&
			 (nowIndex >= gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].slowUpDownThresh)) ){

			if( targetIndex < nowIndex ){
				if( gDD_STM_ConvertIndexTable[nowIndex / 2] == gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex ){	// ���݃C���f�b�N�X�̕ϊ��C���f�b�N�X��idle�̂Ƃ�
					targetIndex = nowIndex;

					indexConvPlsCnt = 0;
				}
				else{														// ���݃C���f�b�N�X�̕ϊ��C���f�b�N�X��idle�ł͂Ȃ��Ƃ�
					if( gDD_STM_ConvertIndexTable[targetIndex / 2] == gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex ){	// �ڕW�C���f�b�N�X�̕ϊ��C���f�b�N�X��idle�̂Ƃ�
						nowIndex = gDD_STM_ConvertIndexTable[nowIndex / 2];
						targetIndex = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].profEndIndex;	// profEndIndex���Z�b�g����

						indexConvPlsCnt = 2;
					}
					else{													// �ڕW�C���f�b�N�X�̕ϊ��C���f�b�N�X��idle�ł͂Ȃ��Ƃ�
						nowIndex = gDD_STM_ConvertIndexTable[nowIndex / 2];
						targetIndex = gDD_STM_ConvertIndexTable[targetIndex / 2];

						indexConvPlsCnt = 2;
					}
				}
			}
			else{
				indexConvPlsCnt = 0;
			}
		}
		else{
			if(	gDD_STM_ProfileTable[nowIndex] == gDD_STM_ProfileTable[targetIndex] ){	// ���݃C���f�b�N�X�̎��g���ƁA�ڕW�C���f�b�N�X�̎��g���������Ƃ�
				nowIndex = targetIndex;

				indexConvPlsCnt = 0;
			}
			else{
				if( gDD_STM_ConvertIndexTable[nowIndex / 2] == gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex ){	// ���݃C���f�b�N�X�̕ϊ��C���f�b�N�X��idle�̂Ƃ�
					targetIndex = gDD_STM_ConvertIndexTable[targetIndex / 2];

					indexConvPlsCnt = 0;
				}
				else{
					if( targetIndex < gDD_STM_ConvertIndexTable[nowIndex / 2] ){
						targetIndex = gDD_STM_ConvertIndexTable[targetIndex / 2];

						indexConvPlsCnt = 0;
					}
					else{
						nowIndex = gDD_STM_ConvertIndexTable[nowIndex / 2];

						indexConvPlsCnt = 2;
					}
				}
			}
		}

		// �ڕW�C���f�b�N�X�̒���
		for( ; ; ){
			changePulseCnt = targetIndex - nowIndex + indexConvPlsCnt;

			if( targetIndex < gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].slowUpDownThresh ){	// �ڕW�C���f�b�N�X��Slow-Up�C���f�b�N�X�̏ꍇ
				if( gDD_STM_ConvertIndexTable[targetIndex / 2] == gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex ){
					slowDownPulseCnt = 0;
				}
				else{
					slowDownStartIndex = gDD_STM_ConvertIndexTable[targetIndex / 2];	// Slow-Down�̊J�n�C���f�b�N�X���擾
					slowDownPulseCnt = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].profEndIndex - slowDownStartIndex + 2;	//�C���f�b�N�X�ϊ��̂���+2
				}
			}
			else{															// �ڕW�C���f�b�N�X��Slow-Down�C���f�b�N�X�̏ꍇ
				slowDownPulseCnt = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].profEndIndex - targetIndex;
			}

			if( totalPulseCnt >= (changePulseCnt + slowDownPulseCnt) ){
				gDD_STM_MotorManager[motorID].nowIndex = nowIndex;
				gDD_STM_MotorManager[motorID].targetIndex = targetIndex;
				gDD_STM_MotorManager[motorID].spChangePulseCnt = changePulseCnt;
				gDD_STM_MotorManager[motorID].spConstPulseCnt = totalPulseCnt - changePulseCnt - slowDownPulseCnt;
				if( gDD_STM_MotorManager[motorID].spConstPulseCnt > 0 ){
					for( loopCnt = 0; loopCnt < DD_STM_PROF_CONST_STEP_SIZE; loopCnt++ ){
						gDD_STM_MotorManager[motorID].constTable[loopCnt] = gDD_STM_ProfileTable[gDD_STM_MotorManager[motorID].targetIndex];
					}
				}
				gDD_STM_MotorManager[motorID].spSlowDownPulseCnt = slowDownPulseCnt;
				gDD_STM_MotorManager[motorID].spResult = DD_STM_RESULT_OK;
				break;
			}
			else{
				if( targetIndex < gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].slowUpDownThresh ){	// �ڕW�C���f�b�N�X��Slow-Up�C���f�b�N�X�̏ꍇ
					targetIndex -= 2;
					if( targetIndex < nowIndex ){
						gDD_STM_MotorManager[motorID].nowIndex = nowIndex;
						gDD_STM_MotorManager[motorID].targetIndex = nowIndex;
						gDD_STM_MotorManager[motorID].spChangePulseCnt = 0;
						gDD_STM_MotorManager[motorID].spConstPulseCnt = 0;
						gDD_STM_MotorManager[motorID].spSlowDownPulseCnt = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].profEndIndex - nowIndex;
						gDD_STM_MotorManager[motorID].spResult = DD_STM_RESULT_NG;
						break;
					}
					else{
						/* Continue for loop */
					}
				}
				else{															// �ڕW�C���f�b�N�X��Slow-Down�C���f�b�N�X�̏ꍇ
					gDD_STM_MotorManager[motorID].nowIndex = nowIndex;
					gDD_STM_MotorManager[motorID].targetIndex = nowIndex;
					gDD_STM_MotorManager[motorID].spChangePulseCnt = 0;
					gDD_STM_MotorManager[motorID].spConstPulseCnt = 0;
					gDD_STM_MotorManager[motorID].spSlowDownPulseCnt = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].profEndIndex - nowIndex;
					gDD_STM_MotorManager[motorID].spResult = DD_STM_RESULT_NG;
					break;
				}
			}
		}
	}

	if( SetChangeClockData(motorID) == true ){								// �N���b�N�ύX����
		DD_EVT_Put(evGoNext_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
	else{																	// �N���b�N�ύX�Ȃ�
		DD_EVT_Put(evGoStart_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
}

//============================================================================= 
// Function Name : stSPChange
// Description   : stSPChange_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stSPChange(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stSPChange_ID;
}

//============================================================================= 
// Function Name : stSPChangePulseWait
// Description   : stSPChangePulseWait_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stSPChangePulseWait(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stSPChangePulseWait_ID;

	gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_MotorManager[motorID].targetIndex;

	DD_TAU_IntEnable(gDD_STM_CtrlTable[motorID].timerCH);						// �w�胂�[�^�̃^�C�}���荞�݋���
}

//============================================================================= 
// Function Name : stSPConstEntry
// Description   : stSPConstEntry_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stSPConstEntry(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stSPConstEntry_ID;

	if( gDD_STM_MotorManager[motorID].spConstPulseCnt > DD_STM_PROF_CONST_STEP_SIZE ){
		DD_DMA_Start(gDD_STM_CtrlTable[motorID].dmacCH,
					 &(gDD_STM_MotorManager[motorID].constTable[1]),
					 (DD_STM_PROF_CONST_STEP_SIZE - 1));
		DD_EVT_Put(evGoNext_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
	else if( gDD_STM_MotorManager[motorID].spConstPulseCnt > 0 ){
		DD_DMA_Start(gDD_STM_CtrlTable[motorID].dmacCH,
					 &(gDD_STM_MotorManager[motorID].constTable[1]),
					 (gDD_STM_MotorManager[motorID].spConstPulseCnt - 1));
		DD_EVT_Put(evGoNext_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
	else{																		// �葬�ł̎c��o�̓p���X����0�̂Ƃ�
		DD_EVT_Put(evGoCancel_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
}

//============================================================================= 
// Function Name : stSPConstant
// Description   : stSPConstant_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stSPConstant(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stSPConstant_ID;
}

//============================================================================= 
// Function Name : stSPConstPulseWait
// Description   : stSPConstPulseWait_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stSPConstPulseWait(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stSPConstPulseWait_ID;

	if( gDD_STM_MotorManager[motorID].spConstPulseCnt > DD_STM_PROF_CONST_STEP_SIZE ){
		gDD_STM_MotorManager[motorID].spConstPulseCnt -= DD_STM_PROF_CONST_STEP_SIZE;
	}
	else{
		gDD_STM_MotorManager[motorID].spConstPulseCnt = 0;
	}

	DD_TAU_IntEnable(gDD_STM_CtrlTable[motorID].timerCH);						// �w�胂�[�^�̃^�C�}���荞�݋���
}

//============================================================================= 
// Function Name : stSPConstJudge
// Description   : stSPConstJudge_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stSPConstJudge(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stSPConstJudge_ID;

	if( gDD_STM_MotorManager[motorID].spConstPulseCnt > 0 ){
		DD_EVT_Put(evGoStart_ID, evuDD_STM_ID, (UC*)(motorID), 0);				// ConstPulse�o�͌p��
	}
	else{
		DD_EVT_Put(evGoNext_ID, evuDD_STM_ID, (UC*)(motorID), 0);				// ConstPulse�o�͏I��
	}
}

//============================================================================= 
// Function Name : stSPStopEntry
// Description   : stSPStopEntry_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stSPStopEntry(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stSPStopEntry_ID;

	gDD_STM_MotorManager[motorID].targetIndex = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].profEndIndex;

	if( gDD_STM_MotorManager[motorID].spSlowDownPulseCnt > 0 ){					// Slow-Down�����{����
		SetStopClockData(motorID);												// �N���b�N��~���{
		DD_EVT_Put(evGoNext_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
	else{																		// Slow-Down�����{���Ȃ�
		DD_EVT_Put(evGoCancel_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
}

//============================================================================= 
// Function Name : stSPStop
// Description   : stSPStop_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stSPStop(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stSPStop_ID;
}

//============================================================================= 
// Function Name : stSPStopPulseWait
// Description   : stSPStopPulseWait_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stSPStopPulseWait(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stSPStopPulseWait_ID;

	DD_TAU_IntEnable(gDD_STM_CtrlTable[motorID].timerCH);						// �w�胂�[�^�̃^�C�}���荞�݋���
}

//============================================================================= 
// Function Name : stSPAnswer
// Description   : stSPAnswer_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stSPAnswer(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stSPAnswer_ID;

	DD_TAU_Stop(gDD_STM_CtrlTable[motorID].timerCH);

	gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex;

	if( gDD_STM_MotorManager[motorID].func != NULL ){
		gDD_STM_MotorManager[motorID].func(gDD_STM_MotorManager[motorID].spResult);
	}

	DD_EVT_Put(evComplete_ID, evuDD_STM_ID, (UC*)motorID, 0);
}

//============================================================================= 
// Function Name : stShutDown
// Description   : stShutDown_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stShutDown(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stShutDown_ID;

	DD_DMA_Stop(gDD_STM_CtrlTable[motorID].dmacCH);								// DMA�]�����~
	DD_TAU_Stop(gDD_STM_CtrlTable[motorID].timerCH);							// �^�C�}���荞�݂��~

	gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex;
	gDD_STM_MotorManager[motorID].count.controlType = DD_STM_NOT_COUNT_CLOCK;

	DD_EVT_Put(evComplete_ID, evuDD_STM_ID, (UC*)motorID, 0);
}

//============================================================================= 
// Function Name : stPTPulseWait
// Description   : stPTPulseWait_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stPTPulseWait(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stPTPulseWait_ID;

	gDD_STM_MotorManager[motorID].count.clockCount = gDD_STM_MotorManager[motorID].nowIndex - gDD_STM_MotorManager[motorID].count.countStartIndex;

	DD_TAU_IntEnable(gDD_STM_CtrlTable[motorID].timerCH);						// �w�胂�[�^�̃^�C�}���荞�݋���
}

//============================================================================= 
// Function Name : stPTEntry
// Description   : stPTEntry_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stPTEntry(DD_STM_ID motorID)
{
	UC loopCnt;

	if( gDD_STM_MotorManager[motorID].state == stPTPulseWait_ID ){
		if( gDD_STM_MotorManager[motorID].count.controlType == DD_STM_PULSE_TIMING ){
			if( gDD_STM_MotorManager[motorID].count.remain <= 2 ){
				gDD_STM_MotorManager[motorID].count.remain = 0;
			}
			else{
				gDD_STM_MotorManager[motorID].count.remain -= 2;
			}
		}
		else{
			gDD_STM_MotorManager[motorID].count.clockCount += 2;
		}
		for( loopCnt = 0; loopCnt < DD_STM_PROF_CONST_STEP_SIZE; loopCnt++ ){	// DMA�]���p RAM�f�[�^�쐬
			gDD_STM_MotorManager[motorID].constTable[loopCnt] = gDD_STM_ProfileTable[gDD_STM_MotorManager[motorID].nowIndex];
		}
	}

	gDD_STM_MotorManager[motorID].state = stPTEntry_ID;

	if( gDD_STM_MotorManager[motorID].count.controlType == DD_STM_PULSE_TIMING ){
		if( gDD_STM_MotorManager[motorID].count.remain <= 0 ){
			DD_EVT_Put(evGoCancel_ID, evuDD_STM_ID, (UC*)(motorID), 0);
		}
		else if( gDD_STM_MotorManager[motorID].count.remain <= DD_STM_PROF_CONST_STEP_SIZE ){
			DD_DMA_Start(gDD_STM_CtrlTable[motorID].dmacCH, &(gDD_STM_MotorManager[motorID].constTable[1]), (gDD_STM_MotorManager[motorID].count.remain - 1));
			DD_EVT_Put(evGoNext_ID, evuDD_STM_ID, (UC*)(motorID), 0);
		}
		else{
			DD_DMA_Start(gDD_STM_CtrlTable[motorID].dmacCH, &(gDD_STM_MotorManager[motorID].constTable[1]), (DD_STM_PROF_CONST_STEP_SIZE - 1));
			DD_EVT_Put(evGoNext_ID, evuDD_STM_ID, (UC*)(motorID), 0);
		}
	}
	else{
		DD_DMA_Start(gDD_STM_CtrlTable[motorID].dmacCH, &(gDD_STM_MotorManager[motorID].constTable[1]), (DD_STM_PROF_CONST_STEP_SIZE - 1));
		DD_EVT_Put(evGoNext_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
}

//============================================================================= 
// Function Name : stPTExec
// Description   : stPTExec_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stPTExec(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stPTExec_ID;
}

//============================================================================= 
// Function Name : stPTExecPulseWait
// Description   : stPTExecPulseWait_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stPTExecPulseWait(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stPTExecPulseWait_ID;

	DD_TAU_IntEnable(gDD_STM_CtrlTable[motorID].timerCH);						// �w�胂�[�^�̃^�C�}���荞�݋���
}

//============================================================================= 
// Function Name : stPTJudge
// Description   : stPTJudge_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stPTJudge(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stPTJudge_ID;

	if( gDD_STM_MotorManager[motorID].count.controlType == DD_STM_COUNT_CLOCK ){
		gDD_STM_MotorManager[motorID].count.clockCount += DD_STM_PROF_CONST_STEP_SIZE;

		DD_EVT_Put(evGoStart_ID, evuDD_STM_ID, (UC*)(motorID), 0);
	}
	else{																		// controlType == DD_STM_PULSE_TIMING
		if( gDD_STM_MotorManager[motorID].count.remain > DD_STM_PROF_CONST_STEP_SIZE ){
			gDD_STM_MotorManager[motorID].count.remain -= DD_STM_PROF_CONST_STEP_SIZE;

			DD_EVT_Put(evGoStart_ID, evuDD_STM_ID, (UC*)(motorID), 0);
		}
		else{
			gDD_STM_MotorManager[motorID].count.remain = 0;

			DD_EVT_Put(evGoNext_ID, evuDD_STM_ID, (UC*)(motorID), 0);
		}
	}
}

//============================================================================= 
// Function Name : stPTAnswer
// Description   : stPTAnswer_ID�ւ̑J�ڏ���
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : 
//=============================================================================
void stPTAnswer(DD_STM_ID motorID)
{
	gDD_STM_MotorManager[motorID].state = stPTAnswer_ID;
	gDD_STM_MotorManager[motorID].count.controlType = DD_STM_NOT_COUNT_CLOCK;

	if( gDD_STM_MotorManager[motorID].count.func != NULL ){
		gDD_STM_MotorManager[motorID].count.func(DD_STM_RESULT_OK);
	}

	DD_EVT_Put(evComplete_ID, evuDD_STM_ID, (UC*)(motorID), 0);
}

//============================================================================= 
// Function Name : SetProfileIDExec
// Description   : Profile ID �Z�b�g����
// Parameter     : motorID		"���[�^��ID"
// Return        : 
// Note          : ��ԑJ�ڂ͍s��Ȃ�
//=============================================================================
void SetProfileIDExec(DD_STM_ID motorID, DD_STM_PROF_ID profileID)
{
	if( gDD_STM_Profile[profileID].profileID == profileID ){					// �v���t�@�C���ݒ�ς݂�profileID�̂Ƃ�
		gDD_STM_MotorManager[motorID].profileID = profileID;
		gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_Profile[profileID].idleIndex;
	}
	else{																		// �v���t�@�C���ݒ�����Ă��Ȃ�profileID�̂Ƃ�
		/* Irregular Case */
	}
}

//============================================================================= 
// Function Name : SetChangeClockData
// Description   : �N���b�N�ύX�ɕK�v�ȃf�[�^���Z�b�g
// Parameter     : motorID		"���[�^��ID"
// Return        : true			"�N���b�N�ύX����"
//				 : false		"�N���b�N�ύX�Ȃ�"
// Note          : 
//=============================================================================
bool SetChangeClockData(DD_STM_ID motorID)
{
	US nowIndex, targetIndex, indexConvPlsCnt, movePulseCnt;

	nowIndex = gDD_STM_MotorManager[motorID].nowIndex;
	targetIndex = gDD_STM_MotorManager[motorID].targetIndex;

	// ���݃C���f�b�N�X�A�ڕW�C���f�b�N�X�̐ݒ�
	if( ((targetIndex < gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].slowUpDownThresh) &&	// �ڕW�C���f�b�N�X�A���݃C���f�b�N�����ɁASlow-Up�C���f�b�N�X
		 (nowIndex < gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].slowUpDownThresh)) ||
		((targetIndex >= gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].slowUpDownThresh) &&	// �ڕW�C���f�b�N�X�A���݃C���f�b�N�����ɁASlow-Down�C���f�b�N�X
		 (nowIndex >= gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].slowUpDownThresh)) ){

		if( targetIndex < nowIndex ){
			if( gDD_STM_ConvertIndexTable[nowIndex / 2] == gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex ){	// ���݃C���f�b�N�X�̕ϊ��C���f�b�N�X��idle�̂Ƃ�
				gDD_STM_MotorManager[motorID].targetIndex = gDD_STM_MotorManager[motorID].nowIndex;	// �N���b�N�ύX���s��Ȃ�(���C���f�b�N�X�̕ϊ��悪Idle�̂���)

				indexConvPlsCnt = 0;
			}
			else{																// ���݃C���f�b�N�X�̕ϊ��C���f�b�N�X��idle�ł͂Ȃ��Ƃ�
				if( gDD_STM_ConvertIndexTable[targetIndex / 2] == gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex ){	// �ڕW�C���f�b�N�X�̕ϊ��C���f�b�N�X��idle�̂Ƃ�
					gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_ConvertIndexTable[nowIndex / 2];
					gDD_STM_MotorManager[motorID].targetIndex = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].profEndIndex;	// profEndIndex���Z�b�g����

					indexConvPlsCnt = 2;
				}
				else{															// �ڕW�C���f�b�N�X�̕ϊ��C���f�b�N�X��idle�ł͂Ȃ��Ƃ�
					gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_ConvertIndexTable[nowIndex / 2];
					gDD_STM_MotorManager[motorID].targetIndex = gDD_STM_ConvertIndexTable[targetIndex / 2];

					indexConvPlsCnt = 2;
				}
			}
		}
		else{
			indexConvPlsCnt = 0;
		}
	}
	else{
		if(	gDD_STM_ProfileTable[nowIndex] == gDD_STM_ProfileTable[targetIndex] ){	// ���݃C���f�b�N�X�̎��g���ƁA�ڕW�C���f�b�N�X�̎��g���������Ƃ�
			gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_MotorManager[motorID].targetIndex;

			indexConvPlsCnt = 0;
		}
		else{																	// ���݃C���f�b�N�X�̎��g���ƁA�ڕW�C���f�b�N�X�̎��g�����Ⴄ�Ƃ�
			if( gDD_STM_ConvertIndexTable[nowIndex / 2] == gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex ){	// ���݃C���f�b�N�X�̕ϊ��C���f�b�N�X��idle�̂Ƃ�
				gDD_STM_MotorManager[motorID].targetIndex = gDD_STM_ConvertIndexTable[targetIndex / 2];

				indexConvPlsCnt = 0;
			}
			else{																// ���݃C���f�b�N�X�̕ϊ��C���f�b�N�X��idle�ł͂Ȃ��Ƃ�
				if( gDD_STM_ConvertIndexTable[targetIndex / 2] == gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex ){	// �ڕW�C���f�b�N�X�̕ϊ��C���f�b�N�X��idle�̂Ƃ�
					gDD_STM_MotorManager[motorID].targetIndex = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].profEndIndex;	// profEndIndex���Z�b�g����

					indexConvPlsCnt = 0;
				}
				else{
					if( targetIndex < gDD_STM_ConvertIndexTable[nowIndex / 2] ){
						gDD_STM_MotorManager[motorID].targetIndex = gDD_STM_ConvertIndexTable[targetIndex / 2];

						indexConvPlsCnt = 0;
					}
					else{
						gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_ConvertIndexTable[nowIndex / 2];

						indexConvPlsCnt = 2;									// ���݃C���f�b�N�X�ϊ��������ꍇ�́A+2
					}
				}
			}
		}
	}

	// �N���b�N�o��
	movePulseCnt = gDD_STM_MotorManager[motorID].targetIndex - gDD_STM_MotorManager[motorID].nowIndex + indexConvPlsCnt;

	if( movePulseCnt > 0){
		if( indexConvPlsCnt == 0 ){												// ���݃C���f�b�N�X��ϊ����Ă��Ȃ��ꍇ
			DD_DMA_Start(gDD_STM_CtrlTable[motorID].dmacCH,					// ���C���f�b�N�X����N���b�N�o�͊J�n
						 &(gDD_STM_ProfileTable[gDD_STM_MotorManager[motorID].nowIndex + 2 + 1]),
						 (movePulseCnt - 1));
			DD_TAU_SetTDR(gDD_STM_CtrlTable[motorID].timerCH, gDD_STM_ProfileTable[gDD_STM_MotorManager[motorID].nowIndex + 2]);
			if( gDD_STM_MotorManager[motorID].nowIndex == gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex ){	// Clock�o�͒�~���̂Ƃ��́A�N���b�N�X�^�[�g����
				DD_TAU_Start(gDD_STM_CtrlTable[motorID].timerCH);
			}
		}
		else{																	// ���݃C���f�b�N�X��ϊ������ꍇ
			DD_DMA_Start(gDD_STM_CtrlTable[motorID].dmacCH,					// ���݃C���f�b�N�X����N���b�N�o�͊J�n
						 &(gDD_STM_ProfileTable[gDD_STM_MotorManager[motorID].nowIndex + 1]),
						 (movePulseCnt - 1));
			DD_TAU_SetTDR(gDD_STM_CtrlTable[motorID].timerCH, gDD_STM_ProfileTable[gDD_STM_MotorManager[motorID].nowIndex]);
			if( gDD_STM_MotorManager[motorID].nowIndex == gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex ){	// Clock�o�͒�~���̂Ƃ��́A�N���b�N�X�^�[�g����
				DD_TAU_Start(gDD_STM_CtrlTable[motorID].timerCH);
			}
		}

		return true;
	}
	else{
		return false;
	}
}

//============================================================================= 
// Function Name : SetStopClockData
// Description   : �N���b�N�ύX�ɕK�v�ȃf�[�^���Z�b�g
// Parameter     : motorID		"���[�^��ID"
// Return        : true			"Slow-Down����"
//				 : false		"Slow-Down�Ȃ�"
// Note          : 
//=============================================================================
bool SetStopClockData(DD_STM_ID motorID)
{
	US nowIndex, targetIndex, indexConvPlsCnt, movePulseCnt;

	nowIndex = gDD_STM_MotorManager[motorID].nowIndex;
	targetIndex = gDD_STM_MotorManager[motorID].targetIndex;

	// ���݃C���f�b�N�X�A�ڕW�C���f�b�N�X�̐ݒ�
	if( gDD_STM_ConvertIndexTable[nowIndex / 2] == gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex ){	// ���݃C���f�b�N�X�̕ϊ��C���f�b�N�X��idle�̂Ƃ�
		gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_MotorManager[motorID].targetIndex;

		indexConvPlsCnt = 0;
	}
	else{																		// ���݃C���f�b�N�X�̕ϊ��C���f�b�N�X��idle�ł͂Ȃ��Ƃ�
		if( nowIndex < gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].slowUpDownThresh ){	// ���݃C���f�b�N�X��Slow-Up�C���f�b�N�X�̂Ƃ�
			gDD_STM_MotorManager[motorID].nowIndex = gDD_STM_ConvertIndexTable[nowIndex / 2];
			gDD_STM_MotorManager[motorID].targetIndex = gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].profEndIndex;	// profEndIndex���Z�b�g����

			indexConvPlsCnt = 2;
		}
		else{																	// ���݃C���f�b�N�X��Slow-Down�C���f�b�N�X�̂Ƃ�
			indexConvPlsCnt = 0;
		}
	}

	// �N���b�N�o��
	movePulseCnt = gDD_STM_MotorManager[motorID].targetIndex - gDD_STM_MotorManager[motorID].nowIndex + indexConvPlsCnt;

	if( movePulseCnt > 0){
		if( indexConvPlsCnt == 0 ){												// ���݃C���f�b�N�X��ϊ����Ă��Ȃ��ꍇ
			DD_DMA_Start(gDD_STM_CtrlTable[motorID].dmacCH,					// ���C���f�b�N�X����N���b�N�o�͊J�n
						 &(gDD_STM_ProfileTable[gDD_STM_MotorManager[motorID].nowIndex + 2 + 1]),
						 (movePulseCnt - 1));
			DD_TAU_SetTDR(gDD_STM_CtrlTable[motorID].timerCH, gDD_STM_ProfileTable[gDD_STM_MotorManager[motorID].nowIndex + 2]);
			if( gDD_STM_MotorManager[motorID].nowIndex == gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex ){	// Clock�o�͒�~���̂Ƃ��́A�N���b�N�X�^�[�g����
				DD_TAU_Start(gDD_STM_CtrlTable[motorID].timerCH);
			}
		}
		else{																	// ���݃C���f�b�N�X��ϊ������ꍇ
			DD_DMA_Start(gDD_STM_CtrlTable[motorID].dmacCH,					// ���݃C���f�b�N�X����N���b�N�o�͊J�n
						 &(gDD_STM_ProfileTable[gDD_STM_MotorManager[motorID].nowIndex + 1]),
						 (movePulseCnt - 1));
			DD_TAU_SetTDR(gDD_STM_CtrlTable[motorID].timerCH, gDD_STM_ProfileTable[gDD_STM_MotorManager[motorID].nowIndex]);
			if( gDD_STM_MotorManager[motorID].nowIndex == gDD_STM_Profile[gDD_STM_MotorManager[motorID].profileID].idleIndex ){	// Clock�o�͒�~���̂Ƃ��́A�N���b�N�X�^�[�g����
				DD_TAU_Start(gDD_STM_CtrlTable[motorID].timerCH);
			}
		}

		return true;
	}
	else{
		return false;
	}
}

//============================================================================= 
// Function Name : SetCountStartIndex
// Description   : �o�̓N���b�N���J�E���g�̃f�[�^�Z�b�g
// Parameter     : arg  : ���[�^ID�l									
// Return        : -										
// Note          : -
//=============================================================================
void SetCountStartIndex(DD_STM_ID motorID)
{
	US remainCnt = DD_DMA_GetRemainTransferCnt(gDD_STM_CtrlTable[motorID].dmacCH);

	if( gDD_STM_MotorManager[motorID].count.countStartIndex == 0xFFFF ){
		gDD_STM_MotorManager[motorID].count.countStartIndex = gDD_STM_MotorManager[motorID].targetIndex - ((remainCnt / 2) * 2);
	}
	else{
		/* No Action(���łɃf�[�^�Z�b�g�����Ă��邽��) */
	}
}


//============================================================================= 
// Function Name : TransferEndCallback							
// Description   : �]���������ɃR�[���o�b�N����銄���ݏ���
// Parameter     : arg  : ���[�^ID�l									
// Return        : -										
// Note          : -
//=============================================================================
void TransferEndCallback(void* arg)
{
	DD_STM_ID motorID = (DD_STM_ID)(arg);

	switch( gDD_STM_MotorManager[motorID].state ){
	case stChangeExecute_ID:
	case stStopExecute_ID:
	case stSPChange_ID:
	case stSPConstant_ID:
	case stSPStop_ID:
	case stPTExec_ID:
	//����(TB-XXX) DMA�]�������ݏ������ł����AApp����Callback���Ȃ� -> �ȉ���"stOOEntry_ID"�ǉ�.
	case stSPChangeEntry_ID :
	case stSPConstEntry_ID :
	case stSPStopEntry_ID :
	case stChangeEntry_ID :
	case stPTEntry_ID :
	case stStopEntry_ID :
		DD_EVT_Put(evSend_ID, evuDD_STM_ID, (UC*)(motorID), 0);
		break;

	default:
		/* No Action */
		break;
	}
}

//============================================================================= 
// Function Name : TimeoutCallback							
// Description   : �^�C�}(CPU)�̃^�C���A�E�g���ɃR�[���o�b�N����銄���ݏ���									
// Parameter     : arg : ���[�^��ID�l											
// Return        : -									
// Note          : -
//=============================================================================
void TimeoutCallback(void* arg)
{
	DD_STM_ID motorID = (DD_STM_ID)(arg);

	if( DD_TAU_GetLevel(gDD_STM_CtrlTable[motorID].timerCH) == 0x00 ){		// TimeOut���x����Low�̏ꍇ�iHigh�Ȃ牽�����Ȃ��j
		switch( gDD_STM_MotorManager[motorID].state ){
		case stChangePulseWait_ID:
		case stStopPulseWait_ID:
		case stStopExecPulseWait_ID:
		case stSPPulseWait_ID:
		case stSPChangePulseWait_ID:
		case stSPConstPulseWait_ID:		// ���v���� T.B.D.
		case stSPStopPulseWait_ID:
		case stPTPulseWait_ID:
		case stPTExecPulseWait_ID:
			DD_EVT_Put(evGoComplete_ID, evuDD_STM_ID, (UC*)(motorID), 0);
			break;

		default:
			/* No Action */
			break;
		}

		DD_TAU_IntDisable(gDD_STM_CtrlTable[motorID].timerCH);					// Timer�����݂𖳌��ɂ���
	}
}


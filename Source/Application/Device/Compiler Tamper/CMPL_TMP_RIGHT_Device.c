// File Name:	CMPL_TMP_RIGHT_Device.c
// Description:	Compiler Right Tamper
// Engineer:	FXK
// Last Edit:	15.03.12.
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"
#include "DD_DIO.h"
#include "DD_STM.h"
#include "DD_INT.h"
#include "DD_ResourceAssign.h"
#include "MSP_EventTimer.h"
#include "ECMPL_TMP_Profile.h"
#include "CMPL_TMP_COMM_Device.h"
#include "CMPL_TMP_RIGHT_Device.h"
#include "CMPL_TMP_Boundary.h"

/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	ECMPL_TMP_Action action;
	ECMPL_TMP_Action compoAction;
	UC sheetNo;

	UC profile;		 // NewDD�K�p(Profile) //ECMPL_TMP_ProfileID profile;
	US targetIndex;
	US movePulse;
	UC direction;
	US startDelayTime;
	SS timerIndex;
	UC interruptIndex;
	UC sizePosAfterReturn;  //(CR-008)  Return�����ASizePositioning������{����/���Ȃ�
	UC offFailCheckCount;  //(V3.5.95) Home�O������`�F�b�NCount�B2��ȏ�Ȃ��HomeSNROffFail
} SCMP_TMP_RIGHT_Control;

typedef struct
{
	void (*mpHandler)(void) ;

} TMP_RIGHT_CallFunc;

static SCMP_TMP_RIGHT_Control 		gCMPL_TMP_RIGHT;
static EFaultStatus 		  		gCMPL_TMP_RIGHT_FaultStat;

#define CMPL_TMP_RIGHT_NORMAL_EVT		0
#define CMPL_TMP_RIGHT_INTERRUPT_EVT	1
#define CMPL_TMP_RIGHT_FAIL_EVT                  0x80  //0526 Fail Event added

/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

static void CMPL_TMP_RIGHT_ControlThread();
static void CMPL_TMP_RIGHT_ReceiveEventTimerTimeUp(SS aTimerID, UL aNotUsed);
static void CMPL_TMP_RIGHT_ReceiveSteppingMotorEvent(SS aResult);

static void CMPL_TMP_RIGHT_InterruptRTHomeSensorON();

static void CMPL_TMP_RIGHT_SetEventTimer(US aTime, UL aEventType);
static void CMPL_TMP_RIGHT_CancelEventTimer();
static US CMPL_TMP_RIGHT_GetSizePosition(UC aSheetNo);
static US CMPL_TMP_RIGHT_GetOffsetPosition(UC aSheetNo);
US CMPL_TMP_RIGHT_GetOffsetDirection(UC aSheetNo);
US CMPL_TMP_RIGHT_GetReturnPosition(UC aSheetNo);
US CMPL_TMP_RIGHT_GetReturnDirection(UC aSheetNo);

// Component����
// CMPL_TMP_COMPO_TAMPER_MOT = 0x0100,
static void  RT_Component_Mot_st1(); 	// Mot �쓮�ݒ�
static void  RT_Component_Mot_st2(); 	// ����J�n
static void  RT_Component_Mot_st3(); 	// ��㎥����
static void  RT_Component_Mot_st4(); 	// Mot Off


// �T�C�Y�|�W�V���j���O����
// CMPL_TMP_SIZE_POS = 0x0200,
static void RT_SizePositioning_st1(); 		// Tamper HomeSNR�@ON�`�F�b�N
static void RT_SizePositioning_st2_CCW(); 	// Home�O������ݒ�
static void RT_SizePositioning_st3_CCW();	// CCW�����Ƀ��[�^�쓮�J�n
static void RT_SizePositioning_st4_CCW(); 	// Home�O�����슮��
static void RT_SizePositioning_st2_CW(); 	// Home����ݒ�
static void RT_SizePositioning_st3_CW(); 	// CW�����Ƀ��[�^�쓮�J�n
static void RT_SizePositioning_st4_CW(); 	// StepUp���A�����ݎ�M

static void RT_SizePositioning_st4_2_CW(); 	// StepUp���A�����ݎ�M ���� StepDown����

static void RT_SizePositioning_st5_CW(); 	// StepUp������M
static void RT_SizePositioning_st6_CW(); 	// �����ԂŊ����ݎ�M
static void RT_SizePositioning_st7_CW(); 	// Home���슮��
static void RT_SizePositioning_st8();	 	// SizePositioning�J�n
static void RT_SizePositioning_st9();	 	// ���[�^�쓮�J�n
static void RT_SizePositioning_st10();  	// SizePosition�ֈړ�����


// �m�[�}���^���s���O����
// CMPL_TMP_NORMAL_TAMPING = 0x0300,

// �C�W�F�N�g�^���s���O����(SetLastTamping)
// CMPL_TMP_EJECT_TAMPING = 0x0400,

// �I�t�Z�b�g����
// CMPL_TMP_OFFSET = 0x0500,
static void RT_Offset_st1(); 	// �J�nTiming����
static void RT_Offset_st2(); 	// ���[�^�쓮�ݒ�
static void RT_Offset_st3(); 	// ���[�^�쓮�J�n
static void RT_Offset_st4(); 	// ���莞�ԃ^�C�}�ݒ�
static void RT_Offset_st5(); 	// ������҂�

// ���^�[������
// CMPL_TMP_RETURN = 0x0600,
static void RT_Return_st1(); 	// �쓮�������f
static void RT_Return_st2_CW(); // ����J�nTrigger�҂�(LeftOffset)
static void RT_Return_st3_CW(); // �^�C�}�ݒ�(LeftOffset)
static void RT_Return_st4_CW();	// ���[�^�쓮�ݒ�(LeftOffset)
static void RT_Return_st2_CCW();// ����J�nTrigger�҂�(RightOffset)
static void RT_Return_st3_CCW();// �^�C�}�ݒ�(RightOffset)
static void RT_Return_st4_CCW();// ���[�^�쓮�ݒ�(RightOffset)
static void RT_Return_st5();  	// ���[�^�쓮�J�n
static void RT_Return_st6();  	// �^�C�}�ݒ�
static void RT_Return_st7();  	// ������҂�
static void RT_Return_st8();	// SizePos�J�n(CR-008) 


// �I�[�v������(Nigasi)
// CMPL_TMP_OPEN = 0x0700,
static void RT_Open_st1(); 	// ���[�^�쓮�ݒ�
static void RT_Open_st2(); 	// ���[�^�쓮�J�n
static void RT_Open_st3(); 	// �^�C�}�ݒ�
static void RT_Open_st4(); 	// ������҂�
static void RT_Open_st5();  // ������҂�


static const TMP_RIGHT_CallFunc ControlThread_RT_COMPO_MOT[] = {
	{NULL},
	{RT_Component_Mot_st1},	// Mot �쓮�ݒ�
	{RT_Component_Mot_st2},	// ����J�n
	{RT_Component_Mot_st3},	// ��㎥����
	{RT_Component_Mot_st4},	// Mot Off
};

// �T�C�Y�|�W�V���j���O����
static const TMP_RIGHT_CallFunc ControlThread_RT_SIZEPOS[] = {
	{NULL},
	{RT_SizePositioning_st1}, 		// Tamper HomeSNR�@ON�`�F�b�N
	{RT_SizePositioning_st2_CCW}, 	// Home�O������ݒ�
	{RT_SizePositioning_st3_CCW}, 	// CCW�����Ƀ��[�^�쓮�J�n
	{RT_SizePositioning_st4_CCW}, 	// Home�O�����슮��
	{RT_SizePositioning_st2_CW}, 	// Home����ݒ�
	{RT_SizePositioning_st3_CW}, 	// CW�����Ƀ��[�^�쓮�J�n
	{RT_SizePositioning_st4_CW}, 	// StepUp���A�����ݎ�M
	{RT_SizePositioning_st4_2_CW}, 	// StepUp���A�����ݎ�M ���� StepDown����
	{RT_SizePositioning_st5_CW}, 	// StepUp������M
	{RT_SizePositioning_st6_CW}, 	// �����ԂŊ����ݎ�M
	{RT_SizePositioning_st7_CW}, 	// Home���슮��

	{RT_SizePositioning_st8},		// SizePositioning�J�n
	{RT_SizePositioning_st9},		// ���[�^�쓮�J�n
	{RT_SizePositioning_st10}, 		// SizePosition�ֈړ�����

};

// �m�[�}���^���s���O����
// �C�W�F�N�g�^���s���O����(SetLastTamping)

// �I�t�Z�b�g����
static const TMP_RIGHT_CallFunc ControlThread_RT_OFFSET[] = {
	{NULL},
	{RT_Offset_st1}, 	// �J�nTiming����
	{RT_Offset_st2}, 	// ���[�^�쓮�ݒ�
	{RT_Offset_st3}, 	// ���[�^�쓮�J�n
	{RT_Offset_st4}, 	// ���莞�ԃ^�C�}�ݒ�
	{RT_Offset_st5}, 	// ������҂�
};

// ���^�[������
static const TMP_RIGHT_CallFunc  ControlThread_RT_RETURN[] = {
	{NULL},
	{RT_Return_st1}, 		// �쓮�������f
	{RT_Return_st2_CW}, 	// ����J�nTrigger�҂�(LeftOffset)
	{RT_Return_st3_CW}, 	// �^�C�}�ݒ�(LeftOffset)
	{RT_Return_st4_CW},   	// ���[�^�쓮�ݒ�(LeftOffset)
	{RT_Return_st2_CCW},  	// ����J�nTrigger�҂�(RightOffset)
	{RT_Return_st3_CCW},  	// �^�C�}�ݒ�(RightOffset)
	{RT_Return_st4_CCW},  	// ���[�^�쓮�ݒ�(RightOffset)
	{RT_Return_st5}, 		// ���[�^�쓮�J�n
	{RT_Return_st6},  		// �^�C�}�ݒ�
	{RT_Return_st7},  		// ������҂�
	{RT_Return_st8},		// SizePos�J�n(CR-008) 
};

// �I�[�v������(Nigasi)
static const TMP_RIGHT_CallFunc  ControlThread_RT_OPEN[] = {
	{NULL},
	{RT_Open_st1}, //���[�^�쓮�ݒ�
	{RT_Open_st2}, //���[�^�쓮�J�n
	{RT_Open_st3}, //�^�C�}�ݒ�
	{RT_Open_st4}, //������҂�
	{RT_Open_st5}, //������҂�
};

/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_RIGHT_StartAct
// Description   : �^���p�[����ݒ�_����J�n
// Parameter     : aAction (���䂷�铮��)
//				 : aSheetNo (���䂷��SheetNo)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_TMP_RIGHT_StartAct(ECMPL_TMP_Action aAction, UC aSheetNo)
{

	switch ( aAction ) {

	// Component ���� //////////////////////////////////////////////////////////////////////////////
	// RIGH TTamper���O������(CW)�ɒᑬ(643PPS)�ňړ�
	case CMPL_TMP_COMPO_RIGHT_CW_LOW :
		gCMPL_TMP_RIGHT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_LOW;
		gCMPL_TMP_RIGHT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_LOW;

		gCMPL_TMP_RIGHT.direction = CMPL_TMP_COMM_OUTSIDE;
		gCMPL_TMP_RIGHT.compoAction = aAction;
		gCMPL_TMP_RIGHT.action = CMPL_TMP_COMPO_TAMPER_MOT_st1;
		break;

	 // RIGHT Tamper���O������(CW)�ɒ���(800PPS)�ňړ�
	case CMPL_TMP_COMPO_RIGHT_CW_MID :
		gCMPL_TMP_RIGHT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_MID;
		gCMPL_TMP_RIGHT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_MID;

		gCMPL_TMP_RIGHT.direction = CMPL_TMP_COMM_OUTSIDE;
		gCMPL_TMP_RIGHT.compoAction = aAction;
		gCMPL_TMP_RIGHT.action = CMPL_TMP_COMPO_TAMPER_MOT_st1;
		break;

	// RIGHT Tamper���O������(CW)�ɍ���(1337PPS)�ňړ�
	case CMPL_TMP_COMPO_RIGHT_CW_HI :
		gCMPL_TMP_RIGHT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_HI;
		gCMPL_TMP_RIGHT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_HI;

		gCMPL_TMP_RIGHT.direction = CMPL_TMP_COMM_OUTSIDE;
		gCMPL_TMP_RIGHT.compoAction = aAction;
		gCMPL_TMP_RIGHT.action = CMPL_TMP_COMPO_TAMPER_MOT_st1;
		break;

	  // RIGHT Tamper���������(CCW)�ɒᑬ(643PPS)�ňړ�
	case CMPL_TMP_COMPO_RIGHT_CCW_LOW :
		gCMPL_TMP_RIGHT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_LOW;
		gCMPL_TMP_RIGHT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_LOW;

		gCMPL_TMP_RIGHT.direction = CMPL_TMP_COMM_INSIDE;
		gCMPL_TMP_RIGHT.compoAction = aAction;
		gCMPL_TMP_RIGHT.action = CMPL_TMP_COMPO_TAMPER_MOT_st1;
		break;

	// RIGHT Tamper���������(CCW)�ɒ���(800PPS)�ňړ�
	case CMPL_TMP_COMPO_RIGHT_CCW_MID :
		gCMPL_TMP_RIGHT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_MID;
		gCMPL_TMP_RIGHT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_MID;

		gCMPL_TMP_RIGHT.direction = CMPL_TMP_COMM_INSIDE;
		gCMPL_TMP_RIGHT.compoAction = aAction;
		gCMPL_TMP_RIGHT.action = CMPL_TMP_COMPO_TAMPER_MOT_st1;
		break;

	// RIGHT Tamper���������(CCW)�ɍ���(1337PPS)�ňړ�
	case CMPL_TMP_COMPO_RIGHT_CCW_HI :
		gCMPL_TMP_RIGHT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_HI;
		gCMPL_TMP_RIGHT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_HI;

		gCMPL_TMP_RIGHT.direction = CMPL_TMP_COMM_INSIDE;
		gCMPL_TMP_RIGHT.compoAction = aAction;
		gCMPL_TMP_RIGHT.action = CMPL_TMP_COMPO_TAMPER_MOT_st1;
		break;
	// Component ���� //////////////////////////////////////////////////////////////////////////////

	// �T�C�Y�|�W�V���j���O����
	case CMPL_TMP_COMPO_RIGHT_SENSOR :
	case CMPL_TMP_SIZE_POS :  // = 0x0200,  Size Positioning

		gCMPL_TMP_RIGHT.movePulse = CMPL_TMP_RIGHT_GetSizePosition(aSheetNo);
		gCMPL_TMP_RIGHT.profile = CMPL_TMP_PRF_ID_INITIALIZE;
		gCMPL_TMP_RIGHT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_INIT;

		gCMPL_TMP_RIGHT.direction =CMPL_TMP_COMM_INSIDE;
		gCMPL_TMP_RIGHT.action = CMPL_TMP_SIZE_POS_st1;
		gCMPL_TMP_RIGHT.offFailCheckCount = 0;
		
		PrintTamperLog_Short(CMPL_TMP_SIZE_POS, gCMPL_TMP_RIGHT.movePulse, aSheetNo);
		break;

	// �m�[�}���^���s���O����
	case CMPL_TMP_NORMAL_TAMPING : // = 0x0300,
	// �C�W�F�N�g�^���s���O����(SetLastTamping)
	case CMPL_TMP_EJECT_TAMPING :  //= 0x0400,

		if (gCMPL_TMP_RIGHT.action == CMPL_TMP_st0) { //��~��

			DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_ENB, ON);
			DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, LOW); //��d���ݒ�
			
			gCMPL_TMP_RIGHT.action = CMPL_TMP_st0;
			gCMPL_TMP_RIGHT.sheetNo = SHEETNO_NUM;
			
		} else { //���쒆
		
		}
		CMPL_TMP_InformedRightTamperComplete(aAction, aSheetNo);
		return;   //0502 
		//break;

	// �I�t�Z�b�g����
	case CMPL_TMP_OFFSET :  //= 0x0500,	
		gCMPL_TMP_RIGHT.movePulse = CMPL_TMP_RIGHT_GetOffsetPosition(aSheetNo);
		gCMPL_TMP_RIGHT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_LOW; // �ʏ� �ᑬ Profile
		gCMPL_TMP_RIGHT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_LOW;

		gCMPL_TMP_RIGHT.direction= (UC)CMPL_TMP_RIGHT_GetOffsetDirection(aSheetNo);
		gCMPL_TMP_RIGHT.action = CMPL_TMP_OFFSET_st1;
		PrintTamperLog_Short(CMPL_TMP_OFFSET, gCMPL_TMP_RIGHT.movePulse, aSheetNo);
		break;

	// ���^�[������
	case CMPL_TMP_RIGHT_RETURN : // = 0x0700,
		gCMPL_TMP_RIGHT.movePulse = CMPL_TMP_RIGHT_GetReturnPosition(aSheetNo);
		gCMPL_TMP_RIGHT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_HI; // �ʏ� ���� Profile
	
		gCMPL_TMP_RIGHT.direction=  (UC)CMPL_TMP_RIGHT_GetReturnDirection(aSheetNo);
		gCMPL_TMP_RIGHT.action = CMPL_TMP_RIGHT_RETURN_st1;
		gCMPL_TMP_RIGHT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_HI;

	   //CR-010
		if ((gCMPL_TMP_RIGHT.movePulse == 10)  &&  (gCMPL_TMP_RIGHT.direction == CMPL_TMP_COMM_OUTSIDE)) {

			gCMPL_TMP_RIGHT.sizePosAfterReturn = true;				
		} else {
			gCMPL_TMP_RIGHT.sizePosAfterReturn = false;			
		}	
		PrintTamperLog_Short(CMPL_TMP_RIGHT_RETURN, gCMPL_TMP_RIGHT.movePulse, aSheetNo);
		break;

	// �I�[�v������(Nigasi)
	case CMPL_TMP_OPEN :  // = 0x0800,		
		gCMPL_TMP_RIGHT.profile = CMPL_TMP_PRF_ID_NORMAL_HI; // �ʏ� ���� Profile
		gCMPL_TMP_RIGHT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_HI;

		gCMPL_TMP_RIGHT.direction= CMPL_TMP_COMM_OUTSIDE;
		gCMPL_TMP_RIGHT.action = CMPL_TMP_OPEN_st1;
		PrintTamperLog_Short(CMPL_TMP_OPEN, 0, aSheetNo);
		break;

	default  :  break;

	}
	gCMPL_TMP_RIGHT.sheetNo = aSheetNo;
	gCMPL_TMP_RIGHT.timerIndex = MSP_MAX_TMR_NUM;

	CMPL_TMP_RIGHT_ControlThread();
}



///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_RIGHT_ControlThread
// Description   : RIGHT �^���p�[����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_TMP_RIGHT_ControlThread()
{

	ECMPL_TMP_Action aAction =  gCMPL_TMP_RIGHT.action & 0xFF00;
	US  				 aStatus =  gCMPL_TMP_RIGHT.action & 0x00FF;

	if ( aStatus == 0 )  {
		CMPL_TMP_RIGHT_DeviceStop();
		return;
	}

	switch ( aAction)	{
	//Component
	case  CMPL_TMP_COMPO_TAMPER_MOT :
		//ControlThread_RT_COMPO_MOT[aStatus](); break;
		ControlThread_RT_COMPO_MOT[aStatus].mpHandler();  break;

	// �T�C�Y�|�W�V���j���O����
	case CMPL_TMP_SIZE_POS :  // = 0x0200,  Size Positioning
		ControlThread_RT_SIZEPOS[aStatus].mpHandler(); break;

	// �I�t�Z�b�g����
	case CMPL_TMP_OFFSET :  //= 0x0500,
		ControlThread_RT_OFFSET[aStatus].mpHandler(); break;

	// ���^�[������
	case CMPL_TMP_RIGHT_RETURN : // = 0x0700,
		ControlThread_RT_RETURN[aStatus].mpHandler(); break;

	// �I�[�v������(Nigasi)
	case CMPL_TMP_OPEN :  // = 0x0800,
		ControlThread_RT_OPEN[aStatus].mpHandler(); break;

	default : 							break;
	}
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_RIGHT_ReceiveEventTimerTimeUp
// Description   : �C�x���g�^�C�}�̃^�C���A�b�v
// Parameter     : timerID (Timer ID)
//               : notUsed (Not Used)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_TMP_RIGHT_ReceiveEventTimerTimeUp(SS aTimerID, UL aEvtType)
{
	if ( aTimerID != gCMPL_TMP_RIGHT.timerIndex ) {
		return;
	}

	gCMPL_TMP_RIGHT.timerIndex = MSP_MAX_TMR_NUM;

	
	if ( aEvtType <  CMPL_TMP_RIGHT_FAIL_EVT) { //NORMAL EVT

		switch ( gCMPL_TMP_RIGHT.action ) {
		// Component
		case CMPL_TMP_COMPO_TAMPER_MOT_st1 :
		case CMPL_TMP_COMPO_TAMPER_MOT_st3 :
			gCMPL_TMP_RIGHT.action++;
			CMPL_TMP_RIGHT_ControlThread();
			break;

		// Size Positioning
		case CMPL_TMP_SIZE_POS_st2_CCW :  // 20ms Timeout �O�㎥����
			gCMPL_TMP_RIGHT.action = CMPL_TMP_SIZE_POS_st3_CCW;
			CMPL_TMP_RIGHT_ControlThread();
			break;
		case CMPL_TMP_SIZE_POS_st4_CCW : // Home �O�����슮�� -> 50ms Timeout
			gCMPL_TMP_RIGHT.action = CMPL_TMP_SIZE_POS_st2_CW; // Home ����ݒ�
			CMPL_TMP_RIGHT_ControlThread();
			break;
		case CMPL_TMP_SIZE_POS_st2_CW : // 20ms Timeout �O�㎥����
			gCMPL_TMP_RIGHT.action = CMPL_TMP_SIZE_POS_st3_CW;
			CMPL_TMP_RIGHT_ControlThread();
			break;
		case CMPL_TMP_SIZE_POS_st4_2_CW : //50ms Timeout
			gCMPL_TMP_RIGHT.action = CMPL_TMP_SIZE_POS_st2_CCW; //Home�O�������
			CMPL_TMP_RIGHT_ControlThread();
			break;
		case CMPL_TMP_SIZE_POS_st7_CW : //50ms Timeout
			gCMPL_TMP_RIGHT.action = CMPL_TMP_SIZE_POS_st8; //Home���슮�� -> Size Positioning Start ..
			CMPL_TMP_RIGHT_ControlThread();
			break;
		case CMPL_TMP_SIZE_POS_st8: //20ms Timeout��㎥����
			gCMPL_TMP_RIGHT.action++;
			CMPL_TMP_RIGHT_ControlThread();
			break;

		// Offset
		case CMPL_TMP_OFFSET_st1 : 		// �J�nTiming����
		case CMPL_TMP_OFFSET_st2 : 		// ���[�^�쓮�ݒ�
		case CMPL_TMP_OFFSET_st4 : 		// ���莞�ԃ^�C�}�ݒ�
			gCMPL_TMP_RIGHT.action++;
			CMPL_TMP_RIGHT_ControlThread();
			break;

		// Return
		case CMPL_TMP_RIGHT_RETURN_st3_CW : 	// �J�nTiming����
		case CMPL_TMP_RIGHT_RETURN_st3_CCW : 	// �J�nTiming����
		case CMPL_TMP_RIGHT_RETURN_st6 : 		// �^�C�}�ݒ�
		case CMPL_TMP_RIGHT_RETURN_st7 :            // (CR-008)
			gCMPL_TMP_RIGHT.action++;
			CMPL_TMP_RIGHT_ControlThread();
			break;
		case CMPL_TMP_RIGHT_RETURN_st4_CW :    	// ���[�^�쓮�ݒ�
		case CMPL_TMP_RIGHT_RETURN_st4_CCW : 	// ���[�^�쓮�ݒ�
			gCMPL_TMP_RIGHT.action  = CMPL_TMP_RIGHT_RETURN_st5;
			CMPL_TMP_RIGHT_ControlThread();
			break;

		// Open
		case CMPL_TMP_OPEN_st1 : 		// ���[�^�쓮�ݒ�
		case CMPL_TMP_OPEN_st4 : 		// �^�C�}�ݒ�
			gCMPL_TMP_RIGHT.action++;
			CMPL_TMP_RIGHT_ControlThread();
			break;
	
		default:
			break;
		}
	} else {  //Fail Event 

		switch ( gCMPL_TMP_RIGHT.action ) {

		case CMPL_TMP_SIZE_POS_st3_CW : // Fail  Timeout
		case CMPL_TMP_SIZE_POS_st5_CW : // Fail  Timeout(V3.5.93Tamper�s�(Fail�ʒm����Ȃ�))
				CMPL_TMP_InformedRightTamperHomeSensorOnFail(gCMPL_TMP_RIGHT.sheetNo);
				break;

		case CMPL_TMP_OPEN_st2 :		// Fail Timeout
				CMPL_TMP_InformedRightTamperHomeSensorOnFail(gCMPL_TMP_RIGHT.sheetNo);
				break;
		default : break; 
		}	

	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_RIGHT_ReceiveSteppingMotorEvent
// Description   :
// Parameter     : timerID (Timer ID)
//               : notUsed (Not Used)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_TMP_RIGHT_ReceiveSteppingMotorEvent(SS aResult)
{
	switch ( gCMPL_TMP_RIGHT.action ) {

	// Component
	case CMPL_TMP_COMPO_TAMPER_MOT_st2 :	// StepDown Wait ..
		gCMPL_TMP_RIGHT.action = CMPL_TMP_COMPO_TAMPER_MOT_st3;
		CMPL_TMP_RIGHT_ControlThread();
		break;

	case CMPL_TMP_SIZE_POS_st3_CCW :		// Home �O�����슮�� Event
		if ( DD_DIO_GetInput(DD_DI_RIGHT_TAMPER_HOME_SNR) == HIGH ) {  // Tamper HomeSNR Off Fail
			gCMPL_TMP_RIGHT_FaultStat = FAULT_OCCUR;
			CMPL_TMP_InformedRightTamperHomeSensorOffFail(gCMPL_TMP_RIGHT.sheetNo);
		} else {
			gCMPL_TMP_RIGHT.action = CMPL_TMP_SIZE_POS_st4_CCW;
			CMPL_TMP_RIGHT_ControlThread();
		}
		break;

	case CMPL_TMP_SIZE_POS_st3_CW : 		// Home���슮�� Event
		// �葬���AON������
		gCMPL_TMP_RIGHT.action = CMPL_TMP_SIZE_POS_st5_CW;
		break;

	case CMPL_TMP_SIZE_POS_st4_CW : 	// StepDown���� Event
		gCMPL_TMP_RIGHT.action = CMPL_TMP_SIZE_POS_st4_2_CW;
		CMPL_TMP_RIGHT_ControlThread();
		break;

	case CMPL_TMP_SIZE_POS_st6_CW :
		gCMPL_TMP_RIGHT.action = CMPL_TMP_SIZE_POS_st7_CW;
		CMPL_TMP_RIGHT_ControlThread();
		break;

	case CMPL_TMP_SIZE_POS_st9 : 	// SizePositioning Pulse A  �ړ�����
		gCMPL_TMP_RIGHT.action = CMPL_TMP_SIZE_POS_st10;
		CMPL_TMP_RIGHT_ControlThread();
		break;

	// Offset
	case CMPL_TMP_OFFSET_st3 : 		// ���[�^�쓮�J�n
		gCMPL_TMP_RIGHT.action++;
		CMPL_TMP_RIGHT_ControlThread();
		break;

	// Return
	case CMPL_TMP_RIGHT_RETURN_st5 : // ���[�^�쓮�J�n
		gCMPL_TMP_RIGHT.action++;
		CMPL_TMP_RIGHT_ControlThread();
		break;
	// Open
	case CMPL_TMP_OPEN_st3 : 		// ���[�^�쓮�J�n
		gCMPL_TMP_RIGHT.action++;
		CMPL_TMP_RIGHT_ControlThread();
		break;

	default:
		break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name 	: CMPL_TMP_RIGHT_RTHomeSensorON
// Description 		:  RightHomeSensor ON
// Parameter    	:
///////////////////////////////////////////////////////////////////
void CMPL_TMP_RIGHT_RTHomeSensorON()
{
}

///////////////////////////////////////////////////////////////////
// Function Name	: CMPL_TMP_RIGHT_RTHomeSensorON
// Description 		:  RightHomeSensor ON
// Parameter     	:
///////////////////////////////////////////////////////////////////
void CMPL_TMP_RIGHT_InterruptRTHomeSensorON()
{
	if ( gCMPL_TMP_RIGHT.action == CMPL_TMP_st0 )
				return;
	
	DD_INT_CancelCallback(DD_INT_RIGHT_TAMPER_HOME_SENSOR, TMP_RISE, gCMPL_TMP_RIGHT.interruptIndex);

	switch ( gCMPL_TMP_RIGHT.action ) {
	case CMPL_TMP_SIZE_POS_st3_CW :  // StepUp���A�����ݎ�M
		CMPL_TMP_RIGHT_CancelEventTimer();	// Fail Timer cancel ..
		gCMPL_TMP_RIGHT.action = CMPL_TMP_SIZE_POS_st4_CW;
		CMPL_TMP_RIGHT_ControlThread();
		break;

	case CMPL_TMP_SIZE_POS_st5_CW :  // �����ԂŊ����ݎ�M
		CMPL_TMP_RIGHT_CancelEventTimer();	// Fail Timer cancel ..
		gCMPL_TMP_RIGHT.action = CMPL_TMP_SIZE_POS_st6_CW;
		CMPL_TMP_RIGHT_ControlThread();
		break;

	case CMPL_TMP_OPEN_st2 :   // �����ݎ�M
		CMPL_TMP_RIGHT_CancelEventTimer();	// Fail Timer cancel ..
		gCMPL_TMP_RIGHT.action++;
		CMPL_TMP_RIGHT_ControlThread();
		break;

	default :
		break;
	}
}

void CMPL_TMP_RIGHT_RTHomeSensorOFF()
{
}

///////////////////////////////////////////////////////////////////
// Function Name 	: CMPL_TMP_RIGHT_InformedEjectStart
// Description 		:  Eject ����J�n�ʒm
// Parameter     	:
///////////////////////////////////////////////////////////////////
void CMPL_TMP_RIGHT_InformedEjectStart()
{
	if ( gCMPL_TMP_RIGHT.action == CMPL_TMP_RIGHT_RETURN_st2_CW ) {
		gCMPL_TMP_RIGHT.action = CMPL_TMP_RIGHT_RETURN_st3_CW;
		CMPL_TMP_RIGHT_ControlThread();
	}
}

///////////////////////////////////////////////////////////////////
// Function Name 	: CMPL_TMP_RIGHT_InformedEjectSheetThrough
// Description 		:  EjectMot�����J�n�ʒm
// Parameter     	:
///////////////////////////////////////////////////////////////////
void CMPL_TMP_RIGHT_InformedEjectSheetThrough()
{
	if ( gCMPL_TMP_RIGHT.action == CMPL_TMP_RIGHT_RETURN_st2_CCW ) {
		gCMPL_TMP_RIGHT.action = CMPL_TMP_RIGHT_RETURN_st3_CCW;
		CMPL_TMP_RIGHT_ControlThread();
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_RIGHT_SetEventTimer
// Description   : �C�x���g�^�C�}�[�Z�b�g
// Parameter     : aTime (�^�C�}�[�l (�~1[ms]))
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_TMP_RIGHT_SetEventTimer(US aTime, UL aEventType)
{
	gCMPL_TMP_RIGHT.timerIndex = MSP_SetEventTimer(aTime, CMPL_TMP_RIGHT_ReceiveEventTimerTimeUp, aEventType);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_RIGHT_CancelEventTimer
// Description   : �C�x���g�^�C�}�[�Z�b�g
// Parameter     : aTime (�^�C�}�[�l (�~1[ms]))
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_TMP_RIGHT_CancelEventTimer()
{
	if ( gCMPL_TMP_RIGHT.timerIndex != MSP_MAX_TMR_NUM ) {
		MSP_CancelEventTimer(gCMPL_TMP_RIGHT.timerIndex);
		gCMPL_TMP_RIGHT.timerIndex = MSP_MAX_TMR_NUM;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_RIGHT_IsExecutiongDeviceAction
// Description   : ���A�^���p�[�̓����Ԃ��擾
// Parameter     : �]
// Return        : true�F���쒆�Afalse�F��~��
///////////////////////////////////////////////////////////////////
bool CMPL_TMP_RIGHT_IsExecutiongDeviceAction()
{
	if( gCMPL_TMP_RIGHT.action == CMPL_TMP_st0 ) {
		return false;
	} else {
		return true;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name :
// Description   :
// Parameter     :
// Return        :
///////////////////////////////////////////////////////////////////
bool CMPL_TMP_RIGHT_IsOccurFault()
{
	if ( gCMPL_TMP_RIGHT_FaultStat == FAULT_OCCUR ) {
		return true;
	} else {
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name :
// Description   :
// Parameter     :
// Return        :
///////////////////////////////////////////////////////////////////
UC CMPL_TMP_RIGHT_GetControlSheetNo()
{
	return gCMPL_TMP_RIGHT.sheetNo;
}

///////////////////////////////////////////////////////////////////
// Function Name :
// Description   :
// Parameter     :
// Return        :
///////////////////////////////////////////////////////////////////
US CMPL_TMP_RIGHT_GetTamperPosition(UC aSheetNo)
{
	US aPosition;

	if ( aSheetNo == SHEETNO_NUM ) {
		aPosition = 0; // gCMPL_TMP_RIGHT.nowPosition;
	} else {
		aPosition = CMPL_TMP_RIGHT_GetSizePosition(aSheetNo);
	}

	return aPosition;
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_RIGHT_Reset
// Description   : �^���p�[ ���Z�b�g
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_TMP_RIGHT_Reset()
{
	gCMPL_TMP_RIGHT.action = CMPL_TMP_st0;
	gCMPL_TMP_RIGHT.sheetNo = SHEETNO_NUM;
	gCMPL_TMP_RIGHT.compoAction = 0;

	gCMPL_TMP_RIGHT.profile = 0;
	gCMPL_TMP_RIGHT.targetIndex = 0;
	gCMPL_TMP_RIGHT.direction = CMPL_TMP_COMM_OUTSIDE;

	gCMPL_TMP_RIGHT.timerIndex = MSP_MAX_TMR_NUM;
	gCMPL_TMP_RIGHT.sizePosAfterReturn = false;
	gCMPL_TMP_RIGHT.offFailCheckCount = 0;

	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_ENB, OFF);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, LOW);
	DD_STM_ShutDownStop(DD_STM_ID_RIGHT_TAMPER);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_RIGHT_DeviceStop
// Description   : �^���p�[��~
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_TMP_RIGHT_DeviceStop()
{
	ECMPL_TMP_Action aAction = gCMPL_TMP_RIGHT.action & CMPL_TMP_ACTION_MASK;
	UC aSheetNo = gCMPL_TMP_RIGHT.sheetNo;

	if ( aAction == CMPL_TMP_COMPO_TAMPER_MOT ) {
		aAction = gCMPL_TMP_RIGHT.compoAction;
	}

	gCMPL_TMP_RIGHT.action = CMPL_TMP_st0;
	gCMPL_TMP_RIGHT.sheetNo = SHEETNO_NUM;
	gCMPL_TMP_RIGHT.sizePosAfterReturn = false;
	gCMPL_TMP_RIGHT.offFailCheckCount = 0;

	CMPL_TMP_RIGHT_CancelEventTimer();

	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_ENB, OFF);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, LOW);
	DD_STM_ShutDownStop(DD_STM_ID_RIGHT_TAMPER);

	CMPL_TMP_InformedRightTamperComplete(aAction, aSheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_RIGHT_GetSizePosition
// Description   : �T�C�Y�|�W�V�����ʒu���擾
// Parameter     : aSheetNo (�ΏۃV�[�g�i���o�[)
// Return        : �T�C�Y�|�W�V�����ʒu
///////////////////////////////////////////////////////////////////
US CMPL_TMP_RIGHT_GetSizePosition(UC aSheetNo)
{
	float aPosition;
	float aWidth = (float)SRVC_GetCrossProcessSize(aSheetNo);

	if ( (CMPL_TMP_IsCanTampingSheetSize(aSheetNo) == true ) &&
		(SRVC_GetCrossProcessSizeMixSet(aSheetNo) == SHT_SIZE_UNMIX) ) {

		switch ( SRVC_GetStapleMode(aSheetNo) ) {

		case eCdiFinStaple_Off :
			aPosition = (1360.0 - (aWidth / 2.0) - 100.0) / CMPL_TMP_COMM_MOT_STEP + 5;
			aPosition /= 10;
			break;
		case eCdiFinStaple_SingleFront :
			aPosition = 0;  // Home�ʒu(Staple�ʒu)
			break;

		default :
			aPosition = 0;
			break;
		}

		if ( aPosition < 0 ) {
			aPosition = 0;															// Irregular Case
		}

	} else {
		aPosition = 0;
	}

	return (US)aPosition;
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_RIGHT_GetOffsetPosition
// Description   : Offset�ʒu�擾
// Parameter     : aSheetNo (�ΏۃV�[�g�i���o�[)
// Return        : Offset �ʒu
///////////////////////////////////////////////////////////////////
US CMPL_TMP_RIGHT_GetOffsetPosition(UC aSheetNo)
{
	US aPosition=0;

	switch ( SRVC_GetStapleMode(aSheetNo) ) {

	case eCdiFinStaple_Off :
		if ( SRVC_GetOffsetFinish(aSheetNo) == SHT_OFST_FIN_LEFT ) {
			aPosition = 0;
		} else if ( SRVC_GetOffsetFinish(aSheetNo) == SHT_OFST_FIN_RIGHT ) {
			//CR-010
			aPosition =SRVC_GetNvmValueUS(NVMID_TAMPER_OFFSET_PUSH_PULSE); // CMPL_TMP_COMM_OFFSET_PLS;
		}
		break;

	case eCdiFinStaple_SingleFront :

		if ( SRVC_GetOffsetFinish(aSheetNo) == SHT_OFST_FIN_LEFT ) {
			//CR-010
			aPosition = SRVC_GetNvmValueUS(NVMID_TAMPER_OFFSET_PUSH_PULSE); //CMPL_TMP_COMM_OFFSET_PLS;
		} else if ( SRVC_GetOffsetFinish(aSheetNo) == SHT_OFST_FIN_RIGHT ) {
			aPosition = 0;
		}
		break;

	default :																	// Irregular Case
		aPosition = 0;
		break;
	}

	return  aPosition;
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_RIGHT_GetOffsetDirection
// Description   : Offset���� �擾
// Parameter     : aSheetNo (�ΏۃV�[�g�i���o�[)
// Return        : Offset ����
///////////////////////////////////////////////////////////////////
US CMPL_TMP_RIGHT_GetOffsetDirection(UC aSheetNo)
{
	US  aDirection=0;

	switch ( SRVC_GetStapleMode(aSheetNo) ) {

	case eCdiFinStaple_Off :
		if ( SRVC_GetOffsetFinish(aSheetNo) == SHT_OFST_FIN_RIGHT ) {
			aDirection= CMPL_TMP_COMM_OUTSIDE;	// CW
		}
		break;

	case eCdiFinStaple_SingleFront :
		if ( SRVC_GetOffsetFinish(aSheetNo) == SHT_OFST_FIN_LEFT ) {
			aDirection= CMPL_TMP_COMM_INSIDE; 	// CCW
		}
		break;

	default :				// Irregular Case
			break;
	}

	return  aDirection;
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_RIGHT_GetReturnPosition
// Description   : Return�ړ��� �擾
// Parameter     : aSheetNo (�ΏۃV�[�g�i���o�[)
// Return        : Return �ړ���
///////////////////////////////////////////////////////////////////
US CMPL_TMP_RIGHT_GetReturnPosition(UC aSheetNo)
{
	float  aPosition=0;
	float aWidth = (float)(SRVC_GetCrossProcessSize(aSheetNo));

	switch ( SRVC_GetStapleMode(aSheetNo) ) {

	case eCdiFinStaple_Off :
		if ( SRVC_GetOffsetFinish(aSheetNo) == SHT_OFST_FIN_RIGHT ) { // Right Offset
			// Offset(80)
			aPosition = SRVC_GetNvmValueUS(NVMID_TAMPER_OFFSET_PUSH_PULSE); //CMPL_TMP_COMM_OFFSET_PLS;
		}
		break;

	case eCdiFinStaple_SingleFront :
		if ( SRVC_GetOffsetFinish(aSheetNo) == SHT_OFST_FIN_LEFT ) {  //Left Offset
			 //CR-010 - 10Pulse �ړ�
			aPosition = 10;
		}
		break;

	default :				// Irregular Case
			break;
	}
	return  (US)aPosition;
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_RIGHT_GetReturnDirection
// Description   : Return���� �擾
// Parameter     : aSheetNo (�ΏۃV�[�g�i���o�[)
// Return        : Return ����
///////////////////////////////////////////////////////////////////
US CMPL_TMP_RIGHT_GetReturnDirection(UC aSheetNo)
{
	US  aDirection=0;

	switch ( SRVC_GetStapleMode(aSheetNo) ) {

	case eCdiFinStaple_Off :
		if ( SRVC_GetOffsetFinish(aSheetNo) == SHT_OFST_FIN_RIGHT ) {
			aDirection= CMPL_TMP_COMM_INSIDE;	// CCW
		}
		break;

	case eCdiFinStaple_SingleFront :
		if ( SRVC_GetOffsetFinish(aSheetNo) == SHT_OFST_FIN_LEFT ) {
			aDirection= CMPL_TMP_COMM_OUTSIDE; // CW
		}
		break;

	default :									// Irregular Case
			break;
	}

	return  aDirection;
}


///////////////////////////////////////////////////////////////////
// Function Name : RT_Component_Mot_st1
// Description   : Mot �쓮�ݒ�
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_Component_Mot_st1()
{
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_DIR, gCMPL_TMP_RIGHT.direction);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, HIGH);

	CMPL_TMP_RIGHT_SetEventTimer(CMPL_TMP_COMM_MOTOR_ENABLE_TIME_20, CMPL_TMP_RIGHT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Component_Mot_st2
// Description   : ����J�n
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_Component_Mot_st2()
{
	DD_STM_SetProfileID(DD_STM_ID_RIGHT_TAMPER,	gCMPL_TMP_RIGHT.profile);
	DD_STM_MoveBySetPulse(DD_STM_ID_RIGHT_TAMPER,  gCMPL_TMP_RIGHT.targetIndex,
						     CMPL_TMP_COMPO_TOTAL_PULSE, CMPL_TMP_RIGHT_ReceiveSteppingMotorEvent);
}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Component_Mot_st3
// Description	 //��㎥����
// Parameter	 : �]
// Return		 : �]
///////////////////////////////////////////////////////////////////
void RT_Component_Mot_st3()
{
  	CMPL_TMP_RIGHT_SetEventTimer(CMPL_TMP_COMM_MOTOR_STOP_TIME_20, CMPL_TMP_RIGHT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Component_Mot_st4
// Description   ://Mot Off
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_Component_Mot_st4()
{
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_ENB, OFF);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_DIR, OFF);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, LOW);

	CMPL_TMP_InformedCompoComplete((US)gCMPL_TMP_RIGHT.compoAction);

	gCMPL_TMP_RIGHT.action = CMPL_TMP_st0;
	gCMPL_TMP_RIGHT.sheetNo = SHEETNO_NUM;
}


// �T�C�Y�|�W�V���j���O����
// CMPL_TMP_SIZE_POS = 0x0200,
///////////////////////////////////////////////////////////////////
// Function Name : RT_SizePositioning_st1
// Description   : Tamper HomeSNR�@ON�`�F�b�N
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_SizePositioning_st1()
{
	// 1. Tamper Home SNR Level Check
	if ( DD_DIO_GetInput(DD_DI_RIGHT_TAMPER_HOME_SNR) == HIGH )  // HIGH=
	   gCMPL_TMP_RIGHT.action = CMPL_TMP_SIZE_POS_st2_CCW;  // Home�O������ݒ�
	else
	   gCMPL_TMP_RIGHT.action = CMPL_TMP_SIZE_POS_st2_CW;  	// Home����ݒ�

	CMPL_TMP_RIGHT_ControlThread();
}

///////////////////////////////////////////////////////////////////
// Function Name : RT_SizePositioning_st2_CCW
// Description   : Home�O������ݒ�
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_SizePositioning_st2_CCW()
{
	// Home�O������`�F�b�NCount�B2��ȏ�Ȃ��HomeSNROffFail
	if ( ++gCMPL_TMP_RIGHT.offFailCheckCount > 1 ) {

		CMPL_TMP_InformedRightTamperHomeSensorOffFail(gCMPL_TMP_RIGHT.sheetNo);
		PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st2_CCW, 
							gCMPL_TMP_RIGHT.offFailCheckCount, gCMPL_TMP_RIGHT.offFailCheckCount);
	}	

	// ������(CCW�j�ŋ��d���ݒ�
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_DIR, CMPL_TMP_COMM_INSIDE);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, HIGH);

	//RightSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st2_CCW, CMPL_TMP_SIZE_POS_st2_CCW, 0x00);

	// 20ms Timer�ݒ�
	//CMPL_TMP_RIGHT_SetEventTimer(CMPL_TMP_COMM_MOTOR_ENABLE_TIME_20,
	CMPL_TMP_RIGHT_SetEventTimer(25, //0502
									  CMPL_TMP_RIGHT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : RT_SizePositioning_st3_CCW
// Description   : CCW�����Ƀ��[�^�쓮�J�n
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_SizePositioning_st3_CCW()
{
	DD_STM_SetProfileID(DD_STM_ID_RIGHT_TAMPER, DD_STM_PROF_ID_TAMPER_INITIALIZE);

	//RightSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st3_CCW, CMPL_TMP_SIZE_POS_st3_CCW, 0x00);
	// INITIALIZE Profile�� ��120(StepUp+Const) + 30(StepDown)�p���X�ړ�
    DD_STM_MoveBySetPulse(DD_STM_ID_RIGHT_TAMPER, CMPL_TMP_PROF_TARGET_STEP_NUM_INIT,
      						 CMPL_TMP_SIZE_POSITION_MOVE_PUSE_150, CMPL_TMP_RIGHT_ReceiveSteppingMotorEvent );
}

///////////////////////////////////////////////////////////////////
// Function Name : RT_SizePositioning_st4_CCW
// Description   : Home�O�����슮��
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_SizePositioning_st4_CCW()
{
	// 50ms Timer�ݒ�
	CMPL_TMP_RIGHT_SetEventTimer(CMPL_TMP_COMM_SWITCH_TIME_50,	CMPL_TMP_RIGHT_NORMAL_EVT);
	//RightSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st4_CCW, CMPL_TMP_SIZE_POS_st4_CCW, 0x00);

}

///////////////////////////////////////////////////////////////////
// Function Name : RT_SizePositioning_st2_CW
// Description   : Home����ݒ�
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_SizePositioning_st2_CW()
{
	// �O����(CW�j�ŋ��d���ݒ�
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_DIR, CMPL_TMP_COMM_OUTSIDE);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, HIGH);

	//V3.3.99  Tamper T/B
	if ( gCMPL_TMP_RIGHT.sizePosAfterReturn == true ) {

		// 20ms Timer�ݒ�
		CMPL_TMP_RIGHT_SetEventTimer(CMPL_TMP_COMM_MOTOR_ENABLE_TIME_20,
										  CMPL_TMP_RIGHT_NORMAL_EVT);
	} else {
		//V3.3.93  �����΍�@20ms��200ms�ɕύX
		CMPL_TMP_RIGHT_SetEventTimer(200,	CMPL_TMP_RIGHT_NORMAL_EVT);
	}
	
	//RightSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st2_CW, CMPL_TMP_SIZE_POS_st2_CW, 0x00);
}

///////////////////////////////////////////////////////////////////
// Function Name : RT_SizePositioning_st3_CW
// Description   : CW�����Ƀ��[�^�쓮�J�n
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_SizePositioning_st3_CW()
{
  	// Fail Timer�ݒ�
	CMPL_TMP_RIGHT_SetEventTimer(CMPL_TMP_COMM_FAIL_TIME_500,  CMPL_TMP_RIGHT_FAIL_EVT);
  	//

	gCMPL_TMP_RIGHT.interruptIndex = 	DD_INT_EntryCallback(DD_INT_RIGHT_TAMPER_HOME_SENSOR,
										TMP_RISE, CMPL_TMP_RIGHT_InterruptRTHomeSensorON, 0);  // Intrupt Set
   	DD_STM_SetProfileID(DD_STM_ID_RIGHT_TAMPER, DD_STM_PROF_ID_TAMPER_INITIALIZE);
   	DD_STM_StartClock(DD_STM_ID_RIGHT_TAMPER, CMPL_TMP_PROF_TARGET_STEP_NUM_INIT,
		                 CMPL_TMP_RIGHT_ReceiveSteppingMotorEvent);
	
	//RightSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st3_CW, CMPL_TMP_SIZE_POS_st3_CW, 0x00);
}

///////////////////////////////////////////////////////////////////
// Function Name : RT_SizePositioning_st4_CW
// Description   : StepUp���A�����ݎ�M-> SlowDown-> 50ms Timerset->Home�O������
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_SizePositioning_st4_CW()
{
	// StepDown
	DD_STM_SlowDownStop(DD_STM_ID_RIGHT_TAMPER, CMPL_TMP_RIGHT_ReceiveSteppingMotorEvent );
	//RightSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st4_CW, CMPL_TMP_SIZE_POS_st4_CW, 0x00);

}

///////////////////////////////////////////////////////////////////
// Function Name : RT_SizePositioning_st4_2_CW
// Description   : /StepUp���A�����ݎ�M ���� StepDown����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////

void RT_SizePositioning_st4_2_CW() // StepUp���A�����ݎ�M ���� StepDown����
{
	// 50ms Timer�ݒ�
	CMPL_TMP_RIGHT_SetEventTimer(CMPL_TMP_COMM_SWITCH_TIME_50,	CMPL_TMP_RIGHT_NORMAL_EVT);
	//RightSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st4_2_CW, CMPL_TMP_SIZE_POS_st4_2_CW, 0x00);

}

///////////////////////////////////////////////////////////////////
// Function Name : RT_SizePositioning_st5_CW
// Description   : //StepUp������M
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_SizePositioning_st5_CW()
{
	// Nothing to do... No call
}

///////////////////////////////////////////////////////////////////
// Function Name : RT_SizePositioning_st6_CW
// Description   : �����ԂŊ����ݎ�M
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_SizePositioning_st6_CW()
{
	// StepDown
	DD_STM_SlowDownStop(DD_STM_ID_RIGHT_TAMPER, CMPL_TMP_RIGHT_ReceiveSteppingMotorEvent );
	//RightSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st6_CW, CMPL_TMP_SIZE_POS_st6_CW, 0x00);

}

///////////////////////////////////////////////////////////////////
// Function Name : RT_SizePositioning_st7_CW
// Description   : Home���슮��
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_SizePositioning_st7_CW()
{
	// 50ms Timer�ݒ�
	CMPL_TMP_RIGHT_SetEventTimer(CMPL_TMP_COMM_SWITCH_TIME_50,	CMPL_TMP_RIGHT_NORMAL_EVT);
	//RightSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st7_CW, CMPL_TMP_SIZE_POS_st7_CW, 0x00);

	gCMPL_TMP_RIGHT.offFailCheckCount = 0; //Home�O������`�F�b�NCount Clear(V3.5.95)
}

///////////////////////////////////////////////////////////////////
// Function Name : RT_SizePositioning_st8
// Description   : SizePositioning�J�n
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_SizePositioning_st8()
{
	UC  sheetNo = gCMPL_TMP_RIGHT.sheetNo;
	
	if ( gCMPL_TMP_RIGHT.movePulse == 0 ) { // �@�ړ��ʖ������I��

		DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, LOW);

		gCMPL_TMP_RIGHT.action = CMPL_TMP_st0;
		gCMPL_TMP_RIGHT.sheetNo = SHEETNO_NUM;
		
	 //CR-008
		if ( gCMPL_TMP_RIGHT.sizePosAfterReturn == true ) {
			
			gCMPL_TMP_RIGHT.sizePosAfterReturn = false;
			CMPL_TMP_InformedRightTamperComplete(CMPL_TMP_RIGHT_RETURN,	 sheetNo);

		} else {
		
			CMPL_TMP_InformedRightTamperComplete(CMPL_TMP_SIZE_POS,	 sheetNo);
		}
	
		//RightSizePos
		PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st8, CMPL_TMP_SIZE_POS_st8, 0x00);
		return;
	}

	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_DIR, gCMPL_TMP_RIGHT.direction);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, HIGH);

	//V3.4.0 (Return:20ms,  SizePosi:200ms)
	if ( gCMPL_TMP_RIGHT.sizePosAfterReturn == true ) {
		// 20ms Timer�ݒ�
		CMPL_TMP_RIGHT_SetEventTimer(CMPL_TMP_COMM_MOTOR_ENABLE_TIME_20, CMPL_TMP_RIGHT_NORMAL_EVT);

	} else {	
		//V3.3.93  �����΍�@20ms��200ms�ɕύX
		CMPL_TMP_RIGHT_SetEventTimer(200,	CMPL_TMP_RIGHT_NORMAL_EVT);
	}	
}

///////////////////////////////////////////////////////////////////
// Function Name : RT_SizePositioning_st9
// Description   : ���[�^�쓮�J�n
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_SizePositioning_st9()
{
	// A-Pulse�ړ�
	DD_STM_MoveBySetPulse(DD_STM_ID_RIGHT_TAMPER, CMPL_TMP_PROF_TARGET_STEP_NUM_INIT,
	  						  gCMPL_TMP_RIGHT.movePulse, CMPL_TMP_RIGHT_ReceiveSteppingMotorEvent );
	//RightSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st9, CMPL_TMP_SIZE_POS_st9, 0x00);

}

///////////////////////////////////////////////////////////////////
// Function Name : RT_SizePositioning_st10
// Description   : SizePosition�ֈړ�����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_SizePositioning_st10()
{
	UC    sheetNo = gCMPL_TMP_RIGHT.sheetNo;
	
	// Right Tamper HomeSNR OFF Fail Check (33 Pulse �@�ȏ� & HOME SNR=ON)
	if((gCMPL_TMP_RIGHT.movePulse >= 33) && (DD_DIO_GetInput(DD_DI_RIGHT_TAMPER_HOME_SNR) == HIGH )) {
		CMPL_TMP_InformedRightTamperHomeSensorOffFail(gCMPL_TMP_RIGHT.sheetNo);
		return;
	}

	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, LOW);

	gCMPL_TMP_RIGHT.action = CMPL_TMP_st0;
	gCMPL_TMP_RIGHT.sheetNo = SHEETNO_NUM;

 //CR-008
	if ( gCMPL_TMP_RIGHT.sizePosAfterReturn == true ) {
		
		gCMPL_TMP_RIGHT.sizePosAfterReturn = false;
		CMPL_TMP_InformedRightTamperComplete(CMPL_TMP_RIGHT_RETURN,  sheetNo);
		
		//RightSizePos
		PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st10, 1, 0xFF);

	} else {
	
		CMPL_TMP_InformedRightTamperComplete(CMPL_TMP_SIZE_POS,	 sheetNo);
		
		//RightSizePos
		PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st10, 0, 0x00);
	}

}


// �I�t�Z�b�g����
//CMPL_TMP_OFFSET = 0x0500,
///////////////////////////////////////////////////////////////////
// Function Name : RT_Offset_st1
// Description   : ; //�J�nTiming����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_Offset_st1()
{
	UC  sheetNo=gCMPL_TMP_RIGHT.sheetNo;
	
	// �@�ړ���=0�̏ꍇ�A�I��
	if( gCMPL_TMP_RIGHT.movePulse == 0 ) { //

		gCMPL_TMP_RIGHT.action = CMPL_TMP_st0;
		gCMPL_TMP_RIGHT.sheetNo = SHEETNO_NUM;
	
   		CMPL_TMP_InformedRightTamperComplete(CMPL_TMP_OFFSET,	sheetNo);
		return;
	}

	//Tamper�쓮�����ɂ���Đ����ς���
	if( gCMPL_TMP_RIGHT.direction == CMPL_TMP_COMM_INSIDE ) {  // ���iCCW)����
		// 20msTimerSet
		CMPL_TMP_RIGHT_SetEventTimer( CMPL_TMP_COMM_START_DELAY_20,
										CMPL_TMP_RIGHT_NORMAL_EVT);
	} else {   //�O(CW)����
		gCMPL_TMP_RIGHT.action = CMPL_TMP_OFFSET_st2;
		CMPL_TMP_RIGHT_ControlThread();
	}

}

///////////////////////////////////////////////////////////////////
// Function Name :RT_Offset_st2
// Description   : ; //���[�^�쓮�ݒ�
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_Offset_st2()
{
	//1. ���d���� OFFSET�����ݒ�
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_DIR, gCMPL_TMP_RIGHT.direction);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, HIGH);


	//2. 10msTimerSet
	CMPL_TMP_RIGHT_SetEventTimer( CMPL_TMP_COMM_SWITCH_TIME_10,
					 				CMPL_TMP_RIGHT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Offset_st3
// Description   ; //���[�^�쓮�J�n
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_Offset_st3()
{
	//�ᑬProfile
	DD_STM_SetProfileID(DD_STM_ID_RIGHT_TAMPER, gCMPL_TMP_RIGHT.profile);

	DD_STM_MoveBySetPulse(DD_STM_ID_RIGHT_TAMPER, gCMPL_TMP_RIGHT.targetIndex,
							gCMPL_TMP_RIGHT.movePulse, CMPL_TMP_RIGHT_ReceiveSteppingMotorEvent);

}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Offset_st4
// Description   : ; //���莞�ԃ^�C�}�ݒ�
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_Offset_st4()
{
	//Tamper�쓮�����ɂ���Đ����ς���
	if(gCMPL_TMP_RIGHT.direction == CMPL_TMP_COMM_INSIDE) {  // ���iCCW)����
		// 10msTimerSet
		CMPL_TMP_RIGHT_SetEventTimer( CMPL_TMP_COMM_MOTOR_STOP_TIME_10,
										CMPL_TMP_RIGHT_NORMAL_EVT);
	} else {  //�O(CW)����
		// 20msTimerSet
		CMPL_TMP_RIGHT_SetEventTimer( CMPL_TMP_COMM_MOTOR_STOP_TIME_20,
										CMPL_TMP_RIGHT_NORMAL_EVT);
	}

}

///////////////////////////////////////////////////////////////////
// Function Name :RT_Offset_st5
// Description   : //������҂�
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_Offset_st5()
{
	UC  sheetNo = gCMPL_TMP_RIGHT.sheetNo;
	
	//��d���ݒ�
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, LOW);

	gCMPL_TMP_RIGHT.action = CMPL_TMP_st0;
	gCMPL_TMP_RIGHT.sheetNo = SHEETNO_NUM;

	CMPL_TMP_InformedRightTamperComplete(CMPL_TMP_OFFSET,	 sheetNo);
	PrintTamperLog_Short(CMPL_TMP_OFFSET_st5, 0xFFFF,0xFF);
}

// ���^�[������
//CMPL_TMP_RIGHT_RETURN = 0x0700,
///////////////////////////////////////////////////////////////////
// Function Name : RT_Return_st1
// Description   : �쓮�������f
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_Return_st1()
{
	UC   sheetNo = gCMPL_TMP_RIGHT.sheetNo;
		
	// �@�ړ���=0�̏ꍇ�A�I��
	if( gCMPL_TMP_RIGHT.movePulse == 0 ) { //

		gCMPL_TMP_RIGHT.action = CMPL_TMP_st0;
		gCMPL_TMP_RIGHT.sheetNo = SHEETNO_NUM;
		gCMPL_TMP_RIGHT.sizePosAfterReturn = false;
	
		CMPL_TMP_InformedRightTamperComplete(CMPL_TMP_RIGHT_RETURN, sheetNo);
		return;
	}

	//���[�^�쓮�������f :  ��->st2_CCW,  �O->st2_CW
	if( gCMPL_TMP_RIGHT.direction == CMPL_TMP_COMM_OUTSIDE ) //
		gCMPL_TMP_RIGHT.action = CMPL_TMP_RIGHT_RETURN_st2_CW;
	else  //CMPL_TMP_COMM_INSIDE
		gCMPL_TMP_RIGHT.action = CMPL_TMP_RIGHT_RETURN_st2_CCW;
	CMPL_TMP_RIGHT_ControlThread();
}


///////////////////////////////////////////////////////////////////
// Function Name : RT_Return_st2_CW
// Description   : ; ����J�nTrigger�҂�(LeftOffset)
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_Return_st2_CW()
{
	//Eject ����J�n
	if( CMPL_TMP_IsEjectStart() == true ) {
		gCMPL_TMP_RIGHT.action = CMPL_TMP_RIGHT_RETURN_st3_CW;
		CMPL_TMP_RIGHT_ControlThread();
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Return_st3_CW
// Description   ; //�^�C�}�ݒ�(LeftOffset)
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_Return_st3_CW()
{
#if 1 //CR-008
	gCMPL_TMP_RIGHT.action = CMPL_TMP_RIGHT_RETURN_st4_CW;
	CMPL_TMP_RIGHT_ControlThread();

#else
	// TimerSet
	CMPL_TMP_RIGHT_SetEventTimer( 20+(SRVC_GetNvmValueUS(NVMID_TAMPER_RETURN_OUT_START_TIME)*10),
					 				CMPL_TMP_RIGHT_NORMAL_EVT);
#endif
}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Return_st4_CW
// Description   :;   //���[�^�쓮�ݒ�(LeftOffset)
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_Return_st4_CW()
{
	//1.��d���� �O(CW)�����ݒ�
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_DIR, CMPL_TMP_COMM_OUTSIDE);
	//Motor�E���΍�(CR-008) TimerSet�O�A���d��
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, HIGH); //LOW);

	//2. 20msTimerSet//Motor�E���΍�(CR-008) - 40ms
	CMPL_TMP_RIGHT_SetEventTimer( 40,  //CMPL_TMP_COMM_MOTOR_ENABLE_TIME_20,
					 				CMPL_TMP_RIGHT_NORMAL_EVT);

}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Return_st2_CCW
// Description   ;  //����J�nTrigger�҂�(RightOffset)
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_Return_st2_CCW()
{
	if( CMPL_TMP_IsEjectSheetThrough() == true ) { //Eject �����J�n
		gCMPL_TMP_RIGHT.action = CMPL_TMP_RIGHT_RETURN_st3_CCW;
		CMPL_TMP_RIGHT_ControlThread();
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Return_st3_CCW
// Description  ;  //�^�C�}�ݒ�(RightOffset)
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_Return_st3_CCW()
{
	SS	   nvmSS  = SRVC_GetNvmValueSS(NVMID_TAMPER_RETURN_IN_START_TIME) * 10;  //10ms 
	US	   nvmUS  = 200 + nvmSS ;

	CMPL_TMP_RIGHT_SetEventTimer(nvmUS, 	CMPL_TMP_RIGHT_NORMAL_EVT);

//	CMPL_TMP_RIGHT_SetEventTimer( 200+(SRVC_GetNvmValueUS(NVMID_TAMPER_RETURN_IN_START_TIME)*10),
//					 			CMPL_TMP_RIGHT_NORMAL_EVT);

}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Return_st4_CCW
// Description   :;  //���[�^�쓮�ݒ�(RightOffset)
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_Return_st4_CCW()
{
	//1.��d���� ���iCCW)�����ݒ�
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_DIR, CMPL_TMP_COMM_INSIDE);
	//Motor�E���΍�(CR-008) TimerSet�O�A���d��
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, HIGH);  //LOW);

	//2. 20msTimerSet
	CMPL_TMP_RIGHT_SetEventTimer( CMPL_TMP_COMM_MOTOR_ENABLE_TIME_20,
					 				CMPL_TMP_RIGHT_NORMAL_EVT);

}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Return_st5
// Description   ;  //���[�^�쓮�J�n
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_Return_st5()
{
	//1. ���d���ݒ�
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, HIGH);

	DD_STM_SetProfileID(DD_STM_ID_RIGHT_TAMPER, gCMPL_TMP_RIGHT.profile);

	DD_STM_MoveBySetPulse(DD_STM_ID_RIGHT_TAMPER, gCMPL_TMP_RIGHT.targetIndex,
		gCMPL_TMP_RIGHT.movePulse, CMPL_TMP_RIGHT_ReceiveSteppingMotorEvent);

}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Return_st6
// Description   : ;  //�^�C�}�ݒ�
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_Return_st6()
{
	// 20msTimerSet
	CMPL_TMP_RIGHT_SetEventTimer( CMPL_TMP_COMM_MOTOR_STOP_TIME_20,
					 				CMPL_TMP_RIGHT_NORMAL_EVT);

}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Return_st7
// Description   :  //������҂�
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_Return_st7()
{
	UC   sheetNo = gCMPL_TMP_RIGHT.sheetNo;
	US   nvm_time;
	
	//��d���ݒ�
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, LOW);

#if 1 //CR-008

	if ( gCMPL_TMP_RIGHT.sizePosAfterReturn == true) {  //SizePos�J�n 
		
		// Timer Set ...
		switch ( SRVC_GetStapleMode(sheetNo) ) {

			case  eCdiFinStaple_Off : 
				nvm_time = (US)(SRVC_GetNvmValueUS(NVMID_TAMPER_SIZE_OUT_UNSTAPLE_ST_T_ADJ) * 10);  //(10ms)
				CMPL_TMP_RIGHT_SetEventTimer( nvm_time , 0);  
				break;

			case  eCdiFinStaple_SingleFront :
			default :	
				nvm_time = (US)(SRVC_GetNvmValueUS(NVMID_TAMPER_SIZE_OUT_STAPLE_ST_T_ADJ) * 10);  //(10ms)
				CMPL_TMP_RIGHT_SetEventTimer( nvm_time , 0 );
				break;
		}
		
	} else { 
		
		gCMPL_TMP_RIGHT.action = CMPL_TMP_st0;
		gCMPL_TMP_RIGHT.sheetNo = SHEETNO_NUM;
		
		CMPL_TMP_InformedRightTamperComplete(CMPL_TMP_RIGHT_RETURN, sheetNo);
		PrintTamperLog_Short(CMPL_TMP_RIGHT_RETURN_st7, 0xFFFF, 0xFF);
	}
#else
	gCMPL_TMP_RIGHT.action = CMPL_TMP_st0;
	gCMPL_TMP_RIGHT.sheetNo = SHEETNO_NUM;

	CMPL_TMP_InformedRightTamperComplete(CMPL_TMP_RIGHT_RETURN, sheetNo);
#endif
}


///////////////////////////////////////////////////////////////////
// Function Name : RT_Return_st8
// Description   :  // SizePos�J�n(CR-008) 
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_Return_st8()
{
	// SizePositioning���{���� 
	CMPL_TMP_RIGHT_StartAct(CMPL_TMP_SIZE_POS, gCMPL_TMP_RIGHT.sheetNo);

}



// �I�[�v������(Nigasi)
//CMPL_TMP_OPEN = 0x0700,
///////////////////////////////////////////////////////////////////
// Function Name : RT_Open_st1
// Description   : ���[�^�쓮�ݒ�
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_Open_st1()
{
	UC    sheetNo =gCMPL_TMP_RIGHT.sheetNo;
		
	//����������J�n�`�F�b�N
	if( (DD_DIO_GetInput(DD_DI_RIGHT_TAMPER_HOME_SNR) == HIGH) ||
	 (SRVC_GetCrossProcessSizeMixSet(gCMPL_TMP_RIGHT.sheetNo) != SHT_SIZE_MIX) ) { //Mix Size

	 	gCMPL_TMP_RIGHT.action = CMPL_TMP_st0;
		gCMPL_TMP_RIGHT.sheetNo = SHEETNO_NUM;
		CMPL_TMP_InformedRightTamperComplete(CMPL_TMP_OPEN,   	sheetNo);
	
		return;
	}

	//1. ���d���ŊO(CW)�����ݒ�
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_DIR, gCMPL_TMP_RIGHT.direction);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, HIGH);

	//2. 20msTimerSet
	CMPL_TMP_RIGHT_SetEventTimer( CMPL_TMP_COMM_MOTOR_ENABLE_TIME_20,
					 			CMPL_TMP_RIGHT_NORMAL_EVT);

}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Open_st2
// Description   : ���[�^�쓮�J�n
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_Open_st2()
{
	// 480ms Fail Timer (Right Tamper Home SNR ON Fail)
	CMPL_TMP_RIGHT_SetEventTimer( CMPL_TMP_COMM_SNR_ON_FAIL_TIME,	CMPL_TMP_RIGHT_FAIL_EVT);

	 gCMPL_TMP_RIGHT.interruptIndex =	  DD_INT_EntryCallback(DD_INT_RIGHT_TAMPER_HOME_SENSOR,
										  TMP_RISE, CMPL_TMP_RIGHT_InterruptRTHomeSensorON, 0);	// Intrupt Set

      DD_STM_SetProfileID(DD_STM_ID_RIGHT_TAMPER,  gCMPL_TMP_RIGHT.profile); 
	   
	DD_STM_StartClock(DD_STM_ID_RIGHT_TAMPER,  gCMPL_TMP_RIGHT.profile, NULL);  //no callback

}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Open_st3
// Description   :; �^�C�}�ݒ�
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_Open_st3()
{

	//StepDown 
	DD_STM_SlowDownStop(DD_STM_ID_RIGHT_TAMPER, CMPL_TMP_RIGHT_ReceiveSteppingMotorEvent);

}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Open_st4
// Description   : ; ������҂�
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_Open_st4()
{

	// 20msTimerSet
	CMPL_TMP_RIGHT_SetEventTimer( CMPL_TMP_COMM_MOTOR_STOP_TIME_20,
									CMPL_TMP_RIGHT_NORMAL_EVT);

}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Open_st5
// Description   : ; ������҂�
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void RT_Open_st5()
{
	UC    sheetNo = gCMPL_TMP_RIGHT.sheetNo;
	//��d���ݒ�	
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, LOW);
	
	gCMPL_TMP_RIGHT.action = CMPL_TMP_st0;
	gCMPL_TMP_RIGHT.sheetNo = SHEETNO_NUM;

	CMPL_TMP_InformedRightTamperComplete(CMPL_TMP_OPEN,	 sheetNo);
	PrintTamperLog_Short(CMPL_TMP_OPEN_st5, 0xFFFF, 0xFF);
}


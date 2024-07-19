// File Name:	CMPL_STPL_HEAD_Device.c
// Description:	Compiler stapler Device Controller
// Engineer:	Takahiko Minaga
// Last Edit:	14.09.01
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"
#include "CMPL_STPL_HEAD_Device.h"
#include "CMPL_STPL_Boundary.h"
#include "MSP_EventTimer.h"
#include "MSP_Service.h"
#include "DD_DIO.h"
#include "DD_INT.h"
#include "DD_PWM.h"
#include "ENvmID.h"
#include "NvmService.h"
#include "DD_ResourceAssign.h"

/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	ECMPL_STPL_HEAD_Action action;
	ECMPL_STPL_HEAD_Action compoAction;
	UC sheetNo;

	SS timerIndex;
	SS FailTimerIndex;
	UC interruptIndex;
	UC direction;	
	US stapleCount;
} SCMP_STPL_HEAD_Control;

static SCMP_STPL_HEAD_Control gCMPL_STPL_HEAD_Control;

typedef struct
{
	EFaultStatus stapleFailstatus;
	EFaultStatus stapleNgstatus;
	EFaultStatus stapleReadyFailstatus;

	UC ReadyFailCount;
	UC stapleSupplyLevel;
} SCMPL_STPL_HEAD_Condition;

static SCMPL_STPL_HEAD_Condition gCMPL_STPL_HEAD_Condition;

typedef struct
{
	void (*mpHandler)(void) ;		

}SCMPL_STPL_HEAD_pFunc ;


#define CMPL_STPL_HEAD_COMPO_REVERSE_TIME50		50
#define CMPL_STPL_HEAD_FAIL_WAIT_TIME100			100
#define CMPL_STPL_HEAD_FAIL_MOVE_TIME200			200
#define CMPL_STPL_HEAD_FAIL_TIME700 				700								// staple Head Fail Time
#define CMPL_STPL_HEAD_WAIT_TIME1000				1000

#define CMPL_STPL_HEAD_PWM_MASKTIME48				48
#define CMPL_STPL_HEAD_PWM_MASKTIME140				140
#define CMPL_STPL_HEAD_PWM_MASKTIME168				168
#define CMPL_STPL_HEAD_PWM_MASKDEFAULT				300


#define CMPL_STPL_HEAD_NEAR_LOW_STAPLE_CNT_MAX	5100

#define CMPL_STPL_HEAD_NORMAL_EVT		0
#define CMPL_STPL_HEAD_INTERRUPT_EVT	1
#define CMPL_STPL_HEAD_NORMAL_EVTPWM	2

// #define CMPL_STPL_HEAD_HARIOK_STAPLE	0
// #define CMPL_STPL_HEAD_NEAR_LOWSTAPLE	1
// #define CMPL_STPL_HEAD_LOWSTAPLE	2

#define CMPL_STPL_HEAD_COMPO_st	0

#define CMPL_STPL_HEAD_MOVE_LOWSPEED	1
#define CMPL_STPL_HEAD_MOVE_HIGHSPEED	0


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
static void CMPL_STPL_HEAD_StaplerMotorStop();
static void CMPL_STPL_HEAD_StaplerMotorBrake();
static void CMPL_STPL_HEAD_StaplerMotorStaple();
static void CMPL_STPL_HEAD_StaplerMotorStaplePwm();

static void CMPL_STPL_HEAD_StaplerMotorReverse();

static void CMPL_STPL_HEAD_SetEventTimer(US aTime, UL aEventType);
static void CMPL_STPL_HEAD_CancelEventTimer();
static void CMPL_STPL_HEAD_SetInterrupt();
static void CMPL_STPL_HEAD_CancelInterrupt();
static UC CMPL_STPL_HEAD_GetStapleHomeSensor();
static UC CMPL_STPL_HEAD_GetSelfPrimingSensor();
static UC CMPL_STPL_HEAD_GetLowstapleSensor();
static void CMPL_STPL_HEAD_StapleFailOccur();

static void CMPL_STPL_HEAD_ReceiveEventTimerTimeUp(SS aTimerID, UL aNotUsed);
static void CMPL_STPL_HEAD_InterruptStapleHomeSensorON();
static void CMPL_STPL_HEAD_ControlThread();

static void CMPL_STPL_HEAD_Home_st1();
static void CMPL_STPL_HEAD_Home_st2();
static void CMPL_STPL_HEAD_Home_st3();
static void CMPL_STPL_HEAD_Home_st10();
static void CMPL_STPL_HEAD_Home_st11();
static void CMPL_STPL_HEAD_Home_st12();
static void CMPL_STPL_HEAD_Home_st13();
static void CMPL_STPL_HEAD_Home_st14();

static void CMPL_STPL_HEAD_StapleStart();
static void CMPL_STPL_HEAD_Staple_st1();
static void CMPL_STPL_HEAD_Staple_st2();
static void CMPL_STPL_HEAD_Staple_st3();
static void CMPL_STPL_HEAD_Staple_st4();
static void CMPL_STPL_HEAD_Staple_st10();
static void CMPL_STPL_HEAD_Staple_st11();
static void CMPL_STPL_HEAD_Staple_st12();
static void CMPL_STPL_HEAD_Staple_st13();
static void CMPL_STPL_HEAD_Staple_st14();

static void CMPL_STPL_HEAD_Hariout_st1();
static void CMPL_STPL_HEAD_Hariout_st2();
static void CMPL_STPL_HEAD_Hariout_st3();
static void CMPL_STPL_HEAD_Hariout_st4();
static void CMPL_STPL_HEAD_Hariout_st10();
static void CMPL_STPL_HEAD_Hariout_st11();
static void CMPL_STPL_HEAD_Hariout_st12();
static void CMPL_STPL_HEAD_Hariout_st13();
static void CMPL_STPL_HEAD_Hariout_st14();

static void CMPL_STPL_HEAD_Compo_st1();
static void CMPL_STPL_HEAD_Compo_st2();
static void CMPL_STPL_HEAD_Compo_st3();
static void CMPL_STPL_HEAD_Compo_st4();
static void CMPL_STPL_HEAD_SetMotorOn(UC  aCurrent);


static const SCMPL_STPL_HEAD_pFunc ControlThread_STAPLE_COMPO[ ] = {
	{NULL							},
	{CMPL_STPL_HEAD_Compo_st1	}, //
	{CMPL_STPL_HEAD_Compo_st2	}, //
	{CMPL_STPL_HEAD_Compo_st3	}, //
    {CMPL_STPL_HEAD_Compo_st4	}, //
};

static const SCMPL_STPL_HEAD_pFunc ControlThread_STAPLE_HOME[ ] = {
	{NULL							},
	{CMPL_STPL_HEAD_Home_st1	}, // Staple���[�^�쓮
	{CMPL_STPL_HEAD_Home_st2	}, // StapleHomeSnr�҂�
	{CMPL_STPL_HEAD_Home_st3	}, /// StapleHomeSnrOn���
    {CMPL_STPL_HEAD_Home_st10	}, // HomeSnrOnFail��������
	{CMPL_STPL_HEAD_Home_st11	}, // StapleMotorReverse�����쓮
	{CMPL_STPL_HEAD_Home_st12	}, // StapleHomeSnr�҂�
	{CMPL_STPL_HEAD_Home_st13	}, // HomeSnrOn����(StapleNG����)
	{CMPL_STPL_HEAD_Home_st14	}, // Fail����   
};
static const SCMPL_STPL_HEAD_pFunc ControlThread_STAPLE[ ] = {
	{NULL							},
	{CMPL_STPL_HEAD_Staple_st1	}, // Staple���[�^�쓮
	{CMPL_STPL_HEAD_Staple_st2	}, // StapleHomeSnr�҂�
	{CMPL_STPL_HEAD_Staple_st3	}, // StapleHomeSnrOn���(���슮��)
    {CMPL_STPL_HEAD_Staple_st10	}, // HomeSnrOnFail��������
	{CMPL_STPL_HEAD_Staple_st11	}, // StapleMotorReverse�����쓮
	{CMPL_STPL_HEAD_Staple_st12	}, // StapleHomeSnr�҂�
	{CMPL_STPL_HEAD_Staple_st13	}, // HomeSnrOn�҂�(StapleNG�ʒm)
	{CMPL_STPL_HEAD_Staple_st14	}, // �@Fail����    
};
static const SCMPL_STPL_HEAD_pFunc ControlThread_STAPLE_HARIOUT[ ] = {
	{NULL							},
	{CMPL_STPL_HEAD_Hariout_st1	}, // Staple���[�^�쓮
	{CMPL_STPL_HEAD_Hariout_st2	}, // StapleHomeSnr�҂�
	{CMPL_STPL_HEAD_Hariout_st3	}, // StapleHomeSnrOn���
    {CMPL_STPL_HEAD_Hariout_st4	}, // Self Priming Sensor����
	{CMPL_STPL_HEAD_Hariout_st10}, // HomeSnrOnFail��������
	{CMPL_STPL_HEAD_Hariout_st11}, // StapleMotorReverse�����쓮
	{CMPL_STPL_HEAD_Hariout_st12}, // StapleHomeSnr�҂�
	{CMPL_STPL_HEAD_Hariout_st13}, // HomeSnrOn���   
	{CMPL_STPL_HEAD_Hariout_st14}, // Fail����	
};

/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_startAct
// Description   : �X�e�[�v������ݒ�_����J�n
// Parameter     : aAction (���䂷�铮��)
//				 : aSheetNo (���䂷��SheetNo)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_StartAct(ECMPL_STPL_HEAD_Action aAction, UC aSheetNo)
{
	switch ( aAction ) {		// ECMPL_STPL_HEAD_Action

	// Component ���� //////////////////////////////////////////////////////////////////////////////
	// 
	case CMPL_STPL_HEAD_COMPO_FORWARD :
		gCMPL_STPL_HEAD_Control.direction = STPL_FORWARD;
		gCMPL_STPL_HEAD_Control.compoAction = aAction;
		gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_COMPO_st1;
		break;

	// 
	case CMPL_STPL_HEAD_COMPO_REVERSE :
		gCMPL_STPL_HEAD_Control.direction = STPL_REVERSE;
		gCMPL_STPL_HEAD_Control.compoAction = aAction;
		gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_COMPO_st1;
		break;
	case CMPL_STPL_HEAD_HARIOUT_st1 :
		gCMPL_STPL_HEAD_Condition.ReadyFailCount = 0;
		gCMPL_STPL_HEAD_Control.stapleCount = SRVC_GetNvmValueUS(NVMID_STAPLE_COUNT_DETECT_NEAR_LOW_STAPLE);
	default :
		gCMPL_STPL_HEAD_Control.action = aAction;
		gCMPL_STPL_HEAD_Control.sheetNo = aSheetNo;
		gCMPL_STPL_HEAD_Control.timerIndex = MSP_MAX_TMR_NUM;
		gCMPL_STPL_HEAD_Control.FailTimerIndex = MSP_MAX_TMR_NUM;
		gCMPL_STPL_HEAD_Control.interruptIndex = DD_INT_NUM;
		break;
	}

	CMPL_STPL_HEAD_ControlThread();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_ControlThread
// Description   : �X�e�[�v������
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_ControlThread()
{

	ECMPL_STPL_HEAD_Action step = gCMPL_STPL_HEAD_Control.action & CMPL_STPL_HEAD_ACTION_MASK;
	US 				 status = gCMPL_STPL_HEAD_Control.action & 0x00FF;


	switch (step) {
	case CMPL_STPL_HEAD_COMPO : 
		ControlThread_STAPLE_COMPO[status].mpHandler(); break;
	
	case  CMPL_STPL_HEAD_HOME : 
		ControlThread_STAPLE_HOME[status].mpHandler();	 break;
		
	case  CMPL_STPL_HEAD_STAPLE : 
		ControlThread_STAPLE[status].mpHandler(); break;

	case  CMPL_STPL_HEAD_HARIOUT : 
		ControlThread_STAPLE_HARIOUT[status].mpHandler();	break;
			
	default : 	break;
	}

}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_ReceiveEventTimerTimeUp
// Description   : �C�x���g�^�C�}�̃^�C���A�b�v
// Parameter     : timerID (Timer ID)
//               : notUsed (Not Used)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_ReceiveEventTimerTimeUp(SS aTimerID, UL aEventType)
{
	gCMPL_STPL_HEAD_Control.timerIndex = MSP_MAX_TMR_NUM;
	gCMPL_STPL_HEAD_Control.FailTimerIndex = MSP_MAX_TMR_NUM;

	switch( gCMPL_STPL_HEAD_Control.action ) {

	case CMPL_STPL_HEAD_COMPO_st1:
	case CMPL_STPL_HEAD_COMPO_st2:
	case CMPL_STPL_HEAD_COMPO_st3:
		gCMPL_STPL_HEAD_Control.action++;
		CMPL_STPL_HEAD_ControlThread();
		break;

	// �X�e�[�v���w�b�h�z�[������	
	case CMPL_STPL_HEAD_HOME_st2:	
		if ( aEventType == CMPL_STPL_HEAD_NORMAL_EVTPWM ) {
			CMPL_STPL_HEAD_StaplerMotorStaplePwm();
		} else {
			gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_HOME_st10;
			CMPL_STPL_HEAD_CancelInterrupt();
			CMPL_STPL_HEAD_ControlThread();
		}
		break;

	case CMPL_STPL_HEAD_HOME_st3:
	case CMPL_STPL_HEAD_HOME_st13:
		if( aEventType == CMPL_STPL_HEAD_INTERRUPT_EVT ) {
			CMPL_STPL_HEAD_ControlThread();
		}
		break;

	case CMPL_STPL_HEAD_HOME_st10:
		gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_HOME_st11;
		CMPL_STPL_HEAD_ControlThread();
		break;

	case CMPL_STPL_HEAD_HOME_st12:
		gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_HOME_st14;			
		CMPL_STPL_HEAD_ControlThread();	
		break;

	// �X�e�[�v������
	case CMPL_STPL_HEAD_STAPLE_st1:
		gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_STAPLE_st10;		
		CMPL_STPL_HEAD_CancelInterrupt();
		CMPL_STPL_HEAD_ControlThread();		
		break;
	case CMPL_STPL_HEAD_STAPLE_st2: 	
		if ( aEventType == CMPL_STPL_HEAD_NORMAL_EVTPWM ) {
			CMPL_STPL_HEAD_StaplerMotorStaplePwm();
		} else {
			gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_STAPLE_st10;
			CMPL_STPL_HEAD_CancelInterrupt();
			CMPL_STPL_HEAD_ControlThread();
		}
		break;

	case CMPL_STPL_HEAD_STAPLE_st3:
	case CMPL_STPL_HEAD_STAPLE_st13:
		if( aEventType == CMPL_STPL_HEAD_INTERRUPT_EVT ) {
			CMPL_STPL_HEAD_ControlThread();
		}
		break;

	case CMPL_STPL_HEAD_STAPLE_st10:
		gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_STAPLE_st11;
		CMPL_STPL_HEAD_ControlThread();
		break;

	case CMPL_STPL_HEAD_STAPLE_st12:
		gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_STAPLE_st14;		
		CMPL_STPL_HEAD_ControlThread();			
		break;

	// staple�j�o������
	case CMPL_STPL_HEAD_HARIOUT_st1:
	case CMPL_STPL_HEAD_HARIOUT_st2:// V3.4.0 Fail Test NG TB(DC Motor ConnecterNo)		
		if( aEventType == CMPL_STPL_HEAD_INTERRUPT_EVT ) {
			CMPL_STPL_HEAD_ControlThread();
		} else {
			gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_HARIOUT_st10;		
			CMPL_STPL_HEAD_CancelInterrupt();
			CMPL_STPL_HEAD_ControlThread(); 	
		}
		break;
		
	case CMPL_STPL_HEAD_HARIOUT_st3:
		if( aEventType == CMPL_STPL_HEAD_INTERRUPT_EVT ) {
			CMPL_STPL_HEAD_ControlThread();
		} else {
			gCMPL_STPL_HEAD_Control.action++;		
			CMPL_STPL_HEAD_ControlThread(); 	
		}
		break;

	case CMPL_STPL_HEAD_HARIOUT_st10:		
		gCMPL_STPL_HEAD_Control.action++;		
		CMPL_STPL_HEAD_ControlThread(); 	
		break;
				
	case CMPL_STPL_HEAD_HARIOUT_st12:
		gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_HARIOUT_st14;		
		CMPL_STPL_HEAD_ControlThread();			
		break;
		
	case CMPL_STPL_HEAD_HARIOUT_st13:
		if( aEventType == CMPL_STPL_HEAD_INTERRUPT_EVT ) {
			CMPL_STPL_HEAD_ControlThread();
		}
		break;

	default:
		break;
	}


}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_InterruptStapleHomeSensorON
// Description   : staple Home Sensor ON ���荞��
// Parameter     : 
//               : 
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_InterruptStapleHomeSensorON()
{
	gCMPL_STPL_HEAD_Control.interruptIndex = DD_INT_NUM;
	// CMPL_STPL_HEAD_CancelEventTimer();	// Fail Timer cancel ..
	DD_INT_CancelCallback(DD_INT_STAPLE_HOME_SENSOR, STPL_RISE, 
	                          gCMPL_STPL_HEAD_Control.interruptIndex);

	switch ( gCMPL_STPL_HEAD_Control.action ) {

	case CMPL_STPL_HEAD_COMPO_st1 :
		switch ( gCMPL_STPL_HEAD_Control.compoAction ) {
		case CMPL_STPL_HEAD_COMPO_FORWARD :
			gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_COMPO_st4;
			CMPL_STPL_HEAD_ControlThread();
			break;
		case CMPL_STPL_HEAD_COMPO_REVERSE :
			if ( gCMPL_STPL_HEAD_Control.timerIndex != MSP_MAX_TMR_NUM ) {
				MSP_CancelEventTimer( gCMPL_STPL_HEAD_Control.timerIndex);
	 			gCMPL_STPL_HEAD_Control.timerIndex = MSP_MAX_TMR_NUM;
			}
			gCMPL_STPL_HEAD_Control.action++;
			CMPL_STPL_HEAD_ControlThread();
			break;
		default :
			break;
		}

	case CMPL_STPL_HEAD_COMPO_st2 :
		switch ( gCMPL_STPL_HEAD_Control.compoAction ) {
		case CMPL_STPL_HEAD_COMPO_FORWARD :
			gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_COMPO_st4;
			CMPL_STPL_HEAD_ControlThread();
			break;
		default : 
			break;
		}

	// �X�e�[�v���w�b�h�z�[������
	case CMPL_STPL_HEAD_HOME_st2:
	case CMPL_STPL_HEAD_HOME_st12:

	// �X�e�[�v������
	case CMPL_STPL_HEAD_STAPLE_st2:
	case CMPL_STPL_HEAD_STAPLE_st12:

	// staple�j�o������
	case CMPL_STPL_HEAD_HARIOUT_st2:
	case CMPL_STPL_HEAD_HARIOUT_st12:

//		if( gCMPL_STPL_HEAD_Control.timerIndex != MSP_MAX_TMR_NUM ) {				// �C�x���g�^�C�}�̏������n�܂��Ă���Ƃ��́A���荞�ݏ��������Ȃ�
			gCMPL_STPL_HEAD_Control.action++;
			CMPL_STPL_HEAD_StaplerMotorStop();
			CMPL_STPL_HEAD_CancelEventTimer();			
			CMPL_STPL_HEAD_SetEventTimer(0, CMPL_STPL_HEAD_INTERRUPT_EVT);
//		}
		break;

	default:
		break;
	}

}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_StaplerMotorStop
// Description   : �X�e�[�v�����[�^ ��~
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_StaplerMotorStop()
{
	// CMPL_STPL_HEAD_SetMotorOn(REVERSE);
	DD_DIO_SetOutput(DD_DO_STAPLER_MOT_IN1, OFF); // 0 - 0 : Stop
	DD_PWM_Stop(DD_PWM_ID_STAPLER_MOT);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_StaplerMotorStaple
// Description   : �X�e�[�v�����[�^ �X�e�[�v�������쓮
// Parameter     : �]
// Return        : �]
//
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_StaplerMotorStaplePwm()
{
	UC StapleCount, DutyUc;
	UL DutyNvm2_5, DutyNvm6_20, DutyNvm21_40; 
	UL Duty, DutyS;
	
	StapleCount =  SRVC_GetCompileCount(gCMPL_STPL_HEAD_Control.sheetNo);

	DutyNvm2_5 = SRVC_GetNvmValue(NVMID_STAPLER_MOT_PWM_LOW_SETTING1_1);
	DutyNvm6_20 = SRVC_GetNvmValue(NVMID_STAPLER_MOT_PWM_LOW_SETTING6_20);
	DutyNvm21_40 = SRVC_GetNvmValue(NVMID_STAPLER_MOT_PWM_LOW_SETTING21_40);

	if (SRVC_GetNvmValue(NVMID_STAPLE_SPEED_CHANGE) == CMPL_STPL_HEAD_MOVE_LOWSPEED ) { //
		if ( 2 <= StapleCount && StapleCount <= 5 ) {
			Duty =  933 - (UL)DutyNvm2_5 ;
			Duty =   (UL)(Duty * 100 / 933) ;
		} else if ( 6 <= StapleCount && StapleCount <= 20 ) {
			Duty =  (UL)( 933 - (UL)DutyNvm6_20 ) *  (UL)100  ;
			Duty =  (UL)(Duty / 933)  ;

		} else if ( 21 <= StapleCount && StapleCount <= 40 ) {
			DutyS =  (UL)(933 - (UL)DutyNvm21_40 ) * (UL)100 ;
			Duty =(UL)( DutyS / 933 );
			DutyUc = Duty ;
			
		} else{
			Duty = 100 ;
		}
		
	} else {
		Duty = 100 ;
	}

	DD_PWM_SetFrequency(DD_PWM_ID_STAPLER_MOT, 30000, (UC)Duty);
	DD_PWM_Start(DD_PWM_ID_STAPLER_MOT);
}
///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_StaplerMotorStaple
// Description   : �X�e�[�v�����[�^ �X�e�[�v�������쓮
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_StaplerMotorStaple()
{
	UC StapleCount;
	US MaskTime;
	
	CMPL_STPL_HEAD_SetMotorOn(STPL_FORWARD);	
	DD_PWM_SetFrequency(DD_PWM_ID_STAPLER_MOT, 30000, DUTY_100);
	DD_PWM_Start(DD_PWM_ID_STAPLER_MOT);
	
	if ( gCMPL_STPL_HEAD_Control.action != CMPL_STPL_HEAD_STAPLE_st1 ) 
		return;

	StapleCount = SRVC_GetCompileCount(gCMPL_STPL_HEAD_Control.sheetNo);

	if ( SRVC_GetNvmValue(NVMID_STAPLE_SPEED_CHANGE)  == CMPL_STPL_HEAD_MOVE_LOWSPEED ){
		if ( 2 <= StapleCount && StapleCount <= 5 ) {
			MaskTime = CMPL_STPL_HEAD_PWM_MASKTIME48;
			CMPL_STPL_HEAD_SetEventTimer(MaskTime, CMPL_STPL_HEAD_NORMAL_EVTPWM);		
		} else if ( 6 <= StapleCount && StapleCount <= 20 ) {
			MaskTime = CMPL_STPL_HEAD_PWM_MASKTIME140;
			CMPL_STPL_HEAD_SetEventTimer(MaskTime, CMPL_STPL_HEAD_NORMAL_EVTPWM);		
		} else if ( 21 <= StapleCount && StapleCount <= 40 ) {
			MaskTime = CMPL_STPL_HEAD_PWM_MASKTIME168;
			CMPL_STPL_HEAD_SetEventTimer(MaskTime, CMPL_STPL_HEAD_NORMAL_EVTPWM);		
		} else{
			MaskTime = CMPL_STPL_HEAD_PWM_MASKDEFAULT;
			CMPL_STPL_HEAD_SetEventTimer(MaskTime, CMPL_STPL_HEAD_NORMAL_EVTPWM);
		}

	} else {
	
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_StaplerMotorReverse
// Description   : �X�e�[�v�����[�^ ���o�[�X�����쓮
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_StaplerMotorReverse()
{   // CCW : LOW - HIGH
	CMPL_STPL_HEAD_SetMotorOn(STPL_REVERSE);
	DD_PWM_SetFrequency(DD_PWM_ID_STAPLER_MOT, 30000, DUTY_70);
	DD_PWM_Start(DD_PWM_ID_STAPLER_MOT);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_SetEventTimer
// Description   : �C�x���g�^�C�}�[�Z�b�g
// Parameter     : aTime (�^�C�}�[�l (�~1[ms]))
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_SetEventTimer(US aTime, UL aEventType)
{
	gCMPL_STPL_HEAD_Control.timerIndex = MSP_SetEventTimer(aTime, CMPL_STPL_HEAD_ReceiveEventTimerTimeUp, aEventType);	// Timer Set
}
///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_SetEventTimer
// Description   : �C�x���g�^�C�}�[�Z�b�g
// Parameter     : aTime (�^�C�}�[�l (�~1[ms]))
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_SetEventFailTimer(US aTime, UL aEventType)
{
	gCMPL_STPL_HEAD_Control.FailTimerIndex = MSP_SetEventTimer(aTime, CMPL_STPL_HEAD_ReceiveEventTimerTimeUp, aEventType);	// Timer Set
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_CancelEventTimer
// Description   : �C�x���g�^�C�}�[�L�����Z��
// Parameter     : aTime (�^�C�}�[�l (�~1[ms]))
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_CancelEventTimer()
{
	if(gCMPL_STPL_HEAD_Control.timerIndex != MSP_MAX_TMR_NUM ) {
		MSP_CancelEventTimer(gCMPL_STPL_HEAD_Control.timerIndex);
		gCMPL_STPL_HEAD_Control.timerIndex = MSP_MAX_TMR_NUM;
	}
	if(gCMPL_STPL_HEAD_Control.FailTimerIndex != MSP_MAX_TMR_NUM ) {
		MSP_CancelEventTimer(gCMPL_STPL_HEAD_Control.FailTimerIndex);
		gCMPL_STPL_HEAD_Control.FailTimerIndex = MSP_MAX_TMR_NUM;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_SetInterrupt
// Description   : ���荞�݃Z�b�g
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_SetInterrupt()
{
	gCMPL_STPL_HEAD_Control.interruptIndex = DD_INT_EntryCallback(DD_INT_STAPLE_HOME_SENSOR, RISE, CMPL_STPL_HEAD_InterruptStapleHomeSensorON, 0);	// Intrupt Set
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_CancelInterrupt
// Description   : ���荞�݃N���A
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_CancelInterrupt()
{
	if( gCMPL_STPL_HEAD_Control.interruptIndex != DD_INT_NUM ) {
		DD_INT_CancelCallback(DD_INT_STAPLE_HOME_SENSOR, STPL_RISE, gCMPL_STPL_HEAD_Control.interruptIndex);	// ���荞�݃L�����Z��
		DD_INT_CancelCallback(DD_INT_STAPLE_HOME_SENSOR, STPL_FALL, gCMPL_STPL_HEAD_Control.interruptIndex);	// ���荞�݃L�����Z��
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_GetStapleHomeSensor
// Description   : �X�e�[�v���z�[���Z���T�[�̏��(Filter����)���擾
// Parameter     : �]
// Return        : �Z���T�[���x��(ON/OFF)
///////////////////////////////////////////////////////////////////
UC CMPL_STPL_HEAD_GetStapleHomeSensor()
{
	if( DD_DIO_GetInput(DD_DI_STAPLE_HOME_SNR) == HIGH ) {
		return ON;
	}
	else {
		return OFF;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_GetSelfPrimingSensor
// Description   : �Z���t�v���C�~���O�Z���T�[�̏��(Filter����)���擾
// Parameter     : �]
// Return        : �Z���T�[���x��(ON/OFF)
///////////////////////////////////////////////////////////////////
UC CMPL_STPL_HEAD_GetSelfPrimingSensor()
{
	if( DD_DIO_GetInput(DD_DI_SELF_PRIMING_SNR) == HIGH ) {
		return ON;
	}
	else {
		return OFF;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_GetLowstapleSensor
// Description   : �Z���t�v���C�~���O�Z���T�[�̏��(Filter����)���擾
// Parameter     : �]
// Return        : �Z���T�[���x��(ON/OFF)
///////////////////////////////////////////////////////////////////
UC CMPL_STPL_HEAD_GetLowstapleSensor()
{
	if( DD_DIO_GetInput(DD_DI_LOW_STAPLE_SNR) == HIGH ) {
		return ON;
	}
	else {
		return OFF;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_IsExecutiongDeviceAction
// Description   : �X�e�[�v���w�b�h�̓����Ԃ��擾
// Parameter     : �]
// Return        : true�F���쒆�Afalse�F��~��
///////////////////////////////////////////////////////////////////
bool CMPL_STPL_HEAD_IsExecutiongDeviceAction()
{
	if( gCMPL_STPL_HEAD_Control.action == CMPL_STPL_HEAD_ACT_IDLE ) {
		return false;
	}
	else {
		return true;
	}
}

bool CMPL_STPL_HEAD_IsOccurFault()
{
	if( gCMPL_STPL_HEAD_Condition.stapleFailstatus == FAULT_OCCUR ) {
		return true;
	}
	else {
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_Reset
// Description   : �X�e�[�v�����Z�b�g
// Parameter     : �]
// Return        : �]
// 20160225 V3.4.0 Merge [ Device Status Value�w�b�_�t�@�C���쐬 ]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Reset()
{
	gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_ACT_IDLE;
	gCMPL_STPL_HEAD_Control.sheetNo = SHEETNO_NUM;
	gCMPL_STPL_HEAD_Control.timerIndex = MSP_MAX_TMR_NUM;
	gCMPL_STPL_HEAD_Control.FailTimerIndex = MSP_MAX_TMR_NUM;
	gCMPL_STPL_HEAD_Control.interruptIndex = DD_INT_NUM;

	gCMPL_STPL_HEAD_Condition.stapleFailstatus = FAULT_NOT_OCCUR;
	gCMPL_STPL_HEAD_Condition.stapleNgstatus = FAULT_NOT_OCCUR;
	gCMPL_STPL_HEAD_Condition.stapleReadyFailstatus = FAULT_NOT_OCCUR;
	gCMPL_STPL_HEAD_Condition.stapleSupplyLevel = DVST_STPL_SUPPLY_LEVEL_STTS_HARIOK;
	
	CMPL_STPL_HEAD_StaplerMotorStop();

}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_DeviceStop
// Description   : �X�^�[�v����~
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_DeviceStop()
{
	ECMPL_STPL_HEAD_Action aAction = gCMPL_STPL_HEAD_Control.action & CMPL_STPL_HEAD_ACTION_MASK;
	UC aSheetNo = gCMPL_STPL_HEAD_Control.sheetNo;
	
	gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_ACT_IDLE;
	gCMPL_STPL_HEAD_Control.sheetNo = SHEETNO_NUM;

	CMPL_STPL_HEAD_CancelEventTimer();
	CMPL_STPL_HEAD_CancelInterrupt();

	CMPL_STPL_HEAD_StaplerMotorStop();

	CMPL_STPL_InformedStapleHeadComp(aAction, aSheetNo);
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_StopSheet
// Description   : �w��V�[�g�̓����~
// Parameter     : aSheetNo (�w��V�[�g�ԍ�)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_StopSheet(UC aSheetNo)
{
	if( aSheetNo == SHEETNO_NUM) {												// ���䒆�̗p���̃R���f�B�V�������`�F�b�N
		if( gCMPL_STPL_HEAD_Control.sheetNo != SHEETNO_NUM ) {
			if( SRVC_CheckSheetConditionNormal(gCMPL_STPL_HEAD_Control.sheetNo) == false ) {
				CMPL_STPL_HEAD_DeviceStop();
			}
			else {																// = Condition Normal
				/* No Action */
			}
		}
		else {																	// �V�[�g�ɑ΂��鐧������Ă��Ȃ�
			/* No Action */
		}
	}
	else {																		// �w��V�[�g�̐�����~����
		if( aSheetNo == gCMPL_STPL_HEAD_Control.sheetNo ) {
			CMPL_STPL_HEAD_DeviceStop();
		}
		else {																	// �w��V�[�g�Ɛ���V�[�g���قȂ�
			/* No Action */
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_NearLowstapleCountIncrement
// Description   : NearLowstaple�J�E���g���C���N�������g����
// Parameter     : �]
// Return        : �]
// 20160225 V3.4.0 Merge [ Device Status Value�w�b�_�t�@�C���쐬 ]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_NearLowstapleCountIncrement()
{
	if(SRVC_GetNvmValueUS(NVMID_STAPLE_COUNT_DETECT_NEAR_LOW_STAPLE) >= CMPL_STPL_HEAD_NEAR_LOW_STAPLE_CNT_MAX ) {
		gCMPL_STPL_HEAD_Control.stapleCount = 0;
		SRVC_SetNvmValue(gCMPL_STPL_HEAD_Control.stapleCount, NVMID_STAPLE_COUNT_DETECT_NEAR_LOW_STAPLE) ;
		if( gCMPL_STPL_HEAD_Condition.stapleSupplyLevel == DVST_STPL_SUPPLY_LEVEL_STTS_NEAR_LOW ) {
			gCMPL_STPL_HEAD_Condition.stapleSupplyLevel = DVST_STPL_SUPPLY_LEVEL_STTS_HARIOK;
			CMPL_STPL_InformedStapleSupplyLevel(DVST_STPL_SUPPLY_LEVEL_STTS_HARIOK);
		}
	}
	else {
		SRVC_SetNvmValue(gCMPL_STPL_HEAD_Control.stapleCount++, NVMID_STAPLE_COUNT_DETECT_NEAR_LOW_STAPLE) ;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_CheckNearLowstapleOuccur
// Description   : NearLowstaple���m���`�F�b�N����
// Parameter     : �]
// Return        : �]
// 20160225 V3.4.0 Merge [ Device Status Value�w�b�_�t�@�C���쐬 ]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_CheckNearLowstapleOuccur()
{
	if( gCMPL_STPL_HEAD_Condition.stapleSupplyLevel != DVST_STPL_SUPPLY_LEVEL_STTS_NEAR_LOW ) {
		if(	(gCMPL_STPL_HEAD_Condition.stapleSupplyLevel != DVST_STPL_SUPPLY_LEVEL_STTS_LOW ) &&
			(gCMPL_STPL_HEAD_Control.stapleCount >= SRVC_GetNvmValue(NVMID_DETECT_NEAR_LOW_STAPLE_THRESHOLD)) ) {
			gCMPL_STPL_HEAD_Condition.stapleSupplyLevel = DVST_STPL_SUPPLY_LEVEL_STTS_NEAR_LOW;
			CMPL_STPL_InformedStapleSupplyLevel(DVST_STPL_SUPPLY_LEVEL_STTS_NEAR_LOW);
		}
		else {
			/* No Action */
		}
	}
	else {
		/* No Action */
	}
}
///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_CheckLowstapleClear
// Description   : Lowstaple�N���A���`�F�b�N����
// Parameter     : �]
// Return        : �]
// 20160225 V3.4.0 Merge [ Device Status Value�w�b�_�t�@�C���쐬 ]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_CheckNearLowstapleClear()
{
	if( gCMPL_STPL_HEAD_Condition.stapleSupplyLevel == DVST_STPL_SUPPLY_LEVEL_STTS_NEAR_LOW ) {
			gCMPL_STPL_HEAD_Condition.stapleSupplyLevel = DVST_STPL_SUPPLY_LEVEL_STTS_HARIOK;
			CMPL_STPL_InformedStapleSupplyLevel(DVST_STPL_SUPPLY_LEVEL_STTS_HARIOK);
	}
	else {
		/* No Action */
	}

}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_CheckLowstapleOccur
// Description   : Lowstaple���m���`�F�b�N����
// Parameter     : �]
// Return        : �]
// 20160225 V3.4.0 Merge [ Device Status Value�w�b�_�t�@�C���쐬 ]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_CheckLowstapleOccur()
{
	if( gCMPL_STPL_HEAD_Condition.stapleSupplyLevel != DVST_STPL_SUPPLY_LEVEL_STTS_LOW ) {
		if( CMPL_STPL_HEAD_GetLowstapleSensor() == ON ) {
			CMPL_STPL_HEAD_CheckNearLowstapleClear();
			gCMPL_STPL_HEAD_Condition.stapleSupplyLevel = DVST_STPL_SUPPLY_LEVEL_STTS_LOW;
			CMPL_STPL_InformedStapleSupplyLevel(DVST_STPL_SUPPLY_LEVEL_STTS_LOW);
		}
		else {
			/* No Action */
		}
	}
	else {
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_CheckLowstapleClear
// Description   : Lowstaple�N���A���`�F�b�N����
// Parameter     : �]
// Return        : �]
// 20160225 V3.4.0 Merge [ Device Status Value�w�b�_�t�@�C���쐬 ]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_CheckLowstapleClear()
{
	if( gCMPL_STPL_HEAD_Condition.stapleSupplyLevel == DVST_STPL_SUPPLY_LEVEL_STTS_LOW ) {
		if( CMPL_STPL_HEAD_GetLowstapleSensor() == OFF ) {
			gCMPL_STPL_HEAD_Control.stapleCount = 0;
			SRVC_SetNvmValue(gCMPL_STPL_HEAD_Control.stapleCount, NVMID_STAPLE_COUNT_DETECT_NEAR_LOW_STAPLE) ;
			gCMPL_STPL_HEAD_Condition.stapleSupplyLevel = DVST_STPL_SUPPLY_LEVEL_STTS_HARIOK;
			CMPL_STPL_InformedStapleSupplyLevel(DVST_STPL_SUPPLY_LEVEL_STTS_HARIOK);
		}
		else {
			/* No Action */
		}
	}
	else {
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_StapleFailOccur
// Description   : staple Fail���m����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_StapleFailOccur()
{
	if( gCMPL_STPL_HEAD_Condition.stapleFailstatus == FAULT_NOT_OCCUR ) {
		gCMPL_STPL_HEAD_Condition.stapleFailstatus = FAULT_OCCUR;
		CMPL_STPL_InformedStapleFail(FAULT_OCCUR);
/*		
		switch( gCMPL_STPL_HEAD_Control.action ) {
		
			case CMPL_STPL_HEAD_HOME_st14:
				CMPL_STPL_InformedStapleHeadComp(CMPL_STPL_HEAD_HOME, gCMPL_STPL_HEAD_Control.sheetNo);
				break;
		
			case CMPL_STPL_HEAD_STAPLE_st14:
				CMPL_STPL_InformedStapleHeadComp(CMPL_STPL_HEAD_STAPLE, gCMPL_STPL_HEAD_Control.sheetNo);
				break;

			case CMPL_STPL_HEAD_HARIOUT_st14:
				CMPL_STPL_InformedStapleHeadComp(CMPL_STPL_HEAD_HARIOUT, gCMPL_STPL_HEAD_Control.sheetNo);
				break;

			default : 	break;

		}
*/		
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_stapleNgOccur
// Description   : staple NG���m����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_stapleNgOccur()
{
	if( gCMPL_STPL_HEAD_Condition.stapleNgstatus == FAULT_NOT_OCCUR ) {
		gCMPL_STPL_HEAD_Condition.stapleNgstatus = FAULT_OCCUR;
		CMPL_STPL_InformedStapleNG(FAULT_OCCUR);
/*		
		switch( gCMPL_STPL_HEAD_Control.action ) {
		
			case CMPL_STPL_HEAD_HOME_st13:
				CMPL_STPL_InformedStapleHeadComp(CMPL_STPL_HEAD_HOME, gCMPL_STPL_HEAD_Control.sheetNo);
				break;
		
			case CMPL_STPL_HEAD_STAPLE_st10:
			case CMPL_STPL_HEAD_STAPLE_st13:
				CMPL_STPL_InformedStapleHeadComp(CMPL_STPL_HEAD_STAPLE, gCMPL_STPL_HEAD_Control.sheetNo);
				break;

			case CMPL_STPL_HEAD_HARIOUT_st10:
			case CMPL_STPL_HEAD_HARIOUT_st13:
				CMPL_STPL_InformedStapleHeadComp(CMPL_STPL_HEAD_HARIOUT, gCMPL_STPL_HEAD_Control.sheetNo);
				break;
			default : 	break;

		}*/
			
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_stapleNgClear
// Description   : staple NG�N���A����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_stapleNgClear()
{
	if( gCMPL_STPL_HEAD_Condition.stapleNgstatus == FAULT_OCCUR ) {
		gCMPL_STPL_HEAD_Condition.stapleNgstatus = FAULT_NOT_OCCUR;
		CMPL_STPL_InformedStapleNG(FAULT_NOT_OCCUR);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_CheckstapleReadySensorFailOccur
// Description   : staple Ready Sensor Fail���m����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_CheckstapleReadySensorFailOccur()
{
	if( gCMPL_STPL_HEAD_Condition.stapleReadyFailstatus == FAULT_NOT_OCCUR ) {
		if( CMPL_STPL_HEAD_GetSelfPrimingSensor() == OFF ) {
			gCMPL_STPL_HEAD_Condition.stapleReadyFailstatus = FAULT_OCCUR;
			CMPL_STPL_InformedStapleReadySensorFail(FAULT_OCCUR);
		}
		else {
			/* No Action */
		}
	}
	else {
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_stapleReadySensorFailClear
// Description   : staple Ready Sensor Fail�N���A����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_CheckstapleReadySensorFailClear()
{
	if( gCMPL_STPL_HEAD_Condition.stapleReadyFailstatus == FAULT_OCCUR ) {
		if( CMPL_STPL_HEAD_GetSelfPrimingSensor() == ON ) {
			gCMPL_STPL_HEAD_Condition.stapleReadyFailstatus = FAULT_NOT_OCCUR;
			CMPL_STPL_InformedStapleReadySensorFail(FAULT_NOT_OCCUR);
		}
		else {
			/* No Action */
		}
	}
	else {
		/* No Action */
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_AllCoverClose
// Description   : �J�o�[�N���[�Y�ʒm�̎�M����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_AllCoverClose()
{
	CMPL_STPL_HEAD_CheckLowstapleClear();
	// For Test LeeJE CMPL_STPL_HEAD_CheckstapleReadySensorFailClear();							// T.B.D.?????
	CMPL_STPL_HEAD_stapleNgClear();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HomeSensorON
// Description   : Staple Home Sensor ON
// Parameter     : 
//               : 
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HomeSensorON()
{

}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HomeSensorOff
// Description   : Staple Home Sensor OFF
// Parameter     : 
//               : 
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HomeSensorOff()
{

}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_Compo_st1
// Description   : �X�e�[�v���[���[�^�[���]����(CompoCont) �J�n
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Compo_st1()
{
	DD_DIO_SetOutput(DD_DO_STAPLER_MOT_IN1, gCMPL_STPL_HEAD_Control.direction);		// DD_DO_STAPLER_MOT_IN1

	switch ( gCMPL_STPL_HEAD_Control.compoAction ) {

	case CMPL_STPL_HEAD_COMPO_FORWARD :
		gCMPL_STPL_HEAD_Control.timerIndex = MSP_SetEventTimer
			                                     (140,CMPL_STPL_HEAD_ReceiveEventTimerTimeUp, 0);
		gCMPL_STPL_HEAD_Control.interruptIndex = DD_INT_EntryCallback(DD_INT_STAPLE_HOME_SENSOR,
										             STPL_RISE, CMPL_STPL_HEAD_InterruptStapleHomeSensorON, 0);  // Intrupt Set
		DD_PWM_SetFrequency(DD_PWM_ID_STAPLER_MOT, 30000, DUTY_100);				// 140ms�� PWM 100 %
		DD_PWM_Start(DD_PWM_ID_STAPLER_MOT);
		break;

	case CMPL_STPL_HEAD_COMPO_REVERSE :
		if ( DD_DIO_GetInput(DD_DI_STAPLE_HOME_SNR) == OFF ) {
			gCMPL_STPL_HEAD_Control.timerIndex = MSP_SetEventTimer(100, CMPL_STPL_HEAD_ReceiveEventTimerTimeUp, 0);
			DD_PWM_SetFrequency(DD_PWM_ID_STAPLER_MOT, 30000, DUTY_100);
			DD_PWM_Start(DD_PWM_ID_STAPLER_MOT);
		} else {
			gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_COMPO_st3;
			gCMPL_STPL_HEAD_Control.timerIndex = MSP_SetEventTimer(0, CMPL_STPL_HEAD_ReceiveEventTimerTimeUp, 0);
		}
		break;
		
	default:
		break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_Compo_st2
// Description   : �X�e�[�v���[���[�^�[���]����(CompoCont) Sensor On Edge���m
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Compo_st2()
{
	switch ( gCMPL_STPL_HEAD_Control.compoAction ) {

	case CMPL_STPL_HEAD_COMPO_FORWARD :
		DD_PWM_SetFrequency(DD_PWM_ID_STAPLER_MOT, 30000, DUTY_70);				// PWM 70 %
		break;

	case CMPL_STPL_HEAD_COMPO_REVERSE :
		gCMPL_STPL_HEAD_Control.timerIndex = MSP_SetEventTimer
			                                     (0, CMPL_STPL_HEAD_ReceiveEventTimerTimeUp, 0);
		break;

	default:
		break;
	}	
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_Compo_st3
// Description   : �X�e�[�v���[���[�^�[���]����(CompoCont) Sensor On Edge���m����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Compo_st3()
{
		gCMPL_STPL_HEAD_Control.timerIndex = MSP_SetEventTimer
			                                     (0, CMPL_STPL_HEAD_ReceiveEventTimerTimeUp, 0);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_Compo_st4
// Description   : �X�e�[�v���[���[�^�[�t�]����(CompoCont) �J�n
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Compo_st4()
{
	DD_DIO_SetOutput(DD_DO_STAPLER_MOT_IN1, OFF);		// DD_DO_STAPLER_MOT_IN1
	DD_PWM_Stop(DD_PWM_ID_STAPLER_MOT);
	CMPL_STPL_InformedCompoComplete((US)gCMPL_STPL_HEAD_Control.compoAction);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_Home_st1
// Description   : �X�e�[�v���w�b�h�z�[������ �J�n
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_SetMotorOn(UC  aCurrent)
{
	switch(aCurrent) {
		case  ON :   
			DD_DIO_SetOutput(DD_DO_STAPLER_MOT_IN1, ON); // CW : FORWARD
			break;
			
		case  OFF :   
			DD_DIO_SetOutput(DD_DO_STAPLER_MOT_IN1, OFF); // CCW : REVERSE
			break;

		default :  break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_Home_st1
// Description   : �X�e�[�v���w�b�h�z�[������ �J�n
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Home_st1()
{
	CMPL_STPL_HEAD_CheckLowstapleOccur();
	CMPL_STPL_HEAD_CheckNearLowstapleOuccur();

	if( CMPL_STPL_HEAD_GetStapleHomeSensor() == ON ) {
		CMPL_STPL_InformedStapleHeadComp(CMPL_STPL_HEAD_HOME, gCMPL_STPL_HEAD_Control.sheetNo);
		CMPL_STPL_PrintLog(1, 1, gCMPL_STPL_HEAD_Control.sheetNo, gCMPL_STPL_HEAD_Control.action);		
		gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_ACT_IDLE;
		
	}
	else {
		CMPL_STPL_HEAD_StaplerMotorStaple();
		CMPL_STPL_HEAD_SetEventTimer(CMPL_STPL_HEAD_FAIL_TIME700, CMPL_STPL_HEAD_NORMAL_EVT);
		CMPL_STPL_HEAD_SetInterrupt();
		CMPL_STPL_PrintLog(1, 2, gCMPL_STPL_HEAD_Control.sheetNo, gCMPL_STPL_HEAD_Control.action);
		gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_HOME_st2;	
		
	}
}
///////////////////////////////////////////////////////////////////
// Function Name :CMPL_STPL_HEAD_Home_st2
// Description   : No Action  - StapleHomeSnr�҂�  -Interrupt �ŏ���
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Home_st2()
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name :CMPL_STPL_HEAD_Home_st3
// Description   :  StapleHomeSnrOn��� ����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Home_st3()
{
	CMPL_STPL_InformedStapleHeadComp(CMPL_STPL_HEAD_HOME, gCMPL_STPL_HEAD_Control.sheetNo);
	CMPL_STPL_PrintLog(1, 3, gCMPL_STPL_HEAD_Control.sheetNo, gCMPL_STPL_HEAD_Control.action);
	gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_ACT_IDLE;
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_Home_st10
// Description   : HomeSnrOnFail��������
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Home_st10()
{

	CMPL_STPL_HEAD_StaplerMotorStop();

	// gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_HOME_st11;

	CMPL_STPL_HEAD_SetEventTimer(CMPL_STPL_HEAD_FAIL_WAIT_TIME100, CMPL_STPL_HEAD_NORMAL_EVT);
	CMPL_STPL_PrintLog(1, 10, gCMPL_STPL_HEAD_Control.sheetNo, gCMPL_STPL_HEAD_Control.action);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_Home_st11
// Description   : StapleMotorReverse�����쓮
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Home_st11()
{
	CMPL_STPL_HEAD_StaplerMotorReverse();

	CMPL_STPL_HEAD_SetEventTimer(CMPL_STPL_HEAD_FAIL_MOVE_TIME200, CMPL_STPL_HEAD_NORMAL_EVT);
	CMPL_STPL_HEAD_SetInterrupt();
	CMPL_STPL_PrintLog(1, 11, gCMPL_STPL_HEAD_Control.sheetNo, gCMPL_STPL_HEAD_Control.action);

	gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_HOME_st12;
}
///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_Home_st12
// Description   : No Action - StapleHomeSnr�҂� -Interrupt �ŏ���
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Home_st12()
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_Home_st13
// Description   : HomeSnrOn����(StapleNG����)
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Home_st13()
{
	CMPL_STPL_HEAD_stapleNgOccur();

	CMPL_STPL_InformedStapleHeadComp(CMPL_STPL_HEAD_HOME, gCMPL_STPL_HEAD_Control.sheetNo);
	CMPL_STPL_PrintLog(1, 13, gCMPL_STPL_HEAD_Control.sheetNo, gCMPL_STPL_HEAD_Control.action);
	gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_ACT_IDLE;

}
///////////////////////////////////////////////////////////////////
// Function Name :CMPL_STPL_HEAD_Home_st14
// Description   : Fail���� 
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Home_st14()
{
	CMPL_STPL_HEAD_CancelInterrupt();
	CMPL_STPL_HEAD_StapleFailOccur();
	CMPL_STPL_InformedStapleHeadComp(CMPL_STPL_HEAD_HOME, gCMPL_STPL_HEAD_Control.sheetNo);
	CMPL_STPL_PrintLog(1, 14, gCMPL_STPL_HEAD_Control.sheetNo, gCMPL_STPL_HEAD_Control.action);
	gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_ACT_IDLE;
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_Staple_st1
// Description   : No Action -> CMPL_STPL_HEAD_StapleStart �ŏ���
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Staple_st1()
{
	CMPL_STPL_HEAD_CheckLowstapleOccur();
	CMPL_STPL_HEAD_CheckstapleReadySensorFailOccur();

	// if ( CMPL_STPL_HEAD_GetSelfPrimingSensor( ) == ON) return;

	if ( (gCMPL_STPL_HEAD_Condition.stapleNgstatus == FAULT_OCCUR) ||
		(gCMPL_STPL_HEAD_Condition.stapleReadyFailstatus == FAULT_OCCUR) ) {
		CMPL_STPL_InformedStapleHeadComp(CMPL_STPL_HEAD_STAPLE, gCMPL_STPL_HEAD_Control.sheetNo);
		CMPL_STPL_PrintLog(2, 1, gCMPL_STPL_HEAD_Control.sheetNo, gCMPL_STPL_HEAD_Control.action);
		gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_ACT_IDLE;
	}
	else {
		CMPL_STPL_HEAD_StaplerMotorStaple();

		CMPL_STPL_HEAD_SetEventFailTimer(CMPL_STPL_HEAD_FAIL_TIME700, CMPL_STPL_HEAD_NORMAL_EVT);
		CMPL_STPL_HEAD_SetInterrupt();
		CMPL_STPL_PrintLog(2, 2, gCMPL_STPL_HEAD_Control.sheetNo, gCMPL_STPL_HEAD_Control.action);		
		gCMPL_STPL_HEAD_Control.action++;//CMPL_STPL_HEAD_STAPLE_st2, CMPL_STPL_HEAD_HARIOUT_st2
	}
}

///////////////////////////////////////////////////////////////////
// Function Name :CMPL_STPL_HEAD_Staple_st2
// Description   : No Action - StapleHomeSnr�҂� - Interrupt �ŏ���
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Staple_st2()
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_STAPLE_st3
// Description   : StapleHomeSnrOn���(���슮��)
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Staple_st3()
{ 
	UL ulStapleCount;
	
	CMPL_STPL_HEAD_NearLowstapleCountIncrement();
	CMPL_STPL_HEAD_CheckNearLowstapleOuccur();
	
	CMPL_STPL_InformedStapleHeadComp(CMPL_STPL_HEAD_STAPLE, gCMPL_STPL_HEAD_Control.sheetNo);
	ulStapleCount = SRVC_GetNvmValueUL(NVMID_STAPLE_COUNT)+1L;
	SRVC_SetNvmValueUL(ulStapleCount, NVMID_STAPLE_COUNT );
	SRVC_IncrementHfsi(HFSIID_STACKER_STAPLER_LIFE);

	CMPL_STPL_PrintLog(2, 3, gCMPL_STPL_HEAD_Control.sheetNo, gCMPL_STPL_HEAD_Control.action);

	gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_ACT_IDLE;
	
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_STAPLE_st10
// Description   : HomeSnrOnFail��������
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Staple_st10()
{
	CMPL_STPL_HEAD_StaplerMotorStop();

	if( CMPL_STPL_HEAD_GetStapleHomeSensor() == ON ) {
		CMPL_STPL_HEAD_stapleNgOccur();

		CMPL_STPL_InformedStapleHeadComp(CMPL_STPL_HEAD_STAPLE, gCMPL_STPL_HEAD_Control.sheetNo);
		CMPL_STPL_PrintLog(2, 10, gCMPL_STPL_HEAD_Control.sheetNo, gCMPL_STPL_HEAD_Control.action);
		gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_ACT_IDLE;		
	}
	else {
		// gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_ACT_STAPLE_FAIL_MOVE_WAIT;

		CMPL_STPL_HEAD_SetEventTimer(CMPL_STPL_HEAD_FAIL_WAIT_TIME100, CMPL_STPL_HEAD_NORMAL_EVT);
		CMPL_STPL_PrintLog(2, 11, gCMPL_STPL_HEAD_Control.sheetNo, gCMPL_STPL_HEAD_Control.action);		
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_Staple_st11
// Description   : StapleMotorReverse�����쓮
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Staple_st11()
{
	CMPL_STPL_HEAD_StaplerMotorReverse();

	CMPL_STPL_HEAD_SetEventTimer(CMPL_STPL_HEAD_FAIL_MOVE_TIME200, CMPL_STPL_HEAD_NORMAL_EVT);
	CMPL_STPL_HEAD_SetInterrupt();
	CMPL_STPL_PrintLog(2, 12, gCMPL_STPL_HEAD_Control.sheetNo, gCMPL_STPL_HEAD_Control.action);
	gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_STAPLE_st12;
}
///////////////////////////////////////////////////////////////////
// Function Name :CMPL_STPL_HEAD_Staple_st12
// Description   : No Action - StapleHomeSnr�҂� - Interrupt �ŏ���
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Staple_st12()
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_Staple_st13
// Description   : HomeSnrOn�҂�(StapleNG�ʒm)
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Staple_st13()
{
	CMPL_STPL_HEAD_stapleNgOccur();

	CMPL_STPL_InformedStapleHeadComp(CMPL_STPL_HEAD_STAPLE, gCMPL_STPL_HEAD_Control.sheetNo);
	CMPL_STPL_PrintLog(2, 13, gCMPL_STPL_HEAD_Control.sheetNo, gCMPL_STPL_HEAD_Control.action);
	gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_ACT_IDLE;
}

///////////////////////////////////////////////////////////////////
// Function Name :CMPL_STPL_HEAD_Staple_st14
// Description   : Fail���� 
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Staple_st14()
{
	CMPL_STPL_HEAD_CancelInterrupt();
	CMPL_STPL_HEAD_StapleFailOccur();
	CMPL_STPL_InformedStapleHeadComp(CMPL_STPL_HEAD_STAPLE, gCMPL_STPL_HEAD_Control.sheetNo);
	CMPL_STPL_PrintLog(2, 14, gCMPL_STPL_HEAD_Control.sheetNo, gCMPL_STPL_HEAD_Control.action);
	gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_ACT_IDLE;	
}
///////////////////////////////////////////////////////////////////
// Function Name :CMPL_STPL_HEAD_Hariout_st1
// Description   : No Action -> CMPL_STPL_HEAD_StapleStart �ŏ��� 
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Hariout_st1()
{
	if ( gCMPL_STPL_HEAD_Condition.ReadyFailCount == 0 ) {
		CMPL_STPL_HEAD_CheckLowstapleOccur();
		CMPL_STPL_HEAD_CheckNearLowstapleOuccur();
	}

	if ( CMPL_STPL_HEAD_GetSelfPrimingSensor( ) == ON){
		CMPL_STPL_HEAD_CheckstapleReadySensorFailClear();
		CMPL_STPL_InformedStapleHeadComp(CMPL_STPL_HEAD_HARIOUT, gCMPL_STPL_HEAD_Control.sheetNo);
		CMPL_STPL_PrintLog(3, 1, gCMPL_STPL_HEAD_Control.sheetNo, gCMPL_STPL_HEAD_Control.action);
		gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_ACT_IDLE;
		return;
	}

/*	if ( (gCMPL_STPL_HEAD_Condition.stapleNgstatus == FAULT_OCCUR) || **** TB37 Stapler Feed Ready Fail�����A���Ȃ����̑΍� V3.7.0�ɓ����\�� *****
		(gCMPL_STPL_HEAD_Condition.stapleReadyFailstatus == FAULT_OCCUR) ) {
		CMPL_STPL_InformedStapleHeadComp(CMPL_STPL_HEAD_HARIOUT, gCMPL_STPL_HEAD_Control.sheetNo);
		CMPL_STPL_PrintLog(3, 2, gCMPL_STPL_HEAD_Control.sheetNo, gCMPL_STPL_HEAD_Control.action);
		gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_ACT_IDLE;
	}
	else { */
		CMPL_STPL_HEAD_StaplerMotorStaple();

		CMPL_STPL_HEAD_SetEventTimer(CMPL_STPL_HEAD_FAIL_TIME700, CMPL_STPL_HEAD_NORMAL_EVT);
		CMPL_STPL_HEAD_SetInterrupt();
		CMPL_STPL_PrintLog(3, 3, gCMPL_STPL_HEAD_Control.sheetNo, gCMPL_STPL_HEAD_Control.action);		
		gCMPL_STPL_HEAD_Control.action++;//CMPL_STPL_HEAD_STAPLE_st2, CMPL_STPL_HEAD_HARIOUT_st2
	// }
}

///////////////////////////////////////////////////////////////////
// Function Name :CMPL_STPL_HEAD_Hariout_st2
// Description   : No Action - StapleHomeSnr�҂� - Interrupt �ŏ���
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Hariout_st2()
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_Hariout_st3
// Description   : StapleHomeSnrOn���
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Hariout_st3()
{
	CMPL_STPL_HEAD_SetEventTimer(CMPL_STPL_HEAD_WAIT_TIME1000, CMPL_STPL_HEAD_NORMAL_EVT);
	CMPL_STPL_PrintLog(3, 4, gCMPL_STPL_HEAD_Control.sheetNo, gCMPL_STPL_HEAD_Control.action);
}
///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_Hariout_st4
// Description   : Self Priming Sensor����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Hariout_st4()
{

	if ( CMPL_STPL_HEAD_GetSelfPrimingSensor() == OFF ) {
		gCMPL_STPL_HEAD_Condition.ReadyFailCount++;

		if( gCMPL_STPL_HEAD_Condition.ReadyFailCount > 12 ){ //  V3.7.0�ɏC��, TB37Follow���Ŕ���
			CMPL_STPL_HEAD_CheckstapleReadySensorFailOccur();
			CMPL_STPL_InformedStapleHeadComp(CMPL_STPL_HEAD_HARIOUT, gCMPL_STPL_HEAD_Control.sheetNo);
			CMPL_STPL_PrintLog(3, 5, gCMPL_STPL_HEAD_Control.sheetNo, gCMPL_STPL_HEAD_Control.action);			
			gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_ACT_IDLE;

		} else {
			CMPL_STPL_HEAD_SetEventTimer(0, CMPL_STPL_HEAD_INTERRUPT_EVT);
			CMPL_STPL_PrintLog(3, 6, gCMPL_STPL_HEAD_Control.sheetNo, gCMPL_STPL_HEAD_Control.action);
			gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_HARIOUT_st1;
		}

	} else {
		CMPL_STPL_HEAD_CheckstapleReadySensorFailClear();
		CMPL_STPL_InformedStapleHeadComp(CMPL_STPL_HEAD_HARIOUT, gCMPL_STPL_HEAD_Control.sheetNo);
		CMPL_STPL_PrintLog(3, 7, gCMPL_STPL_HEAD_Control.sheetNo, gCMPL_STPL_HEAD_Control.action);
		gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_ACT_IDLE;
	}

}
///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_STAPLE_st10
// Description   : HomeSnrOnFail��������
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Hariout_st10()
{
	CMPL_STPL_HEAD_StaplerMotorStop();

	if( CMPL_STPL_HEAD_GetStapleHomeSensor() == ON ) {
		CMPL_STPL_HEAD_stapleNgOccur();
		CMPL_STPL_InformedStapleHeadComp(CMPL_STPL_HEAD_HARIOUT, gCMPL_STPL_HEAD_Control.sheetNo);
		CMPL_STPL_PrintLog(3, 10, gCMPL_STPL_HEAD_Control.sheetNo, gCMPL_STPL_HEAD_Control.action);
		gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_ACT_IDLE;
		
	} else {
		CMPL_STPL_HEAD_SetEventTimer(CMPL_STPL_HEAD_FAIL_WAIT_TIME100, CMPL_STPL_HEAD_NORMAL_EVT);
		CMPL_STPL_PrintLog(3, 11, gCMPL_STPL_HEAD_Control.sheetNo, gCMPL_STPL_HEAD_Control.action);
	}
}
///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_Home_st11
// Description   : StapleMotorReverse�����쓮
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Hariout_st11()
{
	CMPL_STPL_HEAD_StaplerMotorReverse();

	CMPL_STPL_HEAD_SetEventTimer(CMPL_STPL_HEAD_FAIL_MOVE_TIME200, CMPL_STPL_HEAD_NORMAL_EVT);
	CMPL_STPL_HEAD_SetInterrupt();
	CMPL_STPL_PrintLog(3, 12, gCMPL_STPL_HEAD_Control.sheetNo, gCMPL_STPL_HEAD_Control.action);

	gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_HOME_st12;
}
///////////////////////////////////////////////////////////////////
// Function Name :CMPL_STPL_HEAD_Hariout_st12
// Description   : No Action - StapleHomeSnr�҂� - Interrupt �ŏ���
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Hariout_st12()
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_STPL_HEAD_HOME_st13
// Description   : HomeSnrOn���
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Hariout_st13()
{
	CMPL_STPL_HEAD_stapleNgOccur();

	CMPL_STPL_InformedStapleHeadComp(CMPL_STPL_HEAD_HARIOUT, gCMPL_STPL_HEAD_Control.sheetNo);
	CMPL_STPL_PrintLog(3, 13, gCMPL_STPL_HEAD_Control.sheetNo, gCMPL_STPL_HEAD_Control.action);
	gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_ACT_IDLE;
}
///////////////////////////////////////////////////////////////////
// Function Name :CMPL_STPL_HEAD_Hariout_st14
// Description   :  Fail���� 
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_STPL_HEAD_Hariout_st14()
{
	CMPL_STPL_HEAD_CancelInterrupt();
	CMPL_STPL_HEAD_StapleFailOccur();
	CMPL_STPL_InformedStapleHeadComp(CMPL_STPL_HEAD_HARIOUT, gCMPL_STPL_HEAD_Control.sheetNo);
	CMPL_STPL_PrintLog(3, 14, gCMPL_STPL_HEAD_Control.sheetNo, gCMPL_STPL_HEAD_Control.action);
	gCMPL_STPL_HEAD_Control.action = CMPL_STPL_HEAD_ACT_IDLE;	
}

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
// 2015.04.28 | Lee Jae Eun            | CR-xxxx PWM NVM 2�ǉ�  CMPL_STPL_HEAD_StaplerMotorStaple()
//                                             | DD_PWM_SetFrequency(DD_PWM_ID_STAPLER_MOT, 40000(40Khz) --> 30000(30Khz) 
// 2015.12.22 | Lee Jae Eun            | Debug Log Add
// 2016.02.25  | V3.4.0 Merge [ Device Status Value�w�b�_�t�@�C���쐬 ]




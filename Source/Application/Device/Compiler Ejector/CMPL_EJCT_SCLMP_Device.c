// File Name:	CMPL_EJCT_SCLMP_Device.c
// Description:	Compiler Ejector SetClamp Controller
// Engineer:	FXK
// Last Edit:	15.03.03.
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"
#include "CMPL_EJCT_SCLMP_Device.h"
#include "CMPL_EJCT_Boundary.h"
#include "MSP_EventTimer.h"
#include "DD_DIO.h"
#include "DD_SQP.h"
#include "NvmService.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	ECMPL_EJCT_SCLMP_Action action;
	ECMPL_EJCT_SCLMP_Action compoAction;
	UC sheetNo;
	SS timerIndex;

} SCMP_EJCT_SCLMP_Control;

static SCMP_EJCT_SCLMP_Control  gCMPL_EJCT_SCLMP;

static EFaultStatus gCMPL_EJCT_SCLMP_FaultStat;

#define CMPL_EJCT_SCLMP_NORMAL_EVT		0
#define CMPL_EJCT_SCLMP_INTERRUPT_EVT  	1

#define CMPL_EJCT_SCLMP_FAILTIME_400  	400


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
static void CMPL_EJCT_SCLMP_ControlThread();
static void CMPL_EJCT_SCLMP_ReceiveEventTimerTimeUp(SS aTimerID, UL aNotUsed);


static void  SetClamp_st1();		//
static void  SetClamp_st2_OFFWAIT();//  Eject HomeSNR OFF�҂�
static void  SetClamp_st3(); 		// T4�ݒ�
static void SetClamp_st4();			// SetClampHomeSNR OFF�҂�
static void SetClamp_st5();			// T5�^�C�}�ݒ�
static void SetClamp_st6();			// T6�^�C�}�ݒ�
static void SetClamp_st7();			// SetClampHomeSNR ON�҂�
static void SetClamp_st8(); 		// T7�^�C�}�ݒ�
static void SetClamp_st9(); 		// SetClamp���슮��



/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_SCLMP_StartAct
// Description   : Set Clamp����ݒ�_����J�n
// Parameter     : aAction (���䂷�铮��)
//				 : aSheetNo (���䂷��SheetNo)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_SCLMP_StartAct(ECMPL_EJCT_SCLMP_Action aAction, UC aSheetNo)
{
	switch( aAction ) {
	case CMPL_EJCT_SCLMP_COMPO_CLAMP : // Component
		gCMPL_EJCT_SCLMP.action = CMPL_EJCT_SCLMP_st1;
		gCMPL_EJCT_SCLMP.compoAction = CMPL_EJCT_SCLMP_COMPO_CLAMP;
		break;

	case CMPL_EJCT_SCLMP : //Set Clamp
		gCMPL_EJCT_SCLMP.action = CMPL_EJCT_SCLMP_st1;
		break;

	case CMPL_EJCT_COMPO_CLUTCH : // Component
		gCMPL_EJCT_SCLMP.compoAction = CMPL_EJCT_COMPO_CLUTCH;
		DD_DIO_SetOutput(DD_DO_SET_CLAMP_CLUTCH_ENB, ON);
		break;

	default :
		break;
	}

	gCMPL_EJCT_SCLMP.sheetNo = aSheetNo;
	gCMPL_EJCT_SCLMP.timerIndex = MSP_MAX_TMR_NUM;

	CMPL_EJCT_SCLMP_ControlThread();
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_SCLMP_ControlThread
// Description   : Set Clamp ����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_SCLMP_ControlThread()
{
	/* event sheet no , sheet condition check */
	switch( gCMPL_EJCT_SCLMP.action ) {
	case  CMPL_EJCT_SCLMP_st1 :  		// Eject HomeSNR��ԃ`�F�b�N
		SetClamp_st1();
		break;
	case  CMPL_EJCT_SCLMP_st2_OFFWAIT :  // Eject HomeSNR��ԃ`�F�b�N
		SetClamp_st2_OFFWAIT();
		break;
	case  CMPL_EJCT_SCLMP_st3 ://T4�ݒ�
		SetClamp_st3();
		break;
	case  CMPL_EJCT_SCLMP_st4 ://SetClampHomeSNR�@OFF�҂�
		SetClamp_st4();
		break;
	case  CMPL_EJCT_SCLMP_st5 : //T5�^�C�}�ݒ�
		SetClamp_st5();
		break;
	case  CMPL_EJCT_SCLMP_st6 ://T6�^�C�}�ݒ�
		SetClamp_st6();
		break;
	case  CMPL_EJCT_SCLMP_st7 ://SetClampHomeSNR�@ON�҂�
		SetClamp_st7();
		break;
	case  CMPL_EJCT_SCLMP_st8 ://T7�^�C�}�ݒ�
		SetClamp_st8();
		break;
	case  CMPL_EJCT_SCLMP_st9 : //SetClamp���슮��
		SetClamp_st9();
		break;

	default:
		break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_SCLMP_ReceiveEventTimerTimeUp
// Description   : �C�x���g�^�C�}�̃^�C���A�b�v
// Parameter     : timerID (Timer ID)
//               : notUsed (Not Used)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_SCLMP_ReceiveEventTimerTimeUp(SS aTimerID, UL aNotUsed)
{
	if ( aTimerID != gCMPL_EJCT_SCLMP.timerIndex ) {
		return;
	}

	gCMPL_EJCT_SCLMP.timerIndex = MSP_MAX_TMR_NUM;

	switch ( gCMPL_EJCT_SCLMP.action ) {

		case CMPL_EJCT_SCLMP_st1 :  //
		case CMPL_EJCT_SCLMP_st3 :  // T4 Timeout
		case CMPL_EJCT_SCLMP_st5 :  // T5 Timeout
		case CMPL_EJCT_SCLMP_st6 :  // T6 Timeout
		case CMPL_EJCT_SCLMP_st8 :  // T7 Timeout
			gCMPL_EJCT_SCLMP.action++;
			CMPL_EJCT_SCLMP_ControlThread();
			break;

		case  CMPL_EJCT_SCLMP_st4 :  // Fail  Timeout
			gCMPL_EJCT_SCLMP_FaultStat = FAULT_OCCUR;
			CMPL_EJCT_InformedSetClampHomeSensorOffFail();
			break;

		case  CMPL_EJCT_SCLMP_st7 : //Fail Timeout
			gCMPL_EJCT_SCLMP_FaultStat = FAULT_OCCUR;
			CMPL_EJCT_InformedSetClampHomeSensorOnFail();
			break;

		default:
			break;
	}
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_SCLMP_SetEventTimer
// Description   : �C�x���g�^�C�}�[�Z�b�g
// Parameter     : aTime (�^�C�}�[�l (�~1[ms]))
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_SCLMP_SetEventTimer(US aTime, UL aEventType)
{
	gCMPL_EJCT_SCLMP.timerIndex = MSP_SetEventTimer(aTime, CMPL_EJCT_SCLMP_ReceiveEventTimerTimeUp, aEventType);	// Timer Set
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_SCLMP_CancelEventTimer
// Description   : �C�x���g�^�C�}�[�L�����Z��
// Parameter     : aTime (�^�C�}�[�l (�~1[ms]))
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_SCLMP_CancelEventTimer()
{
	if(gCMPL_EJCT_SCLMP.timerIndex != MSP_MAX_TMR_NUM ) {
		MSP_CancelEventTimer(gCMPL_EJCT_SCLMP.timerIndex);
		gCMPL_EJCT_SCLMP.timerIndex = MSP_MAX_TMR_NUM;
	}
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_SCLMP_GetEjectorHomeSensor
// Description   : �C�W�F�N�^�[�z�[���Z���T�[�̏��(Filter����)���擾
// Parameter     : �]
// Return        : �Z���T�[���x��(ON/OFF)
///////////////////////////////////////////////////////////////////
UC CMPL_EJCT_SCLMP_GetEjectorHomeSensor()
{
	if( DD_DIO_GetInput(DD_DI_EJECT_HOME_SNR) == HIGH ) {
		return ON;
	}
	else {
		return OFF;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_SCLMP_GetSetClampHomeSensor
// Description   : SetClampHome��Z���T�[�̏��(Filter����)���擾
// Parameter     : �]
// Return        : �Z���T�[���x��(ON/OFF)
///////////////////////////////////////////////////////////////////
UC CMPL_EJCT_SCLMP_GetSetClampHomeSensor()
{
	if( DD_DIO_GetInput(DD_DI_SET_CLAMP_HOME_SNR) == HIGH ) {
		return ON;
	}
	else {
		return OFF;
	}
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_SCLMP_EjecterHomeSensorON
// Description   : Ejecter Home Sensor OFF��ON�����m
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_SCLMP_EjecterHomeSensorON()
{
	if ( gCMPL_EJCT_SCLMP.action == CMPL_EJCT_SCLMP_st0 )
		return;

	switch(gCMPL_EJCT_SCLMP.action)
	{
		case   CMPL_EJCT_SCLMP_st1 :
			CMPL_EJCT_SCLMP_CancelEventTimer();
			gCMPL_EJCT_SCLMP.action = CMPL_EJCT_SCLMP_st2_OFFWAIT;
			CMPL_EJCT_SCLMP_ControlThread();
		break;

		default :  break;
	}


}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_SCLMP_EjecterHomeSensorOFF
// Description   : Ejecter Home Sensor ON��OFF�����m
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_SCLMP_EjecterHomeSensorOFF()
{
	if ( gCMPL_EJCT_SCLMP.action == CMPL_EJCT_SCLMP_st0 )
		return;

	switch (gCMPL_EJCT_SCLMP.action ) {
		case CMPL_EJCT_SCLMP_st2_OFFWAIT :
			CMPL_EJCT_SCLMP_CancelEventTimer();
			gCMPL_EJCT_SCLMP.action++;  // T4�ݒ��
			CMPL_EJCT_SCLMP_ControlThread();
			break;

		default :
			break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_SCLMP_SetClampHomeSensorON
// Description   : SetClamp Home Sensor OFF��ON�����m
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_SCLMP_SetClampHomeSensorON()
{
	if ( gCMPL_EJCT_SCLMP.action == CMPL_EJCT_SCLMP_st0 )
		return;

	switch(gCMPL_EJCT_SCLMP.action)
	{
		case   CMPL_EJCT_SCLMP_st7 :
			CMPL_EJCT_SCLMP_CancelEventTimer();
			gCMPL_EJCT_SCLMP.action = CMPL_EJCT_SCLMP_st8; //T7 Timer�ݒ��
			CMPL_EJCT_SCLMP_ControlThread();
		break;

		default :  break;
	}

}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_SCLMP_SetClampHomeSensorOFF
// Description   : SetClamp Home Sensor ON��OFF�����m
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_SCLMP_SetClampHomeSensorOFF ()
{
	if ( gCMPL_EJCT_SCLMP.action == CMPL_EJCT_SCLMP_st0 )
		return;

	switch ( gCMPL_EJCT_SCLMP.action ) {
	case CMPL_EJCT_SCLMP_st4 :
		CMPL_EJCT_SCLMP_CancelEventTimer();
		gCMPL_EJCT_SCLMP.action = CMPL_EJCT_SCLMP_st5; 	// T5 Timer�ݒ��
		CMPL_EJCT_SCLMP_ControlThread();
		break;

	default :
		break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_SCLMP_IsExecutiongDeviceAction
// Description   : Set Clamp�̓����Ԃ��擾
// Parameter     : �]
// Return        : true�F���쒆�Afalse�F��~��
///////////////////////////////////////////////////////////////////
bool CMPL_EJCT_SCLMP_IsExecutiongDeviceAction()
{
	if( gCMPL_EJCT_SCLMP.action == CMPL_EJCT_SCLMP_st0) {
		return false;
	}
	else {
		return true;
	}
}

//V3.6.93
//bool CMPL_EJCT_ROLL_IsOccurFault()
bool CMPL_EJCT_SCLMP_IsOccurFault()
{
	if(	gCMPL_EJCT_SCLMP_FaultStat == FAULT_OCCUR ) {
		return true;
	}
	else {
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_SCLMP_Reset
// Description   :  Set Clamp ���Z�b�g
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_SCLMP_Reset()
{
	gCMPL_EJCT_SCLMP.action = CMPL_EJCT_SCLMP_st0;
	gCMPL_EJCT_SCLMP.compoAction = CMPL_EJCT_SCLMP_st0;

	gCMPL_EJCT_SCLMP.sheetNo = SHEETNO_NUM;
	gCMPL_EJCT_SCLMP.timerIndex = MSP_MAX_TMR_NUM;

	gCMPL_EJCT_SCLMP_FaultStat = FAULT_NOT_OCCUR;
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_SCLMP_DeviceStop
// Description   : Set Clamp��~
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_SCLMP_DeviceStop()
{
	US aAction = gCMPL_EJCT_SCLMP.action & CMPL_EJCT_SCLMP_ACTION_MASK;
	UC aSheetNo = gCMPL_EJCT_SCLMP.sheetNo;

	gCMPL_EJCT_SCLMP.action = CMPL_EJCT_SCLMP_st0;
	gCMPL_EJCT_SCLMP.sheetNo = SHEETNO_NUM;

	CMPL_EJCT_SCLMP_CancelEventTimer();
	DD_DIO_SetOutput(DD_DO_SET_CLAMP_CLUTCH_ENB, OFF);

	if(CMPL_EJCT_SCLMP_st0 ==gCMPL_EJCT_SCLMP.compoAction)  // Component Stop����Ă΂��Ƃ��͎��s���Ȃ�
	  CMPL_EJCT_InformedEjectMotorComp(aAction, aSheetNo);

	gCMPL_EJCT_SCLMP.compoAction = CMPL_EJCT_SCLMP_st0;
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_SCLMP_StopSheet
// Description   : �w��V�[�g�̓����~
// Parameter     : aSheetNo (�w��V�[�g�ԍ�)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_SCLMP_StopSheet(UC aSheetNo)
{
	if( aSheetNo == SHEETNO_NUM) {												// ���䒆�̗p���̃R���f�B�V�������`�F�b�N
		if( gCMPL_EJCT_SCLMP.sheetNo != SHEETNO_NUM ) {
			if( SRVC_CheckSheetConditionNormal(gCMPL_EJCT_SCLMP.sheetNo) == false ) {
				CMPL_EJCT_SCLMP_DeviceStop();
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
		if( aSheetNo == gCMPL_EJCT_SCLMP.sheetNo ) {
			CMPL_EJCT_SCLMP_DeviceStop();
		}
		else {																	// �w��V�[�g�Ɛ���V�[�g���قȂ�
			/* No Action */
		}
	}
}
 ///////////////////////////////////////////////////////////////////
 // Function Name : SetClamp_st1_OFFWAIT
 // Description   : Eject HomeSNR�@OFF�҂�
 // Parameter	  : ����
 // Return		  : ����
 ///////////////////////////////////////////////////////////////////
 void SetClamp_st1()
 {
	 CMPL_EJCT_SCLMP_SetEventTimer(0, CMPL_EJCT_SCLMP_NORMAL_EVT);
	 
	 PrintEjectLog_Short(CMPL_EJCT_SCLMP_st1, gCMPL_EJCT_SCLMP.sheetNo, 0);	 //Debug Log ..
 }

///////////////////////////////////////////////////////////////////
// Function Name : SetClamp_st2
// Description	 : Eject HomeSNR�@OFF�҂�
// Parameter	 : ����
// Return		 : ����
///////////////////////////////////////////////////////////////////
 void SetClamp_st2_OFFWAIT()
{
	// EjectHomeSNR�ω���������Callback�֐��ŏ��������̂�
  	// �����ł͏������邱�Ɩ����B
}

///////////////////////////////////////////////////////////////////
// Function Name : SetClamp_st3
// Description	 : T4
// Parameter	 : �]
// Return		 : �]
///////////////////////////////////////////////////////////////////
 void  SetClamp_st3()
{
	 CMPL_EJCT_SCLMP_SetEventTimer(SRVC_GetNvmValueUS(NVMID_SET_CLAMP_UP_START_TIME_4),
	 								     CMPL_EJCT_SCLMP_NORMAL_EVT);
	  PrintEjectLog_Short(CMPL_EJCT_SCLMP_st3, gCMPL_EJCT_SCLMP.sheetNo, 0);	 //Debug Log ..
}

///////////////////////////////////////////////////////////////////
// Function Name : SetClamp_st4
// Description	 : SetClampHomeSNR�@OFF�҂�
// Parameter	 : �]
// Return		 : �]
///////////////////////////////////////////////////////////////////
 void SetClamp_st4()
{
	//V3.4.0 �d�l�ύX�FSetClampHomeSNR=OFF�̏ꍇ�AT5�Z�b�g��
       if ( DD_DIO_GetInput(DD_DI_SET_CLAMP_HOME_SNR) == LOW) { // SetClampHomeSNR = OFF 

		gCMPL_EJCT_SCLMP.action = CMPL_EJCT_SCLMP_st5;	// T5 Timer�ݒ��

		 PrintEjectLog_Short(CMPL_EJCT_SCLMP_st4, gCMPL_EJCT_SCLMP.sheetNo, 0);	 //Debug Log ..		

		CMPL_EJCT_SCLMP_ControlThread();
		
       }else {  // SetClampHomeSNR = ON 
		   // Clutch ON (V3.3.94) : SetClampHomeSNR=ON�̏ꍇ�̂�CLUTCH��ON����
		   DD_DIO_SetOutput(DD_DO_SET_CLAMP_CLUTCH_ENB, ON);

		 CMPL_EJCT_SCLMP_SetEventTimer(CMPL_EJCT_SCLMP_FAILTIME_400,
									   CMPL_EJCT_SCLMP_NORMAL_EVT);
	 	 PrintEjectLog_Short(CMPL_EJCT_SCLMP_st4, gCMPL_EJCT_SCLMP.sheetNo, 0);	 //Debug Log ..
        }
}

///////////////////////////////////////////////////////////////////
// Function Name : SetClamp_st5
// Description	 : T5�^�C�}�ݒ�
// Parameter	 : �]
// Return		 : �]
///////////////////////////////////////////////////////////////////
 void SetClamp_st5()
{
	CMPL_EJCT_SCLMP_SetEventTimer(SRVC_GetNvmValueUC(NVMID_SET_CLAMP_UP_STOP_TIME_5),
									    CMPL_EJCT_SCLMP_NORMAL_EVT);
	 PrintEjectLog_Short(CMPL_EJCT_SCLMP_st5, gCMPL_EJCT_SCLMP.sheetNo, 0);	 //Debug Log ..
}

///////////////////////////////////////////////////////////////////
// Function Name : SetClamp_st6
// Description	 : T6�^�C�}�ݒ�
// Parameter	 : �]
// Return		 : �]
///////////////////////////////////////////////////////////////////
 void SetClamp_st6()
{
	US	 time=0;
	time = SRVC_GetNvmValueUC(NVMID_SET_CLAMP_LOWER_START_TIME_6);
	time = time * 10;
		
	// Clutch OFF
	DD_DIO_SetOutput(DD_DO_SET_CLAMP_CLUTCH_ENB, OFF);
	//V3.3.91 TestROM : 1ms -> 10ms  (T6 Timer)
	CMPL_EJCT_SCLMP_SetEventTimer(time,    CMPL_EJCT_SCLMP_NORMAL_EVT);
	 PrintEjectLog_Short(CMPL_EJCT_SCLMP_st6, gCMPL_EJCT_SCLMP.sheetNo, 0);  //Debug Log ..
}

///////////////////////////////////////////////////////////////////
// Function Name : SetClamp_st7
// Description	 : SetClampHomeSNR�@ON�҂�
// Parameter	 : �]
// Return		 : �]
///////////////////////////////////////////////////////////////////
void SetClamp_st7()
{
	// Clutch ON
	DD_DIO_SetOutput(DD_DO_SET_CLAMP_CLUTCH_ENB, ON);
	//Fail Timer
	 CMPL_EJCT_SCLMP_SetEventTimer(CMPL_EJCT_SCLMP_FAILTIME_400,
									   CMPL_EJCT_SCLMP_NORMAL_EVT);
	 PrintEjectLog_Short(CMPL_EJCT_SCLMP_st7, gCMPL_EJCT_SCLMP.sheetNo, 0);	 //Debug Log ..
}

///////////////////////////////////////////////////////////////////
// Function Name : SetClamp_st8
// Description	 :  T7�^�C�}�ݒ�
// Parameter	 : �]
// Return		 : �]
///////////////////////////////////////////////////////////////////
void SetClamp_st8()
{
	CMPL_EJCT_SCLMP_SetEventTimer(SRVC_GetNvmValueUC(NVMID_SET_CLAMP_LOWER_STOP_TIME_7),
         							   CMPL_EJCT_SCLMP_NORMAL_EVT);
	 PrintEjectLog_Short(CMPL_EJCT_SCLMP_st8, gCMPL_EJCT_SCLMP.sheetNo, 0);	 //Debug Log ..
}

///////////////////////////////////////////////////////////////////
// Function Name : SetClamp_st9
// Description	 : SetClamp���슮��
// Parameter	 : �]
// Return		 : �]
///////////////////////////////////////////////////////////////////
void SetClamp_st9()
{
	// Clutch OFF
	DD_DIO_SetOutput(DD_DO_SET_CLAMP_CLUTCH_ENB, OFF);

	if ( gCMPL_EJCT_SCLMP.compoAction != CMPL_EJCT_SCLMP_st0 ) {  // Component
		CMPL_EJCT_InformedEjectMotorComp((US)gCMPL_EJCT_SCLMP.compoAction,
											   gCMPL_EJCT_SCLMP.sheetNo);
		gCMPL_EJCT_SCLMP.compoAction = CMPL_EJCT_SCLMP_st0;
	} else 	{
		
		CMPL_EJCT_InformedEjectMotorComp((US)CMPL_EJCT_SCLMP,
											   gCMPL_EJCT_SCLMP.sheetNo);
		
	}
	gCMPL_EJCT_SCLMP.action = CMPL_EJCT_SCLMP_st0;

	 PrintEjectLog_Short(CMPL_EJCT_SCLMP_st9, gCMPL_EJCT_SCLMP.sheetNo, 0);	 //Debug Log ..
}


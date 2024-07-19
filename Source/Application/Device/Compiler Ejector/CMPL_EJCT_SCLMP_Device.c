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
static void  SetClamp_st2_OFFWAIT();//  Eject HomeSNR OFF待ち
static void  SetClamp_st3(); 		// T4設定
static void SetClamp_st4();			// SetClampHomeSNR OFF待ち
static void SetClamp_st5();			// T5タイマ設定
static void SetClamp_st6();			// T6タイマ設定
static void SetClamp_st7();			// SetClampHomeSNR ON待ち
static void SetClamp_st8(); 		// T7タイマ設定
static void SetClamp_st9(); 		// SetClamp動作完了



/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_SCLMP_StartAct
// Description   : Set Clamp動作設定_動作開始
// Parameter     : aAction (制御する動作)
//				 : aSheetNo (制御するSheetNo)
// Return        : ‐
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
// Description   : Set Clamp 制御
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_SCLMP_ControlThread()
{
	/* event sheet no , sheet condition check */
	switch( gCMPL_EJCT_SCLMP.action ) {
	case  CMPL_EJCT_SCLMP_st1 :  		// Eject HomeSNR状態チェック
		SetClamp_st1();
		break;
	case  CMPL_EJCT_SCLMP_st2_OFFWAIT :  // Eject HomeSNR状態チェック
		SetClamp_st2_OFFWAIT();
		break;
	case  CMPL_EJCT_SCLMP_st3 ://T4設定
		SetClamp_st3();
		break;
	case  CMPL_EJCT_SCLMP_st4 ://SetClampHomeSNR　OFF待ち
		SetClamp_st4();
		break;
	case  CMPL_EJCT_SCLMP_st5 : //T5タイマ設定
		SetClamp_st5();
		break;
	case  CMPL_EJCT_SCLMP_st6 ://T6タイマ設定
		SetClamp_st6();
		break;
	case  CMPL_EJCT_SCLMP_st7 ://SetClampHomeSNR　ON待ち
		SetClamp_st7();
		break;
	case  CMPL_EJCT_SCLMP_st8 ://T7タイマ設定
		SetClamp_st8();
		break;
	case  CMPL_EJCT_SCLMP_st9 : //SetClamp動作完了
		SetClamp_st9();
		break;

	default:
		break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_SCLMP_ReceiveEventTimerTimeUp
// Description   : イベントタイマのタイムアップ
// Parameter     : timerID (Timer ID)
//               : notUsed (Not Used)
// Return        : ‐
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
// Description   : イベントタイマーセット
// Parameter     : aTime (タイマー値 (×1[ms]))
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_SCLMP_SetEventTimer(US aTime, UL aEventType)
{
	gCMPL_EJCT_SCLMP.timerIndex = MSP_SetEventTimer(aTime, CMPL_EJCT_SCLMP_ReceiveEventTimerTimeUp, aEventType);	// Timer Set
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_SCLMP_CancelEventTimer
// Description   : イベントタイマーキャンセル
// Parameter     : aTime (タイマー値 (×1[ms]))
// Return        : ‐
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
// Description   : イジェクターホームセンサーの状態(Filterあり)を取得
// Parameter     : ‐
// Return        : センサーレベル(ON/OFF)
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
// Description   : SetClampHomeセンサーの状態(Filterあり)を取得
// Parameter     : ‐
// Return        : センサーレベル(ON/OFF)
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
// Description   : Ejecter Home Sensor OFF→ONを検知
// Parameter     : ‐
// Return        : ‐
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
// Description   : Ejecter Home Sensor ON→OFFを検知
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_SCLMP_EjecterHomeSensorOFF()
{
	if ( gCMPL_EJCT_SCLMP.action == CMPL_EJCT_SCLMP_st0 )
		return;

	switch (gCMPL_EJCT_SCLMP.action ) {
		case CMPL_EJCT_SCLMP_st2_OFFWAIT :
			CMPL_EJCT_SCLMP_CancelEventTimer();
			gCMPL_EJCT_SCLMP.action++;  // T4設定へ
			CMPL_EJCT_SCLMP_ControlThread();
			break;

		default :
			break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_SCLMP_SetClampHomeSensorON
// Description   : SetClamp Home Sensor OFF→ONを検知
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_SCLMP_SetClampHomeSensorON()
{
	if ( gCMPL_EJCT_SCLMP.action == CMPL_EJCT_SCLMP_st0 )
		return;

	switch(gCMPL_EJCT_SCLMP.action)
	{
		case   CMPL_EJCT_SCLMP_st7 :
			CMPL_EJCT_SCLMP_CancelEventTimer();
			gCMPL_EJCT_SCLMP.action = CMPL_EJCT_SCLMP_st8; //T7 Timer設定へ
			CMPL_EJCT_SCLMP_ControlThread();
		break;

		default :  break;
	}

}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_SCLMP_SetClampHomeSensorOFF
// Description   : SetClamp Home Sensor ON→OFFを検知
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_SCLMP_SetClampHomeSensorOFF ()
{
	if ( gCMPL_EJCT_SCLMP.action == CMPL_EJCT_SCLMP_st0 )
		return;

	switch ( gCMPL_EJCT_SCLMP.action ) {
	case CMPL_EJCT_SCLMP_st4 :
		CMPL_EJCT_SCLMP_CancelEventTimer();
		gCMPL_EJCT_SCLMP.action = CMPL_EJCT_SCLMP_st5; 	// T5 Timer設定へ
		CMPL_EJCT_SCLMP_ControlThread();
		break;

	default :
		break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_SCLMP_IsExecutiongDeviceAction
// Description   : Set Clampの動作状態を取得
// Parameter     : ‐
// Return        : true：動作中、false：停止中
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
// Description   :  Set Clamp リセット
// Parameter     : ‐
// Return        : ‐
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
// Description   : Set Clamp停止
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_SCLMP_DeviceStop()
{
	US aAction = gCMPL_EJCT_SCLMP.action & CMPL_EJCT_SCLMP_ACTION_MASK;
	UC aSheetNo = gCMPL_EJCT_SCLMP.sheetNo;

	gCMPL_EJCT_SCLMP.action = CMPL_EJCT_SCLMP_st0;
	gCMPL_EJCT_SCLMP.sheetNo = SHEETNO_NUM;

	CMPL_EJCT_SCLMP_CancelEventTimer();
	DD_DIO_SetOutput(DD_DO_SET_CLAMP_CLUTCH_ENB, OFF);

	if(CMPL_EJCT_SCLMP_st0 ==gCMPL_EJCT_SCLMP.compoAction)  // Component Stopから呼ばれるときは実行しない
	  CMPL_EJCT_InformedEjectMotorComp(aAction, aSheetNo);

	gCMPL_EJCT_SCLMP.compoAction = CMPL_EJCT_SCLMP_st0;
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_SCLMP_StopSheet
// Description   : 指定シートの動作停止
// Parameter     : aSheetNo (指定シート番号)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_SCLMP_StopSheet(UC aSheetNo)
{
	if( aSheetNo == SHEETNO_NUM) {												// 制御中の用紙のコンディションをチェック
		if( gCMPL_EJCT_SCLMP.sheetNo != SHEETNO_NUM ) {
			if( SRVC_CheckSheetConditionNormal(gCMPL_EJCT_SCLMP.sheetNo) == false ) {
				CMPL_EJCT_SCLMP_DeviceStop();
			}
			else {																// = Condition Normal
				/* No Action */
			}
		}
		else {																	// シートに対する制御をしていない
			/* No Action */
		}
	}
	else {																		// 指定シートの制御を停止する
		if( aSheetNo == gCMPL_EJCT_SCLMP.sheetNo ) {
			CMPL_EJCT_SCLMP_DeviceStop();
		}
		else {																	// 指定シートと制御シートが異なる
			/* No Action */
		}
	}
}
 ///////////////////////////////////////////////////////////////////
 // Function Name : SetClamp_st1_OFFWAIT
 // Description   : Eject HomeSNR　OFF待ち
 // Parameter	  : 無し
 // Return		  : 無し
 ///////////////////////////////////////////////////////////////////
 void SetClamp_st1()
 {
	 CMPL_EJCT_SCLMP_SetEventTimer(0, CMPL_EJCT_SCLMP_NORMAL_EVT);
	 
	 PrintEjectLog_Short(CMPL_EJCT_SCLMP_st1, gCMPL_EJCT_SCLMP.sheetNo, 0);	 //Debug Log ..
 }

///////////////////////////////////////////////////////////////////
// Function Name : SetClamp_st2
// Description	 : Eject HomeSNR　OFF待ち
// Parameter	 : 無し
// Return		 : 無し
///////////////////////////////////////////////////////////////////
 void SetClamp_st2_OFFWAIT()
{
	// EjectHomeSNR変化した時のCallback関数で処理されるので
  	// ここでは処理すること無い。
}

///////////////////////////////////////////////////////////////////
// Function Name : SetClamp_st3
// Description	 : T4
// Parameter	 : ‐
// Return		 : ‐
///////////////////////////////////////////////////////////////////
 void  SetClamp_st3()
{
	 CMPL_EJCT_SCLMP_SetEventTimer(SRVC_GetNvmValueUS(NVMID_SET_CLAMP_UP_START_TIME_4),
	 								     CMPL_EJCT_SCLMP_NORMAL_EVT);
	  PrintEjectLog_Short(CMPL_EJCT_SCLMP_st3, gCMPL_EJCT_SCLMP.sheetNo, 0);	 //Debug Log ..
}

///////////////////////////////////////////////////////////////////
// Function Name : SetClamp_st4
// Description	 : SetClampHomeSNR　OFF待ち
// Parameter	 : ‐
// Return		 : ‐
///////////////////////////////////////////////////////////////////
 void SetClamp_st4()
{
	//V3.4.0 仕様変更：SetClampHomeSNR=OFFの場合、T5セットへ
       if ( DD_DIO_GetInput(DD_DI_SET_CLAMP_HOME_SNR) == LOW) { // SetClampHomeSNR = OFF 

		gCMPL_EJCT_SCLMP.action = CMPL_EJCT_SCLMP_st5;	// T5 Timer設定へ

		 PrintEjectLog_Short(CMPL_EJCT_SCLMP_st4, gCMPL_EJCT_SCLMP.sheetNo, 0);	 //Debug Log ..		

		CMPL_EJCT_SCLMP_ControlThread();
		
       }else {  // SetClampHomeSNR = ON 
		   // Clutch ON (V3.3.94) : SetClampHomeSNR=ONの場合のみCLUTCH＝ONする
		   DD_DIO_SetOutput(DD_DO_SET_CLAMP_CLUTCH_ENB, ON);

		 CMPL_EJCT_SCLMP_SetEventTimer(CMPL_EJCT_SCLMP_FAILTIME_400,
									   CMPL_EJCT_SCLMP_NORMAL_EVT);
	 	 PrintEjectLog_Short(CMPL_EJCT_SCLMP_st4, gCMPL_EJCT_SCLMP.sheetNo, 0);	 //Debug Log ..
        }
}

///////////////////////////////////////////////////////////////////
// Function Name : SetClamp_st5
// Description	 : T5タイマ設定
// Parameter	 : ‐
// Return		 : ‐
///////////////////////////////////////////////////////////////////
 void SetClamp_st5()
{
	CMPL_EJCT_SCLMP_SetEventTimer(SRVC_GetNvmValueUC(NVMID_SET_CLAMP_UP_STOP_TIME_5),
									    CMPL_EJCT_SCLMP_NORMAL_EVT);
	 PrintEjectLog_Short(CMPL_EJCT_SCLMP_st5, gCMPL_EJCT_SCLMP.sheetNo, 0);	 //Debug Log ..
}

///////////////////////////////////////////////////////////////////
// Function Name : SetClamp_st6
// Description	 : T6タイマ設定
// Parameter	 : ‐
// Return		 : ‐
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
// Description	 : SetClampHomeSNR　ON待ち
// Parameter	 : ‐
// Return		 : ‐
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
// Description	 :  T7タイマ設定
// Parameter	 : ‐
// Return		 : ‐
///////////////////////////////////////////////////////////////////
void SetClamp_st8()
{
	CMPL_EJCT_SCLMP_SetEventTimer(SRVC_GetNvmValueUC(NVMID_SET_CLAMP_LOWER_STOP_TIME_7),
         							   CMPL_EJCT_SCLMP_NORMAL_EVT);
	 PrintEjectLog_Short(CMPL_EJCT_SCLMP_st8, gCMPL_EJCT_SCLMP.sheetNo, 0);	 //Debug Log ..
}

///////////////////////////////////////////////////////////////////
// Function Name : SetClamp_st9
// Description	 : SetClamp動作完了
// Parameter	 : ‐
// Return		 : ‐
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


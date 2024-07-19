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

	UC profile;		 // NewDD適用(Profile) //ECMPL_TMP_ProfileID profile;
	US targetIndex;
	US movePulse;
	UC direction;
	US startDelayTime;
	SS timerIndex;
	UC interruptIndex;
	UC sizePosAfterReturn;  //(CR-008)  Return動作後、SizePositioning動作実施する/しない
	UC offFailCheckCount;  //(V3.5.95) Home外し動作チェックCount。2回以上ならばHomeSNROffFail
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

// Component動作
// CMPL_TMP_COMPO_TAMPER_MOT = 0x0100,
static void  RT_Component_Mot_st1(); 	// Mot 駆動設定
static void  RT_Component_Mot_st2(); 	// 動作開始
static void  RT_Component_Mot_st3(); 	// 後励磁処理
static void  RT_Component_Mot_st4(); 	// Mot Off


// サイズポジショニング動作
// CMPL_TMP_SIZE_POS = 0x0200,
static void RT_SizePositioning_st1(); 		// Tamper HomeSNR　ONチェック
static void RT_SizePositioning_st2_CCW(); 	// Home外し動作設定
static void RT_SizePositioning_st3_CCW();	// CCW方向にモータ駆動開始
static void RT_SizePositioning_st4_CCW(); 	// Home外し動作完了
static void RT_SizePositioning_st2_CW(); 	// Home動作設定
static void RT_SizePositioning_st3_CW(); 	// CW方向にモータ駆動開始
static void RT_SizePositioning_st4_CW(); 	// StepUp中、割込み受信

static void RT_SizePositioning_st4_2_CW(); 	// StepUp中、割込み受信 から StepDown完了

static void RT_SizePositioning_st5_CW(); 	// StepUp完了受信
static void RT_SizePositioning_st6_CW(); 	// 安定区間で割込み受信
static void RT_SizePositioning_st7_CW(); 	// Home動作完了
static void RT_SizePositioning_st8();	 	// SizePositioning開始
static void RT_SizePositioning_st9();	 	// モータ駆動開始
static void RT_SizePositioning_st10();  	// SizePositionへ移動完了


// ノーマルタンピング動作
// CMPL_TMP_NORMAL_TAMPING = 0x0300,

// イジェクトタンピング動作(SetLastTamping)
// CMPL_TMP_EJECT_TAMPING = 0x0400,

// オフセット動作
// CMPL_TMP_OFFSET = 0x0500,
static void RT_Offset_st1(); 	// 開始Timing調整
static void RT_Offset_st2(); 	// モータ駆動設定
static void RT_Offset_st3(); 	// モータ駆動開始
static void RT_Offset_st4(); 	// 安定時間タイマ設定
static void RT_Offset_st5(); 	// 次動作待ち

// リターン動作
// CMPL_TMP_RETURN = 0x0600,
static void RT_Return_st1(); 	// 駆動方向判断
static void RT_Return_st2_CW(); // 動作開始Trigger待ち(LeftOffset)
static void RT_Return_st3_CW(); // タイマ設定(LeftOffset)
static void RT_Return_st4_CW();	// モータ駆動設定(LeftOffset)
static void RT_Return_st2_CCW();// 動作開始Trigger待ち(RightOffset)
static void RT_Return_st3_CCW();// タイマ設定(RightOffset)
static void RT_Return_st4_CCW();// モータ駆動設定(RightOffset)
static void RT_Return_st5();  	// モータ駆動開始
static void RT_Return_st6();  	// タイマ設定
static void RT_Return_st7();  	// 次動作待ち
static void RT_Return_st8();	// SizePos開始(CR-008) 


// オープン動作(Nigasi)
// CMPL_TMP_OPEN = 0x0700,
static void RT_Open_st1(); 	// モータ駆動設定
static void RT_Open_st2(); 	// モータ駆動開始
static void RT_Open_st3(); 	// タイマ設定
static void RT_Open_st4(); 	// 次動作待ち
static void RT_Open_st5();  // 次動作待ち


static const TMP_RIGHT_CallFunc ControlThread_RT_COMPO_MOT[] = {
	{NULL},
	{RT_Component_Mot_st1},	// Mot 駆動設定
	{RT_Component_Mot_st2},	// 動作開始
	{RT_Component_Mot_st3},	// 後励磁処理
	{RT_Component_Mot_st4},	// Mot Off
};

// サイズポジショニング動作
static const TMP_RIGHT_CallFunc ControlThread_RT_SIZEPOS[] = {
	{NULL},
	{RT_SizePositioning_st1}, 		// Tamper HomeSNR　ONチェック
	{RT_SizePositioning_st2_CCW}, 	// Home外し動作設定
	{RT_SizePositioning_st3_CCW}, 	// CCW方向にモータ駆動開始
	{RT_SizePositioning_st4_CCW}, 	// Home外し動作完了
	{RT_SizePositioning_st2_CW}, 	// Home動作設定
	{RT_SizePositioning_st3_CW}, 	// CW方向にモータ駆動開始
	{RT_SizePositioning_st4_CW}, 	// StepUp中、割込み受信
	{RT_SizePositioning_st4_2_CW}, 	// StepUp中、割込み受信 から StepDown完了
	{RT_SizePositioning_st5_CW}, 	// StepUp完了受信
	{RT_SizePositioning_st6_CW}, 	// 安定区間で割込み受信
	{RT_SizePositioning_st7_CW}, 	// Home動作完了

	{RT_SizePositioning_st8},		// SizePositioning開始
	{RT_SizePositioning_st9},		// モータ駆動開始
	{RT_SizePositioning_st10}, 		// SizePositionへ移動完了

};

// ノーマルタンピング動作
// イジェクトタンピング動作(SetLastTamping)

// オフセット動作
static const TMP_RIGHT_CallFunc ControlThread_RT_OFFSET[] = {
	{NULL},
	{RT_Offset_st1}, 	// 開始Timing調整
	{RT_Offset_st2}, 	// モータ駆動設定
	{RT_Offset_st3}, 	// モータ駆動開始
	{RT_Offset_st4}, 	// 安定時間タイマ設定
	{RT_Offset_st5}, 	// 次動作待ち
};

// リターン動作
static const TMP_RIGHT_CallFunc  ControlThread_RT_RETURN[] = {
	{NULL},
	{RT_Return_st1}, 		// 駆動方向判断
	{RT_Return_st2_CW}, 	// 動作開始Trigger待ち(LeftOffset)
	{RT_Return_st3_CW}, 	// タイマ設定(LeftOffset)
	{RT_Return_st4_CW},   	// モータ駆動設定(LeftOffset)
	{RT_Return_st2_CCW},  	// 動作開始Trigger待ち(RightOffset)
	{RT_Return_st3_CCW},  	// タイマ設定(RightOffset)
	{RT_Return_st4_CCW},  	// モータ駆動設定(RightOffset)
	{RT_Return_st5}, 		// モータ駆動開始
	{RT_Return_st6},  		// タイマ設定
	{RT_Return_st7},  		// 次動作待ち
	{RT_Return_st8},		// SizePos開始(CR-008) 
};

// オープン動作(Nigasi)
static const TMP_RIGHT_CallFunc  ControlThread_RT_OPEN[] = {
	{NULL},
	{RT_Open_st1}, //モータ駆動設定
	{RT_Open_st2}, //モータ駆動開始
	{RT_Open_st3}, //タイマ設定
	{RT_Open_st4}, //次動作待ち
	{RT_Open_st5}, //次動作待ち
};

/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_RIGHT_StartAct
// Description   : タンパー動作設定_動作開始
// Parameter     : aAction (制御する動作)
//				 : aSheetNo (制御するSheetNo)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_RIGHT_StartAct(ECMPL_TMP_Action aAction, UC aSheetNo)
{

	switch ( aAction ) {

	// Component 動作 //////////////////////////////////////////////////////////////////////////////
	// RIGH TTamperを外側方向(CW)に低速(643PPS)で移動
	case CMPL_TMP_COMPO_RIGHT_CW_LOW :
		gCMPL_TMP_RIGHT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_LOW;
		gCMPL_TMP_RIGHT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_LOW;

		gCMPL_TMP_RIGHT.direction = CMPL_TMP_COMM_OUTSIDE;
		gCMPL_TMP_RIGHT.compoAction = aAction;
		gCMPL_TMP_RIGHT.action = CMPL_TMP_COMPO_TAMPER_MOT_st1;
		break;

	 // RIGHT Tamperを外側方向(CW)に中速(800PPS)で移動
	case CMPL_TMP_COMPO_RIGHT_CW_MID :
		gCMPL_TMP_RIGHT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_MID;
		gCMPL_TMP_RIGHT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_MID;

		gCMPL_TMP_RIGHT.direction = CMPL_TMP_COMM_OUTSIDE;
		gCMPL_TMP_RIGHT.compoAction = aAction;
		gCMPL_TMP_RIGHT.action = CMPL_TMP_COMPO_TAMPER_MOT_st1;
		break;

	// RIGHT Tamperを外側方向(CW)に高速(1337PPS)で移動
	case CMPL_TMP_COMPO_RIGHT_CW_HI :
		gCMPL_TMP_RIGHT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_HI;
		gCMPL_TMP_RIGHT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_HI;

		gCMPL_TMP_RIGHT.direction = CMPL_TMP_COMM_OUTSIDE;
		gCMPL_TMP_RIGHT.compoAction = aAction;
		gCMPL_TMP_RIGHT.action = CMPL_TMP_COMPO_TAMPER_MOT_st1;
		break;

	  // RIGHT Tamperを内側方向(CCW)に低速(643PPS)で移動
	case CMPL_TMP_COMPO_RIGHT_CCW_LOW :
		gCMPL_TMP_RIGHT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_LOW;
		gCMPL_TMP_RIGHT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_LOW;

		gCMPL_TMP_RIGHT.direction = CMPL_TMP_COMM_INSIDE;
		gCMPL_TMP_RIGHT.compoAction = aAction;
		gCMPL_TMP_RIGHT.action = CMPL_TMP_COMPO_TAMPER_MOT_st1;
		break;

	// RIGHT Tamperを内側方向(CCW)に中速(800PPS)で移動
	case CMPL_TMP_COMPO_RIGHT_CCW_MID :
		gCMPL_TMP_RIGHT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_MID;
		gCMPL_TMP_RIGHT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_MID;

		gCMPL_TMP_RIGHT.direction = CMPL_TMP_COMM_INSIDE;
		gCMPL_TMP_RIGHT.compoAction = aAction;
		gCMPL_TMP_RIGHT.action = CMPL_TMP_COMPO_TAMPER_MOT_st1;
		break;

	// RIGHT Tamperを内側方向(CCW)に高速(1337PPS)で移動
	case CMPL_TMP_COMPO_RIGHT_CCW_HI :
		gCMPL_TMP_RIGHT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_HI;
		gCMPL_TMP_RIGHT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_HI;

		gCMPL_TMP_RIGHT.direction = CMPL_TMP_COMM_INSIDE;
		gCMPL_TMP_RIGHT.compoAction = aAction;
		gCMPL_TMP_RIGHT.action = CMPL_TMP_COMPO_TAMPER_MOT_st1;
		break;
	// Component 動作 //////////////////////////////////////////////////////////////////////////////

	// サイズポジショニング動作
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

	// ノーマルタンピング動作
	case CMPL_TMP_NORMAL_TAMPING : // = 0x0300,
	// イジェクトタンピング動作(SetLastTamping)
	case CMPL_TMP_EJECT_TAMPING :  //= 0x0400,

		if (gCMPL_TMP_RIGHT.action == CMPL_TMP_st0) { //停止中

			DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_ENB, ON);
			DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, LOW); //弱電流設定
			
			gCMPL_TMP_RIGHT.action = CMPL_TMP_st0;
			gCMPL_TMP_RIGHT.sheetNo = SHEETNO_NUM;
			
		} else { //動作中
		
		}
		CMPL_TMP_InformedRightTamperComplete(aAction, aSheetNo);
		return;   //0502 
		//break;

	// オフセット動作
	case CMPL_TMP_OFFSET :  //= 0x0500,	
		gCMPL_TMP_RIGHT.movePulse = CMPL_TMP_RIGHT_GetOffsetPosition(aSheetNo);
		gCMPL_TMP_RIGHT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_LOW; // 通常 低速 Profile
		gCMPL_TMP_RIGHT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_LOW;

		gCMPL_TMP_RIGHT.direction= (UC)CMPL_TMP_RIGHT_GetOffsetDirection(aSheetNo);
		gCMPL_TMP_RIGHT.action = CMPL_TMP_OFFSET_st1;
		PrintTamperLog_Short(CMPL_TMP_OFFSET, gCMPL_TMP_RIGHT.movePulse, aSheetNo);
		break;

	// リターン動作
	case CMPL_TMP_RIGHT_RETURN : // = 0x0700,
		gCMPL_TMP_RIGHT.movePulse = CMPL_TMP_RIGHT_GetReturnPosition(aSheetNo);
		gCMPL_TMP_RIGHT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_HI; // 通常 高速 Profile
	
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

	// オープン動作(Nigasi)
	case CMPL_TMP_OPEN :  // = 0x0800,		
		gCMPL_TMP_RIGHT.profile = CMPL_TMP_PRF_ID_NORMAL_HI; // 通常 高速 Profile
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
// Description   : RIGHT タンパー制御
// Parameter     : ‐
// Return        : ‐
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

	// サイズポジショニング動作
	case CMPL_TMP_SIZE_POS :  // = 0x0200,  Size Positioning
		ControlThread_RT_SIZEPOS[aStatus].mpHandler(); break;

	// オフセット動作
	case CMPL_TMP_OFFSET :  //= 0x0500,
		ControlThread_RT_OFFSET[aStatus].mpHandler(); break;

	// リターン動作
	case CMPL_TMP_RIGHT_RETURN : // = 0x0700,
		ControlThread_RT_RETURN[aStatus].mpHandler(); break;

	// オープン動作(Nigasi)
	case CMPL_TMP_OPEN :  // = 0x0800,
		ControlThread_RT_OPEN[aStatus].mpHandler(); break;

	default : 							break;
	}
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_RIGHT_ReceiveEventTimerTimeUp
// Description   : イベントタイマのタイムアップ
// Parameter     : timerID (Timer ID)
//               : notUsed (Not Used)
// Return        : ‐
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
		case CMPL_TMP_SIZE_POS_st2_CCW :  // 20ms Timeout 前励磁処理
			gCMPL_TMP_RIGHT.action = CMPL_TMP_SIZE_POS_st3_CCW;
			CMPL_TMP_RIGHT_ControlThread();
			break;
		case CMPL_TMP_SIZE_POS_st4_CCW : // Home 外し動作完了 -> 50ms Timeout
			gCMPL_TMP_RIGHT.action = CMPL_TMP_SIZE_POS_st2_CW; // Home 動作設定
			CMPL_TMP_RIGHT_ControlThread();
			break;
		case CMPL_TMP_SIZE_POS_st2_CW : // 20ms Timeout 前励磁処理
			gCMPL_TMP_RIGHT.action = CMPL_TMP_SIZE_POS_st3_CW;
			CMPL_TMP_RIGHT_ControlThread();
			break;
		case CMPL_TMP_SIZE_POS_st4_2_CW : //50ms Timeout
			gCMPL_TMP_RIGHT.action = CMPL_TMP_SIZE_POS_st2_CCW; //Home外し動作へ
			CMPL_TMP_RIGHT_ControlThread();
			break;
		case CMPL_TMP_SIZE_POS_st7_CW : //50ms Timeout
			gCMPL_TMP_RIGHT.action = CMPL_TMP_SIZE_POS_st8; //Home動作完了 -> Size Positioning Start ..
			CMPL_TMP_RIGHT_ControlThread();
			break;
		case CMPL_TMP_SIZE_POS_st8: //20ms Timeout後励磁処理
			gCMPL_TMP_RIGHT.action++;
			CMPL_TMP_RIGHT_ControlThread();
			break;

		// Offset
		case CMPL_TMP_OFFSET_st1 : 		// 開始Timing調整
		case CMPL_TMP_OFFSET_st2 : 		// モータ駆動設定
		case CMPL_TMP_OFFSET_st4 : 		// 安定時間タイマ設定
			gCMPL_TMP_RIGHT.action++;
			CMPL_TMP_RIGHT_ControlThread();
			break;

		// Return
		case CMPL_TMP_RIGHT_RETURN_st3_CW : 	// 開始Timing調整
		case CMPL_TMP_RIGHT_RETURN_st3_CCW : 	// 開始Timing調整
		case CMPL_TMP_RIGHT_RETURN_st6 : 		// タイマ設定
		case CMPL_TMP_RIGHT_RETURN_st7 :            // (CR-008)
			gCMPL_TMP_RIGHT.action++;
			CMPL_TMP_RIGHT_ControlThread();
			break;
		case CMPL_TMP_RIGHT_RETURN_st4_CW :    	// モータ駆動設定
		case CMPL_TMP_RIGHT_RETURN_st4_CCW : 	// モータ駆動設定
			gCMPL_TMP_RIGHT.action  = CMPL_TMP_RIGHT_RETURN_st5;
			CMPL_TMP_RIGHT_ControlThread();
			break;

		// Open
		case CMPL_TMP_OPEN_st1 : 		// モータ駆動設定
		case CMPL_TMP_OPEN_st4 : 		// タイマ設定
			gCMPL_TMP_RIGHT.action++;
			CMPL_TMP_RIGHT_ControlThread();
			break;
	
		default:
			break;
		}
	} else {  //Fail Event 

		switch ( gCMPL_TMP_RIGHT.action ) {

		case CMPL_TMP_SIZE_POS_st3_CW : // Fail  Timeout
		case CMPL_TMP_SIZE_POS_st5_CW : // Fail  Timeout(V3.5.93Tamper不具合(Fail通知されない))
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
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_RIGHT_ReceiveSteppingMotorEvent(SS aResult)
{
	switch ( gCMPL_TMP_RIGHT.action ) {

	// Component
	case CMPL_TMP_COMPO_TAMPER_MOT_st2 :	// StepDown Wait ..
		gCMPL_TMP_RIGHT.action = CMPL_TMP_COMPO_TAMPER_MOT_st3;
		CMPL_TMP_RIGHT_ControlThread();
		break;

	case CMPL_TMP_SIZE_POS_st3_CCW :		// Home 外し動作完了 Event
		if ( DD_DIO_GetInput(DD_DI_RIGHT_TAMPER_HOME_SNR) == HIGH ) {  // Tamper HomeSNR Off Fail
			gCMPL_TMP_RIGHT_FaultStat = FAULT_OCCUR;
			CMPL_TMP_InformedRightTamperHomeSensorOffFail(gCMPL_TMP_RIGHT.sheetNo);
		} else {
			gCMPL_TMP_RIGHT.action = CMPL_TMP_SIZE_POS_st4_CCW;
			CMPL_TMP_RIGHT_ControlThread();
		}
		break;

	case CMPL_TMP_SIZE_POS_st3_CW : 		// Home動作完了 Event
		// 定速中、ON割込み
		gCMPL_TMP_RIGHT.action = CMPL_TMP_SIZE_POS_st5_CW;
		break;

	case CMPL_TMP_SIZE_POS_st4_CW : 	// StepDown完了 Event
		gCMPL_TMP_RIGHT.action = CMPL_TMP_SIZE_POS_st4_2_CW;
		CMPL_TMP_RIGHT_ControlThread();
		break;

	case CMPL_TMP_SIZE_POS_st6_CW :
		gCMPL_TMP_RIGHT.action = CMPL_TMP_SIZE_POS_st7_CW;
		CMPL_TMP_RIGHT_ControlThread();
		break;

	case CMPL_TMP_SIZE_POS_st9 : 	// SizePositioning Pulse A  移動完了
		gCMPL_TMP_RIGHT.action = CMPL_TMP_SIZE_POS_st10;
		CMPL_TMP_RIGHT_ControlThread();
		break;

	// Offset
	case CMPL_TMP_OFFSET_st3 : 		// モータ駆動開始
		gCMPL_TMP_RIGHT.action++;
		CMPL_TMP_RIGHT_ControlThread();
		break;

	// Return
	case CMPL_TMP_RIGHT_RETURN_st5 : // モータ駆動開始
		gCMPL_TMP_RIGHT.action++;
		CMPL_TMP_RIGHT_ControlThread();
		break;
	// Open
	case CMPL_TMP_OPEN_st3 : 		// モータ駆動開始
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
	case CMPL_TMP_SIZE_POS_st3_CW :  // StepUp中、割込み受信
		CMPL_TMP_RIGHT_CancelEventTimer();	// Fail Timer cancel ..
		gCMPL_TMP_RIGHT.action = CMPL_TMP_SIZE_POS_st4_CW;
		CMPL_TMP_RIGHT_ControlThread();
		break;

	case CMPL_TMP_SIZE_POS_st5_CW :  // 安定区間で割込み受信
		CMPL_TMP_RIGHT_CancelEventTimer();	// Fail Timer cancel ..
		gCMPL_TMP_RIGHT.action = CMPL_TMP_SIZE_POS_st6_CW;
		CMPL_TMP_RIGHT_ControlThread();
		break;

	case CMPL_TMP_OPEN_st2 :   // 割込み受信
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
// Description 		:  Eject 動作開始通知
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
// Description 		:  EjectMot減速開始通知
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
// Description   : イベントタイマーセット
// Parameter     : aTime (タイマー値 (×1[ms]))
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_RIGHT_SetEventTimer(US aTime, UL aEventType)
{
	gCMPL_TMP_RIGHT.timerIndex = MSP_SetEventTimer(aTime, CMPL_TMP_RIGHT_ReceiveEventTimerTimeUp, aEventType);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_RIGHT_CancelEventTimer
// Description   : イベントタイマーセット
// Parameter     : aTime (タイマー値 (×1[ms]))
// Return        : ‐
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
// Description   : リアタンパーの動作状態を取得
// Parameter     : ‐
// Return        : true：動作中、false：停止中
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
// Description   : タンパー リセット
// Parameter     : ‐
// Return        : ‐
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
// Description   : タンパー停止
// Parameter     : ‐
// Return        : ‐
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
// Description   : サイズポジション位置を取得
// Parameter     : aSheetNo (対象シートナンバー)
// Return        : サイズポジション位置
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
			aPosition = 0;  // Home位置(Staple位置)
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
// Description   : Offset位置取得
// Parameter     : aSheetNo (対象シートナンバー)
// Return        : Offset 位置
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
// Description   : Offset方向 取得
// Parameter     : aSheetNo (対象シートナンバー)
// Return        : Offset 方向
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
// Description   : Return移動量 取得
// Parameter     : aSheetNo (対象シートナンバー)
// Return        : Return 移動量
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
			 //CR-010 - 10Pulse 移動
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
// Description   : Return方向 取得
// Parameter     : aSheetNo (対象シートナンバー)
// Return        : Return 方向
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
// Description   : Mot 駆動設定
// Parameter     : ‐
// Return        : ‐
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
// Description   : 動作開始
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_Component_Mot_st2()
{
	DD_STM_SetProfileID(DD_STM_ID_RIGHT_TAMPER,	gCMPL_TMP_RIGHT.profile);
	DD_STM_MoveBySetPulse(DD_STM_ID_RIGHT_TAMPER,  gCMPL_TMP_RIGHT.targetIndex,
						     CMPL_TMP_COMPO_TOTAL_PULSE, CMPL_TMP_RIGHT_ReceiveSteppingMotorEvent);
}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Component_Mot_st3
// Description	 //後励磁処理
// Parameter	 : ‐
// Return		 : ‐
///////////////////////////////////////////////////////////////////
void RT_Component_Mot_st3()
{
  	CMPL_TMP_RIGHT_SetEventTimer(CMPL_TMP_COMM_MOTOR_STOP_TIME_20, CMPL_TMP_RIGHT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Component_Mot_st4
// Description   ://Mot Off
// Parameter     : ‐
// Return        : ‐
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


// サイズポジショニング動作
// CMPL_TMP_SIZE_POS = 0x0200,
///////////////////////////////////////////////////////////////////
// Function Name : RT_SizePositioning_st1
// Description   : Tamper HomeSNR　ONチェック
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_SizePositioning_st1()
{
	// 1. Tamper Home SNR Level Check
	if ( DD_DIO_GetInput(DD_DI_RIGHT_TAMPER_HOME_SNR) == HIGH )  // HIGH=
	   gCMPL_TMP_RIGHT.action = CMPL_TMP_SIZE_POS_st2_CCW;  // Home外し動作設定
	else
	   gCMPL_TMP_RIGHT.action = CMPL_TMP_SIZE_POS_st2_CW;  	// Home動作設定

	CMPL_TMP_RIGHT_ControlThread();
}

///////////////////////////////////////////////////////////////////
// Function Name : RT_SizePositioning_st2_CCW
// Description   : Home外し動作設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_SizePositioning_st2_CCW()
{
	// Home外し動作チェックCount。2回以上ならばHomeSNROffFail
	if ( ++gCMPL_TMP_RIGHT.offFailCheckCount > 1 ) {

		CMPL_TMP_InformedRightTamperHomeSensorOffFail(gCMPL_TMP_RIGHT.sheetNo);
		PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st2_CCW, 
							gCMPL_TMP_RIGHT.offFailCheckCount, gCMPL_TMP_RIGHT.offFailCheckCount);
	}	

	// 内方向(CCW）で強電流設定
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_DIR, CMPL_TMP_COMM_INSIDE);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, HIGH);

	//RightSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st2_CCW, CMPL_TMP_SIZE_POS_st2_CCW, 0x00);

	// 20ms Timer設定
	//CMPL_TMP_RIGHT_SetEventTimer(CMPL_TMP_COMM_MOTOR_ENABLE_TIME_20,
	CMPL_TMP_RIGHT_SetEventTimer(25, //0502
									  CMPL_TMP_RIGHT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : RT_SizePositioning_st3_CCW
// Description   : CCW方向にモータ駆動開始
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_SizePositioning_st3_CCW()
{
	DD_STM_SetProfileID(DD_STM_ID_RIGHT_TAMPER, DD_STM_PROF_ID_TAMPER_INITIALIZE);

	//RightSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st3_CCW, CMPL_TMP_SIZE_POS_st3_CCW, 0x00);
	// INITIALIZE Profileで 総120(StepUp+Const) + 30(StepDown)パルス移動
    DD_STM_MoveBySetPulse(DD_STM_ID_RIGHT_TAMPER, CMPL_TMP_PROF_TARGET_STEP_NUM_INIT,
      						 CMPL_TMP_SIZE_POSITION_MOVE_PUSE_150, CMPL_TMP_RIGHT_ReceiveSteppingMotorEvent );
}

///////////////////////////////////////////////////////////////////
// Function Name : RT_SizePositioning_st4_CCW
// Description   : Home外し動作完了
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_SizePositioning_st4_CCW()
{
	// 50ms Timer設定
	CMPL_TMP_RIGHT_SetEventTimer(CMPL_TMP_COMM_SWITCH_TIME_50,	CMPL_TMP_RIGHT_NORMAL_EVT);
	//RightSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st4_CCW, CMPL_TMP_SIZE_POS_st4_CCW, 0x00);

}

///////////////////////////////////////////////////////////////////
// Function Name : RT_SizePositioning_st2_CW
// Description   : Home動作設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_SizePositioning_st2_CW()
{
	// 外方向(CW）で強電流設定
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_DIR, CMPL_TMP_COMM_OUTSIDE);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, HIGH);

	//V3.3.99  Tamper T/B
	if ( gCMPL_TMP_RIGHT.sizePosAfterReturn == true ) {

		// 20ms Timer設定
		CMPL_TMP_RIGHT_SetEventTimer(CMPL_TMP_COMM_MOTOR_ENABLE_TIME_20,
										  CMPL_TMP_RIGHT_NORMAL_EVT);
	} else {
		//V3.3.93  騒音対策　20ms→200msに変更
		CMPL_TMP_RIGHT_SetEventTimer(200,	CMPL_TMP_RIGHT_NORMAL_EVT);
	}
	
	//RightSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st2_CW, CMPL_TMP_SIZE_POS_st2_CW, 0x00);
}

///////////////////////////////////////////////////////////////////
// Function Name : RT_SizePositioning_st3_CW
// Description   : CW方向にモータ駆動開始
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_SizePositioning_st3_CW()
{
  	// Fail Timer設定
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
// Description   : StepUp中、割込み受信-> SlowDown-> 50ms Timerset->Home外し動作
// Parameter     : ‐
// Return        : ‐
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
// Description   : /StepUp中、割込み受信 から StepDown完了
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////

void RT_SizePositioning_st4_2_CW() // StepUp中、割込み受信 から StepDown完了
{
	// 50ms Timer設定
	CMPL_TMP_RIGHT_SetEventTimer(CMPL_TMP_COMM_SWITCH_TIME_50,	CMPL_TMP_RIGHT_NORMAL_EVT);
	//RightSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st4_2_CW, CMPL_TMP_SIZE_POS_st4_2_CW, 0x00);

}

///////////////////////////////////////////////////////////////////
// Function Name : RT_SizePositioning_st5_CW
// Description   : //StepUp完了受信
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_SizePositioning_st5_CW()
{
	// Nothing to do... No call
}

///////////////////////////////////////////////////////////////////
// Function Name : RT_SizePositioning_st6_CW
// Description   : 安定区間で割込み受信
// Parameter     : ‐
// Return        : ‐
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
// Description   : Home動作完了
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_SizePositioning_st7_CW()
{
	// 50ms Timer設定
	CMPL_TMP_RIGHT_SetEventTimer(CMPL_TMP_COMM_SWITCH_TIME_50,	CMPL_TMP_RIGHT_NORMAL_EVT);
	//RightSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st7_CW, CMPL_TMP_SIZE_POS_st7_CW, 0x00);

	gCMPL_TMP_RIGHT.offFailCheckCount = 0; //Home外し動作チェックCount Clear(V3.5.95)
}

///////////////////////////////////////////////////////////////////
// Function Name : RT_SizePositioning_st8
// Description   : SizePositioning開始
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_SizePositioning_st8()
{
	UC  sheetNo = gCMPL_TMP_RIGHT.sheetNo;
	
	if ( gCMPL_TMP_RIGHT.movePulse == 0 ) { // 　移動量無し→終了

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
		// 20ms Timer設定
		CMPL_TMP_RIGHT_SetEventTimer(CMPL_TMP_COMM_MOTOR_ENABLE_TIME_20, CMPL_TMP_RIGHT_NORMAL_EVT);

	} else {	
		//V3.3.93  騒音対策　20ms→200msに変更
		CMPL_TMP_RIGHT_SetEventTimer(200,	CMPL_TMP_RIGHT_NORMAL_EVT);
	}	
}

///////////////////////////////////////////////////////////////////
// Function Name : RT_SizePositioning_st9
// Description   : モータ駆動開始
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_SizePositioning_st9()
{
	// A-Pulse移動
	DD_STM_MoveBySetPulse(DD_STM_ID_RIGHT_TAMPER, CMPL_TMP_PROF_TARGET_STEP_NUM_INIT,
	  						  gCMPL_TMP_RIGHT.movePulse, CMPL_TMP_RIGHT_ReceiveSteppingMotorEvent );
	//RightSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st9, CMPL_TMP_SIZE_POS_st9, 0x00);

}

///////////////////////////////////////////////////////////////////
// Function Name : RT_SizePositioning_st10
// Description   : SizePositionへ移動完了
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_SizePositioning_st10()
{
	UC    sheetNo = gCMPL_TMP_RIGHT.sheetNo;
	
	// Right Tamper HomeSNR OFF Fail Check (33 Pulse 　以上 & HOME SNR=ON)
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


// オフセット動作
//CMPL_TMP_OFFSET = 0x0500,
///////////////////////////////////////////////////////////////////
// Function Name : RT_Offset_st1
// Description   : ; //開始Timing調整
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_Offset_st1()
{
	UC  sheetNo=gCMPL_TMP_RIGHT.sheetNo;
	
	// 　移動量=0の場合、終了
	if( gCMPL_TMP_RIGHT.movePulse == 0 ) { //

		gCMPL_TMP_RIGHT.action = CMPL_TMP_st0;
		gCMPL_TMP_RIGHT.sheetNo = SHEETNO_NUM;
	
   		CMPL_TMP_InformedRightTamperComplete(CMPL_TMP_OFFSET,	sheetNo);
		return;
	}

	//Tamper駆動方向によって制御を変える
	if( gCMPL_TMP_RIGHT.direction == CMPL_TMP_COMM_INSIDE ) {  // 内（CCW)方向
		// 20msTimerSet
		CMPL_TMP_RIGHT_SetEventTimer( CMPL_TMP_COMM_START_DELAY_20,
										CMPL_TMP_RIGHT_NORMAL_EVT);
	} else {   //外(CW)方向
		gCMPL_TMP_RIGHT.action = CMPL_TMP_OFFSET_st2;
		CMPL_TMP_RIGHT_ControlThread();
	}

}

///////////////////////////////////////////////////////////////////
// Function Name :RT_Offset_st2
// Description   : ; //モータ駆動設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_Offset_st2()
{
	//1. 強電流で OFFSET方向設定
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_DIR, gCMPL_TMP_RIGHT.direction);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, HIGH);


	//2. 10msTimerSet
	CMPL_TMP_RIGHT_SetEventTimer( CMPL_TMP_COMM_SWITCH_TIME_10,
					 				CMPL_TMP_RIGHT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Offset_st3
// Description   ; //モータ駆動開始
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_Offset_st3()
{
	//低速Profile
	DD_STM_SetProfileID(DD_STM_ID_RIGHT_TAMPER, gCMPL_TMP_RIGHT.profile);

	DD_STM_MoveBySetPulse(DD_STM_ID_RIGHT_TAMPER, gCMPL_TMP_RIGHT.targetIndex,
							gCMPL_TMP_RIGHT.movePulse, CMPL_TMP_RIGHT_ReceiveSteppingMotorEvent);

}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Offset_st4
// Description   : ; //安定時間タイマ設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_Offset_st4()
{
	//Tamper駆動方向によって制御を変える
	if(gCMPL_TMP_RIGHT.direction == CMPL_TMP_COMM_INSIDE) {  // 内（CCW)方向
		// 10msTimerSet
		CMPL_TMP_RIGHT_SetEventTimer( CMPL_TMP_COMM_MOTOR_STOP_TIME_10,
										CMPL_TMP_RIGHT_NORMAL_EVT);
	} else {  //外(CW)方向
		// 20msTimerSet
		CMPL_TMP_RIGHT_SetEventTimer( CMPL_TMP_COMM_MOTOR_STOP_TIME_20,
										CMPL_TMP_RIGHT_NORMAL_EVT);
	}

}

///////////////////////////////////////////////////////////////////
// Function Name :RT_Offset_st5
// Description   : //次動作待ち
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_Offset_st5()
{
	UC  sheetNo = gCMPL_TMP_RIGHT.sheetNo;
	
	//弱電流設定
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, LOW);

	gCMPL_TMP_RIGHT.action = CMPL_TMP_st0;
	gCMPL_TMP_RIGHT.sheetNo = SHEETNO_NUM;

	CMPL_TMP_InformedRightTamperComplete(CMPL_TMP_OFFSET,	 sheetNo);
	PrintTamperLog_Short(CMPL_TMP_OFFSET_st5, 0xFFFF,0xFF);
}

// リターン動作
//CMPL_TMP_RIGHT_RETURN = 0x0700,
///////////////////////////////////////////////////////////////////
// Function Name : RT_Return_st1
// Description   : 駆動方向判断
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_Return_st1()
{
	UC   sheetNo = gCMPL_TMP_RIGHT.sheetNo;
		
	// 　移動量=0の場合、終了
	if( gCMPL_TMP_RIGHT.movePulse == 0 ) { //

		gCMPL_TMP_RIGHT.action = CMPL_TMP_st0;
		gCMPL_TMP_RIGHT.sheetNo = SHEETNO_NUM;
		gCMPL_TMP_RIGHT.sizePosAfterReturn = false;
	
		CMPL_TMP_InformedRightTamperComplete(CMPL_TMP_RIGHT_RETURN, sheetNo);
		return;
	}

	//モータ駆動方向判断 :  内->st2_CCW,  外->st2_CW
	if( gCMPL_TMP_RIGHT.direction == CMPL_TMP_COMM_OUTSIDE ) //
		gCMPL_TMP_RIGHT.action = CMPL_TMP_RIGHT_RETURN_st2_CW;
	else  //CMPL_TMP_COMM_INSIDE
		gCMPL_TMP_RIGHT.action = CMPL_TMP_RIGHT_RETURN_st2_CCW;
	CMPL_TMP_RIGHT_ControlThread();
}


///////////////////////////////////////////////////////////////////
// Function Name : RT_Return_st2_CW
// Description   : ; 動作開始Trigger待ち(LeftOffset)
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_Return_st2_CW()
{
	//Eject 動作開始
	if( CMPL_TMP_IsEjectStart() == true ) {
		gCMPL_TMP_RIGHT.action = CMPL_TMP_RIGHT_RETURN_st3_CW;
		CMPL_TMP_RIGHT_ControlThread();
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Return_st3_CW
// Description   ; //タイマ設定(LeftOffset)
// Parameter     : ‐
// Return        : ‐
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
// Description   :;   //モータ駆動設定(LeftOffset)
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_Return_st4_CW()
{
	//1.弱電流で 外(CW)方向設定
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_DIR, CMPL_TMP_COMM_OUTSIDE);
	//Motor脱調対策(CR-008) TimerSet前、強電流
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, HIGH); //LOW);

	//2. 20msTimerSet//Motor脱調対策(CR-008) - 40ms
	CMPL_TMP_RIGHT_SetEventTimer( 40,  //CMPL_TMP_COMM_MOTOR_ENABLE_TIME_20,
					 				CMPL_TMP_RIGHT_NORMAL_EVT);

}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Return_st2_CCW
// Description   ;  //動作開始Trigger待ち(RightOffset)
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_Return_st2_CCW()
{
	if( CMPL_TMP_IsEjectSheetThrough() == true ) { //Eject 減速開始
		gCMPL_TMP_RIGHT.action = CMPL_TMP_RIGHT_RETURN_st3_CCW;
		CMPL_TMP_RIGHT_ControlThread();
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Return_st3_CCW
// Description  ;  //タイマ設定(RightOffset)
// Parameter     : ‐
// Return        : ‐
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
// Description   :;  //モータ駆動設定(RightOffset)
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_Return_st4_CCW()
{
	//1.弱電流で 内（CCW)方向設定
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_DIR, CMPL_TMP_COMM_INSIDE);
	//Motor脱調対策(CR-008) TimerSet前、強電流
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, HIGH);  //LOW);

	//2. 20msTimerSet
	CMPL_TMP_RIGHT_SetEventTimer( CMPL_TMP_COMM_MOTOR_ENABLE_TIME_20,
					 				CMPL_TMP_RIGHT_NORMAL_EVT);

}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Return_st5
// Description   ;  //モータ駆動開始
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_Return_st5()
{
	//1. 強電流設定
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, HIGH);

	DD_STM_SetProfileID(DD_STM_ID_RIGHT_TAMPER, gCMPL_TMP_RIGHT.profile);

	DD_STM_MoveBySetPulse(DD_STM_ID_RIGHT_TAMPER, gCMPL_TMP_RIGHT.targetIndex,
		gCMPL_TMP_RIGHT.movePulse, CMPL_TMP_RIGHT_ReceiveSteppingMotorEvent);

}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Return_st6
// Description   : ;  //タイマ設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_Return_st6()
{
	// 20msTimerSet
	CMPL_TMP_RIGHT_SetEventTimer( CMPL_TMP_COMM_MOTOR_STOP_TIME_20,
					 				CMPL_TMP_RIGHT_NORMAL_EVT);

}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Return_st7
// Description   :  //次動作待ち
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_Return_st7()
{
	UC   sheetNo = gCMPL_TMP_RIGHT.sheetNo;
	US   nvm_time;
	
	//弱電流設定
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, LOW);

#if 1 //CR-008

	if ( gCMPL_TMP_RIGHT.sizePosAfterReturn == true) {  //SizePos開始 
		
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
// Description   :  // SizePos開始(CR-008) 
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_Return_st8()
{
	// SizePositioning実施する 
	CMPL_TMP_RIGHT_StartAct(CMPL_TMP_SIZE_POS, gCMPL_TMP_RIGHT.sheetNo);

}



// オープン動作(Nigasi)
//CMPL_TMP_OPEN = 0x0700,
///////////////////////////////////////////////////////////////////
// Function Name : RT_Open_st1
// Description   : モータ駆動設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_Open_st1()
{
	UC    sheetNo =gCMPL_TMP_RIGHT.sheetNo;
		
	//逃がし動作開始チェック
	if( (DD_DIO_GetInput(DD_DI_RIGHT_TAMPER_HOME_SNR) == HIGH) ||
	 (SRVC_GetCrossProcessSizeMixSet(gCMPL_TMP_RIGHT.sheetNo) != SHT_SIZE_MIX) ) { //Mix Size

	 	gCMPL_TMP_RIGHT.action = CMPL_TMP_st0;
		gCMPL_TMP_RIGHT.sheetNo = SHEETNO_NUM;
		CMPL_TMP_InformedRightTamperComplete(CMPL_TMP_OPEN,   	sheetNo);
	
		return;
	}

	//1. 強電流で外(CW)方向設定
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_DIR, gCMPL_TMP_RIGHT.direction);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, HIGH);

	//2. 20msTimerSet
	CMPL_TMP_RIGHT_SetEventTimer( CMPL_TMP_COMM_MOTOR_ENABLE_TIME_20,
					 			CMPL_TMP_RIGHT_NORMAL_EVT);

}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Open_st2
// Description   : モータ駆動開始
// Parameter     : ‐
// Return        : ‐
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
// Description   :; タイマ設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_Open_st3()
{

	//StepDown 
	DD_STM_SlowDownStop(DD_STM_ID_RIGHT_TAMPER, CMPL_TMP_RIGHT_ReceiveSteppingMotorEvent);

}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Open_st4
// Description   : ; 次動作待ち
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_Open_st4()
{

	// 20msTimerSet
	CMPL_TMP_RIGHT_SetEventTimer( CMPL_TMP_COMM_MOTOR_STOP_TIME_20,
									CMPL_TMP_RIGHT_NORMAL_EVT);

}

///////////////////////////////////////////////////////////////////
// Function Name : RT_Open_st5
// Description   : ; 次動作待ち
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void RT_Open_st5()
{
	UC    sheetNo = gCMPL_TMP_RIGHT.sheetNo;
	//弱電流設定	
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, LOW);
	
	gCMPL_TMP_RIGHT.action = CMPL_TMP_st0;
	gCMPL_TMP_RIGHT.sheetNo = SHEETNO_NUM;

	CMPL_TMP_InformedRightTamperComplete(CMPL_TMP_OPEN,	 sheetNo);
	PrintTamperLog_Short(CMPL_TMP_OPEN_st5, 0xFFFF, 0xFF);
}


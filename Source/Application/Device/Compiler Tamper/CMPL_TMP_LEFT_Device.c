// File Name:	CMPL_TMP_LEFT_Device.c
// Description:	Compiler LEFT Tamper
// Engineer:	FXK
// Last Edit:	15.03.12
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
#include "CMPL_TMP_LEFT_Device.h"
#include "CMPL_TMP_Boundary.h"

/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
	ECMPL_TMP_Action action;
	ECMPL_TMP_Action compoAction;
	UC sheetNo;
	UC preSheetNo; //(CR-008) Normal Tamping開始TimingのTimeout以降に状態を持たせるため

	UC profile;		 	// NewDD適用(Profile) //ECMPL_TMP_ProfileID profile;
	US targetIndex;   	// NewDD適用(TargetIndex)
	US movePulse;
	UC direction;
	US startDelayTime;
	UC ejectTampingCnt;  // Eject Tamping 時、繰り返す回数のカウント
	SS timerIndex;
	UC interruptIndex;
	UC sizePosAfterReturn;  //(CR-008)  Return動作後、SizePositioning動作実施する/しない
	SS timerIndex_PreTimer;  //Timer ID 管理用
	SS timerIndex_TimeUpControl; //Timer ID 管理用(RT 電流値切り替え時間l)
	SS timerIndex_TimeUpControl2; //(V3.5.103)Timer ID管理用 (LT電流値切り替え時間)
	UC offFailCheckCount;  //(V3.5.95) Home外し動作チェックCount。2回以上ならばHomeSNROffFail
} SCMP_TMP_LEFT_Control;

typedef struct
{
	void (*mpHandler)(void);

} TMP_LEFT_CallFunc;

static SCMP_TMP_LEFT_Control 	gCMPL_TMP_LEFT;
static EFaultStatus 			gCMPL_TMP_LEFT_FaultStat;


#define CMPL_TMP_LEFT_NORMAL_EVT		0
#define CMPL_TMP_LEFT_INTERRUPT_EVT		1
#define CMPL_TMP_LEFT_FAIL_EVT                  0x80  //0526 Fail Event added

//V3.0.90.  SlowUpしてからEms以降、50msまで強電流にする->Timer EventType define ..
typedef  enum {
	eNORMAL_EMS,   		//NormalTamping開始でEmsTimerのTimeOut
	eNORMAL_HI_CUR, 	//RT強電流～50msのTimeout
	e1stEJECT_EMS,		//EjectTamping開始でEmsTimerのTimeOut
	e1stEJECT_HI_CUR, //RT強電流～50msのTimeout
	e2ndEJECT_EMS,		//SetLastTampingの14msTimerのTimeout
	e2ndEJECT_HI_CUR,//RT強電流～50msのTimeout
	
}ETMP_EVT_TYPE;
//

#define  OSIKOMI_PLS_A	 	5  // 押し込みパルス量
#define  ZURE_PLS_B 		 2 // ズレパルス

/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
static void CMPL_TMP_LEFT_ControlThread();
static void CMPL_TMP_LEFT_ReceiveEventTimerTimeUp(SS aTimerID, UL aNotUsed);
static void CMPL_TMP_LEFT_TimeUpControl(SS aTimerID, UL aEvtType); //V3.0.
static void CMPL_TMP_LEFT_TimeUpControl2(SS aTimerID, UL aEvtType); //V3.5.103

static void CMPL_TMP_LEFT_PreTimerTimeUp(SS aTimerID, UL aEvtType);  //CR008
static void CMPL_TMP_LEFT_ReceiveSteppingMotorEvent(SS aResult);

static void CMPL_TMP_LEFT_InterruptLTHomeSensorON();

static void CMPL_TMP_LEFT_SetEventTimer(US aTime, UL aEventType);
static void CMPL_TMP_LEFT_CancelEventTimer();
static US CMPL_TMP_LEFT_GetSizePosition(UC aSheetNo);
static US CMPL_TMP_LEFT_GetTampingPosition(ECMPL_TMP_Action aAction, UC aSheetNo);
US CMPL_TMP_LEFT_GetOffsetPosition(UC aSheetNo);
US CMPL_TMP_LEFT_GetOffsetDirection(UC aSheetNo);
US CMPL_TMP_LEFT_GetReturnPosition(UC aSheetNo);
static US CMPL_TMP_LEFT_GetEmsForCurrentChange(ECMPL_TMP_Action aAction, UC aSheetNo);
static US CMPL_TMP_LEFT_GetLTCurrentChange(ECMPL_TMP_Action aAction, UC aSheetNo); //V3.5.103

// Component動作
// CMPL_TMP_COMPO_TAMPER_MOT = 0x0100,
static void  LT_Component_Mot_st1(); 	// Mot 駆動設定
static void  LT_Component_Mot_st2(); 	// 動作開始100Pulse 後, StepDown
static void  LT_Component_Mot_st3(); 	// 後励磁処理
static void  LT_Component_Mot_st4(); 	// Mot Off


// サイズポジショニング動作
// CMPL_TMP_SIZE_POS = 0x0200,
static void LT_SizePositioning_st1(); 		// Tamper HomeSNR　ONチェック
static void LT_SizePositioning_st2_CCW(); 	// Home外し動作設定
static void LT_SizePositioning_st3_CCW(); 	// CCW方向にモータ駆動開始
static void LT_SizePositioning_st4_CCW(); 	// Home外し動作完了
static void LT_SizePositioning_st2_CW(); 	// Home動作設定
static void LT_SizePositioning_st3_CW(); 	// CW方向にモータ駆動開始
static void LT_SizePositioning_st4_CW(); 	// StepUp中、割込み受信
static void LT_SizePositioning_st4_2_CW(); 	// StepUp中、割込み受信 から StepDown完了

static void LT_SizePositioning_st5_CW(); 	// StepUp完了受信
static void LT_SizePositioning_st6_CW(); 	// 安定区間で割込み受信
static void LT_SizePositioning_st7_CW(); 	// Home動作完了
static void LT_SizePositioning_st8();	 	// SizePositioning開始
static void LT_SizePositioning_st9();	 	// モータ駆動開始
static void LT_SizePositioning_st10();  	// SizePositionへ移動完了

// ノーマルタンピング動作
// CMPL_TMP_NORMAL_TAMPING = 0x0300,
static void LT_NormalTamping_st1(); 	// 動作開始遅延時間設定
static void LT_NormalTamping_st2(); 	// モータ駆動設定
static void LT_NormalTamping_st3();  	// モータ駆動開始
static void LT_NormalTamping_st4(); 	// Tamping位置へ移動完了
static void LT_NormalTamping_st5(); 	// Return方向へ切り替え
static void LT_NormalTamping_st6(); 	// モータ駆動(Return)開始
static void LT_NormalTamping_st7(); 	// NormalTamping完了
static void LT_NormalTamping_st8(); 	// 次の動作待ち設定

// イジェクトタンピング動作(SetLastTamping)
// CMPL_TMP_EJECT_TAMPING = 0x0400,
static void  LT_EjectTamping_st1(); 	// 動作開始遅延時間設定
static void  LT_EjectTamping_st2(); 	// モータ駆動設定
static void  LT_EjectTamping_st3(); 	// モータ駆動開始
static void  LT_EjectTamping_st4(); 	// Tamping位置へ移動完了
static void  LT_EjectTamping_st5(); 	// Return方向へ切り替え
static void  LT_EjectTamping_st6(); 	// モータ駆動(Return)開始
static void  LT_EjectTamping_st7(); 	// Return動作完了
static void  LT_EjectTamping_st8(); 	// SetLastTamping開始
static void  LT_EjectTamping_st9(); 	// モータ駆動(SetLast)開始
static void  LT_EjectTamping_st10(); 	// SetLast動作完了
static void  LT_EjectTamping_st11(); 	// SetLast繰り返しチェック
static void  LT_EjectTamping_st12(); 	// 次の動作待ち設定

//MissREgi (V3.5.95)
static void LT_MissRegi_st1();	// MissRegi_動作Timer設定
static void LT_MissRegi_st2();  // MissRegi_指定パルス出力
static void LT_MissRegi_st3();	// MissRegi_後励磁設定
static void LT_MissRegi_st4();	// MissRegi_完了	


// オフセット動作
// CMPL_TMP_OFFSET = 0x0500,
static void LT_Offset_st1(); 	// 開始Timing調整
static void LT_Offset_st2(); 	// モータ駆動設定
static void LT_Offset_st3(); 	// モータ駆動開始
static void LT_Offset_st4(); 	// 安定時間タイマ設定
static void LT_Offset_st5(); 	// 次動作待ち

// リターン動作
// CMPL_TMP_LEFT_RETURN = 0x0600,
static void LT_Return_st1(); 	// 動作開始Trigger待ち
static void LT_Return_st2();	// タイマ設定
static void LT_Return_st3();	// モータ駆動設定
static void LT_Return_st4(); 	// モータ駆動開始
static void LT_Return_st5(); 	// タイマ設定
static void LT_Return_st6(); 	// 次動作待ち
static void LT_Return_st7();	// SizePos開始(CR-008) 


// オープン動作(Nigasi)
// CMPL_TMP_OPEN = 0x0800,
static void LT_Open_st1(); 		// モータ駆動設定
static void LT_Open_st2(); 		// モータ駆動開始
static void LT_Open_st3(); 		// StepDown 開始
static void LT_Open_st4(); 		// タイマ設定
static void LT_Open_st5(); 		// 次動作待ち

static const TMP_LEFT_CallFunc  ControlThread_LT_COMPO_MOT[] = {
	{NULL},
	{LT_Component_Mot_st1},  	// Mot 駆動設定, 前励磁処理
	{LT_Component_Mot_st2},  	// 動作開始100Pulse 後, StepDown
	{LT_Component_Mot_st3 },	// 後励磁処理
	{LT_Component_Mot_st4}, 	// Mot Off
};

static const TMP_LEFT_CallFunc ControlThread_LT_SIZEPOS[] = {
	{NULL},
	{LT_SizePositioning_st1}, 		// Tamper HomeSNR　ONチェック
	{LT_SizePositioning_st2_CCW},  	// Home外し動作設定
	{LT_SizePositioning_st3_CCW}, 	// CCW方向にモータ駆動開始
	{LT_SizePositioning_st4_CCW}, 	// Home外し動作完了

	{LT_SizePositioning_st2_CW}, 	// Home動作設定
	{LT_SizePositioning_st3_CW}, 	// CW方向にモータ駆動開始
	{LT_SizePositioning_st4_CW}, 	// StepUp中、割込み受信
	{LT_SizePositioning_st4_2_CW}, 	// StepUp中、割込み受信 から StepDown完了
	{LT_SizePositioning_st5_CW}, 	// StepUp完了受信
	{LT_SizePositioning_st6_CW},   	// 安定区間で割込み受信
	{LT_SizePositioning_st7_CW}, 	// Home動作完了

	{LT_SizePositioning_st8},	 	// SizePositioning開始
	{LT_SizePositioning_st9},	 	// モータ駆動開始
	{LT_SizePositioning_st10},		// SizePositionへ移動完了
};


// ノーマルタンピング動作
static const TMP_LEFT_CallFunc  ControlThread_LT_NORMAL_TAMPING[] = {
	{NULL},
	{LT_NormalTamping_st1}, 	// 動作開始遅延時間設定
	{LT_NormalTamping_st2}, 	// モータ駆動設定
	{LT_NormalTamping_st3},  	// モータ駆動開始
	{LT_NormalTamping_st4}, 	// Tamping位置へ移動完了
	{LT_NormalTamping_st5}, 	// Return方向へ切り替え
	{LT_NormalTamping_st6}, 	// モータ駆動(Return)開始
	{LT_NormalTamping_st7}, 	// NormalTamping完了
	{LT_NormalTamping_st8}, 	// 次の動作待ち設定
};

// イジェクトタンピング動作(SetLastTamping)
static const TMP_LEFT_CallFunc  ControlThread_LT_EJECT_TAMPING[] = {
	{NULL},
	{LT_EjectTamping_st1}, 		// 動作開始遅延時間設定
	{LT_EjectTamping_st2}, 		// モータ駆動設定
	{LT_EjectTamping_st3}, 		// モータ駆動開始
	{LT_EjectTamping_st4}, 		// Tamping位置へ移動完了
	{LT_EjectTamping_st5}, 		// Return方向へ切り替え
	{LT_EjectTamping_st6}, 		// モータ駆動(Return)開始
	{LT_EjectTamping_st7}, 		// Return動作完了
	{LT_EjectTamping_st8}, 		// SetLastTamping開始
	{LT_EjectTamping_st9}, 		// モータ駆動(SetLast)開始
	{LT_EjectTamping_st10}, 	// SetLast動作完了
	{LT_EjectTamping_st11}, 	// SetLast繰り返しチェック
	{LT_EjectTamping_st12}, 	// 次の動作待ち設定
};

// MissRegi防止動作
static const TMP_LEFT_CallFunc  ControlThread_LT_MISSREGI[] = {
	{NULL},
	{LT_MissRegi_st1}, // MissRegi_動作Timer設定
	{LT_MissRegi_st2}, // MissRegi_指定パルス出力
	{LT_MissRegi_st3},  // MissRegi_後励磁設定
	{LT_MissRegi_st4},  // MissRegi_完了	
};

// オフセット動作
static const TMP_LEFT_CallFunc  ControlThread_LT_OFFSET[] = {
	{NULL},
	{LT_Offset_st1}, 	// 開始Timing調整
	{LT_Offset_st2}, 	// モータ駆動設定
	{LT_Offset_st3}, 	// モータ駆動開始
	{LT_Offset_st4}, 	// 安定時間タイマ設定
	{LT_Offset_st5}, 	// 次動作待ち
};

// リターン動作
static const TMP_LEFT_CallFunc ControlThread_LT_RETURN[] = {
	{ NULL},
	{LT_Return_st1}, 	// 動作開始Trigger待ち
	{LT_Return_st2},	// タイマ設定
	{LT_Return_st3},	// モータ駆動設定
	{LT_Return_st4}, 	// モータ駆動開始
	{LT_Return_st5}, 	// タイマ設定
	{LT_Return_st6}, 	// 次動作待ち
	{LT_Return_st7},       // (CR-008)
};

// オープン動作(Nigasi)
static const TMP_LEFT_CallFunc  ControlThread_LT_OPEN[] = {
	{NULL},
	{LT_Open_st1}, 	// モータ駆動設定
	{LT_Open_st2}, 	// モータ駆動開始
	{LT_Open_st3}, 	// StepDown設定
	{LT_Open_st4}, 	// タイマ設定
	{LT_Open_st5}, 	// 次動作待ち
};

/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_LEFT_StartAct
// Description   : タンパー動作設定_動作開始
// Parameter     : aAction (制御する動作)
//				 : aSheetNo (制御するSheetNo)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_LEFT_StartAct(ECMPL_TMP_Action aAction, UC aSheetNo)
{

	switch( aAction ) {
	// Component 動作 //////////////////////////////////////////////////////////////////////////////
	// LEFT Tamperを外側方向(CW)に低速(643PPS)で移動
	case CMPL_TMP_COMPO_LEFT_CW_LOW:
		gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_LOW;
		gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_LOW;

		gCMPL_TMP_LEFT.direction = CMPL_TMP_COMM_OUTSIDE;
		gCMPL_TMP_LEFT.compoAction = aAction;
		gCMPL_TMP_LEFT.action = CMPL_TMP_COMPO_TAMPER_MOT_st1;
		break;

	// LEFT Tamperを外側方向(CW)に中速(800PPS)で移動
	case CMPL_TMP_COMPO_LEFT_CW_MID:
		gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_MID;
		gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_MID;

		gCMPL_TMP_LEFT.direction = CMPL_TMP_COMM_OUTSIDE;
		gCMPL_TMP_LEFT.compoAction = aAction;
		gCMPL_TMP_LEFT.action = CMPL_TMP_COMPO_TAMPER_MOT_st1;
		break;

	// LEFT Tamperを外側方向(CW)に高速(1337PPS)で移動
	case CMPL_TMP_COMPO_LEFT_CW_HI:
		gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_HI;
		gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_HI;

		gCMPL_TMP_LEFT.direction = CMPL_TMP_COMM_OUTSIDE;
		gCMPL_TMP_LEFT.compoAction = aAction;
		gCMPL_TMP_LEFT.action = CMPL_TMP_COMPO_TAMPER_MOT_st1;
		break;

	// LEFT Tamperを内側方向(CCW)に低速(643PPS)で移動
	case CMPL_TMP_COMPO_LEFT_CCW_LOW :
		gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_LOW;
		gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_LOW;

		gCMPL_TMP_LEFT.direction = CMPL_TMP_COMM_INSIDE;
		gCMPL_TMP_LEFT.compoAction = aAction;
		gCMPL_TMP_LEFT.action = CMPL_TMP_COMPO_TAMPER_MOT_st1;
		break;

	// LEFT Tamperを内側方向(CCW)に中速(800PPS)で移動
	case CMPL_TMP_COMPO_LEFT_CCW_MID :
		gCMPL_TMP_LEFT.profile =DD_STM_PROF_ID_TAMPER_NORMAL_MID;
		gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_MID;

		gCMPL_TMP_LEFT.direction = CMPL_TMP_COMM_INSIDE;
		gCMPL_TMP_LEFT.compoAction = aAction;
		gCMPL_TMP_LEFT.action = CMPL_TMP_COMPO_TAMPER_MOT_st1;
		break;

	// LEFT Tamperを内側方向(CCW)に高速(1337PPS)で移動
	case  CMPL_TMP_COMPO_LEFT_CCW_HI :
		gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_HI;
		gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_HI;

		gCMPL_TMP_LEFT.direction = CMPL_TMP_COMM_INSIDE;
		gCMPL_TMP_LEFT.compoAction = aAction;
		gCMPL_TMP_LEFT.action = CMPL_TMP_COMPO_TAMPER_MOT_st1;
		break;
	// Component 動作 //////////////////////////////////////////////////////////////////////////////

	// サイズポジショニング動作
	case CMPL_TMP_SIZE_POS :  // = 0x0200,  Size Positioning
		gCMPL_TMP_LEFT.movePulse = CMPL_TMP_LEFT_GetSizePosition(aSheetNo);
		gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_INITIALIZE;
		gCMPL_TMP_LEFT.direction = CMPL_TMP_COMM_INSIDE;
		gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st1;
		gCMPL_TMP_LEFT.offFailCheckCount = 0;  
		//LeftSizePos
		PrintTamperLog_Short(CMPL_TMP_SIZE_POS+0x80, (US)gCMPL_TMP_LEFT.movePulse, aSheetNo);
		break;

	// ノーマルタンピング動作
	case CMPL_TMP_NORMAL_TAMPING : // = 0x0300,
		gCMPL_TMP_LEFT.movePulse = CMPL_TMP_LEFT_GetTampingPosition(aAction, aSheetNo);

		if ( SRVC_GetProcessSize(aSheetNo) <= CMPL_TMP_COMM_TAMPING_PROFILE_BORDER_LENGTH ) {
			gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_HI;
			gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_HI;

		} else {
		// V3.0  ( 不具合　：　用紙長[mm]297.0超え：993PPS中速Profile)
			gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_MID;
			gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_MID;
		//	gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_LOW;
		//	gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_LOW;
		}

		gCMPL_TMP_LEFT.startDelayTime = CMPL_TMP_GetNormalTampingDelayTime(aSheetNo);
		gCMPL_TMP_LEFT.direction= CMPL_TMP_COMM_INSIDE;
		gCMPL_TMP_LEFT.action = CMPL_TMP_NORMAL_TAMPING_st1;
		PrintTamperLog_Short(CMPL_TMP_NORMAL_TAMPING, (US)gCMPL_TMP_LEFT.movePulse, aSheetNo);
		break;

	// イジェクトタンピング動作(SetLastTamping)
	case CMPL_TMP_EJECT_TAMPING :  //= 0x0400,
		gCMPL_TMP_LEFT.movePulse = CMPL_TMP_LEFT_GetTampingPosition(aAction, aSheetNo);

		if ( SRVC_GetProcessSize(aSheetNo) <= CMPL_TMP_COMM_TAMPING_PROFILE_BORDER_LENGTH ) {
			gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_HI;
			gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_HI;

		} else {
		// V3.0  ( 不具合　：　用紙長[mm]297.0超え：993PPS中速Profile)
			gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_MID;
			gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_MID;
		//	gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_LOW;
		//	gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_LOW;
		}

		gCMPL_TMP_LEFT.startDelayTime = CMPL_TMP_GetEjectTampingDelayTime(aSheetNo);
		gCMPL_TMP_LEFT.ejectTampingCnt = 0;  // Eject Tamping 時、繰り返す回数のカウント
		gCMPL_TMP_LEFT.direction = CMPL_TMP_COMM_INSIDE;
		gCMPL_TMP_LEFT.action = CMPL_TMP_EJECT_TAMPING_st1;
		PrintTamperLog_Short(CMPL_TMP_EJECT_TAMPING, (US)gCMPL_TMP_LEFT.movePulse, aSheetNo);
		break;
		
	//MissRegi防止動作 (V3.5.95)	
	case CMPL_TMP_MISSREGI : //=0x0A00,
		gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_LOW;
		gCMPL_TMP_LEFT.targetIndex = 0;		
		gCMPL_TMP_LEFT.action = CMPL_TMP_MISSREGI_st1;
		PrintTamperLog_Short(CMPL_TMP_MISSREGI, 0, aSheetNo);
		break;
		
	// オフセット動作
	case CMPL_TMP_OFFSET :  // = 0x0500,
		gCMPL_TMP_LEFT.movePulse = CMPL_TMP_LEFT_GetOffsetPosition(aSheetNo);
		gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_LOW; // 通常 低速 Profile
		gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_LOW;

		gCMPL_TMP_LEFT.direction = (UC)CMPL_TMP_LEFT_GetOffsetDirection(aSheetNo);
		gCMPL_TMP_LEFT.action = CMPL_TMP_OFFSET_st1;
		//LeftOffset
		PrintTamperLog_Short(CMPL_TMP_OFFSET+0x80, (US)gCMPL_TMP_LEFT.movePulse, aSheetNo);
		break;

	// リターン動作
	case CMPL_TMP_LEFT_RETURN : // = 0x0600,
		gCMPL_TMP_LEFT.movePulse = CMPL_TMP_LEFT_GetReturnPosition(aSheetNo);
		gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_HI; // 通常 高速 Profile
		gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_HI;
		
		gCMPL_TMP_LEFT.direction = CMPL_TMP_COMM_OUTSIDE;
		gCMPL_TMP_LEFT.action = CMPL_TMP_LEFT_RETURN_st1;
		
		//CR-010		
		if (( gCMPL_TMP_LEFT.movePulse ==10) && (gCMPL_TMP_LEFT.direction == CMPL_TMP_COMM_OUTSIDE)) {
				gCMPL_TMP_LEFT.sizePosAfterReturn = true;
		}		
		PrintTamperLog_Short(CMPL_TMP_LEFT_RETURN, (US)gCMPL_TMP_LEFT.movePulse, aSheetNo);
		break;

	// オープン動作(Nigasi)
	case CMPL_TMP_OPEN :  // = 0x0800,
		gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_HI; // 通常 高速 Profile
		gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_HI;

		gCMPL_TMP_LEFT.direction = CMPL_TMP_COMM_OUTSIDE;
		gCMPL_TMP_LEFT.action = CMPL_TMP_OPEN_st1;
		//Left Open
		PrintTamperLog_Short(CMPL_TMP_OPEN+0x80, 0, aSheetNo);
		break;

	default :
		break;
	}
	
	gCMPL_TMP_LEFT.sheetNo = aSheetNo;
	gCMPL_TMP_LEFT.timerIndex = MSP_MAX_TMR_NUM;

	CMPL_TMP_LEFT_ControlThread();
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_LEFT_ControlThread
// Description   : タンパー制御
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_LEFT_ControlThread()
{

	ECMPL_TMP_Action aAction =  gCMPL_TMP_LEFT.action & 0xFF00;
	US  	   		 aStatus =  gCMPL_TMP_LEFT.action & 0x00FF;

	if ( aStatus == 0 )  {
		CMPL_TMP_LEFT_DeviceStop();
		return;
	}

	switch ( aAction)	{

	case  CMPL_TMP_COMPO_TAMPER_MOT :
		ControlThread_LT_COMPO_MOT[aStatus].mpHandler();  break;

	// サイズポジショニング動作
	case CMPL_TMP_SIZE_POS :  // = 0x0200,  Size Positioning
		ControlThread_LT_SIZEPOS[aStatus].mpHandler(); break;

	// ノーマルタンピング動作
	case CMPL_TMP_NORMAL_TAMPING : // = 0x0300,
		ControlThread_LT_NORMAL_TAMPING[aStatus].mpHandler(); break;

	// イジェクトタンピング動作(SetLastTamping)
	case CMPL_TMP_EJECT_TAMPING :  //= 0x0400,
		ControlThread_LT_EJECT_TAMPING[aStatus].mpHandler(); break;
		
	// MissREGI動作
	case CMPL_TMP_MISSREGI :  //= 0x0A00,
		ControlThread_LT_MISSREGI[aStatus].mpHandler(); break;

	// オフセット動作
	case CMPL_TMP_OFFSET :  //= 0x0500,
		ControlThread_LT_OFFSET[aStatus].mpHandler(); break;

	// リターン動作
	case CMPL_TMP_LEFT_RETURN : // = 0x0600,
		ControlThread_LT_RETURN[aStatus].mpHandler(); break;

	// オープン動作(Nigasi)
	case CMPL_TMP_OPEN :  // = 0x0800,
		ControlThread_LT_OPEN[aStatus].mpHandler(); break;

	default :
		break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_LEFT_ReceiveEventTimerTimeUp
// Description   : イベントタイマのタイムアップ
// Parameter     : timerID (Timer ID)
//               : notUsed (Not Used)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_LEFT_ReceiveEventTimerTimeUp(SS aTimerID, UL aEvtType)
{
	if ( aTimerID != gCMPL_TMP_LEFT.timerIndex ) {
		return;
	}

	gCMPL_TMP_LEFT.timerIndex = MSP_MAX_TMR_NUM;

	
	if ( aEvtType <  CMPL_TMP_LEFT_FAIL_EVT) { //NORMAL EVT

		switch ( gCMPL_TMP_LEFT.action ) {

		case CMPL_TMP_COMPO_TAMPER_MOT_st1 :
		case CMPL_TMP_COMPO_TAMPER_MOT_st3 :
			gCMPL_TMP_LEFT.action++;
			CMPL_TMP_LEFT_ControlThread();
			break;

		// Size Positioning
		case CMPL_TMP_SIZE_POS_st2_CCW :  // 20ms Timeout 前励磁処理
			gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st3_CCW;
			CMPL_TMP_LEFT_ControlThread();
			break;
		case CMPL_TMP_SIZE_POS_st4_CCW : // Home 外し動作完了 -> 50ms Timeout
			gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st2_CW; // Home 動作設定
			CMPL_TMP_LEFT_ControlThread();
			break;
		case CMPL_TMP_SIZE_POS_st2_CW : // 20ms Timeout 前励磁処理
			gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st3_CW;
			CMPL_TMP_LEFT_ControlThread();
			break;
	 	case CMPL_TMP_SIZE_POS_st4_2_CW : // 50ms Timeout
			gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st2_CCW; // Home外し動作へ
			CMPL_TMP_LEFT_ControlThread();
			break;
		case CMPL_TMP_SIZE_POS_st7_CW : // 50ms Timeout
			gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st8; // Home動作完了 -> Size Positioning Start ..
			CMPL_TMP_LEFT_ControlThread();
			break;
		case CMPL_TMP_SIZE_POS_st8 : // 20ms Timeout  前励磁処理
			gCMPL_TMP_LEFT.action++;
			CMPL_TMP_LEFT_ControlThread();
			break;

		// Normal Tamping
		case CMPL_TMP_NORMAL_TAMPING_st1:	// 動作開始遅延時間設定
		case CMPL_TMP_NORMAL_TAMPING_st2:	// モータ駆動設定
		case CMPL_TMP_NORMAL_TAMPING_st4: 	// Tamping位置へ移動完了
		case CMPL_TMP_NORMAL_TAMPING_st5: 	// Return方向へ切り替え
		case CMPL_TMP_NORMAL_TAMPING_st7: 	// NormalTamping完了
			gCMPL_TMP_LEFT.action++;
			CMPL_TMP_LEFT_ControlThread();
			break;

		// Eject Tamping
		case CMPL_TMP_EJECT_TAMPING_st1: 	// 動作開始遅延時間設定
		case CMPL_TMP_EJECT_TAMPING_st2: 	// モータ駆動設定
		case CMPL_TMP_EJECT_TAMPING_st4: 	// Tamping位置へ移動完了
		case CMPL_TMP_EJECT_TAMPING_st5: 	// Return方向へ切り替え
		case CMPL_TMP_EJECT_TAMPING_st7: 	// Return動作完了
		case CMPL_TMP_EJECT_TAMPING_st8:  	// SetLastTamping開始
		case CMPL_TMP_EJECT_TAMPING_st10: 	// SetLast動作完了
			gCMPL_TMP_LEFT.action++;
			CMPL_TMP_LEFT_ControlThread();
			break;
			
		//MissREGi
		case CMPL_TMP_MISSREGI_st1: 	  // MissRegi  前励磁処理(V3.5.94)
		case CMPL_TMP_MISSREGI_st3: 	 // MissRegi 後励磁処理
			gCMPL_TMP_LEFT.action++;
			CMPL_TMP_LEFT_ControlThread();
			break;

		// Offset
		case CMPL_TMP_OFFSET_st1: 			// 開始Timing調整
		case CMPL_TMP_OFFSET_st2: 			// モータ駆動設定
		case CMPL_TMP_OFFSET_st4: 			// 安定時間タイマ設定
			gCMPL_TMP_LEFT.action++;
			CMPL_TMP_LEFT_ControlThread();
			break;

		// Return
		case CMPL_TMP_LEFT_RETURN_st2: 		// タイマ設定
		case CMPL_TMP_LEFT_RETURN_st3: 		// モータ駆動設定
		case CMPL_TMP_LEFT_RETURN_st5: 		// タイマ設定
		case CMPL_TMP_LEFT_RETURN_st6: 		// (CR-008)
			gCMPL_TMP_LEFT.action++;
			CMPL_TMP_LEFT_ControlThread();
			break;

		// Open
		case CMPL_TMP_OPEN_st1: 			// モータ駆動設定
		case CMPL_TMP_OPEN_st4: 			// タイマ設定
			gCMPL_TMP_LEFT.action++;
			CMPL_TMP_LEFT_ControlThread();
			break;
		
		default:
			break;
		}
		
	} else {  //Fail Event 
	
		switch ( gCMPL_TMP_LEFT.action ) {
		case CMPL_TMP_SIZE_POS_st3_CW : // Fail  Timeout
		case CMPL_TMP_SIZE_POS_st5_CW : // Fail  Timeout(V3.5.93Tamper不具合(Fail通知されない))
			CMPL_TMP_InformedLeftTamperHomeSensorOnFail(gCMPL_TMP_LEFT.sheetNo);
			break;
		case CMPL_TMP_OPEN_st2: 			// Fail Timeout
			CMPL_TMP_InformedLeftTamperHomeSensorOnFail(gCMPL_TMP_LEFT.sheetNo);
			break;

		default:
				break;
		}

	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_LEFT_TimeUpControl2
// Description   : 
// Parameter     : timerID (Timer ID)           :  
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_LEFT_TimeUpControl2(SS aTimerID, UL aEvtType)
{
	if ( aTimerID != gCMPL_TMP_LEFT.timerIndex_TimeUpControl2) {
		return;
	}

	gCMPL_TMP_LEFT.timerIndex_TimeUpControl2= MSP_MAX_TMR_NUM;

	// 電流値=弱
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, LOW);

	switch(aEvtType) {		
	case  	eNORMAL_EMS :   	
		PrintTamperLog_Short(CMPL_TMP_NORMAL_TAMPING+0x88, 0xF1, LOW);	
		break;
	case 	e1stEJECT_EMS :
		PrintTamperLog_Short(CMPL_TMP_EJECT_TAMPING+0x88, 0xF1, LOW);	
		break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_LEFT_TimeUpControl
// Description   : 
// Parameter     : timerID (Timer ID)           :  
//aEvtTypetypedef  enum {
//eNORMAL_EMS,		//NormalTamping開始でEmsTimerのTimeOut
//eNORMAL_HI_CURRENT ,	//RT強電流～50msのTimeout
//e1stEJECT_EMS,		//EjectTamping開始でEmsTimerのTimeOut
//e1stEJECT_HI_CURRENT, //RT強電流～50msのTimeout
//e2ndEJECT_EMS,		//SetLastTampingの14msTimerのTimeout
//e2ndEJECT_HI_CURRENT,//RT強電流～50msのTimeout
//} ETMP_EVT_TYPE;
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_LEFT_TimeUpControl(SS aTimerID, UL aEvtType)
{
	if ( aTimerID != gCMPL_TMP_LEFT.timerIndex_TimeUpControl) {
		return;
	}

	gCMPL_TMP_LEFT.timerIndex_TimeUpControl= MSP_MAX_TMR_NUM;

	switch(aEvtType) {
		
	//NormalTamping開始でEmsTimerのTimeOut	
	case  	eNORMAL_EMS :   	
		// RT強電流 , 50ms Start
		DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, HIGH);
		gCMPL_TMP_LEFT.timerIndex_TimeUpControl = MSP_SetEventTimer( 50, CMPL_TMP_LEFT_TimeUpControl, eNORMAL_HI_CUR);
		break;
	
	case		eNORMAL_HI_CUR : 	//RT強電流～50msのTimeout
		//RT弱電流
		DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, LOW);
		break;
		
	//EjectTamping開始でEmsTimerのTimeOut
	case 	e1stEJECT_EMS :		
		// RT強電流 , 50ms Start
		DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, HIGH);
		gCMPL_TMP_LEFT.timerIndex_TimeUpControl = MSP_SetEventTimer( 50, CMPL_TMP_LEFT_TimeUpControl, e1stEJECT_HI_CUR);
		break;
	
	case		e1stEJECT_HI_CUR : //RT強電流～50msのTimeout
		//RT弱電流
		DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, LOW);
		break;
		
	//SetLastTampingの14msTimerのTimeout
	case		e2ndEJECT_EMS :		
		// RT強電流 , 50ms Start
		DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, HIGH);
		gCMPL_TMP_LEFT.timerIndex_TimeUpControl = MSP_SetEventTimer( 50, CMPL_TMP_LEFT_TimeUpControl, e2ndEJECT_HI_CUR);
		break;
	
	case 	e2ndEJECT_HI_CUR :  //RT強電流～50msのTimeout
		//RT弱電流
		DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, LOW);
		break;
	
	default :  break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_LEFT_PreStartAct
// Description   : CR008- Normal Tamping開始TimingのTimeout以降に状態を持たせるため、
// Parameter     :ECMPL_TMP_Action ,  UC  aSheetNo
// Return        : ‐
///////////////////////////////////////////////////////////////////
void  CMPL_TMP_LEFT_PreStartAct(ECMPL_TMP_Action aAction, UC aSheetNo)
{
	US   delay;
	gCMPL_TMP_LEFT.preSheetNo = aSheetNo;

	switch(aAction) {
		
	// ノーマルタンピング動作開始前TimerSet
	case  CMPL_TMP_PRE_NORMAL_TAMPING : 
		delay = CMPL_TMP_GetNormalTampingDelayTime(gCMPL_TMP_LEFT.preSheetNo);
		gCMPL_TMP_LEFT.timerIndex_PreTimer = MSP_SetEventTimer(delay, CMPL_TMP_LEFT_PreTimerTimeUp, CMPL_TMP_PRE_NORMAL_TAMPING);
		break;				
	default :  break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_LEFT_PreTimerTimeUp
// Description   : CR008
// Parameter     : timerID (Timer ID)           :  aEvtType
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_LEFT_PreTimerTimeUp(SS aTimerID, UL aEvtType)
{
	if ( aTimerID != gCMPL_TMP_LEFT.timerIndex_PreTimer) {
		return;
	}

	gCMPL_TMP_LEFT.timerIndex_PreTimer = MSP_MAX_TMR_NUM;

	switch(aEvtType) {
		
	// NormalTamping Start ...
	case  CMPL_TMP_PRE_NORMAL_TAMPING : 
		CMPL_TMP_LEFT_StartAct(CMPL_TMP_NORMAL_TAMPING, gCMPL_TMP_LEFT.preSheetNo);		
		break;
	default :  break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_LEFT_ReceiveSteppingMotorEvent
// Description   :
// Parameter     : timerID (Timer ID)
//               : notUsed (Not Used)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_LEFT_ReceiveSteppingMotorEvent(SS aResult)
{
	switch ( gCMPL_TMP_LEFT.action ) {

	// Component
	case CMPL_TMP_COMPO_TAMPER_MOT_st2 :  // StepDown Wait ..
		gCMPL_TMP_LEFT.action = CMPL_TMP_COMPO_TAMPER_MOT_st3;
		CMPL_TMP_LEFT_ControlThread();
		break;

	// Size Positioning
	case CMPL_TMP_SIZE_POS_st3_CCW :  // Home 外し動作完了 Event
		if ( DD_DIO_GetInput(DD_DI_LEFT_TAMPER_HOME_SNR) == HIGH ) {	// Tamper HomeSNR Off Fail
			gCMPL_TMP_LEFT_FaultStat = FAULT_OCCUR;
			CMPL_TMP_InformedLeftTamperHomeSensorOffFail(gCMPL_TMP_LEFT.sheetNo);
		} else {
			gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st4_CCW;
			CMPL_TMP_LEFT_ControlThread();
		}
		break;

	case CMPL_TMP_SIZE_POS_st3_CW : // Home動作完了 Event

		// 定速中、ON割込み
		gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st5_CW;
		// CMPL_TMP_LEFT_ControlThread();  // Nothing to do ..
		break;

	case CMPL_TMP_SIZE_POS_st4_CW : //StepDown完了 Event
		gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st4_2_CW;
		CMPL_TMP_LEFT_ControlThread();
		break;

	case CMPL_TMP_SIZE_POS_st6_CW :
		gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st7_CW;
		CMPL_TMP_LEFT_ControlThread();
		break;

	case CMPL_TMP_SIZE_POS_st9 : // SizePositioning Pulse A  移動完了
		gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st10;
		CMPL_TMP_LEFT_ControlThread();
		break;

	// Normal Tamping
	case CMPL_TMP_NORMAL_TAMPING_st3 : 	// モータ駆動開始
	case CMPL_TMP_NORMAL_TAMPING_st6 : 	// モータ駆動(Return)開始
		gCMPL_TMP_LEFT.action++;
		CMPL_TMP_LEFT_ControlThread();
		break;

	// Eject Tamping
	case CMPL_TMP_EJECT_TAMPING_st3 : 	// モータ駆動開始
	case CMPL_TMP_EJECT_TAMPING_st6 :  	// モータ駆動(Return)開始
	case CMPL_TMP_EJECT_TAMPING_st9 : 	// モータ駆動(SetLast)開始
		gCMPL_TMP_LEFT.action++;
		CMPL_TMP_LEFT_ControlThread();
		break;
	// MissREGI
 	case CMPL_TMP_MISSREGI_st2:      // MissRegi (V3.5.94)
		gCMPL_TMP_LEFT.action++;
		CMPL_TMP_LEFT_ControlThread();
		break;

	// Offset
	case CMPL_TMP_OFFSET_st3 : 			// モータ駆動開始
		gCMPL_TMP_LEFT.action++;
		CMPL_TMP_LEFT_ControlThread();
		break;

	// Return
	case CMPL_TMP_LEFT_RETURN_st4 : 	// モータ駆動開始
		gCMPL_TMP_LEFT.action++;
		CMPL_TMP_LEFT_ControlThread();
		break;

	// Open
	case CMPL_TMP_OPEN_st3 : 			// Stepdown開始
		gCMPL_TMP_LEFT.action++;
		CMPL_TMP_LEFT_ControlThread();
		break;

	default:
		break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name 	: CMPL_TMP_LEFT_LTHomeSensorON
// Description 	 	:  LeftHomeSensor ON
// Parameter     	:
///////////////////////////////////////////////////////////////////
void CMPL_TMP_LEFT_LTHomeSensorON()
{
}

///////////////////////////////////////////////////////////////////
// Function Name 	: CMPL_TMP_LEFT_InterruptLTHomeSensorON
// Description 		:
// Parameter     	:
///////////////////////////////////////////////////////////////////
void CMPL_TMP_LEFT_InterruptLTHomeSensorON()
{
	if ( gCMPL_TMP_LEFT.action == CMPL_TMP_st0 )
			return;


	DD_INT_CancelCallback(DD_INT_LEFT_TAMPER_HOME_SENSOR, TMP_RISE, gCMPL_TMP_LEFT.interruptIndex);

	switch ( gCMPL_TMP_LEFT.action ) {
	case CMPL_TMP_SIZE_POS_st3_CW :  // StepUp中、割込み受信
		CMPL_TMP_LEFT_CancelEventTimer();	// Fail Timer cancel ..
		gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st4_CW;
		CMPL_TMP_LEFT_ControlThread();
		break;
		
	case CMPL_TMP_SIZE_POS_st5_CW :  // 安定区間で割込み受信
		CMPL_TMP_LEFT_CancelEventTimer();	// Fail Timer cancel ..
		gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st6_CW;		
		CMPL_TMP_LEFT_ControlThread();
		break;

	case CMPL_TMP_OPEN_st2 :   		// 割込み受信
		CMPL_TMP_LEFT_CancelEventTimer();	// Fail Timer cancel ..
		gCMPL_TMP_LEFT.action++;
		CMPL_TMP_LEFT_ControlThread();
		break;

	default :
		break;
	}
}

void CMPL_TMP_LEFT_LTHomeSensorOFF()
{
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_LEFT_InformedEjectStart
// Description :  Eject 動作開始通知
// Parameter     :
///////////////////////////////////////////////////////////////////
void CMPL_TMP_LEFT_InformedEjectStart()
{
	if ( gCMPL_TMP_LEFT.action == CMPL_TMP_LEFT_RETURN_st1 ) {
		gCMPL_TMP_LEFT.action = CMPL_TMP_LEFT_RETURN_st2;
		CMPL_TMP_LEFT_ControlThread();
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_LEFT_InformedEjectSheetThrough
// Description :  EjectMot減速開始通知
// Parameter     :
///////////////////////////////////////////////////////////////////
void CMPL_TMP_LEFT_InformedEjectSheetThrough()
{
  	// 必要無し..(RIGHTのみ必要..)
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_LEFT_SetEventTimer
// Description   : イベントタイマーセット
// Parameter     : aTime (タイマー値 (×1[ms]))
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_LEFT_SetEventTimer(US aTime, UL aEventType)
{
	gCMPL_TMP_LEFT.timerIndex = MSP_SetEventTimer(aTime, CMPL_TMP_LEFT_ReceiveEventTimerTimeUp, aEventType);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_LEFT_CancelEventTimer
// Description   : イベントタイマーセット
// Parameter     : aTime (タイマー値 (×1[ms]))
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_LEFT_CancelEventTimer()
{
	if ( gCMPL_TMP_LEFT.timerIndex != MSP_MAX_TMR_NUM ) {
		MSP_CancelEventTimer(gCMPL_TMP_LEFT.timerIndex);
		gCMPL_TMP_LEFT.timerIndex = MSP_MAX_TMR_NUM;
	}

}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_LEFT_IsExecutiongDeviceAction
// Description   : Leftタンパーの動作状態を取得
// Parameter     : ‐
// Return        : true：動作中、false：停止中
///////////////////////////////////////////////////////////////////
bool CMPL_TMP_LEFT_IsExecutiongDeviceAction()
{
	if ( gCMPL_TMP_LEFT.action == CMPL_TMP_st0 ) {
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
bool CMPL_TMP_LEFT_IsOccurFault()
{
	if( gCMPL_TMP_LEFT_FaultStat == FAULT_OCCUR ) {
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
UC CMPL_TMP_LEFT_GetControlSheetNo()
{
	return gCMPL_TMP_LEFT.sheetNo;
}

///////////////////////////////////////////////////////////////////
// Function Name :
// Description   :
// Parameter     :
// Return        :
///////////////////////////////////////////////////////////////////
US CMPL_TMP_LEFT_GetTamperPosition(UC aSheetNo)
{
	US aPosition;

	if ( aSheetNo == SHEETNO_NUM ) {
		aPosition = 0; //gCMPL_TMP_LEFT.nowPosition;
	} else {
		aPosition = CMPL_TMP_LEFT_GetSizePosition(aSheetNo);
	}

	return aPosition;
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_LEFT_Reset
// Description   : タンパー リセット
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_LEFT_Reset()
{
	gCMPL_TMP_LEFT.action = CMPL_TMP_st0;
	gCMPL_TMP_LEFT.sheetNo = SHEETNO_NUM;
	gCMPL_TMP_LEFT.preSheetNo = SHEETNO_NUM;
	gCMPL_TMP_LEFT.compoAction = 0;

	gCMPL_TMP_LEFT.profile = 0;
	gCMPL_TMP_LEFT.targetIndex = 0;
	gCMPL_TMP_LEFT.direction = CMPL_TMP_COMM_OUTSIDE;

	gCMPL_TMP_LEFT.timerIndex = MSP_MAX_TMR_NUM;
	gCMPL_TMP_LEFT.sizePosAfterReturn = false; 
	gCMPL_TMP_LEFT.timerIndex_PreTimer=MSP_MAX_TMR_NUM;
	gCMPL_TMP_LEFT.timerIndex_TimeUpControl= MSP_MAX_TMR_NUM;
	gCMPL_TMP_LEFT.timerIndex_TimeUpControl2= MSP_MAX_TMR_NUM;
	gCMPL_TMP_LEFT.offFailCheckCount  = 0;

	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_ENB, OFF);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, LOW);
	DD_STM_ShutDownStop(DD_STM_ID_LEFT_TAMPER);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_LEFT_DeviceStop
// Description   : タンパー停止
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_TMP_LEFT_DeviceStop()
{
	ECMPL_TMP_Action aAction = gCMPL_TMP_LEFT.action & CMPL_TMP_ACTION_MASK;
	UC aSheetNo = gCMPL_TMP_LEFT.sheetNo;

	// Component 動作中
	if ( aAction == CMPL_TMP_COMPO_TAMPER_MOT ) {
		aAction = gCMPL_TMP_LEFT.compoAction;
	}

	gCMPL_TMP_LEFT.action = CMPL_TMP_st0;
	gCMPL_TMP_LEFT.sheetNo = SHEETNO_NUM;
	gCMPL_TMP_LEFT.sizePosAfterReturn = false; 
	gCMPL_TMP_LEFT.offFailCheckCount  = 0;

	CMPL_TMP_LEFT_CancelEventTimer();
	
	if ( gCMPL_TMP_LEFT.timerIndex_PreTimer  != MSP_MAX_TMR_NUM ) {
		MSP_CancelEventTimer(gCMPL_TMP_LEFT.timerIndex_PreTimer);
		gCMPL_TMP_LEFT.timerIndex_PreTimer= MSP_MAX_TMR_NUM;
	}
	if ( gCMPL_TMP_LEFT.timerIndex_TimeUpControl  != MSP_MAX_TMR_NUM ) {
		MSP_CancelEventTimer(gCMPL_TMP_LEFT.timerIndex_TimeUpControl);
		gCMPL_TMP_LEFT.timerIndex_TimeUpControl= MSP_MAX_TMR_NUM;
	}
	if ( gCMPL_TMP_LEFT.timerIndex_TimeUpControl2  != MSP_MAX_TMR_NUM ) {
		MSP_CancelEventTimer(gCMPL_TMP_LEFT.timerIndex_TimeUpControl2);
		gCMPL_TMP_LEFT.timerIndex_TimeUpControl2= MSP_MAX_TMR_NUM;
	}

	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_ENB, OFF);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, LOW);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, LOW);
	DD_STM_ShutDownStop(DD_STM_ID_LEFT_TAMPER);

	CMPL_TMP_InformedLeftTamperComplete(aAction, aSheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_LEFT_GetSizePosition
// Description   : サイズポジション位置を取得
// Parameter     : aSheetNo (対象シートナンバー)
// Return        : サイズポジション位置
///////////////////////////////////////////////////////////////////
US CMPL_TMP_LEFT_GetSizePosition(UC aSheetNo)
{
	float aPosition=0;
	float aWidth = (float)(SRVC_GetCrossProcessSize(aSheetNo));

	if ( (CMPL_TMP_IsCanTampingSheetSize(aSheetNo) == true) &&
		(SRVC_GetCrossProcessSizeMixSet(aSheetNo) == SHT_SIZE_UNMIX) ) {
		switch ( SRVC_GetStapleMode(aSheetNo) ) {

		case eCdiFinStaple_Off:
		case eCdiFinStaple_SingleFront:
			aPosition = (1180.0 - (aWidth / 2.0) - 100.0) / CMPL_TMP_COMM_MOT_STEP + 5;
			aPosition /= 10;
			break;
		}
	}
	
	return (US)aPosition;
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_LEFT_GetTampingPosition
// Description   : タンピング位置取得
// Parameter     : aSheetNo (対象シートナンバー)
// Return        : タンピング位置
///////////////////////////////////////////////////////////////////
US CMPL_TMP_LEFT_GetTampingPosition(ECMPL_TMP_Action aAction, UC aSheetNo)
{
	float aPosition=0;
	float aWidth = (float)SRVC_GetCrossProcessSize(aSheetNo);
	UC   aSetLast_N = 0;

	switch ( SRVC_GetStapleMode(aSheetNo) ) {

	case eCdiFinStaple_Off :
		aPosition = 85 + SRVC_GetNvmValueSC(NVMID_TAMPER_UN_STPLE_ADJST); //NVM1;

		// 制御対象用紙が、コンパイル枚数Tamper用紙揃え押しこみ量追加の調整閾値枚目以上?
		if( SRVC_GetCompileCount(aSheetNo) >= SRVC_GetNvmValueUC(NVMID_TAMPING_ADD_PUSH_PAPER_COUNT))
			aPosition += SRVC_GetNvmValueSC(NVMID_TAMPING_ADD_PUSH_PULSE); //NVM3;
		break;

	case eCdiFinStaple_SingleFront :
		aPosition = ((1460.0 - (aWidth / 2.0) )/ CMPL_TMP_COMM_MOT_STEP) +5;
		aPosition /= 10;
		aPosition =  aPosition + OSIKOMI_PLS_A+ZURE_PLS_B + SRVC_GetNvmValueSC(NVMID_TAMPER_R_STPLE_ADJST);//NVM2;

		// 制御対象用紙が、コンパイル枚数Tamper用紙揃え押しこみ量追加の調整閾値枚目以上?
		if ( SRVC_GetCompileCount(aSheetNo) >= SRVC_GetNvmValueUC(NVMID_TAMPING_ADD_PUSH_PAPER_COUNT) )
			aPosition += SRVC_GetNvmValueSC(NVMID_TAMPING_ADD_PUSH_PULSE); //NVM3;

		//(CR-XXX)150716 : B5　シートずれ対策(StapleJob+ EjectTamping)
		if ( aAction ==  CMPL_TMP_EJECT_TAMPING ) {

			//aPosition += SRVC_GetNvmValueSC(NVMID_TAMPING_ADD_PUSH_PULSE_2); //NVM4;
			// (V3.5.102) 
			aSetLast_N = SRVC_GetNvmValueUC(NVMID_TAMPING_SETLAST_ADD_PULSE_ADJ_N);  //NVM4 
			
			if ( SRVC_GetCompileCount(aSheetNo) >= aSetLast_N ) {
				//Staple時のセットラストタンピングの調整 N 枚数以上時、Tamper用紙揃え押し込み量
				aPosition += SRVC_GetNvmValueSC(NVMID_TAMPING_SETLAST_ADD_PULSE_5); //NVM5 				
			}else {
				//Staple時のセットラストタンピングの調整 N 枚数未満時、Tamper用紙揃え押し込み量
				aPosition += SRVC_GetNvmValueSC(NVMID_TAMPING_SETLAST_ADD_PULSE_6);  //NVM6 
			}

			if ( aPosition < 0 )  
				aPosition = 0;
		}
		break;
	}
//	PrintTamperLog_Short(aAction, (US)aPosition, aSheetNo);  //Log
	return (US)aPosition;
}

///////////////////////////////////////////////////////////////////
// Function Name :CMPL_TMP_LEFT_GetNormalTampingTime
// Description   : Normal Tamping 動作時間
// Parameter     : aSheetNo
// Return        : aTime (動作時間)
///////////////////////////////////////////////////////////////////
US CMPL_TMP_LEFT_GetNormalTampingTime(UC aSheetNo)
{
	ECMPL_TMP_ProfileID aProfile;

	SS			 aMovePulse; //移動量
	SS			 aNvm; //NVM
	SL			 aParaSL1,  aParaSL2;  //中間計算用
	
	US 			aTime = 0;

	aMovePulse = (SS)CMPL_TMP_LEFT_GetTampingPosition(CMPL_TMP_NORMAL_TAMPING, aSheetNo) ;

	if (SRVC_GetStapleMode(aSheetNo) == eCdiFinStaple_Off) {
		aNvm = SRVC_GetNvmValueSS(NVMID_TAMPER_UN_STPLE_ADJST); //NVM1;
	} else { //StapleJob
		aNvm = SRVC_GetNvmValueSS(NVMID_TAMPER_R_STPLE_ADJST); //NVM2;
	}

	aProfile = CMPL_TMP_COMM_GetTampingProfile(aSheetNo);

	switch ( aProfile ) {
		case CMPL_TMP_PRF_ID_NORMAL_MID:
			aParaSL1  = (SL) (aMovePulse + aNvm - 26) ;
			if ( aParaSL1 < 0) { 
				aParaSL1 = 0;
			}	

			aParaSL2 = (SL)( aMovePulse - 60);
			if (aParaSL2 < 0) {
				aParaSL2 = 0;
			}
			//993[pps] 動作時間= ( (D＋NVM-26)×1.007+63.8)+( (D-60)×0.6798+94)	
			//	(D-60)の解が負となる場合、(D-60)の解を0とする。
			//	また、(D-26)の解が負となる場合も同様に、(D-26)の解を0とする。
			aTime = (US)((((aParaSL1 * 10.07 + 638) +(aParaSL2 * 6.798 + 940)) +9)/10);
		break;

		case CMPL_TMP_PRF_ID_NORMAL_HI:			
			aParaSL1  = (SL) (aMovePulse + aNvm - 60) ;
			if ( aParaSL1 < 0) {
				aParaSL1 = 0;
			}	
			//1471[pps] 動作時間= ( (D＋NVM-60)×0.6798+94)×2
			aTime = (US)((((aParaSL1 * 6.798 + 940) * 2) + 9)/10);
		break;

		default:
			aTime = 0;
		break;
	}
	return aTime;
}

///////////////////////////////////////////////////////////////////
// Function Name :CMPL_TMP_LEFT_GetEjectTampingTime
// Description   : Eject Tamping 動作時間
// Parameter     : aSheetNo
// Return        : aTime (動作時間)
///////////////////////////////////////////////////////////////////
US CMPL_TMP_LEFT_GetEjectTampingTime(UC aSheetNo)
{

	ECMPL_TMP_ProfileID aProfile;

	SS			 aMovePulse; //移動量
	SS			 aNvm; //NVM
	SS  			aNvmCount;
	SL			 aParaSL1,  aParaSL2;  //中間計算用
	
	US 			aTime = 0;
	US 			aLength = SRVC_GetProcessSize(aSheetNo);

	if ( aLength <= 2970 ) {
		aNvmCount = (SS)(SRVC_GetNvmValueUC(NVMID_SETLAST_TAMPING_NUM1)); // NVM_SETLAST_TAMPING_CNT_SMALL;
	} else {
		aNvmCount = (SS)(SRVC_GetNvmValueUC(NVMID_SETLAST_TAMPING_NUM2)); // NVM_SETLAST_TAMPING_CNT_BIG;
	}

	aMovePulse = (SS)CMPL_TMP_LEFT_GetTampingPosition(CMPL_TMP_EJECT_TAMPING, aSheetNo) ;

	if (SRVC_GetStapleMode(aSheetNo) == eCdiFinStaple_Off) {
		aNvm = SRVC_GetNvmValueSS(NVMID_TAMPER_UN_STPLE_ADJST); //NVM1;
	} else { //StapleJob
		aNvm = SRVC_GetNvmValueSS(NVMID_TAMPER_R_STPLE_ADJST); //NVM2;
	}

	aProfile = CMPL_TMP_COMM_GetTampingProfile(aSheetNo);

	switch ( aProfile ) {
		case CMPL_TMP_PRF_ID_NORMAL_MID:
			aParaSL1  = (SL) (aMovePulse + aNvm - 26) ;
			if ( aParaSL1 < 0) {
				aParaSL1 = 0;
			}
			aParaSL2 = (SL)(1096 * aNvmCount);
			
			//993[pps] 動作時間= ( (D＋NVM-26)×1.007+73.8)+109.6×揃え動作繰り返し回数
			aTime = (US)((((aParaSL1 * 10.07 + 738) + aParaSL2) +9)/10) ;
		break;

		case CMPL_TMP_PRF_ID_NORMAL_HI:			
			aParaSL1  = (SL) (aMovePulse + aNvm - 60) ;
			if ( aParaSL1 < 0) {
				aParaSL1 = 0;
			}
			aParaSL2 = (SL)(1096 * aNvmCount);
			//1471[pps] 動作時間=  ( (D＋NVM-60)×0.6798+104)+ 109.6×揃え動作繰り返し回数
			aTime = (US)((((aParaSL1 * 6.798 + 1040) + aParaSL2) +9) / 10);
		break;

		default:
			aTime = 0;
		break;
	}
	return aTime;

}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_LEFT_GetOffsetPosition
// Description   : Offset位置取得
// Parameter     : aSheetNo (対象シートナンバー)
// Return        : Offset 位置
///////////////////////////////////////////////////////////////////
US CMPL_TMP_LEFT_GetOffsetPosition(UC aSheetNo)
{
	US  aPosition=0;

	switch ( SRVC_GetStapleMode(aSheetNo) ) {

	case eCdiFinStaple_Off :
		if ( SRVC_GetOffsetFinish(aSheetNo) == SHT_OFST_FIN_LEFT ) {
			aPosition = 0;
		} else if ( SRVC_GetOffsetFinish(aSheetNo) == SHT_OFST_FIN_RIGHT ) {
			//CR010
			aPosition = SRVC_GetNvmValueUS(NVMID_TAMPER_OFFSET_PUSH_PULSE); //CMPL_TMP_COMM_OFFSET_PLS;
		}
		break;

	case eCdiFinStaple_SingleFront :
		if ( SRVC_GetOffsetFinish(aSheetNo) == SHT_OFST_FIN_LEFT ) {
			aPosition = SRVC_GetNvmValueUS(NVMID_TAMPER_OFFSET_PUSH_PULSE);  //CMPL_TMP_COMM_OFFSET_PLS;
		} else if ( SRVC_GetOffsetFinish(aSheetNo) == SHT_OFST_FIN_RIGHT ) {
			aPosition = 0;
		}
		break;

	default :																	// Irregular Case
			break;
	}

	return  aPosition;
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_LEFT_GetOffsetDirection
// Description   : Offset方向 取得
// Parameter     : aSheetNo (対象シートナンバー)
// Return        : Offset 方向
///////////////////////////////////////////////////////////////////
US CMPL_TMP_LEFT_GetOffsetDirection(UC aSheetNo)
{
	US  aDirection=0;

	switch ( SRVC_GetStapleMode(aSheetNo) ) {

	case eCdiFinStaple_Off :
		if ( SRVC_GetOffsetFinish(aSheetNo) == SHT_OFST_FIN_RIGHT ) {
			aDirection= CMPL_TMP_COMM_INSIDE;	//CCW
		}
		break;

	case eCdiFinStaple_SingleFront :
		if ( SRVC_GetOffsetFinish(aSheetNo) == SHT_OFST_FIN_LEFT ) {
			aDirection= CMPL_TMP_COMM_OUTSIDE; //CW
		}
		break;

	default :																	// Irregular Case
		break;
	}

	return  aDirection;
}



///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_LEFT_GetReturnPosition
// Description   : Return移動量 取得
// Parameter     : aSheetNo (対象シートナンバー)
// Return        : Return 移動量
///////////////////////////////////////////////////////////////////
US CMPL_TMP_LEFT_GetReturnPosition(UC aSheetNo)
{
	float  aPosition=0;
	float aWidth = (float)(SRVC_GetCrossProcessSize(aSheetNo));

	switch ( SRVC_GetStapleMode(aSheetNo) ) {

 //CR-010
	case eCdiFinStaple_Off :		
	case eCdiFinStaple_SingleFront :	
		// 10 pulse 移動
		  aPosition = 10;
		break;
	default:																	// Irregular Case
		break;
	}

	return  (US)aPosition;
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_LEFT_GetEmsForCurrentChange :( V3.0.4 ) 
// Description   : 用紙揃え及びSetLastTamping時のRT電流値切り替え時間：SlowUp＋定速時間
// Parameter     : ECMPL_TMP_Action   aAction
//			  UC 				 aSheetNo (対象シートナンバー)
// Return        : 移動時間(待ち時間) 
///////////////////////////////////////////////////////////////////
US CMPL_TMP_LEFT_GetEmsForCurrentChange(ECMPL_TMP_Action aAction, UC aSheetNo)
{
	US   aTime;
	SS   aMovePulse;
	ECMPL_TMP_ProfileID aProfile;
	
	aMovePulse = (SS)CMPL_TMP_LEFT_GetTampingPosition(aAction, aSheetNo) ;
	aProfile = CMPL_TMP_COMM_GetTampingProfile(aSheetNo);

	switch(aProfile) {
	case 	CMPL_TMP_PRF_ID_NORMAL_MID :
		if ( aMovePulse < 26) {
			aMovePulse = 26;
		}
		// (D-26)*1.007+(16.9*2)-20 = (((D-26)*10.07 + (169*2) - 200)+9)/10
		aTime = (US)(((((aMovePulse -26) * 10.07) + 338 - 200)  +9)/10);
	break;

	case 	CMPL_TMP_PRF_ID_NORMAL_HI :
	default :
		if ( aMovePulse < 60 ) {
			aMovePulse = 60;
		}
		// (D-60)*0.6798+(32*2)-20 = (((D-60)*6.798 + (320*2) - 200)+9)/10
		aTime = (US)(((((aMovePulse -60)  * 6.798) + 640 -200) +9)/10);
	break;
	}
	
	return   aTime	;
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_LEFT_GetLTCurrentChange: 
// Description   : 用紙揃え及びSetLastTamping時のLT電流値切り替え時間：SlowUp＋定速時間
// Parameter     : ECMPL_TMP_Action   aAction
//			  UC 				 aSheetNo (対象シートナンバー)
// Return        : 移動時間(待ち時間) 
///////////////////////////////////////////////////////////////////
US CMPL_TMP_LEFT_GetLTCurrentChange(ECMPL_TMP_Action aAction, UC aSheetNo)
{
	US   aTime;
	SS   aMovePulse;
	ECMPL_TMP_ProfileID aProfile;
	US    aNvm;
	
	aMovePulse = (SS)CMPL_TMP_LEFT_GetTampingPosition(aAction, aSheetNo) ;
	aProfile = CMPL_TMP_COMM_GetTampingProfile(aSheetNo);

	switch(aProfile) {
	case 	CMPL_TMP_PRF_ID_NORMAL_MID : //297.0mm超え
		if ( aMovePulse < 26) {
			aMovePulse = 26;
		}
		//NVM 
		if ( aAction == CMPL_TMP_NORMAL_TAMPING) {
			aNvm = (US)(SRVC_GetNvmValueUC(NVMID_TAMPING_NORMAL_LT_CURR_CHANGE_2));
		} else {
			aNvm = (US)(SRVC_GetNvmValueUC(NVMID_TAMPING_SETLAST_LT_CURR_CHANGE_2));
		}
	
		// (D-26)*1.007+(16.9*2)-(NVM*1.007) = ((((D-26)*10.07) + (169*2) - (NVM*10.07))+9)/10
		aTime = (US)((((((aMovePulse -26) * 10.07) + 338) - (aNvm*10.07) ) +9)/10);		
	break;

	case 	CMPL_TMP_PRF_ID_NORMAL_HI : //297.0mm以下
	default :
		if ( aMovePulse < 60 ) {
			aMovePulse = 60;
		}
		//NVM 
		if ( aAction == CMPL_TMP_NORMAL_TAMPING) {
			aNvm = (US)(SRVC_GetNvmValueUC(NVMID_TAMPING_NORMAL_LT_CURR_CHANGE_1));
		} else {
			aNvm = (US)(SRVC_GetNvmValueUC(NVMID_TAMPING_SETLAST_LT_CURR_CHANGE_1));
		}
	
		// (D-60)*0.6798+(32*2)-(NVM*0.6798) = (((D-60)*6.798 + (320*2) - (NVM*6.798))+9)/10
		aTime = (US)((((((aMovePulse -60)  * 6.798) + 640) -(aNvm*6.798) )  +9)/10);
	break;
	}

	PrintTamperLog_Short(CMPL_TMP_NORMAL_TAMPING+0x88, aNvm, aTime);	
	return   aTime	;
}


///////////////////////////////////////////////////////////////////
// Function Name : LT_Component_Mot_st1
// Description   : Mot 駆動設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_Component_Mot_st1()
{
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_DIR, gCMPL_TMP_LEFT.direction);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, HIGH);

	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_MOTOR_ENABLE_TIME_20, CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Component_Mot_st2
// Description   : 動作開始, 100Pulse 後, StepDown
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_Component_Mot_st2()
{
	DD_STM_SetProfileID(DD_STM_ID_LEFT_TAMPER,  gCMPL_TMP_LEFT.profile);
	DD_STM_MoveBySetPulse(DD_STM_ID_LEFT_TAMPER,  gCMPL_TMP_LEFT.targetIndex,
		                     CMPL_TMP_COMPO_TOTAL_PULSE, CMPL_TMP_LEFT_ReceiveSteppingMotorEvent);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Component_Mot_st3
// Description	 : 後励磁処理
// Parameter	 : ‐
// Return		 : ‐
///////////////////////////////////////////////////////////////////
void LT_Component_Mot_st3()
{
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_MOTOR_STOP_TIME_20, CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Component_Mot_st4
// Description   : Mot Off
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_Component_Mot_st4()
{
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_ENB, OFF);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_DIR, OFF);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, LOW);

	CMPL_TMP_InformedCompoComplete((US)gCMPL_TMP_LEFT.compoAction);

	gCMPL_TMP_LEFT.action = CMPL_TMP_st0;
	gCMPL_TMP_LEFT.sheetNo = SHEETNO_NUM;
}


// サイズポジショニング動作
// CMPL_TMP_SIZE_POS = 0x0200,
///////////////////////////////////////////////////////////////////
// Function Name : LT_SizePositioning_st1
// Description   : Tamper HomeSNR　ONチェック
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_SizePositioning_st1()
{
	// 1. Tamper Home SNR Level Check
	if ( DD_DIO_GetInput(DD_DI_LEFT_TAMPER_HOME_SNR) == HIGH )  // HIGH=
	   gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st2_CCW;   	// Home外し動作設定
	else
	   gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st2_CW;  		// Home動作設定

	CMPL_TMP_LEFT_ControlThread();
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_SizePositioning_st2_CCW
// Description   : Home外し動作設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_SizePositioning_st2_CCW()
{
	// Home外し動作チェックCount。2回以上ならばHomeSNROffFail
	if ( ++gCMPL_TMP_LEFT.offFailCheckCount > 1 ) {
		
		CMPL_TMP_InformedLeftTamperHomeSensorOffFail(gCMPL_TMP_LEFT.sheetNo);
		PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st2_CCW+0x80,  
							gCMPL_TMP_LEFT.offFailCheckCount, gCMPL_TMP_LEFT.offFailCheckCount);
	}

	// 内方向(CCW）で強電流設定
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_DIR, CMPL_TMP_COMM_INSIDE);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, HIGH);

	//LeftSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st2_CCW+0x80, CMPL_TMP_SIZE_POS_st2_CCW+0x80, 0);
	// 20ms Timer設定
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_MOTOR_ENABLE_TIME_20,
									CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_SizePositioning_st3_CCW
// Description   : CCW方向にモータ駆動開始
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_SizePositioning_st3_CCW()
{
     DD_STM_SetProfileID(DD_STM_ID_LEFT_TAMPER, DD_STM_PROF_ID_TAMPER_INITIALIZE);
	//LeftSizePos
     PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st3_CCW+0x80, CMPL_TMP_SIZE_POS_st3_CCW+0x80, 0);

	// INITIALIZE Profileで 総120(StepUp+Const) + 30(StepDown)パルス移動
     DD_STM_MoveBySetPulse(DD_STM_ID_LEFT_TAMPER,  CMPL_TMP_PROF_TARGET_STEP_NUM_INIT,
     						   CMPL_TMP_SIZE_POSITION_MOVE_PUSE_150, CMPL_TMP_LEFT_ReceiveSteppingMotorEvent );
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_SizePositioning_st4_CCW
// Description   : Home外し動作完了
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_SizePositioning_st4_CCW()
{
	// 50ms Timer設定
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_SWITCH_TIME_50, CMPL_TMP_LEFT_NORMAL_EVT);
	//LeftSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st4_CCW+0x80, CMPL_TMP_SIZE_POS_st4_CCW+0x80, 0);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_SizePositioning_st2_CW
// Description   : Home動作設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_SizePositioning_st2_CW()
{
	// 外方向(CW）で強電流設定
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_DIR, CMPL_TMP_COMM_OUTSIDE);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, HIGH);

	//V3.3.99  Tamper T/B
	if ( gCMPL_TMP_LEFT.sizePosAfterReturn == true ) {
		// 20ms Timer設定 
		CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_MOTOR_ENABLE_TIME_20,
										CMPL_TMP_LEFT_NORMAL_EVT);

	} else {
		//V3.3.93  騒音対策　20ms→200msに変更
		CMPL_TMP_LEFT_SetEventTimer(200, 	CMPL_TMP_LEFT_NORMAL_EVT);

	}
	//LeftSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st2_CW+0x80, CMPL_TMP_SIZE_POS_st2_CW+0x80, 0);
		
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_SizePositioning_st3_CW
// Description   : CW方向にモータ駆動開始
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_SizePositioning_st3_CW()
{
  // Fail Timer設定
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_FAIL_TIME_500, CMPL_TMP_LEFT_FAIL_EVT);

 	gCMPL_TMP_LEFT.interruptIndex = 	DD_INT_EntryCallback(DD_INT_LEFT_TAMPER_HOME_SENSOR,
										TMP_RISE, CMPL_TMP_LEFT_InterruptLTHomeSensorON, 0);  // Intrupt Set

  	DD_STM_SetProfileID(DD_STM_ID_LEFT_TAMPER, DD_STM_PROF_ID_TAMPER_INITIALIZE);
  	DD_STM_StartClock(DD_STM_ID_LEFT_TAMPER, CMPL_TMP_PROF_TARGET_STEP_NUM_INIT, CMPL_TMP_LEFT_ReceiveSteppingMotorEvent);
	//LeftSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st3_CW+0x80, CMPL_TMP_SIZE_POS_st3_CW+0x80, 0);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_SizePositioning_st4_CW
// Description   : StepUp中、割込み受信->SlowDown-> 50ms Timerset->Home外し動作
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_SizePositioning_st4_CW()
{
	// StepDown
	DD_STM_SlowDownStop(DD_STM_ID_LEFT_TAMPER, CMPL_TMP_LEFT_ReceiveSteppingMotorEvent );
	//LeftSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st4_CW+0x80, CMPL_TMP_SIZE_POS_st4_CW+0x80, 0);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_SizePositioning_st4_2_CW
// Description   : StepUp中、割込み受信 から StepDown完了
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_SizePositioning_st4_2_CW() // StepUp中、割込み受信 から StepDown完了
{
	// 50ms Timer設定
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_SWITCH_TIME_50, CMPL_TMP_LEFT_NORMAL_EVT);
	//LeftSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st4_2_CW+0x80, CMPL_TMP_SIZE_POS_st4_2_CW+0x80, 0);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_SizePositioning_st5_CW
// Description   : //StepUp完了受信
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_SizePositioning_st5_CW()
{
	// Nothing to do... No call .
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_SizePositioning_st6_CW
// Description   :  //安定区間で割込み受信
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_SizePositioning_st6_CW()
{
	// StepDown
	DD_STM_SlowDownStop(DD_STM_ID_LEFT_TAMPER, CMPL_TMP_LEFT_ReceiveSteppingMotorEvent);
	//LeftSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st6_CW+0x80, CMPL_TMP_SIZE_POS_st6_CW+0x80, 0);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_SizePositioning_st7_CW
// Description   ://Home動作完了
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_SizePositioning_st7_CW()
{
	// 50ms Timer設定
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_SWITCH_TIME_50, CMPL_TMP_LEFT_NORMAL_EVT);
	//LeftSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st7_CW+0x80, CMPL_TMP_SIZE_POS_st7_CW+0x80, 0);

	gCMPL_TMP_LEFT.offFailCheckCount = 0;  //Home外し動作チェックCount Clear(V3.5.95)
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_SizePositioning_st8
// Description   :  //SizePositioning開始
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_SizePositioning_st8()
{
	UC    sheetNo = gCMPL_TMP_LEFT.sheetNo;
		
	if ( gCMPL_TMP_LEFT.movePulse == 0 ) {// 　移動量無し→終了

		DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, LOW);

		gCMPL_TMP_LEFT.action = CMPL_TMP_st0;
		gCMPL_TMP_LEFT.sheetNo = SHEETNO_NUM;
	//CR-008
		if ( gCMPL_TMP_LEFT.sizePosAfterReturn == true ) {
			
			gCMPL_TMP_LEFT.sizePosAfterReturn = false;
			CMPL_TMP_InformedLeftTamperComplete(CMPL_TMP_LEFT_RETURN,	 sheetNo);
			//Return
			PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st8+0x80, 1,0xFF);		

		} else {
		
			CMPL_TMP_InformedLeftTamperComplete(CMPL_TMP_SIZE_POS,	 sheetNo);
			//LeftSizePos
			PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st8+0x80, 0, 0x00);
		}
		return;
	}

	//　移動量 有り
	// DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_DIR, gCMPL_TMP_LEFT.direction);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_DIR, CMPL_TMP_COMM_INSIDE);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, HIGH);

	//V3.4.0
	if ( gCMPL_TMP_LEFT.sizePosAfterReturn == true ) {
		// 20ms Timer設定
		CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_MOTOR_ENABLE_TIME_20, CMPL_TMP_LEFT_NORMAL_EVT);

	} else {
		//V3.3.93  騒音対策　20ms→200msに変更
		CMPL_TMP_LEFT_SetEventTimer(200,	CMPL_TMP_LEFT_NORMAL_EVT);

	}	
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_SizePositioning_st9
// Description   : モータ駆動開始
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_SizePositioning_st9()
{
	// A-Pulse移動
	DD_STM_MoveBySetPulse(DD_STM_ID_LEFT_TAMPER, CMPL_TMP_PROF_TARGET_STEP_NUM_INIT,
	                         gCMPL_TMP_LEFT.movePulse, CMPL_TMP_LEFT_ReceiveSteppingMotorEvent );
	//LeftSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st9+0x80, CMPL_TMP_SIZE_POS_st9+0x80, 0x80);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_SizePositioning_st10
// Description   : SizePositionへ移動完了
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_SizePositioning_st10()
{
	UC     sheetNo = gCMPL_TMP_LEFT.sheetNo;
	
	// LeftTamper HomeSNR OFF Fail Check (33 Pulse 　以上 & HOME SNR=ON)
	if ((gCMPL_TMP_LEFT.movePulse >= 33) && (DD_DIO_GetInput(DD_DI_LEFT_TAMPER_HOME_SNR) == HIGH )) {
		CMPL_TMP_InformedLeftTamperHomeSensorOffFail(gCMPL_TMP_LEFT.sheetNo);
		return;
	}

	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, LOW);

	gCMPL_TMP_LEFT.action = CMPL_TMP_st0;
	gCMPL_TMP_LEFT.sheetNo = SHEETNO_NUM;

	  //CR-008
	if ( gCMPL_TMP_LEFT.sizePosAfterReturn == true ) {
		
		gCMPL_TMP_LEFT.sizePosAfterReturn = false;
		CMPL_TMP_InformedLeftTamperComplete(CMPL_TMP_LEFT_RETURN,	 sheetNo);
		//LeftSizePos
		PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st10+0x80, 1, 0xFF);		

	} else {
	
		CMPL_TMP_InformedLeftTamperComplete(CMPL_TMP_SIZE_POS,	 sheetNo);
		//LeftSizePos
		PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st10+0x80, 0, 0x00);
	}

}


// ノーマルタンピング動作
// CMPL_TMP_NORMAL_TAMPING = 0x0300,
///////////////////////////////////////////////////////////////////
// Function Name : LT_NormalTamping_st1
// Description   : 動作開始遅延時間設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_NormalTamping_st1()
{
	 //CR-008 
	gCMPL_TMP_LEFT.action = CMPL_TMP_NORMAL_TAMPING_st2;
	CMPL_TMP_LEFT_ControlThread();

}

///////////////////////////////////////////////////////////////////
// Function Name : LT_NormalTamping_st2
// Description   : モータ駆動設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_NormalTamping_st2()
{
	UC    sheetNo = gCMPL_TMP_LEFT.sheetNo;
	// 移動量=0の場合、終了
	if ( gCMPL_TMP_LEFT.movePulse == 0 ) { //

		gCMPL_TMP_LEFT.action = CMPL_TMP_st0;
		gCMPL_TMP_LEFT.sheetNo = SHEETNO_NUM;
		CMPL_TMP_InformedLeftTamperComplete(CMPL_TMP_NORMAL_TAMPING, sheetNo);
		return;
	}
	// 1. 強電流で内（CCW)方向設定
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_DIR, gCMPL_TMP_LEFT.direction);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, HIGH);

	// 2. 20msTimerSet
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_MOTOR_ENABLE_TIME_20, 
	 								 CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_NormalTamping_st3
// Description   : モータ駆動開始
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_NormalTamping_st3()
{
//  (V3.0.4)　　SlowUpしてからEms以降、50msまで強電流にする
	US   E_ms =  CMPL_TMP_LEFT_GetEmsForCurrentChange(CMPL_TMP_NORMAL_TAMPING, gCMPL_TMP_LEFT.sheetNo);

// (V3.5.103) LeftTamper 電流値切り替え時間
	US   Nt = CMPL_TMP_LEFT_GetLTCurrentChange(CMPL_TMP_NORMAL_TAMPING,  gCMPL_TMP_LEFT.sheetNo);

	// time, callback, evtType
	gCMPL_TMP_LEFT.timerIndex_TimeUpControl  =MSP_SetEventTimer(E_ms, CMPL_TMP_LEFT_TimeUpControl, eNORMAL_EMS);
	gCMPL_TMP_LEFT.timerIndex_TimeUpControl2  =MSP_SetEventTimer(Nt, CMPL_TMP_LEFT_TimeUpControl2, eNORMAL_EMS);
//　↑↑↑↑↑↑
	DD_STM_SetProfileID(DD_STM_ID_LEFT_TAMPER, gCMPL_TMP_LEFT.profile);
 	DD_STM_MoveBySetPulse(DD_STM_ID_LEFT_TAMPER, gCMPL_TMP_LEFT.targetIndex,
							gCMPL_TMP_LEFT.movePulse, CMPL_TMP_LEFT_ReceiveSteppingMotorEvent);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_NormalTamping_st4
// Description   : Tamping位置へ移動完了
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_NormalTamping_st4()
{
	// 10msTimerSet
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_SWITCH_TIME_10, 
	                                 CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_NormalTamping_st5
// Description   : Return方向へ切り替え
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_NormalTamping_st5()
{
	// 駆動方向=外に変更
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_DIR, CMPL_TMP_COMM_OUTSIDE);

	//V3.5.103 LT 電流値切り替え LOW->HIGH
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, HIGH);
	
	// 10msTimerSet
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_SWITCH_TIME_10,
					 			     CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_NormalTamping_st6
// Description   : モータ駆動(Return)開始
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_NormalTamping_st6()
{
	// 高速Profile
	DD_STM_SetProfileID(DD_STM_ID_LEFT_TAMPER,  DD_STM_PROF_ID_TAMPER_NORMAL_HI);

	DD_STM_MoveBySetPulse(DD_STM_ID_LEFT_TAMPER, gCMPL_TMP_LEFT.targetIndex,
							 gCMPL_TMP_LEFT.movePulse, CMPL_TMP_LEFT_ReceiveSteppingMotorEvent);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_NormalTamping_st7
// Description   : NormalTamping完了
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_NormalTamping_st7()
{
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_MOTOR_STOP_TIME_20,
				 					CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_NormalTamping_st8
// Description   : 次の動作待ち設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_NormalTamping_st8()
{
	UC    sheetNo = gCMPL_TMP_LEFT.sheetNo;
	
	// 弱電流設定
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, LOW);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, LOW);  //念のため、追加.
	
	gCMPL_TMP_LEFT.action = CMPL_TMP_st0;
	gCMPL_TMP_LEFT.sheetNo = SHEETNO_NUM;

	CMPL_TMP_InformedLeftTamperComplete(CMPL_TMP_NORMAL_TAMPING,  sheetNo);
	PrintTamperLog_Short(CMPL_TMP_NORMAL_TAMPING_st8, CMPL_TMP_NORMAL_TAMPING_st8, 0);

}



// イジェクトタンピング動作(SetLastTamping)
// CMPL_TMP_EJECT_TAMPING = 0x0400,
///////////////////////////////////////////////////////////////////
// Function Name : LT_EjectTamping_st1
// Description   : 動作開始遅延時間設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_EjectTamping_st1()
{
	CMPL_TMP_LEFT_SetEventTimer(gCMPL_TMP_LEFT.startDelayTime,
						 			 CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_EjectTamping_st2
// Description   : モータ駆動設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_EjectTamping_st2()
{
	UC	sheetNo = gCMPL_TMP_LEFT.sheetNo;
	
	// 移動量=0の場合、終了
	if ( gCMPL_TMP_LEFT.movePulse == 0 ) {  //

		gCMPL_TMP_LEFT.action = CMPL_TMP_st0;
		gCMPL_TMP_LEFT.sheetNo = SHEETNO_NUM;
	
		CMPL_TMP_InformedLeftTamperComplete(CMPL_TMP_EJECT_TAMPING,  sheetNo);
		return;
	}
	// 1. 強電流で内（CCW)方向設定
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_DIR, gCMPL_TMP_LEFT.direction);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, HIGH);

	// 2. 20msTimerSet
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_MOTOR_ENABLE_TIME_20,
					 			     CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_EjectTamping_st3
// Description   : モータ駆動開始
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_EjectTamping_st3()
{
//	(V3.0.4)　　SlowUpしてからEms以降、50msまで強電流にする
	US	 E_ms =  CMPL_TMP_LEFT_GetEmsForCurrentChange(CMPL_TMP_EJECT_TAMPING, gCMPL_TMP_LEFT.sheetNo);
// (V3.5.103) LeftTamper 電流値切り替え時間
	US	 Nt = CMPL_TMP_LEFT_GetLTCurrentChange(CMPL_TMP_EJECT_TAMPING,  gCMPL_TMP_LEFT.sheetNo);

	// time, callback, evtType
	gCMPL_TMP_LEFT.timerIndex_TimeUpControl = MSP_SetEventTimer(E_ms, CMPL_TMP_LEFT_TimeUpControl, e1stEJECT_EMS);
	gCMPL_TMP_LEFT.timerIndex_TimeUpControl2 = MSP_SetEventTimer(Nt, CMPL_TMP_LEFT_TimeUpControl2, e1stEJECT_EMS);	
//　↑↑↑↑↑↑

	DD_STM_SetProfileID(DD_STM_ID_LEFT_TAMPER, gCMPL_TMP_LEFT.profile);
	DD_STM_MoveBySetPulse(DD_STM_ID_LEFT_TAMPER, gCMPL_TMP_LEFT.targetIndex,
							  gCMPL_TMP_LEFT.movePulse,  CMPL_TMP_LEFT_ReceiveSteppingMotorEvent);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_EjectTamping_st4
// Description   : Tamping位置へ移動完了
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_EjectTamping_st4()
{
	// 10msTimerSet
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_SWITCH_TIME_10,
					 			     CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_EjectTamping_st5
// Description   : Return方向へ切り替え
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_EjectTamping_st5()
{
	// 駆動方向=外に変更
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_DIR, CMPL_TMP_COMM_OUTSIDE);

	//V3.5.103 LT 電流値切り替え LOW->HIGH
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, HIGH);
	
	// 10msTimerSet
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_SWITCH_TIME_10,
					 			     CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_EjectTamping_st6
// Description   : モータ駆動(Return)開始
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_EjectTamping_st6()
{
	//中速Profileで27Pulseを外方向に移動
	DD_STM_SetProfileID(DD_STM_ID_LEFT_TAMPER, DD_STM_PROF_ID_TAMPER_NORMAL_MID);
	gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_MID;
	
	DD_STM_MoveBySetPulse(DD_STM_ID_LEFT_TAMPER, gCMPL_TMP_LEFT.targetIndex,
							  27, CMPL_TMP_LEFT_ReceiveSteppingMotorEvent);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_EjectTamping_st7
// Description   : Return動作完了
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_EjectTamping_st7()
{
	// 10msTimerSet
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_SWITCH_TIME_10,
					 			     CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_EjectTamping_st8
// Description   : SetLastTamping開始
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_EjectTamping_st8()
{
	// 駆動方向= 内に変更
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_DIR, CMPL_TMP_COMM_INSIDE);
	// 10msTimerSet
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_SWITCH_TIME_10,
					 			     CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_EjectTamping_st9
// Description   : モータ駆動(SetLast)開始
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void  LT_EjectTamping_st9()
{
//	(V3.0.4)　SlowUpしてからEms以降、50msまで強電流にする
	US	 E_ms = 14;  //14ms

	// time, callback, evtType
	gCMPL_TMP_LEFT.timerIndex_TimeUpControl = MSP_SetEventTimer(E_ms, CMPL_TMP_LEFT_TimeUpControl, e2ndEJECT_EMS);
//　↑↑↑↑↑↑

	// 中速Profileで27Pulseを外方向に移動
	DD_STM_SetProfileID(DD_STM_ID_LEFT_TAMPER, DD_STM_PROF_ID_TAMPER_NORMAL_MID);
	gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_MID;

	DD_STM_MoveBySetPulse(DD_STM_ID_LEFT_TAMPER, gCMPL_TMP_LEFT.targetIndex,
							  27, CMPL_TMP_LEFT_ReceiveSteppingMotorEvent);

}

///////////////////////////////////////////////////////////////////
// Function Name : LT_EjectTamping_st10
// Description   : SetLast動作完了
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void  LT_EjectTamping_st10()
{
	// 10msTimerSet
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_SWITCH_TIME_10,
					 			     CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_EjectTamping_st11
// Description   : SetLast繰り返しチェック
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void  LT_EjectTamping_st11()
{
	US aLength = SRVC_GetProcessSize(gCMPL_TMP_LEFT.sheetNo);
	US  aNvmCount;

	gCMPL_TMP_LEFT.ejectTampingCnt++;

	if ( aLength <= 2970 ) {
		aNvmCount = SRVC_GetNvmValueUC(NVMID_SETLAST_TAMPING_NUM1); // NVM_SETLAST_TAMPING_CNT_SMALL;
	} else {
		aNvmCount = SRVC_GetNvmValueUC(NVMID_SETLAST_TAMPING_NUM2); // NVM_SETLAST_TAMPING_CNT_BIG;
	}

	if ( gCMPL_TMP_LEFT.ejectTampingCnt >= aNvmCount) { // SetLastTampingCount >=  NVM
		gCMPL_TMP_LEFT.action = CMPL_TMP_EJECT_TAMPING_st12;
	} else {
		gCMPL_TMP_LEFT.action = CMPL_TMP_EJECT_TAMPING_st5;
	}
	CMPL_TMP_LEFT_ControlThread();
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_EjectTamping_st12
// Description   : 次の動作待ち設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void  LT_EjectTamping_st12()
{
	UC   sheetNo =  gCMPL_TMP_LEFT.sheetNo;
	// 弱電流設定
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, LOW);

	gCMPL_TMP_LEFT.action = CMPL_TMP_st0;
	gCMPL_TMP_LEFT.sheetNo = SHEETNO_NUM;
	gCMPL_TMP_LEFT.ejectTampingCnt = 0;

	CMPL_TMP_InformedLeftTamperComplete(CMPL_TMP_EJECT_TAMPING, sheetNo);
	PrintTamperLog_Short(CMPL_TMP_EJECT_TAMPING_st12, 0xFFFF, 0xFF);
}

//V3.5.93 ミスレジ対策
///////////////////////////////////////////////////////////////////
// Function Name : LT_MissRegi_st1
// Description  // MissRegi_動作Timer設定 
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
 void  LT_MissRegi_st1()
{
	US  timer =0;	

	gCMPL_TMP_LEFT.movePulse= (US)(SRVC_GetNvmValueUC(NVMID_TAMPER_MISSREGI_MOT_PULSE));

	if ( gCMPL_TMP_LEFT.movePulse < 1) {
		
		gCMPL_TMP_LEFT.action = CMPL_TMP_MISSREGI_st4; //MissRegi_完了
		CMPL_TMP_LEFT_ControlThread();	
		
	} else {	

		//Timer set..
		timer = (US)(SRVC_GetNvmValueUC(NVMID_TAMPER_MISSREGI_START_TIME));
		timer = timer * 10;  //10ms 

		if ( timer <  1 ) {		
			gCMPL_TMP_LEFT.action = CMPL_TMP_MISSREGI_st2;
			CMPL_TMP_LEFT_ControlThread();	

		} else {
			DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_ENB, ON);
			// 駆動方向=外
			DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_DIR, CMPL_TMP_COMM_OUTSIDE);
			// 強電流
			DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, HIGH); 
			
			CMPL_TMP_LEFT_SetEventTimer(timer,  CMPL_TMP_LEFT_NORMAL_EVT);
			
			PrintTamperLog_Short(CMPL_TMP_MISSREGI_st1,  timer, 0xFF);
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_MissRegi_st2
// Description   :  MissRegi_指定パルス出力 
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
 void  LT_MissRegi_st2()
{
//	gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_LOW;
//	gCMPL_TMP_LEFT.targetIndex = 0;  //1step : 250PPS

	DD_STM_SetProfileID(DD_STM_ID_LEFT_TAMPER, gCMPL_TMP_LEFT.profile);
	DD_STM_MoveBySetPulse(DD_STM_ID_LEFT_TAMPER, gCMPL_TMP_LEFT.targetIndex,
						  gCMPL_TMP_LEFT.movePulse,  CMPL_TMP_LEFT_ReceiveSteppingMotorEvent);

	PrintTamperLog_Short(CMPL_TMP_MISSREGI_st2,    gCMPL_TMP_LEFT.movePulse, gCMPL_TMP_LEFT.targetIndex);
}


///////////////////////////////////////////////////////////////////
// Function Name : LT_MissRegi_st3
// Description   : // MissRegi_後励磁設定 
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void  LT_MissRegi_st3()
{
	//20ms 
	CMPL_TMP_LEFT_SetEventTimer(20,  CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_MissRegi_st4
// Description   : // MissRegi_完了
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void  LT_MissRegi_st4()
{
	UC   sheetNo =  gCMPL_TMP_LEFT.sheetNo;
	// 弱電流設定
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, LOW);

	gCMPL_TMP_LEFT.action = CMPL_TMP_st0;
	gCMPL_TMP_LEFT.sheetNo = SHEETNO_NUM;
	
	CMPL_TMP_InformedLeftTamperComplete(CMPL_TMP_MISSREGI, sheetNo);
	PrintTamperLog_Short(CMPL_TMP_MISSREGI_st4, CMPL_TMP_MISSREGI_st4, 0xFF);

}


// オフセット動作
// CMPL_TMP_OFFSET = 0x0500,
///////////////////////////////////////////////////////////////////
// Function Name : LT_Offset_st1
// Description   : 開始Timing調整
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_Offset_st1()
{
	UC   sheetNo = gCMPL_TMP_LEFT.sheetNo;
	
	// 移動量=0の場合、終了
	if ( gCMPL_TMP_LEFT.movePulse == 0 ) { //

		gCMPL_TMP_LEFT.action = CMPL_TMP_st0;
		gCMPL_TMP_LEFT.sheetNo = SHEETNO_NUM;
	
		CMPL_TMP_InformedLeftTamperComplete(CMPL_TMP_OFFSET, sheetNo);
		return;
	}

	// Tamper駆動方向によって制御を変える
	if ( gCMPL_TMP_LEFT.direction == CMPL_TMP_COMM_INSIDE ) {  // 内（CCW)方向
		// 20msTimerSet
		CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_START_DELAY_20, CMPL_TMP_LEFT_NORMAL_EVT);
	} else {   // 外(CW)方向
		gCMPL_TMP_LEFT.action = CMPL_TMP_OFFSET_st2;
		CMPL_TMP_LEFT_ControlThread();
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Offset_st2
// Description   : モータ駆動設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_Offset_st2()
{
	// 1. 強電流で OFFSET方向設定
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_DIR, gCMPL_TMP_LEFT.direction);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, HIGH);

	// 2. 10msTimerSet
	CMPL_TMP_LEFT_SetEventTimer( CMPL_TMP_COMM_SWITCH_TIME_10, CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Offset_st3
// Description   : モータ駆動開始
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_Offset_st3()
{
	// 低速Profile
	DD_STM_SetProfileID(DD_STM_ID_LEFT_TAMPER, gCMPL_TMP_LEFT.profile);
	DD_STM_MoveBySetPulse(DD_STM_ID_LEFT_TAMPER, gCMPL_TMP_LEFT.targetIndex,
							  gCMPL_TMP_LEFT.movePulse, CMPL_TMP_LEFT_ReceiveSteppingMotorEvent);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Offset_st4
// Description	 : 安定時間タイマ設定
// Parameter	 : ‐
// Return		 : ‐
///////////////////////////////////////////////////////////////////
void LT_Offset_st4()
{
	// Tamper駆動方向によって制御を変える
	if ( gCMPL_TMP_LEFT.direction == CMPL_TMP_COMM_INSIDE) {  // 内（CCW)方向
		// 10msTimerSet
		CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_MOTOR_STOP_TIME_10,
										 CMPL_TMP_LEFT_NORMAL_EVT);
	} else {   //外(CW)方向
		// 20msTimerSet
		CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_MOTOR_STOP_TIME_20,
										 CMPL_TMP_LEFT_NORMAL_EVT);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Offset_st5
// Description   : 次動作待ち
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_Offset_st5()
{
	UC   sheetNo =  gCMPL_TMP_LEFT.sheetNo;
	// 弱電流設定
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, LOW);

	gCMPL_TMP_LEFT.action = CMPL_TMP_st0;
	gCMPL_TMP_LEFT.sheetNo = SHEETNO_NUM;

	CMPL_TMP_InformedLeftTamperComplete(CMPL_TMP_OFFSET, sheetNo);
	PrintTamperLog_Short(CMPL_TMP_OFFSET_st5+0x80, CMPL_TMP_OFFSET_st5+0x80, 0xFF);
}

// リターン動作
// CMPL_TMP_LEFT_RETURN = 0x0600,
///////////////////////////////////////////////////////////////////
// Function Name : LT_Return_st1
// Description   : Eject 動作開始Trigger待ち
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_Return_st1()
{
	UC   sheetNo =  gCMPL_TMP_LEFT.sheetNo;

	// 移動量=0の場合、終了
	if ( gCMPL_TMP_LEFT.movePulse == 0 ) {  //

		gCMPL_TMP_LEFT.action = CMPL_TMP_st0;
		gCMPL_TMP_LEFT.sheetNo = SHEETNO_NUM;
		gCMPL_TMP_LEFT.sizePosAfterReturn = false;
		CMPL_TMP_InformedLeftTamperComplete(CMPL_TMP_LEFT_RETURN, sheetNo);
		return;
	}

	if ( CMPL_TMP_IsEjectStart() == true ) { // Eject 動作開始
		gCMPL_TMP_LEFT.action = CMPL_TMP_LEFT_RETURN_st2;
		CMPL_TMP_LEFT_ControlThread();
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Return_st2
// Description   : タイマ設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_Return_st2()
{
#if  1 //(CR-008) 
	gCMPL_TMP_LEFT.action = CMPL_TMP_LEFT_RETURN_st3;
	CMPL_TMP_LEFT_ControlThread();
#else
	// LeftTamperは外方向のみ - 
	CMPL_TMP_LEFT_SetEventTimer(20+(SRVC_GetNvmValueUS(NVMID_TAMPER_RETURN_OUT_START_TIME)*10),
					 			     CMPL_TMP_LEFT_NORMAL_EVT);
#endif
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Return_st3
// Description   : モータ駆動設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_Return_st3()
{
	// 1.. 弱電流で内（CCW)方向設定
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_DIR, gCMPL_TMP_LEFT.direction);
	//Motor脱調対策(CR-008) TimerSet前、強電流
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, HIGH); //LOW);

	// 2. 20msTimerSet
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_MOTOR_ENABLE_TIME_20,
					 			     CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Return_st4
// Description   ; モータ駆動開始
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_Return_st4()
{
	// 1. 強電流
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, HIGH);
	DD_STM_SetProfileID(DD_STM_ID_LEFT_TAMPER, gCMPL_TMP_LEFT.profile);
	DD_STM_MoveBySetPulse(DD_STM_ID_LEFT_TAMPER, gCMPL_TMP_LEFT.targetIndex,
	                    	  gCMPL_TMP_LEFT.movePulse, CMPL_TMP_LEFT_ReceiveSteppingMotorEvent);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Return_st5
// Description   : タイマ設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_Return_st5()
{
	// 20msTimerSet
	CMPL_TMP_LEFT_SetEventTimer( CMPL_TMP_COMM_MOTOR_STOP_TIME_20, CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Return_st6
// Description   : 次動作待ち
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_Return_st6()
{
	UC    sheetNo = gCMPL_TMP_LEFT.sheetNo;	
	US   nvm_time;
	
	//弱電流設定
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, LOW);
	
//CR-008
	if ( gCMPL_TMP_LEFT.sizePosAfterReturn == true) {  //SizePos開始 
		
		// Timer Set ...
		switch ( SRVC_GetStapleMode(sheetNo) ) {

			case  eCdiFinStaple_Off : 
				nvm_time = (US)(SRVC_GetNvmValueUS(NVMID_TAMPER_SIZE_OUT_UNSTAPLE_ST_T_ADJ) * 10);  //(10ms)
				CMPL_TMP_LEFT_SetEventTimer(nvm_time ,  0);
				break;

			case  eCdiFinStaple_SingleFront :
			default :	
				nvm_time = (US)(SRVC_GetNvmValueUS(NVMID_TAMPER_SIZE_OUT_STAPLE_ST_T_ADJ) * 10);  //(10ms)
				CMPL_TMP_LEFT_SetEventTimer( nvm_time , 0 );
				break;

		}
		
	} else { 
		
		gCMPL_TMP_LEFT.action = CMPL_TMP_st0;
		gCMPL_TMP_LEFT.sheetNo = SHEETNO_NUM;
		
		CMPL_TMP_InformedLeftTamperComplete(CMPL_TMP_LEFT_RETURN,sheetNo);
		PrintTamperLog_Short(CMPL_TMP_LEFT_RETURN_st6, CMPL_TMP_LEFT_RETURN_st6, 0xFF);
	}
	
}



///////////////////////////////////////////////////////////////////
// Function Name : LT_Return_st7
// Description	 :	SizePos開始(CR-008) 
// Parameter	 : ‐
// Return		 : ‐
///////////////////////////////////////////////////////////////////
void LT_Return_st7()
{

	// LTはすべてのケースでSizePositioning実施する - FlagCheck しない
	CMPL_TMP_LEFT_StartAct(CMPL_TMP_SIZE_POS, gCMPL_TMP_LEFT.sheetNo);
		
}

// オープン動作(Nigasi)
// CMPL_TMP_OPEN = 0x0700,
///////////////////////////////////////////////////////////////////
// Function Name : LT_Open_st1
// Description   : モータ駆動設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_Open_st1()
{
	UC  sheetNo =  gCMPL_TMP_LEFT.sheetNo;
	
	// 逃がし動作開始チェック
	if ( (DD_DIO_GetInput(DD_DI_LEFT_TAMPER_HOME_SNR) == HIGH) ||
	      (SRVC_GetCrossProcessSizeMixSet(gCMPL_TMP_LEFT.sheetNo) != SHT_SIZE_MIX) ) { 	// Mix Size

		 gCMPL_TMP_LEFT.action = CMPL_TMP_st0;
		gCMPL_TMP_LEFT.sheetNo = SHEETNO_NUM;
		CMPL_TMP_InformedLeftTamperComplete(CMPL_TMP_OPEN, sheetNo);
		return;
	}

	// 1. 強電流で外(CW)方向設定
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_DIR, gCMPL_TMP_LEFT.direction);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, HIGH);

	// 2. 20msTimerSet
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_MOTOR_ENABLE_TIME_20,
					 			     CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Open_st2
// Description   : モータ駆動開始
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_Open_st2()
{
	// 480ms Fail Timer (Left Tamper Home SNR ON Fail)
	CMPL_TMP_LEFT_SetEventTimer( CMPL_TMP_COMM_SNR_ON_FAIL_TIME,  CMPL_TMP_LEFT_FAIL_EVT);

	gCMPL_TMP_LEFT.interruptIndex = 	DD_INT_EntryCallback(DD_INT_LEFT_TAMPER_HOME_SENSOR,
										TMP_RISE, CMPL_TMP_LEFT_InterruptLTHomeSensorON, 0);  // Intrupt Set

    	DD_STM_SetProfileID(DD_STM_ID_LEFT_TAMPER,  gCMPL_TMP_LEFT.profile);
	DD_STM_StartClock(DD_STM_ID_LEFT_TAMPER,  gCMPL_TMP_LEFT.profile, NULL);  // no callback
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Open_st3
// Description   : 割り込み受信後、StepDown
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_Open_st3()
{
	// StepDown
	DD_STM_SlowDownStop(DD_STM_ID_LEFT_TAMPER, CMPL_TMP_LEFT_ReceiveSteppingMotorEvent);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Open_st4
// Description   : タイマ設定
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_Open_st4()
{
	// 20msTimerSet
	CMPL_TMP_LEFT_SetEventTimer( CMPL_TMP_COMM_MOTOR_STOP_TIME_20, CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Open_st5
// Description   : 次動作待ち
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void LT_Open_st5()
{
	UC   sheetNo = gCMPL_TMP_LEFT.sheetNo;
	// 弱電流設定
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, LOW);

	gCMPL_TMP_LEFT.action = CMPL_TMP_st0;
	gCMPL_TMP_LEFT.sheetNo = SHEETNO_NUM;

	CMPL_TMP_InformedLeftTamperComplete(CMPL_TMP_OPEN,  sheetNo);
	PrintTamperLog_Short(CMPL_TMP_OPEN_st5+0x80, CMPL_TMP_OPEN_st5+0x80, 0xFF);
}


// File Name:	TRAY_STK_Device.c
// Description:	Stacker Device
// Engineer:	KimYS
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
#include "TRAY_STK_Device.h"
#include "TRAY_Manager.h"
#include "NvmService.h"

#include "CNvmInformer.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////
#define STK_DIR_UP					OFF //ON
#define STK_DIR_DOWN				ON  //OFF

#define STK_MOT_ENABLE_TIME_20	20	//
#define STK_MOT_ENABLE_TIME_30	30	//
#define STK_MOT_STOP_TIME_20  	20	//
#define STK_MOT_SWITCH_TIME_50	50
#define STK_FAIL_TIME_500 			500
#define STK_FAIL_TIME_1000 		1000
#define STK_FAIL_TIME_5000 		5000

//MotStepUp中、SNRイベント発生処理のため
#define  START_CLK_UNKNOWN   		0
#define  START_CLK_WAIT    			0x80
#define  START_CLK_RECV     			0xFF

#define PLS_SCOPE    					11  // Pulse数範囲
#define PROF_PULSE_STEP_45			44


// Profile table of "Stacker Profile "
/*const US cTRAY_STK_Profile_1[] = { //Profile追加で名前変更
	4000,	4000,	4020,	4030,	4060,	4090,	4140,	4180,	4240,	4300,	// 0-9 (SlowUp),
	4370,	4440,	4520,	4600,	4690,	4780,	4880,	4980,	5080,	5180,	// 10-19 (SlowUp),
	5290,	5390,	5500,	5610,	5710,	5820,	5920,	6020,	6120,	6220,	// 20-29 (SlowUp),
	6310,	6400,	6480,	6560,	6630,	6700,	6760,	6820,	6860,	6910,	// 30-39 (SlowUp),
	6940,	6970,	6980,	7000,	7000,	// 40-44 (SlowUp),
	7000,	6980,      6970,      6940,      6910,	// 45-49(SlowDown)
	6860,      6820,	6760,	6700,	6630,	6560,	6480,	6400,	6310,	6220,	// 50-59 (SlowDown),
	6120,	6020,	5920,	5820,	5710,	5610,	5500,	5390,	5290,	5180,	// 60-69 (SlowDown),
	5080,	4980,	4880,	4780,	4690,	4600,	4520,	4440,	4370,	4300,	// 70-79 (SlowDown),
	4240,	4180,	4140,	4090,	4060,	4030,	4020,	4000,	4000,	0,		// 80-88 (SlowDown),  89(Stop)
};*/

//V3.3.92 Stacker Profile Added  & Modified. (V3.3.94)
const US cTRAY_STK_Profile_2[] = {
	4000,	4010,	4040,	4080,	4140,	4220,	4320,	4430,	4560,	4700,	// 0-9 (SlowUp),
	4850,	5030,	5210,	5400,	5610,	5820,	6050,	6280,	6510,	6760,	// 10-19 (SlowUp),
	7000,       7250,       7500,      7750,	8000,	8240,	8490,	8720,	8950,	9180,	// 20-29 (SlowUp),
	9390,	9600,	9790,	9970,	10150,	10300,	10440,	10570,	10680,	10780,	// 30-39 (SlowUp),
	10860,	10920,	10960,	10990,	11000,	// 40-44 (SlowUp),
	10990,	10960,     10920,     10860,     10780,	// 45-49(SlowDown)
	10680,     10570,	10440,	10300,	10150,	9970,	9790,	9600,	9390,	9180,	// 50-59 (SlowDown),
	8950,	8720,	8490,	8240,	8000,	7750,	7500,	7250,	7000,	6760,	// 60-69 (SlowDown),
	6510,	6280,	6050,	5820,	5610,	5400,	5210,	5030,	4850,	4700,	// 70-79 (SlowDown),
	4560,	4430,	4320,	4220,	4140,	4080,	4040,	4010,	4000,	0,		// 80-88 (SlowDown),  89(Stop)
};


typedef enum{
	eSTK_NORMAL = 0,
	eSTK_LOWER_SAFETY, 		// Lower Safety Warning : Height SNR1 1000ms Onしない -> Waring Count  >= 3
	eSTK_TRAY_FAIL_1, 		// Stacker Tray Fail (1)  : NoPaerSnr  1000ms
	eSTK_TRAY_FAIL_2, 		// Stacker Tray Fail(2) : Height SNR1  5000ms Off しない
	eSTK_UPPER_LIMIT_FAIL, //PulseCount=0-> Height SNR1 500ms Offしない
	eSTK_PITCHSKIP_CHK,    // (V3.6.92) 1000ms 以内にHSNR1がOFF、HSNR2がONにならない場合 - 
	eSTK_NUM,
} ESTK_TimeoutType;

typedef enum{	
	eSTK_JOB_NML_INIT, // Normal Initialize
	eSTK_JOB_SHT_INIT, //Job Initialize
	eSTK_JOB_EJECT,  //排出時、StackerJob
	eSTK_JOB_NUM,
} ESTK_JobType;

typedef struct
{
	ETRAY_STK_Action action;
	ETRAY_STK_Action compoAction;

	UL   stackerPulseCnt;  	// Stacker Tray  Pulse Count
	US   beforePulse;	// Pulse Count Restart時、以前Pulse格納用
	UC   lowerSafetyCnt; 	// Count== 3 -> Stacker Lower Safety Warning 検知
	UC 	sheetNo;
	UC   preSheetNo;  //次JobのSheet格納用(ProcessEnd)
	UC   startClock;  // MotStepUp中、SNRイベント発生処理のため
	UC   ejectRecvFlag;  //Eject Job から 完了  Flag ...
	UC   processEndFlag;  //For  Process End  control...
	DD_STM_PROF_ID profile;
	SS   timerIndex[eSTK_NUM];
	UC  jobType;  //Compile位置へ移動Jobは重複されるため、Jobごとに区別必要あり
	UC  standbyFailCheckFlag;  //V3.4.0 FullStack/LowerSafety発生後、Standby時にSNR変化ある場合,FailチェックFlag
} STK_Control;

typedef struct
{
	void (*mpHandler)(void) ;

}STK_CallFunc ;


static STK_Control 		gSTK;
static EFaultStatus 	gSTK_FaultStat;

/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
static void TRAY_STK_ControlThread();
static void TRAY_STK_ReceiveEventTimeUp(SS aTimerID, UL aType);
static void TRAY_STK_ReceiveFailEventTimeUp(SS aTimerID, UL aType);
static void TRAY_STK_ReceiveSteppingMotorEvent(SS aResult);
static void  TRAY_STK_ReceiveStartClock(SS aResult);

static void TRAY_STK_SetEventTimer(US aTime, UL aType);
static void TRAY_STK_CancelEventTimer(UL Type);
static void TRAY_STK_ClearTimerIndex(UL  eType);  //TimerIndex initialize ..

static void TRAY_STK_GetProfilePulse(US aTotalPulse,  UC  *pIndex);
static US   TRAY_STK_Get_HPulse();
static US   TRAY_STK_Get_GPulse();
static UC	  TRAY_STK_ResetPulse(UL  aMovePls);
static void TRAY_STK_UpdatePulse(UC   aDirection,  UL  aMovePls);
static void  TRAY_STK_MotorUpDown_NVMUpdate(UC   updown);
static void  TRAY_STK_ProcessEndCheck(void); //V3.3.92
static void TRAY_Device_StartCompoLiftUpDown(US aLinkNo);
static void TRAY_Device_StopCompoLiftUpDown(US aLinkNo);


// Stacker Home位置へ移動()
// TRAY_STK_HOME = 0x0300,
static void Home_st1_1();  			// 開始タイミング設定
static void Home_st1_2();  			// HeightSNR1，2による位置判断
static void Home_st2_UP();			// モータ駆動設定
static void Home_st3_UP(); 			// モータ駆動開始(上向き)
static void Home_st4_UP_ONWait(); 	// NoPaperSNR　ON待ち
static void Home_st4_UP_OFFWait();	// NoPaperSNR　OFF待ち
static void Home_st5_UP(); 			// Reset開始
static void Home_st6_UP();  		// Reset完了
static void Home_st7_UP();  //　SNR1　OFF受信(10Pulse開始)
static void Home_st8_UP(); //　StepDown開始
static void Home_st9_UP();  //　タイマ設定

static void Home_st2_DN();  //モータ駆動設定
static void Home_st3_DN();  //モータ駆動開始(下向き)
static void Home_st4_DN();  //SNR1　ON受信(H3Pulse開始)
static void Home_st5_DN();  //StepDown開始　
static void Home_st6_DN();  //Stacker待機位置へ移動動作開始Trigger待ち
static void Home_st7_DN();  //モータ駆動開始(上向き)
static void Home_st8_DN();  //SNR1　OFF受信(10Pulse開始)
static void Home_st9_DN();  //StepDown開始
static void Home_st10_DN();	//後励磁
static void Home_st11_DN();	//StackerHome動作完了

//Stacker待機位置へ移動動作開始TriggerからReset動作開始(0519)
static void Home_st20_ONWait();	//NoPaperSNR　ON待ち
static void Home_st20_OFFWait();  //NoPaperSNR　OFF待ち	
static void Home_st21(); //　Reset開始
static void Home_st22();  //　Reset完了


// Compile位置へ移動
// TRAY_STK_COMPILE = 0x0400,
static void Compile_st1();  			// StackerHeightSNRによる分岐判断
static void Compile_st2_UP();  			// モータ駆動設定
static void Compile_st3_UP();  			// モータ駆動開始
static void Compile_st4_UP_ONWait();  	// NoPaperSNR　ON待ち
static void Compile_st4_UP_OFFWait();  	// NoPaperSNR　OFF待ち
static void Compile_st5_UP();  			// Reset開始
static void Compile_st6_UP();  			// Reset完了
static void Compile_st7_UP();   //SNR1　OFF受信(10Pulse開始)
static void Compile_st8_UP();   //StepDown開始
static void Compile_st9_UP();   //タイマ(方向転換用)設定
static void Compile_st10_UP();  //モータ駆動開始(下向き)
static void Compile_st11_UP();   //SNR1　ON受信(G-Pulse開始)
static void Compile_st12_UP();   //StepDown開始

static void Compile_st2_DN();  //モータ駆動設定
static void Compile_st3_DN();   //モータ駆動開始
static void Compile_st4_DN();  //SNR1 ON受信(G-Pulse開始)
static void Compile_st5_DN();   //StepDown開始

static void Compile_st13(); 	//Compile位置へ移動完了

// Eject位置へ移動(ADJUST)
// TRAY_STK_EJECT = 0x0500,
static void Eject_st1();  	// T1タイマ待ち
static void Eject_st2();  	// モータ駆動設定
static void Eject_st3();  	// モータ駆動開始
static void Eject_st4();   	// Eject位置へ移動完了

// 待機位置へ移動(ProcessEnd)
// TRAY_STK_END = 0x0600,
static void End_st1();   		// モータ駆動開始
static void End_st2_ONWait();  	// NoPaperSNR　ON待ち
static void End_st2_OFFWait();  // NoPaperSNR　OFF待ち
static void End_st3();   		// Reset開始
static void End_st4();  //Reset完了
static void End_st5();  //SNR1　OFF受信(10Pulse開始)
static void End_st6();  //StepDown開始
static void End_st7(); //後励磁
static void End_st8();  //待機位置へ移動完了


static const STK_CallFunc  ControlThread_HOME_POSITION[ ] = {
	{NULL},
	{Home_st1_1},	//開始タイミング設定

	{Home_st1_2},	//HeightSNR1，2による位置判断
	{Home_st2_UP},  //モータ駆動設定
	{Home_st3_UP},  //モータ駆動開始(上向き)
	{Home_st4_UP_ONWait}, //NoPaperSNR　ON待ち
	{Home_st4_UP_OFFWait}, //NoPaperSNR　OFF待ち
	{Home_st5_UP},  //　Reset開始
	{Home_st6_UP},  //　Reset完了
	{Home_st7_UP},  //　SNR1　OFF受信(10Pulse開始)
	{Home_st8_UP}, //　StepDown開始
	{Home_st9_UP},  //　タイマ設定

	{Home_st2_DN},  //モータ駆動設定
	{Home_st3_DN},  //モータ駆動開始(下向き)
	{Home_st4_DN},  //SNR1　ON受信(H3Pulse開始)
	{Home_st5_DN},  //StepDown開始　
	{Home_st6_DN},  //Stacker待機位置へ移動動作開始Trigger待ち
	{Home_st7_DN},  //モータ駆動開始(上向き)
	{Home_st8_DN},  //SNR1　OFF受信(10Pulse開始)
	{Home_st9_DN},  //StepDown開始
	{Home_st10_DN}, //後励磁
	{Home_st11_DN}, //StackerHome動作完了
	
	{ Home_st20_ONWait}, //NoPaperSNR　ON待ち
	{ Home_st20_OFFWait},  //NoPaperSNR　OFF待ち 
	{ Home_st21}, //　Reset開始
	{ Home_st22},  //　Reset完了
	
};


static const STK_CallFunc  ControlThread_COMPILE_POSITION[ ] = {
	{NULL},
	{Compile_st1},	//StackerHeightSNRによる分岐判断

	{Compile_st2_UP},  //モータ駆動設定
	{Compile_st3_UP},	//モータ駆動開始
	{Compile_st4_UP_ONWait},   //NoPaperSNR　ON待ち
	{Compile_st4_UP_OFFWait},  //NoPaperSNR　OFF待ち
	{Compile_st5_UP},	//Reset開始
	{Compile_st6_UP},	//Reset完了
	{Compile_st7_UP},	//SNR1　OFF受信(10Pulse開始)
	{Compile_st8_UP},	//StepDown開始
	{Compile_st9_UP},	//タイマ(方向転換用)設定
	{Compile_st10_UP},	//モータ駆動開始(下向き)
	{Compile_st11_UP},	 //SNR1　ON受信(G-Pulse開始)
	{Compile_st12_UP},	 //StepDown開始

	{Compile_st2_DN},  //モータ駆動設定
	{Compile_st3_DN},	//モータ駆動開始
	{Compile_st4_DN},  //SNR1 ON受信(G-Pulse開始)
	{Compile_st5_DN},	//StepDown開始

	{Compile_st13}, 	//Compile位置へ移動完了
};

static const  STK_CallFunc  ControlThread_EJECT_POSITION[ ] = {
	{NULL},
	{Eject_st1},//T1タイマ待ち
	{Eject_st2},  //モータ駆動設定
	{Eject_st3},  //モータ駆動開始
	{Eject_st4},  //Eject位置へ移動完了
};


static const STK_CallFunc  ControlThread_END_POSITION[ ] = {
	 {NULL},
	 {End_st1},   //モータ駆動開始
	 {End_st2_ONWait},	//NoPaperSNR　ON待ち
	 {End_st2_OFFWait},  //NoPaperSNR　OFF待ち
	 {End_st3},  //Reset開始
	 {End_st4},  //Reset完了
	 {End_st5},  //SNR1　OFF受信(10Pulse開始)
	 {End_st6},  //StepDown開始
	 {End_st7}, //後励磁
	 {End_st8},  //待機位置へ移動完了
};


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_PreStartAct
// Description   : Stacker_動作開始前　操作が必要な場合
// Parameter     : aAction (制御する動作)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRAY_STK_PreStartAct(ETRAY_STK_Action aAction, UC aSheetNo)
{
	switch (aAction) {
		case TRAY_STK_END : 
			gSTK.preSheetNo = aSheetNo;			
			gSTK.processEndFlag = true; //0502
			break;
		default : break;	
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_StartAct
// Description   : Stacker_動作開始
// Parameter     : aAction (制御する動作)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRAY_STK_StartAct(ETRAY_STK_Action aAction, UC aSheetNo)
{
	switch( aAction ) {

	case TRAY_STK_COMPO_COMPILE : //= 0x0100,  STACKER MOT (Compile位置へ移動)
		gSTK.action = TRAY_STK_COMPILE_st1;
		gSTK.jobType=eSTK_JOB_NUM; 
		gSTK.compoAction = aAction;
		gSTK.profile = DD_STM_PROF_ID_STACKER_PRF_2; //DD_STM_PROF_ID_STACKER_PRF_1;
		break;

	case TRAY_STK_COMPO_EJECT :  // = 0x0200, 	 STACKER MOT (Eject位置へ移動)
		gSTK.action = TRAY_STK_EJECT_st1;
		gSTK.jobType=eSTK_JOB_NUM; 
		gSTK.compoAction = aAction;
		gSTK.profile =DD_STM_PROF_ID_STACKER_PRF_2;  //DD_STM_PROF_ID_STACKER_PRF_1;
		break;

	case TRAY_STK_HOME :  //= 0x0300,  Stacker Home位置へ移動
		gSTK.action = TRAY_STK_HOME_st1_1;
		gSTK.jobType=eSTK_JOB_SHT_INIT;  //Job Init
		//TB(0528)
		gSTK.stackerPulseCnt = SRVC_GetNvmValueUL(NVMID_ELEVATOR_MOT_PULSE_COUNT);  
		gSTK.profile = DD_STM_PROF_ID_STACKER_PRF_2;   //V3.3.95
		break;

	case TRAY_STK_COMPILE : // = 0x0400,  Compile位置へ移動
		gSTK.action = TRAY_STK_COMPILE_st1;
		gSTK.profile = DD_STM_PROF_ID_STACKER_PRF_2;
		break;

	case TRAY_STK_EJECT :  // = 0x0500,	 Eject位置へ移動(ADJUST)
		gSTK.action = TRAY_STK_EJECT_st1;
		gSTK.jobType=eSTK_JOB_EJECT;  // Eject
		gSTK.profile = DD_STM_PROF_ID_STACKER_PRF_2;
		break;

	case TRAY_STK_END : // = 0x0600,  待機位置へ移動(ProcessEnd)
		gSTK.action = TRAY_STK_END_st1;		
		gSTK.jobType=eSTK_JOB_NUM; 
		gSTK.profile = DD_STM_PROF_ID_STACKER_PRF_2;
		break;

	default :  return;
	}
	gSTK.sheetNo = aSheetNo;
	gSTK.startClock = START_CLK_UNKNOWN;
	gSTK.beforePulse = 0;
	gSTK.ejectRecvFlag = false;

	TRAY_STK_ClearTimerIndex(eSTK_NUM);
	TRAY_STK_ControlThread();

}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_ControlThread
// Description   :  Stacker制御
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRAY_STK_ControlThread()
{
	ETRAY_STK_Action aAction = gSTK.action & TRAY_STK_ACTION_MASK;
	US  				 aStatus = gSTK.action & 0x00FF;

	if ( aStatus == 0 )  {
		TRAY_STK_DeviceStop();
		return;
	}

	switch ( aAction)	{

	case TRAY_STK_HOME :
		ControlThread_HOME_POSITION[aStatus].mpHandler(); 			break;

	case TRAY_STK_COMPO_COMPILE :
	case TRAY_STK_COMPILE :
		ControlThread_COMPILE_POSITION[aStatus].mpHandler(); 		break;

	case TRAY_STK_COMPO_EJECT :
	case TRAY_STK_EJECT :
		ControlThread_EJECT_POSITION[aStatus].mpHandler(); 			break;

	case TRAY_STK_END :
		ControlThread_END_POSITION[aStatus].mpHandler(); 			break;

	default : 									break;
	}

}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_ReceiveEventTimeUp
// Description   : イベントタイマのタイムアップ
// Parameter     : timerID (Timer ID)
//               :    aType
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRAY_STK_ReceiveEventTimeUp(SS aTimerID, UL aType)
{
	if (  aTimerID != gSTK.timerIndex[aType] ) {
		return;
	}

	gSTK.timerIndex[aType] = MSP_MAX_TMR_NUM;

	switch( gSTK.action ) {

	// STACKER HOME動作
	case TRAY_STK_HOME_st1_1 : //T1 Timeout
	case TRAY_STK_HOME_st2_UP :
	case TRAY_STK_HOME_st2_DN :
	case TRAY_STK_HOME_st10_DN :
		gSTK.action++;
		break;

	case TRAY_STK_HOME_st9_UP : // Timer 設定(50ms)
		gSTK.action = TRAY_STK_HOME_st2_DN; //下向き
		break;

	//Compile位置へ移動
	case TRAY_STK_COMPILE_st2_UP :
	case TRAY_STK_COMPILE_st2_DN :
	case TRAY_STK_COMPILE_st9_UP :
	//Eject位置へ移動
	case TRAY_STK_EJECT_st1 :
	case TRAY_STK_EJECT_st2 :
	//待機位置へ移動(ProcessEnd)
	case TRAY_STK_END_st7 :
		gSTK.action++;
		break;

 	default: return;

	}
	TRAY_STK_ControlThread();
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_ReceiveFailEventTimeUp
// Description   : イベントタイマのタイムアップ
// Parameter     : timerID (Timer ID)
//               	: aType
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRAY_STK_ReceiveFailEventTimeUp(SS aTimerID, UL aType)
{
	if ( aTimerID != gSTK.timerIndex[aType] ) {
		return;
	}

	gSTK.timerIndex[aType] = MSP_MAX_TMR_NUM;

	switch( aType ) {

	case  eSTK_LOWER_SAFETY : //Lower Safety Warning : Height SNR1 1000ms Onしない -> Waring Count  >= 3
		if ( ++gSTK.lowerSafetyCnt >= 3 ) {
		// Lower Safety Warning 　発生処理
			//gSTK_FaultStat = FAULT_OCCUR;
			SRVC_UpdateDeviceStatus(FAULT_OCCUR, DSNO_STACKER_LOW_SAFETY) ; //Low Safety
			TRAY_STK_DeviceStop();
				
		}else {  //T/B対策 (LiftDown 時、LowerSafetyFailが正しく動作されない) V3.4.0
			// Fail Timer (Stacker LowerSafety Warning Counter)
			TRAY_STK_SetEventTimer(STK_FAIL_TIME_1000, eSTK_LOWER_SAFETY);
			PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	0x8888, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 
		}
		break;
/* (CR-011)  Stacker TrayFail 1 Delete..
	case  eSTK_TRAY_FAIL_1 :	// Stacker Tray Fail (1)  : NoPaerSnr  1000ms
		// Stacker Tray Fail 　発生処理
		gSTK_FaultStat = FAULT_OCCUR;
		SRVC_UpdateDeviceStatus(FAULT_OCCUR, DSNO_STACKER_TRAY_FAIL) ; //Low Safety
		TRAY_STK_DeviceStop();
		break;
*/
	case  eSTK_TRAY_FAIL_2 : //Stacker Tray Fail(2) : Height SNR1	5000ms Off しない
		// Stacker Tray Fail 　発生処理
		gSTK_FaultStat = FAULT_OCCUR;
		SRVC_UpdateDeviceStatus(FAULT_OCCUR, DSNO_STACKER_TRAY_FAIL) ; //StackerFail
		TRAY_STK_DeviceStop();
		break;

	case  eSTK_UPPER_LIMIT_FAIL : //PulseCount=0-> Height SNR1 500ms Offしない
		// Upper Limit Fail 発生処理
		gSTK_FaultStat = FAULT_OCCUR;
		SRVC_UpdateDeviceStatus(FAULT_OCCUR, DSNO_STACKER_UPLIMIT_FAIL) ; //UpperLimitFail
		TRAY_STK_DeviceStop();
		break;

	case  eSTK_PITCHSKIP_CHK :  //PitchSkip Timeout .. (V3.6.92)
		SRVC_SetPitchSkip(ON); 
		PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	0xCCCC, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 
		break;

	default:
		return;
	}
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_ReceiveSteppingMotorEvent
// Description   :
// Parameter     :aResult
//
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRAY_STK_ReceiveSteppingMotorEvent(SS aResult)
{
	switch( gSTK.action ) {

	//Stacker Home位置へ移動()
	case  TRAY_STK_HOME_st7_UP ://　10Pulse
	case  TRAY_STK_HOME_st8_UP ://　StepDown
	case  TRAY_STK_HOME_st4_DN : // H3 Pulse
	case  TRAY_STK_HOME_st5_DN : // StepDown
	case  TRAY_STK_HOME_st8_DN : // 10 Pulse
	case  TRAY_STK_HOME_st9_DN : // StepDown
		gSTK.action++;
		break;


	//Compile位置へ移動
	case  TRAY_STK_COMPILE_st7_UP:  // 10Pulse
	case  TRAY_STK_COMPILE_st8_UP:  // StepDown
	case  TRAY_STK_COMPILE_st11_UP:	// G-Pulse
	case  TRAY_STK_COMPILE_st4_DN:	// G-Pulse
		gSTK.action++;
		break;

	case  TRAY_STK_COMPILE_st12_UP: // StepDown
	case  TRAY_STK_COMPILE_st5_DN:  //StepDown
		gSTK.action = TRAY_STK_COMPILE_st13;
		break;



	//Eject位置へ移動(ADJUST)
	case  TRAY_STK_EJECT_st3:   //モータ駆動開始
	//待機位置へ移動(ProcessEnd)
	case  TRAY_STK_END_st5 :  //10pulse
	case  TRAY_STK_END_st6 :  //StepDown
		gSTK.action++;
		break;

	default :  return;
	}

	TRAY_STK_ControlThread();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_ReceiveStartClock
// Description   : DD_STM_StartClock()  Callback
//
///////////////////////////////////////////////////////////////////
void  TRAY_STK_ReceiveStartClock(SS aResult)
{

	gSTK.startClock =  START_CLK_RECV;  //Mot StepUp 完了Flag設定

	switch ( gSTK.action ) {

	/*case  TRAY_STK_HOME_st7_DN : 	// Home
		if ( DD_DIO_GetInput(DD_DI_STACKER_TRAY_HEIGHT1_SNR) == OFF ) {

			gSTK.action++;
			TRAY_STK_ControlThread();
		}
		break;*/

	case  TRAY_STK_HOME_st3_DN :
	case  TRAY_STK_COMPILE_st3_DN :
	case  TRAY_STK_COMPILE_st10_UP:
		if ( DD_DIO_GetInput(DD_DI_STACKER_TRAY_HEIGHT1_SNR) == ON ) {
// T/B V3.3.94  LowerSafetyWarningが  Cancelされず
			TRAY_STK_CancelEventTimer(eSTK_LOWER_SAFETY);
			gSTK.lowerSafetyCnt = 0;
// 
			gSTK.action++;
			TRAY_STK_ControlThread();
		}
		break;

	case  TRAY_STK_HOME_st4_UP_ONWait :
	case  TRAY_STK_HOME_st4_UP_OFFWait :
	case  TRAY_STK_HOME_st5_UP :
	case  TRAY_STK_HOME_st6_UP :
		if ( DD_DIO_GetInput(DD_DI_STACKER_TRAY_HEIGHT1_SNR) == OFF ) {
			//V3.3.94  TimerCancel .. 
			TRAY_STK_CancelEventTimer(eSTK_TRAY_FAIL_2);
			TRAY_STK_CancelEventTimer(eSTK_UPPER_LIMIT_FAIL);

			gSTK.action = TRAY_STK_HOME_st7_UP;
			TRAY_STK_ControlThread();
		}
		break;
//// 150519 
	case  TRAY_STK_HOME_st20_ONWait :
	case  TRAY_STK_HOME_st20_OFFWait :
	case  TRAY_STK_HOME_st21 :
	case  TRAY_STK_HOME_st22 :
		if ( DD_DIO_GetInput(DD_DI_STACKER_TRAY_HEIGHT1_SNR) == OFF ) {
			//V3.3.94  TimerCancel ..
			TRAY_STK_CancelEventTimer(eSTK_TRAY_FAIL_2);
			TRAY_STK_CancelEventTimer(eSTK_UPPER_LIMIT_FAIL);

			gSTK.action = TRAY_STK_HOME_st8_DN;
			TRAY_STK_ControlThread();
		}
		break;
		

	case  TRAY_STK_COMPILE_st4_UP_ONWait :
	case  TRAY_STK_COMPILE_st4_UP_OFFWait :
	case  TRAY_STK_COMPILE_st5_UP :
	case  TRAY_STK_COMPILE_st6_UP :
		if ( DD_DIO_GetInput(DD_DI_STACKER_TRAY_HEIGHT1_SNR) == OFF ) {
			//V3.3.94  TimerCancel ..
			TRAY_STK_CancelEventTimer(eSTK_TRAY_FAIL_2);
			TRAY_STK_CancelEventTimer(eSTK_UPPER_LIMIT_FAIL);

			gSTK.action = TRAY_STK_COMPILE_st7_UP;
			TRAY_STK_ControlThread();
		}
		break;
//150526
	case  TRAY_STK_END_st2_ONWait :
	case  TRAY_STK_END_st2_OFFWait :
	case  TRAY_STK_END_st3 :
	case  TRAY_STK_END_st4 :
		if ( DD_DIO_GetInput(DD_DI_STACKER_TRAY_HEIGHT1_SNR) == OFF ) {
			//V3.3.94  TimerCancel ..
			TRAY_STK_CancelEventTimer(eSTK_TRAY_FAIL_2);
			TRAY_STK_CancelEventTimer(eSTK_UPPER_LIMIT_FAIL);

			gSTK.action = TRAY_STK_END_st5;
			TRAY_STK_ControlThread();
		}
		break;		

	default :  break;
	}
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_Height1SensorON
// Description   : Height1SensorON Event 受信
//
///////////////////////////////////////////////////////////////////
void TRAY_STK_Height1SensorON ()
{
	if ( gSTK.action == TRAY_STK_st0 )
		return;

	//Fail Timer Clear
	TRAY_STK_CancelEventTimer(eSTK_LOWER_SAFETY);
	gSTK.lowerSafetyCnt = 0;

	switch ( gSTK.action ) {

	case  TRAY_STK_HOME_st3_DN :
	case  TRAY_STK_COMPILE_st3_DN:
	case  TRAY_STK_COMPILE_st10_UP:

		//StepUp完了&SNR1=ONの条件チェック
		if(gSTK.startClock == START_CLK_RECV) {
			gSTK.action++;
			TRAY_STK_ControlThread();
		}
		else if(gSTK.startClock == START_CLK_WAIT) { //MotStepUp中、SNR1 ONイベント
			//->移動量補正のため
			gSTK.beforePulse = DD_STM_StopCountClock(DD_STM_ID_STACKER);
			DD_STM_StartCountClock(DD_STM_ID_STACKER);
		}
		break;

	default :  return;
	}

}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_Height1SensorOFF
// Description   : Height1SensorOFF Event 受信
//
///////////////////////////////////////////////////////////////////
void TRAY_STK_Height1SensorOFF()
{
	if ( gSTK.action == TRAY_STK_st0 )
		return;

	//Fail Timer Clear
	TRAY_STK_CancelEventTimer(eSTK_TRAY_FAIL_2);
	TRAY_STK_CancelEventTimer(eSTK_UPPER_LIMIT_FAIL);

      // ( V3.6.92)  PitchSkip Check .. 
	TRAY_STK_CancelEventTimer(eSTK_PITCHSKIP_CHK); 
	
	if ( ON == SRVC_GetPitchSkip() ) {

		SRVC_SetPitchSkip(OFF);
		PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	0xCCDD, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 
	}

	switch ( gSTK.action ) {

	/*case  TRAY_STK_HOME_st7_DN :
		// StepUp完了&SNR1=OFFの条件チェック
		if ( gSTK.startClock == START_CLK_RECV ) {
			gSTK.action++;
			TRAY_STK_ControlThread();
		}
		break;*/

	//Reset 動作中、Stacker Height1 SNR　OFF受信時、処理
	case  TRAY_STK_HOME_st4_UP_ONWait :
	case  TRAY_STK_HOME_st4_UP_OFFWait :
	case  TRAY_STK_HOME_st5_UP :
	case  TRAY_STK_HOME_st6_UP :
	// StepUp完了&SNR1=OFFの条件チェック
	if ( gSTK.startClock == START_CLK_RECV ) {
		gSTK.action = TRAY_STK_HOME_st7_UP;
		TRAY_STK_ControlThread();
	}
	break;

	//Reset 動作中、Stacker Height1 SNR　OFF受信時、処理(0519)
	case  TRAY_STK_HOME_st20_ONWait :
	case  TRAY_STK_HOME_st20_OFFWait :
	case  TRAY_STK_HOME_st21 :
	case  TRAY_STK_HOME_st22 :
	// StepUp完了&SNR1=OFFの条件チェック
	if ( gSTK.startClock == START_CLK_RECV ) {
		gSTK.action = TRAY_STK_HOME_st8_DN;
		TRAY_STK_ControlThread();
	}
	break;

	case  TRAY_STK_COMPILE_st4_UP_ONWait :
	case  TRAY_STK_COMPILE_st4_UP_OFFWait :
	case  TRAY_STK_COMPILE_st5_UP :
	case  TRAY_STK_COMPILE_st6_UP:
	// StepUp完了&SNR1=OFFの条件チェック
	if ( gSTK.startClock == START_CLK_RECV ) {
		gSTK.action = TRAY_STK_COMPILE_st7_UP;
		TRAY_STK_ControlThread();
	}
	break;

	case  TRAY_STK_END_st2_ONWait :
	case  TRAY_STK_END_st2_OFFWait :
	case  TRAY_STK_END_st3 :
	case  TRAY_STK_END_st4 :
	// StepUp完了&SNR1=OFFの条件チェック
	if ( gSTK.startClock == START_CLK_RECV ) {
			
		gSTK.action = TRAY_STK_END_st5;
		TRAY_STK_ControlThread();
	}
	break;

	default :  return;
	}

}

//V3.4.0 未導入仕様:　FullStack/LowerSafety発生後、Standby時にSNR変化ある場合、Stacker動作
///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_Height2SensorOFF
// Description   : Stacker Height2 Sensor Event 受信
///////////////////////////////////////////////////////////////////
void TRAY_STK_Height2SensorOFF()
{
	if ( gSTK.action != TRAY_STK_st0  )  // Standby状態のみチェック
		return;

	//Standby && FlagSet状態 
	if ( true == gSTK.standbyFailCheckFlag ) {
	
		//（SNR1=ON、SNR2=OFF　低い状態) -> 動作Start 
		if ( DD_DIO_GetInput(DD_DI_STACKER_TRAY_HEIGHT1_SNR) == HIGH) { 
			TRAY_STK_StartAct(TRAY_STK_COMPILE, SHEETNO_NUM); //0);
			PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	0xBBBB, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 
		}
		
	}else { //V3.6.92

		//StackerStatus＝Running状態 & CompileTrayに用紙ない & HSNR2変化（ON→OFF） &&HSNR1=ON && Fault NotOccur 
		if ( (TRAY_Manager_GetStatus() == FIN_STT_READY) && 
			(SRVC_CheckUnfinishedSheetExist(eCdiFinOutDest_All) == false) &&
			(DD_DIO_GetInput(DD_DI_STACKER_TRAY_HEIGHT1_SNR) == HIGH) &&
			(FAULT_NOT_OCCUR ==TRAY_STK_IsOccurFault()) ){	
			
				TRAY_STK_StartAct(TRAY_STK_COMPILE, SHEETNO_NUM);	
				PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	0xBBCC, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 
		}	
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_Height2SensorON
// Description   : Stacker Height2 Sensor Event 受信
///////////////////////////////////////////////////////////////////
void TRAY_STK_Height2SensorON()
{
	if ( gSTK.action == TRAY_STK_st0 )
		return;

	//V3.6.92 
	TRAY_STK_CancelEventTimer(eSTK_PITCHSKIP_CHK);	

	if ( ON == SRVC_GetPitchSkip() ) {
		
		SRVC_SetPitchSkip(OFF);
		PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	0xCCEE, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 
	}
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_NoPaperFullSensorON
// Description   : NoPaperFullSensorON Event 受信
//
///////////////////////////////////////////////////////////////////
void TRAY_STK_NoPaperFullSensorON()
{
	if ( (gSTK.compoAction != TRAY_STK_st0) || (gSTK.action == TRAY_STK_st0))  //Diag Mode || IDLE status ..
		return;

	//Fail Timer Clear
//	TRAY_STK_CancelEventTimer(eSTK_TRAY_FAIL_1);

	switch ( gSTK.action) {

	case  TRAY_STK_HOME_st4_UP_ONWait :
	case  TRAY_STK_HOME_st5_UP :
		
	case  TRAY_STK_HOME_st20_ONWait : //150519 Reset added
	case  TRAY_STK_HOME_st21 :

	case  TRAY_STK_COMPILE_st4_UP_ONWait :
	case  TRAY_STK_COMPILE_st5_UP :

	case  TRAY_STK_END_st2_ONWait :
	case  TRAY_STK_END_st3 :
		  gSTK.action++;
		  TRAY_STK_ControlThread();
		  break;
	default :  return;
	}
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_NoPaperFullSensorOFF
// Description   : NoPaperFullSensorOFF Event 受信
//
///////////////////////////////////////////////////////////////////
void TRAY_STK_NoPaperFullSensorOFF()
{
	if ( (gSTK.compoAction != TRAY_STK_st0) || (gSTK.action == TRAY_STK_st0))  //Diag Mode || IDLE status ..
		return;

	//Fail Timer Clear
//	TRAY_STK_CancelEventTimer(eSTK_TRAY_FAIL_1);

	switch ( gSTK.action ) {

	case  TRAY_STK_HOME_st4_UP_OFFWait :

	case  TRAY_STK_HOME_st20_OFFWait :	

	case  TRAY_STK_COMPILE_st4_UP_OFFWait :

	case  TRAY_STK_END_st2_OFFWait :
		  gSTK.action++;
		  TRAY_STK_ControlThread();
		  break;

	default :  return;
	}

}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_InformedSetClampTrigger
// Description :  SetClamp突出でStackerの待機位置移動開始Trigger
// Parameter     :
///////////////////////////////////////////////////////////////////
void TRAY_STK_InformedStackerTrigger()
{
	gSTK.ejectRecvFlag = true;  // FLAG  Set ..

	//Stacker待機位置へ移動動作開始Trigger待ち状態
	if (  gSTK.action == TRAY_STK_HOME_st6_DN) {
		gSTK.action++;
		TRAY_STK_ControlThread();
		gSTK.ejectRecvFlag = false;
	}
	else  {
	 //SetClamp動作がより速かった場合?
	 PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,  0xFFEE, (US)gSTK.stackerPulseCnt, 0); 

	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_SetEventTimer
// Description   : イベントタイマーセット
// Parameter     : aTime (タイマー値 (×1[ms]))
//			  aType
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRAY_STK_SetEventTimer(US aTime, UL aType)
{

	if (  aType >= eSTK_NUM)
		return;

	// Failイベントと分けて処理
	if (  aType == eSTK_NORMAL)  {
		gSTK.timerIndex[aType] = MSP_SetEventTimer(aTime,
											TRAY_STK_ReceiveEventTimeUp, aType);

	} else {
		gSTK.timerIndex[aType] = MSP_SetEventTimer(aTime,
											TRAY_STK_ReceiveFailEventTimeUp, aType);
	}
	return;
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_CancelEventTimer
// Description   : イベントタイマーセット
// Parameter     : aType
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRAY_STK_CancelEventTimer(UL  aType)
{
	UC   cnt;

	// ALL CLEAR TIMER
	if (  aType == eSTK_NUM ) {
		for(cnt=0; cnt < eSTK_NUM ; cnt++) {

			if ( gSTK.timerIndex[cnt] != MSP_MAX_TMR_NUM ) {

				MSP_CancelEventTimer(gSTK.timerIndex[cnt]);
				gSTK.timerIndex[cnt] = MSP_MAX_TMR_NUM;
			}
		}
	}

	else if ( gSTK.timerIndex[aType] != MSP_MAX_TMR_NUM ) {

		MSP_CancelEventTimer(gSTK.timerIndex[aType]);
		gSTK.timerIndex[aType] = MSP_MAX_TMR_NUM;
	}
	return;
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_ClearTimerIndex
// Description   : TimerIndex削除
// Parameter     : ESTK_TimeoutType
// Return        : ‐
///////////////////////////////////////////////////////////////////
void  TRAY_STK_ClearTimerIndex(UL  eType)
{
	UC   cnt;

	if ( eType > eSTK_NUM) // Error ..
		return;

	if ( eType == eSTK_NUM) {  // All Clear する

		for(cnt=0; cnt < eSTK_NUM ; cnt++) {
			gSTK.timerIndex[cnt]=MSP_MAX_TMR_NUM;
		}

	} else {  // 特定IndexのみClearする

		gSTK.timerIndex[eType] = MSP_MAX_TMR_NUM;
	}
	return;
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_IsExecutiongDeviceAction
// Description   : Stackerの動作状態を取得
// Parameter     : ‐
// Return        : true：動作中、false：停止中
///////////////////////////////////////////////////////////////////
bool TRAY_STK_IsExecutionDeviceAction()
{
	if (  gSTK.action == TRAY_STK_st0 ) {
		return false;
	} else {
		return true;
	}	
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_IsOccurFault
// Description   : StackerのFail状態を取得
// Parameter     : ‐
// Return        : true：Fail 、false: Not Fail
///////////////////////////////////////////////////////////////////
bool TRAY_STK_IsOccurFault()
{
	if (  gSTK_FaultStat == FAULT_OCCUR ) {
		PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,  0xFFFF, (US)gSTK.stackerPulseCnt, 0); 
		return true;
	}else {
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_Reset
// Description   : Stackerリセット
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRAY_STK_Reset()
{
	gSTK.action = TRAY_STK_st0;
	gSTK.compoAction = TRAY_STK_st0;

	gSTK.stackerPulseCnt = SRVC_GetNvmValueUL(NVMID_ELEVATOR_MOT_PULSE_COUNT);  //  Stacker Tray  Pulse Count -> NVM  ??
	gSTK.lowerSafetyCnt = 0; // Count== 3 -> Stacker Lower Safety Warning 検知
	gSTK.sheetNo = SHEETNO_NUM;
	gSTK.preSheetNo=SHEETNO_NUM;
	gSTK.processEndFlag = false;
	gSTK.jobType=eSTK_JOB_NUM;
	gSTK.standbyFailCheckFlag = false;
	gSTK_FaultStat = FAULT_NOT_OCCUR;//0502

	TRAY_STK_ClearTimerIndex(eSTK_NUM);

	DD_DIO_SetOutput(DD_DO_STACKER_MOT_ENB,  OFF);
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR,  LOW);

// V3.3.92  Profile Added .. 
//	DD_STM_SetProfile(DD_STM_PROF_ID_STACKER_UPDOWN, cTRAY_STK_Profile);
//	DD_STM_SetProfile(DD_STM_PROF_ID_STACKER_PRF_1, cTRAY_STK_Profile_1);
	DD_STM_SetProfile(DD_STM_PROF_ID_STACKER_PRF_2, cTRAY_STK_Profile_2);


}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_DeviceStop
// Description   : Stacker停止
// Parameter     : ‐  // Return        : ‐
///////////////////////////////////////////////////////////////////
void TRAY_STK_DeviceStop()
{
	ETRAY_STK_Action aAction = gSTK.action & TRAY_STK_ACTION_MASK;
	UC aSheetNo = gSTK.sheetNo;

	gSTK.action = TRAY_STK_st0;
	gSTK.sheetNo = SHEETNO_NUM;
	gSTK.processEndFlag = false;
	gSTK.jobType=eSTK_JOB_NUM;

	TRAY_STK_CancelEventTimer(eSTK_NUM);

	DD_DIO_SetOutput(DD_DO_STACKER_MOT_ENB,  OFF);
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR,  LOW);

	DD_STM_ShutDownStop(DD_STM_ID_STACKER);
	SRVC_SetPitchSkip(OFF);

	if(gSTK.compoAction != TRAY_STK_st0)	{

		 gSTK.compoAction = TRAY_STK_st0;

	} else {
//		TRAY_Manager_InformedStackerActionComplete((US)aAction , aSheetNo);
	}
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_GetProfilePulse
// Description   : Eject位置へ移動時、Step数 , ConstPulse算出
// Parameter      : aTotalPulse (総移動量 (×1[Pulse]))
//			   *aIndex(Step),
// Return        : ‐
///////////////////////////////////////////////////////////////////
void  TRAY_STK_GetProfilePulse(US aTotalPulse,  UC  *pIndex)
{
	//　総Pulse >　(StepUpDownPulse*2 )
	if ( aTotalPulse > (SS)(45 * 2 )) {

		*pIndex = PROF_PULSE_STEP_45;

	} else  { // 　総Pulse　<=　(StepUpDownPulse*2 )

		*pIndex = (UC)(aTotalPulse /2) ;

	}
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_Get_HPulse
// Description; //H-Pulse 設定 (Eject )
// Parameter     : ‐    // Return        : H-Pulse
///////////////////////////////////////////////////////////////////
US    TRAY_STK_Get_HPulse()
{

	US	  H_pulse = 0;
	
 //CR-008	
	UC     compileCount = SRVC_GetCompileCount(gSTK.sheetNo);
	UC     nvmCount = SRVC_GetNvmValueUC(NVMID_PAPER_SET_COUNT_N);


	switch ( SRVC_GetStapleMode(gSTK.sheetNo) ) {

	case eCdiFinStaple_SingleFront ://STAPLE
		// N2 >= N1  --> H1
		if ( SRVC_GetNvmValueUC(NVMID_CORNER_STAPLE_MODE_COUNT_N2) >=
			SRVC_GetNvmValueUC(NVMID_CORNER_STAPLE_COUNT_N1)) {

			if ( compileCount <= nvmCount ) { // 用紙セット枚数　N枚以下

				H_pulse = SRVC_GetNvmValueUS(NVMID_STACKER_MOT_LIFT_DOWN_COUNT_H1_1);

			} else {

				H_pulse = SRVC_GetNvmValueUS(NVMID_STACKER_MOT_LIFT_DOWN_COUNT_H1_2);

			}
		} else {  // --> H2

			if ( compileCount <= nvmCount ) { // 用紙セット枚数　N枚以下

				H_pulse = SRVC_GetNvmValueUS(NVMID_STACKER_MOT_LIFT_DOWN_COUNT_H2_1);

			} else {

				H_pulse = SRVC_GetNvmValueUS(NVMID_STACKER_MOT_LIFT_DOWN_COUNT_H2_2);

			}
		}
		break;	

	case eCdiFinStaple_Off : //UNSTAPLE
		// N2 >= N1 --> H1
		if ( SRVC_GetNvmValueUC(NVMID_CORNER_STAPLE_MODE_COUNT_N2) >=
			SRVC_GetNvmValueUC(NVMID_CORNER_STAPLE_COUNT_N1)) {

			if ( compileCount <= nvmCount ) { // 用紙セット枚数　N枚以下

				H_pulse = SRVC_GetNvmValueUS(NVMID_STACKER_MOT_LIFT_DOWN_COUNT_H1_3);

			} else {

				H_pulse = SRVC_GetNvmValueUS(NVMID_STACKER_MOT_LIFT_DOWN_COUNT_H1_4);

			}
		} else { // --> H2

			if ( compileCount <= nvmCount ) { // 用紙セット枚数　N枚以下

				H_pulse = SRVC_GetNvmValueUS(NVMID_STACKER_MOT_LIFT_DOWN_COUNT_H2_3);

			} else {

				H_pulse = SRVC_GetNvmValueUS(NVMID_STACKER_MOT_LIFT_DOWN_COUNT_H2_4);

			}
		}
		break;

	  default : break;
	}

	return	H_pulse;
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_Get_GPulse
// Description; //H-Pulse 設定 (Eject )
// Parameter     : ‐    // Return        : H-Pulse
///////////////////////////////////////////////////////////////////
US    TRAY_STK_Get_GPulse()
{
	US	  G_pulse = 0;

	// N2 >= N1
	if ( SRVC_GetNvmValueUC(NVMID_CORNER_STAPLE_MODE_COUNT_N2) >=
				SRVC_GetNvmValueUC(NVMID_CORNER_STAPLE_COUNT_N1)) {

		G_pulse = SRVC_GetNvmValueUS(NVMID_STACKER_MOT_LIFT_UP_COUNT_G1);

	} else {

		G_pulse = SRVC_GetNvmValueUS(NVMID_STACKER_MOT_LIFT_UP_COUNT_G2);

	}
	//G-Pulse Test  V3.5.94
	PrintStackerTrayLog_Short(0xC0,SRVC_GetNvmValueUC(NVMID_CORNER_STAPLE_MODE_COUNT_N2)  , 
							SRVC_GetNvmValueUC(NVMID_CORNER_STAPLE_COUNT_N1), 
							G_pulse); //Log

	return  G_pulse;
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_ResetPulse
// Description   : Pulse Reset
// Parameter     : ‐
// Return        :  NG/OK
///////////////////////////////////////////////////////////////////
UC	TRAY_STK_ResetPulse(UL  aMovePls)
{

	//Resetパルス値決め
	// CNvmInformer_SaveNvm();
	if ( (aMovePls >= 59-PLS_SCOPE) && (aMovePls <= 59+PLS_SCOPE) ) { // 2mm

		gSTK.stackerPulseCnt = 0;

		// Pulse=0の場合、UpperLimitFail検知開始
		TRAY_STK_SetEventTimer(STK_FAIL_TIME_500,	eSTK_UPPER_LIMIT_FAIL);

	} else  if (( aMovePls >= 82-PLS_SCOPE) && (aMovePls <= 82+PLS_SCOPE)) { // 3mm

		gSTK.stackerPulseCnt = 127; 	//59+0+68

	} else  if (( aMovePls >= 105-PLS_SCOPE) && (aMovePls <= 105+PLS_SCOPE)) { // 4mm

		gSTK.stackerPulseCnt = 277; 	//127+82+68

	} else  if (( aMovePls >= 127-PLS_SCOPE) && (aMovePls <= 127+PLS_SCOPE)) { //5 mm

		gSTK.stackerPulseCnt = 450; 	//277+105+68

	} else  if (( aMovePls >= 150-PLS_SCOPE) && (aMovePls <= 150+PLS_SCOPE)) { //6 mm

		gSTK.stackerPulseCnt = 645; //450+127+68

	} else  if (( aMovePls >= 172-PLS_SCOPE) && (aMovePls <= 172+PLS_SCOPE)) { //7 mm

		gSTK.stackerPulseCnt = 863; //645+150+68

	} else  if (( aMovePls >= 195-PLS_SCOPE) && (aMovePls <= 195+PLS_SCOPE)) { //8 mm

		gSTK.stackerPulseCnt = 1103; //863+172+68

	} else  if (( aMovePls >= 218-PLS_SCOPE) && (aMovePls <= 218+PLS_SCOPE)) { //9 mm

		gSTK.stackerPulseCnt = 1366; //1103+195+68

	} else {
		PrintStackerTrayLog_Short(eSTK_LOG_PLS_RESET, (US)gSTK.stackerPulseCnt, 0xFF, 0xFF); //Log
		return	NG;
	}		
//test
	PrintStackerTrayLog_Short(eSTK_LOG_PLS_RESET, (US)gSTK.stackerPulseCnt, 0, 0); //Log

	return  OK;
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_UpdatePulse
// Description   : Pulse Update
// Parameter     : MOT DIRECTION,  MOVEPLS
// Return        : -
///////////////////////////////////////////////////////////////////
void    TRAY_STK_UpdatePulse(UC   aDirection,  UL  aMovePls)
{

	if ( aDirection == STK_DIR_DOWN ) { 	//DOWN -> 増加

		gSTK.stackerPulseCnt  = gSTK.stackerPulseCnt + aMovePls;

	} else {  //UP -> 減少

		if ( aMovePls >= gSTK.stackerPulseCnt ) {

			gSTK.stackerPulseCnt = 0;

		} else {

			gSTK.stackerPulseCnt = gSTK.stackerPulseCnt  - aMovePls;
		}
	}
	//(CR-013)
	SRVC_SetNvmValueUL(gSTK.stackerPulseCnt, NVMID_ELEVATOR_MOT_PULSE_COUNT);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_SetPulse_FullStackRelease
// Description   : Pulse Update
// Parameter     : aPulse
// Return        : -
///////////////////////////////////////////////////////////////////
void  TRAY_STK_SetPulse_FullStackRelease(UL   aPulse)
{
	gSTK.stackerPulseCnt = aPulse;
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_GetPulse
// Description   : Read pulse
// Parameter     :  
// Return        :  pulse 
///////////////////////////////////////////////////////////////////
UL    TRAY_STK_GetPulse()
{
	return  gSTK.stackerPulseCnt;
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_MotorUpDown_NVMUpdate	// V3.0  NVM 更新	
// Description   : Stacker Motor Up/Down 動作回数更新
// Parameter     :  STK_DIR_UP,  STK_DIR_DOWN
// Return        :  void
///////////////////////////////////////////////////////////////////
void  TRAY_STK_MotorUpDown_NVMUpdate(UC   updown)
{
//STK_DIR_UP=0,  STK_DIR_DOWN=1
	UL	 count=0;

	if ( updown == STK_DIR_UP ) {
		//StackerMotorUP動作回数
		count =  SRVC_GetNvmValueUL(NVMID_STACKER_MOT_UP_COUNT) ;
		SRVC_SetNvmValueUL(count+1,  NVMID_STACKER_MOT_UP_COUNT) ;

	} else {  //DOWN
		//StackerMotorDOWN動作回数
		count = SRVC_GetNvmValueUL(NVMID_STACKER_MOT_DOWN_COUNT);
		SRVC_SetNvmValueUL(count+1,  NVMID_STACKER_MOT_DOWN_COUNT) ;

	}
	return;
}


///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_ProcessEndCheck
// Description   :  ProcessEnd動作するかをチェックする関数（Compile_st13から移動)
// Parameter     :  
// Return        : 
///////////////////////////////////////////////////////////////////
void   TRAY_STK_ProcessEndCheck(void)
{
	if ((gSTK.jobType == eSTK_JOB_EJECT) && ( gSTK.processEndFlag == true ) ) {

		PrintStackerTrayLog_Short((UC)eSTK_LOG_PROCESSEND_CHECK,  (US)0x00,  (US)gSTK.jobType , (US)gSTK.stackerPulseCnt); 

		gSTK.processEndFlag = false;
		TRAY_STK_StartAct(TRAY_STK_END , gSTK.preSheetNo);
		
	} else if (gSTK.processEndFlag == true )  { //Abnormal case .. 

		PrintStackerTrayLog_Short((UC)eSTK_LOG_PROCESSEND_CHECK,	  (US)0xFF,   (US)gSTK.jobType , (US)gSTK.stackerPulseCnt); 

	}

}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_StandbyFailCheckStart
// Description   :  FullStack/LowerSafetyWarning発生後、Standby時にSNR変化ある場合、Stacker動作Start 
// Parameter     :  
// Return        : 
///////////////////////////////////////////////////////////////////
void   TRAY_STK_StandbyFailCheckStart(void)
{
	gSTK.standbyFailCheckFlag = true;
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_StandbyFailCheckStop
// Description   :  FullStack/LowerSafetyWarning発生後、Standby時にSNR変化ある場合、Stacker動作Stop
// Parameter     :  
// Return        : 
///////////////////////////////////////////////////////////////////
void   TRAY_STK_StandbyFailCheckStop(void)
{
	gSTK.standbyFailCheckFlag = false;
}


///////////////////////////////////////////////////////////////////
// Function Name : TRNS_InformedCompoControlComplete
// Description   : Module Action Complete(Transport Internal I/F)通知受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRAY_InformedCompoControlComplete(US aChainNo, US aLinkNo, EAction aAction)
{
	EvtParamComponentControlStopNotify aEvtParameter;

	aEvtParameter.chainNo = aChainNo;
	aEvtParameter.linkNo = aLinkNo;

	if ( aAction == ACT_START ) {
		MSP_SendMessage(MID_TRAY, EVT_DIAG_COMPO_CONT_START, (const UC*)&aEvtParameter, sizeof(aEvtParameter), LOG_OFF);
	} else {																		// ACT_STOP
		MSP_SendMessage(MID_TRAY, EVT_DIAG_COMPO_CONT_STOP, (const UC*)&aEvtParameter, sizeof(aEvtParameter), LOG_OFF);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_InformedCompoControlStart
// Description   : TRNS ModuleにComponent Control Action開始を通知する
// Parameter     : aChainLink(0-15bit:ChainNo、16-31bit:LinkNo)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRAY_InformedCompoControlStart(SS aTimerID, UL aChainLink)
{
	US aChainNo = (US)(aChainLink & 0x0000FFFF);
	US aLinkNo = (US)((aChainLink >> 16) & 0x0000FFFF);
	TRAY_InformedCompoControlComplete(aChainNo, aLinkNo, ACT_START);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_Device_ComponentControl
// Description   :
// Parameter     :
// Return        :
///////////////////////////////////////////////////////////////////
void TRAY_Device_ComponentControl(US aChainNo, US aLinkNo, EAction aAction)
{
	if ( aAction == ACT_START ) {
		TRAY_Device_StartCompoLiftUpDown(aLinkNo);
	} else { // ACT_STOP
		TRAY_Device_StopCompoLiftUpDown(aLinkNo);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Device_StartCompoLiftUpDown
// Description   :
// Parameter     :
// Return        :
///////////////////////////////////////////////////////////////////
void TRAY_Device_StartCompoLiftUpDown(US aLinkNo)
{
	ETRAY_STK_Action aAction;
	UL aChainLink = (UL)(TRAY_COMPO_CHAIN) | ((UL)(aLinkNo) << 16);
	MSP_SetEventTimer(0, TRAY_InformedCompoControlStart, aChainLink);

	aAction = (ETRAY_STK_Action)aLinkNo;
	switch ( aAction ) {
	case TRAY_STK_COMPO_COMPILE :
	case TRAY_STK_COMPO_EJECT :
		TRAY_STK_StartAct(aAction , 0);	// Link No =
		break;

	default:
		break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_Device_StopCompoLiftUpDown
// Description   :
// Parameter     :
// Return        :
///////////////////////////////////////////////////////////////////
void TRAY_Device_StopCompoLiftUpDown(US aLinkNo)
{
	ETRAY_STK_Action aAction;

	aAction = (ETRAY_STK_Action)aLinkNo;
	switch ( aAction ) {
	case TRAY_STK_COMPO_COMPILE :
	case TRAY_STK_COMPO_EJECT :
		TRAY_InformedCompoControlComplete((US)TRAY_COMPO_CHAIN, aLinkNo, ACT_STOP);
		TRAY_STK_DeviceStop();
		break;

	default:
		break;
	}
}


//Stacker Home位置へ移動()
//TRAY_STK_HOME = 0x0300,
///////////////////////////////////////////////////////////////////
// Function Name : Home_st1_1
// Description  ;  //開始タイミング設定
// Parameter     : ‐    // Return        : ‐
// History : V3.6.2 仕様変更：StackerHome動作開始TriggerがNVM→EjectHome動作時間+30ms経過
///////////////////////////////////////////////////////////////////
void Home_st1_1()
{
	US    aEjectHomeTime = CMPL_GetEjectHomeNeedTime(gSTK.sheetNo) + STK_MOT_ENABLE_TIME_30;

	TRAY_STK_SetEventTimer( aEjectHomeTime,	 eSTK_NORMAL);

	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS, gSTK.action , (US)gSTK.stackerPulseCnt, gSTK.sheetNo); //Log
}

///////////////////////////////////////////////////////////////////
// Function Name : Home_st1_2
// Description  ;  //HeightSNR1，2による位置判断
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void Home_st1_2()
{
	ETRAY_STK_Action   action = gSTK.action;
	UC  	sheetNo = gSTK.sheetNo;


	if ( DD_DIO_GetInput(DD_DI_STACKER_TRAY_HEIGHT1_SNR) == HIGH) { // Lift UP

	     gSTK.action = TRAY_STK_HOME_st2_UP;
	     PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS, gSTK.action , (US)gSTK.stackerPulseCnt, gSTK.sheetNo); //Log

	} else  {

		if ( DD_DIO_GetInput(DD_DI_STACKER_TRAY_HEIGHT2_SNR) == HIGH) {//Lift Down

			gSTK.action = TRAY_STK_HOME_st2_DN;
			PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS, gSTK.action , (US)gSTK.stackerPulseCnt, gSTK.sheetNo); //Log

		} else {  //SetClamp 退避状態

			gSTK.action = TRAY_STK_st0;
			gSTK.sheetNo = SHEETNO_NUM;
			//0502
			//TRAY_Manager_InformedStackerActionComplete(TRAY_STK_HOME,  sheetNo); //0502
			TRAY_Manager_InformedStackerActionComplete(TRAY_STK_COMPILE,  sheetNo);
			
			PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS, gSTK.action , (US)gSTK.stackerPulseCnt, gSTK.sheetNo); //Log
			return;
		}
	}
	TRAY_STK_ControlThread();

}


///////////////////////////////////////////////////////////////////
// Function Name : Home_st2_UP
// Description  ;	//モータ駆動設定
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void Home_st2_UP()
{
	//　上方向, 弱電流
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_ENB,  ON);
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_DIR,  STK_DIR_UP);
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, LOW);

	TRAY_STK_SetEventTimer(STK_MOT_ENABLE_TIME_30,	eSTK_NORMAL);

}


///////////////////////////////////////////////////////////////////
// Function Name : Home_st3_UP
// Description ;  //モータ駆動開始(上向き)
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void Home_st3_UP()
{
	//強電流
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, HIGH);

	// Stacker Mot Start
	DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	DD_STM_StartClock(DD_STM_ID_STACKER, PROF_PULSE_STEP_45, TRAY_STK_ReceiveStartClock);

	gSTK.startClock = START_CLK_WAIT;  //Set Wait Flag
	gSTK.beforePulse = 0;

	//Clock Count Start ..
	DD_STM_StartCountClock(DD_STM_ID_STACKER);

	//Fail Timer
//	TRAY_STK_SetEventTimer(STK_FAIL_TIME_1000,	  eSTK_TRAY_FAIL_1);
	TRAY_STK_SetEventTimer(STK_FAIL_TIME_5000,	  eSTK_TRAY_FAIL_2);

	// Reset Start ...
	if (  DD_DIO_GetInput(DD_DI_STACKER_NO_PAPER_FULL_SNR) == LOW) {

		gSTK.action = TRAY_STK_HOME_st4_UP_ONWait;

	} else {

		gSTK.action = TRAY_STK_HOME_st4_UP_OFFWait;
	}

}


///////////////////////////////////////////////////////////////////
// Function Name : Home_st4_UP_ONWait
// Description ; //NoPaperSNR　ON待ち
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void Home_st4_UP_ONWait()
{ //NO ACTION ..
}

///////////////////////////////////////////////////////////////////
// Function Name : Home_st4_UP_OFFWait
// Description ; //NoPaperSNR　OFF待ち
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void Home_st4_UP_OFFWait()
{ //NO ACTION ..
}

///////////////////////////////////////////////////////////////////
// Function Name : Home_st5_UP
// Description;  //　Reset開始
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void Home_st5_UP()
{
	// Reset動作中、SNR1=OFFとなった場合のPulse計算のため, Saveしておく
	gSTK.beforePulse = DD_STM_StopCountClock(DD_STM_ID_STACKER);

	//Restart Pulse Count
	DD_STM_StartCountClock(DD_STM_ID_STACKER);
}

///////////////////////////////////////////////////////////////////
// Function Name : Home_st6_UP
// Description ;  //　Reset完了
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void Home_st6_UP()
{
	UL	  movePls;

	//Reset開始～Reset完了区間パルス計算
	movePls = DD_STM_StopCountClock(DD_STM_ID_STACKER);

	//Restart  Pulse Count
	DD_STM_StartCountClock(DD_STM_ID_STACKER);

	TRAY_STK_ResetPulse(movePls);

	gSTK.beforePulse = 0;  //Pulse更新が終わったのでClearする

}

///////////////////////////////////////////////////////////////////
// Function Name : Home_st7_UP
// Description ;  //　SNR1　OFF受信(10Pulse開始)
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void Home_st7_UP()
{
	UL     movePls;

	// ResetPulseカウントのため、途中で中断したPulseも計算する
	movePls = DD_STM_StopCountClock(DD_STM_ID_STACKER) +  gSTK.beforePulse;
	gSTK.beforePulse = 0;
	TRAY_STK_UpdatePulse( STK_DIR_UP, movePls);

	//10Pulse start ..
	DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	DD_STM_AppointPulseTiming(DD_STM_ID_STACKER, 10, TRAY_STK_ReceiveSteppingMotorEvent);
	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS, gSTK.action , (US)gSTK.stackerPulseCnt, gSTK.sheetNo); //Log

}

///////////////////////////////////////////////////////////////////
// Function Name : Home_st8_UP
// Description; //　StepDown開始
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void Home_st8_UP()
{
	DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	DD_STM_SlowDownStop(DD_STM_ID_STACKER, TRAY_STK_ReceiveSteppingMotorEvent);
	
	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS, gSTK.action , (US)gSTK.stackerPulseCnt, gSTK.sheetNo); //Log
}


///////////////////////////////////////////////////////////////////
// Function Name : Home_st9_UP
// Description;  //　タイマ設定
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void Home_st9_UP()
{
	// 10Pulse + StepDown
	TRAY_STK_UpdatePulse( STK_DIR_UP, (UL)(10L+PROF_PULSE_STEP_45));

	TRAY_STK_MotorUpDown_NVMUpdate(STK_DIR_UP);  //V3.0

	//50ms Timer set
	TRAY_STK_SetEventTimer(STK_MOT_SWITCH_TIME_50, eSTK_NORMAL);
}

///////////////////////////////////////////////////////////////////
// Function Name : Home_st2_DN
// Description  ;  //モータ駆動設定
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void Home_st2_DN()
{
	//Mot Enable,　下方向、 弱電流
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_ENB,  ON);
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_DIR,  STK_DIR_DOWN);
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, LOW);

	TRAY_STK_SetEventTimer(STK_MOT_ENABLE_TIME_30,  eSTK_NORMAL);
}


///////////////////////////////////////////////////////////////////
// Function Name : Home_st3_DN
// Description ;  //モータ駆動開始(下向き)
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void Home_st3_DN()
{
	//強電流
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, HIGH);

	// Stacker Height1 SNR ONからH3Pulse後、StepDown
	DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	DD_STM_StartClock(DD_STM_ID_STACKER, PROF_PULSE_STEP_45, TRAY_STK_ReceiveStartClock);

	gSTK.startClock = START_CLK_WAIT;  //Set Wait Flag
	gSTK.beforePulse = 0;

	//Clock Count Start ..
	DD_STM_StartCountClock(DD_STM_ID_STACKER);
	
	// Fail Timer (Stacker LowerSafety Warning Counter)
	TRAY_STK_SetEventTimer(STK_FAIL_TIME_1000, eSTK_LOWER_SAFETY);
	//  (V3.6.92)1000ms Caution .. PitchSkip 要求
	TRAY_STK_SetEventTimer(STK_FAIL_TIME_1000, eSTK_PITCHSKIP_CHK);
	
}


///////////////////////////////////////////////////////////////////
// Function Name : Home_st4_DN
// Description ;  //SNR1　ON受信(H3Pulse開始)
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void Home_st4_DN()
{
	US	 h_Pulse;
	US   afterPls;
	US   movePls;

	 //CR-008
	h_Pulse = (US)( SRVC_GetNvmValueUS(NVMID_COMPILE_POSITION_H3) * 10) ;//単位変更 : 10count 

	afterPls = DD_STM_StopCountClock(DD_STM_ID_STACKER);

	movePls = gSTK.beforePulse + afterPls;

	TRAY_STK_UpdatePulse( STK_DIR_DOWN, (UL)movePls);
	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS, gSTK.action , (US)gSTK.stackerPulseCnt, gSTK.sheetNo); //Log

	if ( gSTK.beforePulse > 0) {  // StepUp中、SNR1 ON

		if ( h_Pulse > afterPls) {

			movePls = h_Pulse - afterPls;  // StepUp中、SNR ONしたときからの移動パルス分を引く

			//Stacker Height1 SNR ONからH3Pulse後、StepDown
			DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
			DD_STM_AppointPulseTiming(DD_STM_ID_STACKER, movePls, TRAY_STK_ReceiveSteppingMotorEvent);

			TRAY_STK_UpdatePulse( STK_DIR_DOWN, (UL)movePls);

		} else {  // H3-Pulse 無し-> StepDown

			gSTK.action = TRAY_STK_HOME_st5_DN;
			TRAY_STK_ControlThread();
		}

		gSTK.beforePulse=0;

	} else { // gSTK.beforePulse==0

		if ( h_Pulse > 0)  {

			//Stacker Height1 SNR ONからH3Pulse後、StepDown
			DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
			DD_STM_AppointPulseTiming(DD_STM_ID_STACKER, h_Pulse, TRAY_STK_ReceiveSteppingMotorEvent);

			TRAY_STK_UpdatePulse( STK_DIR_DOWN, (UL)h_Pulse);

		} else {	// H3-Pulse 無し-> StepDown

			gSTK.action = TRAY_STK_HOME_st5_DN;
			TRAY_STK_ControlThread();
		}
	}

}


///////////////////////////////////////////////////////////////////
// Function Name : Home_st5_DN
// Description;  //StepDown開始　
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void Home_st5_DN()
{
	DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	DD_STM_SlowDownStop(DD_STM_ID_STACKER, TRAY_STK_ReceiveSteppingMotorEvent);
	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS, gSTK.action , (US)gSTK.stackerPulseCnt, gSTK.sheetNo); //Log

}


///////////////////////////////////////////////////////////////////
// Function Name : Home_st6_DN
// Description;  //Stacker待機位置へ移動動作開始Trigger待ち
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void Home_st6_DN()
{
	//弱電流
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, LOW);

	TRAY_STK_UpdatePulse( STK_DIR_DOWN, PROF_PULSE_STEP_45); //44

	TRAY_STK_MotorUpDown_NVMUpdate(STK_DIR_DOWN);  //V3.0

	if ( gSTK.ejectRecvFlag == true ) {  //Eject Job  finish..
	
		PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS, gSTK.action , (US)gSTK.stackerPulseCnt, gSTK.sheetNo); //Log

		gSTK.ejectRecvFlag = false;
		gSTK.action++;
		TRAY_STK_ControlThread();

	}

}


///////////////////////////////////////////////////////////////////
// Function Name : Home_st7_DN
// Description;  //モータ駆動開始(上向き)
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void Home_st7_DN()
{
	//　上方向, 強電流
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_DIR,  STK_DIR_UP);
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, HIGH);


	DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	DD_STM_StartClock(DD_STM_ID_STACKER, PROF_PULSE_STEP_45, TRAY_STK_ReceiveStartClock);

	gSTK.startClock = START_CLK_WAIT;  //Set Wait Flag
	gSTK.beforePulse = 0;

	//Clock Count Start ..
	DD_STM_StartCountClock(DD_STM_ID_STACKER);

// 0519  Reset function added...
	//Fail Timer
//	TRAY_STK_SetEventTimer(STK_FAIL_TIME_1000,	  eSTK_TRAY_FAIL_1);
	TRAY_STK_SetEventTimer(STK_FAIL_TIME_5000,	  eSTK_TRAY_FAIL_2);

	// Reset Start ...
	if (  DD_DIO_GetInput(DD_DI_STACKER_NO_PAPER_FULL_SNR) == LOW) {

		gSTK.action = TRAY_STK_HOME_st20_ONWait;

	} else {

		gSTK.action = TRAY_STK_HOME_st20_OFFWait;
	}

}

///↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓(0519)
//////////////////////////////////////////////////////////////////////////
// Stacker待機位置へ移動動作開始TriggerからReset動作開始
///////////////////////////////////////////////////////////////////
// Function Name : Home_st20_ONWait
// Description ; //NoPaperSNR　ON待ち
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void Home_st20_ONWait()
{ //NO ACTION ..
}

///////////////////////////////////////////////////////////////////
// Function Name : Home_st20_OFFWait
// Description ; //NoPaperSNR　OFF待ち
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void Home_st20_OFFWait()
{ //NO ACTION ..
}

///////////////////////////////////////////////////////////////////
// Function Name : Home_st21
// Description;  //　Reset開始
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void Home_st21()
{
	// Reset動作中、SNR1=OFFとなった場合のPulse計算のため, Saveしておく
	gSTK.beforePulse = DD_STM_StopCountClock(DD_STM_ID_STACKER);

	//Restart Pulse Count
	DD_STM_StartCountClock(DD_STM_ID_STACKER);
}

///////////////////////////////////////////////////////////////////
// Function Name : Home_st6_UP
// Description ;  //　Reset完了
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void Home_st22()
{
	UL	  movePls;

	//Reset開始～Reset完了区間パルス計算
	movePls = DD_STM_StopCountClock(DD_STM_ID_STACKER);

	//Restart  Pulse Count
	DD_STM_StartCountClock(DD_STM_ID_STACKER);

	TRAY_STK_ResetPulse(movePls);

	gSTK.beforePulse = 0;  //Pulse更新が終わったのでClearする

}
//↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : Home_st8_DN
// Description ;  //SNR1　OFF受信(10Pulse開始)
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void Home_st8_DN()
{
	US  movePls;

	// ResetPulseカウントのため、途中で中断したPulseも計算する
	movePls = DD_STM_StopCountClock(DD_STM_ID_STACKER) +  gSTK.beforePulse; //movePls = DD_STM_StopCountClock(DD_STM_ID_STACKER);
	gSTK.beforePulse = 0;

	TRAY_STK_UpdatePulse( STK_DIR_UP, (UL)movePls);
	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS, gSTK.action , (US)gSTK.stackerPulseCnt, gSTK.sheetNo); //Log

	// Stacker Height1 SNR OFFから 10Pulse後、StepDown
	DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	DD_STM_AppointPulseTiming(DD_STM_ID_STACKER, 10, TRAY_STK_ReceiveSteppingMotorEvent);

}


///////////////////////////////////////////////////////////////////
// Function Name : Home_st9_DN
// Description;  //StepDown開始
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void Home_st9_DN()
{
	DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	DD_STM_SlowDownStop(DD_STM_ID_STACKER, TRAY_STK_ReceiveSteppingMotorEvent);
}


///////////////////////////////////////////////////////////////////
// Function Name : Home_st10_DN
// Description ;	//後励磁
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void Home_st10_DN()
{
	TRAY_STK_UpdatePulse( STK_DIR_UP, (UL)(10L + PROF_PULSE_STEP_45) );
	
	TRAY_STK_MotorUpDown_NVMUpdate(STK_DIR_UP);  //V3.0

	TRAY_STK_SetEventTimer(STK_MOT_SWITCH_TIME_50,  eSTK_NORMAL);
}


///////////////////////////////////////////////////////////////////
// Function Name : Home_st11_DN
// Description ;	//StackerHome動作完了
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void Home_st11_DN()
{
	ETRAY_STK_Action   action = gSTK.action;
	UC  	sheetNo = gSTK.sheetNo;

	//弱電流
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, LOW);
	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS, gSTK.action , (US)gSTK.stackerPulseCnt, gSTK.sheetNo); //Log

	// (CR-013) FullStack 解除条件Check
	 if ( SRVC_GetDeviceStatus(DSNO_STACKER_FULL_STACK) == DVST_TRAY_STK_STTS_NOT_FULL) {
		if( true == TRAY_Manager_ClearCheck()){
			//Lower Safety Warning Count Clear 
			//V3.4.0  T/B 未導入仕様(FullStack/LowerSafety発生後、Standby時にSNR変化ある場合、Stacker動作
			if (SRVC_GetDeviceStatus(DSNO_STACKER_LOW_SAFETY) == FAULT_OCCUR) {
				gSTK_FaultStat = FAULT_NOT_OCCUR;
				SRVC_UpdateDeviceStatus(FAULT_NOT_OCCUR, DSNO_STACKER_LOW_SAFETY) ; //Low Safety
			}
			gSTK.lowerSafetyCnt = 0;
		}
	 } else  {//FullStack 
	 	if ( true == TRAY_Manager_FullStackReleaseCondition() ) { //解除
			//Lower Safety Warning Count Clear 
			//V3.4.0  T/B 未導入仕様(FullStack/LowerSafety発生後、Standby時にSNR変化ある場合、Stacker動作
			if (SRVC_GetDeviceStatus(DSNO_STACKER_LOW_SAFETY) == FAULT_OCCUR) {
				gSTK_FaultStat = FAULT_NOT_OCCUR;
				SRVC_UpdateDeviceStatus(FAULT_NOT_OCCUR, DSNO_STACKER_LOW_SAFETY) ; //Low Safety
			}
			gSTK.lowerSafetyCnt = 0;
			TRAY_STK_SetPulse_FullStackRelease(0); // Pulse = 0 Clear
	 	}	
	}

	gSTK.action = TRAY_STK_st0;
	gSTK.sheetNo = SHEETNO_NUM;
	gSTK.startClock = START_CLK_UNKNOWN;
	gSTK.beforePulse = 0;
	gSTK.ejectRecvFlag = false;

	// NVM Save ..
	SRVC_SetNvmValueUL(gSTK.stackerPulseCnt, NVMID_ELEVATOR_MOT_PULSE_COUNT);
	TRAY_Manager_InformedStackerActionComplete(TRAY_STK_HOME,	sheetNo);
}

//Compile位置へ移動
//TRAY_STK_COMPILE = 0x0400,
///////////////////////////////////////////////////////////////////
// Function Name : Compile_st1
// Description ;  //StackerHeightSNRによる分岐判断
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void Compile_st1()
{
	ETRAY_STK_Action   action = gSTK.action;
	UC  	sheetNo = gSTK.sheetNo;
	

	if ( DD_DIO_GetInput(DD_DI_STACKER_TRAY_HEIGHT1_SNR) == HIGH) { // Lift UP

		if ( true == gSTK.standbyFailCheckFlag ) {
		    //FullStack/LowerSafety発生後、Standby時にSNR変化ある場合、Stacker動作Start ...
		    PrintStackerTrayLog_Short((UC)eSTK_LOG_DO_ORNOT,	0x00, (US)gSTK.action, sheetNo); 

		}//V3.3.92  TestROM :	排出動作時、 正常状態：H1,H2が０でない場合：LiftUp実施
		else if ( ( gSTK.jobType != eSTK_JOB_SHT_INIT) &&
			( gSTK.compoAction != TRAY_STK_COMPO_COMPILE) &&
			((DD_DIO_GetInput(DD_DI_STACKER_TRAY_HEIGHT2_SNR) == HIGH) )) { //

			if (TRAY_STK_Get_HPulse() == 0)  { // H1,H2が０の場合：動作しない

				gSTK.action = TRAY_STK_st0;
				gSTK.sheetNo = SHEETNO_NUM;
				PrintStackerTrayLog_Short((UC)eSTK_LOG_DO_ORNOT,	0xFF, (US)gSTK.action, sheetNo); 
				//Stacker動作後の更新は要らない: ->PF-DVT T/B.. V3.5.1 SetOVer
				TRAY_Manager_InformedStackerActionComplete(TRAY_STK_COMPILE,  sheetNo);
				TRAY_STK_ProcessEndCheck();
				return;
			}

		} else { // H1SNR=ON, H2SNR=OFF ,  or JobInit  or Component 

		}
		
	     gSTK.action = TRAY_STK_COMPILE_st2_UP;
	    PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,  (US)gSTK.action, (US)gSTK.stackerPulseCnt, sheetNo); 

	} else  {

		if ( DD_DIO_GetInput(DD_DI_STACKER_TRAY_HEIGHT2_SNR) == HIGH) {//Lift Down

			gSTK.action = TRAY_STK_COMPILE_st2_DN;
			PrintStackerTrayLog_Short((UC)eSTK_LOG_UPDOWN, 0, (US)gSTK.action , sheetNo); //Log

		} else {  //SetClamp 退避状態

			gSTK.action = TRAY_STK_st0;
			gSTK.sheetNo = SHEETNO_NUM;
			PrintStackerTrayLog_Short((UC)eSTK_LOG_UPDOWN, 0xFF,gSTK.action, sheetNo); //Log

			TRAY_Manager_InformedStackerActionComplete(TRAY_STK_COMPILE,  sheetNo);

			//Diag Component
			if ( gSTK.compoAction == TRAY_STK_COMPO_COMPILE) {
				TRAY_Manager_InformedStackerActionComplete((US) gSTK.compoAction ,  sheetNo);
				gSTK.compoAction = TRAY_STK_st0;
			}
			return;
		}
	}
	TRAY_STK_ControlThread();

}

///////////////////////////////////////////////////////////////////
// Function Name : Compile_st1
// Description;  //モータ駆動設定
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void Compile_st2_UP()
{
	//　上方向, 弱電流
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_ENB,  ON);
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_DIR,  STK_DIR_UP);
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, LOW);

	TRAY_STK_SetEventTimer(STK_MOT_ENABLE_TIME_30,	eSTK_NORMAL);

}

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st3_UP
 // Description;   //モータ駆動開始
 // Parameter	  : ‐	  // Return 	   : ‐
 ///////////////////////////////////////////////////////////////////
void Compile_st3_UP()
{
	 //強電流
	 DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, HIGH);

	 // Stacker Height1 SNR OFFから10Pulse後、StepDown
	 DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	 DD_STM_StartClock(DD_STM_ID_STACKER, PROF_PULSE_STEP_45, TRAY_STK_ReceiveStartClock);

	 gSTK.startClock = START_CLK_WAIT;	//Set Wait Flag
	 gSTK.beforePulse = 0;

	 //Clock Count Start ..
	 DD_STM_StartCountClock(DD_STM_ID_STACKER);

	 //Fail Timer
//	 TRAY_STK_SetEventTimer(STK_FAIL_TIME_1000,    eSTK_TRAY_FAIL_1);
	 TRAY_STK_SetEventTimer(STK_FAIL_TIME_5000,    eSTK_TRAY_FAIL_2);
	 // (V3.6.92)1000ms Caution .. PitchSkip 要求
	 TRAY_STK_SetEventTimer(STK_FAIL_TIME_1000, 	eSTK_PITCHSKIP_CHK);
	
	 // Reset Start ...
	 if (  DD_DIO_GetInput(DD_DI_STACKER_NO_PAPER_FULL_SNR) == LOW) {

		 gSTK.action = TRAY_STK_COMPILE_st4_UP_ONWait;

	 } else {

		 gSTK.action = TRAY_STK_COMPILE_st4_UP_OFFWait;
	 }
	  PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,  (US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 

}

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st4_UP_ONWait
 // Description;   //NoPaperSNR　ON待ち
 // Parameter	  : ‐	  // Return 	   : ‐
 ///////////////////////////////////////////////////////////////////
void Compile_st4_UP_ONWait()
{//NO ACTION
}

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st4_UP_OFFWait
 // Description;  //NoPaperSNR　OFF待ち
 // Parameter	  : ‐	  // Return 	   : ‐
 ///////////////////////////////////////////////////////////////////
void Compile_st4_UP_OFFWait()
{//NO ACTION
 }

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st5_UP
 // Description ; ;   //Reset開始
 // Parameter	  : ‐	  // Return 	   : ‐
 ///////////////////////////////////////////////////////////////////
void Compile_st5_UP()
{
	 // Reset動作中、SNR1=OFFとなった場合のPulse計算のため, Saveしておく
	 gSTK.beforePulse = DD_STM_StopCountClock(DD_STM_ID_STACKER);

	 // Restart Pulse Count
	 DD_STM_StartCountClock(DD_STM_ID_STACKER);
	 PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,  (US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 

}

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st6_UP
 // Description;   //Reset完了
 // Parameter	  : ‐	  // Return 	   : ‐
 ///////////////////////////////////////////////////////////////////
void Compile_st6_UP()
{
	 UL    movePls;

	 //Reset開始～Reset完了区間パルス計算
	 movePls = DD_STM_StopCountClock(DD_STM_ID_STACKER);

	 //Restart	Pulse Count
	 DD_STM_StartCountClock(DD_STM_ID_STACKER);

	 TRAY_STK_ResetPulse(movePls);

	 gSTK.beforePulse = 0;  //Pulse更新が終わったのでClearする

}

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st7_UP
 // Description;   //SNR1　OFF受信(10Pulse開始)
 // Parameter	  : ‐	  // Return 	   : ‐
 ///////////////////////////////////////////////////////////////////
void Compile_st7_UP()
{
	 UL 	movePls;

	 // ResetPulseカウントのため、途中で中断したPulseも計算する
	 movePls = DD_STM_StopCountClock(DD_STM_ID_STACKER) +  gSTK.beforePulse;

	 gSTK.beforePulse = 0;
	 TRAY_STK_UpdatePulse( STK_DIR_UP, movePls);
	 PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS, gSTK.action , (US)gSTK.stackerPulseCnt, gSTK.sheetNo); //Log

	 //10Pulse start ..
	 DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	 DD_STM_AppointPulseTiming(DD_STM_ID_STACKER, 10, TRAY_STK_ReceiveSteppingMotorEvent);

 }

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st8_UP
 // Description ;   //StepDown開始
 // Parameter	  : ‐	  // Return 	   : ‐
 ///////////////////////////////////////////////////////////////////
void Compile_st8_UP()
{
	DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	 DD_STM_SlowDownStop(DD_STM_ID_STACKER, TRAY_STK_ReceiveSteppingMotorEvent);
	 PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,  (US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 

 }

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st9_UP
 // Description;   //タイマ(方向転換用)設定
 // Parameter	  : ‐	  // Return 	   : ‐
 ///////////////////////////////////////////////////////////////////
void Compile_st9_UP()
{
	 // 10Pulse + StepDown
	 TRAY_STK_UpdatePulse( STK_DIR_UP, (UL)(10L+PROF_PULSE_STEP_45));

	 TRAY_STK_MotorUpDown_NVMUpdate(STK_DIR_UP);  //V3.0
	 PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,  (US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 

	 //50ms Timer set
	 TRAY_STK_SetEventTimer(STK_MOT_SWITCH_TIME_50, eSTK_NORMAL);

}

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st10_UP
 // Description;  //モータ駆動開始(下向き)
 // Parameter	  : ‐	  // Return 	   : ‐
 ///////////////////////////////////////////////////////////////////
void Compile_st10_UP()
{
		
	// (CR-013) FullStack 解除条件Check
	 if ( SRVC_GetDeviceStatus(DSNO_STACKER_FULL_STACK) == DVST_TRAY_STK_STTS_NOT_FULL) {
		if( true == TRAY_Manager_ClearCheck()){
			//Lower Safety Warning Count Clear 
			//V3.4.0  T/B 未導入仕様(FullStack/LowerSafety発生後、Standby時にSNR変化ある場合、Stacker動作
			if (SRVC_GetDeviceStatus(DSNO_STACKER_LOW_SAFETY) == FAULT_OCCUR) {
				gSTK_FaultStat = FAULT_NOT_OCCUR;
				SRVC_UpdateDeviceStatus(FAULT_NOT_OCCUR, DSNO_STACKER_LOW_SAFETY) ; //Low Safety
			}
			gSTK.lowerSafetyCnt = 0;
		}
	 } else  {//FullStack 
	 	if ( true == TRAY_Manager_FullStackReleaseCondition() ) { //解除
			//Lower Safety Warning Count Clear 
			//V3.4.0  T/B 未導入仕様(FullStack/LowerSafety発生後、Standby時にSNR変化ある場合、Stacker動作
			if (SRVC_GetDeviceStatus(DSNO_STACKER_LOW_SAFETY) == FAULT_OCCUR) {
				gSTK_FaultStat = FAULT_NOT_OCCUR;
				SRVC_UpdateDeviceStatus(FAULT_NOT_OCCUR, DSNO_STACKER_LOW_SAFETY) ; //Low Safety
			}
			gSTK.lowerSafetyCnt = 0;
			TRAY_STK_SetPulse_FullStackRelease(0); // Pulse = 0 Clear
	 	}	
	}
	
	//　下方向、
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_DIR,  STK_DIR_DOWN);
	// 強電流
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, HIGH);

	// Stacker Height1 SNR ONからH3Pulse後、StepDown
	DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	DD_STM_StartClock(DD_STM_ID_STACKER, PROF_PULSE_STEP_45, TRAY_STK_ReceiveStartClock);

	gSTK.startClock = START_CLK_WAIT;  //Set Wait Flag
	gSTK.beforePulse = 0;

	//Clock Count Start ..
	DD_STM_StartCountClock(DD_STM_ID_STACKER);
	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	(US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 

 }


 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st11_UP
 // Description;   //SNR1　ON受信(G-Pulse開始)
 // Parameter	  : ‐	  // Return 	   : ‐
 ///////////////////////////////////////////////////////////////////
void Compile_st11_UP()
 {

	US   G_Pulse;
	US	 afterPls,  movePls;

	  // Stacker Height1 SNR ONから G-Pulse後、StepDown
	G_Pulse =TRAY_STK_Get_GPulse();

	afterPls = DD_STM_StopCountClock(DD_STM_ID_STACKER);

	movePls = gSTK.beforePulse + afterPls;

	TRAY_STK_UpdatePulse( STK_DIR_DOWN, (UL)movePls);
	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	(US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 


	if ( gSTK.beforePulse > 0) {  // StepUp中、SNR1 ON

		if ( G_Pulse > afterPls) {

			movePls = G_Pulse - afterPls;  // StepUp中、SNR ONしたときからの移動パルス分を引く

			//Stacker Height1 SNR ONから G-Pulse後、StepDown
			DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
			DD_STM_AppointPulseTiming(DD_STM_ID_STACKER, movePls, TRAY_STK_ReceiveSteppingMotorEvent);

			TRAY_STK_UpdatePulse( STK_DIR_DOWN, (UL)movePls);

		} else {  // G-Pulse 無し-> StepDown

			gSTK.action = TRAY_STK_COMPILE_st12_UP;
			TRAY_STK_ControlThread();
		}

		gSTK.beforePulse=0;

	} else { // gSTK.beforePulse==0

		if ( G_Pulse > 0)  {

			//Stacker Height1 SNR ONからG-Pulse後、StepDown
			DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
			DD_STM_AppointPulseTiming(DD_STM_ID_STACKER, G_Pulse, TRAY_STK_ReceiveSteppingMotorEvent);

			TRAY_STK_UpdatePulse( STK_DIR_DOWN, (UL)G_Pulse);

		} else {	// G-Pulse 無し-> StepDown

			gSTK.action = TRAY_STK_COMPILE_st12_UP;
			TRAY_STK_ControlThread();
		}
	}

 }

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st12_UP
 // Description ;   //StepDown開始
 // Parameter	  : ‐	  // Return 	   : ‐
 ///////////////////////////////////////////////////////////////////
 void Compile_st12_UP()
{
	DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	 DD_STM_SlowDownStop(DD_STM_ID_STACKER, TRAY_STK_ReceiveSteppingMotorEvent);

	 TRAY_STK_UpdatePulse( STK_DIR_DOWN, (UL)PROF_PULSE_STEP_45);
 	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	(US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 
 }

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st2_DN
 // Description;  //モータ駆動設定
 // Parameter	  : ‐	  // Return 	   : ‐
 ///////////////////////////////////////////////////////////////////
void Compile_st2_DN()
{
	 //Mot Enable,　下方向、 弱電流
	 DD_DIO_SetOutput(DD_DO_STACKER_MOT_ENB,  ON);
	 DD_DIO_SetOutput(DD_DO_STACKER_MOT_DIR,  STK_DIR_DOWN);
	 DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, LOW);

	 TRAY_STK_SetEventTimer(STK_MOT_ENABLE_TIME_30,  eSTK_NORMAL);

 }

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st3_DN
 // Description ; ;   //モータ駆動開始
 // Parameter	  : ‐	  // Return 	   : ‐
 ///////////////////////////////////////////////////////////////////
void Compile_st3_DN()
{
	 //強電流
	 DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, HIGH);

	// Stacker Height1 SNR ONからH3Pulse後、StepDown
	DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	DD_STM_StartClock(DD_STM_ID_STACKER, PROF_PULSE_STEP_45, TRAY_STK_ReceiveStartClock);

	gSTK.startClock = START_CLK_WAIT;  //Set Wait Flag
	gSTK.beforePulse = 0;

	//Clock Count Start ..
	DD_STM_StartCountClock(DD_STM_ID_STACKER);

	 // Fail Timer (Stacker LowerSafety Warning Counter)
	 TRAY_STK_SetEventTimer(STK_FAIL_TIME_1000, eSTK_LOWER_SAFETY);
	 PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,  (US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 

 }

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st4_DN()
 // Description;  //SNR1 ON受信(G-Pulse開始)
 // Parameter	  : ‐	  // Return 	   : ‐
 ///////////////////////////////////////////////////////////////////
void Compile_st4_DN()
{
	 US   G_Pulse;
	 US   afterPls,  movePls;


	  // Stacker Height1 SNR ONから G-Pulse後、StepDown
	G_Pulse = TRAY_STK_Get_GPulse();

	 afterPls = DD_STM_StopCountClock(DD_STM_ID_STACKER);

	 movePls = gSTK.beforePulse + afterPls;

	 TRAY_STK_UpdatePulse( STK_DIR_DOWN, (UL)movePls);
	 PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,  gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 


	 if ( gSTK.beforePulse > 0) {  // StepUp中、SNR1 ON

		 if ( G_Pulse > afterPls) {

			 movePls = G_Pulse - afterPls;	// StepUp中、SNR ONしたときからの移動パルス分を引く

			 //Stacker Height1 SNR ONから G-Pulse後、StepDown
			 DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
			 DD_STM_AppointPulseTiming(DD_STM_ID_STACKER, movePls, TRAY_STK_ReceiveSteppingMotorEvent);

			 TRAY_STK_UpdatePulse( STK_DIR_DOWN, (UL)movePls);

		 } else {  // G-Pulse 無し-> StepDown

			 gSTK.action = TRAY_STK_COMPILE_st5_DN;
			 TRAY_STK_ControlThread();
		 }

		 gSTK.beforePulse=0;

	 } else { // gSTK.beforePulse==0

		 if ( G_Pulse > 0)	{

			 //Stacker Height1 SNR ONからG-Pulse後、StepDown
			 DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
			 DD_STM_AppointPulseTiming(DD_STM_ID_STACKER, G_Pulse, TRAY_STK_ReceiveSteppingMotorEvent);

			 TRAY_STK_UpdatePulse( STK_DIR_DOWN, (UL)G_Pulse);

		 } else {	 // G-Pulse 無し-> StepDown

			 gSTK.action = TRAY_STK_COMPILE_st5_DN;
			 TRAY_STK_ControlThread();
		 }
	 }

 }

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st5_DN
 // Description;   //StepDown開始
 // Parameter	  : ‐	  // Return 	   : ‐
 ///////////////////////////////////////////////////////////////////
void Compile_st5_DN()
{
	 DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	 DD_STM_SlowDownStop(DD_STM_ID_STACKER, TRAY_STK_ReceiveSteppingMotorEvent);

	 TRAY_STK_UpdatePulse( STK_DIR_DOWN, (UL)PROF_PULSE_STEP_45);
	  //V03.00.90  PreDVT T/B 対策
	 if ( gSTK.jobType != eSTK_JOB_SHT_INIT) 
  	 TRAY_Manager_PreviousPulseUpdate(gSTK.stackerPulseCnt); 
	  //↑↑↑↑↑↑
 }

 ///////////////////////////////////////////////////////////////////
 // Function Name : Compile_st13
 // Description; 	//Compile位置へ移動完了
 // Parameter	  : ‐	  // Return 	   : ‐
 ///////////////////////////////////////////////////////////////////
void Compile_st13()
{
	 ETRAY_STK_Action	 action = gSTK.action;
	 ETRAY_STK_Action compoAction = gSTK.compoAction;
	 UC 			 sheetNO = gSTK.sheetNo;

	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	(US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 

	gSTK.action = TRAY_STK_st0;
	gSTK.compoAction = TRAY_STK_st0;
	gSTK.sheetNo = SHEETNO_NUM;
	gSTK.startClock = START_CLK_UNKNOWN;
	gSTK.beforePulse = 0;

	
	//弱電流
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, LOW);

	TRAY_STK_MotorUpDown_NVMUpdate(STK_DIR_DOWN);  //V3.0
	
   	if ( (compoAction == TRAY_STK_COMPO_COMPILE) || (compoAction == TRAY_STK_COMPO_EJECT) ) {
		   TRAY_Manager_InformedStackerActionComplete((US)compoAction,	sheetNO);
	 } else {
		   SRVC_SetNvmValueUL(gSTK.stackerPulseCnt, NVMID_ELEVATOR_MOT_PULSE_COUNT); 
		   CNvmInformer_SaveNvm();

		   if ( false == gSTK.standbyFailCheckFlag ) { //StandbyFailCheck動作でない場合
		   	
			   TRAY_Manager_InformedStackerActionComplete(TRAY_STK_COMPILE, sheetNO);

		   } else {
				// V3.4.0 Faultがすべてクリアされたら、Flagをクリアする
				//未導入仕様　FullStack/LowerSafety発生後、Standby時にSNR変化ある場合、Stacker動作
				//FullStackCheck .->. No FullStack && No LowerSafety
				if (( SRVC_GetDeviceStatus( DSNO_STACKER_FULL_STACK ) == DVST_TRAY_STK_STTS_NOT_FULL ) &&
					 (SRVC_GetDeviceStatus(DSNO_STACKER_LOW_SAFETY) == FAULT_NOT_OCCUR) ){
			 
					TRAY_STK_StandbyFailCheckStop();  
					return;
				}
		   }
	}
	
	// ProcessEnd Job Start Check ...-> 関数化
	if ( ( gSTK.jobType != eSTK_JOB_SHT_INIT) &&
		( gSTK.compoAction != TRAY_STK_COMPO_COMPILE) ) {
		TRAY_STK_ProcessEndCheck();
	}
}

//Eject位置へ移動(ADJUST)
//TRAY_STK_EJECT = 0x0500,
///////////////////////////////////////////////////////////////////
// Function Name : Eject_st1
// Description ;  //T1タイマ待ち
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void Eject_st1()
{
	if( SRVC_GetStapleMode(gSTK.sheetNo) == eCdiFinStaple_Off ) {  //UnStaple

		TRAY_STK_SetEventTimer(SRVC_GetNvmValueUS(NVMID_UN_STAPLE_STACKER_DOWN_TIMING_AT_EJECT)* 10 ,
								  eSTK_NORMAL);

	} else {

		TRAY_STK_SetEventTimer(SRVC_GetNvmValueUS(NVMID_CORNER_STAPLE_STACKER_DOWN_TIMING_AT_EJECT)* 10 ,
								  eSTK_NORMAL);
	}
//	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	(US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 
}

///////////////////////////////////////////////////////////////////
// Function Name : Eject_st2
// Description  ;  //モータ駆動設定
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void Eject_st2()
{
	//Mot Enable、 弱電流
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_ENB,  ON);	
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_DIR,  STK_DIR_DOWN);	
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, LOW);	

	TRAY_STK_SetEventTimer(STK_MOT_ENABLE_TIME_20,  eSTK_NORMAL);
//	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	(US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 
}

///////////////////////////////////////////////////////////////////
// Function Name : Eject_st3
// Description  ;  //モータ駆動開始
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void Eject_st3()
{	
	US    totalPulse;
	UC    targetIndex;
	
	totalPulse = TRAY_STK_Get_HPulse();

	if(totalPulse  < 3) { //　移動しない
	
		gSTK.action =  TRAY_STK_EJECT_st4;
		TRAY_STK_ControlThread();

	} else {

		//強電流
		DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, HIGH);	
	
	
		TRAY_STK_GetProfilePulse(totalPulse, &targetIndex);

		DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
		//STK Motor Start 
		DD_STM_MoveBySetPulse(DD_STM_ID_STACKER, targetIndex, totalPulse, TRAY_STK_ReceiveSteppingMotorEvent);

		TRAY_STK_UpdatePulse( STK_DIR_DOWN, totalPulse);

		TRAY_STK_MotorUpDown_NVMUpdate(STK_DIR_DOWN);  //V3.0
		PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	(US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 
	//Fail Timer ..		
//		TRAY_STK_SetEventTimer(STK_FAIL_TIME_1000,	  eSTK_TRAY_FAIL_1);
	}

}

///////////////////////////////////////////////////////////////////
// Function Name :Eject_st4
// Description ;  //Eject位置へ移動完了
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void Eject_st4()
{
	ETRAY_STK_Action   action = gSTK.action;
	UC  	sheetNo = gSTK.sheetNo;

	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	(US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 			

	//弱電流
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, LOW);	

	gSTK.action = TRAY_STK_st0;
	gSTK.sheetNo = SHEETNO_NUM;

	
	//Diag Component
	if ( gSTK.compoAction == TRAY_STK_COMPO_EJECT ) {
		TRAY_Manager_InformedStackerActionComplete((US)gSTK.compoAction,  sheetNo);
		gSTK.compoAction = TRAY_STK_st0;
	} else {
	
		SRVC_SetNvmValueUL(gSTK.stackerPulseCnt, NVMID_ELEVATOR_MOT_PULSE_COUNT);
		TRAY_Manager_InformedStackerActionComplete(TRAY_STK_EJECT,  sheetNo);
	}
}


//待機位置へ移動(ProcessEnd)
//TRAY_STK_END = 0x0600,
///////////////////////////////////////////////////////////////////
// Function Name : End_st1
// Description  ;   //モータ駆動開始
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void End_st1()
{
	ETRAY_STK_Action   action = gSTK.action;
	UC  	sheetNo = gSTK.sheetNo;

	if ( DD_DIO_GetInput(DD_DI_STACKER_TRAY_HEIGHT1_SNR) == HIGH) { // Lift UP 
	
		//　上方向,強電流
		DD_DIO_SetOutput(DD_DO_STACKER_MOT_ENB,  ON);
		DD_DIO_SetOutput(DD_DO_STACKER_MOT_DIR,  STK_DIR_UP);
		DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, HIGH);	
		
		DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
		DD_STM_StartClock(DD_STM_ID_STACKER, PROF_PULSE_STEP_45, TRAY_STK_ReceiveStartClock);
		
		gSTK.startClock = START_CLK_WAIT;
		gSTK.beforePulse = 0;
		
	} else  {

		gSTK.action = TRAY_STK_st0;
		gSTK.sheetNo = SHEETNO_NUM;
		gSTK.processEndFlag = false; //V3.3.92

		TRAY_Manager_InformedStackerActionComplete(TRAY_STK_END,  sheetNo);
				
		return;
	}	
		
	//Clock Count Start ..
	DD_STM_StartCountClock(DD_STM_ID_STACKER);

	//Fail Timer
//	TRAY_STK_SetEventTimer(STK_FAIL_TIME_1000,	  eSTK_TRAY_FAIL_1);
	TRAY_STK_SetEventTimer(STK_FAIL_TIME_5000,	  eSTK_TRAY_FAIL_2);

	// Reset Start ...
	if (  DD_DIO_GetInput(DD_DI_STACKER_NO_PAPER_FULL_SNR) == LOW) {

		gSTK.action = TRAY_STK_END_st2_ONWait;

	} else {

		gSTK.action = TRAY_STK_END_st2_OFFWait;
	}
	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	(US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 			

}

///////////////////////////////////////////////////////////////////
// Function Name : End_st2_ONWait
// Description  ;   ;  //NoPaperSNR　ON待ち
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void End_st2_ONWait()
{//NO ACTION
}
///////////////////////////////////////////////////////////////////
// Function Name : End_st2_OFFWait
// Description  ; ;  //NoPaperSNR　OFF待ち
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void End_st2_OFFWait()
{//NO ACTION
}

///////////////////////////////////////////////////////////////////
// Function Name : End_st3
// Description  ;  ;  //Reset開始
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void End_st3()
{
	// Reset動作中、SNR1=OFFとなった場合のPulse計算のため, Saveしておく
	gSTK.beforePulse = DD_STM_StopCountClock(DD_STM_ID_STACKER);
	
	// Restart Pulse Count 
	DD_STM_StartCountClock(DD_STM_ID_STACKER);
}

///////////////////////////////////////////////////////////////////
// Function Name : End_st4
// Description ;  //Reset完了
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void End_st4()
{
	UL	  movePls;

	//Reset開始～Reset完了区間パルス計算
	movePls = DD_STM_StopCountClock(DD_STM_ID_STACKER);
	
	//Restart  Pulse Count 
	DD_STM_StartCountClock(DD_STM_ID_STACKER);
	
	TRAY_STK_ResetPulse(movePls);	

	gSTK.beforePulse = 0;  //Pulse更新が終わったのでClearする

}

///////////////////////////////////////////////////////////////////
// Function Name : End_st5
// Description;  //SNR1　OFF受信(10Pulse開始)
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void End_st5()
{
	UL	   movePls;
	
	// ResetPulseカウントのため、途中で中断したPulseも計算する
	movePls = DD_STM_StopCountClock(DD_STM_ID_STACKER) +  gSTK.beforePulse;
	
	gSTK.beforePulse = 0;
	TRAY_STK_UpdatePulse( STK_DIR_UP, movePls);

	TRAY_STK_MotorUpDown_NVMUpdate(STK_DIR_UP);  //V3.0

	//10Pulse start .. 
	DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	DD_STM_AppointPulseTiming(DD_STM_ID_STACKER, 10, TRAY_STK_ReceiveSteppingMotorEvent);

}

///////////////////////////////////////////////////////////////////
// Function Name : End_st6
// Description ;  //StepDown開始
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void End_st6()
{
	DD_STM_SetProfileID(DD_STM_ID_STACKER,  gSTK.profile);  //DD_STM_PROF_ID_STACKER_UPDOWN);  //V3.3.92
	DD_STM_SlowDownStop(DD_STM_ID_STACKER, TRAY_STK_ReceiveSteppingMotorEvent);
}

///////////////////////////////////////////////////////////////////
// Function Name : End_st7
// Description ; //後励磁
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
void End_st7()
{
	//弱電流	
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_CUR, LOW); 

	TRAY_STK_UpdatePulse( STK_DIR_UP, (UL)(10L + PROF_PULSE_STEP_45));
	
	//50ms Timer set 
	TRAY_STK_SetEventTimer(STK_MOT_SWITCH_TIME_50, eSTK_NORMAL);

}

///////////////////////////////////////////////////////////////////
// Function Name : End_st8
// Description;  //待機位置へ移動完了
// Parameter     : ‐    // Return        : ‐
///////////////////////////////////////////////////////////////////
 void End_st8()
{
	ETRAY_STK_Action    action = gSTK.action;
	UC				sheetNO = gSTK.sheetNo;
	
	//モータ停止
	DD_DIO_SetOutput(DD_DO_STACKER_MOT_ENB,  OFF);
	//Cancel Fail Timer..
//	TRAY_STK_CancelEventTimer(eSTK_TRAY_FAIL_1);
	TRAY_STK_CancelEventTimer(eSTK_TRAY_FAIL_2);
	PrintStackerTrayLog_Short((UC)eSTK_LOG_STATUS,	(US)gSTK.action, (US)gSTK.stackerPulseCnt, gSTK.sheetNo); 	

	gSTK.action = TRAY_STK_st0;
	gSTK.sheetNo = SHEETNO_NUM;
	gSTK.startClock = START_CLK_UNKNOWN;
	gSTK.beforePulse = 0; 		
	gSTK.processEndFlag = false; //V3.3.92
	
	SRVC_SetNvmValueUL(gSTK.stackerPulseCnt, NVMID_ELEVATOR_MOT_PULSE_COUNT);
	CNvmInformer_SaveNvm();
	TRAY_Manager_InformedStackerActionComplete(TRAY_STK_END,  sheetNO);
		
}

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

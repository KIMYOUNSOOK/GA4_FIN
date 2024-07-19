// File Name:TRNS_Device.c
// Description:	Source File of Entrance Controller
// Engineer:	Yuichi Hoshino
// Last Edit:	14.09.23
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "TRNS_Device.h"

#include "DD_DIO.h"
#include "DD_SQP.h"
#include "ENvmID.h"
#include "NvmService.h"
#include "CNvmInformer.h"
#include "MSP_EventTimer.h"
#include "CDvstInformer.h"
#include "CCvrChecker.h"
// #include "CJamChecker.h"
#ifndef FXK_TEST_BINGATE
#include "CCdiAdapter.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
//    NVMID_TRANSPORT_MOT_SPEED_V1 = 26,
//    NVMID_TRANSPORT_MOT_SPEED_V2 = 27,
//    NVMID_TRANSPORT_MOT_DOWN_START_T1_1 = 28,
/////////////////////////////////////////////////////////////////////////////////////////

typedef struct
{
	void (*mpHandler)(void) ;		

} TRNS_DEVICE_pFunc ;

// --- Member of Entrance Controller ---
typedef struct /*TRNSMember*/
{
	ETrnsControlStep mStep;
	ETrnsControlStep compoAction;
	UC mSheet[TRNS_CONT_SHT_NUM];
	UC mNextIndex;		// IOT Exit Snr On ++
	UC mRunningIndex;	// ComExit On ++
	UC mStopSheetNo;	// Stop Fin
	UC mIOTGatePullTimeA;
	UC mIOTGatePullTimeB;	
	UC mIOTGatePullAddCountC;		
	UC mIOTGatePullRepeatC;
	UC mIDTransSpeed;
	US mTransSpeed;	
	US mIOTSpeed;
	US mMediaWeight;
	SS mSJamChkTimerID;
	SS mTrnsIotT3TimerID;
	SS mIOTGateTimerID; //mIOTGateTimerID
} CTrnsControl;

static CTrnsControl gTrnsControl;

#define TRNS_PAPER_TYPE 1 // 0 : No Coat Paper 1: Coat Paper

/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

// Lje modi#define GetFrequency(aSpeed) (((UL)(aSpeed) * 100 * 225 * 50) / (18 * 314 * 100))		// UL (US)	戻り値：周波数[?0..1Hz](小数?第２位以下は切り捨てる)/引数：速度[?0.1mm/s]
// #define GetFrequency(aSpeed) (((UL)(aSpeed) * 100 * 2 * 50) / (20 * 314))		// UL (US)	戻り値：周波数[?0..1Hz](小数?第２位以下は切り捨てる)/引数：速度[?0.1mm/s]

static void TRNS_ControlThread(void);
static void TRNS_StopControlSJamCheck();
static void StopControlJamClearAssist();
static void StartControlSheet(UC sheetNo);
static void StopControlSheet(UC sheetNo);

static void TRNS_Device_StartCompoTransportMotorGateSolenoid(US aLinkNo);
static void TRNS_InformedCompoControlStart(SS aTimerID, UL aChainLink);

static void TRNS_sjam_chk_st1();
static void TRNS_sjam_chk_st2();
static void TRNS_sjam_chk_st3();
static void TRNS_sjam_chk_st4();
static void TRNS_exit_st1();
static void TRNS_exit_st2();
static void TRNS_exit_st2_T3();
static void TRNS_exit_st3();
static void TRNS_exit_st4();
static void TRNS_exit_st4_T4();
static void TRNS_exit_st5();
static void TRNS_exit_st6();
static void TRNS_exit_st7();
static void TRNS_exit_st8();
static void TRNS_exit_st9();

static void TRNS_diag_st1();
static void TRNS_diag_st2();
static void TRNS_diag_st3();
static UL  GetFrequency(US aSpeed);
static US TRNS_GetIotT3Timer(UC sheetNo);
static US TRNS_GetComExitT4Timer(UC sheetNo);
static void TRNS_ReceiveEventTimeUpT3(SS aTimerID, UL aType);


/////////////////////////////////////////////////////////////////////////////////////////
// Timer Callback Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

static const TRNS_DEVICE_pFunc ControlThread_TRNS_DIAG[ ] = {
	{NULL			},
	{TRNS_diag_st1	}, //
	{TRNS_diag_st2	}, //
	{TRNS_diag_st3	}, //

};

static const TRNS_DEVICE_pFunc ControlThread_TRNS_SJAM_CHK[ ] = {
	{NULL					},
	{TRNS_sjam_chk_st1	}, // TransMotClock開始
	{TRNS_sjam_chk_st2	}, // TransMot駆動開始
	{TRNS_sjam_chk_st3	}, // TimeOut待ち
    {TRNS_sjam_chk_st4	}, // Clock停止
};

static const TRNS_DEVICE_pFunc ControlThread_TRNS_EXIT[ ] = {
	{NULL			},
	{TRNS_exit_st1	}, // モータClock開始
	{TRNS_exit_st2	}, // TransMot駆動開始
	{TRNS_exit_st2_T3	}, // モータ用紙IOT速度まで減速	
	{TRNS_exit_st3	}, // No Action ComExitSnrOn待ち 
      {TRNS_exit_st4	}, // ComExitSnrOn状態 : 減速  終了用 T4 Time Set 
//      {TRNS_exit_st4_T4	}, // モータ用紙搬送速度まで増速 ComExitSnrOff待ち     
	{TRNS_exit_st5	}, // ComExitSnrOff状態
	{TRNS_exit_st6	}, // モータ減速
	{TRNS_exit_st7	}, // モータ用紙搬送速度まで増速
	{TRNS_exit_st8	}, //  ﾓｰﾀ停止判断
	{TRNS_exit_st9	}, // 終了     
};
// --- Distance. Table of Dynamic Jam ---
US const TrnsDevDistance[TRNS_DISTANSEID_NUM] = {
	// !!! Product Specific !!!
	1157, // 756,	1177	// IOT筺体の用紙搬出部フレームとIOT Fusing-Roll間パス長 (R)
	243,			//  IOT筺体の用紙搬出部フレームからENTRANCE-ROLLまでの距離   (E)
	1300,			// ENTRANCE-ROLLからCompile-Exit Sensorまでの距離            (K)
	350, // 216,	350		// IOT筺体の用紙搬出部フレームとIOT Exit Sensor(Hot Line)間パス長	
};

/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : TrnsController_Constructor
// Description   : コンストラク?
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_Device_Constructor()
{
	UC index;

	gTrnsControl.mNextIndex = 0;
	gTrnsControl.mRunningIndex = 0;	
	gTrnsControl.mTransSpeed = 0;
	gTrnsControl.mSJamChkTimerID = -1;
	gTrnsControl.mTrnsIotT3TimerID = -1;
	gTrnsControl.mStopSheetNo  =SHEETNO_NUM; 

	for ( index = 0; index < TRNS_CONT_SHT_NUM; index++ ) {
		gTrnsControl.mStep = TRNS_CONT_STEP_INACTIVE;
		gTrnsControl.mSheet[index]		 = SHEETNO_NUM; 
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_StartAct
// Description   : 
// Parameter     : 
// Return        :
///////////////////////////////////////////////////////////////////
void TRNS_StartAct(ETrnsControlStep aStep, UC aSheetNo)
{
	UC index;
	US TimerI;
	switch ( aStep ) {

	// Component 動作 //////////////////////////////////////////////////////////////////////////////
	case TRNS_COMPO_FORWARD_LOW_SPEED : 
		gTrnsControl.compoAction = aStep;
		gTrnsControl.mTransSpeed = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_SPEED_V2) * 10;
		gTrnsControl.mStep = TRNS_DIAG_st1;
		break;

	case TRNS_COMPO_FORWARD_MID_SPEED : 
		gTrnsControl.compoAction = aStep;
		gTrnsControl.mTransSpeed = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_SPEED_V3) * 10; // Speed * 10
		gTrnsControl.mStep = TRNS_DIAG_st1;
		break;

	case TRNS_COMPO_FORWARD_HI_SPEED : 
		gTrnsControl.compoAction = aStep;
		gTrnsControl.mTransSpeed = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_SPEED_V5) * 10;; // Speed * 10 
		gTrnsControl.mStep = TRNS_DIAG_st1;
		break;

	case TRNS_COMPO_GATE_SOLENOID_PULL : 
		gTrnsControl.compoAction = aStep;
		gTrnsControl.mStep = TRNS_DIAG_st1;
		break;

	case TRNS_COMPO_GATE_SOLENOID_PUSH : 
		gTrnsControl.compoAction = aStep;
		gTrnsControl.mStep = TRNS_DIAG_st1;
		break;
	// Component 動作 //////////////////////////////////////////////////////////////////////////////

	case TRNS_SJAM_CHK : //= 0x0200, 
		if ( gTrnsControl.mStep == TRNS_CONT_STEP_IDLE || gTrnsControl.mStep == TRNS_CONT_STEP_INACTIVE ) {
		gTrnsControl.mStep = TRNS_SJAM_CHK_st1;
		} else {  /* No Action */
			PrintTransLog(0x01, 0xFF, (US) gTrnsControl.mStep , gTrnsControl.mTransSpeed);
			return;  
		}
		break;

 	case TRNS_EXIT :  // = 0x0300, 	 	

		index = gTrnsControl.mNextIndex;

		// --- インデックス更新 ---
		gTrnsControl.mNextIndex++; // 1,0
		gTrnsControl.mNextIndex &= TRNS_CONT_SHT_MASK;

		// --- シ?ト情報保持 ---

		gTrnsControl.mSheet[index] = aSheetNo;
		gTrnsControl.mIOTSpeed = SRVC_GetExitInitialSpeed(aSheetNo);		
		gTrnsControl.mMediaWeight =  SRVC_GetMediaWeight(aSheetNo);
		if(  gTrnsControl.mStep == TRNS_CONT_STEP_IDLE  ) {

			if( /*(CDvstInformer_GetDeviceCondtion(DEVID_FINISHER) == DEV_COND_PERM_NG) ||*/
					(CCvrChecker_GetCoverStatus(CVR_ID_NUM) == CVR_STTS_OPEN ) || 
					(CJamChecker_GetJamZoneStatus(JAM_ZONE_ID_NUM) == JAM_ZONE_STTS_SET)){
					index = 0;
					PrintTransLog(0x01, 0xFE, gTrnsControl.mStep,gTrnsControl.mTransSpeed );
						return; 				
			}		

			// CR-0xx 2015.11.05
			// else if ( SRVC_GetNvmValueUS(NVMID_FINISHER_SPEED) == 2 ) {
				// if( SRVC_GetProcessSize(aSheetNo) > TRNS_PAPER_LENGTH297 ) { 
				//	gTrnsControl.mTransSpeed = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_SPEED_V3)*10;//TRNS_MOTSPEED_600; // 600 *10
				// } else {
				//	gTrnsControl.mTransSpeed = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_SPEED_V4)*10;//TRNS_MOTSPEED_600; // 600 *10
				// }
			// }
			gTrnsControl.mStep = TRNS_EXIT_st1;

		} else  {
			TimerI = TRNS_GetIotT3Timer( aSheetNo);
			gTrnsControl.mTrnsIotT3TimerID = MSP_SetEventTimer(TimerI, TRNS_ReceiveEventTimeUpT3, 0);
			PrintTransLog(0x02, aSheetNo, gTrnsControl.mStep,TimerI );
			return;
		}
		
		// if( SRVC_GetExitSpeedChangeTimes(sheetNo) > 0 ) {
			// 取りあえず速度変更1回のみ対応する
			// gTrnsControl.mTransportTimerID = MSP_SetEventTimer(SRVC_GetExitSpeedChangeTimming(sheetNo, 1), ExitSpeedChangeSheetCallback, (UL)sheetNo);
		// }
	 	break;
			
	default :  
		break;
	}
	PrintTransLog(0x01, aSheetNo, gTrnsControl.mStep,gTrnsControl.mTransSpeed );

	TRNS_ControlThread();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_ControlThread
// Description   : 
// Parameter     : 
// Return        :
///////////////////////////////////////////////////////////////////
void TRNS_ControlThread()
{
	ETrnsControlStep Step = gTrnsControl.mStep & TRNS_CONT_EXT_MASK;
	US 				 Status = gTrnsControl.mStep & 0x00FF;

	if ( Status == 0 )  {
		TRNS_Device_EmgStop();
		return;
	}
	
	switch (Step ) {
	case TRNS_DIAG : 
		ControlThread_TRNS_DIAG[Status].mpHandler(); 		break;
		
	case TRNS_SJAM_CHK : 
		ControlThread_TRNS_SJAM_CHK[Status].mpHandler();	break;
		
	case TRNS_EXIT : 
		ControlThread_TRNS_EXIT[Status].mpHandler();		break;
			
	default : 									break;
	}

}
///////////////////////////////////////////////////////////////////
// Function Name : TRNS_EmgStopEventTimeUp
// Description   : イベントタイマのタイムアップ
// Parameter     : timerID (Timer ID)
//               :    aType
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRNS_EmgStopEventTimeUp(SS aTimerID, UL aType)
{
	DD_SQP_Stop(DD_SQP_ID_TRANSPORT);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRAY_STK_ReceiveEventTimeUp
// Description   : イベントタイマのタイムアップ
// Parameter     : timerID (Timer ID)
//               :    aType
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRNS_ReceiveEventTimeUp(SS aTimerID, UL aType)
{
	if ( aTimerID != gTrnsControl.mSJamChkTimerID ) {  
		return;
	}

	gTrnsControl.mSJamChkTimerID = MSP_MAX_TMR_NUM;

	switch ( gTrnsControl.mStep ) {

	// Component動作開始
	case TRNS_DIAG_st1 :
	case TRNS_DIAG_st2 :	
		gTrnsControl.mStep++;
		TRNS_ControlThread();
		break;

	case TRNS_SJAM_CHK_st1 : 
	case TRNS_SJAM_CHK_st2 : 
	case TRNS_SJAM_CHK_st3 : 
	case TRNS_SJAM_CHK_st4 : 
	case TRNS_EXIT_st1 : 
	case TRNS_EXIT_st2 : 
	case TRNS_EXIT_st3 : 
	case TRNS_EXIT_st4 : 
	// case TRNS_EXIT_st4_T4 : 		
	case TRNS_EXIT_st5 : 
	case TRNS_EXIT_st6 : 
	case TRNS_EXIT_st7 : 
	case TRNS_EXIT_st8 : 
	case TRNS_EXIT_st9 : 
		gTrnsControl.mStep++;
		TRNS_ControlThread();
		break;

	default:
		break;
	}
}
///////////////////////////////////////////////////////////////////
// Function Name : TRNS_GATE_ReceiveEventTimeUp
// Description   : イベントタイマのタイムアップ
// Parameter     : timerID (Timer ID)
//               :    aType
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRNS_IOTGATE_ReceiveEventTimeUp(SS aTimerID, UL aType)
{
	// if ( aTimerID != gTrnsControl.mBinGateTimerID ) {  
	// 	return;
	//  }
	gTrnsControl.mIOTGateTimerID = MSP_MAX_TMR_NUM;

	switch ( gTrnsControl.mStep ) {
		case TRNS_CONT_STEP_INACTIVE :
		case TRNS_CONT_STEP_IDLE :
		case TRNS_DIAG_st1 :
		case TRNS_DIAG_st2 :
		case TRNS_DIAG_st3 :
		case TRNS_DIAG_st4 :			
		case TRNS_SJAM_CHK_st1 :
		case TRNS_SJAM_CHK_st2 :
		case TRNS_SJAM_CHK_st3 :
		case TRNS_SJAM_CHK_st4 :		
		case TRNS_EXIT_st1 : 
		case TRNS_EXIT_st2 : 
		case TRNS_EXIT_st3 : 
		case TRNS_EXIT_st4 : 
		// case TRNS_EXIT_st4_T4 : 		
		case TRNS_EXIT_st5 : 
		case TRNS_EXIT_st6 : 
		case TRNS_EXIT_st7 : 
		case TRNS_EXIT_st8 : 
		case TRNS_EXIT_st9 : 
			if(aType == TRNS_IOTGATE_PUSH){
				 TRNS_Device_IOTGateSol_AllOff();
			} else if ( aType == TRNS_IOTGATE_PULL_STEP100MS ) { // 100ms -> A -> B -> A -> B
				TRNS_Device_IOTGateSol_AllOff();
				gTrnsControl.mIOTGateTimerID = MSP_SetEventTimer( gTrnsControl.mIOTGatePullTimeA, TRNS_IOTGATE_ReceiveEventTimeUp, TRNS_IOTGATE_PULL_STEP_A);
			} else if ( aType == TRNS_IOTGATE_PULL_STEP_A ) { // 100ms -> A (Off) -> B(Pull) ->   A(Off) -> B(Pull)
				 TRNS_Device_IOTGateSol_Pull();
				gTrnsControl.mIOTGateTimerID = MSP_SetEventTimer( gTrnsControl.mIOTGatePullTimeB, TRNS_IOTGATE_ReceiveEventTimeUp, TRNS_IOTGATE_PULL_STEP_B);
			} else if ( aType == TRNS_IOTGATE_PULL_STEP_B ) { // 100ms -> A 	   -> B(Off)   -> A -> B
				gTrnsControl.mIOTGateTimerID = MSP_SetEventTimer( gTrnsControl.mIOTGatePullTimeA, TRNS_IOTGATE_ReceiveEventTimeUp, TRNS_IOTGATE_PULL_STEP_A);
				TRNS_Device_IOTGateSol_AllOff();
			} else if ( aType == TRNS_IOTGATE_PULL_STEP_C_ON ) { // 100ms -> A (Off)   -> B(Pull)   -> A(Off) -> B(Pull)   --> C_ON(Pull) -> C_OFF(Off)
				if ( gTrnsControl.mIOTGatePullRepeatC > 0 ) {
					gTrnsControl.mIOTGateTimerID = MSP_SetEventTimer( gTrnsControl.mIOTGatePullTimeA, TRNS_IOTGATE_ReceiveEventTimeUp, TRNS_IOTGATE_PULL_STEP_C_OFF);
					gTrnsControl.mIOTGatePullRepeatC--;
				}
				TRNS_Device_IOTGateSol_AllOff();
			} else if ( aType == TRNS_IOTGATE_PULL_STEP_C_OFF ) { // 100ms -> A (Off)   -> B(Pull)   -> A(Off) -> B(Pull)    --> C_ON(Pull) -> C_OFF(Off)
				if ( gTrnsControl.mIOTGatePullRepeatC == 0 ) { // Change PUSH
				 	TRNS_Device_IOTGateSol_Push();
					gTrnsControl.mIOTGateTimerID =	MSP_SetEventTimer(TRNS_GSOL_WAITTIME_100, TRNS_IOTGATE_ReceiveEventTimeUp, TRNS_IOTGATE_PUSH);
				} else {  //  Continue PUSH Repeat C
				 	TRNS_Device_IOTGateSol_Pull();
					gTrnsControl.mIOTGateTimerID = MSP_SetEventTimer( gTrnsControl.mIOTGatePullTimeB, TRNS_IOTGATE_ReceiveEventTimeUp, TRNS_IOTGATE_PULL_STEP_C_ON);
				}
			}
			break;
		default:
			break;
					
	}
	
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_ReceiveEventTimeUpT3
// Description   : イベントタイマのタイムアップ
// Parameter     : timerID (Timer ID)
//               :    aType
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRNS_ReceiveEventTimeUpT3(SS aTimerID, UL aType)
{
	if ( aTimerID != gTrnsControl.mTrnsIotT3TimerID  ) {  
		return;
	}

	gTrnsControl.mTrnsIotT3TimerID = MSP_MAX_TMR_NUM;

	switch ( gTrnsControl.mStep ) {
	case TRNS_EXIT_st2 : 
	case TRNS_EXIT_st3 : // ComExit Snr On待ち	
	case TRNS_EXIT_st7 : 		
		gTrnsControl.mStep = TRNS_EXIT_st2_T3;
		TRNS_ControlThread();
		break;

	case TRNS_EXIT_st4 : // ComExit Snr On状態ComExit Snr Off 待ち	
		PrintTransLog(0x08, 0x11, gTrnsControl.mStep , gTrnsControl.mIOTSpeed);
		gTrnsControl.mStep = TRNS_EXIT_st4;
		DD_SQP_SetFrequency(DD_SQP_ID_TRANSPORT, GetFrequency( gTrnsControl.mTransSpeed));
		break;		

	default:
		break;
	}
}

UL GetFrequency(US aSpeed)
{
	float CalcuA, CalcuB,CalcuC;
	UL returnValue;

	CalcuA = ((float)(aSpeed) * 100 * 2.25 * 50);
	CalcuB = (float)18 * (float)314;
	CalcuC = CalcuA/ CalcuB;
	returnValue = (UL) CalcuC;

	return returnValue;
	 	// UL (US)	戻り値：周波数[?0..1Hz](小数?第２位以下は切り捨てる)/引数：速度[?0.1mm/s]
}
US TRNS_GetIotT3Timer(UC sheetNo)
{
	US timer, paraLength,paramSum;

	// paramSum = TrnsDevDistance[TRNS_DISTANSEID_R] + TrnsDevDistance[TRNS_DISTANSEID_E] +TrnsDevDistance[TRNS_DISTANSEID_K] -TrnsDevDistance[TRNS_DISTANSEID_S] ;
	paraLength = SRVC_GetProcessSize(sheetNo);
	paramSum = paraLength - TrnsDevDistance[TRNS_DISTANSEID_S];
	timer = (US)(((UL)(paramSum) * (UL)1000) / gTrnsControl.mIOTSpeed );
	if ( timer < TRNS_MINUSTIME_20 ){
		timer = 0;
	}else {
		timer = timer - TRNS_MINUSTIME_20;
	}
	PrintTransLog(0x01, 0xFC, gTrnsControl.mStep,timer );

	return timer;
}
US TRNS_GetComExitT4Timer(UC sheetNo)
{
	US timer, paramPS, paramSum;

	paramPS = SRVC_GetProcessSize(sheetNo);
	paramSum = TrnsDevDistance[TRNS_DISTANSEID_R] + TrnsDevDistance[TRNS_DISTANSEID_E] +TrnsDevDistance[TRNS_DISTANSEID_K] ;

	if ( paramPS <= paramSum ) {
		timer = 0;
	} else {
		timer = (US)(((UL)(paramPS-paramSum) * (UL)1000) / gTrnsControl.mIOTSpeed);
	}

	return timer;
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_SJamChk_CancelEventTimer
// Description   : 
// Parameter     : 
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRNS_SJamChk_CancelEventTimer()
{
	if ( gTrnsControl.mSJamChkTimerID != MSP_MAX_TMR_NUM ) {
		MSP_CancelEventTimer(gTrnsControl.mSJamChkTimerID);
		gTrnsControl.mSJamChkTimerID = MSP_MAX_TMR_NUM;
	}
}
///////////////////////////////////////////////////////////////////
// Function Name : TRNS_IotT3_CancelEventTimer
// Description   : 
// Parameter     : 
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRNS_IotT3_CancelEventTimer()
{
	if ( gTrnsControl.mTrnsIotT3TimerID != MSP_MAX_TMR_NUM ) {
		MSP_CancelEventTimer(gTrnsControl.mTrnsIotT3TimerID);
		gTrnsControl.mTrnsIotT3TimerID = MSP_MAX_TMR_NUM;
	}
}
///////////////////////////////////////////////////////////////////
// Function Name : TRNS_IotT3_CancelEventTimer
// Description   : 
// Parameter     : 
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRNS_IOTGateTimer_CancelEventTimer()
{
	if ( gTrnsControl.mIOTGateTimerID != MSP_MAX_TMR_NUM ) {
		MSP_CancelEventTimer(gTrnsControl.mIOTGateTimerID);
		gTrnsControl.mIOTGateTimerID = MSP_MAX_TMR_NUM;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_diag_st1
// Description   : Height1SensorON Event 受信
//
///////////////////////////////////////////////////////////////////
void TRNS_diag_st1()
{
	if( gTrnsControl.mStep != TRNS_DIAG_st1) return;

	switch ( gTrnsControl.compoAction ) {
	case TRNS_COMPO_FORWARD_LOW_SPEED :
	case TRNS_COMPO_FORWARD_MID_SPEED :
	case TRNS_COMPO_FORWARD_HI_SPEED :
		DD_DIO_SetOutput(DD_DO_TRANSPORT_MOT_DIR, ON);
		DD_SQP_SetFrequency(DD_SQP_ID_TRANSPORT, GetFrequency( gTrnsControl.mTransSpeed));	
		DD_SQP_Start(DD_SQP_ID_TRANSPORT);	

		gTrnsControl.mSJamChkTimerID = MSP_SetEventTimer(TRNS_SETTIME_50, TRNS_ReceiveEventTimeUp, 0);
		break;

	case TRNS_COMPO_GATE_SOLENOID_PULL :
		DD_DIO_SetOutput(DD_DO_TRANSPORT_MOT_GATE_SOL_PULL, ON);
		DD_DIO_SetOutput(DD_DO_TRANSPORT_MOT_GATE_SOL_PUSH, OFF);
		gTrnsControl.mSJamChkTimerID = MSP_SetEventTimer(TRNS_SETTIME_200, TRNS_ReceiveEventTimeUp, 0);
		break;
	
	case TRNS_COMPO_GATE_SOLENOID_PUSH :
		DD_DIO_SetOutput(DD_DO_TRANSPORT_MOT_GATE_SOL_PULL, OFF);
		DD_DIO_SetOutput(DD_DO_TRANSPORT_MOT_GATE_SOL_PUSH, ON);
		gTrnsControl.mSJamChkTimerID = MSP_SetEventTimer(TRNS_SETTIME_200, TRNS_ReceiveEventTimeUp, 0);
		break;

	default :
		break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_diag_st2
// Description   : Height1SensorON Event 受信
//
///////////////////////////////////////////////////////////////////
void TRNS_diag_st2()
{
	if ( gTrnsControl.mStep != TRNS_DIAG_st2 ) return;

	switch ( gTrnsControl.compoAction ) {
	case TRNS_COMPO_FORWARD_LOW_SPEED :
	case TRNS_COMPO_FORWARD_MID_SPEED :
	case TRNS_COMPO_FORWARD_HI_SPEED :
		DD_DIO_SetOutput(DD_DO_TRANSPORT_MOT_BRAKE, ON);	// Brake 解除
		// DD_DIO_SetOutput(DD_DO_TRANSPORT_MOT_BRAKE, OFF);	// Brake 解除
		break;

	case TRNS_COMPO_GATE_SOLENOID_PULL :
	case TRNS_COMPO_GATE_SOLENOID_PUSH :
		DD_DIO_SetOutput(DD_DO_TRANSPORT_MOT_GATE_SOL_PULL, OFF);
		DD_DIO_SetOutput(DD_DO_TRANSPORT_MOT_GATE_SOL_PUSH, OFF);
		gTrnsControl.mSJamChkTimerID = MSP_SetEventTimer(0, TRNS_ReceiveEventTimeUp, 0);
		break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_diag_st3
// Description   : 
//
///////////////////////////////////////////////////////////////////
void TRNS_diag_st3()
{
	if ( gTrnsControl.mStep != TRNS_DIAG_st3 ) return;

	switch ( gTrnsControl.compoAction ) {
	case TRNS_COMPO_FORWARD_LOW_SPEED :
	case TRNS_COMPO_FORWARD_MID_SPEED :
	case TRNS_COMPO_FORWARD_HI_SPEED :
		DD_SQP_Stop(DD_SQP_ID_TRANSPORT);	// Clock停止
		TRNS_InformedCompoControlComplete((US)TRNS_COMPO_CHAIN, (US)gTrnsControl.compoAction, ACT_STOP);
		break;

	case TRNS_COMPO_GATE_SOLENOID_PULL :
	case TRNS_COMPO_GATE_SOLENOID_PUSH :
		DD_DIO_SetOutput(DD_DO_TRANSPORT_MOT_GATE_SOL_PULL, OFF);
		DD_DIO_SetOutput(DD_DO_TRANSPORT_MOT_GATE_SOL_PUSH, OFF);
		TRNS_InformedCompoControlComplete((US)TRNS_COMPO_CHAIN, (US)gTrnsControl.compoAction, ACT_STOP);
		break;

	default :
		break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_sjam_chk_st1
// Description   : TransMotClock開始
//
///////////////////////////////////////////////////////////////////
void TRNS_sjam_chk_st1 ()
{
	if ( gTrnsControl.mStep != TRNS_SJAM_CHK_st1 ) return;

	DD_SQP_SetFrequency(DD_SQP_ID_TRANSPORT, GetFrequency(4077));			// Set Clock Frequency(350.0[mm/s])
	DD_SQP_Start(DD_SQP_ID_TRANSPORT);
	gTrnsControl.mSJamChkTimerID = MSP_SetEventTimer(TRNS_SETTIME_15, TRNS_ReceiveEventTimeUp, 0);
	PrintTransLog(0x07, 1, gTrnsControl.mStep , gTrnsControl.mTransSpeed);	
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_sjam_chk_st2
// Description   : TransMot駆動開始
//
///////////////////////////////////////////////////////////////////
void TRNS_sjam_chk_st2()
{
	if ( gTrnsControl.mStep != TRNS_SJAM_CHK_st2)
		return;

	DD_DIO_SetOutput(DD_DO_TRANSPORT_MOT_BRAKE, ON); 						// Brake ON
	gTrnsControl.mSJamChkTimerID = MSP_SetEventTimer(TRNS_SETTIME_1500, TRNS_ReceiveEventTimeUp, 0);
	PrintTransLog(0x07, 2, gTrnsControl.mStep , gTrnsControl.mTransSpeed);		
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_sjam_chk_st3
// Description   : TimeOut待ち
//
///////////////////////////////////////////////////////////////////
void TRNS_sjam_chk_st3()
{
	if ( gTrnsControl.mStep != TRNS_SJAM_CHK_st3 )
		return;

	DD_DIO_SetOutput(DD_DO_TRANSPORT_MOT_BRAKE, OFF); 						// Brake OFF
	gTrnsControl.mSJamChkTimerID = MSP_SetEventTimer(TRNS_SETTIME_50, TRNS_ReceiveEventTimeUp, 0);
	PrintTransLog(0x07, 3, gTrnsControl.mStep , gTrnsControl.mTransSpeed);		
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_sjam_chk_st4
// Description   : Clock停止
//
///////////////////////////////////////////////////////////////////
void TRNS_sjam_chk_st4()
{
	if ( gTrnsControl.mStep != TRNS_SJAM_CHK_st4 ) return;

	DD_SQP_Stop(DD_SQP_ID_TRANSPORT);										// Stop Clock
	gTrnsControl.mStep = TRNS_CONT_STEP_IDLE;
	PrintTransLog(0x07, 4, gTrnsControl.mStep , gTrnsControl.mTransSpeed);	
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_EXIT_st1
// Description   : モータClock開始
//
///////////////////////////////////////////////////////////////////
void TRNS_exit_st1()
{
	US TimerI, TransMediaWeight, aIOTSpeed;

	if ( gTrnsControl.mStep != TRNS_EXIT_st1 ) 	return;

	TransMediaWeight = SRVC_GetMediaWeight(gTrnsControl.mSheet[gTrnsControl.mRunningIndex]);
	if( TransMediaWeight >= TRNS_MEDIA_WEIGHT_HEAVY ){
		aIOTSpeed = 4100;
	} else {
		aIOTSpeed = gTrnsControl.mIOTSpeed;
	}
	DD_SQP_SetFrequency(DD_SQP_ID_TRANSPORT, GetFrequency( aIOTSpeed));			// Set Clock Frequency(350.0[mm/s])
	DD_SQP_Start(DD_SQP_ID_TRANSPORT);	
	gTrnsControl.mSJamChkTimerID = MSP_SetEventTimer(TRNS_SETTIME_15, TRNS_ReceiveEventTimeUp, 0);

	TimerI = TRNS_GetIotT3Timer( gTrnsControl.mSheet[gTrnsControl.mRunningIndex]);	
	gTrnsControl.mTrnsIotT3TimerID = MSP_SetEventTimer(TimerI, TRNS_ReceiveEventTimeUpT3, 0);
	PrintTransLog(0x08, gTrnsControl.mSheet[gTrnsControl.mRunningIndex], gTrnsControl.mStep , aIOTSpeed);

}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_exit_st2
// Description   : TransMot駆動開始
//
///////////////////////////////////////////////////////////////////
void TRNS_exit_st2()
{

	if( gTrnsControl.mStep != TRNS_EXIT_st2) return;

	DD_DIO_SetOutput(DD_DO_TRANSPORT_MOT_BRAKE, ON); 						// Brake ON
	PrintTransLog(0x08, gTrnsControl.mSheet[gTrnsControl.mRunningIndex], gTrnsControl.mStep , gTrnsControl.mIOTSpeed);
	// gTrnsControl.mStep = TRNS_EXIT_st2_T3;	
}
///////////////////////////////////////////////////////////////////
// Function Name : TRNS_exit_st3_T3
// Description   : T3 Time Up Event 待ち 
//
///////////////////////////////////////////////////////////////////
void TRNS_exit_st2_T3()
{
	/* No Action */
	DD_SQP_SetFrequency(DD_SQP_ID_TRANSPORT, GetFrequency( gTrnsControl.mTransSpeed));
	PrintTransLog(0x08, gTrnsControl.mSheet[gTrnsControl.mRunningIndex], gTrnsControl.mStep , gTrnsControl.mTransSpeed);
	gTrnsControl.mStep = TRNS_EXIT_st3;
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_exit_st3
// Description   : No Action ComExitSnrOn Event 待ち 
//
///////////////////////////////////////////////////////////////////
void TRNS_exit_st3()
{
	PrintTransLog(0x08, gTrnsControl.mSheet[gTrnsControl.mRunningIndex], gTrnsControl.mStep , gTrnsControl.mTransSpeed);
	/* No Action */
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_exit_st4
// Description   : ComExitSnrOn状態 
//
///////////////////////////////////////////////////////////////////
void TRNS_exit_st4 ()
{
	US TimerI;
	UC RunSheetBufNo ;
	
	/* No Action */
	TRNS_SJamChk_CancelEventTimer();
	// TRNS_IotT3_CancelEventTimer();
	
	// RunSheetBufNo = (( gTrnsControl.mRunningIndex -1 ) & TRNS_CONT_SHT_MASK);
	// TimerI = TRNS_GetComExitT4Timer( gTrnsControl.mSheet[gTrnsControl.mRunningIndex]);
	// gTrnsControl.mSJamChkTimerID = MSP_SetEventTimer(TimerI, TRNS_ReceiveEventTimeUp, 0);
}
///////////////////////////////////////////////////////////////////
// Function Name : TRNS_exit_st4_Spd600
// Description   : T4　後 モータ用紙搬送速度まで増速 ComExitSnrOff待ち状態 
//
///////////////////////////////////////////////////////////////////
void TRNS_exit_st4_T4 ()
{
	/* No Action */
	DD_SQP_SetFrequency(DD_SQP_ID_TRANSPORT, GetFrequency( gTrnsControl.mTransSpeed));
	PrintTransLog(0x08, gTrnsControl.mSheet[gTrnsControl.mRunningIndex], gTrnsControl.mStep , gTrnsControl.mTransSpeed);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_exit_st5
// Description   : ComExitSnrOff状態 受信
//
///////////////////////////////////////////////////////////////////
void TRNS_exit_st5()
{
	UC TransMediaType; //ECdiFinMediaType
	US TransMediaWeight, aTimeValueT1;
	UC RunSheetBufNo ;

	if ( gTrnsControl.mStep != TRNS_EXIT_st5 )	return;

	RunSheetBufNo = (( gTrnsControl.mRunningIndex -1 ) & TRNS_CONT_SHT_MASK);
	TransMediaType = SRVC_GetMediaType(gTrnsControl.mSheet[RunSheetBufNo]);
	// 20160226 V3.4.0
	if ( gTrnsControl.mIDTransSpeed == TRNS_SPEED_ID_V5 ) {

 		if ( TransMediaType == eCdiFinMediaType_Coated ) {
			aTimeValueT1 = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_DOWN_START_T1_4);
 		} else {
 			if ( TransMediaType == eCdiFinMediaType_Plain ){
				TransMediaWeight = SRVC_GetMediaWeight(gTrnsControl.mSheet[RunSheetBufNo]);

				if( TransMediaWeight >= TRNS_MEDIA_WEIGHT_HEAVY ){
					aTimeValueT1 = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_DOWN_START_T1_4);

				} else {
					aTimeValueT1 = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_DOWN_START_T1_3);
				}
			} else {
				aTimeValueT1 = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_DOWN_START_T1_3);
 			}
		}

	} else {
 		if ( TransMediaType == eCdiFinMediaType_Coated ) {
			aTimeValueT1 = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_DOWN_START_T1_2);
 		} else {
 			if ( TransMediaType == eCdiFinMediaType_Plain ){
				TransMediaWeight = SRVC_GetMediaWeight(gTrnsControl.mSheet[RunSheetBufNo]);

				if( TransMediaWeight >= TRNS_MEDIA_WEIGHT_HEAVY ){
					aTimeValueT1 = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_DOWN_START_T1_2);

				} else {
					aTimeValueT1 = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_DOWN_START_T1_1);
				}
			} else {
				aTimeValueT1 = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_DOWN_START_T1_1);
 			}
		}
		
	}
	gTrnsControl.mSJamChkTimerID = MSP_SetEventTimer(aTimeValueT1, TRNS_ReceiveEventTimeUp, 0);	
	PrintTransLog(0x08, gTrnsControl.mIDTransSpeed, gTrnsControl.mStep , aTimeValueT1);

}

//////////////////////////////////////////////////////////////////////////////////////////
// Function Name : TRNS_exit_st6
// Description   : Compile Exit Snr OFF Event 受信 ~ T1　後 モータ減速開始
//
// 15.06.02	| LeeJE	| CR012  [ TRANSPORTとIOT速度差によるJAM対策導入権 ] V02.01.00
/////////////////////////////////////////////////////////////////////////////////////////
void TRNS_exit_st6()
{
	UC TransMediaType; //ECdiFinMediaType
	US TransMediaWeight, TempTransSpeed, aTimeValueT2;
	UC RunSheetBufNo ;
	
	if ( gTrnsControl.mStep != TRNS_EXIT_st6 ) 	return;
	RunSheetBufNo = (( gTrnsControl.mRunningIndex -1 ) & TRNS_CONT_SHT_MASK);

	TransMediaType = SRVC_GetMediaType(gTrnsControl.mSheet[RunSheetBufNo]);
	// 20160226 V3.4.0
	if ( gTrnsControl.mIDTransSpeed == TRNS_SPEED_ID_V5 ) {

 		if ( TransMediaType == eCdiFinMediaType_Coated ) {

			TempTransSpeed = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_SPEED_V1)*10;

			aTimeValueT2 = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_DOWN_START_T2_4);

		} else {
 			if ( TransMediaType == eCdiFinMediaType_Plain ){
				TransMediaWeight = SRVC_GetMediaWeight(gTrnsControl.mSheet[RunSheetBufNo]);

				if( TransMediaWeight >= TRNS_MEDIA_WEIGHT_HEAVY ){

					TempTransSpeed = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_SPEED_V1)*10;

					aTimeValueT2 = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_DOWN_START_T2_4);
				} else {

					TempTransSpeed = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_SPEED_V2)*10;

					aTimeValueT2 = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_DOWN_START_T2_3);
				}
			} else {

				TempTransSpeed = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_SPEED_V2)*10;

				aTimeValueT2 = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_DOWN_START_T2_3);
 			}
		}
	}else {
 		if ( TransMediaType == eCdiFinMediaType_Coated ) {

			TempTransSpeed = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_SPEED_V1)*10;

			aTimeValueT2 = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_DOWN_START_T2_2);

		} else {
 			if ( TransMediaType == eCdiFinMediaType_Plain ){
				TransMediaWeight = SRVC_GetMediaWeight(gTrnsControl.mSheet[RunSheetBufNo]);

				if( TransMediaWeight >= TRNS_MEDIA_WEIGHT_HEAVY ){

					TempTransSpeed = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_SPEED_V1)*10;
					
					aTimeValueT2 = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_DOWN_START_T2_2);

				} else {

					TempTransSpeed = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_SPEED_V2)*10;

					aTimeValueT2 = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_DOWN_START_T2_1);					
				}
			} else {

				TempTransSpeed = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_SPEED_V2)*10;

				aTimeValueT2 = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_DOWN_START_T2_1);				
 			}
		}

	}
	
	DD_SQP_SetFrequency(DD_SQP_ID_TRANSPORT, GetFrequency(TempTransSpeed)); 		// Set Clock Frequency(350.0[mm/s])

	gTrnsControl.mSJamChkTimerID = MSP_SetEventTimer(aTimeValueT2, TRNS_ReceiveEventTimeUp, 0);

	PrintTransLog(0x08, gTrnsControl.mIDTransSpeed, gTrnsControl.mStep , aTimeValueT2);
	// gTrnsControl.mSJamChkTimerID = MSP_SetEventTimer(TRNS_SETTIME_150, TRNS_ReceiveEventTimeUp, 0);

}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_exit_st7
// Description   : T2　後 モータIOT用紙搬送速度まで増速
//
///////////////////////////////////////////////////////////////////
void TRNS_exit_st7()
{
	US aIOTSpeed;
	if ( gTrnsControl.mStep != TRNS_EXIT_st7 ) 	return;

#ifndef FXK_NOSTOP
	if( /* (CDvstInformer_GetDeviceCondtion(DEVID_FINISHER) == DEV_COND_PERM_NG) ||*/
		(CCvrChecker_GetCoverStatus(CVR_ID_NUM) == CVR_STTS_OPEN )){ // || 
			TRNS_Device_EmgStop();
			PrintTransLog(0x0A, 0x0C, gTrnsControl.mStep , gTrnsControl.mTransSpeed);				
			return;
		}
#endif
	if( gTrnsControl.mMediaWeight >= TRNS_MEDIA_WEIGHT_HEAVY ){
		aIOTSpeed = 4100;
	} else {
		aIOTSpeed = gTrnsControl.mIOTSpeed;		
	}
	DD_SQP_SetFrequency(DD_SQP_ID_TRANSPORT, GetFrequency(aIOTSpeed));

	if ( gTrnsControl.mSheet[gTrnsControl.mRunningIndex] != gTrnsControl.mSheet[gTrnsControl.mNextIndex]  ) {
		PrintTransLog(0x08, gTrnsControl.mSheet[gTrnsControl.mRunningIndex], gTrnsControl.mStep , aIOTSpeed);		
		gTrnsControl.mStep = TRNS_EXIT_st3;
	} else {
		gTrnsControl.mSJamChkTimerID = MSP_SetEventTimer(TRNS_SETTIME_1100, TRNS_ReceiveEventTimeUp, 0);
		PrintTransLog(0x09, 0x10, gTrnsControl.mStep , gTrnsControl.mIOTSpeed);		
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_exit_st8
// Description   :  ﾓｰﾀ停止判断
//
///////////////////////////////////////////////////////////////////
void TRNS_exit_st8()
{
	if ( gTrnsControl.mStep != TRNS_EXIT_st8 ) return;

	if ( gTrnsControl.mSheet[gTrnsControl.mRunningIndex] != gTrnsControl.mSheet[gTrnsControl.mNextIndex]  ) {
		PrintTransLog(0x08, gTrnsControl.mSheet[gTrnsControl.mRunningIndex], gTrnsControl.mStep , gTrnsControl.mIOTSpeed);		
		gTrnsControl.mStep = TRNS_EXIT_st3;
	} else {
	
		DD_DIO_SetOutput(DD_DO_TRANSPORT_MOT_BRAKE, OFF); 						// Brake OFF	
		gTrnsControl.mSJamChkTimerID = MSP_SetEventTimer(TRNS_SETTIME_50, TRNS_ReceiveEventTimeUp, 0);
		PrintTransLog(0x09, 0x11 , gTrnsControl.mStep , gTrnsControl.mIOTSpeed);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_exit_st9
// Description   :  終了 
//
///////////////////////////////////////////////////////////////////
void TRNS_exit_st9()
{
  	UC index;
		
	if ( gTrnsControl.mStep != TRNS_EXIT_st9) return;

	if ( gTrnsControl.mSheet[gTrnsControl.mRunningIndex] != gTrnsControl.mSheet[gTrnsControl.mNextIndex]  ) {
		DD_DIO_SetOutput(DD_DO_TRANSPORT_MOT_BRAKE, ON); 
		PrintTransLog(0x08, gTrnsControl.mSheet[gTrnsControl.mRunningIndex], gTrnsControl.mStep , gTrnsControl.mTransSpeed);
		gTrnsControl.mStep = TRNS_EXIT_st3;
		return;
	}

	for ( index = 0; index < TRNS_CONT_SHT_NUM; index++ ) {
		gTrnsControl.mSheet[index]		 = SHEETNO_NUM;
	}

	DD_SQP_Stop(DD_SQP_ID_TRANSPORT);										// Stop Clock
	PrintTransLog(0x09, 0x22, gTrnsControl.mStep , gTrnsControl.mTransSpeed);
	gTrnsControl.mStep = TRNS_CONT_STEP_IDLE;

	if ( TRNS_MGR_GetState() == TRNS_MGR_STT_CYCLE_DOWN || TRNS_MGR_GetState() == TRNS_MGR_STT_HARD_DOWN ) {
		TRNS_MGR_SheetControlComplete(TRNS_CONT_ID_ENTRANCE, 0);
	}		 

	
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_StopControlSJamCheck
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_StopControlSJamCheck()
{
	switch ( gTrnsControl.mStep ) {
	case TRNS_SJAM_CHK :
	case TRNS_SJAM_CHK_st1 :
	case TRNS_SJAM_CHK_st2 :
	case TRNS_SJAM_CHK_st3 :
	case TRNS_SJAM_CHK_st4 :
		if ( gTrnsControl.mSJamChkTimerID != -1 ) {
			MSP_CancelEventTimer( gTrnsControl.mSJamChkTimerID );
			gTrnsControl.mSJamChkTimerID = -1;
		} else { /* No Action */ }

		if ( gTrnsControl.mTrnsIotT3TimerID != -1 ) {
			MSP_CancelEventTimer( gTrnsControl.mTrnsIotT3TimerID );
			gTrnsControl.mTrnsIotT3TimerID = -1;
		} else { /* No Action */ }
		DD_SQP_Stop(DD_SQP_ID_TRANSPORT);								// Stop Clock
		DD_DIO_SetOutput(DD_DO_TRANSPORT_MOT_BRAKE, OFF);					// Brake OFF
		PrintTransLog(0x0A, 0x0A, gTrnsControl.mStep , gTrnsControl.mTransSpeed);	
		gTrnsControl.mStep = TRNS_CONT_STEP_IDLE;
		TRNS_MGR_JamCheckComplete(TRNS_CONT_ID_ENTRANCE);
		break;

	default:
		// No Action
		PrintTransLog(0x0A, 0x0B, gTrnsControl.mStep , gTrnsControl.mTransSpeed);	
		break;
	}

}

///////////////////////////////////////////////////////////////////
// Function Name : StopControlJamClearAssist
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void StopControlJamClearAssist()
{
	switch ( gTrnsControl.mStep ) {
	case TRNS_SJAM_CHK :
	case TRNS_SJAM_CHK_st1 :
	case TRNS_SJAM_CHK_st2 :
	case TRNS_SJAM_CHK_st3 :
	case TRNS_SJAM_CHK_st4 :		
		if ( gTrnsControl.mSJamChkTimerID != -1 ) {
			MSP_CancelEventTimer( gTrnsControl.mSJamChkTimerID );
			gTrnsControl.mSJamChkTimerID = -1;
		} else { /* No Action */ }

		if ( gTrnsControl.mTrnsIotT3TimerID != -1 ) {
			MSP_CancelEventTimer( gTrnsControl.mTrnsIotT3TimerID );
			gTrnsControl.mTrnsIotT3TimerID = -1;
		} else { /* No Action */ }

		DD_SQP_Stop(DD_SQP_ID_TRANSPORT);										// Stop Clock
		DD_DIO_SetOutput(DD_DO_TRANSPORT_MOT_BRAKE, OFF);					// Brake OFF
		PrintTransLog(0x0A, 0x01, gTrnsControl.mStep , gTrnsControl.mTransSpeed);
		gTrnsControl.mStep = TRNS_CONT_STEP_IDLE;

		TRNS_MGR_JamClearAssistComplete(TRNS_CONT_ID_ENTRANCE);

		break;

	default:
		// No Action
		PrintTransLog(0x0A, 0x02, gTrnsControl.mStep , gTrnsControl.mTransSpeed);
		break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : StopControlSheet
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void StopControlSheet(UC sheetNo)
{
	UC index, otherSheetIdx;

	for ( index = 0; index < TRNS_CONT_SHT_NUM; index++ ) {
		if( gTrnsControl.mSheet[index] == sheetNo ) {
			// if( gTrnsControl.mSJamChkTimerID != -1 ) {
			//	MSP_CancelEventTimer( gTrnsControl.mSJamChkTimerID);
			//	gTrnsControl.mSJamChkTimerID = -1;
			// }

			// gTrnsControl.mStep = TRNS_CONT_STEP_IDLE;
			// gTrnsControl.mSheet[index] = SHEETNO_NUM;
			TRNS_MGR_SheetControlComplete(TRNS_CONT_ID_ENTRANCE, sheetNo);
			PrintTransLog(0x0A, 0x03, gTrnsControl.mStep , gTrnsControl.mTransSpeed);			
		} else { /* Go for loop */ }
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_Device_EmgStop
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_Device_EmgStop()
{
	UC index;

	DD_DIO_SetOutput(DD_DO_TRANSPORT_MOT_BRAKE, OFF);
	MSP_SetEventTimer(TRNS_SETTIME_50, TRNS_EmgStopEventTimeUp, 0);
	// DD_SQP_Stop(DD_SQP_ID_TRANSPORT);
	TRNS_MGR_SheetControlComplete(TRNS_CONT_ID_ENTRANCE, 0);
	TRNS_SJamChk_CancelEventTimer();
	TRNS_IotT3_CancelEventTimer();

	gTrnsControl.mStep = TRNS_CONT_STEP_IDLE ;
	
	for ( index = 0; index < TRNS_CONT_SHT_NUM; index++ ) {
		gTrnsControl.mSheet[index]		 = SHEETNO_NUM;
	}
	
	PrintTransLog(0x0A, 0x0F, gTrnsControl.mStep , gTrnsControl.mTransSpeed);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_GetState
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
ETraControllerState TRNS_Device_GetState()
{
	UC index;

	for ( index = 0; index < TRNS_CONT_SHT_NUM; index++ ) {
		if ( gTrnsControl.mStep > TRNS_CONT_STEP_IDLE ) {
			return TRNS_CONT_STT_RUNNING;
		}
	}

	return TRNS_CONT_STT_IDLE;
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_Reset
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_Device_Reset()
{
	// --- Output Device初期化 ---
	DD_DIO_SetOutput(DD_DO_TRANSPORT_MOT_BRAKE, OFF);
	DD_SQP_SetFrequency(DD_SQP_ID_TRANSPORT, GetFrequency(3500));//Check
	DD_SQP_Stop(DD_SQP_ID_TRANSPORT);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_Device_SetUp
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_Device_SetUp()
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_Device_StartSheet
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_Device_StartSheet()
{
	UC index,aProductivity;
	UL ulRepeatCount;
	gTrnsControl.mNextIndex = 0;
	gTrnsControl.mRunningIndex= 0;
	gTrnsControl.mStopSheetNo= SHEETNO_NUM;
	gTrnsControl.mTransSpeed = 0;
	gTrnsControl.mIDTransSpeed = TRNS_SPEED_ID_V4;
	aProductivity = SRVC_GetProductivity();
	if ( SRVC_GetNvmValueUS(NVMID_FINISHER_SPEED) == 0 ){
		if(aProductivity > TRNS_SPEEDCHANGE_CPM65){ // Speed * 10
			gTrnsControl.mIDTransSpeed = TRNS_SPEED_ID_V5;
			gTrnsControl.mTransSpeed = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_SPEED_V5)*10; // Speed *10 
		} else if  ( aProductivity > TRNS_SPEEDCHANGE_CPM55 ) {
			gTrnsControl.mIDTransSpeed = TRNS_SPEED_ID_V4;
			gTrnsControl.mTransSpeed = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_SPEED_V4)*10;
		} else {
			gTrnsControl.mIDTransSpeed = TRNS_SPEED_ID_V3;
			gTrnsControl.mTransSpeed = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_SPEED_V3)*10;
		} 	
	} else if ( SRVC_GetNvmValueUS(NVMID_FINISHER_SPEED) == 1 ) {
		gTrnsControl.mIDTransSpeed = TRNS_SPEED_ID_V3;
		gTrnsControl.mTransSpeed = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_SPEED_V3)*10;//TRNS_MOTSPEED_600; // 600 *10
	} else if ( SRVC_GetNvmValueUS(NVMID_FINISHER_SPEED) == 2 )  {
		gTrnsControl.mIDTransSpeed = TRNS_SPEED_ID_V4;
		gTrnsControl.mTransSpeed = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_SPEED_V4)*10;//TRNS_MOTSPEED_700; // 700 * 10
	} else if ( SRVC_GetNvmValueUS(NVMID_FINISHER_SPEED) == 3 )  {
		gTrnsControl.mIDTransSpeed = TRNS_SPEED_ID_V5;
		gTrnsControl.mTransSpeed = SRVC_GetNvmValueUS(NVMID_TRANSPORT_MOT_SPEED_V5)*10;//TRNS_MOTSPEED_800; // 800 * 10
	}	


	gTrnsControl.mIOTGatePullTimeA = SRVC_GetNvmValueUC(NVMID_TRANSPORT_SOLENOID_PUSH_TIME_A);
	gTrnsControl.mIOTGatePullTimeB = SRVC_GetNvmValueUC(NVMID_TRANSPORT_SOLENOID_PULL_TIME_B);
	gTrnsControl.mIOTGatePullAddCountC = SRVC_GetNvmValueUC(NVMID_TRANSPORT_SOLENOID_COUNT_C);

	gTrnsControl.mIOTSpeed = SRVC_GetExitInitialSpeed(0);

	// 用紙Size / IOT Speed 
	// ulRepeatCount = (UL) SRVC_GetProcessSize(0) * 10000/ gTrnsControl.mIOTSpeed ;
	// gTrnsControl.mIOTGatePullRepeatC = (US)ulRepeatCount/(gTrnsControl.mIOTGatePullTimeA + gTrnsControl.mIOTGatePullTimeB) + gTrnsControl.mIOTGatePullAddCountC;
	// ※算出結果は小数点以下切り上げ *1000 かわりに *10000を使う
	// gTrnsControl.mIOTGatePullRepeatC = (gTrnsControl.mIOTGatePullRepeatC + 9)/10;

	for ( index = 0; index < TRNS_CONT_SHT_NUM; index++ ) {
		if ( gTrnsControl.mStep == TRNS_CONT_STEP_INACTIVE ) {
			gTrnsControl.mStep = TRNS_CONT_STEP_IDLE;
		} else {
			// Unexpected case
		}
	}
	PrintTransLog(0xF0, gTrnsControl.mIDTransSpeed, gTrnsControl.mStep, gTrnsControl.mIOTSpeed);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_StopSheet
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_Device_StopSheet()
{
	UC index;

	// --- 実?中制御の停? ---
	 for ( index = 0; index < TRNS_CONT_SHT_NUM; index++ ) {
		if ( gTrnsControl.mSheet[index] != SHEETNO_NUM ) {		// 制御対象用紙あり
			StopControlSheet(gTrnsControl.mSheet[index]);
		} else { /* No Action */ }
	 }
	// if ( (CDvstInformer_GetDeviceCondtion(DEVID_FINISHER) == DEV_COND_PERM_NG) ||
	//		(CCvrChecker_GetCoverStatus(CVR_ID_NUM) == CVR_STTS_OPEN ) || 
	//		(CJamChecker_GetJamZoneStatus(JAM_ZONE_ID_NUM) == JAM_ZONE_STTS_SET) ) {
	//			TRNS_Device_EmgStop();				
	// }
	PrintTransLog(0x0A, 0x04, gTrnsControl.mStep , gTrnsControl.mTransSpeed);		
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_AbortSheet
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_Device_AbortSheet()
{
	UC index;

	for ( index = 0; index < TRNS_CONT_SHT_NUM; index++ ) {
		if ( gTrnsControl.mSheet[index] != SHEETNO_NUM ) {		// 制御対象用紙あり
			if ( SRVC_CheckSheetConditionStop( gTrnsControl.mSheet[index]) == true ) { //| Leave	| Jam	| Rem	| -> True
				TRNS_Device_EmgStop();
				// StopControlSheet( gTrnsControl.mSheet[index]);
			} else { /* 制御継続 */ }
		} else { /* No Action */ }
	}
	PrintTransLog(0x0A, 0x05, gTrnsControl.mStep , gTrnsControl.mTransSpeed);		
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_StartStaticJamCheck
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_Device_StartJamCheck()
{
	// StartControlJamCheck();
	TRNS_StartAct(TRNS_SJAM_CHK,  SHEETNO_NUM);

}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_StopJamCheck
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_Device_StopJamCheck()
{
	TRNS_StopControlSJamCheck();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_StartJamClearAssist
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_Device_StartJamClearAssist()
{
	// StartControlJamClearAssist();
	TRNS_StartAct(TRNS_SJAM_CHK,  SHEETNO_NUM);	
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_StopJamClearAssist
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_Device_StopJamClearAssist()
{
	StopControlJamClearAssist();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_Device_ComponentControl
// Description   : 
// Parameter     : 
// Return        : 
///////////////////////////////////////////////////////////////////
void TRNS_Device_StartCompoTransportMotorGateSolenoid(US aLinkNo)
{
	ETrnsControlStep aAction;
	UL aChainLink = (UL)(TRNS_COMPO_CHAIN) | ((UL)(aLinkNo) << 16);
	MSP_SetEventTimer(0, TRNS_InformedCompoControlStart, aChainLink);

	aAction = (ETrnsControlStep)aLinkNo;
	switch( aAction ) {
	case TRNS_COMPO_FORWARD_LOW_SPEED :
	case TRNS_COMPO_FORWARD_MID_SPEED :
	case TRNS_COMPO_FORWARD_HI_SPEED :
	case TRNS_COMPO_GATE_SOLENOID_PULL :
	case TRNS_COMPO_GATE_SOLENOID_PUSH :
		TRNS_StartAct(aAction , 0);	// Link No =
		break;

	default:
		break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_Device_StopCompoTransportMotorGateSolenoid
// Description   : 
// Parameter     : 
// Return        : 
///////////////////////////////////////////////////////////////////
void TRNS_Device_StopCompoTransportMotorGateSolenoid(US aLinkNo)
{
	ETrnsControlStep aAction;
	aAction = (ETrnsControlStep)aLinkNo;

	TRNS_SJamChk_CancelEventTimer();

	switch ( aAction ) {
	case TRNS_COMPO_FORWARD_LOW_SPEED :
	case TRNS_COMPO_FORWARD_MID_SPEED :
	case TRNS_COMPO_FORWARD_HI_SPEED :
		DD_DIO_SetOutput(DD_DO_TRANSPORT_MOT_BRAKE, OFF);
		gTrnsControl.mSJamChkTimerID = MSP_SetEventTimer(TRNS_SETTIME_50, TRNS_ReceiveEventTimeUp, 0);
		break;

	case TRNS_COMPO_GATE_SOLENOID_PULL :
	case TRNS_COMPO_GATE_SOLENOID_PUSH :
		TRNS_InformedCompoControlComplete((US)TRNS_COMPO_CHAIN, aLinkNo, ACT_STOP);
		break;

	default:
		break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_InformedCompoControlStart
// Description   : TRNS ModuleにComponent Control Action開始を通知する
// Parameter     : aChainLink(0-15bit:ChainNo、16-31bit:LinkNo)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void TRNS_InformedCompoControlStart(SS aTimerID, UL aChainLink)
{	
	US aChainNo = (US)(aChainLink & 0x0000FFFF);
	US aLinkNo = (US)((aChainLink >> 16) & 0x0000FFFF);
	TRNS_InformedCompoControlComplete(aChainNo, aLinkNo, ACT_START);
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_Device_ComponentControl
// Description   : 
// Parameter     : 
// Return        : 
///////////////////////////////////////////////////////////////////
void TRNS_Device_ComponentControl(US aChainNo, US aLinkNo, EAction aAction)
{
	if ( aAction == ACT_START ) {
		TRNS_Device_StartCompoTransportMotorGateSolenoid(aLinkNo);
	} else { // ACT_STOP
		TRNS_Device_StopCompoTransportMotorGateSolenoid(aLinkNo);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_DevIotExitLocationLead
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_Device_IotExitLocationLead(UC sheetNo)
{
	if ( SRVC_CheckSheetConditionDelivery(sheetNo) == true ) {
		// StartControlSheet(sheetNo);
		TRNS_StartAct(TRNS_EXIT, sheetNo);
	
	} else { /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_Device_EntranceLocationTail
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_Device_EntranceLocationTail(UC sheetNo)
{
	if ( SRVC_CheckSheetConditionDelivery(sheetNo) == true ) {
		// ChangeSpeedTransportSheet(sheetNo);
	} else {
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_EntranceSensorON
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_Device_EntranceSensorON()
{
	// No Action BrakeOnJamCheck();
	// TRNS_ControlThread();
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_CompileExitSensorON
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_Device_CompileExitSnrLocationLead()
{

	// No Action BrakeOnJamCheck();
	PrintTransLog(0x03, 1, gTrnsControl.mStep, (US)gTrnsControl.mSheet[gTrnsControl.mRunningIndex] );		
	if ( (gTrnsControl.mStep == TRNS_EXIT_st3) || (gTrnsControl.mStep == TRNS_EXIT_st2) || (gTrnsControl.mStep >= TRNS_EXIT_st7) ) {
	
		gTrnsControl.mStep = TRNS_EXIT_st4; // Exit Sensor Off 待ち
		TRNS_ControlThread();
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : TRNS_DevCompileExitSensorOFF
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_Device_CompileExitSnrLocationTail()
{
	gTrnsControl.mSheet[gTrnsControl.mRunningIndex] = SHEETNO_NUM ;
	gTrnsControl.mRunningIndex++;
	gTrnsControl.mRunningIndex &= TRNS_CONT_SHT_MASK;
	// No Action BrakeOnJamClearAssist();
	PrintTransLog(0x03, 0, gTrnsControl.mStep, gTrnsControl.mSheet[gTrnsControl.mRunningIndex]);

	if ( gTrnsControl.mStep != TRNS_EXIT_st4) return;
	
	gTrnsControl.mStep = TRNS_EXIT_st5; // Exit Sensor Off 状態
	TRNS_ControlThread();

}
///////////////////////////////////////////////////////////////////
// Function Name : TRNS_Device_IOTGATE_SignalON
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_Device_IOTGATE_SignalON(void)
{
	TRNS_Device_IOTGateSol_AllOff();
	TRNS_IOTGateTimer_CancelEventTimer();

	if ( gTrnsControl.mIOTGatePullAddCountC > 0 ) { // PULL Repeat Count After PUSH Start
		gTrnsControl.mIOTGatePullRepeatC = gTrnsControl.mIOTGatePullAddCountC ;
		TRNS_Device_IOTGateSol_Pull();
		gTrnsControl.mIOTGateTimerID =	MSP_SetEventTimer(gTrnsControl.mIOTGatePullTimeB, TRNS_IOTGATE_ReceiveEventTimeUp, TRNS_IOTGATE_PULL_STEP_C_ON);

	} else {
		TRNS_Device_IOTGateSol_Push();
		gTrnsControl.mIOTGateTimerID =	MSP_SetEventTimer(TRNS_GSOL_WAITTIME_100, TRNS_IOTGATE_ReceiveEventTimeUp, TRNS_IOTGATE_PUSH);

	}

	PrintTransLog(0x10, 1, gTrnsControl.mStep, (US)gTrnsControl.mIOTGatePullAddCountC );

}
///////////////////////////////////////////////////////////////////
// Function Name : TRNS_Device_IOTGATE_SignalOFF
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_Device_IOTGATE_SignalOFF(void)
{
	UL ulRepeatCount;
	UC mSheetNum;

	TRNS_Device_IOTGateSol_AllOff();
	TRNS_IOTGateTimer_CancelEventTimer();

	mSheetNum = gTrnsControl.mSheet[gTrnsControl.mRunningIndex];
	
	TRNS_Device_IOTGateSol_Pull();

	gTrnsControl.mIOTGateTimerID =  MSP_SetEventTimer(TRNS_GSOL_WAITTIME_100, TRNS_IOTGATE_ReceiveEventTimeUp, TRNS_IOTGATE_PULL_STEP100MS);

	PrintTransLog(0x10, 0, gTrnsControl.mStep, (US)gTrnsControl.mIOTGatePullAddCountC );
}
///////////////////////////////////////////////////////////////////
// Function Name : TRNS_Device_IOTGateSol_Pull
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_Device_IOTGateSol_Pull( void)
{
	DD_DIO_SetOutput(DD_DO_TRANSPORT_MOT_GATE_SOL_PULL, ON);
	DD_DIO_SetOutput(DD_DO_TRANSPORT_MOT_GATE_SOL_PUSH, OFF);
//#ifndef FXK_TEST_BINGATE
//	FwdFinLogDataMsgCdiFin(0x51);
//#endif	
}
///////////////////////////////////////////////////////////////////
// Function Name : TRNS_Device_IOTGateSol_Push
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_Device_IOTGateSol_Push(void)
{
	DD_DIO_SetOutput(DD_DO_TRANSPORT_MOT_GATE_SOL_PULL, OFF);
	DD_DIO_SetOutput(DD_DO_TRANSPORT_MOT_GATE_SOL_PUSH, ON);
//#ifndef FXK_TEST_BINGATE
//	FwdFinLogDataMsgCdiFin(0x52);
//#endif	
}
///////////////////////////////////////////////////////////////////
// Function Name : TRNS_Device_IOTGateSol_AllOff
// Description   : 
// Parameter     : ?
// Return        : ?
///////////////////////////////////////////////////////////////////
void TRNS_Device_IOTGateSol_AllOff(void)
{
	DD_DIO_SetOutput(DD_DO_TRANSPORT_MOT_GATE_SOL_PULL, OFF);
	DD_DIO_SetOutput(DD_DO_TRANSPORT_MOT_GATE_SOL_PUSH, OFF);
//#ifndef FXK_TEST_BINGATE
//	FwdFinLogDataMsgCdiFin(0x50);
//#endif	
}
///////////////////////////////////////////////////////////////////
// Function Name : PrintTransLog
// Description   :  
// Parameter     : para1 (数字列の最初に付加する値) : SNR情報(0x01: Iot ExitSNR, 0x02:CompileExitSNR)
//               : para2 (数字列の２番目に付加する値) : ON/OFF
//               : para3 (数字列の３番目に付加する値) : SheetNo
//		  : para4(数字列の 4番目に付加する値) : Trans Status
// Return        : ‐
///////////////////////////////////////////////////////////////////
void PrintTransLog(UC para1, UC para2, US para3, US para4)
{

	UC 	log[6];

	log[0] =  para1;
	log[1] =  para2;	
	log[2] =  (UC)(para3 >>8 );
	log[3] =  (UC)(para3 & 0x00FF);
	log[4] =  (UC)(para4 >>8 );
	log[5] =  (UC)(para4 & 0x00FF);

	
//#define	LOGID_TRNS=14, 'S'=Send,   LOG_CATEGORY_DEBUG=2
	DD_LOG_Put(LOGID_TRNS, 'S', log, sizeof(log), LOG_CATEGORY_DEBUG);

}

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+-------------------------------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
// 14.12.15	| Yuichi Hoshino	| Trouble Action [ Transportの残留紙検出動作停?制御不正 ]
// 15.06.02	| LeeJE	| CR012  [ TRANSPORTとIOT速度差によるJAM対策導入権 ] V02.01.00
// 15.06.02	| LeeJE	| TR0XX  [ ComExitSnrOff状態 受信処理(TRNS_exit_st5)でタイマセットが2重セット ] V02.02.00
// 15.06.12	| LeeJE	| CR013  [ Interlock Open状態でIOTExitSensorがOnされるとTransMotが駆動される件 ] V02.04.00
// 15.06.26	| LeeJE	| CR015  [ 仕様最適化の件‐②：TransMot変更 ] V02.05.00
// 15.06.26	| LeeJE	| TrNo.020  [ 厚紙Modeで減速で搬送速度に変更時間NVMか適用されない(机上発見TB) ] V02.05.00
// 15.06.26	| LeeJE	| DA029  [ Eject Home SNR Off Failによる回りぱなし ] V02.05.00
// 15.06.26	| LeeJE	| CR014  [ Transport騒音改善対策の仕様変更依頼 ] V02.05.00
// 15.11.05	| LeeJE	| CR0XX  [ 用紙Size関係なくて全用紙についてV4(800)速度に変更 ] V03.00.05
// 15.11.16	| LeeJE	| TR0XX  [ 机上発見TB] 机上発見TB　DiagHighSpeed　仕様 800mm/sec -> 600mm/sec 駆動された、MIDSpeed NVM SPEED_V3 High Speed NVM SPEED_V4
// 16.02.12    | LeeJE | TB-00XX TRNS_Manager_Stanby遷移前、Abort処理によってTRNSDeviceがIdleに遷移させる。	V3.3.0



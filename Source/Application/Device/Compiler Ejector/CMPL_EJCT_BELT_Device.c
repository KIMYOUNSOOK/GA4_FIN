// File Name:	CMPL_EJCT_BELT_Device.c
// Description:	Compiler Ejector  Device Controller
// Engineer:	KimYS
// Last Edit:	15.02.26
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"
#include "CMPL_EJCT_BELT_Device.h"
#include "CMPL_EJCT_Boundary.h"
#include "MSP_EventTimer.h"
#include "DD_DIO.h"
#include "DD_SQP.h"
#include "DD_INT.h"
#include "DD_STM.h"
#include "DD_PWM.h"
#include "DD_ResourceAssign.h"
#include "NvmService.h"

/////////////////////////////////////////////////////////////////////////////////////////
// Private Data
/////////////////////////////////////////////////////////////////////////////////////////

typedef struct
{
	ECMPL_EJCT_BELT_Action    action;    //Main  State
	ECMPL_EJCT_BELT_Action	  compoAction; //Component ActionState

	UC  	 sheetNo;
	SS 	 timerIndex;
	SS    interruptIndex;
	UC 	 direction;
	
	UC 	paddleCount;//現在Count数
	UC 	paddleStatus_before;//直前レベルチェック
	DD_STM_PROF_ID profile;
	UC   startClock;  // MotStepUp中、SNRイベント発生処理のため
	UC   diagPaddleCnt;  // SubPaddleComponent動作 ： 1回―>2回変更（CR-016)    
}EJCT_BELT_Control;


typedef struct
{
	void (*mpHandler)(void) ;		

}EJCT_BELT_CallFunc ;


static EJCT_BELT_Control     	  gCMPL_EJCT_BELT;
static EFaultStatus  			  gCMPL_EJCT_BELT_FaultStat;

//Event Timeout Parameter
typedef enum
{
    eEJCT_NORMAL_EVT,
    eEJCT_INTERRUPT_EVT,
    eEJCT_FAIL_EVT=0x80,
    eEJCT_SUBPADDLE_HOMESNR_ONFAIL_EVT,
    eEJCT_SUBPADDLE_HOMESNR_OFFFAIL_EVT,
    eEJCT_EJECT_HOMESNR_ONFAIL_EVT,
    eEJCT_EJECT_HOMESNR_OFFFAIL_EVT,

}EEJCT_TimeoutType;

#define CMPL_EJCT_BELT_TIME_20   		20
#define CMPL_EJCT_BELT_TIME_30   		30
#define CMPL_EJCT_BELT_FAILTIME_300   300
#define CMPL_EJCT_BELT_FAILTIME_400   400


#define PROF_PULSE_STEP_30			29

//MotStepUp中、SNRイベント発生処理のため
#define  EJCT_START_CLK_UNKNOWN   		0
#define  EJCT_START_CLK_WAIT    			0x80
#define  EJCT_START_CLK_RECV     			0xFF

//PWM Current (Eject Mot)
#define  EJCT_MOT_CUR_LOW     15
#define  EJCT_MOT_CUR_HIGH    70

const US cEJCT_Profile_1[] = {

	6000,	6010,	6050,	6120,	6210,	6320,	6450,	6610,	6780,	6980,	// 0-9(SlowUp)
	7180,	7400,	7630,	7870,	8100,	8350,	8580,	8820,	9050,	9270,	// 10-19(SlowUp)
	9470,	9670,	9840,	10000,	10130,	10240,	10330,	10400,	10440,	10450,	// 20-29(SlowUp)
	10000,    9500,     8800,        8200,      7600,      6900,      6200,      5500,           0,                  // 30-37(SlowDown) , 38(Stop)
};

const US cEJCT_Profile_2[] = {

	6000,	6030,	6110,	6240,	6420,	6640,	6920,	7230,	7590,	7970,	// 0-09(SlowUp)
	8390,	8830,	9300,	9770,	10260,	10740,	11230,	11700,	12170,	12610,	// 10-19(SlowUp)
	13030,	13410,	13770,	14080,	14360,	14580,	14760,	14890,	14970,	15000,	// 21-29(SlowUp)
	13800,    12700,     11600,    10500,      9500,      8200,      7000,      5500,           0,                  // 30-37(SlowDown) , 38(Stop)
};

const US cEJCT_Profile_3[] = {

	6000,	6040,	6170,	6390,	6690,	7070,	7520,	8040,	8630,	9270,	// 0-09(SlowUp)
	9960,	10690,    11460,	12240,	13050,	13850,	14660,	15440,	16210,	16940,	// 10-19(SlowUp)
	17630,	18270,	18860,	19380,	19830,	20210,	20510,	20730,	20860,	20900,	// 20-29(SlowUp)
	18000,    15000,     13000,    11500,     10000,      8500,      7000,      5500,           0,                  //30-37(SlowDown) , 38(Stop)
};

 //CR009
const US cEJCT_Profile_Eject[] = {
	//V3.3.100
	//2700,	2700,	2700,	2700,	2990,	3150,	3340,	3560,	3810,	4080,	// 0-09(SlowUp)
	2990,	2990,	2990,	2990,	2990,	3150,	3340,	3560,	3810,	4080,	// 0-09(SlowUp)
	4370,	4680,	5010,	5340,	5680,	6020,	6360,	6690,	7020,	7330,	// 10-19(SlowUp)
	7620,	7890,	8140,	8360,	8550,	8710,	8840,	8930,	8980,	9000,	// 20-29(SlowUp)
	8980,      8930,      8840,      8710,      8550,      8360,      8140,      7890,      7620,      7330,    // 30-39(SlowDown)
	7020,      6690,      6360,      6020,      5680,      5340,      5010,      4680,      4370,      4080,    // 40-49(SlowDown)
	3810,      3560,      3340,      3150,      2990,      2700,      2700,      2700,      2700,           0,     // 50-58(SlowDown), 59(Stop)
}; 



/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
static void CMPL_EJCT_BELT_ControlThread();
static  void CMPL_EJCT_BELT_ReceiveEventTimerTimeUp(SS aTimerID, UL aType);
static void CMPL_EJCT_BELT_SetInterrupt(UC);
static void CMPL_EJCT_BELT_CancelInterrupt();
static void CMPL_EJCT_BELT_InterruptSubPaddleHomeSensorON();
static void CMPL_EJCT_BELT_InterruptSubPaddleHomeSensorOFF();
static UC   CMPL_EJCT_BELT_GetSubPaddleCount();
static UC   CMPL_EJCT_BELT_GetSubPaddleStartTime();
static UC   CMPL_EJCT_BELT_GetSubPaddleProfileType();

static  void CMPL_EJCT_BELT_StartMotorClock();
static  void CMPL_EJCT_BELT_StopMotorClock();
static  void CMPL_EJCT_BELT_SetEventTimer(US aTime, UL aEventType);
static  void CMPL_EJCT_BELT_CancelEventTimer();
static void  EJCT_BELT_ReceiveSteppingMotorEvent(SS aResult);
static void EJCT_BELT_ReceiveStartClock(SS aResult);
static void EJCT_BELT_Receive1PulseCallback(SS aResult);

static void  CMPL_EJCT_BELT_SetProfile(void);

//CMPL_EJCT_COMPO_FWD = 0x0100,  
static void  	CompoForward_st1();		// モータ駆動設定
static void  	CompoForward_st2();		// モータ駆動開始
static void 	CompoForward_st3_ONWait(); //EjectHomeSNR　ON待ち中 
static void	CompoForward_st3_OFFWait(); // EjectHomeSNR　OFF待ち中
static void 	CompoForward_st4();	// OFF受信(T2開始)
static void 	CompoForward_st5();	//　T2後、SpeedDown設定
static void 	CompoForward_st6_ONWait();// EjectHomeSNR　ON待ち中
static void 	CompoForward_st7();	// ON受信(T3開始)
static void 	CompoForward_st8();	// T3後、StepDown設定
static void 	CompoForward_st9();	// 後励磁
static void 	CompoForward_st10();		//完了

//Component 動作(Eject Mot - CCW ) CMPL_EJCT_COMPO_REV = 0x0200,  
static void  	CompoReverse_st1();		// モータ駆動設定
static void  	CompoReverse_st2();		// モータ駆動開始
static void  	CompoReverse_st3_ONWait();	//　SubPaddleHomeSNR　ON待ち
static void  	CompoReverse_st4();	//　T1 Timer
static void  	CompoReverse_st5(); //StepDown
static void  	CompoReverse_st6(); //後励磁
static void  	CompoReverse_st7(); //完了

static  void    SubPaddleHome_st1(void);
static  void    SubPaddleHome_st2(void);
static  void    SubPaddleHome_st3_ONWait(void);
static  void    SubPaddleHome_st4(void);
static  void    SubPaddleHome_st5(void);
static  void    SubPaddleHome_st6(void);
static  void    SubPaddleHome_st7(void);
static  void    SubPaddleHome_st11_1(void); //DummyPulse設定
static  void    SubPaddleHome_st11_2(void); //１Clock後Timer設定

// Eject Home (= Componet動作)
static void  	EjectHome_st1();		// モータ駆動設定
static void  	EjectHome_st2();		// モータ駆動開始
//static void  	EjectHome_st3_ONWait(); //EjectHomeSNR　ON待ち中 
static void  	EjectHome_st3_OFFWait(); // EjectHomeSNR　OFF待ち中
static void  	EjectHome_st4(); // OFF受信(T2開始)
static void  	EjectHome_st5(); //　T2後、SpeedDown設定
static void  	EjectHome_st6_ONWait();// EjectHomeSNR　ON待ち中
static void  	EjectHome_st7(); // ON受信(T3開始)
static void  	EjectHome_st8(); // T3後、StepDown設定
static void  	EjectHome_st9(); // EjectHome動作完了
static  void    	EjectHome_st11_1(void); //DummyPulse設定
static  void    	EjectHome_st11_2(void); //１Clock後Timer設定

//Compile動作
static  void	Compile_st1(void);	//Mot 駆動設定
static  void	Compile_st2(void);	//Compile動作開始
static  void	Compile_st3(void);	//P回分処理
static  void	Compile_st4(void);	//StepDown開始
static  void	Compile_st5(void);	//後励磁
static  void	Compile_st6(void);	//MotOFF

//Eject動作
static void  Eject_st1();		// モータ駆動設定
static void  Eject_st2();		// モータ駆動開始
static void  Eject_st3_OFFWait(); // EjectHomeSNR　OFF待ち中
static void  Eject_st4();	// OFF受信(T2開始)
static void  Eject_st5();	//　T2後、SpeedDown設定
static void  Eject_st6_ONWait();// EjectHomeSNR　ON待ち中
static void  Eject_st7();	// ON受信(T3開始)
static void  Eject_st8();	// T3後、StepDown設定
static void  Eject_st9();	// 後励磁
static void  Eject_st10();		//完了
static  void    Eject_st11_1(void); //DummyPulse設定
static  void    Eject_st11_2(void); //１Clock後Timer設定

static void Eject_Compo_Mot_st1();
static void Eject_Compo_Mot_st2();
static void Eject_Compo_Mot_st3();
static void Eject_Compo_Mot_st4();


static const EJCT_BELT_CallFunc ControlThread_COMPO_FWD[ ] = {
		{ NULL },
		{  CompoForward_st1 },		// モータ駆動設定
		{  CompoForward_st2 },		// モータ駆動開始
		{  CompoForward_st3_ONWait }, //EjectHomeSNR　ON待ち中 
		{  CompoForward_st3_OFFWait}, // EjectHomeSNR　OFF待ち中
		{  CompoForward_st4 },		 // OFF受信(T2開始)
		{  CompoForward_st5 }, 		//　T2後、SpeedDown設定
		{  CompoForward_st6_ONWait},// EjectHomeSNR　ON待ち中
		{  CompoForward_st7 }, 		// ON受信(T3開始)
		{  CompoForward_st8 }, 		// T3後、StepDown設定
		{  CompoForward_st9 }, 		// 後励磁
		{  CompoForward_st10 },		//完了

};

static const EJCT_BELT_CallFunc ControlThread_COMPO_REV[ ]={
		{ NULL },
		{ CompoReverse_st1},		// モータ駆動設定
		{ CompoReverse_st2},		// モータ駆動開始
		{ CompoReverse_st3_ONWait},	//　SubPaddleHomeSNR　ON待ち
		{ CompoReverse_st4},	//　T1 Timer
		{ CompoReverse_st5}, //StepDown
		{ CompoReverse_st6}, //後励磁
		{ CompoReverse_st7}, //完了
};

static const EJCT_BELT_CallFunc ControlThread_PADHOME[ ]={
		{ NULL},
		{ SubPaddleHome_st1},		 //EjectMot,  Paddle 設定
		{ SubPaddleHome_st2},			//モータ駆動開始
		{ SubPaddleHome_st3_ONWait},	//SNR ON待ち
		{ SubPaddleHome_st4},		//SNR ON割込み受信
		{ SubPaddleHome_st5},		//StepDown設定
		{ SubPaddleHome_st6},		//後励磁
		{ SubPaddleHome_st7},		//動作完了
		{ SubPaddleHome_st11_1},		//DummyPulse設定
		{ SubPaddleHome_st11_2},		//１Clock後Timer設定
};

static const EJCT_BELT_CallFunc ControlThread_EJCTHOME[ ]={
		{ NULL},
		{ EjectHome_st1},		// モータ駆動設定
		{ EjectHome_st2},		// モータ駆動開始
		{ EjectHome_st3_OFFWait}, // EjectHomeSNR　OFF待ち中
		{ EjectHome_st4}, // OFF受信(T2開始)
		{ EjectHome_st5}, //　T2後、SpeedDown設定
		{ EjectHome_st6_ONWait},// EjectHomeSNR　ON待ち中
		{ EjectHome_st7}, // ON受信(T3開始)
		{ EjectHome_st8}, // T3後、StepDown設定
		{ EjectHome_st9}, // EjectHome動作完了
		{ EjectHome_st11_1},		//DummyPulse設定
		{ EjectHome_st11_2},		//１Clock後Timer設定
};

static const EJCT_BELT_CallFunc ControlThread_EJCTCOMPILE[ ]={
		{ NULL},
		{ Compile_st1},	//Mot 駆動設定
		{ Compile_st2},	//Compile動作開始
		{ Compile_st3},	//P回分処理
		{ Compile_st4},	//StepDown開始
		{ Compile_st5},	//後励磁
		{ Compile_st6},	//MotOFF	
};

static const EJCT_BELT_CallFunc ControlThread_EJCT[ ]={
		{ NULL},
		{ Eject_st1},		// モータ駆動設定
		{ Eject_st2},		// モータ駆動開始
		{ Eject_st3_OFFWait},// EjectHomeSNR　OFF待ち中
		{ Eject_st4},	// OFF受信(T2開始)
		{ Eject_st5},	//　T2後、SpeedDown設定
		{ Eject_st6_ONWait},// EjectHomeSNR　ON待ち中
		{ Eject_st7},	// ON受信(T3開始)
		{ Eject_st8},	// T3後、StepDown設定
		{ Eject_st9},	// 後励磁
		{ Eject_st10},		//完了
		{ Eject_st11_1},	//DummyPulse設定
		{ Eject_st11_2},	//１Clock後Timer設定
};


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_BELT_StartAct
// Description   : イジェクトMot動作設定_動作開始
// Parameter     : aAction (制御する動作)
//				 : aSheetNo (制御するSheetNo)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_BELT_StartAct(ECMPL_EJCT_BELT_Action aAction, UC aSheetNo)
{

	switch( aAction ) {

	// Component 動作 //////////////////////////////////////////////////////////////////////////////
	case  CMPL_EJCT_COMPO_FWD_EJECT : //Component(EjectMot - CW)
		gCMPL_EJCT_BELT.direction = BELT_FORWARD;
		gCMPL_EJCT_BELT.profile = DD_STM_PROF_ID_EJCT_BELT_PRF_EJECT;
		gCMPL_EJCT_BELT.compoAction = aAction;
		gCMPL_EJCT_BELT.action = CMPL_EJCT_COMPO_FWD_st1; 
		break;

	case  CMPL_EJCT_COMPO_REV_PADDLE : //Component ( EjectMot - CCW)
		gCMPL_EJCT_BELT.direction = BELT_REVERSE;
		gCMPL_EJCT_BELT.profile = DD_STM_PROF_ID_EJCT_BELT_PRF_2;
		gCMPL_EJCT_BELT.compoAction = aAction;
		gCMPL_EJCT_BELT.action = CMPL_EJCT_COMPO_REV_st1; 
		break;
	// Component 動作 //////////////////////////////////////////////////////////////////////////////

	case  CMPL_EJCT_PADHOME : //Paddle Compile Home
		gCMPL_EJCT_BELT.profile = DD_STM_PROF_ID_EJCT_BELT_PRF_2;
		gCMPL_EJCT_BELT.direction = BELT_REVERSE;
		gCMPL_EJCT_BELT.action = CMPL_EJCT_PADHOME_st1;
		
		break;
	case  CMPL_EJCT_EJTHOME: //Eject Home
		gCMPL_EJCT_BELT.direction = BELT_FORWARD;
		gCMPL_EJCT_BELT.profile = DD_STM_PROF_ID_EJCT_BELT_PRF_EJECT;
		gCMPL_EJCT_BELT.action = CMPL_EJCT_EJTHOME_st1;		
		break;

	case  CMPL_EJCT_COMPILE ://Compile
		gCMPL_EJCT_BELT.sheetNo = aSheetNo;
		gCMPL_EJCT_BELT.direction = BELT_REVERSE;
	//	gCMPL_EJCT_BELT.profile = DD_STM_PROF_ID_EJCT_BELT_PRF_2 ;	
		gCMPL_EJCT_BELT.profile = CMPL_EJCT_BELT_GetSubPaddleProfileType();
		gCMPL_EJCT_BELT.action = CMPL_EJCT_COMPILE_st1;
		break;
	case  CMPL_EJCT_EJECT : //Eject
		gCMPL_EJCT_BELT.direction = BELT_FORWARD;
		gCMPL_EJCT_BELT.profile = DD_STM_PROF_ID_EJCT_BELT_PRF_EJECT;
		gCMPL_EJCT_BELT.action = CMPL_EJCT_EJECT_st1;
		break;

	default:	  // illegal case 
			return;
	}

	gCMPL_EJCT_BELT.sheetNo = aSheetNo;
	gCMPL_EJCT_BELT.timerIndex = MSP_MAX_TMR_NUM;
	gCMPL_EJCT_BELT.interruptIndex = DD_INT_NUM;
	gCMPL_EJCT_BELT.paddleCount = 0;//現在Count数
 	gCMPL_EJCT_BELT.paddleStatus_before = 0xFF;//直前レベルチェック
	gCMPL_EJCT_BELT.startClock = EJCT_START_CLK_UNKNOWN;
	CMPL_EJCT_BELT_ControlThread();
}



///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT BELT_ControlThread
// Description   : Eject Mot制御
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_BELT_ControlThread()
{

	ECMPL_EJCT_BELT_Action   aAction = gCMPL_EJCT_BELT.action & 0xFF00;
	US  				 aStatus = gCMPL_EJCT_BELT.action & 0x00FF;

	if ( aStatus == 0 )  {
		CMPL_EJCT_BELT_DeviceStop();
		return;
	}
	
	switch ( aAction)	{
		
	case  CMPL_EJCT_COMPO_FWD : 
		ControlThread_COMPO_FWD[aStatus].mpHandler(); break;
		
	case  CMPL_EJCT_COMPO_REV : 
		ControlThread_COMPO_REV[aStatus].mpHandler(); break;

	case  CMPL_EJCT_PADHOME :
		ControlThread_PADHOME[aStatus].mpHandler(); break;	

	case  CMPL_EJCT_EJTHOME :
		ControlThread_EJCTHOME[aStatus].mpHandler(); break;	
	
	case  CMPL_EJCT_COMPILE :
		ControlThread_EJCTCOMPILE[aStatus].mpHandler(); break;	

	case  CMPL_EJCT_EJECT :
		ControlThread_EJCT[aStatus].mpHandler(); break;	
	
	default:
		break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_BELT_ReceiveEventTimerTimeUp
// Description   : イベントタイマのタイムアップ
// Parameter     : timerID (Timer ID)
//               :     aType
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_BELT_ReceiveEventTimerTimeUp(SS aTimerID, UL aType)
{
	if ( aTimerID != gCMPL_EJCT_BELT.timerIndex ) {
		return;
	}
	gCMPL_EJCT_BELT.timerIndex = MSP_MAX_TMR_NUM;


	if ( aType < eEJCT_FAIL_EVT ) {

		switch ( gCMPL_EJCT_BELT.action ) {

		case CMPL_EJCT_COMPO_FWD_st1 :
		case CMPL_EJCT_COMPO_FWD_st9 :
		case CMPL_EJCT_COMPO_REV_st1 :
		case CMPL_EJCT_COMPO_REV_st4 :
		case CMPL_EJCT_COMPO_REV_st6 :
			gCMPL_EJCT_BELT.action++;
			CMPL_EJCT_BELT_ControlThread();
			break;

		// SubPaddle Home動作
		case CMPL_EJCT_PADHOME_st1 :   			// PaddleHome Timeout
			//V3.5.91
			gCMPL_EJCT_BELT.action=CMPL_EJCT_PADHOME_st11_1;
			CMPL_EJCT_BELT_ControlThread();
			break;
		case CMPL_EJCT_PADHOME_st11_2:   			// PaddleHome Timeout
			//V3.5.91
			gCMPL_EJCT_BELT.action=CMPL_EJCT_PADHOME_st2;
			CMPL_EJCT_BELT_ControlThread();
			break;	
		case CMPL_EJCT_PADHOME_st4 :  			// T1 Timeout
		case CMPL_EJCT_PADHOME_st6 :  			// 後励磁Timeout
			gCMPL_EJCT_BELT.action++;
			CMPL_EJCT_BELT_ControlThread();
			break;
				
		// Eject Home 動作
		case CMPL_EJCT_EJTHOME_st1 : 			// 前励磁Timeout
		// V3.5.91 
		//	gCMPL_EJCT_BELT.action++;
			gCMPL_EJCT_BELT.action = CMPL_EJCT_EJTHOME_st11_1;
			CMPL_EJCT_BELT_ControlThread();
			break;	
		case CMPL_EJCT_EJTHOME_st11_2 :   //Added V3.5.91
			gCMPL_EJCT_BELT.action = CMPL_EJCT_EJTHOME_st2;
			CMPL_EJCT_BELT_ControlThread();
			break;				

		// Compile動作
		case CMPL_EJCT_COMPILE_st1 :  
		case CMPL_EJCT_COMPILE_st5 :  			//  後励磁Timeout
			gCMPL_EJCT_BELT.action++;
			CMPL_EJCT_BELT_ControlThread();
			break;

		// Eject 動作
		case CMPL_EJCT_EJECT_st1 :  
		// V3.5.91
			gCMPL_EJCT_BELT.action = CMPL_EJCT_EJECT_st11_1;
			CMPL_EJCT_BELT_ControlThread();
			break;
		case CMPL_EJCT_EJECT_st11_2 :   //Added V3.5.91
			gCMPL_EJCT_BELT.action = CMPL_EJCT_EJECT_st2;
			CMPL_EJCT_BELT_ControlThread();
			break;	
		case CMPL_EJCT_EJECT_st9 :			//	後励磁Timeout
			gCMPL_EJCT_BELT.action++;
			CMPL_EJCT_BELT_ControlThread();
			break;
			
		default:
			break;
		}

	} else {  // Fail Timeout

		gCMPL_EJCT_BELT_FaultStat = FAULT_OCCUR;

		switch(aType) {
		case eEJCT_SUBPADDLE_HOMESNR_ONFAIL_EVT :
			CMPL_EJCT_BELT_CancelInterrupt(RISE);
			CMPL_EJCT_InformedPaddleHomeSensorOnFail();
			break;

		case eEJCT_SUBPADDLE_HOMESNR_OFFFAIL_EVT :
			CMPL_EJCT_BELT_CancelInterrupt(FALL);
			CMPL_EJCT_InformedPaddleHomeSensorOffFail();
			break;

		case eEJCT_EJECT_HOMESNR_ONFAIL_EVT :			
			if ( (gCMPL_EJCT_BELT.action & 0xFF00 ) ==  CMPL_EJCT_EJECT)  { //Eject Job
				// Set Eject Jam 
				CMPL_EJCT_InformedSetEjectJam(gCMPL_EJCT_BELT.sheetNo);
			}
			CMPL_EJCT_InformedEjectHomeSensorOnFail();
			break;

		case eEJCT_EJECT_HOMESNR_OFFFAIL_EVT :
			CMPL_EJCT_InformedEjectHomeSensorOffFail();
			break;

		default :
			break;
		}

	}

}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_BELT_EjecterHomeSensorON
// Description   : Ejecter Home Sensor OFF→ONを検知
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_BELT_EjecterHomeSensorON()
{
	if ( gCMPL_EJCT_BELT.action == CMPL_EJCT_st0 )
		return;
		
	switch ( gCMPL_EJCT_BELT.action ) {

	// Component
	case CMPL_EJCT_COMPO_FWD_st3_ONWait :
	case CMPL_EJCT_COMPO_FWD_st6_ONWait :
		gCMPL_EJCT_BELT.action++;
		CMPL_EJCT_BELT_ControlThread();
		break;	
		
	case CMPL_EJCT_EJTHOME_st3_OFFWait : 
		//OFF→ON変化でFailTimer設定
		CMPL_EJCT_BELT_SetEventTimer(CMPL_EJCT_BELT_FAILTIME_300, eEJCT_EJECT_HOMESNR_OFFFAIL_EVT);
		break;	
	case CMPL_EJCT_EJECT_st3_OFFWait :	
		//OFF→ON変化でFailTimer設定
		CMPL_EJCT_BELT_SetEventTimer(CMPL_EJCT_BELT_FAILTIME_400, eEJCT_EJECT_HOMESNR_OFFFAIL_EVT);
		break;
		
	case CMPL_EJCT_EJTHOME_st6_ONWait :	
		CMPL_EJCT_BELT_CancelEventTimer();			
		gCMPL_EJCT_BELT.action++;
		CMPL_EJCT_BELT_ControlThread();
		break;

	case  CMPL_EJCT_EJECT_st6_ONWait :
		CMPL_EJCT_BELT_CancelEventTimer();
		gCMPL_EJCT_BELT.action++;
		CMPL_EJCT_BELT_ControlThread();
		break;

		default :  
			break;
	}	
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_BELT_EjecterHomeSensorOFF
// Description   : Ejecter Home Sensor ON→OFFを検知
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_BELT_EjecterHomeSensorOFF()
{
	if ( gCMPL_EJCT_BELT.action == CMPL_EJCT_st0 )
		return;

	switch ( gCMPL_EJCT_BELT.action ) {
		
	// Component
	case CMPL_EJCT_COMPO_FWD_st3_ONWait :
		gCMPL_EJCT_BELT.action = CMPL_EJCT_COMPO_FWD_st4;
		CMPL_EJCT_BELT_ControlThread();
		break;
		
	case CMPL_EJCT_COMPO_FWD_st3_OFFWait :
		gCMPL_EJCT_BELT.action++;
		CMPL_EJCT_BELT_ControlThread();
		break;	
			
	// Eject Home
	case  CMPL_EJCT_EJTHOME_st3_OFFWait :
		CMPL_EJCT_BELT_CancelEventTimer();
		gCMPL_EJCT_BELT.action ++;
		CMPL_EJCT_BELT_ControlThread();
		break;

	// Eject
	case CMPL_EJCT_EJECT_st3_OFFWait :
		CMPL_EJCT_BELT_CancelEventTimer();
		gCMPL_EJCT_BELT.action++;
		CMPL_EJCT_BELT_ControlThread();
		break;

	default :
		break;
	}

}

 void CMPL_EJCT_SubPaddleHomeSensorON() { }
 void CMPL_EJCT_SubPaddleHomeSensorOFF() { }


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_BELT_GetSubPaddleCount
// Description   : SubPaddle Count
// Parameter	  :
// Return		  : UC paddleCnt
///////////////////////////////////////////////////////////////////
UC   CMPL_EJCT_BELT_GetSubPaddleCount()
{

	//CR008
	UC	paddleCnt = 1;
	US 	length = SRVC_GetProcessSize(gCMPL_EJCT_BELT.sheetNo);
	US   width = SRVC_GetCrossProcessSize(gCMPL_EJCT_BELT.sheetNo);  //(CR-XXX) 150716 B5 シートずれ対策
	US 	aMediaType = SRVC_GetMediaType(gCMPL_EJCT_BELT.sheetNo);
	US 	aMediaWeight =  SRVC_GetMediaWeight(gCMPL_EJCT_BELT.sheetNo);
	UC   aCompileCount = SRVC_GetCompileCount(gCMPL_EJCT_BELT.sheetNo);
	UC   aN_Count;
	UC   aP2;   //CR-014
	
	if(length <= 2970) {
	
		// MediaType＝コート紙　
		if ( aMediaType == eCdiFinMediaType_Coated ) {

			paddleCnt = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_COUNT5);
			aN_Count = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_CHANGE_COUNT_N_3);  
			
		} else if ((aMediaType ==eCdiFinMediaType_Plain) && (aMediaWeight >= 106))	{

		//　　MediaType=普通紙＆厚紙(106～221)
			paddleCnt = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_COUNT3);
			aN_Count = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_CHANGE_COUNT_N);  
		
		} else { //それ以外
		
			paddleCnt = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_COUNT1);
			 //(CR-XXX) 150716 B5 シートずれ対策
			 if ( width < 2000 ) {   // 200mm 未満
				aN_Count = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_CHANGE_COUNT_N_2_B5);  
			 } else {  // 200mm 以上
				aN_Count = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_CHANGE_COUNT_N_2); 
			 }
		}
		
	} else { //  >2970	
	
		// MediaType＝コート紙　
		if ( aMediaType == eCdiFinMediaType_Coated ) {

			paddleCnt = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_COUNT6);
			aN_Count = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_CHANGE_COUNT_N_3);  
			
		} else if ((aMediaType ==eCdiFinMediaType_Plain) && (aMediaWeight >= 106))	{

		//　　MediaType=普通紙＆厚紙(106～221)
			paddleCnt = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_COUNT4);
			aN_Count = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_CHANGE_COUNT_N); 
		
		} else { //それ以外
		
			paddleCnt = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_COUNT2);
			 //(CR-XXX) 150716 B5 シートずれ対策
			 if ( width < 2000 ) {   // 200mm 未満
				aN_Count = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_CHANGE_COUNT_N_2_B5);  
			 } else {  // 200mm 以上
				aN_Count = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_CHANGE_COUNT_N_2); 
			 }
		}
	}

	//セット枚数がN枚以上
	if ( aCompileCount >= aN_Count) {

		 if ((aMediaType ==eCdiFinMediaType_Plain) && (aMediaWeight >= 106))	{
			//(CR-014) //　　MediaType=普通紙＆厚紙(106～221) --> P-P2回
			aP2 = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_CHANGE_COUNT_P2);

			if ( paddleCnt <= aP2 ) {
				paddleCnt = 1;
			} else {
				paddleCnt = paddleCnt - aP2;
			}
		
		 } else { //それ以外 --> P-1回
			 if ( paddleCnt < 2 ) {
				paddleCnt = 1;
			 } else {
				paddleCnt = paddleCnt - 1;
			 }
		 }
	}	
	return	paddleCnt;
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_BELT_GetSubPaddleStartTime
// Description   : SubPaddle Start Time
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
UC   CMPL_EJCT_BELT_GetSubPaddleStartTime()
{
	//CR008
	US aMediaType = SRVC_GetMediaType(gCMPL_EJCT_BELT.sheetNo);
	US aMediaWeight =  SRVC_GetMediaWeight(gCMPL_EJCT_BELT.sheetNo);
	UC  startTime;
	
// MediaType＝コート紙　
	if ( aMediaType == eCdiFinMediaType_Coated ) {

		startTime = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_START_TIME_8_3);	
		
	} else if ((aMediaType ==eCdiFinMediaType_Plain) && (aMediaWeight >= 106))	{

	//　　MediaType=普通紙＆厚紙(106～221)
		startTime =SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_START_TIME_8_1);

	} else { //それ以外
	
		startTime = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_START_TIME_8_2);
	}
	return startTime;
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_BELT_GetSubPaddleProfileType
// Description   : SubPaddle Profile Type
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
UC   CMPL_EJCT_BELT_GetSubPaddleProfileType()
{
	 //CR008
	US 	aMediaType = SRVC_GetMediaType(gCMPL_EJCT_BELT.sheetNo);
	US 	aMediaWeight =  SRVC_GetMediaWeight(gCMPL_EJCT_BELT.sheetNo);
	UC	profileType;

	// MediaType＝コート紙　
	if ( aMediaType == eCdiFinMediaType_Coated ) {
		
		profileType = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_PROFILE_TYPE_3);
		
	} else if ((aMediaType ==eCdiFinMediaType_Plain) && (aMediaWeight >= 106))	{

	//　　MediaType=普通紙＆厚紙(106～221)
		profileType = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_PROFILE_TYPE_1);

	} else { //それ以外
	
		profileType = SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_PROFILE_TYPE_2);
	}

	if ( profileType == 1 ) {
		profileType =	DD_STM_PROF_ID_EJCT_BELT_PRF_1;
	} else if ( profileType == 2 ) {
		profileType = DD_STM_PROF_ID_EJCT_BELT_PRF_2;
	} else	{ // 3	  
		profileType = DD_STM_PROF_ID_EJCT_BELT_PRF_3;
	}

	return (UC)profileType;

}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_BELT_InterruptSubPaddleHomeSensorON
// Description   : SubPaddle Home Sensor ON 割り込み
// Parameter	  :
// 			  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_BELT_InterruptSubPaddleHomeSensorON()
{
	if ( gCMPL_EJCT_BELT.action == CMPL_EJCT_st0 )
		return;

	CMPL_EJCT_BELT_CancelInterrupt(RISE);

	 gCMPL_EJCT_BELT.interruptIndex = DD_INT_NUM;

	 switch( gCMPL_EJCT_BELT.action ) {
	 	
	 	case CMPL_EJCT_COMPO_REV_st2 :
			// 割込み設定( SubPaddleHome SNR Off)
			CMPL_EJCT_BELT_SetInterrupt(FALL);
		break;
		
		case  CMPL_EJCT_COMPO_REV_st3_ONWait :	
		// 1回―>2回変更 （CR-016)
			if (++gCMPL_EJCT_BELT.diagPaddleCnt == 2) { 
				gCMPL_EJCT_BELT.diagPaddleCnt = 0;
				gCMPL_EJCT_BELT.action = CMPL_EJCT_COMPO_REV_st4;
				CMPL_EJCT_BELT_ControlThread();
			} else {
				gCMPL_EJCT_BELT.action = CMPL_EJCT_COMPO_REV_st2;
				// 割込み設定( SubPaddleHome SNR Off)
				CMPL_EJCT_BELT_SetInterrupt(FALL);
			}	
		break;

	 	case  CMPL_EJCT_PADHOME_st2 :
			CMPL_EJCT_BELT_CancelEventTimer();
			// 割込み設定( SubPaddleHome SNR Off)
			CMPL_EJCT_BELT_SetInterrupt(FALL);
		break;
		
		case  CMPL_EJCT_PADHOME_st3_ONWait :

			CMPL_EJCT_BELT_CancelEventTimer();
			gCMPL_EJCT_BELT.action = CMPL_EJCT_PADHOME_st4;
			CMPL_EJCT_BELT_ControlThread();
		break;

		case  CMPL_EJCT_COMPILE_st2 :  //OFF(Fail)
			//CMPL_EJCT_BELT_CancelEventTimer();
			gCMPL_EJCT_BELT.action = CMPL_EJCT_COMPILE_st3;
			gCMPL_EJCT_BELT.paddleStatus_before=ON;
			gCMPL_EJCT_BELT.paddleCount=0;
			
			CMPL_EJCT_BELT_SetInterrupt(FALL);
			break;

		case  CMPL_EJCT_COMPILE_st3 :

			//CMPL_EJCT_BELT_CancelEventTimer();
			if(gCMPL_EJCT_BELT.paddleStatus_before == OFF)
			{
				
				if(++gCMPL_EJCT_BELT.paddleCount == CMPL_EJCT_BELT_GetSubPaddleCount())
				{
					gCMPL_EJCT_BELT.paddleCount=0;
					// (V3.6.90)  回数動作終了後、FailTimerClearするように変更
					CMPL_EJCT_BELT_CancelEventTimer();
					gCMPL_EJCT_BELT.action = CMPL_EJCT_COMPILE_st4;
					CMPL_EJCT_BELT_ControlThread();
				}
				else {
					// 割込み設定( SubPaddleHome SNR Off)
					CMPL_EJCT_BELT_SetInterrupt(FALL);
				}
			}
			gCMPL_EJCT_BELT.paddleStatus_before=ON;
		break;

		default : break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_BELT_InterruptSubPaddleHomeSensorOFF
// Description   : SubPaddle Home Sensor OFF割り込み
// Parameter	  :
// 			  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_BELT_InterruptSubPaddleHomeSensorOFF()
{
	if ( gCMPL_EJCT_BELT.action == CMPL_EJCT_st0 )
		return;

	CMPL_EJCT_BELT_CancelInterrupt(FALL);

	 gCMPL_EJCT_BELT.interruptIndex = DD_INT_NUM;

	 switch( gCMPL_EJCT_BELT.action ) {
	 	
	 	case  CMPL_EJCT_COMPO_REV_st2 :
			  gCMPL_EJCT_BELT.action = CMPL_EJCT_COMPO_REV_st3_ONWait;
			 CMPL_EJCT_BELT_ControlThread();
		break;

	 	case  CMPL_EJCT_PADHOME_st2 :

			 CMPL_EJCT_BELT_CancelEventTimer();
			 gCMPL_EJCT_BELT.action = CMPL_EJCT_PADHOME_st3_ONWait;
			 CMPL_EJCT_BELT_ControlThread();
		break;

	 	case  CMPL_EJCT_COMPILE_st3 :
			
			// 割込み設定( SubPaddleHome SNR On)
			CMPL_EJCT_BELT_SetInterrupt(RISE);
			
			//V3.6.90 回数動作終了後、FailTimerClearするように変更
			// CMPL_EJCT_BELT_CancelEventTimer(); 
	
			gCMPL_EJCT_BELT.paddleStatus_before = OFF;
		  break;

		 default : break;

	 }
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_BELT_SetInterrupt
// Description   : 割り込みセット
// Parameter	  : aEdge (RISE/FALL)
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_BELT_SetInterrupt(UC   aEdge)
{
 	if(aEdge == RISE) {
	 gCMPL_EJCT_BELT.interruptIndex = DD_INT_EntryCallback(DD_INT_SUB_PADDLE_HOME_SENSOR,
	 														BELT_RISE,
	 														CMPL_EJCT_BELT_InterruptSubPaddleHomeSensorON,
															0);  // Intrupt Set
	}
	else { //FALL
	gCMPL_EJCT_BELT.interruptIndex = DD_INT_EntryCallback(DD_INT_SUB_PADDLE_HOME_SENSOR,
														   BELT_FALL,
														   CMPL_EJCT_BELT_InterruptSubPaddleHomeSensorOFF,
														   0);	// Intrupt Set
	}

}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_BELT_CancelInterrupt
// Description   : 割り込みクリア
// Parameter	  :aEdge (RISE/FALL)
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_BELT_CancelInterrupt(UC   aEdge)
{
	if( gCMPL_EJCT_BELT.interruptIndex != DD_INT_NUM ) {

		if(aEdge == RISE) {
			DD_INT_CancelCallback(DD_INT_SUB_PADDLE_HOME_SENSOR,
			 						BELT_RISE,
			 						(UC)gCMPL_EJCT_BELT.interruptIndex); // 割り込みキャンセル
		}
		else {
			DD_INT_CancelCallback(DD_INT_SUB_PADDLE_HOME_SENSOR,
									BELT_FALL,
									(UC)gCMPL_EJCT_BELT.interruptIndex); // 割り込みキャンセル

		}
	 }
}


 ///////////////////////////////////////////////////////////////////
 // Function Name : CMPL_EJCT_BELT_SetEventTimer
 // Description   : イベントタイマーセット
 // Parameter	  : aTime (タイマー値 (×1[ms]))
 // Return		  : ‐
 ///////////////////////////////////////////////////////////////////
void CMPL_EJCT_BELT_SetEventTimer(US aTime, UL aEventType)
{
	 gCMPL_EJCT_BELT.timerIndex = MSP_SetEventTimer(aTime,
	 												CMPL_EJCT_BELT_ReceiveEventTimerTimeUp,
	 												aEventType);	 // Timer Set
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_BELT_CancelEventTimer
// Description   : イベントタイマーキャンセル
// Parameter	  : aTime (タイマー値 (×1[ms]))
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_BELT_CancelEventTimer()
{
	 if( gCMPL_EJCT_BELT.timerIndex != MSP_MAX_TMR_NUM ) {
		 MSP_CancelEventTimer( gCMPL_EJCT_BELT.timerIndex);
	 	 gCMPL_EJCT_BELT.timerIndex = MSP_MAX_TMR_NUM;
	 }
}



///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_BELT_GetEjectorHomeSensor
// Description   : イジェクターホームセンサーの状態(Filterあり)を取得
// Parameter	  : ‐
// Return		  : センサーレベル(ON/OFF)
///////////////////////////////////////////////////////////////////
UC CMPL_EJCT_BELT_GetEjectorHomeSensor()
{
	 if( DD_DIO_GetInput(DD_DI_EJECT_HOME_SNR) == HIGH ) {
		 return ON;
	 }
	 else {
		 return OFF;
	 }
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_BELT_IsExecutiongDeviceAction
// Description   : イジェクトロールの動作状態を取得
// Parameter	  : ‐
// Return		  : true：動作中、false：停止中
///////////////////////////////////////////////////////////////////
bool CMPL_EJCT_BELT_IsExecutiongDeviceAction()
{
	 if( gCMPL_EJCT_BELT.action == CMPL_EJCT_st0 ) {  //IDLE 状態
		 return false;
	 }
	 else {
		 return true;
	 }
}


bool CMPL_EJCT_BELT_IsOccurFault()
{
	 if( gCMPL_EJCT_BELT_FaultStat == FAULT_OCCUR ) {
		 return true;
	 }
	 else {
		 return false;
	 }
}



///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_ROLL_Reset
// Description   : イジェクトロール リセット
// Parameter	  : ‐
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_BELT_Reset()
{
	 gCMPL_EJCT_BELT.action = CMPL_EJCT_st0;
	 gCMPL_EJCT_BELT.compoAction = CMPL_EJCT_st0;

	 gCMPL_EJCT_BELT.direction = 0;
	 gCMPL_EJCT_BELT.sheetNo = SHEETNO_NUM;
	 gCMPL_EJCT_BELT.timerIndex = MSP_MAX_TMR_NUM;
	 gCMPL_EJCT_BELT.interruptIndex = DD_INT_NUM;

	 gCMPL_EJCT_BELT_FaultStat = FAULT_NOT_OCCUR;

	 
	 DD_PWM_Stop(DD_PWM_ID_EJECT_BELT_CUR);
	 
	 DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_ENB, OFF);	 
	 DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_PHASE1, OFF);
	 DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_PHASE2, OFF);

	 CMPL_EJCT_BELT_SetProfile(); 
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_BELT_DeviceStop
// Description   : イジェクトロール停止
// Parameter	  : ‐
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_BELT_DeviceStop()
{
	 US  aAction = gCMPL_EJCT_BELT.action & CMPL_EJCT_BELT_ACTION_MASK;
	 UC aSheetNo = gCMPL_EJCT_BELT.sheetNo;

	 CMPL_EJCT_BELT_CancelEventTimer();
	 
	 CMPL_EJCT_BELT_CancelInterrupt(RISE);
	 CMPL_EJCT_BELT_CancelInterrupt(FALL);

	if ( gCMPL_EJCT_BELT.action != CMPL_EJCT_st0 ) {
		
		 // Slow Down Stop
		 DD_STM_SlowDownStop(DD_STM_ID_EJECT_BELT, CMPL_EJCT_BELT_DeviceStopEnd);
	}

	//V3.6.92  StopFinisher -> Stacker Fail発生時 →Ejectの動作が止まれてStandbyに行かない不具合
	if(CMPL_EJCT_st0 ==gCMPL_EJCT_BELT.compoAction)  // Component Stopから呼ばれるときは実行しない
	  	CMPL_EJCT_InformedEjectMotorComp(aAction, aSheetNo);
	
 	gCMPL_EJCT_BELT.action = CMPL_EJCT_st0;
	gCMPL_EJCT_BELT.sheetNo = SHEETNO_NUM;
	gCMPL_EJCT_BELT.interruptIndex = DD_INT_NUM;
	gCMPL_EJCT_BELT.compoAction = CMPL_EJCT_st0;
	
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_BELT_DeviceStopEnd
// Description   : イジェクトロール停止
// Parameter	 : ‐
// Return		 : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_BELT_DeviceStopEnd(SS aResult)
{
	DD_PWM_Stop(DD_PWM_ID_EJECT_BELT_CUR);
	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_ENB, OFF);
	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_PHASE1, OFF);
	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_PHASE2, OFF);	
}


///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_BELT_StopSheet
// Description   : 指定シートの動作停止
// Parameter	  : aSheetNo (指定シート番号)
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_BELT_StopSheet(UC aSheetNo)
{
	 if( aSheetNo == SHEETNO_NUM) {												 // 制御中の用紙のコンディションをチェック
		 if( gCMPL_EJCT_BELT.sheetNo != SHEETNO_NUM ) {
			 if( SRVC_CheckSheetConditionNormal(gCMPL_EJCT_BELT.sheetNo) == false ) {
				 CMPL_EJCT_BELT_DeviceStop();
			 }
			
		 }		
	 }
	 else {																		 // 指定シートの制御を停止する
		 if( aSheetNo == gCMPL_EJCT_BELT.sheetNo ) {
			 CMPL_EJCT_BELT_DeviceStop();
		 }		 
	 }
}


void EJCT_BELT_ReceiveSteppingMotorEvent(SS aResult)
{
	switch( gCMPL_EJCT_BELT.action ) {

	// Component
	case CMPL_EJCT_COMPO_FWD_st2 :
	case CMPL_EJCT_COMPO_FWD_st4 : 	// T2
	case CMPL_EJCT_COMPO_FWD_st5 : 	// SpeedDown Wait ..
	case CMPL_EJCT_COMPO_FWD_st7 : 	// T3
	case CMPL_EJCT_COMPO_FWD_st8 :  // StepDown 
	case CMPL_EJCT_COMPO_REV_st5 :  	
		gCMPL_EJCT_BELT.action  ++; 	break;

	// SubPaddle Home
	case CMPL_EJCT_PADHOME_st5 :
		gCMPL_EJCT_BELT.action  ++;		break;

	case CMPL_EJCT_EJTHOME_st4 :  //T2 
	case CMPL_EJCT_EJTHOME_st5 :  // Speed Down 
	case CMPL_EJCT_EJTHOME_st7 :  //T3	
	case CMPL_EJCT_EJTHOME_st8 :  // StepDown	
		gCMPL_EJCT_BELT.action  ++;		break;

	// SubPaddle Compile
	case CMPL_EJCT_COMPILE_st4 :
		gCMPL_EJCT_BELT.action  ++;		break;

	// Eject 
	case CMPL_EJCT_EJECT_st4: //T2	
	case CMPL_EJCT_EJECT_st5: //Speed Down
	case CMPL_EJCT_EJECT_st7: //T3		
	case CMPL_EJCT_EJECT_st8: //StepDown	
		gCMPL_EJCT_BELT.action  ++;		break;
		
	default:  return;
		
	}

	CMPL_EJCT_BELT_ControlThread();
}

void EJCT_BELT_ReceiveStartClock(SS aResult)
{
	gCMPL_EJCT_BELT.startClock = EJCT_START_CLK_RECV;

	switch( gCMPL_EJCT_BELT.action ) {

	case CMPL_EJCT_EJTHOME_st4 :  //OFF 
	case CMPL_EJCT_EJECT_st4 :  //(0526) StartClock完了後、
		CMPL_EJCT_BELT_ControlThread();
		break;

	default :  break;
	}
}
// ↓↓↓↓↓↓↓↓↓↓↓↓↓↓
// V3.5.91 
///////////////////////////////////////////////////////////////////
// Function Name : EJCT_BELT_Receive1PulseCallback
// Description   : //Receive DummyPulse callback
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void EJCT_BELT_Receive1PulseCallback(SS aResult)
{
	switch( gCMPL_EJCT_BELT.action ) {
	
		case CMPL_EJCT_PADHOME_st11_1 : 
			gCMPL_EJCT_BELT.action = CMPL_EJCT_PADHOME_st11_2 ;
			CMPL_EJCT_BELT_ControlThread();
		break;

		case CMPL_EJCT_EJTHOME_st11_1 : 
			gCMPL_EJCT_BELT.action = CMPL_EJCT_EJTHOME_st11_2 ;
			CMPL_EJCT_BELT_ControlThread();
		break;

		case CMPL_EJCT_EJECT_st11_1 : 
			gCMPL_EJCT_BELT.action = CMPL_EJCT_EJECT_st11_2 ;
			CMPL_EJCT_BELT_ControlThread();
		break;
	
		default :  break;
	}

}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_EJCT_BELT_SetProfile
// Description   :   // モータProfile設定
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void CMPL_EJCT_BELT_SetProfile(void)
{
	DD_STM_SetProfile(DD_STM_PROF_ID_EJCT_BELT_PRF_1, cEJCT_Profile_1);
	DD_STM_SetProfile(DD_STM_PROF_ID_EJCT_BELT_PRF_2, cEJCT_Profile_2);
	DD_STM_SetProfile(DD_STM_PROF_ID_EJCT_BELT_PRF_3, cEJCT_Profile_3);
	DD_STM_SetProfile(DD_STM_PROF_ID_EJCT_BELT_PRF_EJECT, cEJCT_Profile_Eject);
}


///////////////////////////////////////////////////////////////////
// Function Name : CompoForward_st1
// Description   : モータ駆動設定
// Parameter	 :
// Return		 : 
///////////////////////////////////////////////////////////////////
void CompoForward_st1()		
{
	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_PHASE1, OFF);
	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_PHASE2, OFF);
	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_DIR, gCMPL_EJCT_BELT.direction);  // CW

	DD_PWM_SetFrequency(DD_PWM_ID_EJECT_BELT_CUR, 100000, EJCT_MOT_CUR_LOW);
	DD_PWM_Start(DD_PWM_ID_EJECT_BELT_CUR);

	 CMPL_EJCT_BELT_SetEventTimer(CMPL_EJCT_BELT_TIME_30, eEJCT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : CompoForward_st2
// Description   : モータ駆動開始
// Parameter	 :
// Return		 :
///////////////////////////////////////////////////////////////////
void CompoForward_st2()		
{
	// 1. 強電流　2600PPSまで(EjectProfile)SlowUp
	// DD_PWM_SetFrequency(DD_PWM_ID_EJECT_BELT_CUR, 100000, 90);
	DD_PWM_SetFrequency(DD_PWM_ID_EJECT_BELT_CUR, 100000, EJCT_MOT_CUR_HIGH);

	DD_STM_SetProfileID(DD_STM_ID_EJECT_BELT, gCMPL_EJCT_BELT.profile); // Profile2
	DD_STM_StartClock(DD_STM_ID_EJECT_BELT, PROF_PULSE_STEP_30, EJCT_BELT_ReceiveSteppingMotorEvent);
}

///////////////////////////////////////////////////////////////////
// Function Name : CompoForward_st3_ONWait
// Description   : EjectHomeSNR ON待ち中 
// Parameter	 :
// Return		 :
///////////////////////////////////////////////////////////////////
void CompoForward_st3_ONWait()  
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : CompoForward_st3_OFFWait
// Description   : EjectHomeSNR　OFF待ち中
// Parameter	 :
// Return		 : ‐
///////////////////////////////////////////////////////////////////
void CompoForward_st3_OFFWait()
{
	// No Action
}


///////////////////////////////////////////////////////////////////
// Function Name : CompoForward_st4
// Description   : OFF受信(T2開始)
// Parameter	 :
// Return		 : ‐
///////////////////////////////////////////////////////////////////
void CompoForward_st4()		
{
	// T2 Pulse
	DD_STM_AppointPulseTiming(DD_STM_ID_EJECT_BELT, 
							       SRVC_GetNvmValueUS(NVMID_EJECT_SPEED_CHANGE_TIME_2),
							       EJCT_BELT_ReceiveSteppingMotorEvent);
}

///////////////////////////////////////////////////////////////////
// Function Name : CompoForward_st5
// Description   : T2 -> SpeedDown
// Parameter	 :
// Return		 : ‐
///////////////////////////////////////////////////////////////////
void CompoForward_st5()	
{
	// Speed Down 
	DD_STM_ChangeClock(DD_STM_ID_EJECT_BELT, 55, EJCT_BELT_ReceiveSteppingMotorEvent);
}

///////////////////////////////////////////////////////////////////
// Function Name : CompoForward_st6_ONWait
// Description   : EjectHomeSNR　ON待ち中
// Parameter	 :
// Return		 : ‐
///////////////////////////////////////////////////////////////////
void CompoForward_st6_ONWait()	
{
	// SS aResult;
	// For New DD Test /////////////////////////////////////////////////////////
	// DD_STM_ShutDownStop(DD_STM_ID_EJECT_BELT);
	// CMPL_EJCT_BELT_DeviceStopEnd(aResult);		
	// CMPL_EJCT_CompleteCompoEjcterMotor((US)gCMPL_EJCT_BELT.compoAction);
	// For New DD Test /////////////////////////////////////////////////////////
}

///////////////////////////////////////////////////////////////////
// Function Name : CompoForward_st7
// Description   : ON受信(T3開始)
// Parameter	 : 無し
// Return		 : 
///////////////////////////////////////////////////////////////////
void CompoForward_st7()	
{
	 // Eject Home SNR ON検知後
	 DD_STM_AppointPulseTiming(DD_STM_ID_EJECT_BELT, 
								    SRVC_GetNvmValueUC(NVMID_EJECT_COMPILE_MOTOR_STOP_PULSE),
								    EJCT_BELT_ReceiveSteppingMotorEvent);
}

///////////////////////////////////////////////////////////////////
// Function Name : CompoForward_st7
// Description   : T3後、StepDown設定
// Parameter	 : 無し
// Return		 : 
///////////////////////////////////////////////////////////////////
void CompoForward_st8()
{
	// StepDown
	DD_STM_SlowDownStop(DD_STM_ID_EJECT_BELT, EJCT_BELT_ReceiveSteppingMotorEvent);
}

///////////////////////////////////////////////////////////////////
// Function Name : CompoForward_st7
// Description   : 後励磁
// Parameter	 : 無し
// Return		 : 無し
///////////////////////////////////////////////////////////////////
void CompoForward_st9()
{
	// 弱電流
	DD_PWM_SetFrequency(DD_PWM_ID_EJECT_BELT_CUR, 100000, EJCT_MOT_CUR_LOW);
	CMPL_EJCT_BELT_SetEventTimer(CMPL_EJCT_BELT_TIME_30 , eEJCT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : CompoForward_st7
// Description  ;		//完了
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void CompoForward_st10()
{
	DD_PWM_Stop(DD_PWM_ID_EJECT_BELT_CUR);

	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_ENB, OFF);	
//	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_PHASE1, OFF);
//	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_PHASE2, OFF);
	
	CMPL_EJCT_InformedEjectMotorComp((US)gCMPL_EJCT_BELT.compoAction,
										   gCMPL_EJCT_BELT.sheetNo);
	  gCMPL_EJCT_BELT.compoAction =  CMPL_EJCT_st0;
	  gCMPL_EJCT_BELT.action = CMPL_EJCT_st0;
}


// Component 動作(Eject Mot - CCW ) CMPL_EJCT_COMPO_REV = 0x0200,  
///////////////////////////////////////////////////////////////////
// Function Name : CompoReverse_st1
// Description	// モータ駆動設定
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void CompoReverse_st1()
{
	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_PHASE1, OFF);
	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_PHASE2, ON);
	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_DIR, gCMPL_EJCT_BELT.direction);  // CW

	DD_PWM_SetFrequency(DD_PWM_ID_EJECT_BELT_CUR, 100000, EJCT_MOT_CUR_LOW);
	DD_PWM_Start(DD_PWM_ID_EJECT_BELT_CUR);

	CMPL_EJCT_BELT_SetEventTimer(CMPL_EJCT_BELT_TIME_30 , eEJCT_NORMAL_EVT);

}

///////////////////////////////////////////////////////////////////
// Function Name : CompoReverse_st2
// Description ;		// モータ駆動開始
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void CompoReverse_st2()
{
	//1. 強電流　Profile2 SlowUp
	DD_PWM_SetFrequency(DD_PWM_ID_EJECT_BELT_CUR, 100000, EJCT_MOT_CUR_HIGH);

	DD_STM_SetProfileID(DD_STM_ID_EJECT_BELT, gCMPL_EJCT_BELT.profile); //Profile2
	DD_STM_StartClock(DD_STM_ID_EJECT_BELT, 29, NULL);

	gCMPL_EJCT_BELT.diagPaddleCnt = 0; 

	//2. Interrupt 設定
	if(DD_DIO_GetInput(DD_DI_SUB_PADDLE_HOME_SNR)) { //SubPaddle HomeSNR=ON

		// 割込み設定( SubPaddleHome SNR  Off)
		CMPL_EJCT_BELT_SetInterrupt(FALL);

	}
	else { //SubPaddle HomeSNR=OFF

		// 割込み設定( SubPaddleHome SNR On)
		CMPL_EJCT_BELT_SetInterrupt(RISE);

	}

}

///////////////////////////////////////////////////////////////////
// Function Name : CompoReverse_st3_ONWait
// Description ;	//　SubPaddleHomeSNR　ON待ち
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void CompoReverse_st3_ONWait()
{
	// 割込み設定( SubPaddleHome SNR On)
	CMPL_EJCT_BELT_SetInterrupt(RISE);

}

///////////////////////////////////////////////////////////////////
// Function Name : CompoReverse_st4
// Description ;	//　T1 Timer
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void CompoReverse_st4()
{
	//2. T1 Timer設定(NVM)
	CMPL_EJCT_BELT_SetEventTimer(SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_STOP_TIME_1),
									eEJCT_NORMAL_EVT);

}

///////////////////////////////////////////////////////////////////
// Function Name : CompoReverse_st5
// Description  ; //StepDown
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void CompoReverse_st5()
{
	//StepDown
	DD_STM_SlowDownStop(DD_STM_ID_EJECT_BELT, EJCT_BELT_ReceiveSteppingMotorEvent);

}
///////////////////////////////////////////////////////////////////
// Function Name : CompoReverse_st6
// Description  ; //後励磁
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void CompoReverse_st6()
{
 	//弱電流 - 
 	DD_PWM_SetFrequency(DD_PWM_ID_EJECT_BELT_CUR, 100000, EJCT_MOT_CUR_LOW);
 	
	CMPL_EJCT_BELT_SetEventTimer(CMPL_EJCT_BELT_TIME_30,  eEJCT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : CompoReverse_st7
// Description  ;		//完了
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void CompoReverse_st7()
{
	DD_PWM_Stop(DD_PWM_ID_EJECT_BELT_CUR);

	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_ENB, OFF);

	CMPL_EJCT_BELT_CancelInterrupt(RISE);
	CMPL_EJCT_BELT_CancelInterrupt(FALL);

	CMPL_EJCT_InformedEjectMotorComp((US)gCMPL_EJCT_BELT.compoAction,
										   gCMPL_EJCT_BELT.sheetNo);
	   gCMPL_EJCT_BELT.compoAction=  CMPL_EJCT_st0;
	   gCMPL_EJCT_BELT.action =  CMPL_EJCT_st0;

}



///////////////////////////////////////////////////////////////////
// Function Name : SubPaddleHome_st1
// Description   : EjectMot,  Paddle 設定
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void SubPaddleHome_st1(void)
{

	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_ENB, ON);

	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_PHASE1, OFF);	
	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_PHASE2, ON);
	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_DIR, gCMPL_EJCT_BELT.direction);  // CW

	DD_PWM_SetFrequency(DD_PWM_ID_EJECT_BELT_CUR, 100000, EJCT_MOT_CUR_LOW);

	DD_PWM_Start(DD_PWM_ID_EJECT_BELT_CUR);
	
	//20ms Timer
	CMPL_EJCT_BELT_SetEventTimer(CMPL_EJCT_BELT_TIME_20, eEJCT_NORMAL_EVT);

	PrintEjectLog_Short(CMPL_EJCT_PADHOME_st1, gCMPL_EJCT_BELT.sheetNo, 0);	//Debug Log ..
}

//V3.5.91
///////////////////////////////////////////////////////////////////
// Function Name : SubPaddleHome_st11_1
// Description   :DummyPulse設定
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void SubPaddleHome_st11_1(void)
{
	//Dummy Pulse 1 Clock Out.
	DD_STM_SetProfileID(DD_STM_ID_EJECT_BELT, gCMPL_EJCT_BELT.profile); //Profile2
	DD_STM_MoveBySetPulse(DD_STM_ID_EJECT_BELT,  PROF_PULSE_STEP_30, 1, EJCT_BELT_Receive1PulseCallback);
	PrintEjectLog_Short(CMPL_EJCT_PADHOME_st11_1, gCMPL_EJCT_BELT.sheetNo, 0);	//Debug Log ..
}


///////////////////////////////////////////////////////////////////
// Function Name : SubPaddleHome_st11_2
// Description   : DummyPulseCallback後、Timer設定
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void SubPaddleHome_st11_2(void)
{
	//1. Timer設定(30ms)
	CMPL_EJCT_BELT_SetEventTimer(CMPL_EJCT_BELT_TIME_30 , eEJCT_NORMAL_EVT);
	
	PrintEjectLog_Short(CMPL_EJCT_PADHOME_st11_2,  gCMPL_EJCT_BELT.sheetNo, 0);	//Debug Log ..
}


///////////////////////////////////////////////////////////////////
// Function Name : SubPaddleHome_st2
// Description   :EjectMot駆動開始
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void SubPaddleHome_st2(void)
{

	//1. 強電流　Profile2 SlowUp
	DD_PWM_SetFrequency(DD_PWM_ID_EJECT_BELT_CUR, 100000, EJCT_MOT_CUR_HIGH);

	DD_STM_SetProfileID(DD_STM_ID_EJECT_BELT, gCMPL_EJCT_BELT.profile); //Profile2
	DD_STM_StartClock(DD_STM_ID_EJECT_BELT, 29, NULL);


	//2. FailTimer設定
	if(DD_DIO_GetInput(DD_DI_SUB_PADDLE_HOME_SNR)) { //SubPaddle HomeSNR=ON

		// 割込み設定( SubPaddleHome SNR  Off)
		CMPL_EJCT_BELT_SetInterrupt(FALL);

		 //Fail Timer
		CMPL_EJCT_BELT_SetEventTimer(CMPL_EJCT_BELT_FAILTIME_300,
										eEJCT_SUBPADDLE_HOMESNR_OFFFAIL_EVT);
		PrintEjectLog_Short(CMPL_EJCT_PADHOME_st2, gCMPL_EJCT_BELT.sheetNo, ON);	//Debug Log ..
	} else { //SubPaddle HomeSNR=OFF

		// 割込み設定( SubPaddleHome SNR On)
		CMPL_EJCT_BELT_SetInterrupt(RISE);

		gCMPL_EJCT_BELT.action = CMPL_EJCT_PADHOME_st3_ONWait;
		
		//Fail Timer
		CMPL_EJCT_BELT_SetEventTimer(CMPL_EJCT_BELT_FAILTIME_300,
										eEJCT_SUBPADDLE_HOMESNR_ONFAIL_EVT);
		PrintEjectLog_Short(CMPL_EJCT_PADHOME_st2, gCMPL_EJCT_BELT.sheetNo, OFF);	//Debug Log ..
	}
}



///////////////////////////////////////////////////////////////////
// Function Name : SubPaddleHome_st3
// Description   :  SNR ON Wait ..
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void SubPaddleHome_st3_ONWait(void)
{
	// 割込み設定( SubPaddleHome SNR On)
	CMPL_EJCT_BELT_SetInterrupt(RISE);
	
	// ON FailTimer設定
	CMPL_EJCT_BELT_SetEventTimer(CMPL_EJCT_BELT_FAILTIME_300,
	                                                        eEJCT_SUBPADDLE_HOMESNR_ONFAIL_EVT);
}


///////////////////////////////////////////////////////////////////
// Function Name : SubPaddleHome_st4
// Description   :   T1 Timer設定(NVM)
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void SubPaddleHome_st4(void)
{
	//2. T1 Timer設定(NVM)
	CMPL_EJCT_BELT_SetEventTimer(SRVC_GetNvmValueUC(NVMID_SUB_PADDLE_STOP_TIME_1),
									eEJCT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : SubPaddleHome_st5
// Description   :  StepDown設定
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void SubPaddleHome_st5(void)
{
	//StepDown
	DD_STM_SlowDownStop(DD_STM_ID_EJECT_BELT, EJCT_BELT_ReceiveSteppingMotorEvent);
}

///////////////////////////////////////////////////////////////////
// Function Name : SubPaddleHome_st6
// Description   : 後励磁
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void SubPaddleHome_st6(void)
{
 	//弱電流 - 
 	DD_PWM_SetFrequency(DD_PWM_ID_EJECT_BELT_CUR, 100000, EJCT_MOT_CUR_LOW); 	

//	CMPL_EJCT_BELT_SetEventTimer(CMPL_EJCT_BELT_TIME_30,	eEJCT_NORMAL_EVT);
	CMPL_EJCT_BELT_SetEventTimer(0,		eEJCT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : SubPaddleHome_st7
// Description   : //動作完了
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void SubPaddleHome_st7(void)
{
	CMPL_EJCT_BELT_CancelInterrupt(RISE);
	CMPL_EJCT_BELT_CancelInterrupt(FALL);

	 gCMPL_EJCT_BELT.action=  CMPL_EJCT_st0;

	CMPL_EJCT_InformedEjectMotorComp((US)CMPL_EJCT_PADHOME,
	 									gCMPL_EJCT_BELT.sheetNo);
	PrintEjectLog_Short(CMPL_EJCT_PADHOME_st7, gCMPL_EJCT_BELT.sheetNo, 0);	//Debug Log ..
}


///////////////////////////////////////////////////////////////////
// Function Name : EjectHome_st1
// Description   :  EjectMot駆動設定
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void EjectHome_st1(void)
{

	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_PHASE1, OFF);
	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_PHASE2, OFF);
	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_DIR, gCMPL_EJCT_BELT.direction);	// CW

	//30ms前強電流に変更（V3.0.0)
	DD_PWM_SetFrequency(DD_PWM_ID_EJECT_BELT_CUR, 100000, EJCT_MOT_CUR_HIGH);
	DD_PWM_Start(DD_PWM_ID_EJECT_BELT_CUR);

	CMPL_EJCT_BELT_SetEventTimer(CMPL_EJCT_BELT_TIME_20 , eEJCT_NORMAL_EVT);

	PrintEjectLog_Short(CMPL_EJCT_EJTHOME_st1, gCMPL_EJCT_BELT.sheetNo, 0); //Debug Log ..
}

//V3.5.91
///////////////////////////////////////////////////////////////////
// Function Name : EjectHome_st11_1
// Description   :DummyPulse設定
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void EjectHome_st11_1(void)
{
	//Dummy Pulse 1 Clock Out.
	DD_STM_SetProfileID(DD_STM_ID_EJECT_BELT, gCMPL_EJCT_BELT.profile); //Profile2
	DD_STM_MoveBySetPulse(DD_STM_ID_EJECT_BELT,  PROF_PULSE_STEP_30, 1, EJCT_BELT_Receive1PulseCallback);
	PrintEjectLog_Short(CMPL_EJCT_EJTHOME_st11_1, gCMPL_EJCT_BELT.sheetNo, 0);	//Debug Log ..
}


///////////////////////////////////////////////////////////////////
// Function Name : EjectHome_st11_2
// Description   : DummyPulseCallback後、Timer設定
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void EjectHome_st11_2(void)
{
	//1. Timer設定(30ms)
	CMPL_EJCT_BELT_SetEventTimer(CMPL_EJCT_BELT_TIME_30 , eEJCT_NORMAL_EVT);

	PrintEjectLog_Short(CMPL_EJCT_EJTHOME_st11_2,  gCMPL_EJCT_BELT.sheetNo, 0);	//Debug Log ..
}

///////////////////////////////////////////////////////////////////
// Function Name : EjectHome_st2
// Description   :  EjectMot駆動開始
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void EjectHome_st2(void)
{
	DD_STM_SetProfileID(DD_STM_ID_EJECT_BELT, gCMPL_EJCT_BELT.profile); //Profile2
	DD_STM_StartClock(DD_STM_ID_EJECT_BELT, PROF_PULSE_STEP_30, EJCT_BELT_ReceiveStartClock);
	gCMPL_EJCT_BELT.startClock = EJCT_START_CLK_WAIT;  //StepUp Wait
	gCMPL_EJCT_BELT.action = CMPL_EJCT_EJTHOME_st3_OFFWait; 
	PrintEjectLog_Short(CMPL_EJCT_EJTHOME_st2, gCMPL_EJCT_BELT.sheetNo, 0);	//Debug Log ..

	// 2. FailTimer設定
	 if(CMPL_EJCT_BELT_GetEjectorHomeSensor()) { // Eject HomeSNR=ON
 		 CMPL_EJCT_BELT_SetEventTimer(CMPL_EJCT_BELT_FAILTIME_300, eEJCT_EJECT_HOMESNR_OFFFAIL_EVT);
	 } else { // Eject HomeSNR=OFF
	 	// OFF→ON変化でFailTimer設定(0526)		
	 }
	
}

///////////////////////////////////////////////////////////////////
// Function Name : EjectHome_st3_OFFWait
// Description   :  EjectHomeSNR　Off待ち中
// Parameter	   :// Return 	   : ‐
///////////////////////////////////////////////////////////////////
void EjectHome_st3_OFFWait(void)
{
}

///////////////////////////////////////////////////////////////////
// Function Name : EjectHome_st4
// Description   :  Eject Home SNR OFF受信(T2 Pulse+SpeedDown)
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void EjectHome_st4(void)
{
	// Step Up 完了 Check 
	if ( gCMPL_EJCT_BELT.startClock == EJCT_START_CLK_RECV ) {
		
		 //T2 Pulse
		 DD_STM_SetProfileID(DD_STM_ID_EJECT_BELT, gCMPL_EJCT_BELT.profile); //Profile2
		 DD_STM_AppointPulseTiming(DD_STM_ID_EJECT_BELT, 
		 							SRVC_GetNvmValueUS(NVMID_EJECT_SPEED_CHANGE_TIME_2),
		 							EJCT_BELT_ReceiveSteppingMotorEvent);
		PrintEjectLog_Short(CMPL_EJCT_EJTHOME_st4, gCMPL_EJCT_BELT.sheetNo, 0);	//Debug Log ..
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : EjectHome_st5
// Description   :  SpeedDown
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void EjectHome_st5(void)
{
	//Speed Down 
	DD_STM_SetProfileID(DD_STM_ID_EJECT_BELT, gCMPL_EJCT_BELT.profile); //Profile2
	DD_STM_ChangeClock(DD_STM_ID_EJECT_BELT, 55, EJCT_BELT_ReceiveSteppingMotorEvent);
	PrintEjectLog_Short(CMPL_EJCT_EJTHOME_st5, gCMPL_EJCT_BELT.sheetNo, 0);	//Debug Log ..

 }


///////////////////////////////////////////////////////////////////
// Function Name : EjectHome_st6_ONWait
// Description   :  EjectHomeSNR　On待ち中
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void EjectHome_st6_ONWait(void)
{	
   	 // FailTimer設定
	 CMPL_EJCT_BELT_SetEventTimer(1000,		 eEJCT_EJECT_HOMESNR_ONFAIL_EVT);
	PrintEjectLog_Short(CMPL_EJCT_EJTHOME_st6_ONWait, gCMPL_EJCT_BELT.sheetNo, 0);	//Debug Log ..
}


///////////////////////////////////////////////////////////////////
// Function Name : EjectHome_st7
// Description   :  T3
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void EjectHome_st7(void)
{
 	 //T3 Pulse後
 	 DD_STM_SetProfileID(DD_STM_ID_EJECT_BELT, gCMPL_EJCT_BELT.profile); //Profile2
	 DD_STM_AppointPulseTiming(DD_STM_ID_EJECT_BELT, 
	 							SRVC_GetNvmValueUC(NVMID_EJECT_COMPILE_MOTOR_STOP_PULSE),
	 							EJCT_BELT_ReceiveSteppingMotorEvent);
	 PrintEjectLog_Short(CMPL_EJCT_EJTHOME_st7, gCMPL_EJCT_BELT.sheetNo, 0);	//Debug Log ..
}


///////////////////////////////////////////////////////////////////
// Function Name : EjectHome_st8
// Description   :   T3後、StepDown設定
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void EjectHome_st8(void)
{
	//StepDown
	DD_STM_SetProfileID(DD_STM_ID_EJECT_BELT, gCMPL_EJCT_BELT.profile); //Profile2
	DD_STM_SlowDownStop(DD_STM_ID_EJECT_BELT, EJCT_BELT_ReceiveSteppingMotorEvent);
	PrintEjectLog_Short(CMPL_EJCT_EJTHOME_st8, gCMPL_EJCT_BELT.sheetNo, 0);	//Debug Log ..
}


///////////////////////////////////////////////////////////////////
// Function Name : EjectHome_st9
// Description   : EjectHome動作完了
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void EjectHome_st9(void)
{
	//弱電流
	DD_PWM_SetFrequency(DD_PWM_ID_EJECT_BELT_CUR, 100000, EJCT_MOT_CUR_LOW);
	
	gCMPL_EJCT_BELT.action=  CMPL_EJCT_st0;
	gCMPL_EJCT_BELT.startClock = EJCT_START_CLK_UNKNOWN;
	CMPL_EJCT_InformedEjectMotorComp((US)CMPL_EJCT_EJTHOME,	gCMPL_EJCT_BELT.sheetNo);
	PrintEjectLog_Short(CMPL_EJCT_EJTHOME_st9, gCMPL_EJCT_BELT.sheetNo, 0);	//Debug Log ..
}


//Compile動作
///////////////////////////////////////////////////////////////////
// Function Name : Compile_st1
// Description   :  //Mot 駆動設定
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void Compile_st1(void)
{
	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_PHASE1, OFF);
	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_PHASE2, ON);
	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_DIR, gCMPL_EJCT_BELT.direction);	// CW

	DD_PWM_SetFrequency(DD_PWM_ID_EJECT_BELT_CUR, 100000, EJCT_MOT_CUR_LOW);

	DD_PWM_Start(DD_PWM_ID_EJECT_BELT_CUR);

	//T6 Timer 
	CMPL_EJCT_BELT_SetEventTimer((US)CMPL_EJCT_BELT_GetSubPaddleStartTime() , 
									eEJCT_NORMAL_EVT);
	
}


///////////////////////////////////////////////////////////////////
// Function Name : Compile_st2
// Description   : //Compile動作開始
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void Compile_st2(void)
{
	US   failTime=0;
	// (CR-XXX)  150716 B5 シートずれ対策
	US   num_P= (US) CMPL_EJCT_BELT_GetSubPaddleCount();
	 //SubPaddle P=1回の場合、300rpm(Profile1）にする
	if ( num_P  == 1)  {
		 gCMPL_EJCT_BELT.profile = DD_STM_PROF_ID_EJCT_BELT_PRF_1;
	}	
	
	 //1. 強電流　 SubPaddle Profile SlowUp
	 DD_PWM_SetFrequency(DD_PWM_ID_EJECT_BELT_CUR, 100000, EJCT_MOT_CUR_HIGH);
	 
	 DD_STM_SetProfileID(DD_STM_ID_EJECT_BELT, gCMPL_EJCT_BELT.profile); 
	 DD_STM_StartClock(DD_STM_ID_EJECT_BELT, 29, NULL);

	  //2. FailTimer設定
	 if(DD_DIO_GetInput(DD_DI_SUB_PADDLE_HOME_SNR)) { //SubPaddle HomeSNR=ON

		 // 割込み設定( SubPaddleHome SNR  Off)
		 CMPL_EJCT_BELT_SetInterrupt(FALL);
	 	
		  //Fail Timer(500ms * P回) : SubPaddle Timer 500->200ms (V3.6.93)
		  failTime = num_P *  200;
		 CMPL_EJCT_BELT_SetEventTimer( failTime,  eEJCT_SUBPADDLE_HOMESNR_OFFFAIL_EVT);
		 
		 gCMPL_EJCT_BELT.paddleCount=0;
		 gCMPL_EJCT_BELT.paddleStatus_before=ON;

		 gCMPL_EJCT_BELT.action = CMPL_EJCT_COMPILE_st3;
		 
		 PrintEjectLog_Short(CMPL_EJCT_COMPILE_st2,gCMPL_EJCT_BELT.sheetNo, num_P);
	 }
	 else { //SubPaddle HomeSNR=OFF
	// Fail ...
	
		CMPL_EJCT_BELT_SetInterrupt(RISE);

		  //Fail Timer(500ms * P回)  // V3.6.90 ( SubPaddleHomeSNR OFFの場合もFailタイマー同様設定が必要 ）
		  failTime = num_P * 200; //SubPaddle Timer 500->200ms (V3.6.93)
		 CMPL_EJCT_BELT_SetEventTimer( failTime,  eEJCT_SUBPADDLE_HOMESNR_OFFFAIL_EVT);
		
		gCMPL_EJCT_BELT.paddleCount=0;
		gCMPL_EJCT_BELT.paddleStatus_before=OFF;

		PrintEjectLog_Short(CMPL_EJCT_COMPILE_st2,gCMPL_EJCT_BELT.sheetNo, 0xFF); //Fail ..
	 }

}


///////////////////////////////////////////////////////////////////
// Function Name : Compile_st3
// Description   :  //P回分処理(割込みで処理するのでここでは、実装中身なし)
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void Compile_st3(void)
{

}


///////////////////////////////////////////////////////////////////
// Function Name : Compile_st4
// Description   : //StepDown開始
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void Compile_st4(void)
{
	//StepDown
	DD_STM_SlowDownStop(DD_STM_ID_EJECT_BELT, EJCT_BELT_ReceiveSteppingMotorEvent);
	PrintEjectLog_Short(CMPL_EJCT_COMPILE_st4,gCMPL_EJCT_BELT.sheetNo, 0);

}


///////////////////////////////////////////////////////////////////
// Function Name : Compile_st5
// Description   : //後励磁
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void Compile_st5(void)
{
	//弱電流 - 
	DD_PWM_SetFrequency(DD_PWM_ID_EJECT_BELT_CUR, 100000, EJCT_MOT_CUR_LOW);
	
	CMPL_EJCT_BELT_SetEventTimer(CMPL_EJCT_BELT_TIME_30,  eEJCT_NORMAL_EVT);
	 PrintEjectLog_Short(CMPL_EJCT_COMPILE_st5,gCMPL_EJCT_BELT.sheetNo, 0);
}


///////////////////////////////////////////////////////////////////
// Function Name : Compile_st6
// Description   :  //MotOFF
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void Compile_st6(void)
{
	DD_PWM_Stop(DD_PWM_ID_EJECT_BELT_CUR);

	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_ENB, OFF);	

	CMPL_EJCT_BELT_CancelInterrupt(RISE);
	CMPL_EJCT_BELT_CancelInterrupt(FALL);

	 gCMPL_EJCT_BELT.action=  CMPL_EJCT_st0;

 	 CMPL_EJCT_InformedEjectMotorComp((US)CMPL_EJCT_COMPILE,
									gCMPL_EJCT_BELT.sheetNo);
	 PrintEjectLog_Short(CMPL_EJCT_COMPILE_st6,gCMPL_EJCT_BELT.sheetNo, 0);
}


//Eject動作
///////////////////////////////////////////////////////////////////
// Function Name : Eject_st1
// Description   : //Mot 駆動設定
//////////////////////////////////////////////////////////////////
void Eject_st1(void)
{

	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_DIR, gCMPL_EJCT_BELT.direction);	// CW
	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_PHASE1, OFF);
	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_PHASE2, OFF);

	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_ENB, ON);

	// 20ms前強電流に変更（V3.0.0)
	DD_PWM_SetFrequency(DD_PWM_ID_EJECT_BELT_CUR, 100000, EJCT_MOT_CUR_HIGH);
	DD_PWM_Start(DD_PWM_ID_EJECT_BELT_CUR);

	CMPL_EJCT_BELT_SetEventTimer(CMPL_EJCT_BELT_TIME_20 , eEJCT_NORMAL_EVT);
	
	//V3.5.90 LogCheck .,.
	PrintEjectLog_Short(CMPL_EJCT_EJECT_st1, gCMPL_EJCT_BELT.sheetNo, 0);	//Debug Log ..

}

//V3.5.91 脱調対策 
///////////////////////////////////////////////////////////////////
// Function Name : Eject_st11_1
// Description   :DummyPulse設定
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void Eject_st11_1(void)
{
	//Dummy Pulse 1 Clock Out.
	DD_STM_SetProfileID(DD_STM_ID_EJECT_BELT, gCMPL_EJCT_BELT.profile); //Profile2
	DD_STM_MoveBySetPulse(DD_STM_ID_EJECT_BELT,  PROF_PULSE_STEP_30, 1, EJCT_BELT_Receive1PulseCallback);
	PrintEjectLog_Short(CMPL_EJCT_EJECT_st11_1, gCMPL_EJCT_BELT.sheetNo, 0);	//Debug Log ..
}


///////////////////////////////////////////////////////////////////
// Function Name : Eject_st11_2
// Description   : DummyPulseCallback後、Timer設定
// Parameter	  :
// Return		  : ‐
///////////////////////////////////////////////////////////////////
void Eject_st11_2(void)
{

	//1. Timer設定(30ms)
	CMPL_EJCT_BELT_SetEventTimer(CMPL_EJCT_BELT_TIME_30 , eEJCT_NORMAL_EVT);
	
	PrintEjectLog_Short(CMPL_EJCT_EJECT_st11_2,  gCMPL_EJCT_BELT.sheetNo, 0);	//Debug Log ..
}


///////////////////////////////////////////////////////////////////
// Function Name : Eject_st2
// Description   : //Eject動作開始
///////////////////////////////////////////////////////////////////
void Eject_st2(void)
{
 	  //1. 2600PPSまで(EjectProfile)SlowUp	
	  DD_STM_SetProfileID(DD_STM_ID_EJECT_BELT, gCMPL_EJCT_BELT.profile); //Profile2
	  DD_STM_StartClock(DD_STM_ID_EJECT_BELT,  PROF_PULSE_STEP_30, EJCT_BELT_ReceiveStartClock);
	  
	  gCMPL_EJCT_BELT.startClock = EJCT_START_CLK_WAIT;  //StepUp Wait
	  gCMPL_EJCT_BELT.action = CMPL_EJCT_EJECT_st3_OFFWait;

	 //2. FailTimer設定
	 if(CMPL_EJCT_BELT_GetEjectorHomeSensor()) { //Eject HomeSNR=ON
		//V3.4.0 FailTimer  300ms -> 400ms
		 CMPL_EJCT_BELT_SetEventTimer(CMPL_EJCT_BELT_FAILTIME_400,  eEJCT_EJECT_HOMESNR_OFFFAIL_EVT);
	 }
	 else { //Eject HomeSNR=OFF
		 // OFF→ON変化でFailTimer設定(0526)
		 PrintEjectLog_Short(CMPL_EJCT_EJECT_st2, 0xFFFF, 0xFFFF);	 //Debug Log ..
		 
	 }
	
	//排出動作開始を通知
	 CMPL_EJCT_InformedEjectStart(gCMPL_EJCT_BELT.sheetNo);
	PrintEjectLog_Short(CMPL_EJCT_EJECT_st2, gCMPL_EJCT_BELT.sheetNo, 0);	//Debug Log ..

}


///////////////////////////////////////////////////////////////////
// Function Name : Eject_st3_OFFWait
// Description   : //EjectHomeSNR　Off待ち
///////////////////////////////////////////////////////////////////
void Eject_st3_OFFWait()
{
}

///////////////////////////////////////////////////////////////////
// Function Name : Eject_st4
// Description   : //OFF受信(T2設定）
///////////////////////////////////////////////////////////////////
void Eject_st4(void)
{
	// Step Up 完了 Check 
	if ( gCMPL_EJCT_BELT.startClock == EJCT_START_CLK_RECV ) {

		DD_STM_SetProfileID(DD_STM_ID_EJECT_BELT, gCMPL_EJCT_BELT.profile); //Profile2
		  //T2 Pulse
		 DD_STM_AppointPulseTiming(DD_STM_ID_EJECT_BELT, 
									 SRVC_GetNvmValueUS(NVMID_EJECT_SPEED_CHANGE_TIME_2),
									 EJCT_BELT_ReceiveSteppingMotorEvent);

		  PrintEjectLog_Short(CMPL_EJCT_EJECT_st4, gCMPL_EJCT_BELT.sheetNo, 0);   //Debug Log ..
		  
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : Eject_st5
// Description   ://T2後、SpeedDown設定
///////////////////////////////////////////////////////////////////
void Eject_st5(void)
{
	// Tamperへ用紙排出完了通知
	 CMPL_EJCT_InformedEjectSheetThrough(gCMPL_EJCT_BELT.sheetNo);
//V3.0 不具合　：Set Finished At Outputメッセージ送信処理しない↓↓↓↓↓↓
//SheetMgrへ 排出完了通知
	 CMPL_EJCT_InformedEjectComp(gCMPL_EJCT_BELT.sheetNo);
//↑↑↑↑↑↑↑↑
	 DD_STM_SetProfileID(DD_STM_ID_EJECT_BELT, gCMPL_EJCT_BELT.profile); //Profile2
	 //Speed Down 
	 DD_STM_ChangeClock(DD_STM_ID_EJECT_BELT, 55, EJCT_BELT_ReceiveSteppingMotorEvent);
	 
	 PrintEjectLog_Short(CMPL_EJCT_EJECT_st5, gCMPL_EJCT_BELT.sheetNo, 0);	 //Debug Log ..
 
}


///////////////////////////////////////////////////////////////////
// Function Name : Eject_st6_ONWait
// Description   :	//EjectHomeSNR　On待ち
///////////////////////////////////////////////////////////////////
void Eject_st6_ONWait(void)
{
   	 // FailTimer設定(1000ms) 
 	 CMPL_EJCT_BELT_SetEventTimer(1000,  eEJCT_EJECT_HOMESNR_ONFAIL_EVT); 	
	 PrintEjectLog_Short(CMPL_EJCT_EJECT_st6_ONWait, gCMPL_EJCT_BELT.sheetNo, 0);	 //Debug Log ..	 
 }


///////////////////////////////////////////////////////////////////
// Function Name : Eject_st7
// Description   :  // ON受信(T3開始)
///////////////////////////////////////////////////////////////////
void Eject_st7(void)
{

	DD_STM_SetProfileID(DD_STM_ID_EJECT_BELT, gCMPL_EJCT_BELT.profile); //Profile2
	//T3 Pulse後
	DD_STM_AppointPulseTiming(DD_STM_ID_EJECT_BELT, 
							   SRVC_GetNvmValueUC(NVMID_EJECT_COMPILE_MOTOR_STOP_PULSE),
							   EJCT_BELT_ReceiveSteppingMotorEvent);
	
	PrintEjectLog_Short(CMPL_EJCT_EJECT_st7, gCMPL_EJCT_BELT.sheetNo, 0);	//Debug Log ..
}

///////////////////////////////////////////////////////////////////
// Function Name : Eject_st8
// Description  ;	// T3後、StepDown設定
///////////////////////////////////////////////////////////////////
 void Eject_st8()
{
	//StepDown
	DD_STM_SlowDownStop(DD_STM_ID_EJECT_BELT, EJCT_BELT_ReceiveSteppingMotorEvent);
	PrintEjectLog_Short(CMPL_EJCT_EJECT_st8, gCMPL_EJCT_BELT.sheetNo, 0);	//Debug Log ..

}

 ///////////////////////////////////////////////////////////////////
// Function Name : Eject_st8
// Description  ;	// 後励磁
///////////////////////////////////////////////////////////////////
 void Eject_st9()
{
	 //弱電流 - 
	 DD_PWM_SetFrequency(DD_PWM_ID_EJECT_BELT_CUR, 100000, EJCT_MOT_CUR_LOW);

 //CR008 : 30ms-> 20ms 
	CMPL_EJCT_BELT_SetEventTimer(CMPL_EJCT_BELT_TIME_20,  eEJCT_NORMAL_EVT);
	 PrintEjectLog_Short(CMPL_EJCT_EJECT_st9, gCMPL_EJCT_BELT.sheetNo, 0);	 //Debug Log ..

}

 ///////////////////////////////////////////////////////////////////
// Function Name : Eject_st8
// Description   :  //Mot Off
///////////////////////////////////////////////////////////////////
 void Eject_st10()
{
	DD_PWM_Stop(DD_PWM_ID_EJECT_BELT_CUR);

	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_ENB, OFF);	

	 gCMPL_EJCT_BELT.action=  CMPL_EJCT_st0;

	 CMPL_EJCT_InformedEjectMotorComp((US)CMPL_EJCT_EJECT,  gCMPL_EJCT_BELT.sheetNo);

	 //HFSI Update .. 
	 SRVC_IncrementHfsi(HFSIID_PAPER_REV_SUB_PADDLE);
	 SRVC_IncrementHfsi(HFSIID_PAPER_REV_PADDLE_LIFE);
	 
	 PrintEjectLog_Short(CMPL_EJCT_EJECT_st10, gCMPL_EJCT_BELT.sheetNo, 0);	 //Debug Log ..
}



void Eject_Compo_Mot_st1()
{

	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_PHASE1, OFF);
	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_PHASE2, ON);
	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_DIR, gCMPL_EJCT_BELT.direction);  // CW

	DD_PWM_SetFrequency(DD_PWM_ID_EJECT_BELT_CUR, 100000, 15);
	DD_PWM_Start(DD_PWM_ID_EJECT_BELT_CUR);
	CMPL_EJCT_BELT_SetEventTimer(CMPL_EJCT_BELT_TIME_30 , eEJCT_NORMAL_EVT);
}

void Eject_Compo_Mot_st2()
{
 	DD_PWM_SetFrequency(DD_PWM_ID_EJECT_BELT_CUR, 100000, 65);
	DD_STM_StartClock(DD_STM_ID_EJECT_BELT, 29, NULL);		
}

void Eject_Compo_Mot_st3()
{
	CMPL_EJCT_BELT_SetEventTimer(CMPL_EJCT_BELT_TIME_30 , eEJCT_NORMAL_EVT);
}

void Eject_Compo_Mot_st4()
{

	EvtParamComponentControlStopNotify aEvtParameter;

	aEvtParameter.chainNo = 12;
	aEvtParameter.linkNo = gCMPL_EJCT_BELT.compoAction;
	
	MSP_SendMessage(MID_CMPL, EVT_DIAG_COMPO_CONT_STOP, (const UC*)&aEvtParameter, sizeof(aEvtParameter), LOG_OFF);
	
	DD_PWM_Stop(DD_PWM_ID_EJECT_BELT_CUR);

	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_ENB, OFF);
	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_PHASE1, OFF);
	DD_DIO_SetOutput(DD_DO_EJECT_BELT_MOT_PHASE2, OFF);

	gCMPL_EJCT_BELT.action=  CMPL_EJCT_st0;
}



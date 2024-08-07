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
	UC preSheetNo; //(CR-008) Normal TampingJnTimingÌTimeoutÈ~ÉóÔð½¹é½ß

	UC profile;		 	// NewDDKp(Profile) //ECMPL_TMP_ProfileID profile;
	US targetIndex;   	// NewDDKp(TargetIndex)
	US movePulse;
	UC direction;
	US startDelayTime;
	UC ejectTampingCnt;  // Eject Tamping AJèÔ·ñÌJEg
	SS timerIndex;
	UC interruptIndex;
	UC sizePosAfterReturn;  //(CR-008)  Return®ìãASizePositioning®ìÀ{·é/µÈ¢
	SS timerIndex_PreTimer;  //Timer ID Çp
	SS timerIndex_TimeUpControl; //Timer ID Çp(RT d¬lØèÖ¦Ôl)
	SS timerIndex_TimeUpControl2; //(V3.5.103)Timer IDÇp (LTd¬lØèÖ¦Ô)
	UC offFailCheckCount;  //(V3.5.95) HomeOµ®ì`FbNCountB2ñÈãÈçÎHomeSNROffFail
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

//V3.0.90.  SlowUpµÄ©çEmsÈ~A50msÜÅ­d¬É·é->Timer EventType define ..
typedef  enum {
	eNORMAL_EMS,   		//NormalTampingJnÅEmsTimerÌTimeOut
	eNORMAL_HI_CUR, 	//RT­d¬`50msÌTimeout
	e1stEJECT_EMS,		//EjectTampingJnÅEmsTimerÌTimeOut
	e1stEJECT_HI_CUR, //RT­d¬`50msÌTimeout
	e2ndEJECT_EMS,		//SetLastTampingÌ14msTimerÌTimeout
	e2ndEJECT_HI_CUR,//RT­d¬`50msÌTimeout
	
}ETMP_EVT_TYPE;
//

#define  OSIKOMI_PLS_A	 	5  // µÝpXÊ
#define  ZURE_PLS_B 		 2 // YpX

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

// Component®ì
// CMPL_TMP_COMPO_TAMPER_MOT = 0x0100,
static void  LT_Component_Mot_st1(); 	// Mot ì®Ýè
static void  LT_Component_Mot_st2(); 	// ®ìJn100Pulse ã, StepDown
static void  LT_Component_Mot_st3(); 	// ãã¥
static void  LT_Component_Mot_st4(); 	// Mot Off


// TCY|WVjO®ì
// CMPL_TMP_SIZE_POS = 0x0200,
static void LT_SizePositioning_st1(); 		// Tamper HomeSNR@ON`FbN
static void LT_SizePositioning_st2_CCW(); 	// HomeOµ®ìÝè
static void LT_SizePositioning_st3_CCW(); 	// CCWûüÉ[^ì®Jn
static void LT_SizePositioning_st4_CCW(); 	// HomeOµ®ì®¹
static void LT_SizePositioning_st2_CW(); 	// Home®ìÝè
static void LT_SizePositioning_st3_CW(); 	// CWûüÉ[^ì®Jn
static void LT_SizePositioning_st4_CW(); 	// StepUpAÝóM
static void LT_SizePositioning_st4_2_CW(); 	// StepUpAÝóM ©ç StepDown®¹

static void LT_SizePositioning_st5_CW(); 	// StepUp®¹óM
static void LT_SizePositioning_st6_CW(); 	// ÀèæÔÅÝóM
static void LT_SizePositioning_st7_CW(); 	// Home®ì®¹
static void LT_SizePositioning_st8();	 	// SizePositioningJn
static void LT_SizePositioning_st9();	 	// [^ì®Jn
static void LT_SizePositioning_st10();  	// SizePositionÖÚ®®¹

// m[}^sO®ì
// CMPL_TMP_NORMAL_TAMPING = 0x0300,
static void LT_NormalTamping_st1(); 	// ®ìJnxÔÝè
static void LT_NormalTamping_st2(); 	// [^ì®Ýè
static void LT_NormalTamping_st3();  	// [^ì®Jn
static void LT_NormalTamping_st4(); 	// TampingÊuÖÚ®®¹
static void LT_NormalTamping_st5(); 	// ReturnûüÖØèÖ¦
static void LT_NormalTamping_st6(); 	// [^ì®(Return)Jn
static void LT_NormalTamping_st7(); 	// NormalTamping®¹
static void LT_NormalTamping_st8(); 	// Ì®ìÒ¿Ýè

// CWFNg^sO®ì(SetLastTamping)
// CMPL_TMP_EJECT_TAMPING = 0x0400,
static void  LT_EjectTamping_st1(); 	// ®ìJnxÔÝè
static void  LT_EjectTamping_st2(); 	// [^ì®Ýè
static void  LT_EjectTamping_st3(); 	// [^ì®Jn
static void  LT_EjectTamping_st4(); 	// TampingÊuÖÚ®®¹
static void  LT_EjectTamping_st5(); 	// ReturnûüÖØèÖ¦
static void  LT_EjectTamping_st6(); 	// [^ì®(Return)Jn
static void  LT_EjectTamping_st7(); 	// Return®ì®¹
static void  LT_EjectTamping_st8(); 	// SetLastTampingJn
static void  LT_EjectTamping_st9(); 	// [^ì®(SetLast)Jn
static void  LT_EjectTamping_st10(); 	// SetLast®ì®¹
static void  LT_EjectTamping_st11(); 	// SetLastJèÔµ`FbN
static void  LT_EjectTamping_st12(); 	// Ì®ìÒ¿Ýè

//MissREgi (V3.5.95)
static void LT_MissRegi_st1();	// MissRegi_®ìTimerÝè
static void LT_MissRegi_st2();  // MissRegi_wèpXoÍ
static void LT_MissRegi_st3();	// MissRegi_ãã¥Ýè
static void LT_MissRegi_st4();	// MissRegi_®¹	


// ItZbg®ì
// CMPL_TMP_OFFSET = 0x0500,
static void LT_Offset_st1(); 	// JnTiming²®
static void LT_Offset_st2(); 	// [^ì®Ýè
static void LT_Offset_st3(); 	// [^ì®Jn
static void LT_Offset_st4(); 	// ÀèÔ^C}Ýè
static void LT_Offset_st5(); 	// ®ìÒ¿

// ^[®ì
// CMPL_TMP_LEFT_RETURN = 0x0600,
static void LT_Return_st1(); 	// ®ìJnTriggerÒ¿
static void LT_Return_st2();	// ^C}Ýè
static void LT_Return_st3();	// [^ì®Ýè
static void LT_Return_st4(); 	// [^ì®Jn
static void LT_Return_st5(); 	// ^C}Ýè
static void LT_Return_st6(); 	// ®ìÒ¿
static void LT_Return_st7();	// SizePosJn(CR-008) 


// I[v®ì(Nigasi)
// CMPL_TMP_OPEN = 0x0800,
static void LT_Open_st1(); 		// [^ì®Ýè
static void LT_Open_st2(); 		// [^ì®Jn
static void LT_Open_st3(); 		// StepDown Jn
static void LT_Open_st4(); 		// ^C}Ýè
static void LT_Open_st5(); 		// ®ìÒ¿

static const TMP_LEFT_CallFunc  ControlThread_LT_COMPO_MOT[] = {
	{NULL},
	{LT_Component_Mot_st1},  	// Mot ì®Ýè, Oã¥
	{LT_Component_Mot_st2},  	// ®ìJn100Pulse ã, StepDown
	{LT_Component_Mot_st3 },	// ãã¥
	{LT_Component_Mot_st4}, 	// Mot Off
};

static const TMP_LEFT_CallFunc ControlThread_LT_SIZEPOS[] = {
	{NULL},
	{LT_SizePositioning_st1}, 		// Tamper HomeSNR@ON`FbN
	{LT_SizePositioning_st2_CCW},  	// HomeOµ®ìÝè
	{LT_SizePositioning_st3_CCW}, 	// CCWûüÉ[^ì®Jn
	{LT_SizePositioning_st4_CCW}, 	// HomeOµ®ì®¹

	{LT_SizePositioning_st2_CW}, 	// Home®ìÝè
	{LT_SizePositioning_st3_CW}, 	// CWûüÉ[^ì®Jn
	{LT_SizePositioning_st4_CW}, 	// StepUpAÝóM
	{LT_SizePositioning_st4_2_CW}, 	// StepUpAÝóM ©ç StepDown®¹
	{LT_SizePositioning_st5_CW}, 	// StepUp®¹óM
	{LT_SizePositioning_st6_CW},   	// ÀèæÔÅÝóM
	{LT_SizePositioning_st7_CW}, 	// Home®ì®¹

	{LT_SizePositioning_st8},	 	// SizePositioningJn
	{LT_SizePositioning_st9},	 	// [^ì®Jn
	{LT_SizePositioning_st10},		// SizePositionÖÚ®®¹
};


// m[}^sO®ì
static const TMP_LEFT_CallFunc  ControlThread_LT_NORMAL_TAMPING[] = {
	{NULL},
	{LT_NormalTamping_st1}, 	// ®ìJnxÔÝè
	{LT_NormalTamping_st2}, 	// [^ì®Ýè
	{LT_NormalTamping_st3},  	// [^ì®Jn
	{LT_NormalTamping_st4}, 	// TampingÊuÖÚ®®¹
	{LT_NormalTamping_st5}, 	// ReturnûüÖØèÖ¦
	{LT_NormalTamping_st6}, 	// [^ì®(Return)Jn
	{LT_NormalTamping_st7}, 	// NormalTamping®¹
	{LT_NormalTamping_st8}, 	// Ì®ìÒ¿Ýè
};

// CWFNg^sO®ì(SetLastTamping)
static const TMP_LEFT_CallFunc  ControlThread_LT_EJECT_TAMPING[] = {
	{NULL},
	{LT_EjectTamping_st1}, 		// ®ìJnxÔÝè
	{LT_EjectTamping_st2}, 		// [^ì®Ýè
	{LT_EjectTamping_st3}, 		// [^ì®Jn
	{LT_EjectTamping_st4}, 		// TampingÊuÖÚ®®¹
	{LT_EjectTamping_st5}, 		// ReturnûüÖØèÖ¦
	{LT_EjectTamping_st6}, 		// [^ì®(Return)Jn
	{LT_EjectTamping_st7}, 		// Return®ì®¹
	{LT_EjectTamping_st8}, 		// SetLastTampingJn
	{LT_EjectTamping_st9}, 		// [^ì®(SetLast)Jn
	{LT_EjectTamping_st10}, 	// SetLast®ì®¹
	{LT_EjectTamping_st11}, 	// SetLastJèÔµ`FbN
	{LT_EjectTamping_st12}, 	// Ì®ìÒ¿Ýè
};

// MissRegih~®ì
static const TMP_LEFT_CallFunc  ControlThread_LT_MISSREGI[] = {
	{NULL},
	{LT_MissRegi_st1}, // MissRegi_®ìTimerÝè
	{LT_MissRegi_st2}, // MissRegi_wèpXoÍ
	{LT_MissRegi_st3},  // MissRegi_ãã¥Ýè
	{LT_MissRegi_st4},  // MissRegi_®¹	
};

// ItZbg®ì
static const TMP_LEFT_CallFunc  ControlThread_LT_OFFSET[] = {
	{NULL},
	{LT_Offset_st1}, 	// JnTiming²®
	{LT_Offset_st2}, 	// [^ì®Ýè
	{LT_Offset_st3}, 	// [^ì®Jn
	{LT_Offset_st4}, 	// ÀèÔ^C}Ýè
	{LT_Offset_st5}, 	// ®ìÒ¿
};

// ^[®ì
static const TMP_LEFT_CallFunc ControlThread_LT_RETURN[] = {
	{ NULL},
	{LT_Return_st1}, 	// ®ìJnTriggerÒ¿
	{LT_Return_st2},	// ^C}Ýè
	{LT_Return_st3},	// [^ì®Ýè
	{LT_Return_st4}, 	// [^ì®Jn
	{LT_Return_st5}, 	// ^C}Ýè
	{LT_Return_st6}, 	// ®ìÒ¿
	{LT_Return_st7},       // (CR-008)
};

// I[v®ì(Nigasi)
static const TMP_LEFT_CallFunc  ControlThread_LT_OPEN[] = {
	{NULL},
	{LT_Open_st1}, 	// [^ì®Ýè
	{LT_Open_st2}, 	// [^ì®Jn
	{LT_Open_st3}, 	// StepDownÝè
	{LT_Open_st4}, 	// ^C}Ýè
	{LT_Open_st5}, 	// ®ìÒ¿
};

/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_LEFT_StartAct
// Description   : ^p[®ìÝè_®ìJn
// Parameter     : aAction (§ä·é®ì)
//				 : aSheetNo (§ä·éSheetNo)
// Return        : ]
///////////////////////////////////////////////////////////////////
void CMPL_TMP_LEFT_StartAct(ECMPL_TMP_Action aAction, UC aSheetNo)
{

	switch( aAction ) {
	// Component ®ì //////////////////////////////////////////////////////////////////////////////
	// LEFT TamperðO¤ûü(CW)Éá¬(643PPS)ÅÚ®
	case CMPL_TMP_COMPO_LEFT_CW_LOW:
		gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_LOW;
		gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_LOW;

		gCMPL_TMP_LEFT.direction = CMPL_TMP_COMM_OUTSIDE;
		gCMPL_TMP_LEFT.compoAction = aAction;
		gCMPL_TMP_LEFT.action = CMPL_TMP_COMPO_TAMPER_MOT_st1;
		break;

	// LEFT TamperðO¤ûü(CW)É¬(800PPS)ÅÚ®
	case CMPL_TMP_COMPO_LEFT_CW_MID:
		gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_MID;
		gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_MID;

		gCMPL_TMP_LEFT.direction = CMPL_TMP_COMM_OUTSIDE;
		gCMPL_TMP_LEFT.compoAction = aAction;
		gCMPL_TMP_LEFT.action = CMPL_TMP_COMPO_TAMPER_MOT_st1;
		break;

	// LEFT TamperðO¤ûü(CW)É¬(1337PPS)ÅÚ®
	case CMPL_TMP_COMPO_LEFT_CW_HI:
		gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_HI;
		gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_HI;

		gCMPL_TMP_LEFT.direction = CMPL_TMP_COMM_OUTSIDE;
		gCMPL_TMP_LEFT.compoAction = aAction;
		gCMPL_TMP_LEFT.action = CMPL_TMP_COMPO_TAMPER_MOT_st1;
		break;

	// LEFT Tamperðà¤ûü(CCW)Éá¬(643PPS)ÅÚ®
	case CMPL_TMP_COMPO_LEFT_CCW_LOW :
		gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_LOW;
		gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_LOW;

		gCMPL_TMP_LEFT.direction = CMPL_TMP_COMM_INSIDE;
		gCMPL_TMP_LEFT.compoAction = aAction;
		gCMPL_TMP_LEFT.action = CMPL_TMP_COMPO_TAMPER_MOT_st1;
		break;

	// LEFT Tamperðà¤ûü(CCW)É¬(800PPS)ÅÚ®
	case CMPL_TMP_COMPO_LEFT_CCW_MID :
		gCMPL_TMP_LEFT.profile =DD_STM_PROF_ID_TAMPER_NORMAL_MID;
		gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_MID;

		gCMPL_TMP_LEFT.direction = CMPL_TMP_COMM_INSIDE;
		gCMPL_TMP_LEFT.compoAction = aAction;
		gCMPL_TMP_LEFT.action = CMPL_TMP_COMPO_TAMPER_MOT_st1;
		break;

	// LEFT Tamperðà¤ûü(CCW)É¬(1337PPS)ÅÚ®
	case  CMPL_TMP_COMPO_LEFT_CCW_HI :
		gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_HI;
		gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_HI;

		gCMPL_TMP_LEFT.direction = CMPL_TMP_COMM_INSIDE;
		gCMPL_TMP_LEFT.compoAction = aAction;
		gCMPL_TMP_LEFT.action = CMPL_TMP_COMPO_TAMPER_MOT_st1;
		break;
	// Component ®ì //////////////////////////////////////////////////////////////////////////////

	// TCY|WVjO®ì
	case CMPL_TMP_SIZE_POS :  // = 0x0200,  Size Positioning
		gCMPL_TMP_LEFT.movePulse = CMPL_TMP_LEFT_GetSizePosition(aSheetNo);
		gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_INITIALIZE;
		gCMPL_TMP_LEFT.direction = CMPL_TMP_COMM_INSIDE;
		gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st1;
		gCMPL_TMP_LEFT.offFailCheckCount = 0;  
		//LeftSizePos
		PrintTamperLog_Short(CMPL_TMP_SIZE_POS+0x80, (US)gCMPL_TMP_LEFT.movePulse, aSheetNo);
		break;

	// m[}^sO®ì
	case CMPL_TMP_NORMAL_TAMPING : // = 0x0300,
		gCMPL_TMP_LEFT.movePulse = CMPL_TMP_LEFT_GetTampingPosition(aAction, aSheetNo);

		if ( SRVC_GetProcessSize(aSheetNo) <= CMPL_TMP_COMM_TAMPING_PROFILE_BORDER_LENGTH ) {
			gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_HI;
			gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_HI;

		} else {
		// V3.0  ( sï@F@p·[mm]297.0´¦F993PPS¬Profile)
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

	// CWFNg^sO®ì(SetLastTamping)
	case CMPL_TMP_EJECT_TAMPING :  //= 0x0400,
		gCMPL_TMP_LEFT.movePulse = CMPL_TMP_LEFT_GetTampingPosition(aAction, aSheetNo);

		if ( SRVC_GetProcessSize(aSheetNo) <= CMPL_TMP_COMM_TAMPING_PROFILE_BORDER_LENGTH ) {
			gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_HI;
			gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_HI;

		} else {
		// V3.0  ( sï@F@p·[mm]297.0´¦F993PPS¬Profile)
			gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_MID;
			gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_MID;
		//	gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_LOW;
		//	gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_LOW;
		}

		gCMPL_TMP_LEFT.startDelayTime = CMPL_TMP_GetEjectTampingDelayTime(aSheetNo);
		gCMPL_TMP_LEFT.ejectTampingCnt = 0;  // Eject Tamping AJèÔ·ñÌJEg
		gCMPL_TMP_LEFT.direction = CMPL_TMP_COMM_INSIDE;
		gCMPL_TMP_LEFT.action = CMPL_TMP_EJECT_TAMPING_st1;
		PrintTamperLog_Short(CMPL_TMP_EJECT_TAMPING, (US)gCMPL_TMP_LEFT.movePulse, aSheetNo);
		break;
		
	//MissRegih~®ì (V3.5.95)	
	case CMPL_TMP_MISSREGI : //=0x0A00,
		gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_LOW;
		gCMPL_TMP_LEFT.targetIndex = 0;		
		gCMPL_TMP_LEFT.action = CMPL_TMP_MISSREGI_st1;
		PrintTamperLog_Short(CMPL_TMP_MISSREGI, 0, aSheetNo);
		break;
		
	// ItZbg®ì
	case CMPL_TMP_OFFSET :  // = 0x0500,
		gCMPL_TMP_LEFT.movePulse = CMPL_TMP_LEFT_GetOffsetPosition(aSheetNo);
		gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_LOW; // Êí á¬ Profile
		gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_LOW;

		gCMPL_TMP_LEFT.direction = (UC)CMPL_TMP_LEFT_GetOffsetDirection(aSheetNo);
		gCMPL_TMP_LEFT.action = CMPL_TMP_OFFSET_st1;
		//LeftOffset
		PrintTamperLog_Short(CMPL_TMP_OFFSET+0x80, (US)gCMPL_TMP_LEFT.movePulse, aSheetNo);
		break;

	// ^[®ì
	case CMPL_TMP_LEFT_RETURN : // = 0x0600,
		gCMPL_TMP_LEFT.movePulse = CMPL_TMP_LEFT_GetReturnPosition(aSheetNo);
		gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_HI; // Êí ¬ Profile
		gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_HI;
		
		gCMPL_TMP_LEFT.direction = CMPL_TMP_COMM_OUTSIDE;
		gCMPL_TMP_LEFT.action = CMPL_TMP_LEFT_RETURN_st1;
		
		//CR-010		
		if (( gCMPL_TMP_LEFT.movePulse ==10) && (gCMPL_TMP_LEFT.direction == CMPL_TMP_COMM_OUTSIDE)) {
				gCMPL_TMP_LEFT.sizePosAfterReturn = true;
		}		
		PrintTamperLog_Short(CMPL_TMP_LEFT_RETURN, (US)gCMPL_TMP_LEFT.movePulse, aSheetNo);
		break;

	// I[v®ì(Nigasi)
	case CMPL_TMP_OPEN :  // = 0x0800,
		gCMPL_TMP_LEFT.profile = DD_STM_PROF_ID_TAMPER_NORMAL_HI; // Êí ¬ Profile
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
// Description   : ^p[§ä
// Parameter     : ]
// Return        : ]
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

	// TCY|WVjO®ì
	case CMPL_TMP_SIZE_POS :  // = 0x0200,  Size Positioning
		ControlThread_LT_SIZEPOS[aStatus].mpHandler(); break;

	// m[}^sO®ì
	case CMPL_TMP_NORMAL_TAMPING : // = 0x0300,
		ControlThread_LT_NORMAL_TAMPING[aStatus].mpHandler(); break;

	// CWFNg^sO®ì(SetLastTamping)
	case CMPL_TMP_EJECT_TAMPING :  //= 0x0400,
		ControlThread_LT_EJECT_TAMPING[aStatus].mpHandler(); break;
		
	// MissREGI®ì
	case CMPL_TMP_MISSREGI :  //= 0x0A00,
		ControlThread_LT_MISSREGI[aStatus].mpHandler(); break;

	// ItZbg®ì
	case CMPL_TMP_OFFSET :  //= 0x0500,
		ControlThread_LT_OFFSET[aStatus].mpHandler(); break;

	// ^[®ì
	case CMPL_TMP_LEFT_RETURN : // = 0x0600,
		ControlThread_LT_RETURN[aStatus].mpHandler(); break;

	// I[v®ì(Nigasi)
	case CMPL_TMP_OPEN :  // = 0x0800,
		ControlThread_LT_OPEN[aStatus].mpHandler(); break;

	default :
		break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_LEFT_ReceiveEventTimerTimeUp
// Description   : Cxg^C}Ì^CAbv
// Parameter     : timerID (Timer ID)
//               : notUsed (Not Used)
// Return        : ]
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
		case CMPL_TMP_SIZE_POS_st2_CCW :  // 20ms Timeout Oã¥
			gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st3_CCW;
			CMPL_TMP_LEFT_ControlThread();
			break;
		case CMPL_TMP_SIZE_POS_st4_CCW : // Home Oµ®ì®¹ -> 50ms Timeout
			gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st2_CW; // Home ®ìÝè
			CMPL_TMP_LEFT_ControlThread();
			break;
		case CMPL_TMP_SIZE_POS_st2_CW : // 20ms Timeout Oã¥
			gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st3_CW;
			CMPL_TMP_LEFT_ControlThread();
			break;
	 	case CMPL_TMP_SIZE_POS_st4_2_CW : // 50ms Timeout
			gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st2_CCW; // HomeOµ®ìÖ
			CMPL_TMP_LEFT_ControlThread();
			break;
		case CMPL_TMP_SIZE_POS_st7_CW : // 50ms Timeout
			gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st8; // Home®ì®¹ -> Size Positioning Start ..
			CMPL_TMP_LEFT_ControlThread();
			break;
		case CMPL_TMP_SIZE_POS_st8 : // 20ms Timeout  Oã¥
			gCMPL_TMP_LEFT.action++;
			CMPL_TMP_LEFT_ControlThread();
			break;

		// Normal Tamping
		case CMPL_TMP_NORMAL_TAMPING_st1:	// ®ìJnxÔÝè
		case CMPL_TMP_NORMAL_TAMPING_st2:	// [^ì®Ýè
		case CMPL_TMP_NORMAL_TAMPING_st4: 	// TampingÊuÖÚ®®¹
		case CMPL_TMP_NORMAL_TAMPING_st5: 	// ReturnûüÖØèÖ¦
		case CMPL_TMP_NORMAL_TAMPING_st7: 	// NormalTamping®¹
			gCMPL_TMP_LEFT.action++;
			CMPL_TMP_LEFT_ControlThread();
			break;

		// Eject Tamping
		case CMPL_TMP_EJECT_TAMPING_st1: 	// ®ìJnxÔÝè
		case CMPL_TMP_EJECT_TAMPING_st2: 	// [^ì®Ýè
		case CMPL_TMP_EJECT_TAMPING_st4: 	// TampingÊuÖÚ®®¹
		case CMPL_TMP_EJECT_TAMPING_st5: 	// ReturnûüÖØèÖ¦
		case CMPL_TMP_EJECT_TAMPING_st7: 	// Return®ì®¹
		case CMPL_TMP_EJECT_TAMPING_st8:  	// SetLastTampingJn
		case CMPL_TMP_EJECT_TAMPING_st10: 	// SetLast®ì®¹
			gCMPL_TMP_LEFT.action++;
			CMPL_TMP_LEFT_ControlThread();
			break;
			
		//MissREGi
		case CMPL_TMP_MISSREGI_st1: 	  // MissRegi  Oã¥(V3.5.94)
		case CMPL_TMP_MISSREGI_st3: 	 // MissRegi ãã¥
			gCMPL_TMP_LEFT.action++;
			CMPL_TMP_LEFT_ControlThread();
			break;

		// Offset
		case CMPL_TMP_OFFSET_st1: 			// JnTiming²®
		case CMPL_TMP_OFFSET_st2: 			// [^ì®Ýè
		case CMPL_TMP_OFFSET_st4: 			// ÀèÔ^C}Ýè
			gCMPL_TMP_LEFT.action++;
			CMPL_TMP_LEFT_ControlThread();
			break;

		// Return
		case CMPL_TMP_LEFT_RETURN_st2: 		// ^C}Ýè
		case CMPL_TMP_LEFT_RETURN_st3: 		// [^ì®Ýè
		case CMPL_TMP_LEFT_RETURN_st5: 		// ^C}Ýè
		case CMPL_TMP_LEFT_RETURN_st6: 		// (CR-008)
			gCMPL_TMP_LEFT.action++;
			CMPL_TMP_LEFT_ControlThread();
			break;

		// Open
		case CMPL_TMP_OPEN_st1: 			// [^ì®Ýè
		case CMPL_TMP_OPEN_st4: 			// ^C}Ýè
			gCMPL_TMP_LEFT.action++;
			CMPL_TMP_LEFT_ControlThread();
			break;
		
		default:
			break;
		}
		
	} else {  //Fail Event 
	
		switch ( gCMPL_TMP_LEFT.action ) {
		case CMPL_TMP_SIZE_POS_st3_CW : // Fail  Timeout
		case CMPL_TMP_SIZE_POS_st5_CW : // Fail  Timeout(V3.5.93Tampersï(FailÊm³êÈ¢))
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
// Return        : ]
///////////////////////////////////////////////////////////////////
void CMPL_TMP_LEFT_TimeUpControl2(SS aTimerID, UL aEvtType)
{
	if ( aTimerID != gCMPL_TMP_LEFT.timerIndex_TimeUpControl2) {
		return;
	}

	gCMPL_TMP_LEFT.timerIndex_TimeUpControl2= MSP_MAX_TMR_NUM;

	// d¬l=ã
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
//eNORMAL_EMS,		//NormalTampingJnÅEmsTimerÌTimeOut
//eNORMAL_HI_CURRENT ,	//RT­d¬`50msÌTimeout
//e1stEJECT_EMS,		//EjectTampingJnÅEmsTimerÌTimeOut
//e1stEJECT_HI_CURRENT, //RT­d¬`50msÌTimeout
//e2ndEJECT_EMS,		//SetLastTampingÌ14msTimerÌTimeout
//e2ndEJECT_HI_CURRENT,//RT­d¬`50msÌTimeout
//} ETMP_EVT_TYPE;
// Return        : ]
///////////////////////////////////////////////////////////////////
void CMPL_TMP_LEFT_TimeUpControl(SS aTimerID, UL aEvtType)
{
	if ( aTimerID != gCMPL_TMP_LEFT.timerIndex_TimeUpControl) {
		return;
	}

	gCMPL_TMP_LEFT.timerIndex_TimeUpControl= MSP_MAX_TMR_NUM;

	switch(aEvtType) {
		
	//NormalTampingJnÅEmsTimerÌTimeOut	
	case  	eNORMAL_EMS :   	
		// RT­d¬ , 50ms Start
		DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, HIGH);
		gCMPL_TMP_LEFT.timerIndex_TimeUpControl = MSP_SetEventTimer( 50, CMPL_TMP_LEFT_TimeUpControl, eNORMAL_HI_CUR);
		break;
	
	case		eNORMAL_HI_CUR : 	//RT­d¬`50msÌTimeout
		//RTãd¬
		DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, LOW);
		break;
		
	//EjectTampingJnÅEmsTimerÌTimeOut
	case 	e1stEJECT_EMS :		
		// RT­d¬ , 50ms Start
		DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, HIGH);
		gCMPL_TMP_LEFT.timerIndex_TimeUpControl = MSP_SetEventTimer( 50, CMPL_TMP_LEFT_TimeUpControl, e1stEJECT_HI_CUR);
		break;
	
	case		e1stEJECT_HI_CUR : //RT­d¬`50msÌTimeout
		//RTãd¬
		DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, LOW);
		break;
		
	//SetLastTampingÌ14msTimerÌTimeout
	case		e2ndEJECT_EMS :		
		// RT­d¬ , 50ms Start
		DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, HIGH);
		gCMPL_TMP_LEFT.timerIndex_TimeUpControl = MSP_SetEventTimer( 50, CMPL_TMP_LEFT_TimeUpControl, e2ndEJECT_HI_CUR);
		break;
	
	case 	e2ndEJECT_HI_CUR :  //RT­d¬`50msÌTimeout
		//RTãd¬
		DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, LOW);
		break;
	
	default :  break;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_LEFT_PreStartAct
// Description   : CR008- Normal TampingJnTimingÌTimeoutÈ~ÉóÔð½¹é½ßA
// Parameter     :ECMPL_TMP_Action ,  UC  aSheetNo
// Return        : ]
///////////////////////////////////////////////////////////////////
void  CMPL_TMP_LEFT_PreStartAct(ECMPL_TMP_Action aAction, UC aSheetNo)
{
	US   delay;
	gCMPL_TMP_LEFT.preSheetNo = aSheetNo;

	switch(aAction) {
		
	// m[}^sO®ìJnOTimerSet
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
// Return        : ]
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
// Return        : ]
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
	case CMPL_TMP_SIZE_POS_st3_CCW :  // Home Oµ®ì®¹ Event
		if ( DD_DIO_GetInput(DD_DI_LEFT_TAMPER_HOME_SNR) == HIGH ) {	// Tamper HomeSNR Off Fail
			gCMPL_TMP_LEFT_FaultStat = FAULT_OCCUR;
			CMPL_TMP_InformedLeftTamperHomeSensorOffFail(gCMPL_TMP_LEFT.sheetNo);
		} else {
			gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st4_CCW;
			CMPL_TMP_LEFT_ControlThread();
		}
		break;

	case CMPL_TMP_SIZE_POS_st3_CW : // Home®ì®¹ Event

		// è¬AONÝ
		gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st5_CW;
		// CMPL_TMP_LEFT_ControlThread();  // Nothing to do ..
		break;

	case CMPL_TMP_SIZE_POS_st4_CW : //StepDown®¹ Event
		gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st4_2_CW;
		CMPL_TMP_LEFT_ControlThread();
		break;

	case CMPL_TMP_SIZE_POS_st6_CW :
		gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st7_CW;
		CMPL_TMP_LEFT_ControlThread();
		break;

	case CMPL_TMP_SIZE_POS_st9 : // SizePositioning Pulse A  Ú®®¹
		gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st10;
		CMPL_TMP_LEFT_ControlThread();
		break;

	// Normal Tamping
	case CMPL_TMP_NORMAL_TAMPING_st3 : 	// [^ì®Jn
	case CMPL_TMP_NORMAL_TAMPING_st6 : 	// [^ì®(Return)Jn
		gCMPL_TMP_LEFT.action++;
		CMPL_TMP_LEFT_ControlThread();
		break;

	// Eject Tamping
	case CMPL_TMP_EJECT_TAMPING_st3 : 	// [^ì®Jn
	case CMPL_TMP_EJECT_TAMPING_st6 :  	// [^ì®(Return)Jn
	case CMPL_TMP_EJECT_TAMPING_st9 : 	// [^ì®(SetLast)Jn
		gCMPL_TMP_LEFT.action++;
		CMPL_TMP_LEFT_ControlThread();
		break;
	// MissREGI
 	case CMPL_TMP_MISSREGI_st2:      // MissRegi (V3.5.94)
		gCMPL_TMP_LEFT.action++;
		CMPL_TMP_LEFT_ControlThread();
		break;

	// Offset
	case CMPL_TMP_OFFSET_st3 : 			// [^ì®Jn
		gCMPL_TMP_LEFT.action++;
		CMPL_TMP_LEFT_ControlThread();
		break;

	// Return
	case CMPL_TMP_LEFT_RETURN_st4 : 	// [^ì®Jn
		gCMPL_TMP_LEFT.action++;
		CMPL_TMP_LEFT_ControlThread();
		break;

	// Open
	case CMPL_TMP_OPEN_st3 : 			// StepdownJn
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
	case CMPL_TMP_SIZE_POS_st3_CW :  // StepUpAÝóM
		CMPL_TMP_LEFT_CancelEventTimer();	// Fail Timer cancel ..
		gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st4_CW;
		CMPL_TMP_LEFT_ControlThread();
		break;
		
	case CMPL_TMP_SIZE_POS_st5_CW :  // ÀèæÔÅÝóM
		CMPL_TMP_LEFT_CancelEventTimer();	// Fail Timer cancel ..
		gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st6_CW;		
		CMPL_TMP_LEFT_ControlThread();
		break;

	case CMPL_TMP_OPEN_st2 :   		// ÝóM
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
// Description :  Eject ®ìJnÊm
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
// Description :  EjectMot¸¬JnÊm
// Parameter     :
///////////////////////////////////////////////////////////////////
void CMPL_TMP_LEFT_InformedEjectSheetThrough()
{
  	// Kv³µ..(RIGHTÌÝKv..)
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_LEFT_SetEventTimer
// Description   : Cxg^C}[Zbg
// Parameter     : aTime (^C}[l (~1[ms]))
// Return        : ]
///////////////////////////////////////////////////////////////////
void CMPL_TMP_LEFT_SetEventTimer(US aTime, UL aEventType)
{
	gCMPL_TMP_LEFT.timerIndex = MSP_SetEventTimer(aTime, CMPL_TMP_LEFT_ReceiveEventTimerTimeUp, aEventType);
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_LEFT_CancelEventTimer
// Description   : Cxg^C}[Zbg
// Parameter     : aTime (^C}[l (~1[ms]))
// Return        : ]
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
// Description   : Left^p[Ì®ìóÔðæ¾
// Parameter     : ]
// Return        : trueF®ìAfalseFâ~
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
// Description   : ^p[ Zbg
// Parameter     : ]
// Return        : ]
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
// Description   : ^p[â~
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void CMPL_TMP_LEFT_DeviceStop()
{
	ECMPL_TMP_Action aAction = gCMPL_TMP_LEFT.action & CMPL_TMP_ACTION_MASK;
	UC aSheetNo = gCMPL_TMP_LEFT.sheetNo;

	// Component ®ì
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
// Description   : TCY|WVÊuðæ¾
// Parameter     : aSheetNo (ÎÛV[gio[)
// Return        : TCY|WVÊu
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
// Description   : ^sOÊuæ¾
// Parameter     : aSheetNo (ÎÛV[gio[)
// Return        : ^sOÊu
///////////////////////////////////////////////////////////////////
US CMPL_TMP_LEFT_GetTampingPosition(ECMPL_TMP_Action aAction, UC aSheetNo)
{
	float aPosition=0;
	float aWidth = (float)SRVC_GetCrossProcessSize(aSheetNo);
	UC   aSetLast_N = 0;

	switch ( SRVC_GetStapleMode(aSheetNo) ) {

	case eCdiFinStaple_Off :
		aPosition = 85 + SRVC_GetNvmValueSC(NVMID_TAMPER_UN_STPLE_ADJST); //NVM1;

		// §äÎÛpªARpCTamperpµ¦µ±ÝÊÇÁÌ²®èlÚÈã?
		if( SRVC_GetCompileCount(aSheetNo) >= SRVC_GetNvmValueUC(NVMID_TAMPING_ADD_PUSH_PAPER_COUNT))
			aPosition += SRVC_GetNvmValueSC(NVMID_TAMPING_ADD_PUSH_PULSE); //NVM3;
		break;

	case eCdiFinStaple_SingleFront :
		aPosition = ((1460.0 - (aWidth / 2.0) )/ CMPL_TMP_COMM_MOT_STEP) +5;
		aPosition /= 10;
		aPosition =  aPosition + OSIKOMI_PLS_A+ZURE_PLS_B + SRVC_GetNvmValueSC(NVMID_TAMPER_R_STPLE_ADJST);//NVM2;

		// §äÎÛpªARpCTamperpµ¦µ±ÝÊÇÁÌ²®èlÚÈã?
		if ( SRVC_GetCompileCount(aSheetNo) >= SRVC_GetNvmValueUC(NVMID_TAMPING_ADD_PUSH_PAPER_COUNT) )
			aPosition += SRVC_GetNvmValueSC(NVMID_TAMPING_ADD_PUSH_PULSE); //NVM3;

		//(CR-XXX)150716 : B5@V[g¸êÎô(StapleJob+ EjectTamping)
		if ( aAction ==  CMPL_TMP_EJECT_TAMPING ) {

			//aPosition += SRVC_GetNvmValueSC(NVMID_TAMPING_ADD_PUSH_PULSE_2); //NVM4;
			// (V3.5.102) 
			aSetLast_N = SRVC_GetNvmValueUC(NVMID_TAMPING_SETLAST_ADD_PULSE_ADJ_N);  //NVM4 
			
			if ( SRVC_GetCompileCount(aSheetNo) >= aSetLast_N ) {
				//StapleÌZbgXg^sOÌ²® N ÈãATamperpµ¦µÝÊ
				aPosition += SRVC_GetNvmValueSC(NVMID_TAMPING_SETLAST_ADD_PULSE_5); //NVM5 				
			}else {
				//StapleÌZbgXg^sOÌ²® N ¢ATamperpµ¦µÝÊ
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
// Description   : Normal Tamping ®ìÔ
// Parameter     : aSheetNo
// Return        : aTime (®ìÔ)
///////////////////////////////////////////////////////////////////
US CMPL_TMP_LEFT_GetNormalTampingTime(UC aSheetNo)
{
	ECMPL_TMP_ProfileID aProfile;

	SS			 aMovePulse; //Ú®Ê
	SS			 aNvm; //NVM
	SL			 aParaSL1,  aParaSL2;  //ÔvZp
	
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
			//993[pps] ®ìÔ= ( (D{NVM-26)~1.007+63.8)+( (D-60)~0.6798+94)	
			//	(D-60)ÌðªÆÈéêA(D-60)Ìðð0Æ·éB
			//	Ü½A(D-26)ÌðªÆÈéêà¯lÉA(D-26)Ìðð0Æ·éB
			aTime = (US)((((aParaSL1 * 10.07 + 638) +(aParaSL2 * 6.798 + 940)) +9)/10);
		break;

		case CMPL_TMP_PRF_ID_NORMAL_HI:			
			aParaSL1  = (SL) (aMovePulse + aNvm - 60) ;
			if ( aParaSL1 < 0) {
				aParaSL1 = 0;
			}	
			//1471[pps] ®ìÔ= ( (D{NVM-60)~0.6798+94)~2
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
// Description   : Eject Tamping ®ìÔ
// Parameter     : aSheetNo
// Return        : aTime (®ìÔ)
///////////////////////////////////////////////////////////////////
US CMPL_TMP_LEFT_GetEjectTampingTime(UC aSheetNo)
{

	ECMPL_TMP_ProfileID aProfile;

	SS			 aMovePulse; //Ú®Ê
	SS			 aNvm; //NVM
	SS  			aNvmCount;
	SL			 aParaSL1,  aParaSL2;  //ÔvZp
	
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
			
			//993[pps] ®ìÔ= ( (D{NVM-26)~1.007+73.8)+109.6~µ¦®ìJèÔµñ
			aTime = (US)((((aParaSL1 * 10.07 + 738) + aParaSL2) +9)/10) ;
		break;

		case CMPL_TMP_PRF_ID_NORMAL_HI:			
			aParaSL1  = (SL) (aMovePulse + aNvm - 60) ;
			if ( aParaSL1 < 0) {
				aParaSL1 = 0;
			}
			aParaSL2 = (SL)(1096 * aNvmCount);
			//1471[pps] ®ìÔ=  ( (D{NVM-60)~0.6798+104)+ 109.6~µ¦®ìJèÔµñ
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
// Description   : OffsetÊuæ¾
// Parameter     : aSheetNo (ÎÛV[gio[)
// Return        : Offset Êu
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
// Description   : Offsetûü æ¾
// Parameter     : aSheetNo (ÎÛV[gio[)
// Return        : Offset ûü
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
// Description   : ReturnÚ®Ê æ¾
// Parameter     : aSheetNo (ÎÛV[gio[)
// Return        : Return Ú®Ê
///////////////////////////////////////////////////////////////////
US CMPL_TMP_LEFT_GetReturnPosition(UC aSheetNo)
{
	float  aPosition=0;
	float aWidth = (float)(SRVC_GetCrossProcessSize(aSheetNo));

	switch ( SRVC_GetStapleMode(aSheetNo) ) {

 //CR-010
	case eCdiFinStaple_Off :		
	case eCdiFinStaple_SingleFront :	
		// 10 pulse Ú®
		  aPosition = 10;
		break;
	default:																	// Irregular Case
		break;
	}

	return  (US)aPosition;
}

///////////////////////////////////////////////////////////////////
// Function Name : CMPL_TMP_LEFT_GetEmsForCurrentChange :( V3.0.4 ) 
// Description   : pµ¦yÑSetLastTampingÌRTd¬lØèÖ¦ÔFSlowUp{è¬Ô
// Parameter     : ECMPL_TMP_Action   aAction
//			  UC 				 aSheetNo (ÎÛV[gio[)
// Return        : Ú®Ô(Ò¿Ô) 
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
// Description   : pµ¦yÑSetLastTampingÌLTd¬lØèÖ¦ÔFSlowUp{è¬Ô
// Parameter     : ECMPL_TMP_Action   aAction
//			  UC 				 aSheetNo (ÎÛV[gio[)
// Return        : Ú®Ô(Ò¿Ô) 
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
	case 	CMPL_TMP_PRF_ID_NORMAL_MID : //297.0mm´¦
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

	case 	CMPL_TMP_PRF_ID_NORMAL_HI : //297.0mmÈº
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
// Description   : Mot ì®Ýè
// Parameter     : ]
// Return        : ]
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
// Description   : ®ìJn, 100Pulse ã, StepDown
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_Component_Mot_st2()
{
	DD_STM_SetProfileID(DD_STM_ID_LEFT_TAMPER,  gCMPL_TMP_LEFT.profile);
	DD_STM_MoveBySetPulse(DD_STM_ID_LEFT_TAMPER,  gCMPL_TMP_LEFT.targetIndex,
		                     CMPL_TMP_COMPO_TOTAL_PULSE, CMPL_TMP_LEFT_ReceiveSteppingMotorEvent);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Component_Mot_st3
// Description	 : ãã¥
// Parameter	 : ]
// Return		 : ]
///////////////////////////////////////////////////////////////////
void LT_Component_Mot_st3()
{
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_MOTOR_STOP_TIME_20, CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Component_Mot_st4
// Description   : Mot Off
// Parameter     : ]
// Return        : ]
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


// TCY|WVjO®ì
// CMPL_TMP_SIZE_POS = 0x0200,
///////////////////////////////////////////////////////////////////
// Function Name : LT_SizePositioning_st1
// Description   : Tamper HomeSNR@ON`FbN
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_SizePositioning_st1()
{
	// 1. Tamper Home SNR Level Check
	if ( DD_DIO_GetInput(DD_DI_LEFT_TAMPER_HOME_SNR) == HIGH )  // HIGH=
	   gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st2_CCW;   	// HomeOµ®ìÝè
	else
	   gCMPL_TMP_LEFT.action = CMPL_TMP_SIZE_POS_st2_CW;  		// Home®ìÝè

	CMPL_TMP_LEFT_ControlThread();
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_SizePositioning_st2_CCW
// Description   : HomeOµ®ìÝè
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_SizePositioning_st2_CCW()
{
	// HomeOµ®ì`FbNCountB2ñÈãÈçÎHomeSNROffFail
	if ( ++gCMPL_TMP_LEFT.offFailCheckCount > 1 ) {
		
		CMPL_TMP_InformedLeftTamperHomeSensorOffFail(gCMPL_TMP_LEFT.sheetNo);
		PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st2_CCW+0x80,  
							gCMPL_TMP_LEFT.offFailCheckCount, gCMPL_TMP_LEFT.offFailCheckCount);
	}

	// àûü(CCWjÅ­d¬Ýè
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_DIR, CMPL_TMP_COMM_INSIDE);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, HIGH);

	//LeftSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st2_CCW+0x80, CMPL_TMP_SIZE_POS_st2_CCW+0x80, 0);
	// 20ms TimerÝè
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_MOTOR_ENABLE_TIME_20,
									CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_SizePositioning_st3_CCW
// Description   : CCWûüÉ[^ì®Jn
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_SizePositioning_st3_CCW()
{
     DD_STM_SetProfileID(DD_STM_ID_LEFT_TAMPER, DD_STM_PROF_ID_TAMPER_INITIALIZE);
	//LeftSizePos
     PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st3_CCW+0x80, CMPL_TMP_SIZE_POS_st3_CCW+0x80, 0);

	// INITIALIZE ProfileÅ 120(StepUp+Const) + 30(StepDown)pXÚ®
     DD_STM_MoveBySetPulse(DD_STM_ID_LEFT_TAMPER,  CMPL_TMP_PROF_TARGET_STEP_NUM_INIT,
     						   CMPL_TMP_SIZE_POSITION_MOVE_PUSE_150, CMPL_TMP_LEFT_ReceiveSteppingMotorEvent );
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_SizePositioning_st4_CCW
// Description   : HomeOµ®ì®¹
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_SizePositioning_st4_CCW()
{
	// 50ms TimerÝè
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_SWITCH_TIME_50, CMPL_TMP_LEFT_NORMAL_EVT);
	//LeftSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st4_CCW+0x80, CMPL_TMP_SIZE_POS_st4_CCW+0x80, 0);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_SizePositioning_st2_CW
// Description   : Home®ìÝè
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_SizePositioning_st2_CW()
{
	// Oûü(CWjÅ­d¬Ýè
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_DIR, CMPL_TMP_COMM_OUTSIDE);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, HIGH);

	//V3.3.99  Tamper T/B
	if ( gCMPL_TMP_LEFT.sizePosAfterReturn == true ) {
		// 20ms TimerÝè 
		CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_MOTOR_ENABLE_TIME_20,
										CMPL_TMP_LEFT_NORMAL_EVT);

	} else {
		//V3.3.93  ¹Îô@20ms¨200msÉÏX
		CMPL_TMP_LEFT_SetEventTimer(200, 	CMPL_TMP_LEFT_NORMAL_EVT);

	}
	//LeftSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st2_CW+0x80, CMPL_TMP_SIZE_POS_st2_CW+0x80, 0);
		
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_SizePositioning_st3_CW
// Description   : CWûüÉ[^ì®Jn
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_SizePositioning_st3_CW()
{
  // Fail TimerÝè
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
// Description   : StepUpAÝóM->SlowDown-> 50ms Timerset->HomeOµ®ì
// Parameter     : ]
// Return        : ]
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
// Description   : StepUpAÝóM ©ç StepDown®¹
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_SizePositioning_st4_2_CW() // StepUpAÝóM ©ç StepDown®¹
{
	// 50ms TimerÝè
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_SWITCH_TIME_50, CMPL_TMP_LEFT_NORMAL_EVT);
	//LeftSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st4_2_CW+0x80, CMPL_TMP_SIZE_POS_st4_2_CW+0x80, 0);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_SizePositioning_st5_CW
// Description   : //StepUp®¹óM
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_SizePositioning_st5_CW()
{
	// Nothing to do... No call .
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_SizePositioning_st6_CW
// Description   :  //ÀèæÔÅÝóM
// Parameter     : ]
// Return        : ]
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
// Description   ://Home®ì®¹
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_SizePositioning_st7_CW()
{
	// 50ms TimerÝè
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_SWITCH_TIME_50, CMPL_TMP_LEFT_NORMAL_EVT);
	//LeftSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st7_CW+0x80, CMPL_TMP_SIZE_POS_st7_CW+0x80, 0);

	gCMPL_TMP_LEFT.offFailCheckCount = 0;  //HomeOµ®ì`FbNCount Clear(V3.5.95)
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_SizePositioning_st8
// Description   :  //SizePositioningJn
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_SizePositioning_st8()
{
	UC    sheetNo = gCMPL_TMP_LEFT.sheetNo;
		
	if ( gCMPL_TMP_LEFT.movePulse == 0 ) {// @Ú®Ê³µ¨I¹

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

	//@Ú®Ê Lè
	// DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_DIR, gCMPL_TMP_LEFT.direction);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_DIR, CMPL_TMP_COMM_INSIDE);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, HIGH);

	//V3.4.0
	if ( gCMPL_TMP_LEFT.sizePosAfterReturn == true ) {
		// 20ms TimerÝè
		CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_MOTOR_ENABLE_TIME_20, CMPL_TMP_LEFT_NORMAL_EVT);

	} else {
		//V3.3.93  ¹Îô@20ms¨200msÉÏX
		CMPL_TMP_LEFT_SetEventTimer(200,	CMPL_TMP_LEFT_NORMAL_EVT);

	}	
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_SizePositioning_st9
// Description   : [^ì®Jn
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_SizePositioning_st9()
{
	// A-PulseÚ®
	DD_STM_MoveBySetPulse(DD_STM_ID_LEFT_TAMPER, CMPL_TMP_PROF_TARGET_STEP_NUM_INIT,
	                         gCMPL_TMP_LEFT.movePulse, CMPL_TMP_LEFT_ReceiveSteppingMotorEvent );
	//LeftSizePos
	PrintTamperLog_Short(CMPL_TMP_SIZE_POS_st9+0x80, CMPL_TMP_SIZE_POS_st9+0x80, 0x80);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_SizePositioning_st10
// Description   : SizePositionÖÚ®®¹
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_SizePositioning_st10()
{
	UC     sheetNo = gCMPL_TMP_LEFT.sheetNo;
	
	// LeftTamper HomeSNR OFF Fail Check (33 Pulse @Èã & HOME SNR=ON)
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


// m[}^sO®ì
// CMPL_TMP_NORMAL_TAMPING = 0x0300,
///////////////////////////////////////////////////////////////////
// Function Name : LT_NormalTamping_st1
// Description   : ®ìJnxÔÝè
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_NormalTamping_st1()
{
	 //CR-008 
	gCMPL_TMP_LEFT.action = CMPL_TMP_NORMAL_TAMPING_st2;
	CMPL_TMP_LEFT_ControlThread();

}

///////////////////////////////////////////////////////////////////
// Function Name : LT_NormalTamping_st2
// Description   : [^ì®Ýè
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_NormalTamping_st2()
{
	UC    sheetNo = gCMPL_TMP_LEFT.sheetNo;
	// Ú®Ê=0ÌêAI¹
	if ( gCMPL_TMP_LEFT.movePulse == 0 ) { //

		gCMPL_TMP_LEFT.action = CMPL_TMP_st0;
		gCMPL_TMP_LEFT.sheetNo = SHEETNO_NUM;
		CMPL_TMP_InformedLeftTamperComplete(CMPL_TMP_NORMAL_TAMPING, sheetNo);
		return;
	}
	// 1. ­d¬ÅàiCCW)ûüÝè
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_DIR, gCMPL_TMP_LEFT.direction);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, HIGH);

	// 2. 20msTimerSet
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_MOTOR_ENABLE_TIME_20, 
	 								 CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_NormalTamping_st3
// Description   : [^ì®Jn
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_NormalTamping_st3()
{
//  (V3.0.4)@@SlowUpµÄ©çEmsÈ~A50msÜÅ­d¬É·é
	US   E_ms =  CMPL_TMP_LEFT_GetEmsForCurrentChange(CMPL_TMP_NORMAL_TAMPING, gCMPL_TMP_LEFT.sheetNo);

// (V3.5.103) LeftTamper d¬lØèÖ¦Ô
	US   Nt = CMPL_TMP_LEFT_GetLTCurrentChange(CMPL_TMP_NORMAL_TAMPING,  gCMPL_TMP_LEFT.sheetNo);

	// time, callback, evtType
	gCMPL_TMP_LEFT.timerIndex_TimeUpControl  =MSP_SetEventTimer(E_ms, CMPL_TMP_LEFT_TimeUpControl, eNORMAL_EMS);
	gCMPL_TMP_LEFT.timerIndex_TimeUpControl2  =MSP_SetEventTimer(Nt, CMPL_TMP_LEFT_TimeUpControl2, eNORMAL_EMS);
//@ªªªªªª
	DD_STM_SetProfileID(DD_STM_ID_LEFT_TAMPER, gCMPL_TMP_LEFT.profile);
 	DD_STM_MoveBySetPulse(DD_STM_ID_LEFT_TAMPER, gCMPL_TMP_LEFT.targetIndex,
							gCMPL_TMP_LEFT.movePulse, CMPL_TMP_LEFT_ReceiveSteppingMotorEvent);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_NormalTamping_st4
// Description   : TampingÊuÖÚ®®¹
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_NormalTamping_st4()
{
	// 10msTimerSet
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_SWITCH_TIME_10, 
	                                 CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_NormalTamping_st5
// Description   : ReturnûüÖØèÖ¦
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_NormalTamping_st5()
{
	// ì®ûü=OÉÏX
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_DIR, CMPL_TMP_COMM_OUTSIDE);

	//V3.5.103 LT d¬lØèÖ¦ LOW->HIGH
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, HIGH);
	
	// 10msTimerSet
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_SWITCH_TIME_10,
					 			     CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_NormalTamping_st6
// Description   : [^ì®(Return)Jn
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_NormalTamping_st6()
{
	// ¬Profile
	DD_STM_SetProfileID(DD_STM_ID_LEFT_TAMPER,  DD_STM_PROF_ID_TAMPER_NORMAL_HI);

	DD_STM_MoveBySetPulse(DD_STM_ID_LEFT_TAMPER, gCMPL_TMP_LEFT.targetIndex,
							 gCMPL_TMP_LEFT.movePulse, CMPL_TMP_LEFT_ReceiveSteppingMotorEvent);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_NormalTamping_st7
// Description   : NormalTamping®¹
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_NormalTamping_st7()
{
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_MOTOR_STOP_TIME_20,
				 					CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_NormalTamping_st8
// Description   : Ì®ìÒ¿Ýè
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_NormalTamping_st8()
{
	UC    sheetNo = gCMPL_TMP_LEFT.sheetNo;
	
	// ãd¬Ýè
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, LOW);
	DD_DIO_SetOutput(DD_DO_RIGHT_TAMPER_MOT_CUR, LOW);  //OÌ½ßAÇÁ.
	
	gCMPL_TMP_LEFT.action = CMPL_TMP_st0;
	gCMPL_TMP_LEFT.sheetNo = SHEETNO_NUM;

	CMPL_TMP_InformedLeftTamperComplete(CMPL_TMP_NORMAL_TAMPING,  sheetNo);
	PrintTamperLog_Short(CMPL_TMP_NORMAL_TAMPING_st8, CMPL_TMP_NORMAL_TAMPING_st8, 0);

}



// CWFNg^sO®ì(SetLastTamping)
// CMPL_TMP_EJECT_TAMPING = 0x0400,
///////////////////////////////////////////////////////////////////
// Function Name : LT_EjectTamping_st1
// Description   : ®ìJnxÔÝè
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_EjectTamping_st1()
{
	CMPL_TMP_LEFT_SetEventTimer(gCMPL_TMP_LEFT.startDelayTime,
						 			 CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_EjectTamping_st2
// Description   : [^ì®Ýè
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_EjectTamping_st2()
{
	UC	sheetNo = gCMPL_TMP_LEFT.sheetNo;
	
	// Ú®Ê=0ÌêAI¹
	if ( gCMPL_TMP_LEFT.movePulse == 0 ) {  //

		gCMPL_TMP_LEFT.action = CMPL_TMP_st0;
		gCMPL_TMP_LEFT.sheetNo = SHEETNO_NUM;
	
		CMPL_TMP_InformedLeftTamperComplete(CMPL_TMP_EJECT_TAMPING,  sheetNo);
		return;
	}
	// 1. ­d¬ÅàiCCW)ûüÝè
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_DIR, gCMPL_TMP_LEFT.direction);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, HIGH);

	// 2. 20msTimerSet
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_MOTOR_ENABLE_TIME_20,
					 			     CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_EjectTamping_st3
// Description   : [^ì®Jn
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_EjectTamping_st3()
{
//	(V3.0.4)@@SlowUpµÄ©çEmsÈ~A50msÜÅ­d¬É·é
	US	 E_ms =  CMPL_TMP_LEFT_GetEmsForCurrentChange(CMPL_TMP_EJECT_TAMPING, gCMPL_TMP_LEFT.sheetNo);
// (V3.5.103) LeftTamper d¬lØèÖ¦Ô
	US	 Nt = CMPL_TMP_LEFT_GetLTCurrentChange(CMPL_TMP_EJECT_TAMPING,  gCMPL_TMP_LEFT.sheetNo);

	// time, callback, evtType
	gCMPL_TMP_LEFT.timerIndex_TimeUpControl = MSP_SetEventTimer(E_ms, CMPL_TMP_LEFT_TimeUpControl, e1stEJECT_EMS);
	gCMPL_TMP_LEFT.timerIndex_TimeUpControl2 = MSP_SetEventTimer(Nt, CMPL_TMP_LEFT_TimeUpControl2, e1stEJECT_EMS);	
//@ªªªªªª

	DD_STM_SetProfileID(DD_STM_ID_LEFT_TAMPER, gCMPL_TMP_LEFT.profile);
	DD_STM_MoveBySetPulse(DD_STM_ID_LEFT_TAMPER, gCMPL_TMP_LEFT.targetIndex,
							  gCMPL_TMP_LEFT.movePulse,  CMPL_TMP_LEFT_ReceiveSteppingMotorEvent);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_EjectTamping_st4
// Description   : TampingÊuÖÚ®®¹
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_EjectTamping_st4()
{
	// 10msTimerSet
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_SWITCH_TIME_10,
					 			     CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_EjectTamping_st5
// Description   : ReturnûüÖØèÖ¦
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_EjectTamping_st5()
{
	// ì®ûü=OÉÏX
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_DIR, CMPL_TMP_COMM_OUTSIDE);

	//V3.5.103 LT d¬lØèÖ¦ LOW->HIGH
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, HIGH);
	
	// 10msTimerSet
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_SWITCH_TIME_10,
					 			     CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_EjectTamping_st6
// Description   : [^ì®(Return)Jn
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_EjectTamping_st6()
{
	//¬ProfileÅ27PulseðOûüÉÚ®
	DD_STM_SetProfileID(DD_STM_ID_LEFT_TAMPER, DD_STM_PROF_ID_TAMPER_NORMAL_MID);
	gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_MID;
	
	DD_STM_MoveBySetPulse(DD_STM_ID_LEFT_TAMPER, gCMPL_TMP_LEFT.targetIndex,
							  27, CMPL_TMP_LEFT_ReceiveSteppingMotorEvent);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_EjectTamping_st7
// Description   : Return®ì®¹
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_EjectTamping_st7()
{
	// 10msTimerSet
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_SWITCH_TIME_10,
					 			     CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_EjectTamping_st8
// Description   : SetLastTampingJn
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_EjectTamping_st8()
{
	// ì®ûü= àÉÏX
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_DIR, CMPL_TMP_COMM_INSIDE);
	// 10msTimerSet
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_SWITCH_TIME_10,
					 			     CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_EjectTamping_st9
// Description   : [^ì®(SetLast)Jn
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void  LT_EjectTamping_st9()
{
//	(V3.0.4)@SlowUpµÄ©çEmsÈ~A50msÜÅ­d¬É·é
	US	 E_ms = 14;  //14ms

	// time, callback, evtType
	gCMPL_TMP_LEFT.timerIndex_TimeUpControl = MSP_SetEventTimer(E_ms, CMPL_TMP_LEFT_TimeUpControl, e2ndEJECT_EMS);
//@ªªªªªª

	// ¬ProfileÅ27PulseðOûüÉÚ®
	DD_STM_SetProfileID(DD_STM_ID_LEFT_TAMPER, DD_STM_PROF_ID_TAMPER_NORMAL_MID);
	gCMPL_TMP_LEFT.targetIndex = CMPL_TMP_PROF_TARGET_STEP_NUM_MID;

	DD_STM_MoveBySetPulse(DD_STM_ID_LEFT_TAMPER, gCMPL_TMP_LEFT.targetIndex,
							  27, CMPL_TMP_LEFT_ReceiveSteppingMotorEvent);

}

///////////////////////////////////////////////////////////////////
// Function Name : LT_EjectTamping_st10
// Description   : SetLast®ì®¹
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void  LT_EjectTamping_st10()
{
	// 10msTimerSet
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_SWITCH_TIME_10,
					 			     CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_EjectTamping_st11
// Description   : SetLastJèÔµ`FbN
// Parameter     : ]
// Return        : ]
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
// Description   : Ì®ìÒ¿Ýè
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void  LT_EjectTamping_st12()
{
	UC   sheetNo =  gCMPL_TMP_LEFT.sheetNo;
	// ãd¬Ýè
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, LOW);

	gCMPL_TMP_LEFT.action = CMPL_TMP_st0;
	gCMPL_TMP_LEFT.sheetNo = SHEETNO_NUM;
	gCMPL_TMP_LEFT.ejectTampingCnt = 0;

	CMPL_TMP_InformedLeftTamperComplete(CMPL_TMP_EJECT_TAMPING, sheetNo);
	PrintTamperLog_Short(CMPL_TMP_EJECT_TAMPING_st12, 0xFFFF, 0xFF);
}

//V3.5.93 ~XWÎô
///////////////////////////////////////////////////////////////////
// Function Name : LT_MissRegi_st1
// Description  // MissRegi_®ìTimerÝè 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
 void  LT_MissRegi_st1()
{
	US  timer =0;	

	gCMPL_TMP_LEFT.movePulse= (US)(SRVC_GetNvmValueUC(NVMID_TAMPER_MISSREGI_MOT_PULSE));

	if ( gCMPL_TMP_LEFT.movePulse < 1) {
		
		gCMPL_TMP_LEFT.action = CMPL_TMP_MISSREGI_st4; //MissRegi_®¹
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
			// ì®ûü=O
			DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_DIR, CMPL_TMP_COMM_OUTSIDE);
			// ­d¬
			DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, HIGH); 
			
			CMPL_TMP_LEFT_SetEventTimer(timer,  CMPL_TMP_LEFT_NORMAL_EVT);
			
			PrintTamperLog_Short(CMPL_TMP_MISSREGI_st1,  timer, 0xFF);
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_MissRegi_st2
// Description   :  MissRegi_wèpXoÍ 
// Parameter     : ]
// Return        : ]
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
// Description   : // MissRegi_ãã¥Ýè 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void  LT_MissRegi_st3()
{
	//20ms 
	CMPL_TMP_LEFT_SetEventTimer(20,  CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_MissRegi_st4
// Description   : // MissRegi_®¹
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void  LT_MissRegi_st4()
{
	UC   sheetNo =  gCMPL_TMP_LEFT.sheetNo;
	// ãd¬Ýè
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, LOW);

	gCMPL_TMP_LEFT.action = CMPL_TMP_st0;
	gCMPL_TMP_LEFT.sheetNo = SHEETNO_NUM;
	
	CMPL_TMP_InformedLeftTamperComplete(CMPL_TMP_MISSREGI, sheetNo);
	PrintTamperLog_Short(CMPL_TMP_MISSREGI_st4, CMPL_TMP_MISSREGI_st4, 0xFF);

}


// ItZbg®ì
// CMPL_TMP_OFFSET = 0x0500,
///////////////////////////////////////////////////////////////////
// Function Name : LT_Offset_st1
// Description   : JnTiming²®
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_Offset_st1()
{
	UC   sheetNo = gCMPL_TMP_LEFT.sheetNo;
	
	// Ú®Ê=0ÌêAI¹
	if ( gCMPL_TMP_LEFT.movePulse == 0 ) { //

		gCMPL_TMP_LEFT.action = CMPL_TMP_st0;
		gCMPL_TMP_LEFT.sheetNo = SHEETNO_NUM;
	
		CMPL_TMP_InformedLeftTamperComplete(CMPL_TMP_OFFSET, sheetNo);
		return;
	}

	// Tamperì®ûüÉæÁÄ§äðÏ¦é
	if ( gCMPL_TMP_LEFT.direction == CMPL_TMP_COMM_INSIDE ) {  // àiCCW)ûü
		// 20msTimerSet
		CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_START_DELAY_20, CMPL_TMP_LEFT_NORMAL_EVT);
	} else {   // O(CW)ûü
		gCMPL_TMP_LEFT.action = CMPL_TMP_OFFSET_st2;
		CMPL_TMP_LEFT_ControlThread();
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Offset_st2
// Description   : [^ì®Ýè
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_Offset_st2()
{
	// 1. ­d¬Å OFFSETûüÝè
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_DIR, gCMPL_TMP_LEFT.direction);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, HIGH);

	// 2. 10msTimerSet
	CMPL_TMP_LEFT_SetEventTimer( CMPL_TMP_COMM_SWITCH_TIME_10, CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Offset_st3
// Description   : [^ì®Jn
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_Offset_st3()
{
	// á¬Profile
	DD_STM_SetProfileID(DD_STM_ID_LEFT_TAMPER, gCMPL_TMP_LEFT.profile);
	DD_STM_MoveBySetPulse(DD_STM_ID_LEFT_TAMPER, gCMPL_TMP_LEFT.targetIndex,
							  gCMPL_TMP_LEFT.movePulse, CMPL_TMP_LEFT_ReceiveSteppingMotorEvent);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Offset_st4
// Description	 : ÀèÔ^C}Ýè
// Parameter	 : ]
// Return		 : ]
///////////////////////////////////////////////////////////////////
void LT_Offset_st4()
{
	// Tamperì®ûüÉæÁÄ§äðÏ¦é
	if ( gCMPL_TMP_LEFT.direction == CMPL_TMP_COMM_INSIDE) {  // àiCCW)ûü
		// 10msTimerSet
		CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_MOTOR_STOP_TIME_10,
										 CMPL_TMP_LEFT_NORMAL_EVT);
	} else {   //O(CW)ûü
		// 20msTimerSet
		CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_MOTOR_STOP_TIME_20,
										 CMPL_TMP_LEFT_NORMAL_EVT);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Offset_st5
// Description   : ®ìÒ¿
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_Offset_st5()
{
	UC   sheetNo =  gCMPL_TMP_LEFT.sheetNo;
	// ãd¬Ýè
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, LOW);

	gCMPL_TMP_LEFT.action = CMPL_TMP_st0;
	gCMPL_TMP_LEFT.sheetNo = SHEETNO_NUM;

	CMPL_TMP_InformedLeftTamperComplete(CMPL_TMP_OFFSET, sheetNo);
	PrintTamperLog_Short(CMPL_TMP_OFFSET_st5+0x80, CMPL_TMP_OFFSET_st5+0x80, 0xFF);
}

// ^[®ì
// CMPL_TMP_LEFT_RETURN = 0x0600,
///////////////////////////////////////////////////////////////////
// Function Name : LT_Return_st1
// Description   : Eject ®ìJnTriggerÒ¿
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_Return_st1()
{
	UC   sheetNo =  gCMPL_TMP_LEFT.sheetNo;

	// Ú®Ê=0ÌêAI¹
	if ( gCMPL_TMP_LEFT.movePulse == 0 ) {  //

		gCMPL_TMP_LEFT.action = CMPL_TMP_st0;
		gCMPL_TMP_LEFT.sheetNo = SHEETNO_NUM;
		gCMPL_TMP_LEFT.sizePosAfterReturn = false;
		CMPL_TMP_InformedLeftTamperComplete(CMPL_TMP_LEFT_RETURN, sheetNo);
		return;
	}

	if ( CMPL_TMP_IsEjectStart() == true ) { // Eject ®ìJn
		gCMPL_TMP_LEFT.action = CMPL_TMP_LEFT_RETURN_st2;
		CMPL_TMP_LEFT_ControlThread();
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Return_st2
// Description   : ^C}Ýè
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_Return_st2()
{
#if  1 //(CR-008) 
	gCMPL_TMP_LEFT.action = CMPL_TMP_LEFT_RETURN_st3;
	CMPL_TMP_LEFT_ControlThread();
#else
	// LeftTamperÍOûüÌÝ - 
	CMPL_TMP_LEFT_SetEventTimer(20+(SRVC_GetNvmValueUS(NVMID_TAMPER_RETURN_OUT_START_TIME)*10),
					 			     CMPL_TMP_LEFT_NORMAL_EVT);
#endif
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Return_st3
// Description   : [^ì®Ýè
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_Return_st3()
{
	// 1.. ãd¬ÅàiCCW)ûüÝè
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_DIR, gCMPL_TMP_LEFT.direction);
	//MotorE²Îô(CR-008) TimerSetOA­d¬
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, HIGH); //LOW);

	// 2. 20msTimerSet
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_MOTOR_ENABLE_TIME_20,
					 			     CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Return_st4
// Description   ; [^ì®Jn
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_Return_st4()
{
	// 1. ­d¬
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, HIGH);
	DD_STM_SetProfileID(DD_STM_ID_LEFT_TAMPER, gCMPL_TMP_LEFT.profile);
	DD_STM_MoveBySetPulse(DD_STM_ID_LEFT_TAMPER, gCMPL_TMP_LEFT.targetIndex,
	                    	  gCMPL_TMP_LEFT.movePulse, CMPL_TMP_LEFT_ReceiveSteppingMotorEvent);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Return_st5
// Description   : ^C}Ýè
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_Return_st5()
{
	// 20msTimerSet
	CMPL_TMP_LEFT_SetEventTimer( CMPL_TMP_COMM_MOTOR_STOP_TIME_20, CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Return_st6
// Description   : ®ìÒ¿
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_Return_st6()
{
	UC    sheetNo = gCMPL_TMP_LEFT.sheetNo;	
	US   nvm_time;
	
	//ãd¬Ýè
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, LOW);
	
//CR-008
	if ( gCMPL_TMP_LEFT.sizePosAfterReturn == true) {  //SizePosJn 
		
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
// Description	 :	SizePosJn(CR-008) 
// Parameter	 : ]
// Return		 : ]
///////////////////////////////////////////////////////////////////
void LT_Return_st7()
{

	// LTÍ·×ÄÌP[XÅSizePositioningÀ{·é - FlagCheck µÈ¢
	CMPL_TMP_LEFT_StartAct(CMPL_TMP_SIZE_POS, gCMPL_TMP_LEFT.sheetNo);
		
}

// I[v®ì(Nigasi)
// CMPL_TMP_OPEN = 0x0700,
///////////////////////////////////////////////////////////////////
// Function Name : LT_Open_st1
// Description   : [^ì®Ýè
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_Open_st1()
{
	UC  sheetNo =  gCMPL_TMP_LEFT.sheetNo;
	
	// ¦ªµ®ìJn`FbN
	if ( (DD_DIO_GetInput(DD_DI_LEFT_TAMPER_HOME_SNR) == HIGH) ||
	      (SRVC_GetCrossProcessSizeMixSet(gCMPL_TMP_LEFT.sheetNo) != SHT_SIZE_MIX) ) { 	// Mix Size

		 gCMPL_TMP_LEFT.action = CMPL_TMP_st0;
		gCMPL_TMP_LEFT.sheetNo = SHEETNO_NUM;
		CMPL_TMP_InformedLeftTamperComplete(CMPL_TMP_OPEN, sheetNo);
		return;
	}

	// 1. ­d¬ÅO(CW)ûüÝè
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_ENB, ON);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_DIR, gCMPL_TMP_LEFT.direction);
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, HIGH);

	// 2. 20msTimerSet
	CMPL_TMP_LEFT_SetEventTimer(CMPL_TMP_COMM_MOTOR_ENABLE_TIME_20,
					 			     CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Open_st2
// Description   : [^ì®Jn
// Parameter     : ]
// Return        : ]
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
// Description   : èÝóMãAStepDown
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_Open_st3()
{
	// StepDown
	DD_STM_SlowDownStop(DD_STM_ID_LEFT_TAMPER, CMPL_TMP_LEFT_ReceiveSteppingMotorEvent);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Open_st4
// Description   : ^C}Ýè
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_Open_st4()
{
	// 20msTimerSet
	CMPL_TMP_LEFT_SetEventTimer( CMPL_TMP_COMM_MOTOR_STOP_TIME_20, CMPL_TMP_LEFT_NORMAL_EVT);
}

///////////////////////////////////////////////////////////////////
// Function Name : LT_Open_st5
// Description   : ®ìÒ¿
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void LT_Open_st5()
{
	UC   sheetNo = gCMPL_TMP_LEFT.sheetNo;
	// ãd¬Ýè
	DD_DIO_SetOutput(DD_DO_LEFT_TAMPER_MOT_CUR, LOW);

	gCMPL_TMP_LEFT.action = CMPL_TMP_st0;
	gCMPL_TMP_LEFT.sheetNo = SHEETNO_NUM;

	CMPL_TMP_InformedLeftTamperComplete(CMPL_TMP_OPEN,  sheetNo);
	PrintTamperLog_Short(CMPL_TMP_OPEN_st5+0x80, CMPL_TMP_OPEN_st5+0x80, 0xFF);
}


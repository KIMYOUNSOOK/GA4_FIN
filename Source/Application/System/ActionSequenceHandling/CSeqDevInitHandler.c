// File Name:	CSeqDevInitHandler.c
// Description:	Implimentation File of Device Initialize Handler
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.22
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CSeqDevInitHandler.h"

#include "CSysManager.h"
#include "CDvstInformer.h"
#include "CCvrChecker.h"
#include "CJamChecker.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Const Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Device Initialize State ---
typedef enum /*ESeqDeviceInitState*/
{
	SEQ_DEV_INIT_STT_PWR_IDLE,			// 0
	SEQ_DEV_INIT_STT_PWR_INIT,			// 1
	SEQ_DEV_INIT_STT_PWR_INIT_PAUSE,	// 2
	SEQ_DEV_INIT_STT_PWR_INIT_DOWN,	// 3
	SEQ_DEV_INIT_STT_PWR_RSM,			// 4
	SEQ_DEV_INIT_STT_PWR_RSM_PAUSE,	// 5
	SEQ_DEV_INIT_STT_PWR_RSM_DOWN,	// 6
	SEQ_DEV_INIT_STT_PWR_JAM_CLR,		// 7
	SEQ_DEV_INIT_STT_PWR_JAM_CLR_DOWN,// 8
	SEQ_DEV_INIT_STT_PWR_NG_IDLE,		// 9
	SEQ_DEV_INIT_STT_NRML_IDLE,			// 10
	SEQ_DEV_INIT_STT_NRML_INIT,			// 11
	SEQ_DEV_INIT_STT_NRML_INIT_PAUSE,	// 12
	SEQ_DEV_INIT_STT_NRML_INIT_DOWN,	// 13
	SEQ_DEV_INIT_STT_NRML_JAM_CLR,	// 14
	SEQ_DEV_INIT_STT_NRML_JAM_CLR_DOWN,// 15
	SEQ_DEV_INIT_STT_PAUSE,				// 16
	SEQ_DEV_INIT_STT_RESET_IDLE,		// 17
	SEQ_DEV_INIT_STT_NUM
} ESeqDeviceInitState;

// --- Device Initialize Step ---
typedef enum /*ESeqDeviceInitStep*/
{
	SEQ_DEV_INIT_STEP_IDLE,
	SEQ_DEV_INIT_STEP_BEFORE_CHECK,
	SEQ_DEV_INIT_STEP_JAM_CHECK,
	SEQ_DEV_INIT_STEP_AFTER_CHECK,
	SEQ_DEV_INIT_STEP_NUM
} ESeqDeviceInitStep;

// --- Device Initialize Action Kind ---
typedef enum /*ESeqDeviceInitActionKind*/
{
	SEQ_DEV_INIT_ACT_KIND_PWR_ON,
	SEQ_DEV_INIT_ACT_KIND_PWR_RSM,
	SEQ_DEV_INIT_ACT_KIND_NRML,
	SEQ_DEV_INIT_ACT_KIND_NUM
} ESeqDeviceInitActionKind;

// --- Device Initialize Action Step ---
typedef enum /*ESeqDeviceInitActionStep*/
{
	SEQ_DEV_INIT_ACT_STEP_BEFORE_CHECK,
	SEQ_DEV_INIT_ACT_STEP_AFTER_CHECK,
	SEQ_DEV_INIT_ACT_STEP_NUM
} ESeqDeviceInitActionStep;

// --- Device Initialize Activity Exist Info Convert Bit ---
#define SEQ_DEV_INIT_ACTIVITY_EXIST_BIT(exist) (0x0001 << (exist))


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Convert Device Initialize State to Sequence Action State ---
static UC const cSeq_ActionStateConv[SEQ_DEV_INIT_STT_NUM] = {
	SEQ_ACT_STT_IDLE,					// SEQ_DEV_INIT_STT_PWR_IDLE
	SEQ_ACT_STT_RUNNING,				// SEQ_DEV_INIT_STT_PWR_INIT
	SEQ_ACT_STT_PAUSE,					// SEQ_DEV_INIT_STT_PWR_INIT_PAUSE
	SEQ_ACT_STT_RUNNING,				// SEQ_DEV_INIT_STT_PWR_INIT_DOWN
	SEQ_ACT_STT_RUNNING,				// SEQ_DEV_INIT_STT_PWR_RSM
	SEQ_ACT_STT_PAUSE,					// SEQ_DEV_INIT_STT_PWR_RSM_PAUSE
	SEQ_ACT_STT_RUNNING,				// SEQ_DEV_INIT_STT_PWR_RSM_DOWN
	SEQ_ACT_STT_RUNNING,				// SEQ_DEV_INIT_STT_PWR_JAM_CLR
	SEQ_ACT_STT_RUNNING,				// SEQ_DEV_INIT_STT_PWR_JAM_CLR_DOWN
	SEQ_ACT_STT_IDLE,					// SEQ_DEV_INIT_STT_PWR_NG_IDLE
	SEQ_ACT_STT_IDLE,					// SEQ_DEV_INIT_STT_NRML_IDLE
	SEQ_ACT_STT_RUNNING,				// SEQ_DEV_INIT_STT_NRML_INIT
	SEQ_ACT_STT_PAUSE,					// SEQ_DEV_INIT_STT_NRML_INIT_PAUSE
	SEQ_ACT_STT_RUNNING,				// SEQ_DEV_INIT_STT_NRML_INIT_DOWN
	SEQ_ACT_STT_RUNNING,				// SEQ_DEV_INIT_STT_NRML_JAM_CLR
	SEQ_ACT_STT_RUNNING,				// SEQ_DEV_INIT_STT_NRML_JAM_CLR_DOWN
	SEQ_ACT_STT_PAUSE,					// SEQ_DEV_INIT_STT_PAUSE
	SEQ_ACT_STT_IDLE,					// SEQ_DEV_INIT_STT_RESET_IDLE
};

// --- Convert Table Device Initialize State to Device Initialize Action Kind ---
static UC const cSeq_InitActionKindConv[SEQ_DEV_INIT_STT_NUM] ={
	SEQ_DEV_INIT_ACT_KIND_NUM,			// SEQ_DEV_INIT_STT_PWR_IDLE
	SEQ_DEV_INIT_ACT_KIND_PWR_ON,		// SEQ_DEV_INIT_STT_PWR_INIT
	SEQ_DEV_INIT_ACT_KIND_PWR_ON,		// SEQ_DEV_INIT_STT_PWR_INIT_PAUSE
	SEQ_DEV_INIT_ACT_KIND_PWR_ON,		// SEQ_DEV_INIT_STT_PWR_INIT_DOWN
	SEQ_DEV_INIT_ACT_KIND_PWR_RSM,		// SEQ_DEV_INIT_STT_PWR_RSM
	SEQ_DEV_INIT_ACT_KIND_PWR_RSM,		// SEQ_DEV_INIT_STT_PWR_RSM_PAUSE
	SEQ_DEV_INIT_ACT_KIND_PWR_RSM,		// SEQ_DEV_INIT_STT_PWR_RSM_DOWN
	SEQ_DEV_INIT_ACT_KIND_NUM,			// SEQ_DEV_INIT_STT_PWR_JAM_CLR
	SEQ_DEV_INIT_ACT_KIND_NUM,			// SEQ_DEV_INIT_STT_PWR_JAM_CLR_DOWN
	SEQ_DEV_INIT_ACT_KIND_NUM,			// SEQ_DEV_INIT_STT_PWR_NG_IDLE
	SEQ_DEV_INIT_ACT_KIND_NUM,			// SEQ_DEV_INIT_STT_NRML_IDLE
	SEQ_DEV_INIT_ACT_KIND_NRML,			// SEQ_DEV_INIT_STT_NRML_INIT
	SEQ_DEV_INIT_ACT_KIND_NRML,			// SEQ_DEV_INIT_STT_NRML_INIT_PAUSE
	SEQ_DEV_INIT_ACT_KIND_NRML,			// SEQ_DEV_INIT_STT_NRML_INIT_DOWN
	SEQ_DEV_INIT_ACT_KIND_NUM,			// SEQ_DEV_INIT_STT_NRML_JAM_CLR
	SEQ_DEV_INIT_ACT_KIND_NUM,			// SEQ_DEV_INIT_STT_NRML_JAM_CLR_DOWN
	SEQ_DEV_INIT_ACT_KIND_NUM,			// SEQ_DEV_INIT_STT_PAUSE
	SEQ_DEV_INIT_ACT_KIND_NUM			// SEQ_DEV_INIT_STT_RESET_IDLE
};

// --- Spec data of Device Initialize Handler ---
extern SeqDevInitHandlerSpec const cSeq_DevInitHandlerSpec;

// --- Member of Device Initialize Handler ---
typedef struct /*CSeqDevInitHandlerMember*/
{
	CSrvcMsgActivity mDevInitActivity[SEQ_DEV_INIT_ACT_KIND_NUM][SEQ_DEV_INIT_ACT_STEP_NUM];	// Device Initialize Activity
	CSrvcMsgActivity mJamCheckActivity;															// Jam Check Action Activity
	CSrvcMsgActivity mJamClrAssistActivity;														// Jam Clear Assist Activity
	UC mState:6;																				// Device Initialize State
	UC mStep:2;																					// Device Initialize Step
	UC mPowerUpMode;																			// Power Up Mode
	US mDevInitActivityExistBit;																// Device Initialize Activity Exist	
	CSrvcMsgActivity* mpActivity;																// Pointer of Activity
} CSeqDevInitHandlerMember;

static CSeqDevInitHandlerMember gSeq_DevInitHandlerMember;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

static bool StartInitialize();
static bool StopInitialize();
static bool RestartInitialize();
static bool ActivityComplete();
static bool Reset();


/////////////////////////////////////////////////////////////////////////////////////////
// Timer Event Call Back Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void SEQ_InitActivityTimeOut(SS timerID, UL idx);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CSeqDevInitHandler_Constructor
// Description   : コンストラクタ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSeqDevInitHandler_Constructor()
{
	gSeq_DevInitHandlerMember.mState				   = SEQ_DEV_INIT_STT_PWR_IDLE;
	gSeq_DevInitHandlerMember.mStep					   = SEQ_DEV_INIT_STEP_IDLE;
	gSeq_DevInitHandlerMember.mPowerUpMode			   = SYS_PWR_UP_MODE_NORMAL;
	gSeq_DevInitHandlerMember.mDevInitActivityExistBit = 0x0000;
	gSeq_DevInitHandlerMember.mpActivity			   = NULL;

	if ( cSeq_DevInitHandlerSpec.pPwrOnInitBeforeJamCheckScenario != NULL ){
		gSeq_DevInitHandlerMember.mDevInitActivityExistBit |= SEQ_DEV_INIT_ACTIVITY_EXIST_BIT(SEQ_DEV_INIT_ACT_KIND_NUM * SEQ_DEV_INIT_ACT_KIND_PWR_ON + SEQ_DEV_INIT_ACT_STEP_BEFORE_CHECK);

		CSrvcMsgActivity_Constructor(&(gSeq_DevInitHandlerMember.mDevInitActivity[SEQ_DEV_INIT_ACT_KIND_PWR_ON][SEQ_DEV_INIT_ACT_STEP_BEFORE_CHECK]),
									 MID_SYS,
									 cSeq_DevInitHandlerSpec.pwrOnInitBeforeJamCheckScenarioSize,
									 cSeq_DevInitHandlerSpec.pPwrOnInitBeforeJamCheckScenario,
									 SEQ_InitActivityTimeOut);
	}
	else{ /* No Action */ }

	if ( cSeq_DevInitHandlerSpec.pPwrOnInitAfterJamCheckScenario != NULL ){
		gSeq_DevInitHandlerMember.mDevInitActivityExistBit |= SEQ_DEV_INIT_ACTIVITY_EXIST_BIT(SEQ_DEV_INIT_ACT_KIND_NUM * SEQ_DEV_INIT_ACT_KIND_PWR_ON + SEQ_DEV_INIT_ACT_STEP_AFTER_CHECK);

		CSrvcMsgActivity_Constructor(&(gSeq_DevInitHandlerMember.mDevInitActivity[SEQ_DEV_INIT_ACT_KIND_PWR_ON][SEQ_DEV_INIT_ACT_STEP_AFTER_CHECK]),
									 MID_SYS,
									 cSeq_DevInitHandlerSpec.pwrOnInitAfterJamCheckScenarioSize,
									 cSeq_DevInitHandlerSpec.pPwrOnInitAfterJamCheckScenario,
									 SEQ_InitActivityTimeOut);
	}
	else{ /* No Action */ }

	if ( cSeq_DevInitHandlerSpec.pPwrRsmInitBeforeJamCheckScenario != NULL ){
		gSeq_DevInitHandlerMember.mDevInitActivityExistBit |= SEQ_DEV_INIT_ACTIVITY_EXIST_BIT(SEQ_DEV_INIT_ACT_KIND_NUM * SEQ_DEV_INIT_ACT_KIND_PWR_RSM + SEQ_DEV_INIT_ACT_STEP_BEFORE_CHECK);

		CSrvcMsgActivity_Constructor(&(gSeq_DevInitHandlerMember.mDevInitActivity[SEQ_DEV_INIT_ACT_KIND_PWR_RSM][SEQ_DEV_INIT_ACT_STEP_BEFORE_CHECK]),
									 MID_SYS,
									 cSeq_DevInitHandlerSpec.pwrRsmInitBeforeJamCheckScenarioSize,
									 cSeq_DevInitHandlerSpec.pPwrRsmInitBeforeJamCheckScenario,
									 SEQ_InitActivityTimeOut);
	}
	else{ /* No Action */ }

	if ( cSeq_DevInitHandlerSpec.pPwrRsmInitAfterJamCheckScenario != NULL ){
		gSeq_DevInitHandlerMember.mDevInitActivityExistBit |= SEQ_DEV_INIT_ACTIVITY_EXIST_BIT(SEQ_DEV_INIT_ACT_KIND_NUM * SEQ_DEV_INIT_ACT_KIND_PWR_RSM + SEQ_DEV_INIT_ACT_STEP_AFTER_CHECK);

		CSrvcMsgActivity_Constructor(&(gSeq_DevInitHandlerMember.mDevInitActivity[SEQ_DEV_INIT_ACT_KIND_PWR_RSM][SEQ_DEV_INIT_ACT_STEP_AFTER_CHECK]),
									 MID_SYS,
									 cSeq_DevInitHandlerSpec.pwrRsmInitAfterJamCheckScenarioSize,
									 cSeq_DevInitHandlerSpec.pPwrRsmInitAfterJamCheckScenario,
									 SEQ_InitActivityTimeOut);
	}
	else{ /* No Action */ }

	if ( cSeq_DevInitHandlerSpec.pNormalInitBeforeJamCheckScenario != NULL ){
		gSeq_DevInitHandlerMember.mDevInitActivityExistBit |= SEQ_DEV_INIT_ACTIVITY_EXIST_BIT(SEQ_DEV_INIT_ACT_KIND_NUM * SEQ_DEV_INIT_ACT_KIND_NRML + SEQ_DEV_INIT_ACT_STEP_BEFORE_CHECK);

		CSrvcMsgActivity_Constructor(&(gSeq_DevInitHandlerMember.mDevInitActivity[SEQ_DEV_INIT_ACT_KIND_NRML][SEQ_DEV_INIT_ACT_STEP_BEFORE_CHECK]),
									 MID_SYS,
									 cSeq_DevInitHandlerSpec.normalInitBeforeJamCheckScenarioSize,
									 cSeq_DevInitHandlerSpec.pNormalInitBeforeJamCheckScenario,
									 SEQ_InitActivityTimeOut);
	}
	else{ /* No Action */ }

	if ( cSeq_DevInitHandlerSpec.pNormalInitAfterJamCheckScenario != NULL ){
		gSeq_DevInitHandlerMember.mDevInitActivityExistBit |= SEQ_DEV_INIT_ACTIVITY_EXIST_BIT(SEQ_DEV_INIT_ACT_KIND_NUM * SEQ_DEV_INIT_ACT_KIND_NRML + SEQ_DEV_INIT_ACT_STEP_AFTER_CHECK);

		CSrvcMsgActivity_Constructor(&(gSeq_DevInitHandlerMember.mDevInitActivity[SEQ_DEV_INIT_ACT_KIND_NRML][SEQ_DEV_INIT_ACT_STEP_AFTER_CHECK]),
									 MID_SYS,
									 cSeq_DevInitHandlerSpec.normalInitAfterJamCheckScenarioSize,
									 cSeq_DevInitHandlerSpec.pNormalInitAfterJamCheckScenario,
									 SEQ_InitActivityTimeOut);
	}
	else{ /* No Action */ }

	CSrvcMsgActivity_Constructor(&(gSeq_DevInitHandlerMember.mJamCheckActivity), MID_SYS, cSeq_DevInitHandlerSpec.jamCheckScenarioSize, cSeq_DevInitHandlerSpec.pJamCheckScenario, SEQ_InitActivityTimeOut);

	if ( cSeq_DevInitHandlerSpec.pJamClrAssistScenario != NULL ){				// ジャムクリア補助イニシャライズ動作あり
		CSrvcMsgActivity_Constructor(&(gSeq_DevInitHandlerMember.mJamClrAssistActivity), MID_SYS, cSeq_DevInitHandlerSpec.jamClrAssistScenarioSize, cSeq_DevInitHandlerSpec.pJamClrAssistScenario, SEQ_InitActivityTimeOut);
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : StartInitialize
// Description   : イニシャライズ動作開始処理
// Parameter     : ‐
// Return        : true = イニシャライズ動作状態変化あり
//               : false = イニシャライズ動作状態変化なし
///////////////////////////////////////////////////////////////////
bool StartInitialize()
{
	UC currentState, currentStep;
	US TempUS1, TempUS2;

	currentState = gSeq_DevInitHandlerMember.mState;
	currentStep  = gSeq_DevInitHandlerMember.mStep;

	// --- ステート & ステップ遷移 ---
	if( (CDvstInformer_GetDeviceCondtion(DEVID_FINISHER) != DEV_COND_PERM_NG) &&
		(CCvrChecker_GetCoverStatus(CVR_ID_NUM) == CVR_STTS_CLOSE) ){
		switch ( gSeq_DevInitHandlerMember.mState ){
		case SEQ_DEV_INIT_STT_PWR_IDLE:
			if( CJamChecker_GetJamZoneStatus(JAM_ZONE_ID_NUM) == JAM_ZONE_STTS_SET ){
				if ( cSeq_DevInitHandlerSpec.pJamClrAssistScenario != NULL ){
					gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_PWR_JAM_CLR;
					gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_IDLE;
				}
				else{ /* No Action */ }
			}
			else{
				if ( gSeq_DevInitHandlerMember.mPowerUpMode == SYS_PWR_UP_MODE_SAVE ){
					gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_PWR_RSM;
					gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_BEFORE_CHECK;
				}
				else{
					gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_PWR_INIT;

					if ( gSeq_DevInitHandlerMember.mDevInitActivityExistBit & SEQ_DEV_INIT_ACTIVITY_EXIST_BIT(SEQ_DEV_INIT_ACT_KIND_NUM * cSeq_InitActionKindConv[gSeq_DevInitHandlerMember.mState] + SEQ_DEV_INIT_ACT_STEP_BEFORE_CHECK) ){
						gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_BEFORE_CHECK;
					}
					else{														// 残留紙チェック動作前アクティビティなし
						gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_JAM_CHECK;
					}
				}
			}

			break;
		case SEQ_DEV_INIT_STT_PWR_NG_IDLE:
			if( CJamChecker_GetJamZoneStatus(JAM_ZONE_ID_NUM) == JAM_ZONE_STTS_SET ){
				if ( cSeq_DevInitHandlerSpec.pJamClrAssistScenario != NULL ){
					gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_PWR_JAM_CLR;
					gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_IDLE;
				}
				else{ /* No Action */ }
			}
			else{
				gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_PWR_INIT;

				if ( gSeq_DevInitHandlerMember.mDevInitActivityExistBit & SEQ_DEV_INIT_ACTIVITY_EXIST_BIT(SEQ_DEV_INIT_ACT_KIND_NUM * cSeq_InitActionKindConv[gSeq_DevInitHandlerMember.mState] + SEQ_DEV_INIT_ACT_STEP_BEFORE_CHECK) ){
					gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_BEFORE_CHECK;
				}
				else{															// 残留紙チェック動作前アクティビティなし
					gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_JAM_CHECK;
				}
			}

			break;
		case SEQ_DEV_INIT_STT_NRML_IDLE:
			if( CJamChecker_GetJamZoneStatus(JAM_ZONE_ID_NUM) == JAM_ZONE_STTS_SET ){
				if ( cSeq_DevInitHandlerSpec.pJamClrAssistScenario != NULL ){
					gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_NRML_JAM_CLR;
					gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_IDLE;
				}
				else{ /* No Action */ }
			}
			else{
				gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_NRML_INIT;
			// (CR-004) 通常手順シーケンス：Jam検知→Compiler&Stacker Initialize
			//	gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_BEFORE_CHECK;
				gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_JAM_CHECK;
			}

			break;
		case SEQ_DEV_INIT_STT_RESET_IDLE:
			if( CJamChecker_GetJamZoneStatus(JAM_ZONE_ID_NUM) == JAM_ZONE_STTS_SET ){
				if ( cSeq_DevInitHandlerSpec.pJamClrAssistScenario != NULL ){
					gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_PWR_JAM_CLR;
					gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_IDLE;
				}
				else{ /* No Action */ }
			}
			else{
				gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_PWR_INIT;

				if ( gSeq_DevInitHandlerMember.mDevInitActivityExistBit & SEQ_DEV_INIT_ACTIVITY_EXIST_BIT(SEQ_DEV_INIT_ACT_KIND_NUM * cSeq_InitActionKindConv[gSeq_DevInitHandlerMember.mState] + SEQ_DEV_INIT_ACT_STEP_BEFORE_CHECK) ){
					gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_BEFORE_CHECK;
				}
				else{															// 残留紙チェック動作前アクティビティなし
					gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_JAM_CHECK;
				}
			}

			break;
		default:
			break;
		}
	}
	else{
		// No Action
	}
	TempUS1 = (US)currentState;
	TempUS1 = TempUS1 << 8;	
	TempUS1 = TempUS1 | (US)currentStep;
	TempUS2 = (US)gSeq_DevInitHandlerMember.mState ; 
	TempUS2 = TempUS2 << 8 ; 	
	TempUS2 = TempUS2 | (US)gSeq_DevInitHandlerMember.mStep;
	SRVC_INITManagerLog_Wide(0xC0, currentState, TempUS1, TempUS2 );

	// --- ステップ遷移アクション ---
	if ( currentStep != gSeq_DevInitHandlerMember.mStep ){
		switch ( gSeq_DevInitHandlerMember.mStep ){
		case SEQ_DEV_INIT_STEP_BEFORE_CHECK:
			// --- イニシャライズ動作(残留紙チェック動作前アクティビティ)開始 ---
			gSeq_DevInitHandlerMember.mpActivity = &(gSeq_DevInitHandlerMember.mDevInitActivity[cSeq_InitActionKindConv[gSeq_DevInitHandlerMember.mState]][SEQ_DEV_INIT_ACT_STEP_BEFORE_CHECK]);

			CSrvcMsgActivity_Start(gSeq_DevInitHandlerMember.mpActivity);

			break;
		case SEQ_DEV_INIT_STEP_JAM_CHECK:
			// --- イニシャライズ動作(残留紙チェック動作アクティビティ)開始 ---
			gSeq_DevInitHandlerMember.mpActivity = &(gSeq_DevInitHandlerMember.mJamCheckActivity);

			CSrvcMsgActivity_Start(gSeq_DevInitHandlerMember.mpActivity);

			break;
		default:
			break;
		}
	}
	else{
		// No Change
	}

	// --- ステート遷移アクション ---
	if ( currentState != gSeq_DevInitHandlerMember.mState ){
		switch ( gSeq_DevInitHandlerMember.mState ){
		case SEQ_DEV_INIT_STT_PWR_JAM_CLR:
		case SEQ_DEV_INIT_STT_NRML_JAM_CLR:
			// --- ジャムクリア補助イニシャライズ動作開始 ---
			gSeq_DevInitHandlerMember.mpActivity = &(gSeq_DevInitHandlerMember.mJamClrAssistActivity);

			CSrvcMsgActivity_Start(gSeq_DevInitHandlerMember.mpActivity);

			break;
		default:
			break;
		}

		return true;
	}
	else{
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : StopInitialize
// Description   : イニシャライズ動作停止処理
// Parameter     : ‐
// Return        : true = イニシャライズ動作状態変化あり
//               : false = イニシャライズ動作状態変化なし
///////////////////////////////////////////////////////////////////
bool StopInitialize()
{
	UC currentState;
	US TempUS1, TempUS2;
	currentState = gSeq_DevInitHandlerMember.mState;

	// --- ステート & ステップ遷移 ---
	switch ( gSeq_DevInitHandlerMember.mState ){
	case SEQ_DEV_INIT_STT_PWR_INIT:		gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_PWR_INIT_DOWN;		break;
	case SEQ_DEV_INIT_STT_PWR_RSM:		gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_PWR_RSM_DOWN;		break;
	case SEQ_DEV_INIT_STT_PWR_JAM_CLR: gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_PWR_JAM_CLR_DOWN;	break;
	case SEQ_DEV_INIT_STT_NRML_INIT:	gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_NRML_INIT_DOWN;	break;
	case SEQ_DEV_INIT_STT_NRML_JAM_CLR:	gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_NRML_JAM_CLR_DOWN;	break;
	default:																break;
	}
	TempUS1 = (US)currentState;
	TempUS1 = TempUS1 << 8;	
	//TempUS1 = TempUS1 | (US)currentStep;
	TempUS2 = (US)gSeq_DevInitHandlerMember.mState ; 
	TempUS2 = TempUS2 << 8 ; 	
	TempUS2 = TempUS2 | (US)gSeq_DevInitHandlerMember.mStep;
	SRVC_INITManagerLog_Wide(0xC1, currentState, TempUS1, TempUS2 );

	// --- ステート遷移アクション ---
	if ( currentState != gSeq_DevInitHandlerMember.mState ){
		// --- 動作停止開始 ---
		CSrvcMsgActivity_Abort(gSeq_DevInitHandlerMember.mpActivity);

		return true;
	}
	else{
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : RestartInitialize
// Description   : ジャム検知処理終了後のイニシャライズ動作再開処理
// Parameter     : ‐
// Return        : true = イニシャライズ動作状態変化あり
//               : false = イニシャライズ動作状態変化なし
///////////////////////////////////////////////////////////////////
bool RestartInitialize()
{
	UC currentState, currentStep;
	US TempUS1, TempUS2;

	currentState = gSeq_DevInitHandlerMember.mState;
	currentStep  = gSeq_DevInitHandlerMember.mStep;

	// --- ステート & ステップ遷移 ---
	switch ( gSeq_DevInitHandlerMember.mState ){
	case SEQ_DEV_INIT_STT_NRML_INIT_PAUSE:
		if( (CDvstInformer_GetDeviceCondtion(DEVID_FINISHER) != DEV_COND_PERM_NG) &&
			(CCvrChecker_GetCoverStatus(CVR_ID_NUM) == CVR_STTS_CLOSE) ){
			if( CJamChecker_GetJamZoneStatus(JAM_ZONE_ID_NUM) == JAM_ZONE_STTS_SET ){
				if ( cSeq_DevInitHandlerSpec.pJamClrAssistScenario != NULL ){
					gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_IDLE;
					gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_NRML_JAM_CLR;
				}
				else{															// ジャムクリア補助イニシャライズ動作なし
					gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_NRML_IDLE;
				}
			}
			else{
				gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_AFTER_CHECK;
				gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_NRML_INIT;
			}
		}
		else{
			gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_NRML_IDLE;
		}

		break;
	case SEQ_DEV_INIT_STT_PWR_INIT_PAUSE:
		if( (CDvstInformer_GetDeviceCondtion(DEVID_FINISHER) != DEV_COND_PERM_NG) &&
			(CCvrChecker_GetCoverStatus(CVR_ID_NUM) == CVR_STTS_CLOSE) ){
			if( CJamChecker_GetJamZoneStatus(JAM_ZONE_ID_NUM) == JAM_ZONE_STTS_SET ){
				if ( cSeq_DevInitHandlerSpec.pJamClrAssistScenario != NULL ){
					gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_IDLE;
					gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_PWR_JAM_CLR;
				}
				else{															// ジャムクリア補助イニシャライズ動作なし
					gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_PWR_NG_IDLE;
				}
			}
			else{
				gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_AFTER_CHECK;
				gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_PWR_INIT;
			}
		}
		else{
			gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_PWR_NG_IDLE;
		}

		break;
	case SEQ_DEV_INIT_STT_PWR_RSM_PAUSE:
		if( (CDvstInformer_GetDeviceCondtion(DEVID_FINISHER) != DEV_COND_PERM_NG) &&
			(CCvrChecker_GetCoverStatus(CVR_ID_NUM) == CVR_STTS_CLOSE) ){
			if( CJamChecker_GetJamZoneStatus(JAM_ZONE_ID_NUM) == JAM_ZONE_STTS_SET ){
				if ( cSeq_DevInitHandlerSpec.pJamClrAssistScenario != NULL ){
					gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_IDLE;
					gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_PWR_JAM_CLR;
				}
				else{															// ジャムクリア補助イニシャライズ動作なし
					gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_PWR_NG_IDLE;
				}
			}
			else{
				gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_AFTER_CHECK;
				gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_PWR_RSM;
			}
		}
		else{
			gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_PWR_NG_IDLE;
		}

		break;
	case SEQ_DEV_INIT_STT_PAUSE:
		if( (CDvstInformer_GetDeviceCondtion(DEVID_FINISHER) != DEV_COND_PERM_NG) &&
			(CCvrChecker_GetCoverStatus(CVR_ID_NUM) == CVR_STTS_CLOSE) &&
			(CJamChecker_GetJamZoneStatus(JAM_ZONE_ID_NUM) == JAM_ZONE_STTS_SET) &&
			(cSeq_DevInitHandlerSpec.pJamClrAssistScenario != NULL) ){			// ジャムクリア補助イニシャライズ動作あり
			gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_IDLE;
			gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_NRML_JAM_CLR;
		}
		else{
			gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_NRML_IDLE;
		}

		break;
	default:
		break;
	}
	TempUS1 = (US)currentState;
	TempUS1 = TempUS1 << 8;	
	TempUS1 = TempUS1 | (US)currentStep;
	TempUS2 = (US)gSeq_DevInitHandlerMember.mState ; 
	TempUS2 = TempUS2 << 8 ; 	
	TempUS2 = TempUS2 | (US)gSeq_DevInitHandlerMember.mStep;
	SRVC_INITManagerLog_Wide(0xCA, currentState, TempUS1, TempUS2 );

	// --- ステップ遷移アクション ---
	if ( currentStep != gSeq_DevInitHandlerMember.mStep ){
		switch ( gSeq_DevInitHandlerMember.mStep ){
		case SEQ_DEV_INIT_STEP_AFTER_CHECK:
			// --- イニシャライズ動作(残留紙チェック動作後アクティビティ)開始 ---
			gSeq_DevInitHandlerMember.mpActivity = &(gSeq_DevInitHandlerMember.mDevInitActivity[cSeq_InitActionKindConv[gSeq_DevInitHandlerMember.mState]][SEQ_DEV_INIT_ACT_STEP_AFTER_CHECK]);

			CSrvcMsgActivity_Start(gSeq_DevInitHandlerMember.mpActivity);

			break;
		default:
			// No Action
			break;
		}
	}
	else{
		// No Change
	}

	// --- ステート遷移アクション ---
	if ( currentState != gSeq_DevInitHandlerMember.mState ){
		switch ( gSeq_DevInitHandlerMember.mState ){
		case SEQ_DEV_INIT_STT_NRML_JAM_CLR:
		case SEQ_DEV_INIT_STT_PWR_JAM_CLR:
			// --- ジャムクリア補助イニシャライズ動作開始 ---
			gSeq_DevInitHandlerMember.mpActivity = &(gSeq_DevInitHandlerMember.mJamClrAssistActivity);

			CSrvcMsgActivity_Start(gSeq_DevInitHandlerMember.mpActivity);

			break;
		default:
			// No Action
			break;
		}

		return true;
	}
	else{
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : ActivityComplete
// Description   : アクティビティ完了時処理
// Parameter     : ‐
// Return        : true = イニシャライズ動作状態変化あり
//               : false = イニシャライズ動作状態変化なし
///////////////////////////////////////////////////////////////////
bool ActivityComplete()
{
	UC currentState, currentStep;
	US TempUS1, TempUS2;

	currentState = gSeq_DevInitHandlerMember.mState;
	currentStep  = gSeq_DevInitHandlerMember.mStep;

	// --- ステート & ステップ遷移 ---
	switch ( gSeq_DevInitHandlerMember.mState ){
	case SEQ_DEV_INIT_STT_NRML_INIT:
	case SEQ_DEV_INIT_STT_PWR_INIT:
	case SEQ_DEV_INIT_STT_PWR_RSM:
		switch ( gSeq_DevInitHandlerMember.mStep ){
		case SEQ_DEV_INIT_STEP_BEFORE_CHECK:
			gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_JAM_CHECK;

			break;
		case SEQ_DEV_INIT_STEP_JAM_CHECK:
			if ( gSeq_DevInitHandlerMember.mDevInitActivityExistBit & SEQ_DEV_INIT_ACTIVITY_EXIST_BIT(SEQ_DEV_INIT_ACT_KIND_NUM * cSeq_InitActionKindConv[gSeq_DevInitHandlerMember.mState] + SEQ_DEV_INIT_ACT_STEP_AFTER_CHECK) ){
			 	if ( gSeq_DevInitHandlerMember.mState == SEQ_DEV_INIT_STT_PWR_INIT )   gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_PWR_INIT_PAUSE;
				else if (gSeq_DevInitHandlerMember.mState == SEQ_DEV_INIT_STT_PWR_RSM) gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_PWR_RSM_PAUSE;
				else																   gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_NRML_INIT_PAUSE;
			}
			else{																// シーケンス完了
				gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_IDLE;
				gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_PAUSE;
			}

			break;
		case SEQ_DEV_INIT_STEP_AFTER_CHECK:										// シーケンス完了
			gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_IDLE;
			gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_PAUSE;

			break;
		default:
			break;
		}

		break;
	case SEQ_DEV_INIT_STT_PWR_JAM_CLR:
		gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_IDLE;
		gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_PWR_NG_IDLE;

		break;
	case SEQ_DEV_INIT_STT_NRML_JAM_CLR:
		gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_IDLE;
		gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_NRML_IDLE;

		break;
	case SEQ_DEV_INIT_STT_PWR_INIT_DOWN:
		gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_IDLE;
		gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_PWR_NG_IDLE;

		break;
	case SEQ_DEV_INIT_STT_PWR_RSM_DOWN:
		gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_IDLE;
		gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_PWR_NG_IDLE;

		break;
	case SEQ_DEV_INIT_STT_PWR_JAM_CLR_DOWN:
		gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_IDLE;
		gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_PWR_NG_IDLE;

		break;
	case SEQ_DEV_INIT_STT_NRML_INIT_DOWN:
		gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_IDLE;
		gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_NRML_IDLE;

		break;
	case SEQ_DEV_INIT_STT_NRML_JAM_CLR_DOWN:
		gSeq_DevInitHandlerMember.mStep = SEQ_DEV_INIT_STEP_IDLE;
		gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_NRML_IDLE;

		break;
	default:
		break;
	}
	TempUS1 = (US)currentState;
	TempUS1 = TempUS1 << 8;	
	TempUS1 = TempUS1 | (US)currentStep;
	TempUS2 = (US)gSeq_DevInitHandlerMember.mState ; 
	TempUS2 = TempUS2 << 8 ; 	
	TempUS2 = TempUS2 | (US)gSeq_DevInitHandlerMember.mStep;

	SRVC_INITManagerLog_Wide(0xCB, currentState, TempUS1, TempUS2 );

	// --- ステップ遷移アクション ---
	if ( currentStep != gSeq_DevInitHandlerMember.mStep ){
		switch ( gSeq_DevInitHandlerMember.mStep ){
		case SEQ_DEV_INIT_STEP_JAM_CHECK:
			// --- イニシャライズ動作(残留紙チェック動作アクティビティ)開始 ---
			gSeq_DevInitHandlerMember.mpActivity = &(gSeq_DevInitHandlerMember.mJamCheckActivity);

			CSrvcMsgActivity_Start(gSeq_DevInitHandlerMember.mpActivity);

			break;
		default:
			// No Action
			break;
		}
	}
	else{
		// No Change
	}

	// --- ステート遷移アクション ---
	if ( currentState != gSeq_DevInitHandlerMember.mState ){
		// No Action

		return true;
	}
	else{
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : Reset
// Description   : Diag Modeからの復帰時のためのイニシャライズ動作
//               : 状態リセット処理
// Parameter     : ‐
// Return        : true = イニシャライズ動作状態変化あり
//               : false = イニシャライズ動作状態変化なし
///////////////////////////////////////////////////////////////////
bool Reset()
{
	UC currentState;

	currentState = gSeq_DevInitHandlerMember.mState;

	// --- ステート遷移 ---
	switch ( gSeq_DevInitHandlerMember.mState ){
	case SEQ_DEV_INIT_STT_PWR_NG_IDLE:
	case SEQ_DEV_INIT_STT_NRML_IDLE:
		gSeq_DevInitHandlerMember.mState = SEQ_DEV_INIT_STT_RESET_IDLE;

		break;
	default:
		break;
	}

	// --- ステート遷移アクション ---
	if ( currentState != gSeq_DevInitHandlerMember.mState ){
		// No Action

		return true;
	}
	else{
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSeqDevInitHandler_GetState
// Description   : イニシャライズ動作状態取得処理
// Parameter     : ‐
// Return        : イニシャライズ動作状態
///////////////////////////////////////////////////////////////////
ESeqActionState CSeqDevInitHandler_GetState()
{
//	if ( gSeq_DevInitHandlerMember.mState < SEQ_DEV_INIT_STT_NUM ){				// 自身でセットする変数なので範囲外チェック不要
		return cSeq_ActionStateConv[gSeq_DevInitHandlerMember.mState];
//	}
//	else{
//		return SEQ_ACT_STT_IDLE;
//	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CSeqDevInitHandler_Reset
// Description   : Diag Modeからの復帰時のためのリセット指示受信時
//               : 処理
// Parameter     : ‐
// Return        : SEQ_ACT_STT_NUM = イニシャライズ動作状態変化なし
//               : SEQ_ACT_STT_NUM 以外 = 遷移先イニシャライズ動作
//               : 状態
///////////////////////////////////////////////////////////////////
ESeqActionState CSeqDevInitHandler_Reset()
{
	if ( Reset() == true ) return CSeqDevInitHandler_GetState();
	else				   return SEQ_ACT_STT_NUM;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSeqDevInitHandler_Start
// Description   : イニシャライズ動作開始指示受信時処理
// Parameter     : mode (Power Up Mode)
// Return        : SEQ_ACT_STT_NUM = イニシャライズ動作状態変化なし
//               : SEQ_ACT_STT_NUM 以外 = 遷移先イニシャライズ動作
//               : 状態
///////////////////////////////////////////////////////////////////
ESeqActionState CSeqDevInitHandler_Start(ESysPowerUpMode mode)
{
	gSeq_DevInitHandlerMember.mPowerUpMode = mode;

	if ( StartInitialize() == true ) return CSeqDevInitHandler_GetState();
	else							 return SEQ_ACT_STT_NUM;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSeqDevInitHandler_Stop
// Description   : イニシャライズ動作停止指示受信時処理
// Parameter     : ‐
// Return        : SEQ_ACT_STT_NUM = イニシャライズ動作状態変化なし
//               : SEQ_ACT_STT_NUM 以外 = 遷移先イニシャライズ動作
//               : 状態
///////////////////////////////////////////////////////////////////
ESeqActionState CSeqDevInitHandler_Stop()
{
	if ( StopInitialize() == true ) return CSeqDevInitHandler_GetState();
	else							return SEQ_ACT_STT_NUM;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSeqDevInitHandler_DeviceActionResponse
// Description   : Device Action Responseイベント受信時処理
// Parameter     : eventID (MSP Event ID)
//               : moduleID (Device Module ID)
// Return        : SEQ_ACT_STT_NUM = イニシャライズ動作状態変化なし
//               : SEQ_ACT_STT_NUM 以外 = 遷移先イニシャライズ動作
//               : 状態
///////////////////////////////////////////////////////////////////
ESeqActionState CSeqDevInitHandler_DeviceActionResponse(US eventID, UC moduleID)
{
	UC stateChange;

	stateChange = SEQ_ACT_STT_NUM;

	if ( CSeqDevInitHandler_GetState() == SEQ_ACT_STT_RUNNING ){
		CSrvcMsgActivity_SetTrigger(gSeq_DevInitHandlerMember.mpActivity, eventID, moduleID);

		if ( CSrvcMsgActivity_CheckEnd(gSeq_DevInitHandlerMember.mpActivity) == true ){
																				// アクティビティ完了
			if ( ActivityComplete() == true ){
				return CSeqDevInitHandler_GetState();
			}
			else{ /* No Action */ }
		}
		else{
			CSrvcMsgActivity_Progress(gSeq_DevInitHandlerMember.mpActivity);
		}
	}
	else{
		// No Action
	}

	return SEQ_ACT_STT_NUM;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSeqDevInitHandler_Restart
// Description   : 動作再開指示受信処理(ジャム検知処理(ジャムゾーン
//               : セット処理)完了時処理)
// Parameter     : ‐
// Return        : SEQ_ACT_STT_NUM = イニシャライズ動作状態変化なし
//               : SEQ_ACT_STT_NUM 以外 = 遷移先イニシャライズ動作
//               : 状態
///////////////////////////////////////////////////////////////////
ESeqActionState CSeqDevInitHandler_Restart()
{
	UC stateChange;

	stateChange = SEQ_ACT_STT_NUM;

	if ( RestartInitialize() == true ){
		stateChange = CSeqDevInitHandler_GetState();
	}
	else{
		// No Action
	}

	return stateChange;
}


/////////////////////////////////////////////////////////////////////////////////////////
// Timer Event Call Back Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : SEQ_InitActivityTimeOut
// Description   : アクティビティ開始待ち時間経過通知コールバック関
//               : 数
// Parameter     : timerID (MSP Timer ID)
//               : idx (アクティビティ管理テーブルインデックス)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SEQ_InitActivityTimeOut(SS timerID, UL idx)
{
	if ( gSeq_DevInitHandlerMember.mpActivity != NULL ){
		CSrvcMsgActivity_TimeOut(gSeq_DevInitHandlerMember.mpActivity, idx);
	}
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

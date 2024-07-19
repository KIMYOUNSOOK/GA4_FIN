// File Name:	CInitDevInitHandler.c
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
#include "CInitDevInitHandler.h"

#include "DeviceStatusInformation.h"
#include "CoverCheck.h"
#include "JamCheck.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Const Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Device Initialize State ---
typedef enum /*EInitDeviceInitState*/
{
	INIT_DEV_INIT_STT_PWR_IDLE,
	INIT_DEV_INIT_STT_PWR_INIT,
	INIT_DEV_INIT_STT_PWR_RSM,
	INIT_DEV_INIT_STT_PWR_JAM_CLR,
	INIT_DEV_INIT_STT_PWR_NG_IDLE,
	INIT_DEV_INIT_STT_NRML_IDLE,
	INIT_DEV_INIT_STT_NRML_INIT,
	INIT_DEV_INIT_STT_NRML_JAM_CLR,
	INIT_DEV_INIT_STT_PAUSE,
	INIT_DEV_INIT_STT_RESET_IDLE,
	INIT_DEV_INIT_STT_NUM
} EInitDeviceInitState;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Member of Device Initialize Handler ---
typedef struct /*CInitDevInitHandlerMember*/
{
	const InitDevInitHandlerSpec* mpSpec;					// Pointer of Spec Data
	UC mState;												// Device Initialize State
	UC mPowerUpMode;										// Power Up Mode
	CSrvcMsgActivity* mpActivity;							// Pointer of Activity
} CInitDevInitHandlerMember;

static CInitDevInitHandlerMember gInit_DevInitHandlerMember;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void StartDeviceInitialize();
void StopDeviceInitialize();
void StartPowerOnInitialize();
void StopPowerOnInitialize();
void StartResumeInitialize();
void StopResumeInitialize();
void StartJamClearAssist();
void StopJamClearAssist();


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CInitDevInitHandler_Constructor
// Description   : 
// Parameter     : 
// Return        : Å]
///////////////////////////////////////////////////////////////////
void CInitDevInitHandler_Constructor(const InitDevInitHandlerSpec* pSpec)
{
	gInit_DevInitHandlerMember.mpSpec		= pSpec;
	gInit_DevInitHandlerMember.mState		= INIT_DEV_INIT_STT_PWR_IDLE;
	gInit_DevInitHandlerMember.mPowerUpMode	= SYS_PWR_UP_MODE_NORMAL;
	gInit_DevInitHandlerMember.mpActivity	= NULL;	
}

///////////////////////////////////////////////////////////////////
// Function Name : CInitDevInitHandler_Destructor
// Description   : 
// Parameter     : 
// Return        : Å]
///////////////////////////////////////////////////////////////////
void CInitDevInitHandler_Destructor()
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : StartDeviceInitialize
// Description   : 
// Parameter     : 
// Return        : Å]
///////////////////////////////////////////////////////////////////
void StartDeviceInitialize()
{
	gInit_DevInitHandlerMember.mpActivity = gInit_DevInitHandlerMember.mpSpec->pNormalInitActivity;
	CSrvcMsgActivity_Start(gInit_DevInitHandlerMember.mpActivity);
}

///////////////////////////////////////////////////////////////////
// Function Name : StopDeviceInitialize
// Description   : 
// Parameter     : 
// Return        : Å]
///////////////////////////////////////////////////////////////////
void StopDeviceInitialize()
{
	CSrvcMsgActivity_Terminate(gInit_DevInitHandlerMember.mpActivity);
	gInit_DevInitHandlerMember.mpActivity = NULL;
}

///////////////////////////////////////////////////////////////////
// Function Name : StartPowerOnInitialize
// Description   : 
// Parameter     : 
// Return        : Å]
///////////////////////////////////////////////////////////////////
void StartPowerOnInitialize()
{
	gInit_DevInitHandlerMember.mpActivity = gInit_DevInitHandlerMember.mpSpec->pPwrOnInitActivity;
	CSrvcMsgActivity_Start(gInit_DevInitHandlerMember.mpActivity);
}

///////////////////////////////////////////////////////////////////
// Function Name : StopPowerOnInitialize
// Description   : 
// Parameter     : 
// Return        : Å]
///////////////////////////////////////////////////////////////////
void StopPowerOnInitialize()
{
	CSrvcMsgActivity_Terminate(gInit_DevInitHandlerMember.mpActivity);
	gInit_DevInitHandlerMember.mpActivity = NULL;
}

///////////////////////////////////////////////////////////////////
// Function Name : StartResumeInitialize
// Description   : 
// Parameter     : 
// Return        : Å]
///////////////////////////////////////////////////////////////////
void StartResumeInitialize()
{
	gInit_DevInitHandlerMember.mpActivity = gInit_DevInitHandlerMember.mpSpec->pPwrRsmInitActivity;
	CSrvcMsgActivity_Start(gInit_DevInitHandlerMember.mpActivity);
}

///////////////////////////////////////////////////////////////////
// Function Name : StopResumeInitialize
// Description   : 
// Parameter     : 
// Return        : Å]
///////////////////////////////////////////////////////////////////
void StopResumeInitialize()
{
	CSrvcMsgActivity_Terminate(gInit_DevInitHandlerMember.mpActivity);
	gInit_DevInitHandlerMember.mpActivity = NULL;
}

///////////////////////////////////////////////////////////////////
// Function Name : StartJamClearAssist
// Description   : 
// Parameter     : 
// Return        : Å]
///////////////////////////////////////////////////////////////////
void StartJamClearAssist()
{
	gInit_DevInitHandlerMember.mpActivity = gInit_DevInitHandlerMember.mpSpec->pJamClrAssistActivity;
	CSrvcMsgActivity_Start(gInit_DevInitHandlerMember.mpActivity);
}

///////////////////////////////////////////////////////////////////
// Function Name : StopJamClearAssist
// Description   : 
// Parameter     : 
// Return        : Å]
///////////////////////////////////////////////////////////////////
void StopJamClearAssist()
{
	CSrvcMsgActivity_Terminate(gInit_DevInitHandlerMember.mpActivity);
	gInit_DevInitHandlerMember.mpActivity = NULL;
}


///////////////////////////////////////////////////////////////////
// Function Name : CInitDevInitHandler_GetStatus
// Description   : 
// Parameter     : 
// Return        : Å]
///////////////////////////////////////////////////////////////////
EInitInitializeState CInitDevInitHandler_GetState()
{
	static const US cRunningState = {
		(0x0001 << INIT_DEV_INIT_STT_PWR_INIT) |
		(0x0001 << INIT_DEV_INIT_STT_PWR_RSM) |
		(0x0001 << INIT_DEV_INIT_STT_PWR_JAM_CLR) |
		(0x0001 << INIT_DEV_INIT_STT_NRML_INIT) |
		(0x0001 << INIT_DEV_INIT_STT_NRML_JAM_CLR)
	};

	if ( gInit_DevInitHandlerMember.mState == INIT_DEV_INIT_STT_PAUSE ){
		return INIT_STT_PAUSE;
	}
	else{
		if ( cRunningState & (0x0001 << gInit_DevInitHandlerMember.mState) ) return INIT_STT_RUNNING;
		else																 return INIT_STT_IDLE;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CInitDevInitHandler_Reset
// Description   : 
// Parameter     : 
// Return        : Å]
///////////////////////////////////////////////////////////////////
bool CInitDevInitHandler_Reset()
{
	if ( (gInit_DevInitHandlerMember.mState == INIT_DEV_INIT_STT_PWR_NG_IDLE)||
		 (gInit_DevInitHandlerMember.mState == INIT_DEV_INIT_STT_NRML_IDLE) ){
		gInit_DevInitHandlerMember.mState = INIT_DEV_INIT_STT_RESET_IDLE;

		return true;
	}
	else{
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CInitDevInitHandler_Start
// Description   : 
// Parameter     : 
// Return        : Å]
///////////////////////////////////////////////////////////////////
bool CInitDevInitHandler_Start(ESysPowerUpMode mode)
{
	UC jamStts;

	gInit_DevInitHandlerMember.mPowerUpMode = mode;

	if( (CDvstInformer_GetDeviceCondtion(DEVID_FINISHER) != DEV_COND_PERM_NG) &&
		(CCvrChecker_GetCoverStatus(CVR_ID_NUM) != CVR_STTS_OPEN) ){

		CSysManager_DeviceInitializeStatusChange(INIT_STT_RUNNING);					// System ManagerÇ…äJéní ím

		jamStts = CJamChecker_GetJamZoneStatus(JAM_ZONE_ID_NUM);

		switch ( gInit_DevInitHandlerMember.mState ){
		case INIT_DEV_INIT_STT_PWR_IDLE:
			if( jamStts == JAM_ZONE_STTS_SET ){
				gInit_DevInitHandlerMember.mState = INIT_DEV_INIT_STT_PWR_JAM_CLR;
				StartJamClearAssist();	
			}
			else{
				if ( gInit_DevInitHandlerMember.mPowerUpMode == SYS_PWR_UP_MODE_SAVE ){
					gInit_DevInitHandlerMember.mState = INIT_DEV_INIT_STT_PWR_RSM;
					StartResumeInitialize();
				}
				else{
					gInit_DevInitHandlerMember.mState = INIT_DEV_INIT_STT_PWR_INIT;
					StartPowerOnInitialize();
				}
			}

			break;
		case INIT_DEV_INIT_STT_PWR_NG_IDLE:
			if( jamStts == JAM_ZONE_STTS_SET ){
				gInit_DevInitHandlerMember.mState = INIT_DEV_INIT_STT_PWR_JAM_CLR;
				StartJamClearAssist();
			}
			else{
				gInit_DevInitHandlerMember.mState = INIT_DEV_INIT_STT_PWR_INIT;
				StartPowerOnInitialize();
			}
		
			break;
		case INIT_DEV_INIT_STT_NRML_IDLE:
			if( jamStts == JAM_ZONE_STTS_SET ){
				gInit_DevInitHandlerMember.mState = INIT_DEV_INIT_STT_NRML_JAM_CLR;
				StartJamClearAssist();	
			}
			else{
				gInit_DevInitHandlerMember.mState = INIT_DEV_INIT_STT_NRML_INIT;
				StartDeviceInitialize();
			}

			break;
		case INIT_DEV_INIT_STT_RESET_IDLE:
			if( jamStts == JAM_ZONE_STTS_SET ){
				gInit_DevInitHandlerMember.mState = INIT_DEV_INIT_STT_PWR_JAM_CLR;
				StartJamClearAssist();	
			}
			else{
				gInit_DevInitHandlerMember.mState = INIT_DEV_INIT_STT_PWR_INIT;
				StartPowerOnInitialize();
			}

			break;
		default:
			CSysManager_DeviceInitializeStatusChange(INIT_STT_IDLE);				// Fail Soft

			return false;
		}

		return true;
	}
	else{
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CInitDevInitHandler_Stop
// Description   : 
// Parameter     : 
// Return        : Å]
///////////////////////////////////////////////////////////////////
bool CInitDevInitHandler_Stop()
{
	switch ( gInit_DevInitHandlerMember.mState ){
	case INIT_DEV_INIT_STT_PWR_INIT:
		StopPowerOnInitialize();
		gInit_DevInitHandlerMember.mState = INIT_DEV_INIT_STT_PWR_NG_IDLE;

		break;
	case INIT_DEV_INIT_STT_PWR_RSM:
		StopResumeInitialize();
		gInit_DevInitHandlerMember.mState = INIT_DEV_INIT_STT_PWR_NG_IDLE;

		break;
	case INIT_DEV_INIT_STT_NRML_INIT:
		StopDeviceInitialize();
		gInit_DevInitHandlerMember.mState = INIT_DEV_INIT_STT_PWR_NG_IDLE;

		break;
	case INIT_DEV_INIT_STT_PWR_JAM_CLR:
	case INIT_DEV_INIT_STT_NRML_JAM_CLR:
		StopJamClearAssist();
		gInit_DevInitHandlerMember.mState = INIT_DEV_INIT_STT_PWR_NG_IDLE;

		break;
	default:
		return false;
	}

	CSysManager_DeviceInitializeStatusChange(INIT_STT_IDLE);	

	return true;
}

///////////////////////////////////////////////////////////////////
// Function Name : CInitDevInitHandler_DeviceActionResponse
// Description   : 
// Parameter     : 
// Return        : Å]
///////////////////////////////////////////////////////////////////
void CInitDevInitHandler_DeviceActionResponse(ESysDeviceAction action, UC moduleID)
{
	if ( CInitDevInitHandler_GetState() == INIT_STT_RUNNING ){
		CSrvcMsgActivity_SetTrigger(gInit_DevInitHandlerMember.mpActivity, action, moduleID);

		if ( CSrvcMsgActivity_CheckEnd(gInit_DevInitHandlerMember.mpActivity) == true ){						// ÉVÅ[ÉPÉìÉXäÆóπ
			switch( gInit_DevInitHandlerMember.mState ){
			case INIT_DEV_INIT_STT_NRML_INIT:
			case INIT_DEV_INIT_STT_PWR_INIT:
			case INIT_DEV_INIT_STT_PWR_RSM:
				gInit_DevInitHandlerMember.mState = INIT_STT_PAUSE;
				CSysManager_DeviceInitializeStatusChange(INIT_STT_PAUSE);

				if ( CJamChecker_GetJamZoneStatus(JAM_ZONE_ID_NUM) == JAM_ZONE_STTS_SET ){
					gInit_DevInitHandlerMember.mState = INIT_DEV_INIT_STT_NRML_JAM_CLR;
					StartJamClearAssist();	
				}
				else{
					gInit_DevInitHandlerMember.mState = INIT_DEV_INIT_STT_NRML_IDLE;
					CSysManager_DeviceInitializeStatusChange(INIT_STT_IDLE);
				}
		
				break;
			case INIT_DEV_INIT_STT_NRML_JAM_CLR:
				gInit_DevInitHandlerMember.mState = INIT_DEV_INIT_STT_NRML_IDLE;
				CSysManager_DeviceInitializeStatusChange(INIT_STT_IDLE);

				break;
			case INIT_DEV_INIT_STT_PWR_JAM_CLR:
				gInit_DevInitHandlerMember.mState = INIT_DEV_INIT_STT_PWR_NG_IDLE;
				CSysManager_DeviceInitializeStatusChange(INIT_STT_IDLE);

				break;
			default:																					// Idle
				break;
			}
		}
		else{
			CSrvcMsgActivity_Progress(gInit_DevInitHandlerMember.mpActivity);
		}
	}
	else{
		// No Action
	}
}

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

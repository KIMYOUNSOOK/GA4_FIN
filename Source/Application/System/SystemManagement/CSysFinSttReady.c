// File Name:	CSysFinSttReady.c
// Description:	Implimentation File of Finisher State "Ready"
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CSysFinSttReady.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Public Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Public method table of Ready State ---
CSysFinisherState const gSys_FinSttReadyMethod = {
	CSysFinSttReady_Constructor,							// Constructor
	CSysFinisherState_StdIsTransitionState,					// IsTransitionState
	CSysFinSttReady_CheckTransitionPossibility,				// CheckTransitionPossibility
	CSysFinisherState_StdCheckExitCondition,				// CheckExitCondition
	CSysFinisherState_StdGetNextState,						// GetNextState
	CSysFinSttReady_Entry,									// Entry
	CSysFinisherState_StdExit,								// Exit
	CSysFinisherState_StdSendDeviceActionRequest,			// SendDeviceActionRequest
	CSysFinisherState_StdSendDeviceEventNotify,				// SendDeviceEventNotify
	CSysFinisherState_StdReceivePowerUpResponse,			// ReceivePowerUpResponse
	CSysFinisherState_StdReceivePowerDownResponse,			// ReceivePowerDownResponse
	CSysFinisherState_StdReceiveChangeDiagResponse,			// ReceiveChangeDiagResponse
	CSysFinisherState_StdReceiveChangeNormalResponse,		// ReceiveChangeNormalResponse
	CSysFinisherState_StdReceiveCycleUpResponse,			// ReceiveCycleUpResponse
	CSysFinisherState_StdReceiveProcessDownResponse,		// ReceiveProcessDownResponse
	CSysFinisherState_StdReceiveDeviceActionResponse,		// ReceiveDeviceActionResponse
	CSysFinisherState_StdReceiveDeviceEventNotify,			// ReceiveDeviceEventNotify
	CSysFinisherState_StdReceiveDeviceStatusNotify,			// ReceiveDeviceStatusNotify
	CSysFinisherState_StdSystemConfigurationSet,			// SystemConfigurationSet
	CSysFinisherState_StdPowerUpModeSet,					// PowerUpModeSet
	CSysFinisherState_StdEntranceAreaTampered,				// EntranceAreaTampered
	CSysFinisherState_StdLoadNvmComplete,					// LoadNvmComplete
	CSysFinisherState_StdSaveNvmComplete,					// SaveNvmComplete
	CSysFinisherState_StdSheetManagerReady,					// SheetManagerReady
	CSysFinisherState_StdCoverStatusChange,					// CoverStatusChange
	CSysFinisherState_StdFaultStatusChange,					// FaultStatusChange
	CSysFinisherState_StdJamZoneStatusChange				// JamZoneStatusChange
};


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttReady_Constructor
// Description   : ÉRÉìÉXÉgÉâÉNÉ^
// Parameter     : Å]
// Return        : Å]
///////////////////////////////////////////////////////////////////
void CSysFinSttReady_Constructor()
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttReady_CheckTransitionPossibility
// Description   : èÛë‘ëJà⁄â¬î€ämîFèàóù
// Parameter     : nextState (ëJà⁄êÊèÛë‘)
// Return        : èÛë‘ëJà⁄â¬î€
//               :  true = ëJà⁄â¬î\
//               :  false = ëJà⁄ïtâ¡
///////////////////////////////////////////////////////////////////
bool CSysFinSttReady_CheckTransitionPossibility(EFinisherState nextState)
{
	static US const cTransPsbl = {
		FIN_STT_CNV_BIT(FIN_STT_CYCLE_DOWN)|
		FIN_STT_CNV_BIT(FIN_STT_HARD_DOWN)
	};

	if ( cTransPsbl & FIN_STT_CNV_BIT(nextState) ) return true;
	else										   return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSysFinSttReady_Entry
// Description   : åªèÛë‘Ç÷ÇÃëJà⁄éûèàóù
// Parameter     : prevState (ëJà⁄å≥ÉtÉBÉjÉbÉVÉÉÅ[èÛë‘)
// Return        : Å]
///////////////////////////////////////////////////////////////////
void CSysFinSttReady_Entry(EFinisherState prevState)
{
	MSP_SendMessage(MID_SYS, EVT_STT_READY_NOTIFY, NULL, 0, LOG_OFF);
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.04.08	| Yuichi Hoshino	| Created this file
// 14.12.04	| Yuichi Hoshino	| Release V0 (Revision 00)
// 15.03.30	| Yuichi Hoshino	| Modify By Introduction Spec [ NVM ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//

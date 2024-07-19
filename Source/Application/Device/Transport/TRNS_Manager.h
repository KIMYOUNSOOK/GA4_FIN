// File Name:	CTraModuleManager.h
// Description:	Header File of Transport Module Manager
// Engineer:	Yuichi Hoshino
// Last Edit:	14.09.03
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef TRNS_Manager_h
#define TRNS_Manager_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"
#include "CJamChecker.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Const Number Definition
/////////////////////////////////////////////////////////////////////////////////////////
// --- Module State Definition ---
typedef enum /*ETraManagerState*/
{
	TRNS_MGR_STT_UNKNOWN, 	//0
	TRNS_MGR_STT_POWER_UP, 	// 1
	TRNS_MGR_STT_POWER_ON,	// 2
	TRNS_MGR_STT_POWER_DOWN,	// 3
	TRNS_MGR_STT_POWER_OFF,	// 4
	TRNS_MGR_STT_CHANGE_DIAG,// 5
	TRNS_MGR_STT_DIAG,			// 6
	TRNS_MGR_STT_CHANGE_NORMAL,// 7
	TRNS_MGR_STT_STANDBY,		// 8
	TRNS_MGR_STT_CYCLEUP,		// 9
	TRNS_MGR_STT_READY,		// 10
	TRNS_MGR_STT_CYCLE_DOWN, // 11
	TRNS_MGR_STT_HARD_DOWN,	// 12
	TRNS_MGR_STT_NUM
} ETraManagerState;


// --- Transport Module Control Parameter ---
#define TRNS_JAM_CHECK_EXECUTE_TIME							390					// [?1ms]
#define TRNS_JAM_CLEAR_ASSIST_EXECUTE_TIME					400					// [?1ms]

// --- Transport Module Controller State ---
typedef enum /*ETraControllerState*/
{
	TRNS_CONT_STT_IDLE,		// Idle
	TRNS_CONT_STT_RUNNING,	// Running
	// TRNS_CONT_STT_IDLE,		// Idle
	// TRNS_CONT_STT_RUNNING,	// Running
	TRNS_CONT_STT_NUM
} ETraControllerState;

// --- Controller ID of Transport Module ---
typedef enum /*ETraControllerID*/
{
	TRNS_CONT_ID_ENTRANCE,	// Entrance Controller
	TRNS_CONT_ID_EXIT,		// Transport Controller
	TRNS_CONT_ID_NUM			// Number of Controller ID
} ETraControllerID;


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void TRNS_MGR_Constructor();

void TRNS_MGR_StartUp();
void TRNS_MGR_PowerUp();
void TRNS_MGR_PowerDown();
void TRNS_MGR_ChangeDiag();
void TRNS_MGR_ChangeNormal();
void TRNS_MGR_CycleUp();
void TRNS_MGR_HardDown();
void TRNS_MGR_CycleDown();
void TRNS_MGR_JamCheckRequest();
void TRNS_MGR_ProcessEndRequest();
void TRNS_MGR_JamClearAssistRequest();
void TRNS_MGR_AnyCoverOpenNotify();
void TRNS_MGR_AllCoverCloseNotify();
void TRNS_MGR_FinisherConditionNotify();
void TRNS_MGR_SheetAbortedNotify();
void TRNS_MGR_ComponentControlStartNotify();
void TRNS_MGR_ComponentControlStopNotify();
void TRNS_MGR_JamCheckComplete(ETraControllerID contID);
void TRNS_MGR_JamClearAssistComplete(ETraControllerID contID);
void TRNS_MGR_SheetControlComplete(ETraControllerID contID, UC sheetNo);
void TRNS_InformedCompoControlComplete(US aChainNo, US aLinkNo, EAction aAction);
void TRNS_MGR_ComponentControl(const UC* evt, EAction aAction);
UC TRNS_MGR_GetState(void);


#endif // #ifndef TRNS_MGR_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

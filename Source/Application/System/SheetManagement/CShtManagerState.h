// File Name:	CShtManagerState.h
// Description:	Header File of Sheet Manager State
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.20
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CShtManagerState_h
#define CShtManagerState_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Const Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Sheet Manager State ---
typedef enum /*EShtManagerState*/
{
	SHT_MGR_STT_STANDBY,				// Standby
	SHT_MGR_STT_CYCLE_UP,				// Cycle Up
	SHT_MGR_STT_READY,					// Ready
	SHT_MGR_STT_CYCLE_DOWN,				// Cycle Down
	SHT_MGR_STT_PURGE_DOWN,				// Purge Cycle Down
	SHT_MGR_STT_BLOCK_DOWN,				// Block Cycle Down
	SHT_MGR_STT_NUM						// Number of Sheet Manager state
} EShtManagerState;


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Sheet Manager State ---
typedef struct /*CShtManagerState*/
{
	UC mState;
} CShtManagerState;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CShtManagerState_Constructor(CShtManagerState* pThis);

#define CShtManagerState_GetState(apThis) ((EShtManagerState)((apThis)->mState))// EShtManagerState (CShtManagerState*)
bool CShtManagerState_ChangeStandby(CShtManagerState* pThis);
bool CShtManagerState_ChangeCycleUp(CShtManagerState* pThis);
bool CShtManagerState_ChangeReady(CShtManagerState* pThis);
bool CShtManagerState_ChangeCycleDown(CShtManagerState* pThis);
bool CShtManagerState_ChangePurgeDown(CShtManagerState* pThis);
bool CShtManagerState_ChangeBlockDown(CShtManagerState* pThis);


#endif // #ifndef CShtManagerState_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

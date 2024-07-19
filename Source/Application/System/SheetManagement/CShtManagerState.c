// File Name:	CShtManagerState.c
// Description:	Implimentation File of Sheet Manager State
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.20
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CShtManagerState.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- State Change Table ---
static const UC cSht_ShtMgrSttChgTbl[SHT_MGR_STT_NUM][SHT_MGR_STT_NUM] = {
//	| Standby				| Cycle Up				| Ready					| Cycle Down			| Purge Cycle Down		| Block Cycle Down
	{ SHT_MGR_STT_STANDBY	, SHT_MGR_STT_CYCLE_UP	, SHT_MGR_STT_STANDBY	, SHT_MGR_STT_STANDBY	, SHT_MGR_STT_STANDBY	, SHT_MGR_STT_STANDBY	},	// Standby
	{ SHT_MGR_STT_CYCLE_UP	, SHT_MGR_STT_CYCLE_UP	, SHT_MGR_STT_READY		, SHT_MGR_STT_CYCLE_DOWN, SHT_MGR_STT_PURGE_DOWN, SHT_MGR_STT_BLOCK_DOWN},	// Cycle Up
	{ SHT_MGR_STT_READY		, SHT_MGR_STT_READY		, SHT_MGR_STT_READY		, SHT_MGR_STT_CYCLE_DOWN, SHT_MGR_STT_PURGE_DOWN, SHT_MGR_STT_BLOCK_DOWN},	// Ready
	{ SHT_MGR_STT_STANDBY	, SHT_MGR_STT_CYCLE_DOWN, SHT_MGR_STT_CYCLE_DOWN, SHT_MGR_STT_CYCLE_DOWN, SHT_MGR_STT_PURGE_DOWN, SHT_MGR_STT_BLOCK_DOWN},	// Cycle Down
	{ SHT_MGR_STT_STANDBY	, SHT_MGR_STT_PURGE_DOWN, SHT_MGR_STT_PURGE_DOWN, SHT_MGR_STT_PURGE_DOWN, SHT_MGR_STT_PURGE_DOWN, SHT_MGR_STT_BLOCK_DOWN},	// Purge Cycle Down
	{ SHT_MGR_STT_STANDBY	, SHT_MGR_STT_BLOCK_DOWN, SHT_MGR_STT_BLOCK_DOWN, SHT_MGR_STT_BLOCK_DOWN, SHT_MGR_STT_BLOCK_DOWN, SHT_MGR_STT_BLOCK_DOWN}	// Block Cycle Down
};

/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

static bool ChangeState(CShtManagerState* pThis, EShtManagerState changeState);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CShtManagerState_Constructor
// Description   : コンストラクタ
// Parameter     : pThis (インスタンスポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtManagerState_Constructor(CShtManagerState* pThis)
{
	pThis->mState = SHT_MGR_STT_STANDBY;
}

///////////////////////////////////////////////////////////////////
// Function Name : ChangeState
// Description   : 状態変更処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
bool ChangeState(CShtManagerState* pThis, EShtManagerState changeState)
{
	UC crntStt;

	crntStt = pThis->mState;

	pThis->mState = cSht_ShtMgrSttChgTbl[pThis->mState][changeState];

	if ( pThis->mState != crntStt ){
		return true;
	}
	else{ /* No Change */ }

	return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManagerState_ChangeStandby
// Description   : Standby遷移指示受信処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
bool CShtManagerState_ChangeStandby(CShtManagerState* pThis)
{
	return ChangeState(pThis, SHT_MGR_STT_STANDBY);
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManagerState_ChangeCycleUp
// Description   : Cycle Up遷移指示受信処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
bool CShtManagerState_ChangeCycleUp(CShtManagerState* pThis)
{
	return ChangeState(pThis, SHT_MGR_STT_CYCLE_UP);
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManagerState_ChangeReady
// Description   : Ready遷移指示受信処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
bool CShtManagerState_ChangeReady(CShtManagerState* pThis)
{
	return ChangeState(pThis, SHT_MGR_STT_READY);
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManagerState_ChangeCycleDown
// Description   : Cycle Down遷移指示受信処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
bool CShtManagerState_ChangeCycleDown(CShtManagerState* pThis)
{
	return ChangeState(pThis, SHT_MGR_STT_CYCLE_DOWN);
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManagerState_ChangePurgeDown
// Description   : Purge Down遷移指示受信処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
bool CShtManagerState_ChangePurgeDown(CShtManagerState* pThis)
{
	return ChangeState(pThis, SHT_MGR_STT_PURGE_DOWN);
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtManagerState_ChangeBlockDown
// Description   : Block Down遷移指示受信処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
bool CShtManagerState_ChangeBlockDown(CShtManagerState* pThis)
{
	return ChangeState(pThis, SHT_MGR_STT_BLOCK_DOWN);
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

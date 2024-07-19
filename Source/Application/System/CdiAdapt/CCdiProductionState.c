// File Name:	CCdiProductionState.c
// Description:	Implimentation File of Production State
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.24
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CCdiProductionState.h"


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
// Function Name : CCdiProductionState_Constructor
// Description   : コンストラクタ
// Parameter     : pThis (インスタンスポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiProductionState_Constructor(CCdiProductionState* pThis)
{
	pThis->mState = eCdiFinPrdctStt_Unknown;
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiProductionState_GetState
// Description   : 状態取得処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : Production State
///////////////////////////////////////////////////////////////////
UC CCdiProductionState_GetState(CCdiProductionState* pThis)
{
	return pThis->mState;
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiProductionState_InformedEvent
// Description   : 状態遷移イベント通知受信処理
// Parameter     : pThis (インスタンスポインタ)
//               : event (状態遷移イベント識別子)
// Return        : true = 状態遷移あり
//               : false = 状態遷移なし
///////////////////////////////////////////////////////////////////
bool CCdiProductionState_InformedEvent(CCdiProductionState* pThis, ECdiProductSttEvt event)
{
//															| InitStart Event			| InitComp Event			| CycleUpStart Event		| ChangeStandby Event
	static const UC cInitChg[CDI_PRDCT_STT_EVT_NUM] = {		eCdiFinPrdctStt_Init,		eCdiFinPrdctStt_Standby,	eCdiFinPrdctStt_Init,		eCdiFinPrdctStt_Init	};
	static const UC cStandbyChg[CDI_PRDCT_STT_EVT_NUM] = {	eCdiFinPrdctStt_Standby,	eCdiFinPrdctStt_Standby,	eCdiFinPrdctStt_Running,	eCdiFinPrdctStt_Standby	};
	static const UC cRunningChg[CDI_PRDCT_STT_EVT_NUM] = {	eCdiFinPrdctStt_Running,	eCdiFinPrdctStt_Running,	eCdiFinPrdctStt_Running,	eCdiFinPrdctStt_Standby	};
	static const UC cUnknownChg[CDI_PRDCT_STT_EVT_NUM] = {	eCdiFinPrdctStt_Init,		eCdiFinPrdctStt_Unknown,	eCdiFinPrdctStt_Unknown,	eCdiFinPrdctStt_Unknown	};

	UC currentState = pThis->mState;

	if ( event < CDI_PRDCT_STT_EVT_NUM ){
		switch ( pThis->mState ){
		case eCdiFinPrdctStt_Init:		pThis->mState = cInitChg[event];	break;
		case eCdiFinPrdctStt_Standby:	pThis->mState = cStandbyChg[event];	break;
		case eCdiFinPrdctStt_Running:	pThis->mState = cRunningChg[event];	break;
		case eCdiFinPrdctStt_Unknown:	pThis->mState = cUnknownChg[event];	break;
		default:						/* No Action */						break;
		};
	}
	else{ /* No Action */ }

	if ( currentState != pThis->mState ) return true;							// State change
	else								 return false;							// State don't change
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

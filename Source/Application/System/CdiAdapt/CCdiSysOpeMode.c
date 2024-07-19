// File Name:	CCdiSysOpeMode.c
// Description:	Implimentation File of System Operating Mode
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.24
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CCdiSysOpeMode.h"


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
// Function Name : CCdiSysOpeMode_Constructor
// Description   : コンストラクタ
// Parameter     : pThis (インスタンスポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CCdiSysOpeMode_Constructor(CCdiSysOpeMode* pThis)
{
	pThis->mMode = eCdiFinSysOpeMode_Unknown;
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiSysOpeMode_GetMode
// Description   : モード取得処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : System Operating Mode
///////////////////////////////////////////////////////////////////
UC CCdiSysOpeMode_GetMode(CCdiSysOpeMode* pThis)
{
	return pThis->mMode;
}

///////////////////////////////////////////////////////////////////
// Function Name : CCdiSysOpeMode_InformedEvent
// Description   : 状態遷移イベント通知受信処理
// Parameter     : pThis (インスタンスポインタ)
//               : event (状態遷移イベント識別子)
// Return        : true = 状態遷移あり
//               : false = 状態遷移なし
///////////////////////////////////////////////////////////////////
bool CCdiSysOpeMode_InformedEvent(CCdiSysOpeMode* pThis, ECdiSysOpeModeEvt event)
{
//																	| ChgNormal Event			| ChgServiceDiag Event			| ChgDownload Event
	static const UC cNormalChg[CDI_SYS_OPE_MODE_EVT_NUM] = {		eCdiFinSysOpeMode_Nrml,		eCdiFinSysOpeMode_SrvcDiag,		eCdiFinSysOpeMode_Nrml		};
	static const UC cServiceDiagChg[CDI_SYS_OPE_MODE_EVT_NUM] = {	eCdiFinSysOpeMode_Nrml,		eCdiFinSysOpeMode_SrvcDiag,		eCdiFinSysOpeMode_SoftDL	};
	static const UC cDownloadChg[CDI_SYS_OPE_MODE_EVT_NUM] = {		eCdiFinSysOpeMode_SoftDL,	eCdiFinSysOpeMode_SoftDL,		eCdiFinSysOpeMode_SoftDL	};
	static const UC cUnknownChg[CDI_SYS_OPE_MODE_EVT_NUM] = {		eCdiFinSysOpeMode_Nrml,		eCdiFinSysOpeMode_SrvcDiag,		eCdiFinSysOpeMode_SoftDL	};

	UC currentMode = pThis->mMode;

	if ( event < CDI_SYS_OPE_MODE_EVT_NUM ){
		switch ( pThis->mMode ){
		case eCdiFinSysOpeMode_Nrml:		pThis->mMode = cNormalChg[event];		break;
		case eCdiFinSysOpeMode_SrvcDiag:	pThis->mMode = cServiceDiagChg[event];	break;
		case eCdiFinSysOpeMode_SoftDL:		pThis->mMode = cDownloadChg[event];		break;
		case eCdiFinSysOpeMode_Unknown:		pThis->mMode = cUnknownChg[event];		break;
		default:							/* No Action */							break;
		};
	}
	else{ /* No Action */ }

	if ( currentMode != pThis->mMode ) return true;								// Mode change
	else							   return false;
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// File Name:	CSeqShtInitHandler.c
// Description:	Implimentation File of Sheet Initialize Handler
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.22
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CSeqShtInitHandler.h"

#include "CShtManager.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Const Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void StartInitialize(CSeqShtInitHandler* pThis);
void StopInitialize(CSeqShtInitHandler* pThis);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CSeqShtInitHandler_Constructor
// Description   : コンストラクタ
// Parameter     : pThis (インスタンスポインタ)
//               : pSpec (スペックデータポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CSeqShtInitHandler_Constructor(CSeqShtInitHandler* pThis, const SeqShtInitHandlerSpec* pSpec)
{
	pThis->mpSpec			   = pSpec;
	pThis->mEventParam.sheetNo = SHEETNO_NUM;
	pThis->mState			   = SEQ_ACT_STT_IDLE;

	CSrvcMsgActivity_Constructor(&(pThis->mActivity), MID_SYS, pThis->mpSpec->msgActivityNum, pThis->mpSpec->pMsgActivity, pThis->mpSpec->TimerCallBack);
}

///////////////////////////////////////////////////////////////////
// Function Name : StartInitialize
// Description   : イニシャライズ動作開始処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void StartInitialize(CSeqShtInitHandler* pThis)
{
	CSrvcMsgActivity_SetData(&(pThis->mActivity), (UC *)&(pThis->mEventParam), sizeof(EvtParamSheetActionRequest));

	CSrvcMsgActivity_Start(&(pThis->mActivity));
}

///////////////////////////////////////////////////////////////////
// Function Name : StopInitialize
// Description   : イニシャライズ動作停止処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void StopInitialize(CSeqShtInitHandler* pThis)
{
	CSrvcMsgActivity_Terminate(&(pThis->mActivity));
}

///////////////////////////////////////////////////////////////////
// Function Name : CSeqShtInitHandler_GetState
// Description   : シートイニシャライズ動作状態取得処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : シートイニシャライズ動作状態
///////////////////////////////////////////////////////////////////
ESeqActionState CSeqShtInitHandler_GetState(CSeqShtInitHandler* pThis)
{
	return (ESeqActionState)pThis->mState;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSeqShtInitHandler_GetSheetNo
// Description   : シートイニシャライズ動作制御対象シートのSheet 
//               : No. 取得処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : Sheet No.
///////////////////////////////////////////////////////////////////
UC CSeqShtInitHandler_GetSheetNo(CSeqShtInitHandler* pThis)
{
	return pThis->mEventParam.sheetNo;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSeqShtInitHandler_Start
// Description   : シートイニシャライズ動作開始指示受信処理
// Parameter     : pThis (インスタンスポインタ)
//               : sheetNo (Sheet No.)
// Return        : SEQ_ACT_STT_NUM = イニシャライズ動作状態変化なし
//               : SEQ_ACT_STT_NUM 以外 = 遷移先イニシャライズ動作
//               : 状態
///////////////////////////////////////////////////////////////////
ESeqActionState CSeqShtInitHandler_Start(CSeqShtInitHandler* pThis, UC sheetNo)
{
	UC stateChange;

	stateChange = SEQ_ACT_STT_NUM;

	if ( pThis->mState == SEQ_ACT_STT_IDLE ){
		pThis->mState			   = SEQ_ACT_STT_RUNNING;
		pThis->mEventParam.sheetNo = sheetNo;
		StartInitialize(pThis);

		stateChange = SEQ_ACT_STT_RUNNING;
	}
	else{
		// No Action
	}
	SRVC_INITManagerLog_Wide(0xC3, pThis->mState, (US)stateChange, (US)pThis->mEventParam.sheetNo);

	return stateChange;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSeqShtInitHandler_Stop
// Description   : シートイニシャライズ動作停止指示受信処理
// Parameter     : pThis (インスタンスポインタ)
//               : sheetNo (Sheet No.)
// Return        : SEQ_ACT_STT_NUM = イニシャライズ動作状態変化なし
//               : SEQ_ACT_STT_NUM 以外 = 遷移先イニシャライズ動作
//               : 状態
///////////////////////////////////////////////////////////////////
ESeqActionState CSeqShtInitHandler_Stop(CSeqShtInitHandler* pThis, UC sheetNo)
{
	UC stateChange;

	stateChange = SEQ_ACT_STT_NUM;

	if ( (pThis->mState == SEQ_ACT_STT_RUNNING) &&
		 ( (sheetNo == pThis->mEventParam.sheetNo) || (sheetNo == SHEETNO_NUM) ) ){
		// --- アクティビティ制御中止 ---(V3.4.0 Merge)
		CSrvcMsgActivity_Abort(&(pThis->mActivity));
	}
	else{
		// No Action
	}
	SRVC_INITManagerLog_Wide(0xC4, pThis->mState, (US)stateChange, (US)pThis->mEventParam.sheetNo);

	return stateChange;
}

///////////////////////////////////////////////////////////////////
// Function Name : CSeqShtInitHandler_SheetActionResponse
// Description   : Sheet Action Responseイベント受信時処理
// Parameter     : pThis (インスタンスポインタ)
//               : sheetNo (Sheet No.)
//               : eventID (MSP Event ID)
//               : moduleID (Device Module ID)
// Return        : SEQ_ACT_STT_NUM = イニシャライズ動作状態変化なし
//               : SEQ_ACT_STT_NUM 以外 = 遷移先イニシャライズ動作
//               : 状態
///////////////////////////////////////////////////////////////////
ESeqActionState CSeqShtInitHandler_SheetActionResponse(CSeqShtInitHandler* pThis, UC sheetNo, US eventID, UC moduleID)
{
	UC stateChange;

	stateChange = SEQ_ACT_STT_NUM;

	if ( (pThis->mEventParam.sheetNo == sheetNo) &&
		 (pThis->mState == SEQ_ACT_STT_RUNNING) ){
		CSrvcMsgActivity_SetTrigger(&(pThis->mActivity), eventID, moduleID);

		if ( CSrvcMsgActivity_CheckEnd(&(pThis->mActivity)) == true ){			// シーケンス完了
			pThis->mEventParam.sheetNo = SHEETNO_NUM;
			pThis->mState			   = SEQ_ACT_STT_IDLE;

			stateChange = SEQ_ACT_STT_IDLE;
		}
		else{
			CSrvcMsgActivity_Progress(&(pThis->mActivity));
		}
	}
	else{
		// No Action
	}

	return stateChange;
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// File Name: SRVC_MsgActivity.cpp
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 14.05.14
// Revision:  000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#include "FinisherCommon.h"
#include "MSP_Service.h"
#include "SRVC_MsgActivity.h"

//==============================================================================
// Function Name : CSrvcMsgActivity_Constructor
// Description   : コンストラクタ
// Parameter     : 
// Return        : -
//==============================================================================
void CSrvcMsgActivity_Constructor(CSrvcMsgActivity* pThis, UC userMID, UC tableSize, SrvcMsgActivity* pTable, void (*pCallBack)(SS timerID, UL idx))
{
	pThis->mUserMID = userMID;
	pThis->mTableSize = tableSize;
	pThis->mCrntPattern = 0;
	pThis->mStartPattern = 0;
	pThis->mIsActive = false;
	pThis->mpMsgData = NULL;
	pThis->mMsgDataSize = 0;
	pThis->mpTable = pTable;
	pThis->mpTimerCallBack = pCallBack;
}

//==============================================================================
// Function Name : CSrvcMsgActivity::Start
// Description   : 関連パラメータを初期化してActivityを開始させる
// Parameter     : -
// Return        : -
//==============================================================================
void CSrvcMsgActivity_Start(CSrvcMsgActivity* pThis)
{ 
	pThis->mCrntPattern = 0;
	pThis->mStartPattern = 0;
	pThis->mIsActive = true;
	CSrvcMsgActivity_Progress(pThis);
}

//==============================================================================
// Function Name : CSrvcMsgActivity::Progress
// Description   : Activityを進行させる
// Parameter     : -
// Return        : -
//==============================================================================
void CSrvcMsgActivity_Progress(CSrvcMsgActivity* pThis)
{
	UC idx;

	if (pThis->mIsActive) {
		for (idx = 0; idx < pThis->mTableSize; idx++) {
			if ((pThis->mStartPattern & SRVC_ID2BIT(idx)) == 0) {										// Actionが未開始の場合
				if ((pThis->mCrntPattern & pThis->mpTable[idx].startTrgPattern) == pThis->mpTable[idx].startTrgPattern) {
																										// 開始条件成立
					pThis->mStartPattern |= SRVC_ID2BIT(idx);

					if (pThis->mpTable[idx].pWaitTime != 0) {											// 待ち時間あり
						MSP_SetEventTimer(*(pThis->mpTable[idx].pWaitTime), pThis->mpTimerCallBack, idx);
					}
					else {																				// 待ち時間なし
						if (pThis->mpTable[idx].sendEventID != MSP_EVT_NUM_VOID) {
							MSP_SendMessage(pThis->mUserMID, pThis->mpTable[idx].sendEventID, (const UC*)(pThis->mpMsgData), pThis->mMsgDataSize, LOG_ON);
						}
					}
				}
				else {/* 開始条件不成立であれば何もしない */}
			}
			else {/* 既にActionが完了しているので何もしない */}
		}
	}
}

//==============================================================================
// Function Name : CSrvcMsgActivity::SetTrigger
// Description   : 受信トリガを設定する
// Parameter     : recvEventID	"受信トリガID"
//               : senderID		"送信元のモジュールID"
// Return        : -
//==============================================================================
void CSrvcMsgActivity_SetTrigger(CSrvcMsgActivity* pThis, US recvEventID, UC senderID)
{
	UC idx;

	if (pThis->mIsActive) {
		for (idx = 0; idx < pThis->mTableSize; idx++) {
			if (( recvEventID == pThis->mpTable[idx].recvEventID ) 
				&& ( senderID == pThis->mpTable[idx].senderID )){
				pThis->mCrntPattern |= SRVC_ID2BIT(idx);
				break;
			}
		}
	}
}

//==============================================================================
// Function Name : CSrvcMsgActivity::TimeOut
// Description   : メッセージ送信待ち時間後メッセージを送信する
// Parameter     : idx	"Activityテーブルのインデックス"
// Return        : -
//==============================================================================
void CSrvcMsgActivity_TimeOut(CSrvcMsgActivity* pThis, UL idx)
{
	if (pThis->mIsActive) {
		if (pThis->mpTable[idx].sendEventID != MSP_EVT_NUM_VOID) {
			MSP_SendMessage(pThis->mUserMID, pThis->mpTable[(UC)idx].sendEventID, (const UC*)pThis->mpMsgData, pThis->mMsgDataSize, LOG_ON);
		}
	}
}

//==============================================================================
// Function Name : CSrvcMsgActivity::CheckEnd
// Description   : Activityの完了を確認する
// Parameter     : -
// Return        : 完了:true / 未完了:false
//==============================================================================
bool CSrvcMsgActivity_CheckEnd(CSrvcMsgActivity* pThis)
{
	if (pThis->mIsActive) {
//		return (mCrntPattern == pThis->mEndPattern);
		return (pThis->mCrntPattern == SRVC_END_BIT_PATTERN(pThis->mTableSize));
	}
	else {
		return true;
	}
}

//==============================================================================
// Function Name : CSrvcMsgActivity::CSrvcMsgActivity_Abort
// Description   : Activityを中止させる
// Parameter     : -
// Return        : -
//==============================================================================
void CSrvcMsgActivity_Abort(CSrvcMsgActivity* pThis)
{
	UL pattern;

	if (pThis->mIsActive) {
		pThis->mCrntPattern |= (~(pThis->mStartPattern) & SRVC_END_BIT_PATTERN(pThis->mTableSize));
	}
}

//==============================================================================
// Function Name : CSrvcMsgActivity::Terminate
// Description   : Activityを終了させる
// Parameter     : -
// Return        : -
//==============================================================================
void CSrvcMsgActivity_Terminate(CSrvcMsgActivity* pThis)
{ 
	pThis->mIsActive = false;
}

//==============================================================================
// Function Name : CSrvcMsgActivity::SetData
// Description   : メッセージデータをセットする。
// Parameter     : -
// Return        : -
//==============================================================================
void CSrvcMsgActivity_SetData(CSrvcMsgActivity* pThis, UC* pData, UC size)
{ 
	pThis->mpMsgData	= pData;
	pThis->mMsgDataSize = size;
}
// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+-----------------------------------------------------------------------------------------------------------------------------
// 14.05.14 | Nishizawa Mitsuyuki | Create This File.
// 15.06.19	| Yuichi Hoshino      | Trouble Action [ Abort指示後にアクティビティ停止せず ]
// 15.12.24	| Yuichi Hoshino      | Design Improvement [ MSP_SendMessage()のログをオフに変更 ]


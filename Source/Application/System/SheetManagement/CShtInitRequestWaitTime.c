// File Name:	CShtInitRequestWaitTime.c
// Description:	Implimentation File of Sheet Initialize Request(Sheet Finish Request) Wait Time
// Engineer:	Yuichi Hoshino
// Last Edit:	15.01.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CShtInitRequestWaitTime.h"


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
// Function Name : CShtInitRequestWaitTime_Constructor
// Description   : コンストラクタ
// Parameter     : pThis (インスタンスポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtInitRequestWaitTime_Constructor(CShtInitRequestWaitTime *pThis)
{
	CStopWatch_CStopWatch(&(pThis->mStopWatch));

	CShtInitRequestWaitTime_Reset(pThis);
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtInitRequestWaitTime_Reset
// Description   : リセット処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtInitRequestWaitTime_Reset(CShtInitRequestWaitTime *pThis)
{
	UL elapsedTime;

	CStopWatch_Stop(&(pThis->mStopWatch), &elapsedTime);

	pThis->mState	= false;
	pThis->mSheetNo = SHEETNO_NUM;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtInitRequestWaitTime_SetWaitSheet
// Description   : 待ち対象シート設定処理
// Parameter     : pThis (インスタンスポインタ)
//               : sheetNo (Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtInitRequestWaitTime_SetWaitSheet(CShtInitRequestWaitTime *pThis, UC sheetNo)
{
	if ( pThis->mSheetNo != sheetNo ){
		pThis->mSheetNo = sheetNo;
		pThis->mState = false;
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtInitRequestWaitTime_StartWaiting
// Description   : 待ち開始処理
// Parameter     : pThis (インスタンスポインタ)
//               : sheetNo (Sheet No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtInitRequestWaitTime_StartWaiting(CShtInitRequestWaitTime *pThis, UC sheetNo)
{
	if ( (pThis->mSheetNo == sheetNo) && (pThis->mState == false) ){
		CStopWatch_Start(&(pThis->mStopWatch));
		pThis->mSheetNo = sheetNo;
		pThis->mState = true;
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtInitRequestWaitTime_GetWaitTime
// Description   : 待ち時間取得処理
// Parameter     : pThis (インスタンスポインタ)
//               : sheetNo (Sheet No.)
// Return        : 待ち時間[×1ms]
///////////////////////////////////////////////////////////////////
UL CShtInitRequestWaitTime_GetWaitTime(CShtInitRequestWaitTime *pThis, UC sheetNo){
	UL elapsedTime;
	
	if ( (pThis->mState == true) && (pThis->mSheetNo == sheetNo) && (pThis->mSheetNo != SHEETNO_NUM) ){
		CStopWatch_Split(&(pThis->mStopWatch), &elapsedTime);
		return elapsedTime;
	}
	else{
		return 0;
	}
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.05.23	| Yuichi Hoshino	| Created this file
// 14.12.04	| Yuichi Hoshino	| Release V0 (Revision 00)
// 15.01.30	| Yuichi Hoshino	| Design Improvement [ Stop Watchライブラリ設計変更対応 ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
// 15.08.18	| Yuichi Hoshino	| Trouble Action [ Sheet Finish Request送信待ち時間が65535msを超えると誤った待ち時間で処理してしまう ]
//

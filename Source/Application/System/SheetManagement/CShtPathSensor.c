// File Name:	CShtPathSensor.c
// Description:	Implimentation File of Path Sensor
// Engineer:	Yuichi Hoshino
// Last Edit:	15.01.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CShtPathSensor.h"

#include "DD_DIO.h"

#include "CShtManager.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Step of late detect ---
typedef enum /*EShtPathSnrLateDetectStep*/
{
	SHT_LATE_DETECT_IDLE,				// 休止状態
	SHT_LATE_DETECT_WAIT,				// エッジ確定待ち状態
	SHT_LATE_DETECT_TIMEUP,
	SHT_LATE_DETECT_NUM
} EShtPathSnrLateDetectStep;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Timer Event Call Back Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void SHT_PathSensorLateDetectTimeUp(SS timerID, UL parameter);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CShtPathSensor_Constructor
// Description   : コンストラクタ
// Parameter     : pThis (インスタンスポインタ)
//               : pSpec (スペックデータポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtPathSensor_Constructor(CShtPathSensor* pThis, const ShtPathSensorSpec* pSpec)
{
	UC index;

	pThis->mpSpec				 = pSpec;
	pThis->mWaitEdge			 = SHT_EDGE_LEAD;
	pThis->mReverseFilterBit	 = 0x00;
	pThis->mLateDetectStep		 = SHT_LATE_DETECT_IDLE;
	pThis->mLateDetectErrorCount = 0;
	pThis->mLateDetectTimerID	 = MSP_LOC_TMR_VOID;

	CStopWatch_CStopWatch(&(pThis->mStopWatch));

	for (index = 0; index < SHT_EDGE_NUM; index++ ){
		pThis->mNextEdgeSheetNo[index] = SHEETNO_NUM;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtPathSensor_IsRevereseFilterPath
// Description   : 逆送フィルタ処理対象センサかどうかの取得処理
// Parameter     : pThis (インスタンスポインタ)
//               : reverseKind (逆送識別子)
// Return        : true = 対象センサである
//               : false = 対象センサでない
///////////////////////////////////////////////////////////////////
bool CShtPathSensor_IsRevereseFilterPath(CShtPathSensor* pThis, EShtReverseKind reverseKind)
{
	if ( reverseKind < SHT_REVERSE_KIND_NUM ){
		if ( pThis->mReverseFilterBit & (0x01 << reverseKind) ) return true;
		else													return false;
	}
	else{
		if ( pThis->mReverseFilterBit != 0 ) return true;
		else								 return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtPathSensor_IsLateDetectFilterPath
// Description   : 時間待ち確定フィルタ処理対象センサかどうかの取得
//               : 処理
// Parameter     : pThis (インスタンスポインタ)
//               : edge (シートエッジ識別子)
// Return        : true = 対象センサである
//               : false = 対象センサでない
///////////////////////////////////////////////////////////////////
bool CShtPathSensor_IsLateDetectFilterPath(CShtPathSensor* pThis, EShtSheetEdge edge)
{
	if ( pThis->mpSpec->lateDetectWaitTime[edge] != 0 ) return true;
	else												return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtPathSensor_GetLevel
// Description   : パスセンサ管理レベル取得処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : パスセンサレベル
///////////////////////////////////////////////////////////////////
EShtPathSnrLevel CShtPathSensor_GetLevel(CShtPathSensor* pThis)
{
	if ( pThis->mWaitEdge == SHT_EDGE_TAIL ) return SHT_PATH_SNR_LVL_ON;
	else									 return SHT_PATH_SNR_LVL_OFF;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtPathSensor_GetActualLevel
// Description   : パスセンサレベル(DDフィルタ処理あり)取得処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : パスセンサレベル
///////////////////////////////////////////////////////////////////
EShtPathSnrLevel CShtPathSensor_GetActualLevel(CShtPathSensor* pThis)
{
	if ( DD_DIO_GetInput(pThis->mpSpec->inputID) == ON ) return SHT_PATH_SNR_LVL_ON;
	else												 return SHT_PATH_SNR_LVL_OFF;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtPathSensor_GetLateDetectFilterTime
// Description   : 時間待ち確定フィルタ時間取得処理
// Parameter     : pThis (インスタンスポインタ)
//               : edge (シートエッジ識別子)
// Return        : ‐
///////////////////////////////////////////////////////////////////
UC CShtPathSensor_GetLateDetectFilterTime(CShtPathSensor* pThis, UC edge)
{
	return pThis->mpSpec->lateDetectWaitTime[edge];
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtPathSensor_Reset
// Description   : リセット処理
// Parameter     : pThis (インスタンスポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtPathSensor_Reset(CShtPathSensor* pThis)
{
	UC index;

	pThis->mWaitEdge			 = SHT_EDGE_LEAD;
	pThis->mReverseFilterBit	 = 0x00;
	pThis->mLateDetectStep		 = SHT_LATE_DETECT_IDLE;
	pThis->mLateDetectErrorCount = 0;
	pThis->mLateDetectTimerID	 = MSP_LOC_TMR_VOID;

	CStopWatch_Start(&(pThis->mStopWatch));

	for ( index = 0; index < SHT_EDGE_NUM; index++ ) {
		pThis->mNextEdgeSheetNo[index] = SHEETNO_NUM;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtPathSensor_EntrySheet
// Description   : センサ変化予定シートの登録処理
// Parameter     : pThis (インスタンスポインタ)
//               : sheetNo (Sheet No.)
//               : edge (シートエッジ識別子)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtPathSensor_EntrySheet(CShtPathSensor* pThis, UC sheetNo, EShtSheetEdge edge)
{
	if ( pThis->mNextEdgeSheetNo[edge] == SHEETNO_NUM ){
		pThis->mNextEdgeSheetNo[edge] = sheetNo;
	}
	else { /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtPathSensor_FilterReverse
// Description   : 逆送フィルタ実施指示受信時処理
// Parameter     : pThis (インスタンスポインタ)
//               : action (Action)
//               : reverseKind (逆送識別子)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtPathSensor_FilterReverse(CShtPathSensor* pThis, EAction action, EShtReverseKind reverseKind)
{
	UC reverseKindBit = (UC)(0x01 << reverseKind);								// ビット情報に変換

	if ( reverseKindBit & pThis->mpSpec->reverseFilterBit ){					// フィルタ処理実行対象の逆送動作
		if ( action == ACT_START ){												// フィルタ処理開始指示
			pThis->mReverseFilterBit |= reverseKindBit;
		}
		else{																	// フィルタ処理停止指示
			pThis->mReverseFilterBit &= (UC)(~reverseKindBit);
		}
	}
	else { /* No Action */ }	
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtPathSensor_InformedEdge
// Description   : センサ変化通知受信処理
// Parameter     : pThis (インスタンスポインタ)
//               : edge (シートエッジ識別子)
// Return        : センサ変化原因シートのSheet No.
///////////////////////////////////////////////////////////////////
UC CShtPathSensor_InformedEdge(CShtPathSensor* pThis, EShtSheetEdge edge)
{
	UL elapsedTime;
	UC returnShtNo = SHEETNO_NUM;

	static const UC cNextWaitEdge[SHT_EDGE_NUM] = { SHT_EDGE_TAIL, SHT_EDGE_LEAD };

	if ( pThis->mNextEdgeSheetNo[edge] != SHEETNO_NUM ){						// 次パスセンサ到達予定シートの登録あり
		if ( pThis->mWaitEdge == edge ){										// 待ちエッジと合致
			if ( pThis->mLateDetectStep != SHT_LATE_DETECT_TIMEUP ){
				if ( pThis->mReverseFilterBit == 0x00 ){						// 逆走フィルタ処理未実施
					CStopWatch_Split(&(pThis->mStopWatch), &elapsedTime);// 経過時間取得

					if ( (pThis->mpSpec->filterTime[edge] < elapsedTime) ||		// Filter時間経過済み
						 (pThis->mpSpec->filterTime[edge] == 0) ){				// Filter処理不要
						if ( pThis->mpSpec->lateDetectWaitTime[pThis->mWaitEdge] == 0 ){
																				// エッジ確定対象センサの場合
							returnShtNo = pThis->mNextEdgeSheetNo[edge];

							CStopWatch_Start(&(pThis->mStopWatch));

							pThis->mWaitEdge			  = cNextWaitEdge[pThis->mWaitEdge];
							pThis->mNextEdgeSheetNo[edge] = SHEETNO_NUM;
						}
						else{													// 時間経過確定対象センサの場合
							if ( pThis->mLateDetectStep == SHT_LATE_DETECT_IDLE ){
								pThis->mLateDetectStep = SHT_LATE_DETECT_WAIT;

								pThis->mLateDetectTimerID = MSP_SetEventTimer(pThis->mpSpec->lateDetectWaitTime[pThis->mWaitEdge], SHT_PathSensorLateDetectTimeUp, (UL)pThis);
							}
							else{ /* No Action */ }
						}
					}
					else {  /* No Action */ }
				}
				else{ /* No Action */ }
			}
			else{																// 時間経過確定待ち時間経過済み
				returnShtNo = pThis->mNextEdgeSheetNo[edge];

				CStopWatch_Start(&(pThis->mStopWatch));

				pThis->mWaitEdge			  = cNextWaitEdge[pThis->mWaitEdge];
				pThis->mNextEdgeSheetNo[edge] = SHEETNO_NUM;

				pThis->mLateDetectStep		 = SHT_LATE_DETECT_IDLE;
				pThis->mLateDetectErrorCount = 0;
				pThis->mLateDetectTimerID	 = MSP_LOC_TMR_VOID;
			}
		}
		else{																	// 待ちエッジと合致しない
			if ( (pThis->mLateDetectStep == SHT_LATE_DETECT_WAIT) &&			// 時間待ち中
				 (pThis->mLateDetectErrorCount < pThis->mpSpec->lateDetectErrorCheckTimes[pThis->mWaitEdge]) ){
				pThis->mLateDetectStep = SHT_LATE_DETECT_IDLE;
				MSP_CancelEventTimer(pThis->mLateDetectTimerID);
				pThis->mLateDetectTimerID = MSP_LOC_TMR_VOID;
				pThis->mLateDetectErrorCount++;
			}
			else{ /* No Action */ }
		}
	}
	else{ /* No Action */ }

	return returnShtNo;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtPathSensor_InformedLateDetectTimeUp
// Description   : 時間待ち確定の時間経過通知受信処理
// Parameter     : pThis (インスタンスポインタ)
//               : timerID (タイマー識別子)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtPathSensor_InformedLateDetectTimeUp(CShtPathSensor* pThis, SS timerID)
{
	if ( (pThis->mLateDetectStep == SHT_LATE_DETECT_WAIT) &&					// 時間待ち中
		 (pThis->mLateDetectTimerID == timerID) ){
		pThis->mLateDetectStep = SHT_LATE_DETECT_TIMEUP;

		CShtManager_PathSensorLateDetect(pThis, pThis->mWaitEdge);
	}
	else{ /* No Action */ }
}


/////////////////////////////////////////////////////////////////////////////////////////
// Timer Event Call Back Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : SHT_PathSensorLateDetectTimeUp
// Description   : 時間待ち確定の時間経過イベントコールバック
// Parameter     : timerID (MSP Event Timer ID)
//               : parameter (パスセンサインスタンスポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SHT_PathSensorLateDetectTimeUp(SS timerID, UL parameter)
{
	CShtPathSensor_InformedLateDetectTimeUp((CShtPathSensor *)parameter, timerID);
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.05.20	| Yuichi Hoshino	| Created this file
// 14.12.04	| Yuichi Hoshino	| Release V0 (Revision 00)
// 15.01.30	| Yuichi Hoshino	| Design Improvement [ Stop Watchライブラリ設計変更対応 ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//

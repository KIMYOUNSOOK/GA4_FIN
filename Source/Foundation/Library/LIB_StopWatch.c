// ☆☆☆ Module Software Platform Frame ☆☆☆
/// @file    LIB_StopWatch.h
/// @author  Tsuyoshi Seto
/// @date    15.01.30
/// @version 001
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#define LIB_StopWatch_c

#include "Common.h"
#include "DD_Event.h"
#include "LIB_StopWatch.h"


// *******************************************************  <<<<< Definition & Declaration Area >>>>>  ********************************************************

// ===========================  <<< マクロ定義 >>>  ===========================


// ==========================  <<< データ型定義 >>>  ==========================


// ===========================  <<< 関数プロトタイプ宣言 >>>  ===========================
// static void CStopWatch_Start(CStopWatch*) ;												// 時間計測開始処理 ☆☆☆
// static bool CStopWatch_Stop(CStopWatch*, UL*) ;											// 時間計測終了処理 ☆☆☆
// static bool CStopWatch_Split(CStopWatch*, UL*) ;											// 経過時間取得処理 ☆☆☆


// ************************************************************  <<<<< Program Code Area >>>>>  ***************************************************************

//=[[[ Constructor ]]]=================================================================
// Function Name : CStopWatch::CStopWatch()
// Description   : このクラスのコンストラクタ.
// Parameter     : なし
// Return        : -
//=====================================================================================
void CStopWatch_CStopWatch(CStopWatch *self)
{
// ----- メンバー初期化 -----
	self->mTime = 0 ;
	self->mStatus = OFF ;

// ----- 関数ポインター設定 -----															// メンバー関数方式→直接関数方式に変更('15/1/30)
//	self->Start = CStopWatch_Start ;														// Start()：時間計測開始処理 ☆☆☆
//	self->Stop  = CStopWatch_Stop ;															// Stop() ：時間計測終了処理 ☆☆☆
//	self->Split = CStopWatch_Split ;														// Split()：経過時間取得処理 ☆☆☆
}

//=====================================================================================
// Function Name : CStopWatch::Start()
// Description   : ストップウォッチによる時間計測を開始する.
// Parameter     : なし
// Return        : なし
//=====================================================================================
void CStopWatch_Start(CStopWatch *self)
{
	self->mTime = DD_EVT_GetCurrentTime() ;
	self->mStatus = ON ;
}

//=====================================================================================
// Function Name : CStopWatch::Stop()
// Description   : ストップウォッチによる時間計測を取得&計測終了する.
// Parameter     : *pTime		"計測時間格納アドレス"
// Return        : 処理結果（計測開始していないときは"false"）
//=====================================================================================
bool CStopWatch_Stop(CStopWatch *self, UL *pTime)
{
	UL currentTime = DD_EVT_GetCurrentTime() ;
	
	if ( (self->mStatus) == ON ) {
		if ( currentTime >= (self->mTime) ) {
			*pTime  = currentTime - (self->mTime) ;
		}
		else {																				// 型の最大値を回り込んでいる場合
			*pTime  = currentTime + ( 0xFFFFFFFF - (self->mTime) ) + 1;
		}
		self->mStatus = OFF ;
		return 	true ;
	}
	else {
		*pTime = 0 ;
		return 	false ;
	}
}

//=====================================================================================
// Function Name : CStopWatch::Split()
// Description   : ストップウォッチによる途中経過の時間を取得する.
// Parameter     : *pTime		"計測時間格納アドレス"
// Return        : 処理結果（計測開始していないときは"false"）
//=====================================================================================
bool CStopWatch_Split(CStopWatch *self, UL *pTime)
{
	UL currentTime = DD_EVT_GetCurrentTime() ;
	
	if ( (self->mStatus) == ON ) {
		if ( currentTime >= (self->mTime) ) {
			*pTime  = currentTime - (self->mTime) ;
		}
		else {																				// 型の最大値を回り込んでいる場合
			*pTime  = currentTime + ( 0xFFFFFFFF - (self->mTime) ) + 1;
		}
		return 	true ;
	}
	else {
		*pTime = 0 ;
		return 	false ;
	}
}


// Change History
// Date:    | Engineer:          | Note:
// ---------+--------------------+-----------------------------------------------------------------------------------------------------------------------------
// 14.02.20 | Tsuyoshi Seto      | Module Software Platform Frameのプロトタイプとして本ファイルを作成. (Rev.000)
// 15.01.30 | Tsuyoshi Seto      | メンバー関数(関数ポインター)方式から直接関数コール方式に変更.<RAM使用量低減のため> (Rev.001)
//
//


// ***************************************************************  [ LIB_StopWatch.c : EOF ]  ****************************************************************

// ☆☆☆ Module Software Platform Library ☆☆☆
/// @file    LIB_StopWatch.h
/// @author  Tsuyoshi Seto
/// @date    15.01.30
/// @version 001
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef LIB_StopWatch_h
#define LIB_StopWatch_h


// *******************************************************  <<<<< Definition & Declaration Area >>>>>  ********************************************************

// ===========================  <<< マクロ定義 >>>  ===========================


// ***********************************************************  <<<<< Class Declaration Area >>>>>  ***********************************************************

// ===========================  <<< クラス定義 >>>  ===========================

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Class Name    : CStopWatch
// Description   : 高精度なストップウォッチ.（汎用ライブラリクラス）
// Note          : 1カウント＝1000[ns] : 0[ms]～65.535[ms]
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
typedef struct StopWatch {
/*public:*/
	//=[[[ Constructor ]]]=================================================================
	// Function Name : CStopWatch::CStopWatch()
	// Description   : このクラスのコンストラクタ.
	// Parameter     : なし
	// Return        : -
	//=====================================================================================
	// CStopWatch() { mTime = 0 ; mStatus = OFF ; }

	//=====================================================================================
	// Function Name : CStopWatch::Start()
	// Description   : ストップウォッチによる時間計測を開始する.
	// Parameter     : なし
	// Return        : なし
	//=====================================================================================
	// void Start() { mTime = DD_GetUsec() ; mStatus = ON ; }
	// ☆☆☆void (*Start)(struct StopWatch*) ;												// メンバー関数方式→直接関数方式に変更('15/1/30)

	//=====================================================================================
	// Function Name : CStopWatch::Stop()
	// Description   : ストップウォッチによる時間計測を取得&計測終了する.
	// Parameter     : *pTime		"計測時間格納アドレス"
	// Return        : 処理結果（計測開始していないときは"false"）
	//=====================================================================================
	// bool Stop(US *pTime)
	// ☆☆☆bool (*Stop)(struct StopWatch*, UL*) ;											// メンバー関数方式→直接関数方式に変更('15/1/30)

	//=====================================================================================
	// Function Name : CStopWatch::Split()
	// Description   : ストップウォッチによる途中経過の時間を取得する.
	// Parameter     : *pTime		"計測時間格納アドレス"
	// Return        : 処理結果（計測開始していないときは"false"）
	//=====================================================================================
	// bool Split(US *pTime)
	// ☆☆☆bool (*Split)(struct StopWatch*, UL*) ;										// メンバー関数方式→直接関数方式に変更('15/1/30)

/*private:*/
	UL	mTime ;																				// スタート時間格納データ
	UC	mStatus ;																			// ストップウォッチの動作状態
} CStopWatch ;


// ========================  <<< プロトタイプ宣言 >>>  ========================
void CStopWatch_CStopWatch(CStopWatch*) ;													// ストップウォッチの初期化

void CStopWatch_Start(CStopWatch*) ;														// 時間計測開始処理 ☆☆☆
bool CStopWatch_Stop(CStopWatch*, UL*) ;													// 時間計測終了処理 ☆☆☆
bool CStopWatch_Split(CStopWatch*, UL*) ;													// 経過時間取得処理 ☆☆☆


#endif			// LIB_StopWatch_h


// Change History
// Date:    | Engineer:          | Note:
// ---------+--------------------+-----------------------------------------------------------------------------------------------------------------------------
// 14.02.20 | Tsuyoshi Seto      | Module Software Platform Frameのプロトタイプとして本ファイルを作成. (Rev.000)
// 15.01.30 | Tsuyoshi Seto      | メンバー関数(関数ポインター)方式から直接関数コール方式に変更.<RAM使用量低減のため> (Rev.001)
//
//


// ***************************************************************  [ LIB_StopWatch.h : EOF ]  ****************************************************************

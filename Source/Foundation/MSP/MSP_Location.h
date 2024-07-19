// ☆☆☆ Module Software Platform Frame ☆☆☆
/// @file    MSP_Location.h
/// @author  Mitsuyuki Nishizawa
/// @date    14.11.10
/// @version 001
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef MSP_Location_h
#define MSP_Location_h


// *******************************************************  <<<<< Definition & Declaration Area >>>>>  ********************************************************

// ----- ロケーション･タイマー -----
#define	MSP_LOC_TMR_STOP	0																// ステータス：タイマー未使用（停止中）
#define	MSP_LOC_TMR_RUN		1																// ステータス：タイマー使用中（動作中）

// ----- ロケーション･タイミング更新状態 -----
#define	MSP_LOC_UPDT_ENBL	1																// ロケーション･タイミング更新できる
#define	MSP_LOC_UPDT_DSBL	0																// ロケーション･タイミング更新できない

// ----- OSタイマー領域コントロール定義 -----
#define	OS_TMR_KEEP			1																// OSタイマー領域確保


// ==========================  <<< データ型定義 >>>  ==========================				// コマンド＆ロケーション･イベント型はMSP_EventID.hにあります.

// ----- ロケーション･タイマーのデータ型 -----
typedef struct {																			// <<< "Location Time Information"の型 >>>	
	US mTrigger ;																			// ロケーション･タイマーのトリガーとなったイベントID
	US mSheetNo ;																			// ロケーション･タイマー対象となるシート･ナンバー
	US mTimer ;																				// ロケーション･タイマーID
} TimerInfo ;

typedef struct {																			// <<< "Location Timer"の型 >>>
	US	mStatus  : 1 ;																		// ロケーション･タイマー起動ステータス
	US	mTimerID : 15 ;																		// OSタイマーID
	UL	mPattern ;																			// ロケーション･パターン (for Sheet)
	UC	mIndex ;																			// ロケーション･スケジュール･インデックス
	TimerInfo  mInfo ;																		// ロケーション･タイマー情報
} LocationTimer ;


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Class Name    : CTimingController
// Description   : ロケーション・イベントの生成･通知を実施する.
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
typedef struct TimingController {
/*public:*/
	//=[[[ Constructor ]]]=================================================================
	// Function Name : CTimingController::CTimingController()
	// Description   : このクラスのコンストラクタ.
	// Parameter     : pSpec		"スペック･テーブルへのポインタ"
	// Return        : -
	//=====================================================================================
	//CTimingController(const LocationSpec *pSpec) ;

	//=====================================================================================
	// Function Name : CTimingController::ResetSchedule()
	// Description   : ロケーション･イベントのスケジュールテーブルをリセットする.
	// Parameter     : なし
	// Return        : なし
	//=====================================================================================
	//void ResetSchedule() ;

	//=====================================================================================
	// Function Name : CTimingController::UpdateTiming()
	// Description   : ロケーション･イベントの発生タイミングを更新する.
	// Parameter     : location		"更新対象のロケーション番号（=Event Number）"
	//               : pTiming		"更新タイミングテーブル先頭アドレス"
	// Return        : なし
	//=====================================================================================
	//void UpdateTiming(US location, const SS *pTiming) ;

	//=====================================================================================
	// Function Name : CTimingController::CreateSchedule()
	// Description   : ロケーション･イベントのスケジュールテーブルを構築する.
	// Parameter     : なし
	// Return        : なし
	//=====================================================================================
	//void CreateSchedule() ;

	//=====================================================================================
	// Function Name : CTimingController::NotifyTrigger()
	// Description   : ロケーション･イベントの発生トリガーを通知する.
	// Parameter     : trigger		"トリガーとなるロケーション番号（=Event Number）
	//               : sheetNo		"対象シート･ナンバー"
	//               : pattern		"対象シートのロケーション･パターン"
	//               : index		"対象シートのスケジュール･インデックス番号"
	// Return        : なし
	//=====================================================================================
	//void NotifyTrigger(US trigger, US sheetNo, UL pattern, UC index) ;

	//=====================================================================================
	// Function Name : CTimingController::NotifyTimeout()
	// Description   : ロケーション･イベントの発生タイミングを通知する.
	// Parameter     : timerID		"タイミング生成用のTimer ID"
	//               : location		"トリガーとなるロケーション番号（=Event Number）
	//               : sheetNo		"対象シート･ナンバー"
	//               : timer		"ロケーション･タイマー番号"
	// Return        : なし
	// Note          : この処理は基本的にTimer割り込みハンドラーで実施される.
	//               : ※NotifyTrigger()時のみイベントループからの呼び出しとなる可能性あり.
	//=====================================================================================
	//void NotifyTimeout(SL timerID, US location, US sheetNo, US timer) ;

	//=====================================================================================
	// Function Name : CTimingController::AbortScheduling()
	// Description   : ロケーション･イベントのスケジューリング･タイマーをキャンセルする.
	// Parameter     : なし
	// Return        : なし
	//=====================================================================================
	//void AbortScheduling() ;

/*private:*/
	US mNext[MSP_NUM_OF_LOC][MSP_NUM_OF_SCH] ;												// [Member] 次のロケーションのロケーションID（外部RAM配置のためstatic）
	US mDelay[MSP_NUM_OF_LOC][MSP_NUM_OF_SCH] ;												// [Member] 次のロケーションの発生遅延時間（外部RAM配置のためstatic）

	const LocationSpec *mpSpec ;															// [Member] Location Timing Controllerのスペック
	LocationTimer mTimer[MSP_NUM_OF_TMR] ;													// [Member] ロケーション生成タイマー･リソース
	UC mTimerIndex ;																		// [Member] タイマー･リソース管理用インデックス
	UC mUpdateStatus ;																		// [Member] ロケーション･タイミング更新状態

	//=====================================================================================
	// Function Name : CTimingController::CreateEvent()
	// Description   : ロケーション･イベント通知する.
	// Parameter     : location		"通知対象となるロケーション番号（=Event Number）
	//               : sheetNo		"対象シート･ナンバー"
	//               : pattern		"対象シートのロケーション･パターン"
	// Return        : なし
	//=====================================================================================
	//void CreateEvent(US location, US sheetNo, UL pattern) ;

	//=====================================================================================
	// Function Name : CTimingController::GetNextSchedule()
	// Description   : 次のロケーション･イベント情報を取得する.
	// Parameter     : location		"通知対象となるロケーション番号（=Event Number）
	//               : pattern		"対象シートのロケーション･パターン"
	//               : index		"対象シートのスケジュール･インデックス番号"
	//               : pNext		"次のロケーション番号格納先アドレス"
	// Return        : 次のロケーション･イベントの遅延時間（"次ロケーションなし"＝0xFFFF）
	// Note          : スケジューリング･パターンのインデックスはプロダクト･ユニーク
	//=====================================================================================
	//US GetNextSchedule(US location, UL pattern, UC index, US *pNextLocation)) ;

	//=====================================================================================
	// Function Name : CTimingController::GetTimer()
	// Description   : ロケーション･イベント生成用タイマー･リソースを取得する.
	// Parameter     : なし
	// Return        : 取得されたロケーション･タイマー番号（取得できないときは0xFFFF）
	//=====================================================================================
	//US GetTimer() ;

	//=====================================================================================
	// Function Name : CTimingController::ReleaseTimer()
	// Description   : ロケーション･タイマー･リソースを解放する.
	// Parameter     : timer		"ロケーション･タイマー番号"
	// Return        : なし
	//=====================================================================================
	//void ReleaseTimer(US timer) ;
} CTimingController ;


// ===========================  <<< 関数プロトタイプ宣言 >>>  ===========================
void CTimingController_CTimingController(const LocationSpec*) ;								// ロケーション・コントローラー初期化処理
void CTimingController_ResetSchedule() ;													// ロケーション・イベント･スケジュール更新
void CTimingController_UpdateTiming(US, const SS*) ;										// ロケーション・イベント情報更新
void CTimingController_CreateSchedule() ;													// ロケーション・イベント･スケジュール生成
void CTimingController_NotifyTrigger(US, US, UL, UC) ;										// ロケーション・イベント発生トリガー通知
void CTimingController_NotifyTimeout(SS, US, US, US) ;										// ロケーション・イベント発生
void CTimingController_AbortScheduling() ;													// ロケーション・イベント生成処理中止


#endif			// MSP_Location_h


// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+----------------------------------------------------------------------------------------------------------------------------
// 13.05.05 | Mitsuyuki Nishizawa | Module Software Platform Frameのプロトタイプとして本ファイルを作成.
// 14.11.10 | Mitsuyuki Nishizawa | 関数名称および引数を変更&初期化処理を追加.
//
//


// **************************************************************  [ MSP_Location.h : EOF ]  ******************************************************************

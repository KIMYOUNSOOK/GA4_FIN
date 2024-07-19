// ☆☆☆ Module Software Platform Frame ☆☆☆
/// @file    MSP_Location.c
/// @author  Mitsuyuki Nishizawa
/// @date    15.02.12
/// @version 002
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#define MSP_Location_c

#include "Common.h"
#include "LIB_Service.h"

#include "MSP_Common.h"
#include "MSP_Config.h"
#include "MSP_EventID.h"
#include "MSP_ProductSpec.h"
#include "MSP_EventMain.h"
#include "MSP_EventTimer.h"
#include "MSP_Location.h"


// ===========================  <<< 関数プロトタイプ宣言 >>>  ===========================
void MSP_LocationCallback(SS, UL) ;															// ロケーション･タイマーのコールバック処理

static void CTimingController_CreateEvent(US, US, UL) ;										// ロケーション･イベント生成&通知
static US CTimingController_GetNextSchedule(US, UL, UC, US*) ;								// 次の動作スケジュールの取得
static US CTimingController_GetTimer() ;													// タイマー･リソース取得
static void CTimingController_ReleaseTimer(US) ;											// タイマー･リソース解放


// ========================  <<< インスタンス生成 >>>  ========================

// ----- ロケーション･タイミング･コントローラー -----
CTimingController	msp_TimingController ;													// ロケーション･タイミング･コントローラー


// ===========================  <<< 外部参照変数 >>>  ===========================
extern CEventTimer	msp_EventTimer ;														// [外部参照] イベントタイマー


//$$$$$$ [[[[[ Class: CTimingController ]]]]] $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// Class Name    : CTimingController（ロケーション･タイミング生成者）
// Description   : ロケーション･イベントの生成･通知を実施する.
//               :
// Note          : 特になし
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

//=[[[ Constructor ]]]=================================================================
// Function Name : CTimingController::CTimingController()
// Description   : このクラスのコンストラクタ.
// Parameter     : pSpec		"スペック･テーブルへのポインタ"
// Return        : -
//=====================================================================================
void CTimingController_CTimingController(const LocationSpec *pSpec)
{
	CTimingController	*self = &msp_TimingController ;

	self->mpSpec = pSpec ;																	// スペック･テーブルを登録する

// ----- ロケーション･スケジュール･テーブルのリセット -----
	CTimingController_ResetSchedule() ;
	CTimingController_AbortScheduling() ;
	
// ----- ロケーション･コントローラ情報の初期化 -----
	self->mTimerIndex   = 0 ;
	self->mUpdateStatus = MSP_LOC_UPDT_DSBL ;
}


//=====================================================================================
// Function Name : CTimingController::ResetSchedule()
// Description   : ロケーション･イベントのスケジュールテーブルをリセットする.
// Parameter     : なし
// Return        : なし
//=====================================================================================
void CTimingController_ResetSchedule()
{
	CTimingController  *self = &msp_TimingController ;
	SL	location, schedule ;

	if ( self->mUpdateStatus == MSP_LOC_UPDT_DSBL ) {										// ロケーション･タイミング更新状態が"更新不可能"である

	// ----- スケジュール･テーブルのリセット -----
		for ( location = 0 ; location < MSP_NUM_OF_LOC ; location++ ) {						// [[[ LOOP ]]] デバイス･ロケーションID毎に処理を実施する（ループ#1）
			for ( schedule = 0 ; schedule < MSP_NUM_OF_SCH ; schedule++ ) {					// [[[ LOOP ]]] スケジュール数分の処理を実施する（ループ#2）
				self->mNext[location][schedule]  = MSP_EVT_NUM_VOID ;						// 次のロケーションIDを"VOID"にする
				self->mDelay[location][schedule] = self->mpSpec[location].mDelay[schedule] ; // 遅延時間に"ノミナル値"をセットする
			}																				// [[[ LOOP ]]] （ここまでループ#2）
		}																					// [[[ LOOP ]]] （ここまでループ#1）

	// ----- ロケーション･タイミング更新状態 -----
		self->mUpdateStatus = MSP_LOC_UPDT_ENBL ;											// タイミング更新可能な状態
	}
}


//=====================================================================================
// Function Name : CTimingController::UpdateTiming()
// Description   : ロケーション･イベントの発生タイミングを更新する.
// Parameter     : location		"更新対象のロケーション番号（=Event Number）"
//               : pTiming		"更新タイミングテーブル先頭アドレス"
// Return        : なし
//=====================================================================================
void CTimingController_UpdateTiming(US location, const SS *pTiming)
{
	static const char  eMsg1[] = {"FRAME ERROR: Location ID Error"} ;
	static const char  eMsg2[] = {"FRAME ERROR: Location Update Disable"} ;

	CTimingController	*self = &msp_TimingController ;
	SL	schedule, time ;

// ----- ロケーション･タイミングの更新 -----
	if ( self->mUpdateStatus == MSP_LOC_UPDT_ENBL ) {										// ロケーション･タイミング更新状態が"更新可能"である
		if ( location < MSP_NUM_OF_LOC ) {													// ロケーション番号は有効
			for ( schedule = 0 ; schedule < MSP_NUM_OF_SCH ; schedule++ ) {					// [[[ LOOP ]]] スケジュール数分の更新処理を実施する（ループ）
				if ( self->mpSpec[location].mDelay[schedule] != 0xFFFF ) {					// 対象ロケーション有効/無効のチェック ⇒ "0xFFFF"は無効なロケーション
					time = self->mpSpec[location].mDelay[schedule] + pTiming[schedule] ;
					if ( time < 0 ) {														// <<< "ノミナル＋補正値"がマイナス >>>
						self->mDelay[location][schedule] = 0 ;								// ⇒ ロケーション･タイミングを"0"とする
					}
					else {																	// <<< "ノミナル＋補正値"がプラス >>>
						self->mDelay[location][schedule] = (US)time ;						// ⇒ ロケーション･タイミングを更新する
					}
				}
			}																				// [[[ LOOP ]]] （ここまでループ）
		}
		else {																				// ロケーション番号が無効
			Printf(LOGID_FRAME, 0, (UC*)eMsg1, (US)sizeof(eMsg1)) ;
		}
	}
	else {
		Printf(LOGID_FRAME, 0, (UC*)eMsg2, (US)sizeof(eMsg2)) ;
	}
}


//=====================================================================================
// Function Name : CTimingController::CreateSchedule()
// Description   : ロケーション･イベントのスケジュールテーブルを構築する.
// Parameter     : なし
// Return        : なし
//=====================================================================================
void CTimingController_CreateSchedule()
{
	CTimingController	*self = &msp_TimingController ;
	SL	location, schedule ;
	US	current, next, delay ;

// ----- ロケーション･タイミング更新状態 -----
	if ( self->mUpdateStatus == MSP_LOC_UPDT_ENBL ) {										// ロケーション･タイミング更新状態が"更新可能"である
		self->mUpdateStatus = MSP_LOC_UPDT_DSBL ;											// タイミング更新不可能な状態

	// ----- スケジュール･テーブルの構築 -----
		for ( location = 0 ; location < MSP_NUM_OF_LOC ; location++ ) {						// [[[ LOOP ]]] デバイス･ロケーションID毎に処理を実施する（ループ#1）
			for ( schedule = 0 ; schedule < MSP_NUM_OF_SCH ; schedule++ ) {					// [[[ LOOP ]]] スケジュール数分の処理を実施する（ループ#2）
			// ----- 初期設定 -----
				delay   = self->mDelay[location][schedule] ;								// 対象ロケーションの遅延時間を"delay"にセット
				current = MSP_GetEventNumber(self->mpSpec[location].mTrigger) ;				// カレント･ロケーションIDに対象ロケーションのトリガーをセット
				next    = self->mNext[current][schedule] ;									// ネクスト･ロケーションIDにトリガー･ロケーションの"Next"をセット

			// ----- スケジュールテーブルの更新 -----
				if ( ( current != MSP_EVT_NUM_VOID ) && ( delay != 0xFFFF ) ) {				// スケジュール生成対象ロケーション有効/無効のチェック

				// ☆☆☆ 設計メモ ☆☆☆
				// トリガー･ロケーションの設定が"MSP_EVT_NUM_VOID"となっている場合、
				// 最上流のロケーションを示すため、スケジュール生成の対象としない。
				// （トリガー発生通知時にハンドラが登録されていればコールバックされる。）
				// 遅延時間の設定が"0xFFFF"となっている場合、そのスケジュールNo.に対応する
				// テーブルへの登録は実行されない。

				// ----- スケジュール設定場所の決定 -----
					while ( next != MSP_EVT_NUM_VOID ) {									// [[[ LOOP ]]] チェーンの端末でない間は処理を実施する（ループ#3）
						if ( self->mDelay[next][schedule] <= delay ) {						// <<< "delay"の値が"next"の遅延時間以上 >>>
							delay  -= self->mDelay[next][schedule];							// ⇒ "delay"から"next"の遅延時間を差し引く
							current = next ;												//    カレント･ロケーションIDを"next"のロケーションIDに更新する
							next    = self->mNext[current][schedule] ;						//    ネクスト･ロケーションIDをカレント･ロケーションの"Next"に更新する
						}
						else {																// <<< "delay"の値が"next"の遅延時間未満 >>>
							break ;															// ⇒ ループを抜ける！
						}
					}																		// [[[ LOOP ]]] （ここまでループ#3）

				// ----- 対象ロケーションのスケジュール設定 -----
					self->mNext[current][schedule]   = (US)location ;						// 前に繋がるロケーションの"Next"を対象ロケーションIDに更新する
					self->mNext[location][schedule]  = next ;								// 対象ロケーションの"Next"を後ろに繋がるロケーションIDに更新する
					self->mDelay[location][schedule] = delay ;								// 対象ロケーションの"Delay"を前のロケーションとの差分に更新する

					if ( next != MSP_EVT_NUM_VOID ) {										// 後ろに繋がるロケーションがあれば…
						self->mDelay[next][schedule] -= delay ;								// 後ろのロケーションの"Delay"から対象ロケーションの"delay"の値を差し引く
					}
				}
			}																				// [[[ LOOP ]]] （ここまでループ#2）
		}																					// [[[ LOOP ]]] （ここまでループ#1）
	}
}


//=====================================================================================
// Function Name : CTimingController::NotifyTrigger()
// Description   : ロケーション･イベントの発生トリガーを通知する.
// Parameter     : trigger		"トリガーとなるロケーション番号（=Event Number）
//               : sheetNo		"対象シート･ナンバー"
//               : pattern		"対象シートのロケーション･パターン"
//               : index		"対象シートのスケジュール･インデックス番号"
// Return        : なし
//=====================================================================================
void CTimingController_NotifyTrigger(US trigger, US sheetNo, UL pattern, UC index)
{
	static const char  eMsg1[] = {"FRAME ERROR: Timer Set Error#1"} ;
	static const char  eMsg2[] = {"FRAME ERROR: Timer Resource Full"} ;
	static const char  eMsg3[] = {"FRAME ERROR: Invalid Location ID"} ;
	
	CTimingController	*self = &msp_TimingController ;
	TimerInfo  *pTimerInfo ;
	US	timer ;
	SS	osTimer ;
	Handler  info ;

// ----- タイマー･リソース取得 & イベント生成 -----
	if ( trigger < MSP_NUM_OF_LOC ) {														// ロケーション番号は有効
		timer = CTimingController_GetTimer() ;												// ロケーション･タイマー･リソース取得

		if ( timer != MSP_LOC_TMR_FULL ) {													// タイマー･リソースが確保できた！
			pTimerInfo = &(self->mTimer[timer].mInfo) ;
			
			pTimerInfo->mTrigger = trigger ;												// パラメータの生成
			pTimerInfo->mSheetNo = sheetNo ;												// 待ち時間が0msのタイマーは領域確保制御できないが念のため
			pTimerInfo->mTimer   = timer;
			
			self->mTimer[timer].mPattern = pattern ;										// ロケーション･パターンをセット
			self->mTimer[timer].mIndex   = index ;											// スケジュール･インデックス番号をセット
			self->mTimer[timer].mTimerID = MSP_TMR_TERM ;									// 注意!!! 0ms遅延のタイムアウト処理実施のためTimer IDに0をセットする

			info.mpHandler = MSP_LocationCallback ;
			info.mParam = (UL)pTimerInfo ;
			osTimer = CEventTimer_Set(&msp_EventTimer, MSP_TMR_HIGH, 0, &info) ;			// 高優先度リアル･タイマーをセット
			
			// ☆☆☆ 設計メモ ☆☆☆
			// トリガー発生の為の処理（タスク）の優先度が低いため、
			// このタイミングで既にmTimerIDが更新されている可能性がある。
			// そのためTimerセットの戻り値として取得されたIDでの更新は実施しない。

			if ( osTimer < 0 ) {															// タイマー･セット時のエラー発生！！！
				CTimingController_ReleaseTimer(timer) ;
				Printf(LOGID_FRAME, 0, (UC*)eMsg1, (US)sizeof(eMsg1)) ;
			}
		}
		else {																				// タイマー･リソース確保できず！
			Printf(LOGID_FRAME, 0, (UC*)eMsg2, (US)sizeof(eMsg2)) ;
		}
	}
	else {
		Printf(LOGID_FRAME, 0, (UC*)eMsg3, (US)sizeof(eMsg3)) ;
	}
}


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
void CTimingController_NotifyTimeout(SS timerID, US location, US sheetNo, US timer)
{
	static const char  eMsg1[] = {"FRAME ERROR: Timer Set Error#1"} ;
	static const char  eMsg2[] = {"FRAME ERROR: Timer Invalid Timeout"} ;

	CTimingController	*self = &msp_TimingController ;
	LocationTimer  *pTimer    = &(self->mTimer[timer]) ;
	TimerInfo  *pTimerInfo    = &(pTimer->mInfo) ;
	US	delay, next ;
	SS	osTimer ;
	Handler  info ;

	if ( ( pTimer->mTimerID == (US)timerID ) && ( pTimer->mStatus == MSP_LOC_TMR_RUN ) ) {		// タイマーIDの整合性チェック
		CTimingController_CreateEvent(location, sheetNo, pTimer->mPattern) ;					// ロケーション･イベント生成

		delay = CTimingController_GetNextSchedule(location, pTimer->mPattern, pTimer->mIndex, &next) ;	// 次のスケジュールを取得

		while ( next != MSP_EVT_NUM_VOID ) {													// 次のスケジュールがある！（ループ）
			if ( delay > 0 ) {																	// <<< 遅延時間が"0"でないとき >>>
				pTimerInfo->mTrigger = next ;													// パラメータの生成
				pTimerInfo->mSheetNo = sheetNo ;													// 待ち時間が0msのタイマーは領域確保制御できないが念のため
				pTimerInfo->mTimer   = timer;		
				
				info.mpHandler = MSP_LocationCallback ;
				info.mParam = (UL)pTimerInfo ;
				
				osTimer = CEventTimer_Set(&msp_EventTimer, MSP_TMR_HIGH, delay, &info) ;		// 高優先度リアル･タイマーをセット
				
				if ( osTimer < 0 ) {															// タイマー･セット時のエラー発生！！！
					CTimingController_ReleaseTimer(timer) ;
					Printf(LOGID_FRAME, 0, (UC*)eMsg1, (US)sizeof(eMsg1)) ;
				}
				else {																			// タイマーセット完了
					pTimer->mTimerID = osTimer ;												// OS管理のタイマーIDをセーブ
				}

				break ;																			// ループから脱出！！！
			}
			else {																				// <<< 遅延時間が"0"のとき >>>　※タスク･ディスパッチを回避し実行時間を軽減！！！
				pTimer->mTimerID = 0 ;															// タイマーIDを"0"とする（OSタイマーを遅延=0でセットした状態と同じ）
				CTimingController_CreateEvent(next, sheetNo, pTimer->mPattern) ;				// ロケーション･イベント生成

				delay = CTimingController_GetNextSchedule(next, pTimer->mPattern, pTimer->mIndex, &next) ; // 次のスケジュールを取得 ⇒ ループ判断へ
			}
		}

		if ( next == MSP_EVT_NUM_VOID ) {														// 次のスケジュールがない！
			CTimingController_ReleaseTimer(timer) ;												// ロケーション･タイマー･リソース解放
		}
	}
	else {																						// タイマーIDが不整合またはタイマーが停止状態
		CTimingController_ReleaseTimer(timer) ;													// ロケーション･タイマー･リソース解放
		Printf(LOGID_FRAME, 0, (UC*)eMsg2, (US)sizeof(eMsg2)) ;
	}
}


//=====================================================================================
// Function Name : CTimingController::AbortScheduling()
// Description   : ロケーション･イベントのスケジューリング･タイマーをキャンセルする.
// Parameter     : なし
// Return        : なし
//=====================================================================================
void CTimingController_AbortScheduling()
{
	CTimingController  *self = &msp_TimingController ;
	SL	index ;

	for ( index = 0 ; index < MSP_NUM_OF_TMR ; index++ ) {									// [[[ LOOP ]]] 全てのタイマーIDに対して処理を実施する
		self->mTimer[index].mStatus  = MSP_LOC_TMR_STOP ;									// タイマー管理テーブルのリセット
		self->mTimer[index].mTimerID = MSP_LOC_TMR_VOID ;
		self->mTimer[index].mPattern = 0x00000000 ;
		self->mTimer[index].mIndex   = 0 ;

		self->mTimer[index].mInfo.mTrigger = 0 ;											// タイマー通知情報エリアのリセット
		self->mTimer[index].mInfo.mSheetNo = 0 ;
		self->mTimer[index].mInfo.mTimer   = 0 ;
	}																						// [[[ LOOP ]]]（ここまでループ）
}


//=====================================================================================
// Function Name : CTimingController::CreateEvent()
// Description   : ロケーション･イベント通知する.
// Parameter     : location		"通知対象となるロケーション番号（=Event Number）
//               : sheetNo		"対象シート･ナンバー"
//               : pattern		"対象シートのロケーション･パターン"
// Return        : なし
//=====================================================================================
static void CTimingController_CreateEvent(US location, US sheetNo, UL pattern)
{
	CTimingController	*self = &msp_TimingController ;
	LocationEvent	event ;
	UL	filter ;
	US	eventID ;

	filter = self->mpSpec[location].mFilter ;												// ロケーション･イベントのフィルタリング･データを取得する

	if ( (pattern & filter) != 0x00000000 ) {												// ロケーション･イベント生成パターン成立している？
		if ( (msp_EvtIndexTable[EVT_LOC_TMNG].mpControlTable)[location].mCount != 0 ) {		// イベント･ハンドラが登録されている？
		// ----- ロケーション･イベント生成 -----
			eventID          = MSP_GetEventID(EVT_LOC_TMNG, location) ;
			event.mSheetNo   = sheetNo;
			event.mParameter = self->mpSpec[location].mParameter ;

		// ----- イベント通知 -----
			MSP_SendEvent(MID_FRAME, eventID, (const UC*)&event, sizeof(LocationEvent), OFF) ; // ロケーション･イベントを送信する
		}
	}
}


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
static US CTimingController_GetNextSchedule(US location, UL pattern, UC index, US *pNext)
{
	CTimingController	*self = &msp_TimingController ;
	UL	filter ;
	US	next, delay = 0x0000 ;

// ----- 次のスケジュールを検索 -----
	for ( next = self->mNext[location][index] ; next != MSP_EVT_NUM_VOID ; next = self->mNext[next][index] ) {
		filter = self->mpSpec[next].mFilter ;

		delay += self->mDelay[next][index] ;
		if ( (filter & pattern) != 0x00000000 ) {
			*pNext = next ;
			break ;
		}
	}

	if ( next == MSP_EVT_NUM_VOID ) {														// 次のスケジュールがない！
		delay  = 0xFFFF ;																	// 遅延時間の設定を行う（"次ロケーションなし"＝0xFFFF）
		*pNext = MSP_EVT_NUM_VOID ;															// 次のロケーション番号＝"VOID"
	}

	return	delay ;
}


//=====================================================================================
// Function Name : CTimingController::GetTimer()
// Description   : ロケーション･イベント生成用タイマー･リソースを取得する.
// Parameter     : なし
// Return        : 取得されたロケーション･タイマー番号（取得できないときは0xFFFF）
//=====================================================================================
static US CTimingController_GetTimer()
{
	CTimingController	*self = &msp_TimingController ;
	SL	count ;
	UC	key ;
	US	number = MSP_LOC_TMR_FULL ;
	static UC timerIndex = 0;																//  タイマー･リソース管理用インデックス
	
	key = LIB_GetKey() ; LIB_Lock() ;														// 注意!!! 割り込み禁止 ここから!!!

	for ( count = 0 ; count < MSP_NUM_OF_TMR ; count++ ) {									// タイマー･リソース管理テーブルをチェックする
		if ( ++timerIndex >= MSP_NUM_OF_TMR ) timerIndex = 0 ;

		if ( self->mTimer[timerIndex].mStatus == MSP_LOC_TMR_STOP ) {						// リソースの空きを見つけた！
			self->mTimer[timerIndex].mStatus  = MSP_LOC_TMR_RUN ;							// ===> ステータスの更新（TMR_RUN）
			number = timerIndex ;															//      戻り値（オート変数）の更新
			break ;
		}
	}

	LIB_Unlock(key) ;																		// 注意!!! 割り込み禁止 ここまで!!!

	return	number ;																		// ロケーション･タイマー番号をリターン
}


//=====================================================================================
// Function Name : CTimingController::ReleaseTimer()
// Description   : ロケーション･タイマー･リソースを解放する.
// Parameter     : timer		"ロケーション･タイマー番号"
// Return        : なし
//=====================================================================================
static void CTimingController_ReleaseTimer(US timer) 
{
	CTimingController	*self = &msp_TimingController ;

	self->mTimer[timer].mTimerID = MSP_LOC_TMR_VOID ;
	self->mTimer[timer].mStatus  = MSP_LOC_TMR_STOP ;
	self->mTimer[timer].mPattern = 0x00000000 ;
	self->mTimer[timer].mIndex   = 0 ;
}


//=====================================================================================
// Function Name : MSP_LocationCallback()
// Description   : ロケーション･タイマーのタイムアウト通知を受ける.
//               : param	"OSタイマーのコールバックパラメータ"
// Return        : なし
//=====================================================================================
void MSP_LocationCallback(SS timerID, UL param)
{
	TimerInfo *pInfo = (TimerInfo*)param ;

	CTimingController_NotifyTimeout(timerID, pInfo->mTrigger, pInfo->mSheetNo, pInfo->mTimer) ;
}


// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+----------------------------------------------------------------------------------------------------------------------------
// 14.04.01 | Mitsuyuki Nishizawa | Module Software Platform Frameのプロトタイプとして本ファイルを作成.
// 14.11.10 | Mitsuyuki Nishizawa | 関数名称および引数を変更&初期化処理を追加.
// 15.02.12 | Yuichi Hoshino      | Printf()に渡すIDをLog IDに変更
// 15.03.30 | Yuichi Hoshino      | Release version 002
//
//


// **************************************************************  [ MSP_Location.c : EOF ]  ******************************************************************

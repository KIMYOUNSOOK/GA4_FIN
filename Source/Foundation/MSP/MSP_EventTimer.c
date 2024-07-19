// ☆☆☆ Module Software Platform Frame ☆☆☆
/// @file    MSP_EventTimer.c
/// @author  Mitsuyuki Nishizawa
/// @date    14.09.24
/// @version 002
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#define MSP_EventTimer_c

#include "Common.h"
#include "LIB_Service.h"

#include "MSP_Common.h"
#include "MSP_Config.h"
#include "MSP_EventID.h"
#include "MSP_ProductSpec.h"
#include "MSP_EventMain.h"
#include "MSP_EventTimer.h"


// ******************************************************  <<<<< Definition & Declaration Area >>>>>  *********************************************************

// ===========================  <<< マクロ定義 >>>  ===========================


// ===========================  <<< データ型定義 >>>  ===========================


// ===========================  <<< 関数プロトタイプ宣言 >>>  ===========================
static void CEventTimer_Fire(CEventTimer*, SS) ;											// タイムアウト処理
static void CEventTimer_ResetField(CEventTimer*, T_SZ) ;									// タイマー管理情報：初期化処理
static void CEventTimer_SetField(CEventTimer*, T_SZ, UC, US, Handler*) ;					// タイマー管理情報：情報設定処理
static T_SZ CEventTimer_SearchField(CEventTimer*) ;											// タイマー管理情報：空き領域検索処理
static void CEventTimer_HookField(CEventTimer*, T_SZ) ;										// タイマー管理情報：登録処理
static void CEventTimer_RemoveField(CEventTimer*, T_SZ) ;									// タイマー管理情報：抹消処理
static UC CEventTimer_GetType(CEventTimer*, T_SZ) ;											// タイマー管理情報：優先度情報取得処理
static void CEventTimer_ExecuteHandler(CEventTimer*, T_SZ) ;								// コールバックハンドラー実行処理


// ========================  <<< インスタンス生成 >>>  ========================

// ----- イベントタイマー -----
CEventTimer msp_EventTimer ;																// イベントタイマー (1つのバッファを複数のキューで共有可能)


// ************************************************************  <<<<< Program Code Area >>>>>  ***************************************************************

//$$$$$$ [[[[[ Class: CEventTimer ]]]]] $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// Class Name    : CEventTimer（イベントタイマー）
// Description   : タイマー管理.
//               :
// Note          : なし
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

//=[[[ Constructor ]]]=================================================================
// Function Name : CEventTimer::CEventTimer()                                          
// Description   : このクラスのコンストラクタ.                                         
// Parameter     : *self		"対象データバッファのアドレス"                         
// Return        : -                                                                   
//=====================================================================================
void CEventTimer_CEventTimer(CEventTimer *self)
{
	T_SZ  index ;

// ----- メンバー初期化 -----
	self->mTop   = MSP_TMR_TERM ;															// 先頭のタイマーIDを"なし"にする
	self->mIndex = MSP_MAX_TMR_NUM - 1 ;													// 空き領域検索用インデックスをリセットする（末端領域）
	self->mCount = 0 ;																		// 現在のタイマー使用本数をリセットする
	self->mMax   = 0 ;																		// タイマー使用最大本数(履歴)をリセットする

	for ( index = 0 ; index < MSP_MAX_TMR_NUM ; index++ ) {									// タイマー管理情報領域をクリアする
		CEventTimer_ResetField(self, index) ;
	}
}


//=====================================================================================
// Function Name : CEventTimer::Set()                                                  
// Description   : コールバック･タイマーをセットする.                                  
// Parameter     : *self		"対象タイマー管理情報のアドレス"                       
//               : type			"タイマー優先度（HI/LO）"                              
//               : delay		"タイマー遅延時間[ms]"                                 
//               : *handler		"コールバックハンドラー情報格納先アドレス"             
// Return        : タイマーID（マイナスはエラー）                                      
//=====================================================================================
SS CEventTimer_Set(CEventTimer *self, UC type, US delay, Handler *handler)
{
	SS	result = MSP_TMR_ERR ;
	UC	key ;
	T_SZ  index ;

	key = LIB_GetKey() ; LIB_Lock() ;														// 注意!!! 割り込み禁止 ここから!!!
	index = CEventTimer_SearchField(self) ;													// タイマー管理情報領域の空きを検索する
	if ( index != MSP_TMR_TERM ) {															// < 空き領域が見つかった >
		CEventTimer_SetField(self, index, type, delay, handler) ;							// 管理データを登録する
		result = (SS)index ;																// 戻りタイマーIDに登録先インデックスをセットする

		if ( delay == 0 ) {																	// < 遅延なし(遅延時間"0ms")で設定されている >
			CEventTimer_Fire(self, (SS)index) ;												// 即時タイムアウト処理を実施
			if ( type == MSP_TMR_HIGH ) {													// < 遅延なし＆高優先度タイマーのとき >
				result = (SS)MSP_TMR_TERM ;													// 戻りタイマーIDに"TERM"をセットする（コールバック済のため）
			}
		}
		else {																				// < 遅延ありで設定されている >
			CEventTimer_HookField(self, index) ;											// タイマー管理情報をリストに登録する
		}
	}
	LIB_Unlock(key) ;																		// 注意!!! 割り込み禁止 ここまで!!!

	return	result ;
}


//=====================================================================================
// Function Name : CEventTimer::Cancel()                                               
// Description   : コールバック･タイマーをキャンセルする.                              
// Parameter     : *self		"対象タイマー管理情報のアドレス"                       
//               : id			"キャンセル対象タイマーID"                             
// Return        : 処理結果（0:処理成功 / -1:処理失敗）                                
//=====================================================================================
SS CEventTimer_Cancel(CEventTimer *self, SS id)
{
	SS	result = MSP_TMR_ERR ;
	UC	status, key ;

	if ( ( id >= 0 ) && ( id < MSP_MAX_TMR_NUM ) ) {										// < イベントタイマーIDが有効である >
		key = LIB_GetKey() ; LIB_Lock() ;													// 注意!!! 割り込み禁止 ここから!!!
		status = self->mField[id].mStatus ;													// < イベントタイマーが起動中である >
		if ( ( status == MSP_TMR_LOW ) || ( status == MSP_TMR_HIGH ) ) {					// ※ コールバック処理実施状態でないこと！
			CEventTimer_RemoveField(self, (T_SZ)id) ;										// タイマー管理情報をリストから削除する
			CEventTimer_ResetField(self, (T_SZ)id) ;										// タイマー管理情報を初期化する
			result = MSP_TMR_OK ;
		}
		LIB_Unlock(key) ;																	// 注意!!! 割り込み禁止 ここまで!!!
	}

	return	result ;
}


//=====================================================================================
// Function Name : CEventTimer::IsRunning()                                            
// Description   : コールバック･タイマーの動作状態を取得する.                          
// Parameter     : *self		"対象タイマー管理情報のアドレス"                       
//               : id			"チェック対象タイマーID"                               
// Return        : タイマー動作状態（0:停止中 / 1:動作中）                             
//=====================================================================================
UC CEventTimer_IsRunning(CEventTimer *self, SS id)
{
	return	((self->mField[id].mStatus) & 0x01) ;
}


//=====================================================================================
// Function Name : CEventTimer::Tick()                                                 
// Description   : カウントダウン処理を実行する.                                       
// Parameter     : *self		"対象タイマー管理情報のアドレス"                       
// Return        : -                                                                   
//=====================================================================================
void CEventTimer_Tick(CEventTimer *self)
{
	TimerField	*field ;

	field = self->mField ;																	// タイマー管理情報領域の先頭アドレスをセットする

	if ( (self->mTop) != MSP_TMR_TERM ) {													// < 起動されているタイマーが存在する >
		(field[self->mTop].mDelay)-- ;														// 先頭のタイマーの遅延時間をデクリメントする

		while ( (field[self->mTop].mDelay) == 0 ) {											// < 先頭のタイマーの遅延時間が"0"となった >
			CEventTimer_Fire(self, (SS)(self->mTop)) ;										// コールバック処理を実施する（ハンドラー実行orイベント発行）

			if ( (self->mTop) == MSP_TMR_TERM ) {											// < 接続タイマーがなくなった >
				break ;
			}
		}
	}
}


//=====================================================================================
// Function Name : CEventTimer::Timeout()                                              
// Description   : コールバック処理を実行する. (低優先度タイマー専用関数)              
// Parameter     : *self		"対象タイマー管理情報のアドレス"                       
//               : id			"チェック対象タイマーID"                               
// Return        : -                                                                   
//=====================================================================================
void CEventTimer_Timeout(CEventTimer *self, SS id)
{
	UC	key ;

	if ( ( id >= 0 ) && ( id < MSP_MAX_TMR_NUM ) ) {										// < イベントタイマーIDが有効である >
		if ( CEventTimer_GetType(self, (T_SZ)id) == MSP_TMR_LOW ) {							// < イベントタイマー(禎優先度)が起動中である >
			CEventTimer_ExecuteHandler(self, (T_SZ)id) ;									// コールバック処理を実行する

			key = LIB_GetKey() ; LIB_Lock() ;												// 注意!!! 割り込み禁止 ここから!!!
			CEventTimer_ResetField(self, (T_SZ)id) ;										// タイマー管理情報を初期化する
			LIB_Unlock(key) ;																// 注意!!! 割り込み禁止 ここまで!!!
		}
	}
}


//=====================================================================================
// Function Name : CEventTimer::Fire()                                                 
// Description   : タイムアウト処理を実行する.                                         
// Parameter     : *self		"対象タイマー管理情報のアドレス"                       
//               : id			"チェック対象タイマーID"                               
// Return        : -                                                                   
//=====================================================================================
static void CEventTimer_Fire(CEventTimer *self, SS id)
{
	US	eventID = MSP_GetEventID(EVT_TIME_OUT, 0) ;

	if ( CEventTimer_GetType(self, (T_SZ)id) == MSP_TMR_HIGH ) {							// < 高優先度タイマーのタイムアウトである >
		CEventTimer_ExecuteHandler(self, (T_SZ)id) ;										// 即時コールバック処理を実行する
		CEventTimer_RemoveField(self, (T_SZ)id) ;											// タイマーをリンクから外す
		CEventTimer_ResetField(self, (T_SZ)id) ;											// タイマー管理情報を初期化する（領域を解放する）
	}
	else if ( CEventTimer_GetType(self, (T_SZ)id) == MSP_TMR_LOW ) {						// < 低優先度タイマーのタイムアウトである >
		CEventTimer_RemoveField(self, (T_SZ)id) ;											// タイマーをリンクから外す（領域はハンドラー実行まで解放しない）
		MSP_SendEvent(MID_FRAME, (US)eventID, (const UC*)&id, sizeof(id), LOG_OFF) ;		// タイマー･イベントを送信する（ログ出力しない）
	}
}


//=====================================================================================
// Function Name : CEventTimer::ResetField()                                           
// Description   : タイマー管理情報を初期化する.                                       
// Parameter     : *self		"対象タイマー管理情報のアドレス"                       
//               : index		"初期化対象情報格納先インデックス"                     
// Return        : -                                                                   
//=====================================================================================
static void CEventTimer_ResetField(CEventTimer *self, T_SZ index)
{
	self->mField[index].mInfo.mpHandler = MSP_HDLR_VOID ;									// コールバック情報を初期化する
	self->mField[index].mInfo.mParam    = 0 ;

	self->mField[index].mPrev   = MSP_TMR_TERM ;											// タイマー管理情報を初期化する
	self->mField[index].mNext   = MSP_TMR_TERM ;
	self->mField[index].mStatus = MSP_TMR_STOP ;
	self->mField[index].mZero   = CLEAR ;
	self->mField[index].mDelay  = 0 ;

	if ( (self->mCount) > 0 ) {																// [デバッグ] カウンター更新（-1）
		(self->mCount)-- ;
	}
}


//=====================================================================================
// Function Name : CEventTimer::SetField()                                             
// Description   : タイマー管理情報を設定する.                                         
// Parameter     : *self		"対象タイマー管理情報のアドレス"                       
//               : index		"初期化対象情報格納先インデックス"                     
//               : type			"タイマー優先度（HI/LO）"                              
//               : delay		"タイマー遅延時間[ms]"                                 
//               : *handler		"コールバックハンドラー情報格納先アドレス"             
// Return        : -                                                                   
//=====================================================================================
static void CEventTimer_SetField(CEventTimer *self, T_SZ index, UC type, US delay, Handler *handler)
{
	self->mField[index].mInfo   = *handler ;												// コールバック情報を設定する
	self->mField[index].mStatus = type ;													// タイマー管理情報を設定する
	self->mField[index].mDelay  = delay ;

	if ( delay == 0 ) {																		// 設定遅延時間が0[ms]か判断する
		self->mField[index].mZero = SET ;
	}
	else {
		self->mField[index].mZero = CLEAR ;
	}

	if ( ++(self->mCount) > (self->mMax) ) {												// [デバッグ] カウンター更新（+1）
		self->mMax = self->mCount ;
	}
}


//=====================================================================================
// Function Name : CEventTimer::SearchField()                                          
// Description   : タイマー管理情報の空き領域を検索する.                               
// Parameter     : *self		"対象タイマー管理情報のアドレス"                       
// Return        : タイマー管理情報の空き領域を示すインデックス（TERM:空き領域なし)    
//=====================================================================================
static T_SZ CEventTimer_SearchField(CEventTimer *self)
{
	T_SZ  limit, index ;

	index = self->mIndex ;

	for ( limit = 0 ; limit < MSP_MAX_TMR_NUM ; limit++ ) {									// [[[ 繰り返し ]]] ：最大回数は領域確保数
		if ( ++index >= MSP_MAX_TMR_NUM ) {													// 最後にチェックした次領域にインデックスを更新する
			index = 0 ;
		}

		if ( (self->mField[index].mStatus) == MSP_TMR_STOP ) {								// < 未使用タイマー領域を発見した >
			self->mIndex = index ;															// 検索開始インデックスを更新する
			break ;																			// 空き領域の検索処理完了！！！
		}
	}																						// [[[ 繰り返し ]]] ：ここまで

	if ( limit == MSP_MAX_TMR_NUM ) {														// < 空き領域を発見できなかった >
		index = MSP_TMR_TERM ;																// インデックスにTERMをセットする
	}

	return	index ;
}


//=====================================================================================
// Function Name : CEventTimer::HookField()                                            
// Description   : タイマー管理情報を登録する.                                         
// Parameter     : *self		"対象タイマー管理情報のアドレス"                       
//               : index		"登録対象情報格納先インデックス"                       
// Return        : -                                                                   
//=====================================================================================
static void CEventTimer_HookField(CEventTimer *self, T_SZ index)
{
	TimerField	*field ;
	T_SZ  crnt, prev, limit ;
	T_SZ  pos = MSP_TMR_TERM ;

// ----- タイマー情報の設定 -----
	field = self->mField ;																	// タイマー管理情報領域の先頭アドレスをセットする

// ----- タイマー情報の登録 -----
	if ( (self->mTop) == MSP_TMR_TERM ) {													// < 起動中のタイマーがない >
		self->mTop = index ;
	}
	else {																					// < 起動中のタイマーがある >
		crnt = self->mTop ;																	// 先頭のタイマーからチェックを実施する
		prev = MSP_TMR_TERM ;

	// ----- タイマー接続先の検索 -----
		for ( limit = 0 ; limit < MSP_MAX_TMR_NUM ; limit++ ) {								// [[[ 繰り返し ]]] ：最大回数は領域確保数
			if ( field[crnt].mDelay > field[index].mDelay ) {								// < タイマー接続先を発見した >
				field[crnt].mDelay -= field[index].mDelay ;									// 遅延時間の差分を算出する（接続済みタイマーの遅延時間を補正）
				break ;																		// 処理中断！！！
			}

			field[index].mDelay -= field[crnt].mDelay ;										// 遅延時間の差分を算出する（接続対象のタイマー値を補正する）

			prev = crnt ;																	// チェック対象インデックスを前段インデックスにセットする
			crnt = field[crnt].mNext ;														// チェック対象インデックスに後段インデックスをセットする

			if ( crnt == MSP_TMR_TERM ) {													// リンクの最後尾まで検索を実施した
				break ;																		// 処理中断！！！
			}
		}																					// [[[ 繰り返し ]]] ：ここまで

	// ----- リンクの更新 -----
		if ( limit < MSP_MAX_TMR_NUM ) {													// エラー未発生！！！（エラー：最大カウント分の検索にて発見できず）
			if ( crnt == MSP_TMR_TERM ) {													// < 最後尾にタイマーをセットする >
				field[prev].mNext = index ;
				field[index].mPrev = prev ;
			}
			else if ( crnt == (self->mTop) ) {												// < 先頭にタイマーをセットする >
				field[crnt].mPrev = index ;
				field[index].mNext = crnt ;
				self->mTop = index ;
			}
			else {																			// < 途中にタイマーをセットする >
				field[prev].mNext = index ;
				field[index].mPrev = prev ;
				field[crnt].mPrev = index ;
				field[index].mNext = crnt ;
			}
		}
	}
}


//=====================================================================================
// Function Name : CEventTimer::RemoveField()                                          
// Description   : タイマー管理情報を抹消する.                                         
// Parameter     : *self		"対象タイマー管理情報のアドレス"                       
//               : index		"抹消対象情報格納先インデックス"                       
// Return        : -                                                                   
//=====================================================================================
static void CEventTimer_RemoveField(CEventTimer *self, T_SZ index)
{
	TimerField	*field ;
	T_SZ  prev, next ;
	US	delay ;

// ----- タイマー情報の設定 -----
	field = self->mField ;																	// タイマー管理情報領域の先頭アドレスをセットする
	prev  = field[index].mPrev ;															// 抹消対象タイマー管理情報を取得する
	next  = field[index].mNext ;
	delay = field[index].mDelay ;

// ----- リンクの更新 -----
	if ( (self->mTop) == index ) {															// < 抹消対象が先頭につながれている >
		self->mTop = field[index].mNext ;
	}

	if ( prev != MSP_TMR_TERM ) {															// < 抹消対象の前につながれたタイマーがある >
		field[prev].mNext = next ;
	}

	if ( next != MSP_TMR_TERM ) {															// < 抹消対象の後につながれたタイマーがある >
		field[next].mPrev = prev ;
		field[next].mDelay += delay ;
	}
}


//=====================================================================================
// Function Name : CEventTimer::GetType()                                              
// Description   : コールバック･タイマーのタイマー優先度（HI/LO）を取得する.           
// Parameter     : *self		"対象タイマー管理情報のアドレス"                       
//               : id			"チェック対象タイマーID"                               
// Return        : タイマー優先度（HI/LO）（0x01:低優先度 / 0x11:高優先度）            
//=====================================================================================
static UC CEventTimer_GetType(CEventTimer *self, T_SZ id)
{
	return	((self->mField[id].mStatus) & 0x11) ;
}


//=====================================================================================
// Function Name : CEventTimer::ExecuteHandler()                                       
// Description   : コールバックハンドラーを実行する.                                   
// Parameter     : *self		"対象タイマー管理情報のアドレス"                       
//               : index		"実行対象情報格納先インデックス"                       
// Return        : -                                                                   
//=====================================================================================
static void CEventTimer_ExecuteHandler(CEventTimer *self, T_SZ index)
{
	TimerField	*field ;
	UC	key, type ;

	field = &(self->mField[index]) ;														// タイマー管理情報領域の先頭アドレスをセットする
	type  = CEventTimer_GetType(self, index) ;												// タイマー優先度を取得する

	key = LIB_GetKey() ; LIB_Lock() ;														// 注意!!! 割り込み禁止 ここから!!!
	if ( CEventTimer_IsRunning(self, index) == MSP_TMR_RUN ) {								// < イベントタイマーが起動中である >
		field->mStatus |= MSP_TMR_EXEC ;													// コールバック処理実行状態にする
	}																						// ※ 高優先度処理からのキャンセルをできなくする！
	LIB_Unlock(key) ;																		// 注意!!! 割り込み禁止 ここまで!!!

	if ( (field->mInfo.mpHandler) != MSP_HDLR_VOID ) {										// < ハンドラーのポインターが有効である >
		if ( ( (field->mZero) == SET ) && ( type == MSP_TMR_HIGH ) ) {						// < 高優先度タイマー使用かつ遅延設定が0[ms]であった >
			(field->mInfo.mpHandler)((SS)MSP_TMR_TERM, field->mInfo.mParam) ;				// コールバックハンドラーを実行する（IDは"MSP_TMR_TERM"）
		}
		else {																				// < 低優先度タイマー使用もしくは遅延ありの設定であった >
			(field->mInfo.mpHandler)((SS)index, field->mInfo.mParam) ;						// コールバックハンドラーを実行する（通常タイムアウト）
		}
	}
}


//
// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+----------------------------------------------------------------------------------------------------------------------------
// 14.02.20 | Mitsuyuki Nishizawa | MSP Frame > Event Timerのプロトタイプとして本ファイルを作成. (Rev.000)
// 14.09.08 | Mitsuyuki Nishizawa | コールバック実施直前での高優先度処理内からのタイマーキャンセルを考慮. (Rev.001)
// 14.09.24 | Mitsuyuki Nishizawa | 設定遅延時間0[ms]時のコールバックハンドラーのIDを"MSP_TMR_TERM"で実行. (Rev.002)
//
//


// *************************************************************  [ MSP_EventTimer.c : EOF ]  *****************************************************************

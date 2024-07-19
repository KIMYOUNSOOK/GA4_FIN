// ☆☆☆ Module Software Platform Frame ☆☆☆
/// @file    MSP_EventTimer.h
/// @author  Mitsuyuki Nishizawa
/// @date    14.11.05
/// @version 003
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef MSP_EventTimer_h
#define MSP_EventTimer_h


// ******************************************************  <<<<< Definition & Declaration Area >>>>>  *********************************************************

// ===========================  <<< マクロ定義 >>>  ===========================

// ----- タイマー管理情報領域の確保数 -----													// ☆☆☆ユーザー設定☆☆☆
#define	MSP_MAX_TMR_NUM		255																// 最大タイマー本数を設定（最大：512 … 255以下を推奨）

// ----- ターミネーター定義 -----
#if MSP_MAX_TMR_NUM > 255
	#define	MSP_TMR_TERM	0xFFFF															// タイマーリストのターミネーター（2バイト）
#else
	#define	MSP_TMR_TERM	0xFF															// タイマーリストのターミネーター（1バイト）
#endif

// ----- タイマータイプ (優先度) -----														// 注意）タイマーステータス情報 > bit0,1: ステータス, bit4: 優先度
#define	MSP_TMR_LOW			0x01															// 低優先度タイマー
#define	MSP_TMR_HIGH		0x11															// 高優先度タイマー

// ----- タイマーステータス定義 -----
#define	MSP_TMR_STOP		0x00															// タイマー未使用状態
#define	MSP_TMR_RUN			0x01															// タイマー起動状態
#define	MSP_TMR_EXEC		0x03															// タイマー･コールバック処理実行状態

// ----- 無効ハンドラー定義 -----
#define	MSP_HDLR_VOID		0																// 無効ハンドラー

// ===========================  <<< データ型定義 >>>  ===========================

// ----- タイマー情報管理用インデックス -----
#if MSP_MAX_TMR_NUM > 255
	typedef unsigned short	T_SZ ;															// タイマーインデックスの型（2バイト）
#else
	typedef unsigned char	T_SZ ;															// タイマーインデックスの型（1バイト）
#endif

// ----- コールバック・ハンドラー情報 -----
typedef struct {
	void (*mpHandler)(SS, UL) ;																// ハンドラーのアドレス
	UL	mParam ;																			// コールバック時のパラメータ
} Handler ;


// ----- タイマー管理情報 -----
typedef struct {
	Handler  mInfo ;																		// <<< コールバック情報 >>>
	T_SZ  mPrev ;																			// 前につながれたタイマー管理情報ID
	T_SZ  mNext ;																			// 後につながれたタイマー管理情報ID
	UC	  mStatus ;																			// タイマー使用状況
	UC	  mZero ;																			// 設定遅延時間0[ms]フラグ
	US	  mDelay ;																			// 遅延時間（前のタイマーとの差分）
} TimerField ;


// ********************************************************  <<<<< Class Declaration Area >>>>>  **************************************************************

// ===========================  <<< クラス定義 >>>  ===========================

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Class Name    : CEventTimer
// Description   : タイマー管理.
// Note          : なし
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
typedef struct EventTimer {
/*public:*/
	//=[[[ Constructor ]]]=================================================================
	// Function Name : CEventTimer::CEventTimer()                                          
	// Description   : このクラスのコンストラクタ.                                         
	// Parameter     : *self		"対象タイマー管理情報のアドレス"                       
	// Return        : -                                                                   
	//=====================================================================================
	// void CEventTimer(CEventTimer *self) {}

	//=====================================================================================
	// Function Name : CEventTimer::Set()                                                  
	// Description   : コールバック･タイマーをセットする.                                  
	// Parameter     : *self		"対象タイマー管理情報のアドレス"                       
	//               : type			"タイマー優先度（HI/LO）"                              
	//               : delay		"タイマー遅延時間[ms]"                                 
	//               : *handler		"コールバックハンドラー情報格納先アドレス"             
	// Return        : タイマーID（マイナスはエラー）                                      
	//=====================================================================================
	// SS Set(CEventTimer *self, UC type, US delay, Handler *handler)  {}

	//=====================================================================================
	// Function Name : CEventTimer::Cancel()                                               
	// Description   : コールバック･タイマーをキャンセルする.                              
	// Parameter     : *self		"対象タイマー管理情報のアドレス"                       
	//               : id			"キャンセル対象タイマーID"                             
	// Return        : 処理結果（0:処理成功 / -1:処理失敗）                                
	//=====================================================================================
	// SS Cancel(CEventTimer *self, SS id) {}

	//=====================================================================================
	// Function Name : CEventTimer::IsRunning()                                            
	// Description   : コールバック･タイマーの動作状態を取得する.                          
	// Parameter     : *self		"対象タイマー管理情報のアドレス"                       
	//               : id			"チェック対象タイマーID"                               
	// Return        : タイマー動作状態（0:停止中 / 1:動作中）                             
	//=====================================================================================
	// UC IsRunning(CEventTimer *self, SS id) {}

	//=====================================================================================
	// Function Name : CEventTimer::Tick()                                                 
	// Description   : カウントダウン処理を実行する.                                       
	// Parameter     : *self		"対象タイマー管理情報のアドレス"                       
	// Return        : -                                                                   
	//=====================================================================================
	// void Tick(CEventTimer *self) {}

	//=====================================================================================
	// Function Name : CEventTimer::Timeout()                                              
	// Description   : コールバック処理を実行する. (低優先度タイマー専用関数)              
	// Parameter     : *self		"対象タイマー管理情報のアドレス"                       
	//               : id			"チェック対象タイマーID"                               
	// Return        : -                                                                   
	//=====================================================================================
	// void Timeout(CEventTimer *self, SS id) {}

/*private:*/
	//=====================================================================================
	// Function Name : CEventTimer::Fire()                                                 
	// Description   : タイムアウト処理を実行する.                                         
	// Parameter     : *self		"対象タイマー管理情報のアドレス"                       
	//               : id			"チェック対象タイマーID"                               
	// Return        : -                                                                   
	//=====================================================================================
	// void Fire(CEventTimer *self, SS id) {}

	//=====================================================================================
	// Function Name : CEventTimer::ResetField()                                           
	// Description   : タイマー管理情報を初期化する.                                       
	// Parameter     : *self		"対象タイマー管理情報のアドレス"                       
	//               : index		"初期化対象情報格納先インデックス"                     
	/// Return        : -                                                                  
	//=====================================================================================
	// void ResetField(CEventTimer *self, T_SZ index) {}

	//=====================================================================================
	// Function Name : CEventTimer::SetField()                                             
	// Description   : タイマー管理情報をセットする.                                       
	// Parameter     : *self		"対象タイマー管理情報のアドレス"                       
	//               : index		"初期化対象情報格納先インデックス"                     
	//               : type			"タイマー優先度（HI/LO）"                              
	//               : delay		"タイマー遅延時間[ms]"                                 
	//               : *handler		"コールバックハンドラー情報格納先アドレス"             
	// Return        : -                                                                   
	//=====================================================================================
	// void SetField(CEventTimer *self, T_SZ index, UC type, US delay, Handler *handler) {}

	//=====================================================================================
	// Function Name : CEventTimer::SearchField()                                          
	// Description   : タイマー管理情報の空き領域を検索する.                               
	// Parameter     : *self		"対象タイマー管理情報のアドレス"                       
	// Return        : タイマー管理情報の空き領域を示すインデックス（-1:空き領域なし)      
	//=====================================================================================
	// T_SZ SearchField(CEventTimer *self) {}

	//=====================================================================================
	// Function Name : CEventTimer::HookField()                                            
	// Description   : タイマー管理情報を登録する.                                         
	// Parameter     : *self		"対象タイマー管理情報のアドレス"                       
	//               : index		"登録対象情報格納先インデックス"                       
	// Return        : -                                                                   
	//=====================================================================================
	// void HookField(CEventTimer *self, T_SZ index) {}

	//=====================================================================================
	// Function Name : CEventTimer::RemoveField()                                          
	// Description   : タイマー管理情報を抹消する.                                         
	// Parameter     : *self		"対象タイマー管理情報のアドレス"                       
	//               : index		"抹消対象情報格納先インデックス"                       
	// Return        : -                                                                   
	//=====================================================================================
	// void RemoveField(CEventTimer *self, T_SZ index) {}

	//=====================================================================================
	// Function Name : CEventTimer::GetType()                                              
	// Description   : コールバック･タイマーのタイマー優先度（HI/LO）を取得する.           
	// Parameter     : *self		"対象タイマー管理情報のアドレス"                       
	//               : id			"チェック対象タイマーID"                               
	// Return        : タイマー優先度（HI/LO）（0x01:低優先度 / 0x11:高優先度）            
	//=====================================================================================
	// UC CEventTimer_GetType(CEventTimer *self, T_SZ id) {}

	//=====================================================================================
	// Function Name : CEventTimer::ExecuteHandler()                                       
	// Description   : コールバックハンドラーを実行する.                                   
	// Parameter     : *self		"対象タイマー管理情報のアドレス"                       
	//               : index		"抹消対象情報格納先インデックス"                       
	// Return        : -                                                                   
	//=====================================================================================
	// void ExecuteHandler(CEventTimer *self, T_SZ index) {}

	TimerField	mField[MSP_MAX_TMR_NUM] ;														// タイマー管理情報本体
	T_SZ  mTop ;																				// 先頭につながれたタイマー管理情報ID
	T_SZ  mIndex ;																				// タイマー管理情報領域取得のためのインデックス

	T_SZ  mCount ;																				// 現在のタイマー使用本数	[[[ デバッグ用 ]]]
	T_SZ  mMax ;																				// タイマー使用最大本数		[[[ デバッグ用 ]]]
} CEventTimer ;

// ===========================  <<< 関数プロトタイプ宣言 >>>  ===========================
void CEventTimer_CEventTimer(CEventTimer*) ;													// タイマー管理初期化処理（コンストラクタ）
SS CEventTimer_Set(CEventTimer*, UC, US, Handler*) ;											// タイマーをセットする
SS CEventTimer_Cancel(CEventTimer*, SS) ;														// タイマーをキャンセルする
UC CEventTimer_IsRunning(CEventTimer*, SS) ;													// タイマー状態を取得する
void CEventTimer_Tick(CEventTimer*) ;															// タイマーカウント処理
void CEventTimer_Timeout(CEventTimer*, SS) ;													// タイマータイムアウトハンドラーを実行する


#endif			// MSP_EventTimer_h


//
// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+----------------------------------------------------------------------------------------------------------------------------
// 14.03.20 | Mitsuyuki Nishizawa | MSP Frame > Event Timerのプロトタイプとして本ファイルを作成. (Rev.000)
// 14.09.08 | Mitsuyuki Nishizawa | コールバック実施直前での高優先度処理内からのタイマーキャンセルを考慮. (Rev.001)
// 14.09.24 | Mitsuyuki Nishizawa | 設定遅延時間0[ms]判断用フラグ領域をメンバーに追加. (Rev.002)
// 14.11.05 | Mitsuyuki Nishizawa | タイマー処理結果を共通ヘッダーへ移動. (Rev.003)
//
//


// *************************************************************  [ MSP_EventTimer.h : EOF ]  *****************************************************************

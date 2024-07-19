// ☆☆☆ Module Software Platform Frame ☆☆☆
/// @file    MDL_Main.c
/// @author  Mitsuyuki Nishizawa
/// @date    13.05.05
/// @version 000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#define MDL_Main_c

#include "Common.h"
#include "MSP_Service.h"																	// このヘッダーファイルにはプロダクト依存部が記述されている。


//=====================================================================================
// Function Name : MDL_PrintFunctionChange()							[EVENT HANDLER]
// Description   : Print Function Change遷移時にスケジュールのリセットを実施する.
// Parameter     : *event (イベント情報の参照先アドレス)
//               : from (イベント発行元情報:μFrame)
// Return        : なし
// Caution       : ステートが"Print Function Change"に遷移した際の最初に実行されるよう
//               : にハンドラの登録を行うこと！！！
//=====================================================================================
void MDL_PrintFunctionChange(const UC *pEvent, UC from)
{
	MSP_ResetLocationSchedule() ;															// ロケーション･イベント･スケジュールのリセット処理
}


//=====================================================================================
// Function Name : MDL_PrintStandby()									[EVENT HANDLER]
// Description   : Print Standby遷移時にスケジュールの生成を実施する.
// Parameter     : *event (イベント情報の参照先アドレス)
//               : from (イベント発行元情報:μFrame)
// Return        : なし
// Caution       : ステートが"Print Standby"に遷移した際の最初に実行されるように
//               : ハンドラの登録を行うこと！！！
//=====================================================================================
void MDL_PrintStandby(const UC *pEvent, UC from)
{
	MSP_CreateLocationSchedule() ;															// ロケーション･イベント･スケジュールの生成処理
	MSP_AbortLocationScheduling() ;															// ロケーション･イベント発生処理を中止
}


// Change History
// Date:    | Engineer:            | Note:
// ---------+----------------------+---------------------------------------------------------------------------------------------------------------------------
// 13.05.05 | Mitsuyuki Nishizawa  | Module Software Platform Frameのプロトタイプとして本ファイルを作成.
//


// ******************************************************************  [ MDL_Main.c : EOF ]  ******************************************************************

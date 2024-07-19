// File Name: LIB_Log.c
// Description:
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 15.03.27
// Revision:  001
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#define LIB_Log_c

#include "Common.h"
#include "DD_LOG.h"
#include "LIB_Log.h"

// *******************************************************  <<<<< Definition & Declaration Area >>>>>  ********************************************************


// ************************************************************  <<<<< Program Code Area >>>>>  ***************************************************************

//=====================================================================================
// Function Name : Printf()
// Description   : デバッグ文字列を出力する.
// Parameter     : id			"デバッグ文字列出力モジュールID"
//               : SR			"送受信区分 ["S":送信 / "R":受信 / "0":デバッグ]
//               : *data		"デバッグ文字列格納先アドレス"
//               : size			"デバッグ文字列のサイズ [byte単位]"
// Return        : なし
//=====================================================================================
#ifndef Win32
void Printf(UC id, UC SR, UC *data, US size)
{
	DD_LOG_Put(id, SR, data, size, 0);
}
#endif


// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+--------------------------------------------------------------------------------------
// 13.10.15 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev001 Base)
// 15.03.27 | Yuichi Hoshino      | Printf()ない処理を実装
// 15.03.30 | Yuichi Hoshino      | Release Revision 001
//
//


// ****************************************************************  [ LIB_Log.c : EOF ]  *********************************************************************

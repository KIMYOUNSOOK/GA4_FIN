// ☆☆☆ Module Software Platform Library ☆☆☆
// File Name: LIB_Service.h
// Description:
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 13.10.15
// Revision:  000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef LIB_Service_h
#define LIB_Service_h

#include "DD_Event.h"
#include "LIB_Lock.h"
#include "LIB_Log.h"
#include "LIB_DataBuffer.h"
#include "LIB_MessageBuffer.h"
#include "LIB_StopWatch.h"


// *******************************************************  <<<<< Definition & Declaration Area >>>>>  ********************************************************

// ===========================  <<< 関数プロトタイプ宣言 >>>  ===========================
void LIB_StartUp() ;																	// ライブラリ初期化処理（メッセージ＆データバッファ初期化含む）
SS LIB_snd_mbf(EMbfID, UC*, SS) ;														// メッセージ送信処理
SS LIB_rcv_mbf(EMbfID, UC**) ;															// メッセージ受信処理


#endif			// LIB_Service_h


// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+--------------------------------------------------------------------------------------
// 13.10.15 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev000 Base)
//
//


// ****************************************************************  [ LIB_Service.h : EOF ]  *****************************************************************

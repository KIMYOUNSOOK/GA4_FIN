// ☆☆☆ Module Software Platform Frame ☆☆☆
/// @file    MSP_EventMain.h
/// @author  Mitsuyuki Nishizawa
/// @date    13.05.05
/// @version 000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef MSP_EventMain_h
#define MSP_EventMain_h


// *******************************************************  <<<<< Definition & Declaration Area >>>>>  ********************************************************

// ===========================  <<< 関数プロトタイプ宣言 >>>  ===========================
void MSP_ReceiveEvent() ;
void MSP_SendEvent(UC senderID, US eventID, const UC *pMessage, US length, UC log) ;

#endif			// MSP_EventMain_h


// Change History
// Date:    | Engineer:          | Note:
// ---------+--------------------+-----------------------------------------------------------------------------------------------------------------------------
// 13.05.05 | Mitsuyuki Nishizawa| Module Software Platform Frameのプロトタイプとして本ファイルを作成.
//
//


// ***************************************************************  [ MSP_EventMain.h : EOF ]  ****************************************************************

// ☆☆☆ Module Software Platform Frame ☆☆☆
/// @file    MSP_Sensor.h
/// @author  Mitsuyuki Nishizawa
/// @date    14.11.04
/// @version 001
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


#ifndef MSP_Sensor_h
#define MSP_Sensor_h

#include "DD_Event.h"


// *******************************************************  <<<<< Definition & Declaration Area >>>>>  ********************************************************

// ===========================  <<< マクロ定義 >>>  ===========================

// ----- 無効なポート･エリアID -----
#define	MSP_PORT_AREA_VOID	0xFF															// ポート･エリアIDは最大254まで使用可能！！！

// ----- ポート･エッジ･インデックス -----
#define	MSP_PORT_IDX_ON		0																// ポート･エッジ･インデックス [ON]
#define	MSP_PORT_IDX_OFF	1																// ポート･エッジ･インデックス [OFF]


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Class Name    : CSensorEvent
// Description   : センサー変化に応じたセンサー･イベントの生成･通知を実施する.
// Note          : なし
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//typedef struct SensorEvent {
/*public:*/
	//=[[[ Constructor ]]]=================================================================
	// Function Name : CSensorEvent::CSensorEvent()                                        
	// Description   : このクラスのコンストラクタ.                                         
	// Parameter     : *self		"対象タイマー管理情報のアドレス"                       
	// Return        : -                                                                   
	//=====================================================================================
	// void CSensorEvent(CSensorEvent *self) {}

	//=====================================================================================
	// Function Name : CSensorEvent::NotifyEdge()                                          
	// Description   : 入力ポートの変化を通知する.                                         
	// Parameter     : message		"エッジ通知メッセージ（ON/OFF）"                       
	//               : area			"ポートエリアID"                                       
	//               : port			"変化検知ポート情報"                                   
	// Return        : -                                                                   
	//=====================================================================================
	// void NotifyEdge(UC message, UC area, UL port) {}

/*private:*/

//} CSensorEvent ;


// ===========================  <<< 関数プロトタイプ宣言 >>>  ===========================
void CSensorEvent_NotifyEdge(UC, UC, UL) ;													// センサー変化検知の通知処理()					: void


#endif			// MSP_Sensor_h


// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+----------------------------------------------------------------------------------------------------------------------------
// 13.05.05 | Mitsuyuki Nishizawa | Module Software Platform Frameのプロトタイプとして本ファイルを作成.
// 14.11.04 | Mitsuyuki Nishizawa | 変化通知関数名称および引数を変更.
//
//


// ***************************************************************  [ MSP_Sensor.h : EOF ]  *******************************************************************

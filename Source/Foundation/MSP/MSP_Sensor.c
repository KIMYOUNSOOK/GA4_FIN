// ☆☆☆ Module Software Platform Frame ☆☆☆
/// @file    MSP_Sensor.c
/// @author  Mitsuyuki Nishizawa
/// @date    15.02.12
/// @version 002
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#define MSP_Sensor_c

#include "Common.h"
#include "LIB_Service.h"
#include "DD_DIO.h"

#include "MSP_Common.h"
#include "MSP_Config.h"
#include "MSP_EventID.h"
#include "MSP_ProductSpec.h"
#include "MSP_EventMain.h"
#include "MSP_Sensor.h"


//=====================================================================================
// Function Name : CSensorEvent::NotifyEdge()                                          
// Description   : 入力ポートの変化を通知する.                                         
// Parameter     : message		"エッジ通知メッセージ（ON/OFF）"                       
//               : area			"ポートエリアID"                                       
//               : port			"変化検知ポート情報"                                   
// Return        : -                                                                   
//=====================================================================================
void CSensorEvent_NotifyEdge(UC message, UC area, UL port)
{
	static const char  eMsg1[] = {"FRAME ERROR: Invalid Port Area"} ;
	
	US	start, count, index, eventID ;
	UC	edge ;
	
	if ( message == MSG_SNR_ON_EDGE ) edge = MSP_PORT_IDX_ON ;								// エッジ情報⇒インデックス変換
	else                              edge = MSP_PORT_IDX_OFF ;
		
	if ( ( area < MSP_NUM_OF_AREA ) && ( edge < 2 ) ) {										// ポート･エリア情報アクセス･データの範囲チェック

	// ----- ポート変化データから対応するイベントIDを検索する -----							// <<<<<<  ポート解析処理  >>>>>>
		start = msp_PortAreaInfo[area][edge].mStart ;										// センサー･イベント･テーブルへのアクセス情報を取得
		count = msp_PortAreaInfo[area][edge].mCount ;

		for ( index = start ; index < ((US)(start + count)) ; index++ ) {					// [[[ LOOP ]]] 有効ポート数分のチェックを実施する（ループ）
			if ( (port & GET_BIT_PATTERN(msp_SensorEdgeEvent[index])) != 0x0000 ) {			// ポート変化データをチェックする
				eventID = MSP_GetEventID(EVT_SNR_EDGE, index) ;
				MSP_SendEvent(MID_FRAME, eventID, (const UC*)&eventID, sizeof(US), OFF) ;	// センサー･イベントを送信する
			}
		}																					// [[[ LOOP ]]] （ここまでループ）
	}
	else {
		Printf(LOGID_FRAME, 0, (UC*)eMsg1, (US)sizeof(eMsg1)) ;
	}
}


// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+----------------------------------------------------------------------------------------------------------------------------
// 13.04.01 | Mitsuyuki Nishizawa | Module Software Platform Frameのプロトタイプとして本ファイルを作成.
// 14.11.04 | Mitsuyuki Nishizawa | 変化通知関数の引数を変更.
// 14.12.04 | Yuichi Hoshino      | Release version 001
// 15.02.12 | Yuichi Hoshino      | Printf()に渡すIDをLog IDに変更
// 15.03.30 | Yuichi Hoshino      | Release version 002
//
//


// ****************************************************************  [ MSP_Sensor.c : EOF ]  ******************************************************************

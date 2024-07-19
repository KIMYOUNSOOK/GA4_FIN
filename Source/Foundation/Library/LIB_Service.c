// ☆☆☆ Module Software Platform Library ☆☆☆
/// @file    LIB_Service.c
/// @author  Tsuyoshi Seto
/// @date    14.02.20
/// @version 000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#define LIB_Service_c

#include "Common.h"
#include "LIB_Service.h"
#include "LIB_DataBuffer.h"


// ******************************************************  <<<<< Definition & Declaration Area >>>>>  *********************************************************

// ===========================  <<< マクロ定義 >>>  =========================== 

// ----- バッファ空定義 -----
#define	LIB_MBF_EMPTY		0x00															// データバッファにデータがない状態


// ===========================  <<< 外部参照変数 >>>  ===========================
extern CDataBuffer		lib_DataBuffer ;													// [外部参照] データバッファ
extern CMessageBuffer	lib_MsgBuff[] ;														// [外部参照] メッセージ格納用バッファ
extern const MbfConfig 	lib_MbfConfig[] ;													// [外部参照] メッセージバッファコンフィギュレーション


// ************************************************************  <<<<< Program Code Area >>>>>  ***************************************************************

//=====================================================================================
// Function Name : LIB_StartUp()                                                       
// Description   : ライブラリの初期化.（一括して初期化する）                           
// Parameter     : なし                                                                
// Return        : なし                                                                
//=====================================================================================
void LIB_StartUp()
{
	US	index ;

// ----- データバッファ初期化 -----
	CDataBuffer_CDataBuffer(&lib_DataBuffer) ;												// データバッファを初期化する

// ----- メッセージバッファ初期化 -----
	for ( index = 0 ; index < eNumOfMbf ; index++ ) {										// 各メッセージバッファを初期化する
		CMessageBuffer_CMessageBuffer(&lib_MsgBuff[index], &lib_MbfConfig[index]) ;
	}
}


//=====================================================================================
// Function Name : LIB_snd_mbf()                                                       
// Description   : メッセージを送信する.                                               
// Parameter     : mbfID		"対象メッセージバッファID"                             
//               : *msg			"通知対象メッセージの先頭アドレス"                     
//               : size			"通知対象メッセージ長"                                 
// Return        : 送信処理結果（0：OK / -1：NG)                                       
//=====================================================================================
SS LIB_snd_mbf(EMbfID mbfID, UC *msg, SS size)
{
	return	lib_MsgBuff[mbfID].Send(&lib_MsgBuff[mbfID], msg, size) ;
}


//=====================================================================================
// Function Name : LIB_rcv_mbf()                                                       
// Description   : メッセージを受信する.                                               
// Parameter     : mbfID		"対象メッセージバッファID"                             
//               : **msg		"受信メッセージ格納エリア先頭アドレス書き込みアドレス" 
// Return        : 受信メッセージ長 … "0"はメッセージなし                             
// Note          : 同一メッセージバッファに対する受信処理は1か所で行うことを前提とする 
//=====================================================================================
SS LIB_rcv_mbf(EMbfID mbfID, UC **msg)
{
	return	lib_MsgBuff[mbfID].Receive(&lib_MsgBuff[mbfID], msg) ;
}


//
// Change History
// Date:    | Engineer:          | Note:
// ---------+--------------------+-----------------------------------------------------------------------------------------------------------------------------
// 14.02.20 | Tsuyoshi Seto      | MSP Library > Message Bufferのプロトタイプとして本ファイルを作成. (Rev.000)
//
//


// ***************************************************************  [ LIB_Service.c : EOF ]  ******************************************************************

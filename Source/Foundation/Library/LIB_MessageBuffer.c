// ☆☆☆ Module Software Platform Library ☆☆☆
/// @file    LIB_MessageBuffer.c
/// @author  Tsuyoshi Seto
/// @date    14.02.20
/// @version 000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#define LIB_MessageBuffer_c

#include "Common.h"
#include "LIB_Lock.h"
#include "LIB_DataBuffer.h"
#include "LIB_MessageBuffer.h"


// ******************************************************  <<<<< Definition & Declaration Area >>>>>  *********************************************************

// ===========================  <<< マクロ定義 >>>  =========================== 

// ----- バッファ空定義 -----
#define	LIB_MBF_EMPTY		0x00															// データバッファにデータがない状態


// ===========================  <<< 関数プロトタイプ宣言 >>>  ===========================
static SS CMessageBuffer_Send(CMessageBuffer*, UC*, SS) ;									// データバッファーへのデータ書き込み処理
static SS CMessageBuffer_Receive(CMessageBuffer*, UC**) ;									// データバッファーからのデータ読み出し処理


// ========================  <<< インスタンス生成 >>>  ========================

// ----- メッセージバッファ -----
CMessageBuffer  lib_MsgBuff[eNumOfMbf] ;													// メッセージバッファ本体（ID分の領域を確保）

// ----- メッセージ管理領域 ----- 															// ※注意：管理領域はメッセージバッファID数分確保すること！！！
static MessageQ  lib_MsgQFrame[LIB_MBF_FRM_SIZE] ;											// メッセージ管理領域：MSP Frame用の領域確保
static MessageQ  lib_MsgQDdLow[LIB_MBF_DD_LOW_SIZE] ;										// メッセージ管理領域：Device Driver低優先度用の領域確保
static MessageQ  lib_MsgQDdHigh[LIB_MBF_DD_HIGH_SIZE] ;										// メッセージ管理領域：Device Driver高優先度用の領域確保


// ===========================  <<< 外部参照変数 >>>  ===========================

// ----- データバッファ -----
extern CDataBuffer	lib_DataBuffer ;														// [外部参照] メッセージ格納用データバッファ


// ☆☆☆☆☆☆☆☆☆☆☆☆☆☆ <<< コンフィギュレーション >>> ☆☆☆☆☆☆☆☆☆☆☆☆☆☆

// ----- メッセージバッファコンフィギュレーション -----
const MbfConfig lib_MbfConfig[eNumOfMbf] = {
//	+--------------------+---------------------+---------------------+
//	| Message Q Address  | Message Q Size      | Data Buffer Address |
//	+--------------------+---------------------+---------------------+						// MBF ID : ※ID毎にコンフィギュレーション設定が必要となる！
	{ lib_MsgQFrame      , LIB_MBF_FRM_SIZE    , &lib_DataBuffer     },						//  0: eMbfFrame	: MSP Frame用メッセージバッファ
	{ lib_MsgQDdLow      , LIB_MBF_DD_LOW_SIZE , &lib_DataBuffer     },						//  1: eMbfDdLow    : Device Driver用低優先度メッセージバッファ
	{ lib_MsgQDdHigh	 , LIB_MBF_DD_HIGH_SIZE, &lib_DataBuffer     }						//  2: eMbfDdHigh	: Device Driver用高優先度メッセージバッファ
} ;


// ************************************************************  <<<<< Program Code Area >>>>>  ***************************************************************

//$$$$$$ [[[[[ Class: CMessageBuffer ]]]]] $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// Class Name    : CMessageBuffer
// Description   : メッセージバッファ
//               :
// Note          : なし
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

//=[[[ Constructor ]]]=================================================================
// Function Name : CMessageBuffer::CMessageBuffer()                                    
// Description   : このクラスのコンストラクタ.                                         
// Parameter     : *self		"対象メッセージバッファのアドレス"                     
//               : *config		"メッセージバッファコンフィギュレーションのアドレス"   
// Return        : -                                                                   
//=====================================================================================
void CMessageBuffer_CMessageBuffer(CMessageBuffer *self, const MbfConfig *config)
{
// ----- メンバー初期化 -----
	self->mpBuffer = config->pBuff ;														// データバッファのアドレスをセットする
	self->mpMsgQ   = config->pMsgQ ;														// メッセージ管理キューのアドレスをセットする
	self->mSize    = config->qSize ;														// メッセージバッファのサイズをセットする
	self->mRead    = self->mWrite = 0 ;														// 読み出し/書き込みインデックスをリセットする
	self->mStatus  = LIB_MBF_OK ;															// メッセージバッファステータスをリセットする

// ----- 関数ポインター設定 -----
	self->Send    = CMessageBuffer_Send ;													// Send()   ：メッセージ送信処理
	self->Receive = CMessageBuffer_Receive ;												// Receive()：メッセージ受信処理
}


//=====================================================================================
// Function Name : CMessageBuffer::Send()                                              
// Description   : メッセージバッファにメッセージを送信する.                           
// Parameter     : *self		"対象メッセージバッファのアドレス"                     
//               : *message		"通知対象メッセージの先頭アドレス"                     
//               : size			"通知対象メッセージ長"                                 
// Return        : 送信処理結果（0：OK / -1：NG)                                       
//=====================================================================================
static SS CMessageBuffer_Send(CMessageBuffer *self, UC *message, SS size)
{
	SS	offset ;
	US	index ;
	UC	key ;

	if ( self->mStatus != LIB_MBF_ERR ) {													// < ステータスがエラー状態でない >
		offset = self->mpBuffer->Write(self->mpBuffer, message, size) ;						// データバッファへメッセージ情報を書きこむ

		if ( offset != LIB_MBF_ERR ) {														// < データバッファ書き込みエラーは未発生 >
			key = LIB_GetKey() ; LIB_Lock() ;												// ！！！！！！！！！！ 割り込み禁止 ！！！！！！！！！！
			index = self->mWrite ;															// 書き込みインデックスをセーブする
			self->mpMsgQ[index].offset = offset ;											// 管理情報の登録：
			self->mpMsgQ[index].size   = size ;
			
			if ( ++index >= (self->mSize) ) index = 0 ;										// 書き込みインデックスを更新する
			
			if ( index != (self->mRead) ) {													// < メッセージキューに空きがある >
				self->mWrite = index ;														// ⇒ 書き込みインデックスを更新する
			}
			else {																			// < メッセージキューに空きがない >
				self->mStatus = LIB_MBF_ERR ;												// ⇒ エラー発生（管理キューフル）
			}
			LIB_Unlock(key) ;																// ！！！！！！！！！！ 割り込み許可 ！！！！！！！！！！
		}
		else {																				// < データバッファ書き込みエラーが発生 >
			self->mStatus = LIB_MBF_ERR ;													// メッセージバッファステータスをエラーにする
		}
	}

	return	(SS)(self->mStatus) ;
}


//=====================================================================================
// Function Name : CMessageBuffer::Receive()                                           
// Description   : メッセージバッファからメッセージを受信する.                         
// Parameter     : *self		"対象メッセージバッファのアドレス"                     
//               : **message	"受信メッセージ格納エリア先頭アドレス書き込みアドレス" 
// Return        : 受信メッセージ長 … "0"はメッセージなし                             
// Note          : 同一メッセージバッファに対する受信処理は1か所で行うことを前提とする 
//=====================================================================================
static SS CMessageBuffer_Receive(CMessageBuffer *self, UC **message)
{
	SS	size   = 0 ;
	SS	offset = 0 ;
	SS	result = LIB_DBF_OK ;
	
	if ( self->mStatus != LIB_MBF_ERR ) {													// < ステータスがエラー状態でない >
		if ( (self->mRead) != (self->mWrite) ) {											// < メッセージバッファにメッセージがある >
			size   = self->mpMsgQ[self->mRead].size ;										// メッセージ長をセーブする
			offset = self->mpMsgQ[self->mRead].offset ;	
			
			result = self->mpBuffer->Read(self->mpBuffer, message, offset, size) ;			// データバッファからメッセージ情報を読み出す
			if ( result != LIB_DBF_ERR ) {													// < データバッファ読み出しエラーは未発生 >
				if ( ++(self->mRead) >= (self->mSize) ) self->mRead = 0 ;					// 読み出しインデックスを更新する
			}
			else {
				size = 0 ;
			}
		}
	}

	return	size ;
}


//
// Change History
// Date:    | Engineer:          | Note:
// ---------+--------------------+-----------------------------------------------------------------------------------------------------------------------------
// 14.02.20 | Tsuyoshi Seto      | MSP Library > Message Bufferのプロトタイプとして本ファイルを作成. (Rev.000)
//
//


// ************************************************************  [ LIB_MessageBuffer.c : EOF ]  ***************************************************************

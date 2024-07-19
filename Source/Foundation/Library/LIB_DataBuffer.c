// ☆☆☆ Module Software Platform Library ☆☆☆
/// @file    LIB_DataBuffer.c
/// @author  Tsuyoshi Seto
/// @date    15.01.13
/// @version 003
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#define LIB_DataBuffer_c

#include "Common.h"
#include "LIB_Lock.h"
#include "LIB_DataBuffer.h"


// ******************************************************  <<<<< Definition & Declaration Area >>>>>  *********************************************************

// ===========================  <<< マクロ定義 >>>  ===========================

// ----- バッファ空定義 -----
#define	LIB_DBF_EMPTY		0x00															// データバッファにデータがない状態

// ===========================  <<< 関数プロトタイプ宣言 >>>  ===========================
static SS CDataBuffer_Write(CDataBuffer*, UC*, SS) ;										// データバッファへのデータ書き込み処理
static SS CDataBuffer_Read(CDataBuffer*, UC**, SS, SS) ;									// データバッファからのデータ読み出し処理


// ========================  <<< インスタンス生成 >>>  ========================

// ----- データバッファ -----
CDataBuffer lib_DataBuffer ;																// データバッファ (1つのバッファを複数のキューで共有可能)


// ************************************************************  <<<<< Program Code Area >>>>>  ***************************************************************

//$$$$$$ [[[[[ Class: CDataBuffer ]]]]] $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// Class Name    : CDataBuffer
// Description   : データ格納用バッファ
//               :
// Note          : なし
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

//=[[[ Constructor ]]]=================================================================
// Function Name : CDataBuffer::CDataBuffer()                                          
// Description   : このクラスのコンストラクタ.                                         
// Parameter     : *self		"対象データバッファのアドレス"                         
// Return        : -                                                                   
//=====================================================================================
void CDataBuffer_CDataBuffer(CDataBuffer *self)
{
	US	index ;

// ----- メンバー初期化 -----
	self->mWrite = 0 ;																		// 書き込み用インデックスをリセットする
	self->mStatus = LIB_DBF_OK ;                                                            // データバッファステータスをリセットする

	for ( index = 0 ; index < ((LIB_DBF_SIZE - 1) / 4 + 1) ; index++ ) {					// データバッファをクリアする
		self->mBuffer[index] = LIB_DBF_EMPTY ;
	}

// ----- 関数ポインター設定 -----
	self->Write = CDataBuffer_Write ;														// Write()：データ書き込み処理
	self->Read  = CDataBuffer_Read ;														// Read() ：データ読み出し処理
}

//=====================================================================================
// Function Name : CDataBuffer::Write()                                                
// Description   : データバッファにデータを書き込む.                                   
// Parameter     : *self		"対象データバッファのアドレス"                         
//               : *data		"書き込み対象データの先頭アドレス"                     
//               : size			"書き込み対象データのサイズ"                           
// Return        : 書き込み開始位置（オフセットバイト数）… マイナスは書き込みエラー   
//=====================================================================================
static SS CDataBuffer_Write(CDataBuffer *self, UC *data, SS size)
{
	SS	result ;
	UC	key ;

	if ( ( size < LIB_DBF_SIZE ) && ( self->mStatus != LIB_DBF_ERR ) ) {					// < 書き込みサイズがバッファサイズを超えない > ⇒ 処理継続
		key = LIB_GetKey() ; LIB_Lock() ;													// ！！！！！！！！！！ 割り込み禁止 ！！！！！！！！！！
		if ( ((self->mWrite) + size ) > LIB_DBF_SIZE ) self->mWrite = 0 ;					// 残りサイズよりも格納メッセージが大きい ⇒ 書き込み用インデックスを更新する
		result = self->mWrite ;

		memcpy(((UC*)(self->mBuffer)) + (self->mWrite), data, size) ;						// バッファーへのデータコピーを実施する
		(self->mWrite) += ((size + LIB_DATA_ALIGN) & LIB_ALIGN_MASK) ;						// 書き込み用インデックスにバウンダリー調整済み書き込みサイズを加算する
		if ( (self->mWrite) >= LIB_DBF_SIZE ) self->mWrite = 0 ;							// 書き込み用インデックスを更新する
		LIB_Unlock(key) ;																	// ！！！！！！！！！！ 割り込み許可 ！！！！！！！！！！
	}
	else {																					// < 書き込みサイズがバッファサイズを超える > ⇒ エラー発生
		self->mStatus = LIB_DBF_ERR ;														// データバッファステータスをエラーにする
		result = LIB_DBF_ERR ;
	}

	return	result ;
}


//=====================================================================================
// Function Name : CDataBuffer::Read()                                                 
// Description   : データバッファからデータを読み出す.                                 
// Parameter     : *self		"対象データバッファのアドレス"                         
//               : offset		"読み出し対象データのバッファ読み出し開始オフセット"   
//               : **area		"受信メッセージ格納エリア先頭アドレス書き込みアドレス" 
//               : size			"読み出し対象データのサイズ"                           
// Return        : データ読み出し結果（0：OK / -1:NG)                                  
//=====================================================================================
static SS CDataBuffer_Read(CDataBuffer *self, UC **area, SS offset, SS size)
{
	US	result = LIB_DBF_OK ;

	if ( ( size < LIB_DBF_SIZE ) && ( self->mStatus != LIB_DBF_ERR ) ) {					// < 読み出しサイズがバッファサイズを超えない > ⇒ 処理継続
		*area = ((UC*)(self->mBuffer)) + offset ;											// ユーザー領域へのデータコピーを実施する
	}
	else {																					// < 書き込みサイズがバッファサイズを超える > ⇒ エラー発生
		self->mStatus = LIB_DBF_ERR ;														// データバッファステータスをエラーにする
		result = LIB_DBF_ERR ;
	}

	return	result ;
}


//
// Change History
// Date:    | Engineer:          | Note:
// ---------+--------------------+-----------------------------------------------------------------------------------------------------------------------------
// 14.02.20 | Tsuyoshi Seto      | MSP Library > Data Bufferのプロトタイプとして本ファイルを作成. (Rev.000)
// 14.09.16 | Tsuyoshi Seto      | Data Bufferへのデータ書き込みをループからmemcpy使用に変更. (Rev.001)
// 14.09.22 | Tsuyoshi Seto      | Data Buffer領域への書き込みを4バイト境界に実施するようインデックスの更新を修正. (Rev.002)
// 15.01.13 | Tsuyoshi Seto      | Data Bufferへの書き込みバッティング（通常/割込み）の際のデータ消失トラブルへの対応. (Rev.003)
//
//


// *************************************************************  [ LIB_DataBuffer.c : EOF ]  *****************************************************************

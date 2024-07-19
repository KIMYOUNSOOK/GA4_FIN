// ☆☆☆ Module Software Platform Library ☆☆☆
/// @file    LIB_DataBuffer.h
/// @author  Tsuyoshi Seto
/// @date    14.09.22
/// @version 001
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// #####  ☆☆☆ Data Buffer使用時の設定項目および注意事項 ☆☆☆  ############################################################################################
// < 本ファイルにおけるプロダクト･ユニークな設定 >
//  ・本ファイルにおける『バッファ・サイズ（LIB_DBF_SIZE）』は各プロダクトにて設定が必要となる。（コメントに“☆☆☆ プロダクト･ユニーク ☆☆☆”と記載）
//
// < データバッファ使用時の注意事項 >
//  ・本データバッファはリングバッファにより構成されるため、後からWriteした情報により以前のデータは上書きされる。
//    そのため、新しいデータで上書きされる前に読み出しができるよう、十分なサイズを確保する必要がある。
// ############################################################################################################################################################

#ifndef LIB_DataBuffer_h
#define LIB_DataBuffer_h


// ******************************************************  <<<<< Definition & Declaration Area >>>>>  *********************************************************

// ===========================  <<< マクロ定義 >>>  ===========================

// ----- データバッファのサイズ(バイト数) -----
#define	LIB_DBF_SIZE		0x0400															// バッファ・サイズを設定		☆☆☆ プロダクト･ユニーク ☆☆☆

// ----- バッファ処理結果 -----
#define	LIB_DBF_OK			0
#define	LIB_DBF_ERR			(-1)

// ----- データバッファアライメント調整 -----
#define	LIB_DATA_ALIGN		0x0003															// 4バイト境界配置用定数
#define	LIB_ALIGN_MASK		(~LIB_DATA_ALIGN)


// ********************************************************  <<<<< Class Declaration Area >>>>>  **************************************************************

// ===========================  <<< クラス定義 >>>  ===========================

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Class Name    : CDataBuffer
// Description   : データ格納用バッファ
// Note          : なし
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
typedef struct DataBuffer {
/*public:*/
	//=[[[ Constructor ]]]=================================================================
	// Function Name : CDataBuffer::CDataBuffer()                                          
	// Description   : このクラスのコンストラクタ.                                         
	// Parameter     : *self		"対象データバッファのアドレス"                         
	// Return        : -                                                                   
	//=====================================================================================
	// void CDataBuffer(CDataBuffer *self) {}

	//=====================================================================================
	// Function Name : CDataBuffer::Write()                                                
	// Description   : データバッファにデータを書き込む.                                   
	// Parameter     : *self		"対象データバッファのアドレス"                         
	//               : *data		"書き込み対象データの先頭アドレス"                     
	//               : size			"書き込み対象データのサイズ"                           
	// Return        : 書き込み開始位置（オフセットバイト数）… マイナスは書き込みエラー   
	//=====================================================================================
	// SS Write(CDataBuffer *self, UC *data, SS size) {}
	SS (*Write)(struct DataBuffer*, UC*, SS) ;

	//=====================================================================================
	// Function Name : CDataBuffer::Read()                                                 
	// Description   : データバッファからデータを読み出す.                                 
	// Parameter     : *self		"対象データバッファのアドレス"                         
	//               : **area		"受信メッセージ格納エリア先頭アドレス書き込みアドレス" 
	//				 : offset		"読み出し開始位置（オフセットバイト数）                
	//               : size			"読み出し対象データのサイズ"                           
	// Return        : データ読み出し結果（0：OK / -1:NG)                                  
	//=====================================================================================
	// SS Read(CDataBuffer *self, SS offset, UC *area, SS size) {}
	SS (*Read)(struct DataBuffer*, UC**, SS, SS) ;

/*private:*/
	UL	mBuffer[(LIB_DBF_SIZE - 1) / 4 + 1] ;													// データバッファ本体（4バイト境界配置）
	US	mWrite ;																				// 書き込み開始インデックス
	SC	mStatus ;																				// データバッファの状態
} CDataBuffer ;

// ===========================  <<< 関数プロトタイプ宣言 >>>  ===========================
void CDataBuffer_CDataBuffer(CDataBuffer*) ;													// データバッファ初期化処理


#endif			// LIB_DataBuffer_h


//
// Change History
// Date:    | Engineer:          | Note:
// ---------+--------------------+-----------------------------------------------------------------------------------------------------------------------------
// 14.02.20 | Tsuyoshi Seto      | MSP Library > Data Bufferのプロトタイプとして本ファイルを作成. (Rev.000)
// 14.09.22 | Tsuyoshi Seto      | Data Buffer領域を4バイト境界に配置するよう型を修正. (Rev.001)
//
//


// *************************************************************  [ LIB_DataBuffer.h : EOF ]  *****************************************************************

// ☆☆☆ Module Software Platform Library ☆☆☆
/// @file    LIB_MessageBuffer.h
/// @author  Tsuyoshi Seto
/// @date    14.02.20
/// @version 000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef LIB_MessageBuffer_h
#define LIB_MessageBuffer_h


// ******************************************************  <<<<< Definition & Declaration Area >>>>>  *********************************************************

// ===========================  <<< マクロ定義 >>>  ===========================

// ----- メッセージバッファのサイズ(バイト数) -----											// ※注意：サイズ定義はメッセージバッファID数分必要となる！！！
#define	LIB_MBF_FRM_SIZE		0x0080														// メッセージバッファ：MSP Frame用のバッファサイズ
#define	LIB_MBF_DD_HIGH_SIZE	0x0040														// メッセージバッファ：Device Driver高優先度用のバッファサイズ
#define	LIB_MBF_DD_LOW_SIZE		0x0040														// メッセージバッファ：Device Driver低優先度用のバッファサイズ

// ----- バッファ処理結果 -----
#define	LIB_MBF_OK			0
#define	LIB_MBF_ERR			(-1)

// ===========================  <<< 列挙型定義 >>>  ===========================

// ----- メッセージバッファ識別子 -----
typedef enum {
	eMbfFrame ,																				//  0: MSP Frame用メッセージバッファ
	eMbfDdLow ,																				//  1: Device Driver用低優先度メッセージバッファ
	eMbfDdHigh,																				//  2: Device Driver用高優先度メッセージバッファ
	eNumOfMbf 																				// XX: メッセージバッファの数（これより上に識別子を定義する）
} EMbfID ;

// ===========================  <<< データ型定義 >>>  ===========================

// ----- メッセージキュー -----
typedef struct {
	SS	offset ;																			// データバッファへの書き込み位置
	SS	size ;																				// データバッファへの書き込みサイズ
} MessageQ ;

// ----- メッセージバッファコンフィギュレーション -----
typedef struct {
	MessageQ	*pMsgQ ;																	// メッセージキューアドレス
	US			qSize ;																		// メッセージキューサイズ
	CDataBuffer	*pBuff ;																	// データバッファアドレス
} MbfConfig ;


// ********************************************************  <<<<< Class Declaration Area >>>>>  **************************************************************

// ===========================  <<< クラス定義 >>>  ===========================

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Class Name    : CMessageBuffer
// Description   : メッセージバッファ
// Note          : なし
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
typedef struct MessageBuffer {
/*public:*/
	//=[[[ Constructor ]]]=================================================================
	// Function Name : CMessageBuffer::CMessageBuffer()                                    
	// Description   : このクラスのコンストラクタ.                                         
	// Parameter     : *self		"対象メッセージバッファのアドレス"                     
	//               : *config		"メッセージバッファコンフィギュレーションのアドレス"   
	// Return        : -                                                                   
	//=====================================================================================
	// void CMessageBuffer(CMessageBuffer *self, const MbfConfig *config) {}

	//=====================================================================================
	// Function Name : CMessageBuffer::Send()                                              
	// Description   : メッセージバッファにメッセージを送信する.                           
	// Parameter     : *self		"対象メッセージバッファのアドレス"                     
	//               : *message		"通知対象メッセージの先頭アドレス"                     
	//               : size			"通知対象メッセージのサイズ"                           
	// Return        : 送信処理結果（0：OK / -1：NG)                                       
	//=====================================================================================
	// SS Send(CMessageBuffer *self, UC *message, SS size) {}
	SS (*Send)(struct MessageBuffer*, UC*, SS) ;

	//=====================================================================================
	// Function Name : CMessageBuffer::Receive()                                           
	// Description   : メッセージバッファからメッセージを受信する.                         
	// Parameter     : *self		"対象メッセージバッファのアドレス"                     
	//               : **message	"受信メッセージ格納エリア先頭アドレス書き込みアドレス" 
	// Return        : 受信メッセージサイズ（オフセットバイト数）… 0はメッセージなし      
	//=====================================================================================
	// SS Receive(CMessageBuffer *self, UC **message) {}
	SS (*Receive)(struct MessageBuffer*, UC**) ;

/*private:*/
	CDataBuffer	*mpBuffer ;																		// データバッファへのポインター
	MessageQ	*mpMsgQ ;																		// メッセージ管理キュー領域へのポインター

	US	mSize ;																					// メッセージ管理キューのサイズ（キューできるメッセージ数）
	US	mRead ;																					// 読み出しインデックス
	US	mWrite ;																				// 書き込みインデックス
	SC	mStatus ;																				// メッセージバッファの状態
} CMessageBuffer ;


// ===========================  <<< 関数プロトタイプ宣言 >>>  ===========================
void CMessageBuffer_CMessageBuffer(CMessageBuffer*, const MbfConfig*) ;							// メッセージバッファ初期化処理


#endif			// LIB_MessageBuffer_h


//
// Change History
// Date:    | Engineer:          | Note:
// ---------+--------------------+-----------------------------------------------------------------------------------------------------------------------------
// 14.02.20 | Tsuyoshi Seto      | MSP Library > Message Bufferのプロトタイプとして本ファイルを作成. (Rev.000)
//
//


// ************************************************************  [ LIB_MessageBuffer.h : EOF ]  ***************************************************************

// File Name: DD_LOG.c
// Engineer:  Morikawa Atsushi
// Last Edit: 14.11.13
// Revision:  000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:        | Note:
// ---------+------------------+-------------------------------------------------------------
// 14.11.13 | Morikawa Atsushi | Created this file.
// 15.01.05 | Morikawa Atsushi | Added Log Send Function.
// 15.02.04 | Morikawa Atsushi | Changed a method using MSP Event to a method using DD Event.
// 15.02.12 | Morikawa Atsushi | Added Log ID. (LOGID_FRAME)

#include "Common.h"
#include "DD_SCI.h"
#include "DD_LOG.h"

/* Data */
#define	DD_LOG_QUEUE_SIZE	1024

// ID Name
// 「ID Name」を変更する場合は"DD_LOG.h"の「Log ID」も変更し、
// 「Log ID」で定義した数値と、該当する「ID Name」の通し番号を一致させること。
static const UC DD_LOG_IDName[LOGID_MAX][4] =
					 {'F','I','N',':',			// 0:固定 基盤チェックで使用しているため変更不可
					  'I','O','T',':',			// 1:
					  'F','R','M',':',			// 2:
					  'S','C','B',':',			// 3:
					  'S','Y','S',':',			// 4:
					  'S','L','C',':',			// 5:
					  'J','A','M',':',			// 6:
					  'R','T','C',':',			// 7:
					  'L','O','G',':',			// 8:
					  'M','T','R',':',			// 9:
					  'E','J','T',':',			// 10:
					  'S','T','P',':',			// 11:
					  'T','M','P',':',			// 12:
					  'S','T','K',':',			// 13:
					  'T','N','S',':',			// 14:
					  'F','L','D',':',			// 15:
					  'B','K','T',':',			// 16:
					  'D','E','V',':',			// 17:
					  'S','H','F',':',			// 18:
					  'I','N','T',':',	 		// 19:
					  'C','M','L',':',			// 20:
					  'S','H','T',':',			// 21:
					  'S','Y','S',':',			// 22:
					  'T','M','P',':',			// 23:
					  'T','C','B',':',			// 24:
					  'S','C','C',':',			// 25:
					  'S','C','I',':',			// 26:
					  'S','C','P',':',			// 27:
					  'S','C','T',':',			// 28:
					  'S','C','S',':',			// 29:
					  'S','C','V',':',};		// 30:

static const UC DD_LOG_StopMsg[]    = {'S','T','O','P'};
static const UC DD_LOG_RestartMsg[] = {'R','S','T','A'};
static UC sndBuff[225] = {0};
static US sndDataSize = 0;

typedef enum{
	// NonState = 0,
	stIdle_ID = 1,
	stSending_ID,
} DD_LOG_StatusID;
static DD_LOG_StatusID gState;

typedef struct {
	US	readPtr;								// 読み出し先Pointer
	US	writePtr;								// 格納先Pointer
	UC	msgQueue[DD_LOG_QUEUE_SIZE];			// 格納領域
} DD_LOG_RCV_BUFF;
static DD_LOG_RCV_BUFF DD_LOG_RcvBuff;
static UC full;									// 0:Not Full, 1:Full

/* ProtoType */
static void DD_LOG_Send(void);
static US DD_LOG_Get(UC *data);
static void stIdle(void);
static void stSending(void);
static UC DD_LOG_HexToAsciiH(UC dat);
static UC DD_LOG_HexToAsciiL(UC dat);

//=============================================================================
// Function Name : DD_LOG_Init
// Description   : LOGデータ送信モジュールの初期化
// Parameter     : -
// Return        : -
// Note          : -
//=============================================================================
void DD_LOG_Init(void)
{
	DD_LOG_RCV_BUFF *pRcvBuff;
	pRcvBuff = &DD_LOG_RcvBuff;
	pRcvBuff->writePtr = 0;
	pRcvBuff->readPtr = 0;
	gState = stIdle_ID;
	full = 0;
}

//=============================================================================
// Function Name : DD_LOG_Put
// Description   : LOGデータの受信バッファ格納
// Parameter     : id		Log ID
//				 : sr		データ種類(Bin/文字列指定)
//				 : *data	LOG格納エリアの先頭ポインタ
//				 : logSize	Logサイズ
//				 : category	LOGカテゴリ
// Return        : -
// Note          : -
//=============================================================================
void DD_LOG_Put(UC id, UC sr, UC *data, US logSize, UC category)
{
	US queueSpace, i;
	DD_LOG_RCV_BUFF *pRcvBuff;
	US writePos;
	UL sysTim = DD_EVT_GetCurrentTime();
	US size;
	static UL nvmLogSel;
	static US nvmLogSel16To31;
	static UC nvmLogCat;

#ifdef ENABLE_FPGARAM
	if ( get_FPGA_RAM_Status() == 0 ) {			// FPGA RAM アクセスエラー
		return;
	}
#endif
	if ( id > LOGID_MAX ) {						// 不正なLog ID
		return;
	}

	nvmLogSel = 0xFFFF;							// NVMの関数が未実装のため、今のところ0xFFFFで決め打ち
	nvmLogSel16To31 = 0xFFFF;					// NVMの関数が未実装のため、今のところ0xFFFFで決め打ち
	nvmLogSel |= ((UL)nvmLogSel16To31 << 16);
	if ( ((nvmLogSel >> id) & 1) == 0 ) {		// Log IDによる選択　0なら記憶しない
		return;
	}

	nvmLogCat = 0b111;						// NVMの関数が未実装のため、今のところ7(0b111)で決め打ち
	if ( ((nvmLogCat >> category) & 1) == 0 ) {	// Categoryによる選択　0なら記憶しない
		return;
	}

	pRcvBuff = &DD_LOG_RcvBuff;
	size = logSize;

	if ( full > 0 ) {							// Full状態
		return;									// 受信バッファのLogデータを全て出力し終えるまで新しく格納しない
	}
	if ( logSize > DD_LOG_MAX_DATA_SIZE ) {		// MAXより大きい場合は、MAXまで記憶
		size = DD_LOG_MAX_DATA_SIZE;
	}

	if ( pRcvBuff->writePtr < pRcvBuff->readPtr ) {	// 格納領域空き容量算出
		queueSpace = (US)(pRcvBuff->readPtr - pRcvBuff->writePtr);
	}
	else {
		queueSpace = (US)(DD_LOG_QUEUE_SIZE - (pRcvBuff->writePtr - pRcvBuff->readPtr));
	}

	if ( queueSpace < (size + 22) ) {			// 格納領域チェック: データサイズ + データ以外の固定サイズ(9[byte]) ＋ Stopメッセージサイズ(13[byte])
		full = 1;
		size = 4;
		id = LOGID_LOG;
		sr = '0';
		data = (UC *)DD_LOG_StopMsg;			// StopメッセージをBufferに格納
	}

	writePos = pRcvBuff->writePtr;				// Queue Write Pointer取得

	// Header
	pRcvBuff->msgQueue[writePos] = (UC)(0xFF);
	writePos++;
	writePos &= (DD_LOG_QUEUE_SIZE - 1);
	pRcvBuff->msgQueue[writePos] = (UC)(0xAA);
	writePos++;
	writePos &= (DD_LOG_QUEUE_SIZE - 1);

	// Time
	pRcvBuff->msgQueue[writePos] = (UC)(sysTim >> 16);
	writePos++;
	writePos &= (DD_LOG_QUEUE_SIZE - 1);
	pRcvBuff->msgQueue[writePos] = (UC)(sysTim >> 8);
	writePos++;
	writePos &= (DD_LOG_QUEUE_SIZE - 1);
	pRcvBuff->msgQueue[writePos] = (UC)(sysTim & 0x0FF);
	writePos++;
	writePos &= (DD_LOG_QUEUE_SIZE - 1);

	// ID
	pRcvBuff->msgQueue[writePos] = (UC)(id);
	writePos++;
	writePos &= (DD_LOG_QUEUE_SIZE - 1);

	// SR
	pRcvBuff->msgQueue[writePos] = (UC)(sr);
	writePos++;
	writePos &= (DD_LOG_QUEUE_SIZE - 1);

	// Size
	pRcvBuff->msgQueue[writePos] = (UC)(size >> 8);
	writePos++;
	writePos &= (DD_LOG_QUEUE_SIZE - 1);
	pRcvBuff->msgQueue[writePos] = (UC)(size & 0x0FF);
	writePos++;
	writePos &= (DD_LOG_QUEUE_SIZE - 1);

	// Data
	for ( i = 0; i < size; i++ ) {
		pRcvBuff->msgQueue[writePos] = *(data + i);
		writePos++;
		writePos &= (DD_LOG_QUEUE_SIZE - 1);
	}

	pRcvBuff->writePtr = writePos;				// Writeポインタ更新
	DD_EVT_Put(evSend_ID, evuDD_LOG_ID, 0, 0);	// Sendイベント登録
}

//=============================================================================
// Function Name : DD_LOG_Notify
// Description   : Log送信イベント発生時にコールされ、イベントIDに応じた処理を行う
// Parameter     : eventID	イベントID
// Return        : pMsg		データポインタ
//=============================================================================
void DD_LOG_Notify(DD_EventID eventID, UC* pMsg)
{
	switch ( eventID ) {
	case evSend_ID:
		switch ( gState ) {
		case stIdle_ID:
			stSending();
			break;
		case stSending_ID:
			break;
		default:
			break;
		}
		break;
	case evComplete_ID:
		switch ( gState ) {
		case stIdle_ID:
			break;
		case stSending_ID:
			stIdle();
			break;
		default:
			break;
		}
		break;
	}
}

//=============================================================================
// Function Name : DD_LOG_Send
// Description   : LOGデータの送信
// Parameter     : -
// Return        : -
// Note          : -
//=============================================================================
void DD_LOG_Send(void)
{
	sndDataSize = DD_LOG_Get(sndBuff);
	if ( sndDataSize ) {
		DD_SCI_LogPacketSend(sndBuff, sndDataSize);
	}
}

//=============================================================================
// Function Name : DD_LOG_Get
// Description   : LOGデータを受信バッファから取り出し、出力形式に変換する
// Parameter     : *data	送信データの先頭アドレス
// Return        : size		送信データサイズ
//=============================================================================
US DD_LOG_Get(UC *data)
{
	US dataSize;
	US readPos;
	US i;
	DD_LOG_RCV_BUFF *pRcvBuff;
	US size;
	US mix;										// 0:Binモード, 1:Mixモード
	US logID;

	pRcvBuff = &DD_LOG_RcvBuff;
	size = 0;
	readPos = pRcvBuff->readPtr;				// Readポインタ取得

	// Header
	*data = pRcvBuff->msgQueue[readPos];
	readPos++;
	readPos &= (DD_LOG_QUEUE_SIZE - 1);			// 受信バッファ終端を超えたら先頭に戻る
	if ( *data != 0xFF ) {
		return(0);
	}
	*data = pRcvBuff->msgQueue[readPos];
	readPos++;
	readPos &= (DD_LOG_QUEUE_SIZE - 1);
	if ( *data != 0xAA ) {
		return(0);
	}
	// Headerは送信しないため、送信バッファに格納しない

	// Time
	for ( i = 0; i < 3; i++ ) {
		*data = DD_LOG_HexToAsciiH(pRcvBuff->msgQueue[readPos]);
		data++;
		size++;
		*data = DD_LOG_HexToAsciiL(pRcvBuff->msgQueue[readPos]);
		data++;
		size++;
		readPos++;
		readPos &= (DD_LOG_QUEUE_SIZE - 1);
	}
	*data = ' ';
	data++;
	size++;

	// ID
	logID = pRcvBuff->msgQueue[readPos];
	if ( logID < LOGID_END ) {
		*data = DD_LOG_IDName[logID][0];
		data++;
		size++;
		*data = DD_LOG_IDName[logID][1];
		data++;
		size++;
		*data = DD_LOG_IDName[logID][2];
		data++;
		size++;
		*data = DD_LOG_IDName[logID][3];
		data++;
		size++;
	}
	readPos++;
	readPos &= (DD_LOG_QUEUE_SIZE - 1);

	// SR, Data
	if ( pRcvBuff->msgQueue[readPos] == '0' ) {	// 文字列
		*data = pRcvBuff->msgQueue[readPos];
		data++;
		size++;
		readPos++;
		readPos &= (DD_LOG_QUEUE_SIZE - 1);
		*data = ' ';
		data++;
		size++;
		dataSize = pRcvBuff->msgQueue[readPos];
		readPos++;
		readPos &= (DD_LOG_QUEUE_SIZE - 1);
		dataSize = (dataSize << 8) + pRcvBuff->msgQueue[readPos];
		readPos++;
		readPos &= (DD_LOG_QUEUE_SIZE - 1);

		for ( i = 0; i < dataSize; i++ ) {
			*data = pRcvBuff->msgQueue[readPos];
			data++;
			size++;
			readPos++;
			readPos &= (DD_LOG_QUEUE_SIZE - 1);
		}
	}
	else {
		if ( pRcvBuff->msgQueue[readPos] == 'M' ) {
			mix = 1;							// Mix Mode
		}
		else {
			mix = 0;							// Bin Mode
		}
		if ( (pRcvBuff->msgQueue[readPos] == 'S') || (pRcvBuff->msgQueue[readPos] == 'R') ) {
			*data = pRcvBuff->msgQueue[readPos];
			data++;
			size++;
		}
		readPos++;
		readPos &= (DD_LOG_QUEUE_SIZE - 1);
		*data = ' ';
		data++;
		size++;

		dataSize = pRcvBuff->msgQueue[readPos];
		readPos++;
		readPos &= (DD_LOG_QUEUE_SIZE - 1);
		dataSize = (dataSize << 8) + pRcvBuff->msgQueue[readPos];
		readPos++;
		readPos &= (DD_LOG_QUEUE_SIZE - 1);

		for ( i = 0; i < dataSize; i++ ) {
			if ( mix == 1 ) {					// 文字列とバイナリがMIXのケース
				if ( pRcvBuff->msgQueue[readPos] == 0 ) {	// 区切り(Null文字)のチェック
					mix = 0;					// 以降はバイナリとする
					readPos++;
					i++;
					*data = ' ';
					data++;
					size++;
					readPos &= (DD_LOG_QUEUE_SIZE - 1);
				}
			}
			if ( mix == 0 ) {					// バイナリデータを文字変換して表示
				*data = DD_LOG_HexToAsciiH(pRcvBuff->msgQueue[readPos]);
				data++;
				size++;
				*data = DD_LOG_HexToAsciiL(pRcvBuff->msgQueue[readPos]);
				data++;
				size++;
				*data = ' ';
				data++;
				size++;
			}
			else {								// 文字コードそのままの表示（MIXでバイナリ移行前）
				*data = pRcvBuff->msgQueue[readPos];
				data++;
				size++;
			}
			readPos++;
			readPos &= (DD_LOG_QUEUE_SIZE - 1);
		}
	}

	// Break(改行文字)
	*data = 0x0D;
	data++;
	size++;
	*data = 0x0A;
	data++;
	size++;

	pRcvBuff->readPtr = readPos;

	return(size);
}

//=============================================================================
// Function Name : stIdle
// Description   :
// Parameter     : -
// Return        : -
//=============================================================================
void stIdle(void)
{
	DD_LOG_RCV_BUFF *pRcvBuff;
	pRcvBuff = &DD_LOG_RcvBuff;
	gState = stIdle_ID;
	if ( pRcvBuff->readPtr != pRcvBuff->writePtr ) {	// 受信バッファに未送信データがあるか
		DD_EVT_Put(evSend_ID, evuDD_LOG_ID, 0, 0);
	} else if ( full ){
		full = 0;
		DD_LOG_Put(LOGID_LOG, '0', DD_LOG_RestartMsg, 4, LOG_CATEGORY_DEF);
	}
}

//=============================================================================
// Function Name : stSending
// Description   :
// Parameter     : -
// Return        : -
//=============================================================================
void stSending(void)
{
	gState = stSending_ID;
	DD_LOG_Send();
}

//==============================================================================
// Function Name : DD_LOG_HexToAsciiH
// Description   : HexコードをAsciiコードに変換
// Parameter     : dat	変換するHexコードのデータ
// Return        : ans	変換されたAsciiコードの上位ビット
//==============================================================================
UC DD_LOG_HexToAsciiH(UC dat)
{
	UC ans = (dat & 0xF0) >> 4;

	if ( ans <= 9 ) {
		ans = ans + 0x30;
	}
	else {
		ans = ans + 0x37;
	}

	return ans;
}

//==============================================================================
// Function Name : DD_LOG_HexToAsciiL
// Description   : HexをAsciiに変換
// Parameter     : dat	変換するHexコードのデータ
// Return        : ans	変換されたAsciiコードの下位ビット
//==============================================================================
UC DD_LOG_HexToAsciiL(UC dat)
{
	UC ans = (dat & 0x0F);

	if ( ans <= 9 ) {
		ans = ans + 0x30;
	}
	else {
		ans = ans + 0x37;
	}

	return ans;
}

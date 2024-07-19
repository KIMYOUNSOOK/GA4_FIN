// File Name: DD_COM_Commondef.h
// Description:
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 15.03.27
// Revision:  001
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+--------------------------------------------------------------------------------------
// 13.10.15 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev005 Base)
// Revision 000
// 15.03.27 | Hoshino Yuichi      | IOT⇔フィニッシャー間通信規定反映
// Revision 001

#ifndef DD_COM_COMMON_DEF_H 
#define DD_COM_COMMON_DEF_H
//#define DEBUG

#define DD_COM_CONNECTED				0
#define DD_COM_NOT_CONNECTED			0xFE
#define DD_COM_FAIL						0xFF
#define DD_COM_NULL 					0
#define DD_COM_TRUE 					1
#define DD_COM_FALSE 					0

// 1パケットの最大送信サイズ デフォルト
#define DD_COM_MAX_SEND_PACKET_SIZE		64

// マルチパケットを使用したパケットの最大受信サイズ　デフォルト
#define DD_COM_MAX_RECV_DATA_SIZE		256

// 1パケットの最大サイズ[×1Byte]
#define DD_COM_MAX_PACKET_SIZE			64
#define DD_COM_MAX_S_PACKET_SIZE		64

// マルチパケットで送信する最大送信サイズ[×1Byte]
#define DD_COM_MAX_SEND_DATA_SIZE		256

// 送受信パケットキュー数
#define DD_COM_NUMBER_S_COM_PACKET		( 5 ) 
#define DD_COM_NUMBER_S_SEND_COM_PACKET	( 17 )

// SYN送信開始するまでの時間(ms)
#define DD_COM_SYN_SEND_START_TIME		40

// SYN再送するまでの時間(ms)
#define DD_COM_SYN_RESEND_TIME			1200

// SYN再送回数
#define DD_COM_MAX_SYN_RESEND			0

// 再送時間(ms)
#define DD_COM_RESEND_TIME				41

// 再送時間(ms)
#define DD_COM_RESEND_CNT				2

// Byte間TimeOut時間(9600bps以下：20ms/19.2Kbps以上：10ms→6ms)
#define DD_COM_INTER_BYTE_TIME			6

// Polling間隔
#define DD_COM_POLLING_INTARVAL			60000

// 状態遷移の履歴数(REV.001)
#define DD_COM_STS_LOG_SIZE				64

#endif

// File Name: Common.h
// Description:
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 13.10.15
// Revision:  000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+--------------------------------------------------------------------------------------
// 13.10.15 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev009 Base)
// Revision 000

#ifndef COMMON_H
#define COMMON_H


// 要精査
#pragma SFR
#pragma DI
#pragma EI
#pragma NOP
#pragma HALT
#pragma STOP
#pragma inline

#include <string.h>

typedef  unsigned char	UC;
typedef  signed char	SC;
typedef  unsigned short	US;
typedef  signed short	SS;
typedef  unsigned long	UL;
typedef  signed long	SL;
typedef	 unsigned short	ER;
typedef	 unsigned short	ER_UINT;
typedef  void*			VP;
typedef  boolean		bool;

#define NG			(0)
#define OK			(1)

#define false		(0)					// RXコンパイラの予約語 true/falseと共通化を図るために小文字
#define true		(1)

#define RISE		(1)					// Low Active
#define FALL		(0)
#define EDGE_NONE	(2)

#define OFF			(0)
#define	ON			(1)

#define FORWARD		(0)
#define REVERSE		(1)

#define LOW			(0)
#define HIGH		(1)

#define DOWN		(0)
#define UP			(1)

#define CANCELED 	(0xFF)

#define E_OK		0					//!< 正常終了.
#define E_NG		(-1)				//!< エラー
#define E_SYS		(-5)				//!< システムエラー.
#define E_NOSPT 	(-9)				//!< 未サポート機能.
#define E_RSFN		(-10)				//!< 予約機能コード番号.
#define E_RSATR 	(-11)				//!< 予約属性.
#define E_PAR		(-17)				//!< パラメータエラー.
#define E_ID		(-18)				//!< 不正ID番号.
#define E_CTX		(-25)				//!< コンテキストエラー.
#define E_MACV		(-26)				//!< メモリアクセス不能、メモリアクセス権違反.
#define E_OACV		(-27)				//!< オブジェクトアクセス権違反.
#define E_ILUSE		(-28)				//!< サービスコール不正使用.
#define E_NOMEM 	(-33)				//!< メモリ不足.
#define E_NOID	 	(-34)				//!< ID番号不足.
#define E_OBJ		(-41)				//!< オブジェクトの状態が不正.
#define E_NOEXS 	(-42)				//!< オブジェクトが存在しない.
#define E_QOVR		(-43)				//!< キューイングまたはネストのオーバーフロー.
#define E_RLWAI 	(-49)				//!< 待ち状態強制解除.
#define E_TMOUT 	(-50)				//!< ポーリング失敗またはタイムアウト.
#define E_DLT		(-51)				//!< 待ちオブジェクトが削除された.
#define E_CLS		(-52)				//!< 待ちオブジェクトの状態が変化した.
#define E_WBLK		(-57)				//!< ノンブロッキング受付け.
#define E_BOVR		(-58)				//!< バッファオーバーフロー.

#define UC2US(ucAddr) (((US)*(ucAddr)) | ((US)*((ucAddr) + 1)) << 8)
#define GET_BIT_PATTERN(bitNumber) ((UL)0x00000001 << (bitNumber))

// Option種別の型
typedef enum{
	opBooklet_ID,
	opFolder_ID,
	opNoCare_ID,
	opNone_ID
}OptionID;

extern OptionID SubCPU_OptionID;

#define ANS_ACTIVE_CTRL_TYPE_SIZE 	(sizeof(ANS_ACTIVE_CTRL_TYPE) - 1)		// バウンダリの調整で-1

#ifdef DEBUG
#define  DEBUG_PORT_ON		(P4 |=  0x08)	// P43(J1 P02) Port On
#define	 DEBUG_PORT_OFF		(P4 &= ~0x08)	// P43(J1 P02) Port Off
#else
#define  DEBUG_PORT_ON
#define	 DEBUG_PORT_OFF
#endif

extern __far const UL Version;

#endif
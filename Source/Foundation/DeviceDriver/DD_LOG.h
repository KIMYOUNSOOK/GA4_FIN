// File Name: DD_LOG.h
// Description:
// Engineer:  Morikawa Atsushi
// Last Edit: 14.11.13
// Revision:  000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+--------------------------------------------------------------------------------------
// 14.11.13 | Morikawa Atsuhsi    | Created this file.
// 14.01.30 | Morikawa Atsushi    | Changed include file and prototype declarations.
// 15.02.12 | Morikawa Atsushi    | Added Log ID. (LOGID_FRAME)

#ifndef DD_LOG_H
#define DD_LOG_H

#include "DD_Event.h"

// Log ID
// 「Log ID」を変更する場合は"DD_LOG.c"の「ID Name」も変更し、
// 「Log ID」で定義した数値と、該当する「ID Name」の通し番号を一致させること。
#define	LOGID_FINISHER			0					// Common
#define	LOGID_IOT				1
#define	LOGID_FRAME				2
//#define	LOGID_BOOKLET_COM	3					// SUBCPU通信 Booklet
#define	LOGID_SYSMDL			4
#define	LOGID_SHTLOC			5					// Sheet Location
#define	LOGID_JAM				6
#define	LOGID_RTC				7
#define	LOGID_LOG				8
//#define	LOGID_MTRA			9
#define	LOGID_EJCT			10      //GlobalA4Fin用Log再定義(10～22番)
#define	LOGID_STPL			11
#define	LOGID_TMP			12
#define	LOGID_STK			13
#define	LOGID_TRNS			14

#define	LOGID_DEVICE		17
#define	LOGID_INIT_Mgr		19

#define	LOGID_CMPL_Mgr	20
#define	LOGID_SHT_Mgr		21
#define	LOGID_SYS_Mgr		22
#define	LOGID_END			23					// 使用するLog IDはLOGID_ENDより上に書くこと
//#define	LOGID_TCB			24
//#define	LOGID_TCBCOM		25
//#define	LOGID_IPCOM			26
//#define	LOGID_PNCHCOM		27
//#define	LOGID_TSQFCOM		28
//#define	LOGID_STKTMPCOM		29
//#define	LOGID_VTRACOM		30
#define	LOGID_MAX				31					// このIDまでは受け入れる 最大設定は31(32個)まで

#define	DD_LOG_MAX_DATA_SIZE	70

// LOG Category
#define	LOG_CATEGORY_DEF		0					// Category (0)	常時表示
#define	LOG_CATEGORY_TOOL		1					// Category (1)	Tool用
#define	LOG_CATEGORY_DEBUG		2					// Category (2)	Debug用

// アプリ提供API
void DD_LOG_Init(void);
void DD_LOG_Put(UC id, UC sr, UC *data, US logSize, UC category);
void DD_LOG_Notify(DD_EventID eventID, UC* pMsg);

#endif	// DD_LOG_H

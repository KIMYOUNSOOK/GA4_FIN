// File Name: DD_NVM.h
// Description:
// Engineer:  Tanabe
// Last Edit: 15.01.xx
// Revision:  000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+--------------------------------------------------------------------------------------
// 15.01.xx | Tanabe              | 新規作成

#ifndef DD_NVM_H
#define DD_NVM_H

#include "DD_Event.h"

/***********************************************************************************************************************
 Macro definitions
***********************************************************************************************************************/
/* NVM Area ID */
typedef enum /*EnvmAreaID*/ {
	DD_NVM_AREA_KEY,
	DD_NVM_AREA_NVM,
	DD_NVM_AREA_HFSI,
	DD_NVM_AREA_TEMP,
	DD_NVM_AREA_ID_NUM	// Number of NVM Area ID
}EnvmAreaID;

/* NVM Area Size */
#define	DD_SIZE_KEYAREA			32			// Key Area Size(32Byte)
#define	DD_SIZE_NVMAREA_		2016		// NVM Area Size(2048-32Byte)
#define	DD_SIZE_HFSIAREA		512			// HFSI Area Size(512Byte)
#define	DD_SIZE_TEMPAREA		512			// Temporaray Area Size(512Byte)

// NVM Area Start Adress */
#define	DD_ADDR_KEYAREA			0x0000		// Key Area Start Address
#define	DD_ADDR_NVMAREA			0x0020		// NVM Area Start Address
#define	DD_ADDR_HFSIAREA		0x0C00		// HFSI Area Start Address
#define	DD_ADDR_TEMPAREA		0x1000		// Parameter Area Start Address

/* Result Code */
#define DD_NVM_OK				0			// NVM OK
#define DD_NVM_BUS_LEVEL_ERR	(-1)		// Bus Level Err
#define DD_NVM_BUSY				(-2)		// NVM Busy
#define DD_NVM_START_ERR		(-3)		// Start Condition Err
#define DD_NVM_NON_ACK_ERR		(-4)		// Non Ack
#define DD_NVM_WRITE_ERR		(-5)		// Write Err

/* Write MAX Count */
#define	DD_WRITE_MAX_CNT		1000000		// Write Max Count

/* Key Area difine */
#define	DD_KEY_CODE				0x55AA		// FirstPowerOn実施済み
#define	DD_KEY_CONFIG_SIZE		10

typedef union {
	struct {
		US 	KEYCODE;						// KeyCode(0x55AA以外ならFirstPowerOn未実施)
		US 	DLMODE;							// DownLoadMode(現状未使用)
		UC	MAJOR_V;						// Software Major Version
		UC	MINOR_V;						// Software Minor Version
		UC	PATCH_V;						// Software Pathc Version
		UC	SEQ_BIT;						// PAR Sequence Bit(現状未使用)
		UL 	WRTCNT;							// Write Counter
		US 	CONFIG[DD_KEY_CONFIG_SIZE];		// 未使用(20Byte)
	} SIZE;
	US WORD[DD_SIZE_KEYAREA/2];				// 32Byte
}DD_KEY_TYPE;

/***********************************************************************************************************************
 Global functions
***********************************************************************************************************************/
void DD_NVM_Init(void);
void DD_NVM_Load(US area, UC *address, US size, void (*func)(SS result));
void DD_NVM_Save(US area, UC *address, US size, void (*func)(SS result));
void DD_NVM_Notify(DD_EventID eventId, UC* pMsg);


#endif
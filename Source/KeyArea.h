// File Name:	KeyArea.h
// Description:	Header File of Key Area
// Engineer:	Yuichi Hoshino
// Last Edit:	16.02.22
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef KeyArea_h
#define KeyArea_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Const Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Key Area Size ---
#define KEY_AREA_SIZE					32					// Key Area Size[�~1Byte]
#define KEY_AREA_USE_SIZE				13					// Use Area Size[�~1Byte]

// --- Key Area Start Address ---
#define KEY_AREA_START_ADDRESS			0x0000

// --- Key Code ---
#define KEY_CODE						0x55AA				// FirstPowerOn���{�ς�

// --- Download Mode ---
typedef enum /*EDownloadMode*/ {
	DOWNLOAD_MODE_COMPLETE = 0,
	DOWNLOAD_MODE_BUSY	   = 1,
	DOWNLOAD_MODE_CHANGE   = 2
} EDownloadMode;

// --- Sequence Bit Save Position ---
#define SEQUENCE_BIT_SAVE_POS_RX		0					// [�~1�r�b�g��]
#define SEQUENCE_BIT_SAVE_POS_TX		1					// [�~1�r�b�g��]

#define SEQUENCE_BIT_SAVE_POS_PTN(aPosition)	((UC)0x01 << aPosition)

// --- NVM Max Write Count ---
#define NVM_WRITE_COUNT_MAX				1000000				// [�~1��]


/////////////////////////////////////////////////////////////////////////////////////////
// Type Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Key Area Type ---
typedef union {
	struct {
		US 	KEYCODE;										// KeyCode(0x55AA�ȊO�Ȃ�FirstPowerOn�����{)
		US 	DLMODE;											// DownLoadMode
		UC	MAJOR_V;										// Software Major Version
		UC	MINOR_V;										// Software Minor Version
		UC	PATCH_V;										// Software Pathc Version
		UC	SEQ_BIT;										// PAR Sequence Bit
		UL	WRTCNT;											// Write Counter
		UC	IPL_V;											// IPL Version
		// �� �g�p�̈�
		UC 	CONFIG[KEY_AREA_SIZE - KEY_AREA_USE_SIZE];		// ���g�p
	} SIZE;
	US WORD[KEY_AREA_SIZE/2];
} KEY_AREA_TYPE;


#endif // #ifndef KeyArea_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 16.02.22	| Yuichi Hoshino	| Created this file
//

// File Name:	StopSheetSpec.c
// Description:	Specification of stop sheet
// Engineer:	Taro Fuji
// Last Edit:	2015/03/17
// Revision:	XX
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#include "CShtManager.h"

// --- Stop Sheet Specification of Device Status ---
ShtStopSheetSpec const cSht_StopSheetSpec[DSNO_NUM] = {
	{ eCdiFinOutDest_All,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Impossible	},	// DSNO_DOWNLOAD_MODE_FAIL
	{ eCdiFinOutDest_All,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Impossible	},	// DSNO_STATIC_JAM
	{ eCdiFinOutDest_All,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Impossible	},	// DSNO_CMPL_EXIT_SNR_ON_JAM
	{ eCdiFinOutDest_All,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Impossible 	},	// DSNO_CMPL_EXIT_SNR_OFF_JAM
	{ eCdiFinOutDest_All,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Possible	},	// DSNO_SET_EJECT_JAM
	{ eCdiFinOutDest_All,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Impossible	},	// DSNO_NVM_FAIL
	{ eCdiFinOutDest_OutTray,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Possible	},	// DSNO_RIGHT_TAMPER_ON_FAIL
	{ eCdiFinOutDest_OutTray,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Possible	},	// DSNO_RIGHT_TAMPER_OFF_FAIL
	{ eCdiFinOutDest_OutTray,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Possible	},	// DSNO_LEFT_TAMPER_ON_FAIL
	{ eCdiFinOutDest_OutTray,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Possible	},	// DSNO_LEFT_TAMPER_OFF_FAIL
	{ eCdiFinOutDest_OutTray,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Possible	},	// DSNO_STAPLE_FAIL
	{ eCdiFinOutDest_OutTray,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Possible	},	// DSNO_EJECT_BELT_HOME_ON_FAIL
	{ eCdiFinOutDest_OutTray,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Possible	},	// DSNO_EJECT_BELT_HOME_OFF_FAIL
	{ eCdiFinOutDest_OutTray,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Possible	},	// DSNO_SETCLAMP_HOME_ON_FAIL
	{ eCdiFinOutDest_OutTray,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Possible	},	// DSNO_SETCLAMP_HOME_OFF_FAIL
	{ eCdiFinOutDest_OutTray,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Possible	},	// DSNO_PADDLE_HOME_ON_FAIL
	{ eCdiFinOutDest_OutTray,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Possible	},	// DSNO_PADDLE_HOME_OFF_FAIL
	{ eCdiFinOutDest_OutTray,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Possible	},	// DSNO_STACKER_TRAY_FAIL
	{ eCdiFinOutDest_OutTray,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Possible	},	// DSNO_STACKER_UPLIMIT_FAIL
	{ eCdiFinOutDest_OutTray,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Possible	},	// DSNO_STACKER_LOWLIMIT_FAIL
	{ eCdiFinOutDest_All,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Impossible	},	// DSNO_STAPLE_COVER_24V_FAIL
	{ eCdiFinOutDest_All,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Impossible	},	// DSNO_REAR_COVER_ILK_OPEN
	{ eCdiFinOutDest_All,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Impossible	},	// DSNO_STAPLE_COVER_ILK_OPEN
	{ eCdiFinOutDest_All,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Impossible	},	// DSNO_INIT_STATE
	{ eCdiFinOutDest_All,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Impossible	},	// DSNO_STAPLE_READY_FAIL
	{ eCdiFinOutDest_All,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Impossible	},	// DSNO_STAPLE_SUPPLY_LEVEL
	{ eCdiFinOutDest_All,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Impossible	},	// DSNO_STACKER_FULL_STACK
	{ eCdiFinOutDest_All,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Impossible	},	// DSNO_STACKER_LOW_SAFETY
	{ eCdiFinOutDest_OutTray,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Possible	},	// DSNO_STAPLE_STATUS_NG
	{ eCdiFinOutDest_All,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Impossible	},	// DSNO_SCARCH_SHEET_COMPILE
	{ eCdiFinOutDest_All,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Impossible	},	// DSNO_SYS_CONT_COMM_FAIL
	{ eCdiFinOutDest_All,	SHT_PATH_SNR_ID_NUM,	eCdiFinPurgePsbl_Impossible	},	// DSNO_LOGIC_FAIL	
};

// File Name:	CoverCheck.c
// Description:	Implimentation File of Cover Check
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CCvrChecker.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Cover Spec ---
CvrCoverSpec const cCvr_CoverSpec[CVR_ID_NUM] = {
//	| Input ID			| Interlock	| Detect Count
	// !!! Product Specific !!!
	{ DD_DI_REAR_COVER_OPEN_SNR , true		, 5 			},	// CVR_ID_FIN_REAR_COVER
	{ DD_DI_STAPLE_DOOR_INTERLOCK_SW , true		, 5 			},	// CVR_ID_FIN_STAPLE_ILK	
};

// --- Device Status No. of Cover ---
US const cCvr_DeviceStatusNo[CVR_ID_NUM] = {
	// !!! Product Specific !!!
	DSNO_REAR_COVER_ILK_OPEN,								// CVR_ID_FIN_REAR_COVER
	DSNO_STAPLE_COVER_ILK_OPEN							// CVR_ID_FIN_STAPLE_ILK  IOTとの結合デバッグ TB.No6 対策	
};

// --- Option ID that cover is in --- 
UC const cCvr_OptionID[OPTID_NUM] = {
	// !!! Product Specific !!!
	OPTID_BASE									// CVR_ID_FIN_COVER
};


/////////////////////////////////////////////////////////////////////////////////////////
// Cover Sensor Event Call Back Function
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CVR_CoverInterlockSwitchON
// Description   : イベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CVR_RearCoverSwitchON(const UC* notUsed, UC from)
{
	CCvrChecker_NotifyEdge(CVR_ID_FIN_REAR_COVER, ON);
}

///////////////////////////////////////////////////////////////////
// Function Name : CVR_CoverInterlockSwitchOFF
// Description   : イベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CVR_RearCoverSwitchOFF(const UC* notUsed, UC from)
{
	CCvrChecker_NotifyEdge(CVR_ID_FIN_REAR_COVER, OFF);
}

///////////////////////////////////////////////////////////////////
// Function Name : CVR_StapleDoorILKSwitchON
// Description   : イベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CVR_StapleDoorILKSwitchON(const UC* notUsed, UC from)
{
	CCvrChecker_NotifyEdge(CVR_ID_FIN_STAPLE_ILK, ON);
}

///////////////////////////////////////////////////////////////////
// Function Name : CVR_StapleDoorILKSwitchOFF
// Description   : イベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CVR_StapleDoorILKSwitchOFF(const UC* notUsed, UC from)
{
	CCvrChecker_NotifyEdge(CVR_ID_FIN_STAPLE_ILK, OFF);
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.04.11	| Yuichi Hoshino	| Created this file
// 14.12.04	| Yuichi Hoshino	| Release V0 (Revision 00)
// 15.03.30	| Yuichi Hoshino	| Modify by Trouble Action [ cCvr_OptionID[]の配列サイズ指定間違い ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//

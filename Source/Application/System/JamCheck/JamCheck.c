// File Name:	JamCheck.c
// Description:	Implimentation File of Jam Check
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CJamChecker.h"

#include "CShtManager.h"
#include "CCfgInformer.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Jam Timer Value Get Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

UL JAM_GetEntranceSensorOnJamTimer(UC sheetNo);
UL JAM_GetCompileExitSensorOnJamTimer(UC sheetNo);
UL JAM_GetCompileExitSensorOffJamTimer(UC sheetNo);
UL JAM_GetSetEjectJamTimer(UC sheetNo);

/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////
// --- Distance. Table of Dynamic Jam ---
US const cJam_DJamDevDistance[JAM_DJAM_DISTANSEID_NUM] = {
	// !!! Product Specific !!!
	1157, 		// IOT筺体の用紙搬出部フレームとIOT Fusing-Roll間パス長         (R)
	807, 		//  IOT筺体の用紙搬出部フレームとIOT Exit Sensor(Hot Line)間パス長 (C)
	243, 		// IOT筺体の用紙搬出部フレームからENTRANCE-ROLLまでの距離  (E)
	1300,		// ENTRANCE-ROLLからCompile-Exit Sensorまでの距離	(K)
};


// --- Device Status No. Table of Dynamic Jam ---
US const cJam_DJamDevSttsNo[JAM_DJAM_ID_NUM] = {
	// !!! Product Specific !!!
	DSNO_CMPL_EXIT_SNR_ON_JAM,			// JAM_DJAM_ID_CMPL_EXIT_SNR_ON_JAM
	DSNO_CMPL_EXIT_SNR_OFF_JAM,			// JAM_DJAM_ID_CMPL_EXIT_SNR_OFF_JAM
	DSNO_SET_EJECT_JAM,					// JAM_DJAM_ID_SET_EJECT_JAM
};

// --- Jam Zone Check Config Table ---
UC const cJam_ZoneOptionID[JAM_ZONE_ID_NUM] = {
	// !!! Product Specific !!!
	OPTID_BASE,							// JAM_ZONE_ID_ENT_PATH
	OPTID_BASE,							// JAM_ZONE_ID_TRANS_PATH
	OPTID_BASE							// JAM_ZONE_ID_COMPILE_TRAY
};

// --- Input ID table of path sensor ---
UC const cJam_PathSensorInputID[SHT_PATH_SNR_ID_NUM] = {
	// !!! Product Specific !!!
	DD_DI_IOT_EXIT_SNR,					// SHT_PATH_SNR_ID_IOT_EXIT
	DD_DI_COMPILE_EXIT_SNR, 			// SHT_PATH_SNR_ID_COMPILE_EXIT
};

// --- Spec data table of set Jam Zone from path sensor condition ---
JamZoneSetSpecPathSensor const cJam_ZoneSetSpecSensor[] = {
//	| Jam Zone ID				| Path Sensor ID(Bit)									| Compile Check Path ID(Bit)	| Compile Check Output Dest ID
	// !!! Product Specific !!!
//	{ JAM_ZONE_ID_ENT_PATH		, JAM_PATH_SNR_ID_CNV_BIT(SHT_PATH_SNR_ID_IOT_EXIT)		, 0						, eCdiFinOutDest_All			},
	{ JAM_ZONE_ID_TRANS_PATH, JAM_PATH_SNR_ID_CNV_BIT(SHT_PATH_SNR_ID_COMPILE_EXIT)	, 0			, eCdiFinOutDest_All		},
	{ JAM_ZONE_ID_NUM		, 0												, 0						, 0						}	// End of table
};

// --- Spec data table of set Jam Zone from sheet location ---
JamZoneSetSpecSheetLocation const cJam_ZoneSetSpecLocation[] = {
//	| Jam Zone ID				| Location ID
	// !!! Product Specific !!!
	{ JAM_ZONE_ID_ENT_PATH		, SHT_LOC_ID_COMPILE_EXIT_SNR	},
//	{ JAM_ZONE_ID_TRANS_PATH		, SHT_LOC_ID_COMPILE_EXIT_SNR	},	
	{ JAM_ZONE_ID_NUM			, 0							}	// End of table
};

// --- Spec data table of clear Jam Zone from cover close ---
JamZoneClearSpecCover const cJam_ZoneClearSpecCover[] = {
//	| Jam Zone ID				| Path Sensor ID(Bit)									| Compile Check Path ID(Bit)						| Compile Check Output Dest ID	| Cover ID(Bit)
	// !!! Product Specific !!!
	{ JAM_ZONE_ID_ENT_PATH		, JAM_PATH_SNR_ID_CNV_BIT(SHT_PATH_SNR_ID_IOT_EXIT)		, 0													, eCdiFinOutDest_All			, JAM_CVR_ID_CNV_BIT(CVR_ID_FIN_REAR_COVER)		},
	{ JAM_ZONE_ID_TRANS_PATH	, JAM_PATH_SNR_ID_CNV_BIT(SHT_PATH_SNR_ID_COMPILE_EXIT)	, 0													, eCdiFinOutDest_All			, JAM_CVR_ID_CNV_BIT(CVR_ID_FIN_REAR_COVER)		},
	{ JAM_ZONE_ID_COMPILE_TRAY	, 0														, 0													, eCdiFinOutDest_All			, JAM_CVR_ID_CNV_BIT(CVR_ID_FIN_REAR_COVER)		},
	{ JAM_ZONE_ID_NUM			, 0														, 0													, 0								, 0											}	// End of table
};

// --- Spec data table of clear Jam Zone from entrance path enviroment change ---
JamZoneClearSpecEntrance const cJam_ZoneClearSpecEntrance[] = {
//	| Jam Zone ID				| Path Sensor ID(Bit)
	// !!! Product Specific !!!
	// { JAM_ZONE_ID_ENT_PATH		, JAM_PATH_SNR_ID_CNV_BIT(SHT_PATH_SNR_ID_ENTRANCE)		},
	{ JAM_ZONE_ID_NUM			, 0														}	// End of Table
};

// --- Spec data table of clear Jam Zone from cyclic check ---
JamZoneClearSpecCyclic const cJam_ZoneClearSpecCyclic[] = {
//	| Jam Zone ID				| Path Sensor ID(Bit)									| Compile Check Path ID(Bit)						| Compile Check Output Dest ID	| Clear Count Threshold
	// !!! Product Specific !!!
	// { JAM_ZONE_ID_ENT_PATH		, JAM_PATH_SNR_ID_CNV_BIT(SHT_PATH_SNR_ID_ENTRANCE)		, 0													, eCdiFinOutDest_All			, 6						},
	{ JAM_ZONE_ID_NUM			, 0														, 0													, 0								, 0						}	// End of table
};

// --- Dynamic Jam select spec of Sheet Location (IOT Exit Sensor) ---
DJamSelectSpec const cJam_DJamSelectSpecIotExitSnr[] ={
//	| Dynamic Jam ID					| Next Location ID				| Plan Location ID
	{ JAM_DJAM_ID_CMPL_EXIT_SNR_ON_JAM		, SHT_LOC_ID_COMPILE_EXIT_SNR		, SHT_LOC_ID_NUM				},
	{ JAM_DJAM_ID_NUM					, 0								, 0								}	// End of Table
};

// --- Dynamic Jam select spec of Sheet Location (Compile Exit Sensor) ---
DJamSelectSpec const cJam_DJamSelectSpecCompileExitSnr[] ={
//	| Dynamic Jam ID					| Next Location ID				| Plan Location ID
	{ JAM_DJAM_ID_CMPL_EXIT_SNR_OFF_JAM	, SHT_LOC_ID_NUM				, SHT_LOC_ID_NUM				},
	{ JAM_DJAM_ID_NUM					, 0								, 0								}	// End of Table
};

// --- Dynamic Jam select data table from Sheet Location ---
DJamSelectSpec const *cpJam_DynamicJamSelectTableLocation[SHT_LOC_ID_NUM] = {
	// !!! Product Specific !!!
	cJam_DJamSelectSpecIotExitSnr,		// SHT_LOC_ID_IOT_EXIT_SNR
	cJam_DJamSelectSpecCompileExitSnr,	// SHT_LOC_ID_COMPILE_EXIT_SNR
};

// --- Dynamic Jam select data table from Sheet Event ----
DJamSelectSpec const *cpJam_DynamicJamSelectTableEvent[SHT_EVT_ID_NUM] = {
	// !!! Product Specific !!!
	NULL,								// SHT_EVT_ID_EJECT_FINISH_START
	NULL,								// SHT_EVT_ID_EJECT_FINISH_COMP
	NULL,								// SHT_EVT_ID_EJECT_START
	NULL								// SHT_EVT_ID_COMPILE_FINISH_COMP
};

// --- Dynamic Jam detect spec data table ---
DJamDetectSpec const cJam_DJamDetectSpec[JAM_DJAM_ID_NUM] = {
//	| Sheet Location ID				| Sheet Edge			| Add for Punch Time	| Set Jam Zone(Bit)
	// !!! Product Specific !!!
	{ SHT_LOC_ID_COMPILE_EXIT_SNR	, SHT_EDGE_LEAD			, 500					, JAM_ZONE_ID_CNV_BIT(JAM_ZONE_ID_ENT_PATH)	},	// JAM_DJAM_ID_CMPL_EXIT_SNR_ON_JAM V3.4.0 ZamZoneNGTB
	{ SHT_LOC_ID_COMPILE_EXIT_SNR	, SHT_EDGE_TAIL			, 500					, JAM_ZONE_ID_CNV_BIT(JAM_ZONE_ID_TRANS_PATH)	},	// JAM_DJAM_ID_CMPL_EXIT_SNR_OFF_JAM
	{ SHT_LOC_ID_NUM		, SHT_EDGE_NUM			, 0					,JAM_ZONE_ID_CNV_BIT(JAM_ZONE_ID_COMPILE_TRAY)	}	// JAM_DJAM_ID_SET_EJECT_JAM
};

// --- Dynamic Jam detect spec data table ---
UL const (*cpJam_DynamicJamTimerGetTable[JAM_DJAM_ID_NUM])(UC sheetNo) = {
	JAM_GetCompileExitSensorOnJamTimer,	// JAM_DJAM_ID_CMPL_EXIT_SNR_ON_JAM
	JAM_GetCompileExitSensorOffJamTimer,// JAM_DJAM_ID_CMPL_EXIT_SNR_OFF_JAM
	NULL  //JAM_GetSetEjectJamTimer,								// JAM_DJAM_ID_SET_EJECT_JAM
};


/////////////////////////////////////////////////////////////////////////////////////////
// Jam Management Event Call Back Function
/////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
// Function Name : JAM_SheetEventNotifyEventOutTrayJamOccurrence
// Description   : Sheet Event Notifyイベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void JAM_SheetEventNotifyEventOutTrayJamOccurrence(const UC* evt, UC from)
{
	EvtParamSheetEventJamOccurrenceNotify *pMessage;

	pMessage = (EvtParamSheetEventJamOccurrenceNotify *)evt;

	CJamChecker_JamOccurrence(pMessage->statusNo, pMessage->sheetNo);
}

/////////////////////////////////////////////////////////////////////////////////////////
// Jam Timer Value Get Function
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : JAM_GetEntranceSensorOnJamTimer
// Description   :
// Parameter     :
// Return        :
// 監視時間[ms] ＝ (K+ E+C) ÷ I × 1000 × 1.3
///////////////////////////////////////////////////////////////////
UL JAM_GetCompileExitSensorOnJamTimer(UC sheetNo)
{
	const CShtSheet *pSheet;
	const CShtSheetAttribute *pShtAttr;
	US timer, param;
	UL  tempBuf;

	pSheet = CShtManager_GetSheet(sheetNo);
	pShtAttr = CShtSheet_GetAttribute(pSheet);

	param = cJam_DJamDevDistance[JAM_DJAM_DISTANSEID_K]+cJam_DJamDevDistance[JAM_DJAM_DISTANSEID_E] +cJam_DJamDevDistance[JAM_DJAM_DISTANSEID_C];

	//切り上げ計算 :  (((K+E+C)*1000*13/ I )+9)/10
	tempBuf = (UL)(((UL)(param) * 1000L * 13L ) /CShtSheetAttribute_GetInitialSpeed(pShtAttr));
	timer = (US)((tempBuf + 9L) / 10L);
	
	return timer;
}

///////////////////////////////////////////////////////////////////
// Function Name : JAM_GetCompileExitSensorOffJamTimer
// Description   : 
// Parameter     : 
// Return        : 
// L＜(R+E+K)の場合　＝ L ÷ F × 1000
// L≧(R+E+K)の場合＝(R+E+K) ÷ F × 1000＋ ( L－(R+E+K) ※1) ÷ I × 1000
// L ： 用紙の長さ[mm])
// F ： Finisherの用紙搬入速度[mm/s]
///////////////////////////////////////////////////////////////////
UL JAM_GetCompileExitSensorOffJamTimer(UC sheetNo)
{
	const CShtSheet *pSheet;
	const CShtSheetAttribute *pShtAttr;
	US timer,timerTmp, paramPS, paramSum;
	UL  tempBuf, tempBuf2;

	pSheet = CShtManager_GetSheet(sheetNo);
	pShtAttr = CShtSheet_GetAttribute(pSheet);
	paramSum = cJam_DJamDevDistance[JAM_DJAM_DISTANSEID_R] + cJam_DJamDevDistance[JAM_DJAM_DISTANSEID_E] +cJam_DJamDevDistance[JAM_DJAM_DISTANSEID_K];
	paramPS = CShtSheetAttribute_GetProcessSize(pShtAttr);
	
	if ( paramPS < paramSum ) {// L＜(R+E+K)の場合
	
	//  切り上げ計算 :  ((L*1000*13)/F+9)/10
		tempBuf = (UL)(((UL)paramPS * 1000L * 13L) / SRVC_GetFinisherSpeed(sheetNo));
		timer = (US)((tempBuf + 9L)/10L);
	
	} else {

	//	切り上げ計算 : ( (((R+E+K)*1000*13/F) + ((L-(R+E+K))*1000*13/I)) +9)/10
		tempBuf = (UL)(((UL)paramSum * 1000L * 13L) / SRVC_GetFinisherSpeed(sheetNo));
		tempBuf2 = (UL)(((UL)(paramPS - paramSum) * 1000L * 13L ) / CShtSheetAttribute_GetInitialSpeed(pShtAttr));
		timer = (US)( (tempBuf + tempBuf2 + 9L ) / 10L );
	}

	if( CShtSheetAttribute_GetPunchMode(pShtAttr) != eCdiFinPunch_Off ) {
		timer += 500;
	}
	else { /* No Action */ }

	return timer;
}

///////////////////////////////////////////////////////////////////
// Function Name : JAM_GetSetEjectJamTimer
// Description   : 
// Parameter     : 
// Return        : 
// 監視時間[ms] ＝ EJECT HOME SENSOR ON　FAIL時間
// 
///////////////////////////////////////////////////////////////////
UL JAM_GetSetEjectJamTimer(UC sheetNo)
{
//Eject側で処理している ので削除。KimYS
/*	const CShtSheet *pSheet;
	const CShtSheetAttribute *pShtAttr;
	US timer;

	pSheet = CShtManager_GetSheet(sheetNo);
	pShtAttr = CShtSheet_GetAttribute(pSheet);

	timer = (US)(((UL)(200 + 1400 + 1000) * 100 * 13) / CShtSheetAttribute_GetInitialSpeed(pShtAttr));

	if( CShtSheetAttribute_GetPunchMode(pShtAttr) != eCdiFinPunch_Off ) {
		timer += 500;
	}
	else { }

	return timer;*/
	return 0;
}

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.04.14	| Yuichi Hoshino	| Created this file
// 14.12.04	| Yuichi Hoshino	| Release V0 (Revision 00)
// 15.03.30	| Yuichi Hoshino	| Design Improvement [ Sheet Event定義見直し ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//


// File Name:	SheetManagement.c
// Description:	Implimentation File of Sheet Management
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CShtManager.h"
#include "CShtPathSensor.h"
#include "CShtInitializeManager.h"
#include "CCdiAdapter.h"


////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Path Sensor Spec ---
ShtPathSensorSpec const cSht_PathSensorSpec[SHT_PATH_SNR_ID_NUM] = {
//	| Input ID						| Filter Time(Lead)	| Filter Time(Tail)	| Reverse Filter(Bit)	| Late Detect Wait Time(Lead)	| Late Detect Wait Time(Tail)	| Late Detect Check Times(Lead)	| Late Detect Check Times(Tail)	| Buffer Path	| Compile Path	| Pile Trans Path
	// !!! Product Specific !!!
	{ DD_DI_IOT_EXIT_SNR			, 50				, 50				, 0x00			, 0						, 0						, 0						, 0						,false		, false		, false		},	// SHT_PATH_SNR_ID_IOT_EXIT
	{ DD_DI_COMPILE_EXIT_SNR	, 50				, 50				, 0x00			, 0 						, 0						, 0 						, 0 						,false		, false 		, false 		},	// SHT_PATH_SNR_ID_COMPILE_EXIT
};

// --- Output Destination Information ---
ShtOutDestInfo const cSht_OutDestInfoTable[] ={
//	| Output Destination ID		| Device ID			| Delivered Point(Sheet Location ID)	| Set Finished Point(Sheet Location ID)	| Set Finished Point(Sheet Event ID)
	//V3.4.0 Merge (setFinishedLocation, setFinishedEvent 削除によるTable変更)
//	{ eCdiFinOutDest_OutTray	, DEVID_OUT_TRAY	, SHT_LOC_ID_COMPILE_EXIT_SNR			, SHT_LOC_ID_NUM						, SHT_EVT_ID_EJECT_COMP				},		// Output Tray
//	{ eCdiFinOutDest_All		, DEVID_NUM			, SHT_LOC_ID_NUM						, SHT_LOC_ID_NUM						, SHT_EVT_ID_NUM					}		// End of table
	{ eCdiFinOutDest_OutTray	, DEVID_OUT_TRAY	, SHT_LOC_ID_COMPILE_EXIT_SNR },		// Output Tray
	{ eCdiFinOutDest_All		, DEVID_NUM 			, SHT_LOC_ID_NUM			  }		// End of table

};

// --- Function Pointer of Get Eject Execution Sheet Request ---
EShtEject const (*cSht_GetEjectExecutionSheetReqFuncTable[])(UC sheetNo) = {
	// !!! Product Specific !!!
	CMPL_GetEjectExecutionSheetReq,
	NULL																// End of table
};

// --- Function Pointer of Get Eject Execution Next Sheet Request ---
EShtEject const (*cSht_GetEjectExecutionNextSheetReqFuncTable[])(UC sheetNo, UC nextShtNo) = {
	// !!! Product Specific !!!
	CMPL_GetEjectExecutionNextSheetReq,
	NULL																// End of table
};

// --- Function Pointer of Get Eject Execution Standby ---
EShtEject const (*cSht_GetEjectExecutionStandbyFuncTable[])(UC sheetNo) = {
	// !!! Product Specific !!!
	CMPL_GetEjectExecutionStandby,
	NULL																// End of table
};

// --- Function Pointer of Get Weighted Count ---
US const (*cSht_GetWeightedCompileCountFuncTable[])(UC sheetNo) = {
	// !!! Product Specific !!!
	CMPL_GetWeightedCompileCount,
	NULL																// End of table
};

// --- Function Pointer of Get Offset Finish Position ---
EShtOffsetFinish const (*cSht_GetOffsetFinishPositionFuncTable[])(UC sheetNo) = {
	// !!! Product Specific !!!
	CMPL_GetOffsetFinishPosition,
	NULL																// End of table
};

// --- Table of Path Sensor ID to Sheet Location ---
UC const cSht_PathSensorIDLocation[SHT_LOC_ID_NUM] = {
	// !!! Product Specific !!!
	SHT_PATH_SNR_ID_IOT_EXIT,											// SHT_LOC_ID_IOT_EXIT_SNR
	SHT_PATH_SNR_ID_COMPILE_EXIT,										// SHT_LOC_ID_COMPILE_EXIT_SNR
};

// --- Table of Finisher Speed[×0.1mm/s] ---
US const cSht_FinisherSpeed[SHT_FIN_SPD_ID_NUM] = {
	// !!! Product Specific !!!
	4100,																// SHT_FIN_SPD_ID_NORMAL
	6000,																// SHT_FIN_SPD_ID_NORMAL
	6000,																// SHT_FIN_SPD_ID_NORMAL
	8000,																// SHT_FIN_SPD_ID_NORMAL
};

// --- Table of Location Trigger ID ---
US const cSht_LocationEventID[SHT_LOC_ID_NUM][SHT_EDGE_NUM] = {
//	  Lead								| Tail
	// !!! Product Specific !!!
	{ LOC_IOT_EXIT_SENSOR_LEAD			, LOC_IOT_EXIT_SENSOR_TAIL			},	// SHT_LOC_ID_IOT_EXIT_SNR
	{ LOC_COMPILE_EXIT_SENSOR_LEAD		, LOC_COMPILE_EXIT_SENSOR_TAIL		},	// SHT_LOC_ID_COMPILE_EXIT_SNR
};

// --- Sheet Manager Spec ---
ShtManagerSpec const cSht_ManagerSpec = {
	// !!! Product Specific !!!
	eCdiFinOutDest_OutTray
};

// --- Sheet Initialize Spec ---
ShtInitializeManagerSpec const cSht_InitializeManagerSpec = {
	// !!! Product Specific !!!
	{
		{								/// Compiler Sheet Initialize
			OPTID_BASE,
			true,
			{
				{						// SHT_EJECT_NOT
					SHT_LOC_ID_NUM,
					SHT_EDGE_NUM,
					SHT_EVT_ID_COMPILE_FINISH_COMP,
					0
				},
				{						// SHT_EJECT_EXEC
					SHT_LOC_ID_NUM,
					SHT_EDGE_NUM,
					SHT_EVT_ID_EJECT_FINISH_COMP,
					0
				}
			}
		},
	}
};


/////////////////////////////////////////////////////////////////////////////////////////
// Path Sensor Edge Event Call Back Function Body
/////////////////////////////////////////////////////////////////////////////////////////
void SHT_IotExitSensorONDelay(SS aTimerID, UL aEvtType)
{
	CShtManager_PathSensorEdgeChange( SHT_PATH_SNR_ID_IOT_EXIT, SHT_EDGE_LEAD );
}


///////////////////////////////////////////////////////////////////
// Function Name : SHT_IotExitSensorON
// Description   : イベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SHT_IotExitSensorON(const UC* notUsed, UC from)
{
	CShtManager_PathSensorEdgeChange(SHT_PATH_SNR_ID_IOT_EXIT, SHT_EDGE_LEAD);	
}

///////////////////////////////////////////////////////////////////
// Function Name : SHT_IotExitSensorOFF
// Description   : イベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SHT_IotExitSensorOFF(const UC* notUsed, UC from)
{
	CShtManager_PathSensorEdgeChange(SHT_PATH_SNR_ID_IOT_EXIT, SHT_EDGE_TAIL);
}

///////////////////////////////////////////////////////////////////
// Function Name : SHT_CompileExitSensorON
// Description   : イベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SHT_CompileExitSensorON(const UC* notUsed, UC from)
{
	CShtManager_PathSensorEdgeChange(SHT_PATH_SNR_ID_COMPILE_EXIT, SHT_EDGE_LEAD);
}

///////////////////////////////////////////////////////////////////
// Function Name : SHT_CompileExitSensorOFF
// Description   : イベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SHT_CompileExitSensorOFF(const UC* notUsed, UC from)
{
	CShtManager_PathSensorEdgeChange(SHT_PATH_SNR_ID_COMPILE_EXIT, SHT_EDGE_TAIL);
}

/////////////////////////////////////////////////////////////////////////////////////////
// Sheet Management Event Call Back Function
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : SHT_SheetActionResponseEventCompilerShtInitCMPL
// Description   : Sheet Action Responseイベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SHT_SheetActionResponseEventCompilerShtInitCMPL(const UC* evt, UC from)
{
	EvtParamSheetActionResponse *pMessage;

	pMessage = (EvtParamSheetActionResponse *)evt;

	CShtManager_ReceiveSheetActionResponse(from, EVT_SHT_RES_COMPILER_SHT_INIT_CMPL, pMessage->sheetNo);
}

///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////
// Function Name : SHT_SheetActionResponseEventStackerShtInitTRAY   [GLOBAL_A4]
// Description   : Sheet Action Responseイベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SHT_SheetActionResponseEventStackerShtInitTRAY(const UC* evt, UC from)
{
	EvtParamSheetActionResponse *pMessage;

	pMessage = (EvtParamSheetActionResponse *)evt;

	CShtManager_ReceiveSheetActionResponse(from, EVT_SHT_RES_STACKER_SHT_INIT_TRAY, pMessage->sheetNo);
}


///////////////////////////////////////////////////////////////////
// Function Name : SHT_SheetEventNotifyEventOutTrayEjectFinishStart
// Description   : Sheet Event Notifyイベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SHT_SheetEventNotifyEventOutTrayEjectFinishStart(const UC* evt, UC from)
{
	EvtParamSheetEventNotify *pMessage;

	pMessage = (EvtParamSheetEventNotify *)evt;

	CShtManager_ReceiveSheetEventNotify(from, EVT_SHT_EVT_OUT_TRAY_EJECT_FINISH_START, SHT_EVT_ID_EJECT_FINISH_START, pMessage->sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : SHT_SheetEventNotifyEventOutTrayEjectFinishComp
// Description   : Sheet Event Notifyイベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SHT_SheetEventNotifyEventOutTrayEjectFinishComp(const UC* evt, UC from)
{
	EvtParamSheetEventNotify *pMessage;

	pMessage = (EvtParamSheetEventNotify *)evt;

	CShtManager_ReceiveSheetEventNotify(from, EVT_SHT_EVT_OUT_TRAY_EJECT_FINISH_COMP, SHT_EVT_ID_EJECT_FINISH_COMP, pMessage->sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : SHT_SheetEventNotifyEventOutTrayEjectStart
// Description   : Sheet Event Notifyイベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SHT_SheetEventNotifyEventOutTrayEjectStart(const UC* evt, UC from)
{
	EvtParamSheetEventNotify *pMessage;

	pMessage = (EvtParamSheetEventNotify *)evt;

	CShtManager_ReceiveSheetEventNotify(from, EVT_SHT_EVT_OUT_TRAY_EJECT_START, SHT_EVT_ID_EJECT_START, pMessage->sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : SHT_SheetEventNotifyEventOutTrayCompileFinishComp
// Description   : Sheet Event Notifyイベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SHT_SheetEventNotifyEventOutTrayCompileFinishComp(const UC* evt, UC from)
{
	EvtParamSheetEventNotify *pMessage;

	pMessage = (EvtParamSheetEventNotify *)evt;

	CShtManager_ReceiveSheetEventNotify(from, EVT_SHT_EVT_OUT_TRAY_COMPILE_FINISH_COMP, SHT_EVT_ID_COMPILE_FINISH_COMP, pMessage->sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : SHT_SheetEventNotifyEventOutTrayEjectComp
// Description   : Sheet Event Notifyイベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SHT_SheetEventNotifyEventOutTrayEjectComp(const UC* evt, UC from)
{
	EvtParamSheetEventNotify *pMessage;

	pMessage = (EvtParamSheetEventNotify *)evt;

	CShtManager_ReceiveSheetEventNotify(from, EVT_SHT_EVT_OUT_TRAY_EJECT_COMP, SHT_EVT_ID_EJECT_COMP, pMessage->sheetNo);
}



/////////////////////////////////////////////////////////////////////////////////////////
// Path Sensor Edge Event Call Back Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : SHT_PathSensorON
// Description   : イベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SHT_PathSensorON(const UC* notUsed, UC from)
{
	// CShtManager_PathSensorEdgeChange(SHT_PATH_SNR_ID_XXX, SHT_EDGE_LEAD);
}

///////////////////////////////////////////////////////////////////
// Function Name : SHT_PathSensorOFF
// Description   : イベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SHT_PathSensorOFF(const UC* notUsed, UC from)
{
	// CShtManager_PathSensorEdgeChange(SHT_PATH_SNR_ID_XXX, SHT_EDGE_TAIL);
}


/////////////////////////////////////////////////////////////////////////////////////////
// Sheet Management Event Call Back Function
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : SYS_SheetActionResponseEventSampleShtInitMdlA
// Description   : Sheet Action Responseイベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SYS_SheetActionResponseEventSampleShtInitMdlA(const UC* evt, UC from)
{
	EvtParamSheetActionResponse *pMessage;

	pMessage = (EvtParamSheetActionResponse *)evt;

	// CShtManager_ReceiveSheetActionResponse(from, EVT_SHT_REQ_A_SHT_INIT_MDLA, pMessage->sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : SYS_SheetActionResponseEventSampleShtInitMdlB
// Description   : Sheet Action Responseイベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SYS_SheetActionResponseEventSampleShtInitMdlB(const UC* evt, UC from)
{
	EvtParamSheetActionResponse *pMessage;

	pMessage = (EvtParamSheetActionResponse *)evt;

	// CShtManager_ReceiveSheetActionResponse(from, EVT_SHT_REQ_A_SHT_INIT_MDLB, pMessage->sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : SYS_SheetEventNotifyEventOutDest1EjectFinishComp
// Description   : Sheet Event Notifyイベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SYS_SheetEventNotifyEventOutDest1EjectFinishComp(const UC* evt, UC from)
{
	EvtParamSheetEventNotify *pMessage;

	pMessage = (EvtParamSheetEventNotify *)evt;

	// CShtManager_ReceiveSheetEventNotify(from, EVT_SHT_EVT_OUTDEST1_EJECT_FINISH_COMP, SHT_EVT_ID_EJECT_FINISH_COMP, pMessage->sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : SYS_SheetEventNotifyEventOutDest2EjectFinishComp
// Description   : Sheet Event Notifyイベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SYS_SheetEventNotifyEventOutDest2EjectFinishComp(const UC* evt, UC from)
{
	EvtParamSheetEventNotify *pMessage;

	pMessage = (EvtParamSheetEventNotify *)evt;

	// CShtManager_ReceiveSheetEventNotify(from, EVT_SHT_EVT_OUTDEST2_EJECT_FINISH_COMP, SHT_EVT_ID_EJECT_FINISH_COMP, pMessage->sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : SYS_SheetEventNotifyEventOutDest1EjectStart
// Description   : Sheet Event Notifyイベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SYS_SheetEventNotifyEventOutDest1EjectStart(const UC* evt, UC from)
{
	EvtParamSheetEventNotify *pMessage;

	pMessage = (EvtParamSheetEventNotify *)evt;

	// CShtManager_ReceiveSheetEventNotify(from, EVT_SHT_EVT_OUTDEST1_EJECT_START, SHT_EVT_ID_EJECT_START, pMessage->sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : SYS_SheetEventNotifyEventOutDest2EjectStart
// Description   : Sheet Event Notifyイベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SYS_SheetEventNotifyEventOutDest2EjectStart(const UC* evt, UC from)
{
	EvtParamSheetEventNotify *pMessage;

	pMessage = (EvtParamSheetEventNotify *)evt;

	// CShtManager_ReceiveSheetEventNotify(from, EVT_SHT_EVT_OUTDEST2_EJECT_START, SHT_EVT_ID_EJECT_START, pMessage->sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : SYS_SheetEventNotifyEventOutDest1CompileFinishComp
// Description   : Sheet Event Notifyイベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SYS_SheetEventNotifyEventOutDest1CompileFinishComp(const UC* evt, UC from)
{
	EvtParamSheetEventNotify *pMessage;

	pMessage = (EvtParamSheetEventNotify *)evt;

	// CShtManager_ReceiveSheetEventNotify(from, EVT_SHT_EVT_OUTDEST1_COMPILE_FINISH_COMP, SHT_EVT_ID_COMPILE_FINISH_COMP, pMessage->sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : SYS_SheetEventNotifyEventOutDest2CompileFinishComp
// Description   : Sheet Event Notifyイベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SYS_SheetEventNotifyEventOutDest2CompileFinishComp(const UC* evt, UC from)
{
	EvtParamSheetEventNotify *pMessage;

	pMessage = (EvtParamSheetEventNotify *)evt;

	// CShtManager_ReceiveSheetEventNotify(from, EVT_SHT_EVT_OUTDEST2_COMPILE_FINISH_COMP, SHT_EVT_ID_COMPILE_FINISH_COMP, pMessage->sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : SYS_SheetEventNotifyEventPunchBackStart
// Description   : Sheet Event Notifyイベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SYS_SheetEventNotifyEventPunchBackStart(const UC* evt, UC from)
{
	EvtParamSheetEventNotify *pMessage;

	pMessage = (EvtParamSheetEventNotify *)evt;

	// CShtManager_ReceiveSheetEventNotify(from, EVT_SHT_EVT_PUNCH_BACK_START, SHT_EVT_ID_PNCH_BACK_START, pMessage->sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : SYS_SheetEventNotifyEventPunchBackComp
// Description   : Sheet Event Notifyイベントコールバック
// Parameter     : evt (メッセージの先頭アドレス)
//               : from (イベント送信者)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SYS_SheetEventNotifyEventPunchBackComp(const UC* evt, UC from)
{
	EvtParamSheetEventNotify *pMessage;

	pMessage = (EvtParamSheetEventNotify *)evt;

	// CShtManager_ReceiveSheetEventNotify(from, EVT_SHT_EVT_PUNCH_BACK_COMP, SHT_EVT_ID_PNCH_BACK_COMP, pMessage->sheetNo);
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
// 15.03.30	| Yuichi Hoshino	| Design Improvement [ Sheet Event定義見直し ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//



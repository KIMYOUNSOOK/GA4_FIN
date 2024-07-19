// File Name:	CCdiFinDiagDataRespDc361Start.h
// Description:	Header File of CCdiFinDiagDataRespDc361Start
// Engineer:	Yuichi Hoshino
// Last Edit:	16.02.24
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDiagDataRespDc361Start_h)
#define CCdiFinDiagDataRespDc361Start_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "ECdiFinNvmArea.h"
#include "ECdiFinDevCont.h"

#include "SCdiFinDiagDataRespDc361StartFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinDiagDataRespDc361Start_GetDiagProgram() ((US)eCdiFinDiagPrgrm_dC361)
																				// US (void)
#define CCdiFinDiagDataRespDc361Start_GetDiagCommand() ((UC)eCdiFinDiagCmd_Start)
																				// UC (void)
#define CCdiFinDiagDataRespDc361Start_GetDiagDataLength(apData) ((UC)CDIFIN_DIAG_DATA_RESP_DC361_START_SIZE)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc361Start_GetNvmArea(apData) (((struct SCdiFinDiagDataRespDc361StartFrmt *)(apData))->nvmArea)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc361Start_SetNvmArea(apData, aArea) (((struct SCdiFinDiagDataRespDc361StartFrmt *)(apData))->nvmArea = aArea)
																				// void (UC*, UC)
#define CCdiFinDiagDataRespDc361Start_GetDeviceControllerID(apData) (((struct SCdiFinDiagDataRespDc361StartFrmt *)(apData))->finDevContID)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc361Start_SetDeviceControllerID(apData, aID) (((struct SCdiFinDiagDataRespDc361StartFrmt *)(apData))->finDevContID = aID)
																				// void (UC*, UC)
#define CCdiFinDiagDataRespDc361Start_GetNvmAreaSize(apData) ( ((US)(((struct SCdiFinDiagDataRespDc361StartFrmt *)(apData))->nvmSize_Octet0) << 8) | ((US)(((struct SCdiFinDiagDataRespDc361StartFrmt *)(apData))->nvmSize_Octet1)) )
																				// US (UC*)
#define CCdiFinDiagDataRespDc361Start_SetNvmAreaSize(apData, aSize) ((struct SCdiFinDiagDataRespDc361StartFrmt *)(apData))->nvmSize_Octet0 = (UC)((US)(aSize) >> 8); ((struct SCdiFinDiagDataRespDc361StartFrmt *)(apData))->nvmSize_Octet1 = (UC)((aSize))
																				// void (UC*, US)
																				
#endif // !defined(CCdiFinDiagDataRespDc361Start_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file based on Base SW VXX
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.12.04	| Yuichi Hoshino	| Created this file
// 14.12.04	| Yuichi Hoshino	| Release V0 (Revision 00)
// 15.07.21 | Yuichi Hoshino	| Trouble Action [ DC361 NVM Save実施時、NVM領域サイズと読み込みデータのサイズが一致しない ]
// 16.XX.XX	| Yuichi Hoshino	| Release V2 (Revision 01)
//

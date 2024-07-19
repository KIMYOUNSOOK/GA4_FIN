// File Name:	CCdiFinDiagDataRespDc361Read.h
// Description:	Header File of CCdiFinDiagDataRespDc361Read
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.19
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDiagDataRespDc361Read_h)
#define CCdiFinDiagDataRespDc361Read_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "ECdiFinNvmArea.h"
#include "ECdiFinDevCont.h"
#include "ECdiFinExist.h"

#include "SCdiFinDiagDataRespDc361ReadFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinDiagDataRespDc361Read_GetDiagProgram() ((US)eCdiFinDiagPrgrm_dC361)
																				// US (void)
#define CCdiFinDiagDataRespDc361Read_GetDiagCommand() ((UC)eCdiFinDiagCmd_Read)	// UC (void)
#define CCdiFinDiagDataRespDc361Read_GetDiagDataLength(apData) (4 + ((struct SCdiFinDiagDataRespDc361ReadFrmt *)(apData))->allocateSize)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc361Read_GetNvmArea(apData) (((struct SCdiFinDiagDataRespDc361ReadFrmt *)(apData))->nvmArea)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc361Read_SetNvmArea(apData, aArea) (((struct SCdiFinDiagDataRespDc361ReadFrmt *)(apData))->nvmArea = aArea)
																				// void (UC*, UC)
#define CCdiFinDiagDataRespDc361Read_GetDeviceControllerID(apData) (((struct SCdiFinDiagDataRespDc361ReadFrmt *)(apData))->finDevContID)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc361Read_SetDeviceControllerID(apData, aID) (((struct SCdiFinDiagDataRespDc361ReadFrmt *)(apData))->finDevContID = aID)
																				// void (UC*, UC)
#define CCdiFinDiagDataRespDc361Read_GetPresenceOfRemainData(apData) (((struct SCdiFinDiagDataRespDc361ReadFrmt *)(apData))->presenceOfRemain)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc361Read_SetPresenceOfRemainData(apData, aPresence) (((struct SCdiFinDiagDataRespDc361ReadFrmt *)(apData))->presenceOfRemain = aPresence)
																				// void (UC*, UC)
#define CCdiFinDiagDataRespDc361Read_GetAllocatedDataSize(apData) (((struct SCdiFinDiagDataRespDc361ReadFrmt *)(apData))->allocateSize)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc361Read_SetAllocatedDataSize(apData, aSize) (((struct SCdiFinDiagDataRespDc361ReadFrmt *)(apData))->allocateSize = aSize)
																				// void (UC*, UC)
#define CCdiFinDiagDataRespDc361Read_GetAllocatedDataValue(apData)	(&(((struct SCdiFinDiagDataRespDc361ReadFrmt *)(apData))->allocateVal[0]))
																				// UC* (UC*)
																				
#endif // !defined(CCdiFinDiagDataRespDc361Read_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

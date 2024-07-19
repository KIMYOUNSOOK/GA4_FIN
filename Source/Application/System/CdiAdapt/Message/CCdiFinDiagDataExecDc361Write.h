// File Name:	CCdiFinDiagDataExecDc361Write.h
// Description:	Header File of CCdiFinDiagDataExecDc361Write
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.19
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDiagDataExecDc361Write_h)
#define CCdiFinDiagDataExecDc361Write_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "ECdiFinNvmArea.h"
#include "ECdiFinDevCont.h"

#include "SCdiFinDiagDataExecDc361WriteFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinDiagDataExecDc361Write_GetDiagProgram() ((US)eCdiFinDiagPrgrm_dC361)
																				// US (void)
#define CCdiFinDiagDataExecDc361Write_GetDiagCommand() ((UC)eCdiFinDiagCmd_Write)
																				// UC (void)
#define CCdiFinDiagDataExecDc361Write_GetDiagDataLength(apData) ((UC)CDIFIN_DIAG_DATA_EXEC_DC361_WRITE_SIZE)
																				// UC (UC*)
#define CCdiFinDiagDataExecDc361Write_GetNvmArea(apData) (((struct SCdiFinDiagDataExecDc361WriteFrmt *)(apData))->nvmArea)
																				// UC (UC*)
#define CCdiFinDiagDataExecDc361Write_SetNvmArea(apData, aArea) (((struct SCdiFinDiagDataExecDc361WriteFrmt *)(apData))->nvmArea = aArea)
																				// void (UC*, UC)
#define CCdiFinDiagDataExecDc361Write_GetDeviceControllerID(apData) (((struct SCdiFinDiagDataExecDc361WriteFrmt *)(apData))->finDevContID)
																				// UC (UC*)
#define CCdiFinDiagDataExecDc361Write_SetDeviceControllerID(apData, aID) (((struct SCdiFinDiagDataExecDc361WriteFrmt *)(apData))->finDevContID = aID)
																				// void (UC*, UC)
#define CCdiFinDiagDataExecDc361Write_GetPresenceOfRemainData(apData) (((struct SCdiFinDiagDataExecDc361WriteFrmt *)(apData))->presenceOfRemain)
																				// UC (UC*)
#define CCdiFinDiagDataExecDc361Write_SetPresenceOfRemainData(apData, aPresence) (((struct SCdiFinDiagDataExecDc361WriteFrmt *)(apData))->presenceOfRemain = aPresence)
																				// void (UC*, UC)
#define CCdiFinDiagDataExecDc361Write_GetAllocatedDataSize(apData) (((struct SCdiFinDiagDataExecDc361WriteFrmt *)(apData))->allocateSize)
																				// UC (UC*)
#define CCdiFinDiagDataExecDc361Write_SetAllocatedDataSize(apData, aSize) ((((struct SCdiFinDiagDataExecDc361WriteFrmt *)(apData))->allocateSize = aSize)
																				// void (UC*, UC)
#define CCdiFinDiagDataExecDc361Write_GetAllocatedDataValue(apData)	(((struct SCdiFinDiagDataExecDc361WriteFrmt *)(apData))->allocateVal)
																				// UC* (UC*)


#endif // !defined(CCdiFinDiagDataExecDc361Write_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

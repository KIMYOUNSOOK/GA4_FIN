// File Name:	CCdiFinDiagDataRespDc361Write.h
// Description:	Header File of CCdiFinDiagDataRespDc361Write
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.19
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDiagDataRespDc361Write_h)
#define CCdiFinDiagDataRespDc361Write_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "ECdiFinNvmArea.h"
#include "ECdiFinDevCont.h"

#include "SCdiFinDiagDataRespDc361WriteFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinDiagDataRespDc361Write_GetDiagProgram() ((US)eCdiFinDiagPrgrm_dC361)
																				// US (void)
#define CCdiFinDiagDataRespDc361Write_GetDiagCommand() ((UC)eCdiFinDiagCmd_Write)
																				// UC (void)
#define CCdiFinDiagDataRespDc361Write_GetDiagDataLength(apData) ((UC)CDIFIN_DIAG_DATA_RESP_DC361_WRITE_SIZE)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc361Write_GetNvmArea(apData) (((struct SCdiFinDiagDataRespDc361WriteFrmt *)(apData))->nvmArea)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc361Write_SetNvmArea(apData, aArea) (((struct SCdiFinDiagDataRespDc361WriteFrmt *)(apData))->nvmArea = aArea)
																				// void (UC*, UC)
#define CCdiFinDiagDataRespDc361Write_GetDeviceControllerID(apData) (((struct SCdiFinDiagDataRespDc361WriteFrmt *)(apData))->finDevContID)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc361Write_SetDeviceControllerID(apData, aID) (((struct SCdiFinDiagDataRespDc361WriteFrmt *)(apData))->finDevContID = aID)
																				// void (UC*, UC)

																				
#endif // !defined(CCdiFinDiagDataRespDc361Write_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

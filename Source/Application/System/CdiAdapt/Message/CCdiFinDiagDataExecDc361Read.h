// File Name:	CCdiFinDiagDataExecDc361Read.h
// Description:	Header File of CCdiFinDiagDataExecDc361Read
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.19
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDiagDataExecDc361Read_h)
#define CCdiFinDiagDataExecDc361Read_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "ECdiFinNvmArea.h"
#include "ECdiFinDevCont.h"

#include "SCdiFinDiagDataExecDc361ReadFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinDiagDataExecDc361Read_GetDiagProgram() ((US)eCdiFinDiagPrgrm_dC361)
																				// US (void)
#define CCdiFinDiagDataExecDc361Read_GetDiagCommand() ((UC)eCdiFinDiagCmd_Read)	// UC (void)
#define CCdiFinDiagDataExecDc361Read_GetDiagDataLength(apData) ((UC)CDIFIN_DIAG_DATA_EXEC_DC361_READ_SIZE)
																				// UC (UC*)
#define CCdiFinDiagDataExecDc361Read_GetNvmArea(apData) (UC)((apData)[0])		// UC (UC*)
#define CCdiFinDiagDataExecDc361Read_SetNvmArea(apData, aArea) (((apData)[0]) = aArea)
																				// void (UC*, UC)
#define CCdiFinDiagDataExecDc361Read_GetDeviceControllerID(apData) (((struct SCdiFinDiagDataExecDc361ReadFrmt *)(apData))->finDevContID)
																				// UC (UC*)
#define CCdiFinDiagDataExecDc361Read_SetDeviceControllerID(apData, aID) (((struct SCdiFinDiagDataExecDc361ReadFrmt *)(apData))->finDevContID = aID)
																				// void (UC*, UC)


#endif // !defined(CCdiFinDiagDataExecDc361Read_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

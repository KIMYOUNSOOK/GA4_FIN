// File Name:	CCdiFinDiagDataExecDc361Start.h
// Description:	Header File of CCdiFinDiagDataExecDc361Start
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.19
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDiagDataExecDc361Start_h)
#define CCdiFinDiagDataExecDc361Start_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "ECdiFinNvmArea.h"
#include "ECdiFinDevCont.h"

#include "SCdiFinDiagDataExecDc361StartFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinDiagDataExecDc361Start_GetDiagProgram(apData) ((US)eCdiFinDiagPrgrm_dC361)
																				// US (UC*)
#define CCdiFinDiagDataExecDc361Start_GetDiagCommand(apData) ((UC)eCdiFinDiagCmd_Start)
																				// UC (UC*)
#define CCdiFinDiagDataExecDc361Start_GetDiagDataLength(apData) ((UC)CDIFIN_DIAG_DATA_EXEC_DC361_START_SIZE)
																				// UC (UC*)
#define CCdiFinDiagDataExecDc361Start_GetNvmArea(apData) (UC)((apData)[0])		// UC (UC*)
#define CCdiFinDiagDataExecDc361Start_SetNvmArea(apData, aArea) (((apData)[0]) = aArea)
																				// void (UC*, UC)
#define CCdiFinDiagDataExecDc361Start_GetDeviceControllerID(apData) (UC)((apData)[1])
																				// UC (UC*)
#define CCdiFinDiagDataExecDc361Start_SetDeviceControllerID(apData, aID) (((apData)[1]) = aID)
																				// void (UC*, UC)


#endif // !defined(CCdiFinDiagDataExecDc361Start_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

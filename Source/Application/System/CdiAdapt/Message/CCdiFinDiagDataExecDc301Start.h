// File Name:	CCdiFinDiagDataExecDc301Start.h
// Description:	Header File of CCdiFinDiagDataExecDc301Start
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.19
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDiagDataExecDc301Start_h)
#define CCdiFinDiagDataExecDc301Start_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "ECdiFinDevCont.h"

#include "SCdiFinDiagDataExecDc301StartFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinDiagDataExecDc301Start_GetDiagProgram() ((US)eCdiFinDiagPrgrm_dC301)
																				// US (void)
#define CCdiFinDiagDataExecDc301Start_GetDiagCommand() ((UC)eCdiFinDiagCmd_Start)
																				// UC (void)
#define CCdiFinDiagDataExecDc301Start_GetDiagDataLength(apData) ((UC)CDIFIN_DIAG_DATA_EXEC_DC301_STRAT_SIZE)
																				// UC (UC*)
#define CCdiFinDiagDataExecDc301Start_GetChain(apData) ( ((US)((apData)[0]) << 8) | ((US)((apData)[1])) )
																				// US (UC*)
#define CCdiFinDiagDataExecDc301Start_SetChain(apData, aChain) ((apThis)[0]) = (UC)((US)(aChain) >> 8); ((apThis)[1]) = (UC)(aChain)
																				// void (UC*, US)
#define CCdiFinDiagDataExecDc301Start_GetLink(apData) ( ((US)((apData)[2]) << 8) | ((US)((apData)[3])) )
																				// US (UC*)
#define CCdiFinDiagDataExecDc301Start_SetLink(apData, aLink) ((apData)[2]) = (UC)((US)(aChain) >> 8); ((apThis)[3]) = (UC)(aChain)
																				// void (UC*, US)
#define CCdiFinDiagDataExecDc301Start_GetDeviceControllerID(apData) (UC)((apData)[5])
																				// UC (UC*)
#define CCdiFinDiagDataExecDc301Start_SetDeviceControllerID(apData, aID) (((apData)[5]) = aID)
																				// void (UC*, UC)

																				
#endif // !defined(CCdiFinDiagDataExecDc301Start_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

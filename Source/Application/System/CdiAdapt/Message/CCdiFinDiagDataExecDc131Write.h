// File Name:	CCdiFinDiagDataExecDc131Write.h
// Description:	Header File of CCdiFinDiagDataExecDc131Write
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.19
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDiagDataExecDc131Write_h)
#define CCdiFinDiagDataExecDc131Write_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "ECdiFinDataType.h"

#include "SCdiFinDiagDataExecDc131WriteFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinDiagDataExecDc131Write_GetDiagProgram() ((US)eCdiFinDiagPrgrm_dC131)
																				// US (void)
#define CCdiFinDiagDataExecDc131Write_GetDiagCommand() ((UC)eCdiFinDiagCmd_Write)
																				// UC (void)
#define CCdiFinDiagDataExecDc131Write_GetDiagDataLength(apData) ((UC)CDIFIN_DIAG_DATA_EXEC_DC131_WRITE_SIZE)
																				// UC (UC*)
#define CCdiFinDiagDataExecDc131Write_GetChain(apData) ( ((US)((apData)[0]) << 8) | ((US)((apData)[1])) )
																				// US (UC*)
#define CCdiFinDiagDataExecDc131Write_SetChain(apData, aChain) ((apThis)[0]) = (UC)((US)(aChain) >> 8); ((apThis)[1]) = (UC)(aChain)
																				// void (UC*, US)
#define CCdiFinDiagDataExecDc131Write_GetLink(apData) ( ((US)((apData)[2]) << 8) | ((US)((apData)[3])) )
																				// US (UC*)
#define CCdiFinDiagDataExecDc131Write_SetLink(apData, aLink) ((apData)[2]) = (UC)((US)(aChain) >> 8); ((apThis)[3]) = (UC)(aChain)
																				// void (UC*, US)
#define CCdiFinDiagDataExecDc131Write_GetDataType(apData) ((apData)[4])			// UC (UC*)
#define CCdiFinDiagDataExecDc131Write_SetDataType(apData, aType) (((apData)[4]) = aType)
																				// void (UC*, UC)
#define CCdiFinDiagDataExecDc131Write_GetNvmValue(apData) ( ((UL)((apData)[5]) << 24) | ((UL)((apData)[6]) << 16) | ((UL)((apData)[7]) << 8) | ((UL)((apData)[8])) )
																				// UL (UC*)
#define CCdiFinDiagDataExecDc131Write_SetNvmValue(apData, aValue) ((apData)[5]) = (UC)((UL)(aValue) >> 8); ((apData)[6]) = (UC)((UL)(aValue) >> 8); ((apData)[7]) = (UC)((UL)(aValue) >> 8); ((apData)[8]) = (UC)(aValue)
																				// void (UC*, UL)


#endif // !defined(CCdiFinDiagDataExecDc131Write_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

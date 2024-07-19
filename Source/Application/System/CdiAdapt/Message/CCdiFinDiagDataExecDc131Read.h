// File Name:	CCdiFinDiagDataExecDc131Read.h
// Description:	Header File of CCdiFinDiagDataExecDc131Read
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.18
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDiagDataExecDc131Read_h)
#define CCdiFinDiagDataExecDc131Read_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "SCdiFinDiagDataExecDc131ReadFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinDiagDataExecDc131Read_GetDiagProgram() ((US)eCdiFinDiagPrgrm_dC131)
																				// US (void)
#define CCdiFinDiagDataExecDc131Read_GetDiagCommand() ((UC)eCdiFinDiagCmd_Read)	// UC (void)
#define CCdiFinDiagDataExecDc131Read_GetDiagDataLength(apData) ((UC)CDIFIN_DIAG_DATA_EXEC_DC131_READ_SIZE)
																				// UC (UC*)
#define CCdiFinDiagDataExecDc131Read_GetChain(apData) ( ((US)((apData)[0]) << 8) | ((US)((apData)[1])) )
																				// US (UC*)
#define CCdiFinDiagDataExecDc131Read_SetChain(apData, aChain) ((apThis)[0]) = (UC)((US)(aChain) >> 8); ((apThis)[1]) = (UC)((aChain))
																				// void (UC*, US)
#define CCdiFinDiagDataExecDc131Read_GetLink(apData) ( ((US)((apData)[2]) << 8) | ((US)((apData)[3])) )
																				// US (UC*)
#define CCdiFinDiagDataExecDc131Read_SetLink(apData, aLink) ((apThis)[2]) = (UC)((US)(aChain) >> 8); ((apThis)[3]) = (UC)((aChain))
																				// void (UC*, US)


#endif // !defined(CCdiFinDiagDataExecDc131Read_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

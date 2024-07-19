// File Name:	CCdiFinDiagDataExecDc135Read.h
// Description:	Header File of CCdiFinDiagDataExecDc135Read
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.18
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDiagDataExecDc135Read_h)
#define CCdiFinDiagDataExecDc135Read_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "SCdiFinDiagDataExecDc135ReadFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinDiagDataExecDc135Read_GetDiagProgram() ((US)eCdiFinDiagPrgrm_dC135)
																				// US (UC*)
#define CCdiFinDiagDataExecDc135Read_GetDiagCommand() ((UC)eCdiFinDiagCmd_Read)	// UC (void)
#define CCdiFinDiagDataExecDc135Read_GetDiagDataLength(apData) ((UC)CDIFIN_DIAG_DATA_EXEC_DC135_READ_SIZE)
																				// UC (UC*)
#define CCdiFinDiagDataExecDc135Read_GetChain(apData) ( ((US)(((struct SCdiFinDiagDataExecDc135ReadFrmt *)(apData))->chain_Octet0) << 8) | ((US)(((struct SCdiFinDiagDataExecDc135ReadFrmt *)(apData))->chain_Octet1)) )
																				// US (UC*)
#define CCdiFinDiagDataExecDc135Read_SetChain(apData, aChain) ((struct SCdiFinDiagDataExecDc135ReadFrmt *)(apData))->chain_Octet0 = (UC)((US)(aChain) >> 8); ((struct SCdiFinDiagDataExecDc135ReadFrmt *)(apData))->chain_Octet1 = (UC)((aChain))
																				// void (UC*, US)
#define CCdiFinDiagDataExecDc135Read_GetLink(apData) ( ((US)(((struct SCdiFinDiagDataExecDc135ReadFrmt *)(apData))->link_Octet0) << 8) | ((US)(((struct SCdiFinDiagDataExecDc135ReadFrmt *)(apData))->link_Octet1)) )
																				// US (UC*)
#define CCdiFinDiagDataExecDc135Read_SetLink(apData, aLink) ((struct SCdiFinDiagDataExecDc135ReadFrmt *)(apData))->link_Octet0 = (UC)((US)(aChain) >> 8); ((struct SCdiFinDiagDataExecDc135ReadFrmt *)(apData))->link_Octet1 = (UC)((aChain))
																				// void (UC*, US)


#endif // !defined(CCdiFinDiagDataExecDc135Read_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

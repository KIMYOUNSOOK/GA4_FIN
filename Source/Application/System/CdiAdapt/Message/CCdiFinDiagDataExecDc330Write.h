// File Name:	CCdiFinDiagDataExecDc330Write.h
// Description:	Header File of CCdiFinDiagDataExecDc330Write
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.19
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDiagDataExecDc330Write_h)
#define CCdiFinDiagDataExecDc330Write_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "SCdiFinDiagDataExecDc330WriteFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinDiagDataExecDc330Write_GetDiagProgram() ((US)eCdiFinDiagPrgrm_dC330)
																				// US (void)
#define CCdiFinDiagDataExecDc330Write_GetDiagCommand() ((UC)eCdiFinDiagCmd_Write)
																				// UC (void)
#define CCdiFinDiagDataExecDc330Write_GetDiagDataLength(apData) ((UC)CDIFIN_DIAG_DATA_EXEC_DC330_WRITE_SIZE)
																				// UC (UC*)
#define CCdiFinDiagDataExecDc330Write_GetChain(apData) ( ((US)(((struct SCdiFinDiagDataExecDc330WriteFrmt *)(apData))->chain_Octet0) << 8) | ((US)(((struct SCdiFinDiagDataExecDc330WriteFrmt *)(apData))->chain_Octet1)) )
																				// US (UC*)
#define CCdiFinDiagDataExecDc330Write_SetChain(apData, aChain) ((struct SCdiFinDiagDataExecDc330WriteFrmt *)(apData))->chain_Octet0 = (UC)((US)(aChain) >> 8); ((struct SCdiFinDiagDataExecDc330WriteFrmt *)(apData))->chain_Octet1 = (UC)((aChain))
																				// void (UC*, US)
#define CCdiFinDiagDataExecDc330Write_GetLink(apData) ( ((US)(((struct SCdiFinDiagDataExecDc330WriteFrmt *)(apData))->link_Octet0) << 8) | ((US)(((struct SCdiFinDiagDataExecDc330WriteFrmt *)(apData))->link_Octet1)) )
																				// US (UC*)
#define CCdiFinDiagDataExecDc330Write_SetLink(apData, aLink) ((struct SCdiFinDiagDataExecDc330WriteFrmt *)(apData))->link_Octet0 = (UC)((US)(aChain) >> 8); ((struct SCdiFinDiagDataExecDc330WriteFrmt *)(apData))->link_Octet1 = (UC)((aChain))
																				// void (UC*, US)
#define CCdiFinDiagDataExecDc330Write_GetComponentState(apData) (((struct SCdiFinDiagDataExecDc330WriteFrmt *)(apData))->componentStt)
																				// UC (UC*)
#define CCdiFinDiagDataExecDc330Write_SetComponentState(apData, aState) (((struct SCdiFinDiagDataExecDc330WriteFrmt *)(apData))->componentStt = aState)
																				// void (UC*, UC)


#endif // !defined(CCdiFinDiagDataExecDc330Write_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

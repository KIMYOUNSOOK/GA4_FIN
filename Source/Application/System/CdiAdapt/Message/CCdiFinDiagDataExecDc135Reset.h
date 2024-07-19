// File Name:	CCdiFinDiagDataExecDc135Reset.h
// Description:	Header File of CCdiFinDiagDataExecDc135Reset
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.19
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDiagDataExecDc135Reset_h)
#define CCdiFinDiagDataExecDc135Reset_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "SCdiFinDiagDataExecDc135ResetFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinDiagDataExecDc135Reset_GetDiagProgram() ((US)eCdiFinDiagPrgrm_dC135)
																				// US (void)
#define CCdiFinDiagDataExecDc135Reset_GetDiagCommand() ((UC)eCdiFinDiagCmd_Reset)
																				// UC (void)
#define CCdiFinDiagDataExecDc135Reset_GetDiagDataLength(apData) ((UC)CDIFIN_DIAG_DATA_EXEC_DC135_RESET_SIZE)
																				// UC (UC*)
#define CCdiFinDiagDataExecDc135Reset_GetChain(apData) ( ((US)(((struct SCdiFinDiagDataExecDc135ResetFrmt *)(apData))->chain_Octet0) << 8) | ((US)(((struct SCdiFinDiagDataExecDc135ResetFrmt *)(apData))->chain_Octet1)) )
																				// US (UC*)
#define CCdiFinDiagDataExecDc135Reset_SetChain(apData, aChain) ((struct SCdiFinDiagDataExecDc135ResetFrmt *)(apData))->chain_Octet0 = (UC)((US)(aChain) >> 8); ((struct SCdiFinDiagDataExecDc135ResetFrmt *)(apData))->chain_Octet1 = (UC)((aChain))
																				// void (UC*, US)
#define CCdiFinDiagDataExecDc135Reset_GetLink(apData) ( ((US)(((struct SCdiFinDiagDataExecDc135ResetFrmt *)(apData))->link_Octet0) << 8) | ((US)(((struct SCdiFinDiagDataExecDc135ResetFrmt *)(apData))->link_Octet1)) )
																				// US (UC*)
#define CCdiFinDiagDataExecDc135Reset_SetLink(apData, aLink) ((struct SCdiFinDiagDataExecDc135ResetFrmt *)(apData))->link_Octet0 = (UC)((US)(aChain) >> 8); ((struct SCdiFinDiagDataExecDc135ResetFrmt *)(apData))->link_Octet1 = (UC)((aChain))
																				// void (UC*, US)
#define CCdiFinDiagDataExecDc135Reset_GetReasonForExchange(apData) (((struct SCdiFinDiagDataExecDc135ResetFrmt *)(apData))->reasonExchg)
																				// UC (UC*)
#define CCdiFinDiagDataExecDc135Reset_SetReasonForExchange(apData, aReason) (((struct SCdiFinDiagDataExecDc135ResetFrmt *)(apData))->reasonExchg = aReason)
																				// void (UC*, UC)
#define CCdiFinDiagDataExecDc135Reset_GetDeviceControllerID(apData) (((struct SCdiFinDiagDataExecDc135ResetFrmt *)(apData))->finDevContID)
																				// UC (UC*)
#define CCdiFinDiagDataExecDc135Reset_SetDeviceControllerID(apData, aID) (((struct SCdiFinDiagDataExecDc135ResetFrmt *)(apData))->finDevContID = aID)
																				// void (UC*, UC)


#endif // !defined(CCdiFinDiagDataExecDc135Reset_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

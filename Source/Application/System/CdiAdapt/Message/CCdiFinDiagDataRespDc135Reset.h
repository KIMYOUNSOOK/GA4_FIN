// File Name:	CCdiFinDiagDataRespDc135Reset.h
// Description:	Header File of CCdiFinDiagDataRespDc135Reset
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.24
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDiagDataRespDc135Reset_h)
#define CCdiFinDiagDataRespDc135Reset_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "ECdiFinDevCont.h"

#include "SCdiFinDiagDataRespDc135ResetFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinDiagDataRespDc135Reset_GetDiagProgram() ((US)eCdiFinDiagPrgrm_dC135)
																				// US (void)
#define CCdiFinDiagDataRespDc135Reset_GetDiagCommand() ((UC)eCdiFinDiagCmd_Reset)
																				// UC (void)
#define CCdiFinDiagDataRespDc135Reset_GetDiagDataLength(apData) ((UC)CDIFIN_DIAG_DATA_RESP_DC135_RESET_SIZE)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc135Reset_GetChain(apData) ( ((US)(((struct SCdiFinDiagDataRespDc135ResetFrmt *)(apData))->chain_Octet0) << 8) | ((US)(((struct SCdiFinDiagDataRespDc135ResetFrmt *)(apData))->chain_Octet1)) )
																				// US (UC*)
#define CCdiFinDiagDataRespDc135Reset_SetChain(apData, aChain) ((struct SCdiFinDiagDataRespDc135ResetFrmt *)(apData))->chain_Octet0 = (UC)((US)(aChain) >> 8); ((struct SCdiFinDiagDataRespDc135ResetFrmt *)(apData))->chain_Octet1 = (UC)((aChain))
																				// void (UC*, US)
#define CCdiFinDiagDataRespDc135Reset_GetLink(apData) ( ((US)(((struct SCdiFinDiagDataRespDc135ResetFrmt *)(apData))->link_Octet0) << 8) | ((US)(((struct SCdiFinDiagDataRespDc135ResetFrmt *)(apData))->link_Octet1)) )
																				// US (UC*)
#define CCdiFinDiagDataRespDc135Reset_SetLink(apData, aLink) ((struct SCdiFinDiagDataRespDc135ResetFrmt *)(apData))->link_Octet0 = (UC)((US)(aLink) >> 8); ((struct SCdiFinDiagDataRespDc135ResetFrmt *)(apData))->link_Octet1 = (UC)((aLink))
																				// void (UC*, US)
#define CCdiFinDiagDataRespDc135Reset_GetDeviceControllerID(apData) (((struct SCdiFinDiagDataRespDc135ResetFrmt *)(apData))->finDevContID)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc135Reset_SetDeviceControllerID(apData, aID) (((struct SCdiFinDiagDataRespDc135ResetFrmt *)(apData))->finDevContID = aID)
																				// void (UC*, UC)

#endif // !defined(CCdiFinDiagDataRespDc135Reset_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

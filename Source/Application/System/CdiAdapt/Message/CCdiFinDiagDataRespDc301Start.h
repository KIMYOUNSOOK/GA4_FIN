// File Name:	CCdiFinDiagDataRespDc301Start.h
// Description:	Header File of CCdiFinDiagDataRespDc301Start
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.24
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDiagDataRespDc301Start_h)
#define CCdiFinDiagDataRespDc301Start_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "ECdiFinInitResp.h"
#include "ECdiFinDevCont.h"

#include "SCdiFinDiagDataRespDc301StartFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinDiagDataRespDc301Start_GetDiagProgram() ((US)eCdiFinDiagPrgrm_dC301)
																				// US (void)
#define CCdiFinDiagDataRespDc301Start_GetDiagCommand() ((UC)eCdiFinDiagCmd_Start)
																				// UC (void)
#define CCdiFinDiagDataRespDc301Start_GetDiagDataLength(apData) ((UC)CDIFIN_DIAG_DATA_RESP_DC301_START_SIZE)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc301Start_GetChain(apData) ( ((US)(((struct SCdiFinDiagDataRespDc301StartFrmt *)(apData))->chain_Octet0) << 8) | ((US)(((struct SCdiFinDiagDataRespDc301StartFrmt *)(apData))->chain_Octet1)) )
																				// US (UC*)
#define CCdiFinDiagDataRespDc301Start_SetChain(apData, aChain) ((struct SCdiFinDiagDataRespDc301StartFrmt *)(apData))->chain_Octet0 = (UC)((US)(aChain) >> 8); ((struct SCdiFinDiagDataRespDc301StartFrmt *)(apData))->chain_Octet1 = (UC)((aChain))
																				// void (UC*, US)
#define CCdiFinDiagDataRespDc301Start_GetLink(apData) ( ((US)(((struct SCdiFinDiagDataRespDc301StartFrmt *)(apData))->link_Octet0) << 8) | ((US)(((struct SCdiFinDiagDataRespDc301StartFrmt *)(apData))->link_Octet1)) )
																				// US (UC*)
#define CCdiFinDiagDataRespDc301Start_SetLink(apData, aLink) ((struct SCdiFinDiagDataRespDc301StartFrmt *)(apData))->link_Octet0 = (UC)((US)(aLink) >> 8); ((struct SCdiFinDiagDataRespDc301StartFrmt *)(apData))->link_Octet1 = (UC)((aLink))
																				// void (UC*, US)
#define CCdiFinDiagDataRespDc301Start_GetAttributeInitResponse(apData) (((struct SCdiFinDiagDataRespDc301StartFrmt *)(apData))->attrInitResp)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc301Start_SetAttributeInitResponse(apData, aResp) (((struct SCdiFinDiagDataRespDc301StartFrmt *)(apData))->attrInitResp = aResp)
																				// void (UC*, UC)
#define CCdiFinDiagDataRespDc301Start_GetDeviceControllerID(apData) (((struct SCdiFinDiagDataRespDc301StartFrmt *)(apData))->finDevContID)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc301Start_SetDeviceControllerID(apData, aID) (((struct SCdiFinDiagDataRespDc301StartFrmt *)(apData))->finDevContID = aID)
																				// void (UC*, UC)


#endif // !defined(CCdiFinDiagDataRespDc301Start_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

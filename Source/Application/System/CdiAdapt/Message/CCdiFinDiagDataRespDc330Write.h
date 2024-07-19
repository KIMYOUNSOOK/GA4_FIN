// File Name:	CCdiFinDiagDataRespDc330Write.h
// Description:	Header File of CCdiFinDiagDataRespDc330Write
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.24
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDiagDataRespDc330Write_h)
#define CCdiFinDiagDataRespDc330Write_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "ECdiFinComponentResp.h"

#include "SCdiFinDiagDataRespDc330WriteFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinDiagDataRespDc330Write_GetDiagProgram() ((US)eCdiFinDiagPrgrm_dC330)
																				// US (void)
#define CCdiFinDiagDataRespDc330Write_GetDiagCommand() ((UC)eCdiFinDiagCmd_Write)
																				// UC (void)
#define CCdiFinDiagDataRespDc330Write_GetDiagDataLength(apData) ((UC)CDIFIN_DIAG_DATA_RESP_DC330_WRITE_SIZE)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc330Write_GetChain(apData) ( ((US)(((struct SCdiFinDiagDataRespDc330WriteFrmt *)(apData))->chain_Octet0) << 8) | ((US)(((struct SCdiFinDiagDataRespDc330WriteFrmt *)(apData))->chain_Octet1)) )
																				// US (UC*)
#define CCdiFinDiagDataRespDc330Write_SetChain(apData, aChain) ((struct SCdiFinDiagDataRespDc330WriteFrmt *)(apData))->chain_Octet0 = (UC)((US)(aChain) >> 8); ((struct SCdiFinDiagDataRespDc330WriteFrmt *)(apData))->chain_Octet1 = (UC)((aChain))
																				// void (UC*, US)
#define CCdiFinDiagDataRespDc330Write_GetLink(apData) ( ((US)(((struct SCdiFinDiagDataRespDc330WriteFrmt *)(apData))->link_Octet0) << 8) | ((US)(((struct SCdiFinDiagDataRespDc330WriteFrmt *)(apData))->link_Octet1)) )
																				// US (UC*)
#define CCdiFinDiagDataRespDc330Write_SetLink(apData, aLink) ((struct SCdiFinDiagDataRespDc330WriteFrmt *)(apData))->link_Octet0 = (UC)((US)(aLink) >> 8); ((struct SCdiFinDiagDataRespDc330WriteFrmt *)(apData))->link_Octet1 = (UC)((aLink))
																				// void (UC*, US)
#define CCdiFinDiagDataRespDc330Write_GetComponentResponse(apData) (((struct SCdiFinDiagDataRespDc330WriteFrmt *)(apData))->componentResp)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc330Write_SetComponentResponse(apData, aResp) (((struct SCdiFinDiagDataRespDc330WriteFrmt *)(apData))->componentResp = aResp)
																				// void (UC*, UC)


#endif // !defined(CCdiFinDiagDataRespDc330Write_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// File Name:	CCdiFinDiagDataRespDc131Write.h
// Description:	Header File of CCdiFinDiagDataRespDc131Write
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.24
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDiagDataRespDc131Write_h)
#define CCdiFinDiagDataRespDc131Write_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "ECdiFinUpdateResp.h"

#include "SCdiFinDiagDataRespDc131WriteFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinDiagDataRespDc131Write_GetDiagProgram() ((US)eCdiFinDiagPrgrm_dC131)
																				// US (void)
#define CCdiFinDiagDataRespDc131Write_GetDiagCommand() ((UC)eCdiFinDiagCmd_Write)
																				// UC (void)
#define CCdiFinDiagDataRespDc131Write_GetDiagDataLength(apData) ((UC)CDIFIN_DIAG_DATA_RESP_DC131_WRITE_SIZE)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc131Write_GetChain(apData) ( ((US)(((struct SCdiFinDiagDataRespDc131WriteFrmt *)(apData))->chain_Octet0) << 8) | ((US)(((struct SCdiFinDiagDataRespDc131WriteFrmt *)(apData))->chain_Octet1)) )
																				// US (UC*)
#define CCdiFinDiagDataRespDc131Write_SetChain(apData, aChain) ((struct SCdiFinDiagDataRespDc131WriteFrmt *)(apData))->chain_Octet0 = (UC)((US)(aChain) >> 8); ((struct SCdiFinDiagDataRespDc131WriteFrmt *)(apData))->chain_Octet1 = (UC)((aChain))
																				// void (UC*, US)
#define CCdiFinDiagDataRespDc131Write_GetLink(apData) ( ((US)(((struct SCdiFinDiagDataRespDc131WriteFrmt *)(apData))->link_Octet0) << 8) | ((US)(((struct SCdiFinDiagDataRespDc131WriteFrmt *)(apData))->link_Octet1)) )
																				// US (UC*)
#define CCdiFinDiagDataRespDc131Write_SetLink(apData, aLink) ((struct SCdiFinDiagDataRespDc131WriteFrmt *)(apData))->link_Octet0 = (UC)((US)(aLink) >> 8); ((struct SCdiFinDiagDataRespDc131WriteFrmt *)(apData))->link_Octet1 = (UC)((aLink))
																				// void (UC*, US)													
#define CCdiFinDiagDataRespDc131Write_GetUpdateResponse(apData) (((struct SCdiFinDiagDataRespDc131WriteFrmt *)(apData))->updateResp)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc131Write_SetUpdateResponse(apData, aResp) (((struct SCdiFinDiagDataRespDc131WriteFrmt *)(apData))->updateResp = aResp)
																				// void (UC*, UC)


#endif // !defined(CCdiFinDiagDataRespDc131Write_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

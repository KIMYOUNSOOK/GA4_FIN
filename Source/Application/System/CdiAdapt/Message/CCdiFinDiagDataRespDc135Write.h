// File Name:	CCdiFinDiagDataRespDc135Write.h
// Description:	Header File of CCdiFinDiagDataRespDc135Write
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.24
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDiagDataRespDc135Write_h)
#define CCdiFinDiagDataRespDc135Write_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "SCdiFinDiagDataRespDc135WriteFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinDiagDataRespDc135Write_GetDiagProgram() ((US)eCdiFinDiagPrgrm_dC135)
																				// US (void)
#define CCdiFinDiagDataRespDc135Write_GetDiagCommand() ((UC)eCdiFinDiagCmd_Write)
																				// UC (void)
#define CCdiFinDiagDataRespDc135Write_GetDiagDataLength(apData) ((UC)CDIFIN_DIAG_DATA_RESP_DC135_WRITE_SIZE)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc135Write_GetChain(apData) ( ((US)(((struct SCdiFinDiagDataRespDc135WriteFrmt *)(apData))->chain_Octet0) << 8) | ((US)(((struct SCdiFinDiagDataRespDc135WriteFrmt *)(apData))->chain_Octet1)) )
																				// US (UC*)
#define CCdiFinDiagDataRespDc135Write_SetChain(apData, aChain) ((struct SCdiFinDiagDataRespDc135WriteFrmt *)(apData))->chain_Octet0 = (UC)((US)(aChain) >> 8); ((struct SCdiFinDiagDataRespDc135WriteFrmt *)(apData))->chain_Octet1 = (UC)((aChain))
																				// void (UC*, US)
#define CCdiFinDiagDataRespDc135Write_GetLink(apData) ( ((US)(((struct SCdiFinDiagDataRespDc135WriteFrmt *)(apData))->link_Octet0) << 8) | ((US)(((struct SCdiFinDiagDataRespDc135WriteFrmt *)(apData))->link_Octet1)) )
																				// US (UC*)
#define CCdiFinDiagDataRespDc135Write_SetLink(apData, aLink) ((struct SCdiFinDiagDataRespDc135WriteFrmt *)(apData))->link_Octet0 = (UC)((US)(aLink) >> 8); ((struct SCdiFinDiagDataRespDc135WriteFrmt *)(apData))->link_Octet1 = (UC)((aLink))
																				// void (UC*, US)


#endif // !defined(CCdiFinDiagDataRespDc135Write_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

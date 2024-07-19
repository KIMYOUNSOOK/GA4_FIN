// File Name:	CCdiFinDiagDataExecDc135Write.h
// Description:	Header File of CCdiFinDiagDataExecDc135Write
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.19
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDiagDataExecDc135Write_h)
#define CCdiFinDiagDataExecDc135Write_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "SCdiFinDiagDataExecDc135WriteFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinDiagDataExecDc135Write_GetDiagProgram() ((US)eCdiFinDiagPrgrm_dC135)
																				// US (void)
#define CCdiFinDiagDataExecDc135Write_GetDiagCommand() ((UC)eCdiFinDiagCmd_Write)
																				// UC (void)
#define CCdiFinDiagDataExecDc135Write_GetDiagDataLength(apData) ((UC)CDIFIN_DIAG_DATA_EXEC_DC135_WRITE_SIZE)
																				// UC (UC*)
#define CCdiFinDiagDataExecDc135Write_GetChain(apData) ( ((US)(((struct SCdiFinDiagDataExecDc135WriteFrmt *)(apData))->chain_Octet0) << 8) | ((US)(((struct SCdiFinDiagDataExecDc135WriteFrmt *)(apData))->chain_Octet1)) )
																				// US (UC*)
#define CCdiFinDiagDataExecDc135Write_SetChain(apData, aChain) ((struct SCdiFinDiagDataExecDc135WriteFrmt *)(apData))->chain_Octet0 = (UC)((US)(aChain) >> 8); ((struct SCdiFinDiagDataExecDc135WriteFrmt *)(apData))->chain_Octet1 = (UC)((aChain))
																				// void (UC*, US)
#define CCdiFinDiagDataExecDc135Write_GetLink(apData) ( ((US)(((struct SCdiFinDiagDataExecDc135WriteFrmt *)(apData))->link_Octet0) << 8) | ((US)(((struct SCdiFinDiagDataExecDc135WriteFrmt *)(apData))->link_Octet1)) )
																				// US (UC*)
#define CCdiFinDiagDataExecDc135Write_SetLink(apData, aLink) ((struct SCdiFinDiagDataExecDc135WriteFrmt *)(apData))->link_Octet0 = (UC)((US)(aChain) >> 8); ((struct SCdiFinDiagDataExecDc135WriteFrmt *)(apData))->link_Octet1 = (UC)((aChain))
																				// void (UC*, US)
#define CCdiFinDiagDataExecDc135Write_GetLifeSpec(apData) ( ((UL)(((struct SCdiFinDiagDataExecDc135WriteFrmt *)(apData))->lifeSpec_Octet0) << 24) | ((UL)(((struct SCdiFinDiagDataExecDc135WriteFrmt *)(apData))->lifeSpec_Octet1) << 16) | ((UL)(((struct SCdiFinDiagDataExecDc135WriteFrmt *)(apData))->lifeSpec_Octet2) << 8) | ((UL)(((struct SCdiFinDiagDataExecDc135WriteFrmt *)(apData))->lifeSpec_Octet3)) )
																				// UL (UC*)
#define CCdiFinDiagDataExecDc135Write_SetLifeSpec(apData, aLife) ((struct SCdiFinDiagDataExecDc135WriteFrmt *)(apData))->lifeSpec_Octet0 = (UC)((UL)(aLife) >> 24); ((struct SCdiFinDiagDataExecDc135WriteFrmt *)(apData))->lifeSpec_Octet1 = (UC)((UL)(aLife) >> 16); ((struct SCdiFinDiagDataExecDc135WriteFrmt *)(apData))->lifeSpec_Octet2 = (UC)((UL)(aLife) >> 8); ((struct SCdiFinDiagDataExecDc135WriteFrmt *)(apData))->lifeSpec_Octet3 = (UC)((aLife))
																				// void (UC*, UL)


#endif // !defined(CCdiFinDiagDataExecDc135Write_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// File Name:	CCdiFinDiagDataRespDc135Read.h
// Description:	Header File of CCdiFinDiagDataRespDc135Read
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.24
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDiagDataRespDc135Read_h)
#define CCdiFinDiagDataRespDc135Read_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "ECdiFinReasonExchg.h"

#include "SCdiFinDiagDataRespDc135ReadFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinDiagDataRespDc135Read_GetDiagProgram() ((US)eCdiFinDiagPrgrm_dC135)
																				// US (void)
#define CCdiFinDiagDataRespDc135Read_GetDiagCommand() ((UC)eCdiFinDiagCmd_Read)	// UC (void)
#define CCdiFinDiagDataRespDc135Read_GetDiagDataLength(apData) ((UC)CDIFIN_DIAG_DATA_RESP_DC135_READ_SIZE)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc135Read_GetChain(apData) ( ((US)(((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->chain_Octet0) << 8) | ((US)(((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->chain_Octet1)) )
																				// US (UC*)
#define CCdiFinDiagDataRespDc135Read_SetChain(apData, aChain) ((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->chain_Octet0 = (UC)((US)(aChain) >> 8); ((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->chain_Octet1 = (UC)((aChain))
																				// void (UC*, US)
#define CCdiFinDiagDataRespDc135Read_GetLink(apData) ( ((US)(((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->link_Octet0) << 8) | ((US)(((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->link_Octet1)) )
																				// US (UC*)
#define CCdiFinDiagDataRespDc135Read_SetLink(apData, aLink) ((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->link_Octet0 = (UC)((US)(aLink) >> 8); ((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->link_Octet1 = (UC)((aLink))
																				// void (UC*, US)
#define CCdiFinDiagDataRespDc135Read_GetCounterValue(apData) ( ((UL)(((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterVal_Octet0) << 24) | ((UL)(((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterVal_Octet1) << 16) | ((UL)(((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterVal_Octet2) << 8) | ((UL)(((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterVal_Octet3)) )
																				// UL (UC*)
#define CCdiFinDiagDataRespDc135Read_SetCounterValue(apData, aValue) ((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterVal_Octet0 = (UC)((UL)(aValue) >> 24); ((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterVal_Octet1 = (UC)((UL)(aValue) >> 16); ((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterVal_Octet2 = (UC)((UL)(aValue) >> 8); ((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterVal_Octet3 = (UC)((aValue))
																				// void (UC*, UL)
#define CCdiFinDiagDataRespDc135Read_GetLifeSpec(apData) ( ((UL)(((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->lifeSpec_Octet0) << 24) | ((UL)(((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->lifeSpec_Octet1) << 16) | ((UL)(((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->lifeSpec_Octet2) << 8) | ((UL)(((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->lifeSpec_Octet3)) )
																				// UL (UC*)
#define CCdiFinDiagDataRespDc135Read_SetLifeSpec(apData, aLife) ((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->lifeSpec_Octet0 = (UC)((UL)(aLife) >> 24); ((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->lifeSpec_Octet1 = (UC)((UL)(aLife) >> 16); ((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->lifeSpec_Octet2 = (UC)((UL)(aLife) >> 8); ((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->lifeSpec_Octet3 = (UC)((aLife))
																				// void (UC*, UL)
#define CCdiFinDiagDataRespDc135Read_GetReasonForLast(apData) (((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->reasonLastExchg)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc135Read_SetReasonForLast(apData, aReason) (((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->reasonLastExchg = aReason)
																				// void (UC*, UC)
#define CCdiFinDiagDataRespDc135Read_GetCounterValueAtLast(apData) ( ((UL)(((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterValAtLast_Octet0) << 24) | ((UL)(((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterValAtLast_Octet1) << 16) | ((UL)(((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterValAtLast_Octet2) << 8) | ((UL)(((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterValAtLast_Octet3)) )
																				// UL (UC*)
#define CCdiFinDiagDataRespDc135Read_SetCounterValueAtLast(apData, aValue) ((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterValAtLast_Octet0 = (UC)((UL)(aValue) >> 24); ((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterValAtLast_Octet1 = (UC)((UL)(aValue) >> 16); ((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterValAtLast_Octet2 = (UC)((UL)(aValue) >> 8); ((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterValAtLast_Octet3 = (UC)((aValue))
																				// void (UC*, UL)
#define CCdiFinDiagDataRespDc135Read_GetReasonForLastButOne(apData) (((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->reasonLastExchgButOne)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc135Read_SetReasonForLastButOne(apData, aReason) (((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->reasonLastExchgButOne = aReason)
																				// void (UC*, UC)
#define CCdiFinDiagDataRespDc135Read_GetCounterValueAtLastButOne(apData) ( ((UL)(((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterValLastButOne_Octet0) << 24) | ((UL)(((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterValLastButOne_Octet1) << 16) | ((UL)(((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterValLastButOne_Octet2) << 8) | ((UL)(((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterValLastButOne_Octet3)) )
																				// UL (UC*)
#define CCdiFinDiagDataRespDc135Read_SetCounterValueAtLastButOne(apData, aValue) ((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterValLastButOne_Octet0 = (UC)((UL)(aValue) >> 24); ((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterValLastButOne_Octet1 = (UC)((UL)(aValue) >> 16); ((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterValLastButOne_Octet2 = (UC)((UL)(aValue) >> 8); ((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterValLastButOne_Octet3 = (UC)((aValue))
																				// void (UC*, UL)
#define CCdiFinDiagDataRespDc135Read_GetReasonForLastButTwo(apData) (((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->reasonLastExchgButTwo)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc135Read_SetReasonForLastButTwo(apData, aReason) (((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->reasonLastExchgButTwo = aReason)
																				// void (UC*, UC)
#define CCdiFinDiagDataRespDc135Read_GetCounterValueAtLastButTwo(apData) ( ((UL)(((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterValLastButTwo_Octet0) << 24) | ((UL)(((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterValLastButTwo_Octet1) << 16) | ((UL)(((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterValLastButTwo_Octet2) << 8) | ((UL)(((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterValLastButTwo_Octet3)) )
																				// UL (UC*)
#define CCdiFinDiagDataRespDc135Read_SetCounterValueAtLastButTwo(apData, aValue) ((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterValLastButTwo_Octet0 = (UC)((UL)(aValue) >> 24); ((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterValLastButTwo_Octet1 = (UC)((UL)(aValue) >> 16); ((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterValLastButTwo_Octet2 = (UC)((UL)(aValue) >> 8); ((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterValLastButTwo_Octet3 = (UC)((aValue))
																				// void (UC*, UL)
#define CCdiFinDiagDataRespDc135Read_GetCounterName(apData) (((struct SCdiFinDiagDataRespDc135ReadFrmt *)(apData))->counterName)
																				// UC* (UC*)


#endif // !defined(CCdiFinDiagDataRespDc135Read_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

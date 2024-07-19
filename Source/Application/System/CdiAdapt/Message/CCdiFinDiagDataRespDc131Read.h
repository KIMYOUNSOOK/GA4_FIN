// File Name:	CCdiFinDiagDataRespDc131Read.h
// Description:	Header File of CCdiFinDiagDataRespDc131Read
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.19
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDiagDataRespDc131Read_h)
#define CCdiFinDiagDataRespDc131Read_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "ECdiFinDataType.h"
#include "ECdiFinActive.h"

#include "SCdiFinDiagDataRespDc131ReadFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinDiagDataRespDc131Read_GetDiagProgram() ((US)eCdiFinDiagPrgrm_dC131)
																				// US (void)
#define CCdiFinDiagDataRespDc131Read_GetDiagCommand() ((UC)eCdiFinDiagCmd_Read)	// UC (void)
#define CCdiFinDiagDataRespDc131Read_GetDiagDataLength(apData) ((UC)CDIFIN_DIAG_DATA_RESP_DC131_READ_SIZE)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc131Read_GetChain(apData) ( ((US)(((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->chain_Octet0) << 8) | ((US)(((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->chain_Octet1)) )
																				// US (UC*)
#define CCdiFinDiagDataRespDc131Read_SetChain(apData, aChain) ((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->chain_Octet0 = (UC)((US)(aChain) >> 8); ((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->chain_Octet1 = (UC)((aChain))
																				// void (UC*, US)
#define CCdiFinDiagDataRespDc131Read_GetLink(apData) ( ((US)(((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->link_Octet0) << 8) | ((US)(((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->link_Octet1)) )
																				// US (UC*)
#define CCdiFinDiagDataRespDc131Read_SetLink(apData, aLink) ((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->link_Octet0 = (UC)((US)(aLink) >> 8); ((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->link_Octet1 = (UC)((aLink))
																				// void (UC*, US)													
#define CCdiFinDiagDataRespDc131Read_GetDataType(apData) (((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->dataType)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc131Read_SetDataType(apData, aDataType) (((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->dataType = aDataType)
																				// void (UC*, UC)
#define CCdiFinDiagDataRespDc131Read_GetReadAllowed(apData) (((((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->attrChar) >> 0) & 0x01)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc131Read_SetReadAllowed(apData, aAllowed) ((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->attrChar &= ~(0x01 << 0); ((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->attrChar |= ((UC)(0x01 & (aAllowed)) << 0)
																				// void (UC*, UC)
#define CCdiFinDiagDataRespDc131Read_GetWriteAllowed(apData) (((((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->attrChar) >> 2) & 0x01)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc131Read_SetWriteAllowed(apData, aAllowed) ((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->attrChar &= ~(0x01 << 2); ((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->attrChar |= ((UC)(0x01 & (aAllowed)) << 2)
																				// void (UC*, UC)
#define CCdiFinDiagDataRespDc131Read_GetFactoryInitAllowed(apData) (((((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->attrChar) >> 3) & 0x01)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc131Read_SetFactoryInitAllowed(apData, aAllowed) ((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->attrChar &= ~(0x01 << 3); ((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->attrChar |= ((UC)(0x01 & (aAllowed)) << 3)
																				// void (UC*, UC)
#define CCdiFinDiagDataRespDc131Read_GetCurrentValue(apData) ( ((UL)(((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->crntVal_Octet0) << 24) | ((UL)(((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->crntVal_Octet1) << 16) | ((UL)(((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->crntVal_Octet2) << 8) | ((UL)(((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->crntVal_Octet3)) )
																				// UL (UC*)
#define CCdiFinDiagDataRespDc131Read_SetCurrentValue(apData, aValue) ((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->crntVal_Octet0 = (UC)((UL)(aValue) >> 24); ((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->crntVal_Octet1 = (UC)((UL)(aValue) >> 16); ((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->crntVal_Octet2 = (UC)((UL)(aValue) >> 8); ((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->crntVal_Octet3 = (UC)((aValue))
																				// void (UC*, UL)
#define CCdiFinDiagDataRespDc131Read_GetMinimumValue(apData) ( ((UL)(((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->minVal_Octet0) << 24) | ((UL)(((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->minVal_Octet1) << 16) | ((UL)(((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->minVal_Octet2) << 8) | ((UL)(((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->minVal_Octet3)) )
																				// UL (UC*)
#define CCdiFinDiagDataRespDc131Read_SetMinimumValue(apData, aValue) ((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->minVal_Octet0 = (UC)((UL)(aValue) >> 24); ((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->minVal_Octet1 = (UC)((UL)(aValue) >> 16); ((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->minVal_Octet2 = (UC)((UL)(aValue) >> 8); ((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->minVal_Octet3 = (UC)((aValue))
																				// void (UC*, UL)
#define CCdiFinDiagDataRespDc131Read_GetMaximumValue(apData) ( ((UL)(((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->maxVal_Octet0) << 24) | ((UL)(((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->maxVal_Octet1) << 16) | ((UL)(((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->maxVal_Octet2) << 8) | ((UL)(((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->maxVal_Octet3)) )
																				// UL (UC*)
#define CCdiFinDiagDataRespDc131Read_SetMaximumValue(apData, aValue) ((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->maxVal_Octet0 = (UC)((UL)(aValue) >> 24); ((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->maxVal_Octet1 = (UC)((UL)(aValue) >> 16); ((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->maxVal_Octet2 = (UC)((UL)(aValue) >> 8); ((struct SCdiFinDiagDataRespDc131ReadFrmt *)(apData))->maxVal_Octet3 = (UC)((aValue))
																				// void (UC*, UL)


#endif // !defined(CCdiFinDiagDataRespDc131Read_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

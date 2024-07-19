// File Name:	NvmService.h
// Description:	Header File of NVM Service
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef NvmService_h
#define NvmService_h

////////////////////////////////////////////////////////////
// Include File
////////////////////////////////////////////////////////////

#include "FinisherCommon.h"
#include "ENvmID.h"
#include "EHfsiItemID.h"


////////////////////////////////////////////////////////////
// Function Prototype Definition
////////////////////////////////////////////////////////////

UL SRVC_GetNvmValue(ENvmID id);
UC SRVC_GetNvmValueUC(ENvmID id);
SC SRVC_GetNvmValueSC(ENvmID id);
US SRVC_GetNvmValueUS(ENvmID id);
SS SRVC_GetNvmValueSS(ENvmID id);
UL SRVC_GetNvmValueUL(ENvmID id);
SL SRVC_GetNvmValueSL(ENvmID id);
void SRVC_SetNvmValue(UL value, ENvmID id);
void SRVC_SetNvmValueUC(UC value, ENvmID id);
void SRVC_SetNvmValueSC(SC value, ENvmID id);
void SRVC_SetNvmValueUS(US value, ENvmID id);
void SRVC_SetNvmValueSS(SS value, ENvmID id);
void SRVC_SetNvmValueUL(UL value, ENvmID id);
void SRVC_SetNvmValueSL(SL value, ENvmID id);
UL SRVC_GetHfsiValue(EHfsiItemID id);
void SRVC_IncrementHfsi(EHfsiItemID id);


#endif // #ifndef NvmService_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 15.03.30	| Yuichi Hoshino	| Created this file
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 00)
//

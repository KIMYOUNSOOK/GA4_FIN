// File Name:	CCdiSysOpeMode.h
// Description:	Header File of System Operating Mode
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.24
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CCdiSysOpeMode_h
#define CCdiSysOpeMode_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinSysOpeMode.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- System Operating Mode Change Event ---
typedef enum /*ECdiSysOpeModeEvt*/
{
	CDI_SYS_OPE_MODE_EVT_CHG_NRML,		// Changed To Normal
	CDI_SYS_OPE_MODE_EVT_CHG_DIAG,		// Changed To Service Diagnostics
	CDI_SYS_OPE_MODE_EVT_CHG_DL,		// Changed To Software Download
	CDI_SYS_OPE_MODE_EVT_NUM			// Number Of System Operating Mode
} ECdiSysOpeModeEvt;

// --- System Operating Mode ---
typedef struct /* CCdiSysOpeMode */
{
	UC mMode;
} CCdiSysOpeMode;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CCdiSysOpeMode_Constructor(CCdiSysOpeMode* pThis);

UC CCdiSysOpeMode_GetMode(CCdiSysOpeMode* pThis);
bool CCdiSysOpeMode_InformedEvent(CCdiSysOpeMode* pThis, ECdiSysOpeModeEvt event);


#endif // #ifndef CCdiSysOpeMode_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

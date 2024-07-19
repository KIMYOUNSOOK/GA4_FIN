// File Name:	CCdiProductionState.h
// Description:	Header File of Production State
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.24
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CCdiProductionState_h
#define CCdiProductionState_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinPrdctStt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Const Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Production State Change Event ---
typedef enum /*ECdiProductSttEvt*/
{
	CDI_PRDCT_STT_EVT_INIT_STA,			// Start Initialization Message Sequence(Power-Up Sequence)
	CDI_PRDCT_STT_EVT_INIT_COMP,		// Complete Initialization Message Sequence(Power-Up Sequence)
	CDI_PRDCT_STT_EVT_CYC_UP_STA,		// Start Cycle Up Sequence
	CDI_PRDCT_STT_EVT_DOWN_COMP,		// Complete Down Sequence
	CDI_PRDCT_STT_EVT_NUM				// Number Of Production State Event
} ECdiProductSttEvt;


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- System Operating Mode ---
typedef struct /* CCdiProductionState */
{
	UC mState;
} CCdiProductionState;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CCdiProductionState_Constructor(CCdiProductionState* pThis);

UC CCdiProductionState_GetState(CCdiProductionState* pThis);
bool CCdiProductionState_InformedEvent(CCdiProductionState* pThis, ECdiProductSttEvt event);


#endif // #ifndef CCdiProductionState_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

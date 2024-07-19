// File Name:	CCdiTransitionState.h
// Description:	Header File of Transition State
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.24
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CCdiTransitionState_h
#define CCdiTransitionState_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinTransStt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Transition State Change Event ---
typedef enum /*ECdiTransSttEvt*/
{
	CDI_TRANS_STT_EVT_CYC_UP_STA,		// Start Cycle Up Sequence
	CDI_TRANS_STT_EVT_CYC_UP_COMP,		// Start Cycle Up Sequence
	CDI_TRANS_STT_EVT_CYC_DOWN_STA,		// Start Cycle Down Sequence
	CDI_TRANS_STT_EVT_HARD_DOWN_STA,	// Start Hard Down Sequence
	CDI_TRANS_STT_EVT_DOWN_COMP,		// Complete Down Sequence
	CDI_TRANS_STT_EVT_NUM				// Number Of Transition State Event
} ECdiTransSttEvt;

// --- Transition State ---
typedef struct /* CCdiTransitionState */
{
	UC mState;
} CCdiTransitionState;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CCdiTransitionState_Constructor(CCdiTransitionState* pThis);

UC CCdiTransitionState_GetState(CCdiTransitionState* pThis);
bool CCdiTransitionState_InformedEvent(CCdiTransitionState* pThis, ECdiTransSttEvt event);


#endif // #ifndef CCdiTransitionState_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

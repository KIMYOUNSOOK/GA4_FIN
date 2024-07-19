// File Name:	CCvrCover.h
// Description:	Header File of Cover
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CCvrCover_h
#define CCvrCover_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "DD_DIO.h"
#include "ECvrCoverStatus.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Cover Spec ---
typedef struct /*CvrCoverSpec*/
{
	UC inputID;
	UC saftyInterlock;
	UC detectCount;
} CvrCoverSpec;

// --- Cover Member ---
typedef struct /*CCvrCover*/
{
	const CvrCoverSpec* mpSpec;
	UC mState;
	UC mCount;
} CCvrCover;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CCvrCover_Constructor(CCvrCover* pThis, const CvrCoverSpec* pSpec);

DD_DIO_InputPortID CCvrCover_GetInputID(CCvrCover* pThis);
bool CCvrCover_IsSafetyInterlock(CCvrCover* pThis);
ECvrCoverStatus CCvrCover_GetStatus(CCvrCover* pThis);
bool CCvrCover_Initialize(CCvrCover* pThis);
bool CCvrCover_NotifyEdge(CCvrCover* pThis, UC edge);
bool CCvrCover_CloseCheck(CCvrCover* pThis);
bool CCvrCover_DesignateStatus(CCvrCover* pThis, ECvrCoverStatus status);


#endif // #ifndef CCvrCover_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.04.11	| Yuichi Hoshino	| Created this file
// 14.12.04	| Yuichi Hoshino	| Release V0 (Revision 00)
// 14.12.29	| Yuichi Hoshino	| Design Improvement [ 強制ステータス変更処理見直し ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//

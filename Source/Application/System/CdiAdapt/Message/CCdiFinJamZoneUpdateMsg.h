// File Name:	CCdiFinJamZoneUpdateMsg.h
// Description:	Header File of Jam Zone Update Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.22
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinJamZoneUpdateMsg_h)
#define CCdiFinJamZoneUpdateMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "SCdiFinJamZoneUpdateFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Jam Zone Update Message ---
typedef struct /*CCdiFinJamZoneUpdateMsg*/
{
	UC*	mpMsg;
	US	mSize;
} CCdiFinJamZoneUpdateMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void CCdiFinJamZoneUpdateMsg_Constructor(CCdiFinJamZoneUpdateMsg* pThis)
{
	pThis->mpMsg = NULL;
	pThis->mSize = 0;
}

#define CCdiFinJamZoneUpdateMsg_GetID(apThis) ((UC)eCdiFinMsgID_JamZoneUpdate)		// UC (CCdiFinJamZoneUpdateMsg*)
#define CCdiFinJamZoneUpdateMsg_GetSize(apThis) ((apThis)->mSize)					// US (CCdiFinJamZoneUpdateMsg*)
#define CCdiFinJamZoneUpdateMsg_GetMessage(apThis) ((apThis)->mpMsg)				// UC* (CCdiFinJamZoneUpdateMsg*)
void CCdiFinJamZoneUpdateMsg_SetMessage(CCdiFinJamZoneUpdateMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;
	pThis->mSize = size;

	*(pThis->mpMsg) = eCdiFinMsgID_JamZoneUpdate;
}

#define CCdiFinJamZoneUpdateMsg_GetJamZoneStatusSize(apThis) ((UC)((apThis)->mSize - CDIFIN_JAM_ZONE_UPDT_HEAD_SIZE))
																					// UC (CCdiFinJamZoneUpdateMsg*)
void CCdiFinJamZoneUpdateMsg_SetJamZoneStatusSize(CCdiFinJamZoneUpdateMsg* pThis, UC size)
{
	UC index;

	pThis->mSize = CDIFIN_JAM_ZONE_UPDT_HEAD_SIZE + size;

	// --- 0 Clear Jam Zone Status ---
	for ( index = CDIFIN_JAM_ZONE_UPDT_HEAD_SIZE; index < pThis->mSize; index++ ){
		pThis->mpMsg[index] = 0x00;
	}
}

#define CCdiFinJamZoneUpdateMsg_GetFinDevContID(apThis) ((UC)((struct SCdiFinJamZoneUpdateFrmt*)((apThis)->mpMsg))->finDevContID)
																					// UC (CCdiFinJamZoneUpdateMsg*)
#define CCdiFinJamZoneUpdateMsg_SetFinDevContID(apThis, aFinDevContID) (((struct SCdiFinJamZoneUpdateFrmt*)((apThis)->mpMsg))->finDevContID = aFinDevContID)
																					 // void (CCdiFinJamZoneUpdateMsg*)
//#define CCdiFinJamZoneUpdateMsg_GetJamZoneStatus(apThis) ((UC*)((struct SCdiFinJamZoneUpdateFrmt*)((apThis)->mpMsg))->jamZone)
//																					// UC* (CCdiFinJamZoneUpdateMsg*)
#define CCdiFinJamZoneUpdateMsg_GetJamZoneStatus(apThis, aOctet, aBit) ((UC)((((struct SCdiFinJamZoneUpdateFrmt*)((apThis)->mpMsg))->jamZone[(aOctet)] >> (aBit)) & 0x01))
																					// UC (CCdiFinJamZoneUpdateMsg*, UC, UC)
#define CCdiFinJamZoneUpdateMsg_SetJamZoneStatus(apThis, aStatus, aOctet, aBit) (((struct SCdiFinJamZoneUpdateFrmt*)((apThis)->mpMsg))->jamZone[(aOctet)] = (((((struct SCdiFinJamZoneUpdateFrmt*)((apThis)->mpMsg))->jamZone[(aOctet)] & ~((UC)0x01 << (aBit)))) | (((UC)(aStatus) & 0x01) << (aBit))))
//																					// void (CCdiFinJamZoneUpdateMsg*, UC, UC, UC)


#endif // !defined(CCdiFinJamZoneUpdateMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

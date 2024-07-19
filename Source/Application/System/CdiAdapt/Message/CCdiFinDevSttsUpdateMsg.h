// File Name:	CCdiFinDevSttsUpdateMsg.h
// Description:	Header File of Device Status Update Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.22
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDevSttsUpdateMsg_h)
#define CCdiFinDevSttsUpdateMsg_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "SCdiFinDevSttsUpdateFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Device Status Update Message ---
typedef struct /*CCdiFinDevSttsUpdateMsg*/
{
	UC*	mpMsg;
} CCdiFinDevSttsUpdateMsg;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void CCdiFinDevSttsUpdateMsg_Constructor(CCdiFinDevSttsUpdateMsg* pThis)
{
	pThis->mpMsg = NULL;
}

#define CCdiFinDevSttsUpdateMsg_GetID(apThis) ((UC)eCdiFinMsgID_DevSttsUpdate)	// UC (CCdiFinDevSttsUpdateMsg*)
#define CCdiFinDevSttsUpdateMsg_GetSize(apThis) ((US)CDIFIN_DEV_STTS_UPDATE_MSG_SIZE)
																				// US (CCdiFinDevSttsUpdateMsg*)
#define CCdiFinDevSttsUpdateMsg_GetMessage(apThis) ((apThis)->mpMsg)			// UC* (CCdiFinDevSttsUpdateMsg*)
void CCdiFinDevSttsUpdateMsg_SetMessage(CCdiFinDevSttsUpdateMsg* pThis, UC* pMsg, US size)
{
	pThis->mpMsg = pMsg;

	*(pThis->mpMsg) = eCdiFinMsgID_DevSttsUpdate;
}

US CCdiFinDevSttsUpdateMsg_GetStatusID(CCdiFinDevSttsUpdateMsg* pThis)
{
	return CF2_US(&(((struct SCdiFinDevSttsUpdateFrmt*)(pThis->mpMsg))->statusID_Octet0));
}

void CCdiFinDevSttsUpdateMsg_SetStatusID(CCdiFinDevSttsUpdateMsg* pThis, US statusID)
{
	US_CF2(statusID, &(((struct SCdiFinDevSttsUpdateFrmt*)(pThis->mpMsg))->statusID_Octet0));
}

US CCdiFinDevSttsUpdateMsg_GetStatusValue(CCdiFinDevSttsUpdateMsg* pThis)
{
	return CF2_US(&(((struct SCdiFinDevSttsUpdateFrmt*)(pThis->mpMsg))->sttsVal_Octet0));
}

void CCdiFinDevSttsUpdateMsg_SetStatusValue(CCdiFinDevSttsUpdateMsg* pThis, US statusValue)
{
	US_CF2(statusValue, &(((struct SCdiFinDevSttsUpdateFrmt*)((pThis)->mpMsg))->sttsVal_Octet0));
}

US CCdiFinDevSttsUpdateMsg_GetChain(CCdiFinDevSttsUpdateMsg* pThis)
{
	return CF2_US(&(((struct SCdiFinDevSttsUpdateFrmt*)((pThis)->mpMsg))->chain_Octet0));
}

void CCdiFinDevSttsUpdateMsg_SetChain(CCdiFinDevSttsUpdateMsg* pThis, US chain)
{
	US_CF2(chain, &(((struct SCdiFinDevSttsUpdateFrmt*)((pThis)->mpMsg))->chain_Octet0));
}

US CCdiFinDevSttsUpdateMsg_GetLink(CCdiFinDevSttsUpdateMsg* pThis)
{
	return CF2_US(&(((struct SCdiFinDevSttsUpdateFrmt*)((pThis)->mpMsg))->link_Octet0));
}

void CCdiFinDevSttsUpdateMsg_SetLink(CCdiFinDevSttsUpdateMsg* pThis, US link){
	US_CF2(link, &(((struct SCdiFinDevSttsUpdateFrmt*)((pThis)->mpMsg))->link_Octet0));
}


#endif // !defined(CCdiFinDevSttsUpdateMsg_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

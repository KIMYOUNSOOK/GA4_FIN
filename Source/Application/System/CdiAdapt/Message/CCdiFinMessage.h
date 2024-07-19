// File Name:	CCdiFinMessage.h
// Description:	Header File of CDI Finisher Message
// Engineer:	Yuichi Hoshino
// Last Edit:	14.02.08
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinMessage_h)
#define CCdiFinMessage_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"
#include "ECdiFinMsgId.h"

/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////
#define CDIFIN_NOTUSED					0
#define CDIFIN_SPARE					0
#define CDIFIN_RESERVED					0
#define CDIFIN_DONTCARE					0

#define CDIFIN_MAXMSGSIZE				256


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

typedef struct /*CCdiFinMessage*/ {
	UC* mpMessage;
	US mSize;
} CCdiFinMessage;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void CCdiFinMessage_Constructor(CCdiFinMessage* pThis);

UC CCdiFinMessage_GetID(CCdiFinMessage* pThis);
US CCdiFinMessage_GetSize(CCdiFinMessage* pThis);
UC* CCdiFinMessage_GetMessage(CCdiFinMessage* pThis);
void CCdiFinMessage_SetMessage(CCdiFinMessage* pThis, const UC* pMessage, US msgSize);


#endif // !defined(CCdiFinMessage_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

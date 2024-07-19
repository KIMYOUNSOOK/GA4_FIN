/////////////////////////////////////////////////////////////////////////////////////
//  SCdiFinDevPauseReqFrmt.h
//  Implementation of the Class SCdiFinDevPauseReqFrmt
//  Created on:      2012/11/22 16:13:23
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(SCdiFinDevPauseReqFrmt_h)
#define SCdiFinDevPauseReqFrmt_h

#define CDIFIN_DEV_PAUSE_REQ_MSG_SIZE	4


struct SCdiFinDevPauseReqFrmt
{
	UC id;								// Message ID
	UC reqStts;							// Request Status
	UC pauseReq;						// Pause Request
	UC sheetID;							// Sheet ID
};

#endif // !defined(SCdiFinDevPauseReqFrmt_h)

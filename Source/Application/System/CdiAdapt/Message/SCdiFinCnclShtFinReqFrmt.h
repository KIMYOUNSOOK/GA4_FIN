/////////////////////////////////////////////////////////////////////////////////////
//  SCdiFinCnclShtFinReqFrmt.h
//  Implementation of the Class SCdiFinCnclShtFinReqFrmt
//  Created on:      2012/11/22 15:56:38
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(SCdiFinCnclShtFinReqFrmt_h)
#define SCdiFinCnclShtFinReqFrmt_h

#define CDIFIN_CANCEL_SHT_FIN_REQ_MSG_SIZE					3


struct SCdiFinCnclShtFinReqFrmt
{
	UC id;								// Message ID
	UC setID;							// Set ID
	UC sheetID;							// Sheet ID
};

#endif // !defined(SCdiFinCnclShtFinReqFrmt_h)

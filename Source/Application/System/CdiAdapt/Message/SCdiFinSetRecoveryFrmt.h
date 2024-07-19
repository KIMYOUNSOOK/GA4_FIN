/////////////////////////////////////////////////////////////////////////////////////
//  SCdiFinSetRecoveryFrmt.h
//  Implementation of the Class SCdiFinSetRecoveryFrmt
//  Created on:      2012/11/22 15:29:19
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(SCdiFinSetRecoveryFrmt_h)
#define SCdiFinSetRecoveryFrmt_h

#define CDIFIN_SET_RECOVERY_MSG_SIZE	2


struct SCdiFinSetRecoveryFrmt
{
	UC id;								// Message ID
	UC setID;							// Set ID
};

#endif // !defined(SCdiFinSetRecoveryFrmt_h)

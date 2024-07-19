/////////////////////////////////////////////////////////////////////////////////////
//  SCdiFinJamSttsCheckFrmt.h
//  Implementation of the Class SCdiFinJamSttsCheckFrmt
//  Created on:      2012/11/22 16:02:06
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(SCdiFinJamSttsCheckFrmt_h)
#define SCdiFinJamSttsCheckFrmt_h

#define CDIFIN_JAM_STTS_CHECK_MSG_SIZE	2


struct SCdiFinJamSttsCheckFrmt
{
	UC id;								// Message ID
	UC trigger;							// Check Trigger
};

#endif // !defined(SCdiFinJamSttsCheckFrmt_h)

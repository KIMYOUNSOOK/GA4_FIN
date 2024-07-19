/////////////////////////////////////////////////////////////////////////////////////
//  SCdiFinAbnormalFinishingFrmt.h
//  Implementation of the Class SCdiFinAbnormalFinishingFrmt
//  Created on:      2013/05/28 16:00:34
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(SCdiFinAbnormalFinishingFrmt_h)
#define SCdiFinAbnormalFinishingFrmt_h

#define CDIFIN_ABNORMAL_FINISHING_MSG_SIZE					3					// [Å~1Byte]


struct SCdiFinAbnormalFinishingFrmt
{
	UC id;								// Message ID
	UC purgePossibility;				// Purge Possibility
	UC abortSheetID;					// Abort Sheet ID
};

#endif // !defined(SCdiFinAbnormalFinishingFrmt_h)

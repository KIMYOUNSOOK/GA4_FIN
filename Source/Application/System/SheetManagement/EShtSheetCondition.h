// File Name:	EShtSheetCondition.h
// Description:	Implementation of the Enumeration EShtSheetCondition
// Engineer:	Xxxxxx Xxxxxx
// Last Edit:	XX.XX.XX
// Revision:	XX
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef EShtSheetCondition_h
#define EShtSheetCondition_h

typedef enum /*EShtSheetCondition*/
{
	SHT_COND_VOID,						// Void,
	SHT_COND_CANDIDATE,					// Candidate,
	SHT_COND_DELIVERY,					// Delivery,
	SHT_COND_PURGE,						// Purge,
	SHT_COND_EJECT,						// Eject,
	SHT_COND_COMPILE,					// Compile,
	SHT_COND_CANCEL,					// Cancel,
	SHT_COND_LEAVE,						// Leave,
	SHT_COND_JAM,						// Jam,
	SHT_COND_REMAIN,					// Remain,
	SHT_COND_COMPLETE,					// Complete
	SHT_COND_REJECT,					// Reject,
	SHT_COND_NUM,						// Number of Sheet Condition
} EShtSheetCondition;

#endif // #ifndef EShtSheetCondition_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

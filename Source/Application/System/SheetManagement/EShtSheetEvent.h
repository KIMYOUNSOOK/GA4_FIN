// File Name:	EShtSheetEvent.h
// Description:	Implementation of the Enumeration EShtSheetEvent
// Engineer:	Xxxxxx Xxxxxx
// Last Edit:	XX.XX.XX
// Revision:	XX
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(EShtSheetEvent_h)
#define EShtSheetEvent_h

//==============================================================================
// Class Name    : Sheet Management::EShtSheetEvent
// Description   : Sheet Event ID
//==============================================================================
typedef enum /*EShtSheetEvent*/
{
	SHT_EVT_PNCH_BACK_START,
	SHT_EVT_PNCH_BACK_COMP,
	SHT_EVT_PNCH_FEED,
	SHT_EVT_EJECT_START,
	SHT_EVT_FINISH_START,
	SHT_EVT_COMPILE_FINISH_COMP,
	SHT_EVT_EJECT_FINISH_COMP,
	SHT_EVT_NUM	// Number of Sheet Event ID
} EShtSheetEvent;
#endif // !defined(EShtSheetEvent_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

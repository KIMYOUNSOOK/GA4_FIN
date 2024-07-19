// File Name:	EShtSheetEventID.h
// Description:	Implementation of the Enumeration EShtSheetEventID
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef EShtSheetEventID_h
#define EShtSheetEventID_h

typedef enum /*EShtSheetEventID*/
{
	SHT_EVT_ID_EJECT_FINISH_START,
	SHT_EVT_ID_EJECT_FINISH_COMP,
	SHT_EVT_ID_EJECT_START,
	SHT_EVT_ID_COMPILE_FINISH_COMP,
	// !!! Product Specific !!!
	// SHT_EVT_ID_PNCH_BACK_START,
	// SHT_EVT_ID_PNCH_BACK_COMP,
	SHT_EVT_ID_EJECT_COMP,
	SHT_EVT_ID_NUM	// Number of Sheet Event ID
} EShtSheetEventID;

#endif // #ifndef EShtSheetEventID_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

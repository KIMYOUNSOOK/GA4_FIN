// File Name:	EShtOffsetFinish.h
// Description:	Implementation of the Enumeration EShtOffsetFinish
// Engineer:	Xxxxxx Xxxxxx
// Last Edit:	XX.XX.XX
// Revision:	XX
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef EShtOffsetFinish_h
#define EShtOffsetFinish_h

typedef enum /*EShtOffsetFinish*/
{
	SHT_OFST_FIN_NOT,			// Not Offset
	SHT_OFST_FIN_LEFT, 		// LEFT Offset
	SHT_OFST_FIN_RIGHT,		// Right Offset
	SHT_OFST_FIN_CENTER_F,		// Center Offset(from Front)
	SHT_OFST_FIN_CENTER_R,		// Center Offset(from Rear)
	SHT_OFST_FIN_NUM
} EShtOffsetFinish;

#endif // #ifndef EShtOffsetFinish_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

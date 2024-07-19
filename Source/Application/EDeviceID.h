// File Name:	EDeviceID.h
// Description:	Definition of Device ID
// Engineer:	Xxxxx Xxxxx
// Last Edit:	XX.XX.XX
// Revision:	XX
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef EDeviceID_h
#define EDeviceID_h

typedef enum /*EDeviceID*/
{
	DEVID_FINISHER,						// Finisher
	// !!! Product Specific !!!
	DEVID_OUT_TRAY,						// Output Tray
	DEVID_STAPLER,						// Stapler Unit
	// DEVID_PUNCHER,						// Puncher Unit

	DEVID_NUM							// Number of Device ID
} EDeviceID;

#endif // #ifndef EDeviceID_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

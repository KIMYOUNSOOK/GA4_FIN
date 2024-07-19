///////////////////////////////////////////////////////////
//  SCdiFinChgDevContModeFrmt.h
//  Implementation of the Class SCdiFinChgDevContModeFrmt
//  Created on:      2013/12/06 17:15:59
//  Original author: fxat00694
///////////////////////////////////////////////////////////

#if !defined(SCdiFinChgDevContModeFrmt_h)
#define SCdiFinChgDevContModeFrmt_h

#define CDIFIN_CHG_DEV_CONT_MODE_MSG_SIZE					2					// [Å~1Byte]


struct SCdiFinChgDevContModeFrmt
{
	UC id;								// Message ID
	UC devContMode;						// Device Controller Mode - System Operating Mode (Bit0 Å` Bit3)
};

#endif // !defined(SCdiFinChgDevContModeFrmt_h)

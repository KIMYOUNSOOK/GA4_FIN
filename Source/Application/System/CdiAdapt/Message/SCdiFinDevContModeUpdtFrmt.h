///////////////////////////////////////////////////////////
//  SCdiFinDevContModeUpdtFrmt.h
//  Implementation of the Class SCdiFinDevContModeUpdtFrmt
//  Created on:      2013/12/06 17:28:02
//  Original author: fxat00694
///////////////////////////////////////////////////////////

#if !defined(SCdiFinDevContModeUpdtFrmt_h)
#define SCdiFinDevContModeUpdtFrmt_h

#define CDIFIN_DEV_CONT_MODE_UPDT_MSG_SIZE					2


struct SCdiFinDevContModeUpdtFrmt
{
	UC id;								// Message ID
	UC devContMode;						// Device Controller Mode - System Operating Mode (Bit0 Å` Bit3)
};

#endif // !defined(SCdiFinDevContModeUpdtFrmt_h)

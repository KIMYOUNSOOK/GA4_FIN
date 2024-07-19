///////////////////////////////////////////////////////////
//  SCdiFinSheetExitFrmt.h
//  Implementation of the Class SCdiFinSheetExitFrmt
//  Created on:      2013/12/06 16:15:03
//  Original author: fxat00694
///////////////////////////////////////////////////////////

#if !defined(SCdiFinSheetExitFrmt_h)
#define SCdiFinSheetExitFrmt_h

#define CDIFIN_SHT_EXIT_MSG_SIZE		3


struct SCdiFinSheetExitFrmt
{
	UC id;								// Message ID
	UC setID;							// Set ID
	UC sheetID;							// Sheet ID
};

#endif // !defined(SCdiFinSheetExitFrmt_h)

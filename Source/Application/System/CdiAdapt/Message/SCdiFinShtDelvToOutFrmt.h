///////////////////////////////////////////////////////////
//  SCdiFinShtDelvToOutFrmt.h
//  Implementation of the Class SCdiFinShtDelvToOutFrmt
//  Created on:      2013/12/06 16:22:07
//  Original author: fxat00694
///////////////////////////////////////////////////////////

#if !defined(SCdiFinShtDelvToOutFrmt_h)
#define SCdiFinShtDelvToOutFrmt_h

#define CDIFIN_SHT_DELV_TO_OUT_MSG_SIZE	7


struct SCdiFinShtDelvToOutFrmt
{
	UC id;								// Message ID
	UC setID;							// Set ID
	UC sheetID;							// Sheet ID
	UC destID;							// Output Destination ID
	UC binSlct;							// Bin Selection
	UC integrity;						// Sheet Integrity
	UC offsetPos;						// Offset Position
};

#endif // !defined(SCdiFinShtDelvToOutFrmt_h)

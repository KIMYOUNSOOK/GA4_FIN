///////////////////////////////////////////////////////////
//  SCdiFinSetFinAtOutFrmt.h
//  Implementation of the Class SCdiFinSetFinAtOutFrmt
//  Created on:      2013/12/06 16:41:48
//  Original author: fxat00694
///////////////////////////////////////////////////////////

#if !defined(SCdiFinSetFinAtOutFrmt_h)
#define SCdiFinSetFinAtOutFrmt_h

#define CDIFIN_SET_FIN_AT_OUT_MSG_SIZE	5


struct SCdiFinSetFinAtOutFrmt
{
	UC id;								// Message ID
	UC setID;							// Set ID
	UC destID;							// Output Destination ID
	UC binSlct;							// Bin Selection
	UC integrity;						// Set Integrity
};

#endif // !defined(SCdiFinSetFinAtOutFrmt_h)

///////////////////////////////////////////////////////////
//  SCdiFinSetFinReqFrmt.h
//  Implementation of the Class SCdiFinSetFinReqFrmt
//  Created on:      2013/12/06 16:28:55
//  Original author: fxat00694
///////////////////////////////////////////////////////////

#if !defined(SCdiFinSetFinReqFrmt_h)
#define SCdiFinSetFinReqFrmt_h

#define CDIFIN_SET_FIN_REQ_MSG_SIZE		5


struct SCdiFinSetFinReqFrmt
{
	UC id;								// Message ID
	UC destID;							// Output Destination ID
	UC binSlct;							// Bin Selection
	UC setID;							// Set ID
	UC setEndAct;						// Set End Action
};

#endif // !defined(SCdiFinSetFinReqFrmt_h)

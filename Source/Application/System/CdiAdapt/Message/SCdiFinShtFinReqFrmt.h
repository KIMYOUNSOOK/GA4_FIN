///////////////////////////////////////////////////////////
//  SCdiFinShtFinReqFrmt.h
//  Implementation of the Class SCdiFinShtFinReqFrmt
//  Created on:      2013/12/06 15:37:10
//  Original author: fxat00694
///////////////////////////////////////////////////////////

#if !defined(SCdiFinShtFinReqFrmt_h)
#define SCdiFinShtFinReqFrmt_h

//#define CDIFIN_SHT_FIN_REQ_MSG_SIZE	// ‰Â•Ï
#define CDIFIN_SHT_FIN_REQ_MIN_SIZE		6
#define CDIFIN_SHT_FIN_REQ_ATTR_SIZE	(CDIFIN_MAXMSGSIZE - CDIFIN_SHT_FIN_REQ_MIN_SIZE)


struct SCdiFinShtFinReqFrmt
{
	UC id;													// Message ID
	UC destID;												// Output Destination ID
	UC binSlct;												// Bin Selection
	UC setID;												// Set ID
	UC sheetID;												// Sheet ID
	UC numAttr;												// Num Attr Pairs
	UC attrPair[CDIFIN_SHT_FIN_REQ_ATTR_SIZE];				// Attr Pair
};

#endif // !defined(SCdiFinShtFinReqFrmt_h)

///////////////////////////////////////////////////////////
//  SCdiFinChgShtFinReqFrmt.h
//  Implementation of the Class SCdiFinChgShtFinReqFrmt
//  Created on:      2013/12/06 15:51:51
//  Original author: fxat00694
///////////////////////////////////////////////////////////

#if !defined(SCdiFinChgShtFinReqFrmt_h)
#define SCdiFinChgShtFinReqFrmt_h

//#define CDIFIN_CHG_SHT_REQ_MSG_SIZE	// ‰Â•Ï
#define CDIFIN_CHG_SHT_REQ_MIN_SIZE		4
#define CDIFIN_CHG_SHT_REQ_ATTR_SIZE	(CDIFIN_MAXMSGSIZE - CDIFIN_CHG_SHT_REQ_MIN_SIZE)


struct SCdiFinChgShtFinReqFrmt
{
	UC id;													// Message ID
	UC setID;												// Set ID
	UC sheetID;												// Sheet ID
	UC numAttr;												// Num Attr Pairs
	UC attrPair[CDIFIN_CHG_SHT_REQ_ATTR_SIZE];				// Attr Pair
};

#endif // !defined(SCdiFinChgShtFinReqFrmt_h)

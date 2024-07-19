///////////////////////////////////////////////////////////
//  SCdiFinSlctFinAttrFrmt.h
//  Implementation of the Class SCdiFinSlctFinAttrFrmt
//  Created on:      2013/12/04 15:36:43
//  Original author: fxat00694
///////////////////////////////////////////////////////////

#if !defined(SCdiFinSlctFinAttrFrmt_h)
#define SCdiFinSlctFinAttrFrmt_h

//#define CDIFIN_SLCT_FIN_ATTR_MSG_SIZE	// ‰Â•Ï
#define CDIFIN_SLCT_FIN_ATTR_MIN_SIZE	4
#define CDIFIN_SLCT_FIN_ATTR_ATTR_SIZE	(CDIFIN_MAXMSGSIZE - CDIFIN_SLCT_FIN_ATTR_MIN_SIZE)

/**
 * Select Finisher Attribute(20H) Message Format
 */
struct SCdiFinSlctFinAttrFrmt
{
	UC id;													// Message ID
	UC destID;												// Output Destination ID
	UC binSlct;												// Bin Selection
	UC numAttr;												// Num Attr Pairs
	UC attrPair[CDIFIN_SLCT_FIN_ATTR_ATTR_SIZE];				// Attr Pair
};
#endif // !defined(SCdiFinSlctFinAttrFrmt_h)

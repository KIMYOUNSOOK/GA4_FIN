///////////////////////////////////////////////////////////
//  SCdiFinDevCfgUpdtFrmt.h
//  Implementation of the Class SCdiFinDevCfgUpdtFrmt
//  Created on:      2013/12/04 11:36:41
//  Original author: fxat00694
///////////////////////////////////////////////////////////

#if !defined(SCdiFinDevCfgUpdtFrmt_h)
#define SCdiFinDevCfgUpdtFrmt_h

//#define CDIFIN_DEV_CFG_UPDT_MSG_SIZE	// ‰Â•Ï
#define CDIFIN_DEVCFGUPDTMINSIZE		4
#define CDIFIN_DEVCFGUPDTATTRSIZE		(CDIFIN_MAXMSGSIZE - CDIFIN_DEVCFGUPDTMINSIZE)


struct SCdiFinDevCfgUpdtFrmt
{
	UC id;													// Message ID
	UC subsystem;											// Subsystem
	UC instance;											// Instance
	UC numAttr;												// Num Attr Pairs
	UC attrPair[CDIFIN_DEVCFGUPDTATTRSIZE];					// Attr Pair
};

#endif // !defined(SCdiFinDevCfgUpdtFrmt_h)

///////////////////////////////////////////////////////////
//  SCdiFinSetDevCfgFrmt.h
//  Implementation of the Class SCdiFinSetDevCfgFrmt
//  Created on:      2013/12/04 11:47:06
//  Original author: fxat00694
///////////////////////////////////////////////////////////

#if !defined(SCdiFinSetDevCfgFrmt_h)
#define SCdiFinSetDevCfgFrmt_h

//#define CDIFIN_SET_DEV_CFG_MSG_SIZE		// ‰Â•Ï
#define CDIFIN_SET_DEV_CFG_MIN_SIZE				4
#define CDIFIN_SET_DEV_CFG_ATTR_SIZE			(CDIFIN_MAXMSGSIZE - CDIFIN_SET_DEV_CFG_MIN_SIZE)


struct SCdiFinSetDevCfgFrmt
{
	UC id;													// Message ID
	UC subsystem;											// Subsystem
	UC instance;											// Instance
	UC numAttr;												// Num Attr Pairs
	UC attrPair[CDIFIN_SET_DEV_CFG_ATTR_SIZE];					// Attr Pair
};

#endif // !defined(SCdiFinSetDevCfgFrmt_h)

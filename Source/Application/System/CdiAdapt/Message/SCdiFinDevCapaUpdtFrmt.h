///////////////////////////////////////////////////////////
//  SCdiFinDevCapaUpdtFrmt.h
//  Implementation of the Class SCdiFinDevCapaUpdtFrmt
//  Created on:      2013/12/06 17:15:59
//  Original author: fxat00694
///////////////////////////////////////////////////////////

#if !defined(SCdiFinDevCapaUpdtFrmt_h)
#define SCdiFinDevCapaUpdtFrmt_h

//#define CDIFIN_DEV_CAPA_UPDT_MSG_SIZE	// ‰Â•Ï
#define CDIFIN_DEV_CAPA_UPDT_MIN_SIZE	4
#define CDIFIN_DEV_CAPA_UPDT_ATTR_SIZE	(CDIFIN_MAXMSGSIZE - CDIFIN_DEV_CAPA_UPDT_MIN_SIZE)


struct SCdiFinDevCapaUpdtFrmt
{
	UC id;													// Message ID
	UC subsystem;											// Subsystem
	UC instance;											// Instance
	UC numAttr;												// Num Of Attr Pairs
	UC attrPair[CDIFIN_DEV_CAPA_UPDT_ATTR_SIZE];			// Attr Pair
};

#endif // !defined(SCdiFinDevCapaUpdtFrmt_h)

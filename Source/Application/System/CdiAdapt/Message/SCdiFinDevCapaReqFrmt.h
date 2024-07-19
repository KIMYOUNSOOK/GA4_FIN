///////////////////////////////////////////////////////////
//  SCdiFinDevCapaReqFrmt.h
//  Implementation of the Class SCdiFinDevCapaReqFrmt
//  Created on:      2013/12/04 11:00:55
//  Original author: fxat00694
///////////////////////////////////////////////////////////

#if !defined(SCdiFinDevCapaReqFrmt_h)
#define SCdiFinDevCapaReqFrmt_h

#define CDIFIN_DEV_CAPA_REQ_MSG_SIZE	4


struct SCdiFinDevCapaReqFrmt
{
	UC id;					// Message ID
	UC subsystem;			// Subsystem
	UC instance;			// Instance
	UC attrKey;				// Attr Key
};

#endif // !defined(SCdiFinDevCapaReqFrmt_h)

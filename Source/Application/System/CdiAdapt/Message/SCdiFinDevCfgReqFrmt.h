///////////////////////////////////////////////////////////
//  SCdiFinDevCfgReqFrmt.h
//  Implementation of the Class SCdiFinDevCfgReqFrmt
//  Created on:      2013/12/04 11:29:47
//  Original author: fxat00694
///////////////////////////////////////////////////////////

#if !defined(SCdiFinDevCfgReqFrmt_h)
#define SCdiFinDevCfgReqFrmt_h

#define CDIFIN_DEV_CFG_REQ_MSG_SIZE		4


struct SCdiFinDevCfgReqFrmt
{
	UC id;								// Message ID
	UC subsystem;						// Subsystem
	UC instance;						// Instance
	UC attrKey;							// Attr Key
};

#endif // !defined(SCdiFinDevCfgReqFrmt_h)

///////////////////////////////////////////////////////////
//  SCdiFinDevContInfoFrmt.h
//  Implementation of the Class SCdiFinDevContInfoFrmt
//  Created on:      2014/06/18 15:13:28
//  Original author: fxat00694
///////////////////////////////////////////////////////////

#if !defined(SCdiFinDevContInfoFrmt_h)
#define SCdiFinDevContInfoFrmt_h

//#define CDIFIN_DEV_CONT_INFO_MSG_SIZE	// ‰Â•Ï
#define CDIFIN_DEVCONTINFOHEADSIZE		2
#define CDIFIN_DEVCONTINFOSIZE			2


struct SCdiFinDevContInfoFrmt
{
	UC id;													// Message ID
	UC infoID;												// Information ID
	UC infoData[CDIFIN_DEVCONTINFOSIZE];					// Information Data
};

#endif // !defined(SCdiFinDevContInfoFrmt_h)

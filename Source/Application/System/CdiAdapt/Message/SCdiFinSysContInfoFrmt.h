///////////////////////////////////////////////////////////
//  SCdiFinSysContInfoFrmt.h
//  Implementation of the Class SCdiFinSysContInfoFrmt
//  Created on:      2013/12/04 11:47:06
//  Original author: fxat00694
///////////////////////////////////////////////////////////

#if !defined(SCdiFinSysContInfoFrmt_h)
#define SCdiFinSysContInfoFrmt_h

//#define CDIFIN_SYS_CONT_INFO_MSG_SIZE	// ‰Â•Ï
#define CDIFIN_SYS_CONT_INFO_HEAD_SIZE		2
#define CDIFIN_SYS_CONT_INFO_SIZE			1


struct SCdiFinSysContInfoFrmt
{
	UC id;								// Message ID
	UC infoID;							// Information ID
	UC infoData[CDIFIN_SYS_CONT_INFO_SIZE];	// Information Data
};

#endif // !defined(SCdiFinSysContInfoFrmt_h)

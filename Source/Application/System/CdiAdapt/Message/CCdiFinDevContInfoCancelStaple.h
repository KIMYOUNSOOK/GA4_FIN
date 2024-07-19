// File Name:	CCdiFinDevContInfoCancelStaple.h
// Description:	Header File of CCdiFinDevContInfoCancelStaple
// Engineer:	Yuichi Hoshino
// Last Edit:	15.06.25
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDevContInfoCancelStaple_h)
#define CCdiFinDevContInfoCancelStaple_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinDevContInfoID.h"

#include "CCdiFinMessage.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CDIFIN_DEV_CONT_INFO_DATA_CANCEL_STAPLE_SIZE		1


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

struct SCdiFinDevContInfoCancelStapleFrmt
{
	UC setID;							// Set ID
};


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinDevContInfoCancelStaple_GetInformationID(apInfoData) ((UC)eCdiFinDevContInfoID_CancelStaple)
																				// UC (UC*)
#define CCdiFinDevContInfoCancelStaple_GetInfoDataSize(apInfoData) (CDIFIN_DEV_CONT_INFO_DATA_CANCEL_STAPLE_SIZE)
																				// UC (UC*)
#define CCdiFinDevContInfoCancelStaple_GetSetID(apInfoData) ((struct SCdiFinDevContInfoCancelStapleFrmt*)apInfoData->setID)
																				// UC (UC*)
#define CCdiFinDevContInfoCancelStaple_SetSetID(apInfoData, aSetID) (((struct SCdiFinDevContInfoCancelStapleFrmt*)apInfoData)->setID = aSetID)
																				// void (UC*, UC)


#endif // !defined(CCdiFinDevContInfoCancelStaple_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file based on Base SW VXX
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 15.06.19	| Yuichi Hoshino	| Created this file
// 15.06.25 | Yuichi hoshino	| Introduction Spec [ Log Service‹@”\’Ç‰Á ]
// 16.XX.XX	| Yuichi Hoshino	| Release V2 (Revision 00)
//

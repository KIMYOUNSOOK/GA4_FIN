// File Name:	CCvrChecker.h
// Description:	Header File of Cover Checker
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CCvrChecker_h
#define CCvrChecker_h

////////////////////////////////////////////////////////////
// Include File
////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECvrCoverID.h"
#include "ECvrCoverStatus.h"

#include "CCvrCover.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Const Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Cover Checker Spec ---
//typedef struct /*CvrCheckerSpec*/
//{
//	不要なため未定義
//} CvrCheckerSpec;


////////////////////////////////////////////////////////////
// Function Prototype Definition
////////////////////////////////////////////////////////////

void CCvrChecker_Constructor();

US CCvrChecker_GetDeviceStatusNo(ECvrCoverID coverID);
ECvrCoverStatus CCvrChecker_GetCoverStatus(ECvrCoverID coverID);
void CCvrChecker_DetectInitialStatus();
void CCvrChecker_EndCheck();
void CCvrChecker_DiscardOption(EOptionID optionID);
void CCvrChecker_NotifyEdge(ECvrCoverID coverID, UC edge);
void CCvrChecker_CloseCyclicCheck();


#endif // #ifndef CCvrChecker_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.04.14	| Yuichi Hoshino	| Created this file
// 14.12.04	| Yuichi Hoshino	| Release V0 (Revision 00)
// 15.03.30	| Yuichi Hoshino	| Modify By Introduction Spec [ Power Down後ステータス誤通知防止 ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//

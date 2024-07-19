// File Name:	SheetService.h
// Description:	Header File of Sheet Service
// Engineer:	Yuichi Hoshino
// Last Edit:	14.12.29
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef SheetService_h
#define SheetService_h

////////////////////////////////////////////////////////////
// Include File
////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinOutDest.h"
#include "ECdiFinSetEndAct.h"
#include "ECdiFinIntegrity.h"
#include "ECdiFinSetAction.h"
#include "ECdiFinOffsetMode.h"
#include "ECdiFinOffsetPos.h"
#include "ECdiFinStapleMode.h"
#include "ECdiFinPunchMode.h"
#include "ECdiFinBookMode.h"
#include "ECdiFinFoldMode.h"
#include "ECdiFinMediaType.h"
#include "ECdiFinYesNo.h"
#include "ECdiFinSheetCurl.h"
#include "EShtSheetCondition.h"
#include "EShtBuffer.h"
#include "EShtEject.h"
#include "EShtSetTop.h"
#include "EShtSetEnd.h"
#include "EShtSizeClass.h"
#include "EShtOffsetFinish.h"
#include "EShtDevidedSet.h"
#include "EShtSizeMix.h"
#include "EShtFoldInclusion.h"
#include "EShtSheetEventID.h"
#include "EShtSheetEdge.h"
#include "EShtSheetLocationID.h"
#include "EShtPathSensorID.h"


////////////////////////////////////////////////////////////
// Function Prototype Definition
////////////////////////////////////////////////////////////

enum ECdiFinOutDest SRVC_GetOutputDestination(UC sheetNo);
UC SRVC_GetBinSelection(UC sheetNo);
enum ECdiFinSetEndAct SRVC_GetSetEndAction(UC sheetNo);
enum ECdiFinIntegrity SRVC_GetSheetIntegrity(UC sheetNo);
enum ECdiFinSetAction SRVC_GetSetAction(UC sheetNo);
enum ECdiFinOffsetMode SRVC_GetOffsetMode(UC sheetNo);
enum ECdiFinOffsetPos SRVC_GetOffsetPosition(UC sheetNo);
enum ECdiFinStapleMode SRVC_GetStapleMode(UC sheetNo);
enum ECdiFinPunchMode SRVC_GetPunchMode(UC sheetNo);
enum ECdiFinBookMode SRVC_GetBookletMakerMode(UC sheetNo);
enum ECdiFinFoldMode SRVC_GetFoldMode(UC sheetNo);
enum ECdiFinMediaType SRVC_GetMediaType(UC sheetNo);
US SRVC_GetMediaWeight(UC sheetNo);
enum ECdiFinYesNo SRVC_GetEnvelope(UC sheetNo);
enum ECdiFinYesNo SRVC_GetTabbedPaper(UC sheetNo);
enum ECdiFinYesNo SRVC_GetReusedPaper(UC sheetNo);
enum ECdiFinYesNo SRVC_GetPrepunchedPaper(UC sheetNo);
US SRVC_GetProcessSize(UC sheetNo);
US SRVC_GetCrossProcessSize(UC sheetNo);
enum ECdiFinSheetCurl SRVC_GetCurl(UC sheetNo);
UC SRVC_GetCurlQuantity(UC sheetNo);
US SRVC_GetExitInitialSpeed(UC sheetNo);
UC SRVC_GetExitSpeedChangeTimes(UC sheetNo);
US SRVC_GetExitSpeedChangeTimming(UC sheetNo, UC times);
US SRVC_GetExitChangeSpeed(UC sheetNo, UC times);
US SRVC_GetExitSheetInterval(UC sheetNo);
EShtSheetCondition SRVC_GetSheetCondition(UC sheetNo);
bool SRVC_CheckSheetConditionNormal(UC sheetNo);
bool SRVC_CheckSheetConditionProcess(UC sheetNo);
bool SRVC_CheckSheetConditionDelivery(UC sheetNo);
bool SRVC_CheckSheetConditionNormalDelivery(UC sheetNo);
bool SRVC_CheckSheetConditionAbnormalDelivery(UC sheetNo);
bool SRVC_CheckSheetConditionWaitEject(UC sheetNo);
bool SRVC_CheckSheetConditionWaitFinish(UC sheetNo);
bool SRVC_CheckSheetConditionStop(UC sheetNo);
enum ECdiFinOutDest SRVC_GetDestinationFinish(UC sheetNo);
EShtBuffer SRVC_GetBuffer(UC sheetNo);
//US SRVC_GetBufferFeedWaitTime(UC sheetNo);  //V3.4.0
EShtEject SRVC_GetEject(UC sheetNo);
EShtSetTop SRVC_GetSetTop(UC sheetNo);
EShtSetEnd SRVC_GetSetEnd(UC sheetNo);
EShtSizeClass SRVC_GetSizeClass(UC sheetNo);
EShtSizeClass SRVC_GetProcessSizeClass(UC sheetNo);
EShtSizeClass SRVC_GetCrossProcessSizeClass(UC sheetNo);
US SRVC_GetFinisherSpeed(UC sheetNo);
EShtOffsetFinish SRVC_GetOffsetFinish(UC sheetNo);
UC SRVC_GetCompileCount(UC sheetNo);
US SRVC_GetWeightedCompileCount(UC sheetNo);
EShtDevidedSet SRVC_GetDevidedSet(UC sheetNo);
US SRVC_GetMaxMediaWeightCompile(UC sheetNo); //V3.4.0				// Product Specific
EShtSizeMix SRVC_GetSizeMixSet(UC sheetNo);
EShtSizeMix SRVC_GetProcessSizeMixSet(UC sheetNo);
EShtSizeMix SRVC_GetCrossProcessSizeMixSet(UC sheetNo);
US SRVC_GetMaxProcessSizeCompile(UC sheetNo);
US SRVC_GetMaxCrossProcessSizeCompile(UC sheetNo);
US SRVC_GetMinProcessSizeCompile(UC sheetNo);
US SRVC_GetMinCrossProcessSizeCompile(UC sheetNo);
//EShtFoldInclusion SRVC_GetFoldInclusionCompile(UC sheetNo); //V3.4.0
//EShtFoldInclusion SRVC_GetFoldInclusionSet(UC sheetNo);
bool SRVC_CheckSheetEventOccurence(EShtSheetEventID sheetEvent, UC sheetNo);
EShtSheetLocationID SRVC_GetNextLocation(UC sheetNo, EShtSheetEdge edge);
EShtSheetLocationID SRVC_GetLastLocation(UC sheetNo, EShtSheetEdge edge);
bool SRVC_CheckSheetLocationArrival(EShtSheetLocationID location, UC sheetNo, EShtSheetEdge edge);
UC SRVC_GetProcessStartSheet(void);
UC SRVC_GetTopRequestSheet(void);
UC SRVC_GetBottomRequestSheet(void);
UC SRVC_GetPrevRequestSheet(UC sheetNo);
UC SRVC_GetNextRequestSheet(UC sheetNo);
UC SRVC_GetTopExitSheet(void);
UC SRVC_GetBottomExitSheet(void);
UC SRVC_GetPrevExitSheet(UC sheetNo);
UC SRVC_GetNextExitSheet(UC sheetNo);
UC SRVC_GetBufferPairSheet(UC sheetNo);
bool SRVC_CheckUndeliveredSheetExist(enum ECdiFinOutDest outDest);
bool SRVC_CheckUnejectedSheetExist(enum ECdiFinOutDest outDest);
bool SRVC_CheckUnfinishedSheetExist(enum ECdiFinOutDest outDest); //V3.6.92
bool SRVC_CheckUnfinishedSheetExist_2(enum ECdiFinOutDest outDest);

UC SRVC_GetNextArriveSheet(EShtPathSensorID pathSnrID, EShtSheetEdge edge);
bool SRVC_CheckPathSensorPlan(EShtPathSensorID pathSnrID, UC sheetNo);
bool SRVC_CheckPathSensorArrival(EShtPathSensorID pathSnrID, UC sheetNo, EShtSheetEdge edge);
void SRVC_SetPitchSkip(UC  bOnOff);//V3.6.92
UC  SRVC_GetPitchSkip(void);



#endif // #ifndef SheetService_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.12.05	| Yuichi Hoshino	| Created this file
// 14.12.05	| Yuichi Hoshino	| Release V0
// 14.12.29	| Yuichi Hoshino	| Trouble Action [ 速度変更タイミング取得サービス戻り値不正 ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
//

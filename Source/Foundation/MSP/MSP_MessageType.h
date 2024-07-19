// ☆☆☆ Module Software Platform Frame ☆☆☆
/// @file    MSP_MessageType.h
/// @author  Tsuyoshi Seto
/// @date    16/03/11
/// @time    16:55:11
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// ☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆
//  本ヘッダーファイルはイベント管理リストから自動生成されます。（プロダクト･ユニーク）
// ☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆

#ifndef MSP_MessageType_h
#define MSP_MessageType_h


// *******************************************************  <<<<< Definition & Declaration Area >>>>>  ********************************************************
typedef struct /*EvtParamDeviceStatusNotify*/ {
	US statusNo ;
	US statusValue ;
} EvtParamDeviceStatusNotify ;

typedef enum /*EDeviceCondition*/ {
	DEV_COND_OK     ,
	DEV_COND_TEMP_NG,
	DEV_COND_PERM_NG,
	DEV_COND_NUM
} EDeviceCondition ;

typedef struct /*EvtParamDeviceConditionNotify*/ {
	EDeviceCondition condition ;
} EvtParamDeviceConditionNotify ;

typedef struct /*EvtParamSheetAttributeEntry*/ {
	UC sheetNo ;
} EvtParamSheetAttributeEntry ;

typedef struct /*EvtParamSheetAttributeUpdate*/ {
	UC sheetNo ;
} EvtParamSheetAttributeUpdate ;

typedef struct /*EvtParamSheetDeliveredNotify*/ {
	UC sheetNo ;
} EvtParamSheetDeliveredNotify ;

typedef struct /*EvtParamSheetActionRequest*/ {
	UC sheetNo ;
} EvtParamSheetActionRequest ;

typedef struct /*EvtParamSheetActionResponse*/ {
	UC sheetNo ;
} EvtParamSheetActionResponse ;

typedef struct /*EvtParamSheetEventNotify*/ {
	UC sheetNo ;
} EvtParamSheetEventNotify ;

typedef struct /*EvtParamSheetEventJamOccurrenceNotify*/ {
	US statusNo ;
	UC sheetNo ;
} EvtParamSheetEventJamOccurrenceNotify ;

typedef struct /*EvtParamComponentControlStartRequest*/ {
	US chainNo ;
	US linkNo ;
} EvtParamComponentControlStartRequest ;

typedef struct /*EvtParamComponentControlStartNotify*/ {
	US chainNo ;
	US linkNo ;
} EvtParamComponentControlStartNotify ;

typedef struct /*EvtParamComponentControlStopRequest*/ {
	US chainNo ;
	US linkNo ;
} EvtParamComponentControlStopRequest ;

typedef struct /*EvtParamComponentControlStopNotify*/ {
	US chainNo ;
	US linkNo ;
} EvtParamComponentControlStopNotify ;


#endif          // MSP_MessageType_h

// ****************************************************************  [ MSP_MessageType.h : EOF ]  *****************************************************************

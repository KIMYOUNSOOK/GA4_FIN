// File Name: DD_Common.h
// Description:
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 13.10.15
// Revision:  000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+--------------------------------------------------------------------------------------
// 13.10.15 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev002 Base)
// Revision 000

#ifndef DD_COMMON_H
#define DD_COMMON_H

#include "DD_Event.h"
//#include "DD_COM_User.h"

#define DD_MAX_CALLBACK_NUM			0x03
#define DD_CALLBACK_INDEX_NONE		0xFF	

typedef enum{
	actCmdRcv_ID = 0,
	actActivity_ID
}ActorID;

typedef struct {
	UC componentID;
//	ControlID ctrlType;
	ActorID actorID;
	UC actionIndex;
	UC activityIndex;
	UC isAnsReq;
}ACT_COMMON;


// callback‹¤’Ê‚ÌŒ^
typedef struct{
	void (*pFunc)(void* arg);
	void* arg;
}DD_CALLBACK_TYPE;


// ProtoType
//void Action_Answer(ACT_COMMON* pPara, ResultID result);
//void Action_SendResult(UC componentID, ControlID ctrlType, ResultID result);

#endif
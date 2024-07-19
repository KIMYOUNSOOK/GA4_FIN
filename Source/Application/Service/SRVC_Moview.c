// File Name: SRVC_Moview.h
// Description:
// Engineer:  Morikawa Atsushi
// Last Edit: 14.11.17
// Revision:  000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#include "FinisherCommon.h"

#include "DD_Event.h"
#include "DD_LOG.h"
#include "SoftwareVersion.h"
#include "SRVC_Moview.h"
//#ifdef WIN32
//#include "win_itron4_dummy.h"
//#endif

// ProtoType
void SRVC_Moview_SetPortData(UC ,US, UL, US, US, UC) ;				// Set Port Data                    (Type, ID, Level, Ratio, Paper, Side) : void

//= ☆☆☆ Product Uniq ☆☆☆ ================================================
// Function Name : SRVC_Moview_SetIotVersion()
// Description   : Set Version for mo.view
// Parameter     : -
// Return        : -
//=============================================================================
void SRVC_Moview_SetVersion(void)
{
	SrvcMovData data ;
	data.version = ( ((UL)PATCH_VERSION << 24) | ((UL)MINOR_VERSION << 16) | ((UL)MAJOR_VERSION << 8) ) ;
	DD_LOG_Put(LOGID_DEVICE, 'S', (UC *)&data, sizeof(data), LOG_CATEGORY_TOOL) ;
}


//=============================================================================
// Function Name : SRVC_Moview_SetXxx()
// Description   : Set Port Log.
// Parameter     : xxx
// Return        : -
//=============================================================================
void SRVC_Moview_SetPaperLocation(UC lv, UL loc, US sheet)          { SRVC_Moview_SetPortData(SRVC_MOV_PORT_PPR_LOC      , lv    , loc   , 0    , sheet , 0xFF) ; }
void SRVC_Moview_SetPaperAction(UL action, US sheet)                { SRVC_Moview_SetPortData(SRVC_MOV_PORT_PPR_ACT      , 0     , action, 0    , sheet , 0xFF) ; }
void SRVC_Moview_SetStepMotOn(UC id, UC direct)                     { SRVC_Moview_SetPortData(SRVC_MOV_PORT_STEP_MOT_ON  , id    , direct, 0    , 0xFFFF, 0xFF) ; }
void SRVC_Moview_SetStepMotOff(UC id)                               { SRVC_Moview_SetPortData(SRVC_MOV_PORT_STEP_MOT_OFF , id    , 0     , 0    , 0xFFFF, 0xFF) ; }
void SRVC_Moview_SetStepMotMode(UC id, UC number)                   { SRVC_Moview_SetPortData(SRVC_MOV_PORT_STEP_MOT_MODE, id    , number, 0    , 0xFFFF, 0xFF) ; }
void SRVC_Moview_SetStepMotCurrent(UC id, US duty)                  { SRVC_Moview_SetPortData(SRVC_MOV_PORT_STEP_MOT_CRNT, id    , duty  , 0    , 0xFFFF, 0xFF) ; }
void SRVC_Moview_SetStepMotClock(UC id, US step, US ratio)          { SRVC_Moview_SetPortData(SRVC_MOV_PORT_STEP_MOT_CLK , id    , step  , ratio, 0xFFFF, 0xFF) ; }
void SRVC_Moview_SetPwm(UC id, US lv)                               { SRVC_Moview_SetPortData(SRVC_MOV_PORT_PWM          , id    , lv    , 0    , 0xFFFF, 0xFF) ; }
void SRVC_Moview_SetOutputPort(UC id, US lv)                        { SRVC_Moview_SetPortData(SRVC_MOV_PORT_OUT_PORT     , id    , lv    , 0    , 0xFFFF, 0xFF) ; }
void SRVC_Moview_SetDcMotOn(UC id, UC direct)                       { SRVC_Moview_SetPortData(SRVC_MOV_PORT_DC_MOT_ON    , id    , direct, 0    , 0xFFFF, 0xFF) ; }
void SRVC_Moview_SetDcMotOff(UC id)                                 { SRVC_Moview_SetPortData(SRVC_MOV_PORT_DC_MOT_OFF   , id    , 0     , 0    , 0xFFFF, 0xFF) ; }
void SRVC_Moview_SetDcMotClock(UC id, US ratio)                     { SRVC_Moview_SetPortData(SRVC_MOV_PORT_DC_MOT_CLK   , id    , ratio , ratio, 0xFFFF, 0xFF) ; }
void SRVC_Moview_SetUniqPortEvent(ESrvcMoviewUniqPortID id, US lv)	{ SRVC_Moview_SetPortData(SRVC_MOV_PORT_UNIQ         , (US)id, lv    , 0    , 0xFFFF, 0xFF) ; }
void SRVC_Moview_SetJamOccur(US paper)                              { SRVC_Moview_SetPortData(SRVC_MOV_PORT_UNIQ, SRVC_MOV_UNIQ_ID_JAM_OCCUR, 1, 0, paper, 0xFF) ; }
// ratioは分周比と同等な値（大小関係）をいれておく

//=============================================================================
// Function Name : CSrvcMoView_SetPortData()
// Description   : Set Port Data for mo.view
// Parameter     : Port Type
//               : Port ID
//               : Port Level
//               : Ratio
//               : Paper ID
//               : Current Side
// Return        : -
//=============================================================================
void SRVC_Moview_SetPortData(UC type, US id, UL lv, US ratio, US paper, UC side)
{
	SrvcMovPortData data ;
	UL sysTim = DD_EVT_GetCurrentTime();

	data.year  = SRVC_GetPowerUpYear() ;
	data.month = SRVC_GetPowerUpMonth() ;
	data.date  = SRVC_GetPowerUpDay() ;
	data.hour  = SRVC_GetPowerUpHour() ;
	data.min   = SRVC_GetPowerUpMinute() ;
	data.type  = type ;
	data.port  = id ;
	data.paper = paper ;
	data.side  = side ;
	data.ratio = ratio ;
	data.level = lv ;
	data.time  = sysTim ;

	DD_LOG_Put(LOGID_DEVICE, 'S', (UC *)&data, sizeof(data), LOG_CATEGORY_TOOL) ;
}

//=============================================================================
// Function Name : SRVC_UserDebugLog()
// Description   :  User Debug
// Parameter     : 
//=============================================================================
void SRVC_UserDebugLog(UC para1, UC para2)
{
	UC  testLog[2];
	
	testLog[0] = para1;
	testLog[1] = para2;
			
	DD_LOG_Put(LOGID_END, 'S', testLog, sizeof(testLog), LOG_CATEGORY_DEBUG);
}

//=============================================================================
// Function Name : SRVC_UserDebugLog_Wide
// Description   :  User Debug
// Parameter     : 
//=============================================================================
void SRVC_UserDebugLog_Wide(UC para1, UC para2, UC para3, UC para4)
{
	UC  testLog[4];
	
	testLog[0] = para1;
	testLog[1] = para2;
	testLog[2] = para3;
	testLog[3] = para4;
		
	DD_LOG_Put(LOGID_END, 'S', testLog, sizeof(testLog), LOG_CATEGORY_DEBUG);
}
///////////////////////////////////////////////////////////////////
// Function Name : SRVC_INITManagerLog_Wide ( LOGID_INIT_Mgr	19 )
// Description   : シートロケーションログ出力指示処理
// Parameter     : para1 (数字列の最初に付加する値)
//               : para2 (数字列の２番目に付加する値)
//               : para3 (数字列の３番目に付加する値)
//               : para4 (数字列の４番目に付加する値)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SRVC_INITManagerLog_Wide(UC para1, UC para2, US para3, US para4)
{
	UC log[6];

	log[0] = para1;
	log[1] = para2;
	log[2] = (UC) (para3 >>8 ) ;
	log[3] = (UC) (para3 & 0x00FF);	
	log[4] = (UC) (para4 >>8 ) ;
	log[5] = (UC) (para4 & 0x00FF);	

	DD_LOG_Put(LOGID_INIT_Mgr, 'S', log, sizeof(log), LOG_CATEGORY_DEBUG);
}
// Change History
// Date:    | Engineer:          | Note:
// ---------+--------------------+----------------------------------------------------------------------------------------------
// 14.11.17 | Morikawa Atsushi   | Created this file.
// 14.02.19 | Morikawa Atsushi   | Disabled unsupported functions.

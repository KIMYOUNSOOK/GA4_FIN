// File Name: SRVC_Moview.h
// Description:
// Engineer:  Morikawa Atsushi
// Last Edit: 14.11.17
// Revision:  000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef SRVC_Moview_h
#define SRVC_Moview_h

#include "DD_LOG.h"

// ***********************************  <<<<< Device Class Prototype Define Area >>>>>  ************************************
// 前方参照が必要なClassを定義する.
// ----------------------------------------------------------------------------------------------------------------------------

// ***************************************  <<<<< Definition & Declaration Area >>>>>  ****************************************

// ----- mo.view Common Type -----
#define SRVC_MOV_TYPE_POWER_ON			0					// Common Type : Power On
#define SRVC_MOV_TYPE_IF				1					// Common Type : Interface
#define SRVC_MOV_TYPE_TRAY_STT			2					// Common Type : Tray State

// ----- mo.view Port Data Type -----
#define SRVC_MOV_PORT_PPR_LOC			3					// Port Type : Paper Location
#define SRVC_MOV_PORT_PPR_ACT			4					// Port Type : Paper Action
#define SRVC_MOV_PORT_STEP_MOT_ON		5					// Port Type : Stepping Motor : ON & Direction
#define SRVC_MOV_PORT_STEP_MOT_OFF		6					// Port Type : Stepping Motor : OFF
#define SRVC_MOV_PORT_STEP_MOT_MODE		7					// Port Type : Stepping Motor : Mode Change
#define SRVC_MOV_PORT_STEP_MOT_CRNT		8					// Port Type : Stepping Motor : Current Change
#define SRVC_MOV_PORT_STEP_MOT_CLK		9					// Port Type : Stepping Motor : Speed Change
#define SRVC_MOV_PORT_PWM				10					// Port Type : PWM Output (include Clutch and Solenoid. except Motor Current)
#define SRVC_MOV_PORT_OUT_PORT			11					// Port Type : Output Port
#define SRVC_MOV_PORT_DC_MOT_ON			12					// Port Type : DC Motor : ON & Direction
#define SRVC_MOV_PORT_DC_MOT_OFF		13					// Port Type : DC Motor : OFF
#define SRVC_MOV_PORT_DC_MOT_CLK		14					// Port Type : DC Motor : Clock
//#define SRVC_MOV_PORT_DC_MOT_BRK		15					// Port Type : DC Motor : Brake
//#define SRVC_MOV_PORT_DC_MOT_GAIN		16					// Port Type : DC Motor : Gain

#define SRVC_MOV_PORT_UNIQ				30					// Port Type : Product Uniq Event

// ----- mo.view Operation Data Type -----
#define SRVC_MOV_OPE_ITLK				3					// Operation Type : Interlock
#define SRVC_MOV_OPE_TRAY_WIDTH			4					// Operation Type : Tray Width
#define SRVC_MOV_OPE_TRAY_LENGTH		5					// Operation Type : Tray Length
#define SRVC_MOV_OPE_TRAY_STOCK			6					// Operation Type : Tray Stock
#define SRVC_MOV_OPE_PAPER_TYPE			7					// Operation Type : Paper Type
#define SRVC_MOV_OPE_UNIQ				30					// Operation Type : Product Uniq

// ----- mo.view Port Level -----
#define SRVC_MOV_SHOT					0xFFFE				// Port Level of Shot
#define SRVC_MOV_RISE					255					// Port Level of Rise(Int Active)
#define SRVC_MOV_FALL					254					// Port Level of Fall(Int Inactive)


// ############################################################################
//{###   Type Define                                                          #
// ############################################################################

//## [[[ mo.View Port Data ]]]
typedef struct {											//      Byte :   Bit   : Mean
	US year  :  7 ;											//      1- 2 :   1-  7 : Year (00-99)
	US month :  4 ;											//      1- 2 :   8- 11 : Month
	US date  :  5 ;											//      1- 2 :  12- 16 : Date
	US hour  :  5 ;											//      3- 4 :  15- 21 : Hour
	US min   :  6 ;											//      3- 4 :  20- 27 : Minute
	US type  :  5 ;											//      3- 4 :  28- 32 : Port Type (0～31)
	US port       ;											//      5- 6 :  33- 48 : Port ID
	UC side       ;											//      7    :  49- 56 : Side
	UC dummy      ;											//      8    :  57- 64 : Boundary
	US paper      ;											//      9-10 :  65- 80 : PaperID
	US ratio      ;											//     11-12 :  81- 96 : Ratio
	UL level      ;											//     13-16 :  97-128 : Port Level
	UL time       ;											//     17-20 : 129-160 : OS Time [x1ms]
} SrvcMovPortData;


//## [[[ mo.View Operation Data ]]]
/*
typedef struct {											//      Byte :   Bit   : Mean
	US year  :  7 ;											//      1- 2 :   1-  7 : Year (00-99)
	US month :  4 ;											//      1- 2 :   8- 11 : Month
	US date  :  5 ;											//      1- 2 :  12- 16 : Date
	US hour  :  5 ;											//      3- 4 :  17- 21 : Hour
	US min   :  6 ;											//      3- 4 :  22- 27 : Minute
	US type  :  5 ;											//      3- 4 :  28- 32 : Operation Type
	US id         ;											//      5- 6 :  33- 48 : Operation ID
	US info       ;											//      7- 8 :  49- 64 : Operation Information
	UL time       ;											//      9-12 :  65- 96 : OS Time [x1ms]
} SrvcMovOpeData;
*/


//## [[[ mo.View Data ]]]
typedef struct{
	UL version ;											// IOT Version
} SrvcMovData;


//## [[[ mo.View Product Uniq Port ID ]]]
// ※ Product UniqのPort IDは以下のEnumで定義し、mo.viewのPort番号と合わせること
typedef enum{
	SRVC_MOV_UNIQ_ID_JAM_OCCUR ,							// Jam発生
	SRVC_MOV_UNIQ_ID_PRNT_CHG ,								// Print Function Change
	SRVC_MOV_UNIQ_ID_PRNT_STBY ,							// Print Standby
	SRVC_MOV_UNIQ_ID_CYC_UP ,								// Cycle Up
	SRVC_MOV_UNIQ_ID_CYC_DWN ,								// Cycle Down
	SRVC_MOV_UNIQ_ID_JAM_DWN ,								// Jam Down
	SRVC_MOV_UNIQ_ID_HARD_DWN ,								// Hard Down
	SRVC_MOV_UNIQ_ID_DIAG_CHG ,								// Diag Function Change
	SRVC_MOV_UNIQ_ID_PWR_SAVE ,								// Power Save
	SRVC_MOV_UNIQ_ID_PWR_RSM ,								// Power Resume
	SRVC_MOV_UNIQ_ID_PWR_DWN ,								// Power Down
	SRVC_MOV_UNIQ_ID_TAMP_F_HOME_SNR ,						// Tamper Front Home Sensor
	SRVC_MOV_UNIQ_ID_TAMP_R_HOME_SNR ,						// Tamper Rear Home Sensor
	SRVC_MOV_UNIQ_ID_SHELF_HOME_SNR ,						// Shelf Home Sensor
	SRVC_MOV_UNIQ_ID_END_WALL_HOME_SNR ,					// End Wall Home Sensor
	SRVC_MOV_UNIQ_ID_END_WALL_OPEN_SNR ,					// End Wall Open Sensor
	SRVC_MOV_UNIQ_ID_TOP_TRAY_FULL1_SNR ,					// Top Tray Full Sensor 1
	SRVC_MOV_UNIQ_ID_TOP_TRAY_FULL2_SNR ,					// Top Tray Full Sensor 2
	SRVC_MOV_UNIQ_ID_STAPLE_MOVE_HOME_SNR ,					// Staple Move Home Sensor
	SRVC_MOV_UNIQ_ID_STAPLE_MOVE_CENT_SNR ,					// Staple Move Center Sensor
	SRVC_MOV_UNIQ_ID_STAPLE_DUST_NEAR_FULL_SNR ,			// Staple Dust Near Full Sensor
	SRVC_MOV_UNIQ_ID_STAPLE_DUST_BOX_SNR ,					// Staple Dust Box Sensor
	SRVC_MOV_UNIQ_ID_STAPLE_HOME_SNR ,						// Staple Home Sensor
	SRVC_MOV_UNIQ_ID_LOW_STAPLE_SNR ,						// Low Staple Sensor
	SRVC_MOV_UNIQ_ID_STAPLER_READY_SNR ,					// Stapler Ready Sensor
	SRVC_MOV_UNIQ_ID_EJECT_UP_SNR ,							// Eject Up Sensor
	SRVC_MOV_UNIQ_ID_EJECT_SAFTY_SW ,						// Eject Safty Switch
	SRVC_MOV_UNIQ_ID_SUB_PAD_HOME_SNR ,						// Sub Paddle Home Sensor
	SRVC_MOV_UNIQ_ID_STK_NO_PAP_SNR ,						// Stacker No Paper Sensor
	SRVC_MOV_UNIQ_ID_STAPLE_HEIGHT_SNR ,					// Staple Stack Height Sensor
	SRVC_MOV_UNIQ_ID_UNSTAPLE_HEIGHT_SNR ,					// Unstaple Stack Height Sensor
	SRVC_MOV_UNIQ_ID_PAP_REMOVE_SNR ,						// Paper Remove Sensor
	SRVC_MOV_UNIQ_ID_Z_STK_HEIGHT_SNR ,						// Z Stacker Height Sensor
	SRVC_MOV_UNIQ_ID_STK_UPPER_LIMIT_SNR ,					// Stacker Upper Limit Sensor
	SRVC_MOV_UNIQ_ID_TRAY_OFFSET_SNR ,						// Tray Offset Sensor
	SRVC_MOV_UNIQ_ID_STK_FLAP_SNR ,							// Stacker Flap Sensor
	SRVC_MOV_UNIQ_ID_BOOK_TRAY_BELT_SW ,					// Booklet Tray Belt Switch
	SRVC_MOV_UNIQ_ID_FRONT_DOOR_IL_SW ,						// Front Door IL Switch
	SRVC_MOV_UNIQ_ID_STK_FLAP2_SNR ,						// Stacker Flap Sensor 2
} ESrvcMoviewUniqPortID;

// EShtPathSensorIDと同じ並び
typedef enum{
	SRVC_MOV_SNR_ID_IOT_EXIT   ,
	SRVC_MOV_SNR_ID_FIN_IN     ,
	SRVC_MOV_SNR_ID_DP_PATH    ,
	SRVC_MOV_SNR_ID_PUNCH_OUT  ,
	SRVC_MOV_SNR_ID_TOP_PATH   ,
	SRVC_MOV_SNR_ID_TOP_EXIT   ,
	SRVC_MOV_SNR_ID_COMP_EXIT  ,
	SRVC_MOV_SNR_ID_BUFFER_PATH,
	SRVC_MOV_SNR_ID_EJECT	   ,
	SRVC_MOV_SNR_ID_COMP_NO_PAP,
	SRVC_MOV_SNR_ID_EJECT_SET  ,
//	SRVC_MOV_SNR_ID_BOOK_IN    ,
} ESrvcMoviewSensorID;

typedef enum{
	SRVC_MOV_LOC_ID_BUFFER_FEED ,
	SRVC_MOV_LOC_ID_PUNCH_BACK  ,
	SRVC_MOV_LOC_ID_STK_EJCT    ,
	SRVC_MOV_LOC_ID_BOOK_EJCT   ,
} ESrvcMoviewLocID;

typedef enum{
	SRVC_MOV_STEP_ID_FIN_ENT     ,
	SRVC_MOV_STEP_ID_TOP_PATH    ,
	SRVC_MOV_STEP_ID_COMP_EXIT   ,
	SRVC_MOV_STEP_ID_FRONT_TAMP  ,
	SRVC_MOV_STEP_ID_REAR_TAMP   ,
	SRVC_MOV_STEP_ID_SHELF       ,
	SRVC_MOV_STEP_ID_END_WALL    ,
	SRVC_MOV_STEP_ID_BOOK_TRANS  ,
	SRVC_MOV_STEP_ID_STAPLE_MOVE ,
	SRVC_MOV_STEP_ID_EJCT_CLAMP  ,
	SRVC_MOV_STEP_ID_EJCT        ,
	SRVC_MOV_STEP_ID_PADDLE      ,
	SRVC_MOV_STEP_ID_PFIM_TRANS  ,
} ESrvcMoviewStepMotID;

typedef enum{
	SRVC_MOV_PWM_ID_BOOK_GATE_PUSH     ,
	SRVC_MOV_PWM_ID_BOOK_GATE_PULL     ,
	SRVC_MOV_PWM_ID_TOP_GATE_PUSH      ,
	SRVC_MOV_PWM_ID_TOP_GATE_PULL      ,
	SRVC_MOV_PWM_ID_COMP_PAD_SOL_PUSH  ,
	SRVC_MOV_PWM_ID_COMP_PAD_SOL_PULL  ,
	SRVC_MOV_PWM_ID_STAPLE_MOT_STAPLE  ,
	SRVC_MOV_PWM_ID_STAPLE_MOT_REVERSE ,
	SRVC_MOV_PWM_ID_SUB_PAD_SOL_PUSH   ,
	SRVC_MOV_PWM_ID_SUB_PAD_SOL_PULL   ,
	SRVC_MOV_PWM_ID_SUB_PAD_CL         ,
	SRVC_MOV_PWM_ID_SET_CLAMP_SOL_PUSH ,
	SRVC_MOV_PWM_ID_SET_CLAMP_SOL_PULL ,
} ESrvcMoviewPWMID;

typedef enum{
	SRVC_MOV_OUTPUT_ID_LED_E0  ,
	SRVC_MOV_OUTPUT_ID_LED_E1  ,
	SRVC_MOV_OUTPUT_ID_LED_E2  ,
	SRVC_MOV_OUTPUT_ID_LED_E3  ,
	SRVC_MOV_OUTPUT_ID_LED_E4  ,
	SRVC_MOV_OUTPUT_ID_LED_E5  ,
	SRVC_MOV_OUTPUT_ID_LED_E6  ,
	SRVC_MOV_OUTPUT_ID_LED_E7  ,
	SRVC_MOV_OUTPUT_ID_LED_E8  ,
	SRVC_MOV_OUTPUT_ID_LED_E9  ,
	SRVC_MOV_OUTPUT_ID_LED_E10 ,
	SRVC_MOV_OUTPUT_ID_LED_E11 ,
	SRVC_MOV_OUTPUT_ID_LED_E12 ,
} ESrvcMoviewOutputID;

typedef enum{
	SRVC_MOV_DC_ID_STK_OFFSET   ,
	SRVC_MOV_DC_ID_STK_ELEVATOR ,
	SRVC_MOV_DC_ID_STK_FLAPPER  ,
} ESrvcMoviewDCMotID;

// ProtoType
void SRVC_Moview_SetVersion(void);
/*
void SRVC_Moview_SetPowerOn(void);
void SRVC_Moview_SetInterface(UC id);
*/
void SRVC_Moview_SetTrayState(UC id, US state);
void SRVC_Moview_SetPaperLocation(UC lv, UL loc, US sheet);
void SRVC_Moview_SetPaperAction(UL action, US sheet);
void SRVC_Moview_SetStepMotOn(UC id, UC direct);
void SRVC_Moview_SetStepMotOff(UC id);
void SRVC_Moview_SetStepMotMode(UC id, UC number);
void SRVC_Moview_SetStepMotCurrent(UC id, US duty);
void SRVC_Moview_SetStepMotClock(UC id, US step, US ratio);
void SRVC_Moview_SetPwm(UC id, US lv);
void SRVC_Moview_SetOutputPort(UC id, US lv);
void SRVC_Moview_SetDcMotOn(UC id, UC direct);
void SRVC_Moview_SetDcMotOff(UC id);
void SRVC_Moview_SetDcMotClock(UC id, US ratio);
void SRVC_Moview_SetUniqPortEvent(ESrvcMoviewUniqPortID id, US lv);
void SRVC_Moview_SetJamOccur(US paper);
void SRVC_UserDebugLog(UC para1, UC para2); //V3.1.7
void SRVC_UserDebugLog_Wide(UC para1, UC para2, UC para3, UC para4);
void SRVC_INITManagerLog_Wide(UC para1, UC para2, US para3, US para4);

/*
void SRVC_Moview_SetInterlock(US state);
void SRVC_Moview_SetTrayWidth(UC id, US width);
void SRVC_Moview_SetTrayLength(UC id, US length);
void SRVC_Moview_SetTrayStock(UC id, US stock);
void SRVC_Moview_SetTrayPaperType(UC id, US type);
*/

#endif														//------- End of Compile Switch: PH_SW_DEV_MO_VIEW -------//


// Change History
// Date:    | Engineer:          | Note:
// ---------+--------------------+----------------------------------------------------------------------------------------------
// 14.11.17 | Morikawa Atsushi   | Created this file.
// 14.02.19 | Morikawa Atsushi   | Disabled unsupported functions.

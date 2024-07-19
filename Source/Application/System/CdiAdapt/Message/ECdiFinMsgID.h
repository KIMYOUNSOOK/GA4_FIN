/////////////////////////////////////////////////////////////////////////////////////
//  ECdiFinMsgID.h
//  Implementation of the Class ECdiFinMsgID
//  Created on:      2013/05/28 15:49:15
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#if !defined(EA_B96DD54A_F636_43c3_B820_0B592B4DADF5__INCLUDED_)
#define EA_B96DD54A_F636_43c3_B820_0B592B4DADF5__INCLUDED_

///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : CDI Finisher Message::ECdiFinMsgID
// CDI-Finisher Message ID
////////////////////////////////////////////////////////////////////////
enum ECdiFinMsgID
{
	// Device Capabilities Request Message ID
	eCdiFinMsgID_DevCapabilitiesReq = 0x01,
	// Device Capabilities Update Message ID
	eCdiFinMsgID_DevCapabilitiesUpdate = 0x02,
	// Device Configuration  Request Message ID
	eCdiFinMsgID_DevCfgReq = 0x03,
	// Device Configuration Update Message ID
	eCdiFinMsgID_DevCfgUpdate = 0x04,
	// Set Device Configuration Message ID
	eCdiFinMsgID_SetDevCfg = 0x07,
	// Select Finisher Attribute Message ID
	eCdiFinMsgID_SelectFinAttr = 0x20,
	// Sheet Finish Request
	eCdiFinMsgID_SheetFinReq = 0x21,
	// Ready For Next Sheet Message ID
	eCdiFinMsgID_ReadyForNextSheet = 0x22,
	// Sheet Exit Message ID
	eCdiFinMsgID_SheetExit = 0x23,
	// Set Finish Request Message ID
	eCdiFinMsgID_SetFinReq = 0x24,
	// Stop Finisher Message ID
	eCdiFinMsgID_StopFinisher = 0x25,
	// Sheet Fed Message ID
	eCdiFinMsgID_SheetFed = 0x27,
	// Cancel Sheet Finish Request Message ID
	eCdiFinMsgID_CancelSheetFinReq = 0x28,
	// Sheet Delivered To Output Message ID
	eCdiFinMsgID_SheetDelvToOutput = 0x30,
	// Set Finished At Output Message ID
	eCdiFinMsgID_SetFinAtOutput = 0x33,
	// Device Pause Status Message ID
	eCdiFinMsgID_DevPauseReq = 0x36,
	// Report Pause Status Message ID
	eCdiFinMsgID_ReportPauseStts = 0x37,
	// Device Controller State Update Message
	// ID
	eCdiFinMsgID_DevContSttUpdate = 0x41,
	// Device Controller Mode Update Message ID
	eCdiFinMsgID_DevContModeUpdate = 0x45,
	// Change Device Controller Mode Message ID
	eCdiFinMsgID_ChgDevContMode = 0x48,
	// Device Status Request Message ID
	eCdiFinMsgID_DevSttsReq = 0x4E,
	// Device Status Update Message ID 
	eCdiFinMsgID_DevSttsUpdate = 0x4F,
	// Initialization Complete Message ID
	eCdiFinMsgID_InitComp = 0x82,
	// Initialization Request Message ID
	eCdiFinMsgID_InitReq = 0x86,
	// Initialization Acknowledge Message ID
	eCdiFinMsgID_InitAck = 0x87,
	// Poll Request Message ID
	eCdiFinMsgID_PollReq = 0x88,
	// Power Off Announcement Message ID
	eCdiFinMsgID_PwrOffAnnounce = 0x8A,
	// Power Off Acknowledge Message ID
	eCdiFinMsgID_PwrOffAck = 0x8B,
	// LogData Message ID
	eCdiFinMsgID_LogData = 0x92,
	// Execute Diagnostics Message ID
	eCdiFinMsgID_ExecDiag = 0xA0,
	// Diagnostics Response Message ID
	eCdiFinMsgID_DiagResp = 0xA1,
	// Jam Zone Update Message ID
	eCdiFinMsgID_JamZoneUpdate = 0xE0,
	// Change Sheet Finish Request Message ID
	eCdiFinMsgID_ChgShtFinReq = 0xE1,
	// Feed Aborted Message ID
	eCdiFinMsgID_FeedAborted = 0xE2,
	// Set Recovery Request Message ID
	eCdiFinMsgID_SetRecoveryReq = 0xE3,
	// Request Jam Status Check Message ID
	eCdiFinMsgID_ReqJamSttsCheck = 0xE4,
	// Abnormal Finishing Message ID
	eCdiFinMsgID_AbnormalFinishing = 0xE5,
	// Device Controller Action Request
	// Message ID
	eCdiFinMsgID_DevContActReq = 0xEC,
	// System Controller Information Notify
	// Message ID
	eCdiFinMsgID_SysContInfoNotify = 0xED,
	// Device Controller Information Notify
	// Message ID
	eCdiFinMsgID_DevContInfoNotify = 0xEE
};
#endif // !defined(EA_B96DD54A_F636_43c3_B820_0B592B4DADF5__INCLUDED_)

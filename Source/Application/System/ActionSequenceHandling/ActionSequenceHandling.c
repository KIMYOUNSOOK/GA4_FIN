// File Name:	ActionSequenceHandling.c
// Description:	Implimentation File of Action Sequence Handling
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.14
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CSeqDevInitHandler.h"
#include "CSeqShtInitHandler.h"
#include "CSeqProcEndHandler.h"

#include "EShtInitID.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Timer Event Call Back Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void SEQ_CompilerShtInitActivityTimeOut(SS timerID, UL idx);
void SEQ_PuncherShtInitActivityTimeOut(SS timerID, UL idx);


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Power On Initialize Activity(Before Jam Check) ---
SrvcMsgActivity const cSeq_PwrOnInitBeforeJamCheckActivity[] = {
	//startTrgPattern		 pWaitTime									sendActReqEventID					recvEventIDforEndAction					senderID
	// !!! Product Specific !!!
	// { 0, 					 0,											EVT_DEV_REQ_POWER_ON_INIT_PNCH,		EVT_DEV_RES_POWER_ON_INIT_PNCH,			MID_PNCH		},
	{ 0x00000000, 			 0,											EVT_DEV_REQ_POWER_ON_INIT_CMPL, 	EVT_DEV_RES_POWER_ON_INIT_CMPL,			MID_CMPL		},
};

// --- Power On Initialize Activity(After Jam Check) ---
// Ç»Çµ

// --- Power Resume Initialize Activity(Before Jam Check) ---
SrvcMsgActivity const cSeq_PwrRsmInitBeforeJamCheckActivity[] = {
	//startTrgPattern		 pWaitTime									sendActReqEventID					recvEventIDforEndAction					senderID
	// !!! Product Specific !!!
	// { 0, 					 0,											EVT_DEV_REQ_RESUME_INIT_PNCH, 		EVT_DEV_RES_RESUME_INIT_PNCH,			MID_PNCH		},
	{ 0x00000000, 			 0,											EVT_DEV_REQ_RESUME_INIT_CMPL, 		EVT_DEV_RES_RESUME_INIT_CMPL,			MID_CMPL		},
};

// --- Power Resume Initialize Activity(After Jam Check) ---
// Ç»Çµ

// --- Normal Initialize Activity(Before Jam Check) ---
SrvcMsgActivity const cSeq_NormalInitBeforeJamCheckActivity[] = {
	//startTrgPattern		 pWaitTime									sendActReqEventID					recvEventIDforEndAction					senderID
	// !!! Product Specific !!!
	// { 0, 					 0,											EVT_DEV_REQ_NORMAL_INIT_PNCH, 		EVT_DEV_RES_NORMAL_INIT_PNCH,			MID_PNCH		},
	// { 0, 					 0,											EVT_DEV_REQ_NORMAL_INIT_PNCH, 		EVT_DEV_RES_NORMAL_INIT_PNCH,			MID_CMPL		},
	NULL,
		
};

// --- Normal Initialize Activity(After Jam Check) ---
SrvcMsgActivity const cSeq_NormalInitAfterJamCheckActivity[] = {
	//startTrgPattern		 pWaitTime									sendActReqEventID					recvEventIDforEndAction					senderID
	// !!! Product Specific !!!
	{ 0, 					 0,											EVT_DEV_REQ_NORMAL_INIT_CMPL, 		EVT_DEV_RES_NORMAL_INIT_CMPL,			MID_CMPL		},
};

// --- Jam Check Activity ---
SrvcMsgActivity const cSeq_JamCheckActivity[] = {
	//startTrgPattern		 pWaitTime									sendActReqEventID					recvEventIDforEndAction					senderID
	// !!! Product Specific !!!
	{ 0,					 0,											EVT_DEV_REQ_JAM_CHECK_TRA, 			EVT_DEV_RES_JAM_CHECK_TRA,				MID_TRNS			},
};

// --- Jam Clear Assist Initialize Activity ---
SrvcMsgActivity const cSeq_JamClrInitActivity[] = {
	//startTrgPattern		 pWaitTime									sendActReqEventID					recvEventIDforEndAction					senderID
	// !!! Product Specific !!!
	{ 0, 					 0,											EVT_DEV_REQ_JAM_CLEAR_ASSIST_CMPL,	EVT_DEV_RES_JAM_CLEAR_ASSIST_CMPL,		MID_CMPL		},
	// { 0x000000001, 			 0,											EVT_DEV_REQ_JAM_CLEAR_ASSIST_PNCH,	EVT_DEV_RES_JAM_CLEAR_ASSIST_PNCH,		MID_PNCH		},
	{ 0x000000001, 			 0,											EVT_DEV_REQ_JAM_CLEAR_ASSIST_TRA,	EVT_DEV_RES_JAM_CLEAR_ASSIST_TRA,		MID_TRNS			},
};

// --- Spec data of Device Initialize Handler ---
SeqDevInitHandlerSpec const cSeq_DevInitHandlerSpec = {
	cSeq_PwrOnInitBeforeJamCheckActivity,
	(sizeof(cSeq_PwrOnInitBeforeJamCheckActivity)/sizeof(SrvcMsgActivity)),
	NULL,//cSeq_PwrOnInitAfterJamCheckActivity,
	0,//(sizeof(cSeq_PwrOnInitAfterJamCheckActivity)/sizeof(SrvcMsgActivity)),
	cSeq_PwrRsmInitBeforeJamCheckActivity,
	(sizeof(cSeq_PwrRsmInitBeforeJamCheckActivity)/sizeof(SrvcMsgActivity)),
	NULL,//cSeq_PwrRsmInitAfterJamCheckActivity,
	0,//(sizeof(cSeq_PwrRsmInitAfterJamCheckActivity)/sizeof(SrvcMsgActivity)),
	NULL, // cSeq_NormalInitBeforeJamCheckActivity,
	0, // (sizeof(cSeq_NormalInitBeforeJamCheckActivity)/sizeof(SrvcMsgActivity)),
	cSeq_NormalInitAfterJamCheckActivity,
	(sizeof(cSeq_NormalInitAfterJamCheckActivity)/sizeof(SrvcMsgActivity)),
	cSeq_JamCheckActivity,
	(sizeof(cSeq_JamCheckActivity)/sizeof(SrvcMsgActivity)),
	cSeq_JamClrInitActivity,
	(sizeof(cSeq_JamClrInitActivity)/sizeof(SrvcMsgActivity))
};

// --- Process End Action Activity ---
SrvcMsgActivity const cSeq_ProcEndActivity[] = {
	//startTrgPattern		 pWaitTime	sendActReqEventID					recvEventIDforEndAction					senderID
	// !!! Product Specific !!!
	{ 0,		 		 0,			EVT_DEV_REQ_PROCESS_END_CMPL, 	EVT_DEV_RES_PROCESS_END_CMPL,			MID_CMPL		},
	{ 0x000000001, 	 0,			EVT_DEV_REQ_PROCESS_END_TRAY, 	EVT_DEV_RES_PROCESS_END_TRAY,			MID_TRAY		},	
	{ 0x000000002,	 0,			EVT_DEV_REQ_PROCESS_END_TRA, 		EVT_DEV_RES_PROCESS_END_TRA,			MID_TRNS			},
};

// --- Spec data of Process End Action Handler ---
SeqProcEndHandlerSpec const cSeq_ProcEndHandlerSpec = {
	cSeq_ProcEndActivity,
	(sizeof(cSeq_ProcEndActivity)/sizeof(SrvcMsgActivity))
};

// --- Sheet Initialize Activity ---
SrvcMsgActivity const cSeq_ShtInitActivity[] = {
	//startTrgPattern	 pWaitTime	sendActReqEventID					    recvEventIDforEndAction					senderID
	// !!! Product Specific !!!
	{ 0, 				 0,	EVT_SHT_REQ_COMPILER_SHT_INIT_CMPL,  EVT_SHT_RES_COMPILER_SHT_INIT_CMPL,  MID_CMPL },
	{ 0,				 0,	EVT_SHT_REQ_STACKER_SHT_INIT_TRAY,   EVT_SHT_RES_STACKER_SHT_INIT_TRAY,  MID_TRAY }
	
};

// --- Spec data of Sheet Initialize Handler ---
SeqShtInitHandlerSpec cSeq_ShtInitHandlerSpec[SHT_INIT_ID_NUM] = {
//	| Pointer of Activity			| Number of Activity Array 												| Pointer of Timer Call Back Function
	// !!! Product Specific !!!
	{ cSeq_ShtInitActivity, (sizeof(cSeq_ShtInitActivity)/sizeof(SrvcMsgActivity)), SEQ_CompilerShtInitActivityTimeOut	}	// SHT_INIT_ID_COMPILER

};

// --- Instance of Sheet Initialize Handler ---
CSeqShtInitHandler gSeq_ShtInitHandler[SHT_INIT_ID_NUM];


/////////////////////////////////////////////////////////////////////////////////////////
// Timer Event Call Back Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : SEQ_CompilerShtInitActivityTimeOut
// Description   : 
// Parameter     : 
// Return        : Å]
///////////////////////////////////////////////////////////////////
void SEQ_CompilerShtInitActivityTimeOut(SS timerID, UL idx)
{
	CSrvcMsgActivity_TimeOut(&(gSeq_ShtInitHandler[SHT_INIT_ID_COMPILER].mActivity), idx);
}

///////////////////////////////////////////////////////////////////
// Function Name : SEQ_PuncherShtInitActivityTimeOut
// Description   : 
// Parameter     : 
// Return        : Å]
///////////////////////////////////////////////////////////////////
void SEQ_PuncherShtInitActivityTimeOut(SS timerID, UL idx)
{
	// CSrvcMsgActivity_TimeOut(&(gSeq_ShtInitHandler[SHT_INIT_ID_PUNCHER].mActivity), idx);
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

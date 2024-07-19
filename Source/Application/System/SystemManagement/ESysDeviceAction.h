// File Name:	EDeviceAction.h
// Description:	Implementation of the Class Å·enumerationÅ‚ ESysDeviceAction
// Engineer:	Xxxxxx Xxxxxx
// Last Edit:	XX.XX.XX
// Revision:	XX
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(ESysDeviceAction_h)
#define ESysDeviceAction_h

//==============================================================================
// Class Name    : System Module Common Definition::Å·enumerationÅ‚ ESysDeviceAction
// Description   : Device Action ID
//==============================================================================
typedef enum /*ESysDeviceAction*/
{
	DEV_ACT_HD,	// Hard Down,
	DEV_ACT_CD,	// Cycle Down,
	DEV_ACT_PON_INIT,	// Power On Initialize Action (Step1),
	DEV_ACT_PON_INIT_STEP2,	// Power On Initialize Action Step2,
	DEV_ACT_RSM_INIT,	// Power Resume Initialize Action (Step1),
	DEV_ACT_RSM_INIT_STEP2,	// Power Resume Initialize Action Step2,
	DEV_ACT_NRML_INIT,	// Normal Initialize Action (Step1),
	DEV_ACT_NRML_INIT_STEP2,	// Normal Initialize Action Step2,
	DEV_ACT_PROC_END,	// Process End Action (Step1),
	DEV_ACT_PROC_END_STEP2,	// Process End Action Step2,
	DEV_ACT_JAM_CLR_ASSIST,	// Jam Clear Assist Action (Step1),
	DEV_ACT_JAM_CLR_ASSIST_STEP2,	// Jam Clear Assist Action Step2
	DEV_ACT_NUM
} ESysDeviceAction;
#endif // !defined(ESysDeviceAction_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// File Name:	Å·enumerationÅ‚ ESysDeviceEvent.h
// Description:	Implementation of the Class Å·enumerationÅ‚ ESysDeviceEvent
// Engineer:	Xxxxxx Xxxxxx
// Last Edit:	XX.XX.XX
// Revision:	XX
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(ESysDeviceEvent_h)
#define ESysDeviceEvent_h

//==============================================================================
// Class Name    : System Module Common Definition::Å·enumerationÅ‚ ESysDeviceEvent
// Description   : Device Event ID
//==============================================================================
typedef enum /*ESysDeviceEvent*/
{
	DEV_EVT_INTLK_OPEN,
	DEV_EVT_INTLK_CLOSE,
	DEV_EVT_24V_CUT,	// 24V cut for C13,
	DEV_EVT_24V_CONNECT,	// 24V connect for C13,
	DEV_EVT_PON_INIT_COMP,	// Power On Initialize Action complete
		// event,
	DEV_EVT_RSM_INIT_COMP,	// Power Resume Initialize Action complete
		// event,
	DEV_EVT_NRML_INIT_COMP,	// Normal Initialize(cover close or clear
		// all jam zone) Action complete event,
	DEV_EVT_PROC_END_COMP,	// Process End Action complete event
	DEV_EVT_NUM
} ESysDeviceEvent;
#endif // !defined(Å·enumerationÅ‚ ESysDeviceEvent_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

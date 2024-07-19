// File Name:	ECMPL_State.h
// Description:	Compier‚ÌState’è‹`
// Engineer:	Takahiko Minaga
// Last Edit:	14.08.12
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(ECMPL_State_h)
#define ECMPL_State_h

typedef enum /* ECMPL_State */
{
	CMPL_STT_UNKNOWN,//SttUnknown,
	CMPL_STT_POWER_UP,
	CMPL_STT_POWER_ON,
	CMPL_STT_CHANGING_STANDBY,
	CMPL_STT_STANDBY,
	CMPL_STT_CHANGING_DIAG,
	CMPL_STT_DIAG,
	CMPL_STT_POWER_DOWN,
	CMPL_STT_POWER_OFF,
	CMPL_STT_CYCLE_UP,
	CMPL_STT_READY,
	CMPL_STT_CYCLE_DOWN,
	CMPL_STT_HARD_DOWN,
	CMPL_STT_WAITING_STANDBY,
} ECMPL_State;


#endif //!defined(ECMPL_State_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//


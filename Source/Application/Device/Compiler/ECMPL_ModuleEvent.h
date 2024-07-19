// File Name:	ECMPL_ModuleEvent.h
// Description:	���W���[���C�x���g��ʒm
// Engineer:	Takahiko Minaga
// Last Edit:	14.10.28
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(ECMPL_ModuleEvent_h)
#define ECMPL_ModuleEvent_h

typedef enum
{
	CMPL_EVT_EJECT_START,
	CMPL_EVT_EJECT_COMP,
	CMPL_EVT_EJECT_SHEET_THROUGH,
	CMPL_EVT_SET_EJECT_JAM_OCCUR,
	CMPL_EVT_STACKER_INIT_TRIGGER,  //Stacker Job �J�n�ʒm
	CMPL_EVT_STACKER_INIT_WAITING_POS_TRIGGER, // SetClamp ���슮��-> �ҋ@�ʒu�֏グ����J�nTrigger
	CMPL_EVT_STACKER_JOB_TRIGGER1,
	CMPL_EVT_STACKER_JOB_TRIGGER2,
} ECMPL_ModuleEvent;
#endif // !defined(ECMPL_ModuleEvent_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

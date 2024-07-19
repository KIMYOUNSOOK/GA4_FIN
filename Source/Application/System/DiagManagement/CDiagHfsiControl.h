// File Name:	CDiagHfsiControl.h
// Description:	Header File of Diag HFSI Control
// Engineer:	Yuichi Hoshino
// Last Edit:	14.07.14
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CDiagHfsiControl_h
#define CDiagHfsiControl_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "CDiagHFSI.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CDiagHfsiControl_Constructor();

void CDiagHfsiControl_Initialize();
CDiagHFSI* CDiagHfsiControl_Get(US chain, US link);
//CDiagHFSI* CDiagHfsiControl_Get(HFSI_ID id);
void CDiagHfsiControl_ResetIterate();
CDiagHFSI* CDiagHfsiControl_NextIterate();


#endif // #ifndef CDiagHfsiControl_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

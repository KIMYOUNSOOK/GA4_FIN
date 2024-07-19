// File Name:	CCdiFinDiagDataExecDc330Monitor.h
// Description:	Header File of CCdiFinDiagDataExecDc330Monitor
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.19
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDiagDataExecDc330Monitor_h)
#define CCdiFinDiagDataExecDc330Monitor_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "SCdiFinDiagDataExecDc330MonitorFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinDiagDataExecDc330Monitor_GetDiagProgram() ((US)eCdiFinDiagPrgrm_dC330)
																				// US (void)
#define CCdiFinDiagDataExecDc330Monitor_GetDiagCommand() ((UC)eCdiFinDiagCmd_Monitor)
																				// UC (void)
#define CCdiFinDiagDataExecDc330Monitor_GetDiagDataLength(apData) ((UC)CDIFIN_DIAG_DATA_EXEC_DC330_MONITOR_SIZE)
																				// UC (UC*)
#define CCdiFinDiagDataExecDc330Monitor_GetChain(apData) ( ((US)(((struct SCdiFinDiagDataExecDc330MonitorFrmt *)(apData))->chain_Octet0) << 8) | ((US)(((struct SCdiFinDiagDataExecDc330MonitorFrmt *)(apData))->chain_Octet1)) )
																				// US (UC*)
#define CCdiFinDiagDataExecDc330Monitor_SetChain(apData, aChain) ((struct SCdiFinDiagDataExecDc330MonitorFrmt *)(apData))->chain_Octet0 = (UC)((US)(aChain) >> 8); ((struct SCdiFinDiagDataExecDc330MonitorFrmt *)(apData))->chain_Octet1 = (UC)((aChain))
																				// void (UC*, US)
#define CCdiFinDiagDataExecDc330Monitor_GetLink(apData) ( ((US)(((struct SCdiFinDiagDataExecDc330MonitorFrmt *)(apData))->link_Octet0) << 8) | ((US)(((struct SCdiFinDiagDataExecDc330MonitorFrmt *)(apData))->link_Octet1)) )
																				// US (UC*)
#define CCdiFinDiagDataExecDc330Monitor_SetLink(apData, aLink) ((struct SCdiFinDiagDataExecDc330MonitorFrmt *)(apData))->link_Octet0 = (UC)((US)(aChain) >> 8); ((struct SCdiFinDiagDataExecDc330MonitorFrmt *)(apData))->link_Octet1 = (UC)((aChain))
																				// void (UC*, US)
#define CCdiFinDiagDataExecDc330Monitor_GetMonitorOperation(apData) (((struct SCdiFinDiagDataExecDc330MonitorFrmt *)(apData))->monitorOpe)
																				// UC (UC*)
#define CCdiFinDiagDataExecDc330Monitor_SetMonitorOperation(apData, aOperation) (((struct SCdiFinDiagDataExecDc330MonitorFrmt *)(apData))->monitorOpe = aOperation)
																				// void (UC*, UC)


#endif // !defined(CCdiFinDiagDataExecDc330Monitor_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

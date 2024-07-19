// File Name:	CCdiFinDiagDataRespDc330Monitor.h
// Description:	Header File of CCdiFinDiagDataRespDc330Monitor
// Engineer:	Yuichi Hoshino
// Last Edit:	14.06.24
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CCdiFinDiagDataRespDc330Monitor_h)
#define CCdiFinDiagDataRespDc330Monitor_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "CCdiFinMessage.h"

#include "ECdiFinComponentStt.h"

#include "SCdiFinDiagDataRespDc330MonitorFrmt.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define CCdiFinDiagDataRespDc330Monitor_GetDiagProgram() ((US)eCdiFinDiagPrgrm_dC330)
																				// US (void)
#define CCdiFinDiagDataRespDc330Monitor_GetDiagCommand() ((UC)eCdiFinDiagCmd_Monitor)
																				// UC (void)
#define CCdiFinDiagDataRespDc330Monitor_GetDiagDataLength(apData) ((UC)CDIFIN_DIAG_DATA_RESP_DC330_MONITOR_SIZE)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc330Monitor_GetChain(apData) ( ((US)(((struct SCdiFinDiagDataRespDc330MonitorFrmt *)(apData))->chain_Octet0) << 8) | ((US)(((struct SCdiFinDiagDataRespDc330MonitorFrmt *)(apData))->chain_Octet1)) )
																				// US (UC*)
#define CCdiFinDiagDataRespDc330Monitor_SetChain(apData, aChain) ((struct SCdiFinDiagDataRespDc330MonitorFrmt *)(apData))->chain_Octet0 = (UC)((US)(aChain) >> 8); ((struct SCdiFinDiagDataRespDc330MonitorFrmt *)(apData))->chain_Octet1 = (UC)((aChain))
																				// void (UC*, US)
#define CCdiFinDiagDataRespDc330Monitor_GetLink(apData) ( ((US)(((struct SCdiFinDiagDataRespDc330MonitorFrmt *)(apData))->link_Octet0) << 8) | ((US)(((struct SCdiFinDiagDataRespDc330MonitorFrmt *)(apData))->link_Octet1)) )
																				// US (UC*)
#define CCdiFinDiagDataRespDc330Monitor_SetLink(apData, aLink) ((struct SCdiFinDiagDataRespDc330MonitorFrmt *)(apData))->link_Octet0 = (UC)((US)(aLink) >> 8); ((struct SCdiFinDiagDataRespDc330MonitorFrmt *)(apData))->link_Octet1 = (UC)((aLink))
																				// void (UC*, US)
#define CCdiFinDiagDataRespDc330Monitor_GetComponentState(apData) (((struct SCdiFinDiagDataRespDc330MonitorFrmt *)(apData))->componentStt)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc330Monitor_SetComponentState(apData, aState) (((struct SCdiFinDiagDataRespDc330MonitorFrmt *)(apData))->componentStt = aState)
																				// void (UC*, UC)
#define CCdiFinDiagDataRespDc330Monitor_GetTransitionCounter(apData) (((struct SCdiFinDiagDataRespDc330MonitorFrmt *)(apData))->transitionCounter)
																				// UC (UC*)
#define CCdiFinDiagDataRespDc330Monitor_SetTransitionCounter(apData, aCounter) (((struct SCdiFinDiagDataRespDc330MonitorFrmt *)(apData))->transitionCounter = aCounter)
																				// void (UC*, UC)


#endif // !defined(CCdiFinDiagDataRespDc330Monitor_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

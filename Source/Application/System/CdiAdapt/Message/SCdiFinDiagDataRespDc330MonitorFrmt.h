/////////////////////////////////////////////////////////////////////////////////////
//  SCdiFinDiagDataRespDc330MonitorFrmt.h
//  Implementation of the Class SCdiFinDiagDataRespDc330MonitorFrmt
//  Created on:      2012/11/22 15:01:17
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#ifndef SCdiFinDiagDataRespDc330MonitorFrmt_h
#define SCdiFinDiagDataRespDc330MonitorFrmt_h

#define CDIFIN_DIAG_DATA_RESP_DC330_MONITOR_SIZE			6


///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Component Control Messages::SCdiFinDiagDataRespDc330MonitorFrmt
// Diagnostic Data Format of "Diagnostics
// Response / Digital Component Control
// (dC330) / Monitor" Message
////////////////////////////////////////////////////////////////////////
struct SCdiFinDiagDataRespDc330MonitorFrmt
{

	// Chain(Bit15 ~ Bit8)
	UC chain_Octet0;
	// Chain(Bit7 ~ Bit0)
	UC chain_Octet1;
	// Link(Bit15 ~ Bit8)
	UC link_Octet0;
	// Link(Bit7 ~ Bit0)
	UC link_Octet1;
	// Component State
	UC componentStt;
	// Transition Counter
	UC transitionCounter;

};

#endif // #ifndef SCdiFinDiagDataRespDc330MonitorFrmt_h

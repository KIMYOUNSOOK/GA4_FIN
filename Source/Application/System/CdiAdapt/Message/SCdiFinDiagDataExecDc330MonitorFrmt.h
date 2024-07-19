/////////////////////////////////////////////////////////////////////////////////////
//  SCdiFinDiagDataExecDc330MonitorFrmt.h
//  Implementation of the Class SCdiFinDiagDataExecDc330MonitorFrmt
//  Created on:      2012/11/22 15:00:38
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#ifndef SCdiFinDiagDataExecDc330MonitorFrmt_h
#define SCdiFinDiagDataExecDc330MonitorFrmt_h

#define CDIFIN_DIAG_DATA_EXEC_DC330_MONITOR_SIZE			5


///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Component Control Messages::SCdiFinDiagDataExecDc330MonitorFrmt
// Diagnostic Data Format of "Execute
// Diagnostics / Digital Component Control
// (dC330) / Monitor" Message
////////////////////////////////////////////////////////////////////////
struct SCdiFinDiagDataExecDc330MonitorFrmt
{

	// Chain(Bit15 ~ Bit8)
	UC chain_Octet0;
	// Chain(Bit7 ~ Bit0)
	UC chain_Octet1;
	// Link(Bit15 ~ Bit8)
	UC link_Octet0;
	// Link(Bit7 ~ Bit0)
	UC link_Octet1;
	// Monitor Operation
	UC monitorOpe;

};

#endif // #ifndef SCdiFinDiagDataExecDc330MonitorFrmt_h

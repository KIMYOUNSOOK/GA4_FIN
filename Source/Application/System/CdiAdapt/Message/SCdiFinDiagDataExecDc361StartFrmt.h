/////////////////////////////////////////////////////////////////////////////////////
//  SCdiFinDiagDataExecDc361StartFrmt.h
//  Implementation of the Class SCdiFinDiagDataExecDc361StartFrmt
//  Created on:      2012/11/22 14:52:54
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#ifndef SCdiFinDiagDataExecDc361StartFrmt_h
#define SCdiFinDiagDataExecDc361StartFrmt_h

#define CDIFIN_DIAG_DATA_EXEC_DC361_START_SIZE				2


///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Attributes Messages::SCdiFinDiagDataExecDc361StartFrmt
// Diagnostic Data Format of "Execute
// Diagnostics / NVM Area Access (dC361) /
// Start" Message
////////////////////////////////////////////////////////////////////////
struct SCdiFinDiagDataExecDc361StartFrmt
{

	// NVM Area
	UC nvmArea;
	// Finisher Device Controller ID
	UC finDevContID;

};

#endif // #ifndef SCdiFinDiagDataExecDc361StartFrmt_h

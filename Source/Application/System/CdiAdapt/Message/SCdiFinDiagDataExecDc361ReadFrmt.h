/////////////////////////////////////////////////////////////////////////////////////
//  SCdiFinDiagDataExecDc361ReadFrmt.h
//  Implementation of the Class SCdiFinDiagDataExecDc361ReadFrmt
//  Created on:      2012/11/22 14:54:06
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#ifndef SCdiFinDiagDataExecDc361ReadFrmt_h
#define SCdiFinDiagDataExecDc361ReadFrmt_h

#define CDIFIN_DIAG_DATA_EXEC_DC361_READ_SIZE				2


///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Attributes Messages::SCdiFinDiagDataExecDc361ReadFrmt
// Diagnostic Data Format of "Execute
// Diagnostics / NVM Area Access (dC361) /
// Read" Message
////////////////////////////////////////////////////////////////////////
struct SCdiFinDiagDataExecDc361ReadFrmt
{

	// NVM Area
	UC nvmArea;
	// Finisher Device Controller ID
	UC finDevContID;

};

#endif // #ifndef SCdiFinDiagDataExecDc361ReadFrmt_h

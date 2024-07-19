/////////////////////////////////////////////////////////////////////////////////////
//  SCdiFinDiagDataExecDc361WriteFrmt.h
//  Implementation of the Class SCdiFinDiagDataExecDc361WriteFrmt
//  Created on:      2012/11/22 14:55:42
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#ifndef SCdiFinDiagDataExecDc361WriteFrmt_h
#define SCdiFinDiagDataExecDc361WriteFrmt_h

#define CDIFIN_DIAG_DATA_EXEC_DC361_WRITE_SIZE				5


///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Attributes Messages::SCdiFinDiagDataExecDc361WriteFrmt
// Diagnostic Data Format of "Execute
// Diagnostics / NVM Area Access (dC361) /
// Write" Message
////////////////////////////////////////////////////////////////////////
struct SCdiFinDiagDataExecDc361WriteFrmt
{

	// NVM Area
	UC nvmArea;
	// Finisher Device Controller ID
	UC finDevContID;
	// Presence Of Remain Data
	UC presenceOfRemain;
	// Allocated Data Size
	UC allocateSize;
	// Allocated Data Value
	UC allocateVal[128];

};

#endif // #ifndef SCdiFinDiagDataExecDc361WriteFrmt_h

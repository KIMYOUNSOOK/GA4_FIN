/////////////////////////////////////////////////////////////////////////////////////
//  SCdiFinDiagDataRespDc361WriteFrmt.h
//  Implementation of the Class SCdiFinDiagDataRespDc361WriteFrmt
//  Created on:      2012/11/22 14:56:15
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#ifndef SCdiFinDiagDataRespDc361WriteFrmt_h
#define SCdiFinDiagDataRespDc361WriteFrmt_h

#define CDIFIN_DIAG_DATA_RESP_DC361_WRITE_SIZE				2


///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Attributes Messages::SCdiFinDiagDataRespDc361WriteFrmt
// Diagnostic Data Format of "Diagnostics
// Response / NVM Access(dC361) / Write"
// Message
////////////////////////////////////////////////////////////////////////
struct SCdiFinDiagDataRespDc361WriteFrmt
{

	// NVM Area
	UC nvmArea;
	// Finisher Device Controller ID
	UC finDevContID;

};

#endif // #ifndef SCdiFinDiagDataRespDc361WriteFrmt_h

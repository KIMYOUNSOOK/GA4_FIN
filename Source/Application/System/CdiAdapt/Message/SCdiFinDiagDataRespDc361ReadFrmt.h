/////////////////////////////////////////////////////////////////////////////////////
//  SCdiFinDiagDataRespDc361ReadFrmt.h
//  Implementation of the Class SCdiFinDiagDataRespDc361ReadFrmt
//  Created on:      2012/11/22 14:54:47
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#ifndef SCdiFinDiagDataRespDc361ReadFrmt_h
#define SCdiFinDiagDataRespDc361ReadFrmt_h

//#define CDIFIN_DIAG_DATA_RESP_DC361_READ_SIZE	// ‰Â•Ï


///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Attributes Messages::SCdiFinDiagDataRespDc361ReadFrmt
// Diagnostic Data Format of "Diagnostics
// Response / NVM Area Access (dC361) /
// Read" Message
////////////////////////////////////////////////////////////////////////
struct SCdiFinDiagDataRespDc361ReadFrmt
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
#endif // #ifndef SCdiFinDiagDataRespDc361ReadFrmt_h

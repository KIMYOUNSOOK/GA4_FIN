/////////////////////////////////////////////////////////////////////////////////////
//  SCdiFinDiagDataRespDc361StartFrmt.h
//  Implementation of the Class SCdiFinDiagDataRespDc361StartFrmt
//  Created on:      2012/11/22 14:53:30
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#ifndef SCdiFinDiagDataRespDc361StartFrmt_h
#define SCdiFinDiagDataRespDc361StartFrmt_h

#define CDIFIN_DIAG_DATA_RESP_DC361_START_SIZE				4


///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Attributes Messages::SCdiFinDiagDataRespDc361StartFrmt
// Diagnostic Data Format of "Diagnostics
// Response / NVM Area Access (dC361) /
// Start" Message
////////////////////////////////////////////////////////////////////////
struct SCdiFinDiagDataRespDc361StartFrmt
{

	// NVM Area
	UC nvmArea;
	// Finisher Device Controller ID
	UC finDevContID;
	// NVM Size(Bit15 ~ Bit8)
	UC nvmSize_Octet0;
	// NVM Size(Bit7 ~ Bit0)
	UC nvmSize_Octet1;

};

#endif // #ifndef SCdiFinDiagDataRespDc361StartFrmt_h

/////////////////////////////////////////////////////////////////////////////////////
//  SCdiFinDiagDataRespDc128WriteFrmt.h
//  Implementation of the Class SCdiFinDiagDataRespDc128WriteFrmt
//  Created on:      2013/04/02 19:50:09
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#ifndef SCdiFinDiagDataRespDc128WriteFrmt_h
#define SCdiFinDiagDataRespDc128WriteFrmt_h

#define CDIFIN_DIAG_DATA_RESP_DC128_WRITE_SIZE				2


///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Adjustment Messages::SCdiFinDiagDataRespDc128WriteFrmt
// Diagnostic Data Format of "Diagnostics
// Response / Fold Position Adjust (dC128)
// / Write" Message
////////////////////////////////////////////////////////////////////////
struct SCdiFinDiagDataRespDc128WriteFrmt
{
	// Fold Device
	UC foldDevice;
	// Update Response
	UC updateResponse;
};

#endif // #ifndef SCdiFinDiagDataRespDc128WriteFrmt_h

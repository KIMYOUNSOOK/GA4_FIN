/////////////////////////////////////////////////////////////////////////////////////
//  SCdiFinDiagDataRespDc128ReadFrmt.h
//  Implementation of the Class SCdiFinDiagDataRespDc128ReadFrmt
//  Created on:      2013/04/02 19:50:00
//  Original author: fxat00694
//  Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
/////////////////////////////////////////////////////////////////////////////////////

#ifndef SCdiFinDiagDataRespDc128ReadFrmt_h
#define SCdiFinDiagDataRespDc128ReadFrmt_h

//#define CDIFIN_DIAG_DATA_RESP_DC128_READ					//‰Â•Ï


///// [ CLASS ] ////////////////////////////////////////////////////////
// Name : Device Adjustment Messages::SCdiFinDiagDataRespDc128ReadFrmt
// Diagnostic Data Format of "Diagnostics
// Response / Fold Position Adjust (dC128)
// / Read" Message
////////////////////////////////////////////////////////////////////////
struct SCdiFinDiagDataRespDc128ReadFrmt
{
	UC foldDevice;
	UC foldAdjustData[99];
};

#endif // #ifndef SCdiFinDiagDataRespDc128ReadFrmt_h

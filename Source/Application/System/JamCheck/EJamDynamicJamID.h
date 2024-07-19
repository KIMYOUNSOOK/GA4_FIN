// File Name:	EJamDynamicJamID.h
// Description:	Implementation of the Enumeration EJamDynamicJamID
// Engineer:	Xxxxxx Xxxxxx
// Last Edit:	XX.XX.XX
// Revision:	XX
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef EJamDynamicJamID_h
#define EJamDynamicJamID_h

typedef enum /*EJamDynamicJamID*/
{
	// !!! Product Specific !!!
	JAM_DJAM_ID_CMPL_EXIT_SNR_ON_JAM,	// Compile Exit Sensor ON Jam
	JAM_DJAM_ID_CMPL_EXIT_SNR_OFF_JAM,	// Compile Exit Sensor OFF Jam
	JAM_DJAM_ID_SET_EJECT_JAM,			// Set Eject Jam
	JAM_DJAM_ID_NUM
} EJamDynamicJamID;

typedef enum /*EJamDynamicJamID*/
{
	// !!! Product Specific !!!
	JAM_DJAM_DISTANSEID_R,	//  IOT筺体の用紙搬出部フレームとIOT Fusing-Roll間パス長
	JAM_DJAM_DISTANSEID_C,	//  IOT筺体の用紙搬出部フレームとIOT Exit Sensor(Hot Line)間パス長
	JAM_DJAM_DISTANSEID_E,	//  IOT筺体の用紙搬出部フレームからENTRANCE-ROLLまでの距離
	JAM_DJAM_DISTANSEID_K,	//  ENTRANCE-ROLLからCompile-Exit Sensorまでの距離
	JAM_DJAM_DISTANSEID_NUM
} EJamDynamicJamID;


#endif // #ifndef EJamDynamicJamID_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

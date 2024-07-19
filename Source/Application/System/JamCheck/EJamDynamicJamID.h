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
	JAM_DJAM_DISTANSEID_R,	//  IOT⟑̗̂p�����o���t���[����IOT Fusing-Roll�ԃp�X��
	JAM_DJAM_DISTANSEID_C,	//  IOT⟑̗̂p�����o���t���[����IOT Exit Sensor(Hot Line)�ԃp�X��
	JAM_DJAM_DISTANSEID_E,	//  IOT⟑̗̂p�����o���t���[������ENTRANCE-ROLL�܂ł̋���
	JAM_DJAM_DISTANSEID_K,	//  ENTRANCE-ROLL����Compile-Exit Sensor�܂ł̋���
	JAM_DJAM_DISTANSEID_NUM
} EJamDynamicJamID;


#endif // #ifndef EJamDynamicJamID_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// File Name:	EShtFinisherSpeedID.h
// Description:	Implementation of the Enumeration EShtFinisherSpeed
// Engineer:	Xxxxxx Xxxxxx
// Last Edit:	XX.XX.XX
// Revision:	XX
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef EShtFinisherSpeedID_h
#define EShtFinisherSpeedID_h

typedef enum /*EShtFinisherSpeedID*/
{
	// !!! Product Specific !!!
	SHT_FIN_SPD_ID_IOT,	
	// SHT_FIN_SPD_ID_NVM1_600,
	// SHT_FIN_SPD_ID_NVM2_600,
	// SHT_FIN_SPD_ID_NVM2_800,
	SHT_FIN_SPD_ID_NVMV3_600,
	SHT_FIN_SPD_ID_NVMV4_700,
	SHT_FIN_SPD_ID_NVMV5_800,	
	SHT_FIN_SPD_ID_NUM				// Number of "Finisher Speed" definition
} EShtFinisherSpeedID;

#define PITCH_SPEEDCHANGE_CPM55		55
#define PITCH_SPEEDCHANGE_CPM65		65


#endif // #ifndef EShtFinisherSpeedID_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

///////////////////////////////////////////////////////////
//  SCdiFinJamZoneUpdateFrmt.h
//  Implementation of the Class SCdiFinJamZoneUpdateFrmt
//  Created on:      2013/12/06 17:31:46
//  Original author: fxat00694
///////////////////////////////////////////////////////////

#if !defined(SCdiFinJamZoneUpdateFrmt_h)
#define SCdiFinJamZoneUpdateFrmt_h

//#define CDIFIN_JAM_ZONE_UPDT_MSG_SIZE	// ‰Â•Ï
#define CDIFIN_JAM_ZONE_UPDT_HEAD_SIZE	2
#define CDIFIN_JAM_ZONE_STTS_SIZE		5


struct SCdiFinJamZoneUpdateFrmt
{
	UC id;								// Message ID
	UC finDevContID;					// Finisher Device Controller ID
	UC jamZone[CDIFIN_JAM_ZONE_STTS_SIZE];	// Jam Zone Status
};

#endif // !defined(SCdiFinJamZoneUpdateFrmt_h)

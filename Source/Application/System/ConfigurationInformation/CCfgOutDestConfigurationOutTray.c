// File Name:	CCfgOutDestConfigurationOutTray.c
// Description:	Implimentation File of Output Destination Configuration
// Engineer:	Yuichi Hoshino
// Last Edit:	14.08.11
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CCfgOutDestConfiguration.h"

#include "COputInformer.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void MakeInformation(CCfgOutDestConfiguration* pThis);
enum ECdiFinCfgAttrKeyOutDest UpdateSelectability(CCfgOutDestConfiguration* pThis, EDeviceID deviceID, EDeviceCondition condition);


/////////////////////////////////////////////////////////////////////////////////////////
// Public Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Standard Override Method Table of Output Destination Capabilities ---
struct CfgOutDestConfigurationOverrideMethod const cCfg_OutDestConfigurationMethodOutTray = {
	MakeInformation,
	UpdateSelectability
};


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : MakeInformation
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void MakeInformation(CCfgOutDestConfiguration* pThis)
{
	// !!! Product Specific !!!
	// --- Selectability ---
	pThis->mSelectability = eCdi_Selectable;

	// --- Current Offset Configuration ---
	pThis->mOffset = eCdi_Selectable;

	// --- Current Staple Configuration ---
	pThis->mSingleFront	 = eCdi_Selectable;

	// --- Current Punch Configuration ---
	// if( COputInformer_GetOptionDetect(OPTID_PUNCHER) == true ) {
	//	pThis->mDoublePunch	= eCdi_Selectable;
	// }
	// else { /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : UpdateSelectability
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
enum ECdiFinCfgAttrKeyOutDest UpdateSelectability(CCfgOutDestConfiguration* pThis, EDeviceID deviceID, EDeviceCondition condition)
{
	// !!! Product Specific !!!
	// Device‚ÌŒÌá’Ê’m‚É‰ž‚¶‚ÄSelectabilityî•ñ‚ðNot Selectable‚É•ÏX‚·‚éB
	// ‚È‚¨A•ÏX‚µ‚½î•ñ‚É‘Î‰ž‚·‚éAttribute Key‚ð•Ô‚·B
	if( condition == DEV_COND_PERM_NG ) {
		switch( deviceID ) {
		case DEVID_OUT_TRAY:
			pThis->mSelectability = eCdi_NotSelectable;

			return eCdiFinCfgKeyOutDest_Select;
		case DEVID_STAPLER:
			pThis->mSingleFront	 = eCdi_NotSelectable;

			return eCdiFinCfgKeyOutDest_Staple;
		case DEVID_FINISHER:
			pThis->mSelectability	= eCdi_NotSelectable;

			return eCdiFinCfgKeyOutDest_Select;
		default:
			break;
		}
	}
	else { /* No Action */ } 

	return eCdiFinCfgKeyOutDest_All;
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

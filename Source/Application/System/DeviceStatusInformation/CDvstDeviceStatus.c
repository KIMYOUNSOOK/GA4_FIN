// File Name:	CDvstDeviceStatus.c
// Description:	Implimentation File of Device Status
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.14
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CDvstDeviceStatus.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Public Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CDvstDeviceStatus_Constructor
// Description   : ƒRƒ“ƒXƒgƒ‰ƒNƒ^
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
void CDvstDeviceStatus_Constructor(CDvstDeviceStatus* pThis)
{
	*(pThis->mpStatusValue) = pThis->mpSpec->defaultStts;
}

//////////////////////////////////////////////////////////////////
// Function Name : CDvstDeviceStatus_GetStatusID
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
US CDvstDeviceStatus_GetStatusID(CDvstDeviceStatus* pThis)
{
	return pThis->mpSpec->id;
}

///////////////////////////////////////////////////////////////////
// Function Name : CDvstDeviceStatus_GetStatusValue
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
US CDvstDeviceStatus_GetStatusValue(CDvstDeviceStatus* pThis)
{
	return *(pThis->mpStatusValue);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDvstDeviceStatus_SetStatusValue
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
bool CDvstDeviceStatus_SetStatusValue(CDvstDeviceStatus* pThis, US statusValue)
{
	if ( *(pThis->mpStatusValue) != statusValue ){
		*(pThis->mpStatusValue) = statusValue;
		return true;
	}
	else{
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CDvstDeviceStatus_GetStatusCategory
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
enum EDvstCategory CDvstDeviceStatus_GetStatusCategory(CDvstDeviceStatus* pThis)
{
	return pThis->mpSpec->category;
}

///////////////////////////////////////////////////////////////////
// Function Name : CDvstDeviceStatus_CheckRelativeDevice
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
bool CDvstDeviceStatus_CheckRelativeDevice(CDvstDeviceStatus* pThis, EDeviceID deviceID)
{
	if ( pThis->mpSpec->deviceBit & DEVID_CNV_BIT(deviceID) ) return true;
	else													  return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CDvstDeviceStatus_GetSubNo
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
UC CDvstDeviceStatus_GetSubNo(CDvstDeviceStatus* pThis)
{
	return pThis->mpSpec->subNo;
}

///////////////////////////////////////////////////////////////////
// Function Name : CDvstDeviceStatus_GetDefaultStatus
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
US CDvstDeviceStatus_GetDefaultStatus(CDvstDeviceStatus* pThis)
{
	return pThis->mpSpec->defaultStts;
}

///////////////////////////////////////////////////////////////////
// Function Name : CDvstDeviceStatus_IsDefaultStatus
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
bool CDvstDeviceStatus_IsDefaultStatus(CDvstDeviceStatus* pThis, US statusValue)
{
	if ( statusValue == pThis->mpSpec->defaultStts ) return true;
	else											 return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CDvstDeviceStatus_GetChainNo
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
US CDvstDeviceStatus_GetChainNo(CDvstDeviceStatus* pThis, US statusValue)
{
	if ( (pThis->mpSpec->pValueInfoTbl != NULL) && (statusValue < pThis->mpSpec->valueInfoTblSize) ){
		return pThis->mpSpec->pValueInfoTbl[statusValue].chain;
	}
	else{
		return 0;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CDvstDeviceStatus_GetLinkNo
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
US CDvstDeviceStatus_GetLinkNo(CDvstDeviceStatus* pThis, US statusValue)
{
	if ( (pThis->mpSpec->pValueInfoTbl != NULL) && (statusValue < pThis->mpSpec->valueInfoTblSize) ){
		return pThis->mpSpec->pValueInfoTbl[statusValue].link;
	}
	else{
		return 0;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CDvstDeviceStatus_CheckDeviceCondition
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
bool CDvstDeviceStatus_CheckDeviceCondition(CDvstDeviceStatus* pThis, US statusValue)
{
	if ( (pThis->mpSpec->ngThresh == 0) || (statusValue < pThis->mpSpec->ngThresh) ) return true;
	else																			 return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CDvstDeviceStatus_GetFinProcessStopType
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
ESysProcStopType CDvstDeviceStatus_GetFinProcessStopType(CDvstDeviceStatus* pThis)
{
	return (ESysProcStopType)pThis->mpSpec->stopType;
}

///////////////////////////////////////////////////////////////////
// Function Name : CDvstDeviceStatus_GetSpecialProcessExecution
// Description   : 
// Parameter     : ]
// Return        : ]
///////////////////////////////////////////////////////////////////
bool CDvstDeviceStatus_GetSpecialProcessExecution(CDvstDeviceStatus* pThis)
{
	return (bool)pThis->mpSpec->specialProcess;
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

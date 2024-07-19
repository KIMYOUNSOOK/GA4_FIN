// File Name:	CdiAdapt.c
// Description:	Implimentation File of CDI Adapt
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.11
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CCdiAdapter.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Attribute Key Array of Finisher Device Controller Capabilities ---
UC const cCdi_CapaAttrKeyArrayDevContAll[] = {
	// !!! Product Specific !!!
	eCdiFinCapaKeyDevCont_All							// End of array
};

UC const cCdi_CapaAttrKeyArrayDevContAries[] = {
	eCdiFinCapaKeyDevCont_SoftVersion,
	eCdiFinCapaKeyDevCont_All							// End of array
};

// --- Attribute Key Array of Finisher Device Controller Configuration ---
UC const cCdi_CfgAttrKeyArrayDevContAll[] = {
	// !!! Product Specific !!!
	eCdiFinCfgKeyDevCont_Market,
	eCdiFinCfgKeyDevCont_DateTime,
	eCdiFinCfgKeyDevCont_SysCfg,
	eCdiFinCfgKeyDevCont_MediaSizeGrp,
	eCdiFinCfgKeyDevCont_Productivity,
	eCdiFinCfgKeyDevCont_ExitSpdRange,
	eCdiFinCfgKeyDevCont_ExitLayout,
	eCdiFinCfgKeyDevCont_JamBypass,
	eCdiFinCfgKeyDevCont_NoPprRun,
	eCdiFinCfgKeyDevCont_All							// End of array
};

UC const cCdi_CfgAttrKeyArrayDevContAries[] = {
	// !!! Product Specific !!!
	eCdiFinCfgKeyDevCont_All							// End of array
};

// --- Instance Data of Finisher Device Controller ---
CdiInstanceData const cCdi_DevContDataArray[] = {
	// !!! Product Specific !!!
	{ eCdiFinDevCont_All,		cCdi_CapaAttrKeyArrayDevContAll,	cCdi_CfgAttrKeyArrayDevContAll		},
	{ eCdiFinDevCont_GA4Fin,		cCdi_CapaAttrKeyArrayDevContAries,	cCdi_CfgAttrKeyArrayDevContAries	},
	{ eCdiFinDevCont_Iot,		NULL,								NULL								}	// End of array
};

// --- Attribute Key Array of Output Destination Capabilities ---
// 2015/09/01 LJE
UC const cCdi_CapaAttrKeyArrayOutDestOutTray[] = {
	// !!! Product Specific !!!
	eCdiFinCapaKeyOutDest_MaxCapa,
	eCdiFinCapaKeyOutDest_UnitType,
	eCdiFinCapaKeyOutDest_Offset,
	eCdiFinCapaKeyOutDest_Staple,
	// eCdiFinCapaKeyOutDest_Punch,
	eCdiFinCapaKeyOutDest_StapleType,
	eCdiFinCapaKeyOutDest_StplSpecMediaType,
	eCdiFinCapaKeyOutDest_All							// End of array
};

// --- Attribute Key Array of Output Destination Configuration ---
// 2015/09/01 LJE
UC const cCdi_CfgAttrKeyArrayOutDestOutTray[] = {
	// !!! Product Specific !!!
	eCdiFinCfgKeyOutDest_Select,
	eCdiFinCfgKeyOutDest_Offset,
	eCdiFinCfgKeyOutDest_Staple,
	// eCdiFinCfgKeyOutDest_Punch,
	eCdiFinCfgKeyOutDest_All							// End of array
};

// --- Instance Data of Output Destination ---
CdiInstanceData const cCdi_OutDestDataArray[] = {
	// !!! Product Specific !!!
	{ eCdiFinOutDest_OutTray,	cCdi_CapaAttrKeyArrayOutDestOutTray,	cCdi_CfgAttrKeyArrayOutDestOutTray	},
	{ eCdiFinOutDest_All,		NULL,									NULL								}	// End of array
};

// --- Attribute Key Array of Input Source Capabilities ---
//UC const cCdi_CapaAttrKeyArrayInSrcXxx[] = {
//	// !!! Product Specific !!!
//	eCdiFinCapaKeyInSrc_All								// End of array
//};

// --- Attribute Key Array of Input Source Configuration ---
//UC const cCdi_CfgAttrKeyArrayInSrcXxx[] = {
//	// !!! Product Specific !!!
//	eCdiFinCfgKeyInSrc_All								// End of array
//};

// --- Instance Data of Input Source ---
CdiInstanceData const cCdi_InSrcDataArray[] = {
	// !!! Product Specific !!!
	{ eCdiFinInSrc_All,			NULL,								NULL							}	// End of array
};

// --- Jam Zone Status Map ---
CdiJamZoneStatusMap const cCdi_JamZoneStatusMap[JAM_ZONE_ID_NUM] = {
	// !!! Product Specific !!!
	{ JAM_ZONE_ID_ENT_PATH,			0,		 0 },
	{ JAM_ZONE_ID_TRANS_PATH,		0,		 1 },
	{ JAM_ZONE_ID_COMPILE_TRAY,		0,		 2 }
};

// --- Jam Zone Data ---
CdiJamZoneData const cCdi_JamZoneDataArray[] = {
	// !!! Product Specific !!!
	{ eCdiFinDevCont_GA4Fin,	JAM_ZONE_ID_NUM,	1,					cCdi_JamZoneStatusMap			}, 
	{ eCdiFinDevCont_Iot,	0,					0,					NULL							}	// End of array
};


/////////////////////////////////////////////////////////////////////////////////////////
// Event Call Back Function
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CDI_ComFailNotify
// Description   : �ʐM�m�������ʒm�R�[���o�b�N�֐�
// Parameter     : pMessage (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���M��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CDI_ComSyncNotify(const UC* evt, UC from)
{
	CCdiAdapter_CompleteSync();
}

///////////////////////////////////////////////////////////////////
// Function Name : CDI_ComFailNotify
// Description   : �ʐM�t�F�C�����m�ʒm�R�[���o�b�N�֐�
// Parameter     : pMessage (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���M��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CDI_ComFailNotify(const UC* evt, UC from)
{
	CCdiAdapter_FailedCommunication();
}

///////////////////////////////////////////////////////////////////
// Function Name : CDI_RcvDevCapabilitiesRequest
// Description   : ��M�C�x���g�R�[���o�b�N
// Parameter     : pMessage (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���M��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CDI_RcvDevCapabilitiesRequest(const UC* pMessage, UC from)
{
	CCdiFinMessage message;
	CCdiFinMessage_SetMessage(&message, ((CommandEvent *)pMessage)->mCommand, ((CommandEvent *)pMessage)->mLength);

	CCdiAdapter_RcvDevCapabilitiesRequest(&message);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDI_RcvDevConfigurationRequest
// Description   : ��M�C�x���g�R�[���o�b�N
// Parameter     : pMessage (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���M��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CDI_RcvDevConfigurationRequest(const UC* pMessage, UC from)
{
	CCdiFinMessage message;
	CCdiFinMessage_SetMessage(&message, ((CommandEvent *)pMessage)->mCommand, ((CommandEvent *)pMessage)->mLength);

	CCdiAdapter_RcvDevConfigurationRequest(&message);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDI_RcvSetDevConfiguration
// Description   : ��M�C�x���g�R�[���o�b�N
// Parameter     : pMessage (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���M��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CDI_RcvSetDevConfiguration(const UC* pMessage, UC from)
{
	CCdiFinMessage message;
	CCdiFinMessage_SetMessage(&message, ((CommandEvent *)pMessage)->mCommand, ((CommandEvent *)pMessage)->mLength);

	CCdiAdapter_RcvSetDevConfiguration(&message);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDI_RcvInitRequest
// Description   : ��M�C�x���g�R�[���o�b�N
// Parameter     : pMessage (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���M��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CDI_RcvInitRequest(const UC* pMessage, UC from)
{
	CCdiFinMessage message;
	CCdiFinMessage_SetMessage(&message, ((CommandEvent *)pMessage)->mCommand, ((CommandEvent *)pMessage)->mLength);

	CCdiAdapter_RcvInitRequest(&message);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDI_RcvInitAcknowledge
// Description   : ��M�C�x���g�R�[���o�b�N
// Parameter     : pMessage (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���M��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CDI_RcvInitAcknowledge(const UC* pMessage, UC from)
{
	CCdiFinMessage message;
	CCdiFinMessage_SetMessage(&message, ((CommandEvent *)pMessage)->mCommand, ((CommandEvent *)pMessage)->mLength);

	CCdiAdapter_RcvInitAcknowledge(&message);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDI_RcvInitComplete
// Description   : ��M�C�x���g�R�[���o�b�N
// Parameter     : pMessage (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���M��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CDI_RcvInitComplete(const UC* pMessage, UC from)
{
	CCdiFinMessage message;
	CCdiFinMessage_SetMessage(&message, ((CommandEvent *)pMessage)->mCommand, ((CommandEvent *)pMessage)->mLength);

	CCdiAdapter_RcvInitComplete(&message);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDI_RcvPwrOffAnnouncement
// Description   : ��M�C�x���g�R�[���o�b�N
// Parameter     : pMessage (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���M��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CDI_RcvPwrOffAnnouncement(const UC* pMessage, UC from)
{
	CCdiFinMessage message;
	CCdiFinMessage_SetMessage(&message, ((CommandEvent *)pMessage)->mCommand, ((CommandEvent *)pMessage)->mLength);

	CCdiAdapter_RcvPwrOffAnnouncement(&message);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDI_RcvPollRequest
// Description   : ��M�C�x���g�R�[���o�b�N
// Parameter     : pMessage (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���M��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CDI_RcvPollRequest(const UC* pMessage, UC from)
{
	CCdiFinMessage message;
	CCdiFinMessage_SetMessage(&message, ((CommandEvent *)pMessage)->mCommand, ((CommandEvent *)pMessage)->mLength);

	CCdiAdapter_RcvPollRequest(&message);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDI_RcvSelectFinAttribute
// Description   : ��M�C�x���g�R�[���o�b�N
// Parameter     : pMessage (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���M��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CDI_RcvSelectFinAttribute(const UC* pMessage, UC from)
{
	CCdiFinMessage message;
	CCdiFinMessage_SetMessage(&message, ((CommandEvent *)pMessage)->mCommand, ((CommandEvent *)pMessage)->mLength);

	CCdiAdapter_RcvSelectFinAttribute(&message);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDI_RcvSheetFinRequest
// Description   : ��M�C�x���g�R�[���o�b�N
// Parameter     : pMessage (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���M��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CDI_RcvSheetFinRequest(const UC* pMessage, UC from)
{
	CCdiFinMessage message;
	CCdiFinMessage_SetMessage(&message, ((CommandEvent *)pMessage)->mCommand, ((CommandEvent *)pMessage)->mLength);

	CCdiAdapter_RcvSheetFinRequest(&message);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDI_RcvChangeShtFinRequest
// Description   : ��M�C�x���g�R�[���o�b�N
// Parameter     : pMessage (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���M��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CDI_RcvChangeShtFinRequest(const UC* pMessage, UC from)
{
	CCdiFinMessage message;
	CCdiFinMessage_SetMessage(&message, ((CommandEvent *)pMessage)->mCommand, ((CommandEvent *)pMessage)->mLength);

	CCdiAdapter_RcvChangeShtFinRequest(&message);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDI_RcvCancelShtFinRequest
// Description   : ��M�C�x���g�R�[���o�b�N
// Parameter     : pMessage (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���M��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CDI_RcvCancelShtFinRequest(const UC* pMessage, UC from)
{
	CCdiFinMessage message;
	CCdiFinMessage_SetMessage(&message, ((CommandEvent *)pMessage)->mCommand, ((CommandEvent *)pMessage)->mLength);

	CCdiAdapter_RcvCancelShtFinRequest(&message);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDI_RcvSheetExit
// Description   : ��M�C�x���g�R�[���o�b�N
// Parameter     : pMessage (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���M��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CDI_RcvSheetExit(const UC* pMessage, UC from)
{
	CCdiFinMessage message;
	CCdiFinMessage_SetMessage(&message, ((CommandEvent *)pMessage)->mCommand, ((CommandEvent *)pMessage)->mLength);

	CCdiAdapter_RcvSheetExit(&message);	
}

///////////////////////////////////////////////////////////////////
// Function Name : CDI_RcvSetFinRequest
// Description   : ��M�C�x���g�R�[���o�b�N
// Parameter     : pMessage (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���M��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CDI_RcvSetFinRequest(const UC* pMessage, UC from)
{
	CCdiFinMessage message;
	CCdiFinMessage_SetMessage(&message, ((CommandEvent *)pMessage)->mCommand, ((CommandEvent *)pMessage)->mLength);

	CCdiAdapter_RcvSetFinRequest(&message);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDI_RcvStopFinisher
// Description   : ��M�C�x���g�R�[���o�b�N
// Parameter     : pMessage (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���M��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CDI_RcvStopFinisher(const UC* pMessage, UC from)
{
	CCdiFinMessage message;
	CCdiFinMessage_SetMessage(&message, ((CommandEvent *)pMessage)->mCommand, ((CommandEvent *)pMessage)->mLength);

	CCdiAdapter_RcvStopFinisher(&message);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDI_RcvReqJamSttsCheck
// Description   : ��M�C�x���g�R�[���o�b�N
// Parameter     : pMessage (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���M��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CDI_RcvReqJamSttsCheck(const UC* pMessage, UC from)
{
	CCdiFinMessage message;
	CCdiFinMessage_SetMessage(&message, ((CommandEvent *)pMessage)->mCommand, ((CommandEvent *)pMessage)->mLength);

	CCdiAdapter_RcvReqJamSttsCheck(&message);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDI_RcvDevicePauseReq
// Description   : ��M�C�x���g�R�[���o�b�N
// Parameter     : pMessage (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���M��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CDI_RcvDevicePauseReq(const UC* pMessage, UC from)
{
	CCdiFinMessage message;
	CCdiFinMessage_SetMessage(&message, ((CommandEvent *)pMessage)->mCommand, ((CommandEvent *)pMessage)->mLength);

	CCdiAdapter_RcvDevicePauseReq(&message);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDI_RcvDevContActionReq
// Description   : ��M�C�x���g�R�[���o�b�N
// Parameter     : pMessage (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���M��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CDI_RcvDevContActionReq(const UC* pMessage, UC from)
{
	CCdiFinMessage message;
	CCdiFinMessage_SetMessage(&message, ((CommandEvent *)pMessage)->mCommand, ((CommandEvent *)pMessage)->mLength);

	CCdiAdapter_RcvDevContActionReq(&message);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDI_RcvChangeDevContMode
// Description   : ��M�C�x���g�R�[���o�b�N
// Parameter     : pMessage (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���M��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CDI_RcvChangeDevContMode(const UC* pMessage, UC from)
{
	CCdiFinMessage message;
	CCdiFinMessage_SetMessage(&message, ((CommandEvent *)pMessage)->mCommand, ((CommandEvent *)pMessage)->mLength);

	CCdiAdapter_RcvChangeDevContMode(&message);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDI_RcvDevSttsRequest
// Description   : ��M�C�x���g�R�[���o�b�N
// Parameter     : pMessage (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���M��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CDI_RcvDevSttsRequest(const UC* pMessage, UC from)
{
	CCdiFinMessage message;
	CCdiFinMessage_SetMessage(&message, ((CommandEvent *)pMessage)->mCommand, ((CommandEvent *)pMessage)->mLength);

	CCdiAdapter_RcvDeviceStatusRequest(&message);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDI_RcvSysContInfo
// Description   : ��M�C�x���g�R�[���o�b�N
// Parameter     : pMessage (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���M��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CDI_RcvSysContInfo(const UC* pMessage, UC from)
{
	CCdiFinMessage message;
	CCdiFinMessage_SetMessage(&message, ((CommandEvent *)pMessage)->mCommand, ((CommandEvent *)pMessage)->mLength);

	CCdiAdapter_RcvSysContInfo(&message);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDI_RcvExecuteDiag
// Description   : ��M�C�x���g�R�[���o�b�N
// Parameter     : pMessage (���b�Z�[�W�̐擪�A�h���X)
//               : from (�C�x���g���M��)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CDI_RcvExecuteDiag(const UC* pMessage, UC from)
{
	CCdiFinMessage message;
	CCdiFinMessage_SetMessage(&message, ((CommandEvent *)pMessage)->mCommand, ((CommandEvent *)pMessage)->mLength);

	CCdiAdapter_RcvExecuteDiag(&message);
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

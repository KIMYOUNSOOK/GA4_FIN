// File Name:	CInitDevInitHandler.h
// Description:	Header File of Device Initialize Handler
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.22
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(CInitDevInitHandler_h)
#define CInitDevInitHandler_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////
#include "FinisherCommon.h"

#include "EInitInitializeState.h"
#include "ESysPowerUpMode.h"
#include "ESysDeviceAction.h"
#include "SRVC_MsgActivity.h"

/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Spec data of Device Initialize Handler ---
typedef struct /*InitDevInitHandlerSpec*/
{
	CSrvcMsgActivity* pPwrOnInitActivity;					// �p���[�I���C�j�V�����C�Y����A�N�e�B�r�e�B�C���X�^���X�|�C���^
	CSrvcMsgActivity* pPwrRsmInitActivity;					// �p���[���Y�[���C�j�V�����C�Y����A�N�e�B�r�e�B�C���X�^���X�|�C���^
	CSrvcMsgActivity* pNormalInitActivity;					// �ʏ�菇�C�j�V�����C�Y����A�N�e�B�r�e�B�C���X�^���X�|�C���^
	CSrvcMsgActivity* pJamClrAssistActivity;				// �W�����N���A�A�V�X�g����A�N�e�B�r�e�B�C���X�^���X�|�C���^
} InitDevInitHandlerSpec;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
void CInitDevInitHandler_Constructor(const InitDevInitHandlerSpec* pSpec);
void CInitDevInitHandler_Destructor();

EInitInitializeState CInitDevInitHandler_GetState();
bool CInitDevInitHandler_Reset();
bool CInitDevInitHandler_Start(ESysPowerUpMode mode);
bool CInitDevInitHandler_Stop();
void CInitDevInitHandler_DeviceActionResponse(ESysDeviceAction action, UC moduleID);


#endif // !defined(CInitDevInitHandler_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

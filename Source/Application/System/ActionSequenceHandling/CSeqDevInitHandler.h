// File Name:	CSeqDevInitHandler.h
// Description:	Header File of Device Initialize Handler
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.22
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CSeqDevInitHandler_h
#define CSeqDevInitHandler_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ESeqActionState.h"
#include "ESysPowerUpMode.h"

#include "SRVC_MsgActivity.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Spec data of Device Initialize Handler ---
typedef struct /*SeqDevInitHandlerSpec*/
{
	const SrvcMsgActivity* pPwrOnInitBeforeJamCheckScenario;					// �p���[�I���C�j�V�����C�Y����(�c�������o����O)�A�N�e�B�r�e�B�V�i���I�f�[�^�|�C���^
	UC pwrOnInitBeforeJamCheckScenarioSize;										// �p���[�I���C�j�V�����C�Y����(�c�������o����O)�A�N�e�B�r�e�B�V�i���I�f�[�^�T�C�Y
	const SrvcMsgActivity* pPwrOnInitAfterJamCheckScenario;						// �p���[�I���C�j�V�����C�Y����(�c�������o�����)�A�N�e�B�r�e�B�V�i���I�f�[�^�|�C���^
	UC pwrOnInitAfterJamCheckScenarioSize;										// �p���[�I���C�j�V�����C�Y����(�c�������o�����)�A�N�e�B�r�e�B�V�i���I�f�[�^�T�C�Y
	const SrvcMsgActivity* pPwrRsmInitBeforeJamCheckScenario;					// �p���[���Y�[���C�j�V�����C�Y����(�c�������o����O)�A�N�e�B�r�e�B�V�i���I�f�[�^�|�C���^
	UC pwrRsmInitBeforeJamCheckScenarioSize;									// �p���[���Y�[���C�j�V�����C�Y����(�c�������o����O)�A�N�e�B�r�e�B�V�i���I�f�[�^�T�C�Y
	const SrvcMsgActivity* pPwrRsmInitAfterJamCheckScenario;					// �p���[���Y�[���C�j�V�����C�Y����(�c�������o�����)�A�N�e�B�r�e�B�V�i���I�f�[�^�|�C���^
	UC pwrRsmInitAfterJamCheckScenarioSize;										// �p���[���Y�[���C�j�V�����C�Y����(�c�������o�����)�A�N�e�B�r�e�B�V�i���I�f�[�^�T�C�Y
	const SrvcMsgActivity* pNormalInitBeforeJamCheckScenario;					// �ʏ�菇�C�j�V�����C�Y����(�c�������o����O)�A�N�e�B�r�e�B�V�i���I�f�[�^�|�C���^
	UC normalInitBeforeJamCheckScenarioSize;									// �ʏ�菇�C�j�V�����C�Y����(�c�������o����O)�A�N�e�B�r�e�B�V�i���I�f�[�^�T�C�Y
	const SrvcMsgActivity* pNormalInitAfterJamCheckScenario;					// �ʏ�菇�C�j�V�����C�Y����(�c�������o�����)�A�N�e�B�r�e�B�V�i���I�f�[�^�|�C���^
	UC normalInitAfterJamCheckScenarioSize;										// �ʏ�菇�C�j�V�����C�Y����(�c�������o�����)�A�N�e�B�r�e�B�V�i���I�f�[�^�T�C�Y
	const SrvcMsgActivity* pJamCheckScenario;									// �c�������o����A�N�e�B�r�e�B�V�i���I�f�[�^�|�C���^
	UC jamCheckScenarioSize;													// �c�������o����A�N�e�B�r�e�B�V�i���I�f�[�^�T�C�Y
	const SrvcMsgActivity* pJamClrAssistScenario;								// �W�����N���A�A�V�X�g����A�N�e�B�r�e�B�V�i���I�f�[�^�|�C���^
	UC jamClrAssistScenarioSize;												// �W�����N���A�A�V�X�g����A�N�e�B�r�e�B�V�i���I�f�[�^�T�C�Y
} SeqDevInitHandlerSpec;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CSeqDevInitHandler_Constructor();

ESeqActionState CSeqDevInitHandler_GetState();
ESeqActionState CSeqDevInitHandler_Reset();
ESeqActionState CSeqDevInitHandler_Start(ESysPowerUpMode mode);
ESeqActionState CSeqDevInitHandler_Stop();
ESeqActionState CSeqDevInitHandler_DeviceActionResponse(US eventID, UC moduleID);
ESeqActionState CSeqDevInitHandler_Restart();


#endif // #ifndef CSeqDevInitHandler_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

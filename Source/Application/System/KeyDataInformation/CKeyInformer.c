// File Name:	CKeyInformer.c
// Description:	Implimentation File of Key Data Informer
// Engineer:	Yuichi Hoshino
// Last Edit:	16.02.09
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CKeyInformer.h"

#include "KeyArea.h"

#include "CSysManager.h"
#include "CNmaHandler.h" 


/////////////////////////////////////////////////////////////////////////////////////////
// Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Type Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Member of Key Data Informer ---
typedef struct /*CKeyInformerMember*/
{
	KEY_AREA_TYPE mKeyRamArea;
	UC mNvmChange;
} CKeyInformerMember;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Member of Key Data Informer ---
static CKeyInformerMember gKey_InformerMember;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

static void LoadNvmComplete(SS result);
static void SaveNvmComplete(SS result);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_Constructor
// Description   : �R���X�g���N�^
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CKeyInformer_Constructor()
{
	UC index;

	gKey_InformerMember.mKeyRamArea.SIZE.KEYCODE = 0;
	gKey_InformerMember.mKeyRamArea.SIZE.DLMODE  = DOWNLOAD_MODE_COMPLETE;
	gKey_InformerMember.mKeyRamArea.SIZE.MAJOR_V = 0;
	gKey_InformerMember.mKeyRamArea.SIZE.MINOR_V = 0;
	gKey_InformerMember.mKeyRamArea.SIZE.PATCH_V = 0;
	gKey_InformerMember.mKeyRamArea.SIZE.SEQ_BIT = 0;
	gKey_InformerMember.mKeyRamArea.SIZE.WRTCNT  = 0;
	gKey_InformerMember.mKeyRamArea.SIZE.IPL_V	 = 0;

	for ( index = 0; index < (KEY_AREA_SIZE - KEY_AREA_USE_SIZE); index++ ){
		gKey_InformerMember.mKeyRamArea.SIZE.CONFIG[index] = 0;
	}

	gKey_InformerMember.mNvmChange = KEY_NO_CHANGE;
}

///////////////////////////////////////////////////////////////////
// Function Name : LoadNvmComplete
// Description   : NVM�̃��[�h����������
// Parameter     : result
// Return        : �]
///////////////////////////////////////////////////////////////////
void LoadNvmComplete(SS result)
{
	CNmaHandler_LoadNvmComplete(NMA_INFORMER_KEY, result);
}

///////////////////////////////////////////////////////////////////
// Function Name : SaveNvmComplete
// Description   : NVM�̃Z�[�u����������
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void SaveNvmComplete(SS result)
{
	CNmaHandler_SaveNvmComplete(NMA_INFORMER_KEY, result);
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_LoadNvm
// Description   : NVM�̃��[�h�˗�
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CKeyInformer_LoadNvm()
{
	DD_NVM_Load(DD_NVM_AREA_KEY, (UC *)(&(gKey_InformerMember.mKeyRamArea)), KEY_AREA_SIZE, LoadNvmComplete);
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_SaveNvm
// Description   : NVM�̃Z�[�u�˗�
// Parameter     : �]
// Return        : true = �Z�[�u�˗���(�Z�[�u�����J�n)
//               : false = �Z�[�u��������
///////////////////////////////////////////////////////////////////
bool CKeyInformer_SaveNvm()
{
	if ( gKey_InformerMember.mNvmChange == KEY_CHANGE ){
		(gKey_InformerMember.mKeyRamArea.SIZE.WRTCNT)++;

		DD_NVM_Save(DD_NVM_AREA_KEY, (UC *)(&(gKey_InformerMember.mKeyRamArea)), KEY_AREA_SIZE, SaveNvmComplete);

		return true;
	}
	else{
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_CheckKeyCode
// Description   : KeyCode�̏������ς݃`�F�b�N
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
bool CKeyInformer_CheckKeyCode()
{
	if ( gKey_InformerMember.mKeyRamArea.SIZE.KEYCODE == KEY_CODE ) return true;
	else															return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_SetKeyCode
// Description   : KeyCode�֏������ς݂�ݒ�
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CKeyInformer_SetKeyCode()
{
	if ( gKey_InformerMember.mKeyRamArea.SIZE.KEYCODE != KEY_CODE ){
		gKey_InformerMember.mKeyRamArea.SIZE.KEYCODE = KEY_CODE;

		gKey_InformerMember.mNvmChange = KEY_CHANGE;
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_ClearKeyCode
// Description   : KeyCode�֖���������ݒ�
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CKeyInformer_ClearKeyCode()
{
	if ( gKey_InformerMember.mKeyRamArea.SIZE.KEYCODE == KEY_CODE ){
		gKey_InformerMember.mKeyRamArea.SIZE.KEYCODE = 0;

		gKey_InformerMember.mNvmChange = KEY_CHANGE;
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_CheckDownloadMode
// Description   : DownloadMode�`�F�b�N
// Parameter     : �]
// Return        : 
///////////////////////////////////////////////////////////////////
bool CKeyInformer_CheckDownloadMode()
{
	if ( gKey_InformerMember.mKeyRamArea.SIZE.DLMODE == DOWNLOAD_MODE_COMPLETE ) return false;
	else																		 return true;
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_SetDownloadMode
// Description   : DownloadMode�ݒ�
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CKeyInformer_SetDownloadMode()
{
	if ( gKey_InformerMember.mKeyRamArea.SIZE.DLMODE != DOWNLOAD_MODE_CHANGE ){
		gKey_InformerMember.mKeyRamArea.SIZE.DLMODE = DOWNLOAD_MODE_CHANGE;

		gKey_InformerMember.mNvmChange = KEY_CHANGE;
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_ClearDownloadMode
// Description   : DownloadMode����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CKeyInformer_ClearDownloadMode()
{
	if ( gKey_InformerMember.mKeyRamArea.SIZE.DLMODE != DOWNLOAD_MODE_COMPLETE ){
		gKey_InformerMember.mKeyRamArea.SIZE.DLMODE = DOWNLOAD_MODE_COMPLETE;

		gKey_InformerMember.mNvmChange = KEY_CHANGE;
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_SetMajorVersion
// Description   : Major Version�ݒ�
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CKeyInformer_SetMajorVersion(UC version)
{
	if ( gKey_InformerMember.mKeyRamArea.SIZE.MAJOR_V != version ){
		gKey_InformerMember.mKeyRamArea.SIZE.MAJOR_V = version;

		gKey_InformerMember.mNvmChange = KEY_CHANGE;
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_GetMajorVersion
// Description   : Major Version�擾
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
UC CKeyInformer_GetMajorVersion()
{
	return gKey_InformerMember.mKeyRamArea.SIZE.MAJOR_V;
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_SetMinorVersion
// Description   : Minor Version�ݒ�
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CKeyInformer_SetMinorVersion(UC version)
{
	if ( gKey_InformerMember.mKeyRamArea.SIZE.MINOR_V != version ){
		gKey_InformerMember.mKeyRamArea.SIZE.MINOR_V = version;

		gKey_InformerMember.mNvmChange = KEY_CHANGE;
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_GetMinorVersion
// Description   : Minor Version�擾
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
UC CKeyInformer_GetMinorVersion()
{
	return gKey_InformerMember.mKeyRamArea.SIZE.MINOR_V;
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_SetPatchVersion
// Description   : Patch Version�ݒ�
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CKeyInformer_SetPatchVersion(UC version)
{
	if ( gKey_InformerMember.mKeyRamArea.SIZE.PATCH_V != version ){
		gKey_InformerMember.mKeyRamArea.SIZE.PATCH_V = version;

		gKey_InformerMember.mNvmChange = KEY_CHANGE;
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_GetPatchVersion
// Description   : Patch Version�擾
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
UC CKeyInformer_GetPatchVersion()
{
	return gKey_InformerMember.mKeyRamArea.SIZE.PATCH_V;
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_GetWriteCount
// Description   : �������݉񐔎擾
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
UL CKeyInformer_GetWriteCount()
{
	return gKey_InformerMember.mKeyRamArea.SIZE.WRTCNT;
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_SetRxSequenceBit 
// Description   : Rx�V�[�P���X�r�b�g�Z�b�g����
// Parameter     : seqBit (�V�[�P���X�r�b�g) [0/1]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CKeyInformer_SetRxSequenceBit(UC seqBit)
{
//	if ( ((gKey_InformerMember.mKeyRamArea.SIZE.SEQ_BIT >> SEQUENCE_BIT_SAVE_POS_RX) & 0x01) != seqBit ){
//																				// Download����Download Mode�������݂�����A�K���������ݏ����ΏۂƂȂ邽�ߕω��`�F�b�N�͍s��Ȃ�
		gKey_InformerMember.mKeyRamArea.SIZE.SEQ_BIT &= ~(SEQUENCE_BIT_SAVE_POS_PTN(SEQUENCE_BIT_SAVE_POS_RX));
																				// �Ώۃr�b�g���N���A
		gKey_InformerMember.mKeyRamArea.SIZE.SEQ_BIT |= ((seqBit & 0x01) << SEQUENCE_BIT_SAVE_POS_RX);

		gKey_InformerMember.mNvmChange = KEY_CHANGE;
//	}
//	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_GetRxSequenceBit
// Description   : Rx�V�[�P���X�r�b�g�擾����
// Parameter     : �]
// Return        : �V�[�P���X�r�b�g [0/1]
///////////////////////////////////////////////////////////////////
UC CKeyInformer_GetRxSequenceBit()
{
	return (gKey_InformerMember.mKeyRamArea.SIZE.SEQ_BIT >> SEQUENCE_BIT_SAVE_POS_RX) & 0x01;
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_SetTxSequenceBit
// Description   : Tx�V�[�P���X�r�b�g�Z�b�g����
// Parameter     : seqBit (�V�[�P���X�r�b�g) [0/1]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CKeyInformer_SetTxSequenceBit(UC seqBit)
{
//	if ( ((gKey_InformerMember.mKeyRamArea.SIZE.SEQ_BIT >> SEQUENCE_BIT_SAVE_POS_TX) & 0x01) != seqBit ){
//																				// Download����Download Mode�������݂�����A�K���������ݏ����ΏۂƂȂ邽�ߕω��`�F�b�N�͍s��Ȃ�
		gKey_InformerMember.mKeyRamArea.SIZE.SEQ_BIT &= ~(SEQUENCE_BIT_SAVE_POS_PTN(SEQUENCE_BIT_SAVE_POS_TX));
																				// �Ώۃr�b�g���N���A
		gKey_InformerMember.mKeyRamArea.SIZE.SEQ_BIT |= ((seqBit & 0x01) << SEQUENCE_BIT_SAVE_POS_TX);

		gKey_InformerMember.mNvmChange = KEY_CHANGE;
//	}
//	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_GetTxSequenceBit
// Description   : Tx�V�[�P���X�r�b�g�擾����
// Parameter     : �]
// Return        : �V�[�P���X�r�b�g [0/1]
///////////////////////////////////////////////////////////////////
UC CKeyInformer_GetTxSequenceBit()
{
	return (gKey_InformerMember.mKeyRamArea.SIZE.SEQ_BIT >> SEQUENCE_BIT_SAVE_POS_TX) & 0x01;
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_SetIplVersion
// Description   : IPL Version�Z�b�g����
// Parameter     : version (IPL Version)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CKeyInformer_SetIplVersion(UC version)
{
	if ( gKey_InformerMember.mKeyRamArea.SIZE.IPL_V != version ){ 
		gKey_InformerMember.mKeyRamArea.SIZE.IPL_V = version;

		gKey_InformerMember.mNvmChange = KEY_CHANGE;
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CKeyInformer_GetIplVersion
// Description   : IPL Version�擾����
// Parameter     : �]
// Return        : IPL Version
///////////////////////////////////////////////////////////////////
UC CKeyInformer_GetIplVersion()
{
	return gKey_InformerMember.mKeyRamArea.SIZE.IPL_V;
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 15.03.31	| Yuichi Hoshino	| Created this file based on Base SW V1
// 15.06.19	| Yuichi Hoshino	| Introduction Spec [ NVM Fail�������O���M ]
// 15.06.19	| Yuichi Hoshino	| Release Revision 00 to GA-Finisher V0.1.0 Make.
// 16.02.09	| Yuichi Hoshino	| Introduction Spec [ Download ]
// 16.03.01	| Yuichi Hoshino	| Release Revision 01 to GA-Finisher V1.20.0 Make.
//

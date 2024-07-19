// File Name:	CNvmInformer.c
// Description:	Implimentation File of NVM Informer
// Engineer:	Yuichi Hoshino
// Last Edit:	16.02.26
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CNvmInformer.h"

#include "NvmInst.h"
#include "NvmRam.h"
#include "NvmAutoInitialize.h"
#include "NvmAllRamTable.h"

#include "CNmaHandler.h"

#include "DD_NVM.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- VersionUp/Down ---
#define NVM_VERSION_DOWN			(-1)		// VersionDown
#define NVM_VERSION_NO_CHANGE		0			// Version�ω��Ȃ�
#define NVM_VERSION_UP				1			// VersionUp

// --- NVM Change ---
#define NVM_NO_CHANGE				0			// NVM�ω��Ȃ�
#define NVM_CHANGE					1			// NVM�ω�����

static UC g_NvmChange;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
SS CNvmInformer_CompareVersion(US currentMajor, US currentMinor, US previousMajor, US previousMinor);
void CNvmInformer_AutoInitialize(US previousMajor, US previousMinor);
void CNvmInformer_LoadNvmComplete(SS result);
void CNvmInformer_SaveNvmComplete(SS result);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_Constructor
// Description   : �R���X�g���N�^
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CNvmInformer_Constructor()
{
	US nvmID;
	
	/* NVM������ */
	for ( nvmID = 0; nvmID < NVMID_NUM; nvmID++ ){
		CNvmNVM_Constructor(&(nvmAllData[nvmID]));
	}
	
	/* ���[�J���f�[�^������ */
	g_NvmChange = NVM_NO_CHANGE;
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_LoadNvm
// Description   : NVM�f�[�^���[�h����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CNvmInformer_LoadNvm()
{
	DD_NVM_Load(DD_NVM_AREA_NVM, nvmRamArea, NVM_RAM_SIZE, CNvmInformer_LoadNvmComplete);
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_SaveNvm
// Description   : NVM�f�[�^�Z�[�u����
// Parameter     : �]
// Return        : true = Save�������{
//               : false = Save���������{
///////////////////////////////////////////////////////////////////
bool CNvmInformer_SaveNvm()
{
	if ( g_NvmChange == NVM_CHANGE ) {
		DD_NVM_Save(DD_NVM_AREA_NVM, nvmRamArea, NVM_RAM_SIZE, CNvmInformer_SaveNvmComplete);
		return true;
	}
	else {
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_FirstPowerOnInitialize
// Description   : FirstPowerOn����������
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CNvmInformer_FirstPowerOnInitialize()
{
	US i;
	
	/* �S�Ă�NVM�������� */
	for ( i = 0; i < NVMID_NUM; i++ ) {
		CNvmNVM_Initialize(&nvmAllData[i]);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_VersionUpInitialize
// Description   : Version Up������(Market����)����
// Parameter     : previousMajor �O��MajorVersion
//                 previousMinor �O��MinorVersion
// Return        : �]
// Note          : Market���Ƃ̏����l��ݒ肷��
///////////////////////////////////////////////////////////////////
void CNvmInformer_VersionUpInitialize(US previousMajor, US previousMinor)
{
	US i;
	
	/* Version Up���̂ݏ��������s�� */
	if ( CNvmInformer_CompareVersion(MAJOR_VERSION, MINOR_VERSION, previousMajor, previousMinor) == NVM_VERSION_UP ) {
		CNvmInformer_AutoInitialize(previousMajor, previousMinor);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_OutRangeInitialize
// Description   : �͈͊ONVM����������
// Parameter     : �]
// Return        : �]
// Note          : �]
///////////////////////////////////////////////////////////////////
void CNvmInformer_OutRangeInitialize()
{
	US i;
	
	/* �͈͊O��NVM������������ */
	for ( i = 0; i < NVMID_NUM; i++ ) {
		if ( CNvmNVM_IsOutRange(&nvmAllData[i]) ) {
			CNvmNVM_Initialize(&nvmAllData[i]);
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_DiagInitialize
// Description   : �������֎~�łȂ�NVM�̑S����������
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CNvmInformer_DiagInitialize()
{
	US i;
	
	/* �������֎~�łȂ�NVM�̂ݏ����� */
	for ( i = 0; i < NVMID_NUM; i++ ) {
		if ( CNvmNVM_IsFactoryInitAllowed(&nvmAllData[i]) ) {
			CNvmNVM_Initialize(&nvmAllData[i]);
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_Get
// Description   : NVM ID�ɑΉ������C���X�^���X�|�C���^�̎擾
// Parameter     : id NVM ID
// Return        : �C���X�^���X�|�C���^
///////////////////////////////////////////////////////////////////
CNvmNVM* CNvmInformer_Get(ENvmID id)
{
	if ( id >= NVMID_NUM ) {
		return NULL;
	}
	return &nvmAllData[id];
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_DiagGet
// Description   : Chain Link�ɑΉ������C���X�^���X�|�C���^�̎擾
// Parameter     : chain Chain�ԍ�
//                 link Link�ԍ�
// Return        : �C���X�^���X�̃|�C���^(NVM�������ꍇ��NULL)
///////////////////////////////////////////////////////////////////
CNvmNVM* CNvmInformer_DiagGet(US chain, US link)
{
	US i;
	
	for ( i = 0; i < NVMID_NUM; i++ ) {
		if ( ( nvmAllData[i].mpSpec->chain == chain ) && ( nvmAllData[i].mpSpec->link == link ) ) {
			return &nvmAllData[i];
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_ReadArea
// Description   : NVM�G���A���I�t�Z�b�g�ʒu����w��T�C�Y���ǂݏo��
// Parameter     : offset �ǂݍ��݊J�n�ʒu
//                 data �ǂݏo����f�[�^
//                 size �ǂݏo���f�[�^�T�C�Y
// Return        : �]
///////////////////////////////////////////////////////////////////
void CNvmInformer_ReadArea(US offset, UC* pData, UC size)
{
	US i;
	
	for ( i = 0; i < size; i++ ) {
		if ( offset + i >= NVM_RAM_TABLE_SIZE ) {
			break;
		}
		pData[i] = *(nvmRamTable[offset+i].address);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_WriteArea
// Description   : NVM�G���A�փI�t�Z�b�g�ʒu����w��T�C�Y����������
// Parameter     : offset �������݊J�n�ʒu
//                 data �������݌��f�[�^
//                 size �������݃f�[�^�T�C�Y
// Return        : �]
///////////////////////////////////////////////////////////////////
void CNvmInformer_WriteArea(US offset, UC* pData, UC size)
{
	US i;
	
	for ( i = 0; i < size; i++ ) {
		if ( ( offset + i ) >= NVM_RAM_TABLE_SIZE ) {
			break;
		}
		*(nvmRamTable[offset+i].address) = pData[i];
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_GetAreaSize
// Description   : NVM�G���A�̃T�C�Y�擾
// Parameter     : �]
// Return        : NVM�G���A�̃T�C�Y(Byte�P��)
///////////////////////////////////////////////////////////////////
US CNvmInformer_GetAreaSize()
{
	return NVM_RAM_SIZE;
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_GetNvmAreaAddress
// Description   : NVM�G���A�Ŏg�p���Ă���̈�̃A�h���X��Ԃ�
// Parameter     : �]
// Return        : NVM�G���A�̃A�h���X
///////////////////////////////////////////////////////////////////
UC* CNvmInformer_GetAreaAddress()
{
	return &nvmRamArea[0];
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_SetValueChange
// Description   : NVM�̕ύX�����������Ƃ�o�^
// Parameter     : -
// Return        : -
///////////////////////////////////////////////////////////////////
void CNvmInformer_SetValueChange()
{
	g_NvmChange = NVM_CHANGE;
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_CompareVersion
// Description   : ���݂̃o�[�W�����ƈȑO�̃o�[�W�������r����
// Parameter     : currentMajor:����̃��W���[�o�[�W����
//                 currentMinor:����̃}�C�i�[�o�[�W����
//                 previousMajor:�ȑO�̃��W���[�o�[�W����
//                 previousMinor:�ȑO�̃}�C�i�[�o�[�W����
// Return        : -1: �o�[�W�����_�E��
//                  0: �o�[�W�����ω��Ȃ�
//                 +1: �o�[�W�����A�b�v
///////////////////////////////////////////////////////////////////
SS CNvmInformer_CompareVersion(US currentMajor, US currentMinor, US previousMajor, US previousMinor)
{
	if ( currentMajor > previousMajor ) {
		return NVM_VERSION_UP;
	} 
	else if ( currentMajor == previousMajor ) {
		if ( currentMinor > previousMinor ) {
			return NVM_VERSION_UP;
		} 
		else if ( currentMinor == previousMinor ) {
			return NVM_VERSION_NO_CHANGE;
		}
		else {
			return NVM_VERSION_DOWN;
		}
	}
	else {
		return NVM_VERSION_DOWN;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_AutoInitialize
// Description   : Version Up����������
// Parameter     : �]
// Return        : �]
// Note          : Market���Ƃ̏����l�ŏ��������s��
///////////////////////////////////////////////////////////////////
void CNvmInformer_AutoInitialize(US previousMajor, US previousMinor)
{
	US i;
	
	for ( i = 0; i < NVMID_NUM; i++ ) {
		if ( nvmAutoInitializeData[i].major < previousMajor ) {
			continue;
		}
		if ( ( nvmAutoInitializeData[i].major == previousMajor ) && ( nvmAutoInitializeData[i].minor <= previousMinor ) ) {
			continue;
		}
		CNvmNVM_Initialize(CNvmInformer_Get(nvmAutoInitializeData[i].id));
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_LoadNvmComplete
// Description   : 
// Parameter     : �]
// Return        : �]
// Note          : �]
///////////////////////////////////////////////////////////////////
void CNvmInformer_LoadNvmComplete(SS result)
{
	CNmaHandler_LoadNvmComplete(NMA_INFORMER_NVM, result);
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_SaveNvmComplete
// Description   : 
// Parameter     : �]
// Return        : �]
// Note          : �]
///////////////////////////////////////////////////////////////////
void CNvmInformer_SaveNvmComplete(SS result)
{
	CNmaHandler_SaveNvmComplete(NMA_INFORMER_NVM, result);
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_SetMajorVersion
// Description   : Major Version�ݒ�
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CNvmInformer_SetMajorVersion(UC version)
{
	if ( SRVC_GetNvmValueUS(NVMID_FINISHER_MAJOR_VERSION) != version ) {
		SRVC_SetNvmValueUS(version,	NVMID_FINISHER_MAJOR_VERSION);
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_SetMinorVersion
// Description   : Minor Version�ݒ�
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CNvmInformer_SetMinorVersion(UC version)
{
	if ( SRVC_GetNvmValueUS(NVMID_FINISHER_MINOR_VERSION) != version ) {
		SRVC_SetNvmValueUS(version,	NVMID_FINISHER_MINOR_VERSION);
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_SetPatchVersion
// Description   : Patch Version�ݒ�
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CNvmInformer_SetPatchVersion(UC version)
{
	if ( SRVC_GetNvmValueUS(NVMID_FINISHER_PATCH_VERSION) != version ) {
		SRVC_SetNvmValueUS(version,	NVMID_FINISHER_PATCH_VERSION);
	}
	else{ /* No Action */ }
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_SetIplVersion
// Description   : IPL Version�ݒ�
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CNvmInformer_SetIplVersion(UC version)
{
	if ( SRVC_GetNvmValueUS(NVMID_NVM_IPL_VERSION) != version ) {
		SRVC_SetNvmValueUS(version,	NVMID_NVM_IPL_VERSION);
	}
	else{ /* No Action */ }
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 15.03.31	| Makoto Tanabe		| Created this file based on Base SW V1
// 15.06.19	| Yuichi Hoshino	| Introduction Spec [ NVM Fail�������O���M ]
// 15.06.19	| Yuichi Hoshino	| Release Revision 00 to GA-Finisher V0.1.0 Make.
// 15.07.16	| Makoto Tanabe		| �@�\���P[NVM���X�g�̃}�N���C���ɔ����ύX]
// 16.02.26	| Yuichi Hoshino	| Introduction Spec [ Download ]
// 16.03.01	| Yuichi Hoshino	| Release Revision 01 to GA-Finisher V1.20.0 Make.
//

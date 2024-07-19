// File Name:	CTempInformer.c
// Description:	Implimentation File of Temp Data Informer
// Engineer:	Xxxx Xxxx
// Last Edit:	XX.XX.XX
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CTempInformer.h"

#include "CTempData.h"
#include "CSysManager.h"
#include "DD_NVM.h"

/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////
extern const CTempData tempAllData[];

/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void CTempInformer_LoadNvmComplete(SS result);
void CTempInformer_SaveNvmComplete(SS result);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CTempInformer_Constructor
// Description   : �R���X�g���N�^
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CTempInformer_Constructor()
{
	// �f�[�^��NVM���烍�[�h�����ׁA���������Ȃ�
}

///////////////////////////////////////////////////////////////////
// Function Name : CTempInformer_LoadNvmComplete
// Description   : TempData�̃��[�h����������
// Parameter     : result
// Return        : �]
///////////////////////////////////////////////////////////////////
void CTempInformer_LoadNvmComplete(SS result)
{
	CSysManager_LoadNvmComplete(SYS_NVM_DATA_INFO_TEMP, result);
}

///////////////////////////////////////////////////////////////////
// Function Name : CTempInformer_SaveNvmComplete
// Description   : TempData�̃Z�[�u����������
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CTempInformer_SaveNvmComplete(SS result)
{
	CSysManager_SaveNvmComplete(SYS_NVM_DATA_INFO_TEMP, result);
}

///////////////////////////////////////////////////////////////////
// Function Name : CTempInformer_LoadNvm
// Description   : TempData�̃��[�h�˗�
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CTempInformer_LoadNvm()
{
	DD_NVM_Load(DD_NVM_AREA_TEMP, &temp_saveFlag, TEMP_DATA_RAM_SIZE, CTempInformer_LoadNvmComplete);
}

///////////////////////////////////////////////////////////////////
// Function Name : CTempInformer_SaveNvm
// Description   : TempData�̃Z�[�u�˗�
// Parameter     : �]
// Return        : true = Save�������{
//               : false = Save���������{
///////////////////////////////////////////////////////////////////
bool CTempInformer_SaveNvm()
{
	if( temp_saveFlag ) {
		temp_saveFlag = 0;
		DD_NVM_Save(DD_NVM_AREA_TEMP, &temp_saveFlag, TEMP_DATA_RAM_SIZE, CTempInformer_SaveNvmComplete);
		return true;
	}
	else {
		return false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CTempInformer_FirstPowerOnInitialize
// Description   : FirstPowerOn����������
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CTempInformer_FirstPowerOnInitialize()
{
	US i;
	
	/* �S�Ă�TempData�������� */
	for ( i = 0; i < TEMP_DATA_NUM; i++ ) {
		CTempData_Initialize(&tempAllData[i]);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CTempInformer_VersionInitialize
// Description   : Version Up����������
// Parameter     : previousMajor �O��MajorVersion
//                 previousMinor �O��MinorVersion
//                 previousPatch �O��PatchVersion
// Return        : -
///////////////////////////////////////////////////////////////////
void CTempInformer_VersionInitialize(US previousMajor, US previousMinor, US previousPatch)
{
	US i;
	
	/* Version�ύX���ɏ��������s�� */
	if ( ( MAJOR_VERSION != previousMajor ) || ( MINOR_VERSION != previousMinor ) || ( PATCH_VERSION != previousPatch ) ) {
		for ( i = 0; i < TEMP_DATA_NUM; i++ ) {
			CTempData_Initialize(&tempAllData[i]);
		}
	}
}

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

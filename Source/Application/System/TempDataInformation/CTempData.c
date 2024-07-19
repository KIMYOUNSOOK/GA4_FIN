// File Name:	CTempData.c
// Description:	Implimentation File of Temp Data
// Engineer:	Xxxx Xxxx
// Last Edit:	XX.XX.XX
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CTempData.h"

#include "ECdiFinDataType.h"

/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

enum ECdiFinDataType CTempData_GetDataType(CTempData* pThis);

/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CTempData_Constructor
// Description   : �R���X�g���N�^
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CTempData_Constructor(CTempData* pThis)
{
	/* �f�[�^��NVM���烍�[�h�����ׁA�������͍s��Ȃ� */
}

///////////////////////////////////////////////////////////////////
// Function Name : CTempData_Initialize
// Description   : TempData����������
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : TEMP_RESULT_OK �����ATEMP_RESULT_NG ���s
// Note          : -
///////////////////////////////////////////////////////////////////
SC CTempData_Initialize(CTempData* pThis)
{
	switch ( CTempData_GetDataType(pThis) ) {
	case eCdiFinDataType_SignChar:
		*((SC*)(pThis->mpSpec->address)) = (SC)(pThis->mpSpec->initialValue) ;
		break;
	case eCdiFinDataType_UnsignChar:
		*((UC*)(pThis->mpSpec->address)) = (UC)(pThis->mpSpec->initialValue) ;
		break;
	case eCdiFinDataType_SignShort:
		*((SS*)(pThis->mpSpec->address)) = (SS)(pThis->mpSpec->initialValue) ;
		break;
	case eCdiFinDataType_UnsignShort:
		*((US*)(pThis->mpSpec->address)) = (US)(pThis->mpSpec->initialValue) ;
		break;
	case eCdiFinDataType_SignLong:
		*((SL*)(pThis->mpSpec->address)) = (SL)(pThis->mpSpec->initialValue) ;
		break;
	case eCdiFinDataType_UnsignLong:
		*(pThis->mpSpec->address) = pThis->mpSpec->initialValue ;
		break;
	default:
		return TEMP_RESULT_NG;
	}
	return TEMP_RESULT_OK;
}

///////////////////////////////////////////////////////////////////
// Function Name : CTempData_GetDataType
// Description   : �^���擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �^
///////////////////////////////////////////////////////////////////
enum ECdiFinDataType CTempData_GetDataType(CTempData* pThis)
{
	return (enum ECdiFinDataType)(pThis->mpSpec->dataType);
}

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

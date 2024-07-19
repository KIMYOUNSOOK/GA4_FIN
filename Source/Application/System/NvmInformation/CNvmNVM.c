// File Name:	CNvmNVM.c
// Description:	Implimentation File of NVM
// Engineer:	Xxxx Xxxx
// Last Edit:	XX.XX.XX
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CNvmNVM.h"
#include "CNvmInformer.h"

#include "CCfgDevContConfiguration.h"
#include "CCfgInformer.h"

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
// Function Name : CNvmNVM_Constructor
// Description   : �R���X�g���N�^
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CNvmNVM_Constructor(CNvmNVM* pThis)
{
	/* �������Ȃ� */
	/* NVM�̏�������PowerOnInitialize�ɂčs�� */
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmNVM_GetValue
// Description   : NVM�l�擾
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : NVM�l
///////////////////////////////////////////////////////////////////
UL CNvmNVM_GetValue(CNvmNVM* pThis)
{
	
	switch ( CNvmNVM_GetDataType(pThis) ) {
	case eCdiFinDataType_SignChar:
		return (UL)(*((SC*)(pThis->mpSpec->address)));
	case eCdiFinDataType_UnsignChar:
		return (UL)(*(pThis->mpSpec->address));
	case eCdiFinDataType_SignShort:
		return (UL)(*((SS*)(pThis->mpSpec->address)));
	case eCdiFinDataType_UnsignShort:
		return (UL)(*((US*)(pThis->mpSpec->address)));
	case eCdiFinDataType_SignLong:
		return (UL)(*((SL*)(pThis->mpSpec->address)));
	case eCdiFinDataType_UnsignLong:
		return *((UL*)(pThis->mpSpec->address));
	default:
		return 0;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmNVM_GetSignedValue
// Description   : NVM�l�擾
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : NVM�l
///////////////////////////////////////////////////////////////////
SL CNvmNVM_GetSignedValue(CNvmNVM* pThis)
{
	return (SL)CNvmNVM_GetValue(pThis);
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmNVM_SetValue
// Description   : NVM�l�ݒ菈��
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : NVM_RESULT_OK �����ANVM_RESULT_NG ���s
// Note          : �͈͓��`�F�b�N�͍s��Ȃ�
///////////////////////////////////////////////////////////////////
SC CNvmNVM_SetValue(CNvmNVM* pThis, UL value)
{
	
	switch ( CNvmNVM_GetDataType(pThis) ) {
	case eCdiFinDataType_SignChar:
		if( *((SC*)(pThis->mpSpec->address)) != (SC)value ) {
			*((SC*)(pThis->mpSpec->address)) = (SC)value;
			CNvmInformer_SetValueChange();
		}
		break;
	case eCdiFinDataType_UnsignChar:
		if( *(pThis->mpSpec->address) != (UC)value ) {
			*(pThis->mpSpec->address) = (UC)value;
			CNvmInformer_SetValueChange();
		}
		break;
	case eCdiFinDataType_SignShort:
		if( *((SS*)(pThis->mpSpec->address)) != (SS)value ) {
			*((SS*)(pThis->mpSpec->address)) = (SS)value;
			CNvmInformer_SetValueChange();
		}
		break;
	case eCdiFinDataType_UnsignShort:
		if( *((US*)(pThis->mpSpec->address)) != (US)value ) {
			*((US*)(pThis->mpSpec->address)) = (US)value;
			CNvmInformer_SetValueChange();
		}
		break;
	case eCdiFinDataType_SignLong:
		if( *((SL*)(pThis->mpSpec->address)) != (SL)value ) {
			*((SL*)(pThis->mpSpec->address)) = (SL)value;
			CNvmInformer_SetValueChange();
		}
		break;
	case eCdiFinDataType_UnsignLong:
		if( *((UL*)(pThis->mpSpec->address)) != value ) {
			*((UL*)(pThis->mpSpec->address)) = value;
			CNvmInformer_SetValueChange();
		}
		break;
	default:
		return NVM_RESULT_NG;
	}
	return NVM_RESULT_OK;
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmNVM_Increament
// Description   : NVM�l���C���N�������g
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : NVM_RESULT_OK �����ANVM_RESULT_NG ���s
// Note          : �͈͓��`�F�b�N�͍s��Ȃ�
///////////////////////////////////////////////////////////////////
SC CNvmNVM_Increment(CNvmNVM* pThis)
{
	UL value = CNvmNVM_GetValue(pThis);
	
	switch ( CNvmNVM_GetDataType(pThis) ) {
	case eCdiFinDataType_SignChar:
	case eCdiFinDataType_SignShort:
	case eCdiFinDataType_SignLong:
		if ( (SL)value >= (SL)CNvmNVM_GetMaxValue(pThis) ) {
			return NVM_RESULT_NG;
		}
		value = (UL)((SL)(value)+1);
		break;
	case eCdiFinDataType_UnsignChar:
	case eCdiFinDataType_UnsignShort:
	case eCdiFinDataType_UnsignLong:
		if ( value >= CNvmNVM_GetMaxValue(pThis) ) {
			return NVM_RESULT_NG;
		}
		value++;
		break;
	default:
		return NVM_RESULT_NG;
	}
	return CNvmNVM_SetValue(pThis, value);
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmNVM_Initialize
// Description   : NVM����������
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : NVM_RESULT_OK �����ANVM_RESULT_NG ���s
// Note          : Market���Ƃ̏����l��ݒ肷��
///////////////////////////////////////////////////////////////////
SC CNvmNVM_Initialize(CNvmNVM* pThis)
{
	UL value;
	CCfgDevContConfiguration* config = CCfgInformer_GetDeviceControllerConfiguration(OWN_DEV_CONT_ID);
	
	/* Market���Ƃ̏����l��ݒ� */
	switch (CCfgDevContConfiguration_GetMarketRegion(config)) {
	case eCdiFinMarket_FX:
		value = pThis->mpSpec->initialValueFx;
		break;
	case eCdiFinMarket_AP:
		value = pThis->mpSpec->initialValueAp;
		break;
    case eCdiFinMarket_XC:
		value = pThis->mpSpec->initialValueXc;
		break;
    case eCdiFinMarket_XE:
		value = pThis->mpSpec->initialValueXe;
		break;
    case eCdiFinMarket_SA:
		value = pThis->mpSpec->initialValueSa;
		break;
	default:
		return NVM_RESULT_NG;
	}
	return CNvmNVM_SetValue(pThis, value);
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmNVM_GetDataType
// Description   : NVM�̌^���擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �^
///////////////////////////////////////////////////////////////////
enum ECdiFinDataType CNvmNVM_GetDataType(CNvmNVM* pThis)
{
	return (enum ECdiFinDataType)(pThis->mpSpec->dataType);
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmNVM_GetMinValue
// Description   : NVM�̍ŏ��l���擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �ŏ��l
///////////////////////////////////////////////////////////////////
UL CNvmNVM_GetMinValue(CNvmNVM* pThis)
{
	return pThis->mpSpec->minValue;
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmNVM_GetMaxValue
// Description   : NVM�̍ő�l���擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �ő�l
///////////////////////////////////////////////////////////////////
UL CNvmNVM_GetMaxValue(CNvmNVM* pThis)
{
	return pThis->mpSpec->maxValue;
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmNVM_IsReadAllowed
// Description   : Read����/�֎~��Ԃ�
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Read����/�֎~
///////////////////////////////////////////////////////////////////
UC CNvmNVM_IsReadAllowed(CNvmNVM* pThis)
{
	return pThis->mpSpec->readAllowed;
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmNVM_IsWriteAllowed
// Description   : Write����/�֎~��Ԃ�
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Write����/�֎~
///////////////////////////////////////////////////////////////////
UC CNvmNVM_IsWriteAllowed(CNvmNVM* pThis)
{
	return pThis->mpSpec->writeAllowed;
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmNVM_IsFactoryInitAllowed
// Description   : ����������/�֎~��Ԃ�
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : ����������/�֎~
///////////////////////////////////////////////////////////////////
UC CNvmNVM_IsFactoryInitAllowed(CNvmNVM* pThis)
{
	return pThis->mpSpec->initAllowed;
}

///////////////////////////////////////////////////////////////////
// Function Name : CNvmInformer_IsOutOfRange
// Description   : �͈͊O�`�F�b�N
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : true �͈͊O�Afalse �͈͓�
///////////////////////////////////////////////////////////////////
bool CNvmNVM_IsOutRange(CNvmNVM* pThis)
{
	switch ( CNvmNVM_GetDataType(pThis) ) {
	case eCdiFinDataType_SignChar:
	case eCdiFinDataType_SignShort:
	case eCdiFinDataType_SignLong:
		return ( (SL)CNvmNVM_GetValue(pThis) > (SL)CNvmNVM_GetMaxValue(pThis) ) ||
			   ( (SL)CNvmNVM_GetValue(pThis) < (SL)CNvmNVM_GetMinValue(pThis) ) ;
	case eCdiFinDataType_UnsignChar:
	case eCdiFinDataType_UnsignShort:
	case eCdiFinDataType_UnsignLong:
		return ( CNvmNVM_GetValue(pThis) > CNvmNVM_GetMaxValue(pThis) ) ||
			   ( CNvmNVM_GetValue(pThis) < CNvmNVM_GetMinValue(pThis) ) ;
	default:
		return true;
	}
}

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

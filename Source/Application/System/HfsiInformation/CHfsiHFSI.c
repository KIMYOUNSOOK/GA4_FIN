// File Name:	CHfsiHFSI.c
// Description:	Implimentation File of HFSI
// Engineer:	Xxxxxx Xxxxxx
// Last Edit:	XX.XX.XX
// Revision:	XX
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CHfsiHFSI.h"

#include "CHfsiInformer.h"
#include "HfsiRam.h"

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
// Function Name : CHfsiHFSI_Constructor
// Description   : �R���X�g���N�^
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CHfsiHFSI_Constructor(CHfsiHFSI* pThis)
{
	/* �������Ȃ� */
	/* HFSI�̏�������PowerOnInitialize�ɂčs�� */
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_GetChainNo
// Description   : ChainNo��Ԃ�
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : ChainNo
///////////////////////////////////////////////////////////////////
US CHfsiHFSI_GetChainNo(CHfsiHFSI* pThis)
{
	return pThis->mpSpec->chain;
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_GetLinkNo
// Description   : LinkNo��Ԃ�
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : LinkNo
///////////////////////////////////////////////////////////////////
US CHfsiHFSI_GetLinkNo(CHfsiHFSI* pThis)
{
	return pThis->mpSpec->link;
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_GetValue
// Description   : HFSI���݃J�E���^��Ԃ�
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : ���݃J�E���^
///////////////////////////////////////////////////////////////////
UL CHfsiHFSI_GetValue(CHfsiHFSI* pThis)
{
	return pThis->mpSpec->address->currentCounter;
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_GetSignedValue
// Description   : HFSI���݃J�E���^��Ԃ�
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : ���݃J�E���^
///////////////////////////////////////////////////////////////////
SL CHfsiHFSI_GetSignedValue(CHfsiHFSI* pThis)
{
	return (SL)CHfsiHFSI_GetValue(pThis);
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_SetValue
// Description   : HFSI���݃J�E���^�̍X�V
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : HFSI_RESULT_OK �����AHFSI_RESULT_NG ���s
///////////////////////////////////////////////////////////////////
SC CHfsiHFSI_SetValue(CHfsiHFSI* pThis, UL value)
{
	/* HFSI Save���{���f�ׁ̈A�J�E���^�l�ɕύX�����邩���`�F�b�N���� */
	if ( CHfsiHFSI_GetValue(pThis) != value ) {
		pThis->mpSpec->address->currentCounter = value;
		CHfsiInformer_SetValueChange();
	}
    return HFSI_RESULT_OK;
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_Increment
// Description   : HFSI���݃J�E���^��+1����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : HFSI_RESULT_OK �����AHFSI_RESULT_NG ���s
///////////////////////////////////////////////////////////////////
SC CHfsiHFSI_Increment(CHfsiHFSI* pThis)
{
	/* ���݃J�E���^���ő�l */
	if ( CHfsiHFSI_GetValue(pThis) >= CHfsiHFSI_GetMaxValue(pThis) ) {
		return HFSI_RESULT_NG;
	}
	/* ���݃J�E���^���ő�l�ȊO */
	else {
		pThis->mpSpec->address->currentCounter++;
		CHfsiInformer_SetValueChange();
		return HFSI_RESULT_OK;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_GetDataType
// Description   : HFSI�̃f�[�^�^��Ԃ�
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �f�[�^�^
///////////////////////////////////////////////////////////////////
enum ECdiFinDataType CHfsiHFSI_GetDataType(CHfsiHFSI* pThis)
{
	return eCdiFinDataType_UnsignLong;
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_GetMinValue
// Description   : HFSI�ŏ��l��Ԃ�
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �ŏ��l
///////////////////////////////////////////////////////////////////
UL CHfsiHFSI_GetMinValue(CHfsiHFSI* pThis)
{
	return 0;
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_GetMaxValue
// Description   : HFSI�ő�l��Ԃ�
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �ő�l
///////////////////////////////////////////////////////////////////
UL CHfsiHFSI_GetMaxValue(CHfsiHFSI* pThis)
{
	return 0xFFFFFFFF;;
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_Initialize
// Description   : HFSI��񏉊���
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CHfsiHFSI_Initialize(CHfsiHFSI* pThis)
{
	UC i = 0;
	
	/* ���݃J�E���^������ */
	CHfsiHFSI_SetValue(pThis, 0);
	
	/* ����臒l�������� */
	CHfsiHFSI_InitializeLifeSpec(pThis);

	/* �ߋ�3�񕪂̌������̃J�E���^�E���R�������� */
	for ( i = 0; i < HFSI_EXCHANGE_HISTORY_NUM; i++ ) {
		pThis->mpSpec->address->lastCounter[i] = 0;
		pThis->mpSpec->address->exchangeReason[i] = 0;
	}
	
	/* HFSI�ɕύX������������o�^ */
	CHfsiInformer_SetValueChange();
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_InitializeLifeSpec
// Description   : HFSI����臒l������
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CHfsiHFSI_InitializeLifeSpec(CHfsiHFSI* pThis)
{
	CHfsiHFSI_SetLifeSpec(pThis, pThis->mpSpec->threshold);
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_Reset
// Description   : HFSI��񃊃Z�b�g
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CHfsiHFSI_Reset(CHfsiHFSI* pThis, UC exchangeReason)
{
	/* ���݂̃J�E���^�����Z�b�g */
	CHfsiHFSI_SetValue(pThis, 0);
	
	/* �ߋ�3�񕪂̌������J�E���^���X�V */
	pThis->mpSpec->address->lastCounter[2] = pThis->mpSpec->address->lastCounter[1];
	pThis->mpSpec->address->lastCounter[1] = pThis->mpSpec->address->lastCounter[0];
	pThis->mpSpec->address->lastCounter[0] = pThis->mpSpec->address->currentCounter;
	
	/* �ߋ�3�񕪂̌������R���X�V */
	pThis->mpSpec->address->exchangeReason[2] = pThis->mpSpec->address->exchangeReason[1];
	pThis->mpSpec->address->exchangeReason[1] = pThis->mpSpec->address->exchangeReason[0];
	pThis->mpSpec->address->exchangeReason[0] = exchangeReason;
	
	/* HFSI�ɕύX������������o�^ */
	CHfsiInformer_SetValueChange();
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_GetLastCount
// Description   : HFSI�������J�E���^��Ԃ�
// Parameter     : pThis (�C���X�^���X�|�C���^)
//                 lastCount (����O�̌������J�E���^��������)
// Return        : �������̃J�E���^
///////////////////////////////////////////////////////////////////
UL CHfsiHFSI_GetLastCount(CHfsiHFSI* pThis, UC lastCount)
{
	return pThis->mpSpec->address->lastCounter[lastCount];
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_GetExchangeReason
// Description   : HFSI�������R��Ԃ�
// Parameter     : pThis (�C���X�^���X�|�C���^)
//                 lastCount (����O�̌������J�E���^��������)
// Return        : �������R
///////////////////////////////////////////////////////////////////
UC CHfsiHFSI_GetExchangeReason(CHfsiHFSI* pThis, UC lastCount)
{
	return pThis->mpSpec->address->exchangeReason[lastCount];
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_SetLifeSpec
// Description   : HFSI����臒l���X�V����
// Parameter     : pThis (�C���X�^���X�|�C���^)
//                 spec (����臒l�̍X�V�l)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CHfsiHFSI_SetLifeSpec(CHfsiHFSI* pThis, UL spec)
{
	/* HFSI Save���{���f�ׁ̈A�J�E���^�l�ɕύX�����邩���`�F�b�N���� */
	if ( CHfsiHFSI_GetLifeSpec(pThis) != spec ) {
		pThis->mpSpec->address->lifeSpec = spec;
		CHfsiInformer_SetValueChange();
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_GetLifeSpec
// Description   : HFSI����臒l��Ԃ�
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : ����臒l
///////////////////////////////////////////////////////////////////
UL CHfsiHFSI_GetLifeSpec(CHfsiHFSI* pThis)
{
	return pThis->mpSpec->address->lifeSpec;
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_GetPartsName
// Description   : HFSI�p�[�c���̂�Ԃ�
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �p�[�c����
///////////////////////////////////////////////////////////////////
UC const* CHfsiHFSI_GetPartsName(CHfsiHFSI* pThis)
{
	return pThis->mpSpec->partsName;
}

///////////////////////////////////////////////////////////////////
// Function Name : CHfsiHFSI_GetPartsNameSize
// Description   : HFSI�p�[�c���̂̃T�C�Y��Ԃ�
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �p�[�c���̂̃T�C�Y
///////////////////////////////////////////////////////////////////
UC CHfsiHFSI_GetPartsNameSize(CHfsiHFSI* pThis)
{
	return pThis->mpSpec->partsNameSize;
}

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 15.03.30	| Makoto Tanabe		| Created this file
// 15.03.30	| Makoto Tanabe		| Release V1 (Revision 00)
// 15.04.02	| Makoto Tanabe		| Design Improvement [ �X�y���ԈႢ(CHfsiHFSI_Increment)�C�� ]
// 15.04.02	| Makoto Tanabe		| Design Improvement [ CHfsiHFSI_Increment()�߂�l������ ]
// 15.XX.XX	| Makoto Tanabe		| Release V2 (Revision 01)
//

// ������ Module Software Platform Library ������
/// @file    LIB_DataBuffer.c
/// @author  Tsuyoshi Seto
/// @date    15.01.13
/// @version 003
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#define LIB_DataBuffer_c

#include "Common.h"
#include "LIB_Lock.h"
#include "LIB_DataBuffer.h"


// ******************************************************  <<<<< Definition & Declaration Area >>>>>  *********************************************************

// ===========================  <<< �}�N����` >>>  ===========================

// ----- �o�b�t�@���` -----
#define	LIB_DBF_EMPTY		0x00															// �f�[�^�o�b�t�@�Ƀf�[�^���Ȃ����

// ===========================  <<< �֐��v���g�^�C�v�錾 >>>  ===========================
static SS CDataBuffer_Write(CDataBuffer*, UC*, SS) ;										// �f�[�^�o�b�t�@�ւ̃f�[�^�������ݏ���
static SS CDataBuffer_Read(CDataBuffer*, UC**, SS, SS) ;									// �f�[�^�o�b�t�@����̃f�[�^�ǂݏo������


// ========================  <<< �C���X�^���X���� >>>  ========================

// ----- �f�[�^�o�b�t�@ -----
CDataBuffer lib_DataBuffer ;																// �f�[�^�o�b�t�@ (1�̃o�b�t�@�𕡐��̃L���[�ŋ��L�\)


// ************************************************************  <<<<< Program Code Area >>>>>  ***************************************************************

//$$$$$$ [[[[[ Class: CDataBuffer ]]]]] $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// Class Name    : CDataBuffer
// Description   : �f�[�^�i�[�p�o�b�t�@
//               :
// Note          : �Ȃ�
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

//=[[[ Constructor ]]]=================================================================
// Function Name : CDataBuffer::CDataBuffer()                                          
// Description   : ���̃N���X�̃R���X�g���N�^.                                         
// Parameter     : *self		"�Ώۃf�[�^�o�b�t�@�̃A�h���X"                         
// Return        : -                                                                   
//=====================================================================================
void CDataBuffer_CDataBuffer(CDataBuffer *self)
{
	US	index ;

// ----- �����o�[������ -----
	self->mWrite = 0 ;																		// �������ݗp�C���f�b�N�X�����Z�b�g����
	self->mStatus = LIB_DBF_OK ;                                                            // �f�[�^�o�b�t�@�X�e�[�^�X�����Z�b�g����

	for ( index = 0 ; index < ((LIB_DBF_SIZE - 1) / 4 + 1) ; index++ ) {					// �f�[�^�o�b�t�@���N���A����
		self->mBuffer[index] = LIB_DBF_EMPTY ;
	}

// ----- �֐��|�C���^�[�ݒ� -----
	self->Write = CDataBuffer_Write ;														// Write()�F�f�[�^�������ݏ���
	self->Read  = CDataBuffer_Read ;														// Read() �F�f�[�^�ǂݏo������
}

//=====================================================================================
// Function Name : CDataBuffer::Write()                                                
// Description   : �f�[�^�o�b�t�@�Ƀf�[�^����������.                                   
// Parameter     : *self		"�Ώۃf�[�^�o�b�t�@�̃A�h���X"                         
//               : *data		"�������ݑΏۃf�[�^�̐擪�A�h���X"                     
//               : size			"�������ݑΏۃf�[�^�̃T�C�Y"                           
// Return        : �������݊J�n�ʒu�i�I�t�Z�b�g�o�C�g���j�c �}�C�i�X�͏������݃G���[   
//=====================================================================================
static SS CDataBuffer_Write(CDataBuffer *self, UC *data, SS size)
{
	SS	result ;
	UC	key ;

	if ( ( size < LIB_DBF_SIZE ) && ( self->mStatus != LIB_DBF_ERR ) ) {					// < �������݃T�C�Y���o�b�t�@�T�C�Y�𒴂��Ȃ� > �� �����p��
		key = LIB_GetKey() ; LIB_Lock() ;													// �I�I�I�I�I�I�I�I�I�I ���荞�݋֎~ �I�I�I�I�I�I�I�I�I�I
		if ( ((self->mWrite) + size ) > LIB_DBF_SIZE ) self->mWrite = 0 ;					// �c��T�C�Y�����i�[���b�Z�[�W���傫�� �� �������ݗp�C���f�b�N�X���X�V����
		result = self->mWrite ;

		memcpy(((UC*)(self->mBuffer)) + (self->mWrite), data, size) ;						// �o�b�t�@�[�ւ̃f�[�^�R�s�[�����{����
		(self->mWrite) += ((size + LIB_DATA_ALIGN) & LIB_ALIGN_MASK) ;						// �������ݗp�C���f�b�N�X�Ƀo�E���_���[�����ςݏ������݃T�C�Y�����Z����
		if ( (self->mWrite) >= LIB_DBF_SIZE ) self->mWrite = 0 ;							// �������ݗp�C���f�b�N�X���X�V����
		LIB_Unlock(key) ;																	// �I�I�I�I�I�I�I�I�I�I ���荞�݋��� �I�I�I�I�I�I�I�I�I�I
	}
	else {																					// < �������݃T�C�Y���o�b�t�@�T�C�Y�𒴂��� > �� �G���[����
		self->mStatus = LIB_DBF_ERR ;														// �f�[�^�o�b�t�@�X�e�[�^�X���G���[�ɂ���
		result = LIB_DBF_ERR ;
	}

	return	result ;
}


//=====================================================================================
// Function Name : CDataBuffer::Read()                                                 
// Description   : �f�[�^�o�b�t�@����f�[�^��ǂݏo��.                                 
// Parameter     : *self		"�Ώۃf�[�^�o�b�t�@�̃A�h���X"                         
//               : offset		"�ǂݏo���Ώۃf�[�^�̃o�b�t�@�ǂݏo���J�n�I�t�Z�b�g"   
//               : **area		"��M���b�Z�[�W�i�[�G���A�擪�A�h���X�������݃A�h���X" 
//               : size			"�ǂݏo���Ώۃf�[�^�̃T�C�Y"                           
// Return        : �f�[�^�ǂݏo�����ʁi0�FOK / -1:NG)                                  
//=====================================================================================
static SS CDataBuffer_Read(CDataBuffer *self, UC **area, SS offset, SS size)
{
	US	result = LIB_DBF_OK ;

	if ( ( size < LIB_DBF_SIZE ) && ( self->mStatus != LIB_DBF_ERR ) ) {					// < �ǂݏo���T�C�Y���o�b�t�@�T�C�Y�𒴂��Ȃ� > �� �����p��
		*area = ((UC*)(self->mBuffer)) + offset ;											// ���[�U�[�̈�ւ̃f�[�^�R�s�[�����{����
	}
	else {																					// < �������݃T�C�Y���o�b�t�@�T�C�Y�𒴂��� > �� �G���[����
		self->mStatus = LIB_DBF_ERR ;														// �f�[�^�o�b�t�@�X�e�[�^�X���G���[�ɂ���
		result = LIB_DBF_ERR ;
	}

	return	result ;
}


//
// Change History
// Date:    | Engineer:          | Note:
// ---------+--------------------+-----------------------------------------------------------------------------------------------------------------------------
// 14.02.20 | Tsuyoshi Seto      | MSP Library > Data Buffer�̃v���g�^�C�v�Ƃ��Ė{�t�@�C�����쐬. (Rev.000)
// 14.09.16 | Tsuyoshi Seto      | Data Buffer�ւ̃f�[�^�������݂����[�v����memcpy�g�p�ɕύX. (Rev.001)
// 14.09.22 | Tsuyoshi Seto      | Data Buffer�̈�ւ̏������݂�4�o�C�g���E�Ɏ��{����悤�C���f�b�N�X�̍X�V���C��. (Rev.002)
// 15.01.13 | Tsuyoshi Seto      | Data Buffer�ւ̏������݃o�b�e�B���O�i�ʏ�/�����݁j�̍ۂ̃f�[�^�����g���u���ւ̑Ή�. (Rev.003)
//
//


// *************************************************************  [ LIB_DataBuffer.c : EOF ]  *****************************************************************

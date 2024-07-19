// ������ Module Software Platform Library ������
/// @file    LIB_MessageBuffer.c
/// @author  Tsuyoshi Seto
/// @date    14.02.20
/// @version 000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#define LIB_MessageBuffer_c

#include "Common.h"
#include "LIB_Lock.h"
#include "LIB_DataBuffer.h"
#include "LIB_MessageBuffer.h"


// ******************************************************  <<<<< Definition & Declaration Area >>>>>  *********************************************************

// ===========================  <<< �}�N����` >>>  =========================== 

// ----- �o�b�t�@���` -----
#define	LIB_MBF_EMPTY		0x00															// �f�[�^�o�b�t�@�Ƀf�[�^���Ȃ����


// ===========================  <<< �֐��v���g�^�C�v�錾 >>>  ===========================
static SS CMessageBuffer_Send(CMessageBuffer*, UC*, SS) ;									// �f�[�^�o�b�t�@�[�ւ̃f�[�^�������ݏ���
static SS CMessageBuffer_Receive(CMessageBuffer*, UC**) ;									// �f�[�^�o�b�t�@�[����̃f�[�^�ǂݏo������


// ========================  <<< �C���X�^���X���� >>>  ========================

// ----- ���b�Z�[�W�o�b�t�@ -----
CMessageBuffer  lib_MsgBuff[eNumOfMbf] ;													// ���b�Z�[�W�o�b�t�@�{�́iID���̗̈���m�ہj

// ----- ���b�Z�[�W�Ǘ��̈� ----- 															// �����ӁF�Ǘ��̈�̓��b�Z�[�W�o�b�t�@ID�����m�ۂ��邱�ƁI�I�I
static MessageQ  lib_MsgQFrame[LIB_MBF_FRM_SIZE] ;											// ���b�Z�[�W�Ǘ��̈�FMSP Frame�p�̗̈�m��
static MessageQ  lib_MsgQDdLow[LIB_MBF_DD_LOW_SIZE] ;										// ���b�Z�[�W�Ǘ��̈�FDevice Driver��D��x�p�̗̈�m��
static MessageQ  lib_MsgQDdHigh[LIB_MBF_DD_HIGH_SIZE] ;										// ���b�Z�[�W�Ǘ��̈�FDevice Driver���D��x�p�̗̈�m��


// ===========================  <<< �O���Q�ƕϐ� >>>  ===========================

// ----- �f�[�^�o�b�t�@ -----
extern CDataBuffer	lib_DataBuffer ;														// [�O���Q��] ���b�Z�[�W�i�[�p�f�[�^�o�b�t�@


// ���������������������������� <<< �R���t�B�M�����[�V���� >>> ����������������������������

// ----- ���b�Z�[�W�o�b�t�@�R���t�B�M�����[�V���� -----
const MbfConfig lib_MbfConfig[eNumOfMbf] = {
//	+--------------------+---------------------+---------------------+
//	| Message Q Address  | Message Q Size      | Data Buffer Address |
//	+--------------------+---------------------+---------------------+						// MBF ID : ��ID���ɃR���t�B�M�����[�V�����ݒ肪�K�v�ƂȂ�I
	{ lib_MsgQFrame      , LIB_MBF_FRM_SIZE    , &lib_DataBuffer     },						//  0: eMbfFrame	: MSP Frame�p���b�Z�[�W�o�b�t�@
	{ lib_MsgQDdLow      , LIB_MBF_DD_LOW_SIZE , &lib_DataBuffer     },						//  1: eMbfDdLow    : Device Driver�p��D��x���b�Z�[�W�o�b�t�@
	{ lib_MsgQDdHigh	 , LIB_MBF_DD_HIGH_SIZE, &lib_DataBuffer     }						//  2: eMbfDdHigh	: Device Driver�p���D��x���b�Z�[�W�o�b�t�@
} ;


// ************************************************************  <<<<< Program Code Area >>>>>  ***************************************************************

//$$$$$$ [[[[[ Class: CMessageBuffer ]]]]] $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// Class Name    : CMessageBuffer
// Description   : ���b�Z�[�W�o�b�t�@
//               :
// Note          : �Ȃ�
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

//=[[[ Constructor ]]]=================================================================
// Function Name : CMessageBuffer::CMessageBuffer()                                    
// Description   : ���̃N���X�̃R���X�g���N�^.                                         
// Parameter     : *self		"�Ώۃ��b�Z�[�W�o�b�t�@�̃A�h���X"                     
//               : *config		"���b�Z�[�W�o�b�t�@�R���t�B�M�����[�V�����̃A�h���X"   
// Return        : -                                                                   
//=====================================================================================
void CMessageBuffer_CMessageBuffer(CMessageBuffer *self, const MbfConfig *config)
{
// ----- �����o�[������ -----
	self->mpBuffer = config->pBuff ;														// �f�[�^�o�b�t�@�̃A�h���X���Z�b�g����
	self->mpMsgQ   = config->pMsgQ ;														// ���b�Z�[�W�Ǘ��L���[�̃A�h���X���Z�b�g����
	self->mSize    = config->qSize ;														// ���b�Z�[�W�o�b�t�@�̃T�C�Y���Z�b�g����
	self->mRead    = self->mWrite = 0 ;														// �ǂݏo��/�������݃C���f�b�N�X�����Z�b�g����
	self->mStatus  = LIB_MBF_OK ;															// ���b�Z�[�W�o�b�t�@�X�e�[�^�X�����Z�b�g����

// ----- �֐��|�C���^�[�ݒ� -----
	self->Send    = CMessageBuffer_Send ;													// Send()   �F���b�Z�[�W���M����
	self->Receive = CMessageBuffer_Receive ;												// Receive()�F���b�Z�[�W��M����
}


//=====================================================================================
// Function Name : CMessageBuffer::Send()                                              
// Description   : ���b�Z�[�W�o�b�t�@�Ƀ��b�Z�[�W�𑗐M����.                           
// Parameter     : *self		"�Ώۃ��b�Z�[�W�o�b�t�@�̃A�h���X"                     
//               : *message		"�ʒm�Ώۃ��b�Z�[�W�̐擪�A�h���X"                     
//               : size			"�ʒm�Ώۃ��b�Z�[�W��"                                 
// Return        : ���M�������ʁi0�FOK / -1�FNG)                                       
//=====================================================================================
static SS CMessageBuffer_Send(CMessageBuffer *self, UC *message, SS size)
{
	SS	offset ;
	US	index ;
	UC	key ;

	if ( self->mStatus != LIB_MBF_ERR ) {													// < �X�e�[�^�X���G���[��ԂłȂ� >
		offset = self->mpBuffer->Write(self->mpBuffer, message, size) ;						// �f�[�^�o�b�t�@�փ��b�Z�[�W������������

		if ( offset != LIB_MBF_ERR ) {														// < �f�[�^�o�b�t�@�������݃G���[�͖����� >
			key = LIB_GetKey() ; LIB_Lock() ;												// �I�I�I�I�I�I�I�I�I�I ���荞�݋֎~ �I�I�I�I�I�I�I�I�I�I
			index = self->mWrite ;															// �������݃C���f�b�N�X���Z�[�u����
			self->mpMsgQ[index].offset = offset ;											// �Ǘ����̓o�^�F
			self->mpMsgQ[index].size   = size ;
			
			if ( ++index >= (self->mSize) ) index = 0 ;										// �������݃C���f�b�N�X���X�V����
			
			if ( index != (self->mRead) ) {													// < ���b�Z�[�W�L���[�ɋ󂫂����� >
				self->mWrite = index ;														// �� �������݃C���f�b�N�X���X�V����
			}
			else {																			// < ���b�Z�[�W�L���[�ɋ󂫂��Ȃ� >
				self->mStatus = LIB_MBF_ERR ;												// �� �G���[�����i�Ǘ��L���[�t���j
			}
			LIB_Unlock(key) ;																// �I�I�I�I�I�I�I�I�I�I ���荞�݋��� �I�I�I�I�I�I�I�I�I�I
		}
		else {																				// < �f�[�^�o�b�t�@�������݃G���[������ >
			self->mStatus = LIB_MBF_ERR ;													// ���b�Z�[�W�o�b�t�@�X�e�[�^�X���G���[�ɂ���
		}
	}

	return	(SS)(self->mStatus) ;
}


//=====================================================================================
// Function Name : CMessageBuffer::Receive()                                           
// Description   : ���b�Z�[�W�o�b�t�@���烁�b�Z�[�W����M����.                         
// Parameter     : *self		"�Ώۃ��b�Z�[�W�o�b�t�@�̃A�h���X"                     
//               : **message	"��M���b�Z�[�W�i�[�G���A�擪�A�h���X�������݃A�h���X" 
// Return        : ��M���b�Z�[�W�� �c "0"�̓��b�Z�[�W�Ȃ�                             
// Note          : ���ꃁ�b�Z�[�W�o�b�t�@�ɑ΂����M������1�����ōs�����Ƃ�O��Ƃ��� 
//=====================================================================================
static SS CMessageBuffer_Receive(CMessageBuffer *self, UC **message)
{
	SS	size   = 0 ;
	SS	offset = 0 ;
	SS	result = LIB_DBF_OK ;
	
	if ( self->mStatus != LIB_MBF_ERR ) {													// < �X�e�[�^�X���G���[��ԂłȂ� >
		if ( (self->mRead) != (self->mWrite) ) {											// < ���b�Z�[�W�o�b�t�@�Ƀ��b�Z�[�W������ >
			size   = self->mpMsgQ[self->mRead].size ;										// ���b�Z�[�W�����Z�[�u����
			offset = self->mpMsgQ[self->mRead].offset ;	
			
			result = self->mpBuffer->Read(self->mpBuffer, message, offset, size) ;			// �f�[�^�o�b�t�@���烁�b�Z�[�W����ǂݏo��
			if ( result != LIB_DBF_ERR ) {													// < �f�[�^�o�b�t�@�ǂݏo���G���[�͖����� >
				if ( ++(self->mRead) >= (self->mSize) ) self->mRead = 0 ;					// �ǂݏo���C���f�b�N�X���X�V����
			}
			else {
				size = 0 ;
			}
		}
	}

	return	size ;
}


//
// Change History
// Date:    | Engineer:          | Note:
// ---------+--------------------+-----------------------------------------------------------------------------------------------------------------------------
// 14.02.20 | Tsuyoshi Seto      | MSP Library > Message Buffer�̃v���g�^�C�v�Ƃ��Ė{�t�@�C�����쐬. (Rev.000)
//
//


// ************************************************************  [ LIB_MessageBuffer.c : EOF ]  ***************************************************************

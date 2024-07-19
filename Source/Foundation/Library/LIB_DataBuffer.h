// ������ Module Software Platform Library ������
/// @file    LIB_DataBuffer.h
/// @author  Tsuyoshi Seto
/// @date    14.09.22
/// @version 001
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// #####  ������ Data Buffer�g�p���̐ݒ荀�ڂ���ђ��ӎ��� ������  ############################################################################################
// < �{�t�@�C���ɂ�����v���_�N�g����j�[�N�Ȑݒ� >
//  �E�{�t�@�C���ɂ�����w�o�b�t�@�E�T�C�Y�iLIB_DBF_SIZE�j�x�͊e�v���_�N�g�ɂĐݒ肪�K�v�ƂȂ�B�i�R�����g�Ɂg������ �v���_�N�g����j�[�N �������h�ƋL�ځj
//
// < �f�[�^�o�b�t�@�g�p���̒��ӎ��� >
//  �E�{�f�[�^�o�b�t�@�̓����O�o�b�t�@�ɂ��\������邽�߁A�ォ��Write�������ɂ��ȑO�̃f�[�^�͏㏑�������B
//    ���̂��߁A�V�����f�[�^�ŏ㏑�������O�ɓǂݏo�����ł���悤�A�\���ȃT�C�Y���m�ۂ���K�v������B
// ############################################################################################################################################################

#ifndef LIB_DataBuffer_h
#define LIB_DataBuffer_h


// ******************************************************  <<<<< Definition & Declaration Area >>>>>  *********************************************************

// ===========================  <<< �}�N����` >>>  ===========================

// ----- �f�[�^�o�b�t�@�̃T�C�Y(�o�C�g��) -----
#define	LIB_DBF_SIZE		0x0400															// �o�b�t�@�E�T�C�Y��ݒ�		������ �v���_�N�g����j�[�N ������

// ----- �o�b�t�@�������� -----
#define	LIB_DBF_OK			0
#define	LIB_DBF_ERR			(-1)

// ----- �f�[�^�o�b�t�@�A���C�����g���� -----
#define	LIB_DATA_ALIGN		0x0003															// 4�o�C�g���E�z�u�p�萔
#define	LIB_ALIGN_MASK		(~LIB_DATA_ALIGN)


// ********************************************************  <<<<< Class Declaration Area >>>>>  **************************************************************

// ===========================  <<< �N���X��` >>>  ===========================

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Class Name    : CDataBuffer
// Description   : �f�[�^�i�[�p�o�b�t�@
// Note          : �Ȃ�
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
typedef struct DataBuffer {
/*public:*/
	//=[[[ Constructor ]]]=================================================================
	// Function Name : CDataBuffer::CDataBuffer()                                          
	// Description   : ���̃N���X�̃R���X�g���N�^.                                         
	// Parameter     : *self		"�Ώۃf�[�^�o�b�t�@�̃A�h���X"                         
	// Return        : -                                                                   
	//=====================================================================================
	// void CDataBuffer(CDataBuffer *self) {}

	//=====================================================================================
	// Function Name : CDataBuffer::Write()                                                
	// Description   : �f�[�^�o�b�t�@�Ƀf�[�^����������.                                   
	// Parameter     : *self		"�Ώۃf�[�^�o�b�t�@�̃A�h���X"                         
	//               : *data		"�������ݑΏۃf�[�^�̐擪�A�h���X"                     
	//               : size			"�������ݑΏۃf�[�^�̃T�C�Y"                           
	// Return        : �������݊J�n�ʒu�i�I�t�Z�b�g�o�C�g���j�c �}�C�i�X�͏������݃G���[   
	//=====================================================================================
	// SS Write(CDataBuffer *self, UC *data, SS size) {}
	SS (*Write)(struct DataBuffer*, UC*, SS) ;

	//=====================================================================================
	// Function Name : CDataBuffer::Read()                                                 
	// Description   : �f�[�^�o�b�t�@����f�[�^��ǂݏo��.                                 
	// Parameter     : *self		"�Ώۃf�[�^�o�b�t�@�̃A�h���X"                         
	//               : **area		"��M���b�Z�[�W�i�[�G���A�擪�A�h���X�������݃A�h���X" 
	//				 : offset		"�ǂݏo���J�n�ʒu�i�I�t�Z�b�g�o�C�g���j                
	//               : size			"�ǂݏo���Ώۃf�[�^�̃T�C�Y"                           
	// Return        : �f�[�^�ǂݏo�����ʁi0�FOK / -1:NG)                                  
	//=====================================================================================
	// SS Read(CDataBuffer *self, SS offset, UC *area, SS size) {}
	SS (*Read)(struct DataBuffer*, UC**, SS, SS) ;

/*private:*/
	UL	mBuffer[(LIB_DBF_SIZE - 1) / 4 + 1] ;													// �f�[�^�o�b�t�@�{�́i4�o�C�g���E�z�u�j
	US	mWrite ;																				// �������݊J�n�C���f�b�N�X
	SC	mStatus ;																				// �f�[�^�o�b�t�@�̏��
} CDataBuffer ;

// ===========================  <<< �֐��v���g�^�C�v�錾 >>>  ===========================
void CDataBuffer_CDataBuffer(CDataBuffer*) ;													// �f�[�^�o�b�t�@����������


#endif			// LIB_DataBuffer_h


//
// Change History
// Date:    | Engineer:          | Note:
// ---------+--------------------+-----------------------------------------------------------------------------------------------------------------------------
// 14.02.20 | Tsuyoshi Seto      | MSP Library > Data Buffer�̃v���g�^�C�v�Ƃ��Ė{�t�@�C�����쐬. (Rev.000)
// 14.09.22 | Tsuyoshi Seto      | Data Buffer�̈��4�o�C�g���E�ɔz�u����悤�^���C��. (Rev.001)
//
//


// *************************************************************  [ LIB_DataBuffer.h : EOF ]  *****************************************************************

// ������ Module Software Platform Library ������
/// @file    LIB_Service.c
/// @author  Tsuyoshi Seto
/// @date    14.02.20
/// @version 000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#define LIB_Service_c

#include "Common.h"
#include "LIB_Service.h"
#include "LIB_DataBuffer.h"


// ******************************************************  <<<<< Definition & Declaration Area >>>>>  *********************************************************

// ===========================  <<< �}�N����` >>>  =========================== 

// ----- �o�b�t�@���` -----
#define	LIB_MBF_EMPTY		0x00															// �f�[�^�o�b�t�@�Ƀf�[�^���Ȃ����


// ===========================  <<< �O���Q�ƕϐ� >>>  ===========================
extern CDataBuffer		lib_DataBuffer ;													// [�O���Q��] �f�[�^�o�b�t�@
extern CMessageBuffer	lib_MsgBuff[] ;														// [�O���Q��] ���b�Z�[�W�i�[�p�o�b�t�@
extern const MbfConfig 	lib_MbfConfig[] ;													// [�O���Q��] ���b�Z�[�W�o�b�t�@�R���t�B�M�����[�V����


// ************************************************************  <<<<< Program Code Area >>>>>  ***************************************************************

//=====================================================================================
// Function Name : LIB_StartUp()                                                       
// Description   : ���C�u�����̏�����.�i�ꊇ���ď���������j                           
// Parameter     : �Ȃ�                                                                
// Return        : �Ȃ�                                                                
//=====================================================================================
void LIB_StartUp()
{
	US	index ;

// ----- �f�[�^�o�b�t�@������ -----
	CDataBuffer_CDataBuffer(&lib_DataBuffer) ;												// �f�[�^�o�b�t�@������������

// ----- ���b�Z�[�W�o�b�t�@������ -----
	for ( index = 0 ; index < eNumOfMbf ; index++ ) {										// �e���b�Z�[�W�o�b�t�@������������
		CMessageBuffer_CMessageBuffer(&lib_MsgBuff[index], &lib_MbfConfig[index]) ;
	}
}


//=====================================================================================
// Function Name : LIB_snd_mbf()                                                       
// Description   : ���b�Z�[�W�𑗐M����.                                               
// Parameter     : mbfID		"�Ώۃ��b�Z�[�W�o�b�t�@ID"                             
//               : *msg			"�ʒm�Ώۃ��b�Z�[�W�̐擪�A�h���X"                     
//               : size			"�ʒm�Ώۃ��b�Z�[�W��"                                 
// Return        : ���M�������ʁi0�FOK / -1�FNG)                                       
//=====================================================================================
SS LIB_snd_mbf(EMbfID mbfID, UC *msg, SS size)
{
	return	lib_MsgBuff[mbfID].Send(&lib_MsgBuff[mbfID], msg, size) ;
}


//=====================================================================================
// Function Name : LIB_rcv_mbf()                                                       
// Description   : ���b�Z�[�W����M����.                                               
// Parameter     : mbfID		"�Ώۃ��b�Z�[�W�o�b�t�@ID"                             
//               : **msg		"��M���b�Z�[�W�i�[�G���A�擪�A�h���X�������݃A�h���X" 
// Return        : ��M���b�Z�[�W�� �c "0"�̓��b�Z�[�W�Ȃ�                             
// Note          : ���ꃁ�b�Z�[�W�o�b�t�@�ɑ΂����M������1�����ōs�����Ƃ�O��Ƃ��� 
//=====================================================================================
SS LIB_rcv_mbf(EMbfID mbfID, UC **msg)
{
	return	lib_MsgBuff[mbfID].Receive(&lib_MsgBuff[mbfID], msg) ;
}


//
// Change History
// Date:    | Engineer:          | Note:
// ---------+--------------------+-----------------------------------------------------------------------------------------------------------------------------
// 14.02.20 | Tsuyoshi Seto      | MSP Library > Message Buffer�̃v���g�^�C�v�Ƃ��Ė{�t�@�C�����쐬. (Rev.000)
//
//


// ***************************************************************  [ LIB_Service.c : EOF ]  ******************************************************************

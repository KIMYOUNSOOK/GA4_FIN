// ������ Module Software Platform Frame ������
/// @file    LIB_StopWatch.h
/// @author  Tsuyoshi Seto
/// @date    15.01.30
/// @version 001
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#define LIB_StopWatch_c

#include "Common.h"
#include "DD_Event.h"
#include "LIB_StopWatch.h"


// *******************************************************  <<<<< Definition & Declaration Area >>>>>  ********************************************************

// ===========================  <<< �}�N����` >>>  ===========================


// ==========================  <<< �f�[�^�^��` >>>  ==========================


// ===========================  <<< �֐��v���g�^�C�v�錾 >>>  ===========================
// static void CStopWatch_Start(CStopWatch*) ;												// ���Ԍv���J�n���� ������
// static bool CStopWatch_Stop(CStopWatch*, UL*) ;											// ���Ԍv���I������ ������
// static bool CStopWatch_Split(CStopWatch*, UL*) ;											// �o�ߎ��Ԏ擾���� ������


// ************************************************************  <<<<< Program Code Area >>>>>  ***************************************************************

//=[[[ Constructor ]]]=================================================================
// Function Name : CStopWatch::CStopWatch()
// Description   : ���̃N���X�̃R���X�g���N�^.
// Parameter     : �Ȃ�
// Return        : -
//=====================================================================================
void CStopWatch_CStopWatch(CStopWatch *self)
{
// ----- �����o�[������ -----
	self->mTime = 0 ;
	self->mStatus = OFF ;

// ----- �֐��|�C���^�[�ݒ� -----															// �����o�[�֐����������ڊ֐������ɕύX('15/1/30)
//	self->Start = CStopWatch_Start ;														// Start()�F���Ԍv���J�n���� ������
//	self->Stop  = CStopWatch_Stop ;															// Stop() �F���Ԍv���I������ ������
//	self->Split = CStopWatch_Split ;														// Split()�F�o�ߎ��Ԏ擾���� ������
}

//=====================================================================================
// Function Name : CStopWatch::Start()
// Description   : �X�g�b�v�E�H�b�`�ɂ�鎞�Ԍv�����J�n����.
// Parameter     : �Ȃ�
// Return        : �Ȃ�
//=====================================================================================
void CStopWatch_Start(CStopWatch *self)
{
	self->mTime = DD_EVT_GetCurrentTime() ;
	self->mStatus = ON ;
}

//=====================================================================================
// Function Name : CStopWatch::Stop()
// Description   : �X�g�b�v�E�H�b�`�ɂ�鎞�Ԍv�����擾&�v���I������.
// Parameter     : *pTime		"�v�����Ԋi�[�A�h���X"
// Return        : �������ʁi�v���J�n���Ă��Ȃ��Ƃ���"false"�j
//=====================================================================================
bool CStopWatch_Stop(CStopWatch *self, UL *pTime)
{
	UL currentTime = DD_EVT_GetCurrentTime() ;
	
	if ( (self->mStatus) == ON ) {
		if ( currentTime >= (self->mTime) ) {
			*pTime  = currentTime - (self->mTime) ;
		}
		else {																				// �^�̍ő�l����荞��ł���ꍇ
			*pTime  = currentTime + ( 0xFFFFFFFF - (self->mTime) ) + 1;
		}
		self->mStatus = OFF ;
		return 	true ;
	}
	else {
		*pTime = 0 ;
		return 	false ;
	}
}

//=====================================================================================
// Function Name : CStopWatch::Split()
// Description   : �X�g�b�v�E�H�b�`�ɂ��r���o�߂̎��Ԃ��擾����.
// Parameter     : *pTime		"�v�����Ԋi�[�A�h���X"
// Return        : �������ʁi�v���J�n���Ă��Ȃ��Ƃ���"false"�j
//=====================================================================================
bool CStopWatch_Split(CStopWatch *self, UL *pTime)
{
	UL currentTime = DD_EVT_GetCurrentTime() ;
	
	if ( (self->mStatus) == ON ) {
		if ( currentTime >= (self->mTime) ) {
			*pTime  = currentTime - (self->mTime) ;
		}
		else {																				// �^�̍ő�l����荞��ł���ꍇ
			*pTime  = currentTime + ( 0xFFFFFFFF - (self->mTime) ) + 1;
		}
		return 	true ;
	}
	else {
		*pTime = 0 ;
		return 	false ;
	}
}


// Change History
// Date:    | Engineer:          | Note:
// ---------+--------------------+-----------------------------------------------------------------------------------------------------------------------------
// 14.02.20 | Tsuyoshi Seto      | Module Software Platform Frame�̃v���g�^�C�v�Ƃ��Ė{�t�@�C�����쐬. (Rev.000)
// 15.01.30 | Tsuyoshi Seto      | �����o�[�֐�(�֐��|�C���^�[)�������璼�ڊ֐��R�[�������ɕύX.<RAM�g�p�ʒጸ�̂���> (Rev.001)
//
//


// ***************************************************************  [ LIB_StopWatch.c : EOF ]  ****************************************************************

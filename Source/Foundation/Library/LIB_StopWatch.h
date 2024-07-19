// ������ Module Software Platform Library ������
/// @file    LIB_StopWatch.h
/// @author  Tsuyoshi Seto
/// @date    15.01.30
/// @version 001
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef LIB_StopWatch_h
#define LIB_StopWatch_h


// *******************************************************  <<<<< Definition & Declaration Area >>>>>  ********************************************************

// ===========================  <<< �}�N����` >>>  ===========================


// ***********************************************************  <<<<< Class Declaration Area >>>>>  ***********************************************************

// ===========================  <<< �N���X��` >>>  ===========================

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Class Name    : CStopWatch
// Description   : �����x�ȃX�g�b�v�E�H�b�`.�i�ėp���C�u�����N���X�j
// Note          : 1�J�E���g��1000[ns] : 0[ms]�`65.535[ms]
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
typedef struct StopWatch {
/*public:*/
	//=[[[ Constructor ]]]=================================================================
	// Function Name : CStopWatch::CStopWatch()
	// Description   : ���̃N���X�̃R���X�g���N�^.
	// Parameter     : �Ȃ�
	// Return        : -
	//=====================================================================================
	// CStopWatch() { mTime = 0 ; mStatus = OFF ; }

	//=====================================================================================
	// Function Name : CStopWatch::Start()
	// Description   : �X�g�b�v�E�H�b�`�ɂ�鎞�Ԍv�����J�n����.
	// Parameter     : �Ȃ�
	// Return        : �Ȃ�
	//=====================================================================================
	// void Start() { mTime = DD_GetUsec() ; mStatus = ON ; }
	// ������void (*Start)(struct StopWatch*) ;												// �����o�[�֐����������ڊ֐������ɕύX('15/1/30)

	//=====================================================================================
	// Function Name : CStopWatch::Stop()
	// Description   : �X�g�b�v�E�H�b�`�ɂ�鎞�Ԍv�����擾&�v���I������.
	// Parameter     : *pTime		"�v�����Ԋi�[�A�h���X"
	// Return        : �������ʁi�v���J�n���Ă��Ȃ��Ƃ���"false"�j
	//=====================================================================================
	// bool Stop(US *pTime)
	// ������bool (*Stop)(struct StopWatch*, UL*) ;											// �����o�[�֐����������ڊ֐������ɕύX('15/1/30)

	//=====================================================================================
	// Function Name : CStopWatch::Split()
	// Description   : �X�g�b�v�E�H�b�`�ɂ��r���o�߂̎��Ԃ��擾����.
	// Parameter     : *pTime		"�v�����Ԋi�[�A�h���X"
	// Return        : �������ʁi�v���J�n���Ă��Ȃ��Ƃ���"false"�j
	//=====================================================================================
	// bool Split(US *pTime)
	// ������bool (*Split)(struct StopWatch*, UL*) ;										// �����o�[�֐����������ڊ֐������ɕύX('15/1/30)

/*private:*/
	UL	mTime ;																				// �X�^�[�g���Ԋi�[�f�[�^
	UC	mStatus ;																			// �X�g�b�v�E�H�b�`�̓�����
} CStopWatch ;


// ========================  <<< �v���g�^�C�v�錾 >>>  ========================
void CStopWatch_CStopWatch(CStopWatch*) ;													// �X�g�b�v�E�H�b�`�̏�����

void CStopWatch_Start(CStopWatch*) ;														// ���Ԍv���J�n���� ������
bool CStopWatch_Stop(CStopWatch*, UL*) ;													// ���Ԍv���I������ ������
bool CStopWatch_Split(CStopWatch*, UL*) ;													// �o�ߎ��Ԏ擾���� ������


#endif			// LIB_StopWatch_h


// Change History
// Date:    | Engineer:          | Note:
// ---------+--------------------+-----------------------------------------------------------------------------------------------------------------------------
// 14.02.20 | Tsuyoshi Seto      | Module Software Platform Frame�̃v���g�^�C�v�Ƃ��Ė{�t�@�C�����쐬. (Rev.000)
// 15.01.30 | Tsuyoshi Seto      | �����o�[�֐�(�֐��|�C���^�[)�������璼�ڊ֐��R�[�������ɕύX.<RAM�g�p�ʒጸ�̂���> (Rev.001)
//
//


// ***************************************************************  [ LIB_StopWatch.h : EOF ]  ****************************************************************

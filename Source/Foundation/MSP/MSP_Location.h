// ������ Module Software Platform Frame ������
/// @file    MSP_Location.h
/// @author  Mitsuyuki Nishizawa
/// @date    14.11.10
/// @version 001
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef MSP_Location_h
#define MSP_Location_h


// *******************************************************  <<<<< Definition & Declaration Area >>>>>  ********************************************************

// ----- ���P�[�V������^�C�}�[ -----
#define	MSP_LOC_TMR_STOP	0																// �X�e�[�^�X�F�^�C�}�[���g�p�i��~���j
#define	MSP_LOC_TMR_RUN		1																// �X�e�[�^�X�F�^�C�}�[�g�p���i���쒆�j

// ----- ���P�[�V������^�C�~���O�X�V��� -----
#define	MSP_LOC_UPDT_ENBL	1																// ���P�[�V������^�C�~���O�X�V�ł���
#define	MSP_LOC_UPDT_DSBL	0																// ���P�[�V������^�C�~���O�X�V�ł��Ȃ�

// ----- OS�^�C�}�[�̈�R���g���[����` -----
#define	OS_TMR_KEEP			1																// OS�^�C�}�[�̈�m��


// ==========================  <<< �f�[�^�^��` >>>  ==========================				// �R�}���h�����P�[�V������C�x���g�^��MSP_EventID.h�ɂ���܂�.

// ----- ���P�[�V������^�C�}�[�̃f�[�^�^ -----
typedef struct {																			// <<< "Location Time Information"�̌^ >>>	
	US mTrigger ;																			// ���P�[�V������^�C�}�[�̃g���K�[�ƂȂ����C�x���gID
	US mSheetNo ;																			// ���P�[�V������^�C�}�[�ΏۂƂȂ�V�[�g��i���o�[
	US mTimer ;																				// ���P�[�V������^�C�}�[ID
} TimerInfo ;

typedef struct {																			// <<< "Location Timer"�̌^ >>>
	US	mStatus  : 1 ;																		// ���P�[�V������^�C�}�[�N���X�e�[�^�X
	US	mTimerID : 15 ;																		// OS�^�C�}�[ID
	UL	mPattern ;																			// ���P�[�V������p�^�[�� (for Sheet)
	UC	mIndex ;																			// ���P�[�V������X�P�W���[����C���f�b�N�X
	TimerInfo  mInfo ;																		// ���P�[�V������^�C�}�[���
} LocationTimer ;


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Class Name    : CTimingController
// Description   : ���P�[�V�����E�C�x���g�̐�����ʒm�����{����.
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
typedef struct TimingController {
/*public:*/
	//=[[[ Constructor ]]]=================================================================
	// Function Name : CTimingController::CTimingController()
	// Description   : ���̃N���X�̃R���X�g���N�^.
	// Parameter     : pSpec		"�X�y�b�N��e�[�u���ւ̃|�C���^"
	// Return        : -
	//=====================================================================================
	//CTimingController(const LocationSpec *pSpec) ;

	//=====================================================================================
	// Function Name : CTimingController::ResetSchedule()
	// Description   : ���P�[�V������C�x���g�̃X�P�W���[���e�[�u�������Z�b�g����.
	// Parameter     : �Ȃ�
	// Return        : �Ȃ�
	//=====================================================================================
	//void ResetSchedule() ;

	//=====================================================================================
	// Function Name : CTimingController::UpdateTiming()
	// Description   : ���P�[�V������C�x���g�̔����^�C�~���O���X�V����.
	// Parameter     : location		"�X�V�Ώۂ̃��P�[�V�����ԍ��i=Event Number�j"
	//               : pTiming		"�X�V�^�C�~���O�e�[�u���擪�A�h���X"
	// Return        : �Ȃ�
	//=====================================================================================
	//void UpdateTiming(US location, const SS *pTiming) ;

	//=====================================================================================
	// Function Name : CTimingController::CreateSchedule()
	// Description   : ���P�[�V������C�x���g�̃X�P�W���[���e�[�u�����\�z����.
	// Parameter     : �Ȃ�
	// Return        : �Ȃ�
	//=====================================================================================
	//void CreateSchedule() ;

	//=====================================================================================
	// Function Name : CTimingController::NotifyTrigger()
	// Description   : ���P�[�V������C�x���g�̔����g���K�[��ʒm����.
	// Parameter     : trigger		"�g���K�[�ƂȂ郍�P�[�V�����ԍ��i=Event Number�j
	//               : sheetNo		"�ΏۃV�[�g��i���o�["
	//               : pattern		"�ΏۃV�[�g�̃��P�[�V������p�^�[��"
	//               : index		"�ΏۃV�[�g�̃X�P�W���[����C���f�b�N�X�ԍ�"
	// Return        : �Ȃ�
	//=====================================================================================
	//void NotifyTrigger(US trigger, US sheetNo, UL pattern, UC index) ;

	//=====================================================================================
	// Function Name : CTimingController::NotifyTimeout()
	// Description   : ���P�[�V������C�x���g�̔����^�C�~���O��ʒm����.
	// Parameter     : timerID		"�^�C�~���O�����p��Timer ID"
	//               : location		"�g���K�[�ƂȂ郍�P�[�V�����ԍ��i=Event Number�j
	//               : sheetNo		"�ΏۃV�[�g��i���o�["
	//               : timer		"���P�[�V������^�C�}�[�ԍ�"
	// Return        : �Ȃ�
	// Note          : ���̏����͊�{�I��Timer���荞�݃n���h���[�Ŏ��{�����.
	//               : ��NotifyTrigger()���̂݃C�x���g���[�v����̌Ăяo���ƂȂ�\������.
	//=====================================================================================
	//void NotifyTimeout(SL timerID, US location, US sheetNo, US timer) ;

	//=====================================================================================
	// Function Name : CTimingController::AbortScheduling()
	// Description   : ���P�[�V������C�x���g�̃X�P�W���[�����O��^�C�}�[���L�����Z������.
	// Parameter     : �Ȃ�
	// Return        : �Ȃ�
	//=====================================================================================
	//void AbortScheduling() ;

/*private:*/
	US mNext[MSP_NUM_OF_LOC][MSP_NUM_OF_SCH] ;												// [Member] ���̃��P�[�V�����̃��P�[�V����ID�i�O��RAM�z�u�̂���static�j
	US mDelay[MSP_NUM_OF_LOC][MSP_NUM_OF_SCH] ;												// [Member] ���̃��P�[�V�����̔����x�����ԁi�O��RAM�z�u�̂���static�j

	const LocationSpec *mpSpec ;															// [Member] Location Timing Controller�̃X�y�b�N
	LocationTimer mTimer[MSP_NUM_OF_TMR] ;													// [Member] ���P�[�V���������^�C�}�[����\�[�X
	UC mTimerIndex ;																		// [Member] �^�C�}�[����\�[�X�Ǘ��p�C���f�b�N�X
	UC mUpdateStatus ;																		// [Member] ���P�[�V������^�C�~���O�X�V���

	//=====================================================================================
	// Function Name : CTimingController::CreateEvent()
	// Description   : ���P�[�V������C�x���g�ʒm����.
	// Parameter     : location		"�ʒm�ΏۂƂȂ郍�P�[�V�����ԍ��i=Event Number�j
	//               : sheetNo		"�ΏۃV�[�g��i���o�["
	//               : pattern		"�ΏۃV�[�g�̃��P�[�V������p�^�[��"
	// Return        : �Ȃ�
	//=====================================================================================
	//void CreateEvent(US location, US sheetNo, UL pattern) ;

	//=====================================================================================
	// Function Name : CTimingController::GetNextSchedule()
	// Description   : ���̃��P�[�V������C�x���g�����擾����.
	// Parameter     : location		"�ʒm�ΏۂƂȂ郍�P�[�V�����ԍ��i=Event Number�j
	//               : pattern		"�ΏۃV�[�g�̃��P�[�V������p�^�[��"
	//               : index		"�ΏۃV�[�g�̃X�P�W���[����C���f�b�N�X�ԍ�"
	//               : pNext		"���̃��P�[�V�����ԍ��i�[��A�h���X"
	// Return        : ���̃��P�[�V������C�x���g�̒x�����ԁi"�����P�[�V�����Ȃ�"��0xFFFF�j
	// Note          : �X�P�W���[�����O��p�^�[���̃C���f�b�N�X�̓v���_�N�g����j�[�N
	//=====================================================================================
	//US GetNextSchedule(US location, UL pattern, UC index, US *pNextLocation)) ;

	//=====================================================================================
	// Function Name : CTimingController::GetTimer()
	// Description   : ���P�[�V������C�x���g�����p�^�C�}�[����\�[�X���擾����.
	// Parameter     : �Ȃ�
	// Return        : �擾���ꂽ���P�[�V������^�C�}�[�ԍ��i�擾�ł��Ȃ��Ƃ���0xFFFF�j
	//=====================================================================================
	//US GetTimer() ;

	//=====================================================================================
	// Function Name : CTimingController::ReleaseTimer()
	// Description   : ���P�[�V������^�C�}�[����\�[�X���������.
	// Parameter     : timer		"���P�[�V������^�C�}�[�ԍ�"
	// Return        : �Ȃ�
	//=====================================================================================
	//void ReleaseTimer(US timer) ;
} CTimingController ;


// ===========================  <<< �֐��v���g�^�C�v�錾 >>>  ===========================
void CTimingController_CTimingController(const LocationSpec*) ;								// ���P�[�V�����E�R���g���[���[����������
void CTimingController_ResetSchedule() ;													// ���P�[�V�����E�C�x���g��X�P�W���[���X�V
void CTimingController_UpdateTiming(US, const SS*) ;										// ���P�[�V�����E�C�x���g���X�V
void CTimingController_CreateSchedule() ;													// ���P�[�V�����E�C�x���g��X�P�W���[������
void CTimingController_NotifyTrigger(US, US, UL, UC) ;										// ���P�[�V�����E�C�x���g�����g���K�[�ʒm
void CTimingController_NotifyTimeout(SS, US, US, US) ;										// ���P�[�V�����E�C�x���g����
void CTimingController_AbortScheduling() ;													// ���P�[�V�����E�C�x���g�����������~


#endif			// MSP_Location_h


// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+----------------------------------------------------------------------------------------------------------------------------
// 13.05.05 | Mitsuyuki Nishizawa | Module Software Platform Frame�̃v���g�^�C�v�Ƃ��Ė{�t�@�C�����쐬.
// 14.11.10 | Mitsuyuki Nishizawa | �֐����̂���ш�����ύX&������������ǉ�.
//
//


// **************************************************************  [ MSP_Location.h : EOF ]  ******************************************************************

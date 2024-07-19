// ������ Module Software Platform Frame ������
/// @file    MSP_Location.c
/// @author  Mitsuyuki Nishizawa
/// @date    15.02.12
/// @version 002
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#define MSP_Location_c

#include "Common.h"
#include "LIB_Service.h"

#include "MSP_Common.h"
#include "MSP_Config.h"
#include "MSP_EventID.h"
#include "MSP_ProductSpec.h"
#include "MSP_EventMain.h"
#include "MSP_EventTimer.h"
#include "MSP_Location.h"


// ===========================  <<< �֐��v���g�^�C�v�錾 >>>  ===========================
void MSP_LocationCallback(SS, UL) ;															// ���P�[�V������^�C�}�[�̃R�[���o�b�N����

static void CTimingController_CreateEvent(US, US, UL) ;										// ���P�[�V������C�x���g����&�ʒm
static US CTimingController_GetNextSchedule(US, UL, UC, US*) ;								// ���̓���X�P�W���[���̎擾
static US CTimingController_GetTimer() ;													// �^�C�}�[����\�[�X�擾
static void CTimingController_ReleaseTimer(US) ;											// �^�C�}�[����\�[�X���


// ========================  <<< �C���X�^���X���� >>>  ========================

// ----- ���P�[�V������^�C�~���O��R���g���[���[ -----
CTimingController	msp_TimingController ;													// ���P�[�V������^�C�~���O��R���g���[���[


// ===========================  <<< �O���Q�ƕϐ� >>>  ===========================
extern CEventTimer	msp_EventTimer ;														// [�O���Q��] �C�x���g�^�C�}�[


//$$$$$$ [[[[[ Class: CTimingController ]]]]] $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// Class Name    : CTimingController�i���P�[�V������^�C�~���O�����ҁj
// Description   : ���P�[�V������C�x���g�̐�����ʒm�����{����.
//               :
// Note          : ���ɂȂ�
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

//=[[[ Constructor ]]]=================================================================
// Function Name : CTimingController::CTimingController()
// Description   : ���̃N���X�̃R���X�g���N�^.
// Parameter     : pSpec		"�X�y�b�N��e�[�u���ւ̃|�C���^"
// Return        : -
//=====================================================================================
void CTimingController_CTimingController(const LocationSpec *pSpec)
{
	CTimingController	*self = &msp_TimingController ;

	self->mpSpec = pSpec ;																	// �X�y�b�N��e�[�u����o�^����

// ----- ���P�[�V������X�P�W���[����e�[�u���̃��Z�b�g -----
	CTimingController_ResetSchedule() ;
	CTimingController_AbortScheduling() ;
	
// ----- ���P�[�V������R���g���[�����̏����� -----
	self->mTimerIndex   = 0 ;
	self->mUpdateStatus = MSP_LOC_UPDT_DSBL ;
}


//=====================================================================================
// Function Name : CTimingController::ResetSchedule()
// Description   : ���P�[�V������C�x���g�̃X�P�W���[���e�[�u�������Z�b�g����.
// Parameter     : �Ȃ�
// Return        : �Ȃ�
//=====================================================================================
void CTimingController_ResetSchedule()
{
	CTimingController  *self = &msp_TimingController ;
	SL	location, schedule ;

	if ( self->mUpdateStatus == MSP_LOC_UPDT_DSBL ) {										// ���P�[�V������^�C�~���O�X�V��Ԃ�"�X�V�s�\"�ł���

	// ----- �X�P�W���[����e�[�u���̃��Z�b�g -----
		for ( location = 0 ; location < MSP_NUM_OF_LOC ; location++ ) {						// [[[ LOOP ]]] �f�o�C�X����P�[�V����ID���ɏ��������{����i���[�v#1�j
			for ( schedule = 0 ; schedule < MSP_NUM_OF_SCH ; schedule++ ) {					// [[[ LOOP ]]] �X�P�W���[�������̏��������{����i���[�v#2�j
				self->mNext[location][schedule]  = MSP_EVT_NUM_VOID ;						// ���̃��P�[�V����ID��"VOID"�ɂ���
				self->mDelay[location][schedule] = self->mpSpec[location].mDelay[schedule] ; // �x�����Ԃ�"�m�~�i���l"���Z�b�g����
			}																				// [[[ LOOP ]]] �i�����܂Ń��[�v#2�j
		}																					// [[[ LOOP ]]] �i�����܂Ń��[�v#1�j

	// ----- ���P�[�V������^�C�~���O�X�V��� -----
		self->mUpdateStatus = MSP_LOC_UPDT_ENBL ;											// �^�C�~���O�X�V�\�ȏ��
	}
}


//=====================================================================================
// Function Name : CTimingController::UpdateTiming()
// Description   : ���P�[�V������C�x���g�̔����^�C�~���O���X�V����.
// Parameter     : location		"�X�V�Ώۂ̃��P�[�V�����ԍ��i=Event Number�j"
//               : pTiming		"�X�V�^�C�~���O�e�[�u���擪�A�h���X"
// Return        : �Ȃ�
//=====================================================================================
void CTimingController_UpdateTiming(US location, const SS *pTiming)
{
	static const char  eMsg1[] = {"FRAME ERROR: Location ID Error"} ;
	static const char  eMsg2[] = {"FRAME ERROR: Location Update Disable"} ;

	CTimingController	*self = &msp_TimingController ;
	SL	schedule, time ;

// ----- ���P�[�V������^�C�~���O�̍X�V -----
	if ( self->mUpdateStatus == MSP_LOC_UPDT_ENBL ) {										// ���P�[�V������^�C�~���O�X�V��Ԃ�"�X�V�\"�ł���
		if ( location < MSP_NUM_OF_LOC ) {													// ���P�[�V�����ԍ��͗L��
			for ( schedule = 0 ; schedule < MSP_NUM_OF_SCH ; schedule++ ) {					// [[[ LOOP ]]] �X�P�W���[�������̍X�V���������{����i���[�v�j
				if ( self->mpSpec[location].mDelay[schedule] != 0xFFFF ) {					// �Ώۃ��P�[�V�����L��/�����̃`�F�b�N �� "0xFFFF"�͖����ȃ��P�[�V����
					time = self->mpSpec[location].mDelay[schedule] + pTiming[schedule] ;
					if ( time < 0 ) {														// <<< "�m�~�i���{�␳�l"���}�C�i�X >>>
						self->mDelay[location][schedule] = 0 ;								// �� ���P�[�V������^�C�~���O��"0"�Ƃ���
					}
					else {																	// <<< "�m�~�i���{�␳�l"���v���X >>>
						self->mDelay[location][schedule] = (US)time ;						// �� ���P�[�V������^�C�~���O���X�V����
					}
				}
			}																				// [[[ LOOP ]]] �i�����܂Ń��[�v�j
		}
		else {																				// ���P�[�V�����ԍ�������
			Printf(LOGID_FRAME, 0, (UC*)eMsg1, (US)sizeof(eMsg1)) ;
		}
	}
	else {
		Printf(LOGID_FRAME, 0, (UC*)eMsg2, (US)sizeof(eMsg2)) ;
	}
}


//=====================================================================================
// Function Name : CTimingController::CreateSchedule()
// Description   : ���P�[�V������C�x���g�̃X�P�W���[���e�[�u�����\�z����.
// Parameter     : �Ȃ�
// Return        : �Ȃ�
//=====================================================================================
void CTimingController_CreateSchedule()
{
	CTimingController	*self = &msp_TimingController ;
	SL	location, schedule ;
	US	current, next, delay ;

// ----- ���P�[�V������^�C�~���O�X�V��� -----
	if ( self->mUpdateStatus == MSP_LOC_UPDT_ENBL ) {										// ���P�[�V������^�C�~���O�X�V��Ԃ�"�X�V�\"�ł���
		self->mUpdateStatus = MSP_LOC_UPDT_DSBL ;											// �^�C�~���O�X�V�s�\�ȏ��

	// ----- �X�P�W���[����e�[�u���̍\�z -----
		for ( location = 0 ; location < MSP_NUM_OF_LOC ; location++ ) {						// [[[ LOOP ]]] �f�o�C�X����P�[�V����ID���ɏ��������{����i���[�v#1�j
			for ( schedule = 0 ; schedule < MSP_NUM_OF_SCH ; schedule++ ) {					// [[[ LOOP ]]] �X�P�W���[�������̏��������{����i���[�v#2�j
			// ----- �����ݒ� -----
				delay   = self->mDelay[location][schedule] ;								// �Ώۃ��P�[�V�����̒x�����Ԃ�"delay"�ɃZ�b�g
				current = MSP_GetEventNumber(self->mpSpec[location].mTrigger) ;				// �J�����g����P�[�V����ID�ɑΏۃ��P�[�V�����̃g���K�[���Z�b�g
				next    = self->mNext[current][schedule] ;									// �l�N�X�g����P�[�V����ID�Ƀg���K�[����P�[�V������"Next"���Z�b�g

			// ----- �X�P�W���[���e�[�u���̍X�V -----
				if ( ( current != MSP_EVT_NUM_VOID ) && ( delay != 0xFFFF ) ) {				// �X�P�W���[�������Ώۃ��P�[�V�����L��/�����̃`�F�b�N

				// ������ �݌v���� ������
				// �g���K�[����P�[�V�����̐ݒ肪"MSP_EVT_NUM_VOID"�ƂȂ��Ă���ꍇ�A
				// �ŏ㗬�̃��P�[�V�������������߁A�X�P�W���[�������̑ΏۂƂ��Ȃ��B
				// �i�g���K�[�����ʒm���Ƀn���h�����o�^����Ă���΃R�[���o�b�N�����B�j
				// �x�����Ԃ̐ݒ肪"0xFFFF"�ƂȂ��Ă���ꍇ�A���̃X�P�W���[��No.�ɑΉ�����
				// �e�[�u���ւ̓o�^�͎��s����Ȃ��B

				// ----- �X�P�W���[���ݒ�ꏊ�̌��� -----
					while ( next != MSP_EVT_NUM_VOID ) {									// [[[ LOOP ]]] �`�F�[���̒[���łȂ��Ԃ͏��������{����i���[�v#3�j
						if ( self->mDelay[next][schedule] <= delay ) {						// <<< "delay"�̒l��"next"�̒x�����Ԉȏ� >>>
							delay  -= self->mDelay[next][schedule];							// �� "delay"����"next"�̒x�����Ԃ���������
							current = next ;												//    �J�����g����P�[�V����ID��"next"�̃��P�[�V����ID�ɍX�V����
							next    = self->mNext[current][schedule] ;						//    �l�N�X�g����P�[�V����ID���J�����g����P�[�V������"Next"�ɍX�V����
						}
						else {																// <<< "delay"�̒l��"next"�̒x�����Ԗ��� >>>
							break ;															// �� ���[�v�𔲂���I
						}
					}																		// [[[ LOOP ]]] �i�����܂Ń��[�v#3�j

				// ----- �Ώۃ��P�[�V�����̃X�P�W���[���ݒ� -----
					self->mNext[current][schedule]   = (US)location ;						// �O�Ɍq���郍�P�[�V������"Next"��Ώۃ��P�[�V����ID�ɍX�V����
					self->mNext[location][schedule]  = next ;								// �Ώۃ��P�[�V������"Next"�����Ɍq���郍�P�[�V����ID�ɍX�V����
					self->mDelay[location][schedule] = delay ;								// �Ώۃ��P�[�V������"Delay"��O�̃��P�[�V�����Ƃ̍����ɍX�V����

					if ( next != MSP_EVT_NUM_VOID ) {										// ���Ɍq���郍�P�[�V����������΁c
						self->mDelay[next][schedule] -= delay ;								// ���̃��P�[�V������"Delay"����Ώۃ��P�[�V������"delay"�̒l����������
					}
				}
			}																				// [[[ LOOP ]]] �i�����܂Ń��[�v#2�j
		}																					// [[[ LOOP ]]] �i�����܂Ń��[�v#1�j
	}
}


//=====================================================================================
// Function Name : CTimingController::NotifyTrigger()
// Description   : ���P�[�V������C�x���g�̔����g���K�[��ʒm����.
// Parameter     : trigger		"�g���K�[�ƂȂ郍�P�[�V�����ԍ��i=Event Number�j
//               : sheetNo		"�ΏۃV�[�g��i���o�["
//               : pattern		"�ΏۃV�[�g�̃��P�[�V������p�^�[��"
//               : index		"�ΏۃV�[�g�̃X�P�W���[����C���f�b�N�X�ԍ�"
// Return        : �Ȃ�
//=====================================================================================
void CTimingController_NotifyTrigger(US trigger, US sheetNo, UL pattern, UC index)
{
	static const char  eMsg1[] = {"FRAME ERROR: Timer Set Error#1"} ;
	static const char  eMsg2[] = {"FRAME ERROR: Timer Resource Full"} ;
	static const char  eMsg3[] = {"FRAME ERROR: Invalid Location ID"} ;
	
	CTimingController	*self = &msp_TimingController ;
	TimerInfo  *pTimerInfo ;
	US	timer ;
	SS	osTimer ;
	Handler  info ;

// ----- �^�C�}�[����\�[�X�擾 & �C�x���g���� -----
	if ( trigger < MSP_NUM_OF_LOC ) {														// ���P�[�V�����ԍ��͗L��
		timer = CTimingController_GetTimer() ;												// ���P�[�V������^�C�}�[����\�[�X�擾

		if ( timer != MSP_LOC_TMR_FULL ) {													// �^�C�}�[����\�[�X���m�ۂł����I
			pTimerInfo = &(self->mTimer[timer].mInfo) ;
			
			pTimerInfo->mTrigger = trigger ;												// �p�����[�^�̐���
			pTimerInfo->mSheetNo = sheetNo ;												// �҂����Ԃ�0ms�̃^�C�}�[�͗̈�m�ې���ł��Ȃ����O�̂���
			pTimerInfo->mTimer   = timer;
			
			self->mTimer[timer].mPattern = pattern ;										// ���P�[�V������p�^�[�����Z�b�g
			self->mTimer[timer].mIndex   = index ;											// �X�P�W���[����C���f�b�N�X�ԍ����Z�b�g
			self->mTimer[timer].mTimerID = MSP_TMR_TERM ;									// ����!!! 0ms�x���̃^�C���A�E�g�������{�̂���Timer ID��0���Z�b�g����

			info.mpHandler = MSP_LocationCallback ;
			info.mParam = (UL)pTimerInfo ;
			osTimer = CEventTimer_Set(&msp_EventTimer, MSP_TMR_HIGH, 0, &info) ;			// ���D��x���A����^�C�}�[���Z�b�g
			
			// ������ �݌v���� ������
			// �g���K�[�����ׂ̈̏����i�^�X�N�j�̗D��x���Ⴂ���߁A
			// ���̃^�C�~���O�Ŋ���mTimerID���X�V����Ă���\��������B
			// ���̂���Timer�Z�b�g�̖߂�l�Ƃ��Ď擾���ꂽID�ł̍X�V�͎��{���Ȃ��B

			if ( osTimer < 0 ) {															// �^�C�}�[��Z�b�g���̃G���[�����I�I�I
				CTimingController_ReleaseTimer(timer) ;
				Printf(LOGID_FRAME, 0, (UC*)eMsg1, (US)sizeof(eMsg1)) ;
			}
		}
		else {																				// �^�C�}�[����\�[�X�m�ۂł����I
			Printf(LOGID_FRAME, 0, (UC*)eMsg2, (US)sizeof(eMsg2)) ;
		}
	}
	else {
		Printf(LOGID_FRAME, 0, (UC*)eMsg3, (US)sizeof(eMsg3)) ;
	}
}


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
void CTimingController_NotifyTimeout(SS timerID, US location, US sheetNo, US timer)
{
	static const char  eMsg1[] = {"FRAME ERROR: Timer Set Error#1"} ;
	static const char  eMsg2[] = {"FRAME ERROR: Timer Invalid Timeout"} ;

	CTimingController	*self = &msp_TimingController ;
	LocationTimer  *pTimer    = &(self->mTimer[timer]) ;
	TimerInfo  *pTimerInfo    = &(pTimer->mInfo) ;
	US	delay, next ;
	SS	osTimer ;
	Handler  info ;

	if ( ( pTimer->mTimerID == (US)timerID ) && ( pTimer->mStatus == MSP_LOC_TMR_RUN ) ) {		// �^�C�}�[ID�̐������`�F�b�N
		CTimingController_CreateEvent(location, sheetNo, pTimer->mPattern) ;					// ���P�[�V������C�x���g����

		delay = CTimingController_GetNextSchedule(location, pTimer->mPattern, pTimer->mIndex, &next) ;	// ���̃X�P�W���[�����擾

		while ( next != MSP_EVT_NUM_VOID ) {													// ���̃X�P�W���[��������I�i���[�v�j
			if ( delay > 0 ) {																	// <<< �x�����Ԃ�"0"�łȂ��Ƃ� >>>
				pTimerInfo->mTrigger = next ;													// �p�����[�^�̐���
				pTimerInfo->mSheetNo = sheetNo ;													// �҂����Ԃ�0ms�̃^�C�}�[�͗̈�m�ې���ł��Ȃ����O�̂���
				pTimerInfo->mTimer   = timer;		
				
				info.mpHandler = MSP_LocationCallback ;
				info.mParam = (UL)pTimerInfo ;
				
				osTimer = CEventTimer_Set(&msp_EventTimer, MSP_TMR_HIGH, delay, &info) ;		// ���D��x���A����^�C�}�[���Z�b�g
				
				if ( osTimer < 0 ) {															// �^�C�}�[��Z�b�g���̃G���[�����I�I�I
					CTimingController_ReleaseTimer(timer) ;
					Printf(LOGID_FRAME, 0, (UC*)eMsg1, (US)sizeof(eMsg1)) ;
				}
				else {																			// �^�C�}�[�Z�b�g����
					pTimer->mTimerID = osTimer ;												// OS�Ǘ��̃^�C�}�[ID���Z�[�u
				}

				break ;																			// ���[�v����E�o�I�I�I
			}
			else {																				// <<< �x�����Ԃ�"0"�̂Ƃ� >>>�@���^�X�N��f�B�X�p�b�`����������s���Ԃ��y���I�I�I
				pTimer->mTimerID = 0 ;															// �^�C�}�[ID��"0"�Ƃ���iOS�^�C�}�[��x��=0�ŃZ�b�g������ԂƓ����j
				CTimingController_CreateEvent(next, sheetNo, pTimer->mPattern) ;				// ���P�[�V������C�x���g����

				delay = CTimingController_GetNextSchedule(next, pTimer->mPattern, pTimer->mIndex, &next) ; // ���̃X�P�W���[�����擾 �� ���[�v���f��
			}
		}

		if ( next == MSP_EVT_NUM_VOID ) {														// ���̃X�P�W���[�����Ȃ��I
			CTimingController_ReleaseTimer(timer) ;												// ���P�[�V������^�C�}�[����\�[�X���
		}
	}
	else {																						// �^�C�}�[ID���s�����܂��̓^�C�}�[����~���
		CTimingController_ReleaseTimer(timer) ;													// ���P�[�V������^�C�}�[����\�[�X���
		Printf(LOGID_FRAME, 0, (UC*)eMsg2, (US)sizeof(eMsg2)) ;
	}
}


//=====================================================================================
// Function Name : CTimingController::AbortScheduling()
// Description   : ���P�[�V������C�x���g�̃X�P�W���[�����O��^�C�}�[���L�����Z������.
// Parameter     : �Ȃ�
// Return        : �Ȃ�
//=====================================================================================
void CTimingController_AbortScheduling()
{
	CTimingController  *self = &msp_TimingController ;
	SL	index ;

	for ( index = 0 ; index < MSP_NUM_OF_TMR ; index++ ) {									// [[[ LOOP ]]] �S�Ẵ^�C�}�[ID�ɑ΂��ď��������{����
		self->mTimer[index].mStatus  = MSP_LOC_TMR_STOP ;									// �^�C�}�[�Ǘ��e�[�u���̃��Z�b�g
		self->mTimer[index].mTimerID = MSP_LOC_TMR_VOID ;
		self->mTimer[index].mPattern = 0x00000000 ;
		self->mTimer[index].mIndex   = 0 ;

		self->mTimer[index].mInfo.mTrigger = 0 ;											// �^�C�}�[�ʒm���G���A�̃��Z�b�g
		self->mTimer[index].mInfo.mSheetNo = 0 ;
		self->mTimer[index].mInfo.mTimer   = 0 ;
	}																						// [[[ LOOP ]]]�i�����܂Ń��[�v�j
}


//=====================================================================================
// Function Name : CTimingController::CreateEvent()
// Description   : ���P�[�V������C�x���g�ʒm����.
// Parameter     : location		"�ʒm�ΏۂƂȂ郍�P�[�V�����ԍ��i=Event Number�j
//               : sheetNo		"�ΏۃV�[�g��i���o�["
//               : pattern		"�ΏۃV�[�g�̃��P�[�V������p�^�[��"
// Return        : �Ȃ�
//=====================================================================================
static void CTimingController_CreateEvent(US location, US sheetNo, UL pattern)
{
	CTimingController	*self = &msp_TimingController ;
	LocationEvent	event ;
	UL	filter ;
	US	eventID ;

	filter = self->mpSpec[location].mFilter ;												// ���P�[�V������C�x���g�̃t�B���^�����O��f�[�^���擾����

	if ( (pattern & filter) != 0x00000000 ) {												// ���P�[�V������C�x���g�����p�^�[���������Ă���H
		if ( (msp_EvtIndexTable[EVT_LOC_TMNG].mpControlTable)[location].mCount != 0 ) {		// �C�x���g��n���h�����o�^����Ă���H
		// ----- ���P�[�V������C�x���g���� -----
			eventID          = MSP_GetEventID(EVT_LOC_TMNG, location) ;
			event.mSheetNo   = sheetNo;
			event.mParameter = self->mpSpec[location].mParameter ;

		// ----- �C�x���g�ʒm -----
			MSP_SendEvent(MID_FRAME, eventID, (const UC*)&event, sizeof(LocationEvent), OFF) ; // ���P�[�V������C�x���g�𑗐M����
		}
	}
}


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
static US CTimingController_GetNextSchedule(US location, UL pattern, UC index, US *pNext)
{
	CTimingController	*self = &msp_TimingController ;
	UL	filter ;
	US	next, delay = 0x0000 ;

// ----- ���̃X�P�W���[�������� -----
	for ( next = self->mNext[location][index] ; next != MSP_EVT_NUM_VOID ; next = self->mNext[next][index] ) {
		filter = self->mpSpec[next].mFilter ;

		delay += self->mDelay[next][index] ;
		if ( (filter & pattern) != 0x00000000 ) {
			*pNext = next ;
			break ;
		}
	}

	if ( next == MSP_EVT_NUM_VOID ) {														// ���̃X�P�W���[�����Ȃ��I
		delay  = 0xFFFF ;																	// �x�����Ԃ̐ݒ���s���i"�����P�[�V�����Ȃ�"��0xFFFF�j
		*pNext = MSP_EVT_NUM_VOID ;															// ���̃��P�[�V�����ԍ���"VOID"
	}

	return	delay ;
}


//=====================================================================================
// Function Name : CTimingController::GetTimer()
// Description   : ���P�[�V������C�x���g�����p�^�C�}�[����\�[�X���擾����.
// Parameter     : �Ȃ�
// Return        : �擾���ꂽ���P�[�V������^�C�}�[�ԍ��i�擾�ł��Ȃ��Ƃ���0xFFFF�j
//=====================================================================================
static US CTimingController_GetTimer()
{
	CTimingController	*self = &msp_TimingController ;
	SL	count ;
	UC	key ;
	US	number = MSP_LOC_TMR_FULL ;
	static UC timerIndex = 0;																//  �^�C�}�[����\�[�X�Ǘ��p�C���f�b�N�X
	
	key = LIB_GetKey() ; LIB_Lock() ;														// ����!!! ���荞�݋֎~ ��������!!!

	for ( count = 0 ; count < MSP_NUM_OF_TMR ; count++ ) {									// �^�C�}�[����\�[�X�Ǘ��e�[�u�����`�F�b�N����
		if ( ++timerIndex >= MSP_NUM_OF_TMR ) timerIndex = 0 ;

		if ( self->mTimer[timerIndex].mStatus == MSP_LOC_TMR_STOP ) {						// ���\�[�X�̋󂫂��������I
			self->mTimer[timerIndex].mStatus  = MSP_LOC_TMR_RUN ;							// ===> �X�e�[�^�X�̍X�V�iTMR_RUN�j
			number = timerIndex ;															//      �߂�l�i�I�[�g�ϐ��j�̍X�V
			break ;
		}
	}

	LIB_Unlock(key) ;																		// ����!!! ���荞�݋֎~ �����܂�!!!

	return	number ;																		// ���P�[�V������^�C�}�[�ԍ������^�[��
}


//=====================================================================================
// Function Name : CTimingController::ReleaseTimer()
// Description   : ���P�[�V������^�C�}�[����\�[�X���������.
// Parameter     : timer		"���P�[�V������^�C�}�[�ԍ�"
// Return        : �Ȃ�
//=====================================================================================
static void CTimingController_ReleaseTimer(US timer) 
{
	CTimingController	*self = &msp_TimingController ;

	self->mTimer[timer].mTimerID = MSP_LOC_TMR_VOID ;
	self->mTimer[timer].mStatus  = MSP_LOC_TMR_STOP ;
	self->mTimer[timer].mPattern = 0x00000000 ;
	self->mTimer[timer].mIndex   = 0 ;
}


//=====================================================================================
// Function Name : MSP_LocationCallback()
// Description   : ���P�[�V������^�C�}�[�̃^�C���A�E�g�ʒm���󂯂�.
//               : param	"OS�^�C�}�[�̃R�[���o�b�N�p�����[�^"
// Return        : �Ȃ�
//=====================================================================================
void MSP_LocationCallback(SS timerID, UL param)
{
	TimerInfo *pInfo = (TimerInfo*)param ;

	CTimingController_NotifyTimeout(timerID, pInfo->mTrigger, pInfo->mSheetNo, pInfo->mTimer) ;
}


// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+----------------------------------------------------------------------------------------------------------------------------
// 14.04.01 | Mitsuyuki Nishizawa | Module Software Platform Frame�̃v���g�^�C�v�Ƃ��Ė{�t�@�C�����쐬.
// 14.11.10 | Mitsuyuki Nishizawa | �֐����̂���ш�����ύX&������������ǉ�.
// 15.02.12 | Yuichi Hoshino      | Printf()�ɓn��ID��Log ID�ɕύX
// 15.03.30 | Yuichi Hoshino      | Release version 002
//
//


// **************************************************************  [ MSP_Location.c : EOF ]  ******************************************************************

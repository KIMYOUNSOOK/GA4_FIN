// ������ Module Software Platform Frame ������
/// @file    MSP_EventTimer.c
/// @author  Mitsuyuki Nishizawa
/// @date    14.09.24
/// @version 002
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#define MSP_EventTimer_c

#include "Common.h"
#include "LIB_Service.h"

#include "MSP_Common.h"
#include "MSP_Config.h"
#include "MSP_EventID.h"
#include "MSP_ProductSpec.h"
#include "MSP_EventMain.h"
#include "MSP_EventTimer.h"


// ******************************************************  <<<<< Definition & Declaration Area >>>>>  *********************************************************

// ===========================  <<< �}�N����` >>>  ===========================


// ===========================  <<< �f�[�^�^��` >>>  ===========================


// ===========================  <<< �֐��v���g�^�C�v�錾 >>>  ===========================
static void CEventTimer_Fire(CEventTimer*, SS) ;											// �^�C���A�E�g����
static void CEventTimer_ResetField(CEventTimer*, T_SZ) ;									// �^�C�}�[�Ǘ����F����������
static void CEventTimer_SetField(CEventTimer*, T_SZ, UC, US, Handler*) ;					// �^�C�}�[�Ǘ����F���ݒ菈��
static T_SZ CEventTimer_SearchField(CEventTimer*) ;											// �^�C�}�[�Ǘ����F�󂫗̈挟������
static void CEventTimer_HookField(CEventTimer*, T_SZ) ;										// �^�C�}�[�Ǘ����F�o�^����
static void CEventTimer_RemoveField(CEventTimer*, T_SZ) ;									// �^�C�}�[�Ǘ����F��������
static UC CEventTimer_GetType(CEventTimer*, T_SZ) ;											// �^�C�}�[�Ǘ����F�D��x���擾����
static void CEventTimer_ExecuteHandler(CEventTimer*, T_SZ) ;								// �R�[���o�b�N�n���h���[���s����


// ========================  <<< �C���X�^���X���� >>>  ========================

// ----- �C�x���g�^�C�}�[ -----
CEventTimer msp_EventTimer ;																// �C�x���g�^�C�}�[ (1�̃o�b�t�@�𕡐��̃L���[�ŋ��L�\)


// ************************************************************  <<<<< Program Code Area >>>>>  ***************************************************************

//$$$$$$ [[[[[ Class: CEventTimer ]]]]] $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
// Class Name    : CEventTimer�i�C�x���g�^�C�}�[�j
// Description   : �^�C�}�[�Ǘ�.
//               :
// Note          : �Ȃ�
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

//=[[[ Constructor ]]]=================================================================
// Function Name : CEventTimer::CEventTimer()                                          
// Description   : ���̃N���X�̃R���X�g���N�^.                                         
// Parameter     : *self		"�Ώۃf�[�^�o�b�t�@�̃A�h���X"                         
// Return        : -                                                                   
//=====================================================================================
void CEventTimer_CEventTimer(CEventTimer *self)
{
	T_SZ  index ;

// ----- �����o�[������ -----
	self->mTop   = MSP_TMR_TERM ;															// �擪�̃^�C�}�[ID��"�Ȃ�"�ɂ���
	self->mIndex = MSP_MAX_TMR_NUM - 1 ;													// �󂫗̈挟���p�C���f�b�N�X�����Z�b�g����i���[�̈�j
	self->mCount = 0 ;																		// ���݂̃^�C�}�[�g�p�{�������Z�b�g����
	self->mMax   = 0 ;																		// �^�C�}�[�g�p�ő�{��(����)�����Z�b�g����

	for ( index = 0 ; index < MSP_MAX_TMR_NUM ; index++ ) {									// �^�C�}�[�Ǘ����̈���N���A����
		CEventTimer_ResetField(self, index) ;
	}
}


//=====================================================================================
// Function Name : CEventTimer::Set()                                                  
// Description   : �R�[���o�b�N��^�C�}�[���Z�b�g����.                                  
// Parameter     : *self		"�Ώۃ^�C�}�[�Ǘ����̃A�h���X"                       
//               : type			"�^�C�}�[�D��x�iHI/LO�j"                              
//               : delay		"�^�C�}�[�x������[ms]"                                 
//               : *handler		"�R�[���o�b�N�n���h���[���i�[��A�h���X"             
// Return        : �^�C�}�[ID�i�}�C�i�X�̓G���[�j                                      
//=====================================================================================
SS CEventTimer_Set(CEventTimer *self, UC type, US delay, Handler *handler)
{
	SS	result = MSP_TMR_ERR ;
	UC	key ;
	T_SZ  index ;

	key = LIB_GetKey() ; LIB_Lock() ;														// ����!!! ���荞�݋֎~ ��������!!!
	index = CEventTimer_SearchField(self) ;													// �^�C�}�[�Ǘ����̈�̋󂫂���������
	if ( index != MSP_TMR_TERM ) {															// < �󂫗̈悪�������� >
		CEventTimer_SetField(self, index, type, delay, handler) ;							// �Ǘ��f�[�^��o�^����
		result = (SS)index ;																// �߂�^�C�}�[ID�ɓo�^��C���f�b�N�X���Z�b�g����

		if ( delay == 0 ) {																	// < �x���Ȃ�(�x������"0ms")�Őݒ肳��Ă��� >
			CEventTimer_Fire(self, (SS)index) ;												// �����^�C���A�E�g���������{
			if ( type == MSP_TMR_HIGH ) {													// < �x���Ȃ������D��x�^�C�}�[�̂Ƃ� >
				result = (SS)MSP_TMR_TERM ;													// �߂�^�C�}�[ID��"TERM"���Z�b�g����i�R�[���o�b�N�ς̂��߁j
			}
		}
		else {																				// < �x������Őݒ肳��Ă��� >
			CEventTimer_HookField(self, index) ;											// �^�C�}�[�Ǘ��������X�g�ɓo�^����
		}
	}
	LIB_Unlock(key) ;																		// ����!!! ���荞�݋֎~ �����܂�!!!

	return	result ;
}


//=====================================================================================
// Function Name : CEventTimer::Cancel()                                               
// Description   : �R�[���o�b�N��^�C�}�[���L�����Z������.                              
// Parameter     : *self		"�Ώۃ^�C�}�[�Ǘ����̃A�h���X"                       
//               : id			"�L�����Z���Ώۃ^�C�}�[ID"                             
// Return        : �������ʁi0:�������� / -1:�������s�j                                
//=====================================================================================
SS CEventTimer_Cancel(CEventTimer *self, SS id)
{
	SS	result = MSP_TMR_ERR ;
	UC	status, key ;

	if ( ( id >= 0 ) && ( id < MSP_MAX_TMR_NUM ) ) {										// < �C�x���g�^�C�}�[ID���L���ł��� >
		key = LIB_GetKey() ; LIB_Lock() ;													// ����!!! ���荞�݋֎~ ��������!!!
		status = self->mField[id].mStatus ;													// < �C�x���g�^�C�}�[���N�����ł��� >
		if ( ( status == MSP_TMR_LOW ) || ( status == MSP_TMR_HIGH ) ) {					// �� �R�[���o�b�N�������{��ԂłȂ����ƁI
			CEventTimer_RemoveField(self, (T_SZ)id) ;										// �^�C�}�[�Ǘ��������X�g����폜����
			CEventTimer_ResetField(self, (T_SZ)id) ;										// �^�C�}�[�Ǘ���������������
			result = MSP_TMR_OK ;
		}
		LIB_Unlock(key) ;																	// ����!!! ���荞�݋֎~ �����܂�!!!
	}

	return	result ;
}


//=====================================================================================
// Function Name : CEventTimer::IsRunning()                                            
// Description   : �R�[���o�b�N��^�C�}�[�̓����Ԃ��擾����.                          
// Parameter     : *self		"�Ώۃ^�C�}�[�Ǘ����̃A�h���X"                       
//               : id			"�`�F�b�N�Ώۃ^�C�}�[ID"                               
// Return        : �^�C�}�[�����ԁi0:��~�� / 1:���쒆�j                             
//=====================================================================================
UC CEventTimer_IsRunning(CEventTimer *self, SS id)
{
	return	((self->mField[id].mStatus) & 0x01) ;
}


//=====================================================================================
// Function Name : CEventTimer::Tick()                                                 
// Description   : �J�E���g�_�E�����������s����.                                       
// Parameter     : *self		"�Ώۃ^�C�}�[�Ǘ����̃A�h���X"                       
// Return        : -                                                                   
//=====================================================================================
void CEventTimer_Tick(CEventTimer *self)
{
	TimerField	*field ;

	field = self->mField ;																	// �^�C�}�[�Ǘ����̈�̐擪�A�h���X���Z�b�g����

	if ( (self->mTop) != MSP_TMR_TERM ) {													// < �N������Ă���^�C�}�[�����݂��� >
		(field[self->mTop].mDelay)-- ;														// �擪�̃^�C�}�[�̒x�����Ԃ��f�N�������g����

		while ( (field[self->mTop].mDelay) == 0 ) {											// < �擪�̃^�C�}�[�̒x�����Ԃ�"0"�ƂȂ��� >
			CEventTimer_Fire(self, (SS)(self->mTop)) ;										// �R�[���o�b�N���������{����i�n���h���[���sor�C�x���g���s�j

			if ( (self->mTop) == MSP_TMR_TERM ) {											// < �ڑ��^�C�}�[���Ȃ��Ȃ��� >
				break ;
			}
		}
	}
}


//=====================================================================================
// Function Name : CEventTimer::Timeout()                                              
// Description   : �R�[���o�b�N���������s����. (��D��x�^�C�}�[��p�֐�)              
// Parameter     : *self		"�Ώۃ^�C�}�[�Ǘ����̃A�h���X"                       
//               : id			"�`�F�b�N�Ώۃ^�C�}�[ID"                               
// Return        : -                                                                   
//=====================================================================================
void CEventTimer_Timeout(CEventTimer *self, SS id)
{
	UC	key ;

	if ( ( id >= 0 ) && ( id < MSP_MAX_TMR_NUM ) ) {										// < �C�x���g�^�C�}�[ID���L���ł��� >
		if ( CEventTimer_GetType(self, (T_SZ)id) == MSP_TMR_LOW ) {							// < �C�x���g�^�C�}�[(���D��x)���N�����ł��� >
			CEventTimer_ExecuteHandler(self, (T_SZ)id) ;									// �R�[���o�b�N���������s����

			key = LIB_GetKey() ; LIB_Lock() ;												// ����!!! ���荞�݋֎~ ��������!!!
			CEventTimer_ResetField(self, (T_SZ)id) ;										// �^�C�}�[�Ǘ���������������
			LIB_Unlock(key) ;																// ����!!! ���荞�݋֎~ �����܂�!!!
		}
	}
}


//=====================================================================================
// Function Name : CEventTimer::Fire()                                                 
// Description   : �^�C���A�E�g���������s����.                                         
// Parameter     : *self		"�Ώۃ^�C�}�[�Ǘ����̃A�h���X"                       
//               : id			"�`�F�b�N�Ώۃ^�C�}�[ID"                               
// Return        : -                                                                   
//=====================================================================================
static void CEventTimer_Fire(CEventTimer *self, SS id)
{
	US	eventID = MSP_GetEventID(EVT_TIME_OUT, 0) ;

	if ( CEventTimer_GetType(self, (T_SZ)id) == MSP_TMR_HIGH ) {							// < ���D��x�^�C�}�[�̃^�C���A�E�g�ł��� >
		CEventTimer_ExecuteHandler(self, (T_SZ)id) ;										// �����R�[���o�b�N���������s����
		CEventTimer_RemoveField(self, (T_SZ)id) ;											// �^�C�}�[�������N����O��
		CEventTimer_ResetField(self, (T_SZ)id) ;											// �^�C�}�[�Ǘ���������������i�̈���������j
	}
	else if ( CEventTimer_GetType(self, (T_SZ)id) == MSP_TMR_LOW ) {						// < ��D��x�^�C�}�[�̃^�C���A�E�g�ł��� >
		CEventTimer_RemoveField(self, (T_SZ)id) ;											// �^�C�}�[�������N����O���i�̈�̓n���h���[���s�܂ŉ�����Ȃ��j
		MSP_SendEvent(MID_FRAME, (US)eventID, (const UC*)&id, sizeof(id), LOG_OFF) ;		// �^�C�}�[��C�x���g�𑗐M����i���O�o�͂��Ȃ��j
	}
}


//=====================================================================================
// Function Name : CEventTimer::ResetField()                                           
// Description   : �^�C�}�[�Ǘ���������������.                                       
// Parameter     : *self		"�Ώۃ^�C�}�[�Ǘ����̃A�h���X"                       
//               : index		"�������Ώۏ��i�[��C���f�b�N�X"                     
// Return        : -                                                                   
//=====================================================================================
static void CEventTimer_ResetField(CEventTimer *self, T_SZ index)
{
	self->mField[index].mInfo.mpHandler = MSP_HDLR_VOID ;									// �R�[���o�b�N��������������
	self->mField[index].mInfo.mParam    = 0 ;

	self->mField[index].mPrev   = MSP_TMR_TERM ;											// �^�C�}�[�Ǘ���������������
	self->mField[index].mNext   = MSP_TMR_TERM ;
	self->mField[index].mStatus = MSP_TMR_STOP ;
	self->mField[index].mZero   = CLEAR ;
	self->mField[index].mDelay  = 0 ;

	if ( (self->mCount) > 0 ) {																// [�f�o�b�O] �J�E���^�[�X�V�i-1�j
		(self->mCount)-- ;
	}
}


//=====================================================================================
// Function Name : CEventTimer::SetField()                                             
// Description   : �^�C�}�[�Ǘ�����ݒ肷��.                                         
// Parameter     : *self		"�Ώۃ^�C�}�[�Ǘ����̃A�h���X"                       
//               : index		"�������Ώۏ��i�[��C���f�b�N�X"                     
//               : type			"�^�C�}�[�D��x�iHI/LO�j"                              
//               : delay		"�^�C�}�[�x������[ms]"                                 
//               : *handler		"�R�[���o�b�N�n���h���[���i�[��A�h���X"             
// Return        : -                                                                   
//=====================================================================================
static void CEventTimer_SetField(CEventTimer *self, T_SZ index, UC type, US delay, Handler *handler)
{
	self->mField[index].mInfo   = *handler ;												// �R�[���o�b�N����ݒ肷��
	self->mField[index].mStatus = type ;													// �^�C�}�[�Ǘ�����ݒ肷��
	self->mField[index].mDelay  = delay ;

	if ( delay == 0 ) {																		// �ݒ�x�����Ԃ�0[ms]�����f����
		self->mField[index].mZero = SET ;
	}
	else {
		self->mField[index].mZero = CLEAR ;
	}

	if ( ++(self->mCount) > (self->mMax) ) {												// [�f�o�b�O] �J�E���^�[�X�V�i+1�j
		self->mMax = self->mCount ;
	}
}


//=====================================================================================
// Function Name : CEventTimer::SearchField()                                          
// Description   : �^�C�}�[�Ǘ����̋󂫗̈����������.                               
// Parameter     : *self		"�Ώۃ^�C�}�[�Ǘ����̃A�h���X"                       
// Return        : �^�C�}�[�Ǘ����̋󂫗̈�������C���f�b�N�X�iTERM:�󂫗̈�Ȃ�)    
//=====================================================================================
static T_SZ CEventTimer_SearchField(CEventTimer *self)
{
	T_SZ  limit, index ;

	index = self->mIndex ;

	for ( limit = 0 ; limit < MSP_MAX_TMR_NUM ; limit++ ) {									// [[[ �J��Ԃ� ]]] �F�ő�񐔂͗̈�m�ې�
		if ( ++index >= MSP_MAX_TMR_NUM ) {													// �Ō�Ƀ`�F�b�N�������̈�ɃC���f�b�N�X���X�V����
			index = 0 ;
		}

		if ( (self->mField[index].mStatus) == MSP_TMR_STOP ) {								// < ���g�p�^�C�}�[�̈�𔭌����� >
			self->mIndex = index ;															// �����J�n�C���f�b�N�X���X�V����
			break ;																			// �󂫗̈�̌������������I�I�I
		}
	}																						// [[[ �J��Ԃ� ]]] �F�����܂�

	if ( limit == MSP_MAX_TMR_NUM ) {														// < �󂫗̈�𔭌��ł��Ȃ����� >
		index = MSP_TMR_TERM ;																// �C���f�b�N�X��TERM���Z�b�g����
	}

	return	index ;
}


//=====================================================================================
// Function Name : CEventTimer::HookField()                                            
// Description   : �^�C�}�[�Ǘ�����o�^����.                                         
// Parameter     : *self		"�Ώۃ^�C�}�[�Ǘ����̃A�h���X"                       
//               : index		"�o�^�Ώۏ��i�[��C���f�b�N�X"                       
// Return        : -                                                                   
//=====================================================================================
static void CEventTimer_HookField(CEventTimer *self, T_SZ index)
{
	TimerField	*field ;
	T_SZ  crnt, prev, limit ;
	T_SZ  pos = MSP_TMR_TERM ;

// ----- �^�C�}�[���̐ݒ� -----
	field = self->mField ;																	// �^�C�}�[�Ǘ����̈�̐擪�A�h���X���Z�b�g����

// ----- �^�C�}�[���̓o�^ -----
	if ( (self->mTop) == MSP_TMR_TERM ) {													// < �N�����̃^�C�}�[���Ȃ� >
		self->mTop = index ;
	}
	else {																					// < �N�����̃^�C�}�[������ >
		crnt = self->mTop ;																	// �擪�̃^�C�}�[����`�F�b�N�����{����
		prev = MSP_TMR_TERM ;

	// ----- �^�C�}�[�ڑ���̌��� -----
		for ( limit = 0 ; limit < MSP_MAX_TMR_NUM ; limit++ ) {								// [[[ �J��Ԃ� ]]] �F�ő�񐔂͗̈�m�ې�
			if ( field[crnt].mDelay > field[index].mDelay ) {								// < �^�C�}�[�ڑ���𔭌����� >
				field[crnt].mDelay -= field[index].mDelay ;									// �x�����Ԃ̍������Z�o����i�ڑ��ς݃^�C�}�[�̒x�����Ԃ�␳�j
				break ;																		// �������f�I�I�I
			}

			field[index].mDelay -= field[crnt].mDelay ;										// �x�����Ԃ̍������Z�o����i�ڑ��Ώۂ̃^�C�}�[�l��␳����j

			prev = crnt ;																	// �`�F�b�N�ΏۃC���f�b�N�X��O�i�C���f�b�N�X�ɃZ�b�g����
			crnt = field[crnt].mNext ;														// �`�F�b�N�ΏۃC���f�b�N�X�Ɍ�i�C���f�b�N�X���Z�b�g����

			if ( crnt == MSP_TMR_TERM ) {													// �����N�̍Ō���܂Ō��������{����
				break ;																		// �������f�I�I�I
			}
		}																					// [[[ �J��Ԃ� ]]] �F�����܂�

	// ----- �����N�̍X�V -----
		if ( limit < MSP_MAX_TMR_NUM ) {													// �G���[�������I�I�I�i�G���[�F�ő�J�E���g���̌����ɂĔ����ł����j
			if ( crnt == MSP_TMR_TERM ) {													// < �Ō���Ƀ^�C�}�[���Z�b�g���� >
				field[prev].mNext = index ;
				field[index].mPrev = prev ;
			}
			else if ( crnt == (self->mTop) ) {												// < �擪�Ƀ^�C�}�[���Z�b�g���� >
				field[crnt].mPrev = index ;
				field[index].mNext = crnt ;
				self->mTop = index ;
			}
			else {																			// < �r���Ƀ^�C�}�[���Z�b�g���� >
				field[prev].mNext = index ;
				field[index].mPrev = prev ;
				field[crnt].mPrev = index ;
				field[index].mNext = crnt ;
			}
		}
	}
}


//=====================================================================================
// Function Name : CEventTimer::RemoveField()                                          
// Description   : �^�C�}�[�Ǘ����𖕏�����.                                         
// Parameter     : *self		"�Ώۃ^�C�}�[�Ǘ����̃A�h���X"                       
//               : index		"�����Ώۏ��i�[��C���f�b�N�X"                       
// Return        : -                                                                   
//=====================================================================================
static void CEventTimer_RemoveField(CEventTimer *self, T_SZ index)
{
	TimerField	*field ;
	T_SZ  prev, next ;
	US	delay ;

// ----- �^�C�}�[���̐ݒ� -----
	field = self->mField ;																	// �^�C�}�[�Ǘ����̈�̐擪�A�h���X���Z�b�g����
	prev  = field[index].mPrev ;															// �����Ώۃ^�C�}�[�Ǘ������擾����
	next  = field[index].mNext ;
	delay = field[index].mDelay ;

// ----- �����N�̍X�V -----
	if ( (self->mTop) == index ) {															// < �����Ώۂ��擪�ɂȂ���Ă��� >
		self->mTop = field[index].mNext ;
	}

	if ( prev != MSP_TMR_TERM ) {															// < �����Ώۂ̑O�ɂȂ��ꂽ�^�C�}�[������ >
		field[prev].mNext = next ;
	}

	if ( next != MSP_TMR_TERM ) {															// < �����Ώۂ̌�ɂȂ��ꂽ�^�C�}�[������ >
		field[next].mPrev = prev ;
		field[next].mDelay += delay ;
	}
}


//=====================================================================================
// Function Name : CEventTimer::GetType()                                              
// Description   : �R�[���o�b�N��^�C�}�[�̃^�C�}�[�D��x�iHI/LO�j���擾����.           
// Parameter     : *self		"�Ώۃ^�C�}�[�Ǘ����̃A�h���X"                       
//               : id			"�`�F�b�N�Ώۃ^�C�}�[ID"                               
// Return        : �^�C�}�[�D��x�iHI/LO�j�i0x01:��D��x / 0x11:���D��x�j            
//=====================================================================================
static UC CEventTimer_GetType(CEventTimer *self, T_SZ id)
{
	return	((self->mField[id].mStatus) & 0x11) ;
}


//=====================================================================================
// Function Name : CEventTimer::ExecuteHandler()                                       
// Description   : �R�[���o�b�N�n���h���[�����s����.                                   
// Parameter     : *self		"�Ώۃ^�C�}�[�Ǘ����̃A�h���X"                       
//               : index		"���s�Ώۏ��i�[��C���f�b�N�X"                       
// Return        : -                                                                   
//=====================================================================================
static void CEventTimer_ExecuteHandler(CEventTimer *self, T_SZ index)
{
	TimerField	*field ;
	UC	key, type ;

	field = &(self->mField[index]) ;														// �^�C�}�[�Ǘ����̈�̐擪�A�h���X���Z�b�g����
	type  = CEventTimer_GetType(self, index) ;												// �^�C�}�[�D��x���擾����

	key = LIB_GetKey() ; LIB_Lock() ;														// ����!!! ���荞�݋֎~ ��������!!!
	if ( CEventTimer_IsRunning(self, index) == MSP_TMR_RUN ) {								// < �C�x���g�^�C�}�[���N�����ł��� >
		field->mStatus |= MSP_TMR_EXEC ;													// �R�[���o�b�N�������s��Ԃɂ���
	}																						// �� ���D��x��������̃L�����Z�����ł��Ȃ�����I
	LIB_Unlock(key) ;																		// ����!!! ���荞�݋֎~ �����܂�!!!

	if ( (field->mInfo.mpHandler) != MSP_HDLR_VOID ) {										// < �n���h���[�̃|�C���^�[���L���ł��� >
		if ( ( (field->mZero) == SET ) && ( type == MSP_TMR_HIGH ) ) {						// < ���D��x�^�C�}�[�g�p���x���ݒ肪0[ms]�ł����� >
			(field->mInfo.mpHandler)((SS)MSP_TMR_TERM, field->mInfo.mParam) ;				// �R�[���o�b�N�n���h���[�����s����iID��"MSP_TMR_TERM"�j
		}
		else {																				// < ��D��x�^�C�}�[�g�p�������͒x������̐ݒ�ł����� >
			(field->mInfo.mpHandler)((SS)index, field->mInfo.mParam) ;						// �R�[���o�b�N�n���h���[�����s����i�ʏ�^�C���A�E�g�j
		}
	}
}


//
// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+----------------------------------------------------------------------------------------------------------------------------
// 14.02.20 | Mitsuyuki Nishizawa | MSP Frame > Event Timer�̃v���g�^�C�v�Ƃ��Ė{�t�@�C�����쐬. (Rev.000)
// 14.09.08 | Mitsuyuki Nishizawa | �R�[���o�b�N���{���O�ł̍��D��x����������̃^�C�}�[�L�����Z�����l��. (Rev.001)
// 14.09.24 | Mitsuyuki Nishizawa | �ݒ�x������0[ms]���̃R�[���o�b�N�n���h���[��ID��"MSP_TMR_TERM"�Ŏ��s. (Rev.002)
//
//


// *************************************************************  [ MSP_EventTimer.c : EOF ]  *****************************************************************

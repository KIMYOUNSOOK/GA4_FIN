// ������ Module Software Platform Frame ������
/// @file    MSP_EventTimer.h
/// @author  Mitsuyuki Nishizawa
/// @date    14.11.05
/// @version 003
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef MSP_EventTimer_h
#define MSP_EventTimer_h


// ******************************************************  <<<<< Definition & Declaration Area >>>>>  *********************************************************

// ===========================  <<< �}�N����` >>>  ===========================

// ----- �^�C�}�[�Ǘ����̈�̊m�ې� -----													// ���������[�U�[�ݒ聙����
#define	MSP_MAX_TMR_NUM		255																// �ő�^�C�}�[�{����ݒ�i�ő�F512 �c 255�ȉ��𐄏��j

// ----- �^�[�~�l�[�^�[��` -----
#if MSP_MAX_TMR_NUM > 255
	#define	MSP_TMR_TERM	0xFFFF															// �^�C�}�[���X�g�̃^�[�~�l�[�^�[�i2�o�C�g�j
#else
	#define	MSP_TMR_TERM	0xFF															// �^�C�}�[���X�g�̃^�[�~�l�[�^�[�i1�o�C�g�j
#endif

// ----- �^�C�}�[�^�C�v (�D��x) -----														// ���Ӂj�^�C�}�[�X�e�[�^�X��� > bit0,1: �X�e�[�^�X, bit4: �D��x
#define	MSP_TMR_LOW			0x01															// ��D��x�^�C�}�[
#define	MSP_TMR_HIGH		0x11															// ���D��x�^�C�}�[

// ----- �^�C�}�[�X�e�[�^�X��` -----
#define	MSP_TMR_STOP		0x00															// �^�C�}�[���g�p���
#define	MSP_TMR_RUN			0x01															// �^�C�}�[�N�����
#define	MSP_TMR_EXEC		0x03															// �^�C�}�[��R�[���o�b�N�������s���

// ----- �����n���h���[��` -----
#define	MSP_HDLR_VOID		0																// �����n���h���[

// ===========================  <<< �f�[�^�^��` >>>  ===========================

// ----- �^�C�}�[���Ǘ��p�C���f�b�N�X -----
#if MSP_MAX_TMR_NUM > 255
	typedef unsigned short	T_SZ ;															// �^�C�}�[�C���f�b�N�X�̌^�i2�o�C�g�j
#else
	typedef unsigned char	T_SZ ;															// �^�C�}�[�C���f�b�N�X�̌^�i1�o�C�g�j
#endif

// ----- �R�[���o�b�N�E�n���h���[��� -----
typedef struct {
	void (*mpHandler)(SS, UL) ;																// �n���h���[�̃A�h���X
	UL	mParam ;																			// �R�[���o�b�N���̃p�����[�^
} Handler ;


// ----- �^�C�}�[�Ǘ���� -----
typedef struct {
	Handler  mInfo ;																		// <<< �R�[���o�b�N��� >>>
	T_SZ  mPrev ;																			// �O�ɂȂ��ꂽ�^�C�}�[�Ǘ����ID
	T_SZ  mNext ;																			// ��ɂȂ��ꂽ�^�C�}�[�Ǘ����ID
	UC	  mStatus ;																			// �^�C�}�[�g�p��
	UC	  mZero ;																			// �ݒ�x������0[ms]�t���O
	US	  mDelay ;																			// �x�����ԁi�O�̃^�C�}�[�Ƃ̍����j
} TimerField ;


// ********************************************************  <<<<< Class Declaration Area >>>>>  **************************************************************

// ===========================  <<< �N���X��` >>>  ===========================

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Class Name    : CEventTimer
// Description   : �^�C�}�[�Ǘ�.
// Note          : �Ȃ�
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
typedef struct EventTimer {
/*public:*/
	//=[[[ Constructor ]]]=================================================================
	// Function Name : CEventTimer::CEventTimer()                                          
	// Description   : ���̃N���X�̃R���X�g���N�^.                                         
	// Parameter     : *self		"�Ώۃ^�C�}�[�Ǘ����̃A�h���X"                       
	// Return        : -                                                                   
	//=====================================================================================
	// void CEventTimer(CEventTimer *self) {}

	//=====================================================================================
	// Function Name : CEventTimer::Set()                                                  
	// Description   : �R�[���o�b�N��^�C�}�[���Z�b�g����.                                  
	// Parameter     : *self		"�Ώۃ^�C�}�[�Ǘ����̃A�h���X"                       
	//               : type			"�^�C�}�[�D��x�iHI/LO�j"                              
	//               : delay		"�^�C�}�[�x������[ms]"                                 
	//               : *handler		"�R�[���o�b�N�n���h���[���i�[��A�h���X"             
	// Return        : �^�C�}�[ID�i�}�C�i�X�̓G���[�j                                      
	//=====================================================================================
	// SS Set(CEventTimer *self, UC type, US delay, Handler *handler)  {}

	//=====================================================================================
	// Function Name : CEventTimer::Cancel()                                               
	// Description   : �R�[���o�b�N��^�C�}�[���L�����Z������.                              
	// Parameter     : *self		"�Ώۃ^�C�}�[�Ǘ����̃A�h���X"                       
	//               : id			"�L�����Z���Ώۃ^�C�}�[ID"                             
	// Return        : �������ʁi0:�������� / -1:�������s�j                                
	//=====================================================================================
	// SS Cancel(CEventTimer *self, SS id) {}

	//=====================================================================================
	// Function Name : CEventTimer::IsRunning()                                            
	// Description   : �R�[���o�b�N��^�C�}�[�̓����Ԃ��擾����.                          
	// Parameter     : *self		"�Ώۃ^�C�}�[�Ǘ����̃A�h���X"                       
	//               : id			"�`�F�b�N�Ώۃ^�C�}�[ID"                               
	// Return        : �^�C�}�[�����ԁi0:��~�� / 1:���쒆�j                             
	//=====================================================================================
	// UC IsRunning(CEventTimer *self, SS id) {}

	//=====================================================================================
	// Function Name : CEventTimer::Tick()                                                 
	// Description   : �J�E���g�_�E�����������s����.                                       
	// Parameter     : *self		"�Ώۃ^�C�}�[�Ǘ����̃A�h���X"                       
	// Return        : -                                                                   
	//=====================================================================================
	// void Tick(CEventTimer *self) {}

	//=====================================================================================
	// Function Name : CEventTimer::Timeout()                                              
	// Description   : �R�[���o�b�N���������s����. (��D��x�^�C�}�[��p�֐�)              
	// Parameter     : *self		"�Ώۃ^�C�}�[�Ǘ����̃A�h���X"                       
	//               : id			"�`�F�b�N�Ώۃ^�C�}�[ID"                               
	// Return        : -                                                                   
	//=====================================================================================
	// void Timeout(CEventTimer *self, SS id) {}

/*private:*/
	//=====================================================================================
	// Function Name : CEventTimer::Fire()                                                 
	// Description   : �^�C���A�E�g���������s����.                                         
	// Parameter     : *self		"�Ώۃ^�C�}�[�Ǘ����̃A�h���X"                       
	//               : id			"�`�F�b�N�Ώۃ^�C�}�[ID"                               
	// Return        : -                                                                   
	//=====================================================================================
	// void Fire(CEventTimer *self, SS id) {}

	//=====================================================================================
	// Function Name : CEventTimer::ResetField()                                           
	// Description   : �^�C�}�[�Ǘ���������������.                                       
	// Parameter     : *self		"�Ώۃ^�C�}�[�Ǘ����̃A�h���X"                       
	//               : index		"�������Ώۏ��i�[��C���f�b�N�X"                     
	/// Return        : -                                                                  
	//=====================================================================================
	// void ResetField(CEventTimer *self, T_SZ index) {}

	//=====================================================================================
	// Function Name : CEventTimer::SetField()                                             
	// Description   : �^�C�}�[�Ǘ������Z�b�g����.                                       
	// Parameter     : *self		"�Ώۃ^�C�}�[�Ǘ����̃A�h���X"                       
	//               : index		"�������Ώۏ��i�[��C���f�b�N�X"                     
	//               : type			"�^�C�}�[�D��x�iHI/LO�j"                              
	//               : delay		"�^�C�}�[�x������[ms]"                                 
	//               : *handler		"�R�[���o�b�N�n���h���[���i�[��A�h���X"             
	// Return        : -                                                                   
	//=====================================================================================
	// void SetField(CEventTimer *self, T_SZ index, UC type, US delay, Handler *handler) {}

	//=====================================================================================
	// Function Name : CEventTimer::SearchField()                                          
	// Description   : �^�C�}�[�Ǘ����̋󂫗̈����������.                               
	// Parameter     : *self		"�Ώۃ^�C�}�[�Ǘ����̃A�h���X"                       
	// Return        : �^�C�}�[�Ǘ����̋󂫗̈�������C���f�b�N�X�i-1:�󂫗̈�Ȃ�)      
	//=====================================================================================
	// T_SZ SearchField(CEventTimer *self) {}

	//=====================================================================================
	// Function Name : CEventTimer::HookField()                                            
	// Description   : �^�C�}�[�Ǘ�����o�^����.                                         
	// Parameter     : *self		"�Ώۃ^�C�}�[�Ǘ����̃A�h���X"                       
	//               : index		"�o�^�Ώۏ��i�[��C���f�b�N�X"                       
	// Return        : -                                                                   
	//=====================================================================================
	// void HookField(CEventTimer *self, T_SZ index) {}

	//=====================================================================================
	// Function Name : CEventTimer::RemoveField()                                          
	// Description   : �^�C�}�[�Ǘ����𖕏�����.                                         
	// Parameter     : *self		"�Ώۃ^�C�}�[�Ǘ����̃A�h���X"                       
	//               : index		"�����Ώۏ��i�[��C���f�b�N�X"                       
	// Return        : -                                                                   
	//=====================================================================================
	// void RemoveField(CEventTimer *self, T_SZ index) {}

	//=====================================================================================
	// Function Name : CEventTimer::GetType()                                              
	// Description   : �R�[���o�b�N��^�C�}�[�̃^�C�}�[�D��x�iHI/LO�j���擾����.           
	// Parameter     : *self		"�Ώۃ^�C�}�[�Ǘ����̃A�h���X"                       
	//               : id			"�`�F�b�N�Ώۃ^�C�}�[ID"                               
	// Return        : �^�C�}�[�D��x�iHI/LO�j�i0x01:��D��x / 0x11:���D��x�j            
	//=====================================================================================
	// UC CEventTimer_GetType(CEventTimer *self, T_SZ id) {}

	//=====================================================================================
	// Function Name : CEventTimer::ExecuteHandler()                                       
	// Description   : �R�[���o�b�N�n���h���[�����s����.                                   
	// Parameter     : *self		"�Ώۃ^�C�}�[�Ǘ����̃A�h���X"                       
	//               : index		"�����Ώۏ��i�[��C���f�b�N�X"                       
	// Return        : -                                                                   
	//=====================================================================================
	// void ExecuteHandler(CEventTimer *self, T_SZ index) {}

	TimerField	mField[MSP_MAX_TMR_NUM] ;														// �^�C�}�[�Ǘ����{��
	T_SZ  mTop ;																				// �擪�ɂȂ��ꂽ�^�C�}�[�Ǘ����ID
	T_SZ  mIndex ;																				// �^�C�}�[�Ǘ����̈�擾�̂��߂̃C���f�b�N�X

	T_SZ  mCount ;																				// ���݂̃^�C�}�[�g�p�{��	[[[ �f�o�b�O�p ]]]
	T_SZ  mMax ;																				// �^�C�}�[�g�p�ő�{��		[[[ �f�o�b�O�p ]]]
} CEventTimer ;

// ===========================  <<< �֐��v���g�^�C�v�錾 >>>  ===========================
void CEventTimer_CEventTimer(CEventTimer*) ;													// �^�C�}�[�Ǘ������������i�R���X�g���N�^�j
SS CEventTimer_Set(CEventTimer*, UC, US, Handler*) ;											// �^�C�}�[���Z�b�g����
SS CEventTimer_Cancel(CEventTimer*, SS) ;														// �^�C�}�[���L�����Z������
UC CEventTimer_IsRunning(CEventTimer*, SS) ;													// �^�C�}�[��Ԃ��擾����
void CEventTimer_Tick(CEventTimer*) ;															// �^�C�}�[�J�E���g����
void CEventTimer_Timeout(CEventTimer*, SS) ;													// �^�C�}�[�^�C���A�E�g�n���h���[�����s����


#endif			// MSP_EventTimer_h


//
// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+----------------------------------------------------------------------------------------------------------------------------
// 14.03.20 | Mitsuyuki Nishizawa | MSP Frame > Event Timer�̃v���g�^�C�v�Ƃ��Ė{�t�@�C�����쐬. (Rev.000)
// 14.09.08 | Mitsuyuki Nishizawa | �R�[���o�b�N���{���O�ł̍��D��x����������̃^�C�}�[�L�����Z�����l��. (Rev.001)
// 14.09.24 | Mitsuyuki Nishizawa | �ݒ�x������0[ms]���f�p�t���O�̈�������o�[�ɒǉ�. (Rev.002)
// 14.11.05 | Mitsuyuki Nishizawa | �^�C�}�[�������ʂ����ʃw�b�_�[�ֈړ�. (Rev.003)
//
//


// *************************************************************  [ MSP_EventTimer.h : EOF ]  *****************************************************************

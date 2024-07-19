// ������ Module Software Platform Library ������
/// @file    LIB_MessageBuffer.h
/// @author  Tsuyoshi Seto
/// @date    14.02.20
/// @version 000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef LIB_MessageBuffer_h
#define LIB_MessageBuffer_h


// ******************************************************  <<<<< Definition & Declaration Area >>>>>  *********************************************************

// ===========================  <<< �}�N����` >>>  ===========================

// ----- ���b�Z�[�W�o�b�t�@�̃T�C�Y(�o�C�g��) -----											// �����ӁF�T�C�Y��`�̓��b�Z�[�W�o�b�t�@ID�����K�v�ƂȂ�I�I�I
#define	LIB_MBF_FRM_SIZE		0x0080														// ���b�Z�[�W�o�b�t�@�FMSP Frame�p�̃o�b�t�@�T�C�Y
#define	LIB_MBF_DD_HIGH_SIZE	0x0040														// ���b�Z�[�W�o�b�t�@�FDevice Driver���D��x�p�̃o�b�t�@�T�C�Y
#define	LIB_MBF_DD_LOW_SIZE		0x0040														// ���b�Z�[�W�o�b�t�@�FDevice Driver��D��x�p�̃o�b�t�@�T�C�Y

// ----- �o�b�t�@�������� -----
#define	LIB_MBF_OK			0
#define	LIB_MBF_ERR			(-1)

// ===========================  <<< �񋓌^��` >>>  ===========================

// ----- ���b�Z�[�W�o�b�t�@���ʎq -----
typedef enum {
	eMbfFrame ,																				//  0: MSP Frame�p���b�Z�[�W�o�b�t�@
	eMbfDdLow ,																				//  1: Device Driver�p��D��x���b�Z�[�W�o�b�t�@
	eMbfDdHigh,																				//  2: Device Driver�p���D��x���b�Z�[�W�o�b�t�@
	eNumOfMbf 																				// XX: ���b�Z�[�W�o�b�t�@�̐��i�������Ɏ��ʎq���`����j
} EMbfID ;

// ===========================  <<< �f�[�^�^��` >>>  ===========================

// ----- ���b�Z�[�W�L���[ -----
typedef struct {
	SS	offset ;																			// �f�[�^�o�b�t�@�ւ̏������݈ʒu
	SS	size ;																				// �f�[�^�o�b�t�@�ւ̏������݃T�C�Y
} MessageQ ;

// ----- ���b�Z�[�W�o�b�t�@�R���t�B�M�����[�V���� -----
typedef struct {
	MessageQ	*pMsgQ ;																	// ���b�Z�[�W�L���[�A�h���X
	US			qSize ;																		// ���b�Z�[�W�L���[�T�C�Y
	CDataBuffer	*pBuff ;																	// �f�[�^�o�b�t�@�A�h���X
} MbfConfig ;


// ********************************************************  <<<<< Class Declaration Area >>>>>  **************************************************************

// ===========================  <<< �N���X��` >>>  ===========================

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Class Name    : CMessageBuffer
// Description   : ���b�Z�[�W�o�b�t�@
// Note          : �Ȃ�
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
typedef struct MessageBuffer {
/*public:*/
	//=[[[ Constructor ]]]=================================================================
	// Function Name : CMessageBuffer::CMessageBuffer()                                    
	// Description   : ���̃N���X�̃R���X�g���N�^.                                         
	// Parameter     : *self		"�Ώۃ��b�Z�[�W�o�b�t�@�̃A�h���X"                     
	//               : *config		"���b�Z�[�W�o�b�t�@�R���t�B�M�����[�V�����̃A�h���X"   
	// Return        : -                                                                   
	//=====================================================================================
	// void CMessageBuffer(CMessageBuffer *self, const MbfConfig *config) {}

	//=====================================================================================
	// Function Name : CMessageBuffer::Send()                                              
	// Description   : ���b�Z�[�W�o�b�t�@�Ƀ��b�Z�[�W�𑗐M����.                           
	// Parameter     : *self		"�Ώۃ��b�Z�[�W�o�b�t�@�̃A�h���X"                     
	//               : *message		"�ʒm�Ώۃ��b�Z�[�W�̐擪�A�h���X"                     
	//               : size			"�ʒm�Ώۃ��b�Z�[�W�̃T�C�Y"                           
	// Return        : ���M�������ʁi0�FOK / -1�FNG)                                       
	//=====================================================================================
	// SS Send(CMessageBuffer *self, UC *message, SS size) {}
	SS (*Send)(struct MessageBuffer*, UC*, SS) ;

	//=====================================================================================
	// Function Name : CMessageBuffer::Receive()                                           
	// Description   : ���b�Z�[�W�o�b�t�@���烁�b�Z�[�W����M����.                         
	// Parameter     : *self		"�Ώۃ��b�Z�[�W�o�b�t�@�̃A�h���X"                     
	//               : **message	"��M���b�Z�[�W�i�[�G���A�擪�A�h���X�������݃A�h���X" 
	// Return        : ��M���b�Z�[�W�T�C�Y�i�I�t�Z�b�g�o�C�g���j�c 0�̓��b�Z�[�W�Ȃ�      
	//=====================================================================================
	// SS Receive(CMessageBuffer *self, UC **message) {}
	SS (*Receive)(struct MessageBuffer*, UC**) ;

/*private:*/
	CDataBuffer	*mpBuffer ;																		// �f�[�^�o�b�t�@�ւ̃|�C���^�[
	MessageQ	*mpMsgQ ;																		// ���b�Z�[�W�Ǘ��L���[�̈�ւ̃|�C���^�[

	US	mSize ;																					// ���b�Z�[�W�Ǘ��L���[�̃T�C�Y�i�L���[�ł��郁�b�Z�[�W���j
	US	mRead ;																					// �ǂݏo���C���f�b�N�X
	US	mWrite ;																				// �������݃C���f�b�N�X
	SC	mStatus ;																				// ���b�Z�[�W�o�b�t�@�̏��
} CMessageBuffer ;


// ===========================  <<< �֐��v���g�^�C�v�錾 >>>  ===========================
void CMessageBuffer_CMessageBuffer(CMessageBuffer*, const MbfConfig*) ;							// ���b�Z�[�W�o�b�t�@����������


#endif			// LIB_MessageBuffer_h


//
// Change History
// Date:    | Engineer:          | Note:
// ---------+--------------------+-----------------------------------------------------------------------------------------------------------------------------
// 14.02.20 | Tsuyoshi Seto      | MSP Library > Message Buffer�̃v���g�^�C�v�Ƃ��Ė{�t�@�C�����쐬. (Rev.000)
//
//


// ************************************************************  [ LIB_MessageBuffer.h : EOF ]  ***************************************************************

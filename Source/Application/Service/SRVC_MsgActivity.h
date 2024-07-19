// File Name: SRVC_MsgActivity.h
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 14.05.14
// Revision:  000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// �͂��߂�
//
// �{�N���X�́A�V�X�e�����i��R���p�C�����i�Ȃǂ����A�e�f�o�C�X�ւ̓���w��Activity��
// �������邽�߂̋��ʂ̎d�g�݂ł���B
// ����w��Activity�́A�����̃��b�Z�[�W���MAction����\������Ă���A
// �eAction�́A�J�n�g���K��������ɁA�w�胁�b�Z�[�W�𑗐M(Start)���A
// �w�胁�b�Z�[�W����M������J�n�g���K�̃t���O���X�V(SetTrigger)���A
// ����Action���J�n(Start)������d�g�݂����B

// �ڍׂ͉��L�g�p�Ⴈ���SRVC_MsgActivity.cpp�̊e���\�b�h�̘_�����Q�Ƃ̂��ƁB


#ifndef SRVC_MSG_ACTIVITY_H
#define SRVC_MSG_ACTIVITY_H

#define SRVC_ID2BIT(idx)			(0x00000001 << (idx))		// �C���f�b�N�X(0�`15)����r�b�g�p�^�[��(0x0001�`0x8000)�ւ̕ϊ�
#define SRVC_END_BIT_PATTERN(num)	((0x00000001 << num) - 1)	// �Ⴆ��5�̏ꍇ��20h - 1 ��  1Fh


typedef struct /*SrvcMsgActivity*/ {
	UL startTrgPattern;										// Message�𑗐M���邽�߂̃e�[�u��Index��Bit�p�^�[��(�J�n�g���K�̐�������)
															// �Z���T�̕ω��҂�Action�̏ꍇ�ɂ�"SRVC_SNR_WAIT(0xFFFF)"�Ƃ���
	US* pWaitTime;											// �J�n�g���K���������ォ��w�胁�b�Z�[�W�𑗐M����܂ł̑҂����Ԃ̃|�C���^
	US sendEventID;											// Action���{���ɑ��M����Event(Message)ID
	US recvEventID;											// Action(��Activity)�̏I�������ƂȂ�Event(Message or Sensor)ID
	UC senderID;											// ��M����EventID�̑��M��ID
} SrvcMsgActivity;

/*class CSrvcMsgActivity {
public:

	CSrvcMsgActivity(UC userMID, UC tableSize, US endPattern, SrvcMsgActivity* pTable)
		: mUserMID(userMID), mTableSize(tableSize), mCrntPattern(0), mEndPattern(endPattern), mpTable(pTable){}

	void Start(void);										// Activity���J�n������
	void Progress(void);									// ���g���K�p�^�[���ő��M�ł���C�x���g��S�đ��M
	void SetTrigger(US recvEventID, UC senderID);			// ��M�C�x���g�ɉ��������g���K�p�^�[���̍X�V
	void TimeOut(UL idx);									// �҂����Ԍo�ߌ�̃C�x���g���M
	bool CheckEnd(void);									// ���g���K�p�^�[���ƏI���g���K�p�^�[�����r����Activity���I���������ǂ������f
	void Terminate();										// Acitivity���I��������
	void SetData(UC data);
	UL GetCurrentBitPattern() { return mCrntPattern; }
	
private:
	UC mUserMID;
	UC mTableSize;
	UL mCrntPattern;										// ���݂�bitPattern(1�ɂȂ��Ă���bit���I��Action)
	UL mStartPattern;										// ���݂̊J�n�ς�bitPattern(1�ɂȂ��Ă���bit���J�n�ς�Action)
	UL mEndPattern;											// �I��������bitPattern
	bool mIsActive;
	UC mMsgData;
	SrvcMsgActivity* mpTable;
};*/

typedef struct /*CSrvcMsgActivity*/ {
	UC mUserMID;
	UC mTableSize;
	UL mCrntPattern;										// ���݂�bitPattern(1�ɂȂ��Ă���bit���I��Action)
	UL mStartPattern;										// ���݂̊J�n�ς�bitPattern(1�ɂȂ��Ă���bit���J�n�ς�Action)
//	UL mEndPattern;											// �I��������bitPattern
	UC mIsActive;
	UC* mpMsgData;
	UC mMsgDataSize;
	SrvcMsgActivity* mpTable;
	void (*mpTimerCallBack)(SS timerID, UL idx);			// �^�C�}�[�R�[���o�b�N�֐��̃|�C���^
} CSrvcMsgActivity;

void CSrvcMsgActivity_Constructor(CSrvcMsgActivity* pThis, UC userMID, UC tableSize, SrvcMsgActivity* pTable, void (*pCallBack)(SS, UL));

void CSrvcMsgActivity_Start(CSrvcMsgActivity* pThis);		// Activity���J�n������
void CSrvcMsgActivity_Progress(CSrvcMsgActivity* pThis);	// ���g���K�p�^�[���ő��M�ł���C�x���g��S�đ��M
void CSrvcMsgActivity_SetTrigger(CSrvcMsgActivity* pThis, US recvEventID, UC senderID);
															// ��M�C�x���g�ɉ��������g���K�p�^�[���̍X�V
void CSrvcMsgActivity_TimeOut(CSrvcMsgActivity* pThis, UL idx);
															// �҂����Ԍo�ߌ�̃C�x���g���M
bool CSrvcMsgActivity_CheckEnd(CSrvcMsgActivity* pThis);	// ���g���K�p�^�[���ƏI���g���K�p�^�[�����r����Activity���I���������ǂ������f
void CSrvcMsgActivity_Abort(CSrvcMsgActivity* pThis);		// Acitivity�𒆎~������
void CSrvcMsgActivity_Terminate(CSrvcMsgActivity* pThis);	// Acitivity���I��������
void CSrvcMsgActivity_SetData(CSrvcMsgActivity* pThis, UC* pData, UC size);
#define CSrvcMsgActivity_GetCurrentBitPattern(apThis) ((apThis)->mCrntPattern)
															// UL (CSrvcMsgActivity*)

/* 
������ �g�p�� Power On Initialize��Activity�̏ꍇ ������


	            	   (Start)                          
		                 ��                             
		             CMPL(IDX=0)                        
		                 ��                             
		===================================             
		 ��              ��                             
		BKLT(IDX=1)      ��     XXX_SNR ON(IDX=3)       
		 ��          STK(IDX=2)     ��                  
		PUNCH(IDX=5)     ��     FLD(IDX=4)              
		 ��              ��         ��                  
		===================================             
		                 ��                             
		             TSQF(IDX=6)                        
		                 ��                             
		               (End)                            



//�� �e�[�u���̎�����
// �҂����ԃe�[�u��ID�ƃe�[�u��
enum ESeq_POnWaitTimeID {
	SEQ_P_ON_WAIT_CMPL,
	SEQ_P_ON_WAIT_STK,
	SEQ_P_ON_WAI_PUNCH
};

US gSeq_POnWaitTimTbl[] = {100, 20, 10};


// Activity�e�[�u��
const SrvcMsgActivity gSeq_POnShtInitActTable[] = {
	//startTrgPattern		 pWaitTime									sendActReqEventID					recvEventIDforEndAction				senderID                    
	{ 0, 					 &gSeq_POnWaitTimTbl[SEQ_P_ON_WAIT_CMPL],	ACT_REQ_POWER_ON_INIT_COMPILER, 	ACT_RES_POWER_ON_INIT_COMPILER,		MID_COMPILER		},	// IDX = 0 �� �������ŊJ�n�̏ꍇ��"0"
	{ SRVC_ID2BIT(0),		 0,											ACT_REQ_POWER_ON_INIT_BOOKLET, 		ACT_RES_POWER_ON_INIT_BOOKLET,		MID_BOOKLET			},	// IDX = 1 �� IDX=0�̏I�����g���K�Ȃ̂�"SRVC_ID2BIT(0)"
	{ SRVC_ID2BIT(0),		 &gSeq_POnWaitTimTbl[SEQ_P_ON_WAIT_STK],	ACT_REQ_POWER_ON_INIT_STACKER, 		ACT_RES_POWER_ON_INIT_STACKER,		MID_STACKER_TRAY	},	// IDX = 2 �� IDX=0�̏I�����g���K�Ȃ̂�"SRVC_ID2BIT(0)"
	{ 0,					 0,											MSP_EVT_NUM_VOID,					LOC_IOT_EXIT_SENSOR_ON,				MID_FRAME			},  // IDX = 3 �� XXX�Z���T��ON�G�b�W��҂�����Action (Device Event�ł����l)
																																										//            �J�n�����͖������Ȃ̂�"0"
	{ SRVC_ID2BIT(3),		 0,											ACT_REQ_POWER_ON_INIT_FOLDER, 		ACT_RES_POWER_ON_INIT_FOLDER,		MID_FOLDER			},	// IDX = 4 �� IDX=3�̏I���i���Z���T�̕ω��j���g���K�Ȃ̂�"SRVC_ID2BIT(3)"
	{ SRVC_ID2BIT(1),		 &gSeq_POnWaitTimTbl[SEQ_P_ON_WAI_PUNCH],	ACT_REQ_POWER_ON_INIT_PUNCH, 		ACT_RES_POWER_ON_INIT_PUNCH,		MID_PUNCH			},	// IDX = 5 �� IDX=1�̏I�����g���K�Ȃ̂�"SRVC_ID2BIT(1)"
	{ SRVC_ID2BIT(2)						
		|SRVC_ID2BIT(4)	 
		|SRVC_ID2BIT(5),	 0,											ACT_REQ_POWER_ON_INIT_TSQF,			ACT_RES_POWER_ON_INIT_TSQF,			MID_TSQF			}	// IDX = 6 �� IDX=2,4,5�̏I�����g���K�Ȃ̂�"SRVC_ID2BIT(2)|SRVC_ID2BIT(4)|SRVC_ID2BIT(5)"
};

// Activity�N���X�̃C���X�^���X����
CSrvcMsgActivity gSeq_POnShtInitActivityTest(MID_SYSTEM, 
	                                        sizeof(gSeq_POnShtInitActTable)/sizeof(gSeq_POnShtInitActTable[0]), 
											SRVC_END_BIT_PATTERN(sizeof(gSeq_POnShtInitActTable)/sizeof(gSeq_POnShtInitActTable[0])), 
											(SrvcMsgActivity*)gSeq_POnShtInitActTable);



//�� �����̎�����
// �� Activity�̊J�n
void ActivityTestStart()
{
	gSeq_POnWaitTimTbl[SEQ_P_ON_WAIT_CMPL]	= 100;
	gSeq_POnWaitTimTbl[SEQ_P_ON_WAIT_STK]	= 10;
	gSeq_POnWaitTimTbl[SEQ_P_ON_WAI_PUNCH]	= 250;

	gSeq_POnShtInitActivityTest.Start();
}

void ActivityTestStop()
{
	gSeq_POnShtInitActivityTest.Terminate();
}


// �� Activity�̃X�e�b�v���s�i���I���j
//   �EAction Response�̎�M���g���K�Ƃ���ꍇ�͉��L�̂悤��
//     �R�[���o�b�N�֐���MSP�̃��b�Z�[�W�C�x���g�̃R�[���o�b�N�֐����X�g�ɓo�^����B
      
void RecvEndActTrgOfPowerOnInitActivity(US trgEvent, UC senderID)
{
	gSeq_POnShtInitActivityTest.SetTrigger(trgEvent, senderID);
	
	if (!(gSeq_POnShtInitActivityTest.CheckEnd())) {
		gSeq_POnShtInitActivityTest.Progress();
	}
	else {
		gSeq_POnShtInitActivityTest.Terminate();
	}
}

// �� MSP�̃C�x���g�Ǘ����X�g�ɓo�^����R�[���o�b�N�֐�
void RecvPowerOnInitCmpl(const UC* pMsg, UC sender) { RecvEndActTrgOfPowerOnInitActivity(ACT_RES_POWER_ON_INIT_COMPILER, sender); }
void RecvPowerOnInitBklt(const UC* pMsg, UC sender) { RecvEndActTrgOfPowerOnInitActivity(ACT_RES_POWER_ON_INIT_BOOKLET, sender); }
void RecvPowerOnInitStk(const UC* pMsg, UC sender)	{ RecvEndActTrgOfPowerOnInitActivity(ACT_RES_POWER_ON_INIT_STACKER, sender); }
void RecvLocIotExitSnrOn(const UC* pMsg, UC sender) { RecvEndActTrgOfPowerOnInitActivity(LOC_IOT_EXIT_SENSOR_ON, sender); }
void RecvPowerOnInitFldr(const UC* pMsg, UC sender) { RecvEndActTrgOfPowerOnInitActivity(ACT_RES_POWER_ON_INIT_FOLDER, sender); }
void RecvPowerOnInitPnch(const UC* pMsg, UC sender) { RecvEndActTrgOfPowerOnInitActivity(ACT_RES_POWER_ON_INIT_PUNCH, sender); }
void RecvPowerOnInitTsqf(const UC* pMsg, UC sender) { RecvEndActTrgOfPowerOnInitActivity(ACT_RES_POWER_ON_INIT_TSQF, sender); }

*/
#endif

// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+-----------------------------------------------------------------------------------------------------------------------------
// 14.05.14 | Nishizawa Mitsuyuki | Create This File.


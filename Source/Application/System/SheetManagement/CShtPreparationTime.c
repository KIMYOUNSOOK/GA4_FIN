// File Name:	CShtPreparationTime.c
// Description:	Implimentation File of Preparation Time
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.20
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CShtPreparationTime.h"

#include "CShtManager.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////
// --- Distance. Table of Dynamic Jam ---
US const cSht_PathDistance[PATH_ID_NUM] = {
	// !!! Product Specific !!!
	1157,		// IOT⟑̗̂p�����o���t���[����IOT Fusing-Roll�ԃp�X��
	243,			//   IOT⟑̗̂p�����o���t���[������ENTRANCE-ROLL�܂ł̋���
	1300,			// ENTRANCE-ROLL����Compile-Exit Sensor�܂ł̋���
	4100,		// ��s����IOT����̗p���������x[mm/s]	
};

/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////
static US CShtPrepTime_GetSheetPitchMargin(UC aSheetNo, UC aFinSpeedID, US aLength);

/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CShtPreparationTime_Constructor
// Description   : �R���X�g���N�^
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CShtPreparationTime_Constructor(CShtPreparationTime* pThis)
{
	pThis->mPrepTime	= 0;
	pThis->mShortenTime = 0;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtPreparationTime_Calculate
// Description   : Preparation Time�v�Z����
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : sheetNo (�ΏۃV�[�g�̃V�[�g���ʎq)
//               : prevShtNo (��s�V�[�g�̃V�[�g���ʎq)
//               : shtInitTimeInfo
// Return        : �]
// �p�������Ԋu[ms] �� �@ �{ �A �{ �B �{ �C
// �@ �F ��{����[ms]
// �A �F ���Z����[ms]
// �B �F �V�[�g�C�j�V�����C�Y���쎞��[ms]
// �C �F �V�[�g�t�B�j�b�V�����N�G�X�g�҂�����[ms]
// ��{����M[ms] ��( L�|(R+E+K)) �� F �~ 1000 �{(R+E+K) �� I �~ 1000�{�}�[�W������
// ���Z���� :  	�R���p�C���t�B�j�b�V���O���쎞��
// 				�^���p�[����������K�v����
// 				�T�u�p�h���p������ ���̓���񐔂�3��̏ꍇ�̉��Z����
//				�R���p�C���t�B�j�b�V���O���쎞��
// R �F IOT⟑̗̂p�����o���t���[����IOT Fusing-Roll�ԃp�X��
// E �F IOT⟑̗̂p�����o���t���[������ENTRANCE-ROLL�܂ł̋���
// K �F ENTRANCE-ROLL����Compile-Exit Sensor�܂ł̋���
// I  �F ��s����IOT����̗p���������x[mm/s] ���֘A�T�u�ԓ��o�͎Q��
// F �F ��s���̃t�B�j�b�V���[�������x[mm/s] �� 600.0�^800.0
///////////////////////////////////////////////////////////////////
void CShtPreparationTime_Calculate(CShtPreparationTime* pThis, UC sheetNo, UC prevShtNo,  ShtInitTimeInfo shtInitTimeInfo)
{
	US			 timer;  
	EShtEject		preSheet;
	
	pThis->mPrepTime = 0;
	pThis->mShortenTime = 0;
	
	preSheet = SRVC_GetEject(prevShtNo);

	 // Sheet��Pitch �����߂�Ƃ�
	if(preSheet  != SHT_EJECT_EXEC) {
		timer = CShtPrepTime_Base_SheetPitch(prevShtNo)
			+ CMPL_GetAdd_TamperOpenNeedTime(prevShtNo)
			+ CMPL_GetAdd_SubPaddleNeedTime(prevShtNo);		//�����n �p���ԃs�b�`�Z�o

	} else { 
		//Set��Pitch �����߂�Ƃ�
		timer = CShtPrepTime_Base_SetPitch(prevShtNo) 
			+ CMPL_GetAdd_CompileFinishingNeedTime(prevShtNo);	   //�����n Set�ԃs�b�`�Z�o
	}	
	
	//V3.1.7 : Job Initialize Time Added .. 
	pThis->mPrepTime = timer + shtInitTimeInfo.sheetInitTime[SHT_INIT_ID_COMPILER];
}


///////////////////////////////////////////////////////////////////
// Function Name : CShtPrepTime_Base_SheetPitch
// Description   : Sheet Pitch �v�Z����
// Parameter     : sheetNo (�ΏۃV�[�g�̃V�[�g���ʎq)
//               	: prevShtNo (��s�V�[�g�̃V�[�g���ʎq)
// Return        :  sheetPitch
// ��{����M[ms] ��( L�|(R+E+K)) �� F �~ 1000 �{(R+E+K) �� I �~ 1000�{�}�[�W������
// R �F IOT⟑̗̂p�����o���t���[����IOT Fusing-Roll�ԃp�X��
// E �F IOT⟑̗̂p�����o���t���[������ENTRANCE-ROLL�܂ł̋���
// K �F ENTRANCE-ROLL����Compile-Exit Sensor�܂ł̋���
// I  �F ��s����IOT����̗p���������x[mm/s] ���֘A�T�u�ԓ��o�͎Q��
// F �F ��s���̃t�B�j�b�V���[�������x[mm/s] �� 600.0�^800.0
///////////////////////////////////////////////////////////////////
//US CShtPrepTime_Base_SheetPitch(UC sheetNo, UC prevShtNo)
US CShtPrepTime_Base_SheetPitch(UC sheetNo)
{
	// !!! Product Specific !!!
	const CShtSheet *pSheet;
	const CShtSheetAttribute *pShtAttr;
	US 		length, pathREK, finPSpeed, iotPSpeed, margin;
	US       sheetPitch;
	UC       finSpeedId;
	UL 		timerTmpL1,timerTmpL2,timerSumL;

	pSheet = CShtManager_GetSheet(sheetNo); 
	pShtAttr = CShtSheet_GetAttribute(pSheet);
	
	length = CShtSheetAttribute_GetProcessSize(pShtAttr); //�p����(L)
	pathREK = cSht_PathDistance[PATH_ID_R] + cSht_PathDistance[PATH_ID_E] +cSht_PathDistance[PATH_ID_K];

	iotPSpeed = CShtSheetAttribute_GetInitialSpeed(pShtAttr); //I : IOT����̗p���������x[mm/s] 
	finPSpeed = SRVC_GetFinisherSpeed(sheetNo);  // F �F ��s���̃t�B�j�b�V���[�������x[mm/s]
	finSpeedId = CShtSheet_GetFinisherSpeedID(pSheet);
	// �}�[�W�����Ԃ����߂�
	margin = CShtPrepTime_GetSheetPitchMargin(sheetNo, finSpeedId, length);
	
	//��{����M[ms] ��( L�|(R+E+K)) �� F �~ 1000 �{(R+E+K) �� I �~ 1000�{�}�[�W������
	if ( length <= pathREK) {
		timerTmpL1 =(UL) ( pathREK - length ) * 10000 /  finPSpeed  ;
		timerTmpL2 =  (UL) pathREK * 10000 / iotPSpeed ;
		
		timerSumL = ( timerTmpL2 - timerTmpL1 )/10L ; // 
		sheetPitch = (US)timerSumL + margin ;

	} else {
		timerTmpL1 =(UL) (length - pathREK) * 10000 /  finPSpeed  ;
		timerTmpL2 =  (UL)pathREK * 10000 / iotPSpeed ;
	
		timerSumL = (timerTmpL1 + timerTmpL2)/10L ; // 
		sheetPitch = (US)timerSumL + margin ;
	}
	PutSheetManagerLog(0x11,sheetNo,iotPSpeed,finPSpeed);

	return  sheetPitch;
	
}
///////////////////////////////////////////////////////////////////
// Function Name : CShtPrepTime_Base_SetPitch
// Description   : Set Pitch �v�Z����
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : sheetNo (�ΏۃV�[�g�̃V�[�g���ʎq)
//               : prevShtNo (��s�V�[�g�̃V�[�g���ʎq)
//               : shtInitTimeInfo
// Return        : �]
// ���X�^�b�J�[�r������ �ŏI�p���ł���ꍇ(�Z�b�g�ԃs�b�`) ��
// �ŏI�p���ł���ꍇ�̊�{���Ԃ̎Z�o�����ȉ��ɋL�q����
// ��{����[ms] ��a �{ b �{ c �{ d
// a	�FCompile Exit Sensor OFF����^���s���O����J�n�܂ł̎���[ms]
// b	�C�W�F�N�g���쎞��[ms]
// c	�FCompile Exit Sensor ON����Compile Exit Sensor off�܂ł̎���[ms]
// d	�F�Z�b�g�ԃs�b�`�����W��[ms]
// ��{����M[ms] ��( L�|(R+E+K)) �� F �~ 1000 �{(R+E+K) �� I �~ 1000�{�}�[�W������
///////////////////////////////////////////////////////////////////
//US CShtPrepTime_Base_SetPitch(UC sheetNo, UC prevShtNo)
US CShtPrepTime_Base_SetPitch(UC sheetNo)
{
	// !!! Product Specific !!!
	const CShtSheet *pSheet;
	const CShtSheetAttribute *pShtAttr;
	US   nParaAB, nParaC;
	SS    sParaD;
	SL 	timerTmpL1,timerTmpL2,timerSumL;

	US   length, pathREK, finPSpeed, iotPSpeed;
	US   setPitch;
	
	pSheet = CShtManager_GetSheet(sheetNo);
	pShtAttr = CShtSheet_GetAttribute(pSheet);
	pathREK = cSht_PathDistance[PATH_ID_R] + cSht_PathDistance[PATH_ID_E] +cSht_PathDistance[PATH_ID_K];
	length = CShtSheetAttribute_GetProcessSize(pShtAttr);
	iotPSpeed = CShtSheetAttribute_GetInitialSpeed(pShtAttr);
	finPSpeed = SRVC_GetFinisherSpeed(sheetNo);

	nParaAB = CMPL_GetBase_CompileFinishingNeedTime(sheetNo);  //A+B
	
	if ( length < PATH_LENGTH_REF_2970 ) {
		sParaD =SRVC_GetNvmValueSS(NVMID_SET_PITCH_SETTING1);
	} else if ( length == PATH_LENGTH_REF_2970 ) {
		sParaD =SRVC_GetNvmValueSS(NVMID_SET_PITCH_SETTING3);  //NVM Add ....
	} else { //( length > PATH_LENGTH_REF_2970 )
		sParaD =SRVC_GetNvmValueSS(NVMID_SET_PITCH_SETTING2);
	}

	if ( length < pathREK ) {
		
		timerSumL = (UL) length * 1000 / finPSpeed  ;
		nParaC = (US)timerSumL;
		
	} else {
		timerTmpL1 = (UL) ( length - pathREK ) * 10000  / iotPSpeed ;
		timerTmpL2 = (UL) pathREK * 10000 /finPSpeed ;
		timerSumL = (timerTmpL1 + timerTmpL2)/10;
		nParaC = (US)timerSumL;
	}

	// ��{����[ms] ��a �{ b �{ c �{ d
	setPitch = (US)(nParaAB  + nParaC + sParaD) ; // 

	return setPitch;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name : CShtPrepTime_GetSheetPitchMargin
// Description   : Sheet Pitch Margin �v�Z����
// Parameter     :
//               : sheetNo (�ΏۃV�[�g�̃V�[�g���ʎq)
//               : aFinSpeed (�������x)
//               : aLength (�p�����蒷)
// Return        : �}�[�W������[ms]
///////////////////////////////////////////////////////////////////////////////////////////////////
 US CShtPrepTime_GetSheetPitchMargin(UC aSheetNo, UC aFinSpeedID, US aLength)
{
	US    marginTime=0;
	US 	mediaWeight = SRVC_GetMediaWeight(aSheetNo); 


	if ( aFinSpeedID  == SHT_FIN_SPD_ID_NVMV3_600 ) { //�t�B�j�b�V���[�������x[mm/s] == 600

		if ( aLength < PATH_LENGTH_REF_2970 ) { //297.0����

			if ((mediaWeight >= 60)  && (mediaWeight < 106)) {
				marginTime = 280; // 250 ; 
				
			} else if ((mediaWeight >= 106)  && (mediaWeight < 177)) {
				marginTime = 240 ;
				
			} else { //(mediaWeight >= 177)  && (mediaWeight < 220)
				marginTime = 240 ;
			}

		} else if ( aLength == PATH_LENGTH_REF_2970 ) { //297.0
		
			if ((mediaWeight >= 60)  && (mediaWeight < 106)) {
				marginTime = 280; // 270 ;
				
			} else if ((mediaWeight >= 106)  && (mediaWeight < 177)) {
				marginTime = 281 ;
				
			} else { //(mediaWeight >= 177)  && (mediaWeight < 220)
				marginTime = 281 ;
			}

		} else { //297.0����
		
			if ((mediaWeight >= 60)  && (mediaWeight < 106)) {
				marginTime = 350 ;
				
			} else if ((mediaWeight >= 106)  && (mediaWeight < 177)) {
				marginTime = 550 ;
				
			} else { //(mediaWeight >= 177)  && (mediaWeight < 220)
				marginTime = 550 ;
			}
		}
	} else if ( aFinSpeedID  == SHT_FIN_SPD_ID_NVMV4_700) { //�t�B�j�b�V���[�������x[mm] == 600
		
			if ( aLength < PATH_LENGTH_REF_2970 ) { //297.0����
		
				if ((mediaWeight >= 60)  && (mediaWeight < 106)) {
					marginTime = 190; // 170 ;
					
				} else if ((mediaWeight >= 106)  && (mediaWeight < 177)) {
					marginTime = 100 ;
					
				} else { //(mediaWeight >= 177)  && (mediaWeight < 220)
					marginTime = 100 ;
				}
		
			} else if ( aLength == PATH_LENGTH_REF_2970 ) { //297.0
			
				if ((mediaWeight >= 60)  && (mediaWeight < 106)) {
					marginTime = 190 ;
					
				} else if ((mediaWeight >= 106)  && (mediaWeight < 177)) {
					marginTime = 119 ;
					
				} else { //(mediaWeight >= 177)  && (mediaWeight < 220)
					marginTime = 119 ;
				}
		
			} else { //297.0����
			
				if ((mediaWeight >= 60)  && (mediaWeight < 106)) {
					marginTime = 302 ;
					
				} else if ((mediaWeight >= 106)  && (mediaWeight < 177)) {
					marginTime = 411 ;
					
				} else { //(mediaWeight >= 177)  && (mediaWeight < 220)
					marginTime = 411 ;
				}
			}

	} else {  //�t�B�j�b�V���[�������x[mm/s] == 800

		if ( aLength < PATH_LENGTH_REF_2970 ) { //297.0����

			if ((mediaWeight >= 60)  && (mediaWeight < 106)) {
				marginTime = 170 ;
				
			} else if ((mediaWeight >= 106)  && (mediaWeight < 177)) {
				marginTime = 100 ;
				
			} else { //(mediaWeight >= 177)  && (mediaWeight < 220)
				marginTime = 100 ;
			}

		} else if ( aLength == PATH_LENGTH_REF_2970 ) { //297.0
		
			if ((mediaWeight >= 60)  && (mediaWeight < 106)) {
				marginTime = 190 ;
				
			} else if ((mediaWeight >= 106)  && (mediaWeight < 177)) {
				marginTime = 119 ;
				
			} else { //(mediaWeight >= 177)  && (mediaWeight < 220)
				marginTime = 119 ;
			}

		} else { //297.0����
		
			if ((mediaWeight >= 60)  && (mediaWeight < 106)) {
				marginTime = 302 ;
				
			} else if ((mediaWeight >= 106)  && (mediaWeight < 177)) {
				marginTime = 411 ;
				
			} else { //(mediaWeight >= 177)  && (mediaWeight < 220)
				marginTime = 411 ;
			}
		}
	}

	return  marginTime;
}

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

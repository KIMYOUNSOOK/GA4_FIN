// File Name:	CNmaHandler.c
// Description:	Implimentation File of NVM Access Handler
// Engineer:	Yuichi Hoshino
// Last Edit:	16.02.26
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CNmaHandler.h"

#include "CSysManager.h"
#include "CKeyInformer.h"
#include "CNvmInformer.h"
#include "CHfsiInformer.h"
//#include "CTempInformer.h"	�yROM�팸�z
#include "CDltManager.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Structure Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Private Constant Number Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Load Function Table ---
static void const (*cNma_LoadNvmFuncTable[NMA_INFORMER_NUM])(void) = {
	CKeyInformer_LoadNvm,	// NMA_INFORMER_KEY
	CNvmInformer_LoadNvm,	// NMA_INFORMER_NVM
	CHfsiInformer_LoadNvm,	// NMA_INFORMER_HFSI
	// CTempInformer_LoadNvm	// NMA_INFORMER_TEMP	�yROM�팸�z
};

// --- Save Function Table ---
static bool const (*cNma_SaveNvmFuncTable[NMA_INFORMER_NUM])(void) = {
	CKeyInformer_SaveNvm,	// NMA_INFORMER_KEY
	CNvmInformer_SaveNvm,	// NMA_INFORMER_NVM
	CHfsiInformer_SaveNvm,	// NMA_INFORMER_HFSI
	// CTempInformer_SaveNvm	// NMA_INFORMER_TEMP	�yROM�팸�z
};


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CNmaHandler_Constructor
// Description   : �R���X�g���N�^
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CNmaHandler_Constructor()
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : CNmaHandler_Load
// Description   : NVM�f�[�^�ǂ݂����w����M������
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CNmaHandler_Load()
{
	// --- ���[�h�J�n���� ---
	cNma_LoadNvmFuncTable[0]();
}

///////////////////////////////////////////////////////////////////
// Function Name : CNmaHandler_Save
// Description   : NVM�f�[�^�������ݎw����M������
// Parameter     : �]
// Return        : true = �J�n
//               : false = �J�n����
///////////////////////////////////////////////////////////////////
bool CNmaHandler_Save()
{
	UC retValue, informerID;

	// --- �Z�[�u�J�n���� ---
	retValue = false;

	if ( CKeyInformer_CheckKeyCode() == true ){									// NVM����ُ햢����
		informerID = NMA_INFORMER_NUM - 1;

		while ( true ){
			if ( cNma_SaveNvmFuncTable[informerID]() == true ){					// �Z�[�u�������{
				retValue = true;

				break;															// �����҂�
			}
			else{																// �Z�[�u�����s�v(���������Ȃ�)
				if ( informerID == 0 ){											// �擪ID�̏ꍇ
					// --- NVM�Z�[�u�����I�� ---
					break;
				}
				else{															// ���Z�[�uNVM����
					informerID--;
				}
			}
		}
	}
	else{																		// NVM����ُ픭��
		// --- NVM�Z�[�u�����I�� ---
	}

	return retValue;
}

///////////////////////////////////////////////////////////////////
// Function Name : CNmaHandler_LoadNvmComplete
// Description   : NVM�f�[�^���[�h�����ʒm��M������
// Parameter     : informerID (NVM Control Informer ID)
//               : result (NVM�f�[�^���[�h��������)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CNmaHandler_LoadNvmComplete(ENmaInformerID informerID, SS result)
{
	UC nextInformerID;

	nextInformerID = informerID + 1;

	if ( (result == DD_NVM_OK) && (nextInformerID < NMA_INFORMER_NUM) ){
		cNma_LoadNvmFuncTable[nextInformerID]();
	}
	else{																		// All NVM load complete or NG complete
		CSysManager_LoadNvmComplete(result);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CNmaHandler_SaveNvmComplete
// Description   : NVM�f�[�^�Z�[�u�����ʒm��M������
// Parameter     : informerID (NVM Control Informer ID)
//               : result (NVM�f�[�^�Z�[�u��������)
// Return        : �]
///////////////////////////////////////////////////////////////////
void CNmaHandler_SaveNvmComplete(ENmaInformerID informerID, SS result)
{
	UC nextInformerID;

	if ( result == DD_NVM_OK ){
		if ( informerID == 0 ){													// �擪ID�̏ꍇ
			// --- NVM�Z�[�u�����I�� ---
			CSysManager_SaveNvmComplete(result);
			CDltManager_SaveNvmComplete(result);
		}
		else{
			nextInformerID = informerID - 1;

			while ( true ){
				if ( cNma_SaveNvmFuncTable[nextInformerID]() == true ){			// �Z�[�u�������{
					break;														// �����҂�
				}
				else{															// �Z�[�u�����s�v(���������Ȃ�)
					if ( nextInformerID == 0 ){									// �擪ID�̏ꍇ
						// --- NVM�Z�[�u�����I�� ---
						CSysManager_SaveNvmComplete(result);
						CDltManager_SaveNvmComplete(result);

						break;
					}
					else{														// ���Z�[�uNVM����
						nextInformerID--;
					}
				}
			}
		}
	}
	else{																		// NG complete
		// --- NVM�Z�[�u�����I�� ---
		CSysManager_SaveNvmComplete(result);
		CDltManager_SaveNvmComplete(result);
	}
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 16.02.26	| Yuichi Hoshino	| Created this file
// 16.02.26	| Yuichi Hoshino	| Introduction Spec [ Download ]
// 16.03.01	| Yuichi Hoshino	| Release Revision 00 to GA-Finisher V1.20.0 Make.
//

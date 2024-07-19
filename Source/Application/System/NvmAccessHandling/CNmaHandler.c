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
//#include "CTempInformer.h"	【ROM削減】
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
	// CTempInformer_LoadNvm	// NMA_INFORMER_TEMP	【ROM削減】
};

// --- Save Function Table ---
static bool const (*cNma_SaveNvmFuncTable[NMA_INFORMER_NUM])(void) = {
	CKeyInformer_SaveNvm,	// NMA_INFORMER_KEY
	CNvmInformer_SaveNvm,	// NMA_INFORMER_NVM
	CHfsiInformer_SaveNvm,	// NMA_INFORMER_HFSI
	// CTempInformer_SaveNvm	// NMA_INFORMER_TEMP	【ROM削減】
};


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CNmaHandler_Constructor
// Description   : コンストラクタ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CNmaHandler_Constructor()
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : CNmaHandler_Load
// Description   : NVMデータ読みだし指示受信時処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CNmaHandler_Load()
{
	// --- ロード開始処理 ---
	cNma_LoadNvmFuncTable[0]();
}

///////////////////////////////////////////////////////////////////
// Function Name : CNmaHandler_Save
// Description   : NVMデータ書き込み指示受信時処理
// Parameter     : ‐
// Return        : true = 開始
//               : false = 開始せず
///////////////////////////////////////////////////////////////////
bool CNmaHandler_Save()
{
	UC retValue, informerID;

	// --- セーブ開始処理 ---
	retValue = false;

	if ( CKeyInformer_CheckKeyCode() == true ){									// NVM制御異常未発生
		informerID = NMA_INFORMER_NUM - 1;

		while ( true ){
			if ( cNma_SaveNvmFuncTable[informerID]() == true ){					// セーブ処理実施
				retValue = true;

				break;															// 完了待ち
			}
			else{																// セーブ処理不要(書き換えなし)
				if ( informerID == 0 ){											// 先頭IDの場合
					// --- NVMセーブ処理終了 ---
					break;
				}
				else{															// 次セーブNVMあり
					informerID--;
				}
			}
		}
	}
	else{																		// NVM制御異常発生
		// --- NVMセーブ処理終了 ---
	}

	return retValue;
}

///////////////////////////////////////////////////////////////////
// Function Name : CNmaHandler_LoadNvmComplete
// Description   : NVMデータロード完了通知受信時処理
// Parameter     : informerID (NVM Control Informer ID)
//               : result (NVMデータロード処理結果)
// Return        : ‐
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
// Description   : NVMデータセーブ完了通知受信時処理
// Parameter     : informerID (NVM Control Informer ID)
//               : result (NVMデータセーブ処理結果)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CNmaHandler_SaveNvmComplete(ENmaInformerID informerID, SS result)
{
	UC nextInformerID;

	if ( result == DD_NVM_OK ){
		if ( informerID == 0 ){													// 先頭IDの場合
			// --- NVMセーブ処理終了 ---
			CSysManager_SaveNvmComplete(result);
			CDltManager_SaveNvmComplete(result);
		}
		else{
			nextInformerID = informerID - 1;

			while ( true ){
				if ( cNma_SaveNvmFuncTable[nextInformerID]() == true ){			// セーブ処理実施
					break;														// 完了待ち
				}
				else{															// セーブ処理不要(書き換えなし)
					if ( nextInformerID == 0 ){									// 先頭IDの場合
						// --- NVMセーブ処理終了 ---
						CSysManager_SaveNvmComplete(result);
						CDltManager_SaveNvmComplete(result);

						break;
					}
					else{														// 次セーブNVMあり
						nextInformerID--;
					}
				}
			}
		}
	}
	else{																		// NG complete
		// --- NVMセーブ処理終了 ---
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

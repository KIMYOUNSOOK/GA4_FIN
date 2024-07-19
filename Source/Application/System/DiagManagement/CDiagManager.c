// File Name:	CDiagManager.c
// Description:	Implimentation File of Diag Manager
// Engineer:	Yuichi Hoshino
// Last Edit:	15.03.30
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CDiagManager.h"

#include "CCdiAdapter.h"
#include "CNvmInformer.h"
#include "CHfsiInformer.h"

#include "CDiagComponentControl.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Const Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- dC135 Parameter ---
#define DIAG_REASONFOREXCHG_NGTHRESH						3

// --- dC361 Parameter ---
#define DIAG_NVM_AREA_ALLOCATED_DATA_MAX_SIZE				128


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Member of Diag Manager ---
typedef struct /*CDiagManagerMember*/
{
	US mNvmAreaRemain;					// NVM Area Access時の通知残りデータ数[x1Byte]
	US mNvmAreaOffset;					// NVM Area Access時の次読み出し時のデータエリアオフセット位置
} CDiagManagerMember;

static CDiagManagerMember gDiag_ManagerMember;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_Constructor
// Description   : コンストラクタ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_Constructor()
{
	gDiag_ManagerMember.mNvmAreaRemain = 0;
	gDiag_ManagerMember.mNvmAreaOffset = 0;

	CDiagComponentControl_Constructor();
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_PowerUp
// Description   : Power Up通知受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_PowerUp()
{
	gDiag_ManagerMember.mNvmAreaRemain = CNvmInformer_GetAreaSize();
	gDiag_ManagerMember.mNvmAreaOffset = 0;
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_DiagnosticsModeStart
// Description   : ダイアグモードへの遷移通知受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_DiagnosticsModeStart()
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_DiagnosticsModeEnd
// Description   : ダイアグモードからの遷移通知受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_DiagnosticsModeEnd()
{
	// No Action
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_ExecuteDiagDc131Read
// Description   : DC131/Read実行指示受信時処理
// Parameter     : chain (Chain No.)
//               : link (Link No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_ExecuteDiagDc131Read(US chain, US link)
{
	CNvmNVM* pNvm;
	UL valMask;
	UC dataType;

	pNvm = CNvmInformer_DiagGet(chain, link);

	if ( pNvm != NULL ){														// Diag Request Successful
		if ( CNvmNVM_IsReadAllowed(pNvm) ){
			dataType = CNvmNVM_GetDataType(pNvm);

			switch( dataType ){
			case eCdiFinDataType_SignChar:
			case eCdiFinDataType_UnsignChar:
				valMask = 0x000000FF;
				break;
			case eCdiFinDataType_SignShort:
			case eCdiFinDataType_UnsignShort:
				valMask = 0x0000FFFF;
				break;
			default:															// Signed Long/Unsigned Long
				valMask = 0xFFFFFFFF;
			}

			CCdiAdapter_SendDiagResponseDc131Read(eCdiFinDiagRslt_Successful,
												   chain,
												   link,
												   dataType,
												   CNvmNVM_IsReadAllowed(pNvm),
												   CNvmNVM_IsWriteAllowed(pNvm),
												   CNvmNVM_IsFactoryInitAllowed(pNvm),
												   (CNvmNVM_GetValue(pNvm) & valMask),
												   (CNvmNVM_GetMinValue(pNvm) & valMask),
												   (CNvmNVM_GetMaxValue(pNvm) & valMask));
		}
		else{																	// Abnormal End
			CCdiAdapter_SendDiagResponseDc131Read(eCdiFinDiagRslt_AbnormalEnd, chain, link, 0, 0, 0, 0, 0, 0, 0);
		}
	}
	else{																		// Failed Identifer
		CCdiAdapter_SendDiagResponseDc131Read(eCdiFinDiagRslt_FailedIdentifier, chain, link, 0, 0, 0, 0, 0, 0, 0);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_ExecuteDiagDc131Write
// Description   : DC131/Write実行指示受信時処理
// Parameter     : chain (Chain No.)
//               : link (Link No.)
//               : dataType (Data Type)
//               : nvmValue (NVM Value)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_ExecuteDiagDc131Write(US chain, US link, enum ECdiFinDataType dataType, UL nvmValue)
{
	CNvmNVM* pNvm;
	UL valMask;
	SL nvmValSL;
	SS nvmValSS;
	SC nvmValSC;
	UC result, updateResp;

	switch ( dataType ){
	case eCdiFinDataType_SignChar:
	case eCdiFinDataType_UnsignChar:
		valMask = 0xFFFFFF00;
		break;	
	case eCdiFinDataType_SignShort:
	case eCdiFinDataType_UnsignShort:
		valMask = 0xFFFF0000;
		break;
	case eCdiFinDataType_SignLong:
	case eCdiFinDataType_UnsignLong:
		valMask = 0x00000000;
		break;		
	default:																	// Failed Data
		valMask = 0xFFFFFFFF;
	}

	if ( valMask != 0xFFFFFFFF ){												// Not Failed Data
		pNvm = CNvmInformer_DiagGet(chain, link);

		if ( pNvm != NULL ){
			if ( CNvmNVM_GetDataType(pNvm) == dataType ){
				if ( (nvmValue & valMask) == 0 ){
					if ( CNvmNVM_IsWriteAllowed(pNvm) ){
						switch ( dataType ){
						case eCdiFinDataType_SignChar:
							if ( (SC)nvmValue < (SC)CNvmNVM_GetMinValue(pNvm) ){
								result = eCdiFinDiagRslt_AbnormalEnd;			// Abnormal End
								updateResp = eCdiFinUpdateResp_ValBelowMin;
							}
							else if ( (SC)nvmValue > (SC)CNvmNVM_GetMaxValue(pNvm) ){
								result = eCdiFinDiagRslt_AbnormalEnd;			// Abnormal End	
								updateResp = eCdiFinUpdateResp_ValAboveMax;
							}
							else{												// Diag Request Successful
								result = eCdiFinDiagRslt_Successful;
								updateResp = eCdiFinUpdateResp_Success;
							}

							break;
						case eCdiFinDataType_SignShort:
							if ( (SS)nvmValue < (SS)CNvmNVM_GetMinValue(pNvm) ){
								result = eCdiFinDiagRslt_AbnormalEnd;			// Abnormal End
								updateResp = eCdiFinUpdateResp_ValBelowMin;
							}
							else if ( (SS)nvmValue > (SS)CNvmNVM_GetMaxValue(pNvm) ){
								result = eCdiFinDiagRslt_AbnormalEnd;			// Abnormal End	
								updateResp = eCdiFinUpdateResp_ValAboveMax;
							}
							else{												// Diag Request Successful
								result = eCdiFinDiagRslt_Successful;
								updateResp = eCdiFinUpdateResp_Success;
							}

							break;
						case eCdiFinDataType_SignLong:
							if ( (SL)nvmValue < (SL)CNvmNVM_GetMinValue(pNvm) ){
								result = eCdiFinDiagRslt_AbnormalEnd;			// Abnormal End
								updateResp = eCdiFinUpdateResp_ValBelowMin;
							}
							else if ( (SL)nvmValue > (SL)CNvmNVM_GetMaxValue(pNvm) ){
								result = eCdiFinDiagRslt_AbnormalEnd;			// Abnormal End
								updateResp = eCdiFinUpdateResp_ValAboveMax;
							}
							else{												// Diag Request Successful
								result = eCdiFinDiagRslt_Successful;
								updateResp = eCdiFinUpdateResp_Success;
							}

							break;
						default:												// Unsigned Data
							if ( nvmValue < CNvmNVM_GetMinValue(pNvm) ){		// Abnormal End
								result = eCdiFinDiagRslt_AbnormalEnd;
								updateResp = eCdiFinUpdateResp_ValBelowMin;
							}
							else if ( nvmValue > CNvmNVM_GetMaxValue(pNvm) ){	// Abnormal End
								result = eCdiFinDiagRslt_AbnormalEnd;
								updateResp = eCdiFinUpdateResp_ValAboveMax;
							}
							else{												// Diag Request Successful
								result = eCdiFinDiagRslt_Successful;
								updateResp = eCdiFinUpdateResp_Success;
							}
						}

						// --- NVMへの書き込み ---
						if ( result == eCdiFinDiagRslt_Successful ){
							switch ( dataType ){
							case eCdiFinDataType_SignChar:
								nvmValSC = (SC)nvmValue;
								nvmValSL = nvmValSC;
								nvmValue = (UL)nvmValSL;
								break;
							case eCdiFinDataType_SignShort:
								nvmValSS = (SS)nvmValue;
								nvmValSL = nvmValSS;
								nvmValue = (UL)nvmValSL;
								break;
							default:											// Unsigned Char, Unsigned Short, Signed Long, Unsigned Long
								break;
							}

							CNvmNVM_SetValue(pNvm, nvmValue);					// 条件はチェック済みであるためNGになることはない
						}
						else{ /* No Action */ }									// チェックNGのためNVMに書き込みは行わない

						// --- Diag Responseメッセージ送信 ---
						CCdiAdapter_SendDiagResponseDc131Write(result, chain, link, updateResp);
					}
					else{														// Abnormal End
						CCdiAdapter_SendDiagResponseDc131Write(eCdiFinDiagRslt_AbnormalEnd, chain, link, eCdiFinUpdateResp_AttrReadOnly);
					}
				}
				else{															// Abnormal End
					CCdiAdapter_SendDiagResponseDc131Write(eCdiFinDiagRslt_AbnormalEnd, chain, link, eCdiFinUpdateResp_IllegalValue);
				}			
			}
			else{																// Abnormal End
				CCdiAdapter_SendDiagResponseDc131Write(eCdiFinDiagRslt_AbnormalEnd, chain, link, eCdiFinUpdateResp_IncorrectDataType);
			}
		}
		else{																	// Failed Identifer
			CCdiAdapter_SendDiagResponseDc131Write(eCdiFinDiagRslt_FailedIdentifier, chain, link, CDIFIN_DONTCARE);
		}
	}
	else{																		// Failed Data
		CCdiAdapter_SendDiagResponseDc131Write(eCdiFinDiagRslt_FailedData, chain, link, CDIFIN_DONTCARE);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_ExecuteDiagDc135Read
// Description   : DC135/Read実行指示受信時処理
// Parameter     : chain (Chain No.)
//               : link (Link No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_ExecuteDiagDc135Read(US chain, US link)
{
	CHfsiHFSI* pHfsi;

	if ( (chain == 0) && (link == 0) ){											// All
		pHfsi = CHfsiInformer_ResetIterate();

		while ( pHfsi != NULL ){
			CCdiAdapter_SendDiagResponseDc135Read(
				eCdiFinDiagRslt_Successful,
				CHfsiHFSI_GetChainNo(pHfsi),
				CHfsiHFSI_GetLinkNo(pHfsi),
				CHfsiHFSI_GetValue(pHfsi),
				CHfsiHFSI_GetLifeSpec(pHfsi),
				CHfsiHFSI_GetExchangeReason(pHfsi, 1),
				CHfsiHFSI_GetLastCount(pHfsi, 1),
				CHfsiHFSI_GetExchangeReason(pHfsi, 2),
				CHfsiHFSI_GetLastCount(pHfsi, 2),
				CHfsiHFSI_GetExchangeReason(pHfsi, 3),
				CHfsiHFSI_GetLastCount(pHfsi, 3),
				CHfsiHFSI_GetPartsName(pHfsi),
				CHfsiHFSI_GetPartsNameSize(pHfsi)
			);

			pHfsi = CHfsiInformer_NextIterate();
		}

		// Complete Notify
		CCdiAdapter_SendDiagResponseDc135Read(eCdiFinDiagRslt_Successful, chain, link, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	}
	else{
		pHfsi = CHfsiInformer_DiagGet(chain, link);

		if ( pHfsi != NULL ){													// Diag Request Successful
			CCdiAdapter_SendDiagResponseDc135Read(
				eCdiFinDiagRslt_Successful,
				chain,
				link,
				CHfsiHFSI_GetValue(pHfsi),
				CHfsiHFSI_GetLifeSpec(pHfsi),
				CHfsiHFSI_GetExchangeReason(pHfsi, 1),
				CHfsiHFSI_GetLastCount(pHfsi, 1),
				CHfsiHFSI_GetExchangeReason(pHfsi, 2),
				CHfsiHFSI_GetLastCount(pHfsi, 2),
				CHfsiHFSI_GetExchangeReason(pHfsi, 3),
				CHfsiHFSI_GetLastCount(pHfsi, 3),
				CHfsiHFSI_GetPartsName(pHfsi),
				CHfsiHFSI_GetPartsNameSize(pHfsi)
			);
		}
		else{																	// Failed Identifer
			CCdiAdapter_SendDiagResponseDc135Read(eCdiFinDiagRslt_FailedIdentifier, chain, link, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_ExecuteDiagDc135Reset
// Description   : DC135/Reset実行指示受信時処理
// Parameter     : chain (Chain No.)
//               : link (Link No.)
//               : reason (Reason For Exchange)
//               : devContID (Device Controller ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_ExecuteDiagDc135Reset(US chain, US link, UC reason, enum ECdiFinDevCont devContID)
{
	CHfsiHFSI* pHfsi;
	UC result;

	if ( reason < DIAG_REASONFOREXCHG_NGTHRESH ){								// Value NG of Reason For Exchange
		if ( (chain == 0) && (link == 0) ){										// All reset
			if ( devContID == OWN_DEV_CONT_ID ){								// Diag Request Successful
				pHfsi = CHfsiInformer_ResetIterate();

				while ( pHfsi != NULL ){
					CHfsiHFSI_Initialize(pHfsi);

					pHfsi = CHfsiInformer_NextIterate();
				}

				result = eCdiFinDiagRslt_Successful;
			}
			else{																// Failed Data
				result = eCdiFinDiagRslt_FailedData;
			}
		}
		else{
			pHfsi = CHfsiInformer_DiagGet(chain, link);

			if ( pHfsi != NULL ){												// Diag Request Successful
				CHfsiHFSI_Reset(pHfsi, reason);

				result = eCdiFinDiagRslt_Successful;
			}
			else{																// Failed Identifer
				result = eCdiFinDiagRslt_FailedIdentifier;
			}
		}
	}
	else{																		// Failed Data
		result = eCdiFinDiagRslt_FailedData;
	}

	CCdiAdapter_SendDiagResponseDc135Reset(result, chain, link, devContID);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_ExecuteDiagDc135Write
// Description   : DC135/Write実行指示受信時処理
// Parameter     : chain (Chain No.)
//               : link (Link No.)
//               : lifeSpec (Life Spec)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_ExecuteDiagDc135Write(US chain, US link, UL lifeSpec)
{
	CHfsiHFSI* pHfsi;

	pHfsi = CHfsiInformer_DiagGet(chain, link);

	if ( pHfsi != NULL ){														// Diag Request Successful
		CHfsiHFSI_SetLifeSpec(pHfsi, lifeSpec);

		CCdiAdapter_SendDiagResponseDc135Write(eCdiFinDiagRslt_Successful, chain, link);
	}
	else{																		// Failed Identifer
		CCdiAdapter_SendDiagResponseDc135Write(eCdiFinDiagRslt_FailedIdentifier, chain, link);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_ExecuteDiagDc301Start
// Description   : DC301/Start実行指示受信時処理
// Parameter     : chain (Chain No.)
//               : link (Link No.)
//               : devContID (Device Controller ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_ExecuteDiagDc301Start(US chain, US link, enum ECdiFinDevCont devContID)
{
	CNvmNVM* pNvm;

	if ( (chain != 0) || (link != 0) ){
		pNvm = CNvmInformer_DiagGet(chain, link);

		if ( pNvm != NULL ){
			if ( CNvmNVM_IsFactoryInitAllowed(pNvm) ){
				CNvmNVM_Initialize(pNvm);
				// 条件はチェック済みであるためNGになることはない。
				CCdiAdapter_SendDiagResponseDc301Start(eCdiFinDiagRslt_Successful, chain, link, devContID, eCdiFinInitResp_Success);
			}
			else{																// Abnormal End
				CCdiAdapter_SendDiagResponseDc301Start(eCdiFinDiagRslt_AbnormalEnd, chain, link, devContID, eCdiFinInitResp_AttrInitProtect);
			}
		}
		else{																	// Failed Identifer
			CCdiAdapter_SendDiagResponseDc301Start(eCdiFinDiagRslt_FailedIdentifier, chain, link, devContID, eCdiFinInitResp_NoSuchAttr);
		}
	}
	else{																		// All
		if ( devContID == OWN_DEV_CONT_ID ){
			CNvmInformer_DiagInitialize();

			CCdiAdapter_SendDiagResponseDc301Start(eCdiFinDiagRslt_Successful, chain, link, devContID, eCdiFinInitResp_Success);
		}
		else{																	// Failed Data
			CCdiAdapter_SendDiagResponseDc301Start(eCdiFinDiagRslt_FailedData, chain, link, devContID, CDIFIN_DONTCARE);
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_ExecuteDiagDc330Monitor
// Description   : DC330/Monitor実行指示受信時処理
// Parameter     : chain (Chain No.)
//               : link (Link No.)
//               : operation (Monitor Operation)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_ExecuteDiagDc330Monitor(US chain, US link, enum ECdiFinMonOpe operation)
{
	switch ( operation ){
	case eCdiFinMonOpe_Start:
		CDiagComponentControl_Start(chain, link);

		break;
	case eCdiFinMonOpe_Stop:
		CDiagComponentControl_Stop(chain, link);

		break;
	case eCdiFinMonOpe_AllStop:
		CDiagComponentControl_AllStopInput();

		break;
	default:																	// Failed Data
		CCdiAdapter_SendDiagResponseDc330Monitor(eCdiFinDiagRslt_FailedData, chain, link, 0, 0);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_ExecuteDiagDc330Write
// Description   : DC330/Write実行指示受信時処理
// Parameter     : chain (Chain No.)
//               : link (Link No.)
//               : compoState (Component State)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_ExecuteDiagDc330Write(US chain, US link, UC compoState)
{
	UC result;

	switch ( compoState ){
	case eCdiFinComponentStt_Off:
		CDiagComponentControl_Off(chain, link);

		break;
	case eCdiFinComponentStt_On:
		CDiagComponentControl_On(chain, link, false);

		break;
	case eCdiFinComponentStt_Cycle:												// Fail(Abnormal End)
		CDiagComponentControl_On(chain, link, true);

		break;
	case eCdiFinComponentStt_AllOff:
		CDiagComponentControl_AllOffOutput();

		break;
	default:																	// Fail(Failed Data)
		CCdiAdapter_SendDiagResponseDc330Write(eCdiFinDiagRslt_FailedData, chain, link, 0);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_ExecuteDiagDc361Start
// Description   : DC361/Start実行指示受信時処理
// Parameter     : nvmArea (NVM Area)
//               : devContID (Device Controller ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_ExecuteDiagDc361Start(enum ECdiFinNvmArea nvmArea, enum ECdiFinDevCont devContID)
{
	switch ( nvmArea ){
	case eCdiFinNvmArea_NVM:
	case eCdiFinNvmArea_DebugLog:
		if ( devContID == OWN_DEV_CONT_ID ){
			if ( nvmArea == eCdiFinNvmArea_NVM ){
				gDiag_ManagerMember.mNvmAreaOffset = 0;
				gDiag_ManagerMember.mNvmAreaRemain = CNvmInformer_GetAreaSize();

				CCdiAdapter_SendDiagResponseDc361Start(eCdiFinDiagRslt_Successful, nvmArea, devContID, gDiag_ManagerMember.mNvmAreaRemain);
			}
			else{																// Abnormal End(eCdiFinNvmArea_DebugLog)
				CCdiAdapter_SendDiagResponseDc361Start(eCdiFinDiagRslt_AbnormalEnd, nvmArea, devContID, 0);
			}
		}
		else{																	// Failed Data
			CCdiAdapter_SendDiagResponseDc361Start(eCdiFinDiagRslt_FailedData, nvmArea, devContID, 0);
		}

		break;
	default:																	// Failed Data
		CCdiAdapter_SendDiagResponseDc361Start(eCdiFinDiagRslt_FailedData, nvmArea, devContID, 0);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_ExecuteDiagDc361Read
// Description   : DC361/Read実行指示受信時処理
// Parameter     : nvmArea (NVM Area)
//               : devContID (Device Controller ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_ExecuteDiagDc361Read(enum ECdiFinNvmArea nvmArea, enum ECdiFinDevCont devContID)
{
	UC allocateValue[DIAG_NVM_AREA_ALLOCATED_DATA_MAX_SIZE], remain, size;

	if ( ((nvmArea == eCdiFinNvmArea_NVM) || (nvmArea == eCdiFinNvmArea_DebugLog)) &&
		 (devContID == OWN_DEV_CONT_ID) ){		
		if ( nvmArea == eCdiFinNvmArea_NVM ){
			if ( gDiag_ManagerMember.mNvmAreaRemain > DIAG_NVM_AREA_ALLOCATED_DATA_MAX_SIZE ){
				remain = eCdiFin_Exist;
				size = DIAG_NVM_AREA_ALLOCATED_DATA_MAX_SIZE;

				CNvmInformer_ReadArea(gDiag_ManagerMember.mNvmAreaOffset, allocateValue, size);

				gDiag_ManagerMember.mNvmAreaOffset += DIAG_NVM_AREA_ALLOCATED_DATA_MAX_SIZE;
				gDiag_ManagerMember.mNvmAreaRemain -= DIAG_NVM_AREA_ALLOCATED_DATA_MAX_SIZE;
			}
			else{
				remain = eCdiFin_NotExist;
				size = (UC)gDiag_ManagerMember.mNvmAreaRemain;

				CNvmInformer_ReadArea(gDiag_ManagerMember.mNvmAreaOffset, allocateValue, size);

				gDiag_ManagerMember.mNvmAreaOffset = 0;
				gDiag_ManagerMember.mNvmAreaRemain = CNvmInformer_GetAreaSize();
			}

			CCdiAdapter_SendDiagResponseDc361Read(eCdiFinDiagRslt_Successful, nvmArea, devContID, remain, size, allocateValue);
		}
		else{																	// Abnormal End(eCdiFinNvmArea_DebugLog)
			CCdiAdapter_SendDiagResponseDc361Read(eCdiFinDiagRslt_AbnormalEnd, nvmArea, devContID, 0, 0, 0);
		}
	}
	else{																		// Failed Data
		CCdiAdapter_SendDiagResponseDc361Read(eCdiFinDiagRslt_FailedData, nvmArea, devContID, 0, 0, 0);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_ExecuteDiagDc361Write
// Description   : DC361/Write実行指示受信時処理
// Parameter     : nvmArea (NVM Area)
//               : devContID (Device Controller ID)
//               : presenceRemain (Presence Of Reamain Data)
//               : allocateSize (Allocated Data Size)
//               : pAllocateValue (Pointer of Allocated Data Value)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_ExecuteDiagDc361Write(enum ECdiFinNvmArea nvmArea, enum ECdiFinDevCont devContID, UC presenceRemain, UC allocateSize, UC* pAllocateValue)
{
	if ( ((nvmArea == eCdiFinNvmArea_NVM) || (nvmArea == eCdiFinNvmArea_DebugLog)) &&
		 ((presenceRemain == eCdiFin_NotExist) || (presenceRemain == eCdiFin_Exist)) &&
		 ( devContID == OWN_DEV_CONT_ID ) ){
		if ( (nvmArea == eCdiFinNvmArea_NVM) && (allocateSize <= DIAG_NVM_AREA_ALLOCATED_DATA_MAX_SIZE) ){
			CNvmInformer_WriteArea(gDiag_ManagerMember.mNvmAreaOffset, pAllocateValue, allocateSize);

			if ( presenceRemain == eCdiFin_Exist ) gDiag_ManagerMember.mNvmAreaOffset += allocateSize;
			else								   gDiag_ManagerMember.mNvmAreaOffset = 0;

			CCdiAdapter_SendDiagResponseDc361Write(eCdiFinDiagRslt_Successful, nvmArea, devContID);
		}
		else{																	// Abnormal End(eCdiFinNvmArea_DebugLog)
			CCdiAdapter_SendDiagResponseDc361Write(eCdiFinDiagRslt_AbnormalEnd, nvmArea, devContID);
		}
	}
	else{																		// Failed Data
		CCdiAdapter_SendDiagResponseDc361Write(eCdiFinDiagRslt_FailedData, nvmArea, devContID);
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_ResponseDiagDc131Read
// Description   : DC131/Read実行指示受信時処理
// Parameter     : result (Diagnostic Result)
//               : chain (Chain No.)
//               : link (Link No.)
//               : dataType (Data Type)
//               : readAllowed (Read Allowed)
//               : writeAllowed (Write Allowed)
//               : factoryInitAllowed (Factory Init Allowed)
//               : crntVal (Current Value)
//               : minVal (Minimum Value)
//               : maxVal (Maximum Value)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_ResponseDiagDc131Read(UC result, US chain, US link, UC dataType, UC readAllowed, UC writeAllowed, UC factoryInitAllowed, UL crntVal, UL minVal, UL maxVal)
{
	CCdiAdapter_SendDiagResponseDc131Read(result, chain, link, dataType, readAllowed, writeAllowed, factoryInitAllowed, crntVal, minVal, maxVal);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_ResponseDiagDc131Write
// Description   : DC131/Write実行指示受信時処理
// Parameter     : result (Diagnostic Result)
//               : chain (Chain No.)
//               : link (Link No.)
//               : updateResponse (Update Response)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_ResponseDiagDc131Write(UC result, US chain, US link, UC updateResponse)
{
	CCdiAdapter_SendDiagResponseDc131Write(result, chain, link, updateResponse);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_ResponseDiagDc135Read
// Description   : DC135/Read実行指示受信時処理
// Parameter     : result (Diagnostic Result)
//               : chain (Chain No.)
//               : link (Link No.)
//               : counterValue (Counter Value)
//               : lifeSpec (Life Spec)
//               : reasonLast (Reason For Last Exchange)
//               : counterValLast (Counter Value At Last Exchange)
//               : reasonLastButOne (Reason For Last Exchange But One)
//               : counterValLastButOne (Counter Value At Last Exchange But One)
//               : reasonLastButTwo (Reason For Last Exchange But Two)
//               : counterValLastButTwo (Counter Value At Last Exchange But Two)
//               : pCounterName (Data pointer of Counter Name)
//               : counterNameSize (Data size of Counter Nam)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_ResponseDiagDc135Read(UC result, US chain, US link, UL counterValue, UL lifeSpec, UC reasonLast, UL counterValLast, UC reasonLastButOne, UL counterValLastButOne, UC reasonLastButTwo, UL counterValLastButTwo, UC const* pCounterName, UC counterNameSize)
{
	CCdiAdapter_SendDiagResponseDc135Read(result, chain, link, counterValue, lifeSpec, reasonLast, counterValLast, reasonLastButOne, counterValLastButOne, reasonLastButTwo, counterValLastButTwo, pCounterName, counterNameSize);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_ResponseDiagDc135Reset
// Description   : DC135/Reset実行指示受信時処理
// Parameter     : result (Diagnostic Result)
//               : chain (Chain No.)
//               : link (Link No.)
//               : devContID (Device Controller ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_ResponseDiagDc135Reset(UC result, US chain, US link, UC devContID)
{
	CCdiAdapter_SendDiagResponseDc135Reset(result, chain, link, devContID);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_ResponseDiagDc135Write
// Description   : DC135/Write実行指示受信時処理
// Parameter     : result (Diagnostic Result)
//               : chain (Chain No.)
//               : link (Link No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_ResponseDiagDc135Write(UC result, US chain, US link)
{
	CCdiAdapter_SendDiagResponseDc135Write(result, chain, link);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_ResponseDiagDc301Start
// Description   : DC301/Start実行指示受信時処理
// Parameter     : result (Diagnostic Result)
//               : chain (Chain No.)
//               : link (Link No.)
//               : devContID (Device Controller ID)
//               : attrInitResp (Attribute Init Response)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_ResponseDiagDc301Start(UC result, US chain, US link, UC devContID, UC attrInitResp)
{
	CCdiAdapter_SendDiagResponseDc301Start(result, chain, link, devContID, attrInitResp);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_ResponseDiagDc330Monitor
// Description   : DC330/Monitor実行指示受信時処理
// Parameter     : result (Diagnostic Result)
//               : chain (Chain No.)
//               : link (Link No.)
//               : componentState (Component State)
//               : transitionCounter (Transition Counter)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_ResponseDiagDc330Monitor(UC result, US chain, US link, UC componentState, UC transitionCounter)
{
	CCdiAdapter_SendDiagResponseDc330Monitor(result, chain, link, componentState, transitionCounter);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_ResponseDiagDc330Write
// Description   : DC330/Write実行指示受信時処理
// Parameter     : result (Diagnostic Result)
//               : chain (Chain No.)
//               : link (Link No.)
//               : componentResponse (Component Response)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_ResponseDiagDc330Write(UC result, US chain, US link, UC componentResponse)
{
	CCdiAdapter_SendDiagResponseDc330Write(result, chain, link, componentResponse);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_ResponseDiagDc361Start
// Description   : DC361/Start実行指示受信時処理
// Parameter     : result (Diagnostic Result)
//               : nvmArea (NVM Area)
//               : devContID (Device Controller ID)
//               : nvmAreaSize (NVM Area Size)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_ResponseDiagDc361Start(UC result, UC nvmArea, UC devContID, US nvmAreaSize)
{
	CCdiAdapter_SendDiagResponseDc361Start(result, nvmArea, devContID, nvmAreaSize);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_ResponseDiagDc361Read
// Description   : DC361/Read実行指示受信時処理
// Parameter     : result (Diagnostic Result)
//               : nvmArea (NVM Area)
//               : devContID (Device Controller ID)
//               : presenceRemain (Presence Of Remain Data)
//               : allocateSize (Allocated Data Size )
//               : pAllocateValue (Data pointer of Allocated Data Value )
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_ResponseDiagDc361Read(UC result, UC nvmArea, UC devContID, UC presenceRemain, UC allocateSize, UC* pAllocateValue)
{
	CCdiAdapter_SendDiagResponseDc361Read(result, nvmArea, devContID, presenceRemain, allocateSize, pAllocateValue);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_ResponseDiagDc361Write
// Description   : DC361/Write実行指示受信時処理
// Parameter     : result (Diagnostic Result)
//               : nvmArea (NVM Area)
//               : devContID (Device Controller ID)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_ResponseDiagDc361Write(UC result, UC nvmArea, UC devContID)
{
	CCdiAdapter_SendDiagResponseDc361Write(result, nvmArea, devContID);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_SendComponentControlStartRequest
// Description   : Component Control Startイベント送信処理
// Parameter     : chain (Chain No.)
//               : link (Link No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_SendComponentControlStartRequest(US chain, US link)
{
	EvtParamComponentControlStartRequest evtParameter;

	evtParameter.chainNo = chain;
	evtParameter.linkNo = link;

	MSP_SendMessage(MID_SYS, EVT_DIAG_COMPO_CONT_START_REQ, (const UC *)(&evtParameter), sizeof(EvtParamComponentControlStartRequest), LOG_OFF);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_SendComponentControlStopRequest
// Description   : Component Control Stopイベント送信処理
// Parameter     : chain (Chain No.)
//               : link (Link No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_SendComponentControlStopRequest(US chain, US link)
{
	EvtParamComponentControlStopRequest evtParameter;

	evtParameter.chainNo = chain;
	evtParameter.linkNo = link;

	MSP_SendMessage(MID_SYS, EVT_DIAG_COMPO_CONT_STOP_REQ, (const UC *)(&evtParameter), sizeof(EvtParamComponentControlStopRequest), LOG_OFF);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_ReceiveComponentControlStartNotify
// Description   : Component Control Start Notifyイベント受信時処理
// Parameter     : moduleID (Module ID)
//               : chain (Chain No.)
//               : link (Link No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_ReceiveComponentControlStartNotify(UC moduleID, US chain, US link)
{
	CDiagComponentControl_OutputStartNotify(chain, link);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagManager_ReceiveComponentControlStopNotify
// Description   : Component Control Stop Notifyイベント受信時処理
// Parameter     : moduleID (Module ID)
//               : chain (Chain No.)
//               : link (Link No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagManager_ReceiveComponentControlStopNotify(UC moduleID, US chain, US link)
{
	CDiagComponentControl_OutputStopNotify(chain, link);
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file based on Base SW VXX
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.05.08	| Yuichi Hoshino	| Created this file
// 14.12.04	| Yuichi Hoshino	| Release V0 (Revision 00)
// 15.03.30	| Yuichi Hoshino	| Modify By Introduction Spec [ NVMパラメータ ]
// 15.03.30	| Yuichi Hoshino	| Design Improvement [ NVM Area Access残エリア管理データ初期化タイミング見直し ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
// 15.04.08	| Yuichi Hoshino	| Trouble Action [ dC301 StartのChain-Link指定時にDon't CareであるDevice Controller IDが自IDでないとFaultになってしまう ]
// 15.06.19	| Yuichi Hoshino	| Design Improvement [ Output ItemのSystem Module実行時に備えたComponent Controlイベント送信処理追加 ]
// 15.07.20	| Yuichi Hoshino	| Trouble Action [ DC131 NVM Writeにて範囲外の値をWrite実行した際、Value Above Maximum/Value Below Minimumが応答されない ]
// 15.07.22	| Yuichi Hoshino	| Trouble Action [ DC361 NVM Save実施時、NVM領域サイズと読み込みデータのサイズが一致しない ]
// 15.12.09	| Yuichi Hoshino	| Design Improvement [ CDiagManager綴り修正 ]
// XX.XX.XX	| Yuichi Hoshino	| Release V2 (Revision 02)
//

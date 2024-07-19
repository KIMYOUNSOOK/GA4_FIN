// File Name:	CDiagComponentControl.c
// Description:	Implimentation File of Diag Component Control
// Engineer:	Yuichi Hoshino
// Last Edit:	14.05.08
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CDiagComponentControl.h"

#include "DD_DIO.h"

#include "CDiagManager.h"
#include "COputInformer.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Private Const Number Definition
/////////////////////////////////////////////////////////////////////////////////////////

#define DIAG_INPUT_COMPONENT_CHECK_CYCLE					10					// [×1ms]
#define DIAG_INPUT_COMPONENT_CHANGE_NOTIFY_CYCLE			50					// [×10(DIAG_INPUT_COMPONENT_CHECK_CYCLE)ms]
#define DIAG_INPUT_COMPONENT_ITEM_NUM						(sizeof(cDiag_InputComponentItemSpec)/sizeof(DiagInputComponentItemSpec))
#define DIAG_OUTPUT_COMPONENT_ITEM_NUM						(sizeof(cDiag_OutputComponentItemSpec)/sizeof(DiagOutputComponentItemSpec))

#define DIAG_OUTPUT_24V_REALY_OFF_LINK						70					// 12- 70 Finisher Main 24V Relay OFF


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Input Component Item Spec ---
typedef struct /*DiagInputComponentItemSpec*/
{
	US chain;
	US link;
	UC inputID;
	UC optionID;
} DiagInputComponentItemSpec;

// --- Parallel Execution Prohibit Item Identifier ---
typedef struct /*DiagParallelExecProhNgItem*/
{
	US chain;
	US link;
} DiagParallelExecProhNgItem;

// --- Output Component Item Spec ---
typedef struct /*DiagOutputComponentItemSpec*/
{
	US chain;
	US link;
	UC cyclic;
	UC moduleID;
	UC optionID;
	const DiagParallelExecProhNgItem *prohItemArray;
} DiagOutputComponentItemSpec;

#include "ComponentControlItemInstance.h"
// Item数確定のため、ComponentControlItemInstance.h内で以下2つのデータを定義
//const DiagInputComponentItemSpec cDiag_InputComponentItemSpec;
//const DiagOutputComponentItemSpec cDiag_OutputComponentItemSpec;

// --- Input Component Item Data ---
typedef struct /*DiagInputComponentItemData*/
{
	UC active;
	UC inputLevel;
	UC transCount;
} DiagInputComponentItemData;

// --- Output Component Item Data ---
typedef struct /*DiagOutputComponentItemData*/
{
	UC active;
} DiagOutputComponentItemData;

// --- Member of Diag Component Control ---
typedef struct /*CDiagComponentControlMember*/
{
	UC mExecInNum;
	UC mExecOutNum;
	UC mAllStopOutNum;
	UC mParaProhOnIndex;
	UC mCycleCount;
	DiagInputComponentItemData mInItem[DIAG_INPUT_COMPONENT_ITEM_NUM];
	DiagOutputComponentItemData mOutItem[DIAG_OUTPUT_COMPONENT_ITEM_NUM];
} CDiagComponentControlMember;

static CDiagComponentControlMember gDiag_ComponentControlMember;


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

static void OutputControl(US chain, US link, EAction action);
static void SYS_InformedCompoContComp(US chain, US link, EAction action);


/////////////////////////////////////////////////////////////////////////////////////////
// Timer Event Call Back Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

void DIAG_MonitorCyclicCheck(SS timerID, UL notUsed);


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CDiagComponentControl_Constructor
// Description   : コンストラクタ
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagComponentControl_Constructor()
{
	UC index;

	gDiag_ComponentControlMember.mExecInNum		   = 0;
	gDiag_ComponentControlMember.mExecOutNum	   = 0;
	gDiag_ComponentControlMember.mAllStopOutNum	   = 0;
	gDiag_ComponentControlMember.mParaProhOnIndex  = DIAG_OUTPUT_COMPONENT_ITEM_NUM;
	gDiag_ComponentControlMember.mCycleCount	   = 0;

	for ( index = 0; index < DIAG_INPUT_COMPONENT_ITEM_NUM; index++ ){
		gDiag_ComponentControlMember.mInItem[index].active	   = false;
		gDiag_ComponentControlMember.mInItem[index].inputLevel = OFF;
		gDiag_ComponentControlMember.mInItem[index].transCount = 0;
	}

	for ( index = 0; index < DIAG_OUTPUT_COMPONENT_ITEM_NUM; index++ ){
		gDiag_ComponentControlMember.mOutItem[index].active = false;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : OutputControl
// Description   : システムモジュール管理出力コンポーネント制御処理
// Parameter     : chain (Chain No.)
//               : link (Link No.)
//               : action (Action)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void OutputControl(US aChainNo, US aLinkNo, EAction aAction)
{
	// !!! Product Specific !!!

	UL aChainLink = (UL)(aChainNo) | ((UL)(aLinkNo) << 16);

	switch ( aLinkNo ) {
	case DIAG_OUTPUT_24V_REALY_OFF_LINK:
		if( aAction == ACT_START ) {
			SYS_InformedCompoContComp(aChainNo, aLinkNo, ACT_START);
			DD_DIO_SetOutput(DD_DD_INTLK_24_ON, OFF);	
		}
		else {																	// ACT_STOP
			SYS_InformedCompoContComp(aChainNo, aLinkNo, ACT_STOP);
			DD_DIO_SetOutput(DD_DD_INTLK_24_ON, ON);
		}
		break;
		
	default:
		break;
	}
}


///////////////////////////////////////////////////////////////////
// Function Name : SYS_InformedCompoContComp
// Description   : SYS Action Complete(Compiler Internal I/F)通知受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void SYS_InformedCompoContComp(US aChainNo, US aLinkNo, EAction aAction)
{
	EvtParamComponentControlStopNotify aEvtParameter;

	aEvtParameter.chainNo = aChainNo;
	aEvtParameter.linkNo = aLinkNo;
	
	if( aAction == ACT_START ) {
		MSP_SendMessage(MID_SYS, EVT_DIAG_COMPO_CONT_START, (const UC*)&aEvtParameter, sizeof(aEvtParameter), LOG_OFF);
	}
	else {																		// ACT_STOP
		MSP_SendMessage(MID_SYS, EVT_DIAG_COMPO_CONT_STOP, (const UC*)&aEvtParameter, sizeof(aEvtParameter), LOG_OFF);
	}
}



///////////////////////////////////////////////////////////////////
// Function Name : CDiagComponentControl_Start
// Description   : インプットアイテム監視開始指示受信処理
// Parameter     : chain (Chain No.)
//               : link (Link No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagComponentControl_Start(US chain, US link)
{
	UC index;

	for( index = 0; index < DIAG_INPUT_COMPONENT_ITEM_NUM; index++ ){
		if( ( cDiag_InputComponentItemSpec[index].chain == chain ) && ( cDiag_InputComponentItemSpec[index].link == link ) ){
			if( gDiag_ComponentControlMember.mInItem[index].active == true ){	// 既に実行中
				CDiagManager_ResponseDiagDc330Monitor(eCdiFinDiagRslt_Successful, chain, link, gDiag_ComponentControlMember.mInItem[index].inputLevel, 0);
			}
			else if( COputInformer_GetOptionDetect((EOptionID)(cDiag_InputComponentItemSpec[index].optionID)) == false ){
																				// Config違い,Option未接続で動作不可
				CDiagManager_ResponseDiagDc330Monitor(eCdiFinDiagRslt_FailedIdentifier, chain, link, 0, 0);
			}
			else if( (gDiag_ComponentControlMember.mExecInNum + gDiag_ComponentControlMember.mExecOutNum) >= 24 ){
																				// 既に24個動いていたら
				CDiagManager_ResponseDiagDc330Monitor(eCdiFinDiagRslt_AbnormalEnd, chain, link, 0, 0);
			}
			else{
				gDiag_ComponentControlMember.mInItem[index].active	  = true;
				gDiag_ComponentControlMember.mInItem[index].inputLevel = DD_DIO_GetInputPortLevel(cDiag_InputComponentItemSpec[index].inputID);
				gDiag_ComponentControlMember.mInItem[index].transCount = 0;

				CDiagManager_ResponseDiagDc330Monitor(eCdiFinDiagRslt_Successful, chain, link, gDiag_ComponentControlMember.mInItem[index].inputLevel, 0);

				if( gDiag_ComponentControlMember.mExecInNum == 0 ){
					gDiag_ComponentControlMember.mCycleCount = 0;
					MSP_SetEventTimer(DIAG_INPUT_COMPONENT_CHECK_CYCLE, DIAG_MonitorCyclicCheck, 0);
				}

				gDiag_ComponentControlMember.mExecInNum++;
			}

			break;
		}
		else{
			// Go for loop.
		}
	}

	if ( index >= DIAG_INPUT_COMPONENT_ITEM_NUM ){
		CDiagManager_ResponseDiagDc330Monitor(eCdiFinDiagRslt_FailedIdentifier, chain, link, 0, 0);
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagComponentControl_Stop
// Description   : インプットアイテム監視停止指示受信処理
// Parameter     : chain (Chain No.)
//               : link (Link No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagComponentControl_Stop(US chain, US link)
{
	UC index;

	for( index = 0; index < DIAG_INPUT_COMPONENT_ITEM_NUM; index++ ){
		if( ( cDiag_InputComponentItemSpec[index].chain == chain ) && ( cDiag_InputComponentItemSpec[index].link == link ) ){
			if ( COputInformer_GetOptionDetect((EOptionID)(cDiag_InputComponentItemSpec[index].optionID)) == false ){
				CDiagManager_ResponseDiagDc330Monitor(eCdiFinDiagRslt_FailedIdentifier, chain, link, 0, 0);
			}
			else if( gDiag_ComponentControlMember.mInItem[index].active == false ){
																				// 実行していない
				CDiagManager_ResponseDiagDc330Monitor(eCdiFinDiagRslt_Successful, chain, link, 0, 0);
			}
			else{
				gDiag_ComponentControlMember.mInItem[index].active = false;
				CDiagManager_ResponseDiagDc330Monitor(eCdiFinDiagRslt_Successful, chain, link, gDiag_ComponentControlMember.mInItem[index].inputLevel, gDiag_ComponentControlMember.mInItem[index].transCount);

				gDiag_ComponentControlMember.mExecInNum--;
			}

			break;
		}
		else{
			// Go for loop.
		}
	}

	if ( index >= DIAG_INPUT_COMPONENT_ITEM_NUM ){
		CDiagManager_ResponseDiagDc330Monitor(eCdiFinDiagRslt_FailedIdentifier, chain, link, 0, 0);
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagComponentControl_AllStopInput
// Description   : 全インプットアイテム監視停止指示受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagComponentControl_AllStopInput()
{
	UC index;

	for( index = 0; index < DIAG_INPUT_COMPONENT_ITEM_NUM; index++ ){
		if( gDiag_ComponentControlMember.mInItem[index].active == true ){
			gDiag_ComponentControlMember.mInItem[index].active = false;
		}
		else{
			// Go for loop.
		}
	}

	gDiag_ComponentControlMember.mExecInNum = 0;

	CDiagManager_ResponseDiagDc330Monitor(eCdiFinDiagRslt_Successful, 0, 0, 0, 0);
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagComponentControl_On
// Description   : アウトプットアイテム制御開始指示受信処理
// Parameter     : chain (Chain No.)
//               : link (Link No.)
//               : cyclic (Cyclic)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagComponentControl_On(US chain, US link, UC cyclic)
{
	UC index, prohIndex, checkIndex;

	for( index = 0; index < DIAG_OUTPUT_COMPONENT_ITEM_NUM; index++ ){
		if( ( cDiag_OutputComponentItemSpec[index].chain == chain ) && ( cDiag_OutputComponentItemSpec[index].link == link ) ){
			if( gDiag_ComponentControlMember.mOutItem[index].active == true ){	// 既に実行中
				CDiagManager_ResponseDiagDc330Write(eCdiFinDiagRslt_Successful, chain, link, eCdiFinComponentResp_AlreadyOn);
			}
			else if( COputInformer_GetOptionDetect((EOptionID)(cDiag_OutputComponentItemSpec[index].optionID)) == false ){
																				// Config違い,Option未接続で動作不可
				CDiagManager_ResponseDiagDc330Write(eCdiFinDiagRslt_FailedIdentifier, chain, link, 0);
			}
			else if( (gDiag_ComponentControlMember.mExecInNum + gDiag_ComponentControlMember.mExecOutNum) >= 24 ){
																				// 既に24個動いていたら
				CDiagManager_ResponseDiagDc330Write(eCdiFinDiagRslt_AbnormalEnd, chain, link, 0);
			}
			else if( (cyclic == true) && (cDiag_OutputComponentItemSpec[index].cyclic == false) ){
				CDiagManager_ResponseDiagDc330Write(eCdiFinDiagRslt_AbnormalEnd, chain, link, 0);
			}
			else{
				// --- 同時実行禁止チェック ---
				if ( cDiag_OutputComponentItemSpec[index].prohItemArray != NULL ){
					for ( prohIndex = 0; cDiag_OutputComponentItemSpec[index].prohItemArray[prohIndex].chain != 0; prohIndex++ ){
						for ( checkIndex = 0; checkIndex < DIAG_OUTPUT_COMPONENT_ITEM_NUM; checkIndex++ ){
							if ( (cDiag_OutputComponentItemSpec[index].prohItemArray[prohIndex].chain == cDiag_OutputComponentItemSpec[checkIndex].chain) &&
								 (cDiag_OutputComponentItemSpec[index].prohItemArray[prohIndex].link == cDiag_OutputComponentItemSpec[checkIndex].link) ){
								if( gDiag_ComponentControlMember.mOutItem[checkIndex].active == true ){
									gDiag_ComponentControlMember.mParaProhOnIndex = index;

									if( cDiag_OutputComponentItemSpec[checkIndex].moduleID == MID_SYS ){
										OutputControl(cDiag_OutputComponentItemSpec[checkIndex].chain, cDiag_OutputComponentItemSpec[checkIndex].link, ACT_STOP);
									}
									else{
										CDiagManager_SendComponentControlStopRequest(cDiag_OutputComponentItemSpec[checkIndex].chain, cDiag_OutputComponentItemSpec[checkIndex].link);
									}
								}
								else{
									// No Action
								}

								break;
							}
							else{
								// Go for loop.
							}
						}
					}
				}
				else{
					// No Action
				}

				if ( gDiag_ComponentControlMember.mParaProhOnIndex != index ){	// 開始指示アイテムが同時実行禁止アイテム停止待ち中ではない
					// --- 指示アイテム開始 ---
					gDiag_ComponentControlMember.mOutItem[index].active = true;

					if( cDiag_OutputComponentItemSpec[index].moduleID == MID_SYS ){
						OutputControl(chain, link, ACT_START);
					}
					else{
						CDiagManager_SendComponentControlStartRequest(chain, link);
					}

					gDiag_ComponentControlMember.mExecOutNum++;
				}
				else{															// 停止待ち同時実行禁止アイテムあり
					// No Action
				}
			}

			break;
		}
		else{
			// Go for loop.
		}
	}

	if ( index >= DIAG_OUTPUT_COMPONENT_ITEM_NUM ){
		CDiagManager_ResponseDiagDc330Write(eCdiFinDiagRslt_FailedIdentifier, chain, link, 0);
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagComponentControl_Off
// Description   : アウトプットアイテム制御停止指示受信処理
// Parameter     : chain (Chain No.)
//               : link (Link No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagComponentControl_Off(US chain, US link)
{
	UC index;

	for( index = 0; index < DIAG_OUTPUT_COMPONENT_ITEM_NUM; index++ ){
		if( ( cDiag_OutputComponentItemSpec[index].chain == chain ) && ( cDiag_OutputComponentItemSpec[index].link == link ) ){
			if( gDiag_ComponentControlMember.mOutItem[index].active == false ){	// 実行していない
				CDiagManager_ResponseDiagDc330Write(eCdiFinDiagRslt_Successful, chain, link, eCdiFinComponentResp_AlreadyOff);
			}
			else{
				if( cDiag_OutputComponentItemSpec[index].moduleID == MID_SYS ){
					OutputControl(chain, link, ACT_STOP);
				}
				else{
					CDiagManager_SendComponentControlStopRequest(chain, link);
				}
			}

			break;
		}
		else{
			// Go for loop.
		}
	}

	if ( index >= DIAG_OUTPUT_COMPONENT_ITEM_NUM ){
		CDiagManager_ResponseDiagDc330Write(eCdiFinDiagRslt_FailedIdentifier, chain, link, 0);
	}
	else{
		// No Action
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagComponentControl_AllOffOutput
// Description   : 全アウトプットアイテム制御停止指示受信処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagComponentControl_AllOffOutput()
{
	UC index;

	for( index = 0; index < DIAG_OUTPUT_COMPONENT_ITEM_NUM; index++ ){
		if( gDiag_ComponentControlMember.mOutItem[index].active == true ){
			gDiag_ComponentControlMember.mAllStopOutNum++;
			CDiagManager_SendComponentControlStopRequest(cDiag_OutputComponentItemSpec[index].chain, cDiag_OutputComponentItemSpec[index].link);
		}
		else{
			// Go for loop.
		}
	}
}


///////////////////////////////////////////////////////////////////
// Function Name : CDiagComponentControl_OutputStartNotify
// Description   : アウトプットアイテム制御開始通知受信処理
// Parameter     : chain (Chain No.)
//               : link (Link No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagComponentControl_OutputStartNotify(US chain, US link)
{
	UC index;

	for ( index = 0; index < DIAG_OUTPUT_COMPONENT_ITEM_NUM; index++ ){
		if( ( cDiag_OutputComponentItemSpec[index].chain == chain ) && ( cDiag_OutputComponentItemSpec[index].link == link ) ){
			if ( gDiag_ComponentControlMember.mOutItem[index].active == true ){
				CDiagManager_ResponseDiagDc330Write(eCdiFinDiagRslt_Successful, chain, link, eCdiFinComponentResp_SuccessOn);
			}
			else{
				// No Action
			}

			break;
		}
		else{
			// Go for loop.
		}
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagComponentControl_OutputStopNotify
// Description   : アウトプットアイテム制御停止通知受信処理
// Parameter     : chain (Chain No.)
//               : link (Link No.)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagComponentControl_OutputStopNotify(US chain, US link)
{
	UC index, prohIndex, checkIndex;

	for ( index = 0; index < DIAG_OUTPUT_COMPONENT_ITEM_NUM; index++ ){
		if( ( cDiag_OutputComponentItemSpec[index].chain == chain ) && ( cDiag_OutputComponentItemSpec[index].link == link ) ){
			if ( gDiag_ComponentControlMember.mOutItem[index].active == true ){
				gDiag_ComponentControlMember.mOutItem[index].active = false;

				gDiag_ComponentControlMember.mExecOutNum--;

				if ( gDiag_ComponentControlMember.mAllStopOutNum == 0 ){
					CDiagManager_ResponseDiagDc330Write(eCdiFinDiagRslt_Successful, chain, link, eCdiFinComponentResp_SuccessOff);
				}
				else{															// All Stop Response
					if ( gDiag_ComponentControlMember.mAllStopOutNum == 1 ){
						CDiagManager_ResponseDiagDc330Write(eCdiFinDiagRslt_Successful, 0, 0, eCdiFinComponentResp_SuccessOff);
					}
					else{ /* No Action */ }

					gDiag_ComponentControlMember.mAllStopOutNum--;
				}

				// --- 同時実行禁止による停止待ちアイテムの実行 ---
				if ( gDiag_ComponentControlMember.mParaProhOnIndex != DIAG_OUTPUT_COMPONENT_ITEM_NUM ){
					for ( prohIndex = 0; cDiag_OutputComponentItemSpec[gDiag_ComponentControlMember.mParaProhOnIndex].prohItemArray[prohIndex].chain != 0; prohIndex++ ){
						for ( checkIndex = 0; checkIndex < DIAG_OUTPUT_COMPONENT_ITEM_NUM; checkIndex++ ){
							if ( (cDiag_OutputComponentItemSpec[gDiag_ComponentControlMember.mParaProhOnIndex].prohItemArray[prohIndex].chain == cDiag_OutputComponentItemSpec[checkIndex].chain) &&
								 (cDiag_OutputComponentItemSpec[gDiag_ComponentControlMember.mParaProhOnIndex].prohItemArray[prohIndex].link == cDiag_OutputComponentItemSpec[checkIndex].link) ){
								break;
							}
							else{ /* Go for loop. */ }
						}

						if ( (checkIndex < DIAG_OUTPUT_COMPONENT_ITEM_NUM) &&	// 同時実行禁止アイテム発見出来た(馬鹿よけ)
							 (gDiag_ComponentControlMember.mOutItem[checkIndex].active == true) ){
							break;												// 同時実行禁止アイテム動作中
						}
						else{ /* Go for loop. */ }
					}

					if ( cDiag_OutputComponentItemSpec[gDiag_ComponentControlMember.mParaProhOnIndex].prohItemArray[prohIndex].chain == 0 ){
																				// 動作中同時実行禁止アイテムなし
						// --- 停止待ちアイテム開始 ---
						gDiag_ComponentControlMember.mOutItem[gDiag_ComponentControlMember.mParaProhOnIndex].active = true;

						if( cDiag_OutputComponentItemSpec[gDiag_ComponentControlMember.mParaProhOnIndex].moduleID == MID_SYS ){
							OutputControl(cDiag_OutputComponentItemSpec[gDiag_ComponentControlMember.mParaProhOnIndex].chain,
										  cDiag_OutputComponentItemSpec[gDiag_ComponentControlMember.mParaProhOnIndex].link,
										  ACT_START);
						}
						else{
							CDiagManager_SendComponentControlStartRequest(cDiag_OutputComponentItemSpec[gDiag_ComponentControlMember.mParaProhOnIndex].chain,
																		  cDiag_OutputComponentItemSpec[gDiag_ComponentControlMember.mParaProhOnIndex].link);
						}

						gDiag_ComponentControlMember.mExecOutNum++;

						gDiag_ComponentControlMember.mParaProhOnIndex = DIAG_OUTPUT_COMPONENT_ITEM_NUM;
					}
					else{ /* No Action(同時実行禁止アイテム停止待ち) */ }
				}
				else{ /* No Action */ }											// 停止待ちアイテムなし
			}
			else{ /* No Action */ }												// 未実行アイテムの停止通知(イレギュラーケース)

			break;
		}
		else{ /* Go for loop. */ }
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : CDiagComponentControl_InputMonitor
// Description   : インプットアイテム監視周期処理
// Parameter     : ‐
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CDiagComponentControl_InputMonitor()
{
	UC index, portLevel;

	if ( gDiag_ComponentControlMember.mExecInNum != 0 ){
		for( index = 0; index < DIAG_INPUT_COMPONENT_ITEM_NUM; index++ ){
			if( gDiag_ComponentControlMember.mInItem[index].active == true ){	// Monitor中のコンポーネントに対して処理
				portLevel = DD_DIO_GetInputPortLevel(cDiag_InputComponentItemSpec[index].inputID);

				if ( portLevel != gDiag_ComponentControlMember.mInItem[index].inputLevel ){
					gDiag_ComponentControlMember.mInItem[index].inputLevel = portLevel;
					gDiag_ComponentControlMember.mInItem[index].transCount++;	// OverFlowすることはない(10ms周期で500ms間)ので、ガードはしない
				}
				else{
					// No Action
				}

				if ( gDiag_ComponentControlMember.mCycleCount >= DIAG_INPUT_COMPONENT_CHANGE_NOTIFY_CYCLE ){
					if ( gDiag_ComponentControlMember.mInItem[index].transCount != 0 ){
						CDiagManager_ResponseDiagDc330Monitor(eCdiFinDiagRslt_Successful, cDiag_InputComponentItemSpec[index].chain, cDiag_InputComponentItemSpec[index].link, gDiag_ComponentControlMember.mInItem[index].inputLevel, gDiag_ComponentControlMember.mInItem[index].transCount);

						gDiag_ComponentControlMember.mInItem[index].transCount = 0;
					}
					else{
						// No Action
					}
				}
			}
			else{
				// Go for loop.
			}
		}

		if ( gDiag_ComponentControlMember.mCycleCount >= DIAG_INPUT_COMPONENT_CHANGE_NOTIFY_CYCLE ){
			gDiag_ComponentControlMember.mCycleCount = 0;
		}
		else{
			gDiag_ComponentControlMember.mCycleCount++;
		}

		MSP_SetEventTimer(DIAG_INPUT_COMPONENT_CHECK_CYCLE, DIAG_MonitorCyclicCheck, 0);
	}
	else{
		// No Action
	}
}


/////////////////////////////////////////////////////////////////////////////////////////
// Timer Event Call Back Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : DIAG_MonitorCyclicCheck
// Description   : タイマーイベントコールバック
// Parameter     : timerID (Timer ID)
//               : notUsed (Not used parameter)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void DIAG_MonitorCyclicCheck(SS timerID, UL notUsed)
{
	CDiagComponentControl_InputMonitor();
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
// 15.06.19	| Yuichi Hoshino	| Trouble Action [ 多重実行禁止による停止処理見直し ]
// 15.12.09	| Yuichi Hoshino	| Design Improvement [ CDiagManager綴り修正 ]
// 15.XX.XX	| Yuichi Hoshino	| Release V2 (Revision 01)
//

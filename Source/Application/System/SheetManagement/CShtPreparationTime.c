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
	1157,		// IOT筺体の用紙搬出部フレームとIOT Fusing-Roll間パス長
	243,			//   IOT筺体の用紙搬出部フレームからENTRANCE-ROLLまでの距離
	1300,			// ENTRANCE-ROLLからCompile-Exit Sensorまでの距離
	4100,		// 先行紙のIOTからの用紙搬入速度[mm/s]	
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
// Description   : コンストラクタ
// Parameter     : pThis (インスタンスポインタ)
// Return        : ‐
///////////////////////////////////////////////////////////////////
void CShtPreparationTime_Constructor(CShtPreparationTime* pThis)
{
	pThis->mPrepTime	= 0;
	pThis->mShortenTime = 0;
}

///////////////////////////////////////////////////////////////////
// Function Name : CShtPreparationTime_Calculate
// Description   : Preparation Time計算処理
// Parameter     : pThis (インスタンスポインタ)
//               : sheetNo (対象シートのシート識別子)
//               : prevShtNo (先行シートのシート識別子)
//               : shtInitTimeInfo
// Return        : ‐
// 用紙搬入間隔[ms] ＝ ① ＋ ② ＋ ③ ＋ ④
// ① ： 基本時間[ms]
// ② ： 加算時間[ms]
// ③ ： シートイニシャライズ動作時間[ms]
// ④ ： シートフィニッシュリクエスト待ち時間[ms]
// 基本時間M[ms] ＝( L－(R+E+K)) ÷ F × 1000 ＋(R+E+K) ÷ I × 1000＋マージン時間
// 加算時間 :  	コンパイラフィニッシング動作時間
// 				タンパー逃がし動作必要時間
// 				サブパドル用紙搬入 時の動作回数が3回の場合の加算時間
//				コンパイラフィニッシング動作時間
// R ： IOT筺体の用紙搬出部フレームとIOT Fusing-Roll間パス長
// E ： IOT筺体の用紙搬出部フレームからENTRANCE-ROLLまでの距離
// K ： ENTRANCE-ROLLからCompile-Exit Sensorまでの距離
// I  ： 先行紙のIOTからの用紙搬入速度[mm/s] ※関連サブ間入出力参照
// F ： 先行紙のフィニッシャー搬送速度[mm/s] ＝ 600.0／800.0
///////////////////////////////////////////////////////////////////
void CShtPreparationTime_Calculate(CShtPreparationTime* pThis, UC sheetNo, UC prevShtNo,  ShtInitTimeInfo shtInitTimeInfo)
{
	US			 timer;  
	EShtEject		preSheet;
	
	pThis->mPrepTime = 0;
	pThis->mShortenTime = 0;
	
	preSheet = SRVC_GetEject(prevShtNo);

	 // Sheet間Pitch を求めるとき
	if(preSheet  != SHT_EJECT_EXEC) {
		timer = CShtPrepTime_Base_SheetPitch(prevShtNo)
			+ CMPL_GetAdd_TamperOpenNeedTime(prevShtNo)
			+ CMPL_GetAdd_SubPaddleNeedTime(prevShtNo);		//搬送系 用紙間ピッチ算出

	} else { 
		//Set間Pitch を求めるとき
		timer = CShtPrepTime_Base_SetPitch(prevShtNo) 
			+ CMPL_GetAdd_CompileFinishingNeedTime(prevShtNo);	   //搬送系 Set間ピッチ算出
	}	
	
	//V3.1.7 : Job Initialize Time Added .. 
	pThis->mPrepTime = timer + shtInitTimeInfo.sheetInitTime[SHT_INIT_ID_COMPILER];
}


///////////////////////////////////////////////////////////////////
// Function Name : CShtPrepTime_Base_SheetPitch
// Description   : Sheet Pitch 計算処理
// Parameter     : sheetNo (対象シートのシート識別子)
//               	: prevShtNo (先行シートのシート識別子)
// Return        :  sheetPitch
// 基本時間M[ms] ＝( L－(R+E+K)) ÷ F × 1000 ＋(R+E+K) ÷ I × 1000＋マージン時間
// R ： IOT筺体の用紙搬出部フレームとIOT Fusing-Roll間パス長
// E ： IOT筺体の用紙搬出部フレームからENTRANCE-ROLLまでの距離
// K ： ENTRANCE-ROLLからCompile-Exit Sensorまでの距離
// I  ： 先行紙のIOTからの用紙搬入速度[mm/s] ※関連サブ間入出力参照
// F ： 先行紙のフィニッシャー搬送速度[mm/s] ＝ 600.0／800.0
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
	
	length = CShtSheetAttribute_GetProcessSize(pShtAttr); //用紙長(L)
	pathREK = cSht_PathDistance[PATH_ID_R] + cSht_PathDistance[PATH_ID_E] +cSht_PathDistance[PATH_ID_K];

	iotPSpeed = CShtSheetAttribute_GetInitialSpeed(pShtAttr); //I : IOTからの用紙搬入速度[mm/s] 
	finPSpeed = SRVC_GetFinisherSpeed(sheetNo);  // F ： 先行紙のフィニッシャー搬送速度[mm/s]
	finSpeedId = CShtSheet_GetFinisherSpeedID(pSheet);
	// マージン時間を求める
	margin = CShtPrepTime_GetSheetPitchMargin(sheetNo, finSpeedId, length);
	
	//基本時間M[ms] ＝( L－(R+E+K)) ÷ F × 1000 ＋(R+E+K) ÷ I × 1000＋マージン時間
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
// Description   : Set Pitch 計算処理
// Parameter     : pThis (インスタンスポインタ)
//               : sheetNo (対象シートのシート識別子)
//               : prevShtNo (先行シートのシート識別子)
//               : shtInitTimeInfo
// Return        : ‐
// ＜スタッカー排紙時の 最終用紙である場合(セット間ピッチ) ＞
// 最終用紙である場合の基本時間の算出式を以下に記述する
// 基本時間[ms] ＝a ＋ b ＋ c ＋ d
// a	：Compile Exit Sensor OFFからタンピング動作開始までの時間[ms]
// b	イジェクト動作時間[ms]
// c	：Compile Exit Sensor ONからCompile Exit Sensor offまでの時間[ms]
// d	：セット間ピッチ調整係数[ms]
// 基本時間M[ms] ＝( L－(R+E+K)) ÷ F × 1000 ＋(R+E+K) ÷ I × 1000＋マージン時間
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

	// 基本時間[ms] ＝a ＋ b ＋ c ＋ d
	setPitch = (US)(nParaAB  + nParaC + sParaD) ; // 

	return setPitch;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Function Name : CShtPrepTime_GetSheetPitchMargin
// Description   : Sheet Pitch Margin 計算処理
// Parameter     :
//               : sheetNo (対象シートのシート識別子)
//               : aFinSpeed (搬送速度)
//               : aLength (用紙送り長)
// Return        : マージン時間[ms]
///////////////////////////////////////////////////////////////////////////////////////////////////
 US CShtPrepTime_GetSheetPitchMargin(UC aSheetNo, UC aFinSpeedID, US aLength)
{
	US    marginTime=0;
	US 	mediaWeight = SRVC_GetMediaWeight(aSheetNo); 


	if ( aFinSpeedID  == SHT_FIN_SPD_ID_NVMV3_600 ) { //フィニッシャー搬送速度[mm/s] == 600

		if ( aLength < PATH_LENGTH_REF_2970 ) { //297.0未満

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

		} else { //297.0超え
		
			if ((mediaWeight >= 60)  && (mediaWeight < 106)) {
				marginTime = 350 ;
				
			} else if ((mediaWeight >= 106)  && (mediaWeight < 177)) {
				marginTime = 550 ;
				
			} else { //(mediaWeight >= 177)  && (mediaWeight < 220)
				marginTime = 550 ;
			}
		}
	} else if ( aFinSpeedID  == SHT_FIN_SPD_ID_NVMV4_700) { //フィニッシャー搬送速度[mm] == 600
		
			if ( aLength < PATH_LENGTH_REF_2970 ) { //297.0未満
		
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
		
			} else { //297.0超え
			
				if ((mediaWeight >= 60)  && (mediaWeight < 106)) {
					marginTime = 302 ;
					
				} else if ((mediaWeight >= 106)  && (mediaWeight < 177)) {
					marginTime = 411 ;
					
				} else { //(mediaWeight >= 177)  && (mediaWeight < 220)
					marginTime = 411 ;
				}
			}

	} else {  //フィニッシャー搬送速度[mm/s] == 800

		if ( aLength < PATH_LENGTH_REF_2970 ) { //297.0未満

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

		} else { //297.0超え
		
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

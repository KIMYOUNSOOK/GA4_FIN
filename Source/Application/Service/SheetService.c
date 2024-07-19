// File Name:	SheetService.c
// Description:	Implimentation File of Sheet Service
// Engineer:	Yuichi Hoshino
// Last Edit:	14.12.29
// Revision:	01
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "SheetService.h"

#include "CShtManager.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetOutputDestination
// Description   : 指示された制御対象シートの排紙先指定を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : ECdiFinOutDestの定義を参照
///////////////////////////////////////////////////////////////////
enum ECdiFinOutDest SRVC_GetOutputDestination(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetOutputDestination(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetBinSelection
// Description   : 指示された制御対象シートの排紙先ビン指定を公開す
//               : る。
// Parameter     : sheetNo (Sheet No.)
// Return        : 1～255
///////////////////////////////////////////////////////////////////
UC SRVC_GetBinSelection(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetBinSelection(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetSetEndAction
// Description   : 指示された制御対象シート(セット)のセット処理方法
//               : を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : eCdiFinSetEndAct_NrmlEnd/eCdiFinSetEndAct_AbortSet
///////////////////////////////////////////////////////////////////
enum ECdiFinSetEndAct SRVC_GetSetEndAction(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetSetEndAction(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetSheetIntegrity
// Description   : 指示された制御対象シートの搬入シート状態情報を公
//               : 開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : eCdiFinIntegrity_Normal/eCdiFinIntegrity_Abnormal
///////////////////////////////////////////////////////////////////
enum ECdiFinIntegrity SRVC_GetSheetIntegrity(UC sheetNo)
{
	const CShtSheet *pSheet;
	const CShtSheetAttribute *pAttribute;

	pSheet = CShtManager_GetSheet(sheetNo);
	pAttribute = CShtSheet_GetAttribute(pSheet);

	return CShtSheetAttribute_GetSheetIntegrity(pAttribute);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetSetAction
// Description   : 指示された制御対象シートのセット単位としてのアク
//               : ション情報を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : eCdiFinSetAct_NoAct/eCdiFinSetAct_NewSet
///////////////////////////////////////////////////////////////////
enum ECdiFinSetAction SRVC_GetSetAction(UC sheetNo)
{
	const CShtSheet *pSheet;
	const CShtSheetAttribute *pAttribute;

	pSheet = CShtManager_GetSheet(sheetNo);
	pAttribute = CShtSheet_GetAttribute(pSheet);

	return CShtSheetAttribute_GetSetAction(pAttribute);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetOffsetMode
// Description   : 指示された制御対象シート(を含むセット)のオフセッ
//               : ト機能の制御種別を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : eCdiFinOffset_Off/eCdiFinOffset_On
///////////////////////////////////////////////////////////////////
enum ECdiFinOffsetMode SRVC_GetOffsetMode(UC sheetNo)
{
	const CShtSheet *pSheet;
	const CShtSheetAttribute *pAttribute;

	pSheet = CShtManager_GetSheet(sheetNo);
	pAttribute = CShtSheet_GetAttribute(pSheet);

	return CShtSheetAttribute_GetOffsetMode(pAttribute);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetOffsetPosition
// Description   : 指示された制御対象シート(を含むセット)のオフセッ
//               : ト位置情報を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : ECdiFinOffsetPosの定義を参照
///////////////////////////////////////////////////////////////////
enum ECdiFinOffsetPos SRVC_GetOffsetPosition(UC sheetNo)
{
	const CShtSheet *pSheet;
	const CShtSheetAttribute *pAttribute;

	pSheet = CShtManager_GetSheet(sheetNo);
	pAttribute = CShtSheet_GetAttribute(pSheet);

	return CShtSheetAttribute_GetOffsetPosition(pAttribute);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetStapleMode
// Description   : 指示された制御対象シート(を含むセット)のステープ
//               : ル機能の制御種別を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : ECdiFinStapleModeの定義を参照
///////////////////////////////////////////////////////////////////
enum ECdiFinStapleMode SRVC_GetStapleMode(UC sheetNo)
{
	const CShtSheet *pSheet;
	const CShtSheetAttribute *pAttribute;

	pSheet = CShtManager_GetSheet(sheetNo);
	pAttribute = CShtSheet_GetAttribute(pSheet);

	return CShtSheetAttribute_GetStapleMode(pAttribute);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetPunchMode
// Description   : 指示された制御対象シートのパンチ機能の制御種別を
//               : 公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : ECdiFinPunchModeの定義を参照
///////////////////////////////////////////////////////////////////
enum ECdiFinPunchMode SRVC_GetPunchMode(UC sheetNo)
{
	const CShtSheet *pSheet;
	const CShtSheetAttribute *pAttribute;

	pSheet = CShtManager_GetSheet(sheetNo);
	pAttribute = CShtSheet_GetAttribute(pSheet);

	return CShtSheetAttribute_GetPunchMode(pAttribute);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetBookletMakerMode
// Description   : 指示された制御対象シート(を含むセット)の製本機能
//               : の制御種別を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : ECdiFinBookModeの定義を参照
///////////////////////////////////////////////////////////////////
enum ECdiFinBookMode SRVC_GetBookletMakerMode(UC sheetNo)
{
	const CShtSheet *pSheet;
	const CShtSheetAttribute *pAttribute;

	pSheet = CShtManager_GetSheet(sheetNo);
	pAttribute = CShtSheet_GetAttribute(pSheet);

	return CShtSheetAttribute_GetBookletMakerMode(pAttribute);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetFoldMode
// Description   : 指示された制御対象シートのフォールド機能の制御種
//               : 別を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : ECdiFinFoldModeの定義を参照
///////////////////////////////////////////////////////////////////
enum ECdiFinFoldMode SRVC_GetFoldMode(UC sheetNo)
{
	const CShtSheet *pSheet;
	const CShtSheetAttribute *pAttribute;

	pSheet = CShtManager_GetSheet(sheetNo);
	pAttribute = CShtSheet_GetAttribute(pSheet);

	return CShtSheetAttribute_GetFoldMode(pAttribute);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetMediaType
// Description   : 指示された制御対象シートの紙質を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : ECdiFinMediaTypeの定義を参照
///////////////////////////////////////////////////////////////////
enum ECdiFinMediaType SRVC_GetMediaType(UC sheetNo)
{
	const CShtSheet *pSheet;
	const CShtSheetAttribute *pAttribute;

	pSheet = CShtManager_GetSheet(sheetNo);
	pAttribute = CShtSheet_GetAttribute(pSheet);

	return CShtSheetAttribute_GetMediaType(pAttribute);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetMediaWeight
// Description   : 指示された制御対象シートの坪量を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : 0～1023[×1gsm]
///////////////////////////////////////////////////////////////////
US SRVC_GetMediaWeight(UC sheetNo)
{
	const CShtSheet *pSheet;
	const CShtSheetAttribute *pAttribute;

	pSheet = CShtManager_GetSheet(sheetNo);
	pAttribute = CShtSheet_GetAttribute(pSheet);

	return CShtSheetAttribute_GetMediaWeight(pAttribute);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetEnvelope
// Description   : 指示された制御対象シートが封筒かどうかの情報を公
//               : 開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : eCdiFin_No/eCdiFin_Yes
///////////////////////////////////////////////////////////////////
enum ECdiFinYesNo SRVC_GetEnvelope(UC sheetNo)
{
	const CShtSheet *pSheet;
	const CShtSheetAttribute *pAttribute;

	pSheet = CShtManager_GetSheet(sheetNo);
	pAttribute = CShtSheet_GetAttribute(pSheet);

	return CShtSheetAttribute_GetEnvelope(pAttribute);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetTabbedPaper
// Description   : 指示された制御対象シートがタブ付きかどうかの情報
//               : を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : eCdiFin_No/eCdiFin_Yes
///////////////////////////////////////////////////////////////////
enum ECdiFinYesNo SRVC_GetTabbedPaper(UC sheetNo)
{
	const CShtSheet *pSheet;
	const CShtSheetAttribute *pAttribute;

	pSheet = CShtManager_GetSheet(sheetNo);
	pAttribute = CShtSheet_GetAttribute(pSheet);

	return CShtSheetAttribute_GetTabbedPaper(pAttribute);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetReusedPaper
// Description   : 指示された制御対象シートが裏紙かどうかの情報を公
//               : 開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : eCdiFin_No/eCdiFin_Yes
///////////////////////////////////////////////////////////////////
enum ECdiFinYesNo SRVC_GetReusedPaper(UC sheetNo)
{
	const CShtSheet *pSheet;
	const CShtSheetAttribute *pAttribute;

	pSheet = CShtManager_GetSheet(sheetNo);
	pAttribute = CShtSheet_GetAttribute(pSheet);

	return CShtSheetAttribute_GetReusedPaper(pAttribute);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetPrepunchedPaper
// Description   : 指示された制御対象シートが穴あきかどうかの情報を
//               : 公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : eCdiFin_No/eCdiFin_Yes
///////////////////////////////////////////////////////////////////
enum ECdiFinYesNo SRVC_GetPrepunchedPaper(UC sheetNo)
{
	const CShtSheet *pSheet;
	const CShtSheetAttribute *pAttribute;

	pSheet = CShtManager_GetSheet(sheetNo);
	pAttribute = CShtSheet_GetAttribute(pSheet);

	return CShtSheetAttribute_GetPrepunchedMaterial(pAttribute);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetProcessSize
// Description   : 指示された制御対象シートの送り長を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : 0～65535[×0.1mm]
///////////////////////////////////////////////////////////////////
US SRVC_GetProcessSize(UC sheetNo)
{
	const CShtSheet *pSheet;
	const CShtSheetAttribute *pAttribute;

	pSheet = CShtManager_GetSheet(sheetNo);
	pAttribute = CShtSheet_GetAttribute(pSheet);

	return CShtSheetAttribute_GetProcessSize(pAttribute);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetCrossProcessSize
// Description   : 指示された制御対象シートの用紙幅を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : 0～65535[×0.1mm]
///////////////////////////////////////////////////////////////////
US SRVC_GetCrossProcessSize(UC sheetNo)
{
	const CShtSheet *pSheet;
	const CShtSheetAttribute *pAttribute;

	pSheet = CShtManager_GetSheet(sheetNo);
	pAttribute = CShtSheet_GetAttribute(pSheet);

	return CShtSheetAttribute_GetCrossProcSize(pAttribute);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetCurl
// Description   : 指示された制御対象シートのカール方向を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : eCdiFinShtCurl_ProcShtUp/
//               : eCdiFinShtCurl_ProcShtDown
///////////////////////////////////////////////////////////////////
enum ECdiFinSheetCurl SRVC_GetCurl(UC sheetNo)
{
	const CShtSheet *pSheet;
	const CShtSheetAttribute *pAttribute;

	pSheet = CShtManager_GetSheet(sheetNo);
	pAttribute = CShtSheet_GetAttribute(pSheet);

	return CShtSheetAttribute_GetCurl(pAttribute);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetCurlQuantity
// Description   : 指示された制御対象シートのカール量を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : 0～255[×1mm]
///////////////////////////////////////////////////////////////////
UC SRVC_GetCurlQuantity(UC sheetNo)
{
	const CShtSheet *pSheet;
	const CShtSheetAttribute *pAttribute;

	pSheet = CShtManager_GetSheet(sheetNo);
	pAttribute = CShtSheet_GetAttribute(pSheet);

	return CShtSheetAttribute_GetCurlQuantity(pAttribute);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetExitInitialSpeed
// Description   : 指示された制御対象シートの用紙搬入開始時速度を公
//               : 開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : 1～65535[×0.1mm/s]
///////////////////////////////////////////////////////////////////
US SRVC_GetExitInitialSpeed(UC sheetNo)
{
	const CShtSheet *pSheet;
	const CShtSheetAttribute *pAttribute;

	pSheet = CShtManager_GetSheet(sheetNo);
	pAttribute = CShtSheet_GetAttribute(pSheet);

	return CShtSheetAttribute_GetInitialSpeed(pAttribute);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetExitSpeedChangeTimes
// Description   : 指示された制御対象シートの用紙搬入時速度変動回数
//               : を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : 0～5[×1回]
///////////////////////////////////////////////////////////////////
UC SRVC_GetExitSpeedChangeTimes(UC sheetNo)
{
	const CShtSheet *pSheet;
	const CShtSheetAttribute *pAttribute;

	pSheet = CShtManager_GetSheet(sheetNo);
	pAttribute = CShtSheet_GetAttribute(pSheet);

	return CShtSheetAttribute_GetSpeedChangeTimes(pAttribute);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetExitSpeedChangeTimming
// Description   : 指示された制御対象シートの用紙搬入時速度変動タイ
//               : ミングを公開する。
// Parameter     : sheetNo (Sheet No.)
//               : times (Times of Change Speed)
// Return        : 1～65535[×1ms]
///////////////////////////////////////////////////////////////////
US SRVC_GetExitSpeedChangeTimming(UC sheetNo, UC times)
{
	const CShtSheet *pSheet;
	const CShtSheetAttribute *pAttribute;

	pSheet = CShtManager_GetSheet(sheetNo);
	pAttribute = CShtSheet_GetAttribute(pSheet);

	return CShtSheetAttribute_GetChangeSpeedTiming(pAttribute, times);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetExitChangeSpeed
// Description   : 指示された制御対象シートの用紙搬入時速度変動時の
//               : 変動速度を公開する。
// Parameter     : sheetNo (Sheet No.)
//               : times (Times of Change Speed)
// Return        : 1～65535[×0.1mm/s]
///////////////////////////////////////////////////////////////////
US SRVC_GetExitChangeSpeed(UC sheetNo, UC times)
{
	const CShtSheet *pSheet;
	const CShtSheetAttribute *pAttribute;

	pSheet = CShtManager_GetSheet(sheetNo);
	pAttribute = CShtSheet_GetAttribute(pSheet);

	return CShtSheetAttribute_GetChangeSpeed(pAttribute, times);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetExitSheetInterval
// Description   : 指示された制御対象シートとその後続紙間の搬送間隔
// Parameter     : sheetNo (Sheet No.)
// Return        : 0～65534[×1ms]/65535
///////////////////////////////////////////////////////////////////
US SRVC_GetExitSheetInterval(UC sheetNo)
{
	const CShtSheet *pSheet;
	const CShtSheetAttribute *pAttribute;

	pSheet = CShtManager_GetSheet(sheetNo);
	pAttribute = CShtSheet_GetAttribute(pSheet);

	return CShtSheetAttribute_GetExitSheetInterval(pAttribute);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetSheetCondition
// Description   : 対象シートのコンディション情報を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : EShtSheetConditionの定義を参照
///////////////////////////////////////////////////////////////////
EShtSheetCondition SRVC_GetSheetCondition(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetCondition(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_CheckSheetConditionNormal
// Description   : 対象シートのコンディションが＠＠＠であるかを公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : 
///////////////////////////////////////////////////////////////////
bool SRVC_CheckSheetConditionNormal(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_CheckConditionNormal(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_CheckSheetConditionProcess
// Description   : 対象シートのコンディションが＠＠＠であるかを公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : 
///////////////////////////////////////////////////////////////////
bool SRVC_CheckSheetConditionProcess(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_CheckConditionProcess(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_CheckSheetConditionDelivery
// Description   : 対象シートのコンディションが＠＠＠であるかを公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : 
///////////////////////////////////////////////////////////////////
bool SRVC_CheckSheetConditionDelivery(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_CheckConditionDelivery(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_CheckSheetConditionNormalDelivery
// Description   : 対象シートのコンディションが＠＠＠であるかを公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : 
///////////////////////////////////////////////////////////////////
bool SRVC_CheckSheetConditionNormalDelivery(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_CheckConditionNormalDelivery(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_CheckSheetConditionAbnormalDelivery
// Description   : 対象シートのコンディションが＠＠＠であるかを公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : 
///////////////////////////////////////////////////////////////////
bool SRVC_CheckSheetConditionAbnormalDelivery(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_CheckConditionAbnormalDelivery(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_CheckSheetConditionWaitEject
// Description   : 対象シートのコンディションが＠＠＠であるかを公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : 
///////////////////////////////////////////////////////////////////
bool SRVC_CheckSheetConditionWaitEject(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_CheckConditionWaitEject(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_CheckSheetConditionWaitFinish
// Description   : 対象シートのコンディションが＠＠＠であるかを公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : 
///////////////////////////////////////////////////////////////////
bool SRVC_CheckSheetConditionWaitFinish(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_CheckConditionWaitFinish(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_CheckSheetConditionStop
// Description   : 対象シートのコンディションが制御を停止すべきコン
//               : ディションであるかを公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : 
///////////////////////////////////////////////////////////////////
bool SRVC_CheckSheetConditionStop(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_CheckConditionStop(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetDestinationFinish
// Description   : システムモジュールが判断して決定した対象シートの
//               : 排紙先(パージ対象となった際はパージトレイを示す)
//               : を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : ECdiFinOutDestの定義を参照
///////////////////////////////////////////////////////////////////
enum ECdiFinOutDest SRVC_GetDestinationFinish(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetOutputDestFinish(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetBuffer
// Description   : 制御対象シートがバッファ制御に関わるかどうか、な
//               : らびにバッファ制御におけるどの役割のシートである
//               : かを公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : SHT_BUFFER_NOT/SHT_BUFFER_TOP/SHT_BUFFER_FOLLOW
///////////////////////////////////////////////////////////////////
EShtBuffer SRVC_GetBuffer(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetBuffer(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetBufferFeedWaitTime
// Description   : バッファキャンセルフィードウェイトタイムを公開す
//               : る。
// Parameter     : sheetNo (Sheet No.)
// Return        : 0～65535[×1ms]
///////////////////////////////////////////////////////////////////
//US SRVC_GetBufferFeedWaitTime(UC sheetNo)
//{
//	const CShtSheet *pSheet;

//	pSheet = CShtManager_GetSheet(sheetNo);

//	return CShtSheet_GetBufferFeedWaitTime(pSheet);
//}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetEject
// Description   : 制御対象シートを最終収容シートとしてコンパイルト
//               : レイからの排紙を実施するかどうかを公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : SHT_EJECT_NOT/SHT_EJECT_EXEC
///////////////////////////////////////////////////////////////////
EShtEject SRVC_GetEject(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetEject(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetSetTop
// Description   : システムモジュールが判断した対象シートがセット先
//               : 頭シートかどうかの情報を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : SHT_SET_TOP_NOT/SHT_SET_TOP
///////////////////////////////////////////////////////////////////
EShtSetTop SRVC_GetSetTop(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetSetTop(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetSetEnd
// Description   : 対象シートがセット最終シートかどうかの情報を公開
//               : する。
// Parameter     : sheetNo (Sheet No.)
// Return        : SHT_SET_END_NOT/SHT_SET_END
///////////////////////////////////////////////////////////////////
EShtSetEnd SRVC_GetSetEnd(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetSetEnd(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetSizeClass
// Description   : 対象シートがシートサイズ区分のどれに当たるかを公
//               : 開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : SHT_SIZE_CLASS_SMALL/SHT_SIZE_CLASS_LARGE
///////////////////////////////////////////////////////////////////
EShtSizeClass SRVC_GetSizeClass(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetSizeClass(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetProcessSizeClass
// Description   : 対象シートの用紙送り長がシートサイズ区分のどれに
//               : 当たるかを公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : SHT_SIZE_CLASS_SMALL/SHT_SIZE_CLASS_MIDDLE /
//               : SHT_SIZE_CLASS_LARGE
///////////////////////////////////////////////////////////////////
EShtSizeClass SRVC_GetProcessSizeClass(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetProcSizeClass(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetCrossProcessSizeClass
// Description   : 対象シートの用紙幅がシートサイズ区分のどれに当た
//               : るかを公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : SHT_SIZE_CLASS_SMALL/SHT_SIZE_CLASS_MIDDLE /
//               : SHT_SIZE_CLASS_LARGE
///////////////////////////////////////////////////////////////////
EShtSizeClass SRVC_GetCrossProcessSizeClass(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetCrossProcSizeClass(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetFinisherSpeed
// Description   : 対象シートを搬送する際の基本スピードを公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : 1～65535[×0.1mm/s]
///////////////////////////////////////////////////////////////////
US SRVC_GetFinisherSpeed(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtManager_ConvFinisherSpeed(CShtSheet_GetFinisherSpeedID(pSheet));
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetOffsetFinish
// Description   : フィニッシャーで決定した対象シートのオフセット位
//               : 置を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : EShtOffsetFinishの定義を参照
///////////////////////////////////////////////////////////////////
EShtOffsetFinish SRVC_GetOffsetFinish(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetOffsetFinish(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetCompileCount
// Description   : 対象のシートがコンパイルトレイに積載されている用
//               : 紙束内の何枚目に積載されたシートであるかを公開す
//               : る。
// Parameter     : sheetNo (Sheet No.)
// Return        : 1～255[×1枚目]
///////////////////////////////////////////////////////////////////
UC SRVC_GetCompileCount(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetCompileCount(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetWeightedCompileCount
// Description   : コンパイルトレイに積載されたシート束が、重みづけ
//               : 積載枚数において、対象のシートをコンパイルトレイ
//               : に積載した際に何枚の用紙束として扱われるかを公開
//               : する。
// Parameter     : sheetNo (Sheet No.)
// Return        : 1～65535[×0.1枚]
///////////////////////////////////////////////////////////////////
US SRVC_GetWeightedCompileCount(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetWeightedCompileCount(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetDevidedSet
// Description   : 対象のシートを含むセットが分割排紙対象であるかど
//               : うかを公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : SHT_SET_UNDEVIDED/SHT_SET_DEVIDED
///////////////////////////////////////////////////////////////////
EShtDevidedSet SRVC_GetDevidedSet(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetDevidedSet(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetMaxMediaWeightCompile
// Description   : 対象シートを最終収容用紙とするコンパイラへの収容
//               : 用紙束内のシートの中で、最も重い坪量を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : 0 = Unknown
//               : 1～1023[×1gsm]
//★ V3.4.0  Merge　★
///////////////////////////////////////////////////////////////////
US SRVC_GetMaxMediaWeightCompile(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetMaxMediaWeightOfCompile(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetSizeMixSet
// Description   : 対象のシートを含むセットがサイズミックスしている
//               : かどうかを公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : SHT_SIZE_UNMIX/SHT_SIZE_MIX
///////////////////////////////////////////////////////////////////
EShtSizeMix SRVC_GetSizeMixSet(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetSizeMix(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetProcessSizeMixSet
// Description   : 対象のシートを含むセットが用紙送り長方向のサイズ
//               : ミックスをしているかどうかを公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : SHT_SIZE_UNMIX/SHT_SIZE_MIX
///////////////////////////////////////////////////////////////////
EShtSizeMix SRVC_GetProcessSizeMixSet(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetProcSizeMix(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetCrossProcessSizeMixSet
// Description   : 対象のシートを含むセットが用紙幅方向のサイズミッ
//               : クスをしているかどうかを公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : SHT_SIZE_UNMIX/SHT_SIZE_MIX
///////////////////////////////////////////////////////////////////
EShtSizeMix SRVC_GetCrossProcessSizeMixSet(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetCrossProcSizeMix(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetMaxProcessSizeCompile
// Description   : 対象のシートを含むコンパイル用紙束内の最大用紙送
//               : り長を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : 0～65535[×0.1mm]
///////////////////////////////////////////////////////////////////
US SRVC_GetMaxProcessSizeCompile(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetMaxProcSizeOfCompile(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetMaxCrossProcessSizeCompile
// Description   : 対象のシートを含むコンパイル用紙束内の最大用紙幅
//               : を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : 0～65535[×0.1mm]
///////////////////////////////////////////////////////////////////
US SRVC_GetMaxCrossProcessSizeCompile(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetMaxCrossProcSizeOfCompile(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetMinProcessSizeCompile
// Description   : 対象のシートを含むコンパイル用紙束内の最小用紙送
//               : り長を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : 0～65535[×0.1mm]
///////////////////////////////////////////////////////////////////
US SRVC_GetMinProcessSizeCompile(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetMinProcSizeOfCompile(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetMinCrossProcessSizeCompile
// Description   : 対象のシートを含むコンパイル用紙束内の最小用紙幅
//               : を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : 0～65535[×0.1mm]
///////////////////////////////////////////////////////////////////
US SRVC_GetMinCrossProcessSizeCompile(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetMinCrossProcSizeOfCompile(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetFoldInclusionCompile
// Description   : 対象のシートを含むコンパイル用紙束内に折りシート
//               : が含まれているかどうかを公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : SHT_FOLD_NOT_INCLUDE/SHT_FOLD_INCLUDE
///////////////////////////////////////////////////////////////////
//EShtFoldInclusion SRVC_GetFoldInclusionCompile(UC sheetNo)
//{
//	const CShtSheet *pSheet;

//	pSheet = CShtManager_GetSheet(sheetNo);

//	return CShtSheet_GetInclusionFoldInCompile(pSheet);
//}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetFoldInclusionSet
// Description   : 対象のシートを含むセット内に折りシートが含まれて
//               : いるかどうかを公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : SHT_FOLD_NOT_INCLUDE/SHT_FOLD_INCLUDE
///////////////////////////////////////////////////////////////////
//EShtFoldInclusion SRVC_GetFoldInclusionSet(UC sheetNo)
//{
//	const CShtSheet *pSheet;

//	pSheet = CShtManager_GetSheet(sheetNo);

//	return CShtSheet_GetInclusionFoldInSet(pSheet);
//}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_CheckSheetEventOccurence
// Description   : 対象のシートのシートイベントの発生状況を公開する
//               : 。
// Parameter     : sheetEvent (Sheet Event ID)
//               : sheetNo (Sheet No.)
// Return        : true = 発生済み/false = 未発生
///////////////////////////////////////////////////////////////////
bool SRVC_CheckSheetEventOccurence(EShtSheetEventID sheetEvent, UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_CheckEventOccurence(pSheet, sheetEvent);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetNextLocation
// Description   : 対象のシートのエッジにおいて、次に発行予定である
//               : シートロケーションを公開する。
// Parameter     : sheetNo (Sheet No.)
//               : edge (シートエッジ識別子)
// Return        : EShtSheetLocationIDの定義を参照
///////////////////////////////////////////////////////////////////
EShtSheetLocationID SRVC_GetNextLocation(UC sheetNo, EShtSheetEdge edge)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetNextLocation(pSheet, edge);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetLastLocation
// Description   : 対象のシートのエッジにおいて、最後に(直近で)発行
//               : したシートロケーションを公開する。
// Parameter     : sheetNo (Sheet No.)
//               : edge (シートエッジ識別子)
// Return        : EShtSheetLocationIDの定義を参照
///////////////////////////////////////////////////////////////////
EShtSheetLocationID SRVC_GetLastLocation(UC sheetNo, EShtSheetEdge edge)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetLastLocation(pSheet, edge);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_CheckSheetLocationArrival
// Description   : 対象のシートのエッジにおいて、対象のシートロケー
//               : ションが発行済みであるかどうかを公開する。
// Parameter     : location (Sheet Location ID)
//               : sheetNo (Sheet No.)
//               : edge (シートエッジ識別子)
// Return        : true = 発行済み/false = 未発行
///////////////////////////////////////////////////////////////////
bool SRVC_CheckSheetLocationArrival(EShtSheetLocationID location, UC sheetNo, EShtSheetEdge edge)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_CheckLocationArrival(pSheet, location, edge);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetProcessStartSheet
// Description   : 現在実施中のフィニッシングプロセスにおいて、用紙
//               : 管理上で最初に構築したシート情報のシート識別番号
//               : を公開する。
// Parameter     : ‐
// Return        : 0 ～ SHEETNO_NUM
///////////////////////////////////////////////////////////////////
UC SRVC_GetProcessStartSheet(void)
{
	return CShtManager_GetProcessStartSheet();
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetTopRequestSheet
// Description   : シートリクエストキュー内のトップに当たるシートの
//               : 識別番号を公開する。
// Parameter     : ‐
// Return        : 0 ～ SHEETNO_NUM
///////////////////////////////////////////////////////////////////
UC SRVC_GetTopRequestSheet(void)
{
	return CShtManager_GetTopRequestSheet();
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetBottomRequestSheet
// Description   : シートリクエストキュー内のボトムに当たるシートの
//               : 識別番号を公開する。
// Parameter     : ‐
// Return        : 0 ～ SHEETNO_NUM
///////////////////////////////////////////////////////////////////
UC SRVC_GetBottomRequestSheet(void)
{
	return CShtManager_GetBottomRequestSheet();
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetPrevRequestSheet
// Description   : シートリクエストキューにおいてプレビアスシートに
//               : 当たるシートの識別番号を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : 0 ～ SHEETNO_NUM
///////////////////////////////////////////////////////////////////
UC SRVC_GetPrevRequestSheet(UC sheetNo)
{
	return CShtManager_GetPrevRequestSheet(sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetNextRequestSheet
// Description   : 対象シートの、シートリクエストキューにおいてネク
//               : ストシートに当たるシートの識別番号を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : 0 ～ SHEETNO_NUM
///////////////////////////////////////////////////////////////////
UC SRVC_GetNextRequestSheet(UC sheetNo)
{
	return CShtManager_GetNextRequestSheet(sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetTopExitSheet
// Description   : シートイグジットキュー内のトップに当たるシートの
//               : 識別番号を公開する。
// Parameter     : ‐
// Return        : 0 ～ SHEETNO_NUM
///////////////////////////////////////////////////////////////////
UC SRVC_GetTopExitSheet(void)
{
	return CShtManager_GetTopExitSheet();
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetBottomExitSheet
// Description   : シートイグジットキュー内のボトムに当たるシートの
//               : 識別番号を公開する。
// Parameter     : ‐
// Return        : 0 ～ SHEETNO_NUM
///////////////////////////////////////////////////////////////////
UC SRVC_GetBottomExitSheet(void)
{
	return CShtManager_GetBottomExitSheet();
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetPrevExitSheet
// Description   : 対象シートの、シートイグジットキュー上においてプ
//               : レビアスシートに当たるシートの識別番号を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : 0 ～ SHEETNO_NUM
///////////////////////////////////////////////////////////////////
UC SRVC_GetPrevExitSheet(UC sheetNo)
{
	return CShtManager_GetPrevExitSheet(sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetNextExitSheet
// Description   : 対象シートの、シートイグジットキュー上においてネ
//               : クストシートに当たるシートの識別番号を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : 0 ～ SHEETNO_NUM
///////////////////////////////////////////////////////////////////
UC SRVC_GetNextExitSheet(UC sheetNo)
{
	return CShtManager_GetNextExitSheet(sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetBufferPairSheet
// Description   : バッファ制御において対象シートとペアになる(重ね合
//               : わせ対象となる)シートの識別番号を公開する。
// Parameter     : sheetNo (Sheet No.)
// Return        : 0 ～ SHEETNO_NUM
///////////////////////////////////////////////////////////////////
UC SRVC_GetBufferPairSheet(UC sheetNo)
{
	return CShtManager_GetBufferPairSheet(sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_CheckUndeliveredSheetExist
// Description   : シートリクエストキューとシートイグジットキュー上
//               : に未デリバードのシートがあるかどうかの情報を公開
//               : する。
// Parameter     : outDest (Output Destination ID)
// Return        : true = 未デリバードのシートがある/
//               : false = 未デリバードのシートがない
///////////////////////////////////////////////////////////////////
bool SRVC_CheckUndeliveredSheetExist(enum ECdiFinOutDest outDest)
{
	return CShtManager_CheckUndeliveredSheetExist(outDest);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_CheckUnejectedSheetExist
// Description   : シートイグジットキュー上のデリバード済みシートの
//               : 中に、コンパイルトレイから未排出であるシートがあ
//               : るかどうかの情報を公開する。
// Parameter     : outDest (Output Destination ID)
// Return        : true = 未排出のシートがある/
//               : false = 未排出のシートがない
///////////////////////////////////////////////////////////////////
bool SRVC_CheckUnejectedSheetExist(enum ECdiFinOutDest outDest)
{
	return CShtManager_CheckUnejectedSheetExist(outDest);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_CheckUnfinishedSheetExist  :  V3.6.92
// Description   : 未フィニッシュセット有無確認処理
// Parameter     : outDest (Output Destination ID)
// Return        : 有無確認結果
//               :  true = 未フィニッシュセットあり
//               :  false = 未フィニッシュセットなし
///////////////////////////////////////////////////////////////////
bool SRVC_CheckUnfinishedSheetExist(enum ECdiFinOutDest outDest)
{
	return CShtManager_CheckUnfinishedSetExist(outDest);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_CheckUnfinishedSheetExist  :  V3.1.4 
// Description   : シートイグジットキュー上のデリバード済みシートの
//               : 中に、コンパイルトレイから未排出であるシートがあ
//               : るかどうかの情報を公開する。
// Parameter     : outDest (Output Destination ID)
// Return        : true = 未排出のシートがある/
//               : false = 未排出のシートがない
///////////////////////////////////////////////////////////////////
bool SRVC_CheckUnfinishedSheetExist_2(enum ECdiFinOutDest outDest)
{
	return CShtManager_CheckUnfinishedSetExist_2(outDest);
}



///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetNextArriveSheet
// Description   : 指定エッジにおいて対象のパスセンサーに次に到達す
//               : る予定のシートの識別番号を公開する。
// Parameter     : pathSnrID (Path Sensor ID)
//               : edge (シートエッジ識別子)
// Return        : 0 ～ SHEETNO_NUM
///////////////////////////////////////////////////////////////////
UC SRVC_GetNextArriveSheet(EShtPathSensorID pathSnrID, EShtSheetEdge edge)
{
	return CShtManager_GetNextArriveSheet(pathSnrID, edge);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_CheckPathSensorPlan
// Description   : 指定のパスセンサに、指定のシートが通過する予定が
//               : あるかどうかの情報を提供する。
// Parameter     : pathSnrID (Path Sensor ID)
//               : edge (シートエッジ識別子)
//               : sheetNo (Sheet No.)
// Return        : true = 通過予定あり/false = 通過予定なし
///////////////////////////////////////////////////////////////////
bool SRVC_CheckPathSensorPlan(EShtPathSensorID pathSnrID, UC sheetNo)
{
	return CShtManager_CheckPathSensorPlan(pathSnrID, sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_CheckPathSensorArrival
// Description   : 指定のパスセンサに、指定のシートのエッジが通過済
//               : みかどうかを提供する。
// Parameter     : pathSnrID (Path Sensor ID)
//               : edge (シートエッジ識別子)
//               : sheetNo (Sheet No.)
// Return        : true = 通過済み/false = 未通過
///////////////////////////////////////////////////////////////////
bool SRVC_CheckPathSensorArrival(EShtPathSensorID pathSnrID, UC sheetNo, EShtSheetEdge edge)
{
	return CShtManager_CheckPathSensorArrival(pathSnrID, sheetNo, edge);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_SetPitchSkip
// Description   : Device都合により、PitchSkip要求をする時の設定関数
// Parameter     : bOnOff :  On(PitchSkip ）/ Off（PitchSkipしない）
///////////////////////////////////////////////////////////////////
void SRVC_SetPitchSkip(UC  bOnOff)
{
	CShtManager_SetPitchSkip(bOnOff);
	return;	
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetPitchSkip
// Description   : PitchSkip状態 ： On(PitchSkip中）/ Off（PitchSkipしない）
// Return    :  On(PitchSkip ）/ Off（PitchSkipしない）
///////////////////////////////////////////////////////////////////
UC SRVC_GetPitchSkip(void)
{
	return  CShtManager_GetPitchSkip();
}

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

// Base SW Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// 14.12.05	| Yuichi Hoshino	| Created this file
// 14.12.05	| Yuichi Hoshino	| Release V0
// 14.12.29	| Yuichi Hoshino	| Trouble Action [ 速度変更タイミング取得サービス戻り値不正 ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
// 15.06.19	| Yuichi Hoshino	| Design Improvement [ イグジットマックススピード参照サービス追加 ]
// 15.06.19	| Yuichi Hoshino	| Design Improvement [ マキシマムメディアウェイト(コンパイル)参照サービス追加 ]
// 15.06.19	| Yuichi Hoshino	| Design Improvement [ レイトディテクトフィルタタイム参照サービス追加 ]
//

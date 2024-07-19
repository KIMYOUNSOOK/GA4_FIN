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
// Description   : �w�����ꂽ����ΏۃV�[�g�̔r����w������J����B
// Parameter     : sheetNo (Sheet No.)
// Return        : ECdiFinOutDest�̒�`���Q��
///////////////////////////////////////////////////////////////////
enum ECdiFinOutDest SRVC_GetOutputDestination(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetOutputDestination(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetBinSelection
// Description   : �w�����ꂽ����ΏۃV�[�g�̔r����r���w������J��
//               : ��B
// Parameter     : sheetNo (Sheet No.)
// Return        : 1�`255
///////////////////////////////////////////////////////////////////
UC SRVC_GetBinSelection(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetBinSelection(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetSetEndAction
// Description   : �w�����ꂽ����ΏۃV�[�g(�Z�b�g)�̃Z�b�g�������@
//               : �����J����B
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
// Description   : �w�����ꂽ����ΏۃV�[�g�̔����V�[�g��ԏ�����
//               : �J����B
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
// Description   : �w�����ꂽ����ΏۃV�[�g�̃Z�b�g�P�ʂƂ��ẴA�N
//               : �V�����������J����B
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
// Description   : �w�����ꂽ����ΏۃV�[�g(���܂ރZ�b�g)�̃I�t�Z�b
//               : �g�@�\�̐����ʂ����J����B
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
// Description   : �w�����ꂽ����ΏۃV�[�g(���܂ރZ�b�g)�̃I�t�Z�b
//               : �g�ʒu�������J����B
// Parameter     : sheetNo (Sheet No.)
// Return        : ECdiFinOffsetPos�̒�`���Q��
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
// Description   : �w�����ꂽ����ΏۃV�[�g(���܂ރZ�b�g)�̃X�e�[�v
//               : ���@�\�̐����ʂ����J����B
// Parameter     : sheetNo (Sheet No.)
// Return        : ECdiFinStapleMode�̒�`���Q��
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
// Description   : �w�����ꂽ����ΏۃV�[�g�̃p���`�@�\�̐����ʂ�
//               : ���J����B
// Parameter     : sheetNo (Sheet No.)
// Return        : ECdiFinPunchMode�̒�`���Q��
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
// Description   : �w�����ꂽ����ΏۃV�[�g(���܂ރZ�b�g)�̐��{�@�\
//               : �̐����ʂ����J����B
// Parameter     : sheetNo (Sheet No.)
// Return        : ECdiFinBookMode�̒�`���Q��
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
// Description   : �w�����ꂽ����ΏۃV�[�g�̃t�H�[���h�@�\�̐����
//               : �ʂ����J����B
// Parameter     : sheetNo (Sheet No.)
// Return        : ECdiFinFoldMode�̒�`���Q��
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
// Description   : �w�����ꂽ����ΏۃV�[�g�̎��������J����B
// Parameter     : sheetNo (Sheet No.)
// Return        : ECdiFinMediaType�̒�`���Q��
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
// Description   : �w�����ꂽ����ΏۃV�[�g�̒ؗʂ����J����B
// Parameter     : sheetNo (Sheet No.)
// Return        : 0�`1023[�~1gsm]
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
// Description   : �w�����ꂽ����ΏۃV�[�g���������ǂ����̏�����
//               : �J����B
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
// Description   : �w�����ꂽ����ΏۃV�[�g���^�u�t�����ǂ����̏��
//               : �����J����B
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
// Description   : �w�����ꂽ����ΏۃV�[�g���������ǂ����̏�����
//               : �J����B
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
// Description   : �w�����ꂽ����ΏۃV�[�g�����������ǂ����̏���
//               : ���J����B
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
// Description   : �w�����ꂽ����ΏۃV�[�g�̑��蒷�����J����B
// Parameter     : sheetNo (Sheet No.)
// Return        : 0�`65535[�~0.1mm]
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
// Description   : �w�����ꂽ����ΏۃV�[�g�̗p���������J����B
// Parameter     : sheetNo (Sheet No.)
// Return        : 0�`65535[�~0.1mm]
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
// Description   : �w�����ꂽ����ΏۃV�[�g�̃J�[�����������J����B
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
// Description   : �w�����ꂽ����ΏۃV�[�g�̃J�[���ʂ����J����B
// Parameter     : sheetNo (Sheet No.)
// Return        : 0�`255[�~1mm]
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
// Description   : �w�����ꂽ����ΏۃV�[�g�̗p�������J�n�����x����
//               : �J����B
// Parameter     : sheetNo (Sheet No.)
// Return        : 1�`65535[�~0.1mm/s]
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
// Description   : �w�����ꂽ����ΏۃV�[�g�̗p�����������x�ϓ���
//               : �����J����B
// Parameter     : sheetNo (Sheet No.)
// Return        : 0�`5[�~1��]
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
// Description   : �w�����ꂽ����ΏۃV�[�g�̗p�����������x�ϓ��^�C
//               : �~���O�����J����B
// Parameter     : sheetNo (Sheet No.)
//               : times (Times of Change Speed)
// Return        : 1�`65535[�~1ms]
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
// Description   : �w�����ꂽ����ΏۃV�[�g�̗p�����������x�ϓ�����
//               : �ϓ����x�����J����B
// Parameter     : sheetNo (Sheet No.)
//               : times (Times of Change Speed)
// Return        : 1�`65535[�~0.1mm/s]
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
// Description   : �w�����ꂽ����ΏۃV�[�g�Ƃ��̌㑱���Ԃ̔����Ԋu
// Parameter     : sheetNo (Sheet No.)
// Return        : 0�`65534[�~1ms]/65535
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
// Description   : �ΏۃV�[�g�̃R���f�B�V�����������J����B
// Parameter     : sheetNo (Sheet No.)
// Return        : EShtSheetCondition�̒�`���Q��
///////////////////////////////////////////////////////////////////
EShtSheetCondition SRVC_GetSheetCondition(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetCondition(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_CheckSheetConditionNormal
// Description   : �ΏۃV�[�g�̃R���f�B�V�������������ł��邩�����J����B
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
// Description   : �ΏۃV�[�g�̃R���f�B�V�������������ł��邩�����J����B
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
// Description   : �ΏۃV�[�g�̃R���f�B�V�������������ł��邩�����J����B
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
// Description   : �ΏۃV�[�g�̃R���f�B�V�������������ł��邩�����J����B
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
// Description   : �ΏۃV�[�g�̃R���f�B�V�������������ł��邩�����J����B
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
// Description   : �ΏۃV�[�g�̃R���f�B�V�������������ł��邩�����J����B
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
// Description   : �ΏۃV�[�g�̃R���f�B�V�������������ł��邩�����J����B
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
// Description   : �ΏۃV�[�g�̃R���f�B�V������������~���ׂ��R��
//               : �f�B�V�����ł��邩�����J����B
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
// Description   : �V�X�e�����W���[�������f���Č��肵���ΏۃV�[�g��
//               : �r����(�p�[�W�ΏۂƂȂ����ۂ̓p�[�W�g���C������)
//               : �����J����B
// Parameter     : sheetNo (Sheet No.)
// Return        : ECdiFinOutDest�̒�`���Q��
///////////////////////////////////////////////////////////////////
enum ECdiFinOutDest SRVC_GetDestinationFinish(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetOutputDestFinish(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetBuffer
// Description   : ����ΏۃV�[�g���o�b�t�@����Ɋւ�邩�ǂ����A��
//               : ��тɃo�b�t�@����ɂ�����ǂ̖����̃V�[�g�ł���
//               : �������J����B
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
// Description   : �o�b�t�@�L�����Z���t�B�[�h�E�F�C�g�^�C�������J��
//               : ��B
// Parameter     : sheetNo (Sheet No.)
// Return        : 0�`65535[�~1ms]
///////////////////////////////////////////////////////////////////
//US SRVC_GetBufferFeedWaitTime(UC sheetNo)
//{
//	const CShtSheet *pSheet;

//	pSheet = CShtManager_GetSheet(sheetNo);

//	return CShtSheet_GetBufferFeedWaitTime(pSheet);
//}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetEject
// Description   : ����ΏۃV�[�g���ŏI���e�V�[�g�Ƃ��ăR���p�C���g
//               : ���C����̔r�������{���邩�ǂ��������J����B
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
// Description   : �V�X�e�����W���[�������f�����ΏۃV�[�g���Z�b�g��
//               : ���V�[�g���ǂ����̏������J����B
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
// Description   : �ΏۃV�[�g���Z�b�g�ŏI�V�[�g���ǂ����̏������J
//               : ����B
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
// Description   : �ΏۃV�[�g���V�[�g�T�C�Y�敪�̂ǂ�ɓ����邩����
//               : �J����B
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
// Description   : �ΏۃV�[�g�̗p�����蒷���V�[�g�T�C�Y�敪�̂ǂ��
//               : �����邩�����J����B
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
// Description   : �ΏۃV�[�g�̗p�������V�[�g�T�C�Y�敪�̂ǂ�ɓ���
//               : �邩�����J����B
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
// Description   : �ΏۃV�[�g���������ۂ̊�{�X�s�[�h�����J����B
// Parameter     : sheetNo (Sheet No.)
// Return        : 1�`65535[�~0.1mm/s]
///////////////////////////////////////////////////////////////////
US SRVC_GetFinisherSpeed(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtManager_ConvFinisherSpeed(CShtSheet_GetFinisherSpeedID(pSheet));
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetOffsetFinish
// Description   : �t�B�j�b�V���[�Ō��肵���ΏۃV�[�g�̃I�t�Z�b�g��
//               : �u�����J����B
// Parameter     : sheetNo (Sheet No.)
// Return        : EShtOffsetFinish�̒�`���Q��
///////////////////////////////////////////////////////////////////
EShtOffsetFinish SRVC_GetOffsetFinish(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetOffsetFinish(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetCompileCount
// Description   : �Ώۂ̃V�[�g���R���p�C���g���C�ɐύڂ���Ă���p
//               : �������̉����ڂɐύڂ��ꂽ�V�[�g�ł��邩�����J��
//               : ��B
// Parameter     : sheetNo (Sheet No.)
// Return        : 1�`255[�~1����]
///////////////////////////////////////////////////////////////////
UC SRVC_GetCompileCount(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetCompileCount(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetWeightedCompileCount
// Description   : �R���p�C���g���C�ɐύڂ��ꂽ�V�[�g�����A�d�݂Â�
//               : �ύږ����ɂ����āA�Ώۂ̃V�[�g���R���p�C���g���C
//               : �ɐύڂ����ۂɉ����̗p�����Ƃ��Ĉ����邩�����J
//               : ����B
// Parameter     : sheetNo (Sheet No.)
// Return        : 1�`65535[�~0.1��]
///////////////////////////////////////////////////////////////////
US SRVC_GetWeightedCompileCount(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetWeightedCompileCount(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetDevidedSet
// Description   : �Ώۂ̃V�[�g���܂ރZ�b�g�������r���Ώۂł��邩��
//               : ���������J����B
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
// Description   : �ΏۃV�[�g���ŏI���e�p���Ƃ���R���p�C���ւ̎��e
//               : �p�������̃V�[�g�̒��ŁA�ł��d���ؗʂ����J����B
// Parameter     : sheetNo (Sheet No.)
// Return        : 0 = Unknown
//               : 1�`1023[�~1gsm]
//�� V3.4.0  Merge�@��
///////////////////////////////////////////////////////////////////
US SRVC_GetMaxMediaWeightCompile(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetMaxMediaWeightOfCompile(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetSizeMixSet
// Description   : �Ώۂ̃V�[�g���܂ރZ�b�g���T�C�Y�~�b�N�X���Ă���
//               : ���ǂ��������J����B
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
// Description   : �Ώۂ̃V�[�g���܂ރZ�b�g���p�����蒷�����̃T�C�Y
//               : �~�b�N�X�����Ă��邩�ǂ��������J����B
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
// Description   : �Ώۂ̃V�[�g���܂ރZ�b�g���p���������̃T�C�Y�~�b
//               : �N�X�����Ă��邩�ǂ��������J����B
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
// Description   : �Ώۂ̃V�[�g���܂ރR���p�C���p�������̍ő�p����
//               : �蒷�����J����B
// Parameter     : sheetNo (Sheet No.)
// Return        : 0�`65535[�~0.1mm]
///////////////////////////////////////////////////////////////////
US SRVC_GetMaxProcessSizeCompile(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetMaxProcSizeOfCompile(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetMaxCrossProcessSizeCompile
// Description   : �Ώۂ̃V�[�g���܂ރR���p�C���p�������̍ő�p����
//               : �����J����B
// Parameter     : sheetNo (Sheet No.)
// Return        : 0�`65535[�~0.1mm]
///////////////////////////////////////////////////////////////////
US SRVC_GetMaxCrossProcessSizeCompile(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetMaxCrossProcSizeOfCompile(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetMinProcessSizeCompile
// Description   : �Ώۂ̃V�[�g���܂ރR���p�C���p�������̍ŏ��p����
//               : �蒷�����J����B
// Parameter     : sheetNo (Sheet No.)
// Return        : 0�`65535[�~0.1mm]
///////////////////////////////////////////////////////////////////
US SRVC_GetMinProcessSizeCompile(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetMinProcSizeOfCompile(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetMinCrossProcessSizeCompile
// Description   : �Ώۂ̃V�[�g���܂ރR���p�C���p�������̍ŏ��p����
//               : �����J����B
// Parameter     : sheetNo (Sheet No.)
// Return        : 0�`65535[�~0.1mm]
///////////////////////////////////////////////////////////////////
US SRVC_GetMinCrossProcessSizeCompile(UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetMinCrossProcSizeOfCompile(pSheet);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetFoldInclusionCompile
// Description   : �Ώۂ̃V�[�g���܂ރR���p�C���p�������ɐ܂�V�[�g
//               : ���܂܂�Ă��邩�ǂ��������J����B
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
// Description   : �Ώۂ̃V�[�g���܂ރZ�b�g���ɐ܂�V�[�g���܂܂��
//               : ���邩�ǂ��������J����B
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
// Description   : �Ώۂ̃V�[�g�̃V�[�g�C�x���g�̔����󋵂����J����
//               : �B
// Parameter     : sheetEvent (Sheet Event ID)
//               : sheetNo (Sheet No.)
// Return        : true = �����ς�/false = ������
///////////////////////////////////////////////////////////////////
bool SRVC_CheckSheetEventOccurence(EShtSheetEventID sheetEvent, UC sheetNo)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_CheckEventOccurence(pSheet, sheetEvent);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetNextLocation
// Description   : �Ώۂ̃V�[�g�̃G�b�W�ɂ����āA���ɔ��s�\��ł���
//               : �V�[�g���P�[�V���������J����B
// Parameter     : sheetNo (Sheet No.)
//               : edge (�V�[�g�G�b�W���ʎq)
// Return        : EShtSheetLocationID�̒�`���Q��
///////////////////////////////////////////////////////////////////
EShtSheetLocationID SRVC_GetNextLocation(UC sheetNo, EShtSheetEdge edge)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetNextLocation(pSheet, edge);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetLastLocation
// Description   : �Ώۂ̃V�[�g�̃G�b�W�ɂ����āA�Ō��(���߂�)���s
//               : �����V�[�g���P�[�V���������J����B
// Parameter     : sheetNo (Sheet No.)
//               : edge (�V�[�g�G�b�W���ʎq)
// Return        : EShtSheetLocationID�̒�`���Q��
///////////////////////////////////////////////////////////////////
EShtSheetLocationID SRVC_GetLastLocation(UC sheetNo, EShtSheetEdge edge)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_GetLastLocation(pSheet, edge);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_CheckSheetLocationArrival
// Description   : �Ώۂ̃V�[�g�̃G�b�W�ɂ����āA�Ώۂ̃V�[�g���P�[
//               : �V���������s�ς݂ł��邩�ǂ��������J����B
// Parameter     : location (Sheet Location ID)
//               : sheetNo (Sheet No.)
//               : edge (�V�[�g�G�b�W���ʎq)
// Return        : true = ���s�ς�/false = �����s
///////////////////////////////////////////////////////////////////
bool SRVC_CheckSheetLocationArrival(EShtSheetLocationID location, UC sheetNo, EShtSheetEdge edge)
{
	const CShtSheet *pSheet;

	pSheet = CShtManager_GetSheet(sheetNo);

	return CShtSheet_CheckLocationArrival(pSheet, location, edge);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetProcessStartSheet
// Description   : ���ݎ��{���̃t�B�j�b�V���O�v���Z�X�ɂ����āA�p��
//               : �Ǘ���ōŏ��ɍ\�z�����V�[�g���̃V�[�g���ʔԍ�
//               : �����J����B
// Parameter     : �]
// Return        : 0 �` SHEETNO_NUM
///////////////////////////////////////////////////////////////////
UC SRVC_GetProcessStartSheet(void)
{
	return CShtManager_GetProcessStartSheet();
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetTopRequestSheet
// Description   : �V�[�g���N�G�X�g�L���[���̃g�b�v�ɓ�����V�[�g��
//               : ���ʔԍ������J����B
// Parameter     : �]
// Return        : 0 �` SHEETNO_NUM
///////////////////////////////////////////////////////////////////
UC SRVC_GetTopRequestSheet(void)
{
	return CShtManager_GetTopRequestSheet();
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetBottomRequestSheet
// Description   : �V�[�g���N�G�X�g�L���[���̃{�g���ɓ�����V�[�g��
//               : ���ʔԍ������J����B
// Parameter     : �]
// Return        : 0 �` SHEETNO_NUM
///////////////////////////////////////////////////////////////////
UC SRVC_GetBottomRequestSheet(void)
{
	return CShtManager_GetBottomRequestSheet();
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetPrevRequestSheet
// Description   : �V�[�g���N�G�X�g�L���[�ɂ����ăv���r�A�X�V�[�g��
//               : ������V�[�g�̎��ʔԍ������J����B
// Parameter     : sheetNo (Sheet No.)
// Return        : 0 �` SHEETNO_NUM
///////////////////////////////////////////////////////////////////
UC SRVC_GetPrevRequestSheet(UC sheetNo)
{
	return CShtManager_GetPrevRequestSheet(sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetNextRequestSheet
// Description   : �ΏۃV�[�g�́A�V�[�g���N�G�X�g�L���[�ɂ����ăl�N
//               : �X�g�V�[�g�ɓ�����V�[�g�̎��ʔԍ������J����B
// Parameter     : sheetNo (Sheet No.)
// Return        : 0 �` SHEETNO_NUM
///////////////////////////////////////////////////////////////////
UC SRVC_GetNextRequestSheet(UC sheetNo)
{
	return CShtManager_GetNextRequestSheet(sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetTopExitSheet
// Description   : �V�[�g�C�O�W�b�g�L���[���̃g�b�v�ɓ�����V�[�g��
//               : ���ʔԍ������J����B
// Parameter     : �]
// Return        : 0 �` SHEETNO_NUM
///////////////////////////////////////////////////////////////////
UC SRVC_GetTopExitSheet(void)
{
	return CShtManager_GetTopExitSheet();
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetBottomExitSheet
// Description   : �V�[�g�C�O�W�b�g�L���[���̃{�g���ɓ�����V�[�g��
//               : ���ʔԍ������J����B
// Parameter     : �]
// Return        : 0 �` SHEETNO_NUM
///////////////////////////////////////////////////////////////////
UC SRVC_GetBottomExitSheet(void)
{
	return CShtManager_GetBottomExitSheet();
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetPrevExitSheet
// Description   : �ΏۃV�[�g�́A�V�[�g�C�O�W�b�g�L���[��ɂ����ăv
//               : ���r�A�X�V�[�g�ɓ�����V�[�g�̎��ʔԍ������J����B
// Parameter     : sheetNo (Sheet No.)
// Return        : 0 �` SHEETNO_NUM
///////////////////////////////////////////////////////////////////
UC SRVC_GetPrevExitSheet(UC sheetNo)
{
	return CShtManager_GetPrevExitSheet(sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetNextExitSheet
// Description   : �ΏۃV�[�g�́A�V�[�g�C�O�W�b�g�L���[��ɂ����ăl
//               : �N�X�g�V�[�g�ɓ�����V�[�g�̎��ʔԍ������J����B
// Parameter     : sheetNo (Sheet No.)
// Return        : 0 �` SHEETNO_NUM
///////////////////////////////////////////////////////////////////
UC SRVC_GetNextExitSheet(UC sheetNo)
{
	return CShtManager_GetNextExitSheet(sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetBufferPairSheet
// Description   : �o�b�t�@����ɂ����đΏۃV�[�g�ƃy�A�ɂȂ�(�d�ˍ�
//               : �킹�ΏۂƂȂ�)�V�[�g�̎��ʔԍ������J����B
// Parameter     : sheetNo (Sheet No.)
// Return        : 0 �` SHEETNO_NUM
///////////////////////////////////////////////////////////////////
UC SRVC_GetBufferPairSheet(UC sheetNo)
{
	return CShtManager_GetBufferPairSheet(sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_CheckUndeliveredSheetExist
// Description   : �V�[�g���N�G�X�g�L���[�ƃV�[�g�C�O�W�b�g�L���[��
//               : �ɖ��f���o�[�h�̃V�[�g�����邩�ǂ����̏������J
//               : ����B
// Parameter     : outDest (Output Destination ID)
// Return        : true = ���f���o�[�h�̃V�[�g������/
//               : false = ���f���o�[�h�̃V�[�g���Ȃ�
///////////////////////////////////////////////////////////////////
bool SRVC_CheckUndeliveredSheetExist(enum ECdiFinOutDest outDest)
{
	return CShtManager_CheckUndeliveredSheetExist(outDest);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_CheckUnejectedSheetExist
// Description   : �V�[�g�C�O�W�b�g�L���[��̃f���o�[�h�ς݃V�[�g��
//               : ���ɁA�R���p�C���g���C���疢�r�o�ł���V�[�g����
//               : �邩�ǂ����̏������J����B
// Parameter     : outDest (Output Destination ID)
// Return        : true = ���r�o�̃V�[�g������/
//               : false = ���r�o�̃V�[�g���Ȃ�
///////////////////////////////////////////////////////////////////
bool SRVC_CheckUnejectedSheetExist(enum ECdiFinOutDest outDest)
{
	return CShtManager_CheckUnejectedSheetExist(outDest);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_CheckUnfinishedSheetExist  :  V3.6.92
// Description   : ���t�B�j�b�V���Z�b�g�L���m�F����
// Parameter     : outDest (Output Destination ID)
// Return        : �L���m�F����
//               :  true = ���t�B�j�b�V���Z�b�g����
//               :  false = ���t�B�j�b�V���Z�b�g�Ȃ�
///////////////////////////////////////////////////////////////////
bool SRVC_CheckUnfinishedSheetExist(enum ECdiFinOutDest outDest)
{
	return CShtManager_CheckUnfinishedSetExist(outDest);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_CheckUnfinishedSheetExist  :  V3.1.4 
// Description   : �V�[�g�C�O�W�b�g�L���[��̃f���o�[�h�ς݃V�[�g��
//               : ���ɁA�R���p�C���g���C���疢�r�o�ł���V�[�g����
//               : �邩�ǂ����̏������J����B
// Parameter     : outDest (Output Destination ID)
// Return        : true = ���r�o�̃V�[�g������/
//               : false = ���r�o�̃V�[�g���Ȃ�
///////////////////////////////////////////////////////////////////
bool SRVC_CheckUnfinishedSheetExist_2(enum ECdiFinOutDest outDest)
{
	return CShtManager_CheckUnfinishedSetExist_2(outDest);
}



///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetNextArriveSheet
// Description   : �w��G�b�W�ɂ����đΏۂ̃p�X�Z���T�[�Ɏ��ɓ��B��
//               : ��\��̃V�[�g�̎��ʔԍ������J����B
// Parameter     : pathSnrID (Path Sensor ID)
//               : edge (�V�[�g�G�b�W���ʎq)
// Return        : 0 �` SHEETNO_NUM
///////////////////////////////////////////////////////////////////
UC SRVC_GetNextArriveSheet(EShtPathSensorID pathSnrID, EShtSheetEdge edge)
{
	return CShtManager_GetNextArriveSheet(pathSnrID, edge);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_CheckPathSensorPlan
// Description   : �w��̃p�X�Z���T�ɁA�w��̃V�[�g���ʉ߂���\�肪
//               : ���邩�ǂ����̏���񋟂���B
// Parameter     : pathSnrID (Path Sensor ID)
//               : edge (�V�[�g�G�b�W���ʎq)
//               : sheetNo (Sheet No.)
// Return        : true = �ʉߗ\�肠��/false = �ʉߗ\��Ȃ�
///////////////////////////////////////////////////////////////////
bool SRVC_CheckPathSensorPlan(EShtPathSensorID pathSnrID, UC sheetNo)
{
	return CShtManager_CheckPathSensorPlan(pathSnrID, sheetNo);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_CheckPathSensorArrival
// Description   : �w��̃p�X�Z���T�ɁA�w��̃V�[�g�̃G�b�W���ʉߍ�
//               : �݂��ǂ�����񋟂���B
// Parameter     : pathSnrID (Path Sensor ID)
//               : edge (�V�[�g�G�b�W���ʎq)
//               : sheetNo (Sheet No.)
// Return        : true = �ʉߍς�/false = ���ʉ�
///////////////////////////////////////////////////////////////////
bool SRVC_CheckPathSensorArrival(EShtPathSensorID pathSnrID, UC sheetNo, EShtSheetEdge edge)
{
	return CShtManager_CheckPathSensorArrival(pathSnrID, sheetNo, edge);
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_SetPitchSkip
// Description   : Device�s���ɂ��APitchSkip�v�������鎞�̐ݒ�֐�
// Parameter     : bOnOff :  On(PitchSkip �j/ Off�iPitchSkip���Ȃ��j
///////////////////////////////////////////////////////////////////
void SRVC_SetPitchSkip(UC  bOnOff)
{
	CShtManager_SetPitchSkip(bOnOff);
	return;	
}

///////////////////////////////////////////////////////////////////
// Function Name : SRVC_GetPitchSkip
// Description   : PitchSkip��� �F On(PitchSkip���j/ Off�iPitchSkip���Ȃ��j
// Return    :  On(PitchSkip �j/ Off�iPitchSkip���Ȃ��j
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
// 14.12.29	| Yuichi Hoshino	| Trouble Action [ ���x�ύX�^�C�~���O�擾�T�[�r�X�߂�l�s�� ]
// 15.03.30	| Yuichi Hoshino	| Release V1 (Revision 01)
// 15.06.19	| Yuichi Hoshino	| Design Improvement [ �C�O�W�b�g�}�b�N�X�X�s�[�h�Q�ƃT�[�r�X�ǉ� ]
// 15.06.19	| Yuichi Hoshino	| Design Improvement [ �}�L�V�}�����f�B�A�E�F�C�g(�R���p�C��)�Q�ƃT�[�r�X�ǉ� ]
// 15.06.19	| Yuichi Hoshino	| Design Improvement [ ���C�g�f�B�e�N�g�t�B���^�^�C���Q�ƃT�[�r�X�ǉ� ]
//

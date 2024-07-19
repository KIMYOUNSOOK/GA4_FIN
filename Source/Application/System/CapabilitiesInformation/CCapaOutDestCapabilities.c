// File Name:	CCapaOutDestCapabilities.c
// Description:	Implimentation File of Output Destination Capabilities
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.18
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "CCapaOutDestCapabilities.h"
//#include "CCapaOutDestCapabilities.h" // 2015/09/17 test ���g�p�@LeeJE

/////////////////////////////////////////////////////////////////////////////////////////
// Public Data Definition
/////////////////////////////////////////////////////////////////////////////////////////

// 2015/09/17 ���g�p�@LeeJE
// --- Standard Oveerride Method Table of Output Destination Capabilities ---
struct CapaOutDestCapabilitiesOverrideMethod const cCapa_OutDestCapabilitiesMethodStandard = {
	CCapaOutDestCapabilities_Constructor_Super,
	CCapaOutDestCapabilities_GetInstanceID_Super,
	CCapaOutDestCapabilities_GetRelativeOptionID_Super,
	CCapaOutDestCapabilities_GetOutputMaxCapacity_Super,
	CCapaOutDestCapabilities_GetDeviceUnitType_Super,
	CCapaOutDestCapabilities_GetPurgeOutput_Super,
	CCapaOutDestCapabilities_GetVirtualOutput_Super,
	CCapaOutDestCapabilities_GetOffsetSelectability_Super,
	CCapaOutDestCapabilities_Get3PositionOffsetSupport_Super,
	CCapaOutDestCapabilities_GetOffsetMethod_Super,
	CCapaOutDestCapabilities_GetFoldingSelectability_Super,
	CCapaOutDestCapabilities_GetZFoldSupport_Super,
	CCapaOutDestCapabilities_GetTriFoldCSupport_Super,
	CCapaOutDestCapabilities_GetTriFoldZSupport_Super,
	CCapaOutDestCapabilities_GetSaddleFoldSupport_Super,
	CCapaOutDestCapabilities_GetStaplingSelectability_Super,
	CCapaOutDestCapabilities_GetSingleStitchSupport_Super,
	CCapaOutDestCapabilities_GetDoubleStitchSupport_Super,
	CCapaOutDestCapabilities_GetTripleStitchSupport_Super,
	CCapaOutDestCapabilities_GetQuadStitchSupport_Super,
	CCapaOutDestCapabilities_GetSingleRearStitchSupport_Super,
	CCapaOutDestCapabilities_GetSingleFrontStitchSupport_Super,
	CCapaOutDestCapabilities_GetPinCutSupport_Super,
	CCapaOutDestCapabilities_GetMinNumberSheetsPerStaple_Super,
	CCapaOutDestCapabilities_GetMaxNumberSheetsPerStaple_Super,
	CCapaOutDestCapabilities_GetStapledSetCapacity_Super,
	CCapaOutDestCapabilities_GetPunchingSelectability_Super,
	CCapaOutDestCapabilities_GetDoublePunchSupport_Super,
	CCapaOutDestCapabilities_GetTriplePunchSupport_Super,
	CCapaOutDestCapabilities_GetQuadPunchSupport_Super,
	CCapaOutDestCapabilities_GetMultiPunchSupport_Super,
	CCapaOutDestCapabilities_GetDoublePunchType_Super,
	CCapaOutDestCapabilities_GetQuadPunchType_Super,
	CCapaOutDestCapabilities_GetMultiPunchType_Super,
	CCapaOutDestCapabilities_GetFoldingBookletSelectability_Super,
	CCapaOutDestCapabilities_GetSaddleFoldStapleSupport_Super,
	CCapaOutDestCapabilities_GetSaddleFoldNoStapleSupport_Super,
	CCapaOutDestCapabilities_GetBookletCoverOrder_Super,
	CCapaOutDestCapabilities_GetSquareFoldingSupport_Super,
	CCapaOutDestCapabilities_GetMinNumberSheetsSaddleFoldStaple_Super,
	CCapaOutDestCapabilities_GetMaxNumberSheetsSaddleFoldStaple_Super,
	CCapaOutDestCapabilities_GetBookletTrayCapacitySaddleFoldStaple_Super,
	CCapaOutDestCapabilities_GetMinNumberSheetsSaddleFoldNoStaple_Super,
	CCapaOutDestCapabilities_GetMaxNumberSheetsSaddleFoldNoStaple_Super,
	CCapaOutDestCapabilities_GetBookletTrayCapacitySaddleFoldNoStaple_Super,
	CCapaOutDestCapabilities_GetBinsNumber_Super,
	CCapaOutDestCapabilities_GetZFoldSheetCountLong_Super,
	CCapaOutDestCapabilities_GetZFoldSheetCountShort_Super,
	CCapaOutDestCapabilities_GetStapleType_Super,
	CCapaOutDestCapabilities_GetBookletStapleType_Super,
	CCapaOutDestCapabilities_GetStaplingMediaType_Super,
	CCapaOutDestCapabilities_GetSaddleFoldStapleMediaType_Super
};


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_Constructor_Super
// Description   : �R���X�g���N�^
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void CCapaOutDestCapabilities_Constructor_Super(CCapaOutDestCapabilities* pThis, const CapaOutDestCapabilitiesSpec* pSpec)
{
	pThis->mpSpec = pSpec;
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetInstanceID_Super
// Description   : Instance ID�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Output Destination ID
///////////////////////////////////////////////////////////////////
enum ECdiFinOutDest CCapaOutDestCapabilities_GetInstanceID_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinOutDest)(pThis->mpSpec->instanceID);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetRelativeOptionID_Super
// Description   : �֘A�I�v�V������Option ID�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Option ID
///////////////////////////////////////////////////////////////////
EOptionID CCapaOutDestCapabilities_GetRelativeOptionID_Super(CCapaOutDestCapabilities* pThis)
{
	return (EOptionID)(pThis->mpSpec->relativeOptionID);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetOutputMaxCapacity_Super
// Description   : Output Max Capacity�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : category (�T�C�Y�J�e�S���[)
// Return        : Output Max Capacity[�~1��]
///////////////////////////////////////////////////////////////////
US CCapaOutDestCapabilities_GetOutputMaxCapacity_Super(CCapaOutDestCapabilities* pThis, ECapaSizeCategory category)
{
	if ( category < CAPA_SIZE_CTGRY_NUM ) return pThis->mpSpec->outputMaxCapacity[category];
	else								  return 0;								// Unknown
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetDeviceUnitType_Super
// Description   : Device Unit Type�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Device Unit Type
///////////////////////////////////////////////////////////////////
enum ECdiFinOutUnitType CCapaOutDestCapabilities_GetDeviceUnitType_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinOutUnitType)(pThis->mpSpec->deviceUnitType);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetPurgeOutput_Super
// Description   : Purge�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Purge
///////////////////////////////////////////////////////////////////
enum ECdiFinPurgeOut CCapaOutDestCapabilities_GetPurgeOutput_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinPurgeOut)(pThis->mpSpec->purgeOutput);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetVirtualOutput_Super
// Description   : Virtual�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Virtual
///////////////////////////////////////////////////////////////////
enum ECdiFinVirtual CCapaOutDestCapabilities_GetVirtualOutput_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinVirtual)(pThis->mpSpec->virtualOutput);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetOffsetSelectability_Super
// Description   : Offset Selectability�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Selectability
///////////////////////////////////////////////////////////////////
enum ECdiFinSupport CCapaOutDestCapabilities_GetOffsetSelectability_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinSupport)(pThis->mpSpec->offsetSelectability);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_Get3PositionOffsetSupport_Super
// Description   : Three Position Offset�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Selectability
///////////////////////////////////////////////////////////////////
enum ECdiFinSupport CCapaOutDestCapabilities_Get3PositionOffsetSupport_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinSupport)(pThis->mpSpec->threePositionOffsetSupport);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetOffsetMethod_Super
// Description   : Offset Method�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Offset Method
///////////////////////////////////////////////////////////////////
enum ECdiFinOffsetMethod CCapaOutDestCapabilities_GetOffsetMethod_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinOffsetMethod)(pThis->mpSpec->offsetMethod);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetFoldingSelectability_Super
// Description   : Folding Selectability�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Selectability
///////////////////////////////////////////////////////////////////
enum ECdiFinSupport CCapaOutDestCapabilities_GetFoldingSelectability_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinSupport)(pThis->mpSpec->foldingSelectability);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetZFoldSupport_Super
// Description   : Z-Fold Selectability�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Selectability
///////////////////////////////////////////////////////////////////
enum ECdiFinSupport CCapaOutDestCapabilities_GetZFoldSupport_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinSupport)(pThis->mpSpec->zFoldSupport);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetTriFoldCSupport_Super
// Description   : Tri-Fold C Selectability�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Selectability
///////////////////////////////////////////////////////////////////
enum ECdiFinSupport CCapaOutDestCapabilities_GetTriFoldCSupport_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinSupport)(pThis->mpSpec->triFoldCSupport);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetTriFoldZSupport_Super
// Description   : Tri-Fold Z Selectability�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Selectability
///////////////////////////////////////////////////////////////////
enum ECdiFinSupport CCapaOutDestCapabilities_GetTriFoldZSupport_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinSupport)(pThis->mpSpec->triFoldZSupport);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetSaddleFoldSupport_Super
// Description   : Saddle Fold Selectability�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Selectability
///////////////////////////////////////////////////////////////////
enum ECdiFinSupport CCapaOutDestCapabilities_GetSaddleFoldSupport_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinSupport)(pThis->mpSpec->saddleFoldSupport);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetStaplingSelectability_Super
// Description   : Stapling Selectability�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Selectability
///////////////////////////////////////////////////////////////////
enum ECdiFinSupport CCapaOutDestCapabilities_GetStaplingSelectability_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinSupport)(pThis->mpSpec->staplingSelectability);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetSingleStitchSupport_Super
// Description   : Single Stitch Selectability�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Selectability
///////////////////////////////////////////////////////////////////
enum ECdiFinSupport CCapaOutDestCapabilities_GetSingleStitchSupport_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinSupport)(pThis->mpSpec->singleStitchSupport);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetDoubleStitchSupport_Super
// Description   : Double Stitch Selectability�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Selectability
///////////////////////////////////////////////////////////////////
enum ECdiFinSupport CCapaOutDestCapabilities_GetDoubleStitchSupport_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinSupport)(pThis->mpSpec->doubleStitchSupport);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetTripleStitchSupport_Super
// Description   : Triple Stitch Selectability�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Selectability
///////////////////////////////////////////////////////////////////
enum ECdiFinSupport CCapaOutDestCapabilities_GetTripleStitchSupport_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinSupport)(pThis->mpSpec->tripleStitchSupport);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetQuadStitchSupport_Super
// Description   : Quad Stitch Selectability�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Selectability
///////////////////////////////////////////////////////////////////
enum ECdiFinSupport CCapaOutDestCapabilities_GetQuadStitchSupport_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinSupport)(pThis->mpSpec->quadStitchSupport);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetSingleRearStitchSupport_Super
// Description   : Single Rear Stitch Selectability�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Selectability
///////////////////////////////////////////////////////////////////
enum ECdiFinSupport CCapaOutDestCapabilities_GetSingleRearStitchSupport_Super(CCapaOutDestCapabilities* pThis) {
	return (enum ECdiFinSupport)(pThis->mpSpec->singleRearStitchSupport);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetSingleFrontStitchSupport_Super
// Description   : Single Front Stitch Selectability�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Selectability
///////////////////////////////////////////////////////////////////
enum ECdiFinSupport CCapaOutDestCapabilities_GetSingleFrontStitchSupport_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinSupport)(pThis->mpSpec->singleFrontStitchSupport);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetPinCutSupport_Super
// Description   : Pin Cut Selectability�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Selectability
///////////////////////////////////////////////////////////////////
enum ECdiFinSupport CCapaOutDestCapabilities_GetPinCutSupport_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinSupport)(pThis->mpSpec->pinCutSupport);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetMinNumberSheetsPerStaple_Super
// Description   : Minimum Number Of Sheets Per Stapled Set�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : category (�T�C�Y�J�e�S���[)
// Return        : Minimum Number Of Sheets Per Stapled Set[�~1��]
///////////////////////////////////////////////////////////////////
UC CCapaOutDestCapabilities_GetMinNumberSheetsPerStaple_Super(CCapaOutDestCapabilities* pThis, ECapaSizeCategory category)
{
	if ( category < CAPA_SIZE_CTGRY_NUM ) return pThis->mpSpec->minNumberSheetsPerStaple[category];
	else								  return 0;								// Unknown
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetMaxNumberSheetsPerStaple_Super
// Description   : Maximum Number Of Sheets Per Stapled Set�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : category (�T�C�Y�J�e�S���[)
// Return        : Maximum Number Of Sheets Per Stapled Set[�~1��]
///////////////////////////////////////////////////////////////////
UC CCapaOutDestCapabilities_GetMaxNumberSheetsPerStaple_Super(CCapaOutDestCapabilities* pThis, ECapaSizeCategory category)
{
	if ( category < CAPA_SIZE_CTGRY_NUM ) return pThis->mpSpec->maxNumberSheetsPerStaple[category];
	else								  return 0;								// Unknown
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetStapledSetCapacity_Super
// Description   : Stapled Set Tray Capacity�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : category (�T�C�Y�J�e�S���[)
// Return        : Stapled Set Tray Capacity[�~1�Z�b�g]
///////////////////////////////////////////////////////////////////
UC CCapaOutDestCapabilities_GetStapledSetCapacity_Super(CCapaOutDestCapabilities* pThis, ECapaSizeCategory category)
{
	if ( category < CAPA_SIZE_CTGRY_NUM ) return pThis->mpSpec->stapledSetCapacity[category];
	else								  return 0;								// Unknown
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetPunchingSelectability_Super
// Description   : Punch Selectability�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Selectability
///////////////////////////////////////////////////////////////////
enum ECdiFinSupport CCapaOutDestCapabilities_GetPunchingSelectability_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinSupport)(pThis->mpSpec->punchingSelectability);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetDoublePunchSupport_Super
// Description   : Double Punch Support�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Selectability
///////////////////////////////////////////////////////////////////
enum ECdiFinSupport CCapaOutDestCapabilities_GetDoublePunchSupport_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinSupport)(pThis->mpSpec->doublePunchSupport);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetTriplePunchSupport_Super
// Description   : Triple Punch Support�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Selectability
///////////////////////////////////////////////////////////////////
enum ECdiFinSupport CCapaOutDestCapabilities_GetTriplePunchSupport_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinSupport)(pThis->mpSpec->triplePunchSupport);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetQuadPunchSupport_Super
// Description   : Quad Punch Support�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Selectability
///////////////////////////////////////////////////////////////////
enum ECdiFinSupport CCapaOutDestCapabilities_GetQuadPunchSupport_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinSupport)(pThis->mpSpec->quadPunchSupport);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetMultiPunchSupport_Super
// Description   : Multi Punch Support�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Selectability
///////////////////////////////////////////////////////////////////
enum ECdiFinSupport CCapaOutDestCapabilities_GetMultiPunchSupport_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinSupport)(pThis->mpSpec->multiPunchSupport);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetDoublePunchType_Super
// Description   : Double Punch Type�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Double Punch Type
///////////////////////////////////////////////////////////////////
enum ECdiFinDoublePunchType CCapaOutDestCapabilities_GetDoublePunchType_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinDoublePunchType)(pThis->mpSpec->doublePunchType);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetQuadPunchType_Super
// Description   : Quad Punch Type�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Quad Punch Type
///////////////////////////////////////////////////////////////////
enum ECdiFinQuadPunchType CCapaOutDestCapabilities_GetQuadPunchType_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinQuadPunchType)(pThis->mpSpec->quadPunchType);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetMultiPunchType_Super
// Description   : Multi Punch Type�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Multi Punch Type
///////////////////////////////////////////////////////////////////
enum ECdiFinMultiPunchType CCapaOutDestCapabilities_GetMultiPunchType_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinMultiPunchType)(pThis->mpSpec->multiPunchType);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetFoldingBookletSelectability_Super
// Description   : Folding Booklet Support�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Selectability
///////////////////////////////////////////////////////////////////
enum ECdiFinSupport CCapaOutDestCapabilities_GetFoldingBookletSelectability_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinSupport)(pThis->mpSpec->foldingBookletSelectability);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetSaddleFoldStapleSupport_Super
// Description   : Saddle Fold And Staple Support�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Selectability
///////////////////////////////////////////////////////////////////
enum ECdiFinSupport CCapaOutDestCapabilities_GetSaddleFoldStapleSupport_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinSupport)(pThis->mpSpec->saddleFoldStapleSupport);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetSaddleFoldNoStapleSupport_Super
// Description   : Saddle Fold And No Staple Support�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Selectability
///////////////////////////////////////////////////////////////////
enum ECdiFinSupport CCapaOutDestCapabilities_GetSaddleFoldNoStapleSupport_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinSupport)(pThis->mpSpec->saddleFoldNoStapleSupport);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetBookletCoverOrder_Super
// Description   : Cover Order�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Cover Order
///////////////////////////////////////////////////////////////////
enum ECdiFinCoverOrder CCapaOutDestCapabilities_GetBookletCoverOrder_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinCoverOrder)(pThis->mpSpec->bookletCoverOrder);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetSquareFoldingSupport_Super
// Description   : Square Folding Support�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Selectability
///////////////////////////////////////////////////////////////////
enum ECdiFinSupport CCapaOutDestCapabilities_GetSquareFoldingSupport_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinSupport)(pThis->mpSpec->squareFoldingSupport);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetMinNumberSheetsSaddleFoldStaple_Super
// Description   : Minimum Number Of Sheets Per Booklet(Staple And 
//               : Folded)�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Minimum Number Of Sheets Per Booklet[�~1��]
///////////////////////////////////////////////////////////////////
UC CCapaOutDestCapabilities_GetMinNumberSheetsSaddleFoldStaple_Super(CCapaOutDestCapabilities* pThis)
{
	return pThis->mpSpec->minNumberSheetsSaddleFoldStaple;
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetMaxNumberSheetsSaddleFoldStaple_Super
// Description   : Maximum Number Of Sheets Per Booklet(Staple And 
//               : Folded)�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Maximum Number Of Sheets Per Booklet[�~1��]
///////////////////////////////////////////////////////////////////
UC CCapaOutDestCapabilities_GetMaxNumberSheetsSaddleFoldStaple_Super(CCapaOutDestCapabilities* pThis)
{
	return pThis->mpSpec->maxNumberSheetsSaddleFoldStaple;
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetBookletTrayCapacitySaddleFoldStaple_Super
// Description   : Booklet Tray Capacity(Staple And Folded)�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Booklet Tray Capacity[�~1��]
///////////////////////////////////////////////////////////////////
US CCapaOutDestCapabilities_GetBookletTrayCapacitySaddleFoldStaple_Super(CCapaOutDestCapabilities* pThis)
{
	return pThis->mpSpec->bookletTrayCapacitySaddleFoldStaple;
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetMinNumberSheetsSaddleFoldNoStaple_Super
// Description   : Minimum Number Of Sheets Per Booklet(Folded Only
//               : )�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Minimum Number Of Sheets Per Booklet[�~1��]
///////////////////////////////////////////////////////////////////
UC CCapaOutDestCapabilities_GetMinNumberSheetsSaddleFoldNoStaple_Super(CCapaOutDestCapabilities* pThis)
{
	return pThis->mpSpec->minNumberSheetsSaddleFoldNoStaple;
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetMaxNumberSheetsSaddleFoldNoStaple_Super
// Description   : Maximum Number Of Sheets Per Booklet(Folded Only
//               : )�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Maximum Number Of Sheets Per Bookle[�~1��]
///////////////////////////////////////////////////////////////////
UC CCapaOutDestCapabilities_GetMaxNumberSheetsSaddleFoldNoStaple_Super(CCapaOutDestCapabilities* pThis)
{
	return pThis->mpSpec->maxNumberSheetsSaddleFoldNoStaple;
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetBookletTrayCapacitySaddleFoldNoStaple_Super
// Description   : Booklet Tray Capacity(Folded Only)�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Booklet Tray Capacity[�~1��]
///////////////////////////////////////////////////////////////////
US CCapaOutDestCapabilities_GetBookletTrayCapacitySaddleFoldNoStaple_Super(CCapaOutDestCapabilities* pThis)
{
	return pThis->mpSpec->bookletTrayCapacitySaddleFoldNoStaple;
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetBinsNumber_Super
// Description   : Number Of Bins�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Number Of Bins[�~1�r��]
///////////////////////////////////////////////////////////////////
UC CCapaOutDestCapabilities_GetBinsNumber_Super(CCapaOutDestCapabilities* pThis)
{
	return pThis->mpSpec->binsNumber;
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetZFoldSheetCountLong_Super
// Description   : Count Of Sheet (Z-Fold, A3S/11�h�~17�hS)�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Count Of Sheet[�~1��]
///////////////////////////////////////////////////////////////////
UC CCapaOutDestCapabilities_GetZFoldSheetCountLong_Super(CCapaOutDestCapabilities* pThis)
{
	return pThis->mpSpec->zFoldSheetCountLong;
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetZFoldSheetCountShort_Super
// Description   : Count Of Sheet (Z-Fold, Except A3S/11�h�~17�hS)
//               : �擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Count Of Sheet[�~1��]
///////////////////////////////////////////////////////////////////
UC CCapaOutDestCapabilities_GetZFoldSheetCountShort_Super(CCapaOutDestCapabilities* pThis)
{
	return pThis->mpSpec->zFoldSheetCountShort;
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetStapleType_Super
// Description   : Staple Type�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Staple Type
///////////////////////////////////////////////////////////////////
enum ECdiFinStapleType CCapaOutDestCapabilities_GetStapleType_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinStapleType)(pThis->mpSpec->stapleType);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetBookletStapleType_Super
// Description   : Booklet Staple Type�擾����
// Parameter     : pThis (�C���X�^���X�|�C���^)
// Return        : Staple Type
///////////////////////////////////////////////////////////////////
enum ECdiFinStapleType CCapaOutDestCapabilities_GetBookletStapleType_Super(CCapaOutDestCapabilities* pThis)
{
	return (enum ECdiFinStapleType)(pThis->mpSpec->bookletStapleType);
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetStaplingMediaType_Super
// Description   : Stapling Media Type Number Of Sheets�擾����
//               : ���{�֐���Output Destination���ɒ�`��������邱
//               : �Ƃ�O��Ƃ���Dummy�֐��ł���B
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : type (Media Type)
//               : weight (Media Weight) [�~1gsm]
// Return        : Number Of Sheets[�~1��]
///////////////////////////////////////////////////////////////////
UC CCapaOutDestCapabilities_GetStaplingMediaType_Super(CCapaOutDestCapabilities* pThis, UC type, UC posi )
{
	UC ReturnValue =0;
	// return 0;
	// ssmtMediaType[SPECIFIC_MEDIA_TYPE_NUM]
	// ssmtMediaWeightUp[SPECIFIC_MEDIA_TYPE_NUM];
	// ssmtMediaWeightDown[SPECIFIC_MEDIA_TYPE_NUM];
	// ssmtSizeCategory[SPECIFIC_MEDIA_TYPE_NUM];
	// ssmtNumOfSheet[SPECIFIC_MEDIA_TYPE_NUM];
	
	if (type == 1 ) {
		ReturnValue =  pThis->mpSpec->ssmtMediaType[posi];

	} else if ( type == 2) {
		ReturnValue = pThis->mpSpec->ssmtMediaWeightUp[posi];

	} else if ( type == 3) {
		ReturnValue = pThis->mpSpec->ssmtMediaWeightDown[posi];

	} else if ( type == 4) {
		ReturnValue = pThis->mpSpec->ssmtSizeCategory[posi];

	} else if ( type == 5) {		
		ReturnValue = SRVC_GetNvmValueUC(pThis->mpSpec->ssmtNumOfSheet[posi]);

	} else {
		
	}
	return  ReturnValue ;
}

///////////////////////////////////////////////////////////////////
// Function Name : CCapaOutDestCapabilities_GetSaddleFoldStapleMediaType_Super
// Description   : Saddle Fold & Staple Specific Media Type Number 
//               : Of Sheets�擾����
//               : ���{�֐���Output Destination���ɒ�`��������邱
//               : �Ƃ�O��Ƃ���Dummy�֐��ł���B
// Parameter     : pThis (�C���X�^���X�|�C���^)
//               : type (Media Type)
//               : weight (Media Weight) [�~1gsm]
// Return        : Number Of Sheets[�~1��]
///////////////////////////////////////////////////////////////////
UC CCapaOutDestCapabilities_GetSaddleFoldStapleMediaType_Super(CCapaOutDestCapabilities* pThis, UC type, US weight)
{
	return 0;
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//

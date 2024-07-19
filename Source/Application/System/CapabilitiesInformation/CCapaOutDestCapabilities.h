// File Name:	CCapaOutDestCapabilities.h
// Description:	Header File of Output Destination Capabilities
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.18
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#ifndef CCapaOutDestCapabilities_h
#define CCapaOutDestCapabilities_h

/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "FinisherCommon.h"

#include "ECdiFinOutDest.h"
#include "ECapaSizeCategory.h"
#include "ECdiFinOutUnitType.h"
#include "ECdiFinPurgeOut.h"
#include "ECdiFinVirtual.h"
#include "ECdiFinSupport.h"
#include "ECdiFinOffsetMethod.h"
#include "ECdiFinDoublePunchType.h"
#include "ECdiFinQuadPunchType.h"
#include "ECdiFinMultiPunchType.h"
#include "ECdiFinCoverOrder.h"
#include "ECdiFinStapleType.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Data Definition
/////////////////////////////////////////////////////////////////////////////////////////
#define  SPECIFIC_MEDIA_TYPE_NUM 	11 // Number Of Specific Media Types 
struct CapaOutDestCapabilitiesOverrideMethod;


// --- Spec of Output Destination Capabilities ---
typedef struct /*CapaOutDestCapabilitiesSpec*/
{
	const struct CapaOutDestCapabilitiesOverrideMethod *pMethod;
//
	UC ssmtNumber;									// Number Of Specific Media Types 
	UC ssmtMediaType[SPECIFIC_MEDIA_TYPE_NUM];			// Media Type 1 
	UC ssmtMediaWeightUp[SPECIFIC_MEDIA_TYPE_NUM];		// Media Weight Up1
	UC ssmtMediaWeightDown[SPECIFIC_MEDIA_TYPE_NUM];	// Media Weight Down1	// Pointer of method table
	UC ssmtSizeCategory[SPECIFIC_MEDIA_TYPE_NUM];		// Size Category 1 
	UC ssmtNumOfSheet[SPECIFIC_MEDIA_TYPE_NUM];			// Number Of Sheets 1 
//
	UC instanceID;											// Instance ID
	UC relativeOptionID;									// Relative Option ID

	US outputMaxCapacity[CAPA_SIZE_CTGRY_NUM];				// Output Max Capacity
	UC deviceUnitType;										// Device Unit Type
	UC purgeOutput;											// Purge
	UC virtualOutput;										// Virtual
	UC offsetSelectability;									// Offset Selectability
	UC threePositionOffsetSupport;							// Three Position Offset
	UC offsetMethod;										// Offset Method
	UC foldingSelectability;								// Foding Selecatbility
	UC zFoldSupport;										// Z Fold Support
	UC triFoldCSupport;										// Tri-Fold hCh Support
	UC triFoldZSupport;										// Tri-Fold gZh Support
	UC saddleFoldSupport;									// Saddle Fold Support
	UC staplingSelectability;								// Stapling Selectability
	UC singleStitchSupport;									// Single Stitch
	UC doubleStitchSupport;									// Double Stitch
	UC tripleStitchSupport;									// Triple Stitch
	UC quadStitchSupport;									// Quad Stitch
	UC singleRearStitchSupport;								// Single Rear
	UC singleFrontStitchSupport;							// Single Front
	UC pinCutSupport;										// Pin Cut
	UC minNumberSheetsPerStaple[CAPA_SIZE_CTGRY_NUM];		// Minimum Number Of Sheets Per Stapled Set
	UC maxNumberSheetsPerStaple[CAPA_SIZE_CTGRY_NUM];		// Maximum Number Of Sheets Per Stapled Set
	UC stapledSetCapacity[CAPA_SIZE_CTGRY_NUM];				// Stapled Set Tray Capacity
	UC punchingSelectability;								// Punching Selectability
	UC doublePunchSupport;									// Double Punch
	UC triplePunchSupport;									// Triple Punch
	UC quadPunchSupport;									// Quad Punch
	UC multiPunchSupport;									// Multi Punch
	UC doublePunchType;										// Double Punch Type
	UC quadPunchType;										// Quad Punch Type
	UC multiPunchType;										// Multi Punch Type
	UC foldingBookletSelectability;							// Folding Booklet Maker Selectability
	UC saddleFoldStapleSupport;								// Saddle Fold And Staple
	UC saddleFoldNoStapleSupport;							// Saddle Fold And No Staple
	UC bookletCoverOrder;									// Cover Order
	UC squareFoldingSupport;								// Square Folding
	UC minNumberSheetsSaddleFoldStaple;						// Staple And Folded Minimum Number Of Sheets Per Booklet
	UC maxNumberSheetsSaddleFoldStaple;						// Staple And Folded Maximum Number Of Sheets Per Booklet
	US bookletTrayCapacitySaddleFoldStaple;					// Staple And Folded Booklet Tray Capacity
	UC minNumberSheetsSaddleFoldNoStaple;					// Folded Only Minimum Number Of Sheets Per Booklet
	UC maxNumberSheetsSaddleFoldNoStaple;					// Folded Only Maximum Number Of Sheets Per Booklet
	US bookletTrayCapacitySaddleFoldNoStaple;				// Folded Only Booklet Tray Capacity
	UC binsNumber;											// Number Of Bins
	UC zFoldSheetCountLong;									// Count Of Z-Fold Sheet(Z-Fold, A3S/11"~17"S)
	UC zFoldSheetCountShort;								// Count Of Z-Fold Sheet(Z-Fold, Except A3S/11"~17"S))
	UC stapleType;											// Staple Type
	UC bookletStapleType;									// Booklet Maker Staple Type
	
} CapaOutDestCapabilitiesSpec;


// --- Instance of Output Destination Capabilities ---
typedef struct /*CCapaOutDestCapabilities*/
{
	const CapaOutDestCapabilitiesSpec* mpSpec;

	// --- ˆÈ‰ºAŽæ‚è‚¤‚é’l‚ªBitŠ„‚è•t‚¯‚ÅÏ‚Þ‚à‚Ì ---

} CCapaOutDestCapabilities;

// --- Override Function Table ---
struct CapaOutDestCapabilitiesOverrideMethod
{
	void (*Constructor)(CCapaOutDestCapabilities* pThis, const CapaOutDestCapabilitiesSpec* pSpec);

	enum ECdiFinOutDest (*GetInstanceID)(CCapaOutDestCapabilities* pThis);
	EOptionID (*GetRelativeOptionID)(CCapaOutDestCapabilities* pThis);

	US (*GetOutputMaxCapacity)(CCapaOutDestCapabilities* pThis, ECapaSizeCategory category);
	enum ECdiFinOutUnitType (*GetDeviceUnitType)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinPurgeOut (*GetPurgeOutput)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinVirtual (*GetVirtualOutput)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinSupport (*GetOffsetSelectability)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinSupport (*Get3PositionOffsetSupport)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinOffsetMethod (*GetOffsetMethod)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinSupport (*GetFoldingSelectability)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinSupport (*GetZFoldSupport)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinSupport (*GetTriFoldCSupport)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinSupport (*GetTriFoldZSupport)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinSupport (*GetSaddleFoldSupport)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinSupport (*GetStaplingSelectability)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinSupport (*GetSingleStitchSupport)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinSupport (*GetDoubleStitchSupport)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinSupport (*GetTripleStitchSupport)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinSupport (*GetQuadStitchSupport)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinSupport (*GetSingleRearStitchSupport)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinSupport (*GetSingleFrontStitchSupport)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinSupport (*GetPinCutSupport)(CCapaOutDestCapabilities* pThis);
	UC (*GetMinNumberSheetsPerStaple)(CCapaOutDestCapabilities* pThis, ECapaSizeCategory category);
	UC (*GetMaxNumberSheetsPerStaple)(CCapaOutDestCapabilities* pThis, ECapaSizeCategory category);
	UC (*GetStapledSetCapacity)(CCapaOutDestCapabilities* pThis, ECapaSizeCategory category);
	enum ECdiFinSupport (*GetPunchingSelectability)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinSupport (*GetDoublePunchSupport)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinSupport (*GetTriplePunchSupport)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinSupport (*GetQuadPunchSupport)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinSupport (*GetMultiPunchSupport)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinDoublePunchType (*GetDoublePunchType)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinQuadPunchType (*GetQuadPunchType)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinMultiPunchType (*GetMultiPunchType)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinSupport (*GetFoldingBookletSelectability)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinSupport (*GetSaddleFoldStapleSupport)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinSupport (*GetSaddleFoldNoStapleSupport)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinCoverOrder (*GetBookletCoverOrder)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinSupport (*GetSquareFoldingSupport)(CCapaOutDestCapabilities* pThis);
	UC (*GetMinNumberSheetsSaddleFoldStaple)(CCapaOutDestCapabilities* pThis);
	UC (*GetMaxNumberSheetsSaddleFoldStaple)(CCapaOutDestCapabilities* pThis);
	US (*GetBookletTrayCapacitySaddleFoldStaple)(CCapaOutDestCapabilities* pThis);
	UC (*GetMinNumberSheetsSaddleFoldNoStaple)(CCapaOutDestCapabilities* pThis);
	UC (*GetMaxNumberSheetsSaddleFoldNoStaple)(CCapaOutDestCapabilities* pThis);
	US (*GetBookletTrayCapacitySaddleFoldNoStaple)(CCapaOutDestCapabilities* pThis);
	UC (*GetBinsNumber)(CCapaOutDestCapabilities* pThis);
	UC (*GetZFoldSheetCountLong)(CCapaOutDestCapabilities* pThis);
	UC (*GetZFoldSheetCountShort)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinStapleType (*GetStapleType)(CCapaOutDestCapabilities* pThis);
	enum ECdiFinStapleType (*GetBookletStapleType)(CCapaOutDestCapabilities* pThis);
	UC (*GetStaplingMediaType)(CCapaOutDestCapabilities* pThis, UC type, UC posi);
	UC (*GetSaddleFoldStapleMediaType)(CCapaOutDestCapabilities* pThis, UC type, US weight);
};

// --- Standard Override Method Table of Output Destination Capabilities ---
// 2015/09/17 –¢Žg—p@LeeJE
extern struct CapaOutDestCapabilitiesOverrideMethod const cCapa_OutDestCapabilitiesMethodStandard;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Method of Super Class ---
void CCapaOutDestCapabilities_Constructor_Super(CCapaOutDestCapabilities* pThis, const CapaOutDestCapabilitiesSpec* pSpec);

enum ECdiFinOutDest CCapaOutDestCapabilities_GetInstanceID_Super(CCapaOutDestCapabilities* pThis);
EOptionID CCapaOutDestCapabilities_GetRelativeOptionID_Super(CCapaOutDestCapabilities* pThis);

US CCapaOutDestCapabilities_GetOutputMaxCapacity_Super(CCapaOutDestCapabilities* pThis, ECapaSizeCategory category);
enum ECdiFinOutUnitType CCapaOutDestCapabilities_GetDeviceUnitType_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinPurgeOut CCapaOutDestCapabilities_GetPurgeOutput_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinVirtual CCapaOutDestCapabilities_GetVirtualOutput_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinSupport CCapaOutDestCapabilities_GetOffsetSelectability_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinSupport CCapaOutDestCapabilities_Get3PositionOffsetSupport_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinOffsetMethod CCapaOutDestCapabilities_GetOffsetMethod_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinSupport CCapaOutDestCapabilities_GetFoldingSelectability_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinSupport CCapaOutDestCapabilities_GetZFoldSupport_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinSupport CCapaOutDestCapabilities_GetTriFoldCSupport_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinSupport CCapaOutDestCapabilities_GetTriFoldZSupport_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinSupport CCapaOutDestCapabilities_GetSaddleFoldSupport_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinSupport CCapaOutDestCapabilities_GetStaplingSelectability_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinSupport CCapaOutDestCapabilities_GetSingleStitchSupport_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinSupport CCapaOutDestCapabilities_GetDoubleStitchSupport_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinSupport CCapaOutDestCapabilities_GetTripleStitchSupport_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinSupport CCapaOutDestCapabilities_GetQuadStitchSupport_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinSupport CCapaOutDestCapabilities_GetSingleRearStitchSupport_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinSupport CCapaOutDestCapabilities_GetSingleFrontStitchSupport_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinSupport CCapaOutDestCapabilities_GetPinCutSupport_Super(CCapaOutDestCapabilities* pThis);
UC CCapaOutDestCapabilities_GetMinNumberSheetsPerStaple_Super(CCapaOutDestCapabilities* pThis, ECapaSizeCategory category);
UC CCapaOutDestCapabilities_GetMaxNumberSheetsPerStaple_Super(CCapaOutDestCapabilities* pThis, ECapaSizeCategory category);
UC CCapaOutDestCapabilities_GetStapledSetCapacity_Super(CCapaOutDestCapabilities* pThis, ECapaSizeCategory category);
enum ECdiFinSupport CCapaOutDestCapabilities_GetPunchingSelectability_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinSupport CCapaOutDestCapabilities_GetDoublePunchSupport_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinSupport CCapaOutDestCapabilities_GetTriplePunchSupport_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinSupport CCapaOutDestCapabilities_GetQuadPunchSupport_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinSupport CCapaOutDestCapabilities_GetMultiPunchSupport_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinDoublePunchType CCapaOutDestCapabilities_GetDoublePunchType_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinQuadPunchType CCapaOutDestCapabilities_GetQuadPunchType_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinMultiPunchType CCapaOutDestCapabilities_GetMultiPunchType_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinSupport CCapaOutDestCapabilities_GetFoldingBookletSelectability_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinSupport CCapaOutDestCapabilities_GetSaddleFoldStapleSupport_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinSupport CCapaOutDestCapabilities_GetSaddleFoldNoStapleSupport_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinCoverOrder CCapaOutDestCapabilities_GetBookletCoverOrder_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinSupport CCapaOutDestCapabilities_GetSquareFoldingSupport_Super(CCapaOutDestCapabilities* pThis);
UC CCapaOutDestCapabilities_GetMinNumberSheetsSaddleFoldStaple_Super(CCapaOutDestCapabilities* pThis);
UC CCapaOutDestCapabilities_GetMaxNumberSheetsSaddleFoldStaple_Super(CCapaOutDestCapabilities* pThis);
US CCapaOutDestCapabilities_GetBookletTrayCapacitySaddleFoldStaple_Super(CCapaOutDestCapabilities* pThis);
UC CCapaOutDestCapabilities_GetMinNumberSheetsSaddleFoldNoStaple_Super(CCapaOutDestCapabilities* pThis);
UC CCapaOutDestCapabilities_GetMaxNumberSheetsSaddleFoldNoStaple_Super(CCapaOutDestCapabilities* pThis);
US CCapaOutDestCapabilities_GetBookletTrayCapacitySaddleFoldNoStaple_Super(CCapaOutDestCapabilities* pThis);
UC CCapaOutDestCapabilities_GetBinsNumber_Super(CCapaOutDestCapabilities* pThis);
UC CCapaOutDestCapabilities_GetZFoldSheetCountLong_Super(CCapaOutDestCapabilities* pThis);
UC CCapaOutDestCapabilities_GetZFoldSheetCountShort_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinStapleType CCapaOutDestCapabilities_GetStapleType_Super(CCapaOutDestCapabilities* pThis);
enum ECdiFinStapleType CCapaOutDestCapabilities_GetBookletStapleType_Super(CCapaOutDestCapabilities* pThis);
UC CCapaOutDestCapabilities_GetStaplingMediaType_Super(CCapaOutDestCapabilities* pThis, UC type, UC posi);
UC CCapaOutDestCapabilities_GetSaddleFoldStapleMediaType_Super(CCapaOutDestCapabilities* pThis, UC type, US weight);

// --- I/F Function Prototype Definition ---
#define CCapaOutDestCapabilities_Constructor(pThis, pSpec) (pSpec->pMethod->Constructor(pThis, pSpec))
																				// void (CCapaOutDestCapabilities*, const CapaOutDestCapabilitiesSpec*)

#define CCapaOutDestCapabilities_GetInstanceID(pThis) ((pThis)->mpSpec->pMethod->GetInstanceID(pThis))
																				// enum ECdiFinOutDest (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetRelativeOptionID(pThis) ((EOptionID)((pThis)->mpSpec->pMethod->GetRelativeOptionID(pThis)))
																				// EOptionID (CCapaOutDestCapabilities*)

#define CCapaOutDestCapabilities_GetOutputMaxCapacity(pThis, category) ((pThis)->mpSpec->pMethod->GetOutputMaxCapacity(pThis, category))
																				// US (CCapaOutDestCapabilities* ECapaSizeCategory)
#define CCapaOutDestCapabilities_GetDeviceUnitType(pThis) ((pThis)->mpSpec->pMethod->GetDeviceUnitType(pThis))
																				// enum ECdiFinOutUnitType (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetPurgeOutput(pThis) ((pThis)->mpSpec->pMethod->GetPurgeOutput(pThis))
																				// enum ECdiFinPurgeOut (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetVirtualOutput(pThis) ((pThis)->mpSpec->pMethod->GetVirtualOutput(pThis))
																				// enum ECdiFinVirtual (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetOffsetSelectability(pThis) ((pThis)->mpSpec->pMethod->GetOffsetSelectability(pThis))
																				// enum ECdiFinSupport (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_Get3PositionOffsetSupport(pThis) ((pThis)->mpSpec->pMethod->Get3PositionOffsetSupport(pThis))
																				// enum ECdiFinSupport (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetOffsetMethod(pThis) ((pThis)->mpSpec->pMethod->GetOffsetMethod(pThis))
																				// enum ECdiFinOffsetMethod (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetFoldingSelectability(pThis) ((pThis)->mpSpec->pMethod->GetFoldingSelectability(pThis))
																				// enum ECdiFinSupport (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetZFoldSupport(pThis) ((pThis)->mpSpec->pMethod->GetZFoldSupport(pThis))
																				// enum ECdiFinSupport (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetTriFoldCSupport(pThis) ((pThis)->mpSpec->pMethod->GetTriFoldCSupport(pThis))
																				// enum ECdiFinSupport (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetTriFoldZSupport(pThis) ((pThis)->mpSpec->pMethod->GetTriFoldZSupport(pThis))
																				// enum ECdiFinSupport (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetSaddleFoldSupport(pThis) ((pThis)->mpSpec->pMethod->GetSaddleFoldSupport(pThis))
																				// enum ECdiFinSupport (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetStaplingSelectability(pThis) ((pThis)->mpSpec->pMethod->GetStaplingSelectability(pThis))
																				// enum ECdiFinSupport (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetSingleStitchSupport(pThis) ((pThis)->mpSpec->pMethod->GetSingleStitchSupport(pThis))
																				// enum ECdiFinSupport (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetDoubleStitchSupport(pThis) ((pThis)->mpSpec->pMethod->GetDoubleStitchSupport(pThis))
																				// enum ECdiFinSupport (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetTripleStitchSupport(pThis) ((pThis)->mpSpec->pMethod->GetTripleStitchSupport(pThis))
																				// enum ECdiFinSupport (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetQuadStitchSupport(pThis) ((pThis)->mpSpec->pMethod->GetQuadStitchSupport(pThis))
																				// enum ECdiFinSupport (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetSingleRearStitchSupport(pThis) ((pThis)->mpSpec->pMethod->GetSingleRearStitchSupport(pThis))
																				// enum ECdiFinSupport (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetSingleFrontStitchSupport(pThis) ((pThis)->mpSpec->pMethod->GetSingleFrontStitchSupport(pThis))
																				// enum ECdiFinSupport (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetPinCutSupport(pThis) ((pThis)->mpSpec->pMethod->GetPinCutSupport(pThis))
																				// enum ECdiFinSupport (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetMinNumberSheetsPerStaple(pThis, category) ((pThis)->mpSpec->pMethod->GetMinNumberSheetsPerStaple(pThis, category))
																				// UC (CCapaOutDestCapabilities*, ECapaSizeCategory)
#define CCapaOutDestCapabilities_GetMaxNumberSheetsPerStaple(pThis, category) ((pThis)->mpSpec->pMethod->GetMaxNumberSheetsPerStaple(pThis, category))
																				// UC (CCapaOutDestCapabilities*, ECapaSizeCategory)
#define CCapaOutDestCapabilities_GetStapledSetCapacity(pThis, category) ((pThis)->mpSpec->pMethod->GetStapledSetCapacity(pThis, category))
																				// UC (CCapaOutDestCapabilities*, ECapaSizeCategory)
#define CCapaOutDestCapabilities_GetPunchingSelectability(pThis) ((pThis)->mpSpec->pMethod->GetPunchingSelectability(pThis))
																				// enum ECdiFinSupport (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetDoublePunchSupport(pThis) ((pThis)->mpSpec->pMethod->GetDoublePunchSupport(pThis))
																				// enum ECdiFinSupport (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetTriplePunchSupport(pThis) ((pThis)->mpSpec->pMethod->GetTriplePunchSupport(pThis))
																				// enum ECdiFinSupport (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetQuadPunchSupport(pThis) ((pThis)->mpSpec->pMethod->GetQuadPunchSupport(pThis))
																				// enum ECdiFinSupport (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetMultiPunchSupport(pThis) ((pThis)->mpSpec->pMethod->GetMultiPunchSupport(pThis))
																				// enum ECdiFinSupport (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetDoublePunchType(pThis) ((pThis)->mpSpec->pMethod->GetDoublePunchType(pThis))
																				// enum ECdiFinDoublePunchType (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetQuadPunchType(pThis) ((pThis)->mpSpec->pMethod->GetQuadPunchType(pThis))
																				// enum ECdiFinQuadPunchType (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetMultiPunchType(pThis) ((pThis)->mpSpec->pMethod->GetMultiPunchType(pThis))
																				// enum ECdiFinMultiPunchType (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetFoldingBookletSelectability(pThis) ((pThis)->mpSpec->pMethod->GetFoldingBookletSelectability(pThis))
																				// enum ECdiFinSupport (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetSaddleFoldStapleSupport(pThis) ((pThis)->mpSpec->pMethod->GetSaddleFoldStapleSupport(pThis))
																				// enum ECdiFinSupport (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetSaddleFoldNoStapleSupport(pThis) ((pThis)->mpSpec->pMethod->GetSaddleFoldNoStapleSupport(pThis))
																				// enum ECdiFinSupport (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetBookletCoverOrder(pThis) ((pThis)->mpSpec->pMethod->GetBookletCoverOrder(pThis))
																				// enum ECdiFinCoverOrder (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetSquareFoldingSupport(pThis) ((pThis)->mpSpec->pMethod->GetSquareFoldingSupport(pThis))
																				// enum ECdiFinSupport (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetMinNumberSheetsSaddleFoldStaple(pThis) ((pThis)->mpSpec->pMethod->GetMinNumberSheetsSaddleFoldStaple(pThis))
																				// UC (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetMaxNumberSheetsSaddleFoldStaple(pThis) ((pThis)->mpSpec->pMethod->GetMaxNumberSheetsSaddleFoldStaple(pThis))
																				// UC (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetBookletTrayCapacitySaddleFoldStaple(pThis) ((pThis)->mpSpec->pMethod->GetBookletTrayCapacitySaddleFoldStaple(pThis))
																				// US (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetMinNumberSheetsSaddleFoldNoStaple(pThis) ((pThis)->mpSpec->pMethod->GetMinNumberSheetsSaddleFoldNoStaple(pThis))
																				// UC (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetMaxNumberSheetsSaddleFoldNoStaple(pThis) ((pThis)->mpSpec->pMethod->GetMaxNumberSheetsSaddleFoldNoStaple(pThis))
																				// UC (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetBookletTrayCapacitySaddleFoldNoStaple(pThis) ((pThis)->mpSpec->pMethod->GetBookletTrayCapacitySaddleFoldNoStaple(pThis))
																				// US (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetBinsNumber(pThis) ((pThis)->mpSpec->pMethod->GetBinsNumber(pThis))
																				// UC (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetZFoldSheetCountLong(pThis) ((pThis)->mpSpec->pMethod->GetZFoldSheetCountLong(pThis))
																				// UC (CCapaOutDestCapabilities*, US)
#define CCapaOutDestCapabilities_GetZFoldSheetCountShort(pThis) ((pThis)->mpSpec->pMethod->GetZFoldSheetCountShort(pThis))
																				// UC (CCapaOutDestCapabilities*, US)
#define CCapaOutDestCapabilities_GetStapleType(pThis) ((pThis)->mpSpec->pMethod->GetStapleType(pThis))
																				// enum ECdiFinStapleType (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetBookletStapleType(pThis) ((pThis)->mpSpec->pMethod->GetBookletStapleType(pThis))
																				// enum ECdiFinStapleType (CCapaOutDestCapabilities*)
#define CCapaOutDestCapabilities_GetStaplingMediaType(pThis, type, posi) ((pThis)->mpSpec->pMethod->GetStaplingMediaType(pThis, type, posi))
																				// UC (CCapaOutDestCapabilities*, UC, US)
#define CCapaOutDestCapabilities_GetSaddleFoldStapleMediaType(pThis, type, weight) ((pThis)->mpSpec->pMethod->GetSaddleFoldStapleMediaType(pThis, type, weight))
																				// UC (CCapaOutDestCapabilities*, UC, US)


#endif // #ifndef CCapaOutDestCapabilities_h

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//


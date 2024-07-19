///////////////////////////////////////////////////////////
//  TempDataInst.c
//  Implementation of the Temp Data
//  Created on:      2015/02/19 11:04:47
//  Original author: Auto Generate By Excel
///////////////////////////////////////////////////////////

#include "TempDataInst.h"

#include "CTempData.h"
#include "ECdiFinDataType.h"

//----- Temp 8bit Data -----
UC temp_saveFlag;
UC temp_Data1;
UC temp_Data2;
SC temp_Data3;
UC temp_Data8;
SC temp_Data9;

//----- Temp 16bit Data -----
US temp_Data4;
SS temp_Data5;
SS temp_Data10;

//----- Temp 32bit Data -----
UL temp_Data6;
SL temp_Data7;


static const CTempDataSpec tempid_temp_saveFlag = { (UL*)&temp_saveFlag, 0, eCdiFinDataType_UnsignChar, };
static const CTempDataSpec tempid_temp_Data1 = { (UL*)&temp_Data1, 1, eCdiFinDataType_UnsignChar, };
static const CTempDataSpec tempid_temp_Data2 = { (UL*)&temp_Data2, 2, eCdiFinDataType_UnsignChar, };
static const CTempDataSpec tempid_temp_Data3 = { (UL*)&temp_Data3, (UL)-1, eCdiFinDataType_SignChar, };
static const CTempDataSpec tempid_temp_Data4 = { (UL*)&temp_Data4, 500, eCdiFinDataType_UnsignShort, };
static const CTempDataSpec tempid_temp_Data5 = { (UL*)&temp_Data5, (UL)-130, eCdiFinDataType_SignShort, };
static const CTempDataSpec tempid_temp_Data6 = { (UL*)&temp_Data6, 100000, eCdiFinDataType_UnsignLong, };
static const CTempDataSpec tempid_temp_Data7 = { (UL*)&temp_Data7, (UL)-500, eCdiFinDataType_SignLong, };
static const CTempDataSpec tempid_temp_Data8 = { (UL*)&temp_Data8, 8, eCdiFinDataType_UnsignChar, };
static const CTempDataSpec tempid_temp_Data9 = { (UL*)&temp_Data9, 0, eCdiFinDataType_SignChar, };
static const CTempDataSpec tempid_temp_Data10 = { (UL*)&temp_Data10, (UL)-10, eCdiFinDataType_SignShort, };


const CTempData tempAllData[TEMP_DATA_NUM] = {
    { &tempid_temp_saveFlag },
    { &tempid_temp_Data1 },
    { &tempid_temp_Data2 },
    { &tempid_temp_Data3 },
    { &tempid_temp_Data4 },
    { &tempid_temp_Data5 },
    { &tempid_temp_Data6 },
    { &tempid_temp_Data7 },
    { &tempid_temp_Data8 },
    { &tempid_temp_Data9 },
    { &tempid_temp_Data10 },
};


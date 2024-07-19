///////////////////////////////////////////////////////////
//  TempDataInst.h
//  Implementation of the Temp Data
//  Created on:      2015/02/19 11:04:47
//  Original author: Auto Generate By Excel
///////////////////////////////////////////////////////////

#ifndef TempDataInst_h
#define TempDataInst_h

#include "FinisherCommon.h"

#define TEMP_DATA_NUM			11
#define TEMP_DATA_RAM_SIZE		20

//----- Temp 8bit Data -----
extern UC temp_saveFlag;
extern UC temp_Data1;
extern UC temp_Data2;
extern SC temp_Data3;
extern UC temp_Data8;
extern SC temp_Data9;

//----- Temp 16bit Data -----
extern US temp_Data4;
extern SS temp_Data5;
extern SS temp_Data10;

//----- Temp 32bit Data -----
extern UL temp_Data6;
extern SL temp_Data7;

#endif // #ifndef TempDataInst_h

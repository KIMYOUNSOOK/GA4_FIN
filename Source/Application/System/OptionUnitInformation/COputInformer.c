// File Name:	COputInformer.c
// Description:	Implimentation File of Option Unit Information
// Engineer:	Yuichi Hoshino
// Last Edit:	14.04.14
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


/////////////////////////////////////////////////////////////////////////////////////////
// Include File
/////////////////////////////////////////////////////////////////////////////////////////

#include "COputInformer.h"

#include "DD_DIO.h"


/////////////////////////////////////////////////////////////////////////////////////////
// Public Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Private Data Definition
/////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////
// Private Function Prototype Definition
/////////////////////////////////////////////////////////////////////////////////////////

// --- Member of Option Unit Information Informer ---
struct COputInformerMember
{
	US mOptionBit;
	UC mOptionType[OPTID_NUM];
};

static struct COputInformerMember gOput_InformerMember;


/////////////////////////////////////////////////////////////////////////////////////////
// Function Body
/////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Function Name : COputInformer_Constructor
// Description   : �R���X�g���N�^
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void COputInformer_Constructor()
{
	UC index;

	gOput_InformerMember.mOptionBit = 0x0000;

	for ( index = 0; index < OPTID_NUM; index++ ){	
		gOput_InformerMember.mOptionType[index] = 0;
	}
}

///////////////////////////////////////////////////////////////////
// Function Name : COputInformer_MakeInformation
// Description   : ���\�z����
// Parameter     : �]
// Return        : �]
///////////////////////////////////////////////////////////////////
void COputInformer_MakeInformation()
{
	gOput_InformerMember.mOptionBit |= OPTID_CNV_BIT(OPTID_BASE);

	// !!! Product Specific !!!
	// --- �I�v�V�����ڑ����m ---
//	gOput_InformerMember.mOptionBit |= OPTID_CNV_BIT(OPTID_XXX);
	
	// --- �I�v�V������ʔ��f ---
//	gOput_InformerMember.mOptionType[OPTID_XXX] = XXX;
}

///////////////////////////////////////////////////////////////////
// Function Name : COputInformer_GetOptionDetect
// Description   : �I�v�V�����ڑ����擾����
// Parameter     : optionID (Option ID)
// Return        : true = �ڑ�����
//               : false = �ڑ��Ȃ�
///////////////////////////////////////////////////////////////////
bool COputInformer_GetOptionDetect(EOptionID optionID)
{
	if ( gOput_InformerMember.mOptionBit & OPTID_CNV_BIT(optionID) ) return true;
	else															 return false;
}

///////////////////////////////////////////////////////////////////
// Function Name : COputInformer_GetOptionType
// Description   : �I�v�V������ʎ擾����
// Parameter     : optionID (Option ID)
// Return        : ��ʎ��ʔԍ�(�I�v�V�������ɒ�`)
///////////////////////////////////////////////////////////////////
UC COputInformer_GetOptionType(EOptionID optionID)
{
	if ( optionID < OPTID_NUM ){
		return gOput_InformerMember.mOptionType[optionID];
	}
	else{
		return 0;
	}
}


// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//
// File Name:	ECMPL_TMP_Profile.h
// Description:	Tamper Device ÇÃìÆçÏÇíËã`
// Engineer:	Takahiko Minaga
// Last Edit:	14.09.24
// Revision:	00
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

#if !defined(ECMPL_TMP_Profile_h)
#define ECMPL_TMP_Profile_h


typedef enum /* ECMPL_TMP_ProfileID */
{
	CMPL_TMP_PRF_ID_NORMAL_HI = 0x02, 	// í èÌ çÇë¨
	CMPL_TMP_PRF_ID_NORMAL_MID,	// í èÌ íÜë¨
	CMPL_TMP_PRF_ID_NORMAL_LOW,	// í èÌ í·ë¨
	CMPL_TMP_PRF_ID_INITIALIZE,	// Initialize Profile
	// Number of Module Action
	CMPL_TMP_PRF_NUM,

} ECMPL_TMP_ProfileID;


#endif //#if !defined(ECMPL_TMP_Profile_h)

// Change History
// Date:	| Engineer:			| Note:
// ---------+-------------------+--------------------------------------------------------
// XX.XX.XX	| Xxxxxx Xxxxxx		| Created this file
//





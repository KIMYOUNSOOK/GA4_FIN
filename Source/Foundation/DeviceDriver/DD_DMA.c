// File Name: DD_DMA.c
// Description:
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 13.10.15
// Revision:  000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+--------------------------------------------------------------------------------------
// 13.10.15 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev001 Base)
// Revision 000

#pragma interrupt INTDMA0 DD_DMA0_Interrupt
#pragma interrupt INTDMA1 DD_DMA1_Interrupt
#ifndef STARTER_KIT
#pragma interrupt INTDMA2 DD_DMA2_Interrupt
#pragma interrupt INTDMA3 DD_DMA3_Interrupt
#endif

#include "Common.h"
#include "DD_Common.h"
#include "DD_DMA.h"

/*
    DMA Mode Control Register n (DMCn)
*/
/* Setting of DMA transfer start software trigger (STGn) */
#define _00_DMA_TRIGGER_OPERATION_NO        (0x00U) /* no trigger operation */
#define _80_DMA_TRIGGER_OPERATION_SOFTWARE  (0x80U) /* DMA transfer is started when DMA operation is enabled */
/* Selection of DMA transfer direction (DRSn) */
#define _00_DMA_TRANSFER_DIR_SFR2RAM        (0x00U) /* SFR to internal RAM  */
#define _40_DMA_TRANSFER_DIR_RAM2SFR        (0x40U) /* internal RAM to SFR */
/* Specification of transfer data size for DMA transfer (DSn) */
#define _00_DMA_DATA_SIZE_8                 (0x00U) /* 8 bits */
#define _20_DMA_DATA_SIZE_16                (0x20U) /* 16 bits */
/* Pending of DMA transfer(DWAITn) */
#define _00_DMA_PENDING_NOTHELD             (0x00U) /* executes DMA transfer upon DMA start request */
#define _10_DMA_PENDING_HOLDS               (0x10U) /* hold DMA start request pending if any */
/* Selection of DMA stat source (IFCn3 - IFCn0) */
#define _00_DMA_TRIGGER_SOFTWARE            (0x00U) /* disable DMA transfer by interrupt */
#define _01_DMA_TRIGGER_AD                  (0x01U) /* A/D conversion end interrupt */
#define _02_DMA_TRIGGER_TM00                (0x02U) /* timer channel 0 interrupt */
#define _03_DMA_TRIGGER_TM01                (0x03U) /* timer channel 1 interrupt */
#define _04_DMA_TRIGGER_TM02                (0x04U) /* timer channel 2 interrupt */
#define _05_DMA_TRIGGER_TM03                (0x05U) /* timer channel 3 interrupt */
#define _02_DMA_TRIGGER_TM10                (0x02U) /* timer channel 10 interrupt */
#define _03_DMA_TRIGGER_TM11                (0x03U) /* timer channel 11 interrupt */
#define _04_DMA_TRIGGER_TM12                (0x04U) /* timer channel 12 interrupt */
#define _05_DMA_TRIGGER_TM13                (0x05U) /* timer channel 13 interrupt */
#define _06_DMA_TRIGGER_ST0_CSI00           (0x06U) /* UART0 transmission end / CSI00 transfer end */
#define _07_DMA_TRIGGER_SR0_CSI01           (0x07U) /* UART0 reception end / CSI01 transfer end */
#define _06_DMA_TRIGGER_ST3_CSI30           (0x06U) /* UART3 transmission end / CSI30 transfer end */
#define _07_DMA_TRIGGER_SR3_CSI31           (0x07U) /* UART3 reception end / CSI31 transfer end */
#define _08_DMA_TRIGGER_ST1_CSI10           (0x08U) /* UART1 transmission end / CSI10 transfer end */
#define _09_DMA_TRIGGER_SR1_CSI11           (0x09U) /* UART1 reception end / CSI11 transfer end */
#define _0A_DMA_TRIGGER_ST2_CSI20           (0x0AU) /* UART2 transmission end / CSI20 transfer end */
#define _0B_DMA_TRIGGER_SR2_CSI21           (0x0BU) /* UART2 reception end / CSI21 transfer end */

/*
    DMA operation control register n (DRCn)
*/
/* DMA operation enable flag (DENn) */
#define _80_DMA_OPERATION_ENABLE            (0x80U) /* enable operation of DMA */
#define _00_DMA_OPERATION_DISABLE           (0x00U) /* disable operation of DMA */ 
/* DMA transfer mode flag (DSTn) */
#define _01_DMA_TRANSFER_UNDEREXEC          (0x01U) /* DMA transfer is not completed */ 
#define _00_DMA_TRANSFER_COMPLETED          (0x00U) /* DMA transfer is completed */

#define _18_DMA0_SFR_ADDRESS                (0x18U)
#define _1A_DMA1_SFR_ADDRESS                (0x1AU)
#define _70_DMA2_SFR_ADDRESS                (0x70U)
#define _72_DMA3_SFR_ADDRESS                (0x72U)

#define DD_DMA_CHANNEL_NUM	4

DD_CALLBACK_TYPE dmacCallbackTable[DD_DMA_CHANNEL_NUM];

UC DD_DMA_Get_IFC_Pattern(UC dmaCh, DD_DMA_TRIGGER_TYPE type, UC trgCh);

//============================================================================= 
// Function Name : DD_DMA_InitByChannel							
// Description   : DMAのチャネルごとの初期化
// Parameter     : dmaCh 	"DMAのチャネル"
//				 : type 	"起動要因種別"
//				 : ch 		"起動要因種別のチャネル"
// Return        : 設定結果
// Note          : 転送サイズは16ビット、DMA転送方向は内蔵RAM->SFRで固定
//				 : 転送元RAMアドレスと転送回数は転送開始時に設定するためここでは設定しない
//=============================================================================
SS DD_DMA_InitByChannel(UC dmaCh, DD_DMA_TRIGGER_TYPE type, UC trgCh)
{
	SS ret = DD_DMA_OK;
	
	UC IFC_Value = DD_DMA_Get_IFC_Pattern(dmaCh, type, trgCh);				// DMA起動要因設定値の取得
	
	if (IFC_Value != 0xFF) {
		switch (dmaCh) {
		case 0:		// Channel0
		    DRC0 = _80_DMA_OPERATION_ENABLE;
		    NOP();
		    NOP();
		    DMAMK0 = 1U; /* disable INTDMA0 interrupt */
		    DMAIF0 = 0U; /* clear INTDMA0 interrupt flag */
		    /* Set INTDMA0 low priority */
		    DMAPR10 = 1U;
		    DMAPR00 = 1U;
		    DMC0 = _40_DMA_TRANSFER_DIR_RAM2SFR | _20_DMA_DATA_SIZE_16 | IFC_Value;
		    DSA0 = _18_DMA0_SFR_ADDRESS;
		    DEN0 = 0U; /* disable DMA0 operation */
			break;

#ifndef STARTER_KIT
		case 1:		// Channel1
		    DRC1 = _80_DMA_OPERATION_ENABLE;
		    NOP();
		    NOP();
		    DMAMK1 = 1U; /* disable INTDMA1 interrupt */
		    DMAIF1 = 0U; /* clear INTDMA1 interrupt flag */
		    /* Set INTDMA1 low priority */
		    DMAPR11 = 1U;
		    DMAPR01 = 1U;
		    DMC1 = _40_DMA_TRANSFER_DIR_RAM2SFR | _20_DMA_DATA_SIZE_16 | IFC_Value;
		    DSA1 = _1A_DMA1_SFR_ADDRESS;
		    DEN1 = 0U; /* disable DMA1 operation */	
			break;

		case 2:			// Channel2
		    DRC2 = _80_DMA_OPERATION_ENABLE;
		    NOP();
		    NOP();
		    DMAMK2 = 1U; /* disable INTDMA2 interrupt */
		    DMAIF2 = 0U; /* clear INTDMA2 interrupt flag */
		    /* Set INTDMA2 low priority */
		    DMAPR12 = 1U;
		    DMAPR02 = 1U;
		    DMC2 = _40_DMA_TRANSFER_DIR_RAM2SFR | _20_DMA_DATA_SIZE_16 | IFC_Value;
		    DSA2 = _70_DMA2_SFR_ADDRESS;
		    DEN2 = 0U; /* disable DMA2 operation */
			break;
			
		case 3: 	// Channel3
		    DRC3 = _80_DMA_OPERATION_ENABLE;
		    NOP();
		    NOP();
		    DMAMK3 = 1U; /* disable INTDMA3 interrupt */
		    DMAIF3 = 0U; /* clear INTDMA3 interrupt flag */
		    /* Set INTDMA3 low priority */
		    DMAPR13 = 1U;
		    DMAPR03 = 1U;
		    DMC3 = _40_DMA_TRANSFER_DIR_RAM2SFR | _20_DMA_DATA_SIZE_16 | IFC_Value;
		    DSA3 = _72_DMA3_SFR_ADDRESS;
		    DEN3 = 0U; /* disable DMA3 operation */
			break;
			
		default:
			ret = DD_DMA_PARAM_NG;
			break;
		}
#endif
	}
	else { 
		ret = DD_DMA_PARAM_NG;
	}
	
	return ret;
}

//============================================================================= 
// Function Name : DD_DMA_Start							
// Description   : DMAの転送を開始し、DMA転送完了割込みを許可する									
// Parameter     : ch        : DMAのチャネル
//				 : startAddr : 転送元RAMアドレスの下16bit
//               : cnt       : 転送回数
// Return        : -									
// Note          : bitアクセスが配列化できないのでswitchで分岐
//=============================================================================
SS DD_DMA_Start(UC ch, US* startAddr, US cnt)
{
	UC ret = DD_DMA_OK;
	
	switch (ch) {
	case 0:
		DEN0 = 1U; /* enable DMA0 operation */
		DRA0 = (US)startAddr;
		DBC0 = cnt;
	    DEN0 = 0U; /* disable DMA0 operation */
		
	    DMAIF0 = 0U; /* clear INTDMA0 interrupt flag */
	    DMAMK0 = 0U; /* enable INTDMA0 interrupt */
	    DEN0 = 1U;
	    DST0 = 1U;
		break;
		
	case 1:
		DEN1 = 1U; /* enable DMA0 operation */
		DRA1 = (US)startAddr;
		DBC1 = cnt;
	    DEN1 = 0U; /* disable DMA0 operation */
		
	    DMAIF1 = 0U; /* clear INTDMA0 interrupt flag */
	    DMAMK1 = 0U; /* enable INTDMA0 interrupt */
	    DEN1 = 1U;
	    DST1 = 1U;
		break;
		
#ifndef STARTER_KIT
	case 2:
		DEN2 = 1U; /* enable DMA0 operation */
		DRA2 = (US)startAddr;
		DBC2 = cnt;
	    DEN2 = 0U; /* disable DMA0 operation */
		
	    DMAIF2 = 0U; /* clear INTDMA0 interrupt flag */
	    DMAMK2 = 0U; /* enable INTDMA0 interrupt */
	    DEN2 = 1U;
	    DST2 = 1U;
		break;
		
	case 3:
		DEN3 = 1U; /* enable DMA0 operation */
		DRA3 = (US)startAddr;
		DBC3 = cnt;
	    DEN3 = 0U; /* disable DMA0 operation */
		
	    DMAIF3 = 0U; /* clear INTDMA0 interrupt flag */
	    DMAMK3 = 0U; /* enable INTDMA0 interrupt */
	    DEN3 = 1U;
	    DST3 = 1U;
		break;
#endif
	default:
		ret = DD_DMA_PARAM_NG;
		break;
	}
	
	return ret;
}

//============================================================================= 
// Function Name : DD_DMA_Stop							
// Description   : DMAの転送を停止し、DMA転送完了割込みを停止する											
// Parameter     : ch	"DMAのチャネル"											
// Return        : なし									
// Note          : bitアクセスが配列化できないのでswitchで分岐
//=============================================================================
SS DD_DMA_Stop(UC ch)
{
	UC ret = DD_DMA_OK;
	
	switch (ch) {
	case 0:
	    if (DST0 != 0U) {
	        DST0 = 0U;
	    }
	    NOP();
	    NOP();
	    DEN0 = 0U; /* disable DMA0 operation */
	    DMAMK0 = 1U; /* disable INTDMA0 interrupt */
	    DMAIF0 = 0U; /* clear INTDMA0 interrupt flag */
		break;
		
	case 1:
	    if (DST1 != 0U) {
	        DST1 = 0U;
	    }
	    NOP();
	    NOP();
	    DEN1 = 0U; /* disable DMA0 operation */
	    DMAMK1 = 1U; /* disable INTDMA0 interrupt */
	    DMAIF1 = 0U; /* clear INTDMA0 interrupt flag */
		break;
		
#ifndef STARTER_KIT
	case 2:
	    if (DST2 != 0U) {
	        DST2 = 0U;
	    }
	    NOP();
	    NOP();
	    DEN2 = 0U; /* disable DMA0 operation */
	    DMAMK2 = 1U; /* disable INTDMA0 interrupt */
	    DMAIF2 = 0U; /* clear INTDMA0 interrupt flag */
		break;
		
	case 3:
	    if (DST3 != 0U) {
	        DST3 = 0U;
	    }
	    
	    NOP();
	    NOP();
	    DEN3 = 0U; /* disable DMA0 operation */
	    DMAMK3 = 1U; /* disable INTDMA0 interrupt */
	    DMAIF3 = 0U; /* clear INTDMA0 interrupt flag */
		break;
#endif
	default:
		ret = DD_DMA_PARAM_NG;
		break;
	}
	
	return ret;
}


//============================================================================= 
// Function Name : DD_DMA_EntryCallback
// Description   : DMA転送完了割込みからコールバックされる関数ポインタの登録API
// Parameter     : なし
// Return        : 設定結果
// Note          : なし
//=============================================================================
SS DD_DMA_EntryCallback(UC ch, void (*func)(void* arg), void* arg)
{
	UC ret = DD_DMA_OK;
	
	if (ch < DD_DMA_CHANNEL_NUM) {
		dmacCallbackTable[ch].pFunc = func;
		dmacCallbackTable[ch].arg = arg;
	}
	else {
		ret = DD_DMA_PARAM_NG;	
	}
	
	return ret;
}

//============================================================================= 
// Function Name : DD_DMA_GetRemainTransferCnt
// Description   : DMA転送の転送残り回数を取得する
// Parameter     : ch        : DMAのチャネル
// Return        : -
// Note          : -
//=============================================================================
US DD_DMA_GetRemainTransferCnt(UC ch)
{
	US cnt;

	switch (ch) {
	case 0:
		cnt = DBC0;
		break;

	case 1:
		cnt = DBC1;
		break;

	case 2:
		cnt = DBC2;
		break;

	case 3:
		cnt = DBC3;
		break;

	default:
		cnt = 0;
		break;
	}

	return cnt;
}

//============================================================================= 
// Function Name : DD_DMA0_interrupt
// Description   : DMA CH0の割込み関数
// Parameter     : なし
// Return        : なし
// Note          : 登録されているコールバック関数を呼び出す
//=============================================================================
__interrupt static void DD_DMA0_Interrupt(void)
{
	if (dmacCallbackTable[0].pFunc != 0) {
		dmacCallbackTable[0].pFunc(dmacCallbackTable[0].arg);
	}
}

//============================================================================= 
// Function Name : DD_DMA1_interrupt
// Description   : DMA CH1の割込み関数
// Parameter     : 
// Return        : 										
// Note          : 登録されているコールバック関数を呼び出す
//=============================================================================
__interrupt static void DD_DMA1_Interrupt(void)
{
	if (dmacCallbackTable[1].pFunc != 0) {
		dmacCallbackTable[1].pFunc(dmacCallbackTable[1].arg);
	}
}
#ifndef STARTER_KIT
//============================================================================= 
// Function Name : DD_DMA2_interrupt
// Description   : DMA CH2の割込み関数
// Parameter     : なし
// Return        : なし
// Note          : 登録されているコールバック関数を呼び出す
//=============================================================================
__interrupt static void DD_DMA2_Interrupt(void)
{
	if (dmacCallbackTable[2].pFunc != 0) {
		dmacCallbackTable[2].pFunc(dmacCallbackTable[2].arg);
	}
}

//============================================================================= 
// Function Name : DD_DMA3_interrupt							
// Description   : DMA CH3の割込み関数
// Parameter     : なし
// Return        : なし
// Note          : 登録されているコールバック関数を呼び出す
//=============================================================================
__interrupt static void DD_DMA3_Interrupt(void)
{
	if (dmacCallbackTable[3].pFunc != 0) {
		dmacCallbackTable[3].pFunc(dmacCallbackTable[3].arg);
	}
}

//============================================================================= 
// Function Name : DD_DMA_Get_IFC_Pattern							
// Description   : DMA起動要因の取得
// Parameter     : dmaCh 	"DMAのチャネル"
//				 : type 	"起動要因種別"
//				 : ch 		"起動要因種別のチャネル"
// Return        : DMAモード・コントロール・レジスタのIFC Bitに設定するパターン									
// Note          : DMAのチャネルによって指定できる起動要因が限定されるため
//               : 指定した起動要因が設定不可である場合には"0xFF"を返す
//=============================================================================
UC DD_DMA_Get_IFC_Pattern(UC dmaCh, DD_DMA_TRIGGER_TYPE type, UC trgCh)
{
	UC retPattern = 0xFF;
	
	if (dmaCh < DD_DMA_CHANNEL_NUM) {
		
		switch (type) {
		case DD_DMA_TRIGGER_TYPE_SOFTWARE:	retPattern = 0x00;	break;		// ソフトウェア（割込みによるDMA転送禁止)
		case DD_DMA_TRIGGER_TYPE_AD:		retPattern = 0x01;	break;		// A/D変換終了
		case DD_DMA_TRIGGER_TYPE_TM:										// タイマ・チャネル
			// DMAチャネルと指定起動要因の組み合わせチェック
			if (dmaCh <= 1) {												// DMA CH = 0, 1
				if ( trgCh > 0x03 ) { break; }
			}
			else {															// DMA CH = 2, 3
				if ( !(trgCh >= 0x10 && trgCh <= 0x13) ) { break;	}
			}
			
			switch (trgCh) {
			case 0x00:	retPattern = _02_DMA_TRIGGER_TM00;	break;
			case 0x01:	retPattern = _03_DMA_TRIGGER_TM01;	break;	
			case 0x02:	retPattern = _04_DMA_TRIGGER_TM02;	break;	
			case 0x03:	retPattern = _05_DMA_TRIGGER_TM03;	break;	
			case 0x10:	retPattern = _02_DMA_TRIGGER_TM10;	break;
			case 0x11:	retPattern = _03_DMA_TRIGGER_TM11;	break;
			case 0x12:	retPattern = _04_DMA_TRIGGER_TM12;	break;
			case 0x13:	retPattern = _05_DMA_TRIGGER_TM13;	break;
			default:										break;
			}
			break;
			
		case DD_DMA_TRIGGER_TYPE_UART_SEND:
			// DMAチャネルと指定起動要因の組み合わせチェック
			if (dmaCh <= 1) {												// DMA CH = 0, 1
				if ( trgCh > 0x02 ) { break; }								// UART CH = 0, 1, 2のみ設定有効
			}
			else {															// DMA CH = 2, 3
				if ( !(trgCh >= 0x01 && trgCh <= 0x03) ) { break; }			// UART CH = 1, 2, 3のみ設定有効
			}
			switch (trgCh) {
			case 0x00:	retPattern = _06_DMA_TRIGGER_ST0_CSI00;	break;
			case 0x01:	retPattern = _08_DMA_TRIGGER_ST1_CSI10;	break;	
			case 0x02:	retPattern = _0A_DMA_TRIGGER_ST2_CSI20;	break;	
			case 0x03:	retPattern = _06_DMA_TRIGGER_ST3_CSI30;	break;
			default:											break;
			}
			break;
			
		case DD_DMA_TRIGGER_TYPE_UART_RECV:
			// DMAチャネルと指定起動要因の組み合わせチェック
			if (dmaCh <= 1) {												// DMA CH = 0, 1
				if ( trgCh > 0x02 ) { break; }								// UART CH = 0, 1, 2のみ設定有効
			}
			else {															// DMA CH = 2, 3
				if ( !((trgCh >= 0x01) && (trgCh <= 0x03)) ) { break; }		// UART CH = 1, 2, 3のみ設定有効
			}
			switch (trgCh) {
			case 0x00:	retPattern = _07_DMA_TRIGGER_SR0_CSI01;	break;
			case 0x01:	retPattern = _09_DMA_TRIGGER_SR1_CSI11;	break;	
			case 0x02:	retPattern = _0B_DMA_TRIGGER_SR2_CSI21;	break;	
			case 0x03:	retPattern = _07_DMA_TRIGGER_SR3_CSI31;	break;
			default:											break;
			}
			break;

		default:												break;
		}
		
	}
	return retPattern;
}
#endif


// File Name: Timer.h
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
// 13.10.15 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev000 Base)
// Revision 000

#ifndef TAU_H
#define TAU_H


#define	DD_TAU_CPU_CLOCK_FREQ_MHZ	1	// 32MHzÇTPSÇ≈5ï™é¸ê›íËÇµÇƒÇ¢ÇÈ

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    Peripheral Enable Register 0 (PER0)
*/

/*
    Timer Clock Select Register m (TPSm)
*/
/* Operating mode and clear mode selection (PRSm03 - PRSm00) */
#define _0000_TAU_CKM0_FCLK_0                   (0x0000U) /* ckm0 - fCLK */
#define _0001_TAU_CKM0_FCLK_1                   (0x0001U) /* ckm0 - fCLK/2^1 */
#define _0002_TAU_CKM0_FCLK_2                   (0x0002U) /* ckm0 - fCLK/2^2 */
#define _0003_TAU_CKM0_FCLK_3                   (0x0003U) /* ckm0 - fCLK/2^3 */
#define _0004_TAU_CKM0_FCLK_4                   (0x0004U) /* ckm0 - fCLK/2^4 */
#define _0005_TAU_CKM0_FCLK_5                   (0x0005U) /* ckm0 - fCLK/2^5 */
#define _0006_TAU_CKM0_FCLK_6                   (0x0006U) /* ckm0 - fCLK/2^6 */
#define _0007_TAU_CKM0_FCLK_7                   (0x0007U) /* ckm0 - fCLK/2^7 */
#define _0008_TAU_CKM0_FCLK_8                   (0x0008U) /* ckm0 - fCLK/2^8 */
#define _0009_TAU_CKM0_FCLK_9                   (0x0009U) /* ckm0 - fCLK/2^9 */
#define _000A_TAU_CKM0_FCLK_10                  (0x000AU) /* ckm0 - fCLK/2^10 */
#define _000B_TAU_CKM0_FCLK_11                  (0x000BU) /* ckm0 - fCLK/2^11 */
#define _000C_TAU_CKM0_FCLK_12                  (0x000CU) /* ckm0 - fCLK/2^12 */
#define _000D_TAU_CKM0_FCLK_13                  (0x000DU) /* ckm0 - fCLK/2^13 */
#define _000E_TAU_CKM0_FCLK_14                  (0x000EU) /* ckm0 - fCLK/2^14 */
#define _000F_TAU_CKM0_FCLK_15                  (0x000FU) /* ckm0 - fCLK/2^15 */
/* Operating mode and clear mode selection (PRSm13 - PRSm10) */
#define _0000_TAU_CKM1_FCLK_0                   (0x0000U) /* ckm1 - fCLK */
#define _0010_TAU_CKM1_FCLK_1                   (0x0010U) /* ckm1 - fCLK/2^1 */
#define _0020_TAU_CKM1_FCLK_2                   (0x0020U) /* ckm1 - fCLK/2^2 */
#define _0030_TAU_CKM1_FCLK_3                   (0x0030U) /* ckm1 - fCLK/2^3 */
#define _0040_TAU_CKM1_FCLK_4                   (0x0040U) /* ckm1 - fCLK/2^4 */
#define _0050_TAU_CKM1_FCLK_5                   (0x0050U) /* ckm1 - fCLK/2^5 */
#define _0060_TAU_CKM1_FCLK_6                   (0x0060U) /* ckm1 - fCLK/2^6 */
#define _0070_TAU_CKM1_FCLK_7                   (0x0070U) /* ckm1 - fCLK/2^7 */
#define _0080_TAU_CKM1_FCLK_8                   (0x0080U) /* ckm1 - fCLK/2^8 */
#define _0090_TAU_CKM1_FCLK_9                   (0x0090U) /* ckm1 - fCLK/2^9 */
#define _00A0_TAU_CKM1_FCLK_10                  (0x00A0U) /* ckm1 - fCLK/2^10 */
#define _00B0_TAU_CKM1_FCLK_11                  (0x00B0U) /* ckm1 - fCLK/2^11 */
#define _00C0_TAU_CKM1_FCLK_12                  (0x00C0U) /* ckm1 - fCLK/2^12 */
#define _00D0_TAU_CKM1_FCLK_13                  (0x00D0U) /* ckm1 - fCLK/2^13 */
#define _00E0_TAU_CKM1_FCLK_14                  (0x00E0U) /* ckm1 - fCLK/2^14 */
#define _00F0_TAU_CKM1_FCLK_15                  (0x00F0U) /* ckm1 - fCLK/2^15 */
/* Operating mode and clear mode selection (PRSm21 - PRSm20) */
#define _0000_TAU_CKM2_FCLK_1                   (0x0000U) /* ckm2 - fCLK/2^1 */
#define _0100_TAU_CKM2_FCLK_2                   (0x0100U) /* ckm2 - fCLK/2^2 */
#define _0200_TAU_CKM2_FCLK_4                   (0x0200U) /* ckm2 - fCLK/2^4 */
#define _0300_TAU_CKM2_FCLK_6                   (0x0300U) /* ckm2 - fCLK/2^6 */
/* Operating mode and clear mode selection (PRSm31 - PRSm30) */
#define _0000_TAU_CKM3_FCLK_8                   (0x0000U) /* ckm2 - fCLK/2^8 */
#define _1000_TAU_CKM3_FCLK_10                  (0x1000U) /* ckm2 - fCLK/2^10 */
#define _2000_TAU_CKM3_FCLK_12                  (0x2000U) /* ckm2 - fCLK/2^12 */
#define _3000_TAU_CKM3_FCLK_14                  (0x3000U) /* ckm2 - fCLK/2^14 */

/*
    Timer Mode Register mn (TMRmn)
*/
/* Selection of macro clock (MCK) of channel n (CKSmn1 - CKSmn0) */
#define DD_TAU_CLK_SEL_CKM0                  (0x0000U) /* operation clock CK0 set by PRS register */ 
#define DD_TAU_CLK_SEL_CKM1                  (0x8000U) /* operation clock CK1 set by PRS register */
#define DD_TAU_CLK_SEL_CKM2                  (0x4000U) /* operation clock CK2 set by PRS register */
#define DD_TAU_CLK_SEL_CKM3                  (0xC000U) /* operation clock CK3 set by PRS register */
/* Selection of count clock (CCK) of channel n (CCSmn) */
#define DD_TAU_CLK_MODE_CKS                  (0x0000U) /* macro clock MCK specified by CKSmn bit */  
#define DD_TAU_CLK_MODE_TIMN                 (0x1000U) /* valid edge of input signal input from TImn pin */
/* Selection of slave/master of channel n (MASTERmn) */
#define DD_TAU_COMBINATION_SLAVE             (0x0000U) /* operates as slave channel */
#define DD_TAU_COMBINATION_MASTER            (0x0800U) /* operates as master channel */
/* Operation explanation of channel 1 or 3 (SPLIT) */
#define DD_TAU_16BITS_MODE                   (0x0000U) /* operates as 16 bits timer */  
#define DD_TAU_8BITS_MODE                    (0x0800U) /* operates as 8 bits timer */
/* Setting of start trigger or capture trigger of channel n (STSmn2 - STSmn0) */
#define DD_TAU_TRIGGER_SOFTWARE              (0x0000U) /* only software trigger start is valid */
#define DD_TAU_TRIGGER_TIMN_VALID            (0x0100U) /* TImn input edge is used as a start/capture trigger */
#define DD_TAU_TRIGGER_TIMN_BOTH             (0x0200U) /* TImn input edges are used as a start/capture trigger */
#define DD_TAU_TRIGGER_MASTER_INT            (0x0400U) /* interrupt signal of the master channel is used */
/* Selection of TImn pin input valid edge (CISmn1 - CISmn0) */
#define DD_TAU_TIMN_EDGE_FALLING             (0x0000U) /* falling edge */
#define DD_TAU_TIMN_EDGE_RISING              (0x0040U) /* rising edge */
#define DD_TAU_TIMN_EDGE_BOTH_LOW            (0x0080U) /* both edges (when low-level width is measured) */
#define DD_TAU_TIMN_EDGE_BOTH_HIGH           (0x00C0U) /* both edges (when high-level width is measured) */
/* Operation mode of channel n (MDmn3 - MDmn0) */
#define DD_TAU_MODE_INTERVAL_TIMER           (0x0000U) /* interval timer mode */
#define DD_TAU_MODE_CAPTURE                  (0x0004U) /* capture mode */
#define DD_TAU_MODE_EVENT_COUNT              (0x0006U) /* event counter mode */
#define DD_TAU_MODE_ONE_COUNT                (0x0008U) /* one count mode */
#define DD_TAU_MODE_HIGHLOW_MEASURE          (0x000CU) /* high-/low-level width measurement mode */
#define DD_TAU_MODE_PWM_MASTER               (0x0001U) /* PWM Function (Master Channel) mode */
#define DD_TAU_MODE_PWM_SLAVE                (0x0009U) /* PWM Function (Slave Channel) mode */
#define DD_TAU_MODE_ONESHOT                  (0x0008U) /* one-shot pulse output mode */
/* Setting of starting counting and interrupt (MDmn0) */
#define DD_TAU_START_INT_UNUSED              (0x0000U) /* interrupt is not generated when counting is started */
#define DD_TAU_START_INT_USED                (0x0001U) /* interrupt is generated when counting is started */

typedef enum {
	DD_TAU_MODE_INTERVAL_TIMER_ID,
	DD_TAU_MODE_SQUARE_PULSE_ID,
	DD_TAU_MODE_PWM_MASTER_ID,
	DD_TAU_MODE_PWM_SLAVE_ID,
	DD_TAU_MODE_EXTERNAL_EVENT_COUNTER_ID
} DD_TAU_ModeID;

typedef enum {
	DD_TAU_ACTIVE_HIGH_ID,
	DD_TAU_ACTIVE_LOW_ID,
	DD_TAU_ACTIVE_NO_CARE_ID
} DD_TAU_ActiveLevelID;

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void DD_TAU_Init(void);
void DD_TAU_InitByChannel(UC ch, DD_TAU_ModeID mode, DD_TAU_ActiveLevelID activeLvl);

void DD_TAU_Start(UC ch);
void DD_TAU_Start2CH(UC ch1, UC ch2);

void DD_TAU_Stop(UC ch);
void DD_TAU_Stop2CH(UC ch1, UC ch2);

void DD_TAU_SetTDR(UC ch, US cnt);

void DD_TAU_IntEnable(UC ch);
void DD_TAU_IntDisable(UC ch);

UC DD_TAU_GetLevel(UC ch);
UC DD_TAU_GetDataRegAddr(UC ch);

void DD_TAU_EntryCallback(UC ch, void (*func)(void* arg), void* arg);

#endif

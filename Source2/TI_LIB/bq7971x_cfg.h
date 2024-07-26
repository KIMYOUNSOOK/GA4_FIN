#ifndef BQ7971X_CFG_H
#define BQ7971X_CFG_H

#include "Platform_Types.h"
#include "bq7971x_regs.h"

/*********************************************************************************************************************
 * Exported Preprocessor #define Constants
 *********************************************************************************************************************/
#define TIBMS_MAX_BMICS                    (12u)

#define BQ_CELL_NUM_MAX                    (18u)
#define BQ_GPIO_NUM_MAX                    (11u)

#define BQ_CELL_NUM_ACT                    (18u)
#define BQ_GPIO_NUM_ACT                    (11u)

#define BQ_GPIO_USED_MASK                  (5u)
#define BQ_FAULT_NUM_MAX                   (15u)

#define BQ_DIE_TEMP_NUM                    (2u)

#define BQ_DIAG_MAX_RETRY                  (3u)
#define BQ_DIAG_STAT_RETRY                 (1u)
#define BQ_DIAG_INIT_RETRY                 (3u)
#define BQ_DIAG_CB_TIME_DEFAULT            (1u)

#define BQ_GPIO_PLAU_DB_MAX                (5u)

#define BQ_CB_MODE_MANUAL                  (0u)
#define BQ_CB_MODE_AUTO                    (2u)
#define BQ_BAL_STAT_READ				   (0u)

#define BQ_CELL_VOLTAGE_REG_OFFSET         (BQ_VCELL18_HI_OFFSET + \
                                                ((BQ_CELL_NUM_MAX - BQ_CELL_NUM_ACT) * 2)) //READ_2BYTE))

/** Set the timer for cell balancing
 * 0x01 = 30s
 * 0x02 = 60s
 * 0x03 = 300s
 * 0x04 = 600s
 * 0x05 to 0x0F = range from 20min to 120min in 10min steps
 * 0x10 to 0x1F = range from 150min to 600min in 30min steps
 */
#define BQ_CELL_CB_TIME_DEFAULT            (1u)

#define BQ_WAKEUP_PING_DELAY               (4u)   	/* 3.5ms 	*/
#define BQ_WAKEUP_TONE_DELAY               (10u)  	/* 10ms 	*/
#define BQ_HW_RESET_DELAY                  (45u)  	/* 45ms 	*/
#define BQ_SLP2ACT_TONE_DELAY              (1u)    /* 500us 	*/
#define BQ_GOTO_SLEEP_DELAY                (1u)    /* 100us 	*/
#define BQ_SOFT_RESET_DELAY                (1u)   	/* 1ms 		*/
#define BQ_GOTO_SHUTDOWN_DELAY             (5u)   	/* 5ms 		*/

#define BQ_VAL_MAXVOLREGVALUE              (0xFFFEu)
#define BQ_VAL_INVALDVOLVALUE              (0xFFFFu)

#define BQ_CELLVCCBSHRT_THRESHOLD          (5000u)
#define BQ_CABLE_RES_AB_THRESHOLD          (3000u)

#define BQ_MAXVCELLPLAUVALUE               (48000u)
#define BQ_MIDVCELLPLAUVALUE               (25000u)
#define BQ_MINVCELLPLAUVALUE               (2000u)
#define BQ_VCELL_THRESHHOLD                (10000u)    /* 1V */

#define BQ_GPIOOPENWIRE_THRESHOLD          (10000u)    /* 1V */
#define BQ_GPIOADJSHORT_THRESHOLD          (1000u)     /* 100mV */

#define BQ_MAXGPIOPLAUVALUE                (40000u)   /* 4V - SEM CSU Board */
#define BQ_MINGPIOPLAUVALUE                (5000u)    /* 500mv - SEM CSU Board */
#define BQ_GPIOOW_PULLDET                  (20000u)   /* 2V difference to decide pull down or pull up */

#define BQ_MAXBGPLAUVALUE                  (13000u)
#define BQ_MINBGPLAUVALUE                  (11000u)
#define BQ_MAXREFCAPPLAUVALUE              (40250u)
#define BQ_MINREFCAPPLAUVALUE              (39750u)

#define BQ_MAXDIETEMPPLAUVALUE             (1200u)     /* 120(degress) */
#define BQ_MINDIETEMPPLAUVALUE             (65186u)    /* -35(degress) */
#define BQ_TEMPDIFFPLAUVALUE               (100u)      /* 10 degrees Celsius */
#define BQ_DIETEMPSTUCKVALUE               (3u)        /* 3 FDTI cycles */

#define BQ_LSB_STANDARD                    (10u)
#define BQ_STEP_CENTI_NTC                  (400u)

#define BQ_VCCBSHRT_DELAY                  (1u)        /* Wait 1.1ms for an accurate ADC result in the result registers. */
#define BQ_ACOMP_DONE_DELAY                (9u)
#define BQ_DCOMP_DONE_DELAY                (100u)

#define BQ_GPIO_CONF_DELAY                 (6u)        /* If (GP_DR==1) wait at least 2.8ms; else wait at least 5.5ms */

#define BQ_SMOVDAC_ERR                     (1000u)
#define BQ_SMUVDAC_ERR                     (1000u)
#define BQ_OV_THR_RES                      (2700u)
#define BQ_OV_THR_STEP                     (25u)
#define BQ_OV_THR_REG_VAL1                 (0x0Du)
#define BQ_OV_THR_REG_VAL2                 (0x40u)
#define BQ_OV_THR_REG_VAL3                 (0x51u)
#define BQ_OV_THR_REG_VAL4                 (0x80u)
#define BQ_OV_THR_REG_VAL5                 (0xA6u)
#define BQ_OV_THR_VAL1                     (3600u)
#define BQ_OV_THR_VAL2                     (4175u)
#define BQ_UV_THR_RES                      (1200u)
#define BQ_UV_THR_STEP                     (50u)
#define BQ_UV_THR_REG_VAL                  (0x32u)
#define BQ_UV_THR_VAL                      (3700u)
#define BQ_OVUV_CTRL2_OVUV_THR_LOCK        (4u)
#define BQ_OVUV_CTRL2_OVUV_RR              (2u)
#define BQ_OVUV_CTRL1_OV_THR               (0x00u)
#define BQ_OVUV_CTRL1_UV_THR               (0x40u)

#define BQ_SMOTDAC_ERR                     (400u)
#define BQ_SMUTDAC_ERR                     (400u)
#define BQ_OT_THR_RES                      (10u)
#define BQ_OT_THR_STEP                     (1u)
#define BQ_OT_THR_REG_VAL                  (0x15u)
#define BQ_OT_THR_VAL                      (30u)
#define BQ_UT_THR_RES                      (72u)
#define BQ_UT_THR_STEP                     (2u)
#define BQ_OTUT_CTRL2_OTUT_THR_LOCK        (4u)
#define BQ_OTUT_CTRL2_OTUT_RR              (2u)
#define BQ_OTUT_CTRL1_OT_THR               (0u)
#define BQ_OTUT_CTRL1_UT_THR               (0x20u)


/*********************************************************************************************************************
 * Exported Type Declarations
 *********************************************************************************************************************/

typedef struct 
{
    uint32 uNCellsCfg;
    uint16 uNGpioCfg;
    uint16 uNBBCfg;

    uint8 uDev_Conf[2];
    uint8 uGpio_Conf[6];
    uint8 uBBVC_Posn[3];
    uint8 uUV_Disable[3];  

}BQ_DevCfg;

typedef struct 
{
    uint8 uDev_Conf[2];    /* 0x002 - 0x003 */
    uint8 uComm_Conf;       /* 0x004 */
    uint8 uBBVC_Posn[3];   /* 0x005 - 0x007 Whenever user select and set a channel in BBVC_POSN*, user shall select the same channel setting to 1 in UV_DISABLE* */
    
    uint8 uAdc_Conf;        /* 0x008 */
    uint8 uOVUV_Thresh[2]; /* 0x009 - 0x00A */
    uint8 uOTUT_Thresh;     /* 0x00B */
    uint8 uUV_Disable[3];  /* 0x00C - 0x00E */
    uint8 uFault_Msk[2];   /* 0x00F - 0x010 */
    
    uint8 uCS_Adc_Cal[2];  /* 0x011 - 0x012 */
    uint8 uOC_Conf[2];     /* 0x013 - 0x014 */
    uint8 uIddq_Conf;       /* 0x01D */
    uint8 uGpio_Conf[6];   /* 0x01E - 0x023 */
    uint8 uVcell_Offset[9];/* 0x024 - 0x02C */

}BQ_NVMRegs;

typedef struct 
{
    uint8 uControl[2];       /* 0x309 - 0x30A */
    uint8 uAdc_Ctrl[4];      /* 0x310 - 0x313 */

    uint8 uDiag_Adc_Ctrl[3]; /* 0x314 - 0x316 */
    uint8 uDiag_Misc_Ctrl[2];/* 0x317 - 0x318 */
    
    uint8 uCB_Cell_Ctrl[18]; /* 0x320 - 0x331 */
    uint8 uVCBDone_Thresh;    /* 0x332 */
    uint8 uOTCB_Thresh;       /* 0x333 */

    uint8 uOVUV_Ctrl[2];     /* 0x334 - 0x335 */
    uint8 uOTUT_Ctrl[2];     /* 0x336 - 0x337 */
    uint8 uBal_Ctrl[3];      /* 0x338 - 0x339 */

    uint8 uFault_RST[2u];     /* 0x340 - 0x341 */
    uint8 uOC_Ctrl;           /* 0x360 */

}BQ_CtrlRegs;

typedef struct 
{
    const BQ_DevCfg      *m_pDevCfg;
    BQ_NVMRegs            m_tNVM;
    BQ_CtrlRegs           m_tCtrl;
 
}BQ_Config;


/*********************************************************************************************************************
 * Exported Object Declarations
 *********************************************************************************************************************/
extern const BQ_Config  bqConfig;

#endif /*BQ7971X_CFG_H*/

/*********************************************************************************************************************
 * End of File: bq7971x_cfg.h
 *********************************************************************************************************************/

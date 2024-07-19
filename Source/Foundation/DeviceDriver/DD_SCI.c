// File Name: DD_SCI.c
// Description:
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 15.01.23
// Revision:  002
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+--------------------------------------------------------------------------------------
// 13.10.15 | Nishizawa Mitsuyuki | Created this file.(CH-Finisher SubCPU Rev002 Base)
// Revision 000
// 15.01.21 | Morikawa Atsuhsi    | Log Function Addition
// Revision 001
// 15.01.23 | Morikawa Atsuhsi    | Design Improvement
// Revision 002

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* for IOT(RxD0/TxD0) */
#pragma interrupt INTST0 DD_SCI_UART0_IntSend
#pragma interrupt INTSR0 DD_SCI_UART0_IntReceive
#pragma interrupt INTSRE0 DD_SCI_UART0_IntError

/* for Finisher Log(TxD1) */
#pragma interrupt INTST1 DD_SCI_UART1_IntSend

/* for Finisher Log(TxD3) */
#pragma interrupt INTST3 DD_SCI_UART3_IntSend
// #pragma interrupt INTSR3 DD_SCI_UART3_IntReceive
// #pragma interrupt INTSRE3 DD_SCI_UART3_IntError

#include "Common.h"
#include "DD_Event.h"
#include "DD_COM_ParPacketSender.h"
#include "DD_COM_ParPacketReceiver.h"
#include "DD_SCI.h"
#include "DD_LOG.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
static volatile UC* gp_uart0_tx_address;        /* uart0 transmit buffer address */
static volatile US  g_uart0_tx_count;           /* uart0 transmit data number */
static volatile UC* gp_uart0_rx_address;        /* uart0 receive buffer address */
static volatile US  g_uart0_rx_count;           /* uart0 receive data number */
static volatile US  g_uart0_rx_length;          /* uart0 receive data length */

static volatile UC* gp_uart1_tx_address;        /* uart1 transmit buffer address */
static volatile US  g_uart1_tx_count;           /* uart1 transmit data number */

static volatile UC* gp_uart3_tx_address;        /* uart0 transmit buffer address */
static volatile US  g_uart3_tx_count;           /* uart0 transmit data number */
static volatile UC* gp_uart3_rx_address;        /* uart0 receive buffer address */
static volatile US  g_uart3_rx_count;           /* uart0 receive data number */
static volatile US  g_uart3_rx_length;          /* uart0 receive data length */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */


void DD_SCI_SAU0_Create(void);
void DD_SCI_UART0_Create(void);
void DD_SCI_UART0_Start(void);
void DD_SCI_UART1_Create(void);
void DD_SCI_UART1_Start(void);

void DD_SCI_SAU1_Create(void);
void DD_SCI_UART3_Create(void);
void DD_SCI_UART3_Start(void);


//==============================================================================
// Function Name : DD_SCI_Init
// Description   : 通信を初期化する
// Parameter     : なし
// Return        : なし
//==============================================================================
void DD_SCI_Init(void)
{
	DD_SCI_SAU0_Create();
	DD_SCI_SAU1_Create();
	DD_SCI_UART0_Create();
	DD_SCI_UART0_Start();
#ifdef UART3_DEBUG
	DD_SCI_UART3_Create();
	DD_SCI_UART3_Start();
#else
	DD_SCI_UART1_Create();
	DD_SCI_UART1_Start();
#endif
}

/***********************************************************************************************************************
* Function Name: DD_SCI_SAU0_Create
* Description  : This function initializes the SAU0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void DD_SCI_SAU0_Create(void)
{
    SAU0EN = 1U;    /* supply SAU0 clock */
    NOP();
    NOP();
    NOP();
    NOP();
   // SPS0 = _0002_SAU_CK00_FCLK_2 | _0020_SAU_CK01_FCLK_2;
    SPS0 = _0003_SAU_CK00_FCLK_3 ;		//19200bps
    //SPS0 = 0x02;			// 動作クロック8MHｚ　38400bps)
}

/***********************************************************************************************************************
* Function Name: DD_SCI_UART0_Create
* Description  : This function initializes the UART0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void DD_SCI_UART0_Create(void)
{

    ST0 |= _0002_SAU_CH1_STOP_TRG_ON | _0001_SAU_CH0_STOP_TRG_ON;    /* disable UART0 receive and transmit */
    STMK0 = 1U;    /* disable INTST0 interrupt */
    STIF0 = 0U;    /* clear INTST0 interrupt flag */
    SRMK0 = 1U;    /* disable INTSR0 interrupt */
    SRIF0 = 0U;    /* clear INTSR0 interrupt flag */
    SREMK0 = 1U;   /* disable INTSRE0 interrupt */
    SREIF0 = 0U;   /* clear INTSRE0 interrupt flag */
    /* Set INTST0 level1 priority */
    STPR10 = 0U;
    STPR00 = 1U;
    /* Set INTSR0 level1 priority */
    SRPR10 = 0U;
    SRPR00 = 1U;
    /* Set INTSRE0 level1 priority */
    SREPR10 = 0U;
    SREPR00 = 1U;


    SMR00 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0000_SAU_TRIGGER_SOFTWARE |
            _0002_SAU_MODE_UART | _0001_SAU_BUFFER_EMPTY;	//**0x23
    SCR00 = _8000_SAU_TRANSMISSION | _0000_SAU_INTSRE_MASK | _0300_SAU_PARITY_ODD | _0080_SAU_LSB | _0010_SAU_STOP_1 |
            _0007_SAU_LENGTH_8;//**0x8397
//   SDR00 = _8000_UART0_TRANSMIT_DIVISOR;
//     SDR00 = 0x6700U;//**
    SDR00 = 0xCE00U;//**
    NFEN0 |= _01_SAU_RXD0_FILTER_ON;//**0x01U;

    SIR01 = _0004_SAU_SIRMN_FECTMN | _0002_SAU_SIRMN_PECTMN | _0001_SAU_SIRMN_OVCTMN;    /* clear error flag */
    SMR01 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0100_SAU_TRIGGER_RXD | _0000_SAU_EDGE_FALL |
            _0002_SAU_MODE_UART | _0000_SAU_TRANSFER_END;//**0x122
    SCR01 = _4000_SAU_RECEPTION | _0400_SAU_INTSRE_ENABLE | _0300_SAU_PARITY_ODD | _0080_SAU_LSB | _0010_SAU_STOP_1 |
            _0007_SAU_LENGTH_8;//**0x4797
//  SDR01 = _8000_UART0_RECEIVE_DIVISOR;
//  SDR01 = 0x6700U;//**
    SDR01 = 0xCE00U;//**
    SO0 |= _0001_SAU_CH0_DATA_OUTPUT_1;
//  SO0 = 0x0001U;//**
    SOL0 |= _0000_SAU_CHANNEL0_NORMAL;    /* output level normal *///**
    SOE0 |= _0001_SAU_CH0_OUTPUT_ENABLE;    /* enable UART0 output */
//  SOE0 = 0x0001U;//**    /* enable UART0 output */

     /* Set RxD0 pin */
    PM1 |= 0x02U;
    /* Set TxD0 pin */
    P1 |= 0x04U;
    PM1 &= 0xFBU;

}

/***********************************************************************************************************************
* Function Name: DD_SCI_UART0_Start
* Description  : This function starts the UART0 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void DD_SCI_UART0_Start(void)
{
    STIF0 = 0U;    /* clear INTST0 interrupt flag */
    STMK0 = 0U;    /* enable INTST0 interrupt */
    SRIF0 = 0U;    /* clear INTSR0 interrupt flag */
    SRMK0 = 0U;    /* enable INTSR0 interrupt */
    SREIF0 = 0U;   /* clear INTSRE0 interrupt flag */
    SREMK0 = 0U;   /* enable INTSRE0 interrupt */
    SO0 |= _0001_SAU_CH0_DATA_OUTPUT_1;    /* output level normal */
    SOE0 |= _0001_SAU_CH0_OUTPUT_ENABLE;    /* enable UART0 output */
    SS0 |= _0002_SAU_CH1_START_TRG_ON | _0001_SAU_CH0_START_TRG_ON;    /* enable UART0 receive and transmit */
}

/***********************************************************************************************************************
* Function Name: DD_SCI_UART0_Stop
* Description  : This function stops the UART0 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void DD_SCI_UART0_Stop(void)
{
    ST0 |= _0002_SAU_CH1_STOP_TRG_ON | _0001_SAU_CH0_STOP_TRG_ON;    /* disable UART0 receive and transmit */
    SOE0 &= ~_0001_SAU_CH0_OUTPUT_ENABLE;    /* disable UART0 output */
    STMK0 = 1U;    /* disable INTST0 interrupt */
    STIF0 = 0U;    /* clear INTST0 interrupt flag */
    SRMK0 = 1U;    /* disable INTSR0 interrupt */
    SRIF0 = 0U;    /* clear INTSR0 interrupt flag */
    SREMK0 = 1U;   /* disable INTSRE0 interrupt */
    SREIF0 = 0U;   /* clear INTSRE0 interrupt flag */
}

/***********************************************************************************************************************
* Function Name: DD_SCI_UART1_Create
* Description  : This function initializes the UART1 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void DD_SCI_UART1_Create(void)
{
    ST0 |= _0008_SAU_CH3_STOP_TRG_ON | _0004_SAU_CH2_STOP_TRG_ON;    /* disable UART1 receive and transmit */
    STMK1 = 1U;    /* disable INTST1 interrupt */
    STIF1 = 0U;    /* clear INTST1 interrupt flag */
    SRMK1 = 1U;    /* disable INTSR1 interrupt */
    SRIF1 = 0U;    /* clear INTSR1 interrupt flag */
    SREMK1 = 1U;   /* disable INTSRE0 interrupt */
    SREIF1 = 0U;   /* clear INTSRE0 interrupt flag */
    /* Set INTST1 level1 priority */
    STPR11 = 0U;
    STPR01 = 1U;
    /* Set INTSR1 level1 priority */
    SRPR11 = 0U;
    SRPR01 = 1U;
    /* Set INTSRE1 level1 priority */
    SREPR11 = 0U;
    SREPR01 = 1U;

    SMR02 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0000_SAU_TRIGGER_SOFTWARE |
            _0002_SAU_MODE_UART | _0001_SAU_BUFFER_EMPTY;	//** 0x23
    SCR02 = _8000_SAU_TRANSMISSION | _0000_SAU_INTSRE_MASK | _0300_SAU_PARITY_ODD | _0080_SAU_LSB | _0010_SAU_STOP_1 |
            _0007_SAU_LENGTH_8;// **0x8397

    SDR02 = 0xCE00U; // ** For 19200bps

    SO0 |= _0004_SAU_CH2_DATA_OUTPUT_1;
    SOL0 |= _0000_SAU_CHANNEL2_NORMAL;    	/* output level normal *///**
    SOE0 |= _0004_SAU_CH2_OUTPUT_ENABLE;    /* enable UART0 output */

    /* Set TxD1 pin, P02 is TxD1 */
    P0 |= 0x04U; 		/* Hihg Output */
    PM0 &= 0xFBU;		/* TxD1 Output Mode */
}

/***********************************************************************************************************************
* Function Name: DD_SCI_UART1_Start
* Description  : This function starts the UART1 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void DD_SCI_UART1_Start(void)
{
    STIF1 = 0U;    /* clear INTST1 interrupt flag */
    STMK1 = 0U;    /* enable INTST1 interrupt */
    SRIF1 = 0U;    /* clear INTSR1 interrupt flag */
    SRMK1 = 0U;    /* enable INTSR1 interrupt */
    SREIF1 = 0U;   /* clear INTSRE1 interrupt flag */
    SREMK1 = 0U;   /* enable INTSRE1 interrupt */
    SO0 |= _0004_SAU_CH2_DATA_OUTPUT_1;    	/* output level normal */
    SOE0 |= _0004_SAU_CH2_DATA_OUTPUT_1;    /* enable UART1 output */
    SS0 |= /* _0008_SAU_CH3_START_TRG_ON | */ _0004_SAU_CH2_START_TRG_ON;    /* enable UART1 transmit */
}

/***********************************************************************************************************************
* Function Name: DD_SCI_OpPacketSend
* Description  : This function sends UART0 data.
* Arguments    : tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
* Return Value : status -
*                    0x00 or MD_ARGERROR
***********************************************************************************************************************/
UC DD_SCI_OpPacketSend(UC* tx_buf, US tx_num)
{
   UC status = MD_OK;

    if (tx_num < 1U)
    {
        status = MD_ARGERROR;
    }
    else
    {
        gp_uart0_tx_address = tx_buf;
        g_uart0_tx_count = tx_num;
        SMR00 |= _0001_SAU_BUFFER_EMPTY;
        STMK0 = 1U;    /* disable INTST0 interrupt */
        TXD0 = *gp_uart0_tx_address;
        gp_uart0_tx_address++;
        g_uart0_tx_count--;
        STMK0 = 0U;    /* enable INTST0 interrupt */
    }

    return (status);
}

/***********************************************************************************************************************
* Function Name: DD_SCI_UART0_IntReceive
* Description  : This function is INTSR0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void DD_SCI_UART0_IntReceive(void)
{
    UC rx_data;

    rx_data = RXD0;

    // Create(evGoIdle_ID);
    DD_EVT_Put(evStart_ID, evuDD_COM_ParPacketReceiver_ID, (UC*)rx_data, 1);

}

/***********************************************************************************************************************
* Function Name: DD_SCI_UART0_IntError
* Description  : This function is INTSRE0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void DD_SCI_UART0_IntError(void)
{
 //   UC err_type;

    *gp_uart0_rx_address = RXD0;
    DD_COM_RecvErr();
    SIR01 = 0x07;
}

/***********************************************************************************************************************
* Function Name: DD_SCI_UART0_IntSend
* Description  : This function is INTST0 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void DD_SCI_UART0_IntSend(void)
{
    if (g_uart0_tx_count > 0U)
    {
        TXD0 = *gp_uart0_tx_address;
        gp_uart0_tx_address++;
        g_uart0_tx_count--;
    }
    else
    {
        DD_COM_OpComplete_INTERRUPT();
    }
}

/***********************************************************************************************************************
* Function Name: DD_SCI_SAU1_Create
* Description  : This function initializes the SAU1 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void DD_SCI_SAU1_Create(void)
{
    SAU1EN = 1U;    /* supply SAU1 clock */
    NOP();
    NOP();
    NOP();
    NOP();
    SPS1 = _0002_SAU_CK00_FCLK_2 ;		// fCLK/(2^2)
}


/***********************************************************************************************************************
* Function Name: DD_SCI_UART3_Create
* Description  : This function initializes the UART3 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void DD_SCI_UART3_Create(void)
{
    ST1 |= _0008_SAU_CH3_STOP_TRG_ON | _0004_SAU_CH2_STOP_TRG_ON;    /* disable UART0 receive and transmit */

	/* Configure Interrupt */
    STMK3 = 1U;    /* disable INTST3  interrupt */
    STIF3 = 0U;    /* clear   INTST3  interrupt flag */
    SRMK3 = 1U;    /* disable INTSR3  interrupt */
    SRIF3 = 0U;    /* clear   INTSR3  interrupt flag */
    SREMK3 = 1U;   /* disable INTSRE3 interrupt */
    SREIF3 = 0U;   /* clear   INTSRE3 interrupt flag */
    /* Set INTST3 level1 priority */
    STPR13 = 0U;
    STPR03 = 1U;
    /* Set INTSR3 level1 priority */
    SRPR13 = 0U;
    SRPR03 = 1U;
    /* Set INTSRE3 level1 priority */
    SREPR13 = 0U;
    SREPR03 = 1U;

	/* Configure UART3/TxD3 Register XXXmn : m=1, n=2 */
    SMR12 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0000_SAU_TRIGGER_SOFTWARE |
            _0002_SAU_MODE_UART | _0001_SAU_BUFFER_EMPTY;
    SCR12 = _8000_SAU_TRANSMISSION | _0000_SAU_INTSRE_MASK | _0300_SAU_PARITY_ODD | _0080_SAU_LSB | _0010_SAU_STOP_1 |
            _0007_SAU_LENGTH_8;
    SDR12 = 0xCE00U;
    SOL1 |= _0000_SAU_CHANNEL2_NORMAL;
    SO1  |= _0004_SAU_CH2_DATA_OUTPUT_1;
    SOE1 |= _0004_SAU_CH2_OUTPUT_ENABLE;

    P14   |= 0x10U;	/* P144 is TxD3 */
    PM14  &= 0xEFU;

//	/* Configure UART3/RxD3 Register XXXmn : m=1, n=3 */
//    NFEN1 |= _40_SAU_RXD3_FILTER_ON;
//    SIR13 = _0004_SAU_SIRMN_FECTMN | _0002_SAU_SIRMN_PECTMN | _0001_SAU_SIRMN_OVCTMN;
//    SMR13 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0100_SAU_TRIGGER_RXD | _0000_SAU_EDGE_FALL |
//            _0002_SAU_MODE_UART | _0000_SAU_TRANSFER_END;
//    SCR13 = _4000_SAU_RECEPTION | _0400_SAU_INTSRE_ENABLE | _0300_SAU_PARITY_ODD | _0080_SAU_LSB | _0010_SAU_STOP_1 |
//            _0007_SAU_LENGTH_8;
//    SDR13 = 0xCE00U;
//    PM14 |= 0xF7U;	/* P143 */
}

/***********************************************************************************************************************
* Function Name: DD_SCI_UART3_Start
* Description  : This function starts the UART3 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void DD_SCI_UART3_Start(void)
{
    STIF3 = 0U;    /* clear  INTST3  interrupt flag */
    STMK3 = 0U;    /* enable INTST3  interrupt */
    SRIF3 = 0U;    /* clear  INTSR3  interrupt flag */
    SRMK3 = 0U;    /* enable INTSR3  interrupt */
    SREIF3 = 0U;   /* clear  INTSRE3 interrupt flag */
    SREMK3 = 0U;   /* enable INTSRE3 interrupt */

    SO1  |= _0004_SAU_CH2_DATA_OUTPUT_1;
    SOE1 |= _0004_SAU_CH2_OUTPUT_ENABLE;    /* enable UART3 output */
    SS1  |= /*_0008_SAU_CH3_START_TRG_ON |*/ _0004_SAU_CH2_START_TRG_ON;    /* enable UART3 receive and transmit */
}

/***********************************************************************************************************************
* Function Name: DD_SCI_UART3_Stop
* Description  : This function stops the UART0 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void DD_SCI_UART3_Stop(void)
{
    ST1  |= /* _0008_SAU_CH3_STOP_TRG_ON | */ _0004_SAU_CH2_STOP_TRG_ON;    /* disable UART3 receive and transmit */
    SOE1 &= ~_0004_SAU_CH2_OUTPUT_ENABLE;    /* disable UART3 output */

    STMK3 = 1U;    /* disable INTST3  interrupt */
    STIF3 = 0U;    /* clear   INTST3  interrupt flag */
    SRMK3 = 1U;    /* disable INTSR3  interrupt */
    SRIF3 = 0U;    /* clear   INTSR3  interrupt flag */
    SREMK3 = 1U;   /* disable INTSRE3 interrupt */
    SREIF3 = 0U;   /* clear   INTSRE3 interrupt flag */
}

/***********************************************************************************************************************
* Function Name: DD_SCI_LogPacketSend
* Description  : This function sends UART3 data.
* Arguments    : tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
* Return Value : status -
*                    0x00 or MD_ARGERROR
***********************************************************************************************************************/
UC DD_SCI_LogPacketSend(const UC* tx_buf, US tx_num)
{
   UC status = MD_OK;

    if (tx_num < 1U) {
        status = MD_ARGERROR;
    } else {

#ifdef UART3_DEBUG
        gp_uart3_tx_address = tx_buf;
        g_uart3_tx_count = tx_num;
        SMR12 |= _0001_SAU_BUFFER_EMPTY;
        STMK3 = 1U;    /* disable INTST3 interrupt */
        TXD3 = *gp_uart3_tx_address;
        gp_uart3_tx_address++;
        g_uart3_tx_count--;
        STMK3 = 0U;    /* enable INTST3 interrupt */
#else
        gp_uart1_tx_address = tx_buf;
        g_uart1_tx_count = tx_num;
        SMR02 |= _0001_SAU_BUFFER_EMPTY;
        STMK1 = 1U;    /* disable INTST1 interrupt */
        TXD1 = *gp_uart1_tx_address;
        gp_uart1_tx_address++;
        g_uart1_tx_count--;
        STMK1 = 0U;    /* enable INTST1 interrupt */
#endif

    }
    return (status);
}

/***********************************************************************************************************************
* Function Name: DD_SCI_UART1_IntSend
* Description  : This function is INTST1 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void DD_SCI_UART1_IntSend(void)
{
    if (g_uart1_tx_count > 0U)
    {
        TXD1 = *gp_uart1_tx_address;
        gp_uart1_tx_address++;
        g_uart1_tx_count--;
    }
    else
    {
        STMK1 = 1U;    /* disable INTST1 interrupt */
        DD_EVT_Put(evComplete_ID, evuDD_LOG_ID, 0, 0);
    }
}

/***********************************************************************************************************************
* Function Name: DD_SCI_UART3_IntReceive
* Description  : This function is INTSR1 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
/*
__interrupt static void DD_SCI_UART3_IntReceive(void)
{
    UC rx_data;

    rx_data = RXD3;

    // Create(evGoIdle_ID);
    //DD_EVT_Put(evStart_ID, evuDD_COM_ParPacketReceiver_ID, (UC*)rx_data, 1);

}
*/
/***********************************************************************************************************************
* Function Name: DD_SCI_UART3_IntError
* Description  : This function is INTSRE3 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
/*
__interrupt static void DD_SCI_UART3_IntError(void)
{
 //   UC err_type;

    *gp_uart3_rx_address = RXD3;
    DD_COM_RecvErr();
    SIR13 = 0x07;
}
*/
/***********************************************************************************************************************
* Function Name: DD_SCI_UART3_IntSend
* Description  : This function is INTST3 interrupt service routine.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
__interrupt static void DD_SCI_UART3_IntSend(void)
{
    if (g_uart3_tx_count > 0U)
    {
        TXD3 = *gp_uart3_tx_address;
        gp_uart3_tx_address++;
        g_uart3_tx_count--;
    }
    else
    {
        STMK3 = 1U;    /* disable INTST3 interrupt */
        DD_EVT_Put(evComplete_ID, evuDD_LOG_ID, 0, 0);
    }
}

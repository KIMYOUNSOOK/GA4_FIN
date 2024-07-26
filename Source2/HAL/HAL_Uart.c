
#include "HAL_Uart.h"
#include "CmicM.h"
/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define SERIAL_BAUDRATE        1000000 //                                 /* Baud rate in bit/s                   */

#ifndef SURESOFT 
//UART 무선IC 1 
   //UART_RX_MC_A(RX)   -->P14.9 - IfxAsclin20_RXB_P14_9_IN  //module20
   //UART_TX_MC_A(TX)   -->P14.8  -IfxAsclin20_TX_P14_8_OUT //module20
 
//UART 무선IC 2    
   //UART_RX_MC_B(RX)   -->P11.0 - IfxAsclin3_RXB_P11_0_IN  //module3
   //UART_TX_MC_B(TX)   -->P11.1 - IfxAsclin3_TX_P11_1_OUT //module3
#endif

#define SERIAL_PIN_RX_S         IfxAsclin3_RXB_P11_0_IN     /* Secondary RX pin of the board                  */
#define SERIAL_PIN_TX_S         IfxAsclin3_TX_P11_1_OUT     /* Secondary TX pin of the board                  */

#define SERIAL_PIN_RX_P         IfxAsclin20_RXB_P14_9_IN    /* Primary RX pin of the board                  */
#define SERIAL_PIN_TX_P         IfxAsclin20_TX_P14_8_OUT    /* Primary TX pin of the board                  */

#define INTPRIO_ASCLIN_TX_S      17                                          /* Priority of the ISR                  */
#define INTPRIO_ASCLIN_RX_S      18

#define INTPRIO_ASCLIN_TX_P      19                                          /* Priority of the ISR                  */
#define INTPRIO_ASCLIN_RX_P      20

//(TXBufferSize변경 : SETDEVTBL에서 최대30*(MacID(8)+1)+6만큼의 데이터전송이 필요) 
#define TX_BUFFER_SIZE          512 //128                                         /* Definition of the buffer size        */
#define RX_BUFFER_SIZE          1024 //512 //256   
#define RX_BUF_MAX              20//16
#define DEBUG_MAX               100 //200

typedef struct {

    uint8   m_uIdx;       // 인덱스포인터
 
    struct BufMgr_Tag
    {
        uint16  uRxLen;
        uint8   uRxBuf[256];

    }BufMgr[RX_BUF_MAX];


#ifndef SUREDBG
    uint8  dTxIdx_Dbg;
    uint8  dRxIdx_Dbg;

    struct Debug_TXTag
    {
        uint8   dTXidx;
        uint16  dTxLength;        
        uint8   dTxBuf[256];//[TX_BUFFER_SIZE];
    }Debug_Tx[DEBUG_MAX];

    struct Debug_RXTag
    {
        uint8  dRXidx;
       uint16  dRxLength; 
       uint8   dRxBuf[256];
    }Debug_Rx[DEBUG_MAX];
#endif

}UartTransferMgr;

UartTransferMgr zUartMgr[UART_NUM];

/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/
IfxAsclin_Asc g_ascP;         /* Primary Declaration of the ASC handle        */
IfxAsclin_Asc g_ascS;        /*  Secondary Declaration of the ASC handle        */

/* The transfer buffers allocate memory for the data itself and for FIFO runtime variables.
 * 8 more bytes have to be added to ensure a proper circular buffer handling independent from
 * the address to which the buffers have been located.
 */
uint8 g_ascP_TxBuffer[TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];             /* Declaration of the FIFOs parameters  */
uint8 g_ascP_RxBuffer[RX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];             /* Declaration of the FIFOs parameters  */

uint8 g_ascS_TxBuffer[TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];             /* Declaration of the FIFOs parameters  */
uint8 g_ascS_RxBuffer[RX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];             /* Declaration of the FIFOs parameters  */

uint8 rxData_P[256];
uint8 rxData_S[256];

/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/
IFX_INTERRUPT(UART0_TxNotify, 0, INTPRIO_ASCLIN_TX_P);                         /* Adding the Interrupt Service Routine */
IFX_INTERRUPT(UART0_RxNotify, 0, INTPRIO_ASCLIN_RX_P); 
IFX_INTERRUPT(UART1_TxNotify, 0, INTPRIO_ASCLIN_TX_S);                         /* Adding the Interrupt Service Routine */
IFX_INTERRUPT(UART1_RxNotify, 0, INTPRIO_ASCLIN_RX_S); 

void UART0_TxNotify(void)
{
    IfxAsclin_Asc_isrTransmit(&g_ascP);
}

void UART0_RxNotify(void)
{    
    IfxAsclin_Asc_isrReceive(&g_ascP); 
}

void UART1_TxNotify(void)
{
    IfxAsclin_Asc_isrTransmit(&g_ascS);
}

void UART1_RxNotify(void)
{    
    IfxAsclin_Asc_isrReceive(&g_ascS); 
}

/*******************************************************************************/
/*  Primary CC2662 ~ UART Initialize                                             */
/*******************************************************************************/
void UART0_Init(void)
{
    /* Initialize an instance of IfxAsclin_Asc_Config with default values */
    IfxAsclin_Asc_Config ascConfig;

    IfxAsclin_Asc_initModuleConfig(&ascConfig, SERIAL_PIN_TX_P.module);

    /* Set the desired baud rate */
    ascConfig.baudrate.baudrate = SERIAL_BAUDRATE;

    /* ISR priorities and interrupt target */
    ascConfig.interrupt.txPriority = INTPRIO_ASCLIN_TX_P;
    ascConfig.interrupt.rxPriority = INTPRIO_ASCLIN_RX_P;
    ascConfig.interrupt.typeOfService = IfxCpu_Irq_getTos(IfxCpu_getCoreIndex());


    ascConfig.baudrate.oversampling = IfxAsclin_OversamplingFactor_16; 
    ascConfig.fifo.txFifoInterruptMode = IfxAsclin_FifoInterruptMode_batch;
    ascConfig.fifo.rxFifoInterruptMode = IfxAsclin_FifoInterruptMode_batch;

    /* FIFO configuration */
    ascConfig.txBuffer = &g_ascP_TxBuffer;
    ascConfig.txBufferSize = TX_BUFFER_SIZE;

    ascConfig.rxBuffer = &g_ascP_RxBuffer;
    ascConfig.rxBufferSize = RX_BUFFER_SIZE ; 

    // Loopback mode for test by sure.
  //  ascConfig.loopBack = TRUE;

    /* Port pins configuration */
    const IfxAsclin_Asc_Pins pins =
    {
        NULL_PTR,           IfxPort_InputMode_pullUp,     /* CTS pin not used     */
        &SERIAL_PIN_RX_P,   IfxPort_InputMode_pullUp,     /* RX pin not used      */
        NULL_PTR,           IfxPort_OutputMode_pushPull,  /* RTS pin not used     */
        &SERIAL_PIN_TX_P,   IfxPort_OutputMode_pushPull,  /* TX pin               */
        IfxPort_PadDriver_cmosAutomotiveSpeed1
    };
    ascConfig.pins = &pins;

    IfxAsclin_Asc_initModule(&g_ascP, &ascConfig);                       /* Initialize module with above parameters  */

    //UartMgr initialize
    MemSet(zUartMgr, 0, sizeof(UartTransferMgr));
}

/*******************************************************************************/
/*  Secondary CC2662 ~ UART Initialize                                         */
/*******************************************************************************/
void UART1_Init(void)
{
    /* Initialize an instance of IfxAsclin_Asc_Config with default values */
    IfxAsclin_Asc_Config ascConfig;

    IfxAsclin_Asc_initModuleConfig(&ascConfig, SERIAL_PIN_TX_S.module);

    /* Set the desired baud rate */
    ascConfig.baudrate.baudrate = SERIAL_BAUDRATE;

    /* ISR priorities and interrupt target */
    ascConfig.interrupt.txPriority = INTPRIO_ASCLIN_TX_S;
    ascConfig.interrupt.rxPriority = INTPRIO_ASCLIN_RX_S;
    ascConfig.interrupt.typeOfService = IfxCpu_Irq_getTos(IfxCpu_getCoreIndex());

    ascConfig.baudrate.oversampling = IfxAsclin_OversamplingFactor_16; 
    ascConfig.fifo.txFifoInterruptMode = IfxAsclin_FifoInterruptMode_batch;
    ascConfig.fifo.rxFifoInterruptMode = IfxAsclin_FifoInterruptMode_batch;

    /* FIFO configuration */
    ascConfig.txBuffer = &g_ascS_TxBuffer;
    ascConfig.txBufferSize = TX_BUFFER_SIZE;

    ascConfig.rxBuffer = &g_ascS_RxBuffer;
    ascConfig.rxBufferSize = RX_BUFFER_SIZE ; 

    // Loopback mode for test by sure.
  //  ascConfig.loopBack = TRUE;

    /* Port pins configuration */
    const IfxAsclin_Asc_Pins pins =
    {
        NULL_PTR,         IfxPort_InputMode_pullUp,     /* CTS pin not used     */
        &SERIAL_PIN_RX_S,   IfxPort_InputMode_pullUp,     /* RX pin not used      */
        NULL_PTR,         IfxPort_OutputMode_pushPull,  /* RTS pin not used     */
        &SERIAL_PIN_TX_S,   IfxPort_OutputMode_pushPull,  /* TX pin               */
        IfxPort_PadDriver_cmosAutomotiveSpeed1
    };
    ascConfig.pins = &pins;

    IfxAsclin_Asc_initModule(&g_ascS, &ascConfig);                       /* Initialize module with above parameters  */
    
}

void UART_SendData(eUART uart, uint8 *const pTx, uint16 iLength)
{
    Ifx_SizeT       aLength = iLength;
 
    if (uart == UART_P){        
        IfxAsclin_Asc_write( &g_ascP, pTx, &aLength, TIME_INFINITE);   
    }
    else { //Secondary
        IfxAsclin_Asc_write( &g_ascS, pTx, &aLength, TIME_INFINITE);   
    }

    #ifndef SUREDBG
        uint8 count = zUartMgr[uart].dTxIdx_Dbg;
        zUartMgr[uart].Debug_Tx[count].dTXidx = count;
    
        zUartMgr[uart].Debug_Tx[count].dTxLength = aLength;
        MemSet(zUartMgr[uart].Debug_Tx[count].dTxBuf, 0, sizeof(zUartMgr[uart].Debug_Tx[count].dTxBuf));
        MemCpy(zUartMgr[uart].Debug_Tx[count].dTxBuf, pTx, aLength);
        
        if(++zUartMgr[uart].dTxIdx_Dbg >= DEBUG_MAX){
            zUartMgr[uart].dTxIdx_Dbg=0;
        }
  
    #endif      

}

/*  --FirstRead-----------------------||---SecondRead-------------|
	SOF(1)+LENGTH(2)+CMDTYPE(1)+CMDID(1)+ DATA(N)+FCS(1)  
*/
uint8  UART_Polling_Cb(eUART  uart)
{
    // UartTransferMgr  *pMgr;
    Ifx_SizeT       aLength = 5; //byte read.
    uint8           uIdx;

    if (uart == UART_P){

        MemSet(rxData_P, 0, sizeof(rxData_P));

        IfxAsclin_Asc_read(&g_ascP, rxData_P, &aLength, TIME_INFINITE);
        
        if (rxData_P[0] == NPI_MSG_SOF){

            aLength = (Ifx_SizeT)((rxData_P[2] << 8) | (rxData_P[1] & 0x00FF));
            aLength += 1;  // FCS
                    
            IfxAsclin_Asc_read(&g_ascP, &rxData_P[NPI_IDX_DATA], &aLength, TIME_INFINITE);

            uIdx = zUartMgr[UART_P].m_uIdx;

            MemSet((zUartMgr[UART_P].BufMgr[uIdx].uRxBuf), 0, 256);
            zUartMgr[UART_P].BufMgr[uIdx].uRxLen = (uint16)(aLength + 5);

            MemCpy((zUartMgr[UART_P].BufMgr[uIdx].uRxBuf), rxData_P, zUartMgr[UART_P].BufMgr[uIdx].uRxLen);
            

            return TRUE;
        }else {
            return FALSE;
        }
                

    } else { //SECONDARY

        MemSet(rxData_S, 0, sizeof(rxData_S));

        IfxAsclin_Asc_read(&g_ascS, rxData_S, &aLength, TIME_INFINITE);

        if (rxData_S[0] == NPI_MSG_SOF){

            aLength = (Ifx_SizeT)((rxData_S[2] << 8) | (rxData_S[1] & 0x00FF));
            aLength += 1;  // FCS

            IfxAsclin_Asc_read(&g_ascS, &rxData_S[NPI_IDX_DATA], &aLength, TIME_INFINITE);

            uIdx = zUartMgr[UART_S].m_uIdx;

            MemSet((zUartMgr[UART_S].BufMgr[uIdx].uRxBuf), 0, 256);
            zUartMgr[UART_S].BufMgr[uIdx].uRxLen = (uint16)(aLength + 5);

            MemCpy((zUartMgr[UART_S].BufMgr[uIdx].uRxBuf), rxData_S, zUartMgr[UART_S].BufMgr[uIdx].uRxLen);
           
            return TRUE;
        }else {
            return FALSE;
        }

    }

}

uint8  UART_ReceiveData(eUART uart, uint8 *pRxBuf, uint16 *pLength)
{
    uint8  uCurrentIdx;
    uint8    count;

    if((NULL != pRxBuf) && (NULL != pLength))
    {
        uCurrentIdx = zUartMgr[uart].m_uIdx;

        MemCpy(pRxBuf, zUartMgr[uart].BufMgr[uCurrentIdx].uRxBuf, 
                    zUartMgr[uart].BufMgr[uCurrentIdx].uRxLen);

        *pLength = zUartMgr[uart].BufMgr[uCurrentIdx].uRxLen;          
        
        if(++zUartMgr[uart].m_uIdx >= RX_BUF_MAX){
            zUartMgr[uart].m_uIdx = 0;
        }

        #ifndef SUREDBG  //RxBuffer push. 

            count = zUartMgr[uart].dRxIdx_Dbg;
            zUartMgr[uart].Debug_Rx[count].dRXidx = count;
            zUartMgr[uart].Debug_Rx[count].dRxLength = *pLength;
            MemSet(zUartMgr[uart].Debug_Rx[count].dRxBuf, 0, sizeof(zUartMgr[uart].Debug_Rx[count].dRxBuf));
            MemCpy(zUartMgr[uart].Debug_Rx[count].dRxBuf, pRxBuf, *pLength);

            if(++zUartMgr[uart].dRxIdx_Dbg >= DEBUG_MAX){
                zUartMgr[uart].dRxIdx_Dbg = 0;
            }

        #endif
        
    }
    return FALSE;
}


/*  --FirstRead-----------------------||---SecondRead-------------|
	SOF(1)+LENGTH(2)+CMDTYPE(1)+CMDID(1)+ DATA(N)+FCS(1)
    UART의 RxBuffer index를 넘겨줌
*/
#define FIRST_READ  5
uint8  UART_SEQ_Polling_Cb(uint8 *pPIdx, uint8 *pSIdx)
{
    Ifx_SizeT       aLength = FIRST_READ; //byte read.
    uint8           uIdx;
    uint8           IsMACStat = FALSE; //Check if receive CB or not

    //SECONDARY
    zUartMgr[UART_S].m_uIdx = uIdx = 0;
    MemSet(zUartMgr[UART_S].BufMgr, 0, sizeof(zUartMgr[UART_S].BufMgr));
    IsMACStat=FALSE;

    do{
     
        aLength = FIRST_READ;     
        IfxAsclin_Asc_read(&g_ascS, zUartMgr[UART_S].BufMgr[uIdx].uRxBuf, &aLength, TIME_INFINITE);

        if (zUartMgr[UART_S].BufMgr[uIdx].uRxBuf[0] == NPI_MSG_SOF){

            aLength = (Ifx_SizeT)((zUartMgr[UART_S].BufMgr[uIdx].uRxBuf[2] << 8) \
                        | (zUartMgr[UART_S].BufMgr[uIdx].uRxBuf[1] & 0x00FF)) +1; //aLength += 1;  // FCS

            IfxAsclin_Asc_read(&g_ascS, &zUartMgr[UART_S].BufMgr[uIdx].uRxBuf[NPI_IDX_DATA], &aLength, TIME_INFINITE);
            zUartMgr[UART_S].BufMgr[uIdx].uRxLen = (uint16)(aLength + FIRST_READ);
       
            if( zUartMgr[UART_S].BufMgr[uIdx].uRxBuf[NPI_IDX_CMDID] ==  eCMD_CB_TXCNF){ //macStatus                        
                IsMACStat = TRUE;
                zUartMgr[UART_S].m_uIdx = uIdx;
                *pSIdx = ++zUartMgr[UART_S].m_uIdx; 
            } 
            if (++uIdx >= RX_BUF_MAX)
                uIdx = 0;
        }else {
            return FALSE;
        }
    }while(!IsMACStat);

   //PRIMARY
   zUartMgr[UART_P].m_uIdx = uIdx = 0;
   MemSet(zUartMgr[UART_P].BufMgr, 0, sizeof(zUartMgr[UART_P].BufMgr));
   IsMACStat=FALSE;

   do{
        aLength = FIRST_READ;     
        IfxAsclin_Asc_read(&g_ascP, zUartMgr[UART_P].BufMgr[uIdx].uRxBuf, &aLength, TIME_INFINITE);

        if (zUartMgr[UART_P].BufMgr[uIdx].uRxBuf[0] == NPI_MSG_SOF){

            aLength = (Ifx_SizeT)((zUartMgr[UART_P].BufMgr[uIdx].uRxBuf[2] << 8) \
                       | (zUartMgr[UART_P].BufMgr[uIdx].uRxBuf[1] & 0x00FF)) +1 ; //  aLength += 1;  // FCS

            IfxAsclin_Asc_read(&g_ascP, &zUartMgr[UART_P].BufMgr[uIdx].uRxBuf[NPI_IDX_DATA], &aLength, TIME_INFINITE);

            zUartMgr[UART_P].BufMgr[uIdx].uRxLen = (uint16)(aLength + FIRST_READ);

       
            if( zUartMgr[UART_P].BufMgr[uIdx].uRxBuf[NPI_IDX_CMDID] ==  eCMD_CB_TXCNF){ //macStatus                        
                IsMACStat = TRUE;
                zUartMgr[UART_P].m_uIdx = uIdx;
                *pPIdx = ++zUartMgr[UART_P].m_uIdx; 
            } 
             if (++uIdx >= RX_BUF_MAX)
                 uIdx = 0;
        }else{
            return FALSE;
        }

    }while(!IsMACStat);

    return TRUE;
 
}

void  UART_SEQ_ReceiveData(eUART uart, uint8 Index, uint8 *pRxBuf, uint16 *pLength)
{
    uint8  uCurrentIdx;
    uint8  count;

    if((NULL != pRxBuf) && (NULL != pLength))
    {
        uCurrentIdx = Index; 
       
        MemCpy(pRxBuf, zUartMgr[uart].BufMgr[uCurrentIdx].uRxBuf, 
                    zUartMgr[uart].BufMgr[uCurrentIdx].uRxLen);

        *pLength = zUartMgr[uart].BufMgr[uCurrentIdx].uRxLen;          


        #ifndef SUREDBG  //RxBuffer push. 

            count = zUartMgr[uart].dRxIdx_Dbg;
            zUartMgr[uart].Debug_Rx[count].dRXidx = count;
            zUartMgr[uart].Debug_Rx[count].dRxLength = *pLength;
            MemSet(zUartMgr[uart].Debug_Rx[count].dRxBuf, 0, sizeof(zUartMgr[uart].Debug_Rx[count].dRxBuf));
            MemCpy(zUartMgr[uart].Debug_Rx[count].dRxBuf, pRxBuf, *pLength);

            if(++zUartMgr[uart].dRxIdx_Dbg >= DEBUG_MAX){
                zUartMgr[uart].dRxIdx_Dbg = 0;
            }

        #endif
        
    }
    return;
}

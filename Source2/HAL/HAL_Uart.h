#ifndef HAL_UART_H_
#define HAL_UART_H_

#include "IfxAsclin_Asc.h"
#include "IfxCpu_Irq.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef enum{
    UART_P=0,
    UART_S,
    UART_NUM,
}eUART;

typedef enum{
    NO_RECV=0x00,
    P_RECV=0x01,
    S_RECV=0x02,
    ALL_RECV = 0x03,
}eRECV_Status;

/*********************************************************************************************************************/
/*------------------------------------------------Function Prototypes------------------------------------------------*/
/*********************************************************************************************************************/
 /* Initialization function  */
void UART0_Init(void);
void UART1_Init(void);

void    UART_SendData(eUART uart, uint8 *const pTx, uint16 iLength);
uint8   UART_Polling_Cb(eUART  uart);
uint8   UART_ReceiveData(eUART uart, uint8 *pRxBuf, uint16 *pLength);

uint8   UART_SEQ_Polling_Cb(uint8 *pPIdx, uint8 *pSIdx);
void    UART_SEQ_ReceiveData(eUART uart, uint8 Index, uint8 *pRxBuf, uint16 *pLength);

#endif /* HAL_UART_H_ */

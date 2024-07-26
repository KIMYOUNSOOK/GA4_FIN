#include "IfxStm.h"
#include "CmicM.h"

#define HAL_TICKER_BT_TICK_MSEC   (1)               /* 1ms */
#define HAL_TICKER_BT_TICK_10MSEC   (10)               /* 1ms */
#define HAL_TICKER_BT_STM         &MODULE_STM1   /* Which STM timer to use */

static uint32  HalTickerTicks; 
static uint32  HalTickerTicks_10ms; 

void    TickerInit(void)
{
    /* The STM timers run automatically out of reset, so no hardware configuration required at init. */

    /* freq = 100MHz(fsrc/3),   */
    HalTickerTicks = (uint32)IfxStm_getTicksFromMilliseconds(HAL_TICKER_BT_STM, HAL_TICKER_BT_TICK_MSEC);
    HalTickerTicks_10ms = (uint32)IfxStm_getTicksFromMilliseconds(HAL_TICKER_BT_STM, HAL_TICKER_BT_TICK_10MSEC);
    return;
}

void   TickerStart(void)
{
    /* The STM timers run automatically out of reset, so no hardware configuration required at start. */

    return;   
}

uint32_t  TickerGetTimestamp(void)
{
    /* read current value of STM (STM is 64-bit timer) and divide to convert to milliseconds */
    uint64 iNow = IfxStm_get(HAL_TICKER_BT_STM) / (uint64)HalTickerTicks;
    return (uint32_t)iNow;
}

uint32_t   GetTick_1ms(void)
{
     /* read current value of STM (STM is 64-bit timer) and divide to convert to milliseconds */
   uint64 iNow = IfxStm_get(HAL_TICKER_BT_STM) / (uint64)HalTickerTicks;
   return (uint32_t)iNow;   
}

uint32_t   GetTick_10ms(void)
{
     /* read current value of STM (STM is 64-bit timer) and divide to convert to milliseconds */
   uint64 iNow = IfxStm_get(HAL_TICKER_BT_STM) / (uint64)HalTickerTicks_10ms;
   return (uint32_t)iNow;   
}


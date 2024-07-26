#include "IfxStm.h"
#include "CmicM.h"

//Callback Function type
//typedef void (*HAL_timer_callback_t)(void);

#define HAL_TMR_Period_MicroSEC     100000 //2500
#define HAL_TMR_STM                 &MODULE_STM0
#define HAL_TMR_COMPARATOR          IfxStm_Comparator_1  /* which of the two available comparators to use to track elapsed time. see IfxStm_Comparator */
#define ISR_PRIORITY_HAL_TMR        41 /* Priority for HAL TMR interrupt */

static uint32   HalTmrPeriodTicks; /* HAL_TMR_Period_MicroSEC converted to STM timer ticks */
static HAL_timer_callback_t  pfTmrCb = (void *)0;    /* function pointer, points back to application callback */

IFX_INTERRUPT(HAL_Timer_ISR, 0, ISR_PRIORITY_HAL_TMR);

void HAL_Timer_ISR(void)
{
     /* increment the comparator value by the number of ticks to set next interrupt */
    IfxStm_increaseCompare(HAL_TMR_STM, HAL_TMR_COMPARATOR, HalTmrPeriodTicks);
    
    /* call the application callback */
    if (pfTmrCb != (void *)0)
    {
        IfxCpu_enableInterrupts();
        pfTmrCb();
    } 
}

void  Tmr_Init(void)
{

    HalTmrPeriodTicks = (uint32)IfxStm_getTicksFromMicroseconds(HAL_TMR_STM, HAL_TMR_Period_MicroSEC);
   
}

uint8 Tmr_Start(HAL_timer_callback_t pfCb)
{
   /* config and init 3msc period and interrupt */
    IfxStm_CompareConfig HalTmr;
    uint8  result = FALSE;

    IfxStm_initCompareConfig(&HalTmr);           /* Initialize the configuration structure with default values   */

    HalTmr.comparator          = HAL_TMR_COMPARATOR; 
    HalTmr.comparatorInterrupt = IfxStm_ComparatorInterrupt_ir1; 
    HalTmr.compareOffset       = IfxStm_ComparatorOffset_0; 
    HalTmr.compareSize         = IfxStm_ComparatorSize_32Bits; 
    HalTmr.ticks               = HalTmrPeriodTicks;               /* Set the number of ticks after which the timer triggers an  interrupt for the first time */
    HalTmr.triggerPriority     = ISR_PRIORITY_HAL_TMR;                /* Set the priority of the interrupt */
    HalTmr.typeOfService       = IfxSrc_Tos_cpu0;                 /* Set the service provider for the interrupts */

    /* Configure the STM peripheral by passing the user configuration */
    /* IfxStm_initCompare returns true if init was successful */
    result = IfxStm_initCompare(HAL_TMR_STM, &HalTmr);

    /* store the callback */
    if (result == TRUE)
    {
        pfTmrCb = pfCb;
    }

    return result;   
}

void  Tmr_Stop(void)
{
    /* to stop TMR, just disable interrupt. Since the STM timer may be used by other features, this timer should be left running. */
    IfxStm_disableComparatorInterrupt(HAL_TMR_STM, HAL_TMR_COMPARATOR);

    /* clear the callback as well */
    pfTmrCb = (void *)0;

}
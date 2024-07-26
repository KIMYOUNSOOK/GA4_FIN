#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "IfxStm.h"
#include "Platform_Types.h"

#define HAL_TICKER_BT_STM         &MODULE_STM1   /* Which STM timer to use */

#define Overall_ true
#define Interval false
#define LogStart true
#define LogEnd__ false
#define ON true
#define OFF false

typedef enum
{
    _____________________________________________ = 0,
    Network_Start________________________________ = 100,
    Network_eCMD_CF_SETNWOPMODE_S________________ = 101,
    Network_eCMD_CF_SETNWOPMODE_P________________ = 102,
    Network_eCMD_CF_SETPARAMS____________________ = 103,
    Network_eCMD_CF_SETMAINCONFIG________________ = 104,
    Network_eCMD_CF_SETJOINMODE__________________ = 105,
    Network_eCMD_CF_SETDEVTBLCFG_________________ = 106,
    Network_eCMD_CF_STARTNETWORK_________________ = 107,
    Network_eCMD_WM_NUM_WD_CONN__________________ = 108,
    BQConfigInit_StackRead_BQ_DEV_STAT1__________ = 202,
    BQConfigInit_StackRead_BQ_CUST_CRC_RSLT_HI___ = 203,
    BQConfigInit_StackRead_BQ_DIAG_STAT1_________ = 204,
    BQConfigInit_StackRead_BQ_VCELL18_HI_________ = 205,
    Total_boot_time______________________________ = 999
} ADK_LOG_FUNCTION;

typedef struct
{
    uint16 STEP;
    ADK_LOG_FUNCTION API;
    float DURATION;    
    uint32 timestamp[3]; // START, END, TOTAL
} BOOTTIMESTR[50];

void CmicM_debug_BootTimeLog(bool final, bool start, uint16 step, ADK_LOG_FUNCTION api );
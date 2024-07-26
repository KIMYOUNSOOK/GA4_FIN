
#include "CmicM.h"
#include "Debug_BootTimeLog.h"


static const MAC_DevTblCfg  zMACCfgNODE_ID[MAX_NUM_DEVS] =
{
	 {{ 0x76, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  0},
	 {{ 0x7B, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  1},
	//{{ 0x63, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  2},
	// {{ 0x7D, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  3},


	{{ 0x77, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  0},
	// {{ 0x55, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  1},
	// {{ 0x7F, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  2},
	// {{ 0x50, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  3},

	{{ 0x6B, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  0},
	// {{ 0x77, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  1},
	// {{ 0x55, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  2},
	// {{ 0x7F, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  3},
	// {{ 0x50, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  4},
	{{ 0x80, 0xDA, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  5},

	// {{ 0x5E, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  6},
	// {{ 0x71, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  7},
	// {{ 0x43, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  8},
	// {{ 0x7E, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  9},
	// {{ 0x65, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  10},

	// {{ 0x67, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  11},

	{{ 0x9E, 0xDA, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  0},
	// {{ 0x9C, 0xDA, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  1},
	// {{ 0x70, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  2},
	// {{ 0x49, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  3},
	

	// {{ 0x96, 0xDA, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  8},
	// {{ 0x52, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  9},
 	// {{ 0x66, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  10},
	// {{ 0x68, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  11},

	// {{ 0x6B, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  12},
	// {{ 0x77, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  13},
	// {{ 0x55, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  14},
	// {{ 0x7F, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  15},
	// {{ 0x50, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  16},
	// {{ 0x80, 0xDA, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  17},

	// {{ 0x5E, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  18},
	// {{ 0x71, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  19},
	// {{ 0x43, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  20},
	// {{ 0x7E, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  21},
	// {{ 0x65, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  22},

	// {{ 0x67, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  23},

	// {{ 0x5D, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  24},
	// {{ 0x40, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  25},
	//{{ 0x4F, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  24},
//	{{ 0x0A, 0x9C, 0x07, 0xB0, 0xF3, 0x40, 0xFF, 0xFF },  25},
	// {{ 0x69, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  25},
//	{{ 0x8E, 0x99, 0x07, 0xB0, 0xF3, 0x40, 0xFF, 0xFF },  26},
	// {{ 0x5C, 0xD9, 0x9A, 0xB5, 0x68, 0x34, 0xFF, 0xFF },  26},

};

typedef struct
{
	CmicM_State_t			m_tSt;
	TxRxCmd_t				m_tTxRx[UART_NUM];
	CmicM_ReqFrame			m_ReqFrame[UART_NUM];
	CmicM_RespFrame			m_RespFrame[UART_NUM];

	WbmsAppWMainInfo		m_tQueryInfo[UART_NUM];
	WbmsAppStateType 		m_tWbmsState[UART_NUM];
	WbmsAppDataType 		m_tWbmsData[UART_NUM];
	WbmsRawDataType			m_tRawData[UART_NUM];
	WbmsFdtiResultType		m_tFdtiResult[UART_NUM];
	WbmsAppInfoType         m_tInfo; //측정시간
	uint16 					m_nTaskCnt;
	uint16 					m_nWaitCnt;
	uint16					m_nNGCnt;  //Error Count.
	uint16					m_nTxConfirmNGCnt;
	uint16 					m_nBQStatNGCnt[2];
	uint16 					m_nMacStatFailCnt[2];
	uint8					m_nDiagFaultCnt[2];
	uint32  				m_nTick1ms;
	uint32  				m_nTick10ms;
	uint32					m_nDebug1ms;
	uint16 					m_nDelay;  //각 상태마다 필요한 Delay설정.

	uint8					m_uNAFEs[UART_NUM];
	uint8					m_uSiglewriteRes[UART_NUM][2];

	uint8					m_cHealthNode;

	uint8					m_uBalGo;
	uint8					m_uNumOfCmd;
} CmicM_Instance_t;

static CmicM_Instance_t   gInst;


static void CmicM_ControlBootState(void);
static void CmicM_ControlRunState(void);
static void CmicM_ControlKeyOffState(void);
static void CmicM_ControlKeyOnState(void);
static void CmicM_ControlBalState(void);
//static void  MonitorEventRun(void);
static void CmicM_GetIndex(uint8 *pPindex, uint8 *pSindex);
static void CmicM_SetIndex(uint8 Pindex, uint8 Sindex);
static uint8 CmicM_GetPollStatus(void);
static void	CmicM_SetPollStatus(uint8 eStat);
static void CmicM_Control_Poll_XmsState(void);

//////////////////////////////////////////////////////////////////////////////////////

static void	CmicM_Network_ST0_IDLE(void);
static void	CmicM_Network_ST0_IDLE_RES(void);
static void	CmicM_Network_ST1_REQ(void);
static void	CmicM_Network_ST1_RES(void);
static void	CmicM_Network_ST2_REQ(void);
static void	CmicM_Network_ST2_RES(void);
static void	CmicM_Network_ST3_REQ(void);
static void	CmicM_Network_ST3_RES(void);
static void	CmicM_Network_ST4_REQ(void);
static void	CmicM_Network_ST4_RES(void);
static void	CmicM_Network_ST5_REQ(void);
static void	CmicM_Network_ST5_RES(void);
static void	CmicM_Network_ST6_REQ(void);
static void	CmicM_Network_ST6_RES(void);
static void	CmicM_Network_ST6_1_REQ(void);
static void	CmicM_Network_ST6_1_RES(void);
static void	CmicM_Network_ST7_REQ(void);
static void	CmicM_Network_ST7_RES(void);
static void	CmicM_Network_ST8_REQ(void);
static void	CmicM_Network_ST8_RES(void);

static void	CmicM_BQRegister_ST0_IDLE(void);
static void	CmicM_BQRegister_ST1_REQ(void);
static void	CmicM_BQRegister_ST1_RES(void);
static void	CmicM_BQRegister_ST2_REQ(void);	//Diag StartupInit
static void	CmicM_BQRegister_ST2_RES(void); 	//
static void	CmicM_BQRegister_ST3_REQ(void);  
static void	CmicM_BQRegister_ST3_RES(void);
static void	CmicM_BQRegister_ST4_REQ(void);  
static void	CmicM_BQRegister_ST4_RES(void);
static void	CmicM_BQRegister_ST5_REQ(void);  
static void	CmicM_BQRegister_ST5_RES(void);

static void	CmicM_BQRegister_ST6_REQ(void);  
static void	CmicM_BQRegister_ST6_WAIT(void);  
static void	CmicM_BQRegister_ST6_RES(void);  
static void	CmicM_BQRegister_ST7_REQ(void);  
static void	CmicM_BQRegister_ST7_WAIT(void);  
static void	CmicM_BQRegister_ST7_RES(void);  

static void	CmicM_SetCell_ST0_IDLE(void);
static void	CmicM_SetCell_ST1_REQ(void);	//
static void	CmicM_SetCell_ST1_RES(void);	//
static void	CmicM_SetCell_ST2_REQ(void);	//
static void	CmicM_SetCell_ST2_RES(void);	//

static void	CmicM_Cell_ST0_IDLE(void);
static void	CmicM_Cell_ST1_REQ(void);	//
static void	CmicM_Cell_ST1_WAIT(void);	//
static void	CmicM_Cell_ST1_RES(void);	//

static void	CmicM_SetBal_ST0_IDLE(void);
static void	CmicM_SetBal_ST1_REQ(void);	//
static void	CmicM_SetBal_ST1_RES(void);	//

static void	CmicM_Bal_ST0_IDLE(void);
static void	CmicM_Bal_ST1_REQ(void);	//
static void CmicM_Bal_ST1_WAIT(void);
static void	CmicM_Bal_ST1_RES(void);	//
static void	CmicM_Bal_ST2_REQ(void);	//
static void	CmicM_Bal_ST2_RES(void); 	//


static void CmicM_KeyOff_ST0_IDLE(void);	//
static void	CmicM_KeyOff_ST1_REQ(void);	//
static void	CmicM_KeyOff_ST1_RES(void); 	//
static void	CmicM_KeyOff_ST2_REQ(void);	//
static void	CmicM_KeyOff_ST2_RES(void); 	//

static void CmicM_KeyOn_ST0_IDLE(void);	//
static void	CmicM_KeyOn_ST1_REQ(void);	//
static void	CmicM_KeyOn_ST1_RES(void); 	//
static void	CmicM_KeyOn_ST2_REQ(void);	//
static void	CmicM_KeyOn_ST2_RES(void); 	//

static void	CmicM_Health_ST0_IDLE(void);	//
static void	CmicM_Health_ST1_REQ(void);	//
static void	CmicM_Health_ST1_RES(void); 	//
static void	CmicM_Health_ST2_REQ(void);	//
static void	CmicM_Health_ST2_RES(void); 	//
static void	CmicM_Health_ST3_REQ(void);	//
static void	CmicM_Health_ST3_RES(void); 	//
static void	CmicM_Health_ST4_REQ(void);	//
static void	CmicM_Health_ST4_RES(void); 	//
static void	CmicM_Health_ST5_REQ(void);	//
static void	CmicM_Health_ST5_RES(void); 	//

static uint8_t CmicM_CalculateFCS(const uint8_t *buf, uint32 qLength);
static uint16 CmicM_CalculateCRC16(const uint8_t *pData, uint16 wDataLen);
static uint8_t CmicM_FrameCheck(const uint8_t pRxData[], uint16 nLength);

static uint8 CmicM_RUN_VerifyRx_BQData(uint8_t uart); //Running 상태에서의 Verify함수별도제어.
static uint8 CmicM_CheckBQRegisterStatus(uint8_t uart, uint8_t Dataidx);
static uint8 CmicM_VerifyRx_BQData(uint8_t uart);
static uint8 CmicM_VerifyRx_CFGData(uint8_t uart);
static void CmicM_DecodeVCELLData(uint8_t uart);
static void CmicM_DecodeGPIOData(uint8_t uart);
static uint16 CmicM_MakeBQFrame(eUART uart, uint8_t init, uint16 regAddr, 
								uint8_t *pData, uint16 idx);

static void CmicM_MakeNPIFrameHeader(eUART uart, uint16 length);								
static uint8_t CmicM_MakeNPISyncTxData(eUART uart, WBMS_COMMAND_E cmd);
static void CmicM_BQ7971x_ConfigInit(eUART uart);
static uint32 Bytes2Uint32(uint8 *pData);
static uint32 Bytes2Uint16(uint8 *pData);
static void CmicM_MacStatusFail(eUART uart, uint8  failCode);
static void CmicM_CalculatePDR(uint8  node);
static void	CmicM_PriorVcell_Init(uint8_t uart);
static void CmicM_VcellOWDDecode(uint8_t uart);
typedef void (*pFunc)(void); 

static const pFunc  pArrayBoot_Network[]=
{
	CmicM_Network_ST0_IDLE, 	//eNETWORK_ST0_IDLE
	CmicM_Network_ST0_IDLE_RES, //eNETWORK_ST0_IDLE_RES
	CmicM_Network_ST1_REQ,	//eNETWORK_ST1_REQ
	CmicM_Network_ST1_RES, 	//eNETWORK_ST1_RES
	CmicM_Network_ST2_REQ,	//eNETWORK_ST2_REQ
	CmicM_Network_ST2_RES, 	//eNETWORK_ST2_RES
	CmicM_Network_ST3_REQ,	//eNETWORK_ST3_REQ
	CmicM_Network_ST3_RES, 	//eNETWORK_ST3_RES
	CmicM_Network_ST4_REQ,	//eNETWORK_ST4_REQ
	CmicM_Network_ST4_RES, 	//eNETWORK_ST4_RES
	CmicM_Network_ST5_REQ,	//eNETWORK_ST5_REQ
	CmicM_Network_ST5_RES, 	//eNETWORK_ST5_RES
	CmicM_Network_ST6_REQ,	//eNETWORK_ST6_REQ
	CmicM_Network_ST6_RES, 	//eNETWORK_ST6_RES
	CmicM_Network_ST6_1_REQ,//eNETWORK_ST6_1_REQ
	CmicM_Network_ST6_1_RES,
	CmicM_Network_ST7_REQ,	//eNETWORK_ST7_REQ
	CmicM_Network_ST7_RES, 	//eNETWORK_ST7_RES
	CmicM_Network_ST8_REQ,	//eNETWORK_ST8_REQ
	CmicM_Network_ST8_RES, 	//eNETWORK_ST8_RES	

};

static const pFunc pArrayBoot_BQRegister[]=
{
	CmicM_BQRegister_ST0_IDLE,	//
	CmicM_BQRegister_ST1_REQ,	//BQ ConfigInit
	CmicM_BQRegister_ST1_RES, 	//
	CmicM_BQRegister_ST2_REQ,	//Diag StartupInit
	CmicM_BQRegister_ST2_RES, 	//
	CmicM_BQRegister_ST3_REQ,  
	CmicM_BQRegister_ST3_RES,
	CmicM_BQRegister_ST4_REQ,  
	CmicM_BQRegister_ST4_RES,
	CmicM_BQRegister_ST5_REQ,  
	CmicM_BQRegister_ST5_RES,

	CmicM_BQRegister_ST6_REQ, 
	CmicM_BQRegister_ST6_WAIT, //added for SequenceRead
	CmicM_BQRegister_ST6_RES,  
	CmicM_BQRegister_ST7_REQ, 
	CmicM_BQRegister_ST7_WAIT, //added for SequenceRead
	CmicM_BQRegister_ST7_RES,  
};
static const pFunc pArraySetCell_Monitor[]=
{
	CmicM_SetCell_ST0_IDLE,	//
	CmicM_SetCell_ST1_REQ,	//
	CmicM_SetCell_ST1_RES, 	//
	CmicM_SetCell_ST2_REQ,	//
	CmicM_SetCell_ST2_RES, 	//
};

static const pFunc pArrayCell_Monitor[]=
{
	CmicM_Cell_ST0_IDLE,	//
	CmicM_Cell_ST1_REQ,	//
	CmicM_Cell_ST1_WAIT,
	CmicM_Cell_ST1_RES, 	//
};

static const pFunc pArraySetBal_Monitor[]=
{
	CmicM_SetBal_ST0_IDLE,	//
	CmicM_SetBal_ST1_REQ,	//
	CmicM_SetBal_ST1_RES, 	//
};

static const pFunc pArrayBal_Monitor[]=
{
	CmicM_Bal_ST0_IDLE,	//
	CmicM_Bal_ST1_REQ,	//
	CmicM_Bal_ST1_WAIT,
	CmicM_Bal_ST1_RES, 	//
	CmicM_Bal_ST2_REQ,	//
	CmicM_Bal_ST2_RES, 	//
};


static const pFunc pArrayKeyOff[]=
{
	CmicM_KeyOff_ST0_IDLE,	//
	CmicM_KeyOff_ST1_REQ,	//
	CmicM_KeyOff_ST1_RES, 	//
	CmicM_KeyOff_ST2_REQ,	//
	CmicM_KeyOff_ST2_RES, 	//
};
static const pFunc pArrayKeyOn[]=
{
	CmicM_KeyOn_ST0_IDLE,	//
	CmicM_KeyOn_ST1_REQ,	//
	CmicM_KeyOn_ST1_RES, 	//
	CmicM_KeyOn_ST2_REQ,	//
	CmicM_KeyOn_ST2_RES, 	//
};


static const pFunc pArrayHealth_Monitor[]=
{
	CmicM_Health_ST0_IDLE,
	CmicM_Health_ST1_REQ,
	CmicM_Health_ST1_RES,
	CmicM_Health_ST2_REQ,
	CmicM_Health_ST2_RES,
	CmicM_Health_ST3_REQ,
	CmicM_Health_ST3_RES,
	CmicM_Health_ST4_REQ,
	CmicM_Health_ST4_RES,
	CmicM_Health_ST5_REQ,
	CmicM_Health_ST5_RES,
};
static const uint16 zCrc16Lkup[256] =
{
    0x0000u, 0xC0C1u, 0xC181u, 0x0140u, 0xC301u, 0x03C0u, 0x0280u, 0xC241u, 0xC601u, 0x06C0u, 0x0780u, 0xC741u, 0x0500u,
    0xC5C1u, 0xC481u, 0x0440u, 0xCC01u, 0x0CC0u, 0x0D80u, 0xCD41u, 0x0F00u, 0xCFC1u, 0xCE81u, 0x0E40u, 0x0A00u, 0xCAC1u,
    0xCB81u, 0x0B40u, 0xC901u, 0x09C0u, 0x0880u, 0xC841u, 0xD801u, 0x18C0u, 0x1980u, 0xD941u, 0x1B00u, 0xDBC1u, 0xDA81u,
    0x1A40u, 0x1E00u, 0xDEC1u, 0xDF81u, 0x1F40u, 0xDD01u, 0x1DC0u, 0x1C80u, 0xDC41u, 0x1400u, 0xD4C1u, 0xD581u, 0x1540u,
    0xD701u, 0x17C0u, 0x1680u, 0xD641u, 0xD201u, 0x12C0u, 0x1380u, 0xD341u, 0x1100u, 0xD1C1u, 0xD081u, 0x1040u, 0xF001u,
    0x30C0u, 0x3180u, 0xF141u, 0x3300u, 0xF3C1u, 0xF281u, 0x3240u, 0x3600u, 0xF6C1u, 0xF781u, 0x3740u, 0xF501u, 0x35C0u,
    0x3480u, 0xF441u, 0x3C00u, 0xFCC1u, 0xFD81u, 0x3D40u, 0xFF01u, 0x3FC0u, 0x3E80u, 0xFE41u, 0xFA01u, 0x3AC0u, 0x3B80u,
    0xFB41u, 0x3900u, 0xF9C1u, 0xF881u, 0x3840u, 0x2800u, 0xE8C1u, 0xE981u, 0x2940u, 0xEB01u, 0x2BC0u, 0x2A80u, 0xEA41u,
    0xEE01u, 0x2EC0u, 0x2F80u, 0xEF41u, 0x2D00u, 0xEDC1u, 0xEC81u, 0x2C40u, 0xE401u, 0x24C0u, 0x2580u, 0xE541u, 0x2700u,
    0xE7C1u, 0xE681u, 0x2640u, 0x2200u, 0xE2C1u, 0xE381u, 0x2340u, 0xE101u, 0x21C0u, 0x2080u, 0xE041u, 0xA001u, 0x60C0u,
    0x6180u, 0xA141u, 0x6300u, 0xA3C1u, 0xA281u, 0x6240u, 0x6600u, 0xA6C1u, 0xA781u, 0x6740u, 0xA501u, 0x65C0u, 0x6480u,
    0xA441u, 0x6C00u, 0xACC1u, 0xAD81u, 0x6D40u, 0xAF01u, 0x6FC0u, 0x6E80u, 0xAE41u, 0xAA01u, 0x6AC0u, 0x6B80u, 0xAB41u,
    0x6900u, 0xA9C1u, 0xA881u, 0x6840u, 0x7800u, 0xB8C1u, 0xB981u, 0x7940u, 0xBB01u, 0x7BC0u, 0x7A80u, 0xBA41u, 0xBE01u,
    0x7EC0u, 0x7F80u, 0xBF41u, 0x7D00u, 0xBDC1u, 0xBC81u, 0x7C40u, 0xB401u, 0x74C0u, 0x7580u, 0xB541u, 0x7700u, 0xB7C1u,
    0xB681u, 0x7640u, 0x7200u, 0xB2C1u, 0xB381u, 0x7340u, 0xB101u, 0x71C0u, 0x7080u, 0xB041u, 0x5000u, 0x90C1u, 0x9181u,
    0x5140u, 0x9301u, 0x53C0u, 0x5280u, 0x9241u, 0x9601u, 0x56C0u, 0x5780u, 0x9741u, 0x5500u, 0x95C1u, 0x9481u, 0x5440u,
    0x9C01u, 0x5CC0u, 0x5D80u, 0x9D41u, 0x5F00u, 0x9FC1u, 0x9E81u, 0x5E40u, 0x5A00u, 0x9AC1u, 0x9B81u, 0x5B40u, 0x9901u,
    0x59C0u, 0x5880u, 0x9841u, 0x8801u, 0x48C0u, 0x4980u, 0x8941u, 0x4B00u, 0x8BC1u, 0x8A81u, 0x4A40u, 0x4E00u, 0x8EC1u,
    0x8F81u, 0x4F40u, 0x8D01u, 0x4DC0u, 0x4C80u, 0x8C41u, 0x4400u, 0x84C1u, 0x8581u, 0x4540u, 0x8701u, 0x47C0u, 0x4680u,
    0x8641u, 0x8201u, 0x42C0u, 0x4380u, 0x8341u, 0x4100u, 0x81C1u, 0x8081u, 0x4040u
};



/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of function definition area >>            DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
/*******************************************************************************
** Function Name    : CmicM_Init
** Return Value     : None
** Parameter        : None
** Description      : None
*******************************************************************************/
void CmicM_Init(void)
{

    MemSet(&gInst, 0, sizeof(CmicM_Instance_t));

	TickerInit();
	 Tmr_Init();

	gInst.m_nTick1ms = (uint32)GetTick_1ms();

	gInst.m_tSt.m_eMain = 	eMAIN_BOOT; 

	gInst.m_tSt.m_eBoot = eBOOT_NETWORK;
	//gInst.m_uNAFEs = MAX_NUM_DEVS;

	CmicM_debug_BootTimeLog(Overall_, LogStart, 999, Total_boot_time______________________________);
    // CmicM_InitHandler();
 
} /* End of CmicM_Init */


void CmicM_InitHandler(void)
{

}

void CmicM_Handler(void)
{
	uint32  aTick;
    
	aTick =	GetTick_1ms();

	if (gInst.m_nTick1ms != aTick) { //1ms condition.

	     gInst.m_nTick1ms = aTick;

		switch(gInst.m_tSt.m_eMain)
		{
			case eMAIN_BOOT :
				CmicM_ControlBootState();
				break;
			case eMAIN_RUN :
				CmicM_ControlRunState();
				break;
			case eMAIN_KEYOFF :
				CmicM_ControlKeyOffState();
				break;
			case eMAIN_KEYON :
				CmicM_ControlKeyOnState();
				break;
			case eMAIN_BAL :
				CmicM_ControlBalState();
				break;
			default :
				break;
		}
		gInst.m_nDebug1ms = gInst.m_nTick1ms;
	}
}

/*******************************************************************************
** Function Name    : CmicM_Handler_10ms
** Description      : 10ms 주기처리.
*******************************************************************************/
void CmicM_Handler_10ms(void)
{
	uint32  	aTick;
	
	aTick = GetTick_10ms();

	if(gInst.m_nTick10ms != aTick){ //10ms condition

		gInst.m_nTick10ms = aTick;

		CmicM_Control_Poll_XmsState();
	}
}
/*******************************************************************************
** Function Name    : CmicM_Control_Poll_XmsState
** Return Value     : None
** Parameter        : None
** Description      : UART수신trigger에 대한 Polling처리(P,S).
					 시작(xxREQUEST)->실행(xxRUNNING) ->완료(xxRECEIVE)
*******************************************************************************/
static void CmicM_Control_Poll_XmsState(void)
{
	uint8  Pidx, Sidx;

	if(CmicM_GetPollStatus() == ePOLL_Xms_REQUEST){

		CmicM_SetPollStatus(ePOLL_Xms_RUNNING);

		if (UART_SEQ_Polling_Cb(&Pidx, &Sidx))
		{
			CmicM_SetPollStatus(ePOLL_Xms_RECEIVE);
			CmicM_SetIndex(Pidx, Sidx);

			gInst.m_tInfo.m_10msTime.m_timeP1 = GetTick_1ms();
			gInst.m_tInfo.m_10msTime.m_duration = (float)((gInst.m_tInfo.m_10msTime.m_timeP1 - gInst.m_tInfo.m_10msTime.m_timeP0)/1000.0f);

		}else{
			CmicM_SetPollStatus(ePOLL_Xms_NG);	
		}
	}
}

/*******************************************************************************
** Function Name    : CmicM_GetIndex / CmicM_SetIndex
** Description      : UART BufMgr의 index info의 Get/Set.
*******************************************************************************/
static void CmicM_GetIndex(uint8 *pPindex, uint8 *pSindex)
{
	*pPindex = (uint8)gInst.m_tTxRx[UART_P].m_cIdx;
	*pSindex = (uint8)gInst.m_tTxRx[UART_S].m_cIdx;
	return;
}

static void CmicM_SetIndex(uint8 Pindex, uint8 Sindex)
{
	gInst.m_tTxRx[UART_P].m_cIdx = Pindex;
	gInst.m_tTxRx[UART_S].m_cIdx = Sindex;
	return;
}
/*******************************************************************************
** Function Name    : CmicM_GetPollStatus / CmicM_SetPollStatus
** Description      : m_ePollStatus에 대한 Read/Write
*******************************************************************************/
static uint8 CmicM_GetPollStatus(void)
{
	return (uint8)gInst.m_tSt.m_ePollStatus;
}

static void	CmicM_SetPollStatus(uint8 eStat)
{
	gInst.m_tSt.m_ePollStatus = eStat;
	return;
}

static void CmicM_ControlKeyOffState(void)
{

	if (NULL != pArrayKeyOff[gInst.m_tSt.m_eKeyOff]){
		pArrayKeyOff[gInst.m_tSt.m_eKeyOff]();
	}	
}

static void CmicM_ControlKeyOnState(void)
{
	if (NULL != pArrayKeyOn[gInst.m_tSt.m_eKeyOn]){
		pArrayKeyOn[gInst.m_tSt.m_eKeyOn]();
	}	
}

typedef enum
{
	eEVT_IDLE,
	eEVT_CELL,
	eEVT_HEALTH,
}eJOB_EVENT;

//100ms Timer Interrupt.
void  TimerCallback_Proc(void)
{
	static uint16  nCount = 0;

	if( !(++nCount % 10)){ //1sec 주기로 Health monitor수행.
		gInst.m_tSt.m_eTickEvent = (eEVT_CELL | eEVT_HEALTH);
	}else{
		gInst.m_tSt.m_eTickEvent = eEVT_CELL;
	}
}

static void CmicM_ControlRunState(void)
{

	if((gInst.m_tSt.m_ePriorMain != gInst.m_tSt.m_eMain) && (gInst.m_tSt.m_ePriorMain != eMAIN_BOOT))
	{
		gInst.m_tSt.m_eRun = eRUN_SETCELL_MONITOR; 
	}

	switch(gInst.m_tSt.m_eRun)
	{
		case  eRUN_IDLE :
			
			if (eEVT_CELL & gInst.m_tSt.m_eTickEvent){
				gInst.m_tSt.m_eTickEvent &= (~eEVT_CELL); //bit Clear

				gInst.m_tSt.m_eRun = eRUN_CELL_MONITOR;
				gInst.m_tSt.m_RunSub.m_eCellMonitor = eCELL_ST1_REQ;

				if (NULL != pArrayCell_Monitor[gInst.m_tSt.m_RunSub.m_eCellMonitor]){
					pArrayCell_Monitor[gInst.m_tSt.m_RunSub.m_eCellMonitor]();
				}	
				return;
			}
			
			if(eEVT_HEALTH & gInst.m_tSt.m_eTickEvent){
				gInst.m_tSt.m_eTickEvent = eEVT_IDLE;

				gInst.m_tSt.m_eRun = eRUN_HEALTH_MONITOR;
				gInst.m_tSt.m_eHealthMonitor = eHEALTH_ST0_IDLE;
			
				if (NULL != pArrayHealth_Monitor[gInst.m_tSt.m_eHealthMonitor]){
					pArrayHealth_Monitor[gInst.m_tSt.m_eHealthMonitor]();
				}
				return;
			}			
			break;

		case   eRUN_SETCELL_MONITOR :
			gInst.m_tSt.m_ePriorMain = gInst.m_tSt.m_eMain;

			if (NULL != pArraySetCell_Monitor[gInst.m_tSt.m_RunSub.m_eSetCellMonitor]){
				pArraySetCell_Monitor[gInst.m_tSt.m_RunSub.m_eSetCellMonitor]();
			}
			break;

		case   eRUN_CELL_MONITOR :
			if (NULL != pArrayCell_Monitor[gInst.m_tSt.m_RunSub.m_eCellMonitor]){
				pArrayCell_Monitor[gInst.m_tSt.m_RunSub.m_eCellMonitor]();
			}
			break;
		
		case   eRUN_HEALTH_MONITOR :
			if (NULL != pArrayHealth_Monitor[gInst.m_tSt.m_eHealthMonitor]){
				pArrayHealth_Monitor[gInst.m_tSt.m_eHealthMonitor]();
			}
			break;
		default :
			break;		
	}	
}

static void CmicM_ControlBalState(void)
{
	if((gInst.m_tSt.m_ePriorMain != gInst.m_tSt.m_eMain))
	{
		gInst.m_tSt.m_eBal = eBAL_SETBALANCE_MONITOR;
	}

	switch(gInst.m_tSt.m_eBal)
	{
		case  eBAL_IDLE :
			
			if (eEVT_CELL & gInst.m_tSt.m_eTickEvent){
				gInst.m_tSt.m_eTickEvent &= (~eEVT_CELL); //bit Clear
				
				gInst.m_tSt.m_eBal = eBAL_BALANCE_MONITOR;
				gInst.m_tSt.m_BalSub.m_eBalMonitor =  eBAL_ST1_REQ;
				
				if (NULL != pArrayBal_Monitor[gInst.m_tSt.m_BalSub.m_eBalMonitor]){
					pArrayBal_Monitor[gInst.m_tSt.m_BalSub.m_eBalMonitor]();
				}	
				return;
			}

			if(eEVT_HEALTH & gInst.m_tSt.m_eTickEvent){			
				gInst.m_tSt.m_eTickEvent = eEVT_IDLE;

				gInst.m_tSt.m_eBal = eBAL_HEALTH_MONITOR;
				gInst.m_tSt.m_eHealthMonitor = eHEALTH_ST0_IDLE;
			
				if (NULL != pArrayHealth_Monitor[gInst.m_tSt.m_eHealthMonitor]){
					pArrayHealth_Monitor[gInst.m_tSt.m_eHealthMonitor]();
				}
				return;
			}			
			break;
		
		case   eBAL_SETBALANCE_MONITOR :
			gInst.m_tSt.m_ePriorMain = gInst.m_tSt.m_eMain;

			if (NULL != pArraySetBal_Monitor[gInst.m_tSt.m_BalSub.m_eSetBalMonitor]){
				pArraySetBal_Monitor[gInst.m_tSt.m_BalSub.m_eSetBalMonitor]();
			}
			break;
		
		case   eBAL_BALANCE_MONITOR :
			if (NULL != pArrayBal_Monitor[gInst.m_tSt.m_BalSub.m_eBalMonitor]){
				pArrayBal_Monitor[gInst.m_tSt.m_BalSub.m_eBalMonitor]();
			}
			break;
		
		case   eBAL_HEALTH_MONITOR :
			if (NULL != pArrayHealth_Monitor[gInst.m_tSt.m_eHealthMonitor]){
				pArrayHealth_Monitor[gInst.m_tSt.m_eHealthMonitor]();
			}
			break;
		default :
			break;		
	}	
}

static void CmicM_ControlBootState(void)
{
	switch(gInst.m_tSt.m_eBoot)
	{
		case eBOOT_IDLE :
			break;

		case eBOOT_NETWORK :
			if (NULL != pArrayBoot_Network[gInst.m_tSt.m_BootSub.m_eNetwork]){
				pArrayBoot_Network[gInst.m_tSt.m_BootSub.m_eNetwork]();
			}
			break;
		case eBOOT_BQREGISTER :
			if (NULL != pArrayBoot_BQRegister[gInst.m_tSt.m_BootSub.m_eBQRegister]){
				pArrayBoot_BQRegister[gInst.m_tSt.m_BootSub.m_eBQRegister]();
			}
			break;
		default :
			break;
	}

}

/*******************************************************************************
**                      Private Function Definitions                          **
*******************************************************************************/
static void CmicM_KeyOff_ST0_IDLE(void)
{
	if (gInst.m_tSt.m_eMain != gInst.m_tSt.m_ePriorMain){
		gInst.m_tSt.m_eKeyOff = eKEYOFF_ST1_REQ;

		Tmr_Stop();		
	}
}

static void CmicM_KeyOff_ST1_REQ(void)
{
	WBMS_COMMAND_E cmd;
	cmd = eCMD_CF_SETPOWERMODE;
	
	//SECONDARY
	MemSet(&gInst.m_tTxRx[UART_S].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cTxBuf));
	
	CmicM_MakeNPISyncTxData(UART_S, cmd);
 	gInst.m_tTxRx[UART_S].m_nTxLen = gInst.m_ReqFrame[UART_S].m_NPILen;

	UART_SendData(UART_S, gInst.m_tTxRx[UART_S].m_cTxBuf, gInst.m_tTxRx[UART_S].m_nTxLen);

	//PRIMARY
	MemSet(&gInst.m_tTxRx[UART_P].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cTxBuf));
	
	CmicM_MakeNPISyncTxData(UART_P, cmd);
 	gInst.m_tTxRx[UART_P].m_nTxLen = gInst.m_ReqFrame[UART_P].m_NPILen;

	UART_SendData(UART_P, gInst.m_tTxRx[UART_P].m_cTxBuf, gInst.m_tTxRx[UART_P].m_nTxLen);

	gInst.m_tSt.m_eKeyOff++;
}

static void CmicM_KeyOff_ST1_RES(void)
{
	static  eRECV_Status	rStat = NO_RECV;

	 //Secondary Check
	if (FALSE == (rStat & S_RECV))
	{
		if (UART_Polling_Cb(UART_S))
		{			
			MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
		 	UART_ReceiveData(UART_S, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);

			if (CmicM_VerifyRx_CFGData(UART_S)){
				// CmdID check
				if(gInst.m_RespFrame[UART_S].m_uCmdID == eCMD_CF_SETPOWERMODE ){
					//Status-Success check
					if(gInst.m_RespFrame[UART_S].m_uBQ_DATA[0] == 0x00) //macStatus
					{
						rStat |= S_RECV;
					}
				}
			}		 
		}
	}

	//Primary Check
	if (FALSE == (rStat & P_RECV))
	{
		if (UART_Polling_Cb(UART_P))
		{			
			MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
		 	UART_ReceiveData(UART_P, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);

			if (CmicM_VerifyRx_CFGData(UART_P)){
				// CmdID check
				if(gInst.m_RespFrame[UART_P].m_uCmdID == eCMD_CF_SETPOWERMODE ){
					//Status-Success check
					if(gInst.m_RespFrame[UART_P].m_uBQ_DATA[0] == 0x00) //macStatus
					{
						rStat |= P_RECV;
					}
				}
			}		 
		}
	}

	if (rStat == ALL_RECV )
	{
		rStat = NO_RECV;
		gInst.m_tSt.m_eKeyOff = eKEYOFF_ST0_IDLE;
		gInst.m_tSt.m_ePriorMain = eMAIN_KEYOFF;
	}
}

static void	CmicM_KeyOff_ST2_REQ(void)
{

}	//
static void	CmicM_KeyOff_ST2_RES(void)
{

} 	//

static void CmicM_KeyOn_ST0_IDLE(void)
{
	if (gInst.m_tSt.m_eMain != gInst.m_tSt.m_ePriorMain){
		gInst.m_tSt.m_eKeyOn = eKEYON_ST1_REQ;
		gInst.m_tInfo.m_igOnTime.m_timeP0 = GetTick_1ms(); //IG OnTime measure
		//CmicM_ControlKeyOnState();
	}
}	//

#define GPIO_WAKEUP_P   &MODULE_P11, 3
#define GPIO_WAKEUP_S   &MODULE_P11, 8
static void	CmicM_KeyOn_ST1_REQ(void)
{
	IfxPort_setPinMode(GPIO_WAKEUP_P, IfxPort_Mode_outputPushPullGeneral);
	IfxPort_setPinMode(GPIO_WAKEUP_S, IfxPort_Mode_outputPushPullGeneral);
	
	IfxPort_setPinState(GPIO_WAKEUP_P,IfxPort_State_high);	
	IfxPort_setPinState(GPIO_WAKEUP_S,IfxPort_State_high);
	gInst.m_tSt.m_eKeyOn++;
}	//

//: Host wait APP_DIAG_EXITPS_CB ( 0x2A ) from RFIC 
static void	CmicM_KeyOn_ST1_RES(void)
{
	IfxPort_setPinState(GPIO_WAKEUP_P,IfxPort_State_low);
	IfxPort_setPinState(GPIO_WAKEUP_S,IfxPort_State_low);
	
	gInst.m_tSt.m_eKeyOn = eKEYON_ST2_RES;		
} 

// 
static void	CmicM_KeyOn_ST2_REQ(void)
{

}	
static void	CmicM_KeyOn_ST2_RES(void)
{
	static  eRECV_Status	rStat = NO_RECV;

	//Secondary Check
	if (FALSE == (rStat & S_RECV))
	{
		if (UART_Polling_Cb(UART_S))
		{			
			MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
		 	UART_ReceiveData(UART_S, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);

			if (CmicM_VerifyRx_CFGData(UART_S))
			{	// CmdID check
				if(gInst.m_RespFrame[UART_S].m_uCmdID == eCMD_CB_EXITPS ){
					rStat |= S_RECV;
				}
			}	 
		}
	}

	//Primary Check
	if (FALSE == (rStat & P_RECV))
	{
		if (UART_Polling_Cb(UART_P))
		{			
			MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
		 	UART_ReceiveData(UART_P, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);

			if (CmicM_VerifyRx_CFGData(UART_P))
			{	// CmdID check
				if(gInst.m_RespFrame[UART_P].m_uCmdID == eCMD_CB_EXITPS ){
						rStat |= P_RECV;					
				}
			}				 
		}
	}

	if (rStat == ALL_RECV )
	{
		/* IG OnTime Measurement */
		gInst.m_tInfo.m_igOnTime.m_timeP1 = GetTick_1ms(); 		
		gInst.m_tInfo.m_igOnTime.m_duration = (float)((gInst.m_tInfo.m_igOnTime.m_timeP1 - gInst.m_tInfo.m_igOnTime.m_timeP0)/1000.0f);

		rStat = NO_RECV;
		gInst.m_tSt.m_eKeyOn = eKEYON_ST0_IDLE;	
		gInst.m_tSt.m_eMain = 	eMAIN_RUN; 	
		gInst.m_tSt.m_ePriorMain = 	eMAIN_RUN; 		
		//100ms Timer Start ..
		Tmr_Start(TimerCallback_Proc);
	}	
} 	//

#define GPIO_RST  &MODULE_P02, 7
/********************************************************************************
FUNCTION :  CmicM_Network_ST0_IDLE()
STATUS :   BOOTSUB_NETWORK_E =  eNETWORK_ST0_IDLE
DESCRIPTION : RFIC(CC2662)에 Reset신호출력 = LOW
**********************************************************************************/
static void	CmicM_Network_ST0_IDLE(void)
{
	IfxPort_setPinMode(GPIO_RST, IfxPort_Mode_outputPushPullGeneral);
	IfxPort_setPinState(GPIO_RST,IfxPort_State_low);

	gInst.m_tSt.m_BootSub.m_eNetwork++;
	gInst.m_nDelay = 0;
}

/********************************************************************************
FUNCTION :  CmicM_Network_ST0_IDLE_RES()
STATUS :   BOOTSUB_NETWORK_E =  eNETWORK_ST0_IDLE_RES
DESCRIPTION : RFIC(CC2662)에 Reset신호 HIGH출력후 eCMD_CB_RESET(0x29)의 수신체크(Primary/Secondary)  
**********************************************************************************/
static void CmicM_Network_ST0_IDLE_RES(void)
{
	static  eRECV_Status	rStat = NO_RECV;

	IfxPort_setPinState(GPIO_RST,IfxPort_State_high);

	//Primary Check-------------------------------------------------------------------------
	if (FALSE == (rStat & P_RECV))
	{
		if(UART_Polling_Cb(UART_P))
		{
			MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
			UART_ReceiveData(UART_P, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);

			if(gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_IDX_CMDID] == eCMD_CB_RESET )
			{
				if ( COMIF_OK == CmicM_FrameCheck(gInst.m_tTxRx[UART_P].m_cRxBuf, gInst.m_tTxRx[UART_P].m_nRxLen))
				{				
					rStat |= P_RECV;
					//FWVersion(4Bytes) & UID (8Bytes) 저장
					MemCpy(&gInst.m_tQueryInfo[UART_P].m_qFWVersion, &gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_IDX_DATA], 4);
					MemCpy(&gInst.m_tQueryInfo[UART_P].m_uWMainUID,	&gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_IDX_DATA + 4], COMM_DEVICE_UID_SIZE); 
				}
			}
		}
	}

	//Secondary Check-----------------------------------------------------------------------
	if (FALSE == (rStat & S_RECV))
	{
		if (UART_Polling_Cb(UART_S))
		{
			MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
			UART_ReceiveData(UART_S, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);

			if(gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_IDX_CMDID] == eCMD_CB_RESET )
			{
				if ( COMIF_OK == CmicM_FrameCheck(gInst.m_tTxRx[UART_S].m_cRxBuf, gInst.m_tTxRx[UART_S].m_nRxLen))
				{			
					rStat |= S_RECV;
					MemCpy(&gInst.m_tQueryInfo[UART_S].m_qFWVersion, &gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_IDX_DATA], 4);
					MemCpy(&gInst.m_tQueryInfo[UART_S].m_uWMainUID,	&gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_IDX_DATA + 4], COMM_DEVICE_UID_SIZE); 
				}
			}
		}	
	}

	if (rStat == ALL_RECV)
	{
		rStat = NO_RECV;
		gInst.m_tSt.m_BootSub.m_eNetwork = eNETWORK_ST1_REQ;
	}	
}

/********************************************************************************
FUNCTION :  CmicM_Network_ST1_REQ()/RES()
STATUS :   BOOTSUB_NETWORK_E =  eNETWORK_ST1_REQ/RES
DESCRIPTION : Command : eCMD_CF_SETNWOPMODE(0x47) Secondary 송수신
**********************************************************************************/
static void	CmicM_Network_ST1_REQ(void)
{
	WBMS_COMMAND_E cmd;
	cmd = eCMD_CF_SETNWOPMODE;
	
	//SECONDARY----------------------------------------------------------------
	MemSet(&gInst.m_tTxRx[UART_S].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cTxBuf));
	
	CmicM_MakeNPISyncTxData(UART_S, cmd);
 	gInst.m_tTxRx[UART_S].m_nTxLen = gInst.m_ReqFrame[UART_S].m_NPILen;
	UART_SendData(UART_S, gInst.m_tTxRx[UART_S].m_cTxBuf, gInst.m_tTxRx[UART_S].m_nTxLen);

	gInst.m_tSt.m_BootSub.m_eNetwork++;
}

static void	CmicM_Network_ST1_RES(void)
{
	//Secondary Check-----------------------------------------------------------
	if (UART_Polling_Cb(UART_S))
	{		
		MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
		UART_ReceiveData(UART_S, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);

		if(gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_IDX_CMDID] == eCMD_CF_SETNWOPMODE )
		{
			if ( COMIF_OK == CmicM_FrameCheck(gInst.m_tTxRx[UART_S].m_cRxBuf, gInst.m_tTxRx[UART_S].m_nRxLen))
			{
				gInst.m_tSt.m_BootSub.m_eNetwork++;			
			}
		}
	}
}

/********************************************************************************
FUNCTION :  CmicM_Network_ST2_REQ()/RES()
STATUS :   BOOTSUB_NETWORK_E =  eNETWORK_ST2_REQ/RES
DESCRIPTION : Command : SETNWKOPMODE(0x47) - Primary송수신
**********************************************************************************/
static void CmicM_Network_ST2_REQ(void)
{
	WBMS_COMMAND_E cmd;
	cmd = eCMD_CF_SETNWOPMODE;

	//PRIMARY --------------------------------------------------------------------
	MemSet(&gInst.m_tTxRx[UART_P].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cTxBuf));
	
	CmicM_MakeNPISyncTxData(UART_P, cmd);
	gInst.m_tTxRx[UART_P].m_nTxLen = gInst.m_ReqFrame[UART_P].m_NPILen;
	UART_SendData(UART_P, gInst.m_tTxRx[UART_P].m_cTxBuf, gInst.m_tTxRx[UART_P].m_nTxLen);

	gInst.m_tSt.m_BootSub.m_eNetwork++;
}

static void CmicM_Network_ST2_RES(void)
{
	//Primary Check -------------------------------------------------------------------
	if (UART_Polling_Cb(UART_P))
	{		
		MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
		UART_ReceiveData(UART_P, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);

		if(gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_IDX_CMDID] == eCMD_CF_SETNWOPMODE )
		{
			if ( COMIF_OK == CmicM_FrameCheck(gInst.m_tTxRx[UART_P].m_cRxBuf, gInst.m_tTxRx[UART_P].m_nRxLen))
			{
				gInst.m_tSt.m_BootSub.m_eNetwork++;
			}
		}
	} 		
	gInst.m_nDelay=0;
}

/********************************************************************************
FUNCTION :  CmicM_Network_ST3_REQ()
STATUS :   BOOTSUB_NETWORK_E =  eNETWORK_ST3_REQ
DESCRIPTION : Command : eCMD_CF_SETPARAMS(0x4E):(DWMRole,Role설정)-Primary/Secondary.
**********************************************************************************/
static void CmicM_Network_ST3_REQ(void)
{
	WBMS_COMMAND_E cmd;
	cmd = eCMD_CF_SETPARAMS; 

	// 10ms Delay 후 3번째 커맨드 전송
	if(++gInst.m_nDelay > 10)
	{
		//SECONDARY -----------------------------------
		MemSet(&gInst.m_tTxRx[UART_S].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cTxBuf));

		CmicM_MakeNPISyncTxData(UART_S, cmd);
		gInst.m_tTxRx[UART_S].m_nTxLen = gInst.m_ReqFrame[UART_S].m_NPILen;
		UART_SendData(UART_S, gInst.m_tTxRx[UART_S].m_cTxBuf, gInst.m_tTxRx[UART_S].m_nTxLen);

		//PRIMARY --------------------------------------
		MemSet(&gInst.m_tTxRx[UART_P].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cTxBuf));

		CmicM_MakeNPISyncTxData(UART_P, cmd);
		gInst.m_tTxRx[UART_P].m_nTxLen = gInst.m_ReqFrame[UART_P].m_NPILen;
		UART_SendData(UART_P, gInst.m_tTxRx[UART_P].m_cTxBuf, gInst.m_tTxRx[UART_P].m_nTxLen);

		gInst.m_tSt.m_BootSub.m_eNetwork++;
		gInst.m_nDelay=0;
	}
}

/********************************************************************************
FUNCTION :  CmicM_Network_ST3_RES()
STATUS :   BOOTSUB_NETWORK_E =  eNETWORK_ST3_RES
DESCRIPTION : eCMD_CF_SETPARAMS(0x4E)-0x01(DualRole)-0x00(Success)수신
**********************************************************************************/
static void CmicM_Network_ST3_RES(void)
{
	static  eRECV_Status	rStat = NO_RECV;

	//Secondary Check --------------------------------------------------------------
	if (FALSE == (rStat & S_RECV))
	{
		if (UART_Polling_Cb(UART_S))
		{			
			MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
		 	UART_ReceiveData(UART_S, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);

			if (CmicM_VerifyRx_CFGData(UART_S))
			{	// CmdID check
				if(gInst.m_RespFrame[UART_S].m_uCmdID == eCMD_CF_SETPARAMS )
				{	//Status-Success check
					if((gInst.m_RespFrame[UART_S].m_uBQ_DATA[0] == 0x01) && //Dual WM Role
						(gInst.m_RespFrame[UART_S].m_uBQ_DATA[1] == 0x00)) //macStatus
					{
						rStat |= S_RECV;
					}
				}
			}		 
		}
	}

	//Primary Check ----------------------------------------------------------------
	if (FALSE == (rStat & P_RECV))
	{
		if (UART_Polling_Cb(UART_P))
		{			
			MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
		 	UART_ReceiveData(UART_P, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);

			if (CmicM_VerifyRx_CFGData(UART_P))
			{	// CmdID check
				if(gInst.m_RespFrame[UART_P].m_uCmdID == eCMD_CF_SETPARAMS )
				{	//Status-Success check
					if((gInst.m_RespFrame[UART_P].m_uBQ_DATA[0] == 0x01) && //Dual WM Role
						(gInst.m_RespFrame[UART_P].m_uBQ_DATA[1] == 0x00)) //macStatus
					{
						rStat |= P_RECV;
					}
				}
			}		 
		}
	}

	if (rStat == ALL_RECV )
	{
		rStat = NO_RECV;
		gInst.m_tSt.m_BootSub.m_eNetwork++;	
	}
}

/********************************************************************************
FUNCTION :  CmicM_Network_ST4_REQ()
STATUS :   BOOTSUB_NETWORK_E =  eNETWORK_ST4_REQ
DESCRIPTION : Command : eCMD_CF_SETMAINCONFIG (0x40) : 
               networkID-Device수-Uplink/DownlinkTime-Retry-KeepAlive-DenyList
**********************************************************************************/
static void CmicM_Network_ST4_REQ(void)
{
	WBMS_COMMAND_E cmd;
	cmd = eCMD_CF_SETMAINCONFIG;

	// 10ms Delay 후 3번째 커맨드 전송
	if(++gInst.m_nDelay > 5)
	{
		//SECONDARY -----------------------------------
		MemSet(&gInst.m_tTxRx[UART_S].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cTxBuf));

		CmicM_MakeNPISyncTxData(UART_S, cmd);
		gInst.m_tTxRx[UART_S].m_nTxLen = gInst.m_ReqFrame[UART_S].m_NPILen;
		UART_SendData(UART_S, gInst.m_tTxRx[UART_S].m_cTxBuf, gInst.m_tTxRx[UART_S].m_nTxLen);

		//PRIMARY --------------------------------------
		MemSet(&gInst.m_tTxRx[UART_P].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cTxBuf));

		CmicM_MakeNPISyncTxData(UART_P, cmd);
		gInst.m_tTxRx[UART_P].m_nTxLen = gInst.m_ReqFrame[UART_P].m_NPILen;
		UART_SendData(UART_P, gInst.m_tTxRx[UART_P].m_cTxBuf, gInst.m_tTxRx[UART_P].m_nTxLen);

		gInst.m_tSt.m_BootSub.m_eNetwork++;
		gInst.m_nDelay=0;
	}
}

/********************************************************************************
FUNCTION :  CmicM_Network_ST4_RES()
STATUS :   BOOTSUB_NETWORK_E =  eNETWORK_ST4_RES
DESCRIPTION : Response : eCMD_CF_SETMAINCONFIG (0x40) : 수신
           networkID-Device수-Uplink/DownlinkTime-Retry-KeepAlive-DenyList     
**********************************************************************************/
static void CmicM_Network_ST4_RES(void)
{
	static  eRECV_Status	rStat = NO_RECV;

	 //Secondary Check ------------------------------------------------------------
	if (FALSE == (rStat & S_RECV))
	{
		if (UART_Polling_Cb(UART_S))
		{			
			MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
		 	UART_ReceiveData(UART_S, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);

			if (CmicM_VerifyRx_CFGData(UART_S))
			{
				if(gInst.m_RespFrame[UART_S].m_uCmdID == eCMD_CF_SETMAINCONFIG )
				{					
						rStat |= S_RECV;						
				}
			}	 
		}
	}

	//Primary Check ------------------------------------------------------------------
	if (FALSE == (rStat & P_RECV))
	{
		if (UART_Polling_Cb(UART_P))
		{			
			MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
		 	UART_ReceiveData(UART_P, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);

			if (CmicM_VerifyRx_CFGData(UART_P))
			{
				if(gInst.m_RespFrame[UART_P].m_uCmdID == eCMD_CF_SETMAINCONFIG )
				{
						rStat |= P_RECV;			
				}
			}
		}
	}

	if (rStat == ALL_RECV )
	{
		rStat = NO_RECV;
		gInst.m_tSt.m_BootSub.m_eNetwork++;
	}
}

/********************************************************************************
FUNCTION :  CmicM_Network_ST5_REQ()
STATUS :   BOOTSUB_NETWORK_E =  eNETWORK_ST5_REQ
DESCRIPTION : Command : eCMD_CF_SETJOINMODE(0x48) : Selective(0x01)
**********************************************************************************/
static void CmicM_Network_ST5_REQ(void)
{
	WBMS_COMMAND_E cmd;
	cmd = eCMD_CF_SETJOINMODE;

	// ms Delay 후  번째 커맨드 전송
	if(++gInst.m_nDelay > 5)
	{
		//SECONDARY -----------------------------------
		MemSet(&gInst.m_tTxRx[UART_S].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cTxBuf));

		CmicM_MakeNPISyncTxData(UART_S, cmd);
		gInst.m_tTxRx[UART_S].m_nTxLen = gInst.m_ReqFrame[UART_S].m_NPILen;
		UART_SendData(UART_S, gInst.m_tTxRx[UART_S].m_cTxBuf, gInst.m_tTxRx[UART_S].m_nTxLen);

		//PRIMARY --------------------------------------
		MemSet(&gInst.m_tTxRx[UART_P].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cTxBuf));

		CmicM_MakeNPISyncTxData(UART_P, cmd);
		gInst.m_tTxRx[UART_P].m_nTxLen = gInst.m_ReqFrame[UART_P].m_NPILen;
		UART_SendData(UART_P, gInst.m_tTxRx[UART_P].m_cTxBuf, gInst.m_tTxRx[UART_P].m_nTxLen);

		gInst.m_tSt.m_BootSub.m_eNetwork++;
		gInst.m_nDelay=0;
	}
}

/********************************************************************************
FUNCTION :  CmicM_Network_ST5_RES()
STATUS :   BOOTSUB_NETWORK_E =  eNETWORK_ST5_RES
DESCRIPTION : Response: eCMD_CF_SETJOINMODE(0x48) : Success(0x00)
**********************************************************************************/
static void CmicM_Network_ST5_RES(void)
{
	static  eRECV_Status	rStat = NO_RECV;

	//Secondary Check-----------------------------------------------------------------
	if (FALSE == (rStat & S_RECV))
	{
		if (UART_Polling_Cb(UART_S))
		{			
			MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
		 	UART_ReceiveData(UART_S, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);

			if (CmicM_VerifyRx_CFGData(UART_S))
			{				
				if (gInst.m_RespFrame[UART_S].m_uCmdID == eCMD_CF_SETJOINMODE)
				{
					if((gInst.m_RespFrame[UART_S].m_uBQ_DATA[0] == 0x00 )) //macStatus=OK
					{
						rStat |= S_RECV;	
					}
				}
			}		
		}
	}

	//Primary Check -------------------------------------------------------------------
	if (FALSE == (rStat & P_RECV))
	{
		if (UART_Polling_Cb(UART_P))
		{			
			MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
		 	UART_ReceiveData(UART_P, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);

			if (CmicM_VerifyRx_CFGData(UART_P))
			{
				if (gInst.m_RespFrame[UART_P].m_uCmdID == eCMD_CF_SETJOINMODE)
				{
					if((gInst.m_RespFrame[UART_P].m_uBQ_DATA[0] == 0x00)) //macStatus=OK
					{
						rStat |= P_RECV;	
					}
				}
			}		 
		}
	}

	if (rStat == ALL_RECV )
	{
		rStat = NO_RECV;
		gInst.m_tSt.m_BootSub.m_eNetwork++;	
	}
}

/********************************************************************************
FUNCTION :  CmicM_Network_ST6_REQ()
STATUS :   BOOTSUB_NETWORK_E =  eNETWORK_ST6_REQ
DESCRIPTION : Command : eCMD_CF_SETDEVTBLCFG (0x49) : MAC Addr전송(1st).
**********************************************************************************/
static void CmicM_Network_ST6_REQ(void)
{
	WBMS_COMMAND_E cmd;
	cmd = eCMD_CF_SETDEVTBLCFG;

	// ms Delay 후 번째 커맨드 전송
	if(++gInst.m_nDelay > 5)
	{
		//SECONDARY -----------------------------------
		MemSet(&gInst.m_tTxRx[UART_S].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cTxBuf));

		CmicM_MakeNPISyncTxData(UART_S, cmd);
		gInst.m_tTxRx[UART_S].m_nTxLen = gInst.m_ReqFrame[UART_S].m_NPILen;
		UART_SendData(UART_S, gInst.m_tTxRx[UART_S].m_cTxBuf, gInst.m_tTxRx[UART_S].m_nTxLen);

		//PRIMARY --------------------------------------
		MemSet(&gInst.m_tTxRx[UART_P].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cTxBuf));

		CmicM_MakeNPISyncTxData(UART_P, cmd);
		gInst.m_tTxRx[UART_P].m_nTxLen = gInst.m_ReqFrame[UART_P].m_NPILen;
		UART_SendData(UART_P, gInst.m_tTxRx[UART_P].m_cTxBuf, gInst.m_tTxRx[UART_P].m_nTxLen);

		gInst.m_tSt.m_BootSub.m_eNetwork++;
		gInst.m_nDelay=0;
	}	
}

/********************************************************************************
FUNCTION :  CmicM_Network_ST6_RES()
STATUS :   BOOTSUB_NETWORK_E =  eNETWORK_ST6_RES
DESCRIPTION : Response : eCMD_CF_SETDEVTBLCFG (0x49) : MAC Status(Success=0)수신
			  MAX_NODE 15개 초과시 ST6_1로 분기.
**********************************************************************************/
static void CmicM_Network_ST6_RES(void)
{
	static  eRECV_Status	rStat = NO_RECV;

	//Secondary Check-------------------------------------------------------------------
	if (FALSE == (rStat & S_RECV))
	{
		if (UART_Polling_Cb(UART_S))
		{
			MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
		 	UART_ReceiveData(UART_S, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);

			if (CmicM_VerifyRx_CFGData(UART_S))
			{	//3. CmdID check.
				if (gInst.m_RespFrame[UART_S].m_uCmdID == eCMD_CF_SETDEVTBLCFG)
				{
					if((gInst.m_RespFrame[UART_S].m_uBQ_DATA[0] == 0x00 )) //macStatus=OK
					{
						rStat |= S_RECV;	
					}
				}
			}
		}
	}

	//Primary Check-----------------------------------------------------------------------
	if (FALSE == (rStat & P_RECV))
	{
		if (UART_Polling_Cb(UART_P))
		{			
			MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
		 	UART_ReceiveData(UART_P, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);

			if (CmicM_VerifyRx_CFGData(UART_P))
			{	//3. CmdID check.
				if (gInst.m_RespFrame[UART_P].m_uCmdID == eCMD_CF_SETDEVTBLCFG)
				{
					if((gInst.m_RespFrame[UART_P].m_uBQ_DATA[0] == 0x00)) //macStatus=OK
				 	{
						rStat |= P_RECV;	
				 	}
				}
			}		 
		}
	}

	if (rStat == ALL_RECV )
	{
		rStat = NO_RECV;

		if( MAX_NUM_DEVS > 15 ){
			gInst.m_tSt.m_BootSub.m_eNetwork = eNETWORK_ST6_1_REQ;
		}else {
			gInst.m_tSt.m_BootSub.m_eNetwork = eNETWORK_ST7_REQ;
		}
	}
}

/********************************************************************************
FUNCTION :  CmicM_Network_ST6_1_REQ()
STATUS :   BOOTSUB_NETWORK_E =  eNETWORK_ST6_1_REQ
DESCRIPTION : 30개 노드처리를 위해 데이터를 분리해서 보냄
          Command : eCMD_CF_SETDEVTBLCFG_2nd (0x80) 
**********************************************************************************/
static void CmicM_Network_ST6_1_REQ(void)
{
	WBMS_COMMAND_E cmd;
	cmd = eCMD_CF_SETDEVTBLCFG_2nd;

	// ms Delay 후 번째 커맨드 전송
	if(++gInst.m_nDelay > 5){

		//SECONDARY -----------------------------------
		MemSet(&gInst.m_tTxRx[UART_S].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cTxBuf));

		CmicM_MakeNPISyncTxData(UART_S, cmd);
		gInst.m_tTxRx[UART_S].m_nTxLen = gInst.m_ReqFrame[UART_S].m_NPILen;
		UART_SendData(UART_S, gInst.m_tTxRx[UART_S].m_cTxBuf, gInst.m_tTxRx[UART_S].m_nTxLen);

		//PRIMARY --------------------------------------
		MemSet(&gInst.m_tTxRx[UART_P].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cTxBuf));

		CmicM_MakeNPISyncTxData(UART_P, cmd);
		gInst.m_tTxRx[UART_P].m_nTxLen = gInst.m_ReqFrame[UART_P].m_NPILen;
		UART_SendData(UART_P, gInst.m_tTxRx[UART_P].m_cTxBuf, gInst.m_tTxRx[UART_P].m_nTxLen);

		gInst.m_tSt.m_BootSub.m_eNetwork++;
		gInst.m_nDelay=0;
	}	
}

/********************************************************************************
FUNCTION :  CmicM_Network_ST6_1_RES()
STATUS :   BOOTSUB_NETWORK_E =  eNETWORK_ST6_1_RES
DESCRIPTION : 15이후 노드의 수신.
         Response :eCMD_CF_SETDEVTBLCFG (0x49) - MAC Status(0x00)
**********************************************************************************/
static void CmicM_Network_ST6_1_RES(void)
{
	static  eRECV_Status	rStat = NO_RECV;

	//Secondary Check--------------------------------------------------------------------
	if (FALSE == (rStat & S_RECV))
	{
		if (UART_Polling_Cb(UART_S))
		{			
			MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
		 	UART_ReceiveData(UART_S, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);

			if (CmicM_VerifyRx_CFGData(UART_S))
			{	//3. CmdID check.
				if (gInst.m_RespFrame[UART_S].m_uCmdID == eCMD_CF_SETDEVTBLCFG)
				{
					if((gInst.m_RespFrame[UART_S].m_uBQ_DATA[0] == 0x00 )) //macStatus=OK
					{
						rStat |= S_RECV;	
					}
				}
			}		
		}
	}

	//Primary Check
	if (FALSE == (rStat & P_RECV))
	{
		if (UART_Polling_Cb(UART_P))
		{			
			MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
		 	UART_ReceiveData(UART_P, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);

			if (CmicM_VerifyRx_CFGData(UART_P))
			{	//3. CmdID check.
				if (gInst.m_RespFrame[UART_P].m_uCmdID == eCMD_CF_SETDEVTBLCFG)
				{
					if((gInst.m_RespFrame[UART_P].m_uBQ_DATA[0] == 0x00)) //macStatus=OK
				 	{
						rStat |= P_RECV;	
				 	}
				}
			}		 
		}
	}

	if (rStat == ALL_RECV )
	{
		rStat = NO_RECV;
		gInst.m_tSt.m_BootSub.m_eNetwork++;		
	}
}

/********************************************************************************
FUNCTION :  CmicM_Network_ST7_REQ()
STATUS :   BOOTSUB_NETWORK_E =  eNETWORK_ST7_REQ
DESCRIPTION : Command : eCMD_CF_STARTNETWORK(0x42) - Timeout(4Byte)
**********************************************************************************/
static void CmicM_Network_ST7_REQ(void)
{
	WBMS_COMMAND_E cmd;
	cmd = eCMD_CF_STARTNETWORK;

	// ms Delay 후 번째 커맨드 전송
	if(++gInst.m_nDelay > 5)
	{
		//SECONDARY -----------------------------------
		MemSet(&gInst.m_tTxRx[UART_S].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cTxBuf));

		CmicM_MakeNPISyncTxData(UART_S, cmd);
		gInst.m_tTxRx[UART_S].m_nTxLen = gInst.m_ReqFrame[UART_S].m_NPILen;
		UART_SendData(UART_S, gInst.m_tTxRx[UART_S].m_cTxBuf, gInst.m_tTxRx[UART_S].m_nTxLen);

		//PRIMARY --------------------------------------
		MemSet(&gInst.m_tTxRx[UART_P].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cTxBuf));

		CmicM_MakeNPISyncTxData(UART_P, cmd);
		gInst.m_tTxRx[UART_P].m_nTxLen = gInst.m_ReqFrame[UART_P].m_NPILen;
		UART_SendData(UART_P, gInst.m_tTxRx[UART_P].m_cTxBuf, gInst.m_tTxRx[UART_P].m_nTxLen);

		gInst.m_tSt.m_BootSub.m_eNetwork++;
		gInst.m_nDelay=0;
	}	
}

/********************************************************************************
FUNCTION :  CmicM_Network_ST7_RES()
STATUS :   BOOTSUB_NETWORK_E =  eNETWORK_ST7_RES
DESCRIPTION : Response : eCMD_CF_STARTNETWORK(0x42) - MacStatus
                         eCMD_CB_STATECHANGE(0x22) - NodeID(1)+UniqueID(8)+MAC_DeviceXX(1) : 노드수만큼
						 eCMD_CB_DEVICEPAIRIND(0x20) - NetworkID(2)+UniqueID(8)+NodeID(1) : 노드수만큼
						 eCMD_CB_FORMATION (0x26) - NetworkFormationTime(8)+MacStatus
**********************************************************************************/
static void CmicM_Network_ST7_RES(void)
{
	static  eRECV_Status	rStat = NO_RECV;
	static  uint8_t 		node[2] = {0, 0};
	uint8_t					uNodeId;

	//Secondary Check----------------------------------------------------------------------------
	if (FALSE == (rStat & S_RECV))
	{
		if (UART_Polling_Cb(UART_S))
		{			
			MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
		 	UART_ReceiveData(UART_S, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);

			if (CmicM_VerifyRx_CFGData(UART_S))
			{
				switch(gInst.m_RespFrame[UART_S].m_uCmdID)
				{
					case eCMD_CF_STARTNETWORK :
					break;
					case eCMD_CB_STATECHANGE :
						uNodeId = gInst.m_RespFrame[UART_S].m_uBQ_DATA[0];
						gInst.m_tWbmsState[UART_S].m_zWdPairState[uNodeId].m_MAC_DeviceState = 
							gInst.m_RespFrame[UART_S].m_uBQ_DATA[9];
					break;

					case eCMD_CB_DEVICEPAIRIND :
						node[UART_S]++;
						uNodeId = gInst.m_RespFrame[UART_S].m_uBQ_DATA[10];
						// NWID, UniqueID, SlotInfo(NodeID) copy
						gInst.m_tWbmsState[UART_S].m_zWdPairState[uNodeId].m_uSlotInfo = uNodeId;
						MemCpy(&(gInst.m_tWbmsState[UART_S].m_zWdPairState[uNodeId].m_uNetworkId),
							    &gInst.m_RespFrame[UART_S].m_uBQ_DATA[0], 2);	
						MemCpy(&(gInst.m_tWbmsState[UART_S].m_zWdPairState[uNodeId].m_zDevUniqueID[0]), 
						        &gInst.m_RespFrame[UART_S].m_uBQ_DATA[2], COMM_DEVICE_UID_SIZE);
					break;

					case  eCMD_CB_FORMATION :
						if (node[UART_S] >= (MAX_NUM_DEVS /2))
						{
							node[UART_S] = 0;							
							rStat |= S_RECV;	
							gInst.m_tInfo.m_qNwFormTime_S = (Bytes2Uint32(&gInst.m_RespFrame[UART_S].m_uBQ_DATA[0])/1000000.0);
						}
					break;

					default : 
					break;
				}
			}	
		}
	}

	//Primary Check-----------------------------------------------------------------------------------
	if (FALSE == (rStat & P_RECV))
	{
		if (UART_Polling_Cb(UART_P))
		{			
			MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
		 	UART_ReceiveData(UART_P, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);

			if (CmicM_VerifyRx_CFGData(UART_P))
			{
				switch(gInst.m_RespFrame[UART_P].m_uCmdID)
				{
					case eCMD_CF_STARTNETWORK :
					break;
					case eCMD_CB_STATECHANGE :
						uNodeId = gInst.m_RespFrame[UART_P].m_uBQ_DATA[0];
						gInst.m_tWbmsState[UART_P].m_zWdPairState[uNodeId].m_MAC_DeviceState = 
						gInst.m_RespFrame[UART_P].m_uBQ_DATA[9];
					break;

					case eCMD_CB_DEVICEPAIRIND :
						node[UART_P]++;
						uNodeId = gInst.m_RespFrame[UART_P].m_uBQ_DATA[10];
					// NWID, UniqueID, SlotInfo(NodeID) copy
						gInst.m_tWbmsState[UART_P].m_zWdPairState[uNodeId].m_uSlotInfo = uNodeId;
						MemCpy(&(gInst.m_tWbmsState[UART_P].m_zWdPairState[uNodeId].m_uNetworkId),
							    &gInst.m_RespFrame[UART_P].m_uBQ_DATA[0], 2);	
						MemCpy(&(gInst.m_tWbmsState[UART_P].m_zWdPairState[uNodeId].m_zDevUniqueID[0]), 
						        &gInst.m_RespFrame[UART_P].m_uBQ_DATA[2], COMM_DEVICE_UID_SIZE);
					break;

					case  eCMD_CB_FORMATION :						
						if (node[UART_P] >= (MAX_NUM_DEVS /2))
						{
							node[UART_P] = 0;							
							rStat |= P_RECV;	
							gInst.m_tInfo.m_qNwFormTime_P = (Bytes2Uint32(&gInst.m_RespFrame[UART_P].m_uBQ_DATA[0])/1000000.0);
						}
					break;

					default : 
					break;
				}
			}		
		}
	}
	
	if (rStat == ALL_RECV)
	{
		rStat = NO_RECV;
		gInst.m_tSt.m_BootSub.m_eNetwork++;		
	}
}

/********************************************************************************
FUNCTION :  CmicM_Network_ST8_REQ()
STATUS :   BOOTSUB_NETWORK_E =  eNETWORK_ST8_REQ
DESCRIPTION : Command : eCMD_WM_NUM_WD_CONN (0x01) : 연결노드개수확인
**********************************************************************************/// 
static void CmicM_Network_ST8_REQ(void)
{
	WBMS_COMMAND_E cmd;
	cmd = eCMD_WM_NUM_WD_CONN;
	
	// 10ms Delay 후 3번째 커맨드 전송
	if(++gInst.m_nDelay > 10)
	{
	//SECONDARY ----------------------------------------------------------------------
		MemSet(&gInst.m_tTxRx[UART_S].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cTxBuf));

		CmicM_MakeNPISyncTxData(UART_S, cmd);
		gInst.m_tTxRx[UART_S].m_nTxLen = gInst.m_ReqFrame[UART_S].m_NPILen;
		UART_SendData(UART_S, gInst.m_tTxRx[UART_S].m_cTxBuf, gInst.m_tTxRx[UART_S].m_nTxLen);

	//PRIMARY --------------------------------------
		MemSet(&gInst.m_tTxRx[UART_P].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cTxBuf));

		CmicM_MakeNPISyncTxData(UART_P, cmd);
		gInst.m_tTxRx[UART_P].m_nTxLen = gInst.m_ReqFrame[UART_P].m_NPILen;
		UART_SendData(UART_P, gInst.m_tTxRx[UART_P].m_cTxBuf, gInst.m_tTxRx[UART_P].m_nTxLen);

		gInst.m_tSt.m_BootSub.m_eNetwork++;
		gInst.m_nDelay=0;
	}
}

/********************************************************************************
FUNCTION :  CmicM_Network_ST8_RES()
STATUS :   BOOTSUB_NETWORK_E =  eNETWORK_ST8_RES
DESCRIPTION :Response : eCMD_WM_NUM_WD_CONN (0x01) -
**********************************************************************************/
static void CmicM_Network_ST8_RES(void)
{
	static  eRECV_Status	rStat = NO_RECV;

	//Secondary Check-------------------------------------------------------------------
	if (FALSE == (rStat & S_RECV))
	{
		if (UART_Polling_Cb(UART_S))
		{		
			MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
		 	UART_ReceiveData(UART_S, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);

			if (CmicM_VerifyRx_CFGData(UART_S))
			{
				if (gInst.m_RespFrame[UART_S].m_uCmdID == eCMD_WM_NUM_WD_CONN)
				{	// NumOfConn, m_uNAFEs에 연결된 노드수를 저장.	
					gInst.m_tWbmsState[UART_S].m_uNumOfConn = gInst.m_RespFrame[UART_S].m_uBQ_DATA[0];	
					gInst.m_uNAFEs[UART_S] = gInst.m_RespFrame[UART_S].m_uBQ_DATA[0];
					rStat |= S_RECV;	
				}
			}		
		}
	}

	//Primary Check----------------------------------------------------------------------
	if (FALSE == (rStat & P_RECV))
	{
		if (UART_Polling_Cb(UART_P))
		{			
			MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
		 	UART_ReceiveData(UART_P, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);

			if (CmicM_VerifyRx_CFGData(UART_P))
			{
				if (gInst.m_RespFrame[UART_P].m_uCmdID == eCMD_WM_NUM_WD_CONN)
				{
					// NumOfConn, m_uNAFEs에 연결된 노드수를 저장.
					gInst.m_tWbmsState[UART_P].m_uNumOfConn = gInst.m_RespFrame[UART_P].m_uBQ_DATA[0];	
					gInst.m_uNAFEs[UART_P] = gInst.m_RespFrame[UART_P].m_uBQ_DATA[0];
					rStat |= P_RECV;	
				}
			}		 
		}
	}

	if (rStat == ALL_RECV )
	{
		rStat = NO_RECV;
		gInst.m_tSt.m_BootSub.m_eNetwork = eNETWORK_ST0_IDLE; //Network state go to idle
		gInst.m_tSt.m_eBoot = eBOOT_BQREGISTER;
		gInst.m_tSt.m_BootSub.m_eBQRegister = eBQREG_ST1_REQ; //eBQREG_ST0_IDLE;	
	}	
}

/********************************************************************************
FUNCTION :  CmicM_BQRegister_ST0_IDLE()
STATUS :   BOOTSUB_BQREGISTER_E =  eBQREG_ST0_IDLE
DESCRIPTION : N.A.
**********************************************************************************/
static void	CmicM_BQRegister_ST0_IDLE(void)
{
	gInst.m_tSt.m_BootSub.m_eBQRegister = eBQREG_ST1_REQ;
	gInst.m_nDelay=0;
}

/********************************************************************************
FUNCTION :  CmicM_BQRegister_ST1_REQ()
STATUS :   BOOTSUB_BQREGISTER_E =  eBQREG_ST1_REQ
DESCRIPTION : BQ79718 Register 초기화(Bq7971x_ConfigInit): NVM
**********************************************************************************/
static void	CmicM_BQRegister_ST1_REQ(void)
{
	CmicM_BQ7971x_ConfigInit(UART_S);
	CmicM_BQ7971x_ConfigInit(UART_P);

	gInst.m_tSt.m_BootSub.m_eBQRegister = eBQREG_ST1_RES;
}

/********************************************************************************
FUNCTION :  CmicM_BQRegister_ST1_RES()
STATUS :   BOOTSUB_BQREGISTER_E =  eBQREG_ST1_RES
DESCRIPTION : BQ79718 Register ConfigInit(NVM)에 대한 수신(MAC Status)
**********************************************************************************/
static void	CmicM_BQRegister_ST1_RES(void)
{
	static  eRECV_Status	rStat = NO_RECV;

	//Secondary Check------------------------------------------------------------------------
	if (FALSE == (rStat & S_RECV))
	{
		if (UART_Polling_Cb(UART_S))
		{
			MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
			UART_ReceiveData(UART_S, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);

			switch(gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_IDX_CMDID])
			{				
				case eCMD_CB_TXCNF :					
					if (gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_MAC_STATUS_IDX] == 0x00)
					{					
						rStat |= S_RECV;									
					}
				break;
				
				default :	break;
			}
		}
	}	

	//Primary Check--------------------------------------------------------------------------
	if (FALSE == (rStat & P_RECV))
	{
		if (UART_Polling_Cb(UART_P))
		{
			MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
			UART_ReceiveData(UART_P, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);

			switch(gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_IDX_CMDID])
			{				
				case eCMD_CB_TXCNF :				
					if (gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_MAC_STATUS_IDX] == 0x00)
					{
						rStat |= P_RECV;											
					}
				break;
				
				default :	break;
			}
		}
	}

	if (rStat == ALL_RECV )
	{
		rStat = NO_RECV;
		gInst.m_tSt.m_BootSub.m_eBQRegister = eBQREG_ST2_REQ;	
	}
}

/********************************************************************************
FUNCTION :  CmicM_BQRegister_ST2_REQ()
STATUS :   BOOTSUB_BQREGISTER_E =  eBQREG_ST2_REQ
DESCRIPTION : FAULT_RST2 / DIAG_ADC_CTRL / ADC_CTRL1 설정
**********************************************************************************/
/* Diag StartupInit */
static void	CmicM_BQRegister_ST2_REQ(void)
{
	uint16  nIdx=0;
	uint16  nRetIdx=0;
	uint8_t   buf[10];

	//SECONDARY ------------------------------------------------------------------------
	// Tx Buffer Clear.
	MemSet(&gInst.m_tTxRx[UART_S].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cTxBuf));
	gInst.m_tTxRx[UART_S].m_nTxLen = 0;

	 //1. FAULT_RST2 - b7=1(Reset the registers to 0). 
	MemSet(buf, 0, sizeof(buf));
	buf[0] = BQ_FAULT_RST2_RST_ADC_MSK; 
	
	nRetIdx = CmicM_MakeBQFrame(UART_S, (BROADCAST_WRITE), 
								BQ_FAULT_RST2_OFFSET, buf, NPI_HEADER_SIZE );	
	nIdx = nRetIdx;

	//2. DIAG_ADC_CTRL 
	MemSet(buf, 0, sizeof(buf));
	MemCpy(buf, &bqConfig.m_tCtrl.uDiag_Adc_Ctrl[0], WRITE_2BYTE); //2bytes
	
	nRetIdx = CmicM_MakeBQFrame(UART_S, (BROADCAST_WRITE | WRITE_1BYTE), 
								BQ_DIAG_ADC_CTRL1_OFFSET, buf, nIdx );	
	nIdx = nRetIdx;						  

	//3. ADC_CTRL1
	MemSet(buf, 0, sizeof(buf));
	buf[0] = bqConfig.m_tCtrl.uAdc_Ctrl[0];
	buf[1] = (uint8_t)(bqConfig.m_tCtrl.uAdc_Ctrl[1] | BQ_ADC_CTRL2_ADC_GO_MSK) ;
	
	nRetIdx = CmicM_MakeBQFrame(UART_S, (BROADCAST_WRITE | WRITE_1BYTE), 
								BQ_ADC_CTRL1_OFFSET, buf, nIdx );	
	nIdx = nRetIdx;						  
			
	CmicM_MakeNPIFrameHeader(UART_S, nIdx); 

	gInst.m_tTxRx[UART_S].m_cTxBuf[nIdx++] = CmicM_CalculateFCS(&gInst.m_tTxRx[UART_S].m_cTxBuf[1], nIdx-1);
	gInst.m_tTxRx[UART_S].m_nTxLen = nIdx;
	UART_SendData(UART_S, gInst.m_tTxRx[UART_S].m_cTxBuf, nIdx);
	

	//PRIMARY -------------------------------------------------------------------------
	nIdx=nRetIdx=0;

	// Tx Buffer Clear.
	MemSet(&gInst.m_tTxRx[UART_P].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cTxBuf));
	gInst.m_tTxRx[UART_P].m_nTxLen = 0;

	 //1. FAULT_RST2 - b7=1(Reset the registers to 0). 
	MemSet(buf, 0, sizeof(buf));
	buf[0] = BQ_FAULT_RST2_RST_ADC_MSK; 
	
	nRetIdx = CmicM_MakeBQFrame(UART_P, (BROADCAST_WRITE), 
								BQ_FAULT_RST2_OFFSET, buf, NPI_HEADER_SIZE );	
	nIdx = nRetIdx;

	//2. DIAG_ADC_CTRL 
	MemSet(buf, 0, sizeof(buf));
	MemCpy(buf, &bqConfig.m_tCtrl.uDiag_Adc_Ctrl[0], WRITE_2BYTE); //2bytes
	
	nRetIdx = CmicM_MakeBQFrame(UART_P, (BROADCAST_WRITE | WRITE_1BYTE), 
								BQ_DIAG_ADC_CTRL1_OFFSET, buf, nIdx );	
	nIdx = nRetIdx;						  

	//3. ADC_CTRL1
	MemSet(buf, 0, sizeof(buf));
	buf[0] = bqConfig.m_tCtrl.uAdc_Ctrl[0];
	buf[1] = (uint8_t)(bqConfig.m_tCtrl.uAdc_Ctrl[1] | BQ_ADC_CTRL2_ADC_GO_MSK) ;
	
	nRetIdx = CmicM_MakeBQFrame(UART_P, (BROADCAST_WRITE | WRITE_1BYTE), 
								BQ_ADC_CTRL1_OFFSET, buf, nIdx );	
	nIdx = nRetIdx;						  
			
	CmicM_MakeNPIFrameHeader(UART_P, nIdx); 

	gInst.m_tTxRx[UART_P].m_cTxBuf[nIdx++] = CmicM_CalculateFCS(&gInst.m_tTxRx[UART_P].m_cTxBuf[1], nIdx-1);
	gInst.m_tTxRx[UART_P].m_nTxLen = nIdx;
	UART_SendData(UART_P, gInst.m_tTxRx[UART_P].m_cTxBuf, nIdx);
	//------------------------------------------------------------------

	gInst.m_tSt.m_BootSub.m_eBQRegister = eBQREG_ST2_RES;
	gInst.m_nDelay=0;
		
}	

/********************************************************************************
FUNCTION :  CmicM_BQRegister_ST2_RES()
STATUS :   BOOTSUB_BQREGISTER_E =  eBQREG_ST2_RES
DESCRIPTION : BQ79718 Register(DIAG_ADC_CTRL/ADC_CTRL)에대한 수신(MAC Status)
**********************************************************************************/
static void	CmicM_BQRegister_ST2_RES(void)
{
	static  eRECV_Status	rStat = NO_RECV;

	//Secondary Check-------------------------------------------------------------------------
	if (FALSE == (rStat & S_RECV))
	{
		if (UART_Polling_Cb(UART_S))
		{
			MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
			UART_ReceiveData(UART_S, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);

			switch(gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_IDX_CMDID])
			{				
				case eCMD_CB_TXCNF :
					//if ((gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_MAC_STATUS_IDX] == 0x00))
					{ 
						rStat |= S_RECV;
					}
				break;
				case eCMD_CB_FAULT : //DIAG FAULT CALLBACK
					gInst.m_nDiagFaultCnt[UART_S]++;
				break;
				default :		break;
			}
		}
	}	

	//Primary Check--------------------------------------------------------------------------
	if (FALSE == (rStat & P_RECV))
	{
		if (UART_Polling_Cb(UART_P))
		{
			MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
			UART_ReceiveData(UART_P, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);

			switch(gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_IDX_CMDID])
			{				
				case eCMD_CB_TXCNF :					
					//if (gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_MAC_STATUS_IDX] == 0x00)					
					{
						rStat |= P_RECV;
					}
				break;
				case eCMD_CB_FAULT : //DIAG FAULT CALLBACK
					gInst.m_nDiagFaultCnt[UART_P]++;
				break;
				default :		break;
			}
		}
	}



	if (rStat == ALL_RECV )
	{
		rStat = NO_RECV;
		gInst.m_tSt.m_BootSub.m_eBQRegister = eBQREG_ST3_REQ;	
	}
}

/********************************************************************************
FUNCTION :  CmicM_BQRegister_ST3_REQ()
STATUS :   BOOTSUB_BQREGISTER_E =  eBQREG_ST3_REQ
DESCRIPTION : Xms Delay후 DEV_STAT1_OFFSET 읽기.
**********************************************************************************/
static void	CmicM_BQRegister_ST3_REQ(void)
{
	uint16  nIdx=0;
	uint16  nRetIdx=0;
	uint8_t   buf[5];
	
	// 6ms Delay(NG)->10ms로 변경
	if (++gInst.m_nDelay > 10)
	{
		//SECONDARY ------------------------------------------------------------------------	
		// Tx Buffer Clear.
		MemSet(&gInst.m_tTxRx[UART_S].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cTxBuf));
		gInst.m_tTxRx[UART_S].m_nTxLen = 0;

		// StackRead BQ_DEV_STAT1_OFFSET
		MemSet(buf, 0 , sizeof(buf));
		buf[0] = READ_1BYTE; //2byte read.

		nRetIdx = CmicM_MakeBQFrame(UART_S, (BROADCAST_READ), 
								BQ_DEV_STAT1_OFFSET, buf, NPI_HEADER_SIZE );	
		nIdx = nRetIdx;						  
			
		CmicM_MakeNPIFrameHeader(UART_S, nIdx); 

		gInst.m_tTxRx[UART_S].m_cTxBuf[nIdx++] = CmicM_CalculateFCS(&gInst.m_tTxRx[UART_S].m_cTxBuf[1], nIdx-1);
		gInst.m_tTxRx[UART_S].m_nTxLen = nIdx;
		UART_SendData(UART_S, gInst.m_tTxRx[UART_S].m_cTxBuf, nIdx);

		// BQ FrameData ReqFrame에 저장
		MemSet(&gInst.m_ReqFrame[UART_S], 0, sizeof(gInst.m_ReqFrame[UART_S]));
		gInst.m_ReqFrame[UART_S].m_uCmd = eCMD_WD_BQ_RAWFRAME;
		gInst.m_ReqFrame[UART_S].m_nBQ_REG_ADDR = BQ_DEV_STAT1_OFFSET;
		gInst.m_ReqFrame[UART_S].m_ReqDataLen = READ_1BYTE; //2byte read.
		//////////////////////////////////

		//PRIMARY ---------------------------------------------------------------------------	
		nIdx = nRetIdx = 0;

		// Tx Buffer Clear.
		MemSet(&gInst.m_tTxRx[UART_P].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cTxBuf));
		gInst.m_tTxRx[UART_P].m_nTxLen = 0;

		// StackRead BQ_DEV_STAT1_OFFSET
		MemSet(buf, 0 , sizeof(buf));
		buf[0] = READ_1BYTE; //2byte read.

		nRetIdx = CmicM_MakeBQFrame(UART_P, (BROADCAST_READ), 
								BQ_DEV_STAT1_OFFSET, buf, NPI_HEADER_SIZE );	
		nIdx = nRetIdx;						  
			
		CmicM_MakeNPIFrameHeader(UART_P, nIdx); 

		gInst.m_tTxRx[UART_P].m_cTxBuf[nIdx++] = CmicM_CalculateFCS(&gInst.m_tTxRx[UART_P].m_cTxBuf[1], nIdx-1);
		gInst.m_tTxRx[UART_P].m_nTxLen = nIdx;
		UART_SendData(UART_P, gInst.m_tTxRx[UART_P].m_cTxBuf, nIdx);

		// BQ FrameData ReqFrame에 저장
		MemSet(&gInst.m_ReqFrame[UART_P], 0, sizeof(gInst.m_ReqFrame[UART_P]));
		gInst.m_ReqFrame[UART_P].m_uCmd = eCMD_WD_BQ_RAWFRAME;
		gInst.m_ReqFrame[UART_P].m_nBQ_REG_ADDR = BQ_DEV_STAT1_OFFSET;
		gInst.m_ReqFrame[UART_P].m_ReqDataLen = READ_1BYTE; //2byte read.
		//////////////////////////////////

		gInst.m_tSt.m_BootSub.m_eBQRegister = eBQREG_ST3_RES;
		gInst.m_nDelay=0;
	}
}

/********************************************************************************
FUNCTION :  CmicM_BQRegister_ST3_RES()
STATUS :   BOOTSUB_BQREGISTER_E =  eBQREG_ST3_RES
DESCRIPTION : DEV_STAT1_OFFSET의 조건확인(GPADC_RUN/VCELLADC_RUN)
**********************************************************************************/
static void	CmicM_BQRegister_ST3_RES(void)
{
	static  eRECV_Status	rStat = NO_RECV;

	//Secondary Check-------------------------------------------------------------------------
	if (FALSE == (rStat & S_RECV))
	{
		if (UART_Polling_Cb(UART_S))
		{
			MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
			UART_ReceiveData(UART_S, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);

			if (CmicM_VerifyRx_BQData(UART_S))
			{				
				rStat |= S_RECV;						
			}
		}
	}

	//Primary Check---------------------------------------------------------------------------
	if (FALSE == (rStat & P_RECV))
	{
		if (UART_Polling_Cb(UART_P))
		{
			MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
			UART_ReceiveData(UART_P, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);

			if (CmicM_VerifyRx_BQData(UART_P))
			{
				rStat |= P_RECV;				
			}
		}
	}

	if (rStat == ALL_RECV )
	{
		rStat = NO_RECV;	
		gInst.m_tSt.m_BootSub.m_eBQRegister = eBQREG_ST4_REQ;	
	}
}

/********************************************************************************
FUNCTION :  CmicM_BQRegister_ST4_REQ()
STATUS :   BOOTSUB_BQREGISTER_E =  eBQREG_ST4_REQ
DESCRIPTION : BQ_DIAG_ADC_CTRL3_OFFSET/BQ_CUST_CRC_RSLT_HI_OFFSET 송신
**********************************************************************************/
static void	CmicM_BQRegister_ST4_REQ(void)
{
	uint16  nRetIdx=0;
	uint8_t   buf[10];

	//SECONDARY ----------------------------------------------------------------------------
	MemSet(&gInst.m_tTxRx[UART_S].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cTxBuf));
	gInst.m_tTxRx[UART_S].m_nTxLen = 0;

	//1. StackWrite BQ_DIAG_ADC_CTRL3_OFFSET
	MemSet(buf, 0, sizeof(buf));
	buf[0] = BQ_DIAG_ADC_CTRL3_DIAG_DIG_EN_MSK; 	
	nRetIdx = CmicM_MakeBQFrame(UART_S, BROADCAST_WRITE, 
								BQ_DIAG_ADC_CTRL3_OFFSET, buf, NPI_HEADER_SIZE );	
	
	//2. StackRead BQ_CUST_CRC_RSLT_HI_OFFSET
	MemSet(buf, 0 , sizeof(buf));
	buf[0] = READ_1BYTE; //2byte read.
	nRetIdx = CmicM_MakeBQFrame(UART_S, (BROADCAST_READ), 
							BQ_CUST_CRC_RSLT_HI_OFFSET, buf, nRetIdx );						  
	
	// BQ FrameData ReqFrame에 저장
	MemSet(&gInst.m_ReqFrame[UART_S], 0, sizeof(gInst.m_ReqFrame[UART_S]));
	gInst.m_ReqFrame[UART_S].m_uCmd = eCMD_WD_BQ_RAWFRAME;
	gInst.m_ReqFrame[UART_S].m_nBQ_REG_ADDR = BQ_CUST_CRC_RSLT_HI_OFFSET;
	gInst.m_ReqFrame[UART_S].m_ReqDataLen = READ_1BYTE; //2byte read.
	//////////////////////////////////

	CmicM_MakeNPIFrameHeader(UART_S, nRetIdx); 

	gInst.m_tTxRx[UART_S].m_cTxBuf[nRetIdx++] = CmicM_CalculateFCS(&gInst.m_tTxRx[UART_S].m_cTxBuf[1], nRetIdx-1);
	gInst.m_tTxRx[UART_S].m_nTxLen = nRetIdx;
	UART_SendData(UART_S, gInst.m_tTxRx[UART_S].m_cTxBuf, nRetIdx);

	//PRIMARY -----------------------------------------------------------------------------
	nRetIdx=0;
	MemSet(&gInst.m_tTxRx[UART_P].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cTxBuf));
	gInst.m_tTxRx[UART_P].m_nTxLen = 0;

	//1. StackWrite BQ_DIAG_ADC_CTRL3_OFFSET
	MemSet(buf, 0, sizeof(buf));
	buf[0] = BQ_DIAG_ADC_CTRL3_DIAG_DIG_EN_MSK; 	
	nRetIdx = CmicM_MakeBQFrame(UART_P, BROADCAST_WRITE, 
								BQ_DIAG_ADC_CTRL3_OFFSET, buf, NPI_HEADER_SIZE );	

	//2. StackRead BQ_CUST_CRC_RSLT_HI_OFFSET
	MemSet(buf, 0 , sizeof(buf));
	buf[0] = READ_1BYTE; //2byte read.
	nRetIdx = CmicM_MakeBQFrame(UART_P, (BROADCAST_READ), 
							BQ_CUST_CRC_RSLT_HI_OFFSET, buf, nRetIdx );						  

	// BQ FrameData ReqFrame에 저장
	MemSet(&gInst.m_ReqFrame[UART_P], 0, sizeof(gInst.m_ReqFrame[UART_P]));
	gInst.m_ReqFrame[UART_P].m_uCmd = eCMD_WD_BQ_RAWFRAME;
	gInst.m_ReqFrame[UART_P].m_nBQ_REG_ADDR = BQ_CUST_CRC_RSLT_HI_OFFSET;
	gInst.m_ReqFrame[UART_P].m_ReqDataLen = READ_1BYTE; //2byte read.
	//////////////////////////////////

	CmicM_MakeNPIFrameHeader(UART_P, nRetIdx); 

	gInst.m_tTxRx[UART_P].m_cTxBuf[nRetIdx++] = CmicM_CalculateFCS(&gInst.m_tTxRx[UART_P].m_cTxBuf[1], nRetIdx-1);
	gInst.m_tTxRx[UART_P].m_nTxLen = nRetIdx;
	UART_SendData(UART_P, gInst.m_tTxRx[UART_P].m_cTxBuf, nRetIdx);

	gInst.m_tSt.m_BootSub.m_eBQRegister = eBQREG_ST4_RES;
}

/********************************************************************************
FUNCTION :  CmicM_BQRegister_ST4_RES()
STATUS :   BOOTSUB_BQREGISTER_E =  eBQREG_ST4_RES
DESCRIPTION : 
**********************************************************************************/
static void	CmicM_BQRegister_ST4_RES(void)
{
	static  eRECV_Status	rStat = NO_RECV;
	
	//Secondary Check------------------------------------------------------------------------------------
	if (FALSE == (rStat & S_RECV))
	{
		if ( UART_Polling_Cb(UART_S))
		{
			MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
			UART_ReceiveData(UART_S, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);

			if (CmicM_VerifyRx_BQData(UART_S))
			{			
				rStat |= S_RECV;
			}
		}
	}

	//Primary Check--------------------------------------------------------------------------------------
	if (FALSE == (rStat & P_RECV))
	{
		if ( UART_Polling_Cb(UART_P))
		{
			MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
			UART_ReceiveData(UART_P, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);

			if (CmicM_VerifyRx_BQData(UART_P))
			{
				rStat |= P_RECV;			
			}
		}
	}

	if (rStat == ALL_RECV )
	{
		rStat = NO_RECV;		
		gInst.m_tSt.m_BootSub.m_eBQRegister = eBQREG_ST5_REQ;	
	}		
}

/********************************************************************************
FUNCTION :  CmicM_BQRegister_ST5_REQ()
STATUS :   BOOTSUB_BQREGISTER_E =  eBQREG_ST5_REQ
DESCRIPTION : BQ_CUST_CRC_HI_OFFSET/BQ_FAULT_RST2_OFFSET
**********************************************************************************/
static void	CmicM_BQRegister_ST5_REQ(void)
{
	uint16_t nIdx = NPI_HEADER_SIZE;
	uint16_t  nRetIdx = 0;
	uint8_t   buf[10];

	//SECONDARY ----------------------------------------------------------------------------------------	
	MemSet(&gInst.m_tTxRx[UART_S].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cTxBuf));
	gInst.m_tTxRx[UART_S].m_nTxLen = 0;

	// SingleWrite BQ_CUST_CRC_HI_OFFSET --> Broadcast로 변경
	MemSet(buf, 0, sizeof(buf));
	MemCpy(buf, &gInst.m_uSiglewriteRes[UART_S][0], WRITE_2BYTE); //3bytes

	nRetIdx = CmicM_MakeBQFrame(UART_S, BROADCAST_WRITE| WRITE_1BYTE, 
								BQ_CUST_CRC_HI_OFFSET, buf, nIdx);
	nIdx = nRetIdx;							
		
	// StackWrite BQ_FAULT_RST2_RST_OTP_CRC_MSK | BQ_FAULT_RST2_RST_OTP_DATA_MSK
	MemSet(buf, 0, sizeof(buf));
	buf[0] = BQ_FAULT_RST2_RST_OTP_CRC_MSK | BQ_FAULT_RST2_RST_OTP_DATA_MSK; 
	
	nRetIdx = CmicM_MakeBQFrame(UART_S, BROADCAST_WRITE, 
								BQ_FAULT_RST2_OFFSET, buf, nIdx );								

	CmicM_MakeNPIFrameHeader(UART_S, nRetIdx); 

	gInst.m_tTxRx[UART_S].m_cTxBuf[nRetIdx++] = CmicM_CalculateFCS(&gInst.m_tTxRx[UART_S].m_cTxBuf[1], nRetIdx-1);
	gInst.m_tTxRx[UART_S].m_nTxLen = nRetIdx;
	UART_SendData(UART_S, gInst.m_tTxRx[UART_S].m_cTxBuf, nRetIdx);
	
	//PRIMARY-----------------------------------------------------------------------------------------	
	nIdx = NPI_HEADER_SIZE;
	nRetIdx = 0;

	MemSet(&gInst.m_tTxRx[UART_P].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cTxBuf));
	gInst.m_tTxRx[UART_P].m_nTxLen = 0;

	// SingleWrite BQ_CUST_CRC_HI_OFFSET --> Broadcast로 변경
	MemSet(buf, 0, sizeof(buf));
	MemCpy(buf, &gInst.m_uSiglewriteRes[UART_P][0], WRITE_2BYTE); //3bytes

	nRetIdx = CmicM_MakeBQFrame(UART_P, BROADCAST_WRITE| WRITE_1BYTE, 
								BQ_CUST_CRC_HI_OFFSET, buf, nIdx);
	nIdx = nRetIdx;							
		
	// StackWrite BQ_FAULT_RST2_RST_OTP_CRC_MSK | BQ_FAULT_RST2_RST_OTP_DATA_MSK
	MemSet(buf, 0, sizeof(buf));
	buf[0] = BQ_FAULT_RST2_RST_OTP_CRC_MSK | BQ_FAULT_RST2_RST_OTP_DATA_MSK; 
	
	nRetIdx = CmicM_MakeBQFrame(UART_P, BROADCAST_WRITE, 
								BQ_FAULT_RST2_OFFSET, buf, nIdx );								

	CmicM_MakeNPIFrameHeader(UART_P, nRetIdx); 

	gInst.m_tTxRx[UART_P].m_cTxBuf[nRetIdx++] = CmicM_CalculateFCS(&gInst.m_tTxRx[UART_P].m_cTxBuf[1], nRetIdx-1);
	gInst.m_tTxRx[UART_P].m_nTxLen = nRetIdx;
	UART_SendData(UART_P, gInst.m_tTxRx[UART_P].m_cTxBuf, nRetIdx);

	gInst.m_tSt.m_BootSub.m_eBQRegister = eBQREG_ST5_RES; //eBQREG_ST6_REQ;
	gInst.m_nDelay=0;
}

/********************************************************************************
FUNCTION :  CmicM_BQRegister_ST5_RES()
STATUS :   BOOTSUB_BQREGISTER_E =  eBQREG_ST5_RES
DESCRIPTION : 
**********************************************************************************/
static void	CmicM_BQRegister_ST5_RES(void)
{
	static  eRECV_Status	rStat = NO_RECV;

	//Secondary Check--------------------------------------------------------------------------------
	if (FALSE == (rStat & S_RECV))
	{
		if (UART_Polling_Cb(UART_S))
		{
			MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
			UART_ReceiveData(UART_S, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);

			switch(gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_IDX_CMDID])
			{				
				case eCMD_CB_TXCNF :
					if ((gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_MAC_STATUS_IDX] == 0x00))
					{ 
						rStat |= S_RECV;
					}
				break;				
				default :		break;
			}
		}
	} 
	
	//Primary Check--------------------------------------------------------------------------------
	if (FALSE == (rStat & P_RECV))
	{
		if (UART_Polling_Cb(UART_P))
		{
			MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
			UART_ReceiveData(UART_P, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);

			switch(gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_IDX_CMDID])
			{				
				case eCMD_CB_TXCNF :				
					if (gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_MAC_STATUS_IDX] == 0x00)			
					{
						rStat |= P_RECV;
					}
				break;				
				default :		break;
			}
		}
	}
	
	if (rStat == ALL_RECV )
	{
		rStat = NO_RECV;		
		gInst.m_tSt.m_BootSub.m_eBQRegister = eBQREG_ST6_REQ;
		//gInst.m_tSt.m_eBal = eBAL_IDLE;	
	}
}

/********************************************************************************
FUNCTION :  CmicM_BQRegister_ST6_REQ()
STATUS :   BOOTSUB_BQREGISTER_E =  eBQREG_ST6_REQ
DESCRIPTION : 100ms Delay -> BQ_DIAG_STAT1_OFFSET Read
**********************************************************************************/
static void	CmicM_BQRegister_ST6_REQ(void) 
{
	uint16  nRetIdx=0;
	uint8_t   buf[5];
	
	// 100ms Delay
	if (++gInst.m_nDelay > 100) 
	{
		//SECONDARY --------------------------------------------------------------------------	
		// Tx Buffer Clear.
		MemSet(&gInst.m_tTxRx[UART_S].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cTxBuf));
		gInst.m_tTxRx[UART_S].m_nTxLen = 0;

		// StackRead BQ_DIAG_STAT1_OFFSET
		MemSet(buf, 0 , sizeof(buf));
		buf[0] = READ_4BYTE; //5byte read.

		nRetIdx = CmicM_MakeBQFrame(UART_S, (BROADCAST_READ), 
								BQ_DIAG_STAT1_OFFSET, buf, NPI_HEADER_SIZE );	
				
		// BQ FrameData ReqFrame에 저장
		MemSet(&gInst.m_ReqFrame[UART_S], 0, sizeof(gInst.m_ReqFrame[UART_S]));
		gInst.m_ReqFrame[UART_S].m_uCmd = eCMD_WD_BQ_RAWFRAME;
		gInst.m_ReqFrame[UART_S].m_nBQ_REG_ADDR = BQ_DIAG_STAT1_OFFSET;
		gInst.m_ReqFrame[UART_S].m_ReqDataLen = READ_4BYTE; //5byte read.
		//////////////////////////////////					  
			
		CmicM_MakeNPIFrameHeader(UART_S, nRetIdx); 

		gInst.m_tTxRx[UART_S].m_cTxBuf[nRetIdx++] = CmicM_CalculateFCS(&gInst.m_tTxRx[UART_S].m_cTxBuf[1], nRetIdx-1);
		gInst.m_tTxRx[UART_S].m_nTxLen = nRetIdx;
		UART_SendData(UART_S, gInst.m_tTxRx[UART_S].m_cTxBuf, nRetIdx);
		

		//PRIMARY ------------------------------------------------------------------------------	
	 	nRetIdx=0;
		// Tx Buffer Clear.
		MemSet(&gInst.m_tTxRx[UART_P].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cTxBuf));
		gInst.m_tTxRx[UART_P].m_nTxLen = 0;

		// StackRead BQ_DIAG_STAT1_OFFSET
		MemSet(buf, 0 , sizeof(buf));
		buf[0] = READ_4BYTE; //5byte read.

		nRetIdx = CmicM_MakeBQFrame(UART_P, (BROADCAST_READ), 
								BQ_DIAG_STAT1_OFFSET, buf, NPI_HEADER_SIZE );	
				
		// BQ FrameData ReqFrame에 저장
		MemSet(&gInst.m_ReqFrame[UART_P], 0, sizeof(gInst.m_ReqFrame[UART_P]));
		gInst.m_ReqFrame[UART_P].m_uCmd = eCMD_WD_BQ_RAWFRAME;
		gInst.m_ReqFrame[UART_P].m_nBQ_REG_ADDR = BQ_DIAG_STAT1_OFFSET;
		gInst.m_ReqFrame[UART_P].m_ReqDataLen = READ_4BYTE; //5byte read.
		//////////////////////////////////					  
			
		CmicM_MakeNPIFrameHeader(UART_P, nRetIdx); 

		gInst.m_tTxRx[UART_P].m_cTxBuf[nRetIdx++] = CmicM_CalculateFCS(&gInst.m_tTxRx[UART_P].m_cTxBuf[1], nRetIdx-1);
		gInst.m_tTxRx[UART_P].m_nTxLen = nRetIdx;
		UART_SendData(UART_P, gInst.m_tTxRx[UART_P].m_cTxBuf, nRetIdx);
		
		gInst.m_nDelay=0;

	#ifndef SEQUENCE_READ
		//Polling Request
		CmicM_SetPollStatus(ePOLL_Xms_REQUEST);	
		gInst.m_tSt.m_BootSub.m_eBQRegister = eBQREG_ST6_WAIT;
	#else
		gInst.m_tSt.m_BootSub.m_eBQRegister = eBQREG_ST6_RES;
	#endif
	}
} 

/*******************************************************************************
** Function Name    : CmicM_BQRegister_ST6_WAIT
** Description      : UART 로부터의 전송완료 대기
*******************************************************************************/
static void CmicM_BQRegister_ST6_WAIT(void)
{
	if(CmicM_GetPollStatus() == ePOLL_Xms_RECEIVE)
	{
		CmicM_SetPollStatus(ePOLL_Xms_IDLE);
		gInst.m_tSt.m_BootSub.m_eBQRegister++;
	}else{
		//wait..
		if (CmicM_GetPollStatus() == ePOLL_Xms_RUNNING){
			gInst.m_nWaitCnt++;
		}
	}	
}

/********************************************************************************
FUNCTION :  CmicM_BQRegister_ST6_RES()
STATUS :   BOOTSUB_BQREGISTER_E =  eBQREG_ST6_RES
DESCRIPTION : BQ_DIAG_STAT1_OFFSET Read (DRDY_DIG/DRDY_VCELLADC/DRDY_GPADC)
**********************************************************************************/
static void	CmicM_BQRegister_ST6_RES(void)
{
#ifndef SEQUENCE_READ

	uint8  idxP, idxS;
	CmicM_GetIndex(&idxP, &idxS);

	for (uint8 i=0; i < idxS ; i++ )
	{
	 	MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
	 	UART_SEQ_ReceiveData(UART_S, i, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);
		
	 	CmicM_VerifyRx_BQData(UART_S);	 		
	}
	 
	for (uint8 i=0; i < idxP ; i++ )
	{
		MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
		UART_SEQ_ReceiveData(UART_P, i, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);
		
		CmicM_VerifyRx_BQData(UART_P);			
	}
	gInst.m_tSt.m_BootSub.m_eBQRegister = eBQREG_ST7_REQ;	

#else
	static  eRECV_Status	rStat = NO_RECV;
	
	//Secondary Check-------------------------------------------------------------------------
	if (FALSE == (rStat & S_RECV))
	{
		if (UART_Polling_Cb(UART_S))
		{
			MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
			UART_ReceiveData(UART_S, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);

			if (CmicM_VerifyRx_BQData(UART_S)){				
					rStat |= S_RECV;				
			}
		}
	}

	//Primary Check-------------------------------------------------------------------------------
	if (FALSE == (rStat & P_RECV))
	{
		if (UART_Polling_Cb(UART_P))
		{
			MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
			UART_ReceiveData(UART_P, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);

			if (CmicM_VerifyRx_BQData(UART_P)){				
					rStat |= P_RECV;			
			}
		}
	}

	if (rStat == ALL_RECV )
	{
		rStat = NO_RECV;
		gInst.m_tSt.m_BootSub.m_eBQRegister = eBQREG_ST7_REQ;	
	}
#endif
}  


/********************************************************************************
FUNCTION :  CmicM_BQRegister_ST7_REQ()
STATUS :   BOOTSUB_BQREGISTER_E =  eBQREG_ST7_REQ
DESCRIPTION : VCELL Read
**********************************************************************************/
static void	CmicM_BQRegister_ST7_REQ(void)  
{
	uint16  nRetIdx=0;
	uint8_t   buf[5];

	//SECONDARY ---------------------------------------------------------------------------------------------	
	// Tx Buffer Clear.
	MemSet(&gInst.m_tTxRx[UART_S].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cTxBuf));
	gInst.m_tTxRx[UART_S].m_nTxLen = 0;

	//StackRead BQ_VCELLx_HI_OFFSET
	MemSet(buf, 0 , sizeof(buf));
	buf[0] = (uint8)(NUM_CELLS_ACTIVE * 2 - 1); 

	nRetIdx = CmicM_MakeBQFrame(UART_S, (BROADCAST_READ), 
							BQ_VCELL_START_ADDR, buf, NPI_HEADER_SIZE );						  

	// BQ FrameData ReqFrame에 저장
	MemSet(&gInst.m_ReqFrame[UART_S], 0, sizeof(gInst.m_ReqFrame[UART_S]));
	gInst.m_ReqFrame[UART_S].m_uCmd = eCMD_WD_BQ_RAWFRAME;
	gInst.m_ReqFrame[UART_S].m_nBQ_REG_ADDR = BQ_VCELL_START_ADDR;
	gInst.m_ReqFrame[UART_S].m_ReqDataLen = (NUM_CELLS_ACTIVE * 2 - 1); 
	//////////////////////////////////	

	CmicM_MakeNPIFrameHeader(UART_S, nRetIdx); 

	gInst.m_tTxRx[UART_S].m_cTxBuf[nRetIdx++] = CmicM_CalculateFCS(&gInst.m_tTxRx[UART_S].m_cTxBuf[1], nRetIdx-1);
	gInst.m_tTxRx[UART_S].m_nTxLen = nRetIdx;
	UART_SendData(UART_S, gInst.m_tTxRx[UART_S].m_cTxBuf, nRetIdx);
	

	//PRIMARY ----------------------------------------------------------------------------------------------	
	nRetIdx=0;
	// Tx Buffer Clear.
	MemSet(&gInst.m_tTxRx[UART_P].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cTxBuf));
	gInst.m_tTxRx[UART_P].m_nTxLen = 0;

	//StackRead BQ_VCELLx_HI_OFFSET
	MemSet(buf, 0 , sizeof(buf));
	buf[0] = (uint8)(NUM_CELLS_ACTIVE * 2 - 1); 

	nRetIdx = CmicM_MakeBQFrame(UART_P, (BROADCAST_READ), 
							BQ_VCELL_START_ADDR, buf, NPI_HEADER_SIZE );						  

	// BQ FrameData ReqFrame에 저장
	MemSet(&gInst.m_ReqFrame[UART_P], 0, sizeof(gInst.m_ReqFrame[UART_P]));
	gInst.m_ReqFrame[UART_P].m_uCmd = eCMD_WD_BQ_RAWFRAME;
	gInst.m_ReqFrame[UART_P].m_nBQ_REG_ADDR = BQ_VCELL_START_ADDR;
	gInst.m_ReqFrame[UART_P].m_ReqDataLen = (NUM_CELLS_ACTIVE * 2 - 1); 
	//////////////////////////////////	

	CmicM_MakeNPIFrameHeader(UART_P, nRetIdx); 

	gInst.m_tTxRx[UART_P].m_cTxBuf[nRetIdx++] = CmicM_CalculateFCS(&gInst.m_tTxRx[UART_P].m_cTxBuf[1], nRetIdx-1);
	gInst.m_tTxRx[UART_P].m_nTxLen = nRetIdx;
	UART_SendData(UART_P, gInst.m_tTxRx[UART_P].m_cTxBuf, nRetIdx);

#ifndef SEQUENCE_READ	
	//Polling Request
	CmicM_SetPollStatus(ePOLL_Xms_REQUEST);
	gInst.m_tSt.m_BootSub.m_eBQRegister = eBQREG_ST7_WAIT;
#else
	gInst.m_tSt.m_BootSub.m_eBQRegister = eBQREG_ST7_RES;
#endif
}


/*******************************************************************************
** Function Name    : CmicM_BQRegister_ST7_WAIT
** Description      : UART 로부터의 전송완료 대기
*******************************************************************************/
static void CmicM_BQRegister_ST7_WAIT(void)
{
	if(CmicM_GetPollStatus() == ePOLL_Xms_RECEIVE)
	{
		CmicM_SetPollStatus(ePOLL_Xms_IDLE);
		gInst.m_tSt.m_BootSub.m_eBQRegister++;
	}else{
		//wait..
		if (CmicM_GetPollStatus() == ePOLL_Xms_RUNNING){
			gInst.m_nWaitCnt++;
		}
	}	
}


/********************************************************************************
FUNCTION :  CmicM_BQRegister_ST7_RES()
STATUS :   BOOTSUB_BQREGISTER_E =  eBQREG_ST7_RES
DESCRIPTION : VCELL READ
**********************************************************************************/
static void	CmicM_BQRegister_ST7_RES(void)
{
#ifndef SEQUENCE_READ

	uint8  idxP, idxS;
	CmicM_GetIndex(&idxP, &idxS);

	for (uint8 i=0; i < idxS ; i++ )
	{
	 	MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
	 	UART_SEQ_ReceiveData(UART_S, i, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);
		
	 	if (CmicM_VerifyRx_BQData(UART_S)){
	 		CmicM_PriorVcell_Init(UART_S);
	 	}
	}
	 
	for (uint8 i=0; i < idxP ; i++ )
	{
		MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
		UART_SEQ_ReceiveData(UART_P, i, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);
		
		if (CmicM_VerifyRx_BQData(UART_P)){
			CmicM_PriorVcell_Init(UART_P);
		}
	}

	gInst.m_tSt.m_BootSub.m_eBQRegister = eBQREG_ST0_IDLE;
	gInst.m_tSt.m_eMain = 	eMAIN_RUN; 	
	gInst.m_tSt.m_ePriorMain = 	eMAIN_RUN; 	
	// CmicM_debug_BootTimeLog(Interval, LogEnd__, 205, BQConfigInit_StackRead_BQ_VCELL18_HI_________);
	CmicM_debug_BootTimeLog(Overall_, LogEnd__, 999, Total_boot_time______________________________);

	//100ms Timer Start ..
	Tmr_Start(TimerCallback_Proc);
#else
	static  eRECV_Status	rStat = NO_RECV;

	//Secondary Check------------------------------------------------------------------------------------
	if (FALSE == (rStat & S_RECV))
	{
		if( UART_Polling_Cb(UART_S))
		{
			MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
			UART_ReceiveData(UART_S, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);

			if (CmicM_VerifyRx_BQData(UART_S)){		
				rStat |= S_RECV;	
				CmicM_PriorVcell_Init(UART_S);			
			}
		}
	}

	//Primary Check-------------------------------------------------------------------------------------
	if (FALSE == (rStat & P_RECV))
	{
		if( UART_Polling_Cb(UART_P))
		{
			MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
			UART_ReceiveData(UART_P, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);

			if (CmicM_VerifyRx_BQData(UART_P)){				
				rStat |= P_RECV;
				CmicM_PriorVcell_Init(UART_P);					
			}
		}
	}	

	if (rStat == ALL_RECV )
	{
		rStat = NO_RECV;
		gInst.m_tSt.m_BootSub.m_eBQRegister = eBQREG_ST0_IDLE;
		gInst.m_tSt.m_eMain = 	eMAIN_RUN; 	
		gInst.m_tSt.m_ePriorMain = 	eMAIN_RUN; 	
		// CmicM_debug_BootTimeLog(Interval, LogEnd__, 205, BQConfigInit_StackRead_BQ_VCELL18_HI_________);
		CmicM_debug_BootTimeLog(Overall_, LogEnd__, 999, Total_boot_time______________________________);
	
		//100ms Timer Start ..
		Tmr_Start(TimerCallback_Proc);
	}
#endif
}


static void	CmicM_SetCell_ST0_IDLE(void)
{
	gInst.m_tSt.m_RunSub.m_eSetCellMonitor++;
}


static void	CmicM_SetCell_ST1_REQ(void)
{
	uint16_t nRetIdx=0;
	uint8_t  buf[NUM_CELLS_ACTIVE];
	uint8_t  uCbCtrl[NUM_CELLS_ACTIVE];

	MemSet(uCbCtrl, 0, NUM_CELLS_ACTIVE);

	//SECONDARY -----------------------------------
	// Tx Buffer Clear.
	MemSet(&gInst.m_tTxRx[UART_S].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cTxBuf));
	gInst.m_tTxRx[UART_S].m_nTxLen = 0;

	// Stackwrite BQ_CB_CELL_CTRL9_16_OFFSET
	MemSet(buf, 0 , sizeof(buf));
	MemCpy(buf, &uCbCtrl[0], NUM_CELLS_ACTIVE);

	nRetIdx = CmicM_MakeBQFrame(UART_S, (BROADCAST_WRITE | (NUM_CELLS_ACTIVE-1)), //(NUM_CELLS_ACTIVE-1)), 
								BQ_CB_CELL_CTRL9_16_OFFSET, buf, NPI_HEADER_SIZE );
	
	CmicM_MakeNPIFrameHeader(UART_S, nRetIdx); 

	gInst.m_tTxRx[UART_S].m_cTxBuf[nRetIdx++] = CmicM_CalculateFCS(&gInst.m_tTxRx[UART_S].m_cTxBuf[1], nRetIdx-1);
	gInst.m_tTxRx[UART_S].m_nTxLen = nRetIdx;
	UART_SendData(UART_S, gInst.m_tTxRx[UART_S].m_cTxBuf, nRetIdx);

#ifdef SURESOFT
	nRetIdx = CmicM_MakeBQFrame(UART_S, (BROADCAST_WRITE | (NUM_CELLS_ACTIVE/2-1)), //(NUM_CELLS_ACTIVE-1)), 
								BQ_CB_CELL_CTRL1_8_OFFSET, buf, nRetIdx );		
	CmicM_MakeNPIFrameHeader(UART_S, nRetIdx); 

	gInst.m_tTxRx[UART_S].m_cTxBuf[nRetIdx++] = CmicM_CalculateFCS(&gInst.m_tTxRx[UART_S].m_cTxBuf[1], nRetIdx-1);
	gInst.m_tTxRx[UART_S].m_nTxLen = nRetIdx;
	UART_SendData(UART_S, gInst.m_tTxRx[UART_S].m_cTxBuf, nRetIdx);
#endif  

	//PRIMARY -----------------------------------
	nRetIdx = 0;
	// Tx Buffer Clear.
	MemSet(&gInst.m_tTxRx[UART_P].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cTxBuf));
	gInst.m_tTxRx[UART_P].m_nTxLen = 0;

	// Staclwrite BQ_CB_CELL_CTRL9_16_OFFSET
	MemSet(buf, 0 , sizeof(buf));
	MemCpy(buf, &uCbCtrl[0], NUM_CELLS_ACTIVE);

	nRetIdx = CmicM_MakeBQFrame(UART_P, (BROADCAST_WRITE | (NUM_CELLS_ACTIVE-1)), //(NUM_CELLS_ACTIVE-1)), 
								BQ_CB_CELL_CTRL9_16_OFFSET, buf, NPI_HEADER_SIZE );
	
	CmicM_MakeNPIFrameHeader(UART_P, nRetIdx); 

	gInst.m_tTxRx[UART_P].m_cTxBuf[nRetIdx++] = CmicM_CalculateFCS(&gInst.m_tTxRx[UART_P].m_cTxBuf[1], nRetIdx-1);
	gInst.m_tTxRx[UART_P].m_nTxLen = nRetIdx;
	UART_SendData(UART_P, gInst.m_tTxRx[UART_P].m_cTxBuf, nRetIdx);

#ifdef SURESOFT
	nRetIdx = CmicM_MakeBQFrame(UART_P, (BROADCAST_WRITE | (NUM_CELLS_ACTIVE/2-1)), //(NUM_CELLS_ACTIVE-1)), 
								BQ_CB_CELL_CTRL1_8_OFFSET, buf, nRetIdx );		
	CmicM_MakeNPIFrameHeader(UART_P, nRetIdx); 

	gInst.m_tTxRx[UART_P].m_cTxBuf[nRetIdx++] = CmicM_CalculateFCS(&gInst.m_tTxRx[UART_P].m_cTxBuf[1], nRetIdx-1);
	gInst.m_tTxRx[UART_P].m_nTxLen = nRetIdx;
	UART_SendData(UART_P, gInst.m_tTxRx[UART_P].m_cTxBuf, nRetIdx);
#endif 


	gInst.m_tSt.m_RunSub.m_eSetCellMonitor++;
}


static void	CmicM_SetCell_ST1_RES(void)
{
	static  eRECV_Status	rStat = NO_RECV;

	//Primary Check--------------------------------------------------------------------------
	if (FALSE == (rStat & P_RECV))
	{
		if (UART_Polling_Cb(UART_P))
		{
			MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
			UART_ReceiveData(UART_P, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);

			switch(gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_IDX_CMDID])
			{				
				case eCMD_CB_TXCNF :				
					if (gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_MAC_STATUS_IDX] == 0x00)			
					{
						rStat |= P_RECV;
					}
				break;
				
				default :		break;
			}
		}
	}
	//Secondary Check------------------------------------------------------------------------
	if (FALSE == (rStat & S_RECV))
	{
		if (UART_Polling_Cb(UART_S))
		{
			MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
			UART_ReceiveData(UART_S, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);

			switch(gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_IDX_CMDID])
			{				
				case eCMD_CB_TXCNF :					
					if ((gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_MAC_STATUS_IDX] == 0x00))
					{ 
						rStat |= S_RECV;
					}
				break;
				
				default :		break;
			}
		}
	}

	if (rStat == ALL_RECV )
	{
		rStat = NO_RECV;
		gInst.m_tSt.m_RunSub.m_eSetCellMonitor++;
	}
}

static void	CmicM_SetCell_ST2_REQ(void)
{
	uint16_t nRetIdx=0;
	uint8_t  buf[NUM_CELLS_ACTIVE];
	uint8_t  uCbCtrl[NUM_CELLS_ACTIVE];


	MemSet(uCbCtrl, 0, NUM_CELLS_ACTIVE);

	//SECONDARY --------------------------------------------------------------------------
	// Tx Buffer Clear.
	MemSet(&gInst.m_tTxRx[UART_S].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cTxBuf));
	gInst.m_tTxRx[UART_S].m_nTxLen = 0;

	// StackWrite BQ_BAL_CTRL2_OFFSET
	MemSet(buf, 0 , sizeof(buf));
	buf[0] = BQ_BAL_CTRL2_BAL_GO_MSK;

	nRetIdx = CmicM_MakeBQFrame(UART_S, (BROADCAST_WRITE), //(NUM_CELLS_ACTIVE-1)), 
								BQ_BAL_CTRL2_OFFSET, buf, NPI_HEADER_SIZE );
		
	CmicM_MakeNPIFrameHeader(UART_S, nRetIdx); 

	gInst.m_tTxRx[UART_S].m_cTxBuf[nRetIdx++] = CmicM_CalculateFCS(&gInst.m_tTxRx[UART_S].m_cTxBuf[1], nRetIdx-1);
	gInst.m_tTxRx[UART_S].m_nTxLen = nRetIdx;
	UART_SendData(UART_S, gInst.m_tTxRx[UART_S].m_cTxBuf, nRetIdx);
	

	//PRIMARY -------------------------------------------------------------------------
	nRetIdx = 0;
	// Tx Buffer Clear.
	MemSet(&gInst.m_tTxRx[UART_P].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cTxBuf));
	gInst.m_tTxRx[UART_P].m_nTxLen = 0;

	// StackWrite BQ_BAL_CTRL2_OFFSET
	MemSet(buf, 0 , sizeof(buf));
	buf[0] = BQ_BAL_CTRL2_BAL_GO_MSK;

	nRetIdx = CmicM_MakeBQFrame(UART_P, (BROADCAST_WRITE), //(NUM_CELLS_ACTIVE-1)), 
								BQ_BAL_CTRL2_OFFSET, buf, NPI_HEADER_SIZE );
		
	CmicM_MakeNPIFrameHeader(UART_P, nRetIdx); 

	gInst.m_tTxRx[UART_P].m_cTxBuf[nRetIdx++] = CmicM_CalculateFCS(&gInst.m_tTxRx[UART_P].m_cTxBuf[1], nRetIdx-1);
	gInst.m_tTxRx[UART_P].m_nTxLen = nRetIdx;
	UART_SendData(UART_P, gInst.m_tTxRx[UART_P].m_cTxBuf, nRetIdx);

	gInst.m_tSt.m_RunSub.m_eSetCellMonitor++;
}


static void	CmicM_SetCell_ST2_RES(void)
{
	static  eRECV_Status	rStat = NO_RECV;

	//Primary Check------------------------------------------------------------------------
	if (FALSE == (rStat & P_RECV))
	{
		if (UART_Polling_Cb(UART_P))
		{
			MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
			UART_ReceiveData(UART_P, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);

			switch(gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_IDX_CMDID])
			{				
				case eCMD_CB_TXCNF :				
					if (gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_MAC_STATUS_IDX] == 0x00)//&&					
					{
						rStat |= P_RECV;
					}
				break;				
				default :			break;
			}
		}
	}

	//Secondary Check------------------------------------------------------------------------
	if (FALSE == (rStat & S_RECV))
	{
		if (UART_Polling_Cb(UART_S))
		{
			MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
			UART_ReceiveData(UART_S, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);

			switch(gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_IDX_CMDID])
			{				
				case eCMD_CB_TXCNF :					
					if ((gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_MAC_STATUS_IDX] == 0x00))//&&
					{ 
						rStat |= S_RECV;
					}
				break;				
				default :			break;
			}
		}
	}

	if (rStat == ALL_RECV )
	{
		rStat = NO_RECV;
		gInst.m_tSt.m_eRun = eRUN_IDLE;	
		gInst.m_tSt.m_RunSub.m_eSetCellMonitor = eSETCELL_ST0_IDLE;	
	}
}

/*******************************************************************************
** Function Name    : CmicM_Cell_STX_
** Description      : 
*******************************************************************************/
static void	CmicM_Cell_ST0_IDLE(void)
{
	gInst.m_tSt.m_RunSub.m_eCellMonitor++;
}

/*******************************************************************************
** Function Name    : CmicM_Cell_ST1_REQ
** Description      : VCELL / GPIO StackRead Transfer
*******************************************************************************/
static void	CmicM_Cell_ST1_REQ(void)
{
	// Cell Read Operation
	uint16  nRetIdx=0;
	uint8_t   buf[5];

	//SECONDARY ----------------------------------------------------------------------------
	// Tx Buffer Clear.
	MemSet(&gInst.m_tTxRx[UART_S].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cTxBuf));
	gInst.m_tTxRx[UART_S].m_nTxLen = 0;

	// StackRead VCELL
	MemSet(buf, 0 , sizeof(buf));
	buf[0] = (uint8)(NUM_CELLS_ACTIVE * 2 - 1); //0x23(36-1) byte read.

	nRetIdx = CmicM_MakeBQFrame(UART_S, (BROADCAST_READ), 
							BQ_VCELL_START_ADDR, buf, NPI_HEADER_SIZE );	

	// StackRead BQ_GPIO1_HI_OFFSET
	MemSet(buf, 0 , sizeof(buf));
	buf[0] = (uint8)(NUM_GPIOS_ACTIVE * READ_2BYTE - 1); //0x15(22-1) byte read.

	nRetIdx = CmicM_MakeBQFrame(UART_S, (BROADCAST_READ), 
							BQ_GPIO_START_ADDR, buf, nRetIdx );	
		
	//BQ FrameData ReqFrame에 저장
	MemSet(&gInst.m_ReqFrame[UART_S], 0, sizeof(gInst.m_ReqFrame[UART_S]));
	gInst.m_ReqFrame[UART_S].m_uCmd = eCMD_WD_BQ_RAWFRAME;
	gInst.m_ReqFrame[UART_S].m_nBQ_REG_ADDR = BQ_VCELL_START_ADDR; //BQ_VCELL18_HI_OFFSET;
	gInst.m_ReqFrame[UART_S].m_ReqDataLen = (NUM_CELLS_ACTIVE * 2 - 1)+(NUM_GPIOS_ACTIVE * READ_2BYTE - 1); //READ_35BYTE; //36byte read.
	//////////////////////////////////				  
		
	CmicM_MakeNPIFrameHeader(UART_S, nRetIdx); 

	gInst.m_tTxRx[UART_S].m_cTxBuf[nRetIdx++] = CmicM_CalculateFCS(&gInst.m_tTxRx[UART_S].m_cTxBuf[1], nRetIdx-1);
	gInst.m_tTxRx[UART_S].m_nTxLen = nRetIdx;
	UART_SendData(UART_S, gInst.m_tTxRx[UART_S].m_cTxBuf, nRetIdx);
	

	//PRIMARY ------------------------------------------------------------------------------
	nRetIdx=0;
	// Tx Buffer Clear.
	MemSet(&gInst.m_tTxRx[UART_P].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cTxBuf));
	gInst.m_tTxRx[UART_P].m_nTxLen = 0;

	MemSet(buf, 0 , sizeof(buf));
	buf[0] = (uint8)(NUM_CELLS_ACTIVE * 2 - 1); //0x23(36-1) byte read.

	nRetIdx = CmicM_MakeBQFrame(UART_P, (BROADCAST_READ), 							
							BQ_VCELL_START_ADDR, buf, NPI_HEADER_SIZE );	

	// StackRead BQ_GPIO1_HI_OFFSET
	MemSet(buf, 0 , sizeof(buf));
	buf[0] = (uint8)(NUM_GPIOS_ACTIVE * READ_2BYTE - 1); //0x15(22-1) byte read.

	nRetIdx = CmicM_MakeBQFrame(UART_P, (BROADCAST_READ), 
							BQ_GPIO_START_ADDR, buf, nRetIdx );	

	//BQ FrameData ReqFrame에 저장
	MemSet(&gInst.m_ReqFrame[UART_P], 0, sizeof(gInst.m_ReqFrame[UART_P]));
	gInst.m_ReqFrame[UART_P].m_uCmd = eCMD_WD_BQ_RAWFRAME;
	gInst.m_ReqFrame[UART_P].m_nBQ_REG_ADDR = BQ_VCELL_START_ADDR; //BQ_VCELL18_HI_OFFSET;
	gInst.m_ReqFrame[UART_P].m_ReqDataLen = (NUM_CELLS_ACTIVE * 2 - 1)+(NUM_GPIOS_ACTIVE * READ_2BYTE - 1);
	//////////////////////////////////				  
		
	CmicM_MakeNPIFrameHeader(UART_P, nRetIdx); 

	gInst.m_tTxRx[UART_P].m_cTxBuf[nRetIdx++] = CmicM_CalculateFCS(&gInst.m_tTxRx[UART_P].m_cTxBuf[1], nRetIdx-1);
	gInst.m_tTxRx[UART_P].m_nTxLen = nRetIdx;
	UART_SendData(UART_P, gInst.m_tTxRx[UART_P].m_cTxBuf, nRetIdx);

	gInst.m_uNumOfCmd = 2;
	gInst.m_tSt.m_RunSub.m_eCellMonitor++;

	//Polling Request
	CmicM_SetPollStatus(ePOLL_Xms_REQUEST);

	gInst.m_tInfo.m_cellTime.m_timeP0 = GetTick_1ms();
	gInst.m_tInfo.m_10msTime.m_timeP0 = GetTick_1ms();
}

/*******************************************************************************
** Function Name    : CmicM_Cell_ST1_WAIT
** Description      : UART 로부터의 전송완료 대기
*******************************************************************************/
static void CmicM_Cell_ST1_WAIT(void)
{
	if(CmicM_GetPollStatus() == ePOLL_Xms_RECEIVE)
	{
		CmicM_SetPollStatus(ePOLL_Xms_IDLE);
		gInst.m_tSt.m_RunSub.m_eCellMonitor++;
	}else{
		//wait..
		if (CmicM_GetPollStatus() == ePOLL_Xms_RUNNING){
			gInst.m_nWaitCnt++;
		}
	}	
}

/*******************************************************************************
** Function Name    : CmicM_Cell_ST1_RES
** Description      : 수신데이터의 일괄 처리.(VCELL/GPIO Data Verify, Decode)
*******************************************************************************/
static void	CmicM_Cell_ST1_RES(void)
{
	uint8  idxP, idxS;

	CmicM_GetIndex(&idxP, &idxS);

	for (uint8 i=0; i < idxS ; i++ )
	{
	 	MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
	 	UART_SEQ_ReceiveData(UART_S, i, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);
		
	 	if (CmicM_RUN_VerifyRx_BQData(UART_S)){
	 		CmicM_VcellOWDDecode(UART_S);
	 	}
	}
	 
	for (uint8 i=0; i < idxP ; i++ )
	{
		MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
		UART_SEQ_ReceiveData(UART_P, i, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);
		
		if (CmicM_RUN_VerifyRx_BQData(UART_P)){
			CmicM_VcellOWDDecode(UART_P);
		}
	}

	gInst.m_tInfo.m_cellTime.m_timeP1 = GetTick_1ms();
	gInst.m_tInfo.m_cellTime.m_duration = (float)((gInst.m_tInfo.m_cellTime.m_timeP1 - gInst.m_tInfo.m_cellTime.m_timeP0)/1000.0f);

	gInst.m_tSt.m_eRun = eRUN_IDLE;
	gInst.m_tSt.m_RunSub.m_eCellMonitor = eCELL_ST0_IDLE;

}


static void	CmicM_SetBal_ST0_IDLE(void)
{
	gInst.m_tSt.m_BalSub.m_eSetBalMonitor++;
}

/*******************************************************************************
** Function Name    : CmicM_SetBal_ST1_REQ
** Description      : CB_CELL16_CTRL Register(0x322)의 Timer Set
*******************************************************************************/
static void	CmicM_SetBal_ST1_REQ(void)
{
	gInst.m_uBalGo = FALSE;
	uint16_t nRetIdx=0;
	uint8_t  buf[NUM_CELLS_ACTIVE];
	uint8_t  uCbCtrl[NUM_CELLS_ACTIVE];

	MemSet(uCbCtrl, BQ_DIAG_CB_TIME_DEFAULT, NUM_CELLS_ACTIVE); // 0x01=30s 설정

	//SECONDARY ------------------------------------------------------------------------------------------
	// Tx Buffer Clear.
	MemSet(&gInst.m_tTxRx[UART_S].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cTxBuf));
	gInst.m_tTxRx[UART_S].m_nTxLen = 0;

	// Staclwrite BQ_CB_CELL_CTRL9_16_OFFSET
	MemSet(buf, 0 , sizeof(buf));
	MemCpy(buf, &uCbCtrl[0], NUM_CELLS_ACTIVE);

	nRetIdx = CmicM_MakeBQFrame(UART_S, (BROADCAST_WRITE | (NUM_CELLS_ACTIVE-1)), 
								BQ_CB_CELL_CTRL9_16_OFFSET, buf, NPI_HEADER_SIZE );

	CmicM_MakeNPIFrameHeader(UART_S, nRetIdx); 

	gInst.m_tTxRx[UART_S].m_cTxBuf[nRetIdx++] = CmicM_CalculateFCS(&gInst.m_tTxRx[UART_S].m_cTxBuf[1], nRetIdx-1);
	gInst.m_tTxRx[UART_S].m_nTxLen = nRetIdx;
	UART_SendData(UART_S, gInst.m_tTxRx[UART_S].m_cTxBuf, nRetIdx);

#ifdef SURESOFT
	nRetIdx = CmicM_MakeBQFrame(UART_S, (BROADCAST_WRITE | (NUM_CELLS_ACTIVE/2-1)), //(NUM_CELLS_ACTIVE-1)), 
								BQ_CB_CELL_CTRL1_8_OFFSET, buf, nRetIdx );		

	CmicM_MakeNPIFrameHeader(UART_S, nRetIdx); 

	gInst.m_tTxRx[UART_S].m_cTxBuf[nRetIdx++] = CmicM_CalculateFCS(&gInst.m_tTxRx[UART_S].m_cTxBuf[1], nRetIdx-1);
	gInst.m_tTxRx[UART_S].m_nTxLen = nRetIdx;
	UART_SendData(UART_S, gInst.m_tTxRx[UART_S].m_cTxBuf, nRetIdx);
#endif  
		
	//PRIMARY --------------------------------------------------------------------------------------------
	nRetIdx = 0;
	// Tx Buffer Clear.
	MemSet(&gInst.m_tTxRx[UART_P].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cTxBuf));
	gInst.m_tTxRx[UART_P].m_nTxLen = 0;

	// Staclwrite BQ_CB_CELL_CTRL9_16_OFFSET
	MemSet(buf, 0 , sizeof(buf));
	MemCpy(buf, &uCbCtrl[0], NUM_CELLS_ACTIVE);

	nRetIdx = CmicM_MakeBQFrame(UART_P, (BROADCAST_WRITE | (NUM_CELLS_ACTIVE-1)), 
								BQ_CB_CELL_CTRL9_16_OFFSET, buf, NPI_HEADER_SIZE );

	CmicM_MakeNPIFrameHeader(UART_P, nRetIdx); 

	gInst.m_tTxRx[UART_P].m_cTxBuf[nRetIdx++] = CmicM_CalculateFCS(&gInst.m_tTxRx[UART_P].m_cTxBuf[1], nRetIdx-1);
	gInst.m_tTxRx[UART_P].m_nTxLen = nRetIdx;
	UART_SendData(UART_P, gInst.m_tTxRx[UART_P].m_cTxBuf, nRetIdx);

#ifdef SURESOFT
	nRetIdx = CmicM_MakeBQFrame(UART_P, (BROADCAST_WRITE | (NUM_CELLS_ACTIVE/2-1)), //(NUM_CELLS_ACTIVE-1)), 
								BQ_CB_CELL_CTRL1_8_OFFSET, buf, nRetIdx );	

	CmicM_MakeNPIFrameHeader(UART_P, nRetIdx); 

	gInst.m_tTxRx[UART_P].m_cTxBuf[nRetIdx++] = CmicM_CalculateFCS(&gInst.m_tTxRx[UART_P].m_cTxBuf[1], nRetIdx-1);
	gInst.m_tTxRx[UART_P].m_nTxLen = nRetIdx;
	UART_SendData(UART_P, gInst.m_tTxRx[UART_P].m_cTxBuf, nRetIdx);
#endif  

	gInst.m_tSt.m_BalSub.m_eSetBalMonitor++;
}

/*******************************************************************************
** Function Name    : CmicM_SetBal_ST1_RES
** Description      : MAC Status (Success) check.
*******************************************************************************/
static void	CmicM_SetBal_ST1_RES(void)
{
	static  eRECV_Status	rStat = NO_RECV;

	//Secondary Check-------------------------------------------------------------------------------
	if (FALSE == (rStat & S_RECV))
	{
		if (UART_Polling_Cb(UART_S))
		{
			MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
			UART_ReceiveData(UART_S, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);

			switch(gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_IDX_CMDID])
			{				
				case eCMD_CB_TXCNF :					
					if ((gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_MAC_STATUS_IDX] == 0x00))
					{ 
						rStat |= S_RECV;
					}
				break;				
				default :		break;
			}
		}
	}

	//Primary Check----------------------------------------------------------------------------------
	if (FALSE == (rStat & P_RECV))
	{
		if (UART_Polling_Cb(UART_P))
		{
			MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
			UART_ReceiveData(UART_P, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);

			switch(gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_IDX_CMDID])
			{				
				case eCMD_CB_TXCNF :					
					if (gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_MAC_STATUS_IDX] == 0x00)
					{
						rStat |= P_RECV;
					}
				break;				
				default :	break;
			}
		}
	}

	if (rStat == ALL_RECV )
	{
		rStat = NO_RECV;
		gInst.m_tSt.m_eBal = eBAL_IDLE;	
		gInst.m_tSt.m_BalSub.m_eSetBalMonitor = eSETBAL_ST0_IDLE;
	}
}

static void	CmicM_Bal_ST0_IDLE(void)
{
	gInst.m_tSt.m_BalSub.m_eBalMonitor++;
}

/*******************************************************************************
** Function Name    : CmicM_Bal_ST1_REQ
** Description      : StackRead in one-time.
                      1. BAL_STAT(0x522) 
                      2~3. VCELL(0x578) + GPIO(0x5AC) 
					  4. BAL_SW_STAT2(0x526) 
					  5. BAL_CTRL1(0x338)
*******************************************************************************/
static void	CmicM_Bal_ST1_REQ(void)
{
	// Cell Read Operation
	uint16  nRetIdx=0;
	uint8_t   buf[5];

	//SECONDARY --------------------------------------------------------------------------------
	// Tx Buffer Clear.
	MemSet(&gInst.m_tTxRx[UART_S].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cTxBuf));
	gInst.m_tTxRx[UART_S].m_nTxLen = 0;

	// StackRead BQ_BAL_STAT_OFFSET
	MemSet(buf, 0 , sizeof(buf));
	buf[0] = BQ_BAL_STAT_READ; 

	nRetIdx = CmicM_MakeBQFrame(UART_S, (BROADCAST_READ),
							BQ_BAL_STAT_OFFSET, buf, NPI_HEADER_SIZE );

	// StackRead VCELL
	MemSet(buf, 0 , sizeof(buf));
	buf[0] = (uint8)(NUM_CELLS_ACTIVE * 2 - 1);

	nRetIdx = CmicM_MakeBQFrame(UART_S, (BROADCAST_READ), 
							BQ_VCELL_START_ADDR, buf, nRetIdx );	

	// StackRead BQ_GPIO1_HI_OFFSET
	MemSet(buf, 0 , sizeof(buf));
	buf[0] = (uint8)(NUM_GPIOS_ACTIVE * READ_2BYTE - 1);

	nRetIdx = CmicM_MakeBQFrame(UART_S, (BROADCAST_READ), 
							BQ_GPIO_START_ADDR, buf, nRetIdx );	

	// StackRead BQ_BAL_SW_STAT2_OFFSET
	MemSet(buf, 0 , sizeof(buf));
	buf[0] = READ_1BYTE;

	nRetIdx = CmicM_MakeBQFrame(UART_S, (BROADCAST_READ), 
							BQ_BAL_SW_STAT2_OFFSET, buf, nRetIdx );	

	if(!gInst.m_uBalGo)
	{
		// StackWrite BQ_BAL_CTRL2_OFFSET
		MemSet(buf, 0 , sizeof(buf));
		buf[0] = BQ_BAL_CTRL1_PWM_MSK;
		buf[1] = BQ_BAL_CTRL2_BAL_GO_MSK | BQ_CB_MODE_AUTO; 

		nRetIdx = CmicM_MakeBQFrame(UART_S, (BROADCAST_WRITE | WRITE_1BYTE),
								BQ_BAL_CTRL1_OFFSET, buf, nRetIdx );		  
	}
		
	//BQ FrameData ReqFrame에 저장
	MemSet(&gInst.m_ReqFrame[UART_S], 0, sizeof(gInst.m_ReqFrame[UART_S]));
	gInst.m_ReqFrame[UART_S].m_uCmd = eCMD_WD_BQ_RAWFRAME;
	gInst.m_ReqFrame[UART_S].m_nBQ_REG_ADDR = BQ_VCELL_START_ADDR; //BQ_VCELL18_HI_OFFSET;
	gInst.m_ReqFrame[UART_S].m_ReqDataLen = (NUM_CELLS_ACTIVE * 2 - 1)+(NUM_GPIOS_ACTIVE * READ_2BYTE - 1); //READ_35BYTE; //36byte read.
	//////////////////////////////////				  
		
	CmicM_MakeNPIFrameHeader(UART_S, nRetIdx); 

	gInst.m_tTxRx[UART_S].m_cTxBuf[nRetIdx++] = CmicM_CalculateFCS(&gInst.m_tTxRx[UART_S].m_cTxBuf[1], nRetIdx-1);
	gInst.m_tTxRx[UART_S].m_nTxLen = nRetIdx;
	UART_SendData(UART_S, gInst.m_tTxRx[UART_S].m_cTxBuf, nRetIdx);
	

	//PRIMARY -------------------------------------------------------------------------------------
	nRetIdx = 0;
	// Tx Buffer Clear.
	MemSet(&gInst.m_tTxRx[UART_P].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cTxBuf));
	gInst.m_tTxRx[UART_P].m_nTxLen = 0;

	// StackRead BQ_BAL_STAT_OFFSET
	MemSet(buf, 0 , sizeof(buf));
	buf[0] = BQ_BAL_STAT_READ; 

	nRetIdx = CmicM_MakeBQFrame(UART_P, (BROADCAST_READ),
							BQ_BAL_STAT_OFFSET, buf, NPI_HEADER_SIZE );

	// StackRead BQ_DEV_STAT1_OFFSET
	MemSet(buf, 0 , sizeof(buf));
	buf[0] = (uint8)(NUM_CELLS_ACTIVE * 2 - 1); //0x23(36-1) byte read.

	nRetIdx = CmicM_MakeBQFrame(UART_P, (BROADCAST_READ), 							
							BQ_VCELL_START_ADDR, buf, nRetIdx );	

	// StackRead BQ_GPIO1_HI_OFFSET
	MemSet(buf, 0 , sizeof(buf));
	buf[0] = (uint8)(NUM_GPIOS_ACTIVE * READ_2BYTE - 1); //0x15(22-1) byte read.

	nRetIdx = CmicM_MakeBQFrame(UART_P, (BROADCAST_READ), 
							BQ_GPIO_START_ADDR, buf, nRetIdx );	

	// StackRead BQ_BAL_SW_STAT2_OFFSET
	MemSet(buf, 0 , sizeof(buf));
	buf[0] = READ_1BYTE;

	nRetIdx = CmicM_MakeBQFrame(UART_P, (BROADCAST_READ), 
							BQ_BAL_SW_STAT2_OFFSET, buf, nRetIdx );	

	if(!gInst.m_uBalGo)
	{
		// StackWrite BQ_BAL_CTRL2_OFFSET
		MemSet(buf, 0 , sizeof(buf));
		buf[0] = BQ_BAL_CTRL1_PWM_MSK;
		buf[1] = BQ_BAL_CTRL2_BAL_GO_MSK | BQ_CB_MODE_AUTO; 

		nRetIdx = CmicM_MakeBQFrame(UART_P, (BROADCAST_WRITE | WRITE_1BYTE),
								BQ_BAL_CTRL1_OFFSET, buf, nRetIdx );

		gInst.m_uBalGo = TRUE;			  
	}

	//BQ FrameData ReqFrame에 저장
	MemSet(&gInst.m_ReqFrame[UART_P], 0, sizeof(gInst.m_ReqFrame[UART_P]));
	gInst.m_ReqFrame[UART_P].m_uCmd = eCMD_WD_BQ_RAWFRAME;
	gInst.m_ReqFrame[UART_P].m_nBQ_REG_ADDR = BQ_VCELL_START_ADDR; //BQ_VCELL18_HI_OFFSET;
	gInst.m_ReqFrame[UART_P].m_ReqDataLen = (NUM_CELLS_ACTIVE * 2 - 1)+(NUM_GPIOS_ACTIVE * READ_2BYTE - 1);
	//////////////////////////////////				  
		
	CmicM_MakeNPIFrameHeader(UART_P, nRetIdx); 

	gInst.m_tTxRx[UART_P].m_cTxBuf[nRetIdx++] = CmicM_CalculateFCS(&gInst.m_tTxRx[UART_P].m_cTxBuf[1], nRetIdx-1);
	gInst.m_tTxRx[UART_P].m_nTxLen = nRetIdx;
	UART_SendData(UART_P, gInst.m_tTxRx[UART_P].m_cTxBuf, nRetIdx);

	gInst.m_tInfo.m_balTime.m_timeP0 = GetTick_1ms();
	gInst.m_uNumOfCmd = 4;

	#ifndef SEQUENCE_READ
		//Polling Request
		CmicM_SetPollStatus(ePOLL_Xms_REQUEST);	
		gInst.m_tSt.m_BalSub.m_eBalMonitor = eBAL_ST1_WAIT;
	#else
		gInst.m_tSt.m_BalSub.m_eBalMonitor = eBAL_ST1_RES;
	#endif

}

/*******************************************************************************
** Function Name    : CmicM_Bal_ST1_WAIT
** Description      : UART 로부터의 전송완료 대기
*******************************************************************************/
static void CmicM_Bal_ST1_WAIT(void)
{
	if(CmicM_GetPollStatus() == ePOLL_Xms_RECEIVE)
	{
		CmicM_SetPollStatus(ePOLL_Xms_IDLE);
		gInst.m_tSt.m_BalSub.m_eBalMonitor++;
	}else{
		//wait..
		if (CmicM_GetPollStatus() == ePOLL_Xms_RUNNING){
			gInst.m_nWaitCnt++;
		}
	}	
}

/*******************************************************************************
** Function Name    : CmicM_Bal_ST1_RES
** Description      : BQ_BAL_STAT_OFFSET(0x522)의 check 
                      CB_DONE = 0x01(All cell balancing has completed)
					            0x00(Cell balancing is still running or has not started)
*******************************************************************************/
static void	CmicM_Bal_ST1_RES(void)
{
#ifndef SEQUENCE_READ

	uint8  idxP, idxS;
	CmicM_GetIndex(&idxP, &idxS);

	for (uint8 i=0; i < idxS ; i++ )
	{
	 	MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
	 	UART_SEQ_ReceiveData(UART_S, i, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);
		
	 	CmicM_RUN_VerifyRx_BQData(UART_S);	 		
	}
	 
	for (uint8 i=0; i < idxP ; i++ )
	{
		MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
		UART_SEQ_ReceiveData(UART_P, i, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);
		
		CmicM_RUN_VerifyRx_BQData(UART_P);			
	}
	
	gInst.m_tInfo.m_balTime.m_timeP1 = GetTick_1ms();
	gInst.m_tInfo.m_balTime.m_duration = (float)((gInst.m_tInfo.m_balTime.m_timeP1 - gInst.m_tInfo.m_balTime.m_timeP0)/1000.0f);
	
	if(gInst.m_tWbmsData[UART_P].m_uBalStat[gInst.m_RespFrame[UART_P].m_uNodeID] == 0x01)
	{
		gInst.m_tSt.m_eMain = eMAIN_RUN;
		gInst.m_tSt.m_eRun = eRUN_IDLE;		
	} 
	//else{
		gInst.m_tSt.m_eBal = eBAL_IDLE;
	//}

#else
	eRECV_Status	rStat = NO_RECV;
	uint16_t  fail_count=0;

	do{
		if (UART_Polling_Cb(UART_S))
		{
			MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
			UART_ReceiveData(UART_S, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);

			if (CmicM_RUN_VerifyRx_BQData(UART_S)){
				rStat |= S_RECV;
			
			}
		}else{
			fail_count++;
		}
		
	}while( rStat != S_RECV);

	gInst.m_tSt.m_BalSub.m_eBalMonitor = eBAL_ST2_RES;
#endif
}

static void	CmicM_Bal_ST2_REQ(void)
{

}

static void	CmicM_Bal_ST2_RES(void)
{
	eRECV_Status	rStat = NO_RECV;
	uint16_t  fail_count=0;

	do{
		if (UART_Polling_Cb(UART_P))
		{
			MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
			UART_ReceiveData(UART_P, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);

			if (CmicM_RUN_VerifyRx_BQData(UART_P)){
				rStat |= P_RECV;
			}
		}else{
			fail_count++;
		}		
	}while( rStat != P_RECV);

	gInst.m_tInfo.m_balTime.m_timeP1 = GetTick_1ms();
	gInst.m_tInfo.m_balTime.m_duration = (float)((gInst.m_tInfo.m_balTime.m_timeP1 - gInst.m_tInfo.m_balTime.m_timeP0)/1000.0f);
	
	if(gInst.m_tWbmsData[UART_P].m_uBalStat[gInst.m_RespFrame[UART_P].m_uNodeID] == 0x01)
	{
		gInst.m_tSt.m_eMain = eMAIN_RUN;
		gInst.m_tSt.m_eRun = eRUN_IDLE;
	}else{
		gInst.m_tSt.m_eBal = eBAL_IDLE;
	}
}


static void CmicM_Health_ST0_IDLE(void)
{	
	if(++gInst.m_cHealthNode >= gInst.m_uNAFEs[0])
	{
		gInst.m_cHealthNode=0;
	}
	gInst.m_tInfo.m_healthTime.m_timeP0 = GetTick_1ms();
	gInst.m_tSt.m_eHealthMonitor++;
}


static void CmicM_Health_ST1_REQ(void)
{
	// GETTXPACKETS=0x33 (전송패킷수)
	WBMS_COMMAND_E cmd;
	cmd = eCMD_ST_GETTXPACKETS;

	//SECONDARY -----------------------------------
	MemSet(&gInst.m_tTxRx[UART_S].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cTxBuf));
	
	CmicM_MakeNPISyncTxData(UART_S, cmd);
	gInst.m_tTxRx[UART_S].m_nTxLen = gInst.m_ReqFrame[UART_S].m_NPILen;
	UART_SendData(UART_S, gInst.m_tTxRx[UART_S].m_cTxBuf, gInst.m_tTxRx[UART_S].m_nTxLen);

	//PRIMARY --------------------------------------
	MemSet(&gInst.m_tTxRx[UART_P].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cTxBuf));

	CmicM_MakeNPISyncTxData(UART_P, cmd);
	gInst.m_tTxRx[UART_P].m_nTxLen = gInst.m_ReqFrame[UART_P].m_NPILen;
	UART_SendData(UART_P, gInst.m_tTxRx[UART_P].m_cTxBuf, gInst.m_tTxRx[UART_P].m_nTxLen);

	gInst.m_tSt.m_eHealthMonitor++;
}

static void CmicM_Health_ST1_RES(void)
{
	static  eRECV_Status	rStat = NO_RECV;
	
	//Secondary Check------------------------------------------------------------------------------------
	if (FALSE == (rStat & S_RECV))
	{
		if (UART_Polling_Cb(UART_S))
		{
			MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
			UART_ReceiveData(UART_S, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);

			if (CmicM_VerifyRx_CFGData(UART_S))
			{
				if(gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_IDX_CMDID] == eCMD_ST_GETTXPACKETS )
				{	//NumTxSuccessPacket(4), NumTxFailedPacket(4)
					gInst.m_tWbmsState[UART_S].m_qNumTxSuccessPacket = Bytes2Uint32(&gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_IDX_DATA]);
					gInst.m_tWbmsState[UART_S].m_qNumTxFailedPackets = Bytes2Uint32(&gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_IDX_DATA+4]);
					rStat |= S_RECV;
				}
			}
		}
	}

	//Primary Check--------------------------------------------------------------------------------------
	if (FALSE == (rStat & P_RECV))
	{
		if (UART_Polling_Cb(UART_P))
		{
			MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
			UART_ReceiveData(UART_P, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);

			if (CmicM_VerifyRx_CFGData(UART_P))
			{	//Check  CMDID 
				if(gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_IDX_CMDID] == eCMD_ST_GETTXPACKETS )
				{
					//NumTxSuccessPacket(4), NumTxFailedPacket(4)
					gInst.m_tWbmsState[UART_P].m_qNumTxSuccessPacket = Bytes2Uint32(&gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_IDX_DATA]);
					gInst.m_tWbmsState[UART_P].m_qNumTxFailedPackets = Bytes2Uint32(&gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_IDX_DATA+4]);
					rStat |= P_RECV;									
				}
			}
		}
	}

	if (rStat == ALL_RECV )
	{
		rStat = NO_RECV;		
		gInst.m_tSt.m_eHealthMonitor++;
	}
}

static void CmicM_Health_ST2_REQ(void)
{
	// GETTHROUGHPUT=0x34 (송수신성능)
	WBMS_COMMAND_E cmd;
	cmd = eCMD_ST_GETTHROUGHPUT;

	//SECONDARY -----------------------------------
	MemSet(&gInst.m_tTxRx[UART_S].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cTxBuf));
	
	CmicM_MakeNPISyncTxData(UART_S, cmd);
	gInst.m_tTxRx[UART_S].m_nTxLen = gInst.m_ReqFrame[UART_S].m_NPILen;
	UART_SendData(UART_S, gInst.m_tTxRx[UART_S].m_cTxBuf, gInst.m_tTxRx[UART_S].m_nTxLen);

	//PRIMARY --------------------------------------
	MemSet(&gInst.m_tTxRx[UART_P].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cTxBuf));

	CmicM_MakeNPISyncTxData(UART_P, cmd);
	gInst.m_tTxRx[UART_P].m_nTxLen = gInst.m_ReqFrame[UART_P].m_NPILen;
	UART_SendData(UART_P, gInst.m_tTxRx[UART_P].m_cTxBuf, gInst.m_tTxRx[UART_P].m_nTxLen);

	gInst.m_tSt.m_eHealthMonitor++;	
}

static void CmicM_Health_ST2_RES(void)
{
	static  eRECV_Status	rStat = NO_RECV;
	
	 //Secondary Check------------------------------------------------------------------------------------
	if (FALSE == (rStat & S_RECV))
	{
		if (UART_Polling_Cb(UART_S)){

			MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
			UART_ReceiveData(UART_S, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);

			if (CmicM_VerifyRx_CFGData(UART_S))
			{
				if(gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_IDX_CMDID] == eCMD_ST_GETTHROUGHPUT )
				{	//TxThroughput(4), RxThroughput(4)
					gInst.m_tWbmsState[UART_S].m_qTXthroughput = Bytes2Uint32(&gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_IDX_DATA]);
					gInst.m_tWbmsState[UART_S].m_qRXthroughput= Bytes2Uint32(&gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_IDX_DATA+4]);
					rStat |= S_RECV;
				}
			}
		}
	}

	 //Primary Check--------------------------------------------------------------------------------------
	if (FALSE == (rStat & P_RECV))
	{
		if (UART_Polling_Cb(UART_P))
		{
			MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
			UART_ReceiveData(UART_P, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);

			if (CmicM_VerifyRx_CFGData(UART_P))
			{	//Check  CMDID 
				if(gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_IDX_CMDID] == eCMD_ST_GETTHROUGHPUT )
				{
					//TxThroughput(4), RxThroughput(4)
					gInst.m_tWbmsState[UART_P].m_qTXthroughput = Bytes2Uint32(&gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_IDX_DATA]);
					gInst.m_tWbmsState[UART_P].m_qRXthroughput = Bytes2Uint32(&gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_IDX_DATA+4]);
					rStat |= P_RECV;									
				}
			}
		}
	}

	if (rStat == ALL_RECV )
	{
		rStat = NO_RECV;		
		gInst.m_tSt.m_eHealthMonitor++;
	}
}

static void CmicM_Health_ST3_REQ(void)
{
	// 0x13(WBMS_CMD_WD_RSSI)  RSSI
	WBMS_COMMAND_E cmd;
	cmd = eCMD_WD_RSSI; 

	//SECONDARY -----------------------------------------------------------------------------
	MemSet(&gInst.m_tTxRx[UART_S].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cTxBuf));
	
	CmicM_MakeNPISyncTxData(UART_S, cmd);
	gInst.m_tTxRx[UART_S].m_nTxLen = gInst.m_ReqFrame[UART_S].m_NPILen;
	UART_SendData(UART_S, gInst.m_tTxRx[UART_S].m_cTxBuf, gInst.m_tTxRx[UART_S].m_nTxLen);

	//PRIMARY -------------------------------------------------------------------------------
	MemSet(&gInst.m_tTxRx[UART_P].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cTxBuf));

	CmicM_MakeNPISyncTxData(UART_P, cmd);
	gInst.m_tTxRx[UART_P].m_nTxLen = gInst.m_ReqFrame[UART_P].m_NPILen;
	UART_SendData(UART_P, gInst.m_tTxRx[UART_P].m_cTxBuf, gInst.m_tTxRx[UART_P].m_nTxLen);

	gInst.m_tSt.m_eHealthMonitor++;
}

static void CmicM_Health_ST3_RES(void)
{
	static  eRECV_Status	rStat = NO_RECV;
	uint8   index=0;

	 //Secondary Check-----------------------------------------------------------------------------------
	if (FALSE == (rStat & S_RECV))
	{
		if (UART_Polling_Cb(UART_S))
		{
			MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
			UART_ReceiveData(UART_S, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);

			if (CmicM_VerifyRx_CFGData(UART_S))
			{
				if((gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_IDX_CMDID] == eCMD_WD_RSSI )
				    &&(gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_IDX_DATA] == 0xFF))  //0xFF : Return RSSI for all device nodes in the network.
				{	//RSSI of specific node					
					for (uint8_t i=0 ; i < gInst.m_uNAFEs[0] ; i++ )
					{
						index = NPI_IDX_DATA+ 1 + (i*3); 
						gInst.m_tWbmsState[UART_S].m_zWdRSSI[i].m_uNodeId = i;
						gInst.m_tWbmsState[UART_S].m_zWdRSSI[i].m_xULRSSIValue = (sint8)(gInst.m_tTxRx[UART_S].m_cRxBuf[index] - 0x100);
						gInst.m_tWbmsState[UART_S].m_zWdRSSI[i].m_xDLRSSIValue = (sint8)(gInst.m_tTxRx[UART_S].m_cRxBuf[index+1] - 0x100);
						gInst.m_tWbmsState[UART_S].m_zWdRSSI[i].m_uChannel = gInst.m_tTxRx[UART_S].m_cRxBuf[index+2];
					}
				    rStat |= S_RECV;
				}
			}
		}
	}

	 //Primary Check--------------------------------------------------------------------------------------
	if (FALSE == (rStat & P_RECV))
	{
		if (UART_Polling_Cb(UART_P))
		{
			MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
			UART_ReceiveData(UART_P, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);

			if (CmicM_VerifyRx_CFGData(UART_P))
			{	//Check  CMDID , NodeID
				if((gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_IDX_CMDID] == eCMD_WD_RSSI )
				   && (gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_IDX_DATA] == 0xFF))
				{
					index = 0;
					for (uint8_t i=0 ; i < gInst.m_uNAFEs[0] ; i++ )
					{
						index = NPI_IDX_DATA+ 1 + (i*3); 
						gInst.m_tWbmsState[UART_P].m_zWdRSSI[i].m_uNodeId = i;
						gInst.m_tWbmsState[UART_P].m_zWdRSSI[i].m_xULRSSIValue = (sint8)(gInst.m_tTxRx[UART_P].m_cRxBuf[index] - 0x100);
						gInst.m_tWbmsState[UART_P].m_zWdRSSI[i].m_xDLRSSIValue = (sint8)(gInst.m_tTxRx[UART_P].m_cRxBuf[index+1] - 0x100);
						gInst.m_tWbmsState[UART_P].m_zWdRSSI[i].m_uChannel = gInst.m_tTxRx[UART_P].m_cRxBuf[index+2];
					}
					rStat |= P_RECV;									
				}
			}
		}
	}

	if (rStat == ALL_RECV )
	{
		rStat = NO_RECV;
		gInst.m_tSt.m_eHealthMonitor++;		
	}
}

/*******************************************************************************
** Function Name    : CmicM_Health_ST4_REQ
** Description      : GETNETWORKSTATS(0x32) 특정노드의 네트워크 통계
                     ST4와 ST5는 노드별 네트워크 정보가 필요하기때문에 노드수만큼 반복.
*******************************************************************************/
static void CmicM_Health_ST4_REQ(void)
{
	WBMS_COMMAND_E cmd;
	cmd = eCMD_ST_GETNETWORKSTATS;

	//SECONDARY -----------------------------------
	MemSet(&gInst.m_tTxRx[UART_S].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cTxBuf));
	
	CmicM_MakeNPISyncTxData(UART_S, cmd);
	gInst.m_tTxRx[UART_S].m_nTxLen = gInst.m_ReqFrame[UART_S].m_NPILen;
	UART_SendData(UART_S, gInst.m_tTxRx[UART_S].m_cTxBuf, gInst.m_tTxRx[UART_S].m_nTxLen);

	//PRIMARY --------------------------------------
	MemSet(&gInst.m_tTxRx[UART_P].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cTxBuf));

	CmicM_MakeNPISyncTxData(UART_P, cmd);
	gInst.m_tTxRx[UART_P].m_nTxLen = gInst.m_ReqFrame[UART_P].m_NPILen;
	UART_SendData(UART_P, gInst.m_tTxRx[UART_P].m_cTxBuf, gInst.m_tTxRx[UART_P].m_nTxLen);

	gInst.m_tSt.m_eHealthMonitor++;

}

static void CmicM_Health_ST4_RES(void)
{
	static  eRECV_Status	rStat = NO_RECV;
	
	 //Secondary Check---------------------------------------------------------------------------------------
	if (FALSE == (rStat & S_RECV))
	{
		if (UART_Polling_Cb(UART_S)){

			MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
			UART_ReceiveData(UART_S, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);

			if (CmicM_VerifyRx_CFGData(UART_S))
			{
				if((gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_IDX_CMDID] == eCMD_ST_GETNETWORKSTATS )&&
					(gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_IDX_DATA] == gInst.m_cHealthNode))
				{	//Latency(4)+PER(4)
					gInst.m_tWbmsState[UART_S].m_zWdHealth[gInst.m_cHealthNode].m_fLatency = (float)((Bytes2Uint32(&gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_IDX_DATA+1]))/1000.0);
					//gInst.m_tWbmsState[UART_S].m_zWdHealth[gInst.m_cHealthNode].m_qPER = Bytes2Uint32(&gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_IDX_DATA+5]);
					rStat |= S_RECV;
				}
			}
		}
	}

	 //Primary Check----------------------------------------------------------------------------------------
	if (FALSE == (rStat & P_RECV))
	{
		if (UART_Polling_Cb(UART_P)){

			MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
			UART_ReceiveData(UART_P, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);

			if (CmicM_VerifyRx_CFGData(UART_P))
			{	//Check  CMDID , NodeID
				if((gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_IDX_CMDID] == eCMD_ST_GETNETWORKSTATS )&&
					(gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_IDX_DATA] == gInst.m_cHealthNode))
				{
					//Latency(4)+PER(4)
					gInst.m_tWbmsState[UART_P].m_zWdHealth[gInst.m_cHealthNode].m_fLatency = (float)((Bytes2Uint32(&gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_IDX_DATA+1]))/1000.0);
					//gInst.m_tWbmsState[UART_P].m_zWdHealth[gInst.m_cHealthNode].m_qPER = Bytes2Uint32(&gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_IDX_DATA+5]);
					rStat |= P_RECV;									
				}
			}
		}
	}

	if (rStat == ALL_RECV )
	{
		rStat = NO_RECV;		
		gInst.m_tSt.m_eHealthMonitor++;
	}
}

/*******************************************************************************
** Function Name    : CmicM_Health_ST5_REQ
** Description      :  GETRXPACKETS(0x35) 노드별 수신패킷수
                     ST4와 ST5는 노드별 네트워크 정보가 필요하기때문에 노드수만큼 반복.
*******************************************************************************/
static void CmicM_Health_ST5_REQ(void)
{	
	WBMS_COMMAND_E cmd;
	cmd = eCMD_ST_GETRXPACKETS;

	//SECONDARY -----------------------------------
	MemSet(&gInst.m_tTxRx[UART_S].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cTxBuf));
	
	CmicM_MakeNPISyncTxData(UART_S, cmd);
	gInst.m_tTxRx[UART_S].m_nTxLen = gInst.m_ReqFrame[UART_S].m_NPILen;
	UART_SendData(UART_S, gInst.m_tTxRx[UART_S].m_cTxBuf, gInst.m_tTxRx[UART_S].m_nTxLen);

	//PRIMARY --------------------------------------
	MemSet(&gInst.m_tTxRx[UART_P].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cTxBuf));

	CmicM_MakeNPISyncTxData(UART_P, cmd);
	gInst.m_tTxRx[UART_P].m_nTxLen = gInst.m_ReqFrame[UART_P].m_NPILen;
	UART_SendData(UART_P, gInst.m_tTxRx[UART_P].m_cTxBuf, gInst.m_tTxRx[UART_P].m_nTxLen);

	gInst.m_tSt.m_eHealthMonitor++;

}

static void CmicM_Health_ST5_RES(void)
{
	static  eRECV_Status	rStat = NO_RECV;
	
	 //Secondary Check-----------------------------------------------------------------------------------
	if (FALSE == (rStat & S_RECV))
	{
		if (UART_Polling_Cb(UART_S))
		{
			MemSet(&gInst.m_tTxRx[UART_S].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_S].m_cRxBuf));
			UART_ReceiveData(UART_S, gInst.m_tTxRx[UART_S].m_cRxBuf, &gInst.m_tTxRx[UART_S].m_nRxLen);

			if (CmicM_VerifyRx_CFGData(UART_S))
			{
				if((gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_IDX_CMDID] == eCMD_ST_GETRXPACKETS )&&
					(gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_IDX_DATA] == gInst.m_cHealthNode))
				{	//NumRxSuccessPkt(4)+NumRxMissedPkt(4)
					gInst.m_tWbmsState[UART_S].m_zWdHealth[gInst.m_cHealthNode].m_qNumRxSuccessPkt = Bytes2Uint32(&gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_IDX_DATA+1]);
					gInst.m_tWbmsState[UART_S].m_zWdHealth[gInst.m_cHealthNode].m_qNumRxMissedPkt = Bytes2Uint32(&gInst.m_tTxRx[UART_S].m_cRxBuf[NPI_IDX_DATA+5]);
					rStat |= S_RECV;
				}
			}
		}
	}

	 //Primary Check-------------------------------------------------------------------------------------
	if (FALSE == (rStat & P_RECV))
	{
		if (UART_Polling_Cb(UART_P))
		{
			MemSet(&gInst.m_tTxRx[UART_P].m_cRxBuf, 0, sizeof(gInst.m_tTxRx[UART_P].m_cRxBuf));
			UART_ReceiveData(UART_P, gInst.m_tTxRx[UART_P].m_cRxBuf, &gInst.m_tTxRx[UART_P].m_nRxLen);

			if (CmicM_VerifyRx_CFGData(UART_P))
			{	//Check  CMDID , NodeID
				if((gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_IDX_CMDID] == eCMD_ST_GETRXPACKETS )&&
					(gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_IDX_DATA] == gInst.m_cHealthNode))
				{	//NumRxSuccessPkt(4)+NumRxMissedPkt(4)
					gInst.m_tWbmsState[UART_P].m_zWdHealth[gInst.m_cHealthNode].m_qNumRxSuccessPkt = Bytes2Uint32(&gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_IDX_DATA+1]);
					gInst.m_tWbmsState[UART_P].m_zWdHealth[gInst.m_cHealthNode].m_qNumRxMissedPkt = Bytes2Uint32(&gInst.m_tTxRx[UART_P].m_cRxBuf[NPI_IDX_DATA+5]);
					rStat |= P_RECV;									
				}
			}
		}
	}

	if (rStat == ALL_RECV )
	{
		rStat = NO_RECV;
		CmicM_CalculatePDR(gInst.m_cHealthNode);
		
		if(++gInst.m_cHealthNode >= gInst.m_uNAFEs[0])
		{	
			gInst.m_tSt.m_eBal = eBAL_IDLE;
			gInst.m_tSt.m_eRun = eRUN_IDLE;
			gInst.m_tSt.m_eHealthMonitor = eHEALTH_ST0_IDLE;
		
			gInst.m_tInfo.m_healthTime.m_timeP1 = GetTick_1ms();
			gInst.m_tInfo.m_healthTime.m_duration = (float)((gInst.m_tInfo.m_healthTime.m_timeP1 - gInst.m_tInfo.m_healthTime.m_timeP0)/1000.0f);

		}else {
			gInst.m_tSt.m_eHealthMonitor = eHEALTH_ST4_REQ;
		}
	}
}

/********************************************************************************
FUNCTION :   CmicM_CheckBQRegisterStatus(uint8_t uart, uint8_t Dataidx)
DESCRIPTION : BQ Register 관련 수신에 대한 parsing.
**********************************************************************************/
static uint8 CmicM_CheckBQRegisterStatus(uint8_t uart, uint8_t Dataidx)
{
	uint8    uRet = FALSE;
	uint8    uBQDataLen = (gInst.m_RespFrame[uart].m_uBQ_INIT & 0x7F) + 1;

	MemCpy(&gInst.m_RespFrame[uart].m_uBQ_DATA[0], &gInst.m_tTxRx[uart].m_cRxBuf[Dataidx+10], uBQDataLen);

	switch(gInst.m_RespFrame[uart].m_nBQ_REG_ADDR)
	{
		case	BQ_VCELL_START_ADDR :  //0x574
				CmicM_DecodeVCELLData(uart);
				uRet = TRUE;
				break;
		case	BQ_GPIO_START_ADDR : //0x5A8
				CmicM_DecodeGPIOData(uart);
				uRet = TRUE;
				break;
		
		case	BQ_DEV_STAT1_OFFSET :  // 0x530
				if ((gInst.m_RespFrame[uart].m_uBQ_DATA[0] & 0x02 ) && 
					( gInst.m_RespFrame[uart].m_uBQ_DATA[0] & 0x01))
				{
					uRet = TRUE;
				}else {
					gInst.m_nBQStatNGCnt[uart]++;					
				}
				break;

		case 	BQ_DIAG_STAT1_OFFSET : //0x52D
				if (( gInst.m_RespFrame[uart].m_uBQ_DATA[1] & 0x04 ) && 
					(gInst.m_RespFrame[uart].m_uBQ_DATA[2] & 0x01) && 
					( gInst.m_RespFrame[uart].m_uBQ_DATA[2] & 0x02))
				{
					uRet = TRUE;
				}else {
					gInst.m_nBQStatNGCnt[uart]++;					
				}
				break ;
		case 	BQ_CUST_CRC_RSLT_HI_OFFSET : //0x50C
				MemCpy(gInst.m_uSiglewriteRes[uart], &gInst.m_RespFrame[uart].m_uBQ_DATA[0], 2u);
				uRet = TRUE;
				break;
		case	BQ_BAL_STAT_OFFSET : //BQ_BAL_STAT_OFFSET :  //0x522
				gInst.m_tWbmsData[uart].m_uBalStat[gInst.m_RespFrame[uart].m_uNodeID] = gInst.m_RespFrame[uart].m_uBQ_DATA[0];
				uRet = TRUE;
				break;
		case	BQ_BAL_SW_STAT2_OFFSET : //BQ_BAL_SW_STAT2_OFFSET :  //0x526
				MemCpy(&gInst.m_tWbmsData[uart].m_uBalSWStat[gInst.m_RespFrame[uart].m_uNodeID][0], &gInst.m_RespFrame[uart].m_uBQ_DATA, 2);
				uRet = TRUE;
				break;
		
		default :
			uRet = TRUE;
			break;
	}
	return uRet;
}


/********************************************************************************
FUNCTION :  CmicM_RUN_VerifyRx_BQData(uint8_t uart)
DESCRIPTION : Running 상태에서 BQ Command 수신에 대한 확인
**********************************************************************************/
static uint8 CmicM_RUN_VerifyRx_BQData(uint8_t uart)
{	
	static   uint8  uNode[2] = {0, 0};
	static   uint8	Dataidx;
	uint8    		uRet = FALSE;


	MemSet(&gInst.m_RespFrame[uart], 0, sizeof(CmicM_RespFrame[uart]));
	//  Receive Data Dispatch.
	//  1. Get Length data
	gInst.m_RespFrame[uart].m_nLength = (uint16)((gInst.m_tTxRx[uart].m_cRxBuf[2] << 8)+(gInst.m_tTxRx[uart].m_cRxBuf[1]));

	if ( (gInst.m_tTxRx[uart].m_nRxLen ) >= (gInst.m_RespFrame[uart].m_nLength + NPI_PROTO_LEN)){

		if(COMIF_OK == CmicM_FrameCheck(&gInst.m_tTxRx[uart].m_cRxBuf[0], gInst.m_RespFrame[uart].m_nLength+NPI_PROTO_LEN) ){

			//2. Get CmdType, ID, NodeId
			gInst.m_RespFrame[uart].m_uCmdType = gInst.m_tTxRx[uart].m_cRxBuf[3];
			gInst.m_RespFrame[uart].m_uCmdID = gInst.m_tTxRx[uart].m_cRxBuf[4];

			switch (gInst.m_RespFrame[uart].m_uCmdID)
			{
				case eCMD_WD_BQ_RAWFRAME :
					Dataidx = 0;

					for(uint8_t cellgpio=0; cellgpio < gInst.m_uNumOfCmd; cellgpio++)
					{
						gInst.m_RespFrame[uart].m_uNodeID = gInst.m_tTxRx[uart].m_cRxBuf[5];
						//3. Get BQ Frame (INIT+DEVADDR+REGADDR+DATA+TIMESTAMP)
						gInst.m_RespFrame[uart].m_uBQ_INIT = gInst.m_tTxRx[uart].m_cRxBuf[Dataidx+6];
						gInst.m_RespFrame[uart].m_nBQ_REG_ADDR = 
						 (uint16)((gInst.m_tTxRx[uart].m_cRxBuf[Dataidx+8]<< 8)+(gInst.m_tTxRx[uart].m_cRxBuf[Dataidx+9]));

						switch(gInst.m_RespFrame[uart].m_nBQ_REG_ADDR)
						{
							case BQ_BAL_STAT_OFFSET :
							case BQ_VCELL_START_ADDR :
									CmicM_CheckBQRegisterStatus(uart, Dataidx);
							break;
							case BQ_GPIO_START_ADDR :
									if (CmicM_CheckBQRegisterStatus(uart, Dataidx) &&(gInst.m_tSt.m_eMain == eMAIN_RUN))
									{
										uNode[uart]++;	
									}
							break;
							case BQ_BAL_SW_STAT2_OFFSET :
									if (CmicM_CheckBQRegisterStatus(uart, Dataidx))
									{
										uNode[uart]++;	
									}
							break;
							default :
							break;
						}
						Dataidx += (gInst.m_RespFrame[uart].m_uBQ_INIT+1) + NPI_PROTO_LEN;
					}
					break;

				case eCMD_CB_TXCNF :
				// MAC_STATUS가 0x01로  Fail 이 수신될 경우 ..
					if ((gInst.m_tTxRx[uart].m_cRxBuf[NPI_MAC_STATUS_IDX] != 0x00))
					{
						CmicM_MacStatusFail(uart, (uint8)(gInst.m_tTxRx[uart].m_cRxBuf[NPI_MAC_STATUS_IDX]));
					}

					if (uNode[uart] >= gInst.m_uNAFEs[uart] ) 
					{
						uNode[uart]= 0;
						uRet=TRUE;
					}else {						
						
						gInst.m_nTxConfirmNGCnt++;
					}

				break;
				case eCMD_CB_FAULT :
					gInst.m_nDiagFaultCnt[uart]++;
				break;
				default:
					gInst.m_nNGCnt++;
				break;
			}			
		}
	}
	 return uRet;
	
}

/********************************************************************************
FUNCTION :  CmicM_MacStatusFail
DESCRIPTION :  MacStatus Fail에 대한 Debug Function 
**********************************************************************************/
static void CmicM_MacStatusFail(eUART uart, uint8  failCode)
{
	switch(failCode)
	{
		case 1 : 
		gInst.m_nMacStatFailCnt[uart]++;

		break;
		default :
		gInst.m_nMacStatFailCnt[uart] += (gInst.m_nMacStatFailCnt[uart] << failCode);
		break;
	}
	return;
}

/********************************************************************************
FUNCTION :  CmicM_VerifyRx_BQData(uint8_t uart)
DESCRIPTION : BQ Command 수신에 대한 확인
**********************************************************************************/
static uint8 CmicM_VerifyRx_BQData(uint8_t uart)
{	
	static   uint8  uNodeID[2] = {0, 0};
	uint8    uRet = FALSE;

	MemSet(&gInst.m_RespFrame[uart], 0, sizeof(CmicM_RespFrame[uart]));

	//  1. Get Length data
	gInst.m_RespFrame[uart].m_nLength = (uint16)((gInst.m_tTxRx[uart].m_cRxBuf[2] << 8)+(gInst.m_tTxRx[uart].m_cRxBuf[1]));

	if ( (gInst.m_tTxRx[uart].m_nRxLen ) >= (gInst.m_RespFrame[uart].m_nLength + NPI_PROTO_LEN))
	{
		if(COMIF_OK == CmicM_FrameCheck(&gInst.m_tTxRx[uart].m_cRxBuf[0], gInst.m_RespFrame[uart].m_nLength+NPI_PROTO_LEN) )
		{
			//2. Get CmdType, ID, NodeId
			gInst.m_RespFrame[uart].m_uCmdType = gInst.m_tTxRx[uart].m_cRxBuf[3];
			gInst.m_RespFrame[uart].m_uCmdID = gInst.m_tTxRx[uart].m_cRxBuf[4];

			switch (gInst.m_RespFrame[uart].m_uCmdID)
			{
				case eCMD_WD_BQ_RAWFRAME :
					gInst.m_RespFrame[uart].m_uNodeID = gInst.m_tTxRx[uart].m_cRxBuf[5];
					//3. Get BQ Frame (INIT+DEVADDR+REGADDR+DATA+TIMESTAMP)
					gInst.m_RespFrame[uart].m_uBQ_INIT = gInst.m_tTxRx[uart].m_cRxBuf[6];
					gInst.m_RespFrame[uart].m_nBQ_REG_ADDR = (uint16)((gInst.m_tTxRx[uart].m_cRxBuf[8]<< 8)+(gInst.m_tTxRx[uart].m_cRxBuf[9]));
				
					if (gInst.m_RespFrame[uart].m_nBQ_REG_ADDR == gInst.m_ReqFrame[uart].m_nBQ_REG_ADDR)
					{
						if (CmicM_CheckBQRegisterStatus(uart, 0))  //Register Data OK 	
						{
							uNodeID[uart]++;								
						}
					}
					break;

				case eCMD_CB_TXCNF :
					if(( uNodeID[uart] >= gInst.m_uNAFEs[uart] ) && (gInst.m_tTxRx[uart].m_cRxBuf[NPI_MAC_STATUS_IDX] == 0x00))
					{
						uNodeID[uart] = 0;
						uRet=TRUE;
					}

				break;
				case eCMD_CB_FAULT :
					gInst.m_nDiagFaultCnt[uart]++;
				break;
				default:
					gInst.m_nNGCnt++;
				break;
			}			
		}
	}
	return uRet;	
}


/********************************************************************************
FUNCTION :  CmicM_VerifyRx_CFGData(uint8_t uart)
DESCRIPTION : Network Configuration Command 수신에 대한 확인
**********************************************************************************/
static uint8 CmicM_VerifyRx_CFGData(uint8_t uart)
{

	MemSet(&gInst.m_RespFrame[uart], 0, sizeof(CmicM_RespFrame[uart]));	
	///  Receive Data Dispatch.
	//1. Get Length data
	gInst.m_RespFrame[uart].m_nLength = (uint16)((gInst.m_tTxRx[uart].m_cRxBuf[2] << 8)+(gInst.m_tTxRx[uart].m_cRxBuf[1]));

	if(COMIF_OK == CmicM_FrameCheck(&gInst.m_tTxRx[uart].m_cRxBuf[0], gInst.m_RespFrame[uart].m_nLength+NPI_PROTO_LEN) ){

		//2. Get CmdType, ID, NodeId
		gInst.m_RespFrame[uart].m_uCmdType = gInst.m_tTxRx[uart].m_cRxBuf[3];
		gInst.m_RespFrame[uart].m_uCmdID = gInst.m_tTxRx[uart].m_cRxBuf[4];
	
		//CmdID이후의 데이터를 복사.
		MemCpy( (gInst.m_RespFrame[uart].m_uBQ_DATA), &gInst.m_tTxRx[uart].m_cRxBuf[5], gInst.m_RespFrame[uart].m_nLength);
		return TRUE;

	}
	
	return FALSE;	
}

/********************************************************************************
FUNCTION :  CmicM_DecodeVCELLData(uint8_t uart)
DESCRIPTION : VCELL Data( Sensing Data )
**********************************************************************************/
static void CmicM_DecodeVCELLData(uint8_t uart)
{
	uint8  		i; 
	uint16 		wDataBuf[NUM_CELLS_ACTIVE];

	uint8  		offset;
	uint8  		uLength = (gInst.m_RespFrame[uart].m_uBQ_INIT & 0x7F) + 1;


	//VCELL 1~18 까지 저장
	MemCpy(&gInst.m_tRawData[uart].m_uVoltage[gInst.m_RespFrame[uart].m_uNodeID][0], &gInst.m_RespFrame[uart].m_uBQ_DATA, uLength);
		
	for (i = 0 ; i < NUM_CELLS_ACTIVE; i++)
	{
		offset = uLength-(i*2)-1;
		wDataBuf[i] = (uint16)((gInst.m_RespFrame[uart].m_uBQ_DATA[offset-1] << 8) | (gInst.m_RespFrame[uart].m_uBQ_DATA[offset]));
	}

	//해당 노드에 복사
	MemCpy(&gInst.m_tWbmsData[uart].m_zVoltageAdc[gInst.m_RespFrame[uart].m_uNodeID], wDataBuf, uLength);

	// Voltage Convert.
	for (i =0 ; i < NUM_CELLS_ACTIVE; i++){
		gInst.m_tWbmsData[uart].m_zVoltage[gInst.m_RespFrame[uart].m_uNodeID][i] 
		  = (float)VCELL_CONVERT(gInst.m_tWbmsData[uart].m_zVoltageAdc[gInst.m_RespFrame[uart].m_uNodeID][i]);
	}

	return ;
}

/********************************************************************************
FUNCTION :  CmicM_DecodeGPIOData(uint8_t uart)
DESCRIPTION : GPIO Data( Sensing Data )
**********************************************************************************/
static void CmicM_DecodeGPIOData(uint8_t uart)
{
	uint8_t  		GpioIdx; 
	uint16_t 		wDataBuf[NUM_GPIOS_ACTIVE];

	uint8_t  		uLength = (gInst.m_RespFrame[uart].m_uBQ_INIT & 0x7F) + 1;

	//GPIO 1~18 까지 저장
	MemCpy(&gInst.m_tRawData[uart].m_uGpio[gInst.m_RespFrame[uart].m_uNodeID][0], &gInst.m_RespFrame[uart].m_uBQ_DATA, uLength);
		
	for (GpioIdx = 0 ; GpioIdx < NUM_GPIOS_ACTIVE; GpioIdx++)
	{
		wDataBuf[GpioIdx] = (uint16)((gInst.m_RespFrame[uart].m_uBQ_DATA[GpioIdx+(1*GpioIdx)] << 8) | (gInst.m_RespFrame[uart].m_uBQ_DATA[GpioIdx+(1*GpioIdx)+1]));		
	}
		
	for (GpioIdx = 0 ; GpioIdx < NUM_GPIOS_ACTIVE; GpioIdx++)
	{
		gInst.m_tWbmsData[uart].m_zTempAdc[gInst.m_RespFrame[uart].m_uNodeID][GpioIdx] = (float)(wDataBuf[GpioIdx]/10000.0);		
	}


	return ;
}

/********************************************************************************
FUNCTION :  CmicM_PriorVcell_Init(uint8_t uart)
DESCRIPTION : Prior Vcell decode
**********************************************************************************/
static void	CmicM_PriorVcell_Init(uint8_t uart)
{
	for(uint8_t NodeID = 0; NodeID < MAX_NUM_DEVS; NodeID++)
	{
		for(uint8_t Cell=0; Cell < NUM_CELLS_ACTIVE; Cell++)
		{
			gInst.m_tWbmsData[uart].m_zPriorVoltage[NodeID][Cell] = gInst.m_tWbmsData[uart].m_zVoltageAdc[NodeID][Cell];
		}
	}
}

/********************************************************************************
FUNCTION :  CmicM_AbsDiffVoltage(uint16_t Volt1, uint16_t Volt2)
DESCRIPTION : Absolute voltage difference
**********************************************************************************/
static uint16_t CmicM_AbsDiffVoltage(uint16_t Volt1, uint16_t Volt2)
{
	uint16_t DiffVol = BQ_VAL_INVALDVOLVALUE; /* the vol1/2 is 0xFFFF, invalid value */

    /* VOL1 < 65534, The vol1 is positive */
    if(Volt1 < BQ_VAL_INVALDVOLVALUE)
    {
        /* VOL2 < 32768, The vol2 is positive */
        if (Volt2 < BQ_VAL_INVALDVOLVALUE)
        {
            /* vol1 less than vol2 */
            if (Volt1 <= Volt2)
            {
                DiffVol = Volt2 - Volt1;
            }
            else
            {
                DiffVol = Volt1 - Volt2;
            }
        }
    }

    return DiffVol;
}

/********************************************************************************
FUNCTION :  CmicM_VcellOWDDecode(uint8_t uart)
DESCRIPTION : Vcell OWD Data decode ( Vcell )
**********************************************************************************/
static void CmicM_VcellOWDDecode(uint8_t uart)
{
	uint16_t CurrVolt;
	uint16_t PriorVolt;
	uint16_t AbsVolt;

	for(uint8_t NodeID = 0; NodeID < MAX_NUM_DEVS/2; NodeID++)
	{
		for(uint8_t Cell = 0; Cell < NUM_CELLS_ACTIVE; Cell++)
		{
			CurrVolt = gInst.m_tWbmsData[uart].m_zVoltageAdc[NodeID][Cell];
			PriorVolt = gInst.m_tWbmsData[uart].m_zPriorVoltage[NodeID][Cell];
			AbsVolt = CmicM_AbsDiffVoltage(CurrVolt, PriorVolt);

			// if(AbsVolt > BQ_VCELL_THRESHHOLD)
			// {
			// 	TIBMS_BitSet(gInst.m_tFdtiResult.m_zSM_VC_OPN_DET, Cell);
			// }
			// else
			// {
			// 	TIBMS_BitClear(gInst.m_tFdtiResult.m_zSM_VC_OPN_DET, Cell);
			// }
			if(CurrVolt < BQ_VCELL_THRESHHOLD)
			{
				TIBMS_BitSet(gInst.m_tFdtiResult[uart].m_zSM_VC_OPN_DET[NodeID], Cell);
			}
			else
			{
				TIBMS_BitClear(gInst.m_tFdtiResult[uart].m_zSM_VC_OPN_DET[NodeID], Cell);
			}
		}
	}
}

/********************************************************************************
FUNCTION :  CmicM_MakeNPISyncTxData(eUART uart, WBMS_COMMAND_E cmd)
DESCRIPTION : Network Configuration 관련 송신데이터 작성
**********************************************************************************/
static uint8_t CmicM_MakeNPISyncTxData(eUART uart, WBMS_COMMAND_E cmd)
{
	uint8_t ret = TRUE;
	uint8_t NwParams[] = {0xDD, 0xDD, MAX_NUM_DEVS, 0x46, 0xFA, 0x00, NUM_PARTIAL_DEVS, 0x03
	                             ,KEEP_ALIVE_INTERVAL, 0x28, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	// uint8_t timeout_start_nw[] = {0x80, 0x96, 0x98, 0x00}; //이 값은  NG발생.
	uint8_t timeout_start_nw[] = {0xFF, 0xF7, 0x00, 0x00};
	uint8_t timeout_para[] = {0xD0, 0x07, 0x00, 0x00};
	uint8_t cnt=0;
	uint8_t  i=0;
	uint8_t  start, end, length;  //SETDEVTBL용
	
	MemSet(&gInst.m_ReqFrame[uart], 0, sizeof(gInst.m_ReqFrame[uart])); //Memory Reset 

	switch(cmd)
	{
		case eCMD_CF_SETNWOPMODE : //0x47

			gInst.m_ReqFrame[uart].m_ReqDataLen = READ_1BYTE;
			gInst.m_ReqFrame[uart].m_TxCmd[0] = 0x00;
			gInst.m_ReqFrame[uart].m_NPILen = NPI_PROTO_LEN + gInst.m_ReqFrame[uart].m_ReqDataLen;
			gInst.m_ReqFrame[uart].m_uCmd = eCMD_CF_SETNWOPMODE;
			gInst.m_ReqFrame[uart].m_uCmdTyp = NPI_MSG_SYNC_REQ;
		break;		

		case eCMD_CF_SETPARAMS : //0x4E

			gInst.m_ReqFrame[uart].m_ReqDataLen = READ_16BYTE; // 16Bytes
			gInst.m_ReqFrame[uart].m_TxCmd[cnt++] = 0x01; //Type:MAC_Set_DWMRole				
			gInst.m_ReqFrame[uart].m_TxCmd[cnt++] = 0x0D; //Length1st
			gInst.m_ReqFrame[uart].m_TxCmd[cnt++] = 0x00; //Length2nd
			
			if (uart == UART_P){			
				gInst.m_ReqFrame[uart].m_TxCmd[cnt++] = UART_P; //Role-Primary
				MemCpy(&gInst.m_ReqFrame[uart].m_TxCmd[cnt], 
					&gInst.m_tQueryInfo[UART_S].m_uWMainUID, COMM_DEVICE_UID_SIZE);				
			}else {			
				gInst.m_ReqFrame[uart].m_TxCmd[cnt++] = UART_S; //Role-Secondary
				MemCpy(&gInst.m_ReqFrame[uart].m_TxCmd[cnt], 
					&gInst.m_tQueryInfo[UART_P].m_uWMainUID, COMM_DEVICE_UID_SIZE);				
			}
			cnt += COMM_DEVICE_UID_SIZE;

			MemCpy(&gInst.m_ReqFrame[uart].m_TxCmd[cnt], timeout_para, sizeof(timeout_para));
			
			gInst.m_ReqFrame[uart].m_NPILen = NPI_PROTO_LEN + gInst.m_ReqFrame[uart].m_ReqDataLen;
			gInst.m_ReqFrame[uart].m_uCmd = eCMD_CF_SETPARAMS;
			gInst.m_ReqFrame[uart].m_uCmdTyp = NPI_MSG_SYNC_REQ;
		
		break;

		case eCMD_CF_SETMAINCONFIG : //0x40

			gInst.m_ReqFrame[uart].m_ReqDataLen = READ_16BYTE;
			MemCpy(gInst.m_ReqFrame[uart].m_TxCmd, NwParams, gInst.m_ReqFrame[uart].m_ReqDataLen);
			gInst.m_ReqFrame[uart].m_NPILen = NPI_PROTO_LEN + gInst.m_ReqFrame[uart].m_ReqDataLen;
			gInst.m_ReqFrame[uart].m_uCmd = eCMD_CF_SETMAINCONFIG;
			gInst.m_ReqFrame[uart].m_uCmdTyp = NPI_MSG_SYNC_REQ;
		break;

		case eCMD_CF_SETJOINMODE : //0x48

			gInst.m_ReqFrame[uart].m_ReqDataLen = READ_1BYTE;
			gInst.m_ReqFrame[uart].m_TxCmd[0] = 0x01;  //Selective //0x00;
			gInst.m_ReqFrame[uart].m_NPILen = NPI_PROTO_LEN + gInst.m_ReqFrame[uart].m_ReqDataLen;
			gInst.m_ReqFrame[uart].m_uCmd = eCMD_CF_SETJOINMODE;
			gInst.m_ReqFrame[uart].m_uCmdTyp = NPI_MSG_SYNC_REQ;
		break;
		
		case eCMD_CF_SETDEVTBLCFG : //0x49

			start = 0;
			if (MAX_NUM_DEVS > 15){
				end = 15;
			}else {
				end = MAX_NUM_DEVS;
			}			
			gInst.m_ReqFrame[uart].m_ReqDataLen = (end * (COMM_DEVICE_UID_SIZE + 1)) + 1; //(Dev수*UniqID+SlotN)+nDevice 
			gInst.m_ReqFrame[uart].m_TxCmd[cnt++] = end; // nDevice(1Byte)
			// MAC ID copy
			for (i = start ; i < end; i++){
				MemCpy(&gInst.m_ReqFrame[uart].m_TxCmd[cnt], &zMACCfgNODE_ID[i],  COMM_DEVICE_UID_SIZE );
				gInst.m_ReqFrame[uart].m_TxCmd[cnt+8] = i;
				cnt += 9;				
			}		
			gInst.m_ReqFrame[uart].m_NPILen = NPI_PROTO_LEN + gInst.m_ReqFrame[uart].m_ReqDataLen;
			gInst.m_ReqFrame[uart].m_uCmd = eCMD_CF_SETDEVTBLCFG;
			gInst.m_ReqFrame[uart].m_uCmdTyp = NPI_MSG_SYNC_REQ;
		break;

		case eCMD_CF_SETDEVTBLCFG_2nd : //0x80

			start = 15;
			end = MAX_NUM_DEVS;
			length = end - start;
			gInst.m_ReqFrame[uart].m_ReqDataLen = (length * (COMM_DEVICE_UID_SIZE + 1)) + 1; //(Dev수*UniqID+SlotN)+nDevice 
			gInst.m_ReqFrame[uart].m_TxCmd[cnt++] = length; // nDevice(1Byte)
			// MAC ID copy
			for (i = start  ; i < end ; i++){
				MemCpy(&gInst.m_ReqFrame[uart].m_TxCmd[cnt], &zMACCfgNODE_ID[i],  COMM_DEVICE_UID_SIZE );
				gInst.m_ReqFrame[uart].m_TxCmd[cnt+8] = i;
				cnt += 9;				
			}
			gInst.m_ReqFrame[uart].m_NPILen = NPI_PROTO_LEN + gInst.m_ReqFrame[uart].m_ReqDataLen;
			gInst.m_ReqFrame[uart].m_uCmd = eCMD_CF_SETDEVTBLCFG;
			gInst.m_ReqFrame[uart].m_uCmdTyp = NPI_MSG_SYNC_REQ;
		break; 

		case eCMD_CF_STARTNETWORK : //0x42		

			gInst.m_ReqFrame[uart].m_ReqDataLen = READ_4BYTE;
			MemCpy(gInst.m_ReqFrame[uart].m_TxCmd, timeout_start_nw, gInst.m_ReqFrame[uart].m_ReqDataLen);
			gInst.m_ReqFrame[uart].m_NPILen = NPI_PROTO_LEN + gInst.m_ReqFrame[uart].m_ReqDataLen;
			gInst.m_ReqFrame[uart].m_uCmd = eCMD_CF_STARTNETWORK;
			gInst.m_ReqFrame[uart].m_uCmdTyp = NPI_MSG_SYNC_REQ;
		break;

		case eCMD_WM_NUM_WD_CONN : //0x01

			gInst.m_ReqFrame[uart].m_ReqDataLen = READ_0BYTE;
			gInst.m_ReqFrame[uart].m_NPILen = NPI_PROTO_LEN + gInst.m_ReqFrame[uart].m_ReqDataLen;
			gInst.m_ReqFrame[uart].m_uCmd = eCMD_WM_NUM_WD_CONN;
			gInst.m_ReqFrame[uart].m_uCmdTyp = NPI_MSG_SYNC_REQ;
		break;

		case eCMD_CF_SETPOWERMODE : // 0x44

			gInst.m_ReqFrame[uart].m_ReqDataLen = READ_1BYTE;
			gInst.m_ReqFrame[uart].m_TxCmd[0] = 0x01;  //MAC_PowerMode_KeepAlive
			gInst.m_ReqFrame[uart].m_NPILen = NPI_PROTO_LEN + gInst.m_ReqFrame[uart].m_ReqDataLen;
			gInst.m_ReqFrame[uart].m_uCmd = eCMD_CF_SETPOWERMODE;
			gInst.m_ReqFrame[uart].m_uCmdTyp = NPI_MSG_SYNC_REQ;
		break;

		case eCMD_ST_GETNETWORKSTATS : 
		case eCMD_ST_GETRXPACKETS :
		case eCMD_ST_GETRSSI :
			gInst.m_ReqFrame[uart].m_ReqDataLen = READ_1BYTE; 
			gInst.m_ReqFrame[uart].m_TxCmd[0] = gInst.m_cHealthNode;  
			gInst.m_ReqFrame[uart].m_NPILen = NPI_PROTO_LEN + gInst.m_ReqFrame[uart].m_ReqDataLen;
			gInst.m_ReqFrame[uart].m_uCmd = cmd;
			gInst.m_ReqFrame[uart].m_uCmdTyp = NPI_MSG_SYNC_REQ;
		break;

		case eCMD_ST_GETTXPACKETS :
		case eCMD_ST_GETTHROUGHPUT :
			gInst.m_ReqFrame[uart].m_ReqDataLen = READ_0BYTE; 			
			gInst.m_ReqFrame[uart].m_NPILen = NPI_PROTO_LEN + gInst.m_ReqFrame[uart].m_ReqDataLen;
			gInst.m_ReqFrame[uart].m_uCmd = cmd;
			gInst.m_ReqFrame[uart].m_uCmdTyp = NPI_MSG_SYNC_REQ;
		break;

		case eCMD_WD_RSSI : //0x13
			gInst.m_ReqFrame[uart].m_ReqDataLen = READ_1BYTE; 
			gInst.m_ReqFrame[uart].m_TxCmd[0] = 0xFF;  //Return RSSI for all device nodes in the network.
			gInst.m_ReqFrame[uart].m_NPILen = NPI_PROTO_LEN + gInst.m_ReqFrame[uart].m_ReqDataLen;
			gInst.m_ReqFrame[uart].m_uCmd = cmd;
			gInst.m_ReqFrame[uart].m_uCmdTyp = NPI_MSG_SYNC_REQ;
		break;
	
		default :
		break;
	}

	gInst.m_tTxRx[uart].m_cTxBuf[NPI_LENGTH_START_IDX] = NPI_MSG_SOF;
	gInst.m_tTxRx[uart].m_cTxBuf[NPI_LENGTH_LSB_IDX] = gInst.m_ReqFrame[uart].m_ReqDataLen & 0xFF;
	gInst.m_tTxRx[uart].m_cTxBuf[NPI_LENGTH_MSB_IDX] = ((gInst.m_ReqFrame[uart].m_ReqDataLen & 0xFF00) >> 8u);
	gInst.m_tTxRx[uart].m_cTxBuf[NPI_CMDTYPE_IDX] = gInst.m_ReqFrame[uart].m_uCmdTyp;
	gInst.m_tTxRx[uart].m_cTxBuf[NPI_CMDID_IDX] = gInst.m_ReqFrame[uart].m_uCmd;
	
	if(gInst.m_ReqFrame[uart].m_ReqDataLen > 0)
	{
		MemCpy(&gInst.m_tTxRx[uart].m_cTxBuf[NPI_REQDATA_IDX], &gInst.m_ReqFrame[uart].m_TxCmd[0], gInst.m_ReqFrame[uart].m_ReqDataLen);
	}

	gInst.m_tTxRx[uart].m_cTxBuf[gInst.m_ReqFrame[uart].m_NPILen - NPI_FCS_LEN] 
	  = CmicM_CalculateFCS(&gInst.m_tTxRx[uart].m_cTxBuf[NPI_LENGTH_LSB_IDX], gInst.m_ReqFrame[uart].m_NPILen-2);

	return ret;
}

/********************************************************************************
FUNCTION :  CmicM_CalculateFCS(const uint8_t *buf, uint32 qLength)
DESCRIPTION : NPI Frame의 FCS byte작성.
**********************************************************************************/
static uint8_t CmicM_CalculateFCS(const uint8_t *buf, uint32 qLength)
{
    uint16 i;
    uint8_t fcs = 0;

    for(i = 0; i < qLength; i++) //SOF 제외하고 나머지Data의 XOR.
    {
        fcs ^= buf[i];
    }
    return fcs;
}

/********************************************************************************
FUNCTION :  CmicM_FrameCheck(const uint8_t pRxData[], uint16 nLength)
DESCRIPTION : 수신데이터의 FCS byte check. 
**********************************************************************************/
static uint8_t CmicM_FrameCheck(const uint8_t pRxData[], uint16 nLength)
{
    uint8_t rxFcs;
    uint8_t uRet = COMIF_NOT_OK;

    rxFcs = CmicM_CalculateFCS((uint8_t*) &pRxData[1], (nLength - 2)); //-2= SOF, FCS byte  

    if(rxFcs == pRxData[nLength - 1]) //compare FCS
    {
        uRet = COMIF_OK;
    }
	return uRet;
}

/********************************************************************************
FUNCTION :  CmicM_CalculateCRC16()
DESCRIPTION : BQ Frame의 CRC 2byte 작성
**********************************************************************************/
static uint16 CmicM_CalculateCRC16(const uint8_t *pData, uint16 wDataLen)
{
    uint16 wIndex;
    uint16 wCrc16 = 0xFFFFu;

    for (wIndex = 0u; wIndex < wDataLen; wIndex++)
    {
        wCrc16 ^= (uint16) (pData[wIndex]);
        wCrc16 = zCrc16Lkup[wCrc16 & ((uint16) 0x00FF)] ^ (wCrc16 >> 8);
    }

    return wCrc16;
}

/********************************************************************************
FUNCTION :  CmicM_MakeNPIFrameHeader(eUART uart, uint16 length)
DESCRIPTION : NPI Frame Header (SOF + LENGTH + CmdType + CmdID + NodeID) 작성.
**********************************************************************************/
static void CmicM_MakeNPIFrameHeader(eUART uart, uint16 length)
{
	uint8_t  aTxCmd[NPI_HEADER_SIZE];
	uint16  aCnt=0;
 // 총 더한 길이에서 노드전까지의 길이(SOF~CmdID=5)를 빼고 계산.
 	uint16  aLength = length - 5;

	aTxCmd[aCnt++] = NPI_MSG_SOF;
	aTxCmd[aCnt++] = (uint8_t)(aLength & 0x00FF);
	aTxCmd[aCnt++] = (uint8_t)((aLength >> 8 ) & 0x00FF);
	aTxCmd[aCnt++] = NPI_MSG_ASYNC_REQ;
	aTxCmd[aCnt++] = eCMD_WD_BQ_RAWFRAME;
	aTxCmd[aCnt++] = 0; //Broadcast for nodeID(=0)
	
	MemCpy(&gInst.m_tTxRx[uart].m_cTxBuf[0], &aTxCmd[0], aCnt);

	return;
}



/********************************************************************************
FUNCTION :  CmicM_MakeBQFrame(eUART uart, uint8_t init, uint16 regAddr, 
									uint8_t *pData, uint16 idx)
DESCRIPTION : BQ Frame (INIT + REG ADDR + DATA + CRC) 작성.
**********************************************************************************/
static uint16 CmicM_MakeBQFrame(eUART uart, uint8_t init, uint16 regAddr, 
									uint8_t *pData, uint16 idx)
{
	uint8_t   aTxCmd[16];
	uint8_t   aReqSiz = (init & 0x0F)+1; //하위4비트+1
	uint16  aCnt =0;
	uint16  aCrc;
	int i;

	MemSet(aTxCmd, 0, sizeof(aTxCmd));

	aTxCmd[aCnt++] = init;
	
	if ((init & 0xF0) ==  SINGLE_WRITE)
	{
		aTxCmd[aCnt++] = 0; //Dev Address
	}
	
	aTxCmd[aCnt++] = (uint8_t)(regAddr >> 8);
	aTxCmd[aCnt++] = (uint8_t)(regAddr & 0x00FF);

	
	for(i=0; i< aReqSiz ; i++){
		aTxCmd[aCnt++] = pData[i];
	}
	
	aCrc =CmicM_CalculateCRC16(aTxCmd, aCnt);
	aTxCmd[aCnt++] = (uint8_t)(aCrc & 0x00FF); //LSB
	aTxCmd[aCnt++] = (uint8_t)((aCrc >> 8 )& 0x00FF); //MSB

	MemCpy(&gInst.m_tTxRx[uart].m_cTxBuf[idx], &aTxCmd[0], aCnt);
	return (idx+aCnt);
}


static void CmicM_BQ7971x_ConfigInit(eUART uart)
{
	uint16  nIdx=0;
	uint16  nRetIdx=0;
	uint8_t   buf[10];

	// // Tx Buffer Clear.
	MemSet(&gInst.m_tTxRx[uart].m_cTxBuf, 0, sizeof(gInst.m_tTxRx[uart].m_cTxBuf));
	gInst.m_tTxRx[uart].m_nTxLen = 0;

	// //1. DevConfInit	
	MemSet(buf, 0, sizeof(buf));
	MemCpy(buf, &bqConfig.m_tNVM.uDev_Conf[0], WRITE_2BYTE);
	
	nRetIdx = CmicM_MakeBQFrame(uart, (BROADCAST_WRITE | WRITE_1BYTE), 
								BQ_DEV_CONF1_OFFSET, buf, NPI_HEADER_SIZE );	
	nIdx = nRetIdx;

	// //2. BbConfInit(uBBC_POSN)
	MemSet(buf, 0, sizeof(buf));
	MemCpy(buf, &bqConfig.m_tNVM.uBBVC_Posn[0], WRITE_3BYTE); //3bytes
	
	nRetIdx = CmicM_MakeBQFrame(uart, (BROADCAST_WRITE | WRITE_2BYTE), 
								BQ_BBVC_POSN1_OFFSET, buf, nIdx );	
	nIdx = nRetIdx;						  

	// //3. BbConfInit(UV_DISABLE)
	MemSet(buf, 0, sizeof(buf));
	MemCpy(buf, &bqConfig.m_tNVM.uUV_Disable[0], WRITE_3BYTE); //3bytes

	nRetIdx = CmicM_MakeBQFrame(uart, (BROADCAST_WRITE | WRITE_2BYTE), 
								BQ_UV_DISABLE1_OFFSET, buf, nIdx );	
	nIdx = nRetIdx;						  
	
	// //4. Control2Init
	MemSet(buf, 0, sizeof(buf));
	MemCpy(buf, &bqConfig.m_tCtrl.uControl[1], WRITE_1BYTE);
	
	nRetIdx = CmicM_MakeBQFrame(uart, (BROADCAST_WRITE ), 
								BQ_CONTROL2_OFFSET, buf, nIdx );	
	nIdx = nRetIdx;

	// //5. GpioConfInit
	MemSet(buf, 0, sizeof(buf));
	MemCpy(buf, &bqConfig.m_tNVM.uGpio_Conf[0], WRITE_6BYTE);
	
	nRetIdx = CmicM_MakeBQFrame(uart, (BROADCAST_WRITE | WRITE_5BYTE), 
								BQ_GPIO_CONF1_OFFSET, buf, nIdx );	
	nIdx = nRetIdx;

	// //6. AdcCtrlInit
	MemSet(buf, 0, sizeof(buf));
	MemCpy(buf, &bqConfig.m_tCtrl.uAdc_Ctrl[0], WRITE_4BYTE);
	
	nRetIdx = CmicM_MakeBQFrame(uart, (BROADCAST_WRITE | WRITE_3BYTE ), 
								BQ_ADC_CTRL1_OFFSET, buf, nIdx );	
	nIdx = nRetIdx;

	// //7. OvUvThreshInit
	MemSet(buf, 0, sizeof(buf));
	MemCpy(buf, &bqConfig.m_tNVM.uOVUV_Thresh[0], WRITE_2BYTE);
	
	nRetIdx = CmicM_MakeBQFrame(uart, (BROADCAST_WRITE | WRITE_1BYTE ), 
								BQ_OV_THRESH_OFFSET, buf, nIdx );	
	nIdx = nRetIdx;

	// //8. OtUtThreshInit
	MemSet(buf, 0, sizeof(buf));
	MemCpy(buf, &bqConfig.m_tNVM.uOTUT_Thresh, WRITE_1BYTE);
	
	nRetIdx = CmicM_MakeBQFrame(uart, (BROADCAST_WRITE ), 
								BQ_OTUT_THRESH_OFFSET, buf, nIdx );	

	nIdx = nRetIdx;
		
	CmicM_MakeNPIFrameHeader(uart, nIdx); 

	gInst.m_tTxRx[uart].m_cTxBuf[nIdx++] = CmicM_CalculateFCS(&gInst.m_tTxRx[uart].m_cTxBuf[1], nIdx-1);
	gInst.m_tTxRx[uart].m_nTxLen = nIdx;
	UART_SendData(uart, gInst.m_tTxRx[uart].m_cTxBuf, nIdx);

	return;
}



//Memory function
void  *MemCpy(void *dest, const void *src, uint32 count)
{
	sint8  *tmp = dest;
	const sint8 *s = src;

	while(count--){
		*tmp++ = *s++;
	}
	return dest;
}

int MemCmp(const void *ptr1, const void *ptr2, uint32 num)
{
	uint32 	i;
	sint8  *temp1;
	sint8  *temp2;

	i=0;

	temp1 = (sint8 *)ptr1;
	temp2 = (sint8 *)ptr2;

	while( i < num){
		if (temp1[i] != temp2[i])
			return (temp1[i] - temp2[i]);
		i++;	
	}
	return 0;
}

void *MemSet(void *dest, int value, uint32 count)
{
	uint32  i;
	sint8   *temp;

	temp = (sint8 *)dest;
	i=0;
	
	while(i < count) {
		temp[i++] = (sint8)value;
	}
	return temp;
}

void *MemMove(void *dest, const void *src, uint32 count)
{
        sint8 *tmp;
        const sint8 *s;

        if (dest <= src) {
                /*
                 * src 의 주소가 dest 보다 큰 값이면,
                 * src 의 처음 부터 count 만큼 dest 에 복사한다.
                 * src를 한 byte씩 복사하더라도 dest 보다 큰 위치에 있기 때문에,
                 * 오버랩되더라도 src 를 모두 정상적으로 dest에 복사할수 있다.
                 */
                tmp = dest;
                s = src;
                while (count--)
                        *tmp++ = *s++;
        } else {
                /*
                 * src의 주소가 dest 보다 작은 값이면,
                 * src 의 마지막 데이터부터 한바이트씩 dest의 마지막 바이트부터
                 * 순서대로 복사한다.
                 */
                tmp = dest;
                tmp += count;
                s = src;
                s += count;
                while (count--)
                        *--tmp = *--s;
        }
        return dest;
}
/**********************************************************************************************************************
 * static uint32 Bytes2Uint32(uint8 *pData)
 *********************************************************************************************************************/
static uint32 Bytes2Uint32(uint8 *pData)
{
    return (uint32) (((uint32) pData[0u] << 0u) | ((uint32) pData[1u] << 8u) | \
                        ((uint32) pData[2u] << 16u) | ((uint32) pData[3u] << 24u));
}

/**********************************************************************************************************************
 * static uint32 Bytes2Uint16(uint8 *pData)
 *********************************************************************************************************************/
static uint32 Bytes2Uint16(uint8 *pData)
{
    return (uint16) (((uint16) pData[0u] << 0u) | ((uint16) pData[1u] << 8u));
}



static void CmicM_CalculatePDR(uint8  node)
{
	float  RxPkt[2];
	float  TxPkt[2];

	RxPkt[UART_P] = (float)(gInst.m_tWbmsState[UART_P].m_zWdHealth[node].m_qNumRxSuccessPkt);
	RxPkt[UART_S] = (float)(gInst.m_tWbmsState[UART_S].m_zWdHealth[node].m_qNumRxSuccessPkt);

	TxPkt[UART_P] = (float)(gInst.m_tWbmsState[UART_P].m_qNumTxSuccessPacket);
	TxPkt[UART_S] = (float)(gInst.m_tWbmsState[UART_S].m_qNumTxSuccessPacket); 

	gInst.m_tWbmsState[UART_P].m_zWdHealth[node].m_fPDR = \
	  (float)((RxPkt[UART_P] / TxPkt[UART_P]) * 100.0f);
 
	gInst.m_tWbmsState[UART_S].m_zWdHealth[node].m_fPDR = \
	  (float)((RxPkt[UART_S] /TxPkt[UART_S])* 100.0f);
 
	return;
}

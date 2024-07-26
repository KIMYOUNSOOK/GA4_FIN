/*******************************************************************************
 * CmicM.h *
 *  Created on: 2024. 5. 21.
 *  Author: Suresoft
 *******************************************************************************/

#ifndef CMICM_H_
#define CMICM_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "Platform_Types.h"
#include "HAL_Uart.h"
#include "bq7971x_cfg.h"
#include "bq7971x_regs.h"

//#define  _12NODE_16CELL		TRUE
#define  _30NODE_8CELL		TRUE
/*******************************************************************************
 * Define & Enumerations
 *******************************************************************************/
#if (_30NODE_8CELL == TRUE)
	#define MAX_NUM_DEVS						(6u)//(30u)
	
	#define MAX_BMICS  							(15u)//(30u) //(12u)
	#define NUM_CELLS_ACTIVE 		            (8u)       /** Cells Count in BMICs - 18/16/12 **/
	#define NUM_GPIOS_ACTIVE        		    (4u)       /** GPIO Count in BMICs **/

	#define BQ_VCELL_START_ADDR				BQ_VCELL16_HI_OFFSET//BQ_VCELL16_HI_OFFSET//BQ_VCELL8_HI_OFFSET //VCELL 8,7,...1
	#define BQ_GPIO_START_ADDR				BQ_GPIO3_HI_OFFSET//BQ_GPIO3_HI_OFFSET //GPIO3,4,5,6
	//PartialNetworkDevice최소갯수
	#define NUM_PARTIAL_DEVS                   (MAX_NUM_DEVS/2u)

#elif (_12NODE_16CELL == TRUE)
	#define MAX_NUM_DEVS						(6u)
	#define MAX_BMICS  							(15u)//(12u)
	#define NUM_CELLS_ACTIVE 		            (16u)       /** Cells Count in BMICs - 18/16/12 **/
	#define NUM_GPIOS_ACTIVE        		     (4u)    

	#define BQ_VCELL_START_ADDR				BQ_VCELL16_HI_OFFSET
	#define BQ_GPIO_START_ADDR				BQ_GPIO3_HI_OFFSET
	//PartialNetworkDevice최소갯수
	#define NUM_PARTIAL_DEVS                         (MAX_NUM_DEVS/2u)

#endif
	#define BQ_CB_CELL_CTRL9_16_OFFSET		BQ_CB_CELL16_CTRL_OFFSET	
	#define BQ_CB_CELL_CTRL1_8_OFFSET		BQ_CB_CELL8_CTRL_OFFSET		

#define KEEP_ALIVE_INTERVAL					(0x12u)
#define COMM_DEVICE_UID_SIZE                (0x08u)
#define VCELL_CONVERT(wRawData)         (-1*(~wRawData+1)*0.000100)
/*******************************************************************************
 * Define & Enumerations
 *******************************************************************************/
/* TI WBMS Protocol  */ 
// NPI define .
#define NPI_MSG_SOF 			(0xFE)
#define NPI_MSG_ASYNC_REQ 		(0x5A)
#define NPI_MSG_SYNC_REQ		(0x3A)
#define NPI_MSG_ASYNC_RES		(0x5A)
#define NPI_MSG_SYNC_RES		(0x7A)

#define NPI_IDX_CMDTYPE			(3)
#define NPI_IDX_CMDID			(4)
#define NPI_IDX_DATA			(5)
#define NPI_HEADER_SIZE          (6)
// BQ Command define
#define BROADCAST_WRITE          (0xD0)
#define BROADCAST_READ           (0xC0)
#define SINGLE_WRITE 			 (0x90)
#define NPI_LENGTH_START_IDX						(0X00u)
#define NPI_LENGTH_LSB_IDX                          (0x01u)
#define NPI_LENGTH_MSB_IDX                          (0x02u)
#define NPI_CMDTYPE_IDX                             (0x03u)
#define NPI_CMDID_IDX                               (0x04u)
#define NPI_NODEID_IDX                              (0x05u)
#define NPI_MAC_STATUS_IDX							(0x05u)
#define NPI_REQDATA_IDX								(0x05u)	
#define NPI_BQPAYLOAD_IDX                           (0x06u)
#define NPI_BAL_STAT_IDX								(7u)
#define NPI_BALSW_STAT_IDX								(8u)
#define NPI_GPIO_IDX								(NUM_GPIOS_ACTIVE*2 + 6)
#define NPI_VCELL_IDX								(NUM_CELLS_ACTIVE*2 + 6)

#define NPI_FCS_LEN                           		(0x01u)
#define NPI_PROTO_LEN								(0x06u)

#define WRITE_1BYTE            (1u)
#define WRITE_2BYTE            (2u)
#define WRITE_3BYTE            (3u)
#define WRITE_4BYTE            (4u)
#define WRITE_5BYTE            (5u)
#define WRITE_6BYTE            (6u)
#define READ_0BYTE                                  (0u)
#define READ_1BYTE             (1u)
#define READ_2BYTE             (2u)
#define READ_3BYTE             (3u)
#define READ_4BYTE           (4u)
#define READ_5BYTE           (5u)
#define READ_6BYTE           (6u)
#define READ_7BYTE           (7u)
#define READ_8BYTE           (8u)
#define READ_11BYTE          (11u)
#define READ_13BYTE          (13u)
#define READ_16BYTE          (16u)
#define READ_21BYTE          (21u)
#define READ_35BYTE          (35u)

#define COMIF_OK                ((uint8)0x00u)
#define COMIF_NOT_OK            ((uint8)0x01u)
#define TX_CMD_MAX_LEN			(255u) //(16u)

#define TIBMS_BitSet(arg, pos)                  ((arg) |= (1 << (pos)))
#define TIBMS_BitClear(arg, pos)                ((arg) &= ~(1 << (pos)))

typedef enum{

	//WBMS Control Commands (0x00 ~ 0x13)
	eCMD_WM_IDENTIFY = 0x00, 	// identify Main(WM)node & returns the unique ID of the WM node and network ID.
	eCMD_WM_NUM_WD_CONN,		// get the number of uplink(UL) slots (the number of connected slots).
	eCMD_WM_NUM_TXPACKETS,		// get the number of successfully transmitted packets.
	eCMD_WM_NUMTXFAILED_PACKETS, // get the number of failed to transmit packets.
	eCMD_WM_TX_THROUGHPUT,		// get the TX throughput (kbps)
	eCMD_WM_RX_THROUGHPUT,		// get the RX throughput (kbps).
	eCMD_WD_NUM_RXPACKTETS,		// get the number of received packets.
	eCMD_WD_NUM_RXTIMEOUT_EVENTS, //get the number of missed reception packets
	eCMD_WD_LATENCY,			// get the latency of a node
	eCMD_WD_PER,				// get the Packet-Error-Rate (PER) of a node.
	eCMD_WD_BQ_RAWFRAME = 0x0A,	// send a raw BQ frame.
	eCMD_WD_STORAGE = 0x0D,		// send a storage command to device nodes
	eCMD_WM_RESET_DEVICE = 0x12, //  reset the wireless main device.
	eCMD_WD_RSSI,				// get the RSSI of a node.(dBm)

	//Asynchronous Callback Commands (0x20 ~ 0x2D)
	eCMD_CB_DEVICEPAIRIND = 0x20,	 //device pairing indication callback
	eCMD_CB_OAD, 					// OAD callback
	eCMD_CB_STATECHANGE, 			//state change indication callback
	eCMD_CB_TXCNF, 					// TX confirmation callback
	eCMD_CB_RXIND,					// RX indication callback
	eCMD_CB_ERROR,					// error information callback
	eCMD_CB_FORMATION, 				// network formation time callback
	eCMD_CB_FAULT, 					// fault event callback
	eCMD_CB_RESYNCCNF,				// resync confirmation callback
	eCMD_CB_RESET,					// callback when a reset occurs.
	eCMD_CB_EXITPS = 0x2A, 			// a exit power save mode callback
	eCMD_CB_DFU, 					// WBMS DFU callback
	eCMD_CB_DISCOVERIND,			// a discover indication callback
	eCMD_CB_DISCOVERCNF, 			// a discover confirmation callback

	//Statistics Commands (0x30 ~ 0x3E)
	eCMD_ST_GETFWVERSION = 0x30, 	// get firmware version command
	eCMD_ST_GETNETWORKSTATS = 0x32, // get network statistics command
	eCMD_ST_GETTXPACKETS,			// get the number of transmitted packets.
	eCMD_ST_GETTHROUGHPUT, 			// get the transmit and receive throughputs.
	eCMD_ST_GETRXPACKETS,			// get the number of RX packets.
	eCMD_ST_GETRSSI,				// get the RSSI for specific node.
	eCMD_ST_GETJOINPKTCNT,			//  get the join packet count.
	eCMD_ST_GETSTATS,				// get the WBMS statistics.
	eCMD_ST_GETKLVDURATION = 0x3D,  //get the keep alive duration
	eCMD_ST_GETPARAMS = 0x3E,		// get the WBMS parameters.

	//Network Configuration Commands (0x40 ~ 0x4F)
	eCMD_CF_SETMAINCONFIG = 0x40,	// set network main configuration
	eCMD_CF_OADREQ,					// send WBMS On-the-Air Download (OAD) request.
	eCMD_CF_STARTNETWORK,			// calls wbms_llcInit() to start the network.
	eCMD_CF_SETFREQHOPMODE,			// changes frequency hopping table to a signel channel
	eCMD_CF_SETPOWERMODE,			// enable/disable Power Save mode of the MAC.
	eCMD_CF_DWM_FALLBACK,			// This API runs the dual Wireless Main network in fallback or normal mode
	eCMD_CF_DISCOVERREQ,			// sets network operation MAC mode to discover request to search for WBMS devices.
	eCMD_CF_SETNWOPMODE,			// sets network operation mode of the MAC. (0-Volatile, 1-Persistent)
	eCMD_CF_SETJOINMODE,			// set network joining mode to Selective(1) Join or Non-Selective(0) Join.
	eCMD_CF_SETDEVTBLCFG,			// set device configuration for selective joining.
	eCMD_CF_SETDEVTBLCFG_2nd = 0x80,       // 30개노드전송을 위한 분리.
	eCMD_CF_UNPAIRREQ = 0x4A,		// used to unpair the network.
	eCMD_CF_RESYNC,					//  resynchronize the network.
	eCMD_CF_WM_REPAIRREQ,			// set MAC mode to Wireless Main repair using repair key.
	eCMD_CF_SETWKUPPRD,				//  set the periodic wake up value and go to keep alive.
	eCMD_CF_SETPARAMS,				//  set WBMS parameters.
	eCMD_CF_DFUREQ,					//  send WBMS Device Firmware Update (DFU) request for the WMain firmware update

	eCMD_MAX = 0xFF,

}WBMS_COMMAND_E;

typedef enum
{
	eMAIN_BOOT,
	eMAIN_RUN,
	eMAIN_BAL,	
	eMAIN_KEYOFF,
	eMAIN_KEYON,
	eMAIN_DFU,
}MAIN_STATE_E;

typedef enum
{
	eBOOT_IDLE,
	eBOOT_NETWORK,
	eBOOT_BQREGISTER,

}BOOT_STATE_E;

typedef enum
{
	eNETWORK_ST0_IDLE,
	eNETWORK_ST0_IDLE_RES,  //2ms DelayCheck
	eNETWORK_ST1_REQ, //SETNWKOPMODE(0x47) - Secondary
	eNETWORK_ST1_RES,
	eNETWORK_ST2_REQ, //SETNWKOPMODE(0x47) - Primary
	eNETWORK_ST2_RES,
	eNETWORK_ST3_REQ, //SETPARAMS
	eNETWORK_ST3_RES,	
	eNETWORK_ST4_REQ, //SETMAINCONFIG
	eNETWORK_ST4_RES,
	eNETWORK_ST5_REQ, //SET JOINMODE
	eNETWORK_ST5_RES,
	eNETWORK_ST6_REQ, //SETDEVTBLCFG
	eNETWORK_ST6_RES,
// MacID 30개처리를 위해 별도상태처리
	eNETWORK_ST6_1_REQ, //SETDEVTBLCFG -1
	eNETWORK_ST6_1_RES,
	
	eNETWORK_ST7_REQ, //START NETWORK
	eNETWORK_ST7_RES,
	eNETWORK_ST8_REQ, //NUMOF CONN
	eNETWORK_ST8_RES,
	eNETWORK__END,
}BOOTSUB_NETWORK_E;

typedef enum
{
	eBQREG_ST0_IDLE,
	eBQREG_ST1_REQ, /* BQ ConfigInit */
	eBQREG_ST1_RES, 
	eBQREG_ST2_REQ, /* Diag StartupInit*/
	eBQREG_ST2_RES,
	eBQREG_ST3_REQ,  
	eBQREG_ST3_RES,
	eBQREG_ST4_REQ,  
	eBQREG_ST4_RES,
	eBQREG_ST5_REQ,  
	eBQREG_ST5_RES,
	eBQREG_ST6_REQ,
	eBQREG_ST6_WAIT,  
	eBQREG_ST6_RES, 	
	eBQREG_ST7_REQ,
	eBQREG_ST7_WAIT,  
	eBQREG_ST7_RES,  

	eBQREG__END,
}BOOTSUB_BQREGISTER_E;

typedef enum
{
	eRUN_IDLE,
	eRUN_SETCELL_MONITOR,
	eRUN_CELL_MONITOR,
	eRUN_DIAG_FDTI_ROUTINE,
	eRUN_HEALTH_MONITOR,	
}RUN_STATE_E;

typedef enum
{
	eBAL_IDLE,
	eBAL_SETBALANCE_MONITOR,
	eBAL_BALANCE_MONITOR,
	eBAL_DIAG_FDTI_ROUTINE,
	eBAL_HEALTH_MONITOR,	
}BAL_STATE_E;


typedef enum
{
	eSETCELL_ST0_IDLE,
	eSETCELL_ST1_REQ,
	eSETCELL_ST1_RES,
	eSETCELL_ST2_REQ,
	eSETCELL_ST2_RES,
	eSETCELL__END,
}RUNSUB_SETCELL_MONITOR_E;

typedef enum
{
	eCELL_ST0_IDLE,
	eCELL_ST1_REQ,
	eCELL_ST1_WAIT,  //new added..(Polling - 10ms )
	eCELL_ST1_RES,
	eCELL__END,
}RUNSUB_CELL_MONITOR_E;

typedef enum
{
	eSETBAL_ST0_IDLE,
	eSETBAL_ST1_REQ,
	eSETBAL_ST1_RES,
	eSETBAL__END,
}BALSUB_SETBAL_MONITOR_E;

typedef enum
{
	eBAL_ST0_IDLE,
	eBAL_ST1_REQ,
	eBAL_ST1_WAIT,
	eBAL_ST1_RES,
	eBAL_ST2_REQ,
	eBAL_ST2_RES,

	eBAL__END,
}BALSUB_BAL_MONITOR_E;

typedef enum
{
	eKEYOFF_ST0_IDLE,
	eKEYOFF_ST1_REQ,
	eKEYOFF_ST1_RES,
	eKEYOFF_ST2_REQ,
	eKEYOFF_ST2_RES,
	eKEYOFF__END,
}KEYOFF_E;

typedef enum
{
	eKEYON_ST0_IDLE,
	eKEYON_ST1_REQ,
	eKEYON_ST1_RES,
	eKEYON_ST2_REQ,
	eKEYON_ST2_RES,
	eKEYON__END,
}KEYON_E;

typedef enum
{
	eHEALTH_ST0_IDLE,
	eHEALTH_ST1_REQ,
	eHEALTH_ST1_RES,
	eHEALTH_ST2_REQ,
	eHEALTH_ST2_RES,
	eHEALTH_ST3_REQ,
	eHEALTH_ST3_RES,
	eHEALTH_ST4_REQ,
	eHEALTH_ST4_RES,
	eHEALTH_ST5_REQ,
	eHEALTH_ST5_RES,
	eHEALTH_ST6_REQ,
	eHEALTH_ST6_RES,	
}HEALTH_MONITOR_E;

typedef enum
{
	ePOLL_Xms_IDLE,		//No Request
	ePOLL_Xms_REQUEST,	//1ms에서의 Poll Request
	ePOLL_Xms_RUNNING, 	//Polling 동작중
	ePOLL_Xms_RECEIVE, 	//Receive완료
	ePOLL_Xms_NG, 		//Fail
	ePOLL_Xms_END, 
}POLL_XMS_E;

typedef struct
{
	BOOTSUB_NETWORK_E   		m_eNetwork;   //Network Initialize
	BOOTSUB_BQREGISTER_E 		m_eBQRegister;	//BQ Register Initialize
}BOOTSUB_State_t;

typedef struct 
{
	RUNSUB_SETCELL_MONITOR_E   	m_eSetCellMonitor;
	RUNSUB_CELL_MONITOR_E   	m_eCellMonitor;
}RUNSUB_State_t;

typedef struct
{
	BALSUB_SETBAL_MONITOR_E    	m_eSetBalMonitor;
	BALSUB_BAL_MONITOR_E    	m_eBalMonitor;
}BALSUB_State_t;

typedef struct
{
	MAIN_STATE_E				m_eMain;
	MAIN_STATE_E				m_ePriorMain;
	  
	BOOT_STATE_E 				m_eBoot;
	BOOTSUB_State_t 			m_BootSub;

	RUN_STATE_E					m_eRun;	
	RUNSUB_State_t				m_RunSub;
	
	BAL_STATE_E					m_eBal;
	BALSUB_State_t				m_BalSub;

	KEYOFF_E					m_eKeyOff;
	KEYON_E						m_eKeyOn;
	HEALTH_MONITOR_E			m_eHealthMonitor;

	POLL_XMS_E					m_ePollStatus; //10ms check status.

	uint8 						m_eTickEvent;
}CmicM_State_t;


typedef struct 
{
	uint16  	m_nTxLen;
	uint8_t		m_cTxBuf[128];
	uint16		m_nRxLen;
	uint8_t		m_cRxBuf[512];
	uint8_t     m_cIdx; //new added..from uart index.	
}TxRxCmd_t;

typedef struct CmicM_RespFrame_Tag
{
	uint16 		m_nLength;
	uint8		m_uCmdType;
	uint8 		m_uCmdID;
	uint8 		m_uNodeID;
	uint8		m_uBQ_INIT;
	uint8 		m_uBQ_DEV_ADDR; //added ..
	uint16      m_nBQ_REG_ADDR;
	uint8 		m_uBQ_DATA[128];
	uint32      m_wTimeStamp;
}CmicM_RespFrame;

typedef struct CmicM_ReqFrame_Tag
{
    uint8 		m_TxCmd[TX_CMD_MAX_LEN];
    uint16 		m_ReqDataLen;
	uint16 		m_NPILen;

    uint8 		m_uCmd;
    uint8 		m_uCmdTyp;
	uint16 		m_nBQ_REG_ADDR;
}
CmicM_ReqFrame;

//CB_RESET 시 수신데이터
typedef struct WbmsAppWMainInfoType_Tag
{
	uint32   	m_qFWVersion;
	uint8	 	m_uWMainUID[COMM_DEVICE_UID_SIZE];
}WbmsAppWMainInfo;

typedef enum{
	eMAC_Device_Paired=0,
	eMAC_Device_SyncLost,
	eMAC_Device_Synced,
	eMAC_Device_KeyFallback,
	eMAC_Device_Scanned,
	eMAC_Device_UnPaired,
	eMAC_Device_ResyncTimeout,
}MAC_DeviceStateEvent;

typedef struct WbmsAppWdPairStateType_Tag
{
	uint8 		m_uSlotInfo;
    uint16 		m_uNetworkId;
    uint8_t	 	m_zDevUniqueID[COMM_DEVICE_UID_SIZE];
    MAC_DeviceStateEvent  m_MAC_DeviceState;
}
WbmsAppWdPairState;

typedef struct WbmsAppWdRssiType_Tag
{
    uint8 		m_uNodeId;
    uint8 		m_uChannel;
    sint8 		m_xULRSSIValue;
    sint8 		m_xDLRSSIValue;
}
WbmsAppWdRssi;

typedef struct WbmsAppWdNwStatsType_Tag
{
    float 		m_fLatency;
	float       m_fPDR;

	uint32 		m_qNumRxSuccessPkt; //Number of successfully received packets
	uint32 		m_qNumRxMissedPkt; //Number of failed(missed) receive packets
//	uint8		m_uRSSI;
}
WbmsAppWdNetworkState;

// typedef struct WbmsAppWdStorageModeType_Tag
// {
//     uint32 		m_qStrTimeStamp;
//     uint8 		m_uStorageRsp;
//     uint8 		m_uStorageMode;
//     uint8 		m_uStorageStatus;
//     uint8 		m_uNodeId;
// }
// WbmsAppWdStorageMode;

typedef struct Bmi_CbRequestType_Tag
{
    uint8 uCbTime;
}
Bmi_CbRequestType;

typedef struct WbmsAppStatsType_Tag
{

	WbmsAppWdPairState		m_zWdPairState[MAX_BMICS];
    WbmsAppWdRssi 			m_zWdRSSI[MAX_BMICS];
    WbmsAppWdNetworkState 	m_zWdHealth[MAX_BMICS];
   
   // WbmsAppWdState	 		m_zDeviceState[MAX_BMICS];
   // WbmsAppWdStorageMode 	m_zWdStoragMode[MAX_BMICS];

	uint16		m_uNumOfConn; //연결된 노드개수정보저장
    // uint32 		m_qGetNumOfRXPackets[MAX_BMICS];
    // uint32 		m_qGetNumOfRXTimeout[MAX_BMICS];
    // uint32 		m_qGetLatencyOfNode[MAX_BMICS];
    // uint32 		m_qGetPEROfNode[MAX_BMICS];

    // uint32 		m_qGetNumOfTXPackets;
    // uint32 		m_qGetTXFailedPackets;
    // uint32 		m_qGetTXThroughput;
    // uint32 		m_qGetRXThroughput;

    uint32 		m_qNumTxSuccessPacket; //0x33(GETTXPACKETS)
    uint32 		m_qNumTxFailedPackets; //0x33(GETTXPACKETS)
    uint32 		m_qTXthroughput; //0x34(GETHROUGHPUT)
    uint32 		m_qRXthroughput; //0x34(GETHROUGHPUT)
    
  
}
WbmsAppStateType;

typedef struct
{
	uint32              m_executeCnt;
	uint32				m_timeP0;
	uint32				m_timeP1;
	float				m_duration;	
}PACKET_TIME;

typedef struct WbmsAppInfoType_Tag
{
	PACKET_TIME		m_cellTime; //TxSend ~ Rx Receive at cellsensing
	PACKET_TIME		m_10msTime; //TxSend ~ 10ms Polling start.
	PACKET_TIME     m_healthTime;	
	PACKET_TIME		m_igOnTime;
	PACKET_TIME		m_balTime;

    // Bmi_CbRequestType 	m_zCbCtrlReq[MAX_BMICS][NUM_CELLS_ACTIVE];

    uint32 			m_qNwFormTime_P; //Primary Network Formation Time
    uint32 			m_qNwFormTime_S; //Secondary Network Formation Time

    // uint32 		m_qBmiDriverInitStamp;
    // uint32 		m_qBmiDriverStartupStamp;
    // uint32 		m_qBmiDriverDiagStamp;

    // uint32 		m_nDiagFailed;
    // uint32 		m_nDiagStartupFailed;
    // uint32 		m_nGpioMonitorFail;
    // uint32 		m_nCellMonitorReqFail;
    // uint32 		m_qInitWaitTimer;

}
WbmsAppInfoType;

typedef struct WbmsFdtiResultType_Tag
{
    uint16 m_zSM_VCELL_PLAU[MAX_NUM_DEVS];                  /* CELL Voltage Plausibility, 0 for valid, 1 for invalid */
    uint16 m_zSM_VC_OPN_DET[MAX_NUM_DEVS];                  /* SM_VC_OPN_DET Status */
    // uint16 m_zSM_VCCB_SHRT;                   /* SM_VCCB_SHRT Status */
    // uint16 m_zSM_CABLE_RES_AB;                /* CABLE Resistance */
    // uint16 m_zSM_NFAULT;                      /* NFAULT*/

    // uint16 m_zSM_ACOMP_VCELL;                 /* SM_ACOMP CELL Analog comparison Fault Injection */
    // uint16 m_zSM_ACOMP_GPIO;                  /* SM_ACOMP GPIO Analog comparison Fault Injection */
    // uint16 m_zSM_DCOMP;                       /* SM_DCOMP Digital comparison Fault Injection */

    // uint16 m_zSM_GPIO_PLAU;                   /* GPIO Plausibility, 0 for valid, 1 for invalid */
    // uint16 m_zSM_GPIO_OPNWR[MAX_NUM_DEVS];                  /* SM_GPIO_OPNWR, GPIO Open Wire detection */
    // uint16 m_zSM_GPIO_ADJSHRT;                /* SM_GPIO_ADJSHRT, GPIO Adjacent Short diagnostic */
    // uint16 m_zGpioPullSel;                    /* GPIO Voltage PULL SEL Configuration */

    // uint16 m_zDieTemp1;                       /* Die Temp1 read value as PCB temp */
    // uint16 m_zDieTemp2;                       /* Die Temp2 read value  */
    // uint16 m_zDieTempPrev1;                   /* Die Temp1 Prev read value as PCB temp */
    // uint16 m_zDieTempPrev2;                   /* Die Temp2 Prev read value  */

    // uint32 m_zDieTemp1Stuck:8;                /* Die Temp1 and Temp2 are same */
    // uint32 m_zDieTemp2Stuck:8;                /* Die Temp1 and Temp2 are same */
    // uint32 m_zSM_DIE_TEMP1:1;                 /* SM_DIE_TEMP1 */
    // uint32 m_zSM_DIE_TEMP2:1;                 /* SM_DIE_TEMP2 */
    // uint32 m_zSM_DIE_TEMP_DIFF:1;             /* SM_DIE_TEMP_DIFF */
    // uint32 m_zRsvd:5;                         /* Rsvd */

    // uint32 m_zSM_FAULT_SUMMARY:8;             /* SM_FAULT_SUMMARY */
}
WbmsFdtiResultType;

typedef struct WbmsAppDataType_Tag
{
    // uint16 		m_uTotalBmics;
    // uint16 		m_uDmRdwnWaitTimer;

    // uint8 		m_uFaultSummary[MAX_BMICS];
    // uint8 		m_uFaultDetails[MAX_BMICS][16u];

    float 		m_zVoltage[MAX_BMICS][NUM_CELLS_ACTIVE];
    uint16 		m_zVoltageAdc[MAX_BMICS][NUM_CELLS_ACTIVE];
	uint16		m_zPriorVoltage[MAX_BMICS][NUM_CELLS_ACTIVE];
    float 		m_zTempAdc[MAX_BMICS][NUM_GPIOS_ACTIVE];

    uint8 		m_uBalTime[MAX_BMICS][NUM_CELLS_ACTIVE];
    uint8 		m_uBalDoneStat[MAX_BMICS][3];
    uint8 		m_uBalSWStat[MAX_BMICS][3];
    uint8 		m_uBalStat[MAX_BMICS];
}
WbmsAppDataType;

typedef struct WbmsRawDataType_Tag
{
	uint32 		m_uVoltageTimeStamp[MAX_BMICS];
	uint8 		m_uVoltage[MAX_BMICS][NUM_CELLS_ACTIVE*2];
	uint32 		m_uGpioTimeStamp[MAX_BMICS];
	uint8 		m_uGpio[MAX_BMICS][NUM_GPIOS_ACTIVE*2];	
}WbmsRawDataType;


////////////////////////////////////////////////////////////////
// Config 
typedef struct DeviceIdCfgType_Tag
{
	uint8 		m_zDevUniqueID[COMM_DEVICE_UID_SIZE];
    uint8 		m_eSlotN;

}MAC_DevTblCfg;


void CmicM_Init(void);
void CmicM_IG_Init(void);
void CmicM_InitHandler(void);
void CmicM_Handler(void);
void CmicM_Handler_10ms(void);

extern void  *MemCpy(void *dest, const void *src, uint32 count);
extern int MemCmp(const void *ptr1, const void *ptr2, uint32 num);
extern void *MemSet(void *dest, int value, uint32 count);
extern void *MemMove(void *dest, const void *src, uint32 count);

//HAL_Ticker
void    	TickerInit(void);
uint32_t  	TickerGetTimestamp(void);
uint32_t   	GetTick_1ms(void);
uint32_t   GetTick_10ms(void);

//HAL_Tmr 
typedef void (*HAL_timer_callback_t)(void);
void  Tmr_Init(void);
uint8  Tmr_Start(HAL_timer_callback_t);
void  Tmr_Stop(void);


#endif /* ICCU_DAVINCI_APPL_INCLUDE_INCLUDE_CMICM_H_ */

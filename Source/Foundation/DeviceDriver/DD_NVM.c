// File Name: DD_NVM.c
// Description:
// Engineer:  Tanabe
// Last Edit: 15.01.xx
// Revision:  000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+--------------------------------------------------------------------------------------
// 15.01.xx | Tanabe              | 新規作成


#pragma interrupt INTIICA1 I2C_MST_INT

#include "Common.h"
#include "DD_Common.h"
#include "DD_Event.h"
#include "DD_NVM.h"
#include "LIB_StopWatch.h"


#define	DD_NVM_WRITE_MAX_SIZE			32			// Write Max Size

#define DD_NVM_WRITE_END_TIME			6			// Write End Time[ms]
#define DD_NVM_START_SAFTY_TIME			2			// Start Safty Time[ms] ※StartConditionセットアップ時間0.6us
#define DD_NVM_STOP_SAFTY_TIME			2			// Stop Safty Time[ms] ※StopConditionセットアップ時間0.6us

#define DD_NVM_SLAVE_ADDRESS_RESET		0xFF		// Slave Address + Reset指定
#define DD_NVM_SLAVE_ADDRESS_READ		0xA1		// Slave Address + Read指定
#define DD_NVM_SLAVE_ADDRESS_WRITE		0xA0		// Slave Address + Write指定


// ◆ typdef
typedef struct {
	US dataCnt;
	US dataLen;
	UC* ramAddress;
	US nvmAddress;
	void (*callBack)(SS result);
}DD_NVM_CTRL_DATA;

typedef enum{
	DD_CTRL_DATA,
	DD_CTRL_DATA_TEMP,
	DD_CTRL_DATA_NUM
}EctrlDataID;

typedef enum {
	DD_NVM_ADDRESS_1ST,
	DD_NVM_ADDRESS_2ND,
	DD_NVM_ADDRESS_NUM
}EnvmAddressID;

typedef enum {
	/* 共通State */
	stInit_ID,
	stReset_Start_ID,
	stReset_Stop_ID,
	stIdle_ID,
	// これより上にStateを追加する場合はState遷移を要見直し
	stBusLevelErr_ID,
	stStartErr_ID,
	stAckErr_ID,
	stWriteErr_ID,
	/* Read処理時のState */
	stRead_Start_ID,
	stRead_SendAddress1st_ID,
	stRead_SendAddress2nd_ID,
	stRead_Restart_ID,
	stRead_ReceiveDataStart_ID,
	stRead_ReceiveData_ID,
	stRead_Stop_ID,
	/* Write処理時のState */
	stWrite_Start_ID,
	stWrite_SendAddress1st_ID,
	stWrite_SendAddress2nd_ID,
	stWrite_SendData1st_ID,
	stWrite_SendData_ID,
	stWrite_WaitWriteEnd_ID,
	stWrite_CheckRemainData_ID,
	stWrite_Stop_ID
}EnvmStateID;


// ◆ Prototype(Used Only in DD_NVM.c)

/* 共通処理 */
__interrupt void I2C_MST_INT(void);
static void setNvmCtrlTempData(US area, UC *address, US size, void (*func)(SS result));
static void setNvmAddress(EnvmAddressID addressId);
static void createStartCondition(UC slaveAddress);
static void createStopCondition(void);
static bool checkReceiveAck(void);
static void nvmBusyResponce(void);

static void stReset_Start(void);
static void stReset_Stop(void);
static void stIdle(void);
static void stBusLevelErr(EctrlDataID dataId);
static void stStartErr(EctrlDataID dataId);
static void stAckErr(EctrlDataID dataId);
static void stWriteErr(EctrlDataID dataId);

/* Read処理 */
static void	stRead_Start(void);
static void	stRead_SendAddress1st(void);
static void stRead_SendAddress2nd(void);
static void stRead_Restart(void);
static void stRead_ReceiveDataStart(void);
static void	stRead_ReceiveData(void);
static void stRead_Stop(void);

/* Write処理 */
static void stWrite_Start(EctrlDataID dataId);
static void stWrite_SendAddress1st(void);
static void	stWrite_SendAddress2nd(void);
static void stWrite_SendData1st(void);
static void	stWrite_SendData(void);
static void stWrite_WaitWriteEnd(void);
static void stWrite_CheckRemainData(void);
static void stWrite_Stop(void);


// ◆ Grobal Parameter
static EnvmStateID g_state = stInit_ID;					// State ID
DD_NVM_CTRL_DATA g_ctrlData[DD_CTRL_DATA_NUM];			// NVM制御Data


//============================================================================= 
// Function Name : DD_NVM_Init
// Description   : I2C通信の初期設定を行う。
// Parameter     : なし
// Return        : なし
// Note          : なし
//=============================================================================
void DD_NVM_Init( void )
{
	/* 初期化 */
	g_state = stInit_ID;
	g_ctrlData[DD_CTRL_DATA].callBack = NULL;
	g_ctrlData[DD_CTRL_DATA_TEMP].callBack = NULL;
	// その他のデータはRead/Write要求時に設定
	
	/* レジスタ設定 */
	IICA1EN = 1U;			// クロック許可
	IICE1 = 0U;				// IICAの動作停止
	IICAMK1 = 1U;			// 割り込み禁止
	IICAIF1 = 0U;			// IICA割り込み要求フラグをクリア
	IICAPR11 = 1U;			// IICAの割り込み優先順位をレベル3に設定
	IICAPR01 = 1U;
	
	SMC1 = 1U;				// ファースト・モード(最大転送レート：400kbps)
	DFC1 = 1U;				// フィルタ動作ON
	PRS1 = 1U;				// 動作クロック分周(fCLK/2)
	IICWL1 = 0x2A;			// IICA Low Level幅設定(転送クロック設定400kbps)
	IICWH1 = 0x1F;			// IICA Hi Level幅設定(転送クロック設定400kbps)	
	//SVA1					// Slave Address設定(マスタとして使用する為、設定不要)
	STCEN1 = 1U;			// Stop Conditionを検出せずにStart Condition生成許可
	IICRSV1 = 1U;			// 通信予約機能禁止設定
	SPIE1 = 0U;				// Stop Condition検出による割り込み禁止
	WTIM1 = 1U;				// 9Clock出力後に割り込み
	ACKE1 = 1U;				// ACK許可
	IICAMK1 = 0U;			// 割り込み許可
	IICE1 = 1U;				// IICAの動作許可
	LREL1 = 1U;				// 待機状態
	
	P6.2 = 0U;				// 出力設定
	P6.3 = 0U;				// 出力設定
	PM6.2 = 0U;				// P62出力モード設定
	PM6.3 = 0U;				// P63出力モード設定
	
	/* Reset Stateへ遷移 */
	DD_EVT_Put(evGoNext_ID, evuDD_NVM_ID, 0, 0);
}

//============================================================================= 
// Function Name : DD_NVM_Load
// Description   : NVM Read要求受付。I2C通信を開始する。
// Parameter     : area		Read Area
//				   address	Read Address
//				   size		Read Size
//				   func		Call back関数
// Return        : なし
// Note          : Call back関数にて完了を通知する
//=============================================================================
void DD_NVM_Load( US area, UC *address, US size, void (*func)(SS result) )
{
	setNvmCtrlTempData(area, address, size, func);	// Temporaryに制御データを保持
	DD_EVT_Put(evRead_ID, evuDD_NVM_ID, 0, 0);
}

//=============================================================================
// Function Name : DD_NVM_Save
// Description   : NVM Write要求受付。I2C通信を開始する。
// Parameter     : area		Write Area
//				   address	Write Address
//				   size		Write Size
//				   func		Call back関数
// Return        : なし
// Note          : Call back関数にて完了を通知する
//=============================================================================
void DD_NVM_Save( US area, UC *address, US size, void (*func)(SS result) )
{
	setNvmCtrlTempData(area, address, size, func);	// Temporaryに制御データを保持
	DD_EVT_Put(evWrite_ID, evuDD_NVM_ID, 0, 0);
}

//=============================================================================
// Function Name : I2C_MST_INT
// Description   : I2C 割り込み処理
// Parameter     : なし
// Return        : なし
// Note          : なし
//=============================================================================
__interrupt void I2C_MST_INT(void)
{
    UC rx_data;
	
    rx_data = IICA1;
	
	DD_EVT_Put(evGoNext_ID, evuDD_NVM_ID, (UC*)rx_data, 1);	
	
	IICAIF1 = 0U;		// 割り込みフラグをクリア
}

//=============================================================================
// Function Name : setNvmCtrlTempData
// Description   : TemporaryのNVM制御データを設定する
// Parameter     : area		Read Area、address	Read Address
//				   size		Read Size、func		Call back関数
// Return        : なし
// Note          : なし
//=============================================================================
static void setNvmCtrlTempData( US area, UC *address, US size, void (*func)(SS result) )
{
	g_ctrlData[DD_CTRL_DATA_TEMP].ramAddress = address;				// RAM Address
	g_ctrlData[DD_CTRL_DATA_TEMP].dataLen = size;					// Read/Write予定Byte数
	g_ctrlData[DD_CTRL_DATA_TEMP].callBack = func;				// Call back関数
	g_ctrlData[DD_CTRL_DATA_TEMP].dataCnt = 0;						// Read/Write済みByte数
	
	/* Read/Write NVM Addressの設定 */
	switch (area) {
	case DD_NVM_AREA_KEY:	// Key Area
		g_ctrlData[DD_CTRL_DATA_TEMP].nvmAddress = DD_ADDR_KEYAREA;
		break;
	case DD_NVM_AREA_NVM:	// NVM Area
		g_ctrlData[DD_CTRL_DATA_TEMP].nvmAddress = DD_ADDR_NVMAREA;
		break;
	case DD_NVM_AREA_HFSI:	// HFSI Area
		g_ctrlData[DD_CTRL_DATA_TEMP].nvmAddress = DD_ADDR_HFSIAREA;
		break;
	case DD_NVM_AREA_TEMP:	// Temporary Area
		g_ctrlData[DD_CTRL_DATA_TEMP].nvmAddress = DD_ADDR_TEMPAREA;
		break;		
	default:
		break;
	}
}

//=============================================================================
// Function Name : setNvmAddress
// Description   : NVMアドレスを設定する
// Parameter     : address	Address
// Return        : なし
// Note          : なし
//=============================================================================
static void setNvmAddress( EnvmAddressID addressId )
{
	if ( addressId == DD_NVM_ADDRESS_1ST ) {
		IICA1 = (UC)(g_ctrlData[DD_CTRL_DATA].nvmAddress >> 8);
	}
	else {
		IICA1 = (UC)g_ctrlData[DD_CTRL_DATA].nvmAddress;
	}
}

//=============================================================================
// Function Name : createStartCondition
// Description   : Start Condition生成、Slave Addressを設定する送信。
// Parameter     : slaveAddress スレーブアドレス
// Return        : なし
// Note          : ※StartCondition発行待ちの間(最大1ms)処理を占有する。
//=============================================================================
static void createStartCondition(UC slaveAddress)
{
	CStopWatch stopWatch;
	UL time;
	
	CStopWatch_CStopWatch(&(stopWatch));	// ストップウォッチ初期化

	STT1 = 1U;								// Start Condition送信
	
	CStopWatch_Start(&(stopWatch));
	
	while (1) {
		/* Start Conditionを検出した場合 */
		if ( STD1 == 1U ) {
			IICA1 = slaveAddress;
			break;
		}
		/* Start Condition検出しなかった場合 */
		else if ( STCF1 == 1U ) {
			DD_EVT_Put(evGoFail_ID, evuDD_NVM_ID, 0, 0);
			break;
		}
		/* いずれのレジスタもセットされなかった場合 */
		else {
			CStopWatch_Split(&stopWatch, &time);	// 経過時間取得
			if ( time >= DD_NVM_START_SAFTY_TIME ) {
				DD_EVT_Put(evGoFail_ID, evuDD_NVM_ID, 0, 0);
				break;
			}
		}
	}
	
	CStopWatch_Stop(&stopWatch, &time);
}

//=============================================================================
// Function Name : createStopCondition
// Description   : Stop Condition生成
// Parameter     : なし
// Return        : なし
// Note          : ※StopCondition発行待ちの間(最大1ms)処理を占有する。
//=============================================================================
static void createStopCondition(void)
{
	CStopWatch stopWatch;
	UL time;
	
	CStopWatch_CStopWatch(&(stopWatch));	// ストップウォッチ初期化

	SPT1 = 1U;								// Stop Condition送信
	
	CStopWatch_Start(&(stopWatch));
	
	while (1) {
		if ( SPD1 == 1U ) {								// Stop Conditionを検出した場合
			break;
		}
		else {
			CStopWatch_Split(&stopWatch, &time);
			// Safty時間経過した場合も処理を占有し続けないようループを抜ける
			if ( time >= DD_NVM_STOP_SAFTY_TIME ) {
				break;
			}
		}
	}
	
	CStopWatch_Stop(&stopWatch, &time);
}

//=============================================================================
// Function Name : checkReceiveAck
// Description   : Ack受信チェック
// Parameter     : なし
// Return        : true = Ackあり、false = Ackなし
// Note          : なし
//=============================================================================
static bool checkReceiveAck(void)
{
	if ( ACKD1 == 1U ) {				// ACKあり
		return true;
	}
	else {								// ACKなし
		return false;
	}
}

//=============================================================================
// Function Name : nvmBusyResponce
// Description   : NVM Busyを応答する
// Parameter     : なし
// Return        : なし
// Note          : なし
//=============================================================================
static void nvmBusyResponce(void)
{
	if ( g_ctrlData[DD_CTRL_DATA_TEMP].callBack != NULL ) {
		g_ctrlData[DD_CTRL_DATA_TEMP].callBack(DD_NVM_BUSY);
	}
}

//=============================================================================
// Function Name : stReset_Start
// Description   : Reset開始(Start + 9クロック + Stop)
// Parameter     : なし
// Return        : なし
// Note          : (※StopはstResetStopにて実施)
//=============================================================================
static void	stReset_Start(void)
{
	g_state = stReset_Start_ID;
	
	createStartCondition(DD_NVM_SLAVE_ADDRESS_RESET);
}

//=============================================================================
// Function Name : stReset_Stop
// Description   : Reset停止(Start + 9クロック + Stop)
// Parameter     : なし
// Return        : なし
// Note          : (※Start + 9クロックはstResetStartにて実施)
//=============================================================================
static void	stReset_Stop(void)
{
	g_state = stReset_Stop_ID;
	
	createStopCondition();
	
	if ( ( CLD1 == 1U ) && ( DAD1 == 1U ) ) {		// バス解放
		DD_EVT_Put(evGoComplete_ID, evuDD_NVM_ID, 0, 0);
	}
	else {
		DD_EVT_Put(evGoFail_ID, evuDD_NVM_ID, 0, 0);
	}
}

//=============================================================================
// Function Name : stIdle
// Description   : Idle状態
// Parameter     : なし
// Return        : なし
// Note          : なし
//=============================================================================
static void	stIdle(void)
{
	g_state = stIdle_ID;
}

//=============================================================================
// Function Name : stBusLevelErr
// Description   : Busが解放できない状態
// Parameter     : なし
// Return        : なし
// Note          : なし
//=============================================================================
static void	stBusLevelErr(EctrlDataID dataId)
{
	g_state = stBusLevelErr_ID;
	
	if ( g_ctrlData[dataId].callBack != NULL ) {
		g_ctrlData[dataId].callBack(DD_NVM_BUS_LEVEL_ERR);
	}
}

//=============================================================================
// Function Name : stStartErr
// Description   : StartConditionが発行できなかった場合のエラー状態
// Parameter     : なし
// Return        : なし
// Note          : なし
//=============================================================================
static void	stStartErr(EctrlDataID dataId)
{
	g_state = stStartErr_ID;
	
	if ( g_ctrlData[dataId].callBack != NULL ) {
		g_ctrlData[dataId].callBack(DD_NVM_START_ERR);
	}
}

//=============================================================================
// Function Name : stAckErr
// Description   : ACKなしエラー状態
// Parameter     : なし
// Return        : なし
// Note          : なし
//=============================================================================
static void	stAckErr(EctrlDataID dataId)
{
	g_state = stAckErr_ID;
	
	if ( g_ctrlData[dataId].callBack != NULL ) {
		g_ctrlData[dataId].callBack(DD_NVM_NON_ACK_ERR);
	}
}

//=============================================================================
// Function Name : stWriteErr
// Description   : Write Dataエラー発生状態
// Parameter     : なし
// Return        : なし
// Note          : なし
//=============================================================================
static void	stWriteErr(EctrlDataID dataId)
{
	g_state = stWriteErr_ID;
	
	if ( g_ctrlData[dataId].callBack != NULL ) {
		g_ctrlData[dataId].callBack(DD_NVM_WRITE_ERR);
	}
}

//=============================================================================
// Function Name : stRead_Start
// Description   : Read開始
// Parameter     : なし
// Return        : なし
// Note          : なし
//=============================================================================
static void	stRead_Start(void)
{
	g_state = stRead_Start_ID;
	
	/* Temporaryデータを制御データにコピー */
	g_ctrlData[DD_CTRL_DATA] = g_ctrlData[DD_CTRL_DATA_TEMP];
	
	/* 通信開始時はWrite指定(I2C仕様) */
	createStartCondition(DD_NVM_SLAVE_ADDRESS_WRITE);
}

//=============================================================================
// Function Name : stRead_SendAddress1st
// Description   : Read時、上位Address送信
// Parameter     : なし
// Return        : なし
// Note          : なし
//=============================================================================
static void	stRead_SendAddress1st(void)
{
	g_state = stRead_SendAddress1st_ID;

	if ( checkReceiveAck() ) {
		setNvmAddress(DD_NVM_ADDRESS_1ST);
	}
	else {
		DD_EVT_Put(evGoFail_ID, evuDD_NVM_ID, 0, 0);
	}
}

//=============================================================================
// Function Name : stRead_SendAddress2nd
// Description   : Read時、下位Address送信
// Parameter     : なし
// Return        : なし
// Note          : なし
//=============================================================================
static void	stRead_SendAddress2nd(void)
{
	g_state = stRead_SendAddress2nd_ID;

	if ( checkReceiveAck() ) {
		setNvmAddress(DD_NVM_ADDRESS_2ND);
	}
	else {
		DD_EVT_Put(evGoFail_ID, evuDD_NVM_ID, 0, 0);
	}
}

//=============================================================================
// Function Name : stRead_Restart
// Description   : Restart Condition発行、Slave Address送信
// Parameter     : なし
// Return        : なし
// Note          : なし
//=============================================================================
static void	stRead_Restart(void)
{
	g_state = stRead_Restart_ID;
	
	createStartCondition(DD_NVM_SLAVE_ADDRESS_READ);
}

//=============================================================================
// Function Name : stRead_ReceiveDataStart
// Description   : ReadData受信開始
// Parameter     : なし
// Return        : なし
// Note          : なし
//=============================================================================
static void	stRead_ReceiveDataStart(void)
{
	g_state = stRead_ReceiveDataStart_ID;
	
	if ( checkReceiveAck() ) {
		ACKE1 = 1U;				// ACK許可
		WREL1 = 1U;				// wait解除(Data受付)
	}
	else {
		DD_EVT_Put(evGoFail_ID, evuDD_NVM_ID, 0, 0);
	}
}

//=============================================================================
// Function Name : stRead_ReceiveData
// Description   : ReadData受信
// Parameter     : なし
// Return        : なし
// Note          : 1Byte単位でRead
//=============================================================================
static void	stRead_ReceiveData(void)
{
	g_state = stRead_ReceiveData_ID;
	
	/* ReadData格納&更新 */
	*(g_ctrlData[DD_CTRL_DATA].ramAddress) = IICA1;
	(g_ctrlData[DD_CTRL_DATA].ramAddress)++;
	g_ctrlData[DD_CTRL_DATA].dataCnt++;
	
	/* Read終了チェック */
	if ( g_ctrlData[DD_CTRL_DATA].dataCnt == g_ctrlData[DD_CTRL_DATA].dataLen ) {
		ACKE1 = 1U;					// ACK許可
		DD_EVT_Put(evGoComplete_ID, evuDD_NVM_ID, 0, 0);
	}
	else if ( g_ctrlData[DD_CTRL_DATA].dataCnt == (g_ctrlData[DD_CTRL_DATA].dataLen - 1) ) {
		ACKE1 = 0U;					// ACK禁止
	}
	else {
		// N/A
	}
	WREL1 = 1U;						// wait解除
}

//=============================================================================
// Function Name : stRead_Stop
// Description   : Read時、正常停止
// Parameter     : なし
// Return        : なし
// Note          : なし
//=============================================================================
static void	stRead_Stop(void)
{
	g_state = stRead_Stop_ID;
	
	createStopCondition();
	DD_EVT_Put(evGoNext_ID, evuDD_NVM_ID, 0, 0);
	
	if ( g_ctrlData[DD_CTRL_DATA].callBack != NULL ) {
		g_ctrlData[DD_CTRL_DATA].callBack(DD_NVM_OK);
	}
}

//=============================================================================
// Function Name : stWrite_Start
// Description   : Write開始
// Parameter     : なし
// Return        : なし
// Note          : なし
//=============================================================================
static void	stWrite_Start(EctrlDataID dataId)
{
	g_state = stWrite_Start_ID;
	
	/* Temporaryデータを制御データにコピー */
	g_ctrlData[DD_CTRL_DATA] = g_ctrlData[dataId];
	
	createStartCondition(DD_NVM_SLAVE_ADDRESS_WRITE);
}

//=============================================================================
// Function Name : stWrite_SendAddress1st
// Description   : Write時、上位Address送信
// Parameter     : なし
// Return        : なし
// Note          : なし
//=============================================================================
static void	stWrite_SendAddress1st(void)
{
	g_state = stWrite_SendAddress1st_ID;
	
	if ( checkReceiveAck() ) {
		setNvmAddress(DD_NVM_ADDRESS_1ST);
	}
	else {
		DD_EVT_Put(evGoFail_ID, evuDD_NVM_ID, 0, 0);
	}
}

//=============================================================================
// Function Name : stWrite_SendAddress2nd
// Description   : Write時、下位Address送信
// Parameter     : なし
// Return        : なし
// Note          : なし
//=============================================================================
static void	stWrite_SendAddress2nd(void)
{
	g_state = stWrite_SendAddress2nd_ID;
	
	if ( checkReceiveAck() ) {
		setNvmAddress(DD_NVM_ADDRESS_2ND);
	}
	else {
		DD_EVT_Put(evGoFail_ID, evuDD_NVM_ID, 0, 0);
	}
}

//=============================================================================
// Function Name : stWrite_SendData1st
// Description   : Write1stData送信開始
// Parameter     : なし
// Return        : なし
// Note          : なし
//=============================================================================
static void	stWrite_SendData1st(void)
{
	g_state = stWrite_SendData1st_ID;
	
	if ( checkReceiveAck() ) {
		IICA1 = *(g_ctrlData[DD_CTRL_DATA].ramAddress);			// Data送信(wait解除)
	}
	else {
		DD_EVT_Put(evGoFail_ID, evuDD_NVM_ID, 0, 0);
	}
}

//=============================================================================
// Function Name : stWrite_SendData
// Description   : WriteData送信
// Parameter     : なし
// Return        : なし
// Note          : 1Byte単位でWrite
//=============================================================================
static void	stWrite_SendData(void)
{
	g_state = stWrite_SendData_ID;
	
	if ( checkReceiveAck() ) {
		if ( *(g_ctrlData[DD_CTRL_DATA].ramAddress) == IICA1 ) {		// WriteDataチェック
			g_ctrlData[DD_CTRL_DATA].dataCnt++;
			(g_ctrlData[DD_CTRL_DATA].ramAddress)++;
			
			/* Write終了チェック */
			if ( ( g_ctrlData[DD_CTRL_DATA].dataCnt == g_ctrlData[DD_CTRL_DATA].dataLen ) || ( g_ctrlData[DD_CTRL_DATA].dataCnt % DD_NVM_WRITE_MAX_SIZE == 0 ) ) {
				DD_EVT_Put(evGoComplete_ID, evuDD_NVM_ID, 0, 0);
			}
			else {
				/* Data送信(wait解除) */
				IICA1 = *(g_ctrlData[DD_CTRL_DATA].ramAddress);
			}
		}
		else {
			DD_EVT_Put(evGoCancel_ID, evuDD_NVM_ID, 0, 0);
		}
	}
	else {
		DD_EVT_Put(evGoFail_ID, evuDD_NVM_ID, 0, 0);
	}
}

//=============================================================================
// Function Name : stWrite_WaitWriteEnd
// Description   : Write停止、Write完了待ち状態
// Parameter     : なし
// Return        : なし
// Note          : なし
//=============================================================================
static void	stWrite_WaitWriteEnd(void)
{
	g_state = stWrite_WaitWriteEnd_ID;
	
	createStopCondition();
	DD_EVT_SetTimer( DD_NVM_WRITE_END_TIME, evuDD_NVM_ID, tmDD_NVM_ID, 0);
}

//=============================================================================
// Function Name : stWrite_CheckRemainData
// Description   : Write Dataが残っているか確認する
// Parameter     : なし
// Return        : なし
// Note          : なし
//=============================================================================
static void	stWrite_CheckRemainData(void)
{
	g_state = stWrite_CheckRemainData_ID;
	
	if ( g_ctrlData[DD_CTRL_DATA].dataCnt == g_ctrlData[DD_CTRL_DATA].dataLen ) {				// Remain Dataなし
		DD_EVT_Put(evGoComplete_ID, evuDD_NVM_ID, 0, 0);
	}
	else {										// Remain Dataあり
		g_ctrlData[DD_CTRL_DATA].nvmAddress += DD_NVM_WRITE_MAX_SIZE;
		DD_EVT_Put(evGoStart_ID, evuDD_NVM_ID, 0, 0);
	}
}

//=============================================================================
// Function Name : stWrite_Stop
// Description   : Write時、正常停止
// Parameter     : なし
// Return        : なし
// Note          : なし
//=============================================================================
static void	stWrite_Stop(void)
{
	g_state = stWrite_Stop_ID;
	
	createStopCondition();
	DD_EVT_Put(evGoNext_ID, evuDD_NVM_ID, 0, 0);
	
	if ( g_ctrlData[DD_CTRL_DATA].callBack != NULL ) {
		g_ctrlData[DD_CTRL_DATA].callBack(DD_NVM_OK);
	}
}

//=============================================================================
// Function Name : DD_NVM_Notify
// Description   : 状態遷移
// Parameter     : eventId	"イベントID"
//               : pMsg		"メッセージData"
// Return        : なし
// Note          : なし
//=============================================================================
void DD_NVM_Notify(DD_EventID eventID, UC* pMsg)
{
	switch(eventID){

	/* Timeout */
	case evTimeout_ID:
		switch(g_state){
		case stWrite_WaitWriteEnd_ID:
			stWrite_CheckRemainData();
			break;
		default:
			break;
		}
		break;

	case evRead_ID:
		switch(g_state){
		case stInit_ID:
		case stReset_Start_ID:
		case stReset_Stop_ID:
			// Init/Reset完了を待ち合わせる
			DD_EVT_Put(evRead_ID, evuDD_NVM_ID, 0, 0);
			break;
		case stIdle_ID:
			stRead_Start();
			break;
		case stBusLevelErr_ID:
			stBusLevelErr(DD_CTRL_DATA_TEMP);
			break;
		case stStartErr_ID:
			stStartErr(DD_CTRL_DATA_TEMP);
			break;
		case stAckErr_ID:
			stAckErr(DD_CTRL_DATA_TEMP);
			break;
		case stWriteErr_ID:
			stWriteErr(DD_CTRL_DATA_TEMP);
			break;
		default:
			// NVM制御中はDD_NVM_BUSYを応答。Stateの遷移はしない
			nvmBusyResponce();
			break;
		}
		break;

	case evWrite_ID:
		switch(g_state){
		case stInit_ID:
		case stReset_Start_ID:
		case stReset_Stop_ID:
			// Init/Reset完了を待ち合わせる
			DD_EVT_Put(evWrite_ID, evuDD_NVM_ID, 0, 0);
			break;
		case stIdle_ID:
			stWrite_Start(DD_CTRL_DATA_TEMP);
			break;
		case stBusLevelErr_ID:
			stBusLevelErr(DD_CTRL_DATA_TEMP);
			break;
		case stStartErr_ID:
			stStartErr(DD_CTRL_DATA_TEMP);
			break;
		case stAckErr_ID:
			stAckErr(DD_CTRL_DATA_TEMP);
			break;
		case stWriteErr_ID:
			stWriteErr(DD_CTRL_DATA_TEMP);
			break;
		default:
			// NVM制御中はDD_NVM_BUSYを応答。Stateの遷移はしない
			nvmBusyResponce();
			break;
		}
		break;
		
	case evGoStart_ID:
		switch(g_state){
		case stWrite_CheckRemainData_ID:
			stWrite_Start(DD_CTRL_DATA);
			break;
		default:
			break;
		}
		break;
		
	case evGoNext_ID:
		switch(g_state){
 		case stInit_ID:
			stReset_Start();
			break;
		case stReset_Start_ID:
			stReset_Stop();
			break;
		/* Read時のState遷移 */
		case stRead_Start_ID:
			stRead_SendAddress1st();
			break;
		case stRead_SendAddress1st_ID:
			stRead_SendAddress2nd();
			break;
		case stRead_SendAddress2nd_ID:
			stRead_Restart();
			break;
		case stRead_Restart_ID:
			stRead_ReceiveDataStart();
			break;
		case stRead_ReceiveDataStart_ID:
		case stRead_ReceiveData_ID:
			stRead_ReceiveData();
			break;
		case stRead_Stop_ID:
			stIdle();
			break;
		/* Write時のState遷移 */
		case stWrite_Start_ID:
			stWrite_SendAddress1st();
			break;
		case stWrite_SendAddress1st_ID:
			stWrite_SendAddress2nd();
			break;
		case stWrite_SendAddress2nd_ID:
			stWrite_SendData1st();
			break;
		case stWrite_SendData1st_ID:
		case stWrite_SendData_ID:
			stWrite_SendData();
			break;
		case stWrite_Stop_ID:
			stIdle();
			break;
		default:
			break;
		}
		break;
		
	case evGoComplete_ID:
		switch(g_state){
		case stReset_Stop_ID:
			stIdle();
			break;
		case stRead_ReceiveData_ID:
			stRead_Stop();
			break;
		case stWrite_SendData_ID:
			stWrite_WaitWriteEnd();
			break;
		case stWrite_CheckRemainData_ID:
			stWrite_Stop();
			break;
		default:
			break;
		}
		break;
		
	case evGoFail_ID:
		switch(g_state){
		case stReset_Stop_ID:
			stBusLevelErr(DD_CTRL_DATA_TEMP);
			break;
		case stReset_Start_ID:
			stStartErr(DD_CTRL_DATA_TEMP);
			break;
		case stRead_Start_ID:
		case stRead_Restart_ID:
		case stWrite_Start_ID:
			stStartErr(DD_CTRL_DATA);
			break;
		case stRead_SendAddress1st_ID:
		case stRead_SendAddress2nd_ID:
		case stRead_ReceiveDataStart_ID:
		case stWrite_SendAddress1st_ID:
		case stWrite_SendAddress2nd_ID:
		case stWrite_SendData1st_ID:
		case stWrite_SendData_ID:
			stAckErr(DD_CTRL_DATA);
			break;
		default:
			break;
		}
		break;
		
	case evGoCancel_ID:
		switch(g_state){
		case stWrite_SendData_ID:
			stWriteErr(DD_CTRL_DATA);
			break;
		default:
			break;
		}
		break;
		
	default:
		break;
	}
}

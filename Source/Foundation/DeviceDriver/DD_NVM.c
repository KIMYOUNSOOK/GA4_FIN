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
// 15.01.xx | Tanabe              | �V�K�쐬


#pragma interrupt INTIICA1 I2C_MST_INT

#include "Common.h"
#include "DD_Common.h"
#include "DD_Event.h"
#include "DD_NVM.h"
#include "LIB_StopWatch.h"


#define	DD_NVM_WRITE_MAX_SIZE			32			// Write Max Size

#define DD_NVM_WRITE_END_TIME			6			// Write End Time[ms]
#define DD_NVM_START_SAFTY_TIME			2			// Start Safty Time[ms] ��StartCondition�Z�b�g�A�b�v����0.6us
#define DD_NVM_STOP_SAFTY_TIME			2			// Stop Safty Time[ms] ��StopCondition�Z�b�g�A�b�v����0.6us

#define DD_NVM_SLAVE_ADDRESS_RESET		0xFF		// Slave Address + Reset�w��
#define DD_NVM_SLAVE_ADDRESS_READ		0xA1		// Slave Address + Read�w��
#define DD_NVM_SLAVE_ADDRESS_WRITE		0xA0		// Slave Address + Write�w��


// �� typdef
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
	/* ����State */
	stInit_ID,
	stReset_Start_ID,
	stReset_Stop_ID,
	stIdle_ID,
	// ��������State��ǉ�����ꍇ��State�J�ڂ�v������
	stBusLevelErr_ID,
	stStartErr_ID,
	stAckErr_ID,
	stWriteErr_ID,
	/* Read��������State */
	stRead_Start_ID,
	stRead_SendAddress1st_ID,
	stRead_SendAddress2nd_ID,
	stRead_Restart_ID,
	stRead_ReceiveDataStart_ID,
	stRead_ReceiveData_ID,
	stRead_Stop_ID,
	/* Write��������State */
	stWrite_Start_ID,
	stWrite_SendAddress1st_ID,
	stWrite_SendAddress2nd_ID,
	stWrite_SendData1st_ID,
	stWrite_SendData_ID,
	stWrite_WaitWriteEnd_ID,
	stWrite_CheckRemainData_ID,
	stWrite_Stop_ID
}EnvmStateID;


// �� Prototype(Used Only in DD_NVM.c)

/* ���ʏ��� */
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

/* Read���� */
static void	stRead_Start(void);
static void	stRead_SendAddress1st(void);
static void stRead_SendAddress2nd(void);
static void stRead_Restart(void);
static void stRead_ReceiveDataStart(void);
static void	stRead_ReceiveData(void);
static void stRead_Stop(void);

/* Write���� */
static void stWrite_Start(EctrlDataID dataId);
static void stWrite_SendAddress1st(void);
static void	stWrite_SendAddress2nd(void);
static void stWrite_SendData1st(void);
static void	stWrite_SendData(void);
static void stWrite_WaitWriteEnd(void);
static void stWrite_CheckRemainData(void);
static void stWrite_Stop(void);


// �� Grobal Parameter
static EnvmStateID g_state = stInit_ID;					// State ID
DD_NVM_CTRL_DATA g_ctrlData[DD_CTRL_DATA_NUM];			// NVM����Data


//============================================================================= 
// Function Name : DD_NVM_Init
// Description   : I2C�ʐM�̏����ݒ���s���B
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Note          : �Ȃ�
//=============================================================================
void DD_NVM_Init( void )
{
	/* ������ */
	g_state = stInit_ID;
	g_ctrlData[DD_CTRL_DATA].callBack = NULL;
	g_ctrlData[DD_CTRL_DATA_TEMP].callBack = NULL;
	// ���̑��̃f�[�^��Read/Write�v�����ɐݒ�
	
	/* ���W�X�^�ݒ� */
	IICA1EN = 1U;			// �N���b�N����
	IICE1 = 0U;				// IICA�̓����~
	IICAMK1 = 1U;			// ���荞�݋֎~
	IICAIF1 = 0U;			// IICA���荞�ݗv���t���O���N���A
	IICAPR11 = 1U;			// IICA�̊��荞�ݗD�揇�ʂ����x��3�ɐݒ�
	IICAPR01 = 1U;
	
	SMC1 = 1U;				// �t�@�[�X�g�E���[�h(�ő�]�����[�g�F400kbps)
	DFC1 = 1U;				// �t�B���^����ON
	PRS1 = 1U;				// ����N���b�N����(fCLK/2)
	IICWL1 = 0x2A;			// IICA Low Level���ݒ�(�]���N���b�N�ݒ�400kbps)
	IICWH1 = 0x1F;			// IICA Hi Level���ݒ�(�]���N���b�N�ݒ�400kbps)	
	//SVA1					// Slave Address�ݒ�(�}�X�^�Ƃ��Ďg�p����ׁA�ݒ�s�v)
	STCEN1 = 1U;			// Stop Condition�����o������Start Condition��������
	IICRSV1 = 1U;			// �ʐM�\��@�\�֎~�ݒ�
	SPIE1 = 0U;				// Stop Condition���o�ɂ�銄�荞�݋֎~
	WTIM1 = 1U;				// 9Clock�o�͌�Ɋ��荞��
	ACKE1 = 1U;				// ACK����
	IICAMK1 = 0U;			// ���荞�݋���
	IICE1 = 1U;				// IICA�̓��싖��
	LREL1 = 1U;				// �ҋ@���
	
	P6.2 = 0U;				// �o�͐ݒ�
	P6.3 = 0U;				// �o�͐ݒ�
	PM6.2 = 0U;				// P62�o�̓��[�h�ݒ�
	PM6.3 = 0U;				// P63�o�̓��[�h�ݒ�
	
	/* Reset State�֑J�� */
	DD_EVT_Put(evGoNext_ID, evuDD_NVM_ID, 0, 0);
}

//============================================================================= 
// Function Name : DD_NVM_Load
// Description   : NVM Read�v����t�BI2C�ʐM���J�n����B
// Parameter     : area		Read Area
//				   address	Read Address
//				   size		Read Size
//				   func		Call back�֐�
// Return        : �Ȃ�
// Note          : Call back�֐��ɂĊ�����ʒm����
//=============================================================================
void DD_NVM_Load( US area, UC *address, US size, void (*func)(SS result) )
{
	setNvmCtrlTempData(area, address, size, func);	// Temporary�ɐ���f�[�^��ێ�
	DD_EVT_Put(evRead_ID, evuDD_NVM_ID, 0, 0);
}

//=============================================================================
// Function Name : DD_NVM_Save
// Description   : NVM Write�v����t�BI2C�ʐM���J�n����B
// Parameter     : area		Write Area
//				   address	Write Address
//				   size		Write Size
//				   func		Call back�֐�
// Return        : �Ȃ�
// Note          : Call back�֐��ɂĊ�����ʒm����
//=============================================================================
void DD_NVM_Save( US area, UC *address, US size, void (*func)(SS result) )
{
	setNvmCtrlTempData(area, address, size, func);	// Temporary�ɐ���f�[�^��ێ�
	DD_EVT_Put(evWrite_ID, evuDD_NVM_ID, 0, 0);
}

//=============================================================================
// Function Name : I2C_MST_INT
// Description   : I2C ���荞�ݏ���
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Note          : �Ȃ�
//=============================================================================
__interrupt void I2C_MST_INT(void)
{
    UC rx_data;
	
    rx_data = IICA1;
	
	DD_EVT_Put(evGoNext_ID, evuDD_NVM_ID, (UC*)rx_data, 1);	
	
	IICAIF1 = 0U;		// ���荞�݃t���O���N���A
}

//=============================================================================
// Function Name : setNvmCtrlTempData
// Description   : Temporary��NVM����f�[�^��ݒ肷��
// Parameter     : area		Read Area�Aaddress	Read Address
//				   size		Read Size�Afunc		Call back�֐�
// Return        : �Ȃ�
// Note          : �Ȃ�
//=============================================================================
static void setNvmCtrlTempData( US area, UC *address, US size, void (*func)(SS result) )
{
	g_ctrlData[DD_CTRL_DATA_TEMP].ramAddress = address;				// RAM Address
	g_ctrlData[DD_CTRL_DATA_TEMP].dataLen = size;					// Read/Write�\��Byte��
	g_ctrlData[DD_CTRL_DATA_TEMP].callBack = func;				// Call back�֐�
	g_ctrlData[DD_CTRL_DATA_TEMP].dataCnt = 0;						// Read/Write�ς�Byte��
	
	/* Read/Write NVM Address�̐ݒ� */
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
// Description   : NVM�A�h���X��ݒ肷��
// Parameter     : address	Address
// Return        : �Ȃ�
// Note          : �Ȃ�
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
// Description   : Start Condition�����ASlave Address��ݒ肷�鑗�M�B
// Parameter     : slaveAddress �X���[�u�A�h���X
// Return        : �Ȃ�
// Note          : ��StartCondition���s�҂��̊�(�ő�1ms)�������L����B
//=============================================================================
static void createStartCondition(UC slaveAddress)
{
	CStopWatch stopWatch;
	UL time;
	
	CStopWatch_CStopWatch(&(stopWatch));	// �X�g�b�v�E�H�b�`������

	STT1 = 1U;								// Start Condition���M
	
	CStopWatch_Start(&(stopWatch));
	
	while (1) {
		/* Start Condition�����o�����ꍇ */
		if ( STD1 == 1U ) {
			IICA1 = slaveAddress;
			break;
		}
		/* Start Condition���o���Ȃ������ꍇ */
		else if ( STCF1 == 1U ) {
			DD_EVT_Put(evGoFail_ID, evuDD_NVM_ID, 0, 0);
			break;
		}
		/* ������̃��W�X�^���Z�b�g����Ȃ������ꍇ */
		else {
			CStopWatch_Split(&stopWatch, &time);	// �o�ߎ��Ԏ擾
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
// Description   : Stop Condition����
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Note          : ��StopCondition���s�҂��̊�(�ő�1ms)�������L����B
//=============================================================================
static void createStopCondition(void)
{
	CStopWatch stopWatch;
	UL time;
	
	CStopWatch_CStopWatch(&(stopWatch));	// �X�g�b�v�E�H�b�`������

	SPT1 = 1U;								// Stop Condition���M
	
	CStopWatch_Start(&(stopWatch));
	
	while (1) {
		if ( SPD1 == 1U ) {								// Stop Condition�����o�����ꍇ
			break;
		}
		else {
			CStopWatch_Split(&stopWatch, &time);
			// Safty���Ԍo�߂����ꍇ���������L�������Ȃ��悤���[�v�𔲂���
			if ( time >= DD_NVM_STOP_SAFTY_TIME ) {
				break;
			}
		}
	}
	
	CStopWatch_Stop(&stopWatch, &time);
}

//=============================================================================
// Function Name : checkReceiveAck
// Description   : Ack��M�`�F�b�N
// Parameter     : �Ȃ�
// Return        : true = Ack����Afalse = Ack�Ȃ�
// Note          : �Ȃ�
//=============================================================================
static bool checkReceiveAck(void)
{
	if ( ACKD1 == 1U ) {				// ACK����
		return true;
	}
	else {								// ACK�Ȃ�
		return false;
	}
}

//=============================================================================
// Function Name : nvmBusyResponce
// Description   : NVM Busy����������
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Note          : �Ȃ�
//=============================================================================
static void nvmBusyResponce(void)
{
	if ( g_ctrlData[DD_CTRL_DATA_TEMP].callBack != NULL ) {
		g_ctrlData[DD_CTRL_DATA_TEMP].callBack(DD_NVM_BUSY);
	}
}

//=============================================================================
// Function Name : stReset_Start
// Description   : Reset�J�n(Start + 9�N���b�N + Stop)
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Note          : (��Stop��stResetStop�ɂĎ��{)
//=============================================================================
static void	stReset_Start(void)
{
	g_state = stReset_Start_ID;
	
	createStartCondition(DD_NVM_SLAVE_ADDRESS_RESET);
}

//=============================================================================
// Function Name : stReset_Stop
// Description   : Reset��~(Start + 9�N���b�N + Stop)
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Note          : (��Start + 9�N���b�N��stResetStart�ɂĎ��{)
//=============================================================================
static void	stReset_Stop(void)
{
	g_state = stReset_Stop_ID;
	
	createStopCondition();
	
	if ( ( CLD1 == 1U ) && ( DAD1 == 1U ) ) {		// �o�X���
		DD_EVT_Put(evGoComplete_ID, evuDD_NVM_ID, 0, 0);
	}
	else {
		DD_EVT_Put(evGoFail_ID, evuDD_NVM_ID, 0, 0);
	}
}

//=============================================================================
// Function Name : stIdle
// Description   : Idle���
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Note          : �Ȃ�
//=============================================================================
static void	stIdle(void)
{
	g_state = stIdle_ID;
}

//=============================================================================
// Function Name : stBusLevelErr
// Description   : Bus������ł��Ȃ����
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Note          : �Ȃ�
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
// Description   : StartCondition�����s�ł��Ȃ������ꍇ�̃G���[���
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Note          : �Ȃ�
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
// Description   : ACK�Ȃ��G���[���
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Note          : �Ȃ�
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
// Description   : Write Data�G���[�������
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Note          : �Ȃ�
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
// Description   : Read�J�n
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Note          : �Ȃ�
//=============================================================================
static void	stRead_Start(void)
{
	g_state = stRead_Start_ID;
	
	/* Temporary�f�[�^�𐧌�f�[�^�ɃR�s�[ */
	g_ctrlData[DD_CTRL_DATA] = g_ctrlData[DD_CTRL_DATA_TEMP];
	
	/* �ʐM�J�n����Write�w��(I2C�d�l) */
	createStartCondition(DD_NVM_SLAVE_ADDRESS_WRITE);
}

//=============================================================================
// Function Name : stRead_SendAddress1st
// Description   : Read���A���Address���M
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Note          : �Ȃ�
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
// Description   : Read���A����Address���M
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Note          : �Ȃ�
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
// Description   : Restart Condition���s�ASlave Address���M
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Note          : �Ȃ�
//=============================================================================
static void	stRead_Restart(void)
{
	g_state = stRead_Restart_ID;
	
	createStartCondition(DD_NVM_SLAVE_ADDRESS_READ);
}

//=============================================================================
// Function Name : stRead_ReceiveDataStart
// Description   : ReadData��M�J�n
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Note          : �Ȃ�
//=============================================================================
static void	stRead_ReceiveDataStart(void)
{
	g_state = stRead_ReceiveDataStart_ID;
	
	if ( checkReceiveAck() ) {
		ACKE1 = 1U;				// ACK����
		WREL1 = 1U;				// wait����(Data��t)
	}
	else {
		DD_EVT_Put(evGoFail_ID, evuDD_NVM_ID, 0, 0);
	}
}

//=============================================================================
// Function Name : stRead_ReceiveData
// Description   : ReadData��M
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Note          : 1Byte�P�ʂ�Read
//=============================================================================
static void	stRead_ReceiveData(void)
{
	g_state = stRead_ReceiveData_ID;
	
	/* ReadData�i�[&�X�V */
	*(g_ctrlData[DD_CTRL_DATA].ramAddress) = IICA1;
	(g_ctrlData[DD_CTRL_DATA].ramAddress)++;
	g_ctrlData[DD_CTRL_DATA].dataCnt++;
	
	/* Read�I���`�F�b�N */
	if ( g_ctrlData[DD_CTRL_DATA].dataCnt == g_ctrlData[DD_CTRL_DATA].dataLen ) {
		ACKE1 = 1U;					// ACK����
		DD_EVT_Put(evGoComplete_ID, evuDD_NVM_ID, 0, 0);
	}
	else if ( g_ctrlData[DD_CTRL_DATA].dataCnt == (g_ctrlData[DD_CTRL_DATA].dataLen - 1) ) {
		ACKE1 = 0U;					// ACK�֎~
	}
	else {
		// N/A
	}
	WREL1 = 1U;						// wait����
}

//=============================================================================
// Function Name : stRead_Stop
// Description   : Read���A�����~
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Note          : �Ȃ�
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
// Description   : Write�J�n
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Note          : �Ȃ�
//=============================================================================
static void	stWrite_Start(EctrlDataID dataId)
{
	g_state = stWrite_Start_ID;
	
	/* Temporary�f�[�^�𐧌�f�[�^�ɃR�s�[ */
	g_ctrlData[DD_CTRL_DATA] = g_ctrlData[dataId];
	
	createStartCondition(DD_NVM_SLAVE_ADDRESS_WRITE);
}

//=============================================================================
// Function Name : stWrite_SendAddress1st
// Description   : Write���A���Address���M
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Note          : �Ȃ�
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
// Description   : Write���A����Address���M
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Note          : �Ȃ�
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
// Description   : Write1stData���M�J�n
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Note          : �Ȃ�
//=============================================================================
static void	stWrite_SendData1st(void)
{
	g_state = stWrite_SendData1st_ID;
	
	if ( checkReceiveAck() ) {
		IICA1 = *(g_ctrlData[DD_CTRL_DATA].ramAddress);			// Data���M(wait����)
	}
	else {
		DD_EVT_Put(evGoFail_ID, evuDD_NVM_ID, 0, 0);
	}
}

//=============================================================================
// Function Name : stWrite_SendData
// Description   : WriteData���M
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Note          : 1Byte�P�ʂ�Write
//=============================================================================
static void	stWrite_SendData(void)
{
	g_state = stWrite_SendData_ID;
	
	if ( checkReceiveAck() ) {
		if ( *(g_ctrlData[DD_CTRL_DATA].ramAddress) == IICA1 ) {		// WriteData�`�F�b�N
			g_ctrlData[DD_CTRL_DATA].dataCnt++;
			(g_ctrlData[DD_CTRL_DATA].ramAddress)++;
			
			/* Write�I���`�F�b�N */
			if ( ( g_ctrlData[DD_CTRL_DATA].dataCnt == g_ctrlData[DD_CTRL_DATA].dataLen ) || ( g_ctrlData[DD_CTRL_DATA].dataCnt % DD_NVM_WRITE_MAX_SIZE == 0 ) ) {
				DD_EVT_Put(evGoComplete_ID, evuDD_NVM_ID, 0, 0);
			}
			else {
				/* Data���M(wait����) */
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
// Description   : Write��~�AWrite�����҂����
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Note          : �Ȃ�
//=============================================================================
static void	stWrite_WaitWriteEnd(void)
{
	g_state = stWrite_WaitWriteEnd_ID;
	
	createStopCondition();
	DD_EVT_SetTimer( DD_NVM_WRITE_END_TIME, evuDD_NVM_ID, tmDD_NVM_ID, 0);
}

//=============================================================================
// Function Name : stWrite_CheckRemainData
// Description   : Write Data���c���Ă��邩�m�F����
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Note          : �Ȃ�
//=============================================================================
static void	stWrite_CheckRemainData(void)
{
	g_state = stWrite_CheckRemainData_ID;
	
	if ( g_ctrlData[DD_CTRL_DATA].dataCnt == g_ctrlData[DD_CTRL_DATA].dataLen ) {				// Remain Data�Ȃ�
		DD_EVT_Put(evGoComplete_ID, evuDD_NVM_ID, 0, 0);
	}
	else {										// Remain Data����
		g_ctrlData[DD_CTRL_DATA].nvmAddress += DD_NVM_WRITE_MAX_SIZE;
		DD_EVT_Put(evGoStart_ID, evuDD_NVM_ID, 0, 0);
	}
}

//=============================================================================
// Function Name : stWrite_Stop
// Description   : Write���A�����~
// Parameter     : �Ȃ�
// Return        : �Ȃ�
// Note          : �Ȃ�
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
// Description   : ��ԑJ��
// Parameter     : eventId	"�C�x���gID"
//               : pMsg		"���b�Z�[�WData"
// Return        : �Ȃ�
// Note          : �Ȃ�
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
			// Init/Reset������҂����킹��
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
			// NVM���䒆��DD_NVM_BUSY�������BState�̑J�ڂ͂��Ȃ�
			nvmBusyResponce();
			break;
		}
		break;

	case evWrite_ID:
		switch(g_state){
		case stInit_ID:
		case stReset_Start_ID:
		case stReset_Stop_ID:
			// Init/Reset������҂����킹��
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
			// NVM���䒆��DD_NVM_BUSY�������BState�̑J�ڂ͂��Ȃ�
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
		/* Read����State�J�� */
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
		/* Write����State�J�� */
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

// File Name: DD_LOG.c
// Engineer:  Morikawa Atsushi
// Last Edit: 14.11.13
// Revision:  000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// Change History
// Date:    | Engineer:        | Note:
// ---------+------------------+-------------------------------------------------------------
// 14.11.13 | Morikawa Atsushi | Created this file.
// 15.01.05 | Morikawa Atsushi | Added Log Send Function.
// 15.02.04 | Morikawa Atsushi | Changed a method using MSP Event to a method using DD Event.
// 15.02.12 | Morikawa Atsushi | Added Log ID. (LOGID_FRAME)

#include "Common.h"
#include "DD_SCI.h"
#include "DD_LOG.h"

/* Data */
#define	DD_LOG_QUEUE_SIZE	1024

// ID Name
// �uID Name�v��ύX����ꍇ��"DD_LOG.h"�́uLog ID�v���ύX���A
// �uLog ID�v�Œ�`�������l�ƁA�Y������uID Name�v�̒ʂ��ԍ�����v�����邱�ƁB
static const UC DD_LOG_IDName[LOGID_MAX][4] =
					 {'F','I','N',':',			// 0:�Œ� ��Ճ`�F�b�N�Ŏg�p���Ă��邽�ߕύX�s��
					  'I','O','T',':',			// 1:
					  'F','R','M',':',			// 2:
					  'S','C','B',':',			// 3:
					  'S','Y','S',':',			// 4:
					  'S','L','C',':',			// 5:
					  'J','A','M',':',			// 6:
					  'R','T','C',':',			// 7:
					  'L','O','G',':',			// 8:
					  'M','T','R',':',			// 9:
					  'E','J','T',':',			// 10:
					  'S','T','P',':',			// 11:
					  'T','M','P',':',			// 12:
					  'S','T','K',':',			// 13:
					  'T','N','S',':',			// 14:
					  'F','L','D',':',			// 15:
					  'B','K','T',':',			// 16:
					  'D','E','V',':',			// 17:
					  'S','H','F',':',			// 18:
					  'I','N','T',':',	 		// 19:
					  'C','M','L',':',			// 20:
					  'S','H','T',':',			// 21:
					  'S','Y','S',':',			// 22:
					  'T','M','P',':',			// 23:
					  'T','C','B',':',			// 24:
					  'S','C','C',':',			// 25:
					  'S','C','I',':',			// 26:
					  'S','C','P',':',			// 27:
					  'S','C','T',':',			// 28:
					  'S','C','S',':',			// 29:
					  'S','C','V',':',};		// 30:

static const UC DD_LOG_StopMsg[]    = {'S','T','O','P'};
static const UC DD_LOG_RestartMsg[] = {'R','S','T','A'};
static UC sndBuff[225] = {0};
static US sndDataSize = 0;

typedef enum{
	// NonState = 0,
	stIdle_ID = 1,
	stSending_ID,
} DD_LOG_StatusID;
static DD_LOG_StatusID gState;

typedef struct {
	US	readPtr;								// �ǂݏo����Pointer
	US	writePtr;								// �i�[��Pointer
	UC	msgQueue[DD_LOG_QUEUE_SIZE];			// �i�[�̈�
} DD_LOG_RCV_BUFF;
static DD_LOG_RCV_BUFF DD_LOG_RcvBuff;
static UC full;									// 0:Not Full, 1:Full

/* ProtoType */
static void DD_LOG_Send(void);
static US DD_LOG_Get(UC *data);
static void stIdle(void);
static void stSending(void);
static UC DD_LOG_HexToAsciiH(UC dat);
static UC DD_LOG_HexToAsciiL(UC dat);

//=============================================================================
// Function Name : DD_LOG_Init
// Description   : LOG�f�[�^���M���W���[���̏�����
// Parameter     : -
// Return        : -
// Note          : -
//=============================================================================
void DD_LOG_Init(void)
{
	DD_LOG_RCV_BUFF *pRcvBuff;
	pRcvBuff = &DD_LOG_RcvBuff;
	pRcvBuff->writePtr = 0;
	pRcvBuff->readPtr = 0;
	gState = stIdle_ID;
	full = 0;
}

//=============================================================================
// Function Name : DD_LOG_Put
// Description   : LOG�f�[�^�̎�M�o�b�t�@�i�[
// Parameter     : id		Log ID
//				 : sr		�f�[�^���(Bin/������w��)
//				 : *data	LOG�i�[�G���A�̐擪�|�C���^
//				 : logSize	Log�T�C�Y
//				 : category	LOG�J�e�S��
// Return        : -
// Note          : -
//=============================================================================
void DD_LOG_Put(UC id, UC sr, UC *data, US logSize, UC category)
{
	US queueSpace, i;
	DD_LOG_RCV_BUFF *pRcvBuff;
	US writePos;
	UL sysTim = DD_EVT_GetCurrentTime();
	US size;
	static UL nvmLogSel;
	static US nvmLogSel16To31;
	static UC nvmLogCat;

#ifdef ENABLE_FPGARAM
	if ( get_FPGA_RAM_Status() == 0 ) {			// FPGA RAM �A�N�Z�X�G���[
		return;
	}
#endif
	if ( id > LOGID_MAX ) {						// �s����Log ID
		return;
	}

	nvmLogSel = 0xFFFF;							// NVM�̊֐����������̂��߁A���̂Ƃ���0xFFFF�Ō��ߑł�
	nvmLogSel16To31 = 0xFFFF;					// NVM�̊֐����������̂��߁A���̂Ƃ���0xFFFF�Ō��ߑł�
	nvmLogSel |= ((UL)nvmLogSel16To31 << 16);
	if ( ((nvmLogSel >> id) & 1) == 0 ) {		// Log ID�ɂ��I���@0�Ȃ�L�����Ȃ�
		return;
	}

	nvmLogCat = 0b111;						// NVM�̊֐����������̂��߁A���̂Ƃ���7(0b111)�Ō��ߑł�
	if ( ((nvmLogCat >> category) & 1) == 0 ) {	// Category�ɂ��I���@0�Ȃ�L�����Ȃ�
		return;
	}

	pRcvBuff = &DD_LOG_RcvBuff;
	size = logSize;

	if ( full > 0 ) {							// Full���
		return;									// ��M�o�b�t�@��Log�f�[�^��S�ďo�͂��I����܂ŐV�����i�[���Ȃ�
	}
	if ( logSize > DD_LOG_MAX_DATA_SIZE ) {		// MAX���傫���ꍇ�́AMAX�܂ŋL��
		size = DD_LOG_MAX_DATA_SIZE;
	}

	if ( pRcvBuff->writePtr < pRcvBuff->readPtr ) {	// �i�[�̈�󂫗e�ʎZ�o
		queueSpace = (US)(pRcvBuff->readPtr - pRcvBuff->writePtr);
	}
	else {
		queueSpace = (US)(DD_LOG_QUEUE_SIZE - (pRcvBuff->writePtr - pRcvBuff->readPtr));
	}

	if ( queueSpace < (size + 22) ) {			// �i�[�̈�`�F�b�N: �f�[�^�T�C�Y + �f�[�^�ȊO�̌Œ�T�C�Y(9[byte]) �{ Stop���b�Z�[�W�T�C�Y(13[byte])
		full = 1;
		size = 4;
		id = LOGID_LOG;
		sr = '0';
		data = (UC *)DD_LOG_StopMsg;			// Stop���b�Z�[�W��Buffer�Ɋi�[
	}

	writePos = pRcvBuff->writePtr;				// Queue Write Pointer�擾

	// Header
	pRcvBuff->msgQueue[writePos] = (UC)(0xFF);
	writePos++;
	writePos &= (DD_LOG_QUEUE_SIZE - 1);
	pRcvBuff->msgQueue[writePos] = (UC)(0xAA);
	writePos++;
	writePos &= (DD_LOG_QUEUE_SIZE - 1);

	// Time
	pRcvBuff->msgQueue[writePos] = (UC)(sysTim >> 16);
	writePos++;
	writePos &= (DD_LOG_QUEUE_SIZE - 1);
	pRcvBuff->msgQueue[writePos] = (UC)(sysTim >> 8);
	writePos++;
	writePos &= (DD_LOG_QUEUE_SIZE - 1);
	pRcvBuff->msgQueue[writePos] = (UC)(sysTim & 0x0FF);
	writePos++;
	writePos &= (DD_LOG_QUEUE_SIZE - 1);

	// ID
	pRcvBuff->msgQueue[writePos] = (UC)(id);
	writePos++;
	writePos &= (DD_LOG_QUEUE_SIZE - 1);

	// SR
	pRcvBuff->msgQueue[writePos] = (UC)(sr);
	writePos++;
	writePos &= (DD_LOG_QUEUE_SIZE - 1);

	// Size
	pRcvBuff->msgQueue[writePos] = (UC)(size >> 8);
	writePos++;
	writePos &= (DD_LOG_QUEUE_SIZE - 1);
	pRcvBuff->msgQueue[writePos] = (UC)(size & 0x0FF);
	writePos++;
	writePos &= (DD_LOG_QUEUE_SIZE - 1);

	// Data
	for ( i = 0; i < size; i++ ) {
		pRcvBuff->msgQueue[writePos] = *(data + i);
		writePos++;
		writePos &= (DD_LOG_QUEUE_SIZE - 1);
	}

	pRcvBuff->writePtr = writePos;				// Write�|�C���^�X�V
	DD_EVT_Put(evSend_ID, evuDD_LOG_ID, 0, 0);	// Send�C�x���g�o�^
}

//=============================================================================
// Function Name : DD_LOG_Notify
// Description   : Log���M�C�x���g�������ɃR�[������A�C�x���gID�ɉ������������s��
// Parameter     : eventID	�C�x���gID
// Return        : pMsg		�f�[�^�|�C���^
//=============================================================================
void DD_LOG_Notify(DD_EventID eventID, UC* pMsg)
{
	switch ( eventID ) {
	case evSend_ID:
		switch ( gState ) {
		case stIdle_ID:
			stSending();
			break;
		case stSending_ID:
			break;
		default:
			break;
		}
		break;
	case evComplete_ID:
		switch ( gState ) {
		case stIdle_ID:
			break;
		case stSending_ID:
			stIdle();
			break;
		default:
			break;
		}
		break;
	}
}

//=============================================================================
// Function Name : DD_LOG_Send
// Description   : LOG�f�[�^�̑��M
// Parameter     : -
// Return        : -
// Note          : -
//=============================================================================
void DD_LOG_Send(void)
{
	sndDataSize = DD_LOG_Get(sndBuff);
	if ( sndDataSize ) {
		DD_SCI_LogPacketSend(sndBuff, sndDataSize);
	}
}

//=============================================================================
// Function Name : DD_LOG_Get
// Description   : LOG�f�[�^����M�o�b�t�@������o���A�o�͌`���ɕϊ�����
// Parameter     : *data	���M�f�[�^�̐擪�A�h���X
// Return        : size		���M�f�[�^�T�C�Y
//=============================================================================
US DD_LOG_Get(UC *data)
{
	US dataSize;
	US readPos;
	US i;
	DD_LOG_RCV_BUFF *pRcvBuff;
	US size;
	US mix;										// 0:Bin���[�h, 1:Mix���[�h
	US logID;

	pRcvBuff = &DD_LOG_RcvBuff;
	size = 0;
	readPos = pRcvBuff->readPtr;				// Read�|�C���^�擾

	// Header
	*data = pRcvBuff->msgQueue[readPos];
	readPos++;
	readPos &= (DD_LOG_QUEUE_SIZE - 1);			// ��M�o�b�t�@�I�[�𒴂�����擪�ɖ߂�
	if ( *data != 0xFF ) {
		return(0);
	}
	*data = pRcvBuff->msgQueue[readPos];
	readPos++;
	readPos &= (DD_LOG_QUEUE_SIZE - 1);
	if ( *data != 0xAA ) {
		return(0);
	}
	// Header�͑��M���Ȃ����߁A���M�o�b�t�@�Ɋi�[���Ȃ�

	// Time
	for ( i = 0; i < 3; i++ ) {
		*data = DD_LOG_HexToAsciiH(pRcvBuff->msgQueue[readPos]);
		data++;
		size++;
		*data = DD_LOG_HexToAsciiL(pRcvBuff->msgQueue[readPos]);
		data++;
		size++;
		readPos++;
		readPos &= (DD_LOG_QUEUE_SIZE - 1);
	}
	*data = ' ';
	data++;
	size++;

	// ID
	logID = pRcvBuff->msgQueue[readPos];
	if ( logID < LOGID_END ) {
		*data = DD_LOG_IDName[logID][0];
		data++;
		size++;
		*data = DD_LOG_IDName[logID][1];
		data++;
		size++;
		*data = DD_LOG_IDName[logID][2];
		data++;
		size++;
		*data = DD_LOG_IDName[logID][3];
		data++;
		size++;
	}
	readPos++;
	readPos &= (DD_LOG_QUEUE_SIZE - 1);

	// SR, Data
	if ( pRcvBuff->msgQueue[readPos] == '0' ) {	// ������
		*data = pRcvBuff->msgQueue[readPos];
		data++;
		size++;
		readPos++;
		readPos &= (DD_LOG_QUEUE_SIZE - 1);
		*data = ' ';
		data++;
		size++;
		dataSize = pRcvBuff->msgQueue[readPos];
		readPos++;
		readPos &= (DD_LOG_QUEUE_SIZE - 1);
		dataSize = (dataSize << 8) + pRcvBuff->msgQueue[readPos];
		readPos++;
		readPos &= (DD_LOG_QUEUE_SIZE - 1);

		for ( i = 0; i < dataSize; i++ ) {
			*data = pRcvBuff->msgQueue[readPos];
			data++;
			size++;
			readPos++;
			readPos &= (DD_LOG_QUEUE_SIZE - 1);
		}
	}
	else {
		if ( pRcvBuff->msgQueue[readPos] == 'M' ) {
			mix = 1;							// Mix Mode
		}
		else {
			mix = 0;							// Bin Mode
		}
		if ( (pRcvBuff->msgQueue[readPos] == 'S') || (pRcvBuff->msgQueue[readPos] == 'R') ) {
			*data = pRcvBuff->msgQueue[readPos];
			data++;
			size++;
		}
		readPos++;
		readPos &= (DD_LOG_QUEUE_SIZE - 1);
		*data = ' ';
		data++;
		size++;

		dataSize = pRcvBuff->msgQueue[readPos];
		readPos++;
		readPos &= (DD_LOG_QUEUE_SIZE - 1);
		dataSize = (dataSize << 8) + pRcvBuff->msgQueue[readPos];
		readPos++;
		readPos &= (DD_LOG_QUEUE_SIZE - 1);

		for ( i = 0; i < dataSize; i++ ) {
			if ( mix == 1 ) {					// ������ƃo�C�i����MIX�̃P�[�X
				if ( pRcvBuff->msgQueue[readPos] == 0 ) {	// ��؂�(Null����)�̃`�F�b�N
					mix = 0;					// �ȍ~�̓o�C�i���Ƃ���
					readPos++;
					i++;
					*data = ' ';
					data++;
					size++;
					readPos &= (DD_LOG_QUEUE_SIZE - 1);
				}
			}
			if ( mix == 0 ) {					// �o�C�i���f�[�^�𕶎��ϊ����ĕ\��
				*data = DD_LOG_HexToAsciiH(pRcvBuff->msgQueue[readPos]);
				data++;
				size++;
				*data = DD_LOG_HexToAsciiL(pRcvBuff->msgQueue[readPos]);
				data++;
				size++;
				*data = ' ';
				data++;
				size++;
			}
			else {								// �����R�[�h���̂܂܂̕\���iMIX�Ńo�C�i���ڍs�O�j
				*data = pRcvBuff->msgQueue[readPos];
				data++;
				size++;
			}
			readPos++;
			readPos &= (DD_LOG_QUEUE_SIZE - 1);
		}
	}

	// Break(���s����)
	*data = 0x0D;
	data++;
	size++;
	*data = 0x0A;
	data++;
	size++;

	pRcvBuff->readPtr = readPos;

	return(size);
}

//=============================================================================
// Function Name : stIdle
// Description   :
// Parameter     : -
// Return        : -
//=============================================================================
void stIdle(void)
{
	DD_LOG_RCV_BUFF *pRcvBuff;
	pRcvBuff = &DD_LOG_RcvBuff;
	gState = stIdle_ID;
	if ( pRcvBuff->readPtr != pRcvBuff->writePtr ) {	// ��M�o�b�t�@�ɖ����M�f�[�^�����邩
		DD_EVT_Put(evSend_ID, evuDD_LOG_ID, 0, 0);
	} else if ( full ){
		full = 0;
		DD_LOG_Put(LOGID_LOG, '0', DD_LOG_RestartMsg, 4, LOG_CATEGORY_DEF);
	}
}

//=============================================================================
// Function Name : stSending
// Description   :
// Parameter     : -
// Return        : -
//=============================================================================
void stSending(void)
{
	gState = stSending_ID;
	DD_LOG_Send();
}

//==============================================================================
// Function Name : DD_LOG_HexToAsciiH
// Description   : Hex�R�[�h��Ascii�R�[�h�ɕϊ�
// Parameter     : dat	�ϊ�����Hex�R�[�h�̃f�[�^
// Return        : ans	�ϊ����ꂽAscii�R�[�h�̏�ʃr�b�g
//==============================================================================
UC DD_LOG_HexToAsciiH(UC dat)
{
	UC ans = (dat & 0xF0) >> 4;

	if ( ans <= 9 ) {
		ans = ans + 0x30;
	}
	else {
		ans = ans + 0x37;
	}

	return ans;
}

//==============================================================================
// Function Name : DD_LOG_HexToAsciiL
// Description   : Hex��Ascii�ɕϊ�
// Parameter     : dat	�ϊ�����Hex�R�[�h�̃f�[�^
// Return        : ans	�ϊ����ꂽAscii�R�[�h�̉��ʃr�b�g
//==============================================================================
UC DD_LOG_HexToAsciiL(UC dat)
{
	UC ans = (dat & 0x0F);

	if ( ans <= 9 ) {
		ans = ans + 0x30;
	}
	else {
		ans = ans + 0x37;
	}

	return ans;
}

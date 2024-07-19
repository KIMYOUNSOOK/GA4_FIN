// File Name: SRVC_MsgActivity.h
// Engineer:  Nishizawa Mitsuyuki
// Last Edit: 14.05.14
// Revision:  000
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//

// はじめに
//
// 本クラスは、システム部品やコンパイラ部品などが持つ、各デバイスへの動作指示Activityを
// 実現するための共通の仕組みである。
// 動作指示Activityは、複数のメッセージ送信Actionから構成されており、
// 各Actionは、開始トリガが成立後に、指定メッセージを送信(Start)し、
// 指定メッセージを受信したら開始トリガのフラグを更新(SetTrigger)し、
// 次のActionを開始(Start)させる仕組みを持つ。

// 詳細は下記使用例およびSRVC_MsgActivity.cppの各メソッドの論理を参照のこと。


#ifndef SRVC_MSG_ACTIVITY_H
#define SRVC_MSG_ACTIVITY_H

#define SRVC_ID2BIT(idx)			(0x00000001 << (idx))		// インデックス(0～15)からビットパターン(0x0001～0x8000)への変換
#define SRVC_END_BIT_PATTERN(num)	((0x00000001 << num) - 1)	// 例えば5の場合は20h - 1 →  1Fh


typedef struct /*SrvcMsgActivity*/ {
	UL startTrgPattern;										// Messageを送信するためのテーブルIndexのBitパターン(開始トリガの成立条件)
															// センサの変化待ちActionの場合には"SRVC_SNR_WAIT(0xFFFF)"とする
	US* pWaitTime;											// 開始トリガ成立条件後から指定メッセージを送信するまでの待ち時間のポインタ
	US sendEventID;											// Action実施時に送信するEvent(Message)ID
	US recvEventID;											// Action(≠Activity)の終了条件となるEvent(Message or Sensor)ID
	UC senderID;											// 受信したEventIDの送信元ID
} SrvcMsgActivity;

/*class CSrvcMsgActivity {
public:

	CSrvcMsgActivity(UC userMID, UC tableSize, US endPattern, SrvcMsgActivity* pTable)
		: mUserMID(userMID), mTableSize(tableSize), mCrntPattern(0), mEndPattern(endPattern), mpTable(pTable){}

	void Start(void);										// Activityを開始させる
	void Progress(void);									// 現トリガパターンで送信できるイベントを全て送信
	void SetTrigger(US recvEventID, UC senderID);			// 受信イベントに応じた現トリガパターンの更新
	void TimeOut(UL idx);									// 待ち時間経過後のイベント送信
	bool CheckEnd(void);									// 現トリガパターンと終了トリガパターンを比較してActivityが終了したかどうか判断
	void Terminate();										// Acitivityを終了させる
	void SetData(UC data);
	UL GetCurrentBitPattern() { return mCrntPattern; }
	
private:
	UC mUserMID;
	UC mTableSize;
	UL mCrntPattern;										// 現在のbitPattern(1になっているbitが終了Action)
	UL mStartPattern;										// 現在の開始済みbitPattern(1になっているbitが開始済みAction)
	UL mEndPattern;											// 終了条件のbitPattern
	bool mIsActive;
	UC mMsgData;
	SrvcMsgActivity* mpTable;
};*/

typedef struct /*CSrvcMsgActivity*/ {
	UC mUserMID;
	UC mTableSize;
	UL mCrntPattern;										// 現在のbitPattern(1になっているbitが終了Action)
	UL mStartPattern;										// 現在の開始済みbitPattern(1になっているbitが開始済みAction)
//	UL mEndPattern;											// 終了条件のbitPattern
	UC mIsActive;
	UC* mpMsgData;
	UC mMsgDataSize;
	SrvcMsgActivity* mpTable;
	void (*mpTimerCallBack)(SS timerID, UL idx);			// タイマーコールバック関数のポインタ
} CSrvcMsgActivity;

void CSrvcMsgActivity_Constructor(CSrvcMsgActivity* pThis, UC userMID, UC tableSize, SrvcMsgActivity* pTable, void (*pCallBack)(SS, UL));

void CSrvcMsgActivity_Start(CSrvcMsgActivity* pThis);		// Activityを開始させる
void CSrvcMsgActivity_Progress(CSrvcMsgActivity* pThis);	// 現トリガパターンで送信できるイベントを全て送信
void CSrvcMsgActivity_SetTrigger(CSrvcMsgActivity* pThis, US recvEventID, UC senderID);
															// 受信イベントに応じた現トリガパターンの更新
void CSrvcMsgActivity_TimeOut(CSrvcMsgActivity* pThis, UL idx);
															// 待ち時間経過後のイベント送信
bool CSrvcMsgActivity_CheckEnd(CSrvcMsgActivity* pThis);	// 現トリガパターンと終了トリガパターンを比較してActivityが終了したかどうか判断
void CSrvcMsgActivity_Abort(CSrvcMsgActivity* pThis);		// Acitivityを中止させる
void CSrvcMsgActivity_Terminate(CSrvcMsgActivity* pThis);	// Acitivityを終了させる
void CSrvcMsgActivity_SetData(CSrvcMsgActivity* pThis, UC* pData, UC size);
#define CSrvcMsgActivity_GetCurrentBitPattern(apThis) ((apThis)->mCrntPattern)
															// UL (CSrvcMsgActivity*)

/* 
◆◆◆ 使用例 Power On InitializeのActivityの場合 ◆◆◆


	            	   (Start)                          
		                 ↓                             
		             CMPL(IDX=0)                        
		                 ↓                             
		===================================             
		 ↓              ↓                             
		BKLT(IDX=1)      ↓     XXX_SNR ON(IDX=3)       
		 ↓          STK(IDX=2)     ↓                  
		PUNCH(IDX=5)     ↓     FLD(IDX=4)              
		 ↓              ↓         ↓                  
		===================================             
		                 ↓                             
		             TSQF(IDX=6)                        
		                 ↓                             
		               (End)                            



//◆ テーブルの実装例
// 待ち時間テーブルIDとテーブル
enum ESeq_POnWaitTimeID {
	SEQ_P_ON_WAIT_CMPL,
	SEQ_P_ON_WAIT_STK,
	SEQ_P_ON_WAI_PUNCH
};

US gSeq_POnWaitTimTbl[] = {100, 20, 10};


// Activityテーブル
const SrvcMsgActivity gSeq_POnShtInitActTable[] = {
	//startTrgPattern		 pWaitTime									sendActReqEventID					recvEventIDforEndAction				senderID                    
	{ 0, 					 &gSeq_POnWaitTimTbl[SEQ_P_ON_WAIT_CMPL],	ACT_REQ_POWER_ON_INIT_COMPILER, 	ACT_RES_POWER_ON_INIT_COMPILER,		MID_COMPILER		},	// IDX = 0 ← 無条件で開始の場合は"0"
	{ SRVC_ID2BIT(0),		 0,											ACT_REQ_POWER_ON_INIT_BOOKLET, 		ACT_RES_POWER_ON_INIT_BOOKLET,		MID_BOOKLET			},	// IDX = 1 ← IDX=0の終了がトリガなので"SRVC_ID2BIT(0)"
	{ SRVC_ID2BIT(0),		 &gSeq_POnWaitTimTbl[SEQ_P_ON_WAIT_STK],	ACT_REQ_POWER_ON_INIT_STACKER, 		ACT_RES_POWER_ON_INIT_STACKER,		MID_STACKER_TRAY	},	// IDX = 2 ← IDX=0の終了がトリガなので"SRVC_ID2BIT(0)"
	{ 0,					 0,											MSP_EVT_NUM_VOID,					LOC_IOT_EXIT_SENSOR_ON,				MID_FRAME			},  // IDX = 3 ← XXXセンサのONエッジを待つだけのAction (Device Eventでも同様)
																																										//            開始条件は無条件なので"0"
	{ SRVC_ID2BIT(3),		 0,											ACT_REQ_POWER_ON_INIT_FOLDER, 		ACT_RES_POWER_ON_INIT_FOLDER,		MID_FOLDER			},	// IDX = 4 ← IDX=3の終了（＝センサの変化）がトリガなので"SRVC_ID2BIT(3)"
	{ SRVC_ID2BIT(1),		 &gSeq_POnWaitTimTbl[SEQ_P_ON_WAI_PUNCH],	ACT_REQ_POWER_ON_INIT_PUNCH, 		ACT_RES_POWER_ON_INIT_PUNCH,		MID_PUNCH			},	// IDX = 5 ← IDX=1の終了がトリガなので"SRVC_ID2BIT(1)"
	{ SRVC_ID2BIT(2)						
		|SRVC_ID2BIT(4)	 
		|SRVC_ID2BIT(5),	 0,											ACT_REQ_POWER_ON_INIT_TSQF,			ACT_RES_POWER_ON_INIT_TSQF,			MID_TSQF			}	// IDX = 6 ← IDX=2,4,5の終了がトリガなので"SRVC_ID2BIT(2)|SRVC_ID2BIT(4)|SRVC_ID2BIT(5)"
};

// Activityクラスのインスタンス生成
CSrvcMsgActivity gSeq_POnShtInitActivityTest(MID_SYSTEM, 
	                                        sizeof(gSeq_POnShtInitActTable)/sizeof(gSeq_POnShtInitActTable[0]), 
											SRVC_END_BIT_PATTERN(sizeof(gSeq_POnShtInitActTable)/sizeof(gSeq_POnShtInitActTable[0])), 
											(SrvcMsgActivity*)gSeq_POnShtInitActTable);



//◆ 処理の実装例
// ● Activityの開始
void ActivityTestStart()
{
	gSeq_POnWaitTimTbl[SEQ_P_ON_WAIT_CMPL]	= 100;
	gSeq_POnWaitTimTbl[SEQ_P_ON_WAIT_STK]	= 10;
	gSeq_POnWaitTimTbl[SEQ_P_ON_WAI_PUNCH]	= 250;

	gSeq_POnShtInitActivityTest.Start();
}

void ActivityTestStop()
{
	gSeq_POnShtInitActivityTest.Terminate();
}


// ● Activityのステップ実行（＆終了）
//   ・Action Responseの受信をトリガとする場合は下記のような
//     コールバック関数をMSPのメッセージイベントのコールバック関数リストに登録する。
      
void RecvEndActTrgOfPowerOnInitActivity(US trgEvent, UC senderID)
{
	gSeq_POnShtInitActivityTest.SetTrigger(trgEvent, senderID);
	
	if (!(gSeq_POnShtInitActivityTest.CheckEnd())) {
		gSeq_POnShtInitActivityTest.Progress();
	}
	else {
		gSeq_POnShtInitActivityTest.Terminate();
	}
}

// ● MSPのイベント管理リストに登録するコールバック関数
void RecvPowerOnInitCmpl(const UC* pMsg, UC sender) { RecvEndActTrgOfPowerOnInitActivity(ACT_RES_POWER_ON_INIT_COMPILER, sender); }
void RecvPowerOnInitBklt(const UC* pMsg, UC sender) { RecvEndActTrgOfPowerOnInitActivity(ACT_RES_POWER_ON_INIT_BOOKLET, sender); }
void RecvPowerOnInitStk(const UC* pMsg, UC sender)	{ RecvEndActTrgOfPowerOnInitActivity(ACT_RES_POWER_ON_INIT_STACKER, sender); }
void RecvLocIotExitSnrOn(const UC* pMsg, UC sender) { RecvEndActTrgOfPowerOnInitActivity(LOC_IOT_EXIT_SENSOR_ON, sender); }
void RecvPowerOnInitFldr(const UC* pMsg, UC sender) { RecvEndActTrgOfPowerOnInitActivity(ACT_RES_POWER_ON_INIT_FOLDER, sender); }
void RecvPowerOnInitPnch(const UC* pMsg, UC sender) { RecvEndActTrgOfPowerOnInitActivity(ACT_RES_POWER_ON_INIT_PUNCH, sender); }
void RecvPowerOnInitTsqf(const UC* pMsg, UC sender) { RecvEndActTrgOfPowerOnInitActivity(ACT_RES_POWER_ON_INIT_TSQF, sender); }

*/
#endif

// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+-----------------------------------------------------------------------------------------------------------------------------
// 14.05.14 | Nishizawa Mitsuyuki | Create This File.


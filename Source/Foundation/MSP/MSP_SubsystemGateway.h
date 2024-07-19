// ☆☆☆ Module Software Platform Frame ☆☆☆
/// @file    MSP_SubsystemGateway.h
/// @author  Mitsuyuki Nishizawa
/// @date    14.11.05
/// @version 001
//
// Copyright (C) by Fuji Xerox Advanced Technology Co.,Ltd. All rights reserved.
//


#ifndef MSP_SubsystemGateway_h
#define MSP_SubsystemGateway_h


// *******************************************************  <<<<< Definition & Declaration Area >>>>>  ********************************************************

// ===========================  <<< マクロ定義 >>>  ===========================

// ----- 無効なゲートウェイID -----
#define	MSP_GW_VOID			0xFF													// 存在しないOptionのGateway用のSubsystem IDとしてスペック情報に設定する


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Class Name    : CSubsystemGateway
// Description   : サブシステムへのコマンド（ステータス）送信およびコマンド･イベントの生成･通知を実施する.
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
typedef struct SubsystemGateway {
/*public:*/
	//=[[[ Constructor ]]]=================================================================
	// Function Name : CSubsystemGateway::CSubsystemGateway()
	// Description   : このクラスのコンストラクタ.
	// Parameter     : pSpec		"スペック･テーブルへのポインタ"
	// Return        : -
	//=====================================================================================
	//CSubsystemGateway(const GatewaySpec *const pSpec) : mpSpec(pSpec) {}

	//=====================================================================================
	// Method Name   : CSubsystemGateway::SendCommand()
	// Description   : コマンド（ステータス）を送信する.
	// Parameter     : senderID		"送信元モジュールID"
	//               : pPacket		"送信パケット格納先アドレス"
	//               : length		"送信パケットサイズ"
	//               : log			"送信パケットのログ情報保持の有無"
	// Return        : コマンド送信の結果（0:送信成功 / 1:送信失敗）
	//=====================================================================================
	//UC SendCommand(UC senderID, const UC *pPacket, US length, UC log) {}

	//=====================================================================================
	// Method Name   : CSubsystemGateway::ReceiveCommand()
	// Description   : コマンド（ステータス）の受信およびコマンド･イベントの生成をする.
	// Parameter     : pPacket		"サブシステムからの受信パケット格納アドレス（参照先）"
	//               : length		"受信パケットサイズ"
	// Return        : なし
	//=====================================================================================
	//void ReceiveCommand(const UC *pPacket, US length) {}

/*private:*/
	const GatewaySpec *mpSpec ;																// [Member] Subsystem Gatewayのスペック
	CommandEvent  mSendBuffer ;																// [Member] イベント送信バッファ（コマンド⇒イベント変換用）
} CSubsystemGateway ;


// ===========================  <<< 関数プロトタイプ宣言 >>>  ===========================
void CSubsystemGateway_CSubsystemGateway(CSubsystemGateway*, const GatewaySpec*) ;			// ゲートウェイ初期化処理（コンストラクタ）
UC CSubsystemGateway_SendCommand(CSubsystemGateway*, UC, const UC*, US, UC) ;				// コマンド（ステータス）送信 ()
void CSubsystemGateway_ReceiveCommand(CSubsystemGateway*, const UC*, US) ;					// コマンド受信＆コマンド･イベント生成&通知 ()

#endif			// MSP_SubsystemGateway_h


// Change History
// Date:    | Engineer:           | Note:
// ---------+---------------------+----------------------------------------------------------------------------------------------------------------------------
// 13.05.05 | Mitsuyuki Nishizawa | Module Software Platform Frameのプロトタイプとして本ファイルを作成.
// 14.11.05 | Mitsuyuki Nishizawa | 送受信関数名称および引数を変更&初期化処理を追加.
//
//


// **********************************************************  [ MSP_SubsystemGateway.h : EOF ]  **************************************************************

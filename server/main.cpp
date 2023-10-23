//==========================================================
//
// winsock通信(サーバー)
// Author : Ibuki Okusada
//
//==========================================================
#include <stdio.h>
#include <winsock2.h>
#include <time.h>
#include <string.h>
#include "tcp_server.h"
#include "tcp_client.h"
#include <thread>
#include <stdlib.h>
#include "protocol_online.h"
#include "d3dx9.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")		// システム時刻取得に必要

// マクロ定義
#define MAX_STRING	(2048)	// 最大文字数
#define DEF_PORT	(22333)	// ポート番号

// IP情報クラス定義
void Access(CClient *pClient);
void Join(int nId, CClient *pClient);
void SetPosition(int nId, D3DXVECTOR3 pos, CClient *pClient);
void SetRotation(int nId, D3DXVECTOR3 pos, CClient *pClient);
void SetDamage(int nId, int nDamage, CClient *pClient);
void SetLife(int nId, int nLife, CClient *pClient);
void SetUp(int nId, CClient *pClient);
void Leave(int nId, CClient *pClient);
void Goal(int nId, CClient *pClient);
void Accept(CServer *pServer);
void Send(CServer **ppServer);

// グローバル変数宣言
char g_aSendData[MAX_STRING] = {};	// 送信データ
int g_nSendByte = 0;	// 送信データ量

//==========================================================
// メイン関数
//==========================================================
int main(void)
{
	// データの読み込み	
	CServer *pServer = NULL;	// サーバーのポインタ
	CClient *pClient = NULL;	// 送受信用のポインタ
	CClient *pOperation = NULL;	// 運営のポインタ

	// Winsockの初期化処理
	WSADATA wsaData;
	int nErr = WSAStartup(WINSOCK_VERSION, &wsaData);	// winsockの初期化関数

	if (nErr != 0)
	{// 初期化に失敗した場合
		printf("*** Winsockの初期化に失敗しました ***\n");

		// キー入力待ち
		rewind(stdin);
		getchar();

		return E_FAIL;	// 終了
	}

	// サーバーの生成
	pServer = new CServer;

	// 生成チェック
	if (pServer == NULL)
	{
		return E_FAIL;
	}

	// 初期化処理
	if (pServer->Init(DEF_PORT) == false)
	{
		return E_FAIL;
	}

	// 別スレッドで送信準備
	{
		std::thread th(Send, &pServer);
		th.detach();
	}

	// 別スレッドで受け入れ待ち
	{
		std::thread th(Accept, pServer);
		th.detach();
	}

	// 運営メッセージ送信
	while (1)
	{
		char aMessage[MAX_STRING] = {};
	}

	if (pServer != NULL)
	{// 使用されている場合
		pServer->Uninit();

		delete pServer;
		pServer = NULL;
	}

	//Winsock終了処理
	WSACleanup();	// WSACleanup関数 : winsockの終了処理

	printf("*** Enterキーを押して終了してください ***\n");

	// キー入力待ち
	rewind(stdin);
	getchar();

	return S_OK;
}

//==========================================================
// マルチスレッド受信
//==========================================================
void Access(CClient *pClient)
{
	int nProt = -1;
	int nId = pClient->GetId();

	char aSendData[sizeof(int) * 2] = {};	// 送信用

	Join(pClient->GetId(), pClient);

	while (1)
	{
		D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		char aRecvData[MAX_STRING] = {};	// 受信用
		int nType = COMMAND_TYPE_NONE;
		int nDamage = 0;

		// 受信
		int nRecvbite = pClient->Recv(&aRecvData[0], MAX_STRING);

		memcpy(&nType, &aRecvData[0], sizeof(int));

		if (nRecvbite <= 0)
		{
			Leave(pClient->GetId(), pClient);
			break;
		}
		else
		{
			if (pClient->GetSend() == true)
			{
				continue;
			}

			switch (nType)
			{
			case COMMAND_TYPE_NONE:

				break;
			case COMMAND_TYPE_SETROTATION:

				memcpy(&pos, &aRecvData[sizeof(int)], sizeof(D3DXVECTOR3));
				SetRotation(pClient->GetId(), pos, pClient);
				break;

			case COMMAND_TYPE_SETPOSITION:

				memcpy(&pos, &aRecvData[sizeof(int)], sizeof(D3DXVECTOR3));
				SetPosition(pClient->GetId(), pos, pClient);
				break;

			case COMMAND_TYPE_SETLIFE:

				memcpy(&nDamage, &aRecvData[sizeof(int)], sizeof(nDamage));
				SetLife(pClient->GetId(), nDamage, pClient);
				break;

			case COMMAND_TYPE_GOAL:

				Goal(pClient->GetId(), pClient);
				break;

			case COMMAND_TYPE_START_OK:

				SetUp(pClient->GetId(), pClient);

				break;

			case COMMAND_TYPE_CREATE:

				Join(pClient->GetId(), pClient);

				break;

			case COMMAND_TYPE_DAMAGE:

				memcpy(&nDamage, &aRecvData[sizeof(int)], sizeof(nDamage));
				SetDamage(pClient->GetId(), nDamage, pClient);

				break;

			case COMMAND_TYPE_DELETE:

				Leave(pClient->GetId(), pClient);

				break;

			case COMMAND_TYPE_GETID:

				// protocolを挿入
				memcpy(&aSendData[0], &nProt, sizeof(int));
				memcpy(&aSendData[sizeof(int)], &nId, sizeof(int));

				// プロトコルを挿入
				memcpy(&g_aSendData[g_nSendByte], &aSendData[0], sizeof(aSendData));
				g_nSendByte += sizeof(int) * 2;

				break;
			}
		}
	}
}

//==========================================================
// 入室
//==========================================================
void Join(int nId, CClient *pClient)
{
	int nProt = -1;	// プロトコル番号
	char aSendData[sizeof(int) * 2 + sizeof(int) * 2] = {};	// 送信用まとめデータ

	// IDを挿入
	memcpy(&aSendData[0], &nProt, sizeof(int));
	memcpy(&aSendData[sizeof(int)], &nId, sizeof(int));

	nProt = COMMAND_TYPE_CREATE;

	// IDを挿入
	memcpy(&aSendData[sizeof(int) * 2], &nId, sizeof(int));
	memcpy(&aSendData[sizeof(int) * 3], &nProt, sizeof(int));

	// プロトコルを挿入
	pClient->SetData(&aSendData[0], sizeof(int) * 2 + sizeof(int) * 2);
}

//==========================================================
// 退室
//==========================================================
void Leave(int nId, CClient *pClient)
{
	int nProt = COMMAND_TYPE_DELETE;	// プロトコル番号
	char aSendData[sizeof(int) * 2] = {};	// 送信用まとめデータ

	// IDを挿入
	memcpy(&aSendData[0], &nId, sizeof(int));
	memcpy(&aSendData[sizeof(int)], &nProt, sizeof(int));

	// プロトコルを挿入
	pClient->ResetData();
	pClient->SetData(&aSendData[0], sizeof(int) * 2);
	pClient->SetDeath(true);
}

//==========================================================
// 受け入れ待ち
//==========================================================
void Accept(CServer *pServer)
{
	int nId = -1;

	while (1)
	{
		// 送受信用生成
		CClient *pClient = new CClient;
		pClient->BindSock(pServer->Accept());
		pClient->SetCliePort(pServer->GetPort());
		pClient->SetIP(pServer->GetIP());

		// ID設定
		nId++;
		pClient->SetId(nId);

		// マルチスレッド
		std::thread th(Access, pClient);
		th.detach();
	}
}

//==========================================================
// 座標設定
//==========================================================
void SetPosition(int nId, D3DXVECTOR3 pos, CClient *pClient)
{
	int nProt = COMMAND_TYPE_SETPOSITION;	// プロトコル番号
	char aSendData[sizeof(int) * 2 + sizeof(D3DXVECTOR3)] = {};	// 送信用まとめデータ

	// IDを挿入
	memcpy(&aSendData[0], &nId, sizeof(int));
	memcpy(&aSendData[sizeof(int)], &nProt, sizeof(int));
	memcpy(&aSendData[sizeof(int) * 2], &pos, sizeof(D3DXVECTOR3));

	// プロトコルを挿入
	pClient->SetData(&aSendData[0], sizeof(int) * 2 + sizeof(D3DXVECTOR3));
}

//==========================================================
// 向き設定
//==========================================================
void SetRotation(int nId, D3DXVECTOR3 pos, CClient *pClient)
{
	int nProt = COMMAND_TYPE_SETROTATION;	// プロトコル番号
	char aSendData[sizeof(int) * 2 + sizeof(D3DXVECTOR3)] = {};	// 送信用まとめデータ

	// IDを挿入
	memcpy(&aSendData[0], &nId, sizeof(int));
	memcpy(&aSendData[sizeof(int)], &nProt, sizeof(int));
	memcpy(&aSendData[sizeof(int) * 2], &pos, sizeof(D3DXVECTOR3));

	// プロトコルを挿入
	pClient->SetData(&aSendData[0], sizeof(int) * 2 + sizeof(D3DXVECTOR3));
}

//==========================================================
// ダメージ設定
//==========================================================
void SetDamage(int nId, int nDamage, CClient *pClient)
{
	int nProt = COMMAND_TYPE_DAMAGE;	// プロトコル番号
	char aSendData[sizeof(int) * 2 + sizeof(int)] = {};	// 送信用まとめデータ

	// IDを挿入
	memcpy(&aSendData[0], &nId, sizeof(int));
	memcpy(&aSendData[sizeof(int)], &nProt, sizeof(int));
	memcpy(&aSendData[sizeof(int) * 2], &nDamage, sizeof(int));

	// プロトコルを挿入
	pClient->SetData(&aSendData[0], sizeof(int) * 2 + sizeof(int));
}

//==========================================================
// 準備完了送信
//==========================================================
void SetUp(int nId, CClient *pClient)
{
	int nProt = COMMAND_TYPE_START_OK;	// プロトコル番号
	char aSendData[sizeof(int) * 2] = {};	// 送信用まとめデータ

	// IDを挿入
	memcpy(&aSendData[0], &nId, sizeof(int));
	memcpy(&aSendData[sizeof(int)], &nProt, sizeof(int));

	// プロトコルを挿入
	pClient->SetData(&aSendData[0], sizeof(int) * 2);
}

//==========================================================
// 体力設定
//==========================================================
void SetLife(int nId, int nLife, CClient *pClient)
{
	int nProt = COMMAND_TYPE_SETLIFE;	// プロトコル番号
	char aSendData[sizeof(int) * 2 + sizeof(int)] = {};	// 送信用まとめデータ

	// IDを挿入
	memcpy(&aSendData[0], &nId, sizeof(int));
	memcpy(&aSendData[sizeof(int)], &nProt, sizeof(int));
	memcpy(&aSendData[sizeof(int) * 2], &nLife, sizeof(int));

	// プロトコルを挿入
	pClient->SetData(&aSendData[0], sizeof(int) * 2 + sizeof(int));
}

//==========================================================
// 情報を送信
//==========================================================
void Send(CServer **ppServer)
{
	DWORD dwCurrentTime;	//現在時刻
	DWORD dwExecLastTime;	//最後に処理した時刻
	DWORD dwFrameCount;		//フレームカウント
	DWORD dwFPSLastTime;	//最後にFPSを計測した時刻
	DWORD dwSecondCount;	// 1秒カウント

	//分解能を設定
	timeBeginPeriod(1);

	dwCurrentTime = 0;	//初期化
	dwExecLastTime = timeGetTime();	//現在時刻を取得(保存)
	dwFrameCount = 0;	//初期化
	dwFPSLastTime = timeGetTime();	//現在時刻を取得(保存)
	dwSecondCount = 0;

	// サーバーが閉じられるまで繰り返し
	while (*ppServer != NULL)
	{
		dwCurrentTime = timeGetTime();	//現在時刻を取得
		if ((dwCurrentTime - dwFPSLastTime) >= 500)
		{//0.5秒経過
			//FPSを計測
			dwFPSLastTime = dwCurrentTime;	//FPSを測定した時刻を保存
			dwFrameCount = 0;	//フレームカウントをクリア
		}

		if ((dwCurrentTime - dwExecLastTime) >= (1000 / 120))
		{//60分の1秒経過

			// データの合成
			CClient *pClient = CClient::GetTop();	// 先頭を取得

			while (pClient != NULL)
			{// 使用されていない状態まで

				CClient *pClientNext = pClient->GetNext();	// 次のオブジェクトへのポインタを取得

				pClient->SetSend(true);	// 書き換え不可能な状態にする

				memcpy(&g_aSendData[g_nSendByte], pClient->GetSendData(), pClient->GetSendByte());
				g_nSendByte += pClient->GetSendByte();

				if (pClient->GetDeath() == false)
				{
					pClient->ResetData();	// データのリセット
					pClient->SetSend(false);	// 書き換え可能な状態にする
				}
				else
				{
					if (pClient != NULL)
					{
						pClient->Uninit();
						delete pClient;
						pClient = NULL;
					}
				}

				pClient = pClientNext;	// 次のオブジェクトに移動
			}

			//printf("%d,", g_nSendByte);

			pClient = CClient::GetTop();	// 先頭を取得

			while (pClient != NULL)
			{// 使用されていない状態まで

				CClient *pClientNext = pClient->GetNext();	// 次のオブジェクトへのポインタを取得

				// 送信
				pClient->Send(&g_aSendData[0], g_nSendByte);

				pClient = pClientNext;	// 次のオブジェクトに移動
			}

			// 送信データをクリア
			memset(&g_aSendData[0], '\0', sizeof(g_aSendData));
			g_nSendByte = 0;

			dwExecLastTime = dwCurrentTime;	//処理開始の時刻[現在時刻]を保存
			dwFrameCount++;	//フレームカウントを加算
			dwSecondCount++;
		}
	}
}

//==========================================================
// ゴール送信
//==========================================================
void Goal(int nId, CClient *pClient)
{
	int nProt = COMMAND_TYPE_GOAL;	// プロトコル番号
	char aSendData[sizeof(int) * 2] = {};	// 送信用まとめデータ

	// IDを挿入
	memcpy(&aSendData[0], &nId, sizeof(int));
	memcpy(&aSendData[sizeof(int)], &nProt, sizeof(int));

	// プロトコルを挿入
	pClient->SetData(&aSendData[0], sizeof(int) * 2);
}
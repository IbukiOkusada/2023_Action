//===============================================
//
// ゲーム画面の管理処理 [game.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
#include "texture.h"
#include "objectX.h"
#include "Xfile.h"
#include "input.h"
#include "fade.h"
#include "pause.h"
#include "result.h"
#include "sound.h"
#include "fileload.h"
#include "debugproc.h"
#include "player.h"
#include "editor.h"
#include "gimmick_move.h"
#include "gimmick_rotate.h"
#include "gimmick_fish.h"
#include "time.h"
#include "meshdome.h"
#include "gimmick_button.h"
#include "gimmick_air.h"
#include "gimmick_door.h"
#include "countdown.h"
#include "tcp_client.h"
#include <thread>
#include "protocol_online.h"
#include "object2D.h"

// グローバル

//===============================================
// マクロ定義
//===============================================
#define START_TIME	(60 * 4)	// 制限時間
#define START_SCORE	(0)		// 開始スコア
#define MAP_SIZE	(100.0f)	// マップサイズ
#define STARTSET_NUMENEMY	(3)	// 開始時に配置する敵の数
#define MORNING_TIME	(60 * 6)	// 早朝終了時刻
#define AM_COMRUSH	(60 * 9)	// 通勤ラッシュ
#define RELAX_TIME	(60 * 11)	// リラックスタイム
#define NOON_TIME	(60 * 12.5)	// 昼休み
#define EVENT_RUSH	(60 * 15.5)	// イベント
#define PM_RELAX_TIME	(60 * 16)	// 帰宅開始時間
#define PM_GOHOME_RUSH	(60 * 19)	// 帰宅ラッシュ
#define MAX_TIME	(60 * 20 + 10)	// 最大時間
#define SPEED_UP	(30.0f)
#define DEF_PORT	(22333)	// ポート番号
#define MAX_STRING	(2048)
#define ADDRESSFILE	"data\\TXT\\address.txt"

//===============================================
// 静的メンバ変数
//===============================================
CGame::STATE CGame::m_state = CGame::STATE_MULTI;	// 状態

//===============================================
// コンストラクタ
//===============================================
CGame::CGame()
{
	// 値のクリア
	m_pMapCamera = NULL;
	m_pTime = NULL;
	m_pPlayer = NULL;
	m_pFileLoad = NULL;
	m_pEditor = NULL;
	m_pMeshDome = NULL;
	m_pStart = NULL;
	m_pPause = NULL;
	m_pScore = NULL;
	m_pClient = NULL;
	m_pCountDown = NULL;
	m_nSledCnt = 0;
	m_bEnd = false;
}

//===============================================
// デストラクタ
//===============================================
CGame::~CGame()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CGame::Init(void)
{
	memset(&m_aAddress[0], '\0', sizeof(m_aAddress));
	int nErr = WSAStartup(WINSOCK_VERSION, &m_wsaData);	// winsockの初期化関数

	if (nErr != 0)
	{// 初期化に失敗した場合
		
	}

	// 外部ファイル読み込みの生成
	if (nullptr == m_pFileLoad)
	{// 使用していない場合
		m_pFileLoad = new CFileLoad;

		if (m_pFileLoad != NULL)
		{
			m_pFileLoad->Init();
			m_pFileLoad->OpenFile("data\\TXT\\model.txt");
		}
	}

	// エディターの生成
	if (nullptr == m_pEditor)
	{
		m_pEditor = new CEditor;
		m_pEditor->Init();
	}

	// マップの生成
	CObject2D *pObj = CObject2D::Create();
	pObj->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\map.png"));
	pObj->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.95f, 0.0f));
	pObj->SetSize(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.05f);

	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, -150.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pPlayer->SetUp(true);
	m_pPlayer->SetType(CPlayer::TYPE_SEND);
	m_pCountDown = CCountDown::Create();

	m_pMeshDome = CMeshDome::Create(D3DXVECTOR3(-8000.0f, -300.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10000.0f, 10.0f, 10, 10);

	if (m_state == STATE_MULTI)
	{// マルチの場合

		m_pClient = new CClient;
		AddressLoad(&m_aAddress[0]);

		if (m_pClient->Init(&m_aAddress[0], DEF_PORT))
		{// 初期接続成功
			// マルチスレッド
			std::thread th(&CGame::Online, this);
			th.detach();

			m_pTime = CTime::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.4f, SCREEN_HEIGHT * 0.075f, 0.0f));
			m_pTime->Set(0);
			m_pTime->SetMode(CTime::MODE_PLUS);
		}
		else
		{// 接続失敗

			// タイムアタックに変更
			m_state = STATE_TIMEATTACK;
			m_pTime = CTime::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.4f, SCREEN_HEIGHT * 0.075f, 0.0f));
			m_pTime->Set(180 * 100);
		}
	}
	else
	{
		m_pTime = CTime::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.4f, SCREEN_HEIGHT * 0.075f, 0.0f));
		m_pTime->Set(180 * 100);
	}
	
	// ギミック設置
	GimmickSet();

	//カメラ初期化
	{
		CManager::GetInstance()->GetCamera()->Init();
		D3DVIEWPORT9 viewport;
		//プレイヤー追従カメラの画面位置設定
		viewport.X = 0;
		viewport.Y = 0;
		viewport.Width = (DWORD)(SCREEN_WIDTH * 1.0f);
		viewport.Height = (DWORD)(SCREEN_HEIGHT * 1.0f);
		viewport.MinZ = 0.0f;
		viewport.MaxZ = 1.0f;
		CManager::GetInstance()->GetCamera()->SetViewPort(viewport);
	}

	// ポーズの生成
	if (nullptr == m_pPause)
	{
		m_pPause = CPause::Create();
	}

	// スポットライトをオン
	CManager::GetInstance()->GetLight()->EnablePointLight(true);

	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GAME);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CGame::Uninit(void)
{
	m_pPlayer = nullptr;
	m_bEnd = true;

	while (1)
	{
		OnlineEnd();
		if (m_nSledCnt <= 0)
		{
			break;
		}
	}

	if (m_pFileLoad != NULL)
	{
		m_pFileLoad->Uninit();

		delete m_pFileLoad;		// メモリの開放
		m_pFileLoad = NULL;
	}

	if (m_pPause != NULL)
	{
		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = NULL;
	}

	if (m_pTime != NULL)
	{
		m_pTime->Uninit();
		delete m_pTime;
		m_pTime = NULL;
	}

	if (m_pClient != NULL)
	{
		m_pClient->Uninit();
		delete m_pClient;
		m_pClient = NULL;
	}

	if (m_pCountDown != nullptr)
	{
		m_pCountDown->Uninit();
		delete m_pCountDown;
		m_pCountDown = nullptr;
	}

	//Winsock終了処理
	WSACleanup();	// WSACleanup関数 : winsockの終了処理

}

//===============================================
// 更新処理
//===============================================
void CGame::Update(void)
{
	if (m_pCountDown != nullptr)
	{
		CScene::Update();

		int nSetUp = 0;

		CPlayer *pPlayer = NULL;		// 先頭を取得
		CPlayer *pPlayerNext = NULL;	// 次を保持
		pPlayer = CPlayer::GetTop();	// 先頭を取得

		while (pPlayer != NULL)
		{// 使用されている間繰り返し
			pPlayerNext = pPlayer->GetNext();	// 次を保持

			if (pPlayer->GetSetUp() == true)
			{
				nSetUp++;
			}

			pPlayer = pPlayerNext;	// 次に移動
		}

		SendSetUp();

		if ((m_state == STATE_TIMEATTACK && m_pPlayer->GetSetUp() == true) || (m_state == STATE_MULTI && nSetUp >= 2 && CPlayer::GetNum() >= 2))
		{
			if (!m_pCountDown->GetEnd())
			{
				m_pCountDown->Update();
				return;
			}
			else
			{
				m_pPlayer->SetType(CPlayer::TYPE_ACTIVE);
				if (m_pTime != nullptr)
				{
					m_pTime->SetActive(true);
				}
				if (m_pCountDown != nullptr)
				{
					m_pCountDown->Uninit();
					delete m_pCountDown;
					m_pCountDown = nullptr;
				}
			}
		}
		else
		{
			return;
		}
	}

	// ポーズ
	if (m_pPause != NULL)
	{
		m_pPause->Update();

		if (m_pPause->GetEnable() == true)
		{
			if (m_pPause->GetSelect() == true)
			{
				CManager::GetInstance()->GetFade()->Update();
			}
			return;
		}
	}

#if _DEBUG

	if (m_pEditor != NULL)
	{
		m_pEditor->Update();

		if (m_pEditor->GetActive())
		{
			return;
		}
	}

#endif

	if (m_pTime != NULL)
	{
		m_pTime->Update();

		if (m_pTime->GetNum() <= 0 && m_state == STATE_TIMEATTACK)
		{
			CManager::GetInstance()->GetFade()->Set(CScene::MODE_RESULT);
			CResult::SetScore(m_pTime->GetNum());
		}
	}

	// ゴール判定
	if (m_pPlayer != NULL)
	{
		if (m_pPlayer->GetPosition().x < -15000.0f)
		{
			// マルチプレイ
			if (m_state == STATE_MULTI && CResult::GetType() == CResult::TYPE_MAX)
			{
				int nSetUp = 0;

				CPlayer *pPlayer = NULL;		// 先頭を取得
				CPlayer *pPlayerNext = NULL;	// 次を保持
				pPlayer = CPlayer::GetTop();	// 先頭を取得

				while (pPlayer != NULL)
				{// 使用されている間繰り返し
					pPlayerNext = pPlayer->GetNext();	// 次を保持

					if (pPlayer->GetGoal())
					{
						nSetUp++;
					}

					pPlayer = pPlayerNext;	// 次に移動
				}

				if (nSetUp == 0)
				{
					CResult::SetType(CResult::TYPE_MULTI_WIN);
				}
				else
				{
					CResult::SetType(CResult::TYPE_MULTI_LOSE);
				}
			}

			m_pPlayer->SetGoal(true);
			if (m_pTime != nullptr)
			{
				CResult::SetScore(m_pTime->GetNum());
				m_pTime->SetActive(false);
			}
			SendGoal();
		}
	}
	{
		int nSetUp = 0;

		CPlayer *pPlayer = NULL;		// 先頭を取得
		CPlayer *pPlayerNext = NULL;	// 次を保持
		pPlayer = CPlayer::GetTop();	// 先頭を取得

		while (pPlayer != NULL)
		{// 使用されている間繰り返し
			pPlayerNext = pPlayer->GetNext();	// 次を保持

			if (pPlayer->GetGoal())
			{
				nSetUp++;
			}

			pPlayer = pPlayerNext;	// 次に移動
		}

		SendSetUp();

		if ((m_state == STATE_TIMEATTACK && m_pPlayer->GetGoal() == true) || (m_state == STATE_MULTI && nSetUp >= 2 && CPlayer::GetNum() >= 2))
		{
			CManager::GetInstance()->GetFade()->Set(CScene::MODE_RESULT);
			m_bEnd = true;
		}
	}

	if (!m_bEnd)
	{
		// 更新処理
		CScene::Update();
	}
	else
	{
		CManager::GetInstance()->GetFade()->Update();
	}
}

//===============================================
// 描画処理
//===============================================
void CGame::Draw(void)
{
	CScene::Draw();

	// ポーズ
	if (m_pPause != NULL)
	{
		m_pPause->Draw();
	}
}

//===================================================
// スコアの取得
//===================================================
CScore *CGame::GetScore(void)
{
	return m_pScore;
}

//===================================================
// スコアの取得
//===================================================
CTime *CGame::GetTime(void)
{
	return m_pTime;
}

//===================================================
// プレイヤーの取得
//===================================================
CPlayer *CGame::GetPlayer(void)
{
	return m_pPlayer;
}

//===================================================
// ファイル読み込みの取得
//===================================================
CFileLoad *CGame::GetFileLoad(void)
{
	return m_pFileLoad;
}

//===================================================
// ポーズの取得
//===================================================
CPause *CGame::GetPause(void)
{
	return m_pPause;
}

//===================================================
// エディター情報の取得
//===================================================
CEditor *CGame::GetEditor(void)
{
	return m_pEditor;
}

//===================================================
// ギミックの設置
//===================================================
void CGame::GimmickSet(void)
{
	// 移動
	CGimmickMove *pMove = CGimmickMove::Create(D3DXVECTOR3(-7300.0f, 0.0f, -2100.0f), D3DXVECTOR3(0.0f, 0.0f, 2.0f), 300.0f);
	pMove = CGimmickMove::Create(D3DXVECTOR3(-7800.0f, 0.0f, -1600.0f), D3DXVECTOR3(0.0f, 0.0f, -3.0f), 120.0f);
	pMove = CGimmickMove::Create(D3DXVECTOR3(-8000.0f, 0.0f, -1980.0f), D3DXVECTOR3(-8.0f, 0.0f, 0.0f), 60.0f);
	pMove = CGimmickMove::Create(D3DXVECTOR3(-8400.0f, 0.0f, -1500.0f), D3DXVECTOR3(4.0f, 0.0f, 0.0f), 100.0f);
	pMove = CGimmickMove::Create(D3DXVECTOR3(-9400.0f, 0.0f, -2100.0f), D3DXVECTOR3(-4.0f, 0.0f, 4.0f), 150.0f);
	pMove = CGimmickMove::Create(D3DXVECTOR3(-9400.0f, 0.0f, -1400.0f), D3DXVECTOR3(-6.0f, 0.0f, -6.0f), 100.0f);
	pMove = CGimmickMove::Create(D3DXVECTOR3(-14100.0f, 0.0f, -2250.0f), D3DXVECTOR3(0.0f, 0.0f, -3.0f), 100.0f);

	// 回転
	CGimmickRotate *p = CGimmickRotate::Create();
	p->SetPosition(D3DXVECTOR3(-5625.0f, 0.0f, -350.0f));
	p->SetRotate(p->GetRotate() * 1.0f);

	p = CGimmickRotate::Create();
	p->SetPosition(D3DXVECTOR3(-6900.0f, 0.0f, -850.0f));
	p->SetRotate(p->GetRotate() * 0.8f);
	p->SetRotation(D3DXVECTOR3(0.0f, D3DX_PI * 0.1f, 0.0f));

	p = CGimmickRotate::Create();
	p->SetPosition(D3DXVECTOR3(-7900.0f, 0.0f, -850.0f));
	p->SetRotate(p->GetRotate() * -1.0f);

	p = CGimmickRotate::Create();
	p->SetPosition(D3DXVECTOR3(-8900.0f, 0.0f, -850.0f));
	p->SetRotate(p->GetRotate() * 1.2f);
	p->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	p = CGimmickRotate::Create();
	p->SetPosition(D3DXVECTOR3(-8900.0f, 0.0f, -850.0f));
	p->SetRotate(p->GetRotate() * 1.2f);
	p->SetRotation(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));

	p = CGimmickRotate::Create();
	p->SetPosition(D3DXVECTOR3(-9900.0f, 0.0f, -850.0f));
	p->SetRotate(p->GetRotate() * -1.5f);
	p->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	p = CGimmickRotate::Create();
	p->SetPosition(D3DXVECTOR3(-9900.0f, 0.0f, -850.0f));
	p->SetRotate(p->GetRotate() * -1.5f);
	p->SetRotation(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));

	p = CGimmickRotate::Create();
	p->SetPosition(D3DXVECTOR3(-12850.0f, 0.0f, -2125.0f));
	p->SetRotate(p->GetRotate() * 1.0f);
	p->SetRotation(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));

	p = CGimmickRotate::Create();
	p->SetPosition(D3DXVECTOR3(-13550.0f, 0.0f, 960.0f));
	p->SetRotate(p->GetRotate() * 1.0f);
	p->SetRotation(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));

	// 魚
	CGimmickFish *pFish = CGimmickFish::Create();
	pFish->SetPosition(D3DXVECTOR3(-850.0f, -750.0f, 150.0f));
	pFish->SetRotation(D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f));

	pFish = CGimmickFish::Create();
	pFish->SetPosition(D3DXVECTOR3(-3950.0f, -750.0f, -650.0f));

	pFish = CGimmickFish::Create();
	pFish->SetPosition(D3DXVECTOR3(-8800.0f, -750.0f, -1750.0f));

	pFish = CGimmickFish::Create();
	pFish->SetPosition(D3DXVECTOR3(-14400.0f, -750.0f, 650.0f));
	pFish->SetRotation(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));

	pFish = CGimmickFish::Create();
	pFish->SetPosition(D3DXVECTOR3(-12700.0f, -750.0f, 275.0f));
	pFish->SetRotation(D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f));

	// ボタン配置
	CGimmickButton *pButton = CGimmickButton::Create();
	pButton->SetPosition(D3DXVECTOR3(-14640.0f, 0.0f, -2050.0f));
	pButton->SetRotation(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	pButton->SetGoalPos(D3DXVECTOR3(-14680.0f, 0.0f, -2050.0f));
	pButton->SetType(CGimmickButton::TYPE_REVERSE);

	pButton = CGimmickButton::Create();
	pButton->SetPosition(D3DXVECTOR3(-14400.0f, 0.0f, 110.0f));
	pButton->SetRotation(D3DXVECTOR3(0.0f, D3DX_PI * 0.0f, 0.0f));
	pButton->SetGoalPos(D3DXVECTOR3(-14400.0f, 0.0f, 70.0f));
	pButton->SetType(CGimmickButton::TYPE_DOOR);

	// ドア配置
	CGimmickDoor *pDoor = CGimmickDoor::Create();
	pDoor->SetPosition(D3DXVECTOR3(-13810.0f, 0.0f, -750.0f));
	pDoor->SetRotation(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));

	// 風配置
	CGimmickAir *pAir = CGimmickAir::Create();
	pAir->SetPosition(D3DXVECTOR3(-13525.0f, 80.0f, -800.0f));
	pAir->SetMove(D3DXVECTOR3(0.0f, 0.0f, -0.5f));
	pAir->SetHeight(750.0f);
	pAir->SetWidth(250.0f);

	pAir = CGimmickAir::Create();
	pAir->SetPosition(D3DXVECTOR3(-12800.0f, 80.0f, -1325.0f));
	pAir->SetMove(D3DXVECTOR3(0.5f, 0.0f, 0.0f));
	pAir->SetHeight(225.0f);
	pAir->SetWidth(950.0f);

	pAir = CGimmickAir::Create();
	pAir->SetPosition(D3DXVECTOR3(-12800.0f, 80.0f, -275.0f));
	pAir->SetMove(D3DXVECTOR3(-0.5f, 0.0f, 0.0f));
	pAir->SetHeight(225.0f);
	pAir->SetWidth(950.0f);

	pAir = CGimmickAir::Create();
	pAir->SetPosition(D3DXVECTOR3(-14000.0f, 80.0f, -1875.0f));
	pAir->SetMove(D3DXVECTOR3(0.75f, 0.0f, 0.0f));
	pAir->SetHeight(225.0f);
	pAir->SetWidth(250.0f);
	pAir->SetRevease(true);

	pAir = CGimmickAir::Create();
	pAir->SetPosition(D3DXVECTOR3(-13935.0f, 80.0f, 265.0f));
	pAir->SetMove(D3DXVECTOR3(0.85f, 0.0f, 0.0f));
	pAir->SetHeight(205.0f);
	pAir->SetWidth(185.0f);
	pAir->SetRevease(true);
}

//===================================================
// オンライン通信
//===================================================
void CGame::Online(void)
{
	m_nSledCnt = 1;
	while (1)
	{
		if (m_pPlayer == NULL || m_bEnd == true)
		{
			break;
		}

		char aRecvData[MAX_STRING] = {};	// 受信用

		// 受信
		int nRecvByte = m_pClient->Recv(&aRecvData[0], MAX_STRING);

		// マルチスレッド
		std::thread th(&CGame::ByteCheck, this, &aRecvData[0], nRecvByte);
		th.detach();
	}

	m_nSledCnt--;
}

//===================================================
// 受信確認
//===================================================
void CGame::ByteCheck(char *pRecvData, int nRecvByte)
{
	m_mutex.lock();
	m_nSledCnt++;
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	int nType = COMMAND_TYPE_NONE;	// コマンド番号
	int nId = -1;		// ID
	int nByte = 0;	// バイト数
	int nDamage = 0;	// ダメージ数
	CPlayer *pPlayer = NULL;		// 先頭を取得
	CPlayer *pPlayerNext = NULL;	// 次を保持

	if (nRecvByte <= 0)
	{
		m_nSledCnt--;
		m_mutex.unlock();
		return;
	}

	int nSetUp = 0;
	pPlayer = CPlayer::GetTop();	// 先頭を取得

	while (pPlayer != NULL)
	{// 使用されている間繰り返し
		pPlayerNext = pPlayer->GetNext();	// 次を保持

		if (pPlayer->GetGoal())
		{
			nSetUp++;
		}

		pPlayer = pPlayerNext;	// 次に移動
	}

	if (nSetUp >= 2)
	{
		m_nSledCnt--;
		m_mutex.unlock();
		return;
	}

	// 終端文字まで確認する
	while (nByte < nRecvByte)
	{
		bool bIn = false;	// 既にいるかどうか
		memcpy(&nId, &pRecvData[nByte], sizeof(int));
		nByte += sizeof(int);

		// コマンド取得
		memcpy(&nType, &pRecvData[nByte], sizeof(int));
		nByte += sizeof(int);

		if (nId != -1)
		{
			// コマンドごとに分ける
			switch (nType)
			{

			case COMMAND_TYPE_SETPOSITION:

				memcpy(&pos, &pRecvData[nByte], sizeof(D3DXVECTOR3));
				nByte += sizeof(D3DXVECTOR3);
				break;

			case COMMAND_TYPE_SETROTATION:

				memcpy(&pos, &pRecvData[nByte], sizeof(D3DXVECTOR3));
				nByte += sizeof(D3DXVECTOR3);
				break;

			case COMMAND_TYPE_SETLIFE:

				memcpy(&nDamage, &pRecvData[nByte], sizeof(int));
				nByte += sizeof(int);
				break;

			case COMMAND_TYPE_START_OK:

				break;

			case COMMAND_TYPE_CREATE:
				break;

			case COMMAND_TYPE_DAMAGE:

				memcpy(&nDamage, &pRecvData[nByte], sizeof(int));
				nByte += sizeof(int);
				break;

			case COMMAND_TYPE_DELETE:
				break;

			case COMMAND_TYPE_GETID:
				break;
			}
		}

		if (m_pPlayer == NULL)
		{
			m_nSledCnt--;
			m_mutex.unlock();
			return;
		}

		if (nId != -1 && m_pPlayer->GetId() != -1)
		{// IDを受信できた

			pPlayer = CPlayer::GetTop();	// 先頭を取得

			while (pPlayer != NULL)
			{// 使用されている間繰り返し
				pPlayerNext = pPlayer->GetNext();	// 次を保持

				if (nId == pPlayer->GetId() && m_pPlayer->GetId() != nId)
				{// 自分以外かつ操作していない

				 // コマンドごとに分ける
					switch (nType)
					{
					case COMMAND_TYPE_SETPOSITION:

						pPlayer->SetDiffPosition(pos);
						break;

					case COMMAND_TYPE_SETROTATION:

						pPlayer->SetRotation(pos);
						break;

					case COMMAND_TYPE_SETLIFE:

						pPlayer->SetLife(nDamage);
						break;

					case COMMAND_TYPE_START_OK:

						pPlayer->SetUp(true);
						break;

					case COMMAND_TYPE_CREATE:

						break;

					case COMMAND_TYPE_DAMAGE:

						pPlayer->Damage(nDamage);
						break;

					case COMMAND_TYPE_GOAL:

						if (m_pPlayer != NULL)
						{
							if (!m_pPlayer->GetGoal())
							{
								pPlayer->SetGoal(true);
							}
							else
							{
								pPlayer->SetGoalValue(true);
							}
						}

						break;

					case COMMAND_TYPE_DELETE:

						pPlayer->Uninit();
						break;

					case COMMAND_TYPE_GETID:

						break;
					}

					bIn = true;	// いる状態にする
					break;
				}
				else if (nId == pPlayer->GetId() && m_pPlayer->GetId() == nId)
				{// 自分以外かつ操作キャラ

					bIn = true;	// いる状態にする
					break;
				}

				pPlayer = pPlayerNext;	// 次に移動
			}

			if (bIn == false && m_pPlayer->GetId() != -1 && nType > COMMAND_TYPE_NONE && nType < COMMAND_TYPE_MAX)
			{// まだ存在していない場合
				pPlayer = CPlayer::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				pPlayer->BindId(nId);
				pPlayer->SetType(CPlayer::TYPE_NONE);
			}
		}
		else if (nId == -1 && m_pPlayer->GetId() == -1)
		{// IDが受信できていないかつ自分自身のIDも存在していない
			nId = nType;

			// 自分のIDを設定
			m_pPlayer->BindId(nId);

			break;
		}
	}

	m_nSledCnt--;
	m_mutex.unlock();
}

//===================================================
// 座標送信
//===================================================
void CGame::SendPosition(const D3DXVECTOR3& pos)
{
	if (m_pClient != nullptr)
	{
		char aSendData[MAX_STRING] = {};	// 送信用
		int nProt = COMMAND_TYPE_SETPOSITION;

		// protocolを挿入
		memcpy(&aSendData[0], &nProt, sizeof(int));

		// 座標を挿入
		memcpy(&aSendData[sizeof(int)], &pos, sizeof(D3DXVECTOR3));

		// 送信
		m_pClient->Send(&aSendData[0], sizeof(int) + sizeof(D3DXVECTOR3));
	}
}

//===================================================
// 向き送信
//===================================================
void CGame::SendRotation(const D3DXVECTOR3& rot)
{
	if (m_pClient != nullptr)
	{
		char aSendData[MAX_STRING] = {};	// 送信用
		int nProt = COMMAND_TYPE_SETROTATION;

		// protocolを挿入
		memcpy(&aSendData[0], &nProt, sizeof(int));

		// 座標を挿入
		memcpy(&aSendData[sizeof(int)], &rot, sizeof(D3DXVECTOR3));

		// 送信
		m_pClient->Send(&aSendData[0], sizeof(int) + sizeof(D3DXVECTOR3));
	}
}

//===================================================
// ダメージ送信
//===================================================
void CGame::SendDamage(const int nDamage)
{
	if (m_pClient != nullptr)
	{
		char aSendData[MAX_STRING] = {};	// 送信用
		int nProt = COMMAND_TYPE_DAMAGE;

		// protocolを挿入
		memcpy(&aSendData[0], &nProt, sizeof(int));

		// ダメージを挿入
		memcpy(&aSendData[sizeof(int)], &nDamage, sizeof(int));

		// 送信
		m_pClient->Send(&aSendData[0], sizeof(int) + sizeof(int));
	}
}

//===================================================
// 体力送信
//===================================================
void CGame::SendLife(const int nLife)
{
	if (m_pClient != nullptr)
	{
		char aSendData[MAX_STRING] = {};	// 送信用
		int nProt = COMMAND_TYPE_SETLIFE;

		// protocolを挿入
		memcpy(&aSendData[0], &nProt, sizeof(int));

		// ダメージを挿入
		memcpy(&aSendData[sizeof(int)], &nLife, sizeof(int));

		// 送信
		m_pClient->Send(&aSendData[0], sizeof(int) + sizeof(int));
	}
}

//===================================================
// 終了送信
//===================================================
void CGame::OnlineEnd(void)
{
	if (m_pClient != nullptr)
	{
		char aSendData[MAX_STRING] = {};	// 送信用
		int nProt = COMMAND_TYPE_DELETE;

		// protocolを挿入
		memcpy(&aSendData[0], &nProt, sizeof(int));

		// 送信
		m_pClient->Send(&aSendData[0], sizeof(int));
	}
}

//===============================================
// 体力設定
//===============================================
void CGame::SendSetUp(void)
{
	if (m_pClient != nullptr)
	{
		char aSendData[MAX_STRING] = {};	// 送信用
		int nProt = COMMAND_TYPE_START_OK;

		// protocolを挿入
		memcpy(&aSendData[0], &nProt, sizeof(int));

		// 送信
		m_pClient->Send(&aSendData[0], sizeof(int));
	}
}

//===============================================
// ゴール送信
//===============================================
void CGame::SendGoal(void)
{
	if (m_pClient != nullptr)
	{
		char aSendData[MAX_STRING] = {};	// 送信用
		int nProt = COMMAND_TYPE_GOAL;

		// protocolを挿入
		memcpy(&aSendData[0], &nProt, sizeof(int));

		// 送信
		m_pClient->Send(&aSendData[0], sizeof(int));
	}
}

//===================================================
// アドレス読み込み
//===================================================
void CGame::AddressLoad(char *pAddrss)
{
	FILE *pFile;	// ファイルへのポインタ

	pFile = fopen(ADDRESSFILE, "r");

	if (pFile != NULL)
	{//ファイルが開けた場合
		
		//テキスト読み込み
		fscanf(pFile, "%s", pAddrss);

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		return;
	}
}

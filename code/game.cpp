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

//===============================================
// 静的メンバ変数
//===============================================
CScore *CGame::m_pScore = NULL;		// スコアのポインタ
CMeshField *CGame::m_pMeshField = NULL;	// メッシュフィールドのポインタ
CPause *CGame::m_pPause = NULL;		// ポーズのポインタ

//===============================================
// コンストラクタ
//===============================================
CGame::CGame()
{
	// 値のクリア
	m_pMapCamera = NULL;
	m_pMapThermo = NULL;
	m_nMaxEnemy = 0;
	m_pTime = NULL;
	m_pEnemyManager = NULL;
	m_pPlayer = NULL;
	m_pFileLoad = NULL;
	m_pEditor = NULL;
	m_pCarManager = NULL;
	m_pMeshDome = NULL;
	m_pStart = NULL;

	for (int nCnt = 0; nCnt < NUM_FILTER; nCnt++)
	{
		m_apFilter[nCnt] = NULL;
	}
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
	// 外部ファイル読み込みの生成
	if (m_pFileLoad == NULL)
	{// 使用していない場合
		m_pFileLoad = new CFileLoad;

		if (m_pFileLoad != NULL)
		{
			m_pFileLoad->Init();
			m_pFileLoad->OpenFile("data\\TXT\\model.txt");
		}
	}

	// エディターの生成
	if (m_pEditor == NULL)
	{
		m_pEditor = new CEditor;
		m_pEditor->Init();
	}

	// ポーズの生成
	if (m_pPause == NULL)
	{
		m_pPause = CPause::Create();
	}

	CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), NULL, NULL);
	CGimmickMove::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), 120.0f);
	CGimmickRotate *p = CGimmickRotate::Create();
	p->SetPosition(D3DXVECTOR3(-300.0f, 0.0f, -500.0f));

	// スポットライトをオン
	CManager::GetLight()->EnablePointLight(true);

	CManager::GetSound()->Play(CSound::LABEL_BGM_GAME);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CGame::Uninit(void)
{
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
}

//===============================================
// 更新処理
//===============================================
void CGame::Update(void)
{
	// ポーズ
	if (m_pPause != NULL)
	{
		m_pPause->Update();

		if (m_pPause->GetEnable() == true)
		{
			if (m_pPause->GetSelect() == true)
			{
				CManager::GetFade()->Update();
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

	// 更新処理
	CScene::Update();
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
// メッシュフィールドの取得
//===================================================
CMeshField *CGame::GetMeshField(void)
{
	return m_pMeshField;
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

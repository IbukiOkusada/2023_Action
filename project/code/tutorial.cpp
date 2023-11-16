//===============================================
//
// チュートリアル画面の管理処理 [tutorial.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "tutorial.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "time.h"
#include "camera.h"
#include "light.h"
#include "texture.h"
#include "meshcylinder.h"
#include "meshdome.h"
#include "fileload.h"
#include "slow.h"
#include "player.h"
#include "objectX.h"
#include "Xfile.h"
#include "input.h"
#include "fade.h"
#include "pause.h"
#include "result.h"
#include "editor.h"
#include "sound.h"
#include "game.h"
#include "debugproc.h"

//===============================================
// マクロ定義
//===============================================
#define START_TIME	(60 * 3)	// 制限時間
#define START_SCORE	(0)		// 開始スコア
#define MAP_SIZE	(100.0f)	// マップサイズ
#define STARTSET_NUMENEMY	(30)	// 開始時に配置する敵の数
#define MAX_TIME	(60 * 20)	// 最大時間

//===============================================
// 静的メンバ変数
//===============================================
CScore *CTutorial::m_pScore = NULL;		// スコアのポインタ
CSlow *CTutorial::m_pSlow = NULL;		// スロー状態へのポインタ
CMeshField *CTutorial::m_pMeshField = NULL;	// メッシュフィールドのポインタ

//===============================================
// コンストラクタ
//===============================================
CTutorial::CTutorial()
{
	// 値のクリア
	m_pTime = NULL;
	m_pEnemyManager = NULL;
	m_pPlayer = NULL;
	m_pFileLoad = NULL;
	m_pCarManager = NULL;
	m_pMeshDome = NULL;
}

//===============================================
// デストラクタ
//===============================================
CTutorial::~CTutorial()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CTutorial::Init(void)
{
	// 外部ファイル読み込みの生成
	if (m_pFileLoad == NULL)
	{// 使用していない場合
		m_pFileLoad = new CFileLoad;

		if (m_pFileLoad != NULL)
		{
			m_pFileLoad->Init();
			m_pFileLoad->OpenFile("data\\TXT\\tutorial.txt");
		}
	}

	// オブジェクト生成
	m_pMeshDome = CMeshDome::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20000.0f, 10.0f, 10, 10);
	CMeshCylinder::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 17000.0f, 100.0f, 10, 10);
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 550.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pPlayer->SetType(CPlayer::TYPE_ACTIVE);

	//カメラ初期化
	{
		CManager::GetInstance()->GetCamera()->Init();
		CManager::GetInstance()->GetCamera()->SetRotation(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, CManager::GetInstance()->GetCamera()->GetRotation().z));
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

	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_TUTORIAL);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CTutorial::Uninit(void)
{
	if (m_pFileLoad != NULL)
	{
		m_pFileLoad->Uninit();

		delete m_pFileLoad;		// メモリの開放
		m_pFileLoad = NULL;
	}

	m_pScore = NULL;		// スコアのポインタ
	m_pTime = NULL;		// タイムのポインタ
	m_pPlayer = NULL;	// プレイヤーのポインタ
	m_pMeshField = NULL;
	m_pFileLoad = NULL;
	m_pMeshDome = NULL;
}

//===============================================
// 更新処理
//===============================================
void CTutorial::Update(void)
{
	if (CManager::GetInstance()->GetInputPad()->GetTrigger(CInputPad::BUTTON_Y, 0) || CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_Y))
	{
		CManager::GetInstance()->GetFade()->Set(CScene::MODE_GAME);
		CGame::SetState(CGame::STATE_TIMEATTACK);
	}
	else if (CManager::GetInstance()->GetInputPad()->GetTrigger(CInputPad::BUTTON_B, 0) || CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_B))
	{
		CManager::GetInstance()->GetFade()->Set(CScene::MODE_GAME);
		CGame::SetState(CGame::STATE_MULTI);
	}

	CManager::GetInstance()->GetDebugProc()->Print("[START, ENTER入力でタイムアタック遷移]\n");
	CManager::GetInstance()->GetDebugProc()->Print("[BACK, BACX SPACE入力で対戦遷移]\n");

	// 更新処理
	CScene::Update();
}

//===============================================
// 描画処理
//===============================================
void CTutorial::Draw(void)
{
	CScene::Draw();
}

//===================================================
// スコアの取得
//===================================================
CScore *CTutorial::GetScore(void)
{
	return m_pScore;
}

//===================================================
// スコアの取得
//===================================================
CTime *CTutorial::GetTime(void)
{
	return m_pTime;
}

//===================================================
// プレイヤーの取得
//===================================================
CPlayer *CTutorial::GetPlayer(void)
{
	return m_pPlayer;
}

//===================================================
// スロー情報の取得
//===================================================
CSlow *CTutorial::GetSlow(void)
{
	return m_pSlow;
}

//===================================================
// メッシュフィールドの取得
//===================================================
CMeshField *CTutorial::GetMeshField(void)
{
	return m_pMeshField;
}

//===================================================
// ファイル読み込みの取得
//===================================================
CFileLoad *CTutorial::GetFileLoad(void)
{
	return m_pFileLoad;
}

//===================================================
// 読み込みデータリセット
//===================================================
void CTutorial::DataReset(void)
{
	// 終了
	if (m_pFileLoad != NULL)
	{
		m_pFileLoad->Uninit();

		delete m_pFileLoad;		// メモリの開放
		m_pFileLoad = NULL;
	}

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
}

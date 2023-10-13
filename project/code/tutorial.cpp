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
#include "objectX.h"
#include "Xfile.h"
#include "input.h"
#include "fade.h"
#include "pause.h"
#include "result.h"
#include "sound.h"

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

	CManager::GetSound()->Play(CSound::LABEL_BGM_TUTORIAL);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CTutorial::Uninit(void)
{

}

//===============================================
// 更新処理
//===============================================
void CTutorial::Update(void)
{
	CInputPad *pInputPad = CManager::GetInputPad();
	CInputKeyboard *pInputKey = CManager::GetInputKeyboard();

	if (pInputKey->GetTrigger(DIK_RETURN) || pInputPad->GetTrigger(CInputPad::BUTTON_A, 0) || pInputPad->GetTrigger(CInputPad::BUTTON_START, 0))
	{
		CManager::GetFade()->Set(CScene::MODE_GAME);
	}

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

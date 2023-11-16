//===============================================
//
// タイトル画面の管理処理 [title.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "title.h"
#include "object2D.h"
#include "texture.h"
#include "manager.h"
#include "main.h"
#include "input.h"
#include "fade.h"
#include "fileload.h"
#include "meshcylinder.h"
#include "meshdome.h"
#include "light.h"
#include "sound.h"
#include "camera.h"

//===============================================
// マクロ定義
//===============================================
#define AUTOMOVE_RANKING	(1200)	// ランキング自動遷移
#define TITLE_CAMLENGTH		(1000.0f)
#define TITLE_CAMROTZ		(D3DX_PI * 0.35f)

//===============================================
// コンストラクタ
//===============================================
CTitle::CTitle()
{
	m_nTimer = 0;
	m_pFileLoad = NULL;
	m_bClick = false;
	m_fMoveCol = 0.01f;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//===============================================
// デストラクタ
//===============================================
CTitle::~CTitle()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CTitle::Init(void)
{
	CObject2D *p = CObject2D::Create(7);
	p->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\title_logo.png"));
	p->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.3f, 0.0f));
	p->SetSize(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.3f);

	m_pEnter = CObject2D::Create(7);
	m_pEnter->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\title_enter.png"));
	m_pEnter->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.8f, 0.0f));
	m_pEnter->SetSize(SCREEN_WIDTH * 0.4f, SCREEN_HEIGHT * 0.1f);

	CMeshDome::Create(D3DXVECTOR3(0.0f, -1000.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 7000.0f, 10.0f, 3, 10);

	// オブジェクト生成
	CMeshDome::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20000.0f, 10.0f, 10, 10);
	CMeshCylinder::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 17000.0f, 100.0f, 10, 10);

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

	if (CManager::GetInstance()->GetCamera() != NULL)
	{
		CManager::GetInstance()->GetCamera()->SetLength(1000.0f);
		CManager::GetInstance()->GetCamera()->SetRotation(D3DXVECTOR3(0.0f, 0.0f, TITLE_CAMROTZ));
		CManager::GetInstance()->GetCamera()->SetPositionR(D3DXVECTOR3(800.0f, 100.0f, -100.0f));
	}

	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_TITLE);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CTitle::Uninit(void)
{
	if (m_pFileLoad != NULL)
	{
		m_pFileLoad->Uninit();

		delete m_pFileLoad;		// メモリの開放
		m_pFileLoad = NULL;
	}
}

//===============================================
// 更新処理
//===============================================
void CTitle::Update(void)
{
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();
	CInputKeyboard *pInputKey = CManager::GetInstance()->GetInputKeyboard();

	if (pInputKey->GetTrigger(DIK_RETURN) || pInputPad->GetTrigger(CInputPad::BUTTON_A, 0))
	{
		CManager::GetInstance()->GetFade()->Set(CScene::MODE_TUTORIAL);

		if (m_bClick == false)
		{
			m_col.a = 1.0f;
			m_bClick = true;
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_CLICK);
		}
	}

	if (CManager::GetInstance()->GetFade()->GetState() == CFade::STATE_NONE)
	{
		m_nTimer++;

		if (m_nTimer >= AUTOMOVE_RANKING)
		{// タイトル自動遷移規定値
			CManager::GetInstance()->GetFade()->Set(CScene::MODE_RESULT);
		}
	}

	if (m_bClick == false)
	{
		m_col.a -= m_fMoveCol;

		if (m_col.a > 1.0f || m_col.a < 0.0f)
		{
			m_fMoveCol *= -1.0f;
		}
	}
	else
	{
		m_fMoveCol += 1.0f;

		if (m_fMoveCol > 2)
		{
			m_fMoveCol = 0.0f;

			m_col.a = (float)((int)m_col.a ^ 1);
		}
	}

	// エンターカラー
	if (m_pEnter != NULL)
	{
		m_pEnter->SetCol(m_col);
	}

	if (CManager::GetInstance()->GetCamera() != NULL)
	{
		CManager::GetInstance()->GetCamera()->TitleRotateCamera();
	}

	CScene::Update();
}

//===============================================
// 描画処理
//===============================================
void CTitle::Draw(void)
{
	CScene::Draw();
}

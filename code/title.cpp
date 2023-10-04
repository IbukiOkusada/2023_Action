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
#include "light.h"
#include "sound.h"
#include "camera.h"

//===============================================
// マクロ定義
//===============================================

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
	CManager::GetSound()->Play(CSound::LABEL_BGM_TITLE);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CTitle::Uninit(void)
{
	
}

//===============================================
// 更新処理
//===============================================
void CTitle::Update(void)
{
	CInputPad *pInputPad = CManager::GetInputPad();
	CInputKeyboard *pInputKey = CManager::GetInputKeyboard();

	if (pInputKey->GetTrigger(DIK_RETURN) || pInputPad->GetTrigger(CInputPad::BUTTON_A, 0))
	{
		CManager::GetFade()->Set(CScene::MODE_TUTORIAL);

		if (m_bClick == false)
		{
			m_col.a = 1.0f;
			m_bClick = true;
			CManager::GetSound()->Play(CSound::LABEL_SE_CLICK);
		}
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

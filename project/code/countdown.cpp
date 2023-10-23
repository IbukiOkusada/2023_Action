//==========================================================
//
// カウントダウン処理 [countdown.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "countdown.h"
#include "number.h"
#include "manager.h"
#include "slow.h"
#include "texture.h"
#include "object2D.h"

// マクロ定義
#define NUM_COUNT	(60)	// タイマーカウント

//==========================================================
// コンストラクタ
//==========================================================
CCountDown::CCountDown()
{
	m_nNum = 0;
	m_fDownCounter = 0.0f;
	m_bEnd = false;
	m_pObject = nullptr;
}

//==========================================================
// デストラクタ
//==========================================================
CCountDown::~CCountDown()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CCountDown::Init(void)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();
	m_nNum = COUNT_DOWN_NUM;
	m_fDownCounter = NUM_COUNT;

	m_pObject = CNumber::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), SCREEN_WIDTH * 0.05f, SCREEN_HEIGHT * 0.2f);
	m_pObject->SetIdx(m_nNum);

	if (pTexture != nullptr)
	{
		m_pObject->GetObject2D()->BindTexture(pTexture->Regist(CTexture::GetFileName(CTexture::TYPE_TIMER)));
	}

	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CCountDown::Uninit(void)
{
	if (m_pObject != nullptr)
	{
		m_pObject->Uninit();
		delete m_pObject;
		m_pObject = nullptr;
	}
}

//==========================================================
// 更新処理
//==========================================================
void CCountDown::Update(void)
{
	if (m_bEnd)
	{
		return;
	}

	m_fDownCounter -= CManager::GetInstance()->GetSlow()->Get();

	if (m_fDownCounter <= 0)
	{
		m_fDownCounter = NUM_COUNT;
		m_nNum--;

		if (m_pObject != nullptr && m_nNum >= 0)
		{
			m_pObject->SetIdx(m_nNum);
		}
	}

	if (m_nNum < 0)
	{
		m_pObject->PolygonDelete();
		m_bEnd = true;
	}
}

//==========================================================
// 生成
//==========================================================
CCountDown *CCountDown::Create(void)
{
	CCountDown *pCountDown = new CCountDown;

	if (pCountDown != NULL)
	{
		// 初期化処理
		pCountDown->Init();
	}

	return pCountDown;
}
//===============================================
//
// 数字全般の処理 [number.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "number.h"
#include "object2D.h"

// マクロ定義
#define MAX_WIDTHPATTERN	(10)	// パターン数

//===============================================
// コンストラクタ
//===============================================
CNumber::CNumber()
{
	// 値をクリアする
	m_nIdx = 0;
	m_pObject2D = NULL;
}

//===============================================
// デストラクタ
//===============================================
CNumber::~CNumber()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CNumber::Init(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	if (m_pObject2D == NULL)
	{// 使用されていない場合
		m_pObject2D = CObject2D::Create(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 7);

		if (m_pObject2D != NULL)
		{// 使用されている場合

			// 初期化
			m_pObject2D->Init();

			// サイズ設定
			m_pObject2D->SetSize(fWidth, fHeight);

			// テクスチャ設定
			SetIdx(m_nIdx);
		}

	}
	return S_OK;
}

//===============================================
// 初期化処理
//===============================================
HRESULT CNumber::Init()
{
	if (m_pObject2D == NULL)
	{// 使用されていない場合
		m_pObject2D = CObject2D::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 7);

		if (m_pObject2D != NULL)
		{// 使用されている場合

			// 初期化
			m_pObject2D->Init();

			// テクスチャ設定
			SetIdx(m_nIdx);
		}

	}
	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CNumber::Uninit(void)
{
	if (m_pObject2D != NULL)
	{// 使用されている場合
		m_pObject2D = NULL;
	}
}

//===============================================
// 更新処理
//===============================================
void CNumber::Update(void)
{
	if (m_pObject2D != NULL)
	{// 使用されている場合
		m_pObject2D->Update();
	}
}

//===============================================
// 描画処理
//===============================================
void CNumber::Draw(void)
{
	if (m_pObject2D != NULL)
	{// 使用されている場合
		// 描画処理
		m_pObject2D->Draw();
	}
}

//===============================================
// 生成
//===============================================
CNumber *CNumber::Create(void)
{
	CNumber *pNum = NULL;

	if (pNum != NULL)
	{// 使用されていない場合
		pNum = new CNumber;

		if (pNum != NULL)
		{// 使用されている場合

			// 初期化処理
			pNum->Init();
		}
	}

	return pNum;
}

//===============================================
// 生成
//===============================================
CNumber *CNumber::Create(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	CNumber *pNum = NULL;

	if (pNum == NULL)
	{// 使用されていない場合
		pNum = new CNumber;

		if(pNum != NULL)
		{// 使用されている場合

			// 初期化処理
			pNum->Init(pos, fWidth, fHeight);
		}
	}

	return pNum;
}

//===============================================
// 頂点設定
//===============================================
void CNumber::SetIdx(const int nIdx)
{
	m_nIdx = nIdx;	// 値を設定

	if (m_nIdx > 10)
	{// 値が限界を超えた場合
		m_nIdx = 9;
	}
	else if (m_nIdx < 0)
	{// 値が最低値を超えた場合
		m_nIdx = 0;
	}

	if (m_pObject2D != NULL)
	{// 使用されている場合
		// 頂点情報の設定
		m_pObject2D->SetVtx(m_nIdx, MAX_WIDTHPATTERN, 1);
	}
}
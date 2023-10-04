//===============================================
//
// エフェクトの処理 [effect.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "effect.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "explosion.h"
#include "enemy.h"
#include "texture.h"
#include "slow.h"
#include "game.h"
#include "meshfield.h"

//===============================================
// マクロ定義
//===============================================
#define LIFE	(30)		// 寿命

// 種類ごとの色
const D3DXCOLOR CEffect::m_aColInfo[TYPE_MAX]
{
	{ 1.0f, 1.0f, 1.0f, 1.0f },
	{ 0.5f, 0.5f, 1.0f, 1.0f },
};

// 種類ごとの半径
const float CEffect::m_aRadiusInfo[TYPE_MAX]
{
	{ 30.0f },
	{ 6.0f },
};

//===============================================
// コンストラクタ
//===============================================
//CEffect::CEffect()
//{
//	// 値のクリア
//	m_Info.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//	m_Info.fLife = 0;
//	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	m_Info.fRadius = 0.0f;
//	m_Info.Type = TYPE_NONE;
//}

//===============================================
// コンストラクタ(オーバーロード)
//===============================================
CEffect::CEffect(D3DXVECTOR3 pos)
{
	// 値のクリア
	m_Info.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_Info.fLife = 0;
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.fRadius = 0.0f;
	m_Info.Type = TYPE_NONE;
}

//===============================================
// コンストラクタ(オーバーロード)
//===============================================
CEffect::CEffect(int nPriority) : CObjectBillboard(nPriority)
{
	// 値のクリア
	m_Info.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_Info.fLife = 0;
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.fRadius = 0.0f;
	m_Info.Type = TYPE_NONE;
}

//===============================================
// デストラクタ
//===============================================
CEffect::~CEffect()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CEffect::Init(void)
{
	// オブジェクトの初期化処理
	CObjectBillboard::Init();

	m_Info.fLife = LIFE;	// 体力の初期化
	m_Info.Type = TYPE_NONE;

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CEffect::Uninit(void)
{
	// オブジェクトの終了処理
	CObjectBillboard::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CEffect::Update(void)
{
	m_Info.fLife -= CManager::GetSlow()->Get();
	CMeshField *pMesh = CGame::GetMeshField();
	D3DXVECTOR3 nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fHeight = CMeshField::GetHeight(GetPosition());

	if (m_Info.fLife < 0)
	{// 寿命がなくなった場合

		// 終了する
		Uninit();
	}
	else
	{// まだある場合
		D3DXVECTOR3 pos = GetPosition();	// 座標

		pos += m_Info.move * CManager::GetSlow()->Get();

		// 座標
		SetPosition(pos);

		switch (m_Info.Type)
		{
		case TYPE_NONE:
			m_Info.col.a -= 0.05f * CManager::GetSlow()->Get();
			m_Info.fRadius += 0.1f * CManager::GetSlow()->Get();
			break;

		case TYPE_BULLET:
			m_Info.col.a -= 0.05f * CManager::GetSlow()->Get();
			m_Info.fRadius += 0.1f * CManager::GetSlow()->Get();
			break;

		case TYPE_DUST:
			m_Info.col.a -= 0.01f * CManager::GetSlow()->Get();
			m_Info.fRadius -= 0.1f * CManager::GetSlow()->Get();

			m_Info.move -= m_Info.move * 0.035f * CManager::GetSlow()->Get();

			break;
		case TYPE_EXPLOSION:
			m_Info.col.a -= 0.01f * CManager::GetSlow()->Get();
			m_Info.move.y += 0.01f * CManager::GetSlow()->Get();
			break; 

		case TYPE_SHWBULLET:
			m_Info.col.a -= 0.01f * CManager::GetSlow()->Get();
			m_Info.fRadius -= (rand() % 100 - 50) * 0.01f;
			m_Info.move.y += -0.1f * CManager::GetSlow()->Get();

			break;

		case TYPE_SHWREF:
			m_Info.col.a -= 0.01f * CManager::GetSlow()->Get();
			m_Info.fRadius -= (rand() % 100 - 50) * 0.01f;
			m_Info.move.y += -0.1f * CManager::GetSlow()->Get();

			if (GetPosition().y < fHeight)
			{
				m_Info.move.y *= -1.0f;
			}

			break;

		case TYPE_SWEAT:
			m_Info.col.a -= 0.04f * CManager::GetSlow()->Get();
			m_Info.fRadius -= 0.005f * CManager::GetSlow()->Get();
			//m_Info.move.y += -0.1f * CManager::GetSlow()->Get();
			break;

		case TYPE_HEAT:
			m_Info.col.a -= 0.01f * CManager::GetSlow()->Get();
			m_Info.fRadius -= (rand() % 100 - 50) * 0.01f;
			//m_Info.move.y += -0.1f * CManager::GetSlow()->Get();
			break;

		case TYPE_SWAP:

			m_Info.col.a -= 0.05f * CManager::GetSlow()->Get();
			m_Info.move.x -= m_Info.move.x * 0.5f * CManager::GetSlow()->Get();
			m_Info.move.z -= m_Info.move.z * 0.5f * CManager::GetSlow()->Get();
			m_Info.move.y -= m_Info.move.y * 0.05f * CManager::GetSlow()->Get();

			break;
		case CEffect::TYPE_BALEXPLOSION:	// 爆発

			m_Info.col.a -= 0.01f * CManager::GetSlow()->Get();
			m_Info.fRadius -= 1.2f * CManager::GetSlow()->Get();

			break;

		case CEffect::TYPE_SMAKE:	// 爆発

			m_Info.col.a -= 0.035f * CManager::GetSlow()->Get();
			m_Info.move.y -= m_Info.move.y * 0.005f * CManager::GetSlow()->Get();

			break;

		case CEffect::TYPE_HEATHAZE:	// 爆発

			m_Info.col.a -= 0.0001f * CManager::GetSlow()->Get();
			m_Info.fRadius -= (rand() % 100 - 50) * 0.01f * CManager::GetSlow()->Get();
			m_Info.move.x += (rand() % 100 - 50) * 0.01f;
			m_Info.move.z += (rand() % 100 - 50) * 0.01f;

			break;

		case CEffect::TYPE_SLOWOK:	// 爆発

			m_Info.col.a -= 0.0001f * CManager::GetSlow()->Get();
			m_Info.fRadius -= (rand() % 100 - 50) * 0.05f * CManager::GetSlow()->Get();
			m_Info.move.x += (rand() % 100 - 50) * 0.01f;
			m_Info.move.z += (rand() % 100 - 50) * 0.01f;

			break;
		}

		if (m_Info.col.a < 0.0f || m_Info.fRadius < 0.0f)
		{// 値がひっくり返った
			Uninit();
		}
		else
		{
			if (m_Info.Type != TYPE_SWAP)
			{
				SetSize(m_Info.fRadius, m_Info.fRadius);
			}

			SetCol(m_Info.col);
		}
	}
}

//===============================================
// 描画処理
//===============================================
void CEffect::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//ライティングをオフにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Zテストを無効化する
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CObjectBillboard::Draw();

	//ライティングをオンにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//===============================================
// 生成
//===============================================
CEffect *CEffect::Create(D3DXVECTOR3 pos, TYPE type)
{
	CEffect *pEffect = NULL;
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ

	// エフェクトの生成
	pEffect = new CEffect(7);

	if (pEffect != NULL)
	{// 生成できた場合

		// 座標設定
		pEffect->SetPosition(pos);

		// 初期化処理
		pEffect->Init();

		// オブジェクトの種類の設定
		pEffect->SetType(CObject::TYPE_NONE);

		// エフェクトの設定
		pEffect->m_Info.Type = type;

		// 半径設定
		pEffect->RadiusSet();

		// サイズの設定
		//pEffect->SetSize(pEffect->m_Info.fRadius, pEffect->m_Info.fRadius);

		// 色の設定
		pEffect->ColorSet();

		// テクスチャの割り当て
		pEffect->BindTexture(pTexture->Regist(CTexture::GetFileName(CTexture::TYPE_EFFECT)));
	}
	else
	{// 生成に失敗した場合
		return NULL;
	}

	return pEffect;
}

//===============================================
// 生成
//===============================================
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, float fLife, TYPE type)
{
	CEffect *pEffect = NULL;
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ

	// エフェクトの生成
	pEffect = new CEffect();

	if (pEffect != NULL)
	{// 生成できた場合

		// 座標設定
		pEffect->SetPosition(pos);

		// 初期化処理
		pEffect->Init();

		// オブジェクトの種類の設定
		pEffect->SetType(CObject::TYPE_NONE);

		// エフェクトの設定
		pEffect->m_Info.Type = type;

		// 半径設定
		pEffect->m_Info.fRadius = fRadius;

		// 寿命設定
		pEffect->m_Info.fLife = fLife;

		// 移動量設定
		pEffect->SetMove(move);

		// サイズの設定
		//pEffect->SetSize(pEffect->m_Info.fRadius, pEffect->m_Info.fRadius);

		// 色の設定
		pEffect->m_Info.col = col;
		pEffect->SetCol(pEffect->m_Info.col);

		// テクスチャの割り当て
		pEffect->BindTexture(pTexture->Regist(CTexture::GetFileName(CTexture::TYPE_EFFECT)));
	}
	else
	{// 生成に失敗した場合
		return NULL;
	}

	return pEffect;
}

//===============================================
// 色設定
//===============================================
void CEffect::ColorSet(void)
{
	// 種類ごとに色の設定
	m_Info.col = m_aColInfo[m_Info.Type];
   	SetCol(m_Info.col);
}

//===============================================
// サイズの設定
//===============================================
void CEffect::RadiusSet(void)
{
	// 半径の設定
	m_Info.fRadius = m_aRadiusInfo[m_Info.Type];
}

//===============================================
// 移動量の設定
//===============================================
void CEffect::SetMove(D3DXVECTOR3 move)
{
	m_Info.move = move;
}
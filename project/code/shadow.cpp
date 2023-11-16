//==========================================================
//
//影描画処理
//Author 奥定伊吹
//
//==========================================================
#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//==========================================================
// コンストラクタ
//==========================================================
CShadow::CShadow() : CObject3D(3)
{

}

//==========================================================
// デストラクタ
//==========================================================
CShadow::~CShadow()
{

}

//==========================================================
//影の初期化処理
//==========================================================
HRESULT CShadow::Init(void)
{
	CObject3D::Init();

	return S_OK;
}

//==========================================================
//影の終了処理
//==========================================================
void CShadow::Uninit(void)
{
	CObject3D::Uninit();
}

//==========================================================
//影の更新処理
//==========================================================
void CShadow::Update(void)
{

}

//==========================================================
//影の描画処理
//==========================================================
void CShadow::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスへのポインタを取得

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// 描画
	CObject3D::Draw();

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==========================================================
//影の設定処理
//==========================================================
CShadow *CShadow::Create(const D3DXVECTOR3& pos, float fWidth, float fHeight)
{
	CShadow *pShadow = NULL;	// 影のポインタ
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// テクスチャのポインタ

	// オブジェクト2Dの生成
	pShadow = new CShadow;

	if (pShadow != NULL)
	{// 生成できた場合

		// 初期化処理
		pShadow->Init();

		// 座標設定
		pShadow->SetPosition(pos);

		// サイズ設定
		pShadow->SetpVtx(fWidth, fHeight);

		// 種類設定
		pShadow->SetType(TYPE_NONE);

		// テクスチャ設定
		pShadow->BindTexture(pTexture->Regist(CTexture::GetFileName(CTexture::TYPE_SHADOW)));
	}
	else
	{// 生成に失敗した場合
		return NULL;
	}

	return pShadow;
}

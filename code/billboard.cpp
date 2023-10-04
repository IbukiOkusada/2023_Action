//==========================================================
//
// ビルボードの処理全般 [billboard.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "billboard.h"
#include "input.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//==========================================================
//コンストラクタ
//==========================================================
CObjectBillboard::CObjectBillboard(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	m_nIdexTexture = -1;
	m_pVtxBuff = NULL;
}

//==========================================================
//デストラクタ
//==========================================================
CObjectBillboard::~CObjectBillboard()
{

}

//==========================================================
//ビルボードの初期化処理
//==========================================================
HRESULT CObjectBillboard::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報設定
	SetVtx();

	return S_OK;
}

//==========================================================
//ビルボードの終了処理
//==========================================================
void CObjectBillboard::Uninit(void)
{

	//頂点バッファの廃棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// 廃棄
	Release();
}

//==========================================================
//ビルボードの更新処理
//==========================================================
void CObjectBillboard::Update(void)
{
	
}

//==========================================================
//ビルボードの描画処理
//==========================================================
void CObjectBillboard::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		//デバイスへのポインタを取得
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DXMATRIX mtxView;		//ビューマトリックス取得用

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		m_pVtxBuff,
		0,
		sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->SetAddress(m_nIdexTexture));

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0,
		2	//頂点情報構造体のサイズ
	);
}

//==========================================================
// 向きを反映させた描画処理
//==========================================================
void CObjectBillboard::RotFusionDraw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		//デバイスへのポインタを取得
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DXMATRIX mtxView;		//ビューマトリックス取得用

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		m_pVtxBuff,
		0,
		sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->SetAddress(m_nIdexTexture));

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0,
		2	//頂点情報構造体のサイズ
	);
}

//==========================================================
//生成処理
//==========================================================
CObjectBillboard *CObjectBillboard::Create(D3DXVECTOR3 pos, const int nPriority)
{
	CObjectBillboard *pObjBillboard = NULL;

	// オブジェクト2Dの生成
	pObjBillboard = new CObjectBillboard(nPriority);

	if (pObjBillboard != NULL)
	{// 生成できた場合

		// 初期化処理
		pObjBillboard->Init();

		// 座標設定
		pObjBillboard->SetPosition(pos);

		// 種類設定
		pObjBillboard->SetType(TYPE_NONE);
	}
	else
	{// 生成に失敗した場合
		return NULL;
	}

	return pObjBillboard;
}

//==========================================================
// 頂点情報設定
//==========================================================
void CObjectBillboard::SetVtx(void)
{
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(
		0,
		0,
		(void**)&pVtx,
		0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-5.0f, +5.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+5.0f, +5.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-5.0f, -5.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+5.0f, -5.0f, 0.0f);

	//法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//==========================================================
//テクスチャ座標設定
//==========================================================
void CObjectBillboard::SetVtx(const int nPatternAnim, const int nTexWidth, const int nTexHeight)
{
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(
		0,
		0,
		(void**)&pVtx,
		0
	);

	//頂点座標の設定
	pVtx[0].tex = D3DXVECTOR2(1.0f / nTexWidth * (float)nPatternAnim, (1.0f / nTexHeight * (float)(nPatternAnim / nTexWidth)));
	pVtx[1].tex = D3DXVECTOR2(1.0f / nTexWidth * (float)(nPatternAnim + 1), (1.0f / nTexHeight * (float)(nPatternAnim / nTexWidth)));
	pVtx[2].tex = D3DXVECTOR2(1.0f / nTexWidth * (float)nPatternAnim, (1.0f / nTexHeight * (float)(nPatternAnim / nTexWidth + 1)));
	pVtx[3].tex = D3DXVECTOR2(1.0f / nTexWidth * (float)(nPatternAnim + 1), (1.0f / nTexHeight * (float)(nPatternAnim / nTexWidth + 1)));

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//==========================================================
// 色設定
//==========================================================
void CObjectBillboard::SetVtx(D3DXCOLOR col)
{
	VERTEX_3D *pVtx;
	m_col = col;

	//頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(
		0,
		0,
		(void**)&pVtx,
		0);

	//頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//==========================================================
//テクスチャ設定
//==========================================================
void CObjectBillboard::BindTexture(int nIdx)
{
	m_nIdexTexture = nIdx;
}

//==========================================================
//サイズ設定
//==========================================================
void CObjectBillboard::SetSize(float fWidth, float fHeight)
{
	VERTEX_3D *pVtx;

	m_fWidth = fWidth;
	m_fHeight = fHeight;

	//頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(
		0,
		0,
		(void**)&pVtx,
		0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(sinf(m_rot.z + (-D3DX_PI * 0.25f)) * m_fWidth, cosf(m_rot.z + (-D3DX_PI * 0.25f)) * m_fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(sinf(m_rot.z + (D3DX_PI * 0.25f)) * m_fWidth, cosf(m_rot.z + (D3DX_PI * 0.25f)) * m_fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(sinf(m_rot.z + (-D3DX_PI * 0.75f)) * m_fWidth, cosf(m_rot.z + (-D3DX_PI * 0.75f)) * m_fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(sinf(m_rot.z + (D3DX_PI * 0.75f)) * m_fWidth, cosf(m_rot.z + (D3DX_PI * 0.75f)) * m_fHeight, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//==========================================================
// 色設定
//==========================================================
void CObjectBillboard::SetCol(D3DXCOLOR col)
{
	VERTEX_3D *pVtx;
	m_col = col;

	//頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(
		0,
		0,
		(void**)&pVtx,
		0);

	//頂点カラーの設定
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//==========================================================
// マトリックス設定
//==========================================================
void CObjectBillboard::SetMtx(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		//デバイスへのポインタを取得
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ
	D3DXMATRIX mtxTrans;	//計算用マトリックス
	D3DXMATRIX mtxView;		//ビューマトリックス取得用

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//ビューマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&m_mtxWorld, NULL, &mtxView);
	m_mtxWorld._41 = 0.0f;
	m_mtxWorld._42 = 0.0f;
	m_mtxWorld._43 = 0.0f;

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
}
//==========================================================
//
// メッシュフィールドの処理 [meshfield.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "meshwall.h"
#include "texture.h"
#include "manager.h"

// マクロ定義
#define TEXTUREFILE_DATA	"data\\TEXTURE\\welltile.png"		//テクスチャデータ

// 静的メンバ変数宣言
CMeshWall *CMeshWall::m_pTop = NULL;	// 先頭のオブジェクトへのポインタ
CMeshWall *CMeshWall::m_pCur = NULL;	// 最後尾のオブジェクトへのポインタ

//==========================================================
// コンストラクタ
//==========================================================
CMeshWall::CMeshWall()
{
	m_pNext = NULL;
	m_pPrev = NULL;

	// 自分自身をリストに追加
	if (m_pTop != NULL)
	{// 先頭が存在している場合
		m_pCur->m_pNext = this;	// 現在最後尾のオブジェクトのポインタにつなげる
		m_pPrev = m_pCur;
		m_pCur = this;	// 自分自身が最後尾になる
	}
	else
	{// 存在しない場合
		m_pTop = this;	// 自分自身が先頭になる
		m_pCur = this;	// 自分自身が最後尾になる
	}
}

//==========================================================
// デストラクタ
//==========================================================
CMeshWall::~CMeshWall()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CMeshWall::Init(void)
{
	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CMeshWall::Uninit(void)
{
	// リストから自分自身を削除する
	if (m_pTop == this)
	{// 自身が先頭
		if (m_pNext != NULL)
		{// 次が存在している
			m_pTop = m_pNext;	// 次を先頭にする
			m_pNext->m_pPrev = NULL;	// 次の前のポインタを覚えていないようにする
		}
		else
		{// 存在していない
			m_pTop = NULL;	// 先頭がない状態にする
			m_pCur = NULL;	// 最後尾がない状態にする
		}
	}
	else if (m_pCur == this)
	{// 自身が最後尾
		if (m_pPrev != NULL)
		{// 次が存在している
			m_pCur = m_pPrev;			// 前を最後尾にする
			m_pPrev->m_pNext = NULL;	// 前の次のポインタを覚えていないようにする
		}
		else
		{// 存在していない
			m_pTop = NULL;	// 先頭がない状態にする
			m_pCur = NULL;	// 最後尾がない状態にする
		}
	}
	else
	{
		if (m_pNext != NULL)
		{
			m_pNext->m_pPrev = m_pPrev;	// 自身の次に前のポインタを覚えさせる
		}
		if (m_pPrev != NULL)
		{
			m_pPrev->m_pNext = m_pNext;	// 自身の前に次のポインタを覚えさせる
		}
	}

	//頂点バッファの廃棄
	CObjectMesh::Uninit();
}

//==========================================================
// 更新処理
//==========================================================
void CMeshWall::Update(void)
{

}

//==========================================================
// 描画処理
//==========================================================
void CMeshWall::Draw(void)
{
	// 描画
	CObjectMesh::Draw();
}

//==========================================================
// 頂点情報設定
//==========================================================
void CMeshWall::SetVtxInfo(void)
{
	int nVertex = GetVertex();			// 頂点数を取得
	int nNumWidth = GetNumWidth();		// 幅枚数を取得
	int nNumHeight = GetNumHeight();	// 高さ枚数を取得

	//頂点座標の設定(左奥から右手前に向かって頂点情報を設定する
	for (int nCntpVtx = 0; nCntpVtx < nVertex; nCntpVtx++)
	{
		//頂点座標
		m_pVtx[nCntpVtx].pos.x = -(m_fWidth * nNumWidth) + (nCntpVtx % (nNumWidth + 1) * (m_fWidth * 2));
		m_pVtx[nCntpVtx].pos.y = ((m_fHeight * 2) * nNumHeight) + ((nCntpVtx / (nNumWidth + 1) * (-m_fHeight * 2)));
		m_pVtx[nCntpVtx].pos.z = 0.0f;

		//法線
		m_pVtx[nCntpVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//色
		m_pVtx[nCntpVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		m_pVtx[nCntpVtx].tex = D3DXVECTOR2(1.0f * (nCntpVtx % (nNumWidth + 1)), 1.0f * (nCntpVtx / (nNumWidth + 1)));
	}

	// 頂点設定
	SetVtx();
}

//==========================================================
// 生成
//==========================================================
CMeshWall *CMeshWall::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, float fWidth, float fHeight,
	const char *pFileName, const int nWidth, const int nHeight, const int nPriority )
{
	CMeshWall *pMeshWall = NULL;	// メッシュフィールドのポインタ
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ

	// メモリの確保
	pMeshWall = new CMeshWall;

	if (pMeshWall != NULL)
	{// 確保できた場合

		// 初期化
		pMeshWall->Init();

		// 座標設定
		pMeshWall->SetPosition(pos);

		// 向き設定
		pMeshWall->SetRotation(rot);

		// 頂点生成
		pMeshWall->CObjectMesh::Create(nWidth, nHeight);

		// サイズ設定
		pMeshWall->SetSize(fWidth, fHeight);

		// テクスチャ設定
		pMeshWall->BindTexture(pTexture->Regist(pFileName));
	}

	return pMeshWall;
}

//==========================================================
// 幅設定
//==========================================================
void CMeshWall::SetSize(float fWidth, float fHeight)
{
	// サイズ設定
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	// 頂点情報設定
	SetVtxInfo();
}

//==========================================================
// 当たり判定
//==========================================================
void CMeshWall::Collision(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld)
{
	CMeshWall *pObj = m_pTop;
	bool bCollision = false;		//当たったかどうか判定

	while (pObj != NULL)
	{
		CMeshWall *pObjNext = pObj->m_pNext;

		if (pObj->GetHeight() <= 30.0f && pObj->GetNumHeight() == 1)
		{
			pObj = pObjNext;
			continue;
		}

		if (pObj->GetRotation().y == 0.0f * D3DX_PI || pObj->GetRotation().y == 0.0f * -D3DX_PI)
		{//壁が正面を向いている場合(Z軸がマイナスの方を見ている)
			if (
				pos.y <= pObj->GetPosition().y + pObj->GetHeight() * 2 * pObj->GetNumHeight() &&
				pos.x >= pObj->GetPosition().x - pObj->GetWidth() * pObj->GetNumWidth() &&
				pos.x <= pObj->GetPosition().x + pObj->GetWidth() * pObj->GetNumWidth() &&
				posOld.z <= pObj->GetPosition().z &&
				pos.z > pObj->GetPosition().z)
			{//当たっている場合
				pos.z = pObj->GetPosition().z;
			}
		}
		if (pObj->GetRotation().y == 0.5f * D3DX_PI)
		{//壁が右を向いている場合(X軸がプラスの方を見ている)

			if (
				pos.y < pObj->GetPosition().y + pObj->GetHeight() * 2 * pObj->GetNumHeight() &&
				pos.z >= pObj->GetPosition().z - pObj->GetWidth() * pObj->GetNumWidth() &&
				pos.z <= pObj->GetPosition().z + pObj->GetWidth() * pObj->GetNumWidth() &&
				posOld.x <= pObj->GetPosition().x &&
				pos.x > pObj->GetPosition().x)
			{//当たっている場合
				pos.x = pObj->GetPosition().x;
			}
		}
		if (pObj->GetRotation().y == -0.5f * D3DX_PI)
		{//壁が左を向いている場合(X軸がマイナスの方を見ている)
			if (
				pos.y < pObj->GetPosition().y + pObj->GetHeight() * 2 * pObj->GetNumHeight() &&
				pos.z >= pObj->GetPosition().z - pObj->GetWidth() * pObj->GetNumWidth() &&
				pos.z <= pObj->GetPosition().z + pObj->GetWidth() * pObj->GetNumWidth() &&
				posOld.x >= pObj->GetPosition().x &&
				pos.x < pObj->GetPosition().x)
			{//当たっている場合
				pos.x = pObj->GetPosition().x;
			}

		}
		if (pObj->GetRotation().y == 1.0f * D3DX_PI || pObj->GetRotation().y == 1.0f * -D3DX_PI)
		{//壁が奥を向いている場合(Z軸がプラスの方を見ている)
			if (
				pos.y < pObj->GetPosition().y + pObj->GetHeight() * 2 * pObj->GetNumHeight()  &&
				pos.x >= pObj->GetPosition().x - pObj->GetWidth() * pObj->GetNumWidth() &&
				pos.x <= pObj->GetPosition().x + pObj->GetWidth() * pObj->GetNumWidth() &&
				posOld.z >= pObj->GetPosition().z &&
				pos.z < pObj->GetPosition().z)
			{//当たっている場合
				pos.z = pObj->GetPosition().z;
			}
		}

		pObj = pObjNext;
	}
}
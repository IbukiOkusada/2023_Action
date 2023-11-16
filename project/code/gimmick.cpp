//==========================================================
//
// ギミック基底 [gimmick.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "gimmick.h"
#include "manager.h"
#include "renderer.h"

// 静的メンバ変数宣言
CGimmick *CGimmick::m_pTop = NULL;	// 先頭のオブジェクトへのポインタ
CGimmick *CGimmick::m_pCur = NULL;	// 最後尾のオブジェクトへのポインタ

//==========================================================
// コンストラクタ
//==========================================================
CGimmick::CGimmick()
{
	// 値のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

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
CGimmick::~CGimmick()
{

}

//==========================================================
// マトリックスの設定
//==========================================================
void CGimmick::SetMtxWorld(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//デバイスへのポインタを取得
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DXMATRIX mtxParent;			// 親のマトリックス情報

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//==========================================================
// 当たり判定
//==========================================================
bool CGimmick::Collision(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, const D3DXVECTOR3& vtxMin, const D3DXVECTOR3& vtxMax, int &nDamage)
{
	CGimmick *pObj = m_pTop;	// 先頭取得
	bool bLand = false;	// 着地したか否か

	while (pObj != NULL)
	{
		CGimmick *pObjNext = pObj->m_pNext;
		D3DXVECTOR3 vtxObjMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vtxObjMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		if (pObj->CollisionCheck(pos, posOld, move, vtxMin, vtxMax, nDamage))
		{
			bLand = true;
		}

		pObj = pObjNext;
	}

	return bLand;
}

//==========================================================
// リストから外す
//==========================================================
void CGimmick::ListOut(void)
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
}

//==========================================================
// 風反転
//==========================================================
void CGimmick::AirReverse(void)
{
	CGimmick *pObj = m_pTop;	// 先頭取得

	while (pObj != NULL)
	{
		CGimmick *pObjNext = pObj->m_pNext;

		pObj->Reverse();

		pObj = pObjNext;
	}
}

//==========================================================
// 対応する扉を開ける
//==========================================================
void CGimmick::DoorOpen(int nId)
{
	CGimmick *pObj = m_pTop;	// 先頭取得

	while (pObj != NULL)
	{
		CGimmick *pObjNext = pObj->m_pNext;

		pObj->Open(nId);

		pObj = pObjNext;
	}
}
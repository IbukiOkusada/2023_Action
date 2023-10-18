//==========================================================
//
// 移動オブジェクト [objectmove.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "gimmick_move.h"
#include "manager.h"
#include "slow.h"
#include "Xfile.h"
#include "objectX.h"

// マクロ定義

//==========================================================
// コンストラクタ
//==========================================================
CGimmickMove::CGimmickMove()
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fNowMoveTimer = 0.0f;
	m_fNumMoveTimer = 0.0f;
	m_pObject = NULL;
}

//==========================================================
// デストラクタ
//==========================================================
CGimmickMove::~CGimmickMove()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CGimmickMove::Init(void)
{

	// 読み込み確認
	if(m_pObject == NULL)
	{
		m_pObject = CObjectX::Create(GetPosition(), GetRotation(), "data\\MODEL\\5mcube.x", 3);
	}

	// スローを覚える
	m_pSlow = CManager::GetInstance()->GetSlow();

	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CGimmickMove::Uninit(void)
{
	if (m_pObject != NULL)
	{
		m_pObject->Uninit();
		m_pObject = NULL;
	}

	ListOut();

	Release();
}

//==========================================================
// 更新処理
//==========================================================
void CGimmickMove::Update(void)
{
	m_posOld = GetPosition();

	// 操作関連
	Controller();

	SetMtxWorld();
}

//==========================================================
// 描画処理
//==========================================================
void CGimmickMove::Draw(void)
{

}

//==========================================================
// 生成
//==========================================================
CGimmickMove *CGimmickMove::Create(void)
{
	CGimmickMove *pObjectMove = new CGimmickMove;

	if (pObjectMove != NULL)
	{
		// 初期化処理
		pObjectMove->Init();
	}

	return pObjectMove;
}

//==========================================================
// 生成
//==========================================================
CGimmickMove *CGimmickMove::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fTimer)
{
	CGimmickMove *pObjectMove = new CGimmickMove;

	if (pObjectMove != NULL)
	{
		// 初期化処理
		pObjectMove->Init();

		// 設定
		pObjectMove->SetPosition(pos);
		pObjectMove->SetMove(move);
		pObjectMove->SetNumTimer(fTimer);
		pObjectMove->SetNowTimer(fTimer);
	}

	return pObjectMove;
}

//==========================================================
// 操作関連
//==========================================================
void CGimmickMove::Controller(void)
{
	// 座標更新
	D3DXVECTOR3 pos = GetPosition();
	float m_fSlowMulti = 1.0f;	// スロー倍率

	if (!m_pSlow)
	{
		m_fSlowMulti = m_pSlow->Get();
	}

	pos += m_move;
	SetPosition(pos);

	if (m_fNumMoveTimer == 0.0f)
	{// 往復しない
		return;
	}

	m_fNowMoveTimer -= m_fSlowMulti;

	if (m_fNowMoveTimer <= 0.0f)
	{
		m_move *= -1.0f;
		m_fNowMoveTimer = m_fNumMoveTimer;
	}

	if(m_pObject != NULL)
	{
		m_pObject->SetPosition(GetPosition());
		m_pObject->SetRotation(GetRotation());
	}
}

//==========================================================
// 個別判定チェック
//==========================================================
bool CGimmickMove::CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, const float fRefMulti)
{
	CXFile *pFile = CManager::GetInstance()->GetModelFile();
	bool bLand = false;	// 着地したか否か
	D3DXVECTOR3 vtxObjMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxObjMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 ObjPos = GetPosition();
	D3DXVECTOR3 ObjRot = GetRotation();

	if (m_pObject == NULL)
	{
		return bLand;
	}

	// 向きを反映
	m_pObject->SetRotSize(vtxObjMax,
		vtxObjMin,
		pFile->GetMax(m_pObject->GetIdx()),
		pFile->GetMin(m_pObject->GetIdx()),
		ObjRot.y);

	if (pos.y + vtxMax.y > ObjPos.y + vtxObjMin.y
		&& pos.y + vtxMin.y <= ObjPos.y + vtxObjMax.y)
	{//プレイヤーとモデルが同じ高さにある
		if (posOld.x + vtxMin.x >= m_posOld.x + vtxObjMax.x
			&& pos.x + vtxMin.x < ObjPos.x + vtxObjMax.x
			&& pos.z + vtxMax.z > ObjPos.z + vtxObjMin.z
			&& pos.z + vtxMin.z < ObjPos.z + vtxObjMax.z)
		{//右から左にめり込んだ
			bLand = true;
			move.x *= -1.0f;
			//move.x *= fRefMulti;
			pos.x = ObjPos.x + vtxObjMax.x - vtxMin.x + 0.1f + m_move.x;
		}
		else if (posOld.x + vtxMax.x <= m_posOld.x + vtxObjMin.x
			&& pos.x + vtxMax.x > ObjPos.x + vtxObjMin.x
			&& pos.z + vtxMax.z > ObjPos.z + vtxObjMin.z
			&& pos.z + vtxMin.z < ObjPos.z + vtxObjMax.z)
		{//左から右にめり込んだ
		 //位置を戻す
			bLand = true;
			move.x *= -1.0f;
			//move.x *= fRefMulti;
			pos.x = ObjPos.x + vtxObjMin.x - vtxMax.x - 0.1f + m_move.x;
			//move.x = 0.0f;
		}
		else if (posOld.z + vtxMin.z >= m_posOld.z + vtxObjMax.z
			&& pos.z + vtxMin.z < ObjPos.z + vtxObjMax.z
			&& pos.x + vtxMax.x > ObjPos.x + vtxObjMin.x
			&& pos.x + vtxMin.x < ObjPos.x + vtxObjMax.x)
		{//奥から手前にめり込んだ
			//位置を戻す
			bLand = true;
			move.z *= -1.0f;
			//move.z *= fRefMulti;
			pos.z = ObjPos.z + vtxObjMax.z - vtxMin.z + 0.1f + m_move.z;
			//move.z = 0.0f;
		}
		else if (posOld.z + vtxMax.z <= m_posOld.z + vtxObjMin.z
			&& pos.z + vtxMax.z > ObjPos.z + vtxObjMin.z
			&& pos.x + vtxMax.x > ObjPos.x + vtxObjMin.x
			&& pos.x + vtxMin.x < ObjPos.x + vtxObjMax.x)
		{//手前から奥にめり込んだt
		 //位置を戻す
			bLand = true;
			move.z *= -1.0f;
			//move.z *= fRefMulti;
			pos.z = ObjPos.z + vtxObjMin.z - vtxMax.z - 0.1f + m_move.z;
			//move.z = 0.0f;
		}
	}

	if (pos.x + vtxMax.x > ObjPos.x + vtxObjMin.x
		&& pos.x + vtxMin.x < ObjPos.x + vtxObjMax.x
		&& pos.z + vtxMax.z > ObjPos.z + vtxObjMin.z
		&& pos.z + vtxMin.z < ObjPos.z + vtxObjMax.z)
	{//範囲内にある
	 //上からの判定
		if (posOld.y + vtxMin.y >= ObjPos.y + vtxObjMax.y
			&& pos.y + vtxMin.y < ObjPos.y + vtxObjMax.y)
		{//上からめり込んだ
		 //上にのせる
			pos.y = ObjPos.y + vtxObjMax.y - vtxMin.y;
			move.y = 0.0f;
			bLand = true;
		}
	}

	return bLand;
}
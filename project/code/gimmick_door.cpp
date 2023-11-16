//==========================================================
//
// 移動オブジェクト [objectmove.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "gimmick_door.h"
#include "manager.h"
#include "slow.h"
#include "Xfile.h"
#include "objectX.h"
#include "model.h"

// マクロ定義
#define DAMAGE		(1)

//==========================================================
// コンストラクタ
//==========================================================
CGimmickDoor::CGimmickDoor()
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_aObject[nCnt].GoalPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aObject[nCnt].pModel = NULL;
	}

	m_bOpen = nullptr;
	m_nId = -1;
}

//==========================================================
// デストラクタ
//==========================================================
CGimmickDoor::~CGimmickDoor()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CGimmickDoor::Init(void)
{

	// 読み込み確認
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_aObject[nCnt].pModel = CModel::Create("data\\MODEL\\door.x");
		m_aObject[nCnt].pModel->SetParent(GetMtxWorld());
		m_aObject[nCnt].GoalPos = D3DXVECTOR3(400.0f + (-800.0f * nCnt), 0.0f, 0.0f);
		m_aObject[nCnt].pModel->SetCurrentPosition(D3DXVECTOR3(150.0f + (-300.0f * nCnt), 0.0f, 0.0f));
	}

	// スローを覚える
	m_pSlow = CManager::GetInstance()->GetSlow();

	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CGimmickDoor::Uninit(void)
{
	
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_aObject[nCnt].pModel != nullptr)
		{
			m_aObject[nCnt].pModel->Uninit();
			m_aObject[nCnt].pModel = nullptr;
		}
	}

	ListOut();

	Release();
}

//==========================================================
// 更新処理
//==========================================================
void CGimmickDoor::Update(void)
{
	// 操作関連
	Controller();

	SetMtxWorld();
}

//==========================================================
// 描画処理
//==========================================================
void CGimmickDoor::Draw(void)
{

}

//==========================================================
// 生成
//==========================================================
CGimmickDoor *CGimmickDoor::Create(void)
{
	CGimmickDoor *pObjectDoor = new CGimmickDoor;

	if (pObjectDoor != NULL)
	{
		// 初期化処理
		pObjectDoor->Init();
	}

	return pObjectDoor;
}

//==========================================================
// 生成
//==========================================================
CGimmickDoor *CGimmickDoor::Create(const D3DXVECTOR3& pos)
{
	CGimmickDoor *pObjectDoor = new CGimmickDoor;

	if (pObjectDoor != NULL)
	{
		// 初期化処理
		pObjectDoor->Init();

		// 設定
		pObjectDoor->SetPosition(pos);
	}

	return pObjectDoor;
}

//==========================================================
// 操作関連
//==========================================================
void CGimmickDoor::Controller(void)
{
	// 座標更新
	if (m_bOpen)
	{
		for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
		{
			if (m_aObject[nCnt].pModel == nullptr)
			{
				continue;
			}

			D3DXVECTOR3 pos = m_aObject[nCnt].pModel->GetCurrentPosition();
			D3DXVECTOR3 posDiff = m_aObject[nCnt].GoalPos - 
				pos;

			pos += posDiff * 0.04f;
			m_aObject[nCnt].pModel->SetCurrentPosition(pos);
		}
	}

}

//==========================================================
// 個別判定チェック
//==========================================================
bool CGimmickDoor::CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, const D3DXVECTOR3& vtxMin, const D3DXVECTOR3& vtxMax, int &nDamage)
{
	CXFile *pFile = CManager::GetInstance()->GetModelFile();
	bool bLand = false;	// 着地したか否か
	D3DXVECTOR3 vtxObjMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxObjMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 ObjPos = GetPosition();
	D3DXVECTOR3 ObjRot = GetRotation();

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_aObject[nCnt].pModel == nullptr)
		{
			continue;
		}

		// 向きを反映
		m_aObject[nCnt].pModel->SetRotSize(vtxObjMax,
			vtxObjMin,
			pFile->GetMax(m_aObject[nCnt].pModel->GetId()),
			pFile->GetMin(m_aObject[nCnt].pModel->GetId()),
			ObjRot.y);

		ObjPos = D3DXVECTOR3(m_aObject[nCnt].pModel->GetMtxWorld()->_41, m_aObject[nCnt].pModel->GetMtxWorld()->_42, m_aObject[nCnt].pModel->GetMtxWorld()->_43);

		if (pos.y + vtxMax.y > ObjPos.y + vtxObjMin.y
			&& pos.y + vtxMin.y <= ObjPos.y + vtxObjMax.y)
		{//プレイヤーとモデルが同じ高さにある
			if (posOld.x + vtxMin.x >= ObjPos.x + vtxObjMax.x
				&& pos.x + vtxMin.x < ObjPos.x + vtxObjMax.x
				&& pos.z + vtxMax.z > ObjPos.z + vtxObjMin.z
				&& pos.z + vtxMin.z < ObjPos.z + vtxObjMax.z)
			{//右から左にめり込んだ
				bLand = true;
				move.x *= -1.0f;
				pos.x = ObjPos.x + vtxObjMax.x - vtxMin.x + 0.1f;
				nDamage = DAMAGE;
			}
			else if (posOld.x + vtxMax.x <= ObjPos.x + vtxObjMin.x
				&& pos.x + vtxMax.x > ObjPos.x + vtxObjMin.x
				&& pos.z + vtxMax.z > ObjPos.z + vtxObjMin.z
				&& pos.z + vtxMin.z < ObjPos.z + vtxObjMax.z)
			{//左から右にめり込んだ
			 //位置を戻す
				bLand = true;
				move.x *= -1.0f;
				pos.x = ObjPos.x + vtxObjMin.x - vtxMax.x - 0.1f;
				nDamage = DAMAGE;
			}
			else if (posOld.z + vtxMin.z >= ObjPos.z + vtxObjMax.z
				&& pos.z + vtxMin.z < ObjPos.z + vtxObjMax.z
				&& pos.x + vtxMax.x > ObjPos.x + vtxObjMin.x
				&& pos.x + vtxMin.x < ObjPos.x + vtxObjMax.x)
			{//奥から手前にめり込んだ
			 //位置を戻す
				bLand = true;
				move.z *= -1.0f;
				pos.z = ObjPos.z + vtxObjMax.z - vtxMin.z + 0.1f;
				nDamage = DAMAGE;
			}
			else if (posOld.z + vtxMax.z <= ObjPos.z + vtxObjMin.z
				&& pos.z + vtxMax.z > ObjPos.z + vtxObjMin.z
				&& pos.x + vtxMax.x > ObjPos.x + vtxObjMin.x
				&& pos.x + vtxMin.x < ObjPos.x + vtxObjMax.x)
			{//手前から奥にめり込んだt
			 //位置を戻す
				bLand = true;
				move.z *= -1.0f;
				pos.z = ObjPos.z + vtxObjMin.z - vtxMax.z - 0.1f;
				nDamage = DAMAGE;
			}
		}
	}

	return bLand;
}

void CGimmickDoor::Open(int nId)
{
	if (nId == m_nId)
	{
		m_bOpen = true;
	}
}
//==========================================================
//
// 移動オブジェクト [objectmove.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "gimmick_button.h"
#include "manager.h"
#include "slow.h"
#include "Xfile.h"
#include "objectX.h"

// マクロ定義

//==========================================================
// コンストラクタ
//==========================================================
CGimmickButton::CGimmickButton()
{
	m_pObject = NULL;
	m_type = TYPE_MAX;
	m_nId = -1;
	m_bPress = false;
	m_bPressOld = false;
}

//==========================================================
// デストラクタ
//==========================================================
CGimmickButton::~CGimmickButton()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CGimmickButton::Init(void)
{
	// 読み込み確認
	if (m_pObject == NULL)
	{
		m_pObject = CObjectX::Create(GetPosition(), GetRotation(), "data\\MODEL\\button.x", 3);
		m_pObject->ListOut();
	}

	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CGimmickButton::Uninit(void)
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
void CGimmickButton::Update(void)
{
	// 操作関連
	Controller();

	SetMtxWorld();

	m_bPressOld = m_bPress;
}

//==========================================================
// 生成
//==========================================================
CGimmickButton *CGimmickButton::Create(void)
{
	CGimmickButton *pObjectButton = new CGimmickButton;

	if (pObjectButton != NULL)
	{
		// 初期化処理
		pObjectButton->Init();
	}

	return pObjectButton;
}

//==========================================================
// 操作関連
//==========================================================
void CGimmickButton::Controller(void)
{
	if (m_bPress)
	{
		D3DXVECTOR3 pos = GetPosition();
		D3DXVECTOR3 posDiff = m_GoalPos - pos;

		pos += posDiff * 0.01f;
		SetPosition(pos);

		if (!m_bPressOld)
		{
			switch (m_type)
			{
			case TYPE_REVERSE:
				AirReverse();
				break;

			case TYPE_DOOR:
				DoorOpen(m_nId);
				break;
			default:
				break;
			}
		}
	}

	if (m_pObject != nullptr)
	{
		m_pObject->SetPosition(GetPosition());
		m_pObject->SetRotation(GetRotation());
	}
}

//==========================================================
// 個別判定チェック
//==========================================================
bool CGimmickButton::CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, const D3DXVECTOR3& vtxMin, const D3DXVECTOR3& vtxMax, int &nDamage)
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
		if (posOld.x + vtxMin.x >= ObjPos.x + vtxObjMax.x
			&& pos.x + vtxMin.x < ObjPos.x + vtxObjMax.x
			&& pos.z + vtxMax.z > ObjPos.z + vtxObjMin.z
			&& pos.z + vtxMin.z < ObjPos.z + vtxObjMax.z)
		{//右から左にめり込んだ
			move.x *= -0.5f;
			pos.x = ObjPos.x + vtxObjMax.x - vtxMin.x + 0.1f;
			m_bPress = true;
		}
		else if (posOld.x + vtxMax.x <= ObjPos.x + vtxObjMin.x
			&& pos.x + vtxMax.x > ObjPos.x + vtxObjMin.x
			&& pos.z + vtxMax.z > ObjPos.z + vtxObjMin.z
			&& pos.z + vtxMin.z < ObjPos.z + vtxObjMax.z)
		{//左から右にめり込んだ
		 //位置を戻す
			move.x *= -0.5f;
			pos.x = ObjPos.x + vtxObjMin.x - vtxMax.x - 0.1f;
			m_bPress = true;
		}
		else if (posOld.z + vtxMin.z >= ObjPos.z + vtxObjMax.z
			&& pos.z + vtxMin.z < ObjPos.z + vtxObjMax.z
			&& pos.x + vtxMax.x > ObjPos.x + vtxObjMin.x
			&& pos.x + vtxMin.x < ObjPos.x + vtxObjMax.x)
		{//奥から手前にめり込んだ
		 //位置を戻す
			move.z *= -0.5f;
			pos.z = ObjPos.z + vtxObjMax.z - vtxMin.z + 0.1f;
			m_bPress = true;
		}
		else if (posOld.z + vtxMax.z <= ObjPos.z + vtxObjMin.z
			&& pos.z + vtxMax.z > ObjPos.z + vtxObjMin.z
			&& pos.x + vtxMax.x > ObjPos.x + vtxObjMin.x
			&& pos.x + vtxMin.x < ObjPos.x + vtxObjMax.x)
		{//手前から奥にめり込んだt
		 //位置を戻す
			move.z *= -0.5f;
			pos.z = ObjPos.z + vtxObjMin.z - vtxMax.z - 0.1f;
			m_bPress = true;
		}
	}

	return bLand;
}
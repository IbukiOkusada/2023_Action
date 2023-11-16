//==========================================================
//
// 風ギミック [gimmick_button.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "gimmick_air.h"
#include "manager.h"
#include "slow.h"
#include "debugproc.h"
#include "object3D.h"
#include "texture.h"

// マクロ定義

//==========================================================
// コンストラクタ
//==========================================================
CGimmickAir::CGimmickAir()
{
	m_pObject = NULL;
	m_bRevease = false;
	m_fHeight = 0.0f; 
	m_fWidth = 0.0f;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_moveTex = D3DXVECTOR2(0.0f, 0.0f);
	m_TexPos = D3DXVECTOR2(0.0f, 0.0f);
}

//==========================================================
// デストラクタ
//==========================================================
CGimmickAir::~CGimmickAir()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CGimmickAir::Init(void)
{
	// 読み込み確認
	if (m_pObject == NULL)
	{
		m_pObject = CObject3D::Create(GetPosition(), GetRotation());
	}

	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CGimmickAir::Uninit(void)
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
void CGimmickAir::Update(void)
{
	// 操作関連
	Controller();

	SetMtxWorld();
}

//==========================================================
// 生成
//==========================================================
CGimmickAir *CGimmickAir::Create(void)
{
	CGimmickAir *pObjectAir = new CGimmickAir;

	if (pObjectAir != NULL)
	{
		// 初期化処理
		pObjectAir->Init();
	}

	return pObjectAir;
}

//==========================================================
// 操作関連
//==========================================================
void CGimmickAir::Controller(void)
{
	if (m_pObject == nullptr)
	{
		return;
	}

	m_TexPos += m_moveTex;

	if (m_TexPos.x > 1.0f)
	{
		m_TexPos.x += -1.0f;
	}
	else if (m_TexPos.x < -1.0f)
	{
		m_TexPos.x += 1.0f;
	}
	if (m_TexPos.y > 1.0f)
	{
		m_TexPos.y += -1.0f;
	}
	else if (m_TexPos.y < -1.0f)
	{
		m_TexPos.y += 1.0f;
	}

	// 更新
	m_pObject->SetpVtx(m_fWidth, m_fHeight);
	m_pObject->SetTextureVtx(m_TexPos.x, m_TexPos.y);
	m_pObject->SetPosition(GetPosition());
}

//==========================================================
// 個別判定チェック
//==========================================================
bool CGimmickAir::CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, const D3DXVECTOR3& vtxMin, const D3DXVECTOR3& vtxMax, int &nDamage)
{
	bool bLand = false;	// 着地したか否か
	D3DXVECTOR3 vtxObjMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxObjMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 ObjPos = GetPosition();
	D3DXVECTOR3 ObjRot = GetRotation();

	if (pos.x >= ObjPos.x - m_fWidth &&
		pos.x <= ObjPos.x + m_fWidth &&
		pos.z >= ObjPos.z - m_fHeight &&
		pos.z <= ObjPos.z + m_fHeight)
	{
		CManager::GetInstance()->GetDebugProc()->Print("風に当たったよー\n");
		// 範囲内
		move += m_move;
		nDamage = -1;
	}

	return bLand;
}

//==========================================================
// 反転
//==========================================================
void CGimmickAir::Reverse(void)
{
	if (!m_bRevease){
		SetMove(m_move * -1.0f);
	}
}

//==========================================================
// 移動量設定
//==========================================================
void CGimmickAir::SetMove(D3DXVECTOR3 move)
{
	m_move = move;

	if (m_pObject == nullptr)
	{
		return;
	}

	m_moveTex = D3DXVECTOR2(m_move.x * -0.005f, m_move.z * 0.005f);

	D3DXVECTOR3 vec = m_move;
	D3DXVec3Normalize(&vec, &vec);

	if (vec.x != 0.0f)
	{// 横の風
		m_pObject->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\air_rightleft.png"));
	}
	else if (vec.z != 0.0f)
	{// 縦の風
		m_pObject->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\air_updown.png"));
	}
}
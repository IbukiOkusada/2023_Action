//==========================================================
//
// 風ギミック [gimmick_button.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "gimmick_air.h"
#include "manager.h"
#include "slow.h"
#include "Xfile.h"
#include "objectX.h"
#include "debugproc.h"

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
	
}

//==========================================================
// 個別判定チェック
//==========================================================
bool CGimmickAir::CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, int &nDamage, const float fRefMulti)
{
	CXFile *pFile = CManager::GetInstance()->GetModelFile();
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

void CGimmickAir::Reverse(void)
{
	if (!m_bRevease){
		m_move *= -1.0f;
	}
}
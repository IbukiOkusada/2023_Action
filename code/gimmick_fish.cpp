//==========================================================
//
// 魚ギミック(奥から出てくる) [gimmick_fish.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "gimmick_fish.h"
#include "manager.h"
#include "slow.h"
#include "Xfile.h"

// マクロ定義

//==========================================================
// コンストラクタ
//==========================================================
CGimmickFish::CGimmickFish(int nPriOrity) : CObjectX(nPriOrity)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fNowFishTimer = 0.0f;
	m_fNumFishTimer = 0.0f;
}

//==========================================================
// デストラクタ
//==========================================================
CGimmickFish::~CGimmickFish()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CGimmickFish::Init(void)
{
	if (CObjectX::Init() == E_FAIL)
	{
		return E_FAIL;
	}

	// 読み込み確認
	CXFile *pFile = CManager::GetModelFile();
	BindFile(pFile->Regist("data\\MODEL\\5mcube.x"));

	// スローを覚える
	m_pSlow = CManager::GetSlow();

	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CGimmickFish::Uninit(void)
{
	CObjectX::Uninit();

	Release();
}

//==========================================================
// 更新処理
//==========================================================
void CGimmickFish::Update(void)
{
	m_posOld = GetPosition();

	// 操作関連
	Controller();
}

//==========================================================
// 描画処理
//==========================================================
void CGimmickFish::Draw(void)
{
	CObjectX::Draw();
}

//==========================================================
// 生成
//==========================================================
CGimmickFish *CGimmickFish::Create(void)
{
	CGimmickFish *pObjectFish = new CGimmickFish;

	if (pObjectFish != NULL)
	{
		// 初期化処理
		pObjectFish->Init();
	}

	return pObjectFish;
}

CGimmickFish *CGimmickFish::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fTimer)
{
	CGimmickFish *pObjectFish = new CGimmickFish;

	if (pObjectFish != NULL)
	{
		// 初期化処理
		pObjectFish->Init();

		// 設定
		pObjectFish->SetPosition(pos);
		pObjectFish->SetFish(move);
		pObjectFish->SetNumTimer(fTimer);
		pObjectFish->SetNowTimer(fTimer);
	}

	return pObjectFish;
}

//==========================================================
// 操作関連
//==========================================================
void CGimmickFish::Controller(void)
{
	// 座標更新
	D3DXVECTOR3 pos = GetPosition();
	pos += m_move;
	SetPosition(pos);

	if (m_fNumFishTimer == 0.0f || !m_pSlow)
	{// 往復しない
		return;
	}

	m_fNowFishTimer -= m_pSlow->Get();

	if (m_fNowFishTimer <= 0.0f)
	{
		m_move *= -1.0f;
		m_fNowFishTimer = m_fNumFishTimer;
	}
}

//==========================================================
// 個別判定チェック
//==========================================================
bool CGimmickFish::CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, const float fRefMulti)
{
	CXFile *pFile = CManager::GetModelFile();
	bool bLand = false;	// 着地したか否か
	D3DXVECTOR3 vtxObjMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxObjMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 ObjPos = GetPosition();
	D3DXVECTOR3 ObjRot = GetRotation();

	// 向きを反映
	SetRotSize(vtxObjMax,
		vtxObjMin,
		pFile->GetMax(GetIdx()),
		pFile->GetMin(GetIdx()),
		ObjRot.y);

	if (pos.y + vtxMax.y > ObjPos.y + vtxObjMin.y
		&& pos.y + vtxMin.y <= ObjPos.y + vtxObjMax.y)
	{//プレイヤーとモデルが同じ高さにある
		if (posOld.x + vtxMin.x >= m_posOld.x + vtxObjMax.x
			&& pos.x + vtxMin.x < ObjPos.x + vtxObjMax.x
			&& pos.z + vtxMax.z > ObjPos.z + vtxObjMin.z
			&& pos.z + vtxMin.z < ObjPos.z + vtxObjMax.z)
		{//右から左にめり込んだ
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
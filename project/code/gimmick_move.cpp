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
#include "character.h"
#include "motion.h"
#include "shadow.h"
#include "meshfield.h"
#include "particle.h"
#include "renderer.h"

// マクロ定義
#define COLLISION_SIZE		(50.0f)
#define DAMAGE		(1)

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
		m_pObject = CCharacter::Create("data\\TXT\\motion_bee.txt");
		m_pObject->GetMotion()->InitSet(0);
	}

	// 影の生成
	if (nullptr == m_pShadow)
	{
		//m_pShadow = CShadow::Create(GetPosition(), 50.0f, 50.0f);
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
		delete m_pObject;
		m_pObject = NULL;
	}

	if (nullptr != m_pShadow) {
		m_pShadow->Uninit();
		m_pShadow = NULL;
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

	if (m_pObject != nullptr)
	{
		m_pObject->Update();
	}

	// 起伏との当たり判定
	D3DXVECTOR3 nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fHeight = CMeshField::GetHeight(GetPosition());

	// 影の設定
	if (nullptr != m_pShadow) {
		m_pShadow->SetPosition(D3DXVECTOR3(GetPosition().x, fHeight + 1.0f, GetPosition().z));
	}

	D3DXMATRIX mtxProjection;
	D3DXMATRIX mtxView;
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 ScreenPos;
	D3DVIEWPORT9 Viewport;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProjection);	// プロジェクションマトリックスを取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);				// ビューマトリックスを取得
	pDevice->GetViewport(&Viewport);							// ビューポートを取得

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// ワールド座標からスクリーン座標に変換する
	D3DXVec3Project(&ScreenPos, &pos, &Viewport, &mtxProjection, &mtxView, &mtxWorld);

	if (ScreenPos.x < 0.0f || ScreenPos.x > SCREEN_WIDTH ||
		ScreenPos.y < 0.0f || ScreenPos.y > SCREEN_HEIGHT || ScreenPos.z >= 1.0f)
	{// 画面に描画されていない
		return;
	}

	CParticle::Create(GetPosition(), GetRotation(), CEffect::TYPE_SHWBULLET);
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
CGimmickMove *CGimmickMove::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, float fTimer)
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
	D3DXVECTOR3 rot = GetRotation();
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

		D3DXVECTOR2 vec;
		vec.y = -m_move.x;
		vec.x = -m_move.z;
		D3DXVec2Normalize(&vec, &vec);
		rot.y = atan2f(vec.y, vec.x);
		SetRotation(rot);
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
bool CGimmickMove::CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, const D3DXVECTOR3& vtxMin, const D3DXVECTOR3& vtxMax, int &nDamage)
{
	bool bLand = false;	// 着地したか否か
	D3DXVECTOR3 vtxObjMax = D3DXVECTOR3(COLLISION_SIZE, COLLISION_SIZE, COLLISION_SIZE);
	D3DXVECTOR3 vtxObjMin = D3DXVECTOR3(-COLLISION_SIZE, -COLLISION_SIZE, -COLLISION_SIZE);
	D3DXVECTOR3 ObjPos = GetPosition();
	D3DXVECTOR3 ObjRot = GetRotation();

	if (m_pObject == NULL)
	{
		return bLand;
	}

	// 向きを反映

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
			nDamage = DAMAGE;
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
			nDamage = DAMAGE;
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
			nDamage = DAMAGE;
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
			nDamage = DAMAGE;
		}
	}

	return bLand;
}
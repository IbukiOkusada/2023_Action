//==========================================================
//
// 回転ギミック [gimmick_rotate.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "gimmick_rotate.h"
#include "model.h"
#include "Xfile.h"
#include "manager.h"
#include "debugproc.h"
#include "character.h"
#include "motion.h"
#include "shadow.h"
#include "meshfield.h"
#include "particle.h"
#include "effect.h"
#include "renderer.h"

// マクロ定義
#define FILENAME "data\\TXT\\motion_bird.txt"	// 使用モデル
#define SETSIZE	(100.0f)
#define DAMAGE		(1)

//==========================================================
// コンストラクタ
//==========================================================
CGimmickRotate::CGimmickRotate()
{
	for (int nCnt = 0; nCnt < NUM_ROTATEBOX; nCnt++)
	{
		m_aObj[nCnt].s_pModel = NULL;
		m_aObj[nCnt].s_pShadow = NULL;
	}

	m_RotateSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//==========================================================
// デストラクタ
//==========================================================
CGimmickRotate::~CGimmickRotate()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CGimmickRotate::Init(void)
{
	m_fSize = SETSIZE;
	m_RotateSpeed = D3DXVECTOR3(0.0f, 0.01f, 0.0f);

	for (int nCnt = 0; nCnt < NUM_ROTATEBOX; nCnt++)
	{
		m_aObj[nCnt].s_pModel = CCharacter::Create(FILENAME);
		m_aObj[nCnt].s_pModel->SetParent(GetMtxWorld());
		m_aObj[nCnt].s_pModel->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 
			((-SETSIZE * (int)(NUM_ROTATEBOX * 0.5f)))
			+ (SETSIZE * nCnt) 
			+ (SETSIZE * ((1 - NUM_ROTATEBOX % 2) * 0.5f))));
		m_aObj[nCnt].s_pModel->GetMotion()->InitSet(0);

		if (nullptr == m_aObj[nCnt].s_pShadow)
		{
			//m_aObj[nCnt].s_pShadow = CShadow::Create(GetPosition(), 60.0f, 60.0f);
		}
	}
	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CGimmickRotate::Uninit(void)
{
	for (int nCnt = 0; nCnt < NUM_ROTATEBOX; nCnt++)
	{
		if (m_aObj[nCnt].s_pModel != NULL)
		{
			m_aObj[nCnt].s_pModel->Uninit();
			delete m_aObj[nCnt].s_pModel;
			m_aObj[nCnt].s_pModel = NULL;

			if (nullptr != m_aObj[nCnt].s_pShadow) {
				m_aObj[nCnt].s_pShadow->Uninit();
				m_aObj[nCnt].s_pShadow = NULL;
			}
		}
	}

	ListOut();

	// 廃棄
	Release();
}

//==========================================================
// 更新処理
//==========================================================
void CGimmickRotate::Update(void)
{
	// 向きの設定
	D3DXVECTOR3 rot = GetRotation();
	rot += m_RotateSpeed;

	if (rot.y > D3DX_PI)
	{
		rot.y += -D3DX_PI * 2;
	}
	else if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2;
	}

	SetRotation(rot);

	SetMtxWorld();

	for (int nCnt = 0; nCnt < NUM_ROTATEBOX; nCnt++)
	{
		if (m_aObj[nCnt].s_pModel != NULL)
		{
			m_aObj[nCnt].s_posOld.x = m_aObj[nCnt].s_pModel->GetMtxWorld()->_41;
			m_aObj[nCnt].s_posOld.y = m_aObj[nCnt].s_pModel->GetMtxWorld()->_42;
			m_aObj[nCnt].s_posOld.z = m_aObj[nCnt].s_pModel->GetMtxWorld()->_43;
			m_aObj[nCnt].s_pModel->Update();

			if (nullptr != m_aObj[nCnt].s_pShadow) {
				// 起伏との当たり判定
				D3DXVECTOR3 nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
					
				}
				else
				{
					CParticle::Create(m_aObj[nCnt].s_posOld, CEffect::TYPE_SWEAT);
				}
			}
		}
	}
}

//==========================================================
// 生成
//==========================================================
CGimmickRotate *CGimmickRotate::Create(void)
{
	CGimmickRotate *pObjectRotate = new CGimmickRotate;

	if (pObjectRotate != NULL)
	{
		// 初期化処理
		pObjectRotate->Init();
	}

	return pObjectRotate;
}

//==========================================================
// 個別判定チェック
//==========================================================
bool CGimmickRotate::CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, const D3DXVECTOR3& vtxMin, const D3DXVECTOR3& vtxMax, int &nDamage)
{
	bool bLand = false;	// 着地したか否か
	D3DXVECTOR3 vtxObjMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxObjMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 ObjPos = GetPosition();
	D3DXVECTOR3 ObjRot = GetRotation();

	// 範囲内チェック
	{
		float fLength =
			sqrtf((pos.x - ObjPos.x) * (pos.x - ObjPos.x)
				+ (pos.z - ObjPos.z) * (pos.z - ObjPos.z));

		if (fLength > SETSIZE * NUM_ROTATEBOX * 0.5f)
		{
			return bLand;
		}
	}

	// 回転している子供との判定
	for (int nCnt = 0; nCnt < NUM_ROTATEBOX; nCnt++)
	{
		if (m_aObj[nCnt].s_pModel == NULL)
		{
			continue;
		}

		ObjPos.x = m_aObj[nCnt].s_pModel->GetMtxWorld()->_41;
		ObjPos.y = m_aObj[nCnt].s_pModel->GetMtxWorld()->_42;
		ObjPos.z = m_aObj[nCnt].s_pModel->GetMtxWorld()->_43;

		// 距離を取得
		float fLength = sqrtf((pos.x - ObjPos.x) * (pos.x - ObjPos.x)
			+ (pos.z - ObjPos.z) * (pos.z - ObjPos.z));

		float fSize = m_fSize * 0.75f;

		if (fLength > fSize)
		{// 触れていない
			continue;
		}

		bLand = true;
		nDamage = DAMAGE;

		// 触れているのでめり込みを直す
		D3DXVECTOR3 vec = D3DXVECTOR3(pos.x - ObjPos.x, pos.y - ObjPos.y, pos.z - ObjPos.z);
		D3DXVec3Normalize(&vec, &vec);
		pos = D3DXVECTOR3(ObjPos.x + fSize * vec.x, ObjPos.y + fSize * vec.y, ObjPos.z + fSize * vec.z);

		// 移動量を補正
		D3DXVECTOR3 movevec;
		D3DXVec3Normalize(&movevec, &move);
		D3DXVECTOR3 vecDiff = movevec - vec;
		D3DXVec3Normalize(&vecDiff, &vecDiff);
		//move = D3DXVECTOR3(move.x + move.x * vecDiff.x, move.y + move.y * vecDiff.y, move.z +  move.z * vecDiff.z);
	}

	return bLand;
}

//==========================================================
// 回転速度設定
//==========================================================
void CGimmickRotate::SetRotate(D3DXVECTOR3 rotate)
{ 
	m_RotateSpeed = rotate; 
	SetRotationCharacter();
}

//==========================================================
// 回転オブジェクトの向きを設定
//==========================================================
void CGimmickRotate::SetRotationCharacter(void)
{
	if (NUM_ROTATEBOX <= 0)
	{
		return;
	}

	for (int nCnt = 0; nCnt < NUM_ROTATEBOX; nCnt++)
	{
		if (m_aObj[nCnt].s_pModel != NULL)
		{
			if (m_RotateSpeed.y > 0.0f)
			{
				m_aObj[nCnt].s_pModel->SetRotation(D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f * (1.0f - (nCnt / (int)(NUM_ROTATEBOX * 0.5f)) * 2),
					0.0f));
			}
			else
			{
				m_aObj[nCnt].s_pModel->SetRotation(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f * (1.0f - (nCnt / (int)(NUM_ROTATEBOX * 0.5f)) * 2),
					0.0f));
			}
		}
	}
}
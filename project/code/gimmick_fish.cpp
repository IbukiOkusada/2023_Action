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
#include "debugproc.h"
#include "character.h"
#include "motion.h"

// マクロ定義
#define WAIT_COUNTER	(180.0f)
#define ATK_COUNTER	(60.0f)
#define UP_COUNTER	(60.0f)
#define DOWN_COUNTER	(30.0f)
#define ATK_ENDCOUNTER	(30.0f)
#define COLSIZE		(300)
#define DAMAGE		(4)

//==========================================================
// コンストラクタ
//==========================================================
CGimmickFish::CGimmickFish()
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fNowFishTimer = 0.0f;
	m_fNumFishTimer = 0.0f;
	m_state = STATE_NONE;
	m_pCharacter = NULL;
	m_bEat = false;
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
	// 読み込み確認
	if (m_pCharacter == NULL)
	{
		m_pCharacter = CCharacter::Create("data\\TXT\\motion_fish.txt");
		m_pCharacter->SetParent(GetMtxWorld());
		m_pCharacter->GetMotion()->InitSet(MOTION_NEUTRAL);
	}

	// スローを覚える
	m_pSlow = CManager::GetInstance()->GetSlow();

	m_state = STATE_WAIT;
	m_fStateCounter = WAIT_COUNTER;

	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CGimmickFish::Uninit(void)
{
	if (m_pCharacter != NULL)
	{
		m_pCharacter->Uninit();
		delete m_pCharacter;
		m_pCharacter = NULL;
	}

	ListOut();

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

	SetMtxWorld();

	if (m_pCharacter != NULL)
	{
		m_pCharacter->Update();
	}
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

CGimmickFish *CGimmickFish::Create(D3DXVECTOR3& pos, D3DXVECTOR3& move, float fTimer)
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
	m_fStateCounter -= CManager::GetInstance()->GetSlow()->Get();
	D3DXVECTOR3 pos = GetPosition();
	pos += m_move * CManager::GetInstance()->GetSlow()->Get();
	SetPosition(pos);

	// 状態ごとに更新
	if (m_fStateCounter <= 0.0f)
	{
		switch (m_state)
		{
		case STATE_NONE:

			break;

		case STATE_UP:

			m_state = STATE_ATK;
			m_fStateCounter = ATK_COUNTER;
			m_move = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			m_pCharacter->GetMotion()->BlendSet(MOTION_BITE);

			break;

		case STATE_ATK:

			m_state = STATE_ATKEND;
			m_fStateCounter = ATK_ENDCOUNTER;
			m_move = D3DXVECTOR3(0.0f, -2.0f, 0.0f);
			if (m_bEat)
			{
				m_pCharacter->GetMotion()->BlendSet(MOTION_AFTERGRAW);
			}
			else
			{
				m_pCharacter->GetMotion()->BlendSet(MOTION_NEUTRAL);
			}
			m_bEat = false;

			break;

		case STATE_ATKEND:

			m_state = STATE_DOWN;
			m_fStateCounter = DOWN_COUNTER;
			m_move = D3DXVECTOR3(0.0f, -21.0f, 0.0f);

			break;

		case STATE_DOWN:

			m_state = STATE_WAIT;
			m_fStateCounter = WAIT_COUNTER;
			m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_pCharacter->GetMotion()->BlendSet(MOTION_NEUTRAL);

			break;

		case STATE_WAIT:

			m_state = STATE_UP;
			m_fStateCounter = UP_COUNTER;
			m_move = D3DXVECTOR3(0.0f, 10.5f, 0.0f);
			m_pCharacter->GetMotion()->Set(MOTION_JUMP);
			break;

		default:

			break;
		}
	}

	CManager::GetInstance()->GetDebugProc()->Print("カウント [ %f ], 状態 [ %d ]\n", m_fStateCounter, m_state);
}

//==========================================================
// 個別判定チェック
//==========================================================
bool CGimmickFish::CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, const D3DXVECTOR3& vtxMin, const D3DXVECTOR3& vtxMax, int &nDamage)
{
	bool bLand = false;	// 着地したか否か

	if (m_state != STATE_ATK || m_pCharacter->GetMotion()->GetNowMotion() != m_pCharacter->GetMotion()->GetOldMotion())
	{
		return bLand;
	}

	D3DXVECTOR3 ObjPos = GetPosition();
	D3DXVECTOR3 ObjRot = GetRotation();

	// 距離を取得
	float fLength = sqrtf((pos.x - ObjPos.x) * (pos.x - ObjPos.x)
		+ (pos.z - ObjPos.z) * (pos.z - ObjPos.z));

	if (fLength < COLSIZE)
	{
		nDamage = DAMAGE;
		m_bEat = true;
		bLand = true;
	}

	return bLand;
}
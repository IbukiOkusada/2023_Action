//===============================================
//
// プレイヤーの処理 [player.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "camera.h"
#include "Xfile.h"
#include "slow.h"
#include "texture.h"
#include "meshfield.h"
#include "Xfile.h"
#include "meshorbit.h"
#include "game.h"
#include "filter.h"
#include "object2D.h"
#include "meshwall.h"
#include "objectX.h"
#include "sound.h"
#include "shadow.h"
#include "gimmick.h"

//===============================================
// マクロ定義
//===============================================
#define MOVE	(0.1f)		// 移動量
#define SHW_MOVE	(1.0f)	// シャワー中移動量
#define PLAYER_GRAVITY	(-0.15f)		//プレイヤー重力
#define PLAYER_JUMP		(10.0f)		//プレイヤージャンプ力
#define ROT_MULTI	(0.075f)		// 向き補正倍率
#define WIDTH	(20.0f)		// 幅
#define HEIGHT	(80.0f)		// 高さ
#define INER	(0.003f)	// 慣性
#define STEP_SPEED	(50.0f)
#define STEP_COOLTIME	(180.0f)
#define STEP_INER	(0.115f)
#define START_LIFE	(4)	// 初期体力
#define DAMAGE_INTERVAL	(120.0f)

//===============================================
// コンストラクタ
//===============================================
//CPlayer::CPlayer()
//{
//	// 値をクリアする
//	m_nCounterAnim = 0;
//	m_nPatternAnim = 0;
//}

//===============================================
// コンストラクタ(オーバーロード)
//===============================================
CPlayer::CPlayer(const D3DXVECTOR3 pos)
{
	// 値をクリアする
	m_Info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRotMove = 0.0f;
	m_fRotDiff = 0.0f;
	m_fRotDest = 0.0f;
	m_pObject = NULL;
	m_pShadow = NULL;
	m_nLife = 0;
}

//===============================================
// コンストラクタ(オーバーロード)
//===============================================
CPlayer::CPlayer(int nPriOrity)
{
	// 値をクリアする
	m_Info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRotMove = 0.0f;
	m_fRotDiff = 0.0f;
	m_fRotDest = 0.0f;
	m_pObject = NULL;
	m_pShadow = NULL;
	m_nLife = 0;
}

//===============================================
// デストラクタ
//===============================================
CPlayer::~CPlayer()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CPlayer::Init(void)
{
	if (m_pObject == NULL)
	{
		m_pObject = CObjectX::Create(GetPosition(), GetRotation(), "data\\MODEL\\triangle.x");
		m_pObject->SetType(CObject::TYPE_PLAYER);
	}

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxRot);

	// 影の生成
	if (m_pShadow == NULL)
	{
		m_pShadow = CShadow::Create(m_Info.pos, 50.0f, 50.0f);
	}

	m_Info.state = STATE_APPEAR;
	m_nLife = START_LIFE;

	return S_OK;
}

//===============================================
// 初期化処理(オーバーロード)
//===============================================
HRESULT CPlayer::Init(const char *pBodyName, const char *pLegName)
{
	if (m_pObject == NULL)
	{
		m_pObject = CObjectX::Create(GetPosition(), GetRotation(), "data\\MODEL\\triangle.x");
		m_pObject->SetType(CObject::TYPE_PLAYER);
	}

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxRot);

	// 影の生成
	if (m_pShadow == NULL)
	{
		m_pShadow = CShadow::Create(m_Info.pos, 50.0f, 50.0f);
	}

	m_nLife = START_LIFE;

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CPlayer::Uninit(void)
{
	if (m_pObject != NULL)
	{
		m_pObject->Uninit();
		m_pObject = NULL;
	}

	if (m_pShadow != NULL)
	{
		m_pShadow->Uninit();
		m_pShadow = NULL;
	}

	// 廃棄
	Release();
}

//===============================================
// 更新処理
//===============================================
void CPlayer::Update(void)
{
	// 前回の座標を取得
	m_Info.posOld = GetPosition();

	if (m_nLife <= 0)
	{
		return;
	}

	// プレイヤー操作
	Controller();

	// カメラ追従
	CCamera *pCamera = CManager::GetInstance()->GetCamera();

	// 追従処理
	pCamera->Pursue(GetPosition(), GetRotation());

	CManager::GetInstance()->GetDebugProc()->Print("向き [%f, %f, %f]", GetRotation().x, GetRotation().y, GetRotation().z);
	CManager::GetInstance()->GetDebugProc()->Print("位置 [%f, %f, %f]", GetPosition().x, GetPosition().y, GetPosition().z);
	CManager::GetInstance()->GetDebugProc()->Print("体力 [ %d ]\n", m_nLife);

	// 回転軸の設定

	// パーティクル
	//Particle();

	if (m_nLife <= 0 && m_Info.state == STATE_NORMAL)
	{
		m_pObject->SetDraw(false);
		m_pShadow->SetDraw(false);
		m_Info.state = STATE_DEATH;
	}
}

//===============================================
// 生成
//===============================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, const char *pBodyName, const char *pLegName, const int nPriority)
{
	CPlayer *pPlayer = NULL;
	CXFile *pModelFile = CManager::GetInstance()->GetModelFile();

	// オブジェクト2Dの生成
	pPlayer = new CPlayer(nPriority);

	if (pPlayer != NULL)
	{// 生成できた場合
		// 初期化処理
		pPlayer->Init(pBodyName, pLegName);

		// 座標設定
		pPlayer->SetPosition(pos);

		// 向き設定
		pPlayer->SetRotation(rot);

		pPlayer->m_fRotDest = rot.y;

		// 移動量設定
		pPlayer->SetMove(move);

		// 種類の設定
		//pPlayer->SetType(TYPE_PLAYER);

		CManager::GetInstance()->GetCamera()->Setting(pos, rot);
	}
	else
	{// 生成に失敗した場合
		return NULL;
	}

	return pPlayer;
}

//===============================================
// 操作処理
//===============================================
void CPlayer::Controller(void)
{
	D3DXVECTOR3 pos = GetPosition();	// 座標を取得
	D3DXVECTOR3 rot = GetRotation();	// 向きを取得
	CInputKeyboard *pInputKey = CManager::GetInstance()->GetInputKeyboard();	// キーボードのポインタ
	CInputMouse *pInputMouse = CManager::GetInstance()->GetInputMouse();
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		// カメラのポインタ
	D3DXVECTOR3 CamRot = pCamera->GetRotation();	// カメラの角度
	bool bDamage = false;
	m_fRotMove = rot.y;	//現在の向きを取得

	// 操作処理
	{
		Move();		// 移動
		Rotation();	// 回転
	}

	pos = GetPosition();	// 座標を取得

	if (m_fStepCoolTime <= 0.0f)
	{
		m_Info.move.x += (0.0f - m_Info.move.x) * INER;	//x座標
		m_Info.move.z += (0.0f - m_Info.move.z) * INER;	//x座標
	}
	else
	{
		m_fStepCoolTime -= CManager::GetInstance()->GetSlow()->Get();
		m_Info.move.x += (0.0f - m_Info.move.x) * STEP_INER;	//x座標
		m_Info.move.z += (0.0f - m_Info.move.z) * STEP_INER;	//x座標
	}

	pos.x += m_Info.move.x * CManager::GetInstance()->GetSlow()->Get();
	pos.z += m_Info.move.z * CManager::GetInstance()->GetSlow()->Get();

	// 調整
	Adjust();

	// オブジェクトとの当たり判定
	if (m_pObject != NULL)
	{
		CXFile *pFile = CManager::GetInstance()->GetModelFile();
		D3DXVECTOR3 vtxMax = pFile->GetMax(m_pObject->GetIdx());
		D3DXVECTOR3 vtxMin = pFile->GetMin(m_pObject->GetIdx());

		// ギミック
		if (CGimmick::Collision(pos, m_Info.posOld, m_Info.move, vtxMin, vtxMax, 0.3f))
		{
			bDamage = true;
			CManager::GetInstance()->GetDebugProc()->Print("***ギミックと当たったよ***\n");
		}

		// 壁
		if (CObjectX::Collision(pos, m_Info.posOld, m_Info.move, vtxMin, vtxMax, 0.3f))
		{
			bDamage = true;
			CManager::GetInstance()->GetDebugProc()->Print("***壁と当たったよ***\n");
		}
	}

	// ダメージ確認
	if (m_Info.state == STATE_NORMAL)
	{
		if (bDamage)
		{
			m_nLife--;
			m_Info.fStateCounter = DAMAGE_INTERVAL;
			m_Info.state = STATE_DAMAGE;
		}
	}

	// 壁との当たり判定
	CMeshWall::Collision(pos, m_Info.posOld);

	// 起伏との当たり判定
	D3DXVECTOR3 nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fHeight = CMeshField::GetHeight(pos);

	m_Info.pos = pos;

	if (m_pObject != NULL){
		m_pObject->SetPosition(m_Info.pos);
		m_pObject->SetRotation(m_Info.rot);
	}

	// 影の設定
	if (m_pShadow != NULL) {
		m_pShadow->SetPosition(D3DXVECTOR3(pos.x, fHeight + 1.0f, pos.z));
	}

	//デバッグ表示
	CManager::GetInstance()->GetDebugProc()->Print("\n回転[W,A,S,D] : 加速[SPACE] : ステップ[K, (Rボタン)]\n");
}

//===============================================
// 移動
//===============================================
void CPlayer::Move(void)
{
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		// カメラのポインタ
	D3DXVECTOR3 CamRot = pCamera->GetRotation();	// カメラの角度
	CInputKeyboard *pInputKey = CManager::GetInstance()->GetInputKeyboard();	// キーボードのポインタ
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();
	float fSpeed = MOVE;	// 移動量

	// 入力装置確認
	if (pInputKey == NULL){
		return;
	}

	// 入力装置確認
	if (pInputPad == NULL){
		return;
	}

	if (pInputKey->GetTrigger(DIK_K) || pInputPad->GetTrigger(CInputPad::BUTTON_RIGHTBUTTON, 0))
	{
		if (m_fStepCoolTime <= 0.0f)
		{
			m_fStepCoolTime = STEP_COOLTIME;
			m_Info.move.x += -sinf(m_Info.rot.y) * STEP_SPEED;
			m_Info.move.z += -cosf(m_Info.rot.y) * STEP_SPEED;
		}
	}

	//プレイヤーの更新
	if (pInputKey->GetPress(DIK_SPACE) || pInputPad->GetPress(CInputPad::BUTTON_A, 0))
	{
		m_Info.move.x += -sinf(m_Info.rot.y) * fSpeed;
		m_Info.move.z += -cosf(m_Info.rot.y) * fSpeed;
	}
}

//===============================================
// 回転
//===============================================
void CPlayer::Rotation(void)
{
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		// カメラのポインタ
	D3DXVECTOR3 CamRot = pCamera->GetRotation();	// カメラの角度
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();

	// 入力装置確認
	if (pInputPad == NULL){
		return;
	}

	if (!pInputPad->GetStickPress(0, CInputPad::BUTTON_LEFT_X, 0.1f, CInputPad::STICK_PLUS) && 
		!pInputPad->GetStickPress(0, CInputPad::BUTTON_LEFT_X, 0.1f, CInputPad::STICK_MINUS) &&
		!pInputPad->GetStickPress(0, CInputPad::BUTTON_LEFT_Y, 0.1f, CInputPad::STICK_PLUS) &&
		!pInputPad->GetStickPress(0, CInputPad::BUTTON_LEFT_Y, 0.1f, CInputPad::STICK_MINUS))
	{// コントローラー入力無し
		KeyBoardRotation();
		return;
	}

	D3DXVECTOR2 vec;
	vec.y = pInputPad->GetStickAdd(0, CInputPad::BUTTON_LEFT_X, 0.1f, CInputPad::STICK_PLUS);
	vec.x = pInputPad->GetStickAdd(0, CInputPad::BUTTON_LEFT_Y, 0.1f, CInputPad::STICK_PLUS);
	D3DXVec2Normalize(&vec, &vec);

	m_fRotDest = atan2f(vec.y, vec.x);
}

//===============================================
// 回転
//===============================================
void CPlayer::KeyBoardRotation(void)
{
	CInputKeyboard *pInputKey = CManager::GetInstance()->GetInputKeyboard();	// キーボードのポインタ

	if (pInputKey == NULL){
		return;
	}

	if (pInputKey->GetPress(DIK_W))
	{
		if (pInputKey->GetPress(DIK_A))
		{
			m_fRotDest = -D3DX_PI * 0.25f;
		}
		else if (pInputKey->GetPress(DIK_D))
		{
			m_fRotDest = D3DX_PI * 0.25f;
		}
		else
		{
			m_fRotDest = D3DX_PI * 0.0f;
		}
	}
	else if (pInputKey->GetPress(DIK_S))
	{
		if (pInputKey->GetPress(DIK_A))
		{
			m_fRotDest = -D3DX_PI * 0.75f;
		}
		else if (pInputKey->GetPress(DIK_D))
		{
			m_fRotDest = D3DX_PI * 0.75f;
		}
		else
		{
			m_fRotDest = D3DX_PI * 1.0f;
		}
	}
	else if (pInputKey->GetPress(DIK_A))
	{
		m_fRotDest = -D3DX_PI * 0.5f;
	}
	else if (pInputKey->GetPress(DIK_D))
	{
		m_fRotDest = D3DX_PI * 0.5f;
	}
}

//===============================================
// 調整
//===============================================
void CPlayer::Adjust(void)
{
	while (1)
	{
		if (m_fRotDest > D3DX_PI || m_fRotDest < -D3DX_PI)
		{//-3.14～3.14の範囲外の場合
			if (m_fRotDest > D3DX_PI)
			{
				m_fRotDest += (-D3DX_PI * 2);
			}
			else if (m_fRotDest < -D3DX_PI)
			{
				m_fRotDest += (D3DX_PI * 2);
			}
		}
		else
		{
			break;
		}
	}

	m_fRotDiff = m_fRotDest - m_fRotMove;	//目標までの移動方向の差分

	while (1)
	{
		if (m_fRotDiff > D3DX_PI || m_fRotDiff < -D3DX_PI)
		{//-3.14～3.14の範囲外の場合
			if (m_fRotDiff > D3DX_PI)
			{
				m_fRotDiff += (-D3DX_PI * 2);
			}
			else if (m_fRotDiff < -D3DX_PI)
			{
				m_fRotDiff += (D3DX_PI * 2);
			}
		}
		else
		{
			break;
		}
	}

	m_Info.rot.y += m_fRotDiff * ROT_MULTI;

	while (1)
	{
		if (m_Info.rot.y > D3DX_PI || m_Info.rot.y < -D3DX_PI)
		{//-3.14～3.14の範囲外の場合
			if (m_Info.rot.y > D3DX_PI)
			{
				m_Info.rot.y += (-D3DX_PI * 2);
			}
			else if (m_Info.rot.y < -D3DX_PI)
			{
				m_Info.rot.y += (D3DX_PI * 2);
			}
		}
		else
		{
			break;
		}
	}
}

//===============================================
// 状態管理
//===============================================
void CPlayer::StateSet(void)
{
	switch (m_Info.state)
	{
	case STATE_APPEAR:

		break;

	case STATE_NORMAL:

		break;

	case STATE_DAMAGE:

		break;

	case STATE_DEATH:

		if(m_Info.fStateCounter)
		break;
	}
}
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

	// プレイヤー操作
	Controller();

	// カメラ追従
	CCamera *pCamera = CManager::GetCamera();

	// 追従処理
	pCamera->Pursue(GetPosition(), GetRotation());

	CManager::GetDebugProc()->Print("向き [%f, %f, %f]", GetRotation().x, GetRotation().y, GetRotation().z);

	// パーティクル
	//Particle();
}

//===============================================
// 生成
//===============================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, const char *pBodyName, const char *pLegName, const int nPriority)
{
	CPlayer *pPlayer = NULL;
	CXFile *pModelFile = CManager::GetModelFile();

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

		CManager::GetCamera()->Setting(pos, rot);
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
	CInputKeyboard *pInputKey = CManager::GetInputKeyboard();	// キーボードのポインタ
	CInputMouse *pInputMouse = CManager::GetInputMouse();
	CInputPad *pInputPad = CManager::GetInputPad();
	CCamera *pCamera = CManager::GetCamera();		// カメラのポインタ
	D3DXVECTOR3 CamRot = pCamera->GetRotation();	// カメラの角度
	m_fRotMove = rot.y;	//現在の向きを取得

	// 操作処理
	{
		Move();		// 移動
		Rotation();	// 回転
	}

	m_Info.move.x += (0.0f - m_Info.move.x) * INER;	//x座標
	m_Info.move.z += (0.0f - m_Info.move.z) * INER;	//x座標
	pos.x += m_Info.move.x * CManager::GetSlow()->Get();
	pos.z += m_Info.move.z * CManager::GetSlow()->Get();

	// 調整
	Adjust();

	// オブジェクトとの当たり判定
	if (m_pObject != NULL)
	{
		CXFile *pFile = CManager::GetModelFile();
		D3DXVECTOR3 vtxMax = pFile->GetMax(m_pObject->GetIdx());
		D3DXVECTOR3 vtxMin = pFile->GetMin(m_pObject->GetIdx());
		CObjectX::Collision(pos, m_Info.posOld, m_Info.move, vtxMin, vtxMax, 0.3f);
	}

	// 壁との当たり判定
	CMeshWall::Collision(pos, m_Info.posOld);

	// 起伏との当たり判定
	D3DXVECTOR3 nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fHeight = CMeshField::GetHeight(pos);

	m_Info.pos = pos;

	if (m_pObject != NULL)
	{
		m_pObject->SetPosition(m_Info.pos);
	}

	//デバッグ表示
	CManager::GetDebugProc()->Print("\n移動[W,A,S,D] : ジャンプ[SPACE] : 発射[L, マウス左クリック(長押し可)]\n"
		"スロー[ゲージを貯めてENTER, マウス右クリック] : 武器切り替え[Q]\n");
}

//===============================================
// 移動
//===============================================
void CPlayer::Move(void)
{
	CCamera *pCamera = CManager::GetCamera();		// カメラのポインタ
	D3DXVECTOR3 CamRot = pCamera->GetRotation();	// カメラの角度
	CInputKeyboard *pInputKey = CManager::GetInputKeyboard();	// キーボードのポインタ
	CInputPad *pInputPad = CManager::GetInputPad();
	float fSpeed = MOVE;	// 移動量

	// 入力装置確認
	if (!pInputKey){
		return;
	}

	// 入力装置確認
	if (!pInputPad){
		return;
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
	CCamera *pCamera = CManager::GetCamera();		// カメラのポインタ
	D3DXVECTOR3 CamRot = pCamera->GetRotation();	// カメラの角度
	CInputKeyboard *pInputKey = CManager::GetInputKeyboard();	// キーボードのポインタ
	CInputPad *pInputPad = CManager::GetInputPad();
	float fSpeed = MOVE;	// 移動量

	// 入力装置確認
	if (pInputKey == NULL){
		return;
	}

	// 入力装置確認
	if (pInputPad == NULL){
		return;
	}

	if (!pInputPad->GetStickPress(0, CInputPad::BUTTON_LEFT_X, 0.1f, CInputPad::STICK_PLUS) && 
		!pInputPad->GetStickPress(0, CInputPad::BUTTON_LEFT_X, 0.1f, CInputPad::STICK_MINUS) &&
		!pInputPad->GetStickPress(0, CInputPad::BUTTON_LEFT_Y, 0.1f, CInputPad::STICK_PLUS) &&
		!pInputPad->GetStickPress(0, CInputPad::BUTTON_LEFT_Y, 0.1f, CInputPad::STICK_MINUS))
	{
		return;
	}

	D3DXVECTOR2 vec;
	vec.y = pInputPad->GetStickAdd(0, CInputPad::BUTTON_LEFT_X, 0.1f, CInputPad::STICK_PLUS);
	vec.x = pInputPad->GetStickAdd(0, CInputPad::BUTTON_LEFT_Y, 0.1f, CInputPad::STICK_PLUS);
	D3DXVec2Normalize(&vec, &vec);

	m_fRotDest = atan2f(vec.y, vec.x);

	if (m_pObject != NULL)
	{
		m_pObject->SetRotation(m_Info.rot);
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
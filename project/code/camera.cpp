//==========================================================
//
// カメラの処理全般 [camera.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "camera.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "game.h"
#include "pause.h"
#include "meshfield.h"

//==========================================================
// マクロ定義
//==========================================================
#define CAMERA_MOVESPEED	(1.0f)			// 移動量
#define CAMERA_LENGTHMOVE	(1.0f)			// カメラ距離移動量
#define ROTATE_SPEED		(0.03f)			// カメラの回転速度
#define PAD_ROTATE			(0.02f)
#define CAMERA_MAXLENGTH	(5000.0f)		// カメラ最大距離
#define CAMERA_MINLENGTH	(300.0f)		// カメラ最小距離
#define MOUSE_MOVESPEED		(0.2f)			// マウス移動速度
#define MOUSE_ROTATESPEED_X	(0.004f)		// マウス回転速度x軸
#define MOUSE_ROTATESPEED_Z	(0.005f)		// マウス回転速度z軸
#define MOUSE_WHEELSPEED	(0.1f)			// マウスホイール回転速度
#define MAX_SLOWROT			(0.15f)
#define MESSAGERAND			(120)
#define SLOW_CAMERAROT		(0.7f)
#define TITLE_ROTATESPD		(0.0025f)			// タイトル回転量

//==========================================================
// コンストラクタ
//==========================================================
CCamera::CCamera()
{

}

//==========================================================
// デストラクタ
//==========================================================
CCamera::~CCamera()
{

}

//==========================================================
//カメラの初期化処理
//==========================================================
HRESULT CCamera::Init(void)
{
	//各変数の初期化
	m_posR = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_posV = D3DXVECTOR3(1300.0f, 1500.0f, 0.0f);
	m_fLength = sqrtf((m_posV.x - m_posR.x) * (m_posV.x - m_posR.x) + (m_posV.z - m_posR.z) * (m_posV.z - m_posR.z));
	m_rot = D3DXVECTOR3(0.0f, atan2f(m_posR.x - m_posV.x, m_posR.z - m_posV.z), 1.56f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot.z = MIN_CAMERA_ROTZ;
	m_mode = MODE_NORMAL;

	//視点設定
	SetV();

	return S_OK;
}

//==========================================================
//カメラの終了処理
//==========================================================
void CCamera::Uninit(void)
{

}

//==========================================================
//カメラの更新処理
//==========================================================
void CCamera::Update(void)
{
	//if (CManager::GetInstance()->GetEditor()->GetUse() == false)
	{
		//マウスでの視点移動
	}
	/*else
	{
		Edit();
	}*/
	//全視点の移動
	//MoveVR();

	//注視点の移動
	//MoveR();

	//if (m_mode != MODE_SLOWSHW)
	//{
	//	if (CGame::GetPause() != NULL)
	//	{
	//		if (CGame::GetPause()->GetEnable() == false)
	//		{
	//			//視点の移動
	//			MoveV();
	//			MouseCamera();
	//		}
	//	}
	//	else
	//	{
	//		if (CManager::GetInstance()->GetMode() == CScene::MODE_TUTORIAL)
	//		{
	//			//視点の移動
	//			MoveV();
	//			MouseCamera();
	//		}
	//		//視点の移動
	//		//MoveV();
	//	}
	//}
	//else
	//{
	//	MouseCamera();
	//	SlowShw();
	//}

}

//==========================================================
//カメラの設定処理
//==========================================================
void CCamera::SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスへのポインタを取得

	//プロジェクションマトリクスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	//プロジェクションマトリックスの生成[ 透視投影 ]
	/*D3DXMatrixOrthoLH(&m_mtxProjection,
		(float)SCREEN_WIDTH,
		(float)SCREEN_HEIGHT,
		0.0f,
		8000.0f);*/

	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
								D3DXToRadian(45.0f),
								(float)SCREEN_WIDTH / (float) SCREEN_HEIGHT,
								10.0f,
								40000.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	//ビューマックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//==========================================================
//カメラの全移動処理
//==========================================================
void CCamera::MoveVR(void)
{
	CInputKeyboard *pKey = CManager::GetInstance()->GetInputKeyboard();

	//視点注視点の同時移動
	if (pKey->GetPress(DIK_A) == true)
	{//Aキー入力

		//視点の移動
		if (pKey->GetPress(DIK_W) == true)
		{
			m_posV.x += cosf(m_rot.y + (-D3DX_PI * 0.75f)) * CAMERA_MOVESPEED;
			m_posV.z += sinf(m_rot.y + (-D3DX_PI * 0.75f)) * CAMERA_MOVESPEED;
		}
		else if (pKey->GetPress(DIK_S) == true)
		{
			m_posV.x += cosf(m_rot.y + (-D3DX_PI * 0.25f)) * CAMERA_MOVESPEED;
			m_posV.z += sinf(m_rot.y + (-D3DX_PI * 0.25f)) * CAMERA_MOVESPEED;
		}
		else
		{
			m_posV.x += cosf(m_rot.y + (-D3DX_PI * 0.5f)) * CAMERA_MOVESPEED;
			m_posV.z += sinf(m_rot.y + (-D3DX_PI * 0.5f)) * CAMERA_MOVESPEED;
		}
	}
	else if (pKey->GetPress(DIK_D) == true)
	{//Dキー入力

		//視点の移動
		if (pKey->GetPress(DIK_W) == true)
		{
			m_posV.x += cosf(m_rot.y + (D3DX_PI * 0.75f)) * CAMERA_MOVESPEED;
			m_posV.z += sinf(m_rot.y + (D3DX_PI * 0.75f)) * CAMERA_MOVESPEED;
		}
		else if (pKey->GetPress(DIK_S) == true)
		{
			m_posV.x += cosf(m_rot.y + (D3DX_PI * 0.25f)) * CAMERA_MOVESPEED;
			m_posV.z += sinf(m_rot.y + (D3DX_PI * 0.25f)) * CAMERA_MOVESPEED;
		}
		else
		{
			m_posV.x += cosf(m_rot.y + (D3DX_PI * 0.5f)) * CAMERA_MOVESPEED;
			m_posV.z += sinf(m_rot.y + (D3DX_PI * 0.5f)) * CAMERA_MOVESPEED;
		}

	}
	else if (pKey->GetPress(DIK_W) == true)
	{//Wキー入力

		//視点の移動
		m_posV.x += -cosf(m_rot.y) * CAMERA_MOVESPEED;
		m_posV.z += -sinf(m_rot.y) * CAMERA_MOVESPEED;
	}
	else if (pKey->GetPress(DIK_S) == true)
	{//Sキー入力
		//視点の移動
		m_posV.x += cosf(m_rot.y) * CAMERA_MOVESPEED;
		m_posV.z += sinf(m_rot.y) * CAMERA_MOVESPEED;

	}

	//注視点設定
	SetR();
}

//==========================================================
//カメラの視点移動処理
//==========================================================
void CCamera::MoveV(void)
{
	CInputKeyboard *pKey = CManager::GetInstance()->GetInputKeyboard();
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();	// キーボードのポインタ
	float fMultiSlow = 1.0f;

	//x軸の移動
	if (pInputPad->GetStickPress(0, CInputPad::BUTTON_RIGHT_X, 0.0f, CInputPad::STICK_PLUS) == true)
	{//Qキー入力
		m_rot.y += -D3DX_PI * ROTATE_SPEED * pInputPad->GetStickAdd(0, CInputPad::BUTTON_RIGHT_X, 0.0f) * fMultiSlow;
		if (m_rot.y < -D3DX_PI)
		{//角度がΠを超えた場合
			m_rot.y += D3DX_PI * 2;
		}
	}
	else if (pInputPad->GetStickPress(0, CInputPad::BUTTON_RIGHT_X, 0.0f, CInputPad::STICK_MINUS) == true)
	{//Eキー入力
		m_rot.y += -D3DX_PI * ROTATE_SPEED * pInputPad->GetStickAdd(0, CInputPad::BUTTON_RIGHT_X, 0.0f) * fMultiSlow;
	}

	//x軸の移動
	if ((pKey->GetPress(DIK_Z) == true && pKey->GetPress(DIK_C) != true))
	{//Qキー入力
		m_rot.y += -D3DX_PI * ROTATE_SPEED;
		if (m_rot.y < -D3DX_PI)
		{//角度がΠを超えた場合
			m_rot.y += D3DX_PI * 2;
		}
	}
	else if ((pKey->GetPress(DIK_C) == true && pKey->GetPress(DIK_Z) != true))
	{//Eキー入力
		m_rot.y += D3DX_PI * ROTATE_SPEED;
		if (m_rot.y > D3DX_PI)
		{//角度がΠを超えた場合
			m_rot.y += -D3DX_PI * 2;
		}
	}

	//z軸の移動
	if (pInputPad->GetStickPress(0, CInputPad::BUTTON_RIGHT_Y, 0.1f, CInputPad::STICK_PLUS) == true)
	{//Yキー入力
		//角度の変更
		m_rot.z += PAD_ROTATE * pInputPad->GetStickAdd(0, CInputPad::BUTTON_RIGHT_Y, 0.5f) * fMultiSlow;
		if (m_rot.z < MIN_CAMERA_ROTZ)
		{//角度が限界を超えた場合
			m_rot.z = MIN_CAMERA_ROTZ;
		}
		
	}
	else if (pInputPad->GetStickPress(0, CInputPad::BUTTON_RIGHT_Y, 0.1f, CInputPad::STICK_MINUS) == true)
	{//Nキー入力
		//角度の変更
		m_rot.z += PAD_ROTATE * 2 * pInputPad->GetStickAdd(0, CInputPad::BUTTON_RIGHT_Y, 0.5f) * fMultiSlow;

		if (m_rot.z > MAX_CAMERA_ROTZ)
		{//角度が限界を超えた場合
			m_rot.z = MAX_CAMERA_ROTZ;
		}

	}

	//z軸の移動
	if (pKey->GetPress(DIK_Y) == true && pKey->GetPress(DIK_N) != true)
	{//Yキー入力
	 //角度の変更
		m_rot.z += -D3DX_PI * ROTATE_SPEED;
		if (m_rot.z < MIN_CAMERA_ROTZ)
		{//角度が限界を超えた場合
			m_rot.z = MIN_CAMERA_ROTZ;
		}

	}
	else if (pKey->GetPress(DIK_N) == true && pKey->GetPress(DIK_Y) != true)
	{//Nキー入力
	 //角度の変更
		m_rot.z += D3DX_PI * ROTATE_SPEED;

		if (m_rot.z > MAX_CAMERA_ROTZ)
		{//角度が限界を超えた場合
			m_rot.z = MAX_CAMERA_ROTZ;
		}
	}

	////注視点からの距離の操作
	//if (pKey->GetPress(DIK_U) == true && pKey->GetPress(DIK_M) != true)
	//{//Uキー入力
	//	m_fLength -= CAMERA_LENGTHMOVE;

	//	if (m_fLength < CAMERA_MINLENGTH)
	//	{//距離が最小を超えた場合
	//		m_fLength = CAMERA_MINLENGTH;
	//	}
	//	
	//}
	//else if (pKey->GetPress(DIK_M) == true && pKey->GetPress(DIK_U) != true)
	//{//Mキー入力
	//	m_fLength += CAMERA_LENGTHMOVE;

	//	if (m_fLength > CAMERA_MAXLENGTH)
	//	{//距離が最大を超えた場合
	//		m_fLength = CAMERA_MAXLENGTH;
	//	}
	//}

	if (m_mode == MODE_SLOWGUN)
	{
		Slow();
	}

	//視点設定
	SetV();
}

//==========================================================
//カメラの注視点移動処理
//==========================================================
void CCamera::MoveR(void)
{
	CInputKeyboard *pKey = CManager::GetInstance()->GetInputKeyboard();

	//x軸の移動
	if (pKey->GetPress(DIK_Q) == true && pKey->GetPress(DIK_E) != true)
	{//Qキー入力

		//角度の変更
		m_rot.y += D3DX_PI * ROTATE_SPEED;
		if (m_rot.y > D3DX_PI)
		{//角度がΠを超えた場合
			m_rot.y += -D3DX_PI * 2;
		}
	}
	else if (pKey->GetPress(DIK_E) == true && pKey->GetPress(DIK_Q) != true)
	{//Eキー入力

		//角度の変更
		m_rot.y += -D3DX_PI * ROTATE_SPEED;

		if (m_rot.y < -D3DX_PI)
		{//角度がΠを超えた場合
			m_rot.y += D3DX_PI * 2;
		}
	}

	//z軸の移動
	if (pKey->GetPress(DIK_T) == true && pKey->GetPress(DIK_B) != true)
	{//Tキー入力

		//角度の変更
		m_rot.z += D3DX_PI * ROTATE_SPEED;

		if (m_rot.z >  MAX_CAMERA_ROTZ)
		{//角度が限界を超えた場合
			m_rot.z = MAX_CAMERA_ROTZ;
		}
	}
	else if (pKey->GetPress(DIK_B) == true && pKey->GetPress(DIK_T) != true)
	{//Bキー入力
		//角度の変更
		m_rot.z += -D3DX_PI * ROTATE_SPEED;
		if (m_rot.z < MIN_CAMERA_ROTZ)
		{//角度が限界を超えた場合
			m_rot.z = MIN_CAMERA_ROTZ;
		}
	}

	//注視点設定
	SetR();
}

//==========================================================
//カメラ視点設定処理
//==========================================================
void CCamera::SetV(void)
{
	m_posV.y = m_posR.y + cosf(m_rot.z) * m_fLength;
	m_posV.x = m_posR.x + (sinf(m_rot.z) * cosf(m_rot.y)) * m_fLength;
	m_posV.z = m_posR.z + (sinf(m_rot.z) * sinf(m_rot.y)) * m_fLength;
}

//==========================================================
//カメラ注視点設定処理
//==========================================================
void CCamera::SetR(void)
{
	m_posR.y = m_posV.y - cosf(m_rot.z) * m_fLength;
	m_posR.x = m_posV.x - (sinf(m_rot.z) * cosf(m_rot.y)) * m_fLength;
	m_posR.z = m_posV.z - (sinf(m_rot.z) * sinf(m_rot.y)) * m_fLength;
}

//==========================================================
//マウスでのカメラ移動処理
//==========================================================
void CCamera::MouseCamera(void)
{
	CInputMouse *pMouse = CManager::GetInstance()->GetInputMouse();

	//if (pMouse->GetPress(CInputMouse::BUTTON_LBUTTON) == true && pMouse->GetPress(CInputMouse::BUTTON_RBUTTON) == true)
	//{//左右同時に押されているとき
	//	m_posV.x += cosf(m_rot.y + (-D3DX_PI * 0.5f)) * (pMouse->GetCousorMove().x * MOUSE_MOVESPEED) + -cosf(m_rot.y) * (pMouse->GetCousorMove().y * MOUSE_MOVESPEED);
	//	m_posV.z += sinf(m_rot.y + (-D3DX_PI * 0.5f)) * (pMouse->GetCousorMove().x * MOUSE_MOVESPEED) + -sinf(m_rot.y) * (pMouse->GetCousorMove().y * MOUSE_MOVESPEED);

	//	//移動量を加算
	//	m_posV.x += m_posV.x;
	//	m_posV.z += m_posV.z;

	//	//移動量を初期化
	//	m_posV.x = 0.0f;
	//	m_posV.z = 0.0f;

	//	//注視点設定
	//	SetR();
	//}
	//else if (pMouse->GetPress(CInputMouse::BUTTON_LBUTTON) == true)
	//{//左キーが押されているとき
	//	
	//}
	//else if (pMouse->GetPress(CInputMouse::BUTTON_RBUTTON) == true)
	//{//右キーが押されているとき
	//	m_rot.y -= pMouse->GetCousorMove().x * MOUSE_ROTATESPEED_X;
	//	m_rot.z -= pMouse->GetCousorMove().y * MOUSE_ROTATESPEED_Z;

	//	if (m_rot.y > D3DX_PI)
	//	{//角度がΠを超えた場合
	//		m_rot.y = D3DX_PI;
	//		m_rot.y *= -1.0f;
	//	}
	//	else if (m_rot.y < -D3DX_PI)
	//	{//角度がΠを超えた場合
	//		m_rot.y = -D3DX_PI;
	//		m_rot.y *= -1.0f;
	//	}
	//	if (m_rot.z < MIN_CAMERA_ROTZ)
	//	{//角度が限界を超えた場合
	//		m_rot.z = MIN_CAMERA_ROTZ;
	//	}
	//	else if (m_rot.z > MAX_CAMERA_ROTZ)
	//	{//角度が限界を超えた場合
	//		m_rot.z = MAX_CAMERA_ROTZ;
	//	}

	//	//視点設定
	//	SetR();
	//}

	m_rot.y -= pMouse->GetCousorMove().x * MOUSE_ROTATESPEED_X;
	m_rot.z -= pMouse->GetCousorMove().y * 0.003f;

	if (m_rot.y < -D3DX_PI)
	{//角度がΠを超えた場合
		m_rot.y += D3DX_PI * 2;
	}
	else if (m_rot.y > D3DX_PI)
	{//角度がΠを超えた場合
		m_rot.y += -D3DX_PI * 2;
	}
	if (m_rot.z < MIN_CAMERA_ROTZ)
	{//角度が限界を超えた場合
		m_rot.z = MIN_CAMERA_ROTZ;
	}
	else if (m_rot.z > MAX_CAMERA_ROTZ)
	{//角度が限界を超えた場合
		m_rot.z = MAX_CAMERA_ROTZ;
	}

	//ホイールの使用量で距離の変更
	m_fLength += pMouse->GetCousorMove().z * MOUSE_WHEELSPEED;

	if (m_fLength > CAMERA_MAXLENGTH)
	{//距離が最大を超えた場合
		m_fLength = CAMERA_MAXLENGTH;
	}
	else if (m_fLength < CAMERA_MINLENGTH)
	{//距離が最小を超えた場合
		m_fLength = CAMERA_MINLENGTH;
	}

	//視点設定
	SetV();
}

//==========================================================
// 追従処理
//==========================================================
void CCamera::Pursue(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	D3DXVECTOR3 posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 目標の注視点
	D3DXVECTOR3 posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 目標の視点
	D3DXVECTOR3 RDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 注視点の差分
	D3DXVECTOR3 VDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 視点の差分

	//目的の注視点の座標を取得
	posRDest = D3DXVECTOR3(pos.x - sinf(rot.y) * 30.0f, pos.y + 50.0f, pos.z - cosf(rot.y) * 30.0f);

	//目的の視点の座標を獲得
	posVDest = D3DXVECTOR3(
		posRDest.x + (sinf(m_rot.z) * cosf(m_rot.y)) * m_fLength,
		posRDest.y + cosf(m_rot.z) * m_fLength,
		posRDest.z + (sinf(m_rot.z) * sinf(m_rot.y)) * m_fLength);

	//注視点の補正
	RDiff = D3DXVECTOR3(posRDest.x - m_posR.x, posRDest.y - m_posR.y, posRDest.z - m_posR.z);
	m_posR.x += RDiff.x * 0.2f;
	m_posR.y += RDiff.y * 0.1f;
	m_posR.z += RDiff.z * 0.2f;

	//視点の補正
	VDiff = D3DXVECTOR3(posVDest.x - m_posV.x, posVDest.y - m_posV.y, posVDest.z - m_posV.z);
	m_posV.x += VDiff.x * 0.2f;
	m_posV.y += VDiff.y * 0.1f;
	m_posV.z += VDiff.z * 0.2f;

}

//==========================================================
// 投影場所設定
//==========================================================
void CCamera::Setting(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	m_rot.y = -rot.y + (D3DX_PI * 0.5f);

	//目的の注視点の座標を取得
	m_posR = D3DXVECTOR3(pos.x - sinf(rot.y) * 30.0f, pos.y + 50.0f, pos.z - cosf(rot.y) * 30.0f);

	//目的の視点の座標を獲得
	m_posV = D3DXVECTOR3(
		m_posR.x + (sinf(m_rot.z) * cosf(m_rot.y)) * m_fLength,
		m_posR.y + cosf(m_rot.z) * m_fLength,
		m_posR.z + (sinf(m_rot.z) * sinf(m_rot.y)) * m_fLength);
}

//==========================================================
// エディット用
//==========================================================
void CCamera::Edit(void)
{
	CInputMouse *pMouse = CManager::GetInstance()->GetInputMouse();
	if (pMouse->GetPress(CInputMouse::BUTTON_LBUTTON) == true && pMouse->GetPress(CInputMouse::BUTTON_RBUTTON) == true)
	{//左右同時に押されているとき
		m_posV.x += cosf(m_rot.y + (-D3DX_PI * 0.5f)) * (pMouse->GetCousorMove().x * MOUSE_MOVESPEED) + -cosf(m_rot.y) * (pMouse->GetCousorMove().y * MOUSE_MOVESPEED);
		m_posV.z += sinf(m_rot.y + (-D3DX_PI * 0.5f)) * (pMouse->GetCousorMove().x * MOUSE_MOVESPEED) + -sinf(m_rot.y) * (pMouse->GetCousorMove().y * MOUSE_MOVESPEED);

		//移動量を加算
		m_posV.x += m_posV.x;
		m_posV.z += m_posV.z;

		//移動量を初期化
		m_posV.x = 0.0f;
		m_posV.z = 0.0f;

		//注視点設定
		SetR();
	}
	else if (pMouse->GetPress(CInputMouse::BUTTON_LBUTTON) == true)
	{//左キーが押されているとき
		m_rot.y -= pMouse->GetCousorMove().x * MOUSE_ROTATESPEED_X;
		m_rot.z -= pMouse->GetCousorMove().y * 0.003f;
	}
	else if (pMouse->GetPress(CInputMouse::BUTTON_RBUTTON) == true)
	{//右キーが押されているとき
		m_rot.y -= pMouse->GetCousorMove().x * MOUSE_ROTATESPEED_X;
		m_rot.z -= pMouse->GetCousorMove().y * MOUSE_ROTATESPEED_Z;

		if (m_rot.y > D3DX_PI)
		{//角度がΠを超えた場合
			m_rot.y = D3DX_PI;
			m_rot.y *= -1.0f;
		}
		else if (m_rot.y < -D3DX_PI)
		{//角度がΠを超えた場合
			m_rot.y = -D3DX_PI;
			m_rot.y *= -1.0f;
		}
		if (m_rot.z < MIN_CAMERA_ROTZ)
		{//角度が限界を超えた場合
			m_rot.z = MIN_CAMERA_ROTZ;
		}
		else if (m_rot.z > MAX_CAMERA_ROTZ)
		{//角度が限界を超えた場合
			m_rot.z = MAX_CAMERA_ROTZ;
		}

		//視点設定
		SetR();
	}

	if (m_rot.y > D3DX_PI)
	{//角度がΠを超えた場合
		m_rot.y = D3DX_PI;
		m_rot.y *= -1.0f;
	}
	else if (m_rot.y < -D3DX_PI)
	{//角度がΠを超えた場合
		m_rot.y = -D3DX_PI;
		m_rot.y *= -1.0f;
	}
	if (m_rot.z < MIN_CAMERA_ROTZ)
	{//角度が限界を超えた場合
		m_rot.z = MIN_CAMERA_ROTZ;
	}
	else if (m_rot.z > MAX_CAMERA_ROTZ)
	{//角度が限界を超えた場合
		m_rot.z = MAX_CAMERA_ROTZ;
	}

	//ホイールの使用量で距離の変更
	m_fLength += pMouse->GetCousorMove().z * MOUSE_WHEELSPEED;

	if (m_fLength > CAMERA_MAXLENGTH)
	{//距離が最大を超えた場合
		m_fLength = CAMERA_MAXLENGTH;
	}
	else if (m_fLength < CAMERA_MINLENGTH)
	{//距離が最小を超えた場合
		m_fLength = CAMERA_MINLENGTH;
	}

	//視点設定
	SetV();
}

//==========================================================
// 向き
//==========================================================
void CCamera::SetRot(const D3DXVECTOR3 rot)
{
	float fRotDiff;
	float fRotDest;

	fRotDest = -rot.y + (D3DX_PI * 0.5f);	//目的の向きを取得

	fRotDiff = fRotDest - m_rot.y;

	if (fRotDiff > D3DX_PI || fRotDiff < -D3DX_PI)
	{//-3.14～3.14の範囲外の場合
		if (fRotDiff > D3DX_PI)
		{
			fRotDiff += (-D3DX_PI * 2);
		}
		else if (fRotDiff < -D3DX_PI)
		{
			fRotDiff += (D3DX_PI * 2);
		}
	}

	m_rot.y += fRotDiff * 0.04f;

	if (m_rot.y > D3DX_PI || m_rot.y < -D3DX_PI)
	{//-3.14～3.14の範囲外の場合
		if (m_rot.y > D3DX_PI)
		{
			m_rot.y += (-D3DX_PI * 2);
		}
		else if (m_rot.y  < -D3DX_PI)
		{
			m_rot.y += (D3DX_PI * 2);
		}
	}

	//視点の設定
	SetV();
}

//==========================================================
// スロー時の角度矯正
//==========================================================
void CCamera::Slow(void)
{
	float fLeftRot = m_rot.y - (m_SlowOldRot.y + D3DX_PI * MAX_SLOWROT);
	float fRightRot = m_rot.y - (m_SlowOldRot.y + -D3DX_PI * MAX_SLOWROT);

	if (fLeftRot < -D3DX_PI)
	{//角度がΠを超えた場合
		fLeftRot += D3DX_PI * 2;
	}
	if (fLeftRot > D3DX_PI)
	{//角度がΠを超えた場合
		fLeftRot += -D3DX_PI * 2;
	}

	if (fRightRot < -D3DX_PI)
	{//角度がΠを超えた場合
		fRightRot += D3DX_PI * 2;
	}
	if (fRightRot > D3DX_PI)
	{//角度がΠを超えた場合
		fRightRot += -D3DX_PI * 2;
	}

	if (fLeftRot > 0.0f)
	{
		m_rot.y = (m_SlowOldRot.y + D3DX_PI * MAX_SLOWROT);
	}

	if (fRightRot < 0.0f)
	{
		m_rot.y = (m_SlowOldRot.y + -D3DX_PI * MAX_SLOWROT);
	}
}

//==========================================================
// スローシャワー時のカメラ操作
//==========================================================
void CCamera::SlowShw(void)
{
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();	// キーボードのポインタ
	float fMultiSlow = SLOW_CAMERAROT;

	// 差分を求める
	float fLeftRot = m_rot.y - (m_SlowOldRot.y + D3DX_PI * 0.3f);
	float fRightRot = m_rot.y - (m_SlowOldRot.y + -D3DX_PI * 0.3f);

	if (fLeftRot < -D3DX_PI)
	{//角度がΠを超えた場合
		fLeftRot += D3DX_PI * 2;
	}
	if (fLeftRot > D3DX_PI)
	{//角度がΠを超えた場合
		fLeftRot += -D3DX_PI * 2;
	}

	if (fRightRot < -D3DX_PI)
	{//角度がΠを超えた場合
		fRightRot += D3DX_PI * 2;
	}
	if (fRightRot > D3DX_PI)
	{//角度がΠを超えた場合
		fRightRot += -D3DX_PI * 2;
	}

	//x軸の移動
	if (pInputPad->GetStickPress(0, CInputPad::BUTTON_RIGHT_X, 0.0f, CInputPad::STICK_PLUS) == true)
	{//Qキー入力
		m_rot.y += -D3DX_PI * ROTATE_SPEED * pInputPad->GetStickAdd(0, CInputPad::BUTTON_RIGHT_X, 0.0f) * fMultiSlow;
		if (m_rot.y < -D3DX_PI)
		{//角度がΠを超えた場合
			m_rot.y += D3DX_PI * 2;
		}
	}
	else if (pInputPad->GetStickPress(0, CInputPad::BUTTON_RIGHT_X, 0.0f, CInputPad::STICK_MINUS) == true)
	{//Eキー入力
		m_rot.y += -D3DX_PI * ROTATE_SPEED * pInputPad->GetStickAdd(0, CInputPad::BUTTON_RIGHT_X, 0.0f) * fMultiSlow;

		if (m_rot.y > D3DX_PI)
		{//角度がΠを超えた場合
			m_rot.y += -D3DX_PI * 2;
		}
	}
	else
	{
		m_SlowOldRot = m_rot;
	}

	// 距離の変更
	m_fLength += 30.0f;

	if (m_fLength > CAMERA_MAXLENGTH)
	{//距離が最大を超えた場合
		m_fLength = CAMERA_MAXLENGTH;
	}

	SetV();
}


//==========================================================
// 向きを設定
//==========================================================
void CCamera::SetRotation(D3DXVECTOR3 rot)
{
	m_rot = rot;

	if (m_rot.y > D3DX_PI)
	{//角度がΠを超えた場合
		m_rot.y = D3DX_PI;
		m_rot.y *= -1.0f;
	}
	else if (m_rot.y < -D3DX_PI)
	{//角度がΠを超えた場合
		m_rot.y = -D3DX_PI;
		m_rot.y *= -1.0f;
	}
	//if (m_rot.z < MIN_CAMERA_ROTZ)
	//{//角度が限界を超えた場合
	//	m_rot.z = MIN_CAMERA_ROTZ;
	//}
	//else if (m_rot.z > MAX_CAMERA_ROTZ)
	//{//角度が限界を超えた場合
	//	m_rot.z = MAX_CAMERA_ROTZ;
	//}

	SetV();
}

//==========================================================
// タイトル自動回転
//==========================================================
void CCamera::TitleRotateCamera(void)
{
	m_rot.y += TITLE_ROTATESPD;

	while (1)
	{
		if (m_rot.y >= -D3DX_PI && m_rot.y <= D3DX_PI)
		{
			break;
		}
		else if (m_rot.y > D3DX_PI)
		{
			m_rot.y += -D3DX_PI * 2;
		}
		else if (m_rot.y < -D3DX_PI)
		{
			m_rot.y += D3DX_PI * 2;
		}
	}

	SetV();
}

//==========================================================
// 座標設定
//==========================================================
void CCamera::SetPositionR(D3DXVECTOR3 pos)
{ 
	m_posR = pos; 
	SetV();
}


//==========================================================
// コンストラクタ
//==========================================================
CMultiCamera::CMultiCamera()
{

}

//==========================================================
// デストラクタ
//==========================================================
CMultiCamera::~CMultiCamera()
{

}

//==========================================================
//カメラの初期化処理
//==========================================================
HRESULT CMultiCamera::Init(void)
{
	CCamera::Init();

	//プレイヤー追従カメラの画面位置設定
	m_viewport.X = 0;
	m_viewport.Y = 0;
	m_viewport.Width = (DWORD)(SCREEN_WIDTH * 0.125f);
	m_viewport.Height = (DWORD)(SCREEN_HEIGHT * 0.22f);
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;

	return S_OK;
}

//==========================================================
//カメラの終了処理
//==========================================================
void CMultiCamera::Uninit(void)
{
	CCamera::Uninit();
}

//==========================================================
//カメラの更新処理
//==========================================================
void CMultiCamera::Update(void)
{
	CCamera::Update();
}

//==========================================================
//カメラの設定処理
//==========================================================
void CMultiCamera::SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスへのポインタを取得
	D3DXMATRIX mtxView = GetMtxView(), mtxProjection = GetMtxProjection();
	D3DXVECTOR3 posV = GetPositionV();
	D3DXVECTOR3 posR = GetPositionR();
	D3DXVECTOR3 vecU = GetVectorU();


	//ビューポートの設定
	pDevice->SetViewport(&m_viewport);

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&mtxProjection);

	//プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&mtxProjection,
		D3DXToRadian(45.0f),
		(float)m_viewport.Width / (float)m_viewport.Height,
		10.0f,
		40000.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&mtxView,
		&posV,
		&posR,
		&vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &mtxView);
}

//==========================================================
// コンストラクタ
//==========================================================
CMapCamera::CMapCamera()
{

}

//==========================================================
// デストラクタ
//==========================================================
CMapCamera::~CMapCamera()
{

}

//==========================================================
//カメラの初期化処理
//==========================================================
HRESULT CMapCamera::Init(void)
{
	CMultiCamera::Init();

	return S_OK;
}

//==========================================================
//カメラの終了処理
//==========================================================
void CMapCamera::Uninit(void)
{
	CMultiCamera::Uninit();
}

//==========================================================
//カメラの更新処理
//==========================================================
void CMapCamera::Update(void)
{
	
}

//==========================================================
//カメラの設定処理
//==========================================================
void CMapCamera::SetCamera(void)
{
	CMultiCamera::SetCamera();

	// 床の描画
	CMeshField *pMesh = CMeshField::GetTop();	// 先頭を取得

	while (pMesh != NULL)
	{// 使用されている間繰り返し
		CMeshField *pMeshNext = pMesh->GetNext();	// 次を保持

		pMesh->Draw();

		pMesh = pMeshNext;	// 次に移動
	}

	// 温度表示の描画
	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		CObject *pObject = CObject::GetTop(nCntPri);	// 先頭を取得

		while (pObject != NULL)
		{// 使用されていない状態まで
			CObject *pObjectNext = pObject->GetNext();	// 次のオブジェクトへのポインタを取得

			if (pObject->GetType() == CObject::TYPE_MAP)
			{// マップ表示物のみ
				// 描画処理
				pObject->Draw();
			}
			pObject = pObjectNext;	// 次のオブジェクトに移動
		}
	}
}

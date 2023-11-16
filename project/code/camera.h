//==========================================================
//
// カメラの処理全般 [camera.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _CAMERA_H_		 // このマクロが定義されていない場合
#define _CAMERA_H_		 // 二重インクルード防止用マクロを定義

#include "main.h"		//main.hで定義しているものが必要なためinclude

// マクロ定義
#define MAX_CAMERA_ROTZ (D3DX_PI * 0.999f)	// カメラ最大角度
#define MIN_CAMERA_ROTZ (D3DX_PI * 0.1f)	// カメラ最大角度

//**********************************************************
// カメラクラスの定義
//**********************************************************
class CCamera
{
public:	// 誰でもアクセス可能な定義

	enum MODE
	{
		MODE_NORMAL = 0,	// 通常
		MODE_SLOWGUN,		// スロー角度強制状態
		MODE_SLOWSHW,		// シャワー
		MODE_MAX
	};

public:	// 誰でもアクセス可能
	CCamera();	// コンストラクタ
	virtual ~CCamera();	// デストラクタ

	// メンバ関数
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void SetCamera(void);
	void Pursue(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
	void Setting(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
	void SetRot(const D3DXVECTOR3 rot);
	void SetMode(MODE mode) { m_mode = mode; }
	void SetOldRot(D3DXVECTOR3 rot) { m_SlowOldRot = rot; }
	void MoveV(void);
	void MouseCamera(void);
	void MoveVR(void);
	void SetPositionR(D3DXVECTOR3 pos);

	// タイトル用
	void TitleRotateCamera(void);

	// メンバ関数(取得)
	D3DXMATRIX GetMtxView(void) { return m_mtxView; }
	D3DXMATRIX GetMtxProjection(void) { return m_mtxProjection; }
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	D3DXVECTOR3 GetPositionV(void) { return m_posV; }
	D3DXVECTOR3 GetPositionR(void) { return m_posR; }
	D3DXVECTOR3 GetVectorU(void) { return m_vecU; }
	void SetRotation(D3DXVECTOR3 rot);
	void SetLength(float fLength) { m_fLength = fLength; }

	// メンバ関数(設定)
	D3DXMATRIX SetMtxView(const D3DXMATRIX& mtxView) { m_mtxView = mtxView; }
	D3DXMATRIX SetMtxProjection(const D3DXMATRIX& mtxProjection) { m_mtxProjection = mtxProjection; }

private:	// 自分だけがアクセス可能

	// メンバ関数
	void MoveR(void);
	void SetV(void);
	void SetR(void);
	void Edit(void);
	void Slow(void);
	void SlowShw(void);

	// メンバ変数
	D3DXMATRIX m_mtxView;		// ビューマトリックス
	D3DXMATRIX m_mtxProjection;	// プロジェクションマトリックス
	D3DXVECTOR3 m_posV;		// 視点
	D3DXVECTOR3 m_posR;		// 注視点
	D3DXVECTOR3 m_vecU;		// 上方向ベクトル
	D3DXVECTOR3 m_rot;		// 向き
	D3DXVECTOR3 m_SlowOldRot;	// スロー前の向き
	MODE m_mode;				// モード
	float m_fMulScore;		// スコア倍率
	float m_fLength;			// 視点と注視点の距離
};

//**********************************************************
// 複数カメラクラスの定義
//**********************************************************
class CMultiCamera : public CCamera
{
public:	// 誰でもアクセス可能
	CMultiCamera();	// コンストラクタ
	~CMultiCamera() override;	// デストラクタ

	// メンバ関数
	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void SetCamera(void) override;
	void SetViewPort(const D3DVIEWPORT9& viewport) { m_viewport = viewport; }
	D3DVIEWPORT9 *GetViewPort(void) { return &m_viewport; }

private:	// 自分だけがアクセス可能

	// メンバ変数
	D3DVIEWPORT9 m_viewport;		//ビューポート
};

//**********************************************************
// ミニマップカメラクラスの定義
//**********************************************************
class CMapCamera : public CMultiCamera
{
public:	// 誰でもアクセス可能
	CMapCamera();		// コンストラクタ
	~CMapCamera();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);

private:	// 自分だけがアクセス可能

};

#endif
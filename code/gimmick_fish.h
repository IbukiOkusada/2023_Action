//==========================================================
//
// 魚ギミック(奥から出てくる) [gimmick_fish.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _GIMMICK_Fish_H_		// このマクロが定義されていない場合
#define _GIMMICK_Fish_H_		// 二重インクルード防止用マクロを定義

#include "objectX.h"

// 前方宣言
class CSlow;

//==========================================================
// 移動オブジェクトのクラス定義
//==========================================================
class CGimmickFish : public CObjectX
{
private:	// 自分だけがアクセス可能な定義

	enum STATE
	{
		STATE_NONE = 0,	// 何もしていない
		STATE_UP,		// 上昇
		STATE_ATK,		// 攻撃
		STATE_DOWN,		// 下降
		STATE_WAIT,		// 待機
	};

public:	// 誰でもアクセス可能

	//CGimmickFish();	// コンストラクタ
	CGimmickFish(int nPriOrity = 3);	// コンストラクタ(オーバーロード)
	~CGimmickFish();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CGimmickFish *Create(void);
	static CGimmickFish *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fTimer);

	// メンバ関数(取得)
	D3DXVECTOR3 GetFish(void) { return m_move; }	// 移動量取得
	float GetNowTimer(void) { return m_fNowFishTimer; }	// 現在タイマー取得
	float GetNumTimer(void) { return m_fNumFishTimer; }	// 既定タイマー取得

	// メンバ関数(設定)
	void SetFish(D3DXVECTOR3 move) { m_move = move; }	// 移動量設定
	void SetNowTimer(float fTimer) { m_fNowFishTimer = fTimer; }	// 現在タイマー設定
	void SetNumTimer(float fTimer) { m_fNumFishTimer = fTimer; }	// 既定タイマー設定

private:	// 自分だけがアクセス可能

	// メンバ関数
	void Controller(void);
	bool CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, const float fRefMulti = 0.5f);

	// メンバ変数
	D3DXVECTOR3 m_move;	// 移動量
	D3DXVECTOR3 m_posOld;
	float m_fNowFishTimer;
	float m_fNumFishTimer;
	CSlow *m_pSlow;		// スロー
	STATE m_state;	// 状態
};

#endif

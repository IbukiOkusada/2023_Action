//==========================================================
//
// 魚ギミック(奥から出てくる) [gimmick_fish.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _GIMMICK_Fish_H_		// このマクロが定義されていない場合
#define _GIMMICK_Fish_H_		// 二重インクルード防止用マクロを定義

#include "gimmick.h"

// 前方宣言
class CSlow;
class CCharacter;

//==========================================================
// 移動オブジェクトのクラス定義
//==========================================================
class CGimmickFish : public CGimmick
{
private:	// 自分だけがアクセス可能な定義

	// 状態列挙型
	enum STATE
	{
		STATE_NONE = 0,	// 何もしていない
		STATE_UP,		// 上昇
		STATE_ATK,		// 攻撃
		STATE_ATKEND,		// 攻撃終了
		STATE_DOWN,		// 下降
		STATE_WAIT,		// 待機
	};

	// モーション列挙型
	enum MOTION
	{
		MOTION_NEUTRAL = 0,	// 待機モーション
		MOTION_JUMP,			// ジャンプモーション
		MOTION_BITE,			// 噛みつきモーション
		MOTION_AFTERGRAW,		// 噛みつき後余韻モーション
		MOTION_MAX
	};

public:	// 誰でもアクセス可能

	CGimmickFish();	// コンストラクタ
	~CGimmickFish();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CGimmickFish *Create(void);
	static CGimmickFish *Create(D3DXVECTOR3& pos, D3DXVECTOR3& move, float fTimer);

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
	bool CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, const D3DXVECTOR3& vtxMin, const D3DXVECTOR3& vtxMax, int &nDamage);

	// メンバ変数
	D3DXVECTOR3 m_move;	// 移動量
	D3DXVECTOR3 m_posOld;
	float m_fNowFishTimer;
	float m_fNumFishTimer;
	bool m_bEat;			// 食べたかどうか
	float m_fStateCounter;	// 状態管理カウンター
	CSlow *m_pSlow;		// スロー
	CCharacter *m_pCharacter;	// 階層構造クラスのポインタ
	STATE m_state;	// 状態
};

#endif

//==========================================================
//
// 移動ギミック [gimmick_move.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _GIMMICK_MOVE_H_		// このマクロが定義されていない場合
#define _GIMMICK_MOVE_H_		// 二重インクルード防止用マクロを定義

#include "gimmick.h"

// 前方宣言
class CSlow;
class CCharacter;
class CShadow;

//==========================================================
// 移動オブジェクトのクラス定義
//==========================================================
class CGimmickMove : public CGimmick
{
public:	// 誰でもアクセス可能

	CGimmickMove();	// コンストラクタ
	~CGimmickMove();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CGimmickMove *Create(void);
	static CGimmickMove *Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, float fTimer);

	// メンバ関数(取得)
	D3DXVECTOR3 GetMove(void) { return m_move; }	// 移動量取得
	float GetNowTimer(void) { return m_fNowMoveTimer; }	// 現在タイマー取得
	float GetNumTimer(void) { return m_fNumMoveTimer; }	// 既定タイマー取得

	// メンバ関数(設定)
	void SetMove(const D3DXVECTOR3& move) { m_move = move; }	// 移動量設定
	void SetNowTimer(float fTimer) { m_fNowMoveTimer = fTimer; }	// 現在タイマー設定
	void SetNumTimer(float fTimer) { m_fNumMoveTimer = fTimer; }	// 既定タイマー設定

private:	// 自分だけがアクセス可能

	// メンバ関数
	void Controller(void);
	bool CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, const D3DXVECTOR3& vtxMin, const D3DXVECTOR3& vtxMax, int &nDamage);

	// メンバ変数
	D3DXVECTOR3 m_move;	// 移動量
	D3DXVECTOR3 m_posOld;
	CCharacter *m_pObject;
	CShadow *m_pShadow;
	float m_fNowMoveTimer;
	float m_fNumMoveTimer;
	CSlow *m_pSlow;		// スロー

};

#endif


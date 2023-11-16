//==========================================================
//
// ボタンギミックの処理 [gimmick_button.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _GIMMICK_BUTTON_H_		// このマクロが定義されていない場合
#define _GIMMICK_BUTTON_H_		// 二重インクルード防止用マクロを定義

#include "gimmick.h"

// 前方宣言
class CObjectX;

//==========================================================
// ボタンのクラス定義(派生クラス)
//==========================================================
class CGimmickButton : public CGimmick
{
public:	// 誰でもアクセス可能な定義

	enum TYPE
	{
		TYPE_REVERSE = 0,	// 風反転
		TYPE_DOOR,		// ドア開放
		TYPE_MAX
	};

public:	// 誰でもアクセス可能

	CGimmickButton();	// コンストラクタ
	~CGimmickButton();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CGimmickButton *Create(void);

	// メンバ関数(取得)

	// メンバ関数(設定)
	void SetGoalPos(const D3DXVECTOR3& pos) { m_GoalPos = pos; }
	void SetType(TYPE type) { m_type = type; }
	void SetId(int nId) { m_nId = nId; }

private:	// 自分だけがアクセス可能

	// メンバ関数
	void Controller(void);
	bool CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, const D3DXVECTOR3& vtxMin, const D3DXVECTOR3& vtxMax, int &nDamage);

	// メンバ変数
	D3DXVECTOR3 m_GoalPos;		// 入力後座標
	CObjectX *m_pObject;		// オブジェクトポインタ
	bool m_bPress;	// 入力済み判定
	int m_nId;	// ID
	bool m_bPressOld;
	TYPE m_type;
};

#endif


//==========================================================
//
// 風ギミック [gimmick_button.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _GIMMICK_AIR_H_		// このマクロが定義されていない場合
#define _GIMMICK_AIR_H_		// 二重インクルード防止用マクロを定義

#include "gimmick.h"

// 前方宣言
class CObject3D;

//==========================================================
// 風のクラス定義(派生クラス)
//==========================================================
class CGimmickAir : public CGimmick
{
public:	// 誰でもアクセス可能な定義

	enum TYPE
	{
		TYPE_REVERSE = 0,	// 風反転
		TYPE_DOOR,		// ドア開放
		TYPE_MAX
	};

public:	// 誰でもアクセス可能

	CGimmickAir();	// コンストラクタ
	~CGimmickAir();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CGimmickAir *Create(void);
	void Reverse(void);

	// メンバ関数(取得)

	// メンバ関数(設定)
	void SetMove(D3DXVECTOR3 move);
	void SetWidth(float fWidth) { m_fWidth = fWidth; }
	void SetHeight(float fHeight) { m_fHeight = fHeight; }
	void SetRevease(bool bRevease) { m_bRevease = bRevease; }

private:	// 自分だけがアクセス可能

	// メンバ関数
	void Controller(void);
	bool CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, const D3DXVECTOR3& vtxMin, const D3DXVECTOR3& vtxMax, int &nDamage);

	// メンバ変数
	CObject3D *m_pObject;		// オブジェクトポインタ
	D3DXVECTOR3 m_move;		// 移動量
	D3DXVECTOR2 m_moveTex;		// テクスチャ移動量
	D3DXVECTOR2 m_TexPos;		// テクスチャ座標
	bool m_bRevease;			// 反転するか
	float m_fWidth;			// 幅
	float m_fHeight;			// 高さ
};

#endif
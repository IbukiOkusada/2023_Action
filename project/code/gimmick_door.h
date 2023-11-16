//==========================================================
//
// 移動ギミック [gimmick_move.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _GIMMICK_DOOR_H_		// このマクロが定義されていない場合
#define _GIMMICK_DOOR_H_		// 二重インクルード防止用マクロを定義

#include "gimmick.h"

// 前方宣言
class CSlow;
class CModel;

//==========================================================
// ドアのクラス定義
//==========================================================
class CGimmickDoor : public CGimmick
{
private:	// 自分だけがアクセス可能な定義

	// 種類列挙型
	enum TYPE
	{
		TYPE_UP = 0,	// 上側
		TYPE_DOWN,	// 下側
		TYPE_MAX
	};

	// オブジェクト構造体
	struct SObject
	{
		CModel *pModel;	// モデルのポインタ
		D3DXVECTOR3 GoalPos;	// 座標
	};

public:	// 誰でもアクセス可能

	CGimmickDoor();	// コンストラクタ
	~CGimmickDoor();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CGimmickDoor *Create(void);
	static CGimmickDoor *Create(const D3DXVECTOR3& pos);
	void Open(int nId);

	// メンバ関数(取得)
	void SetId(int nId) { m_nId = nId; }

private:	// 自分だけがアクセス可能

			// メンバ関数
	void Controller(void);
	bool CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, const D3DXVECTOR3& vtxMin, const D3DXVECTOR3& vtxMax, int &nDamage);

	// メンバ変数
	SObject m_aObject[TYPE_MAX];
	CSlow *m_pSlow;		// スロー
	bool m_bOpen;			// 扉
	int m_nId;			// ID
};

#endif


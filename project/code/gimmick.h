//==========================================================
//
// ギミック基底 [gimmick.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _GIMMICK_H_		// このマクロが定義されていない場合
#define _GIMMICK_H_		// 二重インクルード防止用マクロを定義

#include "task.h"

//==========================================================
// サンプルのクラス定義
//==========================================================
class CGimmick : public CTask
{
protected:

	// 種類列挙型
	enum TYPE
	{
		TYPE_NONE = 0,
		TYPE_MOVE,
		TYPE_ROTATE,
		TYPE_FISH,
		TYPE_MAX
	};

public:	// 誰でもアクセス可能

	CGimmick();	// コンストラクタ
	virtual ~CGimmick();	// デストラクタ

	// 純粋仮想関数
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Reverse(void) { }
	virtual void Open(int) { }

	// メンバ関数
	void SetPosition(const D3DXVECTOR3& pos) { m_pos = pos; }
	void SetRotation(const D3DXVECTOR3& rot) { m_rot = rot; }
	D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_rot; }
	void SetMtxWorld(void);
	D3DXMATRIX *GetMtxWorld(void) { return &m_mtxWorld; }
	static bool Collision(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, const D3DXVECTOR3& vtxMin, const D3DXVECTOR3& vtxMax, int &nDamage);
	void AirReverse(void);
	void DoorOpen(int nId);

protected:

	// 種類設定
	void BindType(TYPE type) { m_type = type; }
	void ListOut(void);

private:	// 自分だけがアクセス可能

	virtual bool CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, const D3DXVECTOR3& vtxMin, const D3DXVECTOR3& vtxMax, int &nDamage) = 0;

	// メンバ変数
	static CGimmick *m_pTop;	// 先頭のオブジェクトへのポインタ
	static CGimmick *m_pCur;	// 最後尾のオブジェクトへのポインタ
	CGimmick *m_pPrev;		// 前のオブジェクトへのポインタ
	CGimmick *m_pNext;		// 次のオブジェクトへのポインタ
	TYPE m_type;	// 種類
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_rot;	// 向き
	D3DXMATRIX m_mtxWorld;	//ワールドマトリックス
};

#endif



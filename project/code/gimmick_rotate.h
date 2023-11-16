//==========================================================
//
// 回転ギミック [gimmick_rotate.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _GIMMICK_ROTATE_H_		// このマクロが定義されていない場合
#define _GIMMICK_ROTATE_H_		// 二重インクルード防止用マクロを定義

#include "gimmick.h"

// 前方宣言
class CCharacter;
class CShadow;

// マクロ定義
#define NUM_ROTATEBOX	(10)

//==========================================================
// サンプルのクラス定義
//==========================================================
class CGimmickRotate : public CGimmick
{
private:

	// 追従回転オブジェ
	struct Obj
	{
		D3DXVECTOR3 s_posOld;
		CCharacter *s_pModel;
		CShadow *s_pShadow;
	};

public:	// 誰でもアクセス可能

	CGimmickRotate();	// コンストラクタ
	//CGimmickRotate(int nPriOrity = 0);	// コンストラクタ(オーバーロード)
	~CGimmickRotate();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CGimmickRotate *Create(void);

	// メンバ関数(取得)
	D3DXVECTOR3 GetRotate(void) { return m_RotateSpeed; }

	// メンバ関数(設定)
	void SetRotate(D3DXVECTOR3 rotate);

private:	// 自分だけがアクセス可能

	// メンバ関数
	virtual bool CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, const D3DXVECTOR3& vtxMin, const D3DXVECTOR3& vtxMax, int &nDamage);
	void SetRotationCharacter(void);
	// メンバ変数
	Obj m_aObj[NUM_ROTATEBOX];	// オブジェ配列
	D3DXVECTOR3 m_RotateSpeed;	// 回転量
	float m_fSize;		// 1つ辺りのオブジェサイズ
};

#endif

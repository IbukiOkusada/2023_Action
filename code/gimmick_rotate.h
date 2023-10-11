//==========================================================
//
// 回転ギミック [gimmick_rotate.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _GIMMICK_ROTATE_H_		// このマクロが定義されていない場合
#define _GIMMICK_ROTATE_H_		// 二重インクルード防止用マクロを定義

#include "objectX.h"

// 前方宣言
class CModel;

// マクロ定義
#define NUM_ROTATEBOX	(8)

//==========================================================
// サンプルのクラス定義
//==========================================================
class CGimmickRotate : public CObjectX
{
private:

	// 追従回転オブジェ
	struct Obj
	{
		D3DXVECTOR3 s_posOld;
		CModel *s_pModel;
	};

public:	// 誰でもアクセス可能

	//CGimmickRotate();	// コンストラクタ
	CGimmickRotate(int nPriOrity = 3);	// コンストラクタ(オーバーロード)
	~CGimmickRotate();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CGimmickRotate *Create(void);

	// メンバ関数(取得)

	// メンバ関数(設定)

private:	// 自分だけがアクセス可能

	// メンバ関数
	virtual bool CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, const float fRefMulti = 0.5f);

	// メンバ変数
	Obj m_aObj[NUM_ROTATEBOX];	// オブジェ配列
	float m_fSize;		// 1つ辺りのオブジェサイズ
};

#endif

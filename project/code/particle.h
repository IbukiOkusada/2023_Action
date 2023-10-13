//===================================================
//
//パーティクル処理
//Author 奥定伊吹
//
//===================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"
#include "effect.h"

#define MAX_PARTICLE	(256)	// パーティクルの最大数

// クラスの定義
class CParticle
{
public:	// 誰でもアクセス可能

	CParticle();	// コンストラクタ
	~CParticle();	// デストラクタ

	// メンバ関数
	static void Create(D3DXVECTOR3 pos, CEffect::TYPE type);
	static void Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, CEffect::TYPE type);

private:	// 自分だけがアクセス可能

	// メンバ関数
	static void Set(D3DXVECTOR3 Defpos, D3DXVECTOR3 Defmove, CEffect::TYPE type);
};

#endif

//==========================================================
//
// 影の処理全般 [shadow.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _SHADOW_H_	 // このマクロが定義されていない場合
#define _SHADOW_H_	 // 二重インクルード防止用マクロを定義

#include "main.h"		//main.hで定義しているものが必要なためinclude
#include "object3D.h"

//**********************************************************
// 影クラスの定義
//**********************************************************
class CShadow : public CObject3D
{
public:		// 誰でもアクセス可能

	CShadow();	// コンストラクタ
	~CShadow();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CShadow *Create(D3DXVECTOR3 pos, float fWidth, float fHeight);

private:	// 自分だけがアクセス可能
};

#endif

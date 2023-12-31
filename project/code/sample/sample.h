//==========================================================
//
// サンプル [sample.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _SAMPLE_H_		// このマクロが定義されていない場合
#define _SAMPLE_H_		// 二重インクルード防止用マクロを定義

#include "../object.h"

//==========================================================
// サンプルのクラス定義
//==========================================================
class CSample : public CObject
{

public:	// 誰でもアクセス可能

	//CSample();	// コンストラクタ
	CSample(int nPriOrity = 3);	// コンストラクタ(オーバーロード)
	~CSample();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CSample *Create(void);

	// メンバ関数(取得)
	

	// メンバ関数(設定)

private:	// 自分だけがアクセス可能

	// メンバ関数

	// メンバ変数
};

#endif


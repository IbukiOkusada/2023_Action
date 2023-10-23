//==========================================================
//
// カウントダウン処理 [countdown.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _COUNTDOWN_H_		// このマクロが定義されていない場合
#define _COUNTDOWN_H_		// 二重インクルード防止用マクロを定義

// マクロ定義
#define COUNT_DOWN_NUM	 (3)	// カウントダウンの数値

// 前方宣言
class CNumber;

//==========================================================
// サンプルのクラス定義
//==========================================================
class CCountDown
{

public:	// 誰でもアクセス可能

	CCountDown();	// コンストラクタ
	~CCountDown();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CCountDown *Create(void);

	// メンバ関数(取得)
	bool GetEnd(void) { return m_bEnd; }

	// メンバ関数(設定)

private:	// 自分だけがアクセス可能

	// メンバ変数
	CNumber *m_pObject;
	D3DXVECTOR3 m_pos;	// 座標
	float m_fDownCounter;	// 遷移カウント
	int m_nNum;	// 現在の値
	bool m_bEnd;	// カウントダウン終了
};

#endif


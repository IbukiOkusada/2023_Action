//==========================================================
//
// ポーズの処理 [pause.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "object2D.h"

//**********************************************************
// ポーズクラスの定義
//**********************************************************
class CPause
{
private:	// 自分だけがアクセス可能な定義

	// 状態列挙型
	enum MENU
	{
		MENU_CONTINUE = 0,	// ゲームに戻る
		MENU_RETRY,			// ゲームをやり直す
		MENU_QUIT,			// タイトル画面に戻る
		MENU_MAX
	};

	// 種類列挙型
	enum TYPE
	{
		TYPE_CONTINUE = 0,	// ゲームに戻る
		TYPE_RETRY,			// ゲームをやり直す
		TYPE_QUIT,			// タイトル画面に戻る
		TYPE_BG,			// 背景
		TYPE_MAX
	};

	// ポリゴン構造体
	struct SPolygon
	{
		CObject2D *pObject2D;	// 2Dオブジェクトへのポインタ
		D3DXCOLOR col;			// 色
	};

public:		// 誰でもアクセス可能

	CPause();	// コンストラクタ
	~CPause();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPause *Create(void);
	bool GetEnable(void) { return m_bState; }
	bool GetSelect(void) { return m_bSelect; }

private:	// 自分だけがアクセス可能

	// メンバ関数
	void ColorSet(void);

	// メンバ変数
	static const char* m_apTexPass[TYPE_MAX];	// ファイルパス名
	MENU m_SelectMenu;				// 選択項目
	CObject2D *m_pBg;				// 背景用
	SPolygon m_aMenu[MENU_MAX];		// メニュー用
	bool m_bState;					// 状態かどうか
	bool m_bSelect;					// 選択したかどうか
	float m_fMoveCol_a;				// 透明度の変化量
};

#endif

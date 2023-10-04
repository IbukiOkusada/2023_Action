//===============================================
//
// マネージャーの処理 [manager.h]
// Author : Ibuki Okusada
//
//===============================================
#ifndef _MANAGER_H_		// このマクロが定義されていない場合
#define _MANAGER_H_		// 二重インクルード防止用マクロを定義

#include "main.h"

// 前方宣言
class CRenderer;
class CInputKeyboard;
class CInputMouse;
class CInputPad;
class CDebugProc;
class CSound;
class CCamera;
class CLight;
class CTexture;
class CXFile;
class CFileLoad;
class CSlow;
class CFade;
class CMultiCamera;
class CMapCamera;
class CTime;
class CPlayer;
class CFileLoad;
class CEditor;
class CEnemyRoute;
class CCarManager;
class CEnemyManager;

//===============================================
// シーンクラスの定義
//===============================================
class CScene
{
public:

	// モード列挙型
	enum MODE
	{
		MODE_TITLE = 0,	// タイトル画面
		MODE_TUTORIAL,	// チュートリアル画面
		MODE_GAME,		// ゲーム画面
		MODE_RESULT,	// リザルト画面
		MODE_RANKING,	// ランキング画面
		MODE_MAX
	};

	CScene();
	~CScene();

	// メンバ関数
	static CScene *Create(MODE mode);
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual CMapCamera *GetMapCamera(void) { return NULL; }
	virtual CTime *GetTime(void) { return NULL; }
	virtual CPlayer *GetPlayer(void) { return NULL; }
	virtual CFileLoad *GetFileLoad(void) { return NULL; }
	virtual CEditor *GetEditor(void) { return NULL; }
	virtual CEnemyRoute *GetEnemyRoute(void) { return NULL; }
	virtual CCarManager *GetCarManager(void) { return NULL; }
	virtual CEnemyManager *GetEnemyManager(void) { return NULL; }
	void SetMode(MODE mode) { m_mode = mode; }
	MODE GetMode(void) { return m_mode; }

private:
	MODE m_mode;	// モード
};

//===============================================
// マネージャークラスの定義
//===============================================
class CManager
{
public:	// 誰でもアクセス可能

	CManager();	// コンストラクタ
	~CManager();	// デストラクタ

	// メンバ関数
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CRenderer *GetRenderer(void);
	static CInputKeyboard *GetInputKeyboard(void);
	static CInputMouse *GetInputMouse(void);
	static CInputPad *GetInputPad(void);
	static CDebugProc *GetDebugProc(void);
	static CSound *GetSound(void);
	static CCamera *GetCamera(void);
	static CLight *GetLight(void);
	static CTexture *GetTexture(void);
	static CXFile *GetModelFile(void);
	static CSlow *GetSlow(void);
	static CFade *GetFade(void);
	static void SetMode(CScene::MODE mode);
	static CScene::MODE GetMode(void) { return m_pScene->GetMode(); }
	static CScene *GetScene(void) { return m_pScene; }

private:	// 自分だけがアクセス可能

	// メンバ関数
	static void DataReset(void);

	// 静的メンバ変数
	static CRenderer *m_pRenderer;	// レンダラーのポインタ
	static CInputKeyboard *m_pInputKeyboard;	// キーボードのポインタ
	static CInputMouse *m_pInputMouse;	// マウスのポインタ
	static CInputPad *m_pInputPad;	// パッドのポインタ
	static CDebugProc *m_pDebugProc;	// デバッグ表示のポインタ
	static CMultiCamera *m_pCamera;		// カメラのポインタ
	static CLight *m_pLight;		// ライトのポインタ
	static CTexture *m_pTexture;	// テクスチャのポインタ
	static CXFile *m_pModelFile;	// Xファイルのポインタ
	static CSound *m_pSound;		// サウンドのポインタ
	static CSlow *m_pSlow;			// スローのポインタ
	static CScene *m_pScene;		// シーンのポインタ
	static CFade *m_pFade;			// フェードのポインタ
};


#endif


//===============================================
//
// ゲーム画面の管理処理 [game.h]
// Author : Ibuki Okusada
//
//===============================================
#ifndef _GAME_H_		// このマクロが定義されていない場合
#define _GAME_H_		// 二重インクルード防止用マクロを定義

#include "manager.h"

// 前方宣言
class CScore;
class CTime;
class CPlayer;
class CMapCamera;
class CLight;
class CFileLoad;
class CEditor;
class CSlow;
class CObject3D;
class CMeshField;
class CPause;
class CThermo;
class CEnemyManager;
class CCarManager;
class CEnemyRoute;
class CMeshDome;
class CObject2D;
class CHeatFilter;

// マクロ定義
#define NUM_FILTER	(2)

//===============================================
// ゲームクラスの定義(派生クラス)
//===============================================
class CGame : public CScene
{
public:

	// メンバ関数
	CGame();	// コンストラクタ
	~CGame();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// メンバ関数(ポインタ)
	static CScore *GetScore(void);
	CTime *GetTime(void);
	CPlayer *GetPlayer(void);
	static CMeshField *GetMeshField(void);
	static void SetMesh(CMeshField *pMesh) { m_pMeshField = pMesh; }
	CFileLoad *GetFileLoad(void);
	static CPause *GetPause(void);
	CEditor *GetEditor(void);
	CMapCamera *GetMapCamera(void) { return m_pMapCamera; }
	CEnemyRoute *GetEnemyRoute(void) { return m_pEnemyRoute; }
	CCarManager *GetCarManager(void) { return m_pCarManager; }
	CEnemyManager *GetEnemyManager(void) { return m_pEnemyManager; }

private:

	CFileLoad *m_pFileLoad;			// ファイル読み込みのポインタ
	static CScore *m_pScore;		// スコアのポインタ
	CTime *m_pTime;					// タイムのポインタ
	CPlayer *m_pPlayer;				// プレイヤーのポインタ
	static CMeshField *m_pMeshField;	// メッシュフィールドのポインタ
	static CPause *m_pPause;		// ポーズのポインタ
	CEditor *m_pEditor;				// エディターのポインタ
	CMapCamera *m_pMapCamera;		// ミニマップ用カメラ
	CThermo *m_pMapThermo;			// マップ全体の温度表示
	CEnemyManager *m_pEnemyManager;	// エネミーマネージャーのポインタ
	CCarManager *m_pCarManager;		// カーマネージャーのポインタ
	CEnemyRoute *m_pEnemyRoute;		// 敵の順路管理のポインタ
	CMeshDome *m_pMeshDome;			// メッシュドームのポインタ
	int m_nMaxEnemy;				// 現在のエネミー最大数
	CObject2D *m_pStart;			// スタート時の文字
	CHeatFilter *m_apFilter[NUM_FILTER];
};

#endif
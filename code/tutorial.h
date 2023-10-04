//===============================================
//
// ゲーム画面の管理処理 [game.h]
// Author : Ibuki Okusada
//
//===============================================
#ifndef _TUTORIAL_H_		// このマクロが定義されていない場合
#define _TUTORIAL_H_		// 二重インクルード防止用マクロを定義

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

//===============================================
// ゲームクラスの定義(派生クラス)
//===============================================
class CTutorial : public CScene
{
public:

	// メンバ関数
	CTutorial();	// コンストラクタ
	~CTutorial();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// メンバ関数(ポインタ)
	static CScore *GetScore(void);
	CTime *GetTime(void);
	CPlayer *GetPlayer(void);
	static CSlow *GetSlow(void);
	static CMeshField *GetMeshField(void);
	static void SetMesh(CMeshField *pMesh) { m_pMeshField = pMesh; }
	CFileLoad *GetFileLoad(void);
	CEnemyRoute *GetEnemyRoute(void) { return m_pEnemyRoute; }
	CCarManager *GetCarManager(void) { return m_pCarManager; }

private:

	void DataReset(void);
	void EnemySet(void);
	void CarSet(void);
	void SkySet(void);
	void StartEnemy(void);

	CFileLoad *m_pFileLoad;			// ファイル読み込みのポインタ
	static CScore *m_pScore;		// スコアのポインタ
	CTime *m_pTime;					// タイムのポインタ
	CPlayer *m_pPlayer;				// プレイヤーのポインタ
	static CSlow *m_pSlow;			// スローのポインタ
	static CMeshField *m_pMeshField;	// メッシュフィールドのポインタ
	CEnemyManager *m_pEnemyManager;	// エネミーマネージャーのポインタ
	CCarManager *m_pCarManager;		// カーマネージャーのポインタ
	CEnemyRoute *m_pEnemyRoute;		// 敵の順路管理のポインタ
	CMeshDome *m_pMeshDome;			// メッシュドームのポインタ
};

#endif
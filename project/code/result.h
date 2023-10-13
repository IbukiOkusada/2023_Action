//===============================================
//
// リザルト画面の管理処理 [result.h]
// Author : Ibuki Okusada
//
//===============================================
#ifndef _RESULT_H_		// このマクロが定義されていない場合
#define _RESULT_H_		// 二重インクルード防止用マクロを定義

#include "manager.h"

class CScore;
class CFileLoad;
class CMeshDome;

//===============================================
// タイトルクラスの定義(派生クラス)
//===============================================
class CResult : public CScene
{
private:

	// スコア列挙型
	enum SCORE
	{
		SCORE_SUV = 0,
		SCORE_DEAD,
		SCORE_ALL,
		SCORE_MAX
	};

public:

	// メンバ関数
	CResult();	// コンストラクタ
	~CResult();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// メンバ関数(取得)
	static void SetSuv(int nValue) { m_nSuvNum = nValue; }
	static void SetDead(int nValue) { m_nDeadNum = nValue; }

private:

	CFileLoad *m_pFileLoad;		// ファイル読み込みのポインタ
	CMeshDome *m_pMeshSky;		// 空用
	CScore *m_apScore[SCORE_MAX];
	CScore *m_apNum[SCORE_ALL];
	int m_nTimer;
	static int m_nSuvNum;
	static int m_nDeadNum;
};

#endif
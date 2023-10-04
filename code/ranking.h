//===============================================
//
// ランキング画面の管理処理 [ranking.h]
// Author : Ibuki Okusada
//
//===============================================
#ifndef _RANKING_H_		// このマクロが定義されていない場合
#define _RANKING_H_		// 二重インクルード防止用マクロを定義

#include "manager.h"

//===============================================
// マクロ定義
//===============================================
#define NUM_RANK	(5)		// ランキングの順位数

// 前方宣言
class CScore;
class CFileLoad;
class CCarManager;

//===============================================
// ランキングクラスの定義(派生クラス)
//===============================================
class CRanking : public CScene
{
public:

	// メンバ関数
	CRanking();		// コンストラクタ
	~CRanking();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetScore(int nValue) { m_nScore = nValue; }

private:

	// メンバ関数
	void Save(int *pScore);
	void Load(int *pScore);
	void Sort(int *pScore);
	void RankIn(int *pScore, int nResult);

	// メンバ変数
	CFileLoad *m_pFileLoad;	// ファイル読み込みのポインタ
	static CScore *m_apScore[NUM_RANK];	// スコアのポインタ(順位分)
	static int m_nScore;	// 今回のスコア
	int m_nTimer;			// 自動遷移タイマー
	int m_nRank;
	CCarManager *m_pCarManager;		// カーマネージャーのポインタ
};

#endif
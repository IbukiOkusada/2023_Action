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
class CTime;
class CCharacter;

#define NUM_RANK	(5)		// ランキングの順位数

//===============================================
// リザルトクラスの定義(派生クラス)
//===============================================
class CResult : public CScene
{
public:
	
	// 種類列挙型
	enum TYPE
	{
		TYPE_MULTI_WIN,	// マルチ勝利
		TYPE_MULTI_LOSE,	// マルチ敗北
		TYPE_MAX		// 通常
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
	static void SetScore(int nNum) { m_nScore = nNum; }
	static void SetType(TYPE type) { m_type = type; }
	static TYPE GetType(void) { return m_type; }

private:

	// メンバ関数
	void Save(int *pScore);
	void Load(int *pScore);
	void Sort(int *pScore);
	void RankIn(int *pScore, int nResult);

	// メンバ変数
	CFileLoad *m_pFileLoad;	// ファイル読み込みのポインタ
	CMeshDome *m_pMeshSky;		// 空用
	CTime *m_pTime;			// タイマー
	CTime *m_apScore[NUM_RANK];	// スコアのポインタ(順位分)
	CCharacter *m_apCharacter[TYPE_MAX];
	int m_nRank;				// 今回のランク
	int m_nTimer;				// 遷移タイマー
	static int m_nScore;		// 今回のスコア
	static TYPE m_type;		// 種類
};

#endif
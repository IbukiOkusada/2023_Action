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

//===============================================
// リザルトクラスの定義(派生クラス)
//===============================================
class CResult : public CScene
{
public:
	
	// 種類列挙型
	enum TYPE
	{
		TYPE_NONE,		// 通常
		TYPE_MULTI_WIN,	// マルチ勝利
		TYPE_MULTI_LOSE,	// マルチ敗北
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

	CFileLoad *m_pFileLoad;	// ファイル読み込みのポインタ
	CMeshDome *m_pMeshSky;		// 空用
	CTime *m_pTime;
	CScore *m_pScore;			// スコア
	int m_nTimer;				// 遷移タイマー
	static int m_nScore;		// 今回のスコア
	static TYPE m_type;		// 種類
};

#endif
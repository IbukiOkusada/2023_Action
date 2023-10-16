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
public:

	// メンバ関数
	CResult();	// コンストラクタ
	~CResult();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:

	CFileLoad *m_pFileLoad;	// ファイル読み込みのポインタ
	CMeshDome *m_pMeshSky;		// 空用
	CScore *m_pScore;			// スコア
	int m_nTimer;				// 遷移タイマー
	static int m_nScore;		// 今回のスコア
};

#endif
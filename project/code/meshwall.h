//==========================================================
//
// メッシュウォールの処理 [meshwall.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _MESHWALL_H_	// このマクロが定義されていない場合
#define _MESHWALL_H_	// 二重インクルード防止用マクロを定義

#include "main.h"
#include "mesh.h"

//**********************************************************
// メッシュウォールクラスの定義(派生クラス)
//**********************************************************
class CMeshWall : public CObjectMesh
{
public:		// 誰でもアクセス可能

	CMeshWall();	// コンストラクタ
	~CMeshWall();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetVtxInfo(void);
	static CMeshWall *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,
		float fWidth, float fHeight, const char *pFileName, const int nPriority = 3, const int nWidth = 1, const int nHeight = 1);

	// メンバ関数(取得)
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }

	// メンバ関数(設定)
	void SetSize(float fWidth, float fHeight);
	static void Collision(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld);

private:	// 自分だけがアクセス可能

	// メンバ関数
	static CMeshWall *m_pTop;	// 先頭のオブジェクトへのポインタ
	static CMeshWall *m_pCur;	// 最後尾のオブジェクトへのポインタ
	CMeshWall *m_pPrev;	// 前のオブジェクトへのポインタ
	CMeshWall *m_pNext;	// 次のオブジェクトへのポインタ
	float m_fWidth;		// 幅
	float m_fHeight;	// 高さ
};

#endif

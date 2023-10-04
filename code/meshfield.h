//==========================================================
//
// メッシュフィールドの処理 [meshfield.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _MESHFIELD_H_	// このマクロが定義されていない場合
#define _MESHFIELD_H_	// 二重インクルード防止用マクロを定義

#include "main.h"
#include "mesh.h"

//**********************************************************
// メッシュフィールドクラスの定義(派生クラス)
//**********************************************************
class CMeshField : public CObjectMesh
{
public:		// 誰でもアクセス可能

	CMeshField();	// コンストラクタ
	~CMeshField();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetVtxInfo(void);
	static CMeshField *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, float fWidth, float fHeight,
		const char *pFileName, const int nWidth = 1, const int nHeight = 1, const int nPriority = 3);
	void Edit(float *pLength, float *pSpeed);
	void UpDownLoad(const char *pFileName);
	static bool GetAreaHot(D3DXVECTOR3 pos);
	static CMeshField *GetArea(D3DXVECTOR3 pos);

	// メンバ関数(取得)
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }
	static CMeshField *GetTop(void) { return m_pTop; }
	CMeshField *GetNext(void) { return m_pNext; }
	bool GetHot(void) { return m_bHot; }

	// メンバ関数(設定)
	void SetSize(float fWidth, float fHeight);
	float GetHeight(D3DXVECTOR3 pos, D3DXVECTOR3 &normal);
	static float GetHeight(D3DXVECTOR3 pos);
	void SetHot(bool bHot) { m_bHot = bHot; }

private:	// 自分だけがアクセス可能

	// メンバ関数
	static CMeshField *m_pTop;	// 先頭のオブジェクトへのポインタ
	static CMeshField *m_pCur;	// 最後尾のオブジェクトへのポインタ
	CMeshField *m_pPrev;	// 前のオブジェクトへのポインタ
	CMeshField *m_pNext;	// 次のオブジェクトへのポインタ
	float m_fWidth;			// 幅
	float m_fHeight;		// 高さ
	bool m_bHot;			// 暑い状態か否か
};

#endif

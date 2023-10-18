//==========================================================
//
// 階層構造のモデル処理 [model.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _MODEL_H_	// このマクロが定義されていない場合
#define _MODEL_H_	// 二重インクルード防止用マクロを定義

#include "main.h"
#include "object.h"

//**********************************************************
// 階層構造モデルクラス
//**********************************************************
class CModel : public CObject
{
public:	// 誰でもアクセス可能

	CModel();	// コンストラクタ
	~CModel();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	HRESULT Init(const char *pFileName);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CModel *Create(const char *pFileName);
	void SetParent(D3DXMATRIX *pMtx);
	void BindModelFile(int nIdx);

	// メンバ関数(取得)
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	D3DXVECTOR3 GetCurrentPosition(void) { return m_CurPos; }
	D3DXVECTOR3 GetCurrentRotation(void) { return m_CurRot; }
	D3DXMATRIX *GetMtxWorld(void) { return &m_mtxWorld; }

	// メンバ関数(設定)
	void SetPosition(const D3DXVECTOR3 pos);
	void SetRotation(const D3DXVECTOR3 rot);
	void SetCurrentPosition(const D3DXVECTOR3 pos);
	void SetCurrentRotation(const D3DXVECTOR3 rot);

private:	// 自分だけがアクセス可能

	// メンバ変数
	int m_nIdxModel;		// モデル番号
	bool m_bChangeCol;		// 色変更をするかどうか
	D3DMATERIAL9 m_ChangeMat;
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_rot;		// 向き
	D3DXVECTOR3 m_CurPos;	// 現在の位置
	D3DXVECTOR3 m_CurRot;	// 現在の向き
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
	D3DXMATRIX *m_pParentMtx;	// 親のワールドマトリックス
};

#endif

//==========================================================
//
// エディット処理全般 [editor.h]
//Author : Ibuki Okusada
//
//==========================================================
#ifndef _EDITOR_H_
#define _EDITOR_H_

// 前方宣言
class CObjectX;

//**********************************************************
// メッシュフィールドクラスの定義(派生クラス)
//**********************************************************
class CEditor
{
private:	// 自分だけがアクセス可能な定義

	typedef struct
	{
		D3DXVECTOR3 pos;		//位置
		D3DXVECTOR3 rot;		//向き
		D3DXMATRIX mtxWorld;	//ワールドマトリックス
		int nIdxModel;		// モデル番号
	}ObjectX;

	typedef enum
	{
		TYPE_NONE = 0,
		TYPE_REPEAT,
		TYPE_MAX
	}TYPE;

public:	// 誰でもアクセス可能

	CEditor();	// コンストラクタ
	~CEditor();	// デストラクタ

	// メンバ関数
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Save(void);
	bool GetActive(void) { return m_bUse; }
	bool GetMeshActive(void) { return m_bMesh; }

private:	// 自分だけがアクセス可能
	
	ObjectX m_Object;	// モデルポインタ
	int m_nIdx;			// インデックス
	float m_fSpeed;		// 速度
	int m_nMoveCnt;		// 操作カウント
	int m_nNumMove;		// 移動までの時間
	int m_nType;		// 種類
	float m_fLength;	// メッシュ調整用半径
	float m_fMeshMove;
	bool m_bUse;		// 使用中かどう
	bool m_bMesh;		// メッシュ操作するか否か
};


#endif


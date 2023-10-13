//===============================================
//
// エフェクトの処理 [effect.h]
// Author : Ibuki Okusada
//
//===============================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "object2D.h"
#include "billboard.h"

//===============================================
//クラスの定義
//===============================================
class CEffect : public CObjectBillboard
{
public:	// 誰でもアクセス可能な定義

	// 種類の列挙型定義
	typedef enum
	{
		TYPE_NONE = 0,	// 通常
		TYPE_BULLET,	// 弾
		TYPE_EXPLOSION,	// 爆発
		TYPE_DUST,		// 土煙
		TYPE_SHWBULLET,	// シャワー弾
		TYPE_SHWREF,	// 地面反射シャワー
		TYPE_SWEAT,		// 汗
		TYPE_HEAT,		// 熱中症状態
		TYPE_JUMP,		// ジャンプ
		TYPE_SWAP,		// 軌跡っぽいの
		TYPE_BALEXPLOSION,	// 風船爆発
		TYPE_SMAKE,		// 煙
		TYPE_HEATHAZE,	// 陽炎
		TYPE_SLOWOK,	// スロー可能
		TYPE_MAX
	}TYPE;

private:	// 自分だけアクセス可能な定義

	// 情報構造体の定義
	typedef struct
	{
		float fLife;	// 寿命
		TYPE Type;	// 種類
		D3DXCOLOR col;	//色
		D3DXVECTOR3 move;	// 移動量
		float fRadius;	//半径
	}INFO;

public:	// 誰でもアクセス可能

	//CEffect();	// コンストラクタ
	CEffect(D3DXVECTOR3 pos);	// コンストラクタ(オーバーロード)
	CEffect(int nPriority = 5);	// デフォルト引数コンストラクタ
	~CEffect();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CEffect *Create(D3DXVECTOR3 pos, TYPE type);
	static CEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, float fLife, TYPE type);
	void SetMove(D3DXVECTOR3 move);
	D3DXVECTOR3 GetMove(void) { return m_Info.move; }

private:	// 自分だけがアクセス可能

	// メンバ関数
	void RadiusSet(void);
	void ColorSet(void);

	// メンバ変数
	int nIdxTexture;	// テクスチャ番号
	static const D3DXCOLOR m_aColInfo[TYPE_MAX];	// 色の情報
	static const float m_aRadiusInfo[TYPE_MAX];		// 半径の情報
	INFO m_Info;	// 情報の構造体
};

#endif 

//===============================================
//
// パーティクルの処理 [particle.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "particle.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "texture.h"

//===============================================
// マクロ定義
//===============================================
#define LIFE	(30)		// 寿命

//===============================================
// コンストラクタ
//===============================================
CParticle::CParticle()
{

}

//===============================================
// デストラクタ
//===============================================
CParticle::~CParticle()
{

}

//===============================================
// 生成
//===============================================
void CParticle::Create(const D3DXVECTOR3& pos, CEffect::TYPE type)
{
	// オブジェクトの種類の設定
	Set(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), type);
}

//===============================================
// 頂点情報設定
//===============================================
void CParticle::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, CEffect::TYPE type)
{
	// オブジェクトの種類の設定
	Set(pos, move, type);
}

//===============================================
// 設定
//===============================================
void CParticle::Set(const D3DXVECTOR3& Defpos, const D3DXVECTOR3& Defmove, CEffect::TYPE type)
{
	D3DXVECTOR3 pos = {};
	D3DXVECTOR3 move = {};	// 移動量
	D3DXCOLOR col = {};	// 色
	CEffect::TYPE ParType = type;
	float fRadius = 0.0f;
	float fLife = 0;
	D3DXVECTOR3 nor;
	float fCol = (float)(rand() % 3) / 10.0f + 0.3f;

	// 移動ベクトルを求める
	D3DXVec3Normalize(&nor, &Defmove);	// ベクトルを正規化する

	switch (type)
	{
	case CEffect::TYPE_NONE:	// 何もない

		break;

	case CEffect::TYPE_BULLET:	// 弾

			// 座標の設定
		pos = Defpos;

		//移動量の設定
		move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * ((float)(rand() % 100)) * 0.1f;
		move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * ((float)(rand() % 100)) * 0.1f;
		move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * ((float)(rand() % 100)) * 0.1f;

		//色の設定
		col = D3DXCOLOR(fCol, fCol + 0.1f, 1.0f, 1.0f);

		//半径の設定
		fRadius = 3.0f;

		//寿命の設定
		fLife = 10.0f;

		CEffect::Create(Defpos, move, col, fRadius, fLife, type);

		break;

	case CEffect::TYPE_EXPLOSION:	// 爆発

		for (int nCnt = 0; nCnt < 10; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314)  * 0.01f) * ((float)(rand() % 100)) * 0.01f;
			move.y = ((float)(rand() % 100)) * 0.001f;
			move.z = cosf((float)(rand() % 629 - 314)  * 0.01f) * ((float)(rand() % 100)) * 0.01f;

			//色の設定
			col = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);

			//半径の設定
			fRadius = 20.0f;

			//寿命の設定
			fLife = 80.0f;

			CEffect::Create(Defpos, move + Defmove * 0.5f, col, fRadius, fLife, type);
		}

		break;

	case CEffect::TYPE_DUST:	// 煙

		for (int nCnt = 0; nCnt < 15; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314)  * 0.01f) * ((float)(rand() % 100)) * 0.04f;
			move.y = ((float)(rand() % 100)) * 0.001f;
			move.z = cosf((float)(rand() % 629 - 314)  * 0.01f) * ((float)(rand() % 100)) * 0.04f;

			//色の設定
			col = D3DXCOLOR(0.08f, 0.07f, 0.07f, 1.0f);

			//半径の設定
			fRadius = 30.0f;

			//寿命の設定
			fLife = 100.0f;

			CEffect::Create(Defpos, move + Defmove * 0.5f, col, fRadius, fLife, type);
		}

		break;

	case CEffect::TYPE_SHWBULLET:	// 弾
	{
		for (int nCnt = 0; nCnt < 1; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			fCol = (float)(rand() % 3) * 0.05f + 0.0f;

			//移動量の設定
			move.x = -sinf(Defmove.y + -D3DX_PI * 0.1f + D3DX_PI * 0.2f * ((rand() % 10) * 0.1f)) * 1.5f;
			move.y = rand() % 300 * 0.01f;
			move.z = -cosf(Defmove.y + -D3DX_PI * 0.1f + D3DX_PI * 0.2f * ((rand() % 10) * 0.1f)) * 1.5f;

			//色の設定
			col = D3DXCOLOR(0.5f - fCol, 0.5f - fCol, 0.0f, 1.0f);

			//半径の設定
			fRadius = 8.0f;

			//寿命の設定
			fLife = 100.0f;

			
			ParType = CEffect::TYPE_SHWBULLET;

			CEffect *p = CEffect::Create(Defpos, move, col, fRadius, fLife, ParType);
			p->SetFusion(CEffect::FUSION_ADD);
		}
	}
		break;

	case CEffect::TYPE_SWEAT:	// 汗

		fCol = (float)(rand() % 3) / 10.0f + 0.3f;

		for (int nCnt = 0; nCnt < 1; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			pos.x += (float)(rand() % 21 - 10);
			pos.z += (float)(rand() % 21 - 10);

			//移動量の設定
			move.x = -sinf(Defmove.y + -D3DX_PI * 0.1f + D3DX_PI * 0.2f * ((rand() % 10) * 0.1f)) * 0.5f;
			move.y = rand() % 300 * 0.01f;
			move.z = -cosf(Defmove.y + -D3DX_PI * 0.1f + D3DX_PI * 0.2f * ((rand() % 10) * 0.1f)) * 0.5f;

			//色の設定
			col = D3DXCOLOR(fCol + 0.0f, fCol + 0.0f, fCol + 0.0f, 1.0f);

			//半径の設定
			fRadius = 10.0f;

			//寿命の設定
			fLife = 60.0f;

			CEffect *p = CEffect::Create(pos, move, col, fRadius, fLife, type);
			p->SetFusion(CEffect::FUSION_MINUS);
		}

		break;

	case CEffect::TYPE_HEAT:	// 湯気

		fCol = (float)(rand() % 3) / 10.0f + 0.3f;

		for (int nCnt = 0; nCnt < 1; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			pos.x += (float)(rand() % 21 - 10);
			pos.z += (float)(rand() % 21 - 10);

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * ((float)(rand() % 100)) * 0.002f;
			move.y = rand() % 100 * 0.005f;
			move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * ((float)(rand() % 100)) * 0.002f;
			//色の設定
			col = D3DXCOLOR(fCol + 0.5f, fCol + 0.5f, fCol + 0.5f, 1.0f);

			//半径の設定
			fRadius = 10.0f;

			//寿命の設定
			fLife = 100.0f;

			CEffect::Create(pos, move, col, fRadius, fLife, type);
		}

		break;

	case CEffect::TYPE_JUMP:	// ジャンプ

		for (int nCnt = 0; nCnt < 10; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314)  * 0.01f) * ((float)(rand() % 100)) * 0.04f;
			move.y = -0.01f;
			move.z = cosf((float)(rand() % 629 - 314)  * 0.01f) * ((float)(rand() % 100)) * 0.04f;

			// 移動ベクトルを求める
			D3DXVec3Normalize(&nor, &move);	// ベクトルを正規化する

			pos += nor * 100.0f;
			pos.y = Defpos.y;

			//色の設定
			col = D3DXCOLOR(0.08f, 0.07f, 0.07f, 1.0f);

			//半径の設定
			fRadius = 20.0f;

			//寿命の設定
			fLife = 100.0f;

			CEffect::Create(pos, move + Defmove * 0.5f, col, fRadius, fLife, type);
		}

		break;

	case CEffect::TYPE_SWAP:	// ジャンプの線

		for (int nCnt = 0; nCnt < 20; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314)  * 0.01f) * ((float)(rand() % 100)) * 0.04f;
			move.z = cosf((float)(rand() % 629 - 314)  * 0.01f) * ((float)(rand() % 100)) * 0.04f;

			// 移動ベクトルを求める
			D3DXVec3Normalize(&nor, &move);	// ベクトルを正規化する

			pos += nor * 100.0f;
			pos.y = Defpos.y;

			move = Defmove;

			//色の設定
			col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);

			//半径の設定
			fRadius = 5.0f;

			//寿命の設定
			fLife = 100.0f;

			CEffect *pEffect = CEffect::Create(pos, move, col, fRadius, fLife, type);

			pEffect->SetSize(fRadius, 50.0f);
		}

		break;

	case CEffect::TYPE_BALEXPLOSION:	// 風船爆発

		for (int nCnt = 0; nCnt < 30; nCnt++)
		{
			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.025f;
			move.y = ((float)(rand() % 50)) * 0.025f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.025f;

			//色の設定
			col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//半径の設定
			fRadius = 15.0f;

			//寿命の設定
			fLife = 100.0f + nCnt * 3;

			CEffect *p = CEffect::Create(Defpos + move, move, col, fRadius, fLife, CEffect::TYPE_BUBBLE);
			p->SetFusion(CEffect::FUSION_NORMAL);
			p->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\balloon.png"));
		}

		break;

	case CEffect::TYPE_SMAKE:	// 煙

		for (int nCnt = 0; nCnt < 10; nCnt++)
		{
			// 座標の設定
			pos = Defpos;

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.04f;
			move.y = ((float)(rand() % 50)) * 0.1f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.04f;

			//色の設定
			col = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);

			//半径の設定
			fRadius = 55.0f;

			//寿命の設定
			fLife = 75.0f;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}

		break;

	case CEffect::TYPE_BUBBLE:	// 煙
	{
		//移動量の設定
		move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.0015f;
		move.y = ((float)(rand() % 50)) * 0.025f;
		move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.0015f;

		//色の設定
		col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//半径の設定
		fRadius = 15.0f;

		//寿命の設定
		fLife = 200.0f;

		CEffect *p = CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		p->SetFusion(CEffect::FUSION_NORMAL);
		p->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\balloon.png"));
	}
		break;

	case CEffect::TYPE_HEATHAZE:	// 煙

			// 座標の設定
		pos = Defpos;

		//移動量の設定
		move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.04f;
		move.y = ((float)(rand() % 50)) * 0.075f;
		move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.04f;

		//色の設定
		col = D3DXCOLOR(0.45f, 0.2f, 0.2f, 0.11f);

		//半径の設定
		fRadius = 300.0f;

		//寿命の設定
		fLife = 300.0f;

		CEffect *pEffect = CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		pEffect->BindTexture(CManager::GetInstance()->GetTexture()->Regist(CTexture::GetFileName(CTexture::TYPE_HEATHAZE)));

		break;
	}
}
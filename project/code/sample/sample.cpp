//==========================================================
//
// サンプル [sample.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "sample.h"

// マクロ定義

//==========================================================
// コンストラクタ
//==========================================================
CSample::CSample(int nPriOrity) : CObject(nPriOrity)
{

}

//==========================================================
// デストラクタ
//==========================================================
CSample::~CSample()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CSample::Init(void)
{
	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CSample::Uninit(void)
{
	Release();
}

//==========================================================
// 更新処理
//==========================================================
void CSample::Update(void)
{

}

//==========================================================
// 描画処理
//==========================================================
void CSample::Draw(void)
{
	
}

//==========================================================
// 生成
//==========================================================
CSample *CSample::Create(void)
{
	CSample *pSample = nullptr;

	if (pSample != NULL)
	{
		// 初期化処理
		pSample->Init();
	}

	return pSample;
}
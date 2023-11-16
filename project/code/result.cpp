//===============================================
//
// リザルト画面の管理処理 [result.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "result.h"
#include "object2D.h"
#include "texture.h"
#include "manager.h"
#include "main.h"
#include "input.h"
#include "objectX.h"
#include "fade.h"
#include "ranking.h"
#include "sound.h"
#include "result.h"
#include "time.h"
#include "fileload.h"
#include "meshdome.h"
#include "character.h"
#include "motion.h"
#include "camera.h"

// マクロ定義
#define RANKING_FILE	"data\\FILE\\ranking.bin"	// ランキングファイル
#define MOVE_TIMER	(660)

// リザルトスコア
int CResult::m_nScore = 0;
CResult::TYPE CResult::m_type = CResult::TYPE_MAX;

//===============================================
// コンストラクタ
//===============================================
CResult::CResult()
{
	m_pMeshSky = NULL;
	m_nTimer = 0;
	m_pTime = NULL;

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_apCharacter[nCnt] = NULL;
	}
}

//===============================================
// デストラクタ
//===============================================
CResult::~CResult()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CResult::Init(void)
{
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_RESULT);
	CTexture *pTexture = CManager::GetInstance()->GetTexture();
	CMeshDome::Create(D3DXVECTOR3(0.0f, -1000.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10000.0f, 10.0f, 10, 10);

	if (m_pFileLoad == NULL)
	{// 使用していない場合
		m_pFileLoad = new CFileLoad;

		if (m_pFileLoad != NULL)
		{
			m_pFileLoad->Init();
			m_pFileLoad->OpenFile("data\\TXT\\result.txt");
		}
	}

	// 種類ごとに描画
	switch (m_type)
	{

	case TYPE_MULTI_WIN:
	{
		m_apCharacter[TYPE_MULTI_WIN] = CCharacter::Create("data\\TXT\\motion_kidsboy.txt");
		m_apCharacter[TYPE_MULTI_WIN]->GetMotion()->InitSet(4);
		m_apCharacter[TYPE_MULTI_WIN]->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 100.0f));
		m_apCharacter[TYPE_MULTI_WIN]->SetRotation(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
		m_apCharacter[TYPE_MULTI_LOSE] = CCharacter::Create("data\\TXT\\motion_kidsgirl.txt");
		m_apCharacter[TYPE_MULTI_LOSE]->GetMotion()->InitSet(5);
		m_apCharacter[TYPE_MULTI_LOSE]->SetPosition(D3DXVECTOR3(0.0f, 0.0f, -100.0f));
		m_apCharacter[TYPE_MULTI_LOSE]->SetRotation(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));

		CManager::GetInstance()->GetCamera()->SetRotation(D3DXVECTOR3(0.0f, -D3DX_PI * 1.0f, 1.51f));
		CManager::GetInstance()->GetCamera()->SetPositionR(D3DXVECTOR3(1000.0f, 0.0f, 100.0f));

		CObject2D *pObj = CObject2D::Create();
		pObj->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.8f, 0.0f));
		pObj->SetSize(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.2f);
		pObj->BindTexture(pTexture->Regist("data\\TEXTURE\\result_win.png"));
	}
		break;

	case TYPE_MULTI_LOSE:
	{
		m_apCharacter[TYPE_MULTI_WIN] = CCharacter::Create("data\\TXT\\motion_kidsgirl.txt");
		m_apCharacter[TYPE_MULTI_WIN]->GetMotion()->InitSet(4);
		m_apCharacter[TYPE_MULTI_WIN]->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 100.0f));
		m_apCharacter[TYPE_MULTI_WIN]->SetRotation(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
		m_apCharacter[TYPE_MULTI_LOSE] = CCharacter::Create("data\\TXT\\motion_kidsboy.txt");
		m_apCharacter[TYPE_MULTI_LOSE]->GetMotion()->InitSet(5);
		m_apCharacter[TYPE_MULTI_LOSE]->SetPosition(D3DXVECTOR3(0.0f, 0.0f, -100.0f));
		m_apCharacter[TYPE_MULTI_LOSE]->SetRotation(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));

		CManager::GetInstance()->GetCamera()->SetRotation(D3DXVECTOR3(0.0f, -D3DX_PI * 1.0f, 1.51f));
		CManager::GetInstance()->GetCamera()->SetPositionR(D3DXVECTOR3(1000.0f, 0.0f, -100.0f));

		CObject2D *pObj = CObject2D::Create();
		pObj->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.8f, 0.0f));
		pObj->SetSize(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.2f);
		pObj->BindTexture(pTexture->Regist("data\\TEXTURE\\result_lose.png"));
	}
		break;

	case TYPE_MAX:
	{
		m_apCharacter[TYPE_MULTI_WIN] = CCharacter::Create("data\\TXT\\motion_kidsboy.txt");
		if (m_nScore != 0)
		{
			m_apCharacter[TYPE_MULTI_WIN]->GetMotion()->InitSet(4);
		}
		else
		{
			m_apCharacter[TYPE_MULTI_WIN]->GetMotion()->InitSet(5);
		}

		m_apCharacter[TYPE_MULTI_WIN]->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 100.0f));
		m_apCharacter[TYPE_MULTI_WIN]->SetRotation(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));

		CManager::GetInstance()->GetCamera()->SetRotation(D3DXVECTOR3(0.0f, -D3DX_PI * 1.0f, 1.51f));
		CManager::GetInstance()->GetCamera()->SetPositionR(D3DXVECTOR3(1000.0f, 0.0f, 0.0f));
		m_pTime = CTime::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.4f, SCREEN_HEIGHT * 0.075f, 0.0f));
		m_pTime->Set(m_nScore);
		m_pTime->SetColor(D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f));

		int aScore[NUM_RANK] = {};	// スコア格納用
		m_nRank = -1;	//ランクインしてない状態

		// データの読み込み
		Load(&aScore[0]);

		// データのソート
		Sort(&aScore[0]);

		// ランクイン確認
		RankIn(&aScore[0], m_nScore);

		// 順位分スコアの生成
		for (int nCntRank = 0; nCntRank < NUM_RANK; nCntRank++)
		{
			if (m_apScore[nCntRank] == NULL)
			{
				CObject2D *p = CObject2D::Create(7);
				p->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\ranking001.png"));
				p->SetPosition(D3DXVECTOR3(675.0f, 250.0f + nCntRank * 100.0f, 0.0f));
				p->SetSize(40, 50);
				p->SetVtx(nCntRank + 1, 10, 1);

				m_apScore[nCntRank] = CTime::Create(D3DXVECTOR3(835.0f, 250.0f + nCntRank * 100.0f, 0.0f));
				m_apScore[nCntRank]->Set(aScore[nCntRank]);

				if (m_nRank == nCntRank)
				{
					m_apScore[nCntRank]->SetColor(D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f));
				}
			}
		}
	}
		break;
	}

	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_RANKING);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CResult::Uninit(void)
{
	for (int nCntRank = 0; nCntRank < NUM_RANK; nCntRank++)
	{
		if (m_apScore[nCntRank] != NULL)
		{
			m_apScore[nCntRank]->Uninit();
			delete m_apScore[nCntRank];	// メモリの開放
			m_apScore[nCntRank] = NULL;	// 使用していない状態にする
		}
	}

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apCharacter[nCnt] != NULL)
		{
			m_apCharacter[nCnt]->Uninit();
			delete m_apCharacter[nCnt];
			m_apCharacter[nCnt] = NULL;
		}
	}

	if (m_pTime != NULL)
	{
		m_pTime->Uninit();
		delete m_pTime;
		m_pTime = NULL;
	}

	m_type = TYPE_MAX;
	m_nScore = 0;
}

//===============================================
// 更新処理
//===============================================
void CResult::Update(void)
{
	m_nTimer++;

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		if (m_apCharacter[nCnt] != nullptr)
		{
			m_apCharacter[nCnt]->Update();
		}
	}

	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RETURN) || m_nTimer > MOVE_TIMER 
		|| CManager::GetInstance()->GetInputPad()->GetTrigger(CInputPad::BUTTON_A, 0) || CManager::GetInstance()->GetInputPad()->GetTrigger(CInputPad::BUTTON_START, 0))
	{
		CManager::GetInstance()->GetFade()->Set(CScene::MODE_TITLE);
	}

	CScene::Update();
}

//===============================================
// 描画処理
//===============================================
void CResult::Draw(void)
{
	CScene::Draw();
}

//===============================================
// ランキングデータ保存
//===============================================
void CResult::Save(int *pScore)
{
	FILE *pFile;

	pFile = fopen(RANKING_FILE, "wb");

	if (pFile != NULL)
	{//ファイルが開けた場合

	 //データを読み込む
		fwrite(pScore, sizeof(int), NUM_RANK, pFile);

		//ファイルを閉じる
		fclose(pFile);
	}
}

//===============================================
// ランキングデータ読み込み
//===============================================
void CResult::Load(int *pScore)
{
	FILE *pFile;

	pFile = fopen(RANKING_FILE, "rb");

	if (pFile != NULL)
	{//ファイルが開けた場合

	 //データを読み込む
		fread(pScore, sizeof(int), NUM_RANK, pFile);

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
	 //要素を入れておく
		for (int nCntRanking = 0; nCntRanking < NUM_RANK; nCntRanking++)
		{
			pScore[nCntRanking] = 500 + (nCntRanking * 100);
		}
	}
}

//===============================================
// ランキングデータソート
//===============================================
void CResult::Sort(int *pScore)
{
	// 降順ソート
	for (int nCntFst = 0; nCntFst < NUM_RANK - 1; nCntFst++)
	{
		int nTempNum = nCntFst;	// 仮の一番大きい番号

		for (int nCntSec = nCntFst + 1; nCntSec < NUM_RANK; nCntSec++)
		{
			if (pScore[nCntSec] > pScore[nTempNum])
			{// 値が大きい場合
				nTempNum = nCntSec;	// 大きい番号を変更
			}
		}

		if (nTempNum != nCntFst)
		{// 変更する場合
			int nTemp = pScore[nCntFst];
			pScore[nCntFst] = pScore[nTempNum];
			pScore[nTempNum] = nTemp;
		}
	}
}

//===============================================
// ランキングイン確認
//===============================================
void CResult::RankIn(int *pScore, int nResult)
{
	if (nResult > pScore[NUM_RANK - 1])
	{
		pScore[NUM_RANK - 1] = nResult;

		// ソート処理
		Sort(pScore);

		// 保存処理
		Save(pScore);

		//ランクインした順位を確認する
		for (int nCntRank = 0; nCntRank < NUM_RANK; nCntRank++)
		{
			if (pScore[nCntRank] == nResult)
			{
				m_nRank = nCntRank;	// ランクインした順位を保存			
				break;
			}
		}
	}
}
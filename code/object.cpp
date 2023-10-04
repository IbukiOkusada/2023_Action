//===============================================
//
// オブジェクトの処理 [object.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "game.h"

// マクロ定義
#define DEFAULT_PRIORITY	(3)	// 基本の優先順位

//===============================================
// 静的メンバ変数宣言
//===============================================
CObject *CObject::m_apTop[NUM_PRIORITY] = {};	// 先頭のオブジェクトへのポインタ
CObject *CObject::m_apCur[NUM_PRIORITY] = {};	// 最後尾のオブジェクトへのポインタ
int CObject::m_nNumAll = 0;
int CObject::m_nItemNumAll = 0;
int CObject::m_nNumEnemyAll = 0;
int CObject::m_aPriNumAll[NUM_PRIORITY] = {};

//===============================================
// コンストラクタ
//===============================================
//CObject::CObject()
//{
//	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
//	{
//		if (m_apObject[DEFAULT_PRIORITY][nCntObject] == NULL)
//		{// 使用されていない場合
//			m_apObject[DEFAULT_PRIORITY][nCntObject] = this;	// 自分自身を代入
//			m_nID = nCntObject;	// 自分自身のIDを保
//			m_nNumAll++;			// 総数をカウントアップ
//			m_nPriority = DEFAULT_PRIORITY;
//			m_type = TYPE_NONE;	// 種類をリセット
//			break;
//		}
//	}
//}

//===============================================
// コンストラクタ(オーバーロード)
//===============================================
CObject::CObject(int nPriority)
{
	// 値のクリア
	m_pNext = NULL;
	m_pPrev = NULL;
	m_bDeath = false;

	// 優先順位の確認
	if (nPriority < 0)
	{
		nPriority = 0;
	}
	else if (nPriority >= NUM_PRIORITY)
	{
		nPriority = NUM_PRIORITY - 1;
	}

	m_nPriority = nPriority;	// 設定

	// 自分自身をリストに追加
	if (m_apTop[nPriority] != NULL)
	{// 先頭が存在している場合
		m_apCur[nPriority]->m_pNext = this;	// 現在最後尾のオブジェクトのポインタにつなげる
		m_pPrev = m_apCur[nPriority];
		m_apCur[nPriority] = this;	// 自分自身が最後尾になる
	}
	else
	{// 存在しない場合
		m_apTop[nPriority] = this;	// 自分自身が先頭になる
		m_apCur[nPriority] = this;	// 自分自身が最後尾になる
	}

	m_aPriNumAll[nPriority]++;
	m_nNumAll++;
}

//===============================================
// デストラクタ
//===============================================
CObject::~CObject()
{
	
}

#if 0	// 純粋仮想関数なので通らない
//===============================================
// 初期化処理
//===============================================
HRESULT CObject::Init(void)
{

}
#endif

#if 0	// 純粋仮想関数なので通らない
//===============================================
// 終了処理
//===============================================
void CObject::Uninit(void)
{
	
}
#endif

#if 0	// 純粋仮想関数なので通らない
//===============================================
// 更新処理
//===============================================
void CObject::Update(void)
{

}
#endif

#if 0	// 純粋仮想関数なので通らない
//===============================================
// 描画処理
//===============================================
void CObject::Draw(void)
{

}
#endif

//===============================================
// 全てのオブジェクトの廃棄
//===============================================
void CObject::ReleaseAll(void)
{
	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		CObject *pObject = m_apTop[nCntPri];	// 先頭を取得
		
		while(pObject != NULL)
		{// 使用されていない状態まで

			CObject *pObjectNext = pObject->m_pNext;	// 次のオブジェクトへのポインタを取得

			// 終了処理
			pObject->Uninit();

			pObject = pObjectNext;	// 次のオブジェクトに移動
		}
	}

	// 死亡フラグをチェック
	DeathCheck();
}

//===============================================
// 全てのオブジェクトの更新
//===============================================
void CObject::UpdateAll(void)
{
	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		CObject *pObject = m_apTop[nCntPri];	// 先頭を取得

		while (pObject != NULL)
		{// 使用されていない状態まで
			CObject *pObjectNext = pObject->m_pNext;	// 次のオブジェクトへのポインタを取得

			if (pObject->m_bDeath == false)
			{// 死亡していない場合
				// 更新処理
				pObject->Update();
			}

			pObject = pObjectNext;	// 次のオブジェクトに移動
		}
	}

	// 死亡フラグをチェック
	DeathCheck();
}

//===============================================
// 全てのオブジェクトの描画
//===============================================
void CObject::DrawAll(void)
{
	CCamera *pCamera = CManager::GetCamera();
	CCamera *pMapCamera = CManager::GetScene()->GetMapCamera();

	if (pMapCamera != NULL)
	{
		pMapCamera->SetCamera();
	}

	if (pCamera != NULL)
	{// 使用されている場合
		pCamera->SetCamera();
	}

	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		CObject *pObject = m_apTop[nCntPri];	// 先頭を取得

		while (pObject != NULL)
		{// 使用されていない状態まで
			CObject *pObjectNext = pObject->m_pNext;	// 次のオブジェクトへのポインタを取得

			if (pObject->m_type != TYPE_PAUSE && pObject->m_type != TYPE_MAP)
			{// ポーズ画面以外

				// 描画処理
				pObject->Draw();

			}
			pObject = pObjectNext;	// 次のオブジェクトに移動
		}
	}
}

//===============================================
// オブジェクトの廃棄
//===============================================
void CObject::Release(void)
{
	// 死亡状態にする
	m_bDeath = true;
}

//===============================================
// タイプの設定
//===============================================
void CObject::SetType(const TYPE type)
{
	if (m_type != type)
	{
		if (type == TYPE_MODEL && m_type != TYPE_MODEL)
		{
			m_nItemNumAll++;
		}
		else if (type == TYPE_ENEMY && m_type != TYPE_ENEMY)
		{
			m_nNumEnemyAll++;
		}

		if (type != TYPE_MODEL && m_type == TYPE_MODEL)
		{
			m_nItemNumAll--;
		}
		else if (type != TYPE_ENEMY && m_type == TYPE_ENEMY)
		{
			m_nNumEnemyAll--;
		}
	}

	m_type = type;	// 種類を設定
}

//===============================================
// 死亡フラグをチェック
//===============================================
void CObject::DeathCheck(void)
{
	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		CObject *pObject = m_apTop[nCntPri];	// 先頭を取得

		while (pObject != NULL)
		{// 使用されていない状態まで
			CObject *pObjectNext = pObject->m_pNext;	// 次のオブジェクトへのポインタを取得

			if (pObject->m_bDeath == true)
			{
				TYPE type = pObject->m_type;

				// リストから自分自身を削除する
				if (m_apTop[nCntPri] == pObject)
				{// 自身が先頭
					if (pObject->m_pNext != NULL)
					{// 次が存在している
						m_apTop[nCntPri] = pObject->m_pNext;	// 次を先頭にする
						pObject->m_pNext->m_pPrev = NULL;	// 次の前のポインタを覚えていないようにする
					}
					else
					{// 存在していない
						m_apTop[nCntPri] = NULL;	// 先頭がない状態にする
						m_apCur[nCntPri] = NULL;	// 最後尾がない状態にする
					}
				}
				else if (m_apCur[nCntPri] == pObject)
				{// 自身が最後尾
					if (pObject->m_pPrev != NULL)
					{// 次が存在している
						m_apCur[nCntPri] = pObject->m_pPrev;	// 前を最後尾にする
						pObject->m_pPrev->m_pNext = NULL;	// 前の次のポインタを覚えていないようにする
					}
					else
					{// 存在していない
						m_apTop[nCntPri] = NULL;	// 先頭がない状態にする
						m_apCur[nCntPri] = NULL;	// 最後尾がない状態にする
					}
				}
				else
				{
					if (pObject->m_pPrev != NULL)
					{
						pObject->m_pPrev->m_pNext = pObject->m_pNext;	// 自身の前に次のポインタを覚えさせる
					}

					if (pObject->m_pNext != NULL)
					{
						pObject->m_pNext->m_pPrev = pObject->m_pPrev;	// 自身の次に前のポインタを覚えさせる
					}
				}

				delete pObject;	// メモリの開放
				pObject = NULL;

				m_aPriNumAll[nCntPri]--;
				m_nNumAll--;

				if (type == TYPE_MODEL)
				{
					m_nItemNumAll--;
				}
				else if (type == TYPE_ENEMY)
				{
					m_nNumEnemyAll--;
				}
			}

			pObject = pObjectNext;	// 次のオブジェクトに移動
		}
	}
}
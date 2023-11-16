//===================================================
//
// 階層構造によるモーション処理 [motion.cpp]
// Author Ibuki Okusada
//
//===================================================
#include "motion.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "texture.h"
#include "Xfile.h"
#include "model.h"
#include <string.h>
#include "slow.h"

// マクロ定義
#define DEST_MAG	(0.1f)	// 差分補正倍率

//===================================================
// コンストラクタ
//===================================================
CMotion::CMotion()
{
	m_nNowFrame = 0;
	m_nNumMotion = 0;
	m_nNowKey = 0;
	m_nNowMotion = 1;
	m_FileData.nNumParts = 0;
	m_FileData.ppParts = NULL;

	for (int nCnt = 0; nCnt < MAX_MOTION; nCnt++)
	{
		aInfo[nCnt].nNumKey = 0;
		aInfo[nCnt].pKeyInfo = NULL;
		aInfo[nCnt].bLoop = false;
	}

	//OldKey = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
}

//===================================================
// デストラクタ
//===================================================
CMotion::~CMotion()
{

}

//===================================================
// 終了処理
//===================================================
void CMotion::Uninit(void)
{
	// モーション情報の廃棄
	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{// モーション数分繰り返す
		if (aInfo[nCntMotion].pKeyInfo != NULL)
		{// キー情報が使われている場合

			for (int nCntKey = 0; nCntKey < aInfo[nCntMotion].nNumKey; nCntKey++)
			{// キー数分繰り返す

				delete[] aInfo[nCntMotion].pKeyInfo[nCntKey].aKey;
				aInfo[nCntMotion].pKeyInfo[nCntKey].aKey = NULL;
			}
			delete[] aInfo[nCntMotion].pKeyInfo;
			aInfo[nCntMotion].pKeyInfo = NULL;
		}
	}
}

//===================================================
// 更新処理
//===================================================
void CMotion::Update(void)
{
	if (m_FileData.ppParts != NULL)
	{// 使用されていない場合
		if (aInfo[m_nNowMotion].nNumKey > 0)
		{//キー数が存在している場合
			int nNowMotion = m_nNowMotion;	// 現在のモーション番号
			int nNowKey = m_nNowKey;		// 現在のキー
			int nNextkey = (nNowKey + 1) % aInfo[nNowMotion].nNumKey;	//次のキー
			int nFrame = aInfo[nNowMotion].pKeyInfo[nNowKey].nFrame;
			float fFrame = ((float)m_fNowFrame / (float)nFrame);	// フレーム間の差分

			for (int nCntParts = 0; nCntParts < m_FileData.nNumParts; nCntParts++)
			{
				D3DXVECTOR3 setPos = m_FileData.ppParts[nCntParts]->GetPosition();	// 初期位置
				D3DXVECTOR3 setRot = m_FileData.ppParts[nCntParts]->GetRotation();	// 初期向き

				//差分を算出
				float fPosDiffX = setPos.x + aInfo[nNowMotion].pKeyInfo[nNextkey].aKey[nCntParts].fPosX	// X座標
					- m_OldKey[nCntParts].fPosX;
				float fPosDiffY = setPos.y + aInfo[nNowMotion].pKeyInfo[nNextkey].aKey[nCntParts].fPosY	// Y座標
					- m_OldKey[nCntParts].fPosY;
				float fPosDiffZ = setPos.z + aInfo[nNowMotion].pKeyInfo[nNextkey].aKey[nCntParts].fPosZ	// Z座標
					- m_OldKey[nCntParts].fPosZ;
				float fRotDiffX = setRot.x + aInfo[nNowMotion].pKeyInfo[nNextkey].aKey[nCntParts].fRotX	// X向き
					- m_OldKey[nCntParts].fRotX;
				float fRotDiffY = setRot.y + aInfo[nNowMotion].pKeyInfo[nNextkey].aKey[nCntParts].fRotY	// Y向き
					- m_OldKey[nCntParts].fRotY;
				float fRotDiffZ = setRot.z + aInfo[nNowMotion].pKeyInfo[nNextkey].aKey[nCntParts].fRotZ	// Z向き
					- m_OldKey[nCntParts].fRotZ;

				if (fRotDiffZ < -D3DX_PI)
				{// z座標角度限界
					fRotDiffZ += D3DX_PI * 2;
				}
				else if (fRotDiffZ > D3DX_PI)
				{// z座標角度限界
					fRotDiffZ += -D3DX_PI * 2;
				}

				if (fRotDiffX < -D3DX_PI)
				{// x座標角度限界
					fRotDiffX += D3DX_PI * 2;
				}
				else if (fRotDiffX > D3DX_PI)
				{// x座標角度限界
					fRotDiffX += -D3DX_PI * 2;
				}

				if (fRotDiffY < -D3DX_PI)
				{// x座標角度限界
					fRotDiffY += D3DX_PI * 2;
				}
				else if (fRotDiffY > D3DX_PI)
				{// x座標角度限界
					fRotDiffY += -D3DX_PI * 2;
				}

				//現在のフレームの座標を求める
				float fPosDestX = m_OldKey[nCntParts].fPosX
					+ fPosDiffX * fFrame;
				float fPosDestY = m_OldKey[nCntParts].fPosY
					+ fPosDiffY * fFrame;
				float fPosDestZ = m_OldKey[nCntParts].fPosZ
					+ fPosDiffZ * fFrame;

				//現在のフレームの向きを求める
				float fRotDestX = m_OldKey[nCntParts].fRotX
					+ fRotDiffX * fFrame;
				float fRotDestY = m_OldKey[nCntParts].fRotY
					+ fRotDiffY * fFrame;
				float fRotDestZ = m_OldKey[nCntParts].fRotZ
					+ fRotDiffZ * fFrame;

				// 設定
				m_FileData.ppParts[nCntParts]->SetCurrentPosition(D3DXVECTOR3(fPosDestX, fPosDestY, fPosDestZ));
				m_FileData.ppParts[nCntParts]->SetCurrentRotation(D3DXVECTOR3(fRotDestX, fRotDestY, fRotDestZ));
			}

			m_fNowFrame += CManager::GetInstance()->GetSlow()->Get();

			if (aInfo[nNowMotion].pKeyInfo[nNowKey].nFrame != 0)
			{//フレームが0ではない場合
				if (m_fNowFrame >= nFrame)
				{//現在のフレーム数が到達した場合
					for (int nCntParts = 0; nCntParts < m_FileData.nNumParts; nCntParts++)
					{
						// 前回の値を取得
						m_OldKey[nCntParts].fPosX = m_FileData.ppParts[nCntParts]->GetCurrentPosition().x;
						m_OldKey[nCntParts].fPosY = m_FileData.ppParts[nCntParts]->GetCurrentPosition().y;
						m_OldKey[nCntParts].fPosZ = m_FileData.ppParts[nCntParts]->GetCurrentPosition().z;
						m_OldKey[nCntParts].fRotX = m_FileData.ppParts[nCntParts]->GetCurrentRotation().x;
						m_OldKey[nCntParts].fRotY = m_FileData.ppParts[nCntParts]->GetCurrentRotation().y;
						m_OldKey[nCntParts].fRotZ = m_FileData.ppParts[nCntParts]->GetCurrentRotation().z;
					}

					m_nOldType = nNowMotion;
					m_nNowKey = nNextkey;	//次のキーに移動
					m_fNowFrame = 0;
				}
			}
			else
			{//0の場合
				m_nNowKey = 0;
				m_fNowFrame = 0;
			}
		}
	}
}

//==========================================================
// モーション情報読み込み
//==========================================================
void CMotion::LoadMotionData(FILE *pFile)
{
	char aStr[128] = {};	//配置数文字格納用
	int nCntParts = 0;
	int nCntKey = 0;
	bool bKey = false;

	while (1)
	{
		//テキスト読み込み
		fscanf(pFile, "%s", &aStr[0]);

		if (nCntKey >= aInfo[m_nNumMotion].nNumKey && bKey == true)
		{//キー数に来た場合
			m_nNumMotion++;	// モーション数カウントアップ
			break;
		}
		if (strcmp(&aStr[0], "LOOP") == 0)
		{//ループ確認
			int nLoop = 0;
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%d", &nLoop);	//(=)読み込み

			if (nLoop == 0)
			{//ループしない場合
				aInfo[m_nNumMotion].bLoop = false;	//ループしないようにする
			}
			else
			{
				aInfo[m_nNumMotion].bLoop = true;	//ループするようにする
			}
		}
		else if (strcmp(&aStr[0], "NUM_KEY") == 0)
		{//キー数
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%d", &aInfo[m_nNumMotion].nNumKey);	//キー数読み込み

																					// キー数分のメモリの確保
			aInfo[m_nNumMotion].pKeyInfo = new KEY_INFO[aInfo[m_nNumMotion].nNumKey];

			// パーツ数分キーを確保
			for (int nCnt = 0; nCnt < aInfo[m_nNumMotion].nNumKey; nCnt++)
			{
				memset(&aInfo[m_nNumMotion].pKeyInfo[nCnt], NULL, sizeof(KEY_INFO));

				aInfo[m_nNumMotion].pKeyInfo[nCnt].aKey = new KEY[m_FileData.nNumParts];
			}

			bKey = true;	//キー数を取得した状態にする
		}
		else if (strcmp(&aStr[0], "KEYSET") == 0)
		{//パーツ情報確認文字あった場合
			bool bFrame = false;
			nCntParts = 0;
			while (1)
			{
				//読み込み
				fscanf(pFile, "%s", &aStr[0]);

				if (nCntParts >= m_FileData.nNumParts)
				{//パーツ総数に到達した場合
					break;
				}
				else if (strcmp(&aStr[0], "FRAME") == 0)
				{//フレーム数
					fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
					fscanf(pFile, "%d", &aInfo[m_nNumMotion].pKeyInfo[nCntKey].nFrame);	//読み込み
					bFrame = true;
				}
				else if (strcmp(&aStr[0], "KEY") == 0 && bFrame == true)
				{//親番号
					while (1)
					{
						//読み込み
						fscanf(pFile, "%s", &aStr[0]);

						if (strcmp(&aStr[0], "END_KEY") == 0)
						{//フレーム数
							break;
						}
						else if (strcmp(&aStr[0], "POS") == 0)
						{//フレーム数
							fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
							fscanf(pFile, "%f", &aInfo[m_nNumMotion].pKeyInfo[nCntKey].aKey[nCntParts].fPosX);	//x座標読み込み
							fscanf(pFile, "%f", &aInfo[m_nNumMotion].pKeyInfo[nCntKey].aKey[nCntParts].fPosY);	//y座標読み込み
							fscanf(pFile, "%f", &aInfo[m_nNumMotion].pKeyInfo[nCntKey].aKey[nCntParts].fPosZ);	//z座標読み込み
						}
						else if (strcmp(&aStr[0], "ROT") == 0)
						{//フレーム数																   
							fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み								
							fscanf(pFile, "%f", &aInfo[m_nNumMotion].pKeyInfo[nCntKey].aKey[nCntParts].fRotX);	//x座標読み込み
							fscanf(pFile, "%f", &aInfo[m_nNumMotion].pKeyInfo[nCntKey].aKey[nCntParts].fRotY);	//y座標読み込み
							fscanf(pFile, "%f", &aInfo[m_nNumMotion].pKeyInfo[nCntKey].aKey[nCntParts].fRotZ);	//z座標読み込み
						}
					}
					nCntParts++;
				}
				else if (strcmp(&aStr[0], "END_KEYSET") == 0)
				{//パーツ情報終了文字確認できた場合
					break;
				}
			}
			nCntKey++;	//キーを移動
		}
		if (strcmp(&aStr[0], "END_MOTIONSET") == 0)
		{//キャラクター情報終了文字確認できた場合
			m_nNumMotion++;	// モーション数カウントアップ
			break;
		}
	}
}

//==========================================================
// モーション番号設定
//==========================================================
void CMotion::BlendSet(int nType)
{
	if (nType >= 0 && nType < m_nNumMotion)
	{// モーション範囲内
		if (m_nNowMotion != nType)
		{// 現在のモーションと違う場合
			m_nOldType = m_nNowMotion;
			m_nNowMotion = nType;	// 種類を設定
			m_nNowKey = aInfo[m_nNowMotion].nNumKey - 1;
			m_fNowFrame = 0;

			for (int nCntParts = 0; nCntParts < m_FileData.nNumParts; nCntParts++)
			{
				// 前回の値を取得
				m_OldKey[nCntParts].fPosX = m_FileData.ppParts[nCntParts]->GetCurrentPosition().x;
				m_OldKey[nCntParts].fPosY = m_FileData.ppParts[nCntParts]->GetCurrentPosition().y;
				m_OldKey[nCntParts].fPosZ = m_FileData.ppParts[nCntParts]->GetCurrentPosition().z;
				m_OldKey[nCntParts].fRotX = m_FileData.ppParts[nCntParts]->GetCurrentRotation().x;
				m_OldKey[nCntParts].fRotY = m_FileData.ppParts[nCntParts]->GetCurrentRotation().y;
				m_OldKey[nCntParts].fRotZ = m_FileData.ppParts[nCntParts]->GetCurrentRotation().z;
			}
		}
	}
}

//==========================================================
// パーツ数取得
//==========================================================
void CMotion::SetModel(CModel **pModel, int nNumParts)
{
	m_FileData.ppParts = pModel;
	m_FileData.nNumParts = nNumParts;
}

//==========================================================
// 初期設定
//==========================================================
void CMotion::InitSet(int nType)
{
	if (nType >= 0 && nType < m_nNumMotion)
	{// モーション範囲内
		m_nNowMotion = nType;	// 種類を設定
		m_nNowKey = 0;
		m_fNowFrame = 0;

		for (int nCntParts = 0; nCntParts < m_FileData.nNumParts; nCntParts++)
		{
			// 前回の値を取得
			m_OldKey[nCntParts].fPosX = m_FileData.ppParts[nCntParts]->GetPosition().x + aInfo[m_nNowMotion].pKeyInfo[m_nNowKey].aKey[nCntParts].fPosX;
			m_OldKey[nCntParts].fPosY = m_FileData.ppParts[nCntParts]->GetPosition().y + aInfo[m_nNowMotion].pKeyInfo[m_nNowKey].aKey[nCntParts].fPosY;
			m_OldKey[nCntParts].fPosZ = m_FileData.ppParts[nCntParts]->GetPosition().z + aInfo[m_nNowMotion].pKeyInfo[m_nNowKey].aKey[nCntParts].fPosZ;
			m_OldKey[nCntParts].fRotX = m_FileData.ppParts[nCntParts]->GetRotation().x + aInfo[m_nNowMotion].pKeyInfo[m_nNowKey].aKey[nCntParts].fRotX;
			m_OldKey[nCntParts].fRotY = m_FileData.ppParts[nCntParts]->GetRotation().y + aInfo[m_nNowMotion].pKeyInfo[m_nNowKey].aKey[nCntParts].fRotY;
			m_OldKey[nCntParts].fRotZ = m_FileData.ppParts[nCntParts]->GetRotation().z + aInfo[m_nNowMotion].pKeyInfo[m_nNowKey].aKey[nCntParts].fRotZ;
		}
	}
}

//==========================================================
// 初期設定
//==========================================================
void CMotion::Set(int nType)
{
	if (nType >= 0 && nType < m_nNumMotion)
	{// モーション範囲内
		if (m_nNowMotion != nType)
		{// 現在のモーションと違う場合
			m_nNowMotion = nType;	// 種類を設定
			m_nNowKey = 0;
			m_fNowFrame = 0;

			for (int nCntParts = 0; nCntParts < m_FileData.nNumParts; nCntParts++)
			{
				// 前回の値を取得
				m_OldKey[nCntParts].fPosX = m_FileData.ppParts[nCntParts]->GetPosition().x + aInfo[m_nNowMotion].pKeyInfo[m_nNowKey].aKey[nCntParts].fPosX;
				m_OldKey[nCntParts].fPosY = m_FileData.ppParts[nCntParts]->GetPosition().y + aInfo[m_nNowMotion].pKeyInfo[m_nNowKey].aKey[nCntParts].fPosY;
				m_OldKey[nCntParts].fPosZ = m_FileData.ppParts[nCntParts]->GetPosition().z + aInfo[m_nNowMotion].pKeyInfo[m_nNowKey].aKey[nCntParts].fPosZ;
				m_OldKey[nCntParts].fRotX = m_FileData.ppParts[nCntParts]->GetRotation().x + aInfo[m_nNowMotion].pKeyInfo[m_nNowKey].aKey[nCntParts].fRotX;
				m_OldKey[nCntParts].fRotY = m_FileData.ppParts[nCntParts]->GetRotation().y + aInfo[m_nNowMotion].pKeyInfo[m_nNowKey].aKey[nCntParts].fRotY;
				m_OldKey[nCntParts].fRotZ = m_FileData.ppParts[nCntParts]->GetRotation().z + aInfo[m_nNowMotion].pKeyInfo[m_nNowKey].aKey[nCntParts].fRotZ;

				// 設定
				m_FileData.ppParts[nCntParts]->SetCurrentPosition(D3DXVECTOR3(m_OldKey[nCntParts].fPosX, m_OldKey[nCntParts].fPosY, m_OldKey[nCntParts].fPosZ));
				m_FileData.ppParts[nCntParts]->SetCurrentRotation(D3DXVECTOR3(m_OldKey[nCntParts].fRotX, m_OldKey[nCntParts].fRotY, m_OldKey[nCntParts].fRotZ));
			}
		}
	}
}

//==========================================================
// 初期設定
//==========================================================
void CMotion::Update(float fSpeedMul)
{
	if (m_FileData.ppParts != NULL)
	{// 使用されていない場合
		if (aInfo[m_nNowMotion].nNumKey > 0)
		{//キー数が存在している場合
			int nNowMotion = m_nNowMotion;	// 現在のモーション番号
			int nNowKey = m_nNowKey;		// 現在のキー
			int nNextkey = (nNowKey + 1) % aInfo[nNowMotion].nNumKey;	//次のキー
			float fNumFrame = aInfo[nNowMotion].pKeyInfo[nNowKey].nFrame * fSpeedMul;

			if (fNumFrame == 0.0f)
			{
				fNumFrame = (float)aInfo[nNowMotion].pKeyInfo[nNowKey].nFrame;
			}
			float fFrame = ((float)m_fNowFrame / fNumFrame);	// フレーム間の差分

			for (int nCntParts = 0; nCntParts < m_FileData.nNumParts; nCntParts++)
			{
				D3DXVECTOR3 setPos = m_FileData.ppParts[nCntParts]->GetPosition();	// 初期位置
				D3DXVECTOR3 setRot = m_FileData.ppParts[nCntParts]->GetRotation();	// 初期向き

																					//差分を算出
				float fPosDiffX = setPos.x + aInfo[nNowMotion].pKeyInfo[nNextkey].aKey[nCntParts].fPosX	// X座標
					- m_OldKey[nCntParts].fPosX;
				float fPosDiffY = setPos.y + aInfo[nNowMotion].pKeyInfo[nNextkey].aKey[nCntParts].fPosY	// Y座標
					- m_OldKey[nCntParts].fPosY;
				float fPosDiffZ = setPos.z + aInfo[nNowMotion].pKeyInfo[nNextkey].aKey[nCntParts].fPosZ	// Z座標
					- m_OldKey[nCntParts].fPosZ;
				float fRotDiffX = setRot.x + aInfo[nNowMotion].pKeyInfo[nNextkey].aKey[nCntParts].fRotX	// X向き
					- m_OldKey[nCntParts].fRotX;
				float fRotDiffY = setRot.y + aInfo[nNowMotion].pKeyInfo[nNextkey].aKey[nCntParts].fRotY	// Y向き
					- m_OldKey[nCntParts].fRotY;
				float fRotDiffZ = setRot.z + aInfo[nNowMotion].pKeyInfo[nNextkey].aKey[nCntParts].fRotZ	// Z向き
					- m_OldKey[nCntParts].fRotZ;

				if (fRotDiffZ < -D3DX_PI)
				{// z座標角度限界
					fRotDiffZ += D3DX_PI * 2;
				}
				else if (fRotDiffZ > D3DX_PI)
				{// z座標角度限界
					fRotDiffZ += -D3DX_PI * 2;
				}

				if (fRotDiffX < -D3DX_PI)
				{// x座標角度限界
					fRotDiffX += D3DX_PI * 2;
				}
				else if (fRotDiffX > D3DX_PI)
				{// x座標角度限界
					fRotDiffX += -D3DX_PI * 2;
				}

				if (fRotDiffY < -D3DX_PI)
				{// x座標角度限界
					fRotDiffY += D3DX_PI * 2;
				}
				else if (fRotDiffY > D3DX_PI)
				{// x座標角度限界
					fRotDiffY += -D3DX_PI * 2;
				}

				//現在のフレームの座標を求める
				float fPosDestX = m_OldKey[nCntParts].fPosX
					+ fPosDiffX * fFrame;
				float fPosDestY = m_OldKey[nCntParts].fPosY
					+ fPosDiffY * fFrame;
				float fPosDestZ = m_OldKey[nCntParts].fPosZ
					+ fPosDiffZ * fFrame;

				//現在のフレームの向きを求める
				float fRotDestX = m_OldKey[nCntParts].fRotX
					+ fRotDiffX * fFrame;
				float fRotDestY = m_OldKey[nCntParts].fRotY
					+ fRotDiffY * fFrame;
				float fRotDestZ = m_OldKey[nCntParts].fRotZ
					+ fRotDiffZ * fFrame;

				// 設定
				m_FileData.ppParts[nCntParts]->SetCurrentPosition(D3DXVECTOR3(fPosDestX, fPosDestY, fPosDestZ));
				m_FileData.ppParts[nCntParts]->SetCurrentRotation(D3DXVECTOR3(fRotDestX, fRotDestY, fRotDestZ));
			}

			m_fNowFrame += CManager::GetInstance()->GetSlow()->Get();

			if (aInfo[nNowMotion].pKeyInfo[nNowKey].nFrame != 0)
			{//フレームが0ではない場合
				if (m_fNowFrame >= fNumFrame)
				{//現在のフレーム数が到達した場合
					for (int nCntParts = 0; nCntParts < m_FileData.nNumParts; nCntParts++)
					{
						// 前回の値を取得
						m_OldKey[nCntParts].fPosX = m_FileData.ppParts[nCntParts]->GetCurrentPosition().x;
						m_OldKey[nCntParts].fPosY = m_FileData.ppParts[nCntParts]->GetCurrentPosition().y;
						m_OldKey[nCntParts].fPosZ = m_FileData.ppParts[nCntParts]->GetCurrentPosition().z;
						m_OldKey[nCntParts].fRotX = m_FileData.ppParts[nCntParts]->GetCurrentRotation().x;
						m_OldKey[nCntParts].fRotY = m_FileData.ppParts[nCntParts]->GetCurrentRotation().y;
						m_OldKey[nCntParts].fRotZ = m_FileData.ppParts[nCntParts]->GetCurrentRotation().z;
					}

					m_nOldType = nNowMotion;
					m_nNowKey = nNextkey;	//次のキーに移動
					m_fNowFrame = 0;
				}
			}
			else
			{//0の場合
				m_nNowKey = 0;
				m_fNowFrame = 0;
			}
		}
	}
}
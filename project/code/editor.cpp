//==========================================================
//
// エディット処理全般 [editor.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "main.h"
#include "input.h"
#include "camera.h"
#include "editor.h"
#include "manager.h"
#include "objectX.h"
#include "Xfile.h"
#include "fileload.h"
#include "debugproc.h"
#include "renderer.h"
#include "texture.h"
#include "meshfield.h"
#include "game.h"
#include "player.h"

//==========================================================
//マクロ定義
//==========================================================
#define EDITMODEL_MOVE	(5.0f)	//モデル移動量
#define MAX_EDITSPEED	(100.0f)	//最大移動量
#define MIN_EDITSPEED	(1.0f)	//最低移動量
#define EDIT_DATATXT	"data/TXT/edit.txt"	//エディットデータ保存用テキストファイル
#define MAX_MESHLENGTH	(500.0f)	// メッシュフィールド判定最大
#define MIN_MESHLENGTH	(20.0f)		// メッシュフィールド判定最小
#define MAX_MESHMOVE	(100.0f)	// メッシュフィールド移動量最大
#define MIN_MESHMOVE	(0.5f)		// メッシュフィールド移動量最小

//==========================================================
// コンストラクタ
//==========================================================
CEditor::CEditor()
{
	// 値をクリア
	m_fSpeed = EDITMODEL_MOVE;
	m_nIdx = 0;
	m_Object.nIdxModel = -1;
	m_Object.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Object.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bUse = false;
	m_bMesh = false;
	m_nMoveCnt = 0;
	m_nNumMove = 0;
	m_nType = TYPE_NONE;
	m_fLength = 0.0f;
	m_fMeshMove = 0.0f;
}

//==========================================================
// デストラクタ
//==========================================================
CEditor::~CEditor()
{

}


//==========================================================
// モデルの初期化処理
//==========================================================
void CEditor::Init(void)
{
	if (CManager::GetInstance()->GetScene()->GetFileLoad() == NULL)
	{
		return;
	}

	m_Object.nIdxModel = CManager::GetInstance()->GetModelFile()->Regist(CManager::GetInstance()->GetScene()->GetFileLoad()->GetModelFileName(m_nIdx));
}

//==========================================================
// 終了処理
//==========================================================
void CEditor::Uninit(void)
{
	
}

//==========================================================
// 更新処理
//==========================================================
void CEditor::Update(void)
{
	if (CManager::GetInstance()->GetScene()->GetFileLoad() == NULL)
	{
		return;
	}

	CCamera *pCamera = CManager::GetInstance()->GetCamera();
	CInputKeyboard *pInputKey = CManager::GetInstance()->GetInputKeyboard();
	CPlayer *pPlayer = CManager::GetInstance()->GetScene()->GetPlayer();
	D3DXVECTOR3 pos = m_Object.pos;
	D3DXVECTOR3 rot = m_Object.rot;
	D3DXVECTOR3 CamRot = pCamera->GetRotation();	// カメラの角度
	CMeshField *pMesh = NULL;	// 先頭を取得

	// 配置場所を指定されたエリアに設定
	if (pPlayer != NULL)
	{
		pMesh = CMeshField::GetArea(pPlayer->GetPosition());
	}

	// メッシュフィールド操作
	if (pMesh != NULL && m_bMesh == true)
	{
		pMesh->Edit(&m_fLength, &m_fMeshMove);

		if (m_fLength > MAX_MESHLENGTH)
		{
			m_fLength = MAX_MESHLENGTH;
		}
		if (m_fLength < MIN_MESHLENGTH)
		{
			m_fLength = MIN_MESHLENGTH;
		}

		if (m_fMeshMove > MAX_MESHMOVE)
		{
			m_fMeshMove = MAX_MESHMOVE;
		}
		if (m_fMeshMove < MIN_MESHMOVE)
		{
			m_fMeshMove = MIN_MESHMOVE;
		}
	}
	
	if (pInputKey->GetTrigger(DIK_F3) == true)
	{
		m_bUse = m_bUse ? false : true;
	}
	if (pInputKey->GetTrigger(DIK_F4) == true)
	{
		m_bMesh = m_bMesh ? false : true;
	}

	CManager::GetInstance()->GetDebugProc()->Print("エディター[ F3 ] : メッシュ操作[ F4 ]\n");

	if (m_bUse == false)
	{// 使用中ではない場合
		return;
	}

	if (pInputKey->GetPress(DIK_LEFT) == true)
	{
		if (pInputKey->GetPress(DIK_UP) == true)
		{
			pos.x += cosf(CamRot.y + (-D3DX_PI * 0.75f)) * m_fSpeed;
			pos.z += sinf(CamRot.y + (-D3DX_PI * 0.75f)) * m_fSpeed;
		}
		else if (pInputKey->GetPress(DIK_DOWN) == true)
		{
			pos.x += cosf(CamRot.y + (-D3DX_PI * 0.25f)) * m_fSpeed;
			pos.z += sinf(CamRot.y + (-D3DX_PI * 0.25f)) * m_fSpeed;
		}
		else
		{
			pos.x += cosf(CamRot.y + (-D3DX_PI * 0.5f)) * m_fSpeed;
			pos.z += sinf(CamRot.y + (-D3DX_PI * 0.5f)) * m_fSpeed;
		}

		m_nMoveCnt++;
	}
	else if (pInputKey->GetPress(DIK_RIGHT) == true)
	{
		if (pInputKey->GetPress(DIK_UP) == true)
		{
			pos.x += cosf(CamRot.y + (D3DX_PI * 0.75f)) * m_fSpeed;
			pos.z += sinf(CamRot.y + (D3DX_PI * 0.75f)) * m_fSpeed;
		}
		else if (pInputKey->GetPress(DIK_DOWN) == true)
		{
			pos.x += cosf(CamRot.y + (D3DX_PI * 0.25f)) * m_fSpeed;
			pos.z += sinf(CamRot.y + (D3DX_PI * 0.25f)) * m_fSpeed;
		}
		else
		{
			pos.x += cosf(CamRot.y + (D3DX_PI * 0.5f)) * m_fSpeed;
			pos.z += sinf(CamRot.y + (D3DX_PI * 0.5f)) * m_fSpeed;
		}

		m_nMoveCnt++;
	}
	else if (pInputKey->GetPress(DIK_UP) == true)
	{
		pos.x += -cosf(CamRot.y) * m_fSpeed;
		pos.z += -sinf(CamRot.y) * m_fSpeed;

		m_nMoveCnt++;
	}
	else if (pInputKey->GetPress(DIK_DOWN) == true)
	{
		pos.x += cosf(CamRot.y) * m_fSpeed;
		pos.z += sinf(CamRot.y) * m_fSpeed;

		m_nMoveCnt++;
	}
	//上下移動
	else if (pInputKey->GetPress(DIK_RSHIFT) == true)
	{
		pos.y += m_fSpeed;

		m_nMoveCnt++;
	}
	else if (pInputKey->GetPress(DIK_RCONTROL) == true)
	{
		pos.y -= m_fSpeed;

		m_nMoveCnt++;
	}
	else
	{
		m_nMoveCnt = m_nNumMove - 1;
	}

	if (m_nMoveCnt > m_nNumMove)
	{
		m_nMoveCnt = 0;
	}
	else
	{
		pos = m_Object.pos;
	}

	//設置
	if (pInputKey->GetTrigger(DIK_RETURN) == true)
	{
		CObjectX::Create(pos, rot, CManager::GetInstance()->GetScene()->GetFileLoad()->GetModelFileName(m_nIdx));
	}

	//設置物変更
	if (pInputKey->GetTrigger(DIK_LSHIFT) == true)
	{
		m_nIdx = (m_nIdx + 1) % CManager::GetInstance()->GetScene()->GetFileLoad()->GetModelNumAll();
		m_Object.nIdxModel = CManager::GetInstance()->GetModelFile()->Regist(CManager::GetInstance()->GetScene()->GetFileLoad()->GetModelFileName(m_nIdx));
	}
	else if (pInputKey->GetTrigger(DIK_LCONTROL) == true)
	{
		m_nIdx = (m_nIdx - 1 + CManager::GetInstance()->GetScene()->GetFileLoad()->GetModelNumAll()) % CManager::GetInstance()->GetScene()->GetFileLoad()->GetModelNumAll();
		m_Object.nIdxModel = CManager::GetInstance()->GetModelFile()->Regist(CManager::GetInstance()->GetScene()->GetFileLoad()->GetModelFileName(m_nIdx));
	}

	//回転
	if (pInputKey->GetTrigger(DIK_P) == true)
	{
		rot.y += D3DX_PI * 0.5f;
	}
	else if (pInputKey->GetTrigger(DIK_L) == true)
	{
		rot.y -= -D3DX_PI * 0.5f;
	}

	if (rot.y < -D3DX_PI)
	{// x座標角度限界
		rot.y += D3DX_PI * 2;
	}
	else if (rot.y > D3DX_PI)
	{// x座標角度限界
		rot.y += -D3DX_PI * 2;
	}

	//保存
	if (pInputKey->GetTrigger(DIK_F9) == true)
	{//F9キー押されたとき
		Save();
	}

	//移動量変更
	if (pInputKey->GetPress(DIK_0) == true)
	{
		m_fSpeed += 1.0f;
		if (m_fSpeed > MAX_EDITSPEED)
		{
			m_fSpeed = MAX_EDITSPEED;
		}
	}
	else if (pInputKey->GetPress(DIK_9) == true)
	{
		m_fSpeed -= 1.0f;
		if (m_fSpeed < MIN_EDITSPEED)
		{
			m_fSpeed = MIN_EDITSPEED;
		}
	}

	// 操作方法変更
	if (pInputKey->GetTrigger(DIK_F4) == true)
	{
		m_nType ^= 1;

		switch (m_nType)
		{
		case TYPE_NONE:
			m_nNumMove = 1;

			break;

		case TYPE_REPEAT:

			m_nNumMove = 10;
			break;
		}
	}

	// 座標確認
	float fHeight = CMeshField::GetHeight(m_Object.pos);

	if (fHeight > m_Object.pos.y)
	{
		m_Object.pos.y = fHeight;
	}

	m_Object.pos = pos;
	m_Object.rot = rot;

	CManager::GetInstance()->GetDebugProc()->Print("\n-------------------------------------------------------\n");
	CManager::GetInstance()->GetDebugProc()->Print("エディット操作方法 ---------------\n");
	CManager::GetInstance()->GetDebugProc()->Print("配置【ENTER】: 移動 【↑,↓,←,→】: 回転【P, I】: 移動方法変更【F4】\n");
	CManager::GetInstance()->GetDebugProc()->Print("保存【F9】\n");
	CManager::GetInstance()->GetDebugProc()->Print("配置物変更【LSHIFT, LCTRL】: 高さ移動【RSHIFT, RCTRL\n移動量変更【9, 0】: 削除範囲変更【7, 8】\n");
	CManager::GetInstance()->GetDebugProc()->Print("座標【%f, %f, %f】: 向き【%f】 : 移動量【%f】\n", pos.x, pos.y, pos.z, rot.y, m_fSpeed);

}

//==========================================================
// モデルの描画処理
//==========================================================
void CEditor::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスへのポインタを取得
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// テクスチャへのポインタ
	CXFile *pModelFile = CManager::GetInstance()->GetModelFile();	// Xファイル情報のポインタ
	CXFile::FileData *pFileData = pModelFile->SetAddress(m_Object.nIdxModel);
	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス
	D3DMATERIAL9 matDef;					//現在のマテリアル保存用
	D3DXMATERIAL *pMat;						//マテリアルデータへのポインタ

	if (m_bUse == false)
	{// 使用中ではない場合
		return;
	}

	if (pFileData != NULL)
	{// モデルが使用されている場合
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_Object.mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Object.rot.y, m_Object.rot.x, m_Object.rot.z);
		D3DXMatrixMultiply(&m_Object.mtxWorld, &m_Object.mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_Object.pos.x, m_Object.pos.y, m_Object.pos.z);
		D3DXMatrixMultiply(&m_Object.mtxWorld, &m_Object.mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_Object.mtxWorld);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)pFileData->pBuffMat->GetBufferPointer();
		for (int nCntMat = 0; nCntMat < (int)pFileData->dwNumMat; nCntMat++)
		{
			D3DXMATERIAL Mat = pMat[nCntMat];

			//色を半透明に変更
			Mat.MatD3D.Diffuse.a = 0.5f;
			Mat.MatD3D.Ambient.a = 0.5f;

			//マテリアルの設定
			pDevice->SetMaterial(&Mat.MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, pTexture->SetAddress(pFileData->pIdexTexture[nCntMat]));

			//モデル(パーツ)の描画
			pFileData->pMesh->DrawSubset(nCntMat);
		}

		//保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//==========================================================
// エディット情報ファイル保存
//==========================================================
void CEditor::Save(void)
{
	FILE *pFile = fopen(EDIT_DATATXT, "w");
	D3DXVECTOR3 ROT;

	if (pFile == NULL)
	{//ファイルが開けなかった場合
		return;
	}

	fprintf(pFile, "#==============================================================================\n"
		"# エディットデータファイル[edit.txt]\n"
		"# Author : IBUKI OKUSADA\n"
		"#==============================================================================\n\n");
	fprintf(pFile, "#----------------------------------------------\n"
		"#モデル情報\n"
		"#----------------------------------------------\n\n");

	for (int nCntPri = 0; nCntPri < NUM_PRIORITY - 2; nCntPri++)
	{
		CObject *pObj = CObject::GetTop(nCntPri);

		while (pObj != NULL)
		{
			CObject *pObjNext = pObj->GetNext();

			if (pObj == NULL)
			{//使用されている場合
				pObj = pObjNext;
				continue;
			}

			if (pObj->GetType() != CObject::TYPE_MODEL)
			{// モデル以外
				pObj = pObjNext;
				continue;
			}

			int nType = pObj->GetModelType();

			if (nType == -1)
			{
				pObj = pObjNext;
				continue;
			}

			// ファイル読み込みモデルの番号取得
			char *pChar = CManager::GetInstance()->GetModelFile()->GetFileName(nType);
			nType = CManager::GetInstance()->GetScene()->GetFileLoad()->GetModelNum(pChar);

			if (nType == -1)
			{
				pObj = pObjNext;
				continue;
			}

			ROT = D3DXVECTOR3(pObj->GetRotation().x / D3DX_PI * 180, pObj->GetRotation().y / D3DX_PI * 180, pObj->GetRotation().z / D3DX_PI * 180);

			fprintf(pFile, "MODELSET\n");
			fprintf(pFile, "	TYPE = %d\n", nType);
			fprintf(pFile, "	POS = %.1f %.1f %.1f\n", pObj->GetPosition().x, pObj->GetPosition().y, pObj->GetPosition().z);
			fprintf(pFile, "	ROT = %.1f %.1f %.1f\n", ROT.x, ROT.y, ROT.z);
			fprintf(pFile, "	SHADOW = 1		#使用しない場合は0\n");
			fprintf(pFile, "END_MODELSET\n\n");

			pObj = pObjNext;
		}
	}

	//ファイルを閉じる
	fclose(pFile);
}
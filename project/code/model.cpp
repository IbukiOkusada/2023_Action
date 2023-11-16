//==========================================================
//
// 階層構造のモデル処理 [model.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "model.h"
#include "Xfile.h"
#include "manager.h"
#include "texture.h"
#include "renderer.h"
#include "slow.h"
#include "meshfield.h"

//==========================================================
// コンストラクタ
//==========================================================
CModel::CModel() : CObject(1)
{
	// 値のクリア
	m_nIdxModel = -1;
	m_bChangeCol = false;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_ChangeMat.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_ChangeMat.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_ChangeMat.Emissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_pParentMtx = NULL;
	m_bDraw = true;
	m_bShadow = true;
	m_pCharacterMtx = nullptr;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
}

//==========================================================
// デストラクタ
//==========================================================
CModel::~CModel()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CModel::Init(void)
{
	return S_OK;
}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CModel::Init(const char *pFileName)
{
	CXFile *pModelFile = CManager::GetInstance()->GetModelFile();	// モデルファイル情報のポインタ

	//Xファイル情報を読み込み
	m_nIdxModel = pModelFile->Regist(pFileName);

	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CModel::Uninit(void)
{
	// ポインタを使用していない状態にする
	m_pParentMtx = NULL;

	Release();
}

//==========================================================
// 更新処理
//==========================================================
void CModel::Update(void)
{

}

//==========================================================
// 描画処理
//==========================================================
void CModel::Draw(void)
{
	if (m_bDraw == false)
	{
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//デバイスへのポインタを取得
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// テクスチャへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	CXFile *pModelFile = CManager::GetInstance()->GetModelFile();	// Xファイル情報のポインタ
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ
	D3DXMATRIX mtxParent;			// 親のマトリックス情報
	CSlow *pSlow = CManager::GetInstance()->GetSlow();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_CurRot.y, m_CurRot.x, m_CurRot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_CurPos.x, m_CurPos.y, m_CurPos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if(m_pParentMtx != NULL)
	{// 覚えている場合
		mtxParent = *m_pParentMtx;

		//パーツのマトリックスと親のマトリックスをかけ合わせる
		D3DXMatrixMultiply(&m_mtxWorld,
			&m_mtxWorld, &mtxParent);
	}

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// モデル情報を取得
	CXFile::FileData *pFileData = pModelFile->SetAddress(m_nIdxModel);

	if (pFileData != NULL)
	{// 使用されている場合
	 //マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)pFileData->pBuffMat->GetBufferPointer();
		for (int nCntMat = 0; nCntMat < (int)pFileData->dwNumMat; nCntMat++)
		{
			int nIdxTex = pFileData->pIdexTexture[nCntMat];	// テクスチャ番号

			if (m_bChangeCol == false)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
			}
			else
			{
				//マテリアルの設定
				if (pSlow->Get() != 1.0f)
				{
					m_ChangeMat.Emissive.r -= 0.3f;
					m_ChangeMat.Emissive.g -= 0.3f;
					m_ChangeMat.Emissive.b -= 0.3f;
					m_ChangeMat.Emissive.a -= 0.3f;
				}
				else
				{
					m_ChangeMat.Emissive = pMat[nCntMat].MatD3D.Emissive;
					m_ChangeMat.Emissive.r += 0.1f;
					m_ChangeMat.Emissive.g += 0.1f;
					m_ChangeMat.Emissive.b += 0.1f;
					m_ChangeMat.Emissive.a += 0.1f;
				}
				pDevice->SetMaterial(&m_ChangeMat);
			}

			//テクスチャの設定
			pDevice->SetTexture(0, pTexture->SetAddress(nIdxTex));

			//モデル(パーツ)の描画
			pFileData->pMesh->DrawSubset(nCntMat);
		}
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	if(m_bShadow == true)
	{
		D3DXMATRIX mtxShadow;
		D3DLIGHT9 light;
		D3DXVECTOR4 posLight;
		D3DXVECTOR3 pos, normal;
		D3DXPLANE plane;

		// ライトの位置を設定
		pDevice->GetLight(0, &light);
		posLight = D3DXVECTOR4(-light.Direction.x, -light.Direction.y, -light.Direction.z, 0.0f);

		// 平面情報を設定
		if (m_mtxWorld._42 >= -296.0f)
		{
			pos = D3DXVECTOR3(0.0f, -296.0f, 0.0f);
			pos.y = CMeshField::GetHeight(D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43)) + 2.0f;
			normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			D3DXPlaneFromPointNormal(&plane, &pos, &normal);

			// シャドウマトリックスの初期化
			D3DXMatrixIdentity(&mtxShadow);

			// シャドウマトリックスの作成
			D3DXMatrixShadow(&mtxShadow, &posLight, &plane);
			D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// モデル情報を取得
			pFileData = pModelFile->SetAddress(m_nIdxModel);

			if (pFileData != NULL)
			{// 使用されている場合
				//マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)pFileData->pBuffMat->GetBufferPointer();
				for (int nCntMat = 0; nCntMat < (int)pFileData->dwNumMat; nCntMat++)
				{

					if (m_bChangeCol == false)
					{
						m_ChangeMat.Emissive.r = 0.0f;
						m_ChangeMat.Emissive.g = 0.0f;
						m_ChangeMat.Emissive.b = 0.0f;
						m_ChangeMat.Emissive.a = 0.7f;
						m_ChangeMat.Diffuse.r = 0.0f;
						m_ChangeMat.Diffuse.g = 0.0f;
						m_ChangeMat.Diffuse.b = 0.0f;
						m_ChangeMat.Diffuse.a = 0.7f;

						//マテリアルの設定
						pDevice->SetMaterial(&m_ChangeMat);
					}
					else
					{

						pDevice->SetMaterial(&m_ChangeMat);
					}

					//テクスチャの設定
					pDevice->SetTexture(0, nullptr);

					//モデル(パーツ)の描画
					pFileData->pMesh->DrawSubset(nCntMat);
				}
			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==========================================================
// 生成
//==========================================================
CModel *CModel::Create(const char *pFileName)
{
	CModel *pModel = NULL;

	// オブジェクトXの生成
	pModel = new CModel;

	if (pModel != NULL)
	{// 生成できた場合

		// 初期化処理
		pModel->Init(pFileName);

	}
	else
	{// 生成に失敗した場合
		return NULL;
	}

	return pModel;
}

//==========================================================
// 親モデルを設定
//==========================================================
void CModel::SetParent(D3DXMATRIX *pMtx)
{
	m_pParentMtx = pMtx;
}

//==========================================================
// 初期位置を設定
//==========================================================
void CModel::SetPosition(const D3DXVECTOR3& pos)
{
	m_pos = pos;

	SetCurrentPosition(m_pos);
}

//==========================================================
// 初期向きを設定
//==========================================================
void CModel::SetRotation(const D3DXVECTOR3& rot)
{
	m_rot = rot;

	SetCurrentRotation(m_rot);
}

//==========================================================
// 現在の位置を設定
//==========================================================
void CModel::SetCurrentPosition(const D3DXVECTOR3& pos)
{
	m_CurPos = pos;
}

//==========================================================
// 現在の向きを設定
//==========================================================
void CModel::SetCurrentRotation(const D3DXVECTOR3& rot)
{
	m_CurRot = rot;
}

//==========================================================
// モデル番号の設定
//==========================================================
void CModel::BindModelFile(int nIdx)
{
	m_nIdxModel = nIdx;
}

//==========================================================
// 当たり判定
//==========================================================
void CModel::SetRotSize(D3DXVECTOR3 &SetMax, D3DXVECTOR3 &SetMin, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin, float fRot)
{
	//向きによって変更する
	if (fRot == 0.0f || fRot == -0.0f)
	{//左向き
	 //最大値反映
		SetMax.x = vtxMax.x;	//x座標
		SetMax.y = vtxMax.y;	//y座標
		SetMax.z = vtxMax.z;	//z座標

								//最小値反映
		SetMin.x = vtxMin.x;	//x座標
		SetMin.y = vtxMin.y;	//y座標
		SetMin.z = vtxMin.z;	//z座標
	}
	if (fRot == D3DX_PI * 0.5f)
	{//左向き
	 //最大値反映
		SetMax.x = vtxMax.z;	//x座標
		SetMax.y = vtxMax.y;	//y座標
		SetMax.z = -vtxMin.x;	//z座標

								//最小値反映
		SetMin.x = vtxMin.z;	//x座標
		SetMin.y = vtxMin.y;	//y座標
		SetMin.z = -vtxMax.x;	//z座標
	}
	else if (fRot == -D3DX_PI * 0.5f)
	{//右向き
	 //最大値反映
		SetMax.x = -vtxMin.z;	//x座標
		SetMax.y = vtxMax.y;	//y座標
		SetMax.z = vtxMax.x;	//z座標

								//最小値反映
		SetMin.x = -vtxMax.z;	//x座標
		SetMin.y = vtxMin.y;	//y座標
		SetMin.z = vtxMin.x;	//z座標
	}
	else if (fRot == -D3DX_PI || fRot == D3DX_PI)
	{//反転している場合
	 //高さ以外の最大値を最小値にする
		SetMax.x = -vtxMin.x;	//x座標
		SetMax.y = vtxMax.y;	//y座標
		SetMax.z = -vtxMin.z;	//z座標

								//高さ以外の最小値を最大値にする
		SetMin.x = -vtxMax.x;	//x座標
		SetMin.y = vtxMin.y;	//y座標
		SetMin.z = -vtxMax.z;	//z座標
	}
	else
	{//それ以外の場合
	 //最大値反映
		SetMax.x = vtxMax.x;	//x座標
		SetMax.y = vtxMax.y;	//y座標
		SetMax.z = vtxMax.z;	//z座標

								//最小値反映
		SetMin.x = vtxMin.x;	//x座標
		SetMin.y = vtxMin.y;	//y座標
		SetMin.z = vtxMin.z;	//z座標
	}
}

//==========================================================
//
// Xファイルモデルの処理全般 [objectX.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "main.h"
#include "objectX.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "Xfile.h"

namespace GOD
{
	const int a = 0;
}

// 静的メンバ変数宣言
CObjectX *CObjectX::m_pTop = NULL;	// 先頭のオブジェクトへのポインタ
CObjectX *CObjectX::m_pCur = NULL;	// 最後尾のオブジェクトへのポインタ

//==========================================================
// コンストラクタ
//==========================================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{
	m_nIdxModel = -1;

	m_pNext = NULL;
	m_pPrev = NULL;

	// 自分自身をリストに追加
	if (m_pTop != NULL)
	{// 先頭が存在している場合
		m_pCur->m_pNext = this;	// 現在最後尾のオブジェクトのポインタにつなげる
		m_pPrev = m_pCur;
		m_pCur = this;	// 自分自身が最後尾になる
	}
	else
	{// 存在しない場合
		m_pTop = this;	// 自分自身が先頭になる
		m_pCur = this;	// 自分自身が最後尾になる
	}
}

//==========================================================
// デストラクタ
//==========================================================
CObjectX::~CObjectX()
{

}

//==========================================================
//モデルの初期化処理
//==========================================================
HRESULT CObjectX::Init(void)
{
	//各種変数の初期化
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	return S_OK;
}

//==========================================================
//モデルの終了処理
//==========================================================
void CObjectX::Uninit(void)
{
	// リストから外す
	ListOut();

	// 廃棄
	Release();
}

//==========================================================
//モデルの更新処理
//==========================================================
void CObjectX::Update(void)
{

}

//==========================================================
//モデルの描画処理
//==========================================================
void CObjectX::Draw(void)
{
	//Update();

	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスへのポインタを取得
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// テクスチャへのポインタ
	CXFile *pModelFile = CManager::GetInstance()->GetModelFile();	// Xファイル情報のポインタ
	CXFile::FileData *pFileData = pModelFile->SetAddress(m_nIdxModel);
	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス
	D3DMATERIAL9 matDef;					//現在のマテリアル保存用
	D3DXMATERIAL *pMat;						//マテリアルデータへのポインタ

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 10);

	if (pFileData != NULL)
	{// モデルが使用されている場合
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)pFileData->pBuffMat->GetBufferPointer();
		for (int nCntMat = 0; nCntMat < (int)pFileData->dwNumMat; nCntMat++)
		{
			D3DMATERIAL9 mat = pMat[nCntMat].MatD3D;

			//マテリアルの設定
			pDevice->SetMaterial(&mat);

			//テクスチャの設定
			pDevice->SetTexture(0, pTexture->SetAddress(pFileData->pIdexTexture[nCntMat]));

			//モデル(パーツ)の描画
			pFileData->pMesh->DrawSubset(nCntMat);
		}

		//保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);
}

//==========================================================
// 生成
//==========================================================
CObjectX *CObjectX::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char *pFileName, const int nPriority)
{
	CObjectX *pObjectX = NULL;
	CXFile *pModelFile = CManager::GetInstance()->GetModelFile();

	// オブジェクトXの生成
	pObjectX = new CObjectX(nPriority);

	if (pObjectX != NULL)
	{// 生成できた場合
		// 初期化処理
		pObjectX->Init();

		// 座標
		pObjectX->SetPosition(pos);

		// 向き
		pObjectX->SetRotation(rot);

		// 種類設定
		pObjectX->SetType(TYPE_MODEL);

		// 読み込み確認
		pObjectX->BindFile(pModelFile->Regist(pFileName));
	}
	else
	{// 生成に失敗した場合
		return NULL;
	}

	return pObjectX;
}

//==========================================================
// モデルのファイル設定
//==========================================================
void CObjectX::BindFile(int nIdx)
{
	m_nIdxModel = nIdx;	//使用するモデルの設定
}

//==========================================================
// 座標の設定
//==========================================================
void CObjectX::SetPosition(const D3DXVECTOR3& pos)
{
	m_pos = pos; 
}

//==========================================================
// 向きの設定
//==========================================================
void CObjectX::SetRotation(const D3DXVECTOR3& rot)
{ 
	m_rot = rot;

	if (m_rot.z < -D3DX_PI)
	{// z座標角度限界
		m_rot.z += D3DX_PI * 2;
	}
	else if (m_rot.z > D3DX_PI)
	{// z座標角度限界
		m_rot.z += -D3DX_PI * 2;
	}

	if (m_rot.x < -D3DX_PI)
	{// x座標角度限界
		m_rot.x += D3DX_PI * 2;
	}
	else if (m_rot.x > D3DX_PI)
	{// x座標角度限界
		m_rot.x += -D3DX_PI * 2;
	}

	if (m_rot.y < -D3DX_PI)
	{// x座標角度限界
		m_rot.y += D3DX_PI * 2;
	}
	else if (m_rot.y > D3DX_PI)
	{// x座標角度限界
		m_rot.y += -D3DX_PI * 2;
	}
}

//==========================================================
// 当たり判定
//==========================================================
bool CObjectX::Collision(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, float fRefMulti)
{
	CObjectX *pObj = m_pTop;	// 先頭取得
	bool bLand = false;	// 着地したか否か

	while (pObj != NULL)
	{
		CObjectX *pObjNext = pObj->m_pNext;
		D3DXVECTOR3 vtxObjMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vtxObjMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		if (pObj->CollisionCheck(pos, posOld, move, vtxMin, vtxMax, fRefMulti))
		{
			bLand = true;
		}

		pObj = pObjNext;
	}

	return bLand;
}

//==========================================================
// 当たり判定
//==========================================================
void CObjectX::SetRotSize(D3DXVECTOR3 &SetMax, D3DXVECTOR3 &SetMin, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin, float fRot)
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

//==========================================================
// 個別判定チェック
//==========================================================
bool CObjectX::CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, const float fRefMulti)
{
	CXFile *pFile = CManager::GetInstance()->GetModelFile();
	bool bLand = false;	// 着地したか否か
	D3DXVECTOR3 vtxObjMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxObjMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// 向きを反映
	SetRotSize(vtxObjMax,
		vtxObjMin,
		pFile->GetMax(GetIdx()),
		pFile->GetMin(GetIdx()),
		m_rot.y);

	if (pos.y + vtxMax.y > m_pos.y + vtxObjMin.y
		&& pos.y + vtxMin.y <= m_pos.y + vtxObjMax.y)
	{//プレイヤーとモデルが同じ高さにある
		if (posOld.x + vtxMin.x >= m_pos.x + vtxObjMax.x
			&& pos.x + vtxMin.x < m_pos.x + vtxObjMax.x
			&& pos.z + vtxMax.z > m_pos.z + vtxObjMin.z
			&& pos.z + vtxMin.z < m_pos.z + vtxObjMax.z)
		{//右から左にめり込んだ
			bLand = true;
			move.x *= -1.0f;
			move.x *= fRefMulti;
			pos.x = m_pos.x + vtxObjMax.x - vtxMin.x + 0.1f + move.x;
		}
		else if (posOld.x + vtxMax.x <= m_pos.x + vtxObjMin.x
			&& pos.x + vtxMax.x > m_pos.x + vtxObjMin.x
			&& pos.z + vtxMax.z > m_pos.z + vtxObjMin.z
			&& pos.z + vtxMin.z < m_pos.z + vtxObjMax.z)
		{//左から右にめり込んだ
		 //位置を戻す
			bLand = true;
			move.x *= -1.0f;
			move.x *= fRefMulti;
			pos.x = m_pos.x + vtxObjMin.x - vtxMax.x - 0.1f + move.x;
			//move.x = 0.0f;
		}
		else if (posOld.z + vtxMin.z >= m_pos.z + vtxObjMax.z
			&& pos.z + vtxMin.z < m_pos.z + vtxObjMax.z
			&& pos.x + vtxMax.x > m_pos.x + vtxObjMin.x
			&& pos.x + vtxMin.x < m_pos.x + vtxObjMax.x)
		{//奥から手前にめり込んだ
		 //位置を戻す
			bLand = true;
			move.z *= -1.0f;
			move.z *= fRefMulti;
			pos.z = m_pos.z + vtxObjMax.z - vtxMin.z + 0.1f + move.z;
			//move.z = 0.0f;
		}
		else if (posOld.z + vtxMax.z <= m_pos.z + vtxObjMin.z
			&& pos.z + vtxMax.z > m_pos.z + vtxObjMin.z
			&& pos.x + vtxMax.x > m_pos.x + vtxObjMin.x
			&& pos.x + vtxMin.x < m_pos.x + vtxObjMax.x)
		{//手前から奥にめり込んだt
		 //位置を戻す
			bLand = true;
			move.z *= -1.0f;
			move.z *= fRefMulti;
			pos.z = m_pos.z + vtxObjMin.z - vtxMax.z - 0.1f + move.z;
			//move.z = 0.0f;
		}
	}

	if (pos.x + vtxMax.x > m_pos.x + vtxObjMin.x
		&& pos.x + vtxMin.x < m_pos.x + vtxObjMax.x
		&& pos.z + vtxMax.z > m_pos.z + vtxObjMin.z
		&& pos.z + vtxMin.z < m_pos.z + vtxObjMax.z)
	{//範囲内にある
		//上からの判定
		if (posOld.y + vtxMin.y >= m_pos.y + vtxObjMax.y
			&& pos.y + vtxMin.y < m_pos.y + vtxObjMax.y)
		{//上からめり込んだ
			//上にのせる
			pos.y = m_pos.y + vtxObjMax.y - vtxMin.y;
			move.y = 0.0f;
			bLand = true;
		}
	}

	return bLand;
}

//==========================================================
// リストから外す
//==========================================================
void CObjectX::ListOut(void)
{
	// リストから自分自身を削除する
	if (m_pTop == this)
	{// 自身が先頭
		if (m_pNext != NULL)
		{// 次が存在している
			m_pTop = m_pNext;	// 次を先頭にする
			m_pNext->m_pPrev = NULL;	// 次の前のポインタを覚えていないようにする
		}
		else
		{// 存在していない
			m_pTop = NULL;	// 先頭がない状態にする
			m_pCur = NULL;	// 最後尾がない状態にする
		}
	}
	else if (m_pCur == this)
	{// 自身が最後尾
		if (m_pPrev != NULL)
		{// 次が存在している
			m_pCur = m_pPrev;			// 前を最後尾にする
			m_pPrev->m_pNext = NULL;	// 前の次のポインタを覚えていないようにする
		}
		else
		{// 存在していない
			m_pTop = NULL;	// 先頭がない状態にする
			m_pCur = NULL;	// 最後尾がない状態にする
		}
	}
	else
	{
		if (m_pNext != NULL)
		{
			m_pNext->m_pPrev = m_pPrev;	// 自身の次に前のポインタを覚えさせる
		}
		if (m_pPrev != NULL)
		{
			m_pPrev->m_pNext = m_pNext;	// 自身の前に次のポインタを覚えさせる
		}
	}
}
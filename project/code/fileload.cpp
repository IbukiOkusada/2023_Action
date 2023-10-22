//==========================================================
//
//ファイル、データ読み込み処理
//Author 奥定伊吹
//
//==========================================================
#include "fileload.h"
#include <string.h>
#include "texture.h"
#include "meshfield.h"
#include "meshwall.h"
#include "manager.h"
#include "texture.h"
#include "Xfile.h"
#include "objectX.h"
#include "game.h"

//==========================================================
// マクロ定義
//==========================================================
//確認文字
#define START_SCRIPTTXT	"SCRIPT"				// スクリプト開始確認文字
#define END_SCRIPTTXT	"END_SCRIPT"			// スクリプト終了確認文字
#define TEXTURENUM_TXT	"NUM_TEXTURE"			// テクスチャ使用枚数確認文字
#define TEXTUREFILENAME_TXT	"TEXTURE_FILENAME"	// テクスチャファイル名確認文字
#define MODELNUM_TXT	"NUM_MODEL"				// モデル使用確認文字
#define MODELFILENAME_TXT	"MODEL_FILENAME"	// モデルファイル名確認文字
#define FIELDSET_TXT	"FIELDSET"				// メッシュフィールド設置確認文字
#define ENDFIELDSET_TXT	"END_FIELDSET"			// 床読み込み終了
#define WALLSET_TXT		"WALLSET"				// メッシュウォール設置確認文字
#define ENDWALLSET_TXT	"END_WALLSET"			// 壁読み込み終了
#define MODELSET_TXT	"MODELSET"				// モデル配置確認文字
#define ENDMODELSET_TXT	"END_MODELSET"			// モデル読み込み終了
#define LOAD_POS		"POS"					// 座標
#define LOAD_ROT		"ROT"					// 向き
#define LOAD_TEXTYPE	"TEXTYPE"				// テクスチャ番号
#define LOAD_MODELTYPE	"TYPE"					// モデル番号
#define LOAD_BLOCK		"BLOCK"					// 枚数
#define LOAD_SIZE		"SIZE"					// サイズ
#define LOAD_SHADOW		"SHADOW"				// 影
#define LOAD_UPDOWN		"UPDOWN"				// 起伏
#define LOAD_VTXMAX		"VTXMAX"				// 当たり判定最大
#define LOAD_VTXMIN		"VTXMIN"				// 当たり判定最小

//==========================================================
// コンストラクタ
//==========================================================
CFileLoad::CFileLoad()
{
	m_nModelNumAll = 0;
	m_nTexNumAll = 0;
	m_pTextureFile = NULL;
	m_pModelFile = NULL;
}

//==========================================================
// デストラクタ
//==========================================================
CFileLoad::~CFileLoad()
{

}

//==========================================================
// 初期化処理
//==========================================================
void CFileLoad::Init(void)
{

}

//==========================================================
// 終了処理
//==========================================================
void CFileLoad::Uninit(void)
{

}

//==========================================================
// ファイルを開く
//==========================================================
void CFileLoad::OpenFile(const char *pFileName)
{
	FILE *pFile;	// ファイルへのポインタ

	pFile = fopen(pFileName, "r");

	if (pFile != NULL)
	{//ファイルが開けた場合
		char aStr[256];

		//開始文字まで読み込む
		while (1)
		{
			//テキスト読み込み
			int nResult = fscanf(pFile, "%s", &aStr[0]);

			if (strcmp(&aStr[0], START_SCRIPTTXT) == 0)
			{//スクリプト開始の文字が確認できた場合
				//各データの読み込み開始
				LoadFileData(pFile);

				break;
			}
			else if (nResult == EOF)
			{//ファイルの最後まで読み込んでしまった場合
				break;
			}
		}
		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		return;
	}
}

//==========================================================
//ファイル内容読み込み処理
//==========================================================
void CFileLoad::LoadFileData(FILE *pFile)
{
	char aStr[256];	//余分な文章読み込み用
	int nModelIdx = -1;	// 当たり判定確認場所

	while (1)
	{
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		//読み込み内容確認
		if (strcmp(&aStr[0], TEXTURENUM_TXT) == 0)
		{//使用するテクスチャ枚数の場合
			LoadTexNum(pFile);
		}
		else if (strcmp(&aStr[0], MODELNUM_TXT) == 0)
		{//使用するモデル数の場合
			LoadModelNum(pFile);
		}

		//ファイル名確認
		if (strcmp(&aStr[0], TEXTUREFILENAME_TXT) == 0)
		{//テクスチャファイル名の場合
			LoadTexFile(pFile);
		}
		else if (strcmp(&aStr[0], MODELFILENAME_TXT) == 0)
		{//モデルファイル名の場合
			LoadModelFile(pFile);
			nModelIdx++;
		}

		// サイズ確認
		if (strcmp(&aStr[0], LOAD_VTXMAX) == 0)
		{//テクスチャファイル名の場合
			LoadVtxMaxData(pFile, nModelIdx);
		}
		else if (strcmp(&aStr[0], LOAD_VTXMIN) == 0)
		{//モデルファイル名の場合
			LoadVtxMinData(pFile, nModelIdx);
		}

		//配置情報確認
		if (strcmp(&aStr[0], WALLSET_TXT) == 0)
		{//壁配置の場合
			LoadMeshWallData(pFile);
		}
		else if (strcmp(&aStr[0], FIELDSET_TXT) == 0)
		{//床配置の場合
			LoadMeshFieldData(pFile);
		}
		else if (strcmp(&aStr[0], MODELSET_TXT) == 0)
		{//モデル配置の場合
			LoadModelData(pFile);
		}

		//終了確認
		if (strcmp(&aStr[0], END_SCRIPTTXT) == 0)
		{//スクリプト開始の文字が確認できた場合
			break;
		}
		else if (nResult == EOF)
		{//ファイルの最後まで読み込んでしまった場合
			break;
		}
	}
}

//==========================================================
// テクスチャ枚数読み込み
//==========================================================
void CFileLoad::LoadTexNum(FILE *pFile)
{
	char aStr[4];	//余分な文章読み込み用

	fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
	fscanf(pFile, "%d", &m_nTexNumAll);	//使用枚数読み込み

	if (m_nTexNumAll > 0)
	{// 読み込む場合
		if (nullptr == m_pTextureFile)
		{// 使用していない場合
			m_pTextureFile = new File[m_nTexNumAll];
			memset(m_pTextureFile, NULL, sizeof(File) * m_nTexNumAll);
		}
	}
}

//==========================================================
// モデル数読み込み
//==========================================================
void CFileLoad::LoadModelNum(FILE *pFile)
{
	char aStr[4];	//余分な文章読み込み用

	fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
	fscanf(pFile, "%d", &m_nModelNumAll);	//使用枚数読み込み

	if (m_nModelNumAll > 0)
	{// 読み込む場合
		if (nullptr == m_pModelFile)
		{// 使用していない場合
			m_pModelFile = new File[m_nModelNumAll];
			memset(m_pModelFile, NULL, sizeof(File) * m_nModelNumAll);
		}
	}
}

//==========================================================
// テクスチャ読み込み
//==========================================================
void CFileLoad::LoadTexFile(FILE *pFile)
{
	char aStr[4];	//余分な文章読み込み用
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み

	if (m_pTextureFile != NULL)
	{// 使用されている場合
		for (int nCntTexture = 0; nCntTexture < m_nTexNumAll; nCntTexture++)
		{
			if (m_pTextureFile[nCntTexture].aFileName[0] == '\0')
			{//ファイル名を入れる場所が使われていない場合
				fscanf(pFile, "%s", &m_pTextureFile[nCntTexture].aFileName[0]);	//(=)読み込み
																				// ファイル読み込み確認
				m_pTextureFile[nCntTexture].nIdx = pTexture->Regist(&m_pTextureFile[nCntTexture].aFileName[0]);
				break;
			}
		}
	}
}

//==========================================================
// モデル読み込み
//==========================================================
void CFileLoad::LoadModelFile(FILE *pFile)
{
	char aStr[4];	//余分な文章読み込み用
	CXFile *pModel = CManager::GetInstance()->GetModelFile();

	fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み

	if (m_pModelFile != NULL)
	{// 使用されている場合
		for (int nCntModel = 0; nCntModel < m_nModelNumAll; nCntModel++)
		{
			if (m_pModelFile[nCntModel].aFileName[0] == '\0')
			{//ファイル名を入れる場所が使われていない場合
				fscanf(pFile, "%s", &m_pModelFile[nCntModel].aFileName[0]);	//(=)読み込み
																			// ファイル読み込み確認
				m_pModelFile[nCntModel].nIdx = pModel->Regist(&m_pModelFile[nCntModel].aFileName[0]);
				break;
			}
		}
	}
}

//==========================================================
// テクスチャのファイル名取得
//==========================================================
char *CFileLoad::GetTextureFileName(int nCntTexture)
{
	if (nCntTexture >= 0 && nCntTexture < m_nTexNumAll)
	{// 範囲内
		if (m_pTextureFile != NULL)
		{// 使用されている場合
			return &m_pTextureFile[nCntTexture].aFileName[0];
		}
	}

	return NULL;
}

//==========================================================
// モデルのファイル名取得
//==========================================================
char *CFileLoad::GetModelFileName(int nCntModel)
{
	if (nCntModel >= 0 && nCntModel < m_nModelNumAll)
	{// 範囲内
		if (m_pModelFile != NULL)
		{// 使用されている場合
			return &m_pModelFile[nCntModel].aFileName[0];
		}
	}

	return NULL;
}

//==========================================================
// メッシュウォール配置情報取得
//==========================================================
void CFileLoad::LoadMeshWallData(FILE *pFile)
{
	char aStr[256];	//余分な文章読み込み用

	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int nWidth = 0;			// 幅枚数
	int nHeight = 0;		// 高さ枚数
	float fWidth = 0.0f;	// 幅
	float fHeight = 0.0f;	// 高さ
	int nIdx = -1;

	//終了文字まで読み込み
	while (1)
	{
		fscanf(pFile, "%s", &aStr[0]);

		//配置情報確認
		if (strcmp(&aStr[0], LOAD_TEXTYPE) == 0)
		{//テクスチャ
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%d", &nIdx);	//テクスチャ名読み込み
		}
		else if (strcmp(&aStr[0], LOAD_POS) == 0)
		{//座標
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%f", &pos.x);	//x座標読み込み
			fscanf(pFile, "%f", &pos.y);	//y座標読み込み
			fscanf(pFile, "%f", &pos.z);	//z座標読み込み
		}
		else if (strcmp(&aStr[0], LOAD_ROT) == 0)
		{//向き
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%f", &rot.x);	//x座標読み込み
			fscanf(pFile, "%f", &rot.y);	//y座標読み込み
			fscanf(pFile, "%f", &rot.z);	//z座標読み込み
		}
		else if (strcmp(&aStr[0], LOAD_BLOCK) == 0)
		{//枚数
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%d", &nHeight);	//縦幅読み込み
			fscanf(pFile, "%d", &nWidth);		//横幅読み込み

		}
		else if (strcmp(&aStr[0], LOAD_SIZE) == 0)
		{//サイズ
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%f", &fHeight);//縦幅読み込み
			fscanf(pFile, "%f", &fWidth);	//横幅読み込み
		}

		//終了
		if (strcmp(&aStr[0], ENDWALLSET_TXT) == 0)
		{//終了文字
			break;
		}
	}

	//フィールドの配置
	CMeshWall::Create(pos, D3DXToRadian(rot), fWidth, fHeight, GetTextureFileName(nIdx), nWidth, nHeight);
}

//==========================================================
// メッシュフィールド配置情報取得
//==========================================================
void CFileLoad::LoadMeshFieldData(FILE *pFile)
{
	char aStr[256];	//余分な文章読み込み用
	char aUpDown[256] = {};

	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR2 move = D3DXVECTOR2(0.0f, 0.0f);
	int nWidth = 0;			// 幅枚数
	int nHeight = 0;		// 高さ枚数
	float fWidth = 0.0f;	// 幅
	float fHeight = 0.0f;	// 高さ
	int nIdx = -1;

	//終了文字まで読み込み
	while (1)
	{
		fscanf(pFile, "%s", &aStr[0]);

		//配置情報確認
		if (strcmp(&aStr[0], LOAD_TEXTYPE) == 0)
		{//テクスチャ
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%d", &nIdx);	//テクスチャ名読み込み
		}
		else if (strcmp(&aStr[0], LOAD_POS) == 0)
		{//座標
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%f", &pos.x);	//x座標読み込み
			fscanf(pFile, "%f", &pos.y);	//y座標読み込み
			fscanf(pFile, "%f", &pos.z);	//z座標読み込み
		}
		else if (strcmp(&aStr[0], LOAD_ROT) == 0)
		{//向き
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%f", &rot.x);	//x座標読み込み
			fscanf(pFile, "%f", &rot.y);	//y座標読み込み
			fscanf(pFile, "%f", &rot.z);	//z座標読み込み
		}
		else if (strcmp(&aStr[0], LOAD_BLOCK) == 0)
		{//枚数
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%d", &nHeight);	//縦幅読み込み
			fscanf(pFile, "%d", &nWidth);		//横幅読み込み

		}
		else if (strcmp(&aStr[0], LOAD_SIZE) == 0)
		{//サイズ
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%f", &fHeight);  //縦幅読み込み
			fscanf(pFile, "%f", &fWidth);	//横幅読み込み
		}
		else if (strcmp(&aStr[0], LOAD_UPDOWN) == 0)
		{//サイズ
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%s", &aUpDown[0]);  //縦幅読み込み
		}
		else if (strcmp(&aStr[0], "MOVE") == 0)
		{//サイズ
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%f", &move.x);  //縦読み込み
			fscanf(pFile, "%f", &move.y);	//横読み込み
		}

		//終了
		if (strcmp(&aStr[0], ENDFIELDSET_TXT) == 0)
		{//終了文字
			break;
		}
	}

	//フィールドの配置
	CMeshField *pMesh = CMeshField::Create(pos, D3DXToRadian(rot), fWidth, fHeight, GetTextureFileName(nIdx), nWidth, nHeight);
	
	if (pMesh)
	{
		pMesh->SetTexMove(move);
	}

	if (aUpDown[0] != '\0')
	{
		pMesh->UpDownLoad(&aUpDown[0]);
	}
}

//==========================================================
// モデル配置情報取得
//==========================================================
void CFileLoad::LoadModelData(FILE *pFile)
{
	char aStr[256];	//余分な文章読み込み用

	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int nWidth = 0;			// 幅枚数
	int nHeight = 0;		// 高さ枚数
	float fWidth = 0.0f;	// 幅
	float fHeight = 0.0f;	// 高さ
	int nIdx = -1;

	//終了文字まで読み込み
	while (1)
	{
		fscanf(pFile, "%s", &aStr[0]);

		//配置情報確認
		if (strcmp(&aStr[0], LOAD_MODELTYPE) == 0)
		{//テクスチャ
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%d", &nIdx);	//テクスチャ名読み込み
		}
		else if (strcmp(&aStr[0], LOAD_POS) == 0)
		{//座標
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%f", &pos.x);	//x座標読み込み
			fscanf(pFile, "%f", &pos.y);	//y座標読み込み
			fscanf(pFile, "%f", &pos.z);	//z座標読み込み
		}
		else if (strcmp(&aStr[0], LOAD_ROT) == 0)
		{//向き
			fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
			fscanf(pFile, "%f", &rot.x);	//x座標読み込み
			fscanf(pFile, "%f", &rot.y);	//y座標読み込み
			fscanf(pFile, "%f", &rot.z);	//z座標読み込み
		}

		//終了
		if (strcmp(&aStr[0], ENDMODELSET_TXT) == 0)
		{//終了文字
			break;
		}
	}

	//フィールドの配置
	CObjectX::Create(pos, D3DXToRadian(rot), GetModelFileName(nIdx));
}

//==========================================================
// メッシュドーム配置情報取得
//==========================================================
void CFileLoad::LoadDomeData(FILE *pFile)
{

}

//==========================================================
// メッシュシリンダー配置情報取得
//==========================================================
void CFileLoad::LoadCylinderData(FILE *pFile)
{

}

//==========================================================
// 指定されたファイル名の№取得
//==========================================================
int CFileLoad::GetTextureNum(const char *pFileName)
{
	int nIdx = -1;
	// 読み込まれているか確認
	for (int nCnt = 0; nCnt < m_nTexNumAll; nCnt++)
	{
		if (nullptr == m_pTextureFile)
		{// 使われていない場合
			continue;
		}

		// 同じファイルか確認
		if (strstr(pFileName, &m_pTextureFile[nCnt].aFileName[0]) != NULL)
		{// 同じものが存在している場合
			nIdx = nCnt;
			return nIdx;	// ファイル情報を返す
		}
	}

	return nIdx;
}

//==========================================================
// 指定されたファイル名の№取得
//==========================================================
int CFileLoad::GetModelNum(const char *pFileName)
{
	int nIdx = -1;
	// 読み込まれているか確認
	for (int nCnt = 0; nCnt < m_nModelNumAll; nCnt++)
	{
		if (nullptr == m_pModelFile)
		{// 使われていない場合
			continue;
		}

		// 同じファイルか確認
		if (strstr(pFileName, &m_pModelFile[nCnt].aFileName[0]) != NULL)
		{// 同じものが存在している場合
			nIdx = nCnt;
			return nIdx;	// ファイル情報を返す
		}
	}

	return nIdx;
}

//==========================================================
// 成功
//==========================================================
void CFileLoad::LoadVtxMaxData(FILE *pFile, int nIdx)
{
	char aStr[256];	//余分な文章読み込み用
	D3DXVECTOR3 VtxMax = CManager::GetInstance()->GetModelFile()->GetMax(nIdx);

	fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
	fscanf(pFile, "%f", &VtxMax.x);	//x座標読み込み
	fscanf(pFile, "%f", &VtxMax.y);	//y座標読み込み
	fscanf(pFile, "%f", &VtxMax.z);	//z座標読み込み

	CManager::GetInstance()->GetModelFile()->SetSizeVtxMax(nIdx, VtxMax);
}

//==========================================================
// 成功
//==========================================================
void CFileLoad::LoadVtxMinData(FILE *pFile, int nIdx)
{
	char aStr[256];	//余分な文章読み込み用
	D3DXVECTOR3 VtxMin = CManager::GetInstance()->GetModelFile()->GetMin(nIdx);

	fscanf(pFile, "%s", &aStr[0]);	//(=)読み込み
	fscanf(pFile, "%f", &VtxMin.x);	//x座標読み込み
	fscanf(pFile, "%f", &VtxMin.y);	//y座標読み込み
	fscanf(pFile, "%f", &VtxMin.z);	//z座標読み込み

	CManager::GetInstance()->GetModelFile()->SetSizeVtxMin(nIdx, VtxMin);
}

void LoadGimmickData(FILE *pFile, int nIdx)
{

}
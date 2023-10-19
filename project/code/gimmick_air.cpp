//==========================================================
//
// ���M�~�b�N [gimmick_button.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "gimmick_air.h"
#include "manager.h"
#include "slow.h"
#include "Xfile.h"
#include "objectX.h"
#include "debugproc.h"

// �}�N����`

//==========================================================
// �R���X�g���N�^
//==========================================================
CGimmickAir::CGimmickAir()
{
	m_pObject = NULL;
	m_bRevease = false;
	m_fHeight = 0.0f; 
	m_fWidth = 0.0f;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CGimmickAir::~CGimmickAir()
{

}

//==========================================================
// ����������
//==========================================================
HRESULT CGimmickAir::Init(void)
{
	// �ǂݍ��݊m�F
	if (m_pObject == NULL)
	{
		
	}

	return S_OK;
}

//==========================================================
// �I������
//==========================================================
void CGimmickAir::Uninit(void)
{
	if (m_pObject != NULL)
	{
		m_pObject->Uninit();
		m_pObject = NULL;
	}

	ListOut();

	Release();
}

//==========================================================
// �X�V����
//==========================================================
void CGimmickAir::Update(void)
{
	// ����֘A
	Controller();

	SetMtxWorld();
}

//==========================================================
// ����
//==========================================================
CGimmickAir *CGimmickAir::Create(void)
{
	CGimmickAir *pObjectAir = new CGimmickAir;

	if (pObjectAir != NULL)
	{
		// ����������
		pObjectAir->Init();
	}

	return pObjectAir;
}

//==========================================================
// ����֘A
//==========================================================
void CGimmickAir::Controller(void)
{
	
}

//==========================================================
// �ʔ���`�F�b�N
//==========================================================
bool CGimmickAir::CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, int &nDamage, const float fRefMulti)
{
	CXFile *pFile = CManager::GetInstance()->GetModelFile();
	bool bLand = false;	// ���n�������ۂ�
	D3DXVECTOR3 vtxObjMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxObjMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 ObjPos = GetPosition();
	D3DXVECTOR3 ObjRot = GetRotation();

	if (pos.x >= ObjPos.x - m_fWidth &&
		pos.x <= ObjPos.x + m_fWidth &&
		pos.z >= ObjPos.z - m_fHeight &&
		pos.z <= ObjPos.z + m_fHeight)
	{
		CManager::GetInstance()->GetDebugProc()->Print("���ɓ���������[\n");
		// �͈͓�
		move += m_move;
		nDamage = -1;
	}

	return bLand;
}

void CGimmickAir::Reverse(void)
{
	if (!m_bRevease){
		m_move *= -1.0f;
	}
}
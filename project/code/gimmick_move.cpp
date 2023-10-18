//==========================================================
//
// �ړ��I�u�W�F�N�g [objectmove.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "gimmick_move.h"
#include "manager.h"
#include "slow.h"
#include "Xfile.h"
#include "objectX.h"

// �}�N����`

//==========================================================
// �R���X�g���N�^
//==========================================================
CGimmickMove::CGimmickMove()
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fNowMoveTimer = 0.0f;
	m_fNumMoveTimer = 0.0f;
	m_pObject = NULL;
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CGimmickMove::~CGimmickMove()
{

}

//==========================================================
// ����������
//==========================================================
HRESULT CGimmickMove::Init(void)
{

	// �ǂݍ��݊m�F
	if(m_pObject == NULL)
	{
		m_pObject = CObjectX::Create(GetPosition(), GetRotation(), "data\\MODEL\\5mcube.x", 3);
	}

	// �X���[���o����
	m_pSlow = CManager::GetInstance()->GetSlow();

	return S_OK;
}

//==========================================================
// �I������
//==========================================================
void CGimmickMove::Uninit(void)
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
void CGimmickMove::Update(void)
{
	m_posOld = GetPosition();

	// ����֘A
	Controller();

	SetMtxWorld();
}

//==========================================================
// �`�揈��
//==========================================================
void CGimmickMove::Draw(void)
{

}

//==========================================================
// ����
//==========================================================
CGimmickMove *CGimmickMove::Create(void)
{
	CGimmickMove *pObjectMove = new CGimmickMove;

	if (pObjectMove != NULL)
	{
		// ����������
		pObjectMove->Init();
	}

	return pObjectMove;
}

//==========================================================
// ����
//==========================================================
CGimmickMove *CGimmickMove::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fTimer)
{
	CGimmickMove *pObjectMove = new CGimmickMove;

	if (pObjectMove != NULL)
	{
		// ����������
		pObjectMove->Init();

		// �ݒ�
		pObjectMove->SetPosition(pos);
		pObjectMove->SetMove(move);
		pObjectMove->SetNumTimer(fTimer);
		pObjectMove->SetNowTimer(fTimer);
	}

	return pObjectMove;
}

//==========================================================
// ����֘A
//==========================================================
void CGimmickMove::Controller(void)
{
	// ���W�X�V
	D3DXVECTOR3 pos = GetPosition();
	float m_fSlowMulti = 1.0f;	// �X���[�{��

	if (!m_pSlow)
	{
		m_fSlowMulti = m_pSlow->Get();
	}

	pos += m_move;
	SetPosition(pos);

	if (m_fNumMoveTimer == 0.0f)
	{// �������Ȃ�
		return;
	}

	m_fNowMoveTimer -= m_fSlowMulti;

	if (m_fNowMoveTimer <= 0.0f)
	{
		m_move *= -1.0f;
		m_fNowMoveTimer = m_fNumMoveTimer;
	}

	if(m_pObject != NULL)
	{
		m_pObject->SetPosition(GetPosition());
		m_pObject->SetRotation(GetRotation());
	}
}

//==========================================================
// �ʔ���`�F�b�N
//==========================================================
bool CGimmickMove::CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, const float fRefMulti)
{
	CXFile *pFile = CManager::GetInstance()->GetModelFile();
	bool bLand = false;	// ���n�������ۂ�
	D3DXVECTOR3 vtxObjMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxObjMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 ObjPos = GetPosition();
	D3DXVECTOR3 ObjRot = GetRotation();

	if (m_pObject == NULL)
	{
		return bLand;
	}

	// �����𔽉f
	m_pObject->SetRotSize(vtxObjMax,
		vtxObjMin,
		pFile->GetMax(m_pObject->GetIdx()),
		pFile->GetMin(m_pObject->GetIdx()),
		ObjRot.y);

	if (pos.y + vtxMax.y > ObjPos.y + vtxObjMin.y
		&& pos.y + vtxMin.y <= ObjPos.y + vtxObjMax.y)
	{//�v���C���[�ƃ��f�������������ɂ���
		if (posOld.x + vtxMin.x >= m_posOld.x + vtxObjMax.x
			&& pos.x + vtxMin.x < ObjPos.x + vtxObjMax.x
			&& pos.z + vtxMax.z > ObjPos.z + vtxObjMin.z
			&& pos.z + vtxMin.z < ObjPos.z + vtxObjMax.z)
		{//�E���獶�ɂ߂荞��
			bLand = true;
			move.x *= -1.0f;
			//move.x *= fRefMulti;
			pos.x = ObjPos.x + vtxObjMax.x - vtxMin.x + 0.1f + m_move.x;
		}
		else if (posOld.x + vtxMax.x <= m_posOld.x + vtxObjMin.x
			&& pos.x + vtxMax.x > ObjPos.x + vtxObjMin.x
			&& pos.z + vtxMax.z > ObjPos.z + vtxObjMin.z
			&& pos.z + vtxMin.z < ObjPos.z + vtxObjMax.z)
		{//������E�ɂ߂荞��
		 //�ʒu��߂�
			bLand = true;
			move.x *= -1.0f;
			//move.x *= fRefMulti;
			pos.x = ObjPos.x + vtxObjMin.x - vtxMax.x - 0.1f + m_move.x;
			//move.x = 0.0f;
		}
		else if (posOld.z + vtxMin.z >= m_posOld.z + vtxObjMax.z
			&& pos.z + vtxMin.z < ObjPos.z + vtxObjMax.z
			&& pos.x + vtxMax.x > ObjPos.x + vtxObjMin.x
			&& pos.x + vtxMin.x < ObjPos.x + vtxObjMax.x)
		{//�������O�ɂ߂荞��
			//�ʒu��߂�
			bLand = true;
			move.z *= -1.0f;
			//move.z *= fRefMulti;
			pos.z = ObjPos.z + vtxObjMax.z - vtxMin.z + 0.1f + m_move.z;
			//move.z = 0.0f;
		}
		else if (posOld.z + vtxMax.z <= m_posOld.z + vtxObjMin.z
			&& pos.z + vtxMax.z > ObjPos.z + vtxObjMin.z
			&& pos.x + vtxMax.x > ObjPos.x + vtxObjMin.x
			&& pos.x + vtxMin.x < ObjPos.x + vtxObjMax.x)
		{//��O���牜�ɂ߂荞��t
		 //�ʒu��߂�
			bLand = true;
			move.z *= -1.0f;
			//move.z *= fRefMulti;
			pos.z = ObjPos.z + vtxObjMin.z - vtxMax.z - 0.1f + m_move.z;
			//move.z = 0.0f;
		}
	}

	if (pos.x + vtxMax.x > ObjPos.x + vtxObjMin.x
		&& pos.x + vtxMin.x < ObjPos.x + vtxObjMax.x
		&& pos.z + vtxMax.z > ObjPos.z + vtxObjMin.z
		&& pos.z + vtxMin.z < ObjPos.z + vtxObjMax.z)
	{//�͈͓��ɂ���
	 //�ォ��̔���
		if (posOld.y + vtxMin.y >= ObjPos.y + vtxObjMax.y
			&& pos.y + vtxMin.y < ObjPos.y + vtxObjMax.y)
		{//�ォ��߂荞��
		 //��ɂ̂���
			pos.y = ObjPos.y + vtxObjMax.y - vtxMin.y;
			move.y = 0.0f;
			bLand = true;
		}
	}

	return bLand;
}
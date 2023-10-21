//==========================================================
//
// ��]�M�~�b�N [gimmick_rotate.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "gimmick_rotate.h"
#include "model.h"
#include "Xfile.h"
#include "manager.h"
#include "debugproc.h"
#include "character.h"
#include "motion.h"

// �}�N����`
#define FILENAME "data\\TXT\\motion_bird.txt"	// �g�p���f��
#define SETSIZE	(100.0f)

//==========================================================
// �R���X�g���N�^
//==========================================================
CGimmickRotate::CGimmickRotate()
{
	for (int nCnt = 0; nCnt < NUM_ROTATEBOX; nCnt++)
	{
		m_aObj[nCnt].s_pModel = NULL;
	}

	m_RotateSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CGimmickRotate::~CGimmickRotate()
{

}

//==========================================================
// ����������
//==========================================================
HRESULT CGimmickRotate::Init(void)
{
	m_fSize = SETSIZE;
	m_RotateSpeed = D3DXVECTOR3(0.0f, 0.01f, 0.0f);

	for (int nCnt = 0; nCnt < NUM_ROTATEBOX; nCnt++)
	{
		m_aObj[nCnt].s_pModel = CCharacter::Create(FILENAME);
		m_aObj[nCnt].s_pModel->SetParent(GetMtxWorld());
		m_aObj[nCnt].s_pModel->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 
			((-SETSIZE * (int)(NUM_ROTATEBOX * 0.5f)))
			+ (SETSIZE * nCnt) 
			+ (SETSIZE * ((1 - NUM_ROTATEBOX % 2) * 0.5f))));
		m_aObj[nCnt].s_pModel->GetMotion()->InitSet(0);
	}
	return S_OK;
}

//==========================================================
// �I������
//==========================================================
void CGimmickRotate::Uninit(void)
{
	for (int nCnt = 0; nCnt < NUM_ROTATEBOX; nCnt++)
	{
		if (m_aObj[nCnt].s_pModel != NULL)
		{
			m_aObj[nCnt].s_pModel->Uninit();
			m_aObj[nCnt].s_pModel = NULL;
		}
	}

	ListOut();

	// �p��
	Release();
}

//==========================================================
// �X�V����
//==========================================================
void CGimmickRotate::Update(void)
{
	// �����̐ݒ�
	D3DXVECTOR3 rot = GetRotation();
	rot += m_RotateSpeed;

	if (rot.y > D3DX_PI)
	{
		rot.y += -D3DX_PI * 2;
	}
	else if (rot.y < -D3DX_PI)
	{
		rot.y += D3DX_PI * 2;
	}

	SetRotation(rot);

	for (int nCnt = 0; nCnt < NUM_ROTATEBOX; nCnt++)
	{
		if (m_aObj[nCnt].s_pModel != NULL)
		{
			m_aObj[nCnt].s_posOld.x = m_aObj[nCnt].s_pModel->GetMtxWorld()->_41;
			m_aObj[nCnt].s_posOld.y = m_aObj[nCnt].s_pModel->GetMtxWorld()->_42;
			m_aObj[nCnt].s_posOld.z = m_aObj[nCnt].s_pModel->GetMtxWorld()->_43;
			m_aObj[nCnt].s_pModel->Update();
		}
	}

	SetMtxWorld();
}

//==========================================================
// ����
//==========================================================
CGimmickRotate *CGimmickRotate::Create(void)
{
	CGimmickRotate *pObjectRotate = new CGimmickRotate;

	if (pObjectRotate != NULL)
	{
		// ����������
		pObjectRotate->Init();
	}

	return pObjectRotate;
}

//==========================================================
// �ʔ���`�F�b�N
//==========================================================
bool CGimmickRotate::CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, int &nDamage, const float fRefMulti)
{

	CXFile *pFile = CManager::GetInstance()->GetModelFile();
	bool bLand = false;	// ���n�������ۂ�
	D3DXVECTOR3 vtxObjMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxObjMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 ObjPos = GetPosition();
	D3DXVECTOR3 ObjRot = GetRotation();

	// �͈͓��`�F�b�N
	{
		float fLength =
			sqrtf((pos.x - ObjPos.x) * (pos.x - ObjPos.x)
				+ (pos.z - ObjPos.z) * (pos.z - ObjPos.z));

		if (fLength > SETSIZE * NUM_ROTATEBOX * 0.5f)
		{
			return bLand;
		}
	}

	// ��]���Ă���q���Ƃ̔���
	for (int nCnt = 0; nCnt < NUM_ROTATEBOX; nCnt++)
	{
		if (m_aObj[nCnt].s_pModel == NULL)
		{
			continue;
		}

		ObjPos.x = m_aObj[nCnt].s_pModel->GetMtxWorld()->_41;
		ObjPos.y = m_aObj[nCnt].s_pModel->GetMtxWorld()->_42;
		ObjPos.z = m_aObj[nCnt].s_pModel->GetMtxWorld()->_43;

		// �������擾
		float fLength = sqrtf((pos.x - ObjPos.x) * (pos.x - ObjPos.x)
			+ (pos.z - ObjPos.z) * (pos.z - ObjPos.z));

		float fOldLength = sqrtf((posOld.x - ObjPos.x) * (posOld.x - ObjPos.x)
			+ (posOld.z - ObjPos.z) * (posOld.z - ObjPos.z));

		float fSize = m_fSize * 0.75f;

		if (fLength > fSize)
		{// �G��Ă��Ȃ�
			continue;
		}

		bLand = true;

		// �G��Ă���̂ł߂荞�݂𒼂�
		D3DXVECTOR3 vec = D3DXVECTOR3(pos.x - ObjPos.x, pos.y - ObjPos.y, pos.z - ObjPos.z);
		D3DXVec3Normalize(&vec, &vec);
		pos = D3DXVECTOR3(ObjPos.x + fSize * vec.x, ObjPos.y + fSize * vec.y, ObjPos.z + fSize * vec.z);

		// �ړ��ʂ�␳
		D3DXVECTOR3 movevec;
		D3DXVec3Normalize(&movevec, &move);
		D3DXVECTOR3 vecDiff = movevec - vec;
		D3DXVec3Normalize(&vecDiff, &vecDiff);
		//move = D3DXVECTOR3(move.x + move.x * vecDiff.x, move.y + move.y * vecDiff.y, move.z +  move.z * vecDiff.z);
	}

	return bLand;
}

//==========================================================
// ��]���x�ݒ�
//==========================================================
void CGimmickRotate::SetRotate(D3DXVECTOR3 rotate)
{ 
	m_RotateSpeed = rotate; 
	SetRotationCharacter();
}

//==========================================================
// ��]�I�u�W�F�N�g�̌�����ݒ�
//==========================================================
void CGimmickRotate::SetRotationCharacter(void)
{
	if ((int)(NUM_ROTATEBOX * 0.5f) <= 0)
	{
		return;
	}

	for (int nCnt = 0; nCnt < NUM_ROTATEBOX; nCnt++)
	{
		if (m_aObj[nCnt].s_pModel != NULL)
		{
			if (m_RotateSpeed.y > 0.0f)
			{
				m_aObj[nCnt].s_pModel->SetRotation(D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f * (1.0f - (nCnt / (int)(NUM_ROTATEBOX * 0.5f)) * 2),
					0.0f));
			}
			else
			{
				m_aObj[nCnt].s_pModel->SetRotation(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f * (1.0f - (nCnt / (int)(NUM_ROTATEBOX * 0.5f)) * 2),
					0.0f));
			}
		}
	}
}
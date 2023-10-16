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

// �}�N����`
#define FILENAME "data\\MODEL\\5msphere.x"	// �g�p���f��
#define SETSIZE	(100.0f)

//==========================================================
// �R���X�g���N�^
//==========================================================
CGimmickRotate::CGimmickRotate(int nPriOrity) : CObjectX(nPriOrity)
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
	CXFile *pFile = CManager::GetInstance()->GetModelFile();
	BindFile(pFile->Regist("data\\MODEL\\1mcube.x"));
	m_fSize = SETSIZE;
	m_RotateSpeed = D3DXVECTOR3(0.0f, 0.01f, 0.0f);

	for (int nCnt = 0; nCnt < NUM_ROTATEBOX; nCnt++)
	{
		m_aObj[nCnt].s_pModel = CModel::Create(FILENAME);
		m_aObj[nCnt].s_pModel->SetParent(GetMtx());
		m_aObj[nCnt].s_pModel->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 
			((-SETSIZE * (int)(NUM_ROTATEBOX * 0.5f)))
			+ (SETSIZE * nCnt) 
			+ (SETSIZE * ((1 - NUM_ROTATEBOX % 2) * 0.5f))));
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
			delete m_aObj[nCnt].s_pModel;
			m_aObj[nCnt].s_pModel = NULL;
		}
	}

	CObjectX::Uninit();

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
		}
	}
}

//==========================================================
// �`�揈��
//==========================================================
void CGimmickRotate::Draw(void)
{
	// �`��
	CObjectX::Draw();

	for (int nCnt = 0; nCnt < NUM_ROTATEBOX; nCnt++)
	{
		if (m_aObj[nCnt].s_pModel != NULL)
		{
			m_aObj[nCnt].s_pModel->Draw();
		}
	}
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
bool CGimmickRotate::CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, const float fRefMulti)
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

	// �����𔽉f
	SetRotSize(vtxObjMax,
		vtxObjMin,
		pFile->GetMax(GetIdx()),
		pFile->GetMin(GetIdx()),
		ObjRot.y);

	if (pos.y + vtxMax.y > ObjPos.y + vtxObjMin.y
		&& pos.y + vtxMin.y <= ObjPos.y + vtxObjMax.y)
	{//�v���C���[�ƃ��f�������������ɂ���
		if (posOld.x + vtxMin.x >= ObjPos.x + vtxObjMax.x
			&& pos.x + vtxMin.x < ObjPos.x + vtxObjMax.x
			&& pos.z + vtxMax.z > ObjPos.z + vtxObjMin.z
			&& pos.z + vtxMin.z < ObjPos.z + vtxObjMax.z)
		{//�E���獶�ɂ߂荞��
			move.x *= -1.0f;
			move.x *= fRefMulti;
			pos.x = ObjPos.x + vtxObjMax.x - vtxMin.x + 0.1f + move.x;
		}
		else if (posOld.x + vtxMax.x <= ObjPos.x + vtxObjMin.x
			&& pos.x + vtxMax.x > ObjPos.x + vtxObjMin.x
			&& pos.z + vtxMax.z > ObjPos.z + vtxObjMin.z
			&& pos.z + vtxMin.z < ObjPos.z + vtxObjMax.z)
		{//������E�ɂ߂荞��
		 //�ʒu��߂�
			move.x *= -1.0f;
			move.x *= fRefMulti;
			pos.x = ObjPos.x + vtxObjMin.x - vtxMax.x - 0.1f + move.x;
			//move.x = 0.0f;
		}
		else if (posOld.z + vtxMin.z >= ObjPos.z + vtxObjMax.z
			&& pos.z + vtxMin.z < ObjPos.z + vtxObjMax.z
			&& pos.x + vtxMax.x > ObjPos.x + vtxObjMin.x
			&& pos.x + vtxMin.x < ObjPos.x + vtxObjMax.x)
		{//�������O�ɂ߂荞��
		 //�ʒu��߂�
			move.z *= -1.0f;
			move.z *= fRefMulti;
			pos.z = ObjPos.z + vtxObjMax.z - vtxMin.z + 0.1f + move.z;
			//move.z = 0.0f;
		}
		else if (posOld.z + vtxMax.z <= ObjPos.z + vtxObjMin.z
			&& pos.z + vtxMax.z > ObjPos.z + vtxObjMin.z
			&& pos.x + vtxMax.x > ObjPos.x + vtxObjMin.x
			&& pos.x + vtxMin.x < ObjPos.x + vtxObjMax.x)
		{//��O���牜�ɂ߂荞��t
		 //�ʒu��߂�
			move.z *= -1.0f;
			move.z *= fRefMulti;
			pos.z = ObjPos.z + vtxObjMin.z - vtxMax.z - 0.1f + move.z;
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
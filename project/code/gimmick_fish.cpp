//==========================================================
//
// ���M�~�b�N(������o�Ă���) [gimmick_fish.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "gimmick_fish.h"
#include "manager.h"
#include "slow.h"
#include "Xfile.h"
#include "debugproc.h"

// �}�N����`
#define WAIT_COUNTER	(180.0f)
#define ATK_COUNTER	(60.0f)
#define UP_COUNTER	(60.0f)
#define DOWN_COUNTER	(30.0f)
#define ATK_ENDCOUNTER	(30.0f)

//==========================================================
// �R���X�g���N�^
//==========================================================
CGimmickFish::CGimmickFish(int nPriOrity) : CObjectX(nPriOrity)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fNowFishTimer = 0.0f;
	m_fNumFishTimer = 0.0f;
	m_state = STATE_NONE;
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CGimmickFish::~CGimmickFish()
{

}

//==========================================================
// ����������
//==========================================================
HRESULT CGimmickFish::Init(void)
{
	if (CObjectX::Init() == E_FAIL)
	{
		return E_FAIL;
	}

	// �ǂݍ��݊m�F
	CXFile *pFile = CManager::GetInstance()->GetModelFile();
	BindFile(pFile->Regist("data\\MODEL\\30msphere.x"));

	// �X���[���o����
	m_pSlow = CManager::GetInstance()->GetSlow();

	m_state = STATE_WAIT;
	m_fStateCounter = WAIT_COUNTER;

	return S_OK;
}

//==========================================================
// �I������
//==========================================================
void CGimmickFish::Uninit(void)
{
	CObjectX::Uninit();

	Release();
}

//==========================================================
// �X�V����
//==========================================================
void CGimmickFish::Update(void)
{
	m_posOld = GetPosition();

	// ����֘A
	Controller();
}

//==========================================================
// �`�揈��
//==========================================================
void CGimmickFish::Draw(void)
{
	CObjectX::Draw();
}

//==========================================================
// ����
//==========================================================
CGimmickFish *CGimmickFish::Create(void)
{
	CGimmickFish *pObjectFish = new CGimmickFish;

	if (pObjectFish != NULL)
	{
		// ����������
		pObjectFish->Init();
	}

	return pObjectFish;
}

CGimmickFish *CGimmickFish::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fTimer)
{
	CGimmickFish *pObjectFish = new CGimmickFish;

	if (pObjectFish != NULL)
	{
		// ����������
		pObjectFish->Init();

		// �ݒ�
		pObjectFish->SetPosition(pos);
		pObjectFish->SetFish(move);
		pObjectFish->SetNumTimer(fTimer);
		pObjectFish->SetNowTimer(fTimer);
	}

	return pObjectFish;
}

//==========================================================
// ����֘A
//==========================================================
void CGimmickFish::Controller(void)
{
	m_fStateCounter -= CManager::GetInstance()->GetSlow()->Get();
	D3DXVECTOR3 pos = GetPosition();
	pos += m_move * CManager::GetInstance()->GetSlow()->Get();
	SetPosition(pos);

	// ��Ԃ��ƂɍX�V
	if (m_fStateCounter <= 0.0f)
	{
		switch (m_state)
		{
		case STATE_NONE:

			break;

		case STATE_UP:

			m_state = STATE_ATK;
			m_fStateCounter = ATK_COUNTER;
			m_move = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			break;

		case STATE_ATK:

			m_state = STATE_ATKEND;
			m_fStateCounter = ATK_ENDCOUNTER;
			m_move = D3DXVECTOR3(0.0f, -2.0f, 0.0f);

			break;

		case STATE_ATKEND:

			m_state = STATE_DOWN;
			m_fStateCounter = DOWN_COUNTER;
			m_move = D3DXVECTOR3(0.0f, -21.0f, 0.0f);

			break;

		case STATE_DOWN:

			m_state = STATE_WAIT;
			m_fStateCounter = WAIT_COUNTER;
			m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			break;

		case STATE_WAIT:

			m_state = STATE_UP;
			m_fStateCounter = UP_COUNTER;
			m_move = D3DXVECTOR3(0.0f, 10.5f, 0.0f);
			break;

		default:

			break;
		}
	}

	CManager::GetInstance()->GetDebugProc()->Print("�J�E���g [ %f ], ��� [ %d ]\n", m_fStateCounter, m_state);
}

//==========================================================
// �ʔ���`�F�b�N
//==========================================================
bool CGimmickFish::CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, const float fRefMulti)
{
	if (m_state != STATE_ATK)
	{
		return false;
	}

	CXFile *pFile = CManager::GetInstance()->GetModelFile();
	bool bLand = false;	// ���n�������ۂ�
	D3DXVECTOR3 vtxObjMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxObjMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 ObjPos = GetPosition();
	D3DXVECTOR3 ObjRot = GetRotation();

	// �����𔽉f
	SetRotSize(vtxObjMax,
		vtxObjMin,
		pFile->GetMax(GetIdx()),
		pFile->GetMin(GetIdx()),
		ObjRot.y);

	if (pos.y + vtxMax.y > ObjPos.y + vtxObjMin.y
		&& pos.y + vtxMin.y <= ObjPos.y + vtxObjMax.y)
	{//�v���C���[�ƃ��f�������������ɂ���
		if (posOld.x + vtxMin.x >= m_posOld.x + vtxObjMax.x
			&& pos.x + vtxMin.x < ObjPos.x + vtxObjMax.x
			&& pos.z + vtxMax.z > ObjPos.z + vtxObjMin.z
			&& pos.z + vtxMin.z < ObjPos.z + vtxObjMax.z)
		{//�E���獶�ɂ߂荞��
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
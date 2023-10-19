//===============================================
//
// �v���C���[�̏��� [player.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "camera.h"
#include "Xfile.h"
#include "slow.h"
#include "texture.h"
#include "meshfield.h"
#include "Xfile.h"
#include "meshorbit.h"
#include "game.h"
#include "filter.h"
#include "object2D.h"
#include "meshwall.h"
#include "objectX.h"
#include "sound.h"
#include "shadow.h"
#include "gimmick.h"
#include <assert.h>
#include "billboard.h"

//===============================================
// �}�N����`
//===============================================
#define MOVE	(0.1f)		// �ړ���
#define SHW_MOVE	(1.0f)	// �V�����[���ړ���
#define PLAYER_GRAVITY	(-0.15f)		//�v���C���[�d��
#define PLAYER_JUMP		(10.0f)		//�v���C���[�W�����v��
#define ROT_MULTI	(0.075f)		// �����␳�{��
#define WIDTH	(20.0f)		// ��
#define HEIGHT	(80.0f)		// ����
#define INER	(0.003f)	// ����
#define STEP_SPEED	(50.0f)
#define STEP_COOLTIME	(90.0f)
#define STEP_INER	(0.05f)
#define START_LIFE	(4)	// �����̗�
#define DAMAGE_INTERVAL	(10.0f)
#define DAMAGE_APPEAR	(110.0f)
#define DEATH_INTERVAL	(120.0f)
#define DASH_INTERVAL	(60.0f)
#define SPAWN_INTERVAL	(60.0f)

//===============================================
// �R���X�g���N�^
//===============================================
//CPlayer::CPlayer()
//{
//	// �l���N���A����
//	m_nCounterAnim = 0;
//	m_nPatternAnim = 0;
//}

//===============================================
// �R���X�g���N�^(�I�[�o�[���[�h)
//===============================================
CPlayer::CPlayer(const D3DXVECTOR3 pos)
{
	// �l���N���A����
	m_Info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRotMove = 0.0f;
	m_fRotDiff = 0.0f;
	m_fRotDest = 0.0f;
	m_pObject = NULL;
	m_pShadow = NULL;
	m_nLife = 0;
}

//===============================================
// �R���X�g���N�^(�I�[�o�[���[�h)
//===============================================
CPlayer::CPlayer(int nPriOrity)
{
	// �l���N���A����
	m_Info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRotMove = 0.0f;
	m_fRotDiff = 0.0f;
	m_fRotDest = 0.0f;
	m_pObject = NULL;
	m_pShadow = NULL;
	m_nLife = 0;
	m_ppBillBoard = NULL;
}

//===============================================
// �f�X�g���N�^
//===============================================
CPlayer::~CPlayer()
{
	if (m_pShadow != nullptr)
	{
		assert(false);
	}

	if (m_pObject != nullptr)
	{
		assert(false);
	}
}

//===============================================
// ����������
//===============================================
HRESULT CPlayer::Init(void)
{
	if (nullptr == m_pObject)
	{
		m_pObject = CObjectX::Create(GetPosition(), GetRotation(), "data\\MODEL\\triangle.x");
		m_pObject->SetType(CObject::TYPE_PLAYER);
	}

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxRot);

	// �e�̐���
	if (nullptr == m_pShadow)
	{
		m_pShadow = CShadow::Create(m_Info.pos, 50.0f, 50.0f);
	}

	m_Info.state = STATE_APPEAR;
	m_nLife = START_LIFE;

	return S_OK;
}

//===============================================
// ����������(�I�[�o�[���[�h)
//===============================================
HRESULT CPlayer::Init(const char *pBodyName, const char *pLegName)
{
	if (nullptr == m_pObject)
	{
		m_pObject = CObjectX::Create(GetPosition(), GetRotation(), "data\\MODEL\\triangle.x");
		m_pObject->SetType(CObject::TYPE_PLAYER);
	}

	m_nLife = START_LIFE;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxRot);

	if(m_ppBillBoard == nullptr)
	{
		m_ppBillBoard = new CObjectBillboard*[START_LIFE];

		for (int nCnt = 0; nCnt < START_LIFE; nCnt++)
		{
			m_ppBillBoard[nCnt] = nullptr;

			if (nullptr == m_ppBillBoard[nCnt])
			{
				m_ppBillBoard[nCnt] = CObjectBillboard::Create(m_Info.pos, 5);
				m_ppBillBoard[nCnt]->SetRotation(D3DXVECTOR3(0.0f, 0.0f, D3DXToRadian(rand() % 628 - 314)));
				m_ppBillBoard[nCnt]->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\balloon.png"));
				m_ppBillBoard[nCnt]->SetSize(60.0f + nCnt * 15.0f, 60.0f + nCnt * 15.0f);
			}
		}
	}

	// �e�̐���
	if (nullptr == m_pShadow)
	{
		m_pShadow = CShadow::Create(m_Info.pos, 50.0f, 50.0f);
		m_pShadow->SetpVtx(m_ppBillBoard[m_nLife - 1]->GetWidth(), m_ppBillBoard[m_nLife - 1]->GetHeight());
	}

	m_pObject->SetDraw();

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CPlayer::Uninit(void)
{
	if (nullptr != m_pObject){
		m_pObject->Uninit();
		m_pObject = NULL;
	}

	if (nullptr != m_pShadow){
		m_pShadow->Uninit();
		m_pShadow = NULL;
	}

	if (m_ppBillBoard != NULL)
	{
		for (int nCnt = 0; nCnt < START_LIFE; nCnt++)
		{
			if (nullptr != m_ppBillBoard[nCnt])
			{
				m_ppBillBoard[nCnt]->Uninit();
				m_ppBillBoard[nCnt] = NULL;
			}
		}

		delete[] m_ppBillBoard;	// �|�C���^�̊J��
		m_ppBillBoard = NULL;	// �g�p���Ă��Ȃ���Ԃɂ���
	}

	// �p��
	Release();
}

//===============================================
// �X�V����
//===============================================
void CPlayer::Update(void)
{
	// �O��̍��W���擾
	m_Info.posOld = GetPosition();

	StateSet();

	if (m_nLife <= 0)
	{
		for (int nCnt = 0; nCnt < START_LIFE; nCnt++)
		{
			if (nullptr != m_ppBillBoard[nCnt]) {
				m_ppBillBoard[nCnt]->SetPosition(D3DXVECTOR3(m_Info.pos.x, m_Info.pos.y + 50.0f + 10.0f * nCnt, m_Info.pos.z));
			}
		}

		return;
	}

	// �v���C���[����
	Controller();

	// �J�����Ǐ]
	CCamera *pCamera = CManager::GetInstance()->GetCamera();

	// �Ǐ]����
	pCamera->Pursue(GetPosition(), GetRotation());

	CManager::GetInstance()->GetDebugProc()->Print("���� [%f, %f, %f]", GetRotation().x, GetRotation().y, GetRotation().z);
	CManager::GetInstance()->GetDebugProc()->Print("�ʒu [%f, %f, %f]", GetPosition().x, GetPosition().y, GetPosition().z);
	CManager::GetInstance()->GetDebugProc()->Print("�̗� [ %d ]\n", m_nLife);

	// ��]���̐ݒ�

	// �p�[�e�B�N��
	//Particle();

	if (m_nLife <= 0)
	{
		m_pObject->SetDraw(false);
		m_pShadow->SetDraw(false);
		m_Info.state = STATE_DEATH;
		m_Info.fStateCounter = DEATH_INTERVAL;
	}

	for (int nCnt = 0; nCnt < START_LIFE; nCnt++)
	{
		if (nullptr != m_ppBillBoard[nCnt]){
			D3DXVECTOR3 rot = m_ppBillBoard[nCnt]->GetRotation();

			rot.z += 0.0025f * (1 - (nCnt % 2 * 2));

			if (rot.z > D3DX_PI)
			{
				rot.z += -D3DX_PI * 2;
			}
			else if (rot.z < -D3DX_PI)
			{
				rot.z += D3DX_PI * 2;
			}

			m_ppBillBoard[nCnt]->SetRotation(rot);
			m_ppBillBoard[nCnt]->SetPosition(D3DXVECTOR3(m_Info.pos.x, m_Info.pos.y + 50.0f + 10.0f * nCnt, m_Info.pos.z));
			m_ppBillBoard[nCnt]->SetSize(m_ppBillBoard[nCnt]->GetWidth(), m_ppBillBoard[nCnt]->GetHeight());
		}
	}
}

//===============================================
// ����
//===============================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, const char *pBodyName, const char *pLegName, const int nPriority)
{
	CPlayer *pPlayer = NULL;
	CXFile *pModelFile = CManager::GetInstance()->GetModelFile();

	// �I�u�W�F�N�g2D�̐���
	pPlayer = new CPlayer(nPriority);

	if (nullptr != pPlayer)
	{// �����ł����ꍇ
		// ����������
		pPlayer->Init(pBodyName, pLegName);

		// ���W�ݒ�
		pPlayer->SetPosition(pos);

		// �����ݒ�
		pPlayer->SetRotation(rot);

		pPlayer->m_fRotDest = rot.y;

		// �ړ��ʐݒ�
		pPlayer->SetMove(move);

		// ��ނ̐ݒ�
		//pPlayer->SetType(TYPE_PLAYER);

		CManager::GetInstance()->GetCamera()->Setting(pos, rot);
	}
	else
	{// �����Ɏ��s�����ꍇ
		return NULL;
	}

	return pPlayer;
}

//===============================================
// ���쏈��
//===============================================
void CPlayer::Controller(void)
{
	D3DXVECTOR3 pos = GetPosition();	// ���W���擾
	D3DXVECTOR3 rot = GetRotation();	// �������擾
	CInputKeyboard *pInputKey = CManager::GetInstance()->GetInputKeyboard();	// �L�[�{�[�h�̃|�C���^
	CInputMouse *pInputMouse = CManager::GetInstance()->GetInputMouse();		// �}�E�X
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();				// �p�b�h
	CCamera *pCamera = CManager::GetInstance()->GetCamera();					// �J�����̃|�C���^
	D3DXVECTOR3 CamRot = pCamera->GetRotation();								// �J�����̊p�x
	bool bDamage = false;
	m_fRotMove = rot.y;	//���݂̌������擾
	int nDamage = 0;

	// ���쏈��
	{
		Move();		// �ړ�
		Rotation();	// ��]
	}

	pos = GetPosition();	// ���W���擾

	if (m_fStepCoolTime <= 0.0f)
	{
		m_Info.move.x += (0.0f - m_Info.move.x) * INER;	//x���W
		m_Info.move.z += (0.0f - m_Info.move.z) * INER;	//x���W
	}
	else
	{
		m_fStepCoolTime -= CManager::GetInstance()->GetSlow()->Get();
		m_Info.move.x += (0.0f - m_Info.move.x) * STEP_INER;	//x���W
		m_Info.move.z += (0.0f - m_Info.move.z) * STEP_INER;	//x���W
	}

	pos.x += m_Info.move.x * CManager::GetInstance()->GetSlow()->Get();
	pos.z += m_Info.move.z * CManager::GetInstance()->GetSlow()->Get();

	// ����
	Adjust();

	// �I�u�W�F�N�g�Ƃ̓����蔻��
	if (nullptr != m_pObject)
	{
		CXFile *pFile = CManager::GetInstance()->GetModelFile();
		D3DXVECTOR3 vtxMax = pFile->GetMax(m_pObject->GetIdx());
		D3DXVECTOR3 vtxMin = pFile->GetMin(m_pObject->GetIdx());

		if (m_ppBillBoard != nullptr){
			vtxMax = D3DXVECTOR3(m_ppBillBoard[m_nLife - 1]->GetWidth() * 0.7f, m_ppBillBoard[m_nLife - 1]->GetHeight(), m_ppBillBoard[m_nLife - 1]->GetHeight() * 0.7f);
			vtxMin = vtxMax * -1.0f;
		}

		// �M�~�b�N
		if (CGimmick::Collision(pos, m_Info.posOld, m_Info.move, vtxMin, vtxMax, nDamage, 0.3f))
		{
			bDamage = true;
			CManager::GetInstance()->GetDebugProc()->Print("***�M�~�b�N�Ɠ���������***\n");
		}

		// ��
		if (CObjectX::Collision(pos, m_Info.posOld, m_Info.move, vtxMin, vtxMax, 0.3f))
		{
			bDamage = true;
			CManager::GetInstance()->GetDebugProc()->Print("***�ǂƓ���������***\n");
		}
	}

	// �_���[�W�m�F
	if (m_Info.state == STATE_NORMAL)
	{
		if (nDamage >= 0)
		{
			if (bDamage)
			{
				m_nLife--;
				m_Info.fStateCounter = DAMAGE_INTERVAL;
				m_Info.state = STATE_DAMAGE;
			}
		}
	}

	if (nDamage < 0)
	{
		m_Info.move.x += (0.0f - m_Info.move.x) * STEP_INER;	//x���W
		m_Info.move.z += (0.0f - m_Info.move.z) * STEP_INER;	//x���W
	}

	// �ǂƂ̓����蔻��
	CMeshWall::Collision(pos, m_Info.posOld);

	// �N���Ƃ̓����蔻��
	D3DXVECTOR3 nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fHeight = CMeshField::GetHeight(pos);

	m_Info.pos = pos;

	if (nullptr != m_pObject){
		m_pObject->SetPosition(m_Info.pos);
		m_pObject->SetRotation(m_Info.rot);
	}

	// �e�̐ݒ�
	if (nullptr != m_pShadow) {
		m_pShadow->SetPosition(D3DXVECTOR3(pos.x, fHeight + 1.0f, pos.z));
	}

	//�f�o�b�O�\��
	CManager::GetInstance()->GetDebugProc()->Print("\n��][W,A,S,D] : ����[SPACE] : �X�e�b�v[K, (R�{�^��)]\n");
}

//===============================================
// �ړ�
//===============================================
void CPlayer::Move(void)
{
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		// �J�����̃|�C���^
	D3DXVECTOR3 CamRot = pCamera->GetRotation();	// �J�����̊p�x
	CInputKeyboard *pInputKey = CManager::GetInstance()->GetInputKeyboard();	// �L�[�{�[�h�̃|�C���^
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();
	float fSpeed = MOVE;	// �ړ���

	// ���͑��u�m�F
	if (nullptr == pInputKey){
		return;
	}

	// ���͑��u�m�F
	if (nullptr == pInputPad){
		return;
	}

	if (pInputKey->GetTrigger(DIK_K) || pInputPad->GetTrigger(CInputPad::BUTTON_RIGHTBUTTON, 0))
	{
		if (m_fStepCoolTime <= 0.0f && m_Info.state != STATE_DAMAGE)
		{
			m_fStepCoolTime = STEP_COOLTIME;
			m_Info.move.x = -sinf(m_Info.rot.y) * STEP_SPEED;
			m_Info.move.z = -cosf(m_Info.rot.y) * STEP_SPEED;
			m_Info.state = STATE_APPEAR;
			m_Info.fStateCounter = DASH_INTERVAL;
		}
	}

	//�v���C���[�̍X�V
	if (pInputKey->GetPress(DIK_SPACE) || pInputPad->GetPress(CInputPad::BUTTON_A, 0))
	{
		m_Info.move.x += -sinf(m_Info.rot.y) * fSpeed;
		m_Info.move.z += -cosf(m_Info.rot.y) * fSpeed;
	}
}

//===============================================
// ��]
//===============================================
void CPlayer::Rotation(void)
{
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		// �J�����̃|�C���^
	D3DXVECTOR3 CamRot = pCamera->GetRotation();	// �J�����̊p�x
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();

	// ���͑��u�m�F
	if (nullptr == pInputPad){
		return;
	}

	if (!pInputPad->GetStickPress(0, CInputPad::BUTTON_LEFT_X, 0.1f, CInputPad::STICK_PLUS) && 
		!pInputPad->GetStickPress(0, CInputPad::BUTTON_LEFT_X, 0.1f, CInputPad::STICK_MINUS) &&
		!pInputPad->GetStickPress(0, CInputPad::BUTTON_LEFT_Y, 0.1f, CInputPad::STICK_PLUS) &&
		!pInputPad->GetStickPress(0, CInputPad::BUTTON_LEFT_Y, 0.1f, CInputPad::STICK_MINUS))
	{// �R���g���[���[���͖���
		KeyBoardRotation();
		return;
	}

	D3DXVECTOR2 vec;
	vec.y = pInputPad->GetStickAdd(0, CInputPad::BUTTON_LEFT_X, 0.1f, CInputPad::STICK_PLUS);
	vec.x = pInputPad->GetStickAdd(0, CInputPad::BUTTON_LEFT_Y, 0.1f, CInputPad::STICK_PLUS);
	D3DXVec2Normalize(&vec, &vec);

	m_fRotDest = atan2f(vec.y, vec.x);
}

//===============================================
// ��]
//===============================================
void CPlayer::KeyBoardRotation(void)
{
	CInputKeyboard *pInputKey = CManager::GetInstance()->GetInputKeyboard();	// �L�[�{�[�h�̃|�C���^

	if (nullptr == pInputKey){
		return;
	}

	if (pInputKey->GetPress(DIK_W))
	{
		if (pInputKey->GetPress(DIK_A))
		{
			m_fRotDest = -D3DX_PI * 0.25f;
		}
		else if (pInputKey->GetPress(DIK_D))
		{
			m_fRotDest = D3DX_PI * 0.25f;
		}
		else
		{
			m_fRotDest = D3DX_PI * 0.0f;
		}
	}
	else if (pInputKey->GetPress(DIK_S))
	{
		if (pInputKey->GetPress(DIK_A))
		{
			m_fRotDest = -D3DX_PI * 0.75f;
		}
		else if (pInputKey->GetPress(DIK_D))
		{
			m_fRotDest = D3DX_PI * 0.75f;
		}
		else
		{
			m_fRotDest = D3DX_PI * 1.0f;
		}
	}
	else if (pInputKey->GetPress(DIK_A))
	{
		m_fRotDest = -D3DX_PI * 0.5f;
	}
	else if (pInputKey->GetPress(DIK_D))
	{
		m_fRotDest = D3DX_PI * 0.5f;
	}
}

//===============================================
// ����
//===============================================
void CPlayer::Adjust(void)
{
	while (1)
	{
		if (m_fRotDest > D3DX_PI || m_fRotDest < -D3DX_PI)
		{//-3.14�`3.14�͈̔͊O�̏ꍇ
			if (m_fRotDest > D3DX_PI)
			{
				m_fRotDest += (-D3DX_PI * 2);
			}
			else if (m_fRotDest < -D3DX_PI)
			{
				m_fRotDest += (D3DX_PI * 2);
			}
		}
		else
		{
			break;
		}
	}

	m_fRotDiff = m_fRotDest - m_fRotMove;	//�ڕW�܂ł̈ړ������̍���

	while (1)
	{
		if (m_fRotDiff > D3DX_PI || m_fRotDiff < -D3DX_PI)
		{//-3.14�`3.14�͈̔͊O�̏ꍇ
			if (m_fRotDiff > D3DX_PI)
			{
				m_fRotDiff += (-D3DX_PI * 2);
			}
			else if (m_fRotDiff < -D3DX_PI)
			{
				m_fRotDiff += (D3DX_PI * 2);
			}
		}
		else
		{
			break;
		}
	}

	m_Info.rot.y += m_fRotDiff * ROT_MULTI;

	while (1)
	{
		if (m_Info.rot.y > D3DX_PI || m_Info.rot.y < -D3DX_PI)
		{//-3.14�`3.14�͈̔͊O�̏ꍇ
			if (m_Info.rot.y > D3DX_PI)
			{
				m_Info.rot.y += (-D3DX_PI * 2);
			}
			else if (m_Info.rot.y < -D3DX_PI)
			{
				m_Info.rot.y += (D3DX_PI * 2);
			}
		}
		else
		{
			break;
		}
	}
}

//===============================================
// ��ԊǗ�
//===============================================
void CPlayer::StateSet(void)
{
	switch (m_Info.state)
	{
	case STATE_APPEAR:

		CManager::GetInstance()->GetDebugProc()->Print("��� : [���G]\n");
		m_Info.fStateCounter -= CManager::GetInstance()->GetSlow()->Get();
		if (m_Info.fStateCounter <= 0.0f)
		{
			m_Info.fStateCounter = 0.0f;
			m_Info.state = STATE_NORMAL;
		}

		break;

	case STATE_NORMAL:

		CManager::GetInstance()->GetDebugProc()->Print("��� : [�ʏ�]\n");

		break;

	case STATE_DAMAGE:

		CManager::GetInstance()->GetDebugProc()->Print("��� : [�_���[�W]\n");

		if (m_ppBillBoard != NULL)
		{
			m_ppBillBoard[m_nLife]->SetSize(m_ppBillBoard[m_nLife]->GetWidth() + 11.0f, m_ppBillBoard[m_nLife]->GetHeight() + 11.0f);
		}

		m_Info.fStateCounter -= CManager::GetInstance()->GetSlow()->Get();
		if (m_Info.fStateCounter <= 0.0f)
		{
			m_Info.fStateCounter = DAMAGE_APPEAR;
			m_Info.state = STATE_APPEAR;

			if (m_ppBillBoard != NULL)
			{
				m_ppBillBoard[m_nLife]->SetDraw(false);
			}

			if (m_pShadow != nullptr)
			{
				m_pShadow->SetpVtx(m_ppBillBoard[m_nLife - 1]->GetWidth(), m_ppBillBoard[m_nLife - 1]->GetHeight());
				m_pShadow->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)((float)m_nLife / (float)START_LIFE)));

			}
		}

		break;

	case STATE_DEATH:

		CManager::GetInstance()->GetDebugProc()->Print("��� : [���S]\n");

		if (m_ppBillBoard != NULL)
		{
			m_ppBillBoard[m_nLife]->SetSize(m_ppBillBoard[m_nLife]->GetWidth() + 50.0f, m_ppBillBoard[m_nLife]->GetHeight() + 50.0f);
		}

		{
			float fOld = m_Info.fStateCounter;
			m_Info.fStateCounter -= CManager::GetInstance()->GetSlow()->Get();
			if (m_Info.fStateCounter <= DEATH_INTERVAL * 0.95f && fOld >= DEATH_INTERVAL * 0.95f)
			{
				if (m_ppBillBoard != NULL)
				{
					m_ppBillBoard[m_nLife]->SetDraw(false);
				}
			}
		}

		if (m_Info.fStateCounter <= 0.0f)
		{
			m_Info.fStateCounter = SPAWN_INTERVAL;
			m_Info.state = STATE_SPAWN;
			m_pShadow->SetDraw();
			m_pShadow->SetpVtx(0.0f, 0.0f);
			m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_Info.pos.y = -600.0f;
			SetPosition(m_Info.pos);

			for (int nCnt = 0; nCnt < START_LIFE; nCnt++)
			{
				if (nullptr != m_ppBillBoard[nCnt])
				{
					m_ppBillBoard[nCnt]->SetDraw(true);
					m_ppBillBoard[nCnt]->SetSize(0.0f, 0.0f);
				}
			}
		}

		break;

	case STATE_SPAWN:
	{
		m_Info.fStateCounter -= CManager::GetInstance()->GetSlow()->Get();

		m_Info.pos.y += 10.0f;
		SetPosition(m_Info.pos);

		for (int nCnt = 0; nCnt < START_LIFE; nCnt++)
		{
			if (nullptr != m_ppBillBoard[nCnt])
			{
				m_ppBillBoard[nCnt]->SetSize(m_ppBillBoard[nCnt]->GetWidth() + 1.0f + nCnt * 0.25f, m_ppBillBoard[nCnt]->GetHeight() + 1.0f + nCnt * 0.25f);
			}
		}

		m_pShadow->SetpVtx(m_pShadow->GetWidth() + 1.0f + START_LIFE * 0.25f, m_pShadow->GetHeight() + 1.0f + START_LIFE * 0.25f);
		m_pShadow->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, (float)((float)(SPAWN_INTERVAL - m_Info.fStateCounter) / (float)SPAWN_INTERVAL)));

		if (m_Info.fStateCounter <= 0.0f)
		{
			m_pObject->SetDraw();
			m_Info.fStateCounter = 0.0f;
			m_Info.state = STATE_APPEAR;
			m_nLife = START_LIFE;
			m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}
	}
		break;
	}
}
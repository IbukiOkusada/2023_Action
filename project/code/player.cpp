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
#define STEP_COOLTIME	(180.0f)
#define STEP_INER	(0.115f)

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
}

//===============================================
// �f�X�g���N�^
//===============================================
CPlayer::~CPlayer()
{

}

//===============================================
// ����������
//===============================================
HRESULT CPlayer::Init(void)
{
	if (m_pObject == NULL)
	{
		m_pObject = CObjectX::Create(GetPosition(), GetRotation(), "data\\MODEL\\triangle.x");
		m_pObject->SetType(CObject::TYPE_PLAYER);
	}

	return S_OK;
}

//===============================================
// ����������(�I�[�o�[���[�h)
//===============================================
HRESULT CPlayer::Init(const char *pBodyName, const char *pLegName)
{
	if (m_pObject == NULL)
	{
		m_pObject = CObjectX::Create(GetPosition(), GetRotation(), "data\\MODEL\\triangle.x");
		m_pObject->SetType(CObject::TYPE_PLAYER);
	}

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CPlayer::Uninit(void)
{
	if (m_pObject != NULL)
	{
		m_pObject->Uninit();
		m_pObject = NULL;
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

	// �v���C���[����
	Controller();

	// �J�����Ǐ]
	CCamera *pCamera = CManager::GetCamera();

	// �Ǐ]����
	pCamera->Pursue(GetPosition(), GetRotation());

	CManager::GetDebugProc()->Print("���� [%f, %f, %f]", GetRotation().x, GetRotation().y, GetRotation().z);
	CManager::GetDebugProc()->Print("�ʒu [%f, %f, %f]", GetPosition().x, GetPosition().y, GetPosition().z);

	// �p�[�e�B�N��
	//Particle();
}

//===============================================
// ����
//===============================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, const char *pBodyName, const char *pLegName, const int nPriority)
{
	CPlayer *pPlayer = NULL;
	CXFile *pModelFile = CManager::GetModelFile();

	// �I�u�W�F�N�g2D�̐���
	pPlayer = new CPlayer(nPriority);

	if (pPlayer != NULL)
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

		CManager::GetCamera()->Setting(pos, rot);
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
	CInputKeyboard *pInputKey = CManager::GetInputKeyboard();	// �L�[�{�[�h�̃|�C���^
	CInputMouse *pInputMouse = CManager::GetInputMouse();
	CInputPad *pInputPad = CManager::GetInputPad();
	CCamera *pCamera = CManager::GetCamera();		// �J�����̃|�C���^
	D3DXVECTOR3 CamRot = pCamera->GetRotation();	// �J�����̊p�x
	m_fRotMove = rot.y;	//���݂̌������擾

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
		m_fStepCoolTime -= CManager::GetSlow()->Get();
		m_Info.move.x += (0.0f - m_Info.move.x) * STEP_INER;	//x���W
		m_Info.move.z += (0.0f - m_Info.move.z) * STEP_INER;	//x���W
	}

	pos.x += m_Info.move.x * CManager::GetSlow()->Get();
	pos.z += m_Info.move.z * CManager::GetSlow()->Get();

	// ����
	Adjust();

	// �I�u�W�F�N�g�Ƃ̓����蔻��
	if (m_pObject != NULL)
	{
		CXFile *pFile = CManager::GetModelFile();
		D3DXVECTOR3 vtxMax = pFile->GetMax(m_pObject->GetIdx());
		D3DXVECTOR3 vtxMin = pFile->GetMin(m_pObject->GetIdx());
		CObjectX::Collision(pos, m_Info.posOld, m_Info.move, vtxMin, vtxMax, 0.3f);
	}

	// �ǂƂ̓����蔻��
	CMeshWall::Collision(pos, m_Info.posOld);

	// �N���Ƃ̓����蔻��
	D3DXVECTOR3 nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fHeight = CMeshField::GetHeight(pos);

	m_Info.pos = pos;

	if (m_pObject != NULL){
		m_pObject->SetPosition(m_Info.pos);
		m_pObject->SetRotation(m_Info.rot);
	}

	//�f�o�b�O�\��
	CManager::GetDebugProc()->Print("\n��][W,A,S,D] : ����[SPACE] : �X�e�b�v[K, (R�{�^��)]\n");
}

//===============================================
// �ړ�
//===============================================
void CPlayer::Move(void)
{
	CCamera *pCamera = CManager::GetCamera();		// �J�����̃|�C���^
	D3DXVECTOR3 CamRot = pCamera->GetRotation();	// �J�����̊p�x
	CInputKeyboard *pInputKey = CManager::GetInputKeyboard();	// �L�[�{�[�h�̃|�C���^
	CInputPad *pInputPad = CManager::GetInputPad();
	float fSpeed = MOVE;	// �ړ���

	// ���͑��u�m�F
	if (pInputKey == NULL){
		return;
	}

	// ���͑��u�m�F
	if (pInputPad == NULL){
		return;
	}

	if (pInputKey->GetTrigger(DIK_K) || pInputPad->GetTrigger(CInputPad::BUTTON_RIGHTBUTTON, 0))
	{
		if (m_fStepCoolTime <= 0.0f)
		{
			m_fStepCoolTime = STEP_COOLTIME;
			m_Info.move.x += -sinf(m_Info.rot.y) * STEP_SPEED;
			m_Info.move.z += -cosf(m_Info.rot.y) * STEP_SPEED;
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
	CCamera *pCamera = CManager::GetCamera();		// �J�����̃|�C���^
	D3DXVECTOR3 CamRot = pCamera->GetRotation();	// �J�����̊p�x
	CInputPad *pInputPad = CManager::GetInputPad();

	// ���͑��u�m�F
	if (pInputPad == NULL){
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
	CInputKeyboard *pInputKey = CManager::GetInputKeyboard();	// �L�[�{�[�h�̃|�C���^

	if (pInputKey == NULL){
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
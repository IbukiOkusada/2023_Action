//===============================================
//
// �Q�[����ʂ̊Ǘ����� [game.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
#include "texture.h"
#include "objectX.h"
#include "Xfile.h"
#include "input.h"
#include "fade.h"
#include "pause.h"
#include "result.h"
#include "sound.h"
#include "fileload.h"
#include "debugproc.h"
#include "player.h"
#include "editor.h"
#include "gimmick_move.h"
#include "gimmick_rotate.h"

//===============================================
// �}�N����`
//===============================================
#define START_TIME	(60 * 4)	// ��������
#define START_SCORE	(0)		// �J�n�X�R�A
#define MAP_SIZE	(100.0f)	// �}�b�v�T�C�Y
#define STARTSET_NUMENEMY	(3)	// �J�n���ɔz�u����G�̐�
#define MORNING_TIME	(60 * 6)	// �����I������
#define AM_COMRUSH	(60 * 9)	// �ʋ΃��b�V��
#define RELAX_TIME	(60 * 11)	// �����b�N�X�^�C��
#define NOON_TIME	(60 * 12.5)	// ���x��
#define EVENT_RUSH	(60 * 15.5)	// �C�x���g
#define PM_RELAX_TIME	(60 * 16)	// �A��J�n����
#define PM_GOHOME_RUSH	(60 * 19)	// �A��b�V��
#define MAX_TIME	(60 * 20 + 10)	// �ő厞��
#define SPEED_UP	(30.0f)

//===============================================
// �ÓI�����o�ϐ�
//===============================================
CScore *CGame::m_pScore = NULL;		// �X�R�A�̃|�C���^
CMeshField *CGame::m_pMeshField = NULL;	// ���b�V���t�B�[���h�̃|�C���^
CPause *CGame::m_pPause = NULL;		// �|�[�Y�̃|�C���^

//===============================================
// �R���X�g���N�^
//===============================================
CGame::CGame()
{
	// �l�̃N���A
	m_pMapCamera = NULL;
	m_pMapThermo = NULL;
	m_nMaxEnemy = 0;
	m_pTime = NULL;
	m_pEnemyManager = NULL;
	m_pPlayer = NULL;
	m_pFileLoad = NULL;
	m_pEditor = NULL;
	m_pCarManager = NULL;
	m_pMeshDome = NULL;
	m_pStart = NULL;

	for (int nCnt = 0; nCnt < NUM_FILTER; nCnt++)
	{
		m_apFilter[nCnt] = NULL;
	}
}

//===============================================
// �f�X�g���N�^
//===============================================
CGame::~CGame()
{

}

//===============================================
// ����������
//===============================================
HRESULT CGame::Init(void)
{
	// �O���t�@�C���ǂݍ��݂̐���
	if (m_pFileLoad == NULL)
	{// �g�p���Ă��Ȃ��ꍇ
		m_pFileLoad = new CFileLoad;

		if (m_pFileLoad != NULL)
		{
			m_pFileLoad->Init();
			m_pFileLoad->OpenFile("data\\TXT\\model.txt");
		}
	}

	// �G�f�B�^�[�̐���
	if (m_pEditor == NULL)
	{
		m_pEditor = new CEditor;
		m_pEditor->Init();
	}

	// �|�[�Y�̐���
	if (m_pPause == NULL)
	{
		m_pPause = CPause::Create();
	}

	CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), NULL, NULL);
	CGimmickMove::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), 120.0f);
	CGimmickRotate *p = CGimmickRotate::Create();
	p->SetPosition(D3DXVECTOR3(-300.0f, 0.0f, -500.0f));

	// �X�|�b�g���C�g���I��
	CManager::GetLight()->EnablePointLight(true);

	CManager::GetSound()->Play(CSound::LABEL_BGM_GAME);

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CGame::Uninit(void)
{
	if (m_pFileLoad != NULL)
	{
		m_pFileLoad->Uninit();

		delete m_pFileLoad;		// �������̊J��
		m_pFileLoad = NULL;
	}

	if (m_pPause != NULL)
	{
		m_pPause->Uninit();
		delete m_pPause;
		m_pPause = NULL;
	}
}

//===============================================
// �X�V����
//===============================================
void CGame::Update(void)
{
	// �|�[�Y
	if (m_pPause != NULL)
	{
		m_pPause->Update();

		if (m_pPause->GetEnable() == true)
		{
			if (m_pPause->GetSelect() == true)
			{
				CManager::GetFade()->Update();
			}
			return;
		}
	}

#if _DEBUG

	if (m_pEditor != NULL)
	{
		m_pEditor->Update();

		if (m_pEditor->GetActive())
		{
			return;
		}
	}

#endif

	// �X�V����
	CScene::Update();
}

//===============================================
// �`�揈��
//===============================================
void CGame::Draw(void)
{
	CScene::Draw();

	// �|�[�Y
	if (m_pPause != NULL)
	{
		m_pPause->Draw();
	}
}

//===================================================
// �X�R�A�̎擾
//===================================================
CScore *CGame::GetScore(void)
{
	return m_pScore;
}

//===================================================
// �X�R�A�̎擾
//===================================================
CTime *CGame::GetTime(void)
{
	return m_pTime;
}

//===================================================
// �v���C���[�̎擾
//===================================================
CPlayer *CGame::GetPlayer(void)
{
	return m_pPlayer;
}
//===================================================
// ���b�V���t�B�[���h�̎擾
//===================================================
CMeshField *CGame::GetMeshField(void)
{
	return m_pMeshField;
}

//===================================================
// �t�@�C���ǂݍ��݂̎擾
//===================================================
CFileLoad *CGame::GetFileLoad(void)
{
	return m_pFileLoad;
}

//===================================================
// �|�[�Y�̎擾
//===================================================
CPause *CGame::GetPause(void)
{
	return m_pPause;
}

//===================================================
// �G�f�B�^�[���̎擾
//===================================================
CEditor *CGame::GetEditor(void)
{
	return m_pEditor;
}

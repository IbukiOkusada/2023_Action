//===============================================
//
// ���U���g��ʂ̊Ǘ����� [result.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "result.h"
#include "object2D.h"
#include "texture.h"
#include "manager.h"
#include "main.h"
#include "input.h"
#include "objectX.h"
#include "fade.h"
#include "ranking.h"
#include "sound.h"
#include "result.h"
#include "time.h"

// �}�N����`
#define MOVE_TIMER	(900)

int CResult::m_nScore = 0;
CResult::TYPE CResult::m_type = CResult::TYPE_NONE;

//===============================================
// �R���X�g���N�^
//===============================================
CResult::CResult()
{
	m_pMeshSky = NULL;
	m_nTimer = 0;
	m_pTime = NULL;
}

//===============================================
// �f�X�g���N�^
//===============================================
CResult::~CResult()
{

}

//===============================================
// ����������
//===============================================
HRESULT CResult::Init(void)
{
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_RESULT);
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	// ��ނ��Ƃɕ`��
	switch (m_type)
	{
	case TYPE_NONE:
		m_pTime = CTime::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.4f, SCREEN_HEIGHT * 0.075f, 0.0f));
		m_pTime->Set(m_nScore);
		break;

	case TYPE_MULTI_WIN:
	{
		CObject2D *pObj = CObject2D::Create();
		pObj->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.8f, 0.0f));
		pObj->SetSize(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.2f);
		pObj->BindTexture(pTexture->Regist("data\\TEXTURE\\result_win.png"));
	}
		break;

	case TYPE_MULTI_LOSE:
	{
		CObject2D *pObj = CObject2D::Create();
		pObj->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.8f, 0.0f));
		pObj->SetSize(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.2f);
		pObj->BindTexture(pTexture->Regist("data\\TEXTURE\\result_lose.png"));
	}
		break;
	}

	return S_OK;
}

//===============================================
// �I������
//===============================================
void CResult::Uninit(void)
{
	if (m_pTime != NULL)
	{
		m_pTime->Uninit();
		delete m_pTime;
		m_pTime = NULL;
	}

	m_type = TYPE_NONE;
	m_nScore = 0;
}

//===============================================
// �X�V����
//===============================================
void CResult::Update(void)
{
	m_nTimer++;

	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RETURN) || m_nTimer > MOVE_TIMER 
		|| CManager::GetInstance()->GetInputPad()->GetTrigger(CInputPad::BUTTON_A, 0) || CManager::GetInstance()->GetInputPad()->GetTrigger(CInputPad::BUTTON_START, 0))
	{
		CManager::GetInstance()->GetFade()->Set(CScene::MODE_TITLE);
	}

	CScene::Update();
}

//===============================================
// �`�揈��
//===============================================
void CResult::Draw(void)
{
	CScene::Draw();
}

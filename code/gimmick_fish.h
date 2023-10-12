//==========================================================
//
// ���M�~�b�N(������o�Ă���) [gimmick_fish.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _GIMMICK_Fish_H_		// ���̃}�N������`����Ă��Ȃ��ꍇ
#define _GIMMICK_Fish_H_		// ��d�C���N���[�h�h�~�p�}�N�����`

#include "objectX.h"

// �O���錾
class CSlow;

//==========================================================
// �ړ��I�u�W�F�N�g�̃N���X��`
//==========================================================
class CGimmickFish : public CObjectX
{
private:	// �����������A�N�Z�X�\�Ȓ�`

	enum STATE
	{
		STATE_NONE = 0,	// �������Ă��Ȃ�
		STATE_UP,		// �㏸
		STATE_ATK,		// �U��
		STATE_DOWN,		// ���~
		STATE_WAIT,		// �ҋ@
	};

public:	// �N�ł��A�N�Z�X�\

	//CGimmickFish();	// �R���X�g���N�^
	CGimmickFish(int nPriOrity = 3);	// �R���X�g���N�^(�I�[�o�[���[�h)
	~CGimmickFish();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CGimmickFish *Create(void);
	static CGimmickFish *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fTimer);

	// �����o�֐�(�擾)
	D3DXVECTOR3 GetFish(void) { return m_move; }	// �ړ��ʎ擾
	float GetNowTimer(void) { return m_fNowFishTimer; }	// ���݃^�C�}�[�擾
	float GetNumTimer(void) { return m_fNumFishTimer; }	// ����^�C�}�[�擾

	// �����o�֐�(�ݒ�)
	void SetFish(D3DXVECTOR3 move) { m_move = move; }	// �ړ��ʐݒ�
	void SetNowTimer(float fTimer) { m_fNowFishTimer = fTimer; }	// ���݃^�C�}�[�ݒ�
	void SetNumTimer(float fTimer) { m_fNumFishTimer = fTimer; }	// ����^�C�}�[�ݒ�

private:	// �����������A�N�Z�X�\

	// �����o�֐�
	void Controller(void);
	bool CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, const float fRefMulti = 0.5f);

	// �����o�ϐ�
	D3DXVECTOR3 m_move;	// �ړ���
	D3DXVECTOR3 m_posOld;
	float m_fNowFishTimer;
	float m_fNumFishTimer;
	CSlow *m_pSlow;		// �X���[
	STATE m_state;	// ���
};

#endif

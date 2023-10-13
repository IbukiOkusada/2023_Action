//==========================================================
//
// �v���C���[�̏��� [player.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _PLAYER_H_		// ���̃}�N������`����Ă��Ȃ��ꍇ
#define _PLAYER_H_		// ��d�C���N���[�h�h�~�p�}�N�����`

#include "main.h"
#include "object.h"
#include "objectX.h"
#include "task.h"

// �O���錾
class CShadow;
class CCharacter;
class CWaist;
class CObject2D;
class CObjectBillboard;
class CModel;
class CLockOn;
class CMeshOrbit;
class CObject3D;
class CCar;
class CObjectX;

//==========================================================
// �v���C���[�̃N���X��`(�h���N���X)
//==========================================================
class CPlayer : public CTask
{
private:	// �����������A�N�Z�X�\�Ȓ�`

	// ���\����
	typedef struct
	{
		D3DXVECTOR3 pos;		// �ʒu
		D3DXVECTOR3 rot;		// ����
		D3DXVECTOR3 move;		// �ړ���
		D3DXVECTOR3 posOld;		// �ݒ�ʒu
		D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
	}SInfo;

public:	// �N�ł��A�N�Z�X�\

	//CPlayer();	// �R���X�g���N�^
	CPlayer(const D3DXVECTOR3 pos);	// �R���X�g���N�^(�I�[�o�[���[�h)
	CPlayer(int nPriOrity = 1);
	~CPlayer();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	HRESULT Init(const char *pBodyName, const char *pLegName);	// �I�[�o�[���[�h
	void Uninit(void);
	void Update(void);
	static CPlayer *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 move,
		const char *pBodyName, const char *pLegName, const int nPriority = 4);

	// �����o�֐�(�ݒ�)
	void SetMove(const D3DXVECTOR3 move) { m_Info.move = move; }
	void SetPosition(const D3DXVECTOR3 pos) { m_Info.pos = pos; }
	void SetRotation(const D3DXVECTOR3 rot) { m_Info.rot = rot; }

	// �����o�֐�(�擾)
	D3DXVECTOR3 GetMove(void) { return m_Info.move; }
	D3DXVECTOR3 GetPosition(void) { return m_Info.pos; }
	D3DXVECTOR3 GetRotation(void) { return m_Info.rot; }

private:	// �����������A�N�Z�X�\

	// �����o�֐�
	void Controller(void);
	void Move(void);
	void Rotation(void);
	void Adjust(void);
	void KeyBoardRotation(void);

	// �����o�ϐ�
	SInfo m_Info;			// �������g�̏��
	CObjectX *m_pObject;	// �`��I�u�W�F�N�g
	float m_fRotMove;		// ���݂̊p�x
	float m_fRotDiff;		// �ړI�̊p�x
	float m_fRotDest;		// �p�x�v�Z
	float m_fStepCoolTime;	// �X�e�b�v�N�[���^�C��
};

#endif

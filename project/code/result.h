//===============================================
//
// ���U���g��ʂ̊Ǘ����� [result.h]
// Author : Ibuki Okusada
//
//===============================================
#ifndef _RESULT_H_		// ���̃}�N������`����Ă��Ȃ��ꍇ
#define _RESULT_H_		// ��d�C���N���[�h�h�~�p�}�N�����`

#include "manager.h"

class CScore;
class CFileLoad;
class CMeshDome;
class CTime;

//===============================================
// �^�C�g���N���X�̒�`(�h���N���X)
//===============================================
class CResult : public CScene
{
public:

	// �����o�֐�
	CResult();	// �R���X�g���N�^
	~CResult();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void SetScore(int nNum) { m_nScore = nNum; }

private:

	CFileLoad *m_pFileLoad;	// �t�@�C���ǂݍ��݂̃|�C���^
	CMeshDome *m_pMeshSky;		// ��p
	CTime *m_pTime;
	CScore *m_pScore;			// �X�R�A
	int m_nTimer;				// �J�ڃ^�C�}�[
	static int m_nScore;		// ����̃X�R�A
};

#endif
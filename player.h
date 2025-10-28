//==============================================
//
// �v���C���[�̊��N���X [player.h]
// Author:YUTO YOSHIDA
//
//==============================================

//**********************************************
// ���d�C���N���[�h�h�~
//**********************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//**********************************************
// �C���N���[�h�t�@�C��
//**********************************************
#include "object.h"

//**********************************************
// �O���錾
//**********************************************
class CCharacter3D;
class CTransform;

//**********************************************
// �v���C���[�N���X�̒�`
//**********************************************
class CPlayer : public CObject
{
public:

	// �v���C���[�̎��
	typedef enum
	{
		TYPE_BASE = 0, // ���N���X
		TYPE_GAME,	   // �Q�[��
		TYPE_MAX
	}TYPE;

	// ���[�V�����̎��
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0, // �j���[�g����
		MOTIONTYPE_MOVE,		// �ړ�
		MOTIONTYPE_ACTION,		// �A�N�V����
		MOTIONTYPE_JUMP,		// �W�����v
		MOTIONTYPE_LANDING,		// ���n
		MOTIONTYPE_MAX
	}MOTIONTYPE;

	CPlayer();
	~CPlayer();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	static CPlayer* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const char* pMotionTxt, const int nNumMotion);

	CCharacter3D* GetCharacter(void) { return m_pCharacter; }
private:
	CCharacter3D* m_pCharacter;		// �L�����N�^�[�N���X�ւ̃|�C���^

	CTransform* m_pTransformRef;	// �Q�Ɨp��ԏ��
	D3DXVECTOR3 m_move;				// �ړ���
	float m_fMove;					// �ړ��X�s�[�h
	float m_fRotDest;				// �ړI�̌���
};
#endif

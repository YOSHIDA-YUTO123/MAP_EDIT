//================================================
//
// ��ԃp�^�[����� [statebase.h]
// Author: YUTO YOSHIDA
//
//================================================

//*************************************************
// ���d�C���N���[�h�h�~
//*************************************************
#ifndef _STATEBASE_H_
#define _STATEBASE_H_

//************************************************
// �C���N���[�h�t�@�C��
//************************************************
#include<memory>

//************************************************
// ��ԃN���X�̒�`
//************************************************
class CStateBase
{
public:
	CStateBase();
	~CStateBase();
	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	virtual void Uninit(void) = 0;
	virtual int GetID(void) const = 0;
private:
};

//************************************************
// ��ԃ}�V�[���N���X�̒�`
//************************************************
class CStateMachine
{
public:
	CStateMachine();
	~CStateMachine();
	void Change(std::shared_ptr<CStateBase> pNewState);
	void Update(void);
private:
	std::shared_ptr<CStateBase> m_pNowState; // ���݂̏�ԃ|�C���^
};

#endif
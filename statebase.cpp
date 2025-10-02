//================================================
//
// ��ԃp�^�[����� [statebase.cpp]
// Author: YUTO YOSHIDA
//
//================================================

//************************************************
// �C���N���[�h�t�@�C��
//************************************************
#include "statebase.h"

using namespace std; // ���O���std���g�p

//================================================
// �R���X�g���N�^
//================================================
CStateBase::CStateBase()
{
}

//================================================
// �f�X�g���N�^
//================================================
CStateBase::~CStateBase()
{
}

//================================================
// �R���X�g���N�^
//================================================
CStateMachine::CStateMachine()
{
}

//================================================
// �f�X�g���N�^
//================================================
CStateMachine::~CStateMachine()
{
}

//================================================
// ��Ԃ̕ύX
//================================================
void CStateMachine::Change(shared_ptr<CStateBase> pNewState)
{
	if (m_pNowState != nullptr)
	{
		// ��Ԃ������Ȃ�
		if (m_pNowState->GetID() == pNewState->GetID())
		{
			// �j��
			pNewState.reset();
			return;
		}
	}

	// ���̏�Ԃ𐶐�
	if (m_pNowState != nullptr)
	{
		m_pNowState->Uninit();
		m_pNowState.reset();
		m_pNowState = nullptr;
	}

	// ��Ԃ̐ݒ�
	m_pNowState = pNewState;

	m_pNowState->Init();
}

//================================================
// ��Ԃ̍X�V����
//================================================
void CStateMachine::Update(void)
{
	// �X�V����
	if (m_pNowState != nullptr)
	{
		m_pNowState->Update();
	}
}
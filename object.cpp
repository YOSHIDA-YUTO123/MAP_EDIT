//===================================================
//
// �I�u�W�F�N�g�̕`�揈�� [object.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// �C���N���[�h�t�@�C��
//***************************************************
#include "object.h"
#include "camera.h"
#include "manager.h"
#include "pause.h"

using namespace Const; // ���O���Const���g�p

//***************************************************
// �ÓI�����o�ϐ��̐錾
//***************************************************
CObject* CObject::m_pTop[NUM_PRIORITY] = { nullptr }; // �擪�I�u�W�F�N�g�ւ̃|�C���^
CObject* CObject::m_pCur[NUM_PRIORITY] = { nullptr }; // �Ō���I�u�W�F�N�g�ւ̃|�C���^
int CObject::m_nNumAll[NUM_PRIORITY] = { NULL };	  // �I�u�W�F�N�g�̑���

//===================================================
// �R���X�g���N�^(�I�[�o�[���[�h)
//===================================================
CObject::CObject(int nPriority)
{
	m_type = TYPE_NONE;
	m_nPriority = nPriority;
	m_bDeath = false;

	m_pPrev = nullptr;
	m_pNext = nullptr;

	// �擪��null��������
	if (m_pTop[nPriority] == nullptr)
	{
		// �擪��ݒ�
		m_pTop[nPriority] = this;
		m_pCur[nPriority] = this;
	}
	else
	{
		// �������X�g�̍Ō���ɒǉ�
		m_pPrev = m_pCur[nPriority];
		m_pCur[nPriority]->m_pNext = this;
		m_pCur[nPriority] = this;
	}

	// �������J�E���g
	m_nNumAll[nPriority]++;
}

//===================================================
// �f�X�g���N�^
//===================================================
CObject::~CObject()
{
}

//===================================================
// ���ׂẴI�u�W�F�N�g�̔j��
//===================================================
void CObject::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		// �擪�I�u�W�F�N�g����
		CObject* pObject = m_pTop[nCntPriority]; 

		// null����Ȃ��Ȃ�
		while (pObject != nullptr)
		{
			// ���̃I�u�W�F�N�g�̃|�C���^����
			CObject* pObjectNext = pObject->m_pNext; 

			// �X�V����
			pObject->Uninit();

			// ���̃I�u�W�F�N�g����
			pObject = pObjectNext; 
		}
	}

	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		// �擪�I�u�W�F�N�g����
		CObject* pObject = m_pTop[nCntPriority];

		// null����Ȃ��Ȃ�
		while (pObject != nullptr)
		{
			// ���̃I�u�W�F�N�g�̃|�C���^����
			CObject* pObjectNext = pObject->m_pNext;

			// ���S�t���O�������Ă�����
			if (pObject->m_bDeath == true)
			{
				// �I�u�W�F�N�g�̔j��
				pObject->Destroy(pObject);
			}
			// ���̃I�u�W�F�N�g����
			pObject = pObjectNext;
		}
	}

}
//===================================================
// ���ׂẴI�u�W�F�N�g�̍X�V����
//===================================================
void CObject::UpdateAll(void)
{
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		// �擪�I�u�W�F�N�g����
		CObject* pObject = m_pTop[nCntPriority]; 

		// null����Ȃ��Ȃ�
		while (pObject != nullptr)
		{
			// ���̃I�u�W�F�N�g�̃|�C���^����
			CObject* pObjectNext = pObject->m_pNext;

			// �|�[�Y��Ԃ̎擾
			bool bPause = CPauseManager::GetPause();

			// �|�[�Y����������|�[�Y�ȊO�̃I�u�W�F�N�g�̏��������Ȃ�
			if (pObject->GetType() != TYPE_PAUSE && bPause == true)
			{
				// ���̃I�u�W�F�N�g����
				pObject = pObjectNext;

				continue;
			}

			// �X�V����
			pObject->Update();

			// ���̃I�u�W�F�N�g����
			pObject = pObjectNext;
		}
	}

	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		// �擪�I�u�W�F�N�g����
		CObject* pObject = m_pTop[nCntPriority];

		// null����Ȃ��Ȃ�
		while (pObject != nullptr)
		{
			// ���̃I�u�W�F�N�g�̃|�C���^����
			CObject* pObjectNext = pObject->m_pNext;

			// ���S�t���O�������Ă�����
			if (pObject->m_bDeath == true)
			{
				// �I�u�W�F�N�g�̔j��
				pObject->Destroy(pObject);
			}
			// ���̃I�u�W�F�N�g����
			pObject = pObjectNext;
		}
	}
}
//===================================================
// ���ׂẴI�u�W�F�N�g�̕`�揈��
//===================================================
void CObject::DrawAll(void)
{
	// �J�����̎擾
	CCamera* pCamera = CManager::GetCamera();

	if (pCamera != nullptr)
	{
		pCamera->SetCamera();
	}

	// �D�揇�ʂ̐����������ׂ�
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		CObject* pObject = m_pTop[nCntPriority]; // �擪�I�u�W�F�N�g����

		// null����Ȃ��Ȃ�
		while (pObject != nullptr)
		{
			CObject* pObjectNext = pObject->m_pNext; // ���̃I�u�W�F�N�g�̃|�C���^����

			// �|�[�Y��Ԃ̎擾
			bool bPause = CPauseManager::GetPause();

			// �I�u�W�F�N�g���|�[�Y�Ń|�[�Y������Ȃ��Ȃ�|�[�Y��`�悵�Ȃ�
			if (pObject->GetType() == TYPE_PAUSE && bPause == false)
			{
				pObject = pObjectNext; // ���̃I�u�W�F�N�g����

				// �������΂�
				continue;
			}

			// �X�V����
			pObject->Draw();

			pObject = pObjectNext; // ���̃I�u�W�F�N�g����
		}
	}
}

//===================================================
// ���S�t���O�������Ă��邷�ׂẴI�u�W�F�N�g�̔j��
//===================================================
void CObject::DestroyAll(void)
{
	for (int nCntPriority = 0; nCntPriority < NUM_PRIORITY; nCntPriority++)
	{
		// �擪�I�u�W�F�N�g����
		CObject* pObject = m_pTop[nCntPriority];

		// null����Ȃ��Ȃ�
		while (pObject != nullptr)
		{
			// ���̃I�u�W�F�N�g�̃|�C���^����
			CObject* pObjectNext = pObject->m_pNext;

			// ���S�t���O�������Ă�����
			if (pObject->m_bDeath == true)
			{
				// �I�u�W�F�N�g�̔j��
				pObject->Destroy(pObject);
			}
			// ���̃I�u�W�F�N�g����
			pObject = pObjectNext;
		}
	}
}

//===================================================
//���S�t���O�̐ݒ�
//===================================================
void CObject::Release(void)
{
	// ���S�t���O���I���ɂ���
	m_bDeath = true;
}

//===================================================
// �I�u�W�F�N�g�̔j��
//===================================================
void CObject::Destroy(CObject *pObject)
{
	// �����̗D�揇��
	int nPriority = m_nPriority;

	// �擪��������
	if (pObject == m_pTop[nPriority] && m_pNext != nullptr)
	{
		// �擪�����̃I�u�W�F�N�g�ɂ���
		m_pTop[nPriority] = m_pNext;
		m_pNext->m_pPrev = nullptr;
	}
	// �Ō����������
	if (pObject == m_pCur[nPriority] && m_pPrev != nullptr)
	{
		// �Ō�����ЂƂO�̃I�u�W�F�N�g�ɂ���
		m_pCur[nPriority] = m_pPrev;
		m_pPrev->m_pNext = nullptr;
	}
	// �Ō���ł��擪�ł�����������
	if (m_pPrev != nullptr && m_pNext != nullptr)
	{
		// �Ԃ��Ȃ���
		m_pPrev->m_pNext = m_pNext;
		m_pNext->m_pPrev = m_pPrev;
	}

	// �c�肪���������������
	if (m_pTop[nPriority] == pObject)
	{
		m_pTop[nPriority] = nullptr;
	}

	// �c�肪���������������
	if (m_pCur[nPriority] == pObject)
	{
		m_pCur[nPriority] = nullptr;
	}

	m_pNext = nullptr;
	m_pPrev = nullptr;

	// �I�u�W�F�N�g�̐������炷
	m_nNumAll[nPriority]--;

	delete pObject;

	pObject = nullptr;
}

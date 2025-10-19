//================================================
//
// �R���C�_�[������3D�I�u�W�F�N�g�̏���������N���X [CollisionObject3D.cpp]
// Author: YUTO YOSHIDA
//
//================================================

//************************************************
// �C���N���[�h�t�@�C��
//************************************************
#include "CollisionObject3D.h"
#include "transform.h"
#include "Collider.h"

//================================================
// �R���X�g���N�^
//================================================
CCollisionObject3D::CCollisionObject3D()
{

}

//================================================
// �f�X�g���N�^
//================================================
CCollisionObject3D::~CCollisionObject3D()
{
	
}

//================================================
// ����������
//================================================
HRESULT CCollisionObject3D::Init(void)
{
	if (m_pTransform == nullptr)
	{
		// ��ԏ��̐���
		m_pTransform = std::make_unique<CTransform>();

		// ���s������
		if (m_pTransform == nullptr)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//================================================
// �I������
//================================================
void CCollisionObject3D::Uninit(void)
{
	// �v�f�𒲂ׂ�
	for (auto& collider : m_pCollider)
	{
		// �R���C�_�[�̔j��
		if (collider != nullptr)
		{
			delete collider;
			collider = nullptr;
		}
	}

	if (m_pTransform != nullptr)
	{
		// �j��
		m_pTransform.reset();
	}

	// �������g�̔j��
	CObject::Release();
}

//================================================
// �X�V����
//================================================
void CCollisionObject3D::Update(void)
{
	// �v�f�𒲂ׂ�
	for (auto& collider : m_pCollider)
	{
		if (collider != nullptr)
		{
			// �X�V����
			collider->Update();
		}
	}
}

//================================================
// �`�揈��
//================================================
void CCollisionObject3D::Draw(void)
{
	if (m_pTransform != nullptr)
	{
		// ���[���h�}�g���b�N�X�̐ݒ菈��
		m_pTransform->SetMatrix();
	}
}

//================================================
// �R���C�_�[�̒ǉ�
//================================================
void CCollisionObject3D::AddCollider(CCollider* pCollider)
{
	if (pCollider != nullptr)
	{
		// ��ԏ��̐ݒ�
		pCollider->SetTransform(m_pTransform.get());
	}
	else
	{
		MessageBox(NULL, "�G���[", "�R���C�_�[��ǉ��ł��܂���ł���", MB_OK);
		return;
	}

	m_pCollider.push_back(pCollider);
}

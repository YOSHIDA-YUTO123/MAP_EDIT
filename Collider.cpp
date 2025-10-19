//================================================
//
// �R���C�_�[ [collider.cpp]
// Author: YUTO YOSHIDA
//
//================================================

//************************************************
// �C���N���[�h�t�@�C��
//************************************************
#include "Collider.h"
#include "object.h"
#include "math.h"
#include "transform.h"

//================================================
// �R���X�g���N�^
//================================================
CCollider::CCollider()
{
	m_pTransform = nullptr;
	m_type = TYPE_AABB;
}

//================================================
// �R���X�g���N�^
//================================================
CCollider::CCollider(const TYPE type)
{
	m_pTransform = nullptr;
	m_type = type;
}

//================================================
// �f�X�g���N�^
//================================================
CCollider::~CCollider()
{
	m_pTransform = nullptr;
}

//================================================
// �I������(�����Ŕj��)
//================================================
void CCollider::DeleteTransform(void)
{
	// �������g�̔j��
	if (m_pTransform != nullptr)
	{
		delete m_pTransform;
		m_pTransform = nullptr;
	}
}

//================================================
// �R���X�g���N�^
//================================================
CColliderAABB::CColliderAABB() : CCollider(TYPE_AABB)
{

}

//================================================
// �f�X�g���N�^
//================================================
CColliderAABB::~CColliderAABB()
{
}

//================================================
// �X�V����
//================================================
void CColliderAABB::Update(void)
{
}

//================================================
// �R���C�_�[�̍쐬����
//================================================
std::unique_ptr<CColliderAABB> CColliderAABB::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 posOld, const D3DXVECTOR3 Size)
{
	// AABB�̐���
	std::unique_ptr<CColliderAABB> pAABB = std::make_unique<CColliderAABB>();

	// ��ԏ��̎擾
	CTransform *pTransform = pAABB->GetTransform();

	// �擾�ł��Ȃ��Ȃ珈�����Ȃ�
	if (pTransform == nullptr) return nullptr;

	// ���̎擾
	CTransform::Info info = pTransform->GetInfo();

	// �ݒ菈��
	info.pos = pos;
	info.Size = Size;
	info.posOld = posOld;

	// ���̐ݒ�
	pTransform->SetInfo(info);

	return pAABB;
}

//================================================
// �R���C�_�[�̐�������
//================================================
CColliderAABB CColliderAABB::CreateCollider(const D3DXVECTOR3 pos, const D3DXVECTOR3 posOld, const D3DXVECTOR3 Size)
{
	// AABB
	CColliderAABB aabb;

	// ��ԏ��̎擾
	CTransform* pTransform = aabb.GetTransform();

	// ���̎擾
	CTransform::Info info = pTransform->GetInfo();

	// �ݒ菈��
	info.pos = pos;
	info.Size = Size;
	info.posOld = posOld;

	// ���̐ݒ�
	pTransform->SetInfo(info);

	return aabb;
}

//================================================
// �R���X�g���N�^
//================================================
CColliderSphere::CColliderSphere() : CCollider(TYPE_SPHERE)
{
}

//================================================
// �f�X�g���N�^
//================================================
CColliderSphere::~CColliderSphere()
{
}

//================================================
// �X�V����
//================================================
void CColliderSphere::Update(void)
{

}

//================================================
// �~�̃R���C�_�[�̍쐬
//================================================
std::unique_ptr<CColliderSphere> CColliderSphere::Create(const D3DXVECTOR3 pos, const float fRadius, CTransform* pTransform)
{
	// sphere�̐���
	std::unique_ptr<CColliderSphere> pSphere = std::make_unique<CColliderSphere>();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pTransform == nullptr) return nullptr;

	// ��ԏ��̎擾
	pSphere->SetTransform(pTransform);

	// ���̎擾
	CTransform::Info info = pTransform->GetInfo();

	// �ݒ菈��
	info.pos = pos;
	info.fRadius = fRadius;

	// ���̐ݒ�
	pTransform->SetInfo(info);

	return pSphere;
}

//================================================
// ���|�C���^�Ő�������
//================================================
CColliderSphere* CColliderSphere::CreateRawPtr(const D3DXVECTOR3 pos, const float fRadius, CTransform* pTransform)
{
	// sphere�̐���
	CColliderSphere* pSphere = new CColliderSphere;

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pTransform == nullptr) return nullptr;

	// ��ԏ��̎擾
	pSphere->SetTransform(pTransform);

	// ���̎擾
	CTransform::Info info = pTransform->GetInfo();

	// �ݒ菈��
	info.pos = pos;
	info.fRadius = fRadius;

	// ���̐ݒ�
	pTransform->SetInfo(info);

	return pSphere;
}

//================================================
// �R���C�_�[�̍쐬����
//================================================
CColliderSphere CColliderSphere::CreateCollider(const D3DXVECTOR3 pos, const float fRadius)
{
	// �R���C�_�[
	CColliderSphere sphere;

	// ��ԏ��̎擾
	CTransform* pTransform = CTransform::Create();

	// ��ԏ��̎擾
	sphere.SetTransform(pTransform);

	// ���̎擾
	CTransform::Info info = pTransform->GetInfo();

	// �ݒ菈��
	info.pos = pos;
	info.fRadius = fRadius;

	// ���̐ݒ�
	pTransform->SetInfo(info);

	return sphere;
}

//================================================
// �R���X�g���N�^
//================================================
CColliderFOV::CColliderFOV() : CCollider(TYPE_FOV)
{
	// �������̃N���A
	ZeroMemory(&m_Data, sizeof(m_Data));
}

//================================================
// �f�X�g���N�^
//================================================
CColliderFOV::~CColliderFOV()
{
}

//================================================
// ��������
//================================================
std::unique_ptr<CColliderFOV> CColliderFOV::Create(const D3DXVECTOR3 pos, const float fAngle, const float fAngleLeft, const float fAngleRight, const float fLength)
{
	// ���E�̐���
	std::unique_ptr<CColliderFOV> pFOV = std::make_unique<CColliderFOV>();

	// ��ԏ��̎擾
	CTransform* pTransform = pFOV->GetTransform();

	// ���̎擾
	CTransform::Info info = pTransform->GetInfo();

	// �ݒ菈��
	info.pos = pos;
	pFOV->m_Data.fAngleLeft = fAngleLeft;
	pFOV->m_Data.fAngleRight = fAngleRight;
	pFOV->m_Data.fLength = fLength;
	pFOV->m_Data.fNowAngle = fAngle;

	// ���̐ݒ�
	pTransform->SetInfo(info);

	return pFOV;
}

//================================================
// �R���C�_�[�̐���
//================================================
CColliderFOV CColliderFOV::CreateCollider(const D3DXVECTOR3 pos, const float fAngle, const float fAngleLeft, const float fAngleRight, const float fLength)
{
	// ���E��
	CColliderFOV FOV = {};

	// ��ԏ��̎擾
	CTransform* pTransform = FOV.GetTransform();

	// ���̎擾
	CTransform::Info info = pTransform->GetInfo();

	// �ݒ菈��
	info.pos = pos;
	FOV.m_Data.fAngleLeft = fAngleLeft;
	FOV.m_Data.fAngleRight = fAngleRight;
	FOV.m_Data.fLength = fLength;
	FOV.m_Data.fNowAngle = fAngle;

	// ���̐ݒ�
	pTransform->SetInfo(info);

	return FOV;
}

//================================================
// �X�V����
//================================================
void CColliderFOV::Update(void)
{
	// ��ԏ��̎擾
	CTransform* pTransform = CCollider::GetTransform();

	if (pTransform != nullptr)
	{
		// ��ԏ��̎擾
		CTransform::Info info = pTransform->GetInfo();

		// ���݂̎����̌���
		m_Data.fNowAngle = info.rot.y;
	}
}

//================================================
// �R���X�g���N�^
//================================================
CColliderCapsule::CColliderCapsule() : CCollider(TYPE_CAPSULE)
{
	ZeroMemory(&m_Data, sizeof(m_Data));
	m_LocalEnd = Const::VEC3_NULL;
	m_LocalStart = Const::VEC3_NULL;
}

//================================================
// �f�X�g���N�^
//================================================
CColliderCapsule::~CColliderCapsule()
{
}

//================================================
// ��������
//================================================
std::unique_ptr<CColliderCapsule> CColliderCapsule::Create(const D3DXVECTOR3 StartPos, const D3DXVECTOR3 EndPos, const float fRadius)
{
	// �J�v�Z���̐���
	auto pCapsule = std::make_unique<CColliderCapsule>();

	// �ݒ菈��
	pCapsule->m_LocalStart = StartPos;
	pCapsule->m_LocalEnd = EndPos;
	pCapsule->m_Data.fRadius = fRadius;
	return pCapsule;
}

//================================================
// �R���C�_�[�̍쐬����
//================================================
CColliderCapsule CColliderCapsule::CreateCollider(const D3DXVECTOR3 StartPos, const D3DXVECTOR3 EndPos, const float fRadius)
{
	CColliderCapsule capsule = {};

	// �ݒ菈��
	capsule.m_Data.StartPos = StartPos;
	capsule.m_Data.EndPos = EndPos;
	capsule.m_Data.fRadius = fRadius;

	return capsule;
}

//================================================
// �X�V����
//================================================
void CColliderCapsule::Update(void)
{
	// ��ԏ��̎擾
	CTransform* pTransform = CCollider::GetTransform();

	if (pTransform != nullptr)
	{
		// ��ԏ��̎擾
		CTransform::Info info = pTransform->GetInfo();

		// �I�_�Ǝn�_�̋�Ԃ̈ʒu�����߂�
		D3DXVec3TransformCoord(&m_Data.StartPos, &m_LocalStart, &info.mtxWorld);
		D3DXVec3TransformCoord(&m_Data.EndPos, &m_LocalEnd, &info.mtxWorld);
	}

}
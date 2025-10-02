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
#include"object.h"
#include"math.h"

using namespace Const; // ���O���Const���g�p
using namespace std;   // ���O���std�������g�p
using namespace math;  // ���O���math�������g�p

//================================================
// �R���X�g���N�^
//================================================
CCollider::CCollider()
{
}

//================================================
// �f�X�g���N�^
//================================================
CCollider::~CCollider()
{
}

//================================================
// �R���X�g���N�^
//================================================
CColliderAABB::CColliderAABB()
{
	// �l�̃N���A
	memset(&m_Data, NULL, sizeof(m_Data));
}

//================================================
// �f�X�g���N�^
//================================================
CColliderAABB::~CColliderAABB()
{
}

//================================================
// �R���C�_�[�̍쐬����
//================================================
unique_ptr<CColliderAABB> CColliderAABB::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 posOld, const D3DXVECTOR3 Size)
{
	// AABB�̐���
	unique_ptr<CColliderAABB> pAABB = make_unique<CColliderAABB>();

	// �ݒ菈��
	pAABB->m_pos = pos;
	pAABB->m_Data.Size = Size;
	pAABB->m_Data.posOld = posOld;

	return pAABB;
}

//================================================
// �R���C�_�[�̐�������
//================================================
CColliderAABB CColliderAABB::CreateCollider(const D3DXVECTOR3 pos, const D3DXVECTOR3 posOld, const D3DXVECTOR3 Size)
{
	// AABB
	CColliderAABB aabb;

	// �ݒ菈��
	aabb.m_pos = pos;
	aabb.m_Data.posOld = posOld;
	aabb.m_Data.Size = Size;

	return aabb;
}

//================================================
// �f�[�^�̍X�V����
//================================================
void CColliderAABB::UpdateData(const D3DXVECTOR3 pos, const D3DXVECTOR3 posOld)
{
	m_pos = pos;
	m_Data.posOld = posOld;
}

////================================================
//// �R���X�g���N�^
////================================================
//CColliderSphere::CColliderSphere()
//{
//	m_fRadius = NULL;
//}
//
////================================================
//// �f�X�g���N�^
////================================================
//CColliderSphere::~CColliderSphere()
//{
//}
//
////================================================
//// �R���C�_�[�̍쐬����
////================================================
//CColliderSphere* CColliderSphere::CreateCollider(const float fRadius)
//{
//	// �R���C�_�[�̍쐬����
//	CColliderSphere* pSphere;
//
//	// sphere�̐���
//	pSphere = new CColliderSphere;
//
//	pSphere->m_fRadius = fRadius;
//
//	return pSphere;
//}
//
////================================================
//// �R���X�g���N�^
////================================================
//CColliderFOV::CColliderFOV()
//{
//	m_fAngleLeft = NULL;
//	m_fAngleRight = NULL;
//	m_fLength = NULL;
//	m_fNowAngle = NULL;
//}
//
////================================================
//// �f�X�g���N�^
////================================================
//CColliderFOV::~CColliderFOV()
//{
//
//}
//
////================================================
//// �R���C�_�[�̐���
////================================================
//CColliderFOV* CColliderFOV::CreateCollider(const float fAngle, const float fAngleLeft, const float fAngleRight,const float fLength)
//{
//	// �R���C�_�[�̍쐬����
//	CColliderFOV* pFOV;
//
//	// ���E�̐���
//	pFOV = new CColliderFOV;
//	pFOV->m_fAngleLeft = fAngleLeft;
//	pFOV->m_fAngleRight = fAngleRight;
//	pFOV->m_fLength = fLength;
//	pFOV->m_fNowAngle = fAngle;
//
//	return pFOV;
//}

//================================================
// �R���X�g���N�^
//================================================
CColliderSphere::CColliderSphere()
{
	m_fRadius = NULL;
}

//================================================
// �f�X�g���N�^
//================================================
CColliderSphere::~CColliderSphere()
{
}

//================================================
// �~�̃R���C�_�[�̍쐬
//================================================
unique_ptr<CColliderSphere> CColliderSphere::Create(const D3DXVECTOR3 pos, const float fRadius)
{
	// sphere�̐���
	unique_ptr<CColliderSphere> pSphere = make_unique<CColliderSphere>();

	// �ݒ菈��
	pSphere->m_pos = pos;
	pSphere->m_fRadius = fRadius;

	return pSphere;
}

//================================================
// �R���C�_�[�̍쐬����
//================================================
CColliderSphere CColliderSphere::CreateCollider(const D3DXVECTOR3 pos, const float fRadius)
{
	// �R���C�_�[
	CColliderSphere sphere;

	// �ݒ菈��
	sphere.m_pos = pos;
	sphere.m_fRadius = fRadius;

	return sphere;
}

//================================================
// �R���X�g���N�^
//================================================
CColliderFOV::CColliderFOV()
{
	// �������̃N���A
	memset(&m_Data, NULL, sizeof(m_Data));
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
unique_ptr<CColliderFOV> CColliderFOV::Create(const D3DXVECTOR3 pos, const float fAngle, const float fAngleLeft, const float fAngleRight, const float fLength)
{
	// ���E�̐���
	unique_ptr<CColliderFOV> pFOV = make_unique<CColliderFOV>();

	// �ݒ菈��
	pFOV->m_pos = pos;
	pFOV->m_Data.fAngleLeft = fAngleLeft;
	pFOV->m_Data.fAngleRight = fAngleRight;
	pFOV->m_Data.fLength = fLength;
	pFOV->m_Data.fNowAngle = fAngle;

	return pFOV;
}

//================================================
// �R���C�_�[�̐���
//================================================
CColliderFOV CColliderFOV::CreateCollider(const D3DXVECTOR3 pos, const float fAngle, const float fAngleLeft, const float fAngleRight, const float fLength)
{
	// ���E��
	CColliderFOV FOV = {};

	// �ݒ菈��
	FOV.m_pos = pos;
	FOV.m_Data.fAngleLeft = fAngleLeft;
	FOV.m_Data.fAngleRight = fAngleRight;
	FOV.m_Data.fLength = fLength;
	FOV.m_Data.fNowAngle = fAngle;

	return FOV;
}

//================================================
// �R���X�g���N�^
//================================================
CColliderCapsule::CColliderCapsule()
{
	ZeroMemory(&m_Data, sizeof(m_Data));
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
unique_ptr<CColliderCapsule> CColliderCapsule::Create(const D3DXVECTOR3 StartPos, const D3DXVECTOR3 EndPos, const float fRadius, const int nID)
{
	// �J�v�Z���̐���
	auto pCapsule = make_unique<CColliderCapsule>();

	// �ݒ菈��
	pCapsule->m_Data.StartPos = StartPos;
	pCapsule->m_Data.EndPos = EndPos;
	pCapsule->m_Data.fRadius = fRadius;
	pCapsule->m_Data.nID = nID;
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
// �R���C�_�[�̍X�V
//================================================
void CColliderCapsule::UpdateData(const Data data)
{
	m_Data = data;
}

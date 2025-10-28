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
#include "line.h"
#include "debugproc.h"

//================================================
// �R���X�g���N�^
//================================================
CCollider::CCollider() :
	m_col(Const::WHITE),
	m_pTransform(nullptr),
	m_type(TYPE_BASE),
	m_tag()
{
}

//================================================
// �R���X�g���N�^
//================================================
CCollider::CCollider(const TYPE type) : 
	m_pTransform(nullptr),
	m_tag(),
	m_type(type),
	m_col(Const::WHITE)
{
}

//================================================
// �f�X�g���N�^
//================================================
CCollider::~CCollider()
{
	m_tag.clear();
	m_pTransform = nullptr;
}

//================================================
// �X�V����
//================================================
void CCollider::Update(void)
{
	m_col = m_bHit ? D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) : Const::WHITE;

	DrawLine();
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
	// �X�V����
	CCollider::Update();
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
// ��������
//================================================
CColliderAABB* CColliderAABB::Create(void)
{
	CColliderAABB* pinstance = new CColliderAABB;

	return pinstance;
}

//================================================
// ���̕`��
//================================================
void CColliderAABB::DrawLine(void)
{
	// ���̕`��֐��̎擾
	CLineRenderer* pLine = CLineRenderer::GetInstance();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pLine == nullptr) return;

	// ��ԏ��̎擾
	CTransform* pTransform = CCollider::GetTransform();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pTransform == nullptr) return;

	// ���̎擾
	CTransform::Info info = pTransform->GetInfo();

	// �F�̐ݒ�
	D3DXCOLOR col = CCollider::GetLineColor();

	D3DXVECTOR3 Size = info.Size * 0.5f;
	D3DXVECTOR3 pos = info.pos;

	D3DXVECTOR3 vtx[8] =
	{
		{ pos.x - Size.x, pos.y - Size.y,pos.z - Size.z },
		{ pos.x + Size.x, pos.y - Size.y,pos.z - Size.z },
		{ pos.x + Size.x, pos.y + Size.y,pos.z - Size.z },
		{ pos.x - Size.x, pos.y + Size.y,pos.z - Size.z },

		{ pos.x - Size.x, pos.y - Size.y,pos.z + Size.z },
		{ pos.x + Size.x, pos.y - Size.y,pos.z + Size.z },
		{ pos.x + Size.x, pos.y + Size.y,pos.z + Size.z },
		{ pos.x - Size.x, pos.y + Size.y,pos.z + Size.z },
	};

	pLine->AddLine(vtx[0], vtx[1], col);
	pLine->AddLine(vtx[1], vtx[2], col);
	pLine->AddLine(vtx[2], vtx[3], col);
	pLine->AddLine(vtx[3], vtx[0], col);

	pLine->AddLine(vtx[4], vtx[5], col);
	pLine->AddLine(vtx[5], vtx[6], col);
	pLine->AddLine(vtx[6], vtx[7], col);
	pLine->AddLine(vtx[7], vtx[4], col);

	pLine->AddLine(vtx[0], vtx[4], col);
	pLine->AddLine(vtx[1], vtx[5], col);
	pLine->AddLine(vtx[2], vtx[6], col);
	pLine->AddLine(vtx[3], vtx[7], col);
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
	// �X�V����
	CCollider::Update();
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
// ���̕`��
//================================================
void CColliderSphere::DrawLine(void)
{
	// ���̕`��֐��̎擾
	CLineRenderer* pLine = CLineRenderer::GetInstance();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pLine == nullptr) return;

	// ��ԏ��̎擾
	CTransform* pTransform = CCollider::GetTransform();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pTransform == nullptr) return;

	// ���̎擾
	CTransform::Info info = pTransform->GetInfo();

	const int nSegX = 8;
	const int nSegZ = 8;
	const float fRadius = info.fRadius;

	// �ʒu
	D3DXVECTOR3 pos = info.pos;

	// Z�̕�������
	for (int nCntZ = 0; nCntZ <= nSegZ; nCntZ++)
	{
		// X�̕�������
		for (int nCntX = 0; nCntX <= nSegX; nCntX++)
		{
			// �p�x�̊������v�Z
			float fRateAngleX = (D3DX_PI * 2.0f) / nSegX;
			float fRateAngleZ = D3DX_PI / nSegZ;

			// �n�_�̊p�x�̌v�Z
			float fStartAngleX = fRateAngleX * nCntX;
			float fStartAngleZ = fRateAngleZ * nCntZ;

			// �I�_�̊p�x�̌v�Z
			float fEndAngleX = fRateAngleX * (nCntX + 1);

			D3DXVECTOR3 start,end;

			// �n�_�̌v�Z
			start.x = pos.x + sinf(fStartAngleZ) * sinf(fStartAngleX) * fRadius;
			start.y = pos.y + cosf(fStartAngleZ) * fRadius;
			start.z = pos.z + sinf(fStartAngleZ) * cosf(fStartAngleX) * fRadius;

			// �I�_�̌v�Z
			end.x = pos.x + sinf(fStartAngleZ) * sinf(fEndAngleX) * fRadius;
			end.y = pos.y + cosf(fStartAngleZ) * fRadius;
			end.z = pos.z + sinf(fStartAngleZ) * cosf(fEndAngleX) * fRadius;

			// ���̕`��
			pLine->AddLine(start, end, CCollider::GetLineColor());
		}
	}
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
std::unique_ptr<CColliderFOV> CColliderFOV::Create(const float fAngle, const float fAngleLeft, const float fAngleRight, const float fLength)
{
	// ���E�̐���
	std::unique_ptr<CColliderFOV> pFOV = std::make_unique<CColliderFOV>();

	// �ݒ菈��
	pFOV->m_Data.fAngleLeft = fAngleLeft;
	pFOV->m_Data.fAngleRight = fAngleRight;
	pFOV->m_Data.fLength = fLength;
	pFOV->m_Data.fNowAngle = fAngle;

	return pFOV;
}

//================================================
// ��������(���|�C���^)
//================================================
CColliderFOV* CColliderFOV::CreateRawPtr(const float fAngle, const float fAngleLeft, const float fAngleRight, const float fLength)
{
	// ���E�̐���
	CColliderFOV* pFOV = new CColliderFOV;

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

	// �X�V����
	CCollider::Update();
}

//================================================
// ���̕`��
//================================================
void CColliderFOV::DrawLine(void)
{
	// ���̕`��֐��̎擾
	CLineRenderer* pLine = CLineRenderer::GetInstance();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pLine == nullptr) return;

	// ��ԏ��̎擾
	CTransform* pTransform = CCollider::GetTransform();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pTransform == nullptr) return;

	// ���̎擾
	CTransform::Info info = pTransform->GetInfo();

	D3DXVECTOR3 pos = info.pos;
	D3DXVECTOR3 posLeft,posRight;

	// ���E�̍����������߂�
	posLeft.x = pos.x + sinf(info.rot.y + m_Data.fAngleLeft) * m_Data.fLength;
	posLeft.y = pos.y;
	posLeft.z = pos.z + cosf(info.rot.y + m_Data.fAngleLeft) * m_Data.fLength;

	// ���̕`��
	pLine->AddLine(pos, posLeft, { 1.0f,1.0f,1.0f,1.0f });

	// ���E�̉E���������߂�
	posRight.x = pos.x + sinf(info.rot.y + m_Data.fAngleRight) * m_Data.fLength;
	posRight.y = pos.y;
	posRight.z = pos.z + cosf(info.rot.y + m_Data.fAngleRight) * m_Data.fLength;

	// ���̕`��
	pLine->AddLine(pos, posRight, { 1.0f,1.0f,1.0f,1.0f });

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
// ��������(���|�C���^)
//================================================
CColliderCapsule* CColliderCapsule::CreateRawPtr(const D3DXVECTOR3 StartPos, const D3DXVECTOR3 EndPos)
{
	// �J�v�Z���̐���
	auto pCapsule = new CColliderCapsule;

	// �ݒ菈��
	pCapsule->m_LocalStart = StartPos;
	pCapsule->m_LocalEnd = EndPos;

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

	// �X�V����
	CCollider::Update();
}

//================================================
// ���̕`�揈��
//================================================
void CColliderCapsule::DrawLine(void)
{
	// ���̕`��֐��̎擾
	CLineRenderer* pLine = CLineRenderer::GetInstance();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pLine == nullptr) return;

	// ��ԏ��̎擾
	CTransform* pTransform = CCollider::GetTransform();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pTransform == nullptr) return;

	// ���̎擾
	CTransform::Info info = pTransform->GetInfo();

	const int nSegX = 8;
	const int nSegZ = 8;
	const float fRadius = info.fRadius;

	// �ʒu
	D3DXVECTOR3 pos = info.pos;

	// Z�̕�������
	for (int nCntZ = 0; nCntZ <= nSegZ; nCntZ++)
	{
		// X�̕�������
		for (int nCntX = 0; nCntX <= nSegX; nCntX++)
		{
			float fRateAngleX = (D3DX_PI * 2.0f) / nSegX;

			float fAngleDestX = fRateAngleX * (nCntX + 1);
			float fAngleX = fRateAngleX * nCntX;

			D3DXVECTOR3 start, end;

			start.x = info.pos.x + sinf(fAngleX) * fRadius;
			start.y = info.pos.y + m_Data.EndPos.y - (m_Data.EndPos.y / nSegZ) * nCntZ;
			start.z = info.pos.z + cosf(fAngleX) * fRadius;

			end.x = info.pos.x + sinf(fAngleDestX) * fRadius;
			end.y = info.pos.y + m_Data.EndPos.y - (m_Data.EndPos.y / nSegZ) * nCntZ;
			end.z = info.pos.z + cosf(fAngleDestX) * fRadius;

			// ���̕`��
			pLine->AddLine(start, end, Const::WHITE);
		}
	}
}

//================================================
// �R���X�g���N�^
//================================================
CColliderOBB::CColliderOBB() : 
	CCollider(TYPE_OBB)
{
}

//================================================
// �f�X�g���N�^
//================================================
CColliderOBB::~CColliderOBB()
{
}

//================================================
// ��������
//================================================
CColliderOBB* CColliderOBB::Create(void)
{
	CColliderOBB* pInstance = new CColliderOBB;

	return pInstance;
}

//================================================
// �X�V����
//================================================
void CColliderOBB::Update(void)
{
	// �X�V����
	CCollider::Update();
}

//================================================
// ���̕`��
//================================================
void CColliderOBB::DrawLine(void)
{
	// ���̕`��֐��̎擾
	CLineRenderer* pLine = CLineRenderer::GetInstance();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pLine == nullptr) return;

	// ��ԏ��̎擾
	CTransform* pTransform = CCollider::GetTransform();

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pTransform == nullptr) return;

	// ���̎擾
	CTransform::Info info = pTransform->GetInfo();

	// �F�̐ݒ�
	D3DXCOLOR col = CCollider::GetLineColor();

	D3DXVECTOR3 Size = info.Size * 0.5f;
	D3DXVECTOR3 pos = info.pos;

	D3DXVECTOR3 vtx[8] =
	{
		{ - Size.x,  - Size.y, - Size.z },
		{ + Size.x,  - Size.y, - Size.z },
		{ + Size.x,  + Size.y, - Size.z },
		{ - Size.x,  + Size.y, - Size.z },

		{ - Size.x,  - Size.y, + Size.z },
		{ + Size.x,  - Size.y, + Size.z },
		{ + Size.x,  + Size.y, + Size.z },
		{ - Size.x,  + Size.y, + Size.z },
	};

	for (auto& v : vtx)
	{
		D3DXVec3TransformCoord(&v, &v, &info.mtxWorld);
	}

	pLine->AddLine(vtx[0], vtx[1], col);
	pLine->AddLine(vtx[1], vtx[2], col);
	pLine->AddLine(vtx[2], vtx[3], col);
	pLine->AddLine(vtx[3], vtx[0], col);

	pLine->AddLine(vtx[4], vtx[5], col);
	pLine->AddLine(vtx[5], vtx[6], col);
	pLine->AddLine(vtx[6], vtx[7], col);
	pLine->AddLine(vtx[7], vtx[4], col);

	pLine->AddLine(vtx[0], vtx[4], col);
	pLine->AddLine(vtx[1], vtx[5], col);
	pLine->AddLine(vtx[2], vtx[6], col);
	pLine->AddLine(vtx[3], vtx[7], col);
}

//================================================
//
// �����蔻�� [collision.cpp]
// Author: YUTO YOSHIDA
//
//================================================

//************************************************
// �C���N���[�h�t�@�C��
//************************************************
#include "Collision.h"
#include"math.h"
#include"Collider.h"
#include "transform.h"

using namespace Const; // ���O���Const���g�p
using namespace math; // ���O��Ԃ��g�p
using namespace std; // ���O���std�������g�p

constexpr float HALF_VALUE = 0.5f; // ����

//================================================
// �R���X�g���N�^
//================================================
CCollision::CCollision(TYPE type) : m_type(type)
{
	m_pos = VEC3_NULL;
}

//================================================
// �f�X�g���N�^
//================================================
CCollision::~CCollision()
{
}

//================================================
// �����蔻��̐�������
//================================================
CCollision* CCollision::Create(const D3DXVECTOR3 pos, const TYPE type)
{
	// �����蔻��̃|�C���^
	CCollision* pCollision = nullptr;

	//// ��ނ̑J��
	//switch (type)
	//{
	//case TYPE::TYPE_AABB:
	//	pCollision = new CCollisionAABB;
	//	break;
	//case TYPE::TYPE_SPHERE:
	//	pCollision = new CCollisionSphere;
	//	break;
	//case TYPE::TYPE_FOV:
	//	pCollision = new CCollisionFOV;
	//	break;
	//default:
	//	break;
	//}

	// �ʒu�̐ݒ�
	pCollision->m_pos = pos;

	// ��ނ���
	pCollision->m_type = type;

	// �����Ԃ�
	return pCollision;
}

//================================================
// �R���X�g���N�^
//================================================
CCollisionAABB::CCollisionAABB() : CCollision(TYPE::TYPE_AABB)
{
}

//================================================
// �f�X�g���N�^
//================================================
CCollisionAABB::~CCollisionAABB()
{
}

//================================================
// AABB��AABB�̓����蔻��
//================================================
bool CCollisionAABB::Collision(CCollider* pMyBox, CCollider* pTargetBox, D3DXVECTOR3* pushPos,int* pFace)
{
	// �����̋�ԏ��ւ̃|�C���^�̎擾
	CTransform* pMyTransform = pMyBox->GetTransform();

	// ����̋�ԏ��ւ̃|�C���^�̎擾
	CTransform* pOtherTransform = pTargetBox->GetTransform();

	// �����̋�ԏ��̎擾
	CTransform::Info myInfo = {};
	CTransform::Info otherInfo = {};

	if (pMyTransform != nullptr)
	{
		// ���̎擾
		myInfo = pMyTransform->GetInfo();
	}

	if (pOtherTransform != nullptr)
	{
		// ���̎擾
		otherInfo = pOtherTransform->GetInfo();
	}

	// �����̈ʒu�Ƒ傫���̎擾
	D3DXVECTOR3 pos = myInfo.pos;

	// ����̈ʒu�Ƒ傫���̎擾
	D3DXVECTOR3 TargetPos = otherInfo.pos;

	// �^�[�Q�b�g�̑傫��
	D3DXVECTOR3 TargetSize = otherInfo.Size;
	D3DXVECTOR3 TargetPosOld = otherInfo.posOld;

	// �����̑O��̈ʒu
	D3DXVECTOR3 posOldMin = myInfo.posOld - (myInfo.Size * HALF_VALUE);
	D3DXVECTOR3 posOldMax = myInfo.posOld + (myInfo.Size * HALF_VALUE);

	// �����̈ʒu
	D3DXVECTOR3 posMin = pos - (myInfo.Size * HALF_VALUE);
	D3DXVECTOR3 posMax = pos + (myInfo.Size * HALF_VALUE);

	// �^�[�Q�b�g�̑O��̈ʒu(t�̓^�[�Q�b�g��t)
	D3DXVECTOR3 tPosOldMin = TargetPosOld - (TargetSize * HALF_VALUE);
	D3DXVECTOR3 tPosOldMax = TargetPosOld + (TargetSize * HALF_VALUE);

	// �^�[�Q�b�g�̈ʒu(t�̓^�[�Q�b�g��t)
	D3DXVECTOR3 tPosMin = TargetPos - (TargetSize * HALF_VALUE);
	D3DXVECTOR3 tPosMax = TargetPos + (TargetSize * HALF_VALUE);

	// Y�͈̔͂ɓ����Ă��邩
	const bool bCheckRangeY = posOldMin.y <= tPosOldMax.y && posOldMax.y >= tPosOldMin.y;

	// �͈͊O��������
	if (!bCheckRangeY)
	{
		return false;
	}

	// Z�͈͓̔��ɓ����Ă���
	if (tPosMin.z < posMax.z && tPosMax.z > posMin.z)
	{
		// ������E�ɂ߂荞��
		if (tPosOldMin.x >= posOldMax.x &&
			tPosMin.x < posMax.x)
		{
			// ���������ʂ�ݒ�
			if (pFace != nullptr)
			{
				// ���ɂ߂荞��
				*pFace = FACE_LEFT;
			}

			// �����o����ݒ�
			if (pushPos != nullptr)
			{
				// �߂荞�񂾕��߂�
				pushPos->x = tPosMin.x - myInfo.Size.x * HALF_VALUE - 0.1f;
			}
			return true;
		}
		// �E���獶�ɂ߂荞��
		else if (tPosOldMax.x <= posOldMin.x &&
			tPosMax.x > posMin.x)
		{
			// ���������ʂ�ݒ�
			if (pFace != nullptr)
			{
				// �E�ɂ߂荞��
				*pFace = FACE_RIGHT;
			}

			// �����o����ݒ�
			if (pushPos != nullptr)
			{
				// �߂荞�񂾕��߂�
				pushPos->x = tPosMax.x + myInfo.Size.x * HALF_VALUE + 0.1f;
			}
			return true;
		}
	}

	// x�͈̔͂ɓ����Ă���
	if (tPosMin.x < posMax.x && tPosMax.x > posMin.x)
	{
		// �������߂荞��(������W�n)
		if (tPosOldMax.z <= posOldMin.z &&
			tPosMax.z > posMin.z)
		{
			// ���������ʂ�ݒ�
			if (pFace != nullptr)
			{
				// �������߂荞��
				*pFace = FACE_BACK;
			}

			// �����o����ݒ�
			if (pushPos != nullptr)
			{
				// �߂荞�񂾕��߂�
				pushPos->z = tPosMax.z + myInfo.Size.z * HALF_VALUE + 0.1f;
			}
			return true;
		}
		else if (tPosOldMin.z >= posOldMax.z &&
			tPosMin.z < posMax.z)
		{
			// ���������ʂ�ݒ�
			if (pFace != nullptr)
			{
				// �O������߂荞��
				*pFace = FACE_FRONT;
			}

			if (pushPos != nullptr)
			{
				// �߂荞�񂾕��߂�
				pushPos->z = tPosMin.z - myInfo.Size.z * HALF_VALUE - 0.1f;
			}
			return true;
		}
	}

	//if (pos.x - m_Size.x * HALF_VALUE <= otherPos.x + otherSize.x * HALF_VALUE
	//	&& pos.x + m_Size.x * HALF_VALUE >= otherPos.x - otherSize.x * HALF_VALUE)
	//{
	//	if (pos.z - m_Size.z * HALF_VALUE <= otherPos.z + otherSize.z * HALF_VALUE
	//		&& pos.z + m_Size.z * HALF_VALUE >= otherPos.z - otherSize.z * HALF_VALUE)
	//	{
	//		// �ォ�牺
	//		if (m_posOld.y >= otherPos.y + otherSize.y
	//			&& pos.y < otherPos.y + otherSize.y)
	//		{
	//			bHit = true;

	//			//bLanding = true;
	//			//pos.y = otherPos.y + otherSize.y * g_Block[nCntBlock].Scal.y - Size.y;
	//			//pMove->y = 0.0f;
	//		}
	//		// �������
	//		else if (m_posOld.y + m_Size.y * HALF_VALUE <= otherPos.y - otherSize.y * HALF_VALUE
	//			&& pos.y + m_Size.y * HALF_VALUE > otherPos.y - otherSize.y * HALF_VALUE)
	//		{
	//			bHit = true;

	//			//pos.y = m_posOld.y;
	//			//pMove->y = 0.0f;
	//		}

	//	}
	//}

	return false;
}

//================================================
// �R���X�g���N�^
//================================================
CCollisionSphere::CCollisionSphere() : CCollision(TYPE::TYPE_SPHERE)
{
}

//================================================
// �f�X�g���N�^
//================================================
CCollisionSphere::~CCollisionSphere()
{
}

//================================================
// �����蔻��(�~vs�~)
//================================================
bool CCollisionSphere::Collision(CCollider* myCollider, CCollider* otherCollider)
{
	// �����̋�ԏ��ւ̃|�C���^�̎擾
	CTransform* pMyTransform = myCollider->GetTransform();

	// ����̋�ԏ��ւ̃|�C���^�̎擾
	CTransform* pOtherTransform = otherCollider->GetTransform();

	// �����̋�ԏ��̎擾
	CTransform::Info myInfo = {};
	CTransform::Info otherInfo = {};

	if (pMyTransform != nullptr)
	{
		// ���̎擾
		myInfo = pMyTransform->GetInfo();
	}

	if (pOtherTransform != nullptr)
	{
		// ���̎擾
		otherInfo = pOtherTransform->GetInfo();
	}

	// �ʒu�̎擾
	D3DXVECTOR3 pos = myInfo.pos;

	// ����̈ʒu
	D3DXVECTOR3 otherPos = otherInfo.pos;

	// ����̔��a
	float fOtherRadius = otherInfo.fRadius;

	// �����̔��a
	float fMyRadius = myInfo.fRadius;

	// ���������߂�
	D3DXVECTOR3 diff = otherPos - pos;

	// ���������߂�
	float fDistance = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);

	// ���a����
	float fRadius = fMyRadius + fOtherRadius;

	// ���a��2�悷��
	fRadius = fRadius * fRadius;

	// ���������a�ȉ��������瓖�����Ă���
	if (fDistance <= fRadius)
	{
		return true;
	}

	return false;
}

//================================================
// �R���X�g���N�^
//================================================
CCollisionFOV::CCollisionFOV() : CCollision(TYPE::TYPE_FOV)
{
}

//================================================
// �f�X�g���N�^
//================================================
CCollisionFOV::~CCollisionFOV()
{
}

//================================================
// ���E�̔���
//================================================
bool CCollisionFOV::Collision(const D3DXVECTOR3 otherpos, CCollider* pFOV)
{
	// �����̋�ԏ��ւ̃|�C���^�̎擾
	CTransform* pTransform = pFOV->GetTransform();

	// �����̋�ԏ��̎擾
	CTransform::Info Info = {};

	if (pTransform != nullptr)
	{
		// ���̎擾
		Info = pTransform->GetInfo();
	}

	// �ʒu�̎擾
	D3DXVECTOR3 objectPos = Info.pos;

	// �O���܂ł̃x�N�g��
	D3DXVECTOR3 vecFront = GetVector(otherpos, objectPos);

	// �f�[�^�̎擾
	const CColliderFOV::Data *pData = static_cast<const CColliderFOV::Data*>(pFOV->GetData());

	// �f�[�^���擾�ł��Ȃ������珈�����Ȃ�
	if (pData == nullptr) return false;

	D3DXVECTOR3 LeftPos; // ���̈ʒu

	// �����̎��E�̒[�̈ʒu�����߂�
	LeftPos.x = objectPos.x + sinf(pData->fNowAngle + pData->fAngleLeft) * pData->fLength;
	LeftPos.y = 0.0f;
	LeftPos.z = objectPos.z + cosf(pData->fNowAngle + pData->fAngleLeft) * pData->fLength;

	// �����̎��E�̃x�N�g���̍쐬
	D3DXVECTOR3 VecLeft = GetVector(LeftPos, objectPos);

	D3DXVECTOR3 RightPos; // �E�̈ʒu

	// �E���̎��E�̒[�̈ʒu�����߂�
	RightPos.x = objectPos.x + sinf(pData->fNowAngle + pData->fAngleRight) * pData->fLength;
	RightPos.y = 0.0f;
	RightPos.z = objectPos.z + cosf(pData->fNowAngle + pData->fAngleRight) * pData->fLength;

	// �E���̎��E�̃x�N�g���̍쐬
	D3DXVECTOR3 VecRight = GetVector(RightPos, objectPos);

	D3DXVECTOR3 Cross0,Cross1; // ����p�O�σx�N�g��

	// �O�ς��o��
	D3DXVec3Cross(&Cross0, &VecLeft, &vecFront);
	D3DXVec3Cross(&Cross1, &vecFront, &VecRight);

	// 2�{�Ƃ��������������
	if (Cross0.y >= 0.0f && Cross1.y >= 0.0f)
	{
		return true;
	}

	return false;
}

//================================================
// �J�v�Z���R���C�_�[�̓����蔻��
//================================================
bool CCollisionCapsule::Collision(CCollider* myCapsule, CCollider* otherCapsule,D3DXVECTOR3 *NearPos1, D3DXVECTOR3* NearPos2)
{
	// �J�v�Z���R���C�_�[�̃f�[�^�\���̂��g�p
	using Data = CColliderCapsule::Data;

	// �����̃f�[�^�̎擾
	const Data* pMyData = static_cast<const Data*>(myCapsule->GetData());

	// ����̃f�[�^�̎擾
	const Data* pOtherData = static_cast<const Data*>(otherCapsule->GetData());

	// �f�[�^�̎擾���o���Ȃ������珈�����Ȃ�
	if (pMyData == nullptr || pOtherData == nullptr) return false;

	float param1, param2; // �p�����[�^�[1,2

	D3DXVECTOR3 closestPos1, closestPos2; // �ŋߐړ_1,2

	// ����1�Ɛ���2�̋����̎擾
	float fDistance = ClosestPtSegmentSegment(
		pMyData->StartPos,
		pMyData->EndPos,
		pOtherData->StartPos,
		pOtherData->EndPos,
		&param1,
		&param2,
		&closestPos1,
		&closestPos2);

	// ���������߂�
	fDistance = sqrtf(fDistance);

	if (NearPos1 != nullptr)
	{
		// �ŋߐړ_1��n��
		*NearPos1 = closestPos1;
	}
	if (NearPos2 != nullptr)
	{
		// �ŋߐړ_2��n��
		*NearPos2 = closestPos2;
	}

	// ��ԏ��̎擾
	CTransform* pMyTransform = myCapsule->GetTransform();
	CTransform* pOtherTransform = myCapsule->GetTransform();

	// �f�[�^�̎擾���o���Ȃ������珈�����Ȃ�
	if (pMyTransform == nullptr || pOtherTransform == nullptr) return false;

	// �����̋�ԏ��̎擾
	CTransform::Info myInfo = pMyTransform->GetInfo();
	CTransform::Info otherInfo = pMyTransform->GetInfo();

	// ��̉~�̔��a��n��
	float fRadius = myInfo.fRadius + otherInfo.fRadius;

	// ���������a�ȉ��������瓖�����Ă���
	if (fDistance <= fRadius)
	{
		return true;
	}

	return false;
}

//================================================
// �~�Ƃ̓����蔻��
//================================================
bool CCollisionCapsule::CollisionSphere(CCollider* pCapsule, CCollider* pSphere)
{
	// �J�v�Z���̋�ԏ��ւ̃|�C���^�̎擾
	CTransform* pCapsuleTransform = pCapsule->GetTransform();

	// �~�̋�ԏ��ւ̃|�C���^�̎擾
	CTransform* pSphereTransform = pSphere->GetTransform();

	// �����̋�ԏ��̎擾
	CTransform::Info capsuleInfo = {};
	CTransform::Info sphereInfo = {};

	if (pCapsuleTransform != nullptr)
	{
		// ���̎擾
		capsuleInfo = pCapsuleTransform->GetInfo();
	}

	if (pSphereTransform != nullptr)
	{
		// ���̎擾
		sphereInfo = pSphereTransform->GetInfo();
	}

	// �J�v�Z���R���C�_�[�̃f�[�^�\���̂��g�p
	using CapsuleData = CColliderCapsule::Data;

	// �����̃f�[�^�̎擾
	const CapsuleData *pCapsuleData = static_cast<const CapsuleData*>(pCapsule->GetData());

	// �擾�ł��Ȃ������珈�����Ȃ�
	if (pCapsuleData == nullptr) return false;

	// �~�̈ʒu�̎擾
	D3DXVECTOR3 spherePos = sphereInfo.pos;

	// �~�̔��a�̎擾
	float fSphereRadius = sphereInfo.fRadius;

	float param1, param2; // �p�����[�^�[1,2

	D3DXVECTOR3 closestPos1, closestPos2; // �ŋߐړ_1,2

	// �����Ɖ~�̋��������߂�
	float fDistance = ClosestPtSegmentSegment(
		pCapsuleData->StartPos,
		pCapsuleData->EndPos,
		spherePos,
		spherePos,
		&param1,
		&param2,
		&closestPos1,
		&closestPos2);

	// ���������߂�
	fDistance = sqrtf(fDistance);

	// ��ԏ��̎擾
	CTransform* pCapTransform = pCapsule->GetTransform();

	// �f�[�^�̎擾���o���Ȃ������珈�����Ȃ�
	if (pCapTransform == nullptr) return false;

	// �����̋�ԏ��̎擾
	CTransform::Info myInfo = pCapTransform->GetInfo();

	// ��̉~�̔��a��n��
	float fRadius = myInfo.fRadius + fSphereRadius;

	// ���������a�ȉ��������瓖�����Ă���
	if (fDistance <= fRadius)
	{
		return true;
	}

	return false;
}

//================================================
// �R���X�g���N�^
//================================================
CCollisionCapsule::CCollisionCapsule() : CCollision(TYPE::TYPE_CAPSULE)
{

}

//================================================
// �����̐����̋���
//================================================
float CCollisionCapsule::ClosestPtSegmentSegment(D3DXVECTOR3 Start1, D3DXVECTOR3 End1, D3DXVECTOR3 Start2, D3DXVECTOR3 End2, float* pParam1, float* pParam2, D3DXVECTOR3* c1osestPos1, D3DXVECTOR3* c1osestPos2)
{
	D3DXVECTOR3 dir1 = Start1 - End1; // ����S1�̃x�N�g��
	D3DXVECTOR3 dir2 = Start2 - End2; // ����S2�̃x�N�g��
	D3DXVECTOR3 diffStart = Start1 - Start2;

	float fLengthSq1 = D3DXVec3Dot(&dir1, &dir1); // ����S1�̕����x�N�g��
	float fLengthSq2 = D3DXVec3Dot(&dir2, &dir2); // ����S2�̕����x�N�g��
	
	float dotVec2DiffStart = D3DXVec3Dot(&dir2, &diffStart);

	const float EPSILON = 1e-6f; // �`�F�b�N�p

	// �Е����邢�͗����̐������_�ɑ΂��ďk�ނ��Ă��邩�ǂ����`�F�b�N
	if (fLengthSq1 <= EPSILON && fLengthSq2 <= EPSILON)
	{
		// �����̐������_�ɏk��
		*pParam1 = *pParam2 = 0.0f;
		*c1osestPos1 = Start1;
		*c1osestPos2 = Start2;

		D3DXVECTOR3 diff = *c1osestPos1 - *c1osestPos2;
		
		return D3DXVec3Dot(&diff, &diff);
	}

	if (fLengthSq1 <= EPSILON)
	{
		// �ŏ��̐������_�ɏk��
		*pParam1 = 0.0f;
		*pParam2 = dotVec2DiffStart / fLengthSq2; // s = o >= t = (b*s + f) / e = f / e
		*pParam2 = Clamp(*pParam2, 0.0f, 1.0f);
	}
	else
	{
		float dotDir1Diff = D3DXVec3Dot(&dir1, &diffStart);

		if (fLengthSq2 <= EPSILON)
		{
			// 2�Ԗڂ̐������_�ɏk��
			*pParam2 = 0.0f;
			*pParam1 = Clamp(-dotDir1Diff / fLengthSq1, 0.0f, 1.0f); // t = 0 >= s = (b * t - c) / a = -c / a
		}
		else
		{
			float dotDir1Dir2 = D3DXVec3Dot(&dir1, &dir2);
			float denom = fLengthSq1 * fLengthSq2 - dotDir1Dir2 * dotDir1Dir2; // ��ɔ�

			// ���������s�ł͂Ȃ��ꍇ�AL1���L2�ɑ΂���ŋߐړ_���v�Z�A�����Đ���1�ɑ΂��ăN�����v�B�����łȂ��ꍇ�͔C�ӂ�s��I��
			if (denom != 0.0f)
			{
				*pParam1 = Clamp((dotDir1Dir2 * dotVec2DiffStart - dotDir1Diff * fLengthSq2) / denom, 0.0f, 1.0f);
			}
			else
			{
				*pParam1 = 0.0f;
			}
			// L2���S1(s)�ɑ΂���ŋߐړ_���ȉ���p���Čv�Z
			// t = dot((P1 + D1 * s) - P2,P2) / dot(D2,D2) = (b * s + f) / e
			*pParam2 = (dotDir1Dir2 * *pParam1 + dotVec2DiffStart) / fLengthSq2;
			// t��[0,1]�̒��ɂ���ΏI���B�����Ȃ�΂����N�����v�A�������̐V�����l�ɑ΂��Ĉȉ���p���Čv�Z
			// s = Dot((P2 + D2 * t) - P1,D1) / Dot(D1,D1) = (t * b - c) / a
			// ������s��[0,1]�ɑ΂��ăN�����v
			if (*pParam2 < 0.0f)
			{
				*pParam2 = 0.0f;
				*pParam1 = Clamp(-dotDir1Diff / fLengthSq1, 0.0f, 1.0f);
			}
			else if (*pParam2 > 1.0f)
			{
				*pParam2 = 1.0f;
				*pParam1 = Clamp((dotDir1Dir2 - dotDir1Diff) / fLengthSq1, 0.0f, 1.0f);
			}
		}
	}

	*c1osestPos1 = Start1 + dir1 * *pParam1;
	*c1osestPos2 = Start2 + dir2 * *pParam2;

	D3DXVECTOR3 diff = *c1osestPos1 - *c1osestPos2;

	// ������2��
	float fDistance = D3DXVec3Dot(&diff, &diff);

	return fDistance;
}

//================================================
// �f�X�g���N�^
//================================================
CCollisionCapsule::~CCollisionCapsule()
{
}

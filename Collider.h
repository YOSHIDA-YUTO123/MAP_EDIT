//================================================
//
// �R���C�_�[ [collider.h]
// Author: YUTO YOSHIDA
//
//================================================

//************************************************
// ���d�C���N���[�h�h�~
//************************************************
#ifndef _COLLIDER_H_
#define _COLLIDER_H_

//************************************************
// �C���N���[�h�t�@�C��
//************************************************
#include"main.h"
#include<memory>

//************************************************
// �R���C�_�[�N���X�̒�`
//************************************************
class CCollider
{
public:
	CCollider();
	virtual ~CCollider();
	void SetPosition(const D3DXVECTOR3 pos) { m_pos = pos; }

	D3DXVECTOR3 GetPos(void) const { return m_pos; }
protected:
	D3DXVECTOR3 m_pos; // �ʒu
private:
};

//************************************************
// AABB�̃R���C�_�[�N���X�̒�`
//************************************************
class CColliderAABB : public CCollider
{
public:

	// �����蔻��ɕK�v�ȃf�[�^(�O��̈ʒu,�傫��...)
	struct Data
	{
		D3DXVECTOR3 posOld; // �O��̈ʒu
		D3DXVECTOR3 Size;	// �傫��
	};

	CColliderAABB();
	~CColliderAABB();
	static std::unique_ptr<CColliderAABB> Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 posOld, const D3DXVECTOR3 Size);
	static CColliderAABB CreateCollider(const D3DXVECTOR3 pos, const D3DXVECTOR3 posOld, const D3DXVECTOR3 Size);
	void UpdateData(const D3DXVECTOR3 pos, const D3DXVECTOR3 posOld);
	Data GetData(void) { return m_Data; }
private:
	Data m_Data; // �f�[�^
};

//************************************************
// Sphere�̃R���C�_�[�N���X�̒�`
//************************************************
class CColliderSphere : public CCollider
{
public:
	CColliderSphere();
	~CColliderSphere();
	static std::unique_ptr<CColliderSphere> Create(const D3DXVECTOR3 pos,const float fRadius);
	static CColliderSphere CreateCollider(const D3DXVECTOR3 pos, const float fRadius);
	float GetRadius(void) const { return m_fRadius; }
	void SetRadius(const float fRadius) { m_fRadius = fRadius; }
private:
	float m_fRadius;	// ���a
};

//************************************************
// FOV�̃R���C�_�[�N���X�̒�`
//************************************************
class CColliderFOV : public CCollider
{
public:

	struct Data
	{
		float fLength;		// ���E�̒���
		float fNowAngle;	// ���̊p�x
		float fAngleLeft;	// ���썶
		float fAngleRight;	// ����E
	};

	CColliderFOV();
	~CColliderFOV();
	static std::unique_ptr<CColliderFOV> Create(const D3DXVECTOR3 pos,const float fAngle, const float fAngleLeft, const float fAngleRight,const float fLength);
	static CColliderFOV CreateCollider(const D3DXVECTOR3 pos, const float fAngle, const float fAngleLeft, const float fAngleRight, const float fLength);
	Data GetData(void) const { return m_Data; }
	void UpdateData(const float fNowAngle) { m_Data.fNowAngle = fNowAngle; }
private:
	Data m_Data; // �f�[�^
};

//************************************************
// �J�v�Z���R���C�_�[�N���X�̒�`
//************************************************
class CColliderCapsule : public CCollider
{
public:

	// �����蔻��̃f�[�^
	struct Data
	{
		D3DXVECTOR3 StartPos; // �n�_
		D3DXVECTOR3 EndPos;	// �I�_
		float fRadius;		// ���a
		int nID;			// �����̔ԍ�
	};

	CColliderCapsule();
	~CColliderCapsule();
	static std::unique_ptr<CColliderCapsule> Create(const D3DXVECTOR3 StartPos, const D3DXVECTOR3 EndPos, const float fRadius,const int nID);
	static CColliderCapsule CreateCollider(const D3DXVECTOR3 StartPos, const D3DXVECTOR3 EndPos, const float fRadius);
	Data GetData(void) const { return m_Data; }
	void UpdateData(const Data data);
private:
	Data m_Data; // �f�[�^
};
#endif
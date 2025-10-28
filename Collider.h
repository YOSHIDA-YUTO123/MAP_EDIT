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
#include <memory>
#include <string>

//************************************************
// �O���錾
//************************************************
class CTransform;

//************************************************
// �R���C�_�[�N���X�̒�`
//************************************************
class CCollider
{
public:

	// �R���C�_�[�̎��
	typedef enum
	{
		TYPE_BASE = 0,	// ���N���X
		TYPE_AABB,		// ��`
		TYPE_SPHERE,	// ��
		TYPE_FOV,		// ���E
		TYPE_CAPSULE,	// �J�v�Z��
		TYPE_OBB,		// ��]��`
		TYPE_MAX
	}TYPE;

	CCollider();
	CCollider(const TYPE type);
	virtual ~CCollider();

	// �f�[�^�擾
	virtual const void* GetData(void) const { return nullptr; }

	virtual void Update(void);

	CTransform* GetTransform(void) { return m_pTransform; }
	void SetTransform(CTransform* pTransform) { m_pTransform = pTransform; }
	void SetTag(const char* pTag) { m_tag = pTag; }

	void DeleteTransform(void);
	TYPE GetType(void) const { return m_type; }
	const char* GetTag(void) const { return m_tag.c_str(); }

	void SetResult(const bool bResult) { m_bHit = bResult; }
protected:
	D3DXCOLOR GetLineColor(void) const { return m_col; }
private:
	virtual void DrawLine(void) = 0;

	CTransform* m_pTransform;	// �g�����X�t�H�[���N���X�ւ̃|�C���^
	std::string m_tag;			// �R���C�_�[�̃^�O
	TYPE m_type;				// �R���C�_�[�̎��
	D3DXCOLOR m_col;			// �F
	bool m_bHit;				// �����������ǂ���
};

//************************************************
// AABB�̃R���C�_�[�N���X�̒�`
//************************************************
class CColliderAABB : public CCollider
{
public:

	CColliderAABB();
	~CColliderAABB();
	const void* GetData(void) const override { return nullptr; };
	void Update(void) override;

	static std::unique_ptr<CColliderAABB> Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 posOld, const D3DXVECTOR3 Size);
	static CColliderAABB CreateCollider(const D3DXVECTOR3 pos, const D3DXVECTOR3 posOld, const D3DXVECTOR3 Size);
	static CColliderAABB* Create(void);
private:
	void DrawLine(void) override;
};

//************************************************
// Sphere�̃R���C�_�[�N���X�̒�`
//************************************************
class CColliderSphere : public CCollider
{
public:
	CColliderSphere();
	~CColliderSphere();

	const void* GetData(void) const override { return nullptr; };
	void Update(void) override;

	static std::unique_ptr<CColliderSphere> Create(const D3DXVECTOR3 pos,const float fRadius, CTransform* pTransform);
	static CColliderSphere* CreateRawPtr(const D3DXVECTOR3 pos, const float fRadius, CTransform* pTransform);
	static CColliderSphere CreateCollider(const D3DXVECTOR3 pos, const float fRadius);
private:
	void DrawLine(void) override;
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
	static std::unique_ptr<CColliderFOV> Create(const float fAngle, const float fAngleLeft, const float fAngleRight,const float fLength);
	static CColliderFOV* CreateRawPtr(const float fAngle, const float fAngleLeft, const float fAngleRight, const float fLength);

	static CColliderFOV CreateCollider(const D3DXVECTOR3 pos, const float fAngle, const float fAngleLeft, const float fAngleRight, const float fLength);
	const void* GetData(void) const override { return &m_Data; }
	void Update(void) override;

private:
	void DrawLine(void) override;
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
	};

	CColliderCapsule();
	~CColliderCapsule();

	static std::unique_ptr<CColliderCapsule> Create(const D3DXVECTOR3 StartPos, const D3DXVECTOR3 EndPos, const float fRadius);
	static CColliderCapsule* CreateRawPtr(const D3DXVECTOR3 StartPos, const D3DXVECTOR3 EndPos);
	static CColliderCapsule CreateCollider(const D3DXVECTOR3 StartPos, const D3DXVECTOR3 EndPos, const float fRadius);
	const void* GetData(void) const override { return &m_Data; }
	void Update(void) override;

private:
	void DrawLine(void) override;
	Data m_Data;			  // �f�[�^
	D3DXVECTOR3 m_LocalStart; // �n�_�̈ʒu
	D3DXVECTOR3 m_LocalEnd;	  // �I�_�̈ʒu
};

//************************************************
// OBB�N���X�̒�`
//************************************************
class CColliderOBB : public CCollider
{
public:

	CColliderOBB();
	~CColliderOBB();

	static CColliderOBB* Create(void);
	const void* GetData(void) const override { return nullptr; }
	void Update(void) override;
private:
	void DrawLine(void) override;
};
#endif
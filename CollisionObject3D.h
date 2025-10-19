//================================================
//
// �R���C�_�[������3D�I�u�W�F�N�g�̏���������N���X [CollisionObject3D.h]
// Author: YUTO YOSHIDA
//
//================================================

//*************************************************
// ���d�C���N���[�h�h�~
//*************************************************
#ifndef _COLLISIONOBJECT3D_H_
#define _COLLISIONOBJECT3D_H_

//************************************************
// �C���N���[�h�t�@�C��
//************************************************
#include "object.h"
#include <memory>

//************************************************
// �O���錾
//************************************************
class CCollider;
class CTransform;

//************************************************
// �R���C�_�[�����I�u�W�F�N�g�̃N���X�̒�`
//************************************************
class CCollisionObject3D : public CObject
{
public:
	CCollisionObject3D();
	virtual ~CCollisionObject3D();

	virtual HRESULT Init(void) override;
	virtual void Uninit(void) override;
	virtual void Update(void) override;
	virtual void Draw(void) override;
	CTransform* GetTransform(void) { return m_pTransform.get(); }

	/// <summary>
	/// �R���C�_�[�̒ǉ�
	/// </summary>
	/// <param name="���������R���C�_�["></param>
	void AddCollider(CCollider* pCollider);
	std::vector<CCollider*>& GetCollider(void) { return m_pCollider; }

private:
	std::vector<CCollider*> m_pCollider;		// �R���C�_�[
	std::unique_ptr<CTransform> m_pTransform;	// ��ԏ��
};

#endif

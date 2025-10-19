//================================================
//
// �����蔻�� [collision.h]
// Author: YUTO YOSHIDA
//
//================================================

//************************************************
// ���d�C���N���[�h�h�~
//************************************************
#ifndef _COLLISION_H_
#define _COLLISION_H_

//************************************************
// �C���N���[�h�t�@�C��
//************************************************
#include"main.h"
#include"object.h"
#include<memory>

//************************************************
// �O���錾
//************************************************
class CCollider;

//************************************************
// �����蔻��̃N���X�̒�`
//************************************************
class CCollision
{
public:

	typedef enum
	{
		TYPE_AABB = 0,
		TYPE_SPHERE,
		TYPE_FOV,
		TYPE_CAPSULE,
		TYPE_MAX
	}TYPE;

	CCollision(TYPE type);
	virtual ~CCollision();

	static CCollision* Create(const D3DXVECTOR3 pos,const TYPE type);

	void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }

	D3DXVECTOR3 GetPosition(void) const { return m_pos; }
	TYPE GetType(void) const { return m_type; }
private:
	D3DXVECTOR3 m_pos; // �ʒu
	TYPE m_type;	   // �����蔻��̎��
};

//************************************************
// �����蔻��AABB�N���X
//************************************************
class CCollisionAABB : public CCollision
{
public:

	// �ʂ̎��
	typedef enum
	{
		FACE_ALL = 0,
		FACE_RIGHT,
		FACE_LEFT,
		FACE_FRONT,
		FACE_BACK,
		FACE_MAX
	}FACE;

	~CCollisionAABB();
	static bool Collision(CCollider* pMyBox, CCollider* pTargetBox, D3DXVECTOR3* pushPos = nullptr, int* pFace = nullptr);
private:
	CCollisionAABB();
};

//************************************************
// �����蔻��(�~)�N���X
//************************************************
class CCollisionSphere : public CCollision
{
public:
	~CCollisionSphere();
	static bool Collision(CCollider* myCollider, CCollider* otherCollider);
private:
	CCollisionSphere();
};

//************************************************
// �����蔻��(���E)�N���X
//************************************************
class CCollisionFOV : public CCollision
{
public:
	~CCollisionFOV();

	static bool Collision(const D3DXVECTOR3 otherpos, CCollider* pFOV);
private:
	CCollisionFOV();
};

//************************************************
// �����蔻��(�J�v�Z��)�N���X
//************************************************
class CCollisionCapsule : public CCollision
{
public:
	~CCollisionCapsule();

	/**
	* @brief �J�v�Z��vs�J�v�Z��
	* @param �����̃J�v�Z��
	* @param ����̃J�v�Z��
	* @param �����̍ŋߓ_
	* @param ����̍ŋߓ_
	* @return ����̌���
	*/
	static bool Collision(CCollider* myCapsule, CCollider* otherCapsule, D3DXVECTOR3* NearPos1 = nullptr, D3DXVECTOR3* NearPos2 = nullptr);

	/**
	* @brief �J�v�Z��vs�~
	* @param �����̃J�v�Z��
	* @param ����̉~
	* @return ����̌���
	*/
	static bool CollisionSphere(CCollider* pCapsule, CCollider* pSphere);
private:
	CCollisionCapsule();
	static float ClosestPtSegmentSegment(D3DXVECTOR3 Start1, D3DXVECTOR3 End1, D3DXVECTOR3 Start2, D3DXVECTOR3 End2, float* pParam1, float* pParam2, D3DXVECTOR3* c1osestPos1, D3DXVECTOR3* c1osestPos2);
};

#endif

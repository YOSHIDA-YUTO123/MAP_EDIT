//==============================================
//
// �v���C���[�ƃ}�b�v�I�u�W�F�N�g
// �̓����蔻�������N���X [CollisionSystem.cpp]
// 
// Author:YUTO YOSHIDA
//
//==============================================

//**********************************************
// �C���N���[�h�t�@�C��
//**********************************************
#include "CollisionPlayerToMapObject.h"
#include "player.h"
#include "MapObject.h"
#include "character3D.h"
#include "CollisionObject3D.h"
#include "Collision.h"
#include "Collider.h"
#include "transform.h"
#include "MapObjectManager.h"

//==============================================
// �R���X�g���N�^
//==============================================
CCollisionPlayerToMapObject::CCollisionPlayerToMapObject() :
	m_pPlayer(nullptr)
{
}

//==============================================
// �f�X�g���N�^
//==============================================
CCollisionPlayerToMapObject::~CCollisionPlayerToMapObject()
{
}

//==============================================
// ����������
//==============================================
void CCollisionPlayerToMapObject::Init(void)
{

}

//==============================================
// �X�V����
//==============================================
void CCollisionPlayerToMapObject::Update(void)
{
	bool bResult = false;

	// �}�b�v�I�u�W�F�N�g�̃}�l�[�W���[�̎擾
	CMapObjectManager *pMapObjectManager = CMapObjectManager::GetInstance();

	// �}�b�v�I�u�W�F�N�g�̃R���C�_�[�����ׂ�
	for (auto& mapObject : pMapObjectManager->GetList())
	{
		// �����蔻��
		bResult = Collision(mapObject);
	}

	// ���茋�ʂ̐ݒ�
	SetResult(bResult);
}

//==============================================
// �^�[�Q�b�g�̐ݒ�
//==============================================
void CCollisionPlayerToMapObject::SetTarget(CPlayer* pPlayer)
{
	m_pPlayer = pPlayer;
}

//==============================================
// �����蔻��
//==============================================
bool CCollisionPlayerToMapObject::Collision(CMapObject* pMapObject)
{
	bool bResult = false;

	// �L�����N�^�[�N���X�̎擾
	CCharacter3D* pPlayerCharacter = m_pPlayer->GetCharacter();

	// �����蔻��I�u�W�F�N�g�̃N���X�̎擾
	CCollisionObject3D* pPlayerCollisionObj = pPlayerCharacter->GetCollisionObject();

	// �R���C�_�[�̎擾
	for (auto& mapObjectCollider : pMapObject->GetCollider())
	{
		// �v���C���[�̃R���C�_�[�����ׂ�
		for (auto& playerCollider : pPlayerCollisionObj->GetCollider())
		{
			// �v���C���[�̋�ԏ��̎擾
			CTransform* pPlayerTransform = playerCollider->GetTransform();

			// ���̎擾
			CTransform::Info playerTransform = pPlayerTransform->GetInfo();

			// ��������
			if (CCollisionOBB::Collision(mapObjectCollider, playerCollider))
			{
				bResult = true;
			}
			else
			{
				bResult = false;
			}
		}
	}

	return bResult;
}

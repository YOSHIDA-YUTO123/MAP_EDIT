//==============================================
//
// �v���C���[�ƃ}�b�v�I�u�W�F�N�g
// �̓����蔻�������N���X [CollisionSystem.h]
// 
// Author:YUTO YOSHIDA
//
//==============================================

//**********************************************
// ���d�C���N���[�h�h�~
//**********************************************
#ifndef _COLLSION_PLAYER_TO_MAPOBJECT_H_
#define _COLLSION_PLAYER_TO_MAPOBJECT_H_

//**********************************************
// �C���N���[�h�t�@�C��
//**********************************************
#include "CollisionSystem.h"

//**********************************************
// �O���錾
//**********************************************
class CPlayer;
class CMapObject;

//**********************************************
// �v���C���[�ƃ}�b�v�I�u�W�F�N�g�̓����蔻�������N���X
//**********************************************
class CCollisionPlayerToMapObject : public CCollisionSystem
{
public:
	CCollisionPlayerToMapObject();
	~CCollisionPlayerToMapObject();

	void Init(void) override;
	void Update(void) override;
	void SetTarget(CPlayer* pPlayer);
private:
	bool Collision(CMapObject *pMapObject);

	CPlayer* m_pPlayer;
};

#endif

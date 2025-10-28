//==============================================
//
// �����蔻��̋@�\�̏���������N���X [CollisionSystem.h]
// Author:YUTO YOSHIDA
//
//==============================================

//**********************************************
// ���d�C���N���[�h�h�~
//**********************************************
#ifndef _COLLISIONSYTEM_H_
#define _COLLISIONSYTEM_H_

//**********************************************
// �C���N���[�h�t�@�C��
//**********************************************
#include "SystemBase.h"

//**********************************************
// �����蔻��̋@�\�̃N���X�̒�`
//**********************************************
class CCollisionSystem : public CSystemBase
{
public:
	CCollisionSystem();
	virtual ~CCollisionSystem();

	virtual void Init(void) override = 0;
	virtual void Update(void) override = 0;
	bool OnCollision(void) { return m_bResult; }
protected:
	void SetResult(const bool bResult) { m_bResult = bResult; }
private:
	bool m_bResult; // ���茋��
};
#endif
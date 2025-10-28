//==============================================
//
// 当たり判定の機能の処理をするクラス [CollisionSystem.h]
// Author:YUTO YOSHIDA
//
//==============================================

//**********************************************
// 多重インクルード防止
//**********************************************
#ifndef _COLLISIONSYTEM_H_
#define _COLLISIONSYTEM_H_

//**********************************************
// インクルードファイル
//**********************************************
#include "SystemBase.h"

//**********************************************
// 当たり判定の機能のクラスの定義
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
	bool m_bResult; // 判定結果
};
#endif
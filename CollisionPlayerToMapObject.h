//==============================================
//
// プレイヤーとマップオブジェクト
// の当たり判定をするクラス [CollisionSystem.h]
// 
// Author:YUTO YOSHIDA
//
//==============================================

//**********************************************
// 多重インクルード防止
//**********************************************
#ifndef _COLLSION_PLAYER_TO_MAPOBJECT_H_
#define _COLLSION_PLAYER_TO_MAPOBJECT_H_

//**********************************************
// インクルードファイル
//**********************************************
#include "CollisionSystem.h"

//**********************************************
// 前方宣言
//**********************************************
class CPlayer;
class CMapObject;

//**********************************************
// プレイヤーとマップオブジェクトの当たり判定をするクラス
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

//==============================================
//
// プレイヤーとマップオブジェクト
// の当たり判定をするクラス [CollisionSystem.cpp]
// 
// Author:YUTO YOSHIDA
//
//==============================================

//**********************************************
// インクルードファイル
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
// コンストラクタ
//==============================================
CCollisionPlayerToMapObject::CCollisionPlayerToMapObject() :
	m_pPlayer(nullptr)
{
}

//==============================================
// デストラクタ
//==============================================
CCollisionPlayerToMapObject::~CCollisionPlayerToMapObject()
{
}

//==============================================
// 初期化処理
//==============================================
void CCollisionPlayerToMapObject::Init(void)
{

}

//==============================================
// 更新処理
//==============================================
void CCollisionPlayerToMapObject::Update(void)
{
	/*ここヤバイ修正*/

	////bool bResult = false;

	////// マップオブジェクトのマネージャーの取得
	////CMapObjectManager *pMapObjectManager = CMapObjectManager::GetInstance();

	////// マップオブジェクトのコライダー分調べる
	////for (auto& mapObject : pMapObjectManager->GetList())
	////{
	////	// 当たり判定
	////	bResult = Collision(mapObject);
	////}

	////// 判定結果の設定
	////SetResult(bResult);
}

//==============================================
// ターゲットの設定
//==============================================
void CCollisionPlayerToMapObject::SetTarget(CPlayer* pPlayer)
{
	m_pPlayer = pPlayer;
}

//==============================================
// 当たり判定
//==============================================
bool CCollisionPlayerToMapObject::Collision(CMapObject* pMapObject)
{
	bool bResult = false;

	// キャラクタークラスの取得
	CCharacter3D* pPlayerCharacter = m_pPlayer->GetCharacter();

	// 当たり判定オブジェクトのクラスの取得
	CCollisionObject3D* pPlayerCollisionObj = pPlayerCharacter->GetCollisionObject();

	// コライダーの取得
	for (auto& mapObjectCollider : pMapObject->GetCollider())
	{
		// プレイヤーのコライダー分調べる
		for (auto& playerCollider : pPlayerCollisionObj->GetCollider())
		{
			// プレイヤーの空間情報の取得
			CTransform* pPlayerTransform = playerCollider->GetTransform();

			// 情報の取得
			CTransform::Info playerTransform = pPlayerTransform->GetInfo();

			// 当たった
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

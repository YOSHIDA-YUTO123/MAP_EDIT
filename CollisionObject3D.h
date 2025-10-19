//================================================
//
// コライダーを持つ3Dオブジェクトの処理をするクラス [CollisionObject3D.h]
// Author: YUTO YOSHIDA
//
//================================================

//*************************************************
// 多重インクルード防止
//*************************************************
#ifndef _COLLISIONOBJECT3D_H_
#define _COLLISIONOBJECT3D_H_

//************************************************
// インクルードファイル
//************************************************
#include "object.h"
#include <memory>

//************************************************
// 前方宣言
//************************************************
class CCollider;
class CTransform;

//************************************************
// コライダーをもつオブジェクトのクラスの定義
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
	/// コライダーの追加
	/// </summary>
	/// <param name="生成したコライダー"></param>
	void AddCollider(CCollider* pCollider);
	std::vector<CCollider*>& GetCollider(void) { return m_pCollider; }

private:
	std::vector<CCollider*> m_pCollider;		// コライダー
	std::unique_ptr<CTransform> m_pTransform;	// 空間情報
};

#endif

//================================================
//
// コライダーを持つ3Dオブジェクトの処理をするクラス [CollisionObject3D.cpp]
// Author: YUTO YOSHIDA
//
//================================================

//************************************************
// インクルードファイル
//************************************************
#include "CollisionObject3D.h"
#include "transform.h"
#include "Collider.h"

//================================================
// コンストラクタ
//================================================
CCollisionObject3D::CCollisionObject3D(const int nPriority) : CObject(nPriority)
{

}

//================================================
// デストラクタ
//================================================
CCollisionObject3D::~CCollisionObject3D()
{
	
}

//================================================
// 生成処理
//================================================
CCollisionObject3D* CCollisionObject3D::Create(std::unique_ptr<CTransform> pTransform, CCollider* pCollider, const char* pColliderTag)
{
	CCollisionObject3D* pInstance = new CCollisionObject3D;

	if (pInstance->m_pTransform == nullptr)
	{
		// 空間情報の生成
		pInstance->m_pTransform = std::move(pTransform);
	}

	if (pCollider != nullptr)
	{
		// コライダーの追加
		pInstance->AddCollider(pCollider, pColliderTag);
	}

	// 初期化情報
	if (FAILED(pInstance->Init()))
	{
		pInstance->Uninit();
		pInstance = nullptr;
		return nullptr;
	}

	return pInstance;
}

//================================================
// 初期化処理
//================================================
HRESULT CCollisionObject3D::Init(void)
{
	if (m_pTransform == nullptr)
	{
		// 空間情報の生成
		m_pTransform = std::make_unique<CTransform>();

		// 失敗したら
		if (m_pTransform == nullptr)
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//================================================
// 終了処理
//================================================
void CCollisionObject3D::Uninit(void)
{
	// 要素を調べる
	for (auto& collider : m_pCollider)
	{
		// コライダーの破棄
		if (collider != nullptr)
		{
			delete collider;
			collider = nullptr;
		}
	}

	if (m_pTransform != nullptr)
	{
		// 破棄
		m_pTransform.reset();
	}

	// 自分自身の破棄
	CObject::Release();
}

//================================================
// 更新処理
//================================================
void CCollisionObject3D::Update(void)
{
	// 要素を調べる
	for (auto& collider : m_pCollider)
	{
		if (collider != nullptr)
		{
			// 更新処理
			collider->Update();
		}
	}
}

//================================================
// 描画処理
//================================================
void CCollisionObject3D::Draw(void)
{
	if (m_pTransform != nullptr)
	{
		// ワールドマトリックスの設定処理
		m_pTransform->SetMatrix();
	}
}

//================================================
// コライダーの追加
//================================================
void CCollisionObject3D::AddCollider(CCollider* pCollider, const char* ColliderTag)
{
	if (pCollider != nullptr)
	{
		// 空間情報の設定
		pCollider->SetTransform(m_pTransform.get());
		pCollider->SetTag(ColliderTag);
	}
	else
	{
		MessageBox(NULL, "エラー", "コライダーを追加できませんでした", MB_OK);
		return;
	}

	m_pCollider.push_back(pCollider);
}

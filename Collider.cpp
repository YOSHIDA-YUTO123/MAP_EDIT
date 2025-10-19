//================================================
//
// コライダー [collider.cpp]
// Author: YUTO YOSHIDA
//
//================================================

//************************************************
// インクルードファイル
//************************************************
#include "Collider.h"
#include "object.h"
#include "math.h"
#include "transform.h"

//================================================
// コンストラクタ
//================================================
CCollider::CCollider()
{
	m_pTransform = nullptr;
	m_type = TYPE_AABB;
}

//================================================
// コンストラクタ
//================================================
CCollider::CCollider(const TYPE type)
{
	m_pTransform = nullptr;
	m_type = type;
}

//================================================
// デストラクタ
//================================================
CCollider::~CCollider()
{
	m_pTransform = nullptr;
}

//================================================
// 終了処理(自分で破棄)
//================================================
void CCollider::DeleteTransform(void)
{
	// 自分自身の破棄
	if (m_pTransform != nullptr)
	{
		delete m_pTransform;
		m_pTransform = nullptr;
	}
}

//================================================
// コンストラクタ
//================================================
CColliderAABB::CColliderAABB() : CCollider(TYPE_AABB)
{

}

//================================================
// デストラクタ
//================================================
CColliderAABB::~CColliderAABB()
{
}

//================================================
// 更新処理
//================================================
void CColliderAABB::Update(void)
{
}

//================================================
// コライダーの作成処理
//================================================
std::unique_ptr<CColliderAABB> CColliderAABB::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 posOld, const D3DXVECTOR3 Size)
{
	// AABBの生成
	std::unique_ptr<CColliderAABB> pAABB = std::make_unique<CColliderAABB>();

	// 空間情報の取得
	CTransform *pTransform = pAABB->GetTransform();

	// 取得できないなら処理しない
	if (pTransform == nullptr) return nullptr;

	// 情報の取得
	CTransform::Info info = pTransform->GetInfo();

	// 設定処理
	info.pos = pos;
	info.Size = Size;
	info.posOld = posOld;

	// 情報の設定
	pTransform->SetInfo(info);

	return pAABB;
}

//================================================
// コライダーの生成処理
//================================================
CColliderAABB CColliderAABB::CreateCollider(const D3DXVECTOR3 pos, const D3DXVECTOR3 posOld, const D3DXVECTOR3 Size)
{
	// AABB
	CColliderAABB aabb;

	// 空間情報の取得
	CTransform* pTransform = aabb.GetTransform();

	// 情報の取得
	CTransform::Info info = pTransform->GetInfo();

	// 設定処理
	info.pos = pos;
	info.Size = Size;
	info.posOld = posOld;

	// 情報の設定
	pTransform->SetInfo(info);

	return aabb;
}

//================================================
// コンストラクタ
//================================================
CColliderSphere::CColliderSphere() : CCollider(TYPE_SPHERE)
{
}

//================================================
// デストラクタ
//================================================
CColliderSphere::~CColliderSphere()
{
}

//================================================
// 更新処理
//================================================
void CColliderSphere::Update(void)
{

}

//================================================
// 円のコライダーの作成
//================================================
std::unique_ptr<CColliderSphere> CColliderSphere::Create(const D3DXVECTOR3 pos, const float fRadius, CTransform* pTransform)
{
	// sphereの生成
	std::unique_ptr<CColliderSphere> pSphere = std::make_unique<CColliderSphere>();

	// 取得できなかったら処理しない
	if (pTransform == nullptr) return nullptr;

	// 空間情報の取得
	pSphere->SetTransform(pTransform);

	// 情報の取得
	CTransform::Info info = pTransform->GetInfo();

	// 設定処理
	info.pos = pos;
	info.fRadius = fRadius;

	// 情報の設定
	pTransform->SetInfo(info);

	return pSphere;
}

//================================================
// 生ポインタで生成する
//================================================
CColliderSphere* CColliderSphere::CreateRawPtr(const D3DXVECTOR3 pos, const float fRadius, CTransform* pTransform)
{
	// sphereの生成
	CColliderSphere* pSphere = new CColliderSphere;

	// 取得できなかったら処理しない
	if (pTransform == nullptr) return nullptr;

	// 空間情報の取得
	pSphere->SetTransform(pTransform);

	// 情報の取得
	CTransform::Info info = pTransform->GetInfo();

	// 設定処理
	info.pos = pos;
	info.fRadius = fRadius;

	// 情報の設定
	pTransform->SetInfo(info);

	return pSphere;
}

//================================================
// コライダーの作成処理
//================================================
CColliderSphere CColliderSphere::CreateCollider(const D3DXVECTOR3 pos, const float fRadius)
{
	// コライダー
	CColliderSphere sphere;

	// 空間情報の取得
	CTransform* pTransform = CTransform::Create();

	// 空間情報の取得
	sphere.SetTransform(pTransform);

	// 情報の取得
	CTransform::Info info = pTransform->GetInfo();

	// 設定処理
	info.pos = pos;
	info.fRadius = fRadius;

	// 情報の設定
	pTransform->SetInfo(info);

	return sphere;
}

//================================================
// コンストラクタ
//================================================
CColliderFOV::CColliderFOV() : CCollider(TYPE_FOV)
{
	// メモリのクリア
	ZeroMemory(&m_Data, sizeof(m_Data));
}

//================================================
// デストラクタ
//================================================
CColliderFOV::~CColliderFOV()
{
}

//================================================
// 生成処理
//================================================
std::unique_ptr<CColliderFOV> CColliderFOV::Create(const D3DXVECTOR3 pos, const float fAngle, const float fAngleLeft, const float fAngleRight, const float fLength)
{
	// 視界の生成
	std::unique_ptr<CColliderFOV> pFOV = std::make_unique<CColliderFOV>();

	// 空間情報の取得
	CTransform* pTransform = pFOV->GetTransform();

	// 情報の取得
	CTransform::Info info = pTransform->GetInfo();

	// 設定処理
	info.pos = pos;
	pFOV->m_Data.fAngleLeft = fAngleLeft;
	pFOV->m_Data.fAngleRight = fAngleRight;
	pFOV->m_Data.fLength = fLength;
	pFOV->m_Data.fNowAngle = fAngle;

	// 情報の設定
	pTransform->SetInfo(info);

	return pFOV;
}

//================================================
// コライダーの生成
//================================================
CColliderFOV CColliderFOV::CreateCollider(const D3DXVECTOR3 pos, const float fAngle, const float fAngleLeft, const float fAngleRight, const float fLength)
{
	// 視界の
	CColliderFOV FOV = {};

	// 空間情報の取得
	CTransform* pTransform = FOV.GetTransform();

	// 情報の取得
	CTransform::Info info = pTransform->GetInfo();

	// 設定処理
	info.pos = pos;
	FOV.m_Data.fAngleLeft = fAngleLeft;
	FOV.m_Data.fAngleRight = fAngleRight;
	FOV.m_Data.fLength = fLength;
	FOV.m_Data.fNowAngle = fAngle;

	// 情報の設定
	pTransform->SetInfo(info);

	return FOV;
}

//================================================
// 更新処理
//================================================
void CColliderFOV::Update(void)
{
	// 空間情報の取得
	CTransform* pTransform = CCollider::GetTransform();

	if (pTransform != nullptr)
	{
		// 空間情報の取得
		CTransform::Info info = pTransform->GetInfo();

		// 現在の自分の向き
		m_Data.fNowAngle = info.rot.y;
	}
}

//================================================
// コンストラクタ
//================================================
CColliderCapsule::CColliderCapsule() : CCollider(TYPE_CAPSULE)
{
	ZeroMemory(&m_Data, sizeof(m_Data));
	m_LocalEnd = Const::VEC3_NULL;
	m_LocalStart = Const::VEC3_NULL;
}

//================================================
// デストラクタ
//================================================
CColliderCapsule::~CColliderCapsule()
{
}

//================================================
// 生成処理
//================================================
std::unique_ptr<CColliderCapsule> CColliderCapsule::Create(const D3DXVECTOR3 StartPos, const D3DXVECTOR3 EndPos, const float fRadius)
{
	// カプセルの生成
	auto pCapsule = std::make_unique<CColliderCapsule>();

	// 設定処理
	pCapsule->m_LocalStart = StartPos;
	pCapsule->m_LocalEnd = EndPos;
	pCapsule->m_Data.fRadius = fRadius;
	return pCapsule;
}

//================================================
// コライダーの作成処理
//================================================
CColliderCapsule CColliderCapsule::CreateCollider(const D3DXVECTOR3 StartPos, const D3DXVECTOR3 EndPos, const float fRadius)
{
	CColliderCapsule capsule = {};

	// 設定処理
	capsule.m_Data.StartPos = StartPos;
	capsule.m_Data.EndPos = EndPos;
	capsule.m_Data.fRadius = fRadius;

	return capsule;
}

//================================================
// 更新処理
//================================================
void CColliderCapsule::Update(void)
{
	// 空間情報の取得
	CTransform* pTransform = CCollider::GetTransform();

	if (pTransform != nullptr)
	{
		// 空間情報の取得
		CTransform::Info info = pTransform->GetInfo();

		// 終点と始点の空間の位置を求める
		D3DXVec3TransformCoord(&m_Data.StartPos, &m_LocalStart, &info.mtxWorld);
		D3DXVec3TransformCoord(&m_Data.EndPos, &m_LocalEnd, &info.mtxWorld);
	}

}
//===================================================
//
// キャラクター3D [character.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "character3D.h"
#include "textureManager.h"
#include "manager.h"
#include "renderer.h"
#include "math.h"
#include "model.h"
#include "motion.h"
#include <string>
#include <memory>
#include "transform.h"
#include "CollisionObject3D.h"

//===================================================
// コンストラクタ
//===================================================
CCharacter3D::CCharacter3D() :
	CObject(4),
	m_pCollObject(nullptr),
	m_nNumModel(NULL),
	m_pMotion(nullptr),
	m_nLife(NULL),
	m_state(STATE::STATE_NORMAL),
	m_fSpeed(NULL),
	m_type(TYPE_NONE)
{
}

//===================================================
// コンストラクタ
//===================================================
CCharacter3D::CCharacter3D(const TYPE type)
	: CObject(4),
	m_pCollObject(nullptr),
	m_nNumModel(NULL),
	m_pMotion(nullptr),
	m_nLife(NULL),
	m_state(STATE::STATE_NORMAL),
	m_fSpeed(NULL),
	m_type(type)
{
}

//===================================================
// デストラクタ
//===================================================
CCharacter3D::~CCharacter3D()
{
}

//===================================================
// 初期化処理
//===================================================
HRESULT CCharacter3D::Init(void)
{
	if (m_pCollObject == nullptr)
	{
		m_pCollObject = CCollisionObject3D::Create(std::make_unique<CTransform>());
	}

	// 初期化処理
	if (FAILED(m_pCollObject->Init()))
	{
		return E_FAIL;
	}
	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CCharacter3D::Uninit(void)
{
	// 要素数分調べる
	for (auto &model : m_apModel)
	{
		// モデルの破棄
		if (model != nullptr)
		{
			// 終了処理
			model->Uninit();

			delete model;

			model = nullptr;
		}
	}

	if (m_pMotion != nullptr)
	{
		// モーションの終了処理
		m_pMotion->Uninit();
	}

	m_pCollObject = nullptr;

	// 終了処理
	CObject::Release();
}

//===================================================
// 更新処理
//===================================================
void CCharacter3D::Update(void)
{
	// 状態の遷移
	switch (m_state)
	{
	case STATE_NORMAL:
		break;
	case STATE_MOVE:
		break;
	case STATE_ACTION:

		// 状態のカウンターを減らす
		m_nCounterState--; 

		// カウンターが0になったら
		if (m_nCounterState <= 0)
		{
			// 状態をノーマルに戻す
			m_state = STATE_NORMAL;
		}

		break;
	case STATE_DAMAGE:

		// 状態のカウンターを減らす
		m_nCounterState--; 

		// カウンターが0になったら
		if (m_nCounterState <= 0)
		{
			// 状態をノーマルに戻す
			m_state = STATE_NORMAL;
		}

		break;
	default:
		break;
	}
}

//===================================================
// 描画処理
//===================================================
void CCharacter3D::Draw(void)
{
	if (m_pCollObject != nullptr)
	{
		// 描画処理
		m_pCollObject->Draw();
	}

	// モデルの描画
	for (auto &model : m_apModel)
	{
		if (model != nullptr)
		{
			// 描画処理
			model->Draw();
		}
	}
}

//===================================================
// 描画処理(透明度設定)
//===================================================
void CCharacter3D::Draw(const float fAvl)
{
	if (m_pCollObject != nullptr)
	{
		// 描画処理
		m_pCollObject->Draw();
	}

	// モデルの描画
	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		if (m_apModel[nCnt] != nullptr)
		{
			// 描画処理
			m_apModel[nCnt]->Draw(fAvl);
		}
	}
}

//===================================================
// 描画処理(マルチテクスチャ)
//===================================================
void CCharacter3D::DrawMT(void)
{
	if (m_pCollObject != nullptr)
	{
		// 描画処理
		m_pCollObject->Draw();
	}

	// モデルの描画
	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		if (m_apModel[nCnt] != nullptr)
		{
			// 描画処理
			m_apModel[nCnt]->DrawMultTexture();
		}
	}
}

//===================================================
// モーションのロード
//===================================================
CMotion* CCharacter3D::LoadMotion(const char* pFileName,const int nNumMotion)
{
	// 省略用
	std::string string = "data/MOTION/";

	// 文字列を合成
	string += pFileName;

	// モーションのロード処理
	m_pMotion = CMotion::Load(string.c_str(), m_apModel, &m_nNumModel, nNumMotion, CMotion::LOAD_TEXT);

	return m_pMotion.get();
}

//===================================================
// モデルの位置の取得
//===================================================
D3DXVECTOR3 CCharacter3D::GetModelPos(const int nIdx)
{
	// モデルのマトリックスの取得
	D3DXMATRIX mtx = m_apModel[nIdx]->GetMatrixWorld();

	// ワールドマトリックスの位置の取得
	D3DXVECTOR3 modelPos = math::GetPositionFromMatrix(mtx);

	return modelPos;
}

//===================================================
// モデルの向きの取得
//===================================================
D3DXVECTOR3 CCharacter3D::GetModelRot(const int nIdx)
{
	// モデルの位置の取得
	D3DXVECTOR3 modelRot = m_apModel[nIdx]->GetRotation();

	return modelRot;
}

//===================================================
// モデルの大きさの取得
//===================================================
D3DXVECTOR3 CCharacter3D::GetModelSize(const int nIdx)
{
	// モデルの大きさの取得
	D3DXVECTOR3 modelSize = m_apModel[nIdx]->GetSize();

	return modelSize;
}

//===================================================
// キャラクターの設定処理
//===================================================
void CCharacter3D::SetCharacter(const int nLife, const float fSpeed, const D3DXVECTOR3 Size)
{
	m_nLife = nLife;
	m_fSpeed = fSpeed;

	// nullだったら処理しない
	if (m_pCollObject == nullptr) return;

	// 空間情報の取得
	CTransform* pTransform = m_pCollObject->GetTransform();

	if (pTransform != nullptr)
	{
		// 情報の取得
		CTransform::Info info = pTransform->GetInfo();

		info.Size = Size;

		// 情報の設定
		pTransform->SetInfo(info);
	}
}

//===================================================
// モデルのマルチテクスチャの設定
//===================================================
void CCharacter3D::SetModelMT(const char* pTextureName)
{
	// モデルの総数分調べる
	for (auto itr = m_apModel.begin(); itr != m_apModel.end(); ++itr)
	{
		// テクスチャの設定
		(*itr)->SetTextureMT(pTextureName);
	}
}

//===================================================
// キャラクターのヒット処理
//===================================================
bool CCharacter3D::Hit(int nDamage)
{
	// 体力を減らす
	m_nLife -= nDamage;

	if (m_nLife <= 0)
	{
		m_state = STATE_DEATH;

		return false;
	}

	return true;
}

//===================================================
// モーションの更新処理
//===================================================
void CCharacter3D::UpdateMotion(void)
{
	if (m_pMotion != nullptr)
	{
		// モーションの更新処理
		m_pMotion->Update(&m_apModel[0], m_nNumModel);
	}
}

//===================================================
// 状態の設定処理
//===================================================
void CCharacter3D::SetState(const STATE state, const int nTime)
{
	m_state = state;
	m_nCounterState = nTime;
}

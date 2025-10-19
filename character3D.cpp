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
#include "CharacterManager.h"

using namespace math;	// 名前空間mathを使用
using namespace Const;	// 名前空間Constを使用
using namespace std;	// 名前空間stdを使用

//===================================================
// コンストラクタ
//===================================================
CCharacter3D::CCharacter3D() : CObject(4)
{
	m_bShow = true;
	m_Size = VEC3_NULL;
	m_nNumModel = NULL;
	m_pMotion = nullptr;
	m_pos = VEC3_NULL;
	m_rot = VEC3_NULL;
	memset(m_mtxWorld, NULL, sizeof(m_mtxWorld));
	m_nLife = NULL;
	m_state = STATE::STATE_NORMAL;
	m_fSpeed = NULL;
	m_nHitStopTime = NULL;
}

//===================================================
// コンストラクタ
//===================================================
CCharacter3D::CCharacter3D(const TYPE type) : CObject(4)
{
	m_bShow = true;
	m_type = type;
	m_Size = VEC3_NULL;
	m_nNumModel = NULL;
	m_pMotion = nullptr;
	m_pos = VEC3_NULL;
	m_rot = VEC3_NULL;
	memset(m_mtxWorld, NULL, sizeof(m_mtxWorld));
	m_nLife = NULL;
	m_state = STATE::STATE_NORMAL;
	m_fSpeed = NULL;
	m_nHitStopTime = NULL;
}

//===================================================
// コピーコンストラクタ
//===================================================
CCharacter3D::CCharacter3D(const CCharacter3D& other)
{
	if (other.m_pMotion != nullptr)
	{
		m_pMotion = std::make_unique<CMotion>(*other.m_pMotion);
	}
	else
	{
		m_pMotion = nullptr;
	}

	// モデルの要素のクリア
	m_apModel.clear();

	// モデルの配列の確保
	m_apModel.reserve(other.m_apModel.size());

	for (auto* pModel : other.m_apModel)
	{
		if (pModel != nullptr)
		{
			m_apModel.push_back(new CModel(*pModel));
		}
		else
		{
			m_apModel.push_back(nullptr);
		}
	}

	m_Size = other.m_Size;
	m_nNumModel = other.m_nNumModel;
	m_pos = VEC3_NULL;
	m_rot = VEC3_NULL;
	memset(m_mtxWorld, NULL, sizeof(m_mtxWorld));
	m_nLife = other.m_nLife;
	m_state = STATE::STATE_NORMAL;
	m_fSpeed = other.m_fSpeed;
	m_nHitStopTime = NULL;
	m_bShow = true;
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
	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CCharacter3D::Uninit(void)
{
	for (int nCnt = 0; nCnt < (int)m_apModel.size(); nCnt++)
	{
		// モデルの破棄
		if (m_apModel[nCnt] != nullptr)
		{
			// 終了処理
			m_apModel[nCnt]->Uninit();

			delete m_apModel[nCnt];

			m_apModel[nCnt] = nullptr;
		}
	}

	if (m_pMotion != nullptr)
	{
		// モーションの終了処理
		m_pMotion->Uninit();
	}

	// 自分自身の破棄
	CObject::Release();
}

//===================================================
// 更新処理
//===================================================
void CCharacter3D::Update(void)
{
	// 種類が無いなら処理しない
	if (m_bShow == false) return;

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
	// 種類が無いなら処理しない
	if (m_bShow == false) return;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// モデルの描画
	for (int nCnt = 0; nCnt < m_nNumModel; nCnt++)
	{
		if (m_apModel[nCnt] != nullptr)
		{
			// 描画処理
			m_apModel[nCnt]->Draw();
		}
	}
}

//===================================================
// クローンの生成処理
//===================================================
CCharacter3D* CCharacter3D::Clone(void) const
{
	auto clone = new CCharacter3D(*this);

	return clone;
}

//===================================================
// 描画処理(透明度設定)
//===================================================
void CCharacter3D::Draw(const float fAvl)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

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
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

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
	D3DXVECTOR3 modelPos = GetPositionFromMatrix(mtx);

	return modelPos;
}

//===================================================
// モデルの向きの取得
//===================================================
D3DXVECTOR3 CCharacter3D::GetModelRot(const int nIdx)
{
	// モデルの位置の取得
	D3DXVECTOR3 modelRot = m_apModel[nIdx]->GetRotaition();

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
	m_Size = Size;
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
// 生きているかどうか
//===================================================
bool CCharacter3D::GetAlive(void)
{
	// 死んでいるなら
	if (m_nLife <= 0)
	{
		return false;
	}

	return true;
}

//===================================================
// ヒットストップの設定処理
//===================================================
bool CCharacter3D::HitStop(void)
{
	// カウンターを減らす
	m_nHitStopTime--;

	// 0以下だったら
	if (m_nHitStopTime <= 0)
	{
		// ヒットストップしていない
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

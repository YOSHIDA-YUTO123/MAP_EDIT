//===================================================
//
// 編集中のオブジェクトの表示用クラス [EditMapObject.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "EditMapObject.h"
#include "modelManager.h"
#include "manager.h"
#include "input.h"
#include "imgui.h"
#include "math.h"
#include "renderer.h"
#include "imguimaneger.h"
#include "MapObjectManager.h"

//===================================================
// コンストラクタ
//===================================================
CEditMapObject::CEditMapObject()
{
	m_bMouseUp = true;
	m_bShow = false;
}

//===================================================
// デストラクタ
//===================================================
CEditMapObject::~CEditMapObject()
{
}

//===================================================
// 生成処理
//===================================================
CEditMapObject* CEditMapObject::Create(void)
{
	CEditMapObject* pEditMapObject = new CEditMapObject;

	// 初期化処理
	if (FAILED(pEditMapObject->Init()))
	{
		pEditMapObject->Uninit();
		pEditMapObject = nullptr;
		return nullptr;
	}

	return pEditMapObject;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CEditMapObject::Init(void)
{
	// モデルのマネージャーの取得
	CModelManager* pModelManager = CManager::GetModel();

	// モデルの情報の取得
	CModelManager::ModelInfo modelInfo = {};

	if (pModelManager != nullptr)
	{
		// 最初のモデルの取得
		modelInfo = pModelManager->GetModelInfo(0);

		// 最初のモデルのファイルパス
		m_aModelName = modelInfo.filepath;

		// モデルのロード
		LoadModel(m_aModelName.c_str());
	}
	else
	{
		return E_FAIL;
	}

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CEditMapObject::Uninit(void)
{
	// 終了処理
	CObjectX::Uninit();
}

//===================================================
// 更新処理
//===================================================
void CEditMapObject::Update(void)
{
	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// 視点の操作をしているときに処理しない
	if (pKeyboard->GetPress(DIK_LALT)) return;

	// 位置の取得
	D3DXVECTOR3 pos = CObjectX::GetPosition();

	// 距離
	float fDistance = 0.0f;

	// マウスの取得
	CInputMouse* pMouse = CManager::GetInputMouse();

	// マウスとの当たり判定
	if (CollisionMouse(&fDistance))
	{
		// 左クリックをしていたら
		if (pMouse->OnMousePress(CInputMouse::LEFT))
		{
			// マウスを押している
			m_bMouseUp = false;
		}
		else
		{
			// マウスを押していない
			m_bMouseUp = true;
		}
	}
	// 左クリックをしていないなら
	else if (!pMouse->OnMousePress(CInputMouse::LEFT))
	{
		// マウスを離した
		m_bMouseUp = true;
	}

	// マウスを離していないなら
	if (!m_bMouseUp)
	{
		// 位置と前回の位置の取得
		D3DXVECTOR2 MousePos = pMouse->GetVelocity();
		D3DXVECTOR2 MousePosOld = pMouse->GetOldVelocity();

		// 移動量の設定
		D3DXVECTOR2 MouseMove = MousePos - MousePosOld;

		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		D3DXMATRIX view;
		pDevice->GetTransform(D3DTS_VIEW, &view);

		// カメラの右ベクトルと上ベクトルを抽出
		D3DXVECTOR3 camRight(view._11, view._21, view._31);
		D3DXVECTOR3 camUp(view._12, view._22, view._32);

		D3DXVECTOR3 move;

		if (CMapObjectManager::GetInstance()->GetDragMoveState())
		{
			// XZ平面での移動
			camUp = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 上方向は無効化
			D3DXVECTOR3 forward = D3DXVECTOR3(view._13, 0.0f, view._33);
			D3DXVec3Normalize(&forward, &forward);

			move = camRight * MouseMove.x + forward * -MouseMove.y;
		}
		else
		{
			// 通常のカメラ基準移動
			move = camRight * MouseMove.x + camUp * -MouseMove.y;
		}

		// 位置の更新
		pos += move;

		CObjectX::SetPosition(pos);
	}
}

//===================================================
// 描画処理
//===================================================
void CEditMapObject::Draw(void)
{
	// 見えるなら
	if (m_bShow)
	{
		// 描画処理
		CObjectX::Draw(D3DXCOLOR(0.4f,1.0f,1.0f,0.5f));
	}
}

//===================================================
// マウスの当たり判定
//===================================================
bool CEditMapObject::CollisionMouse(float* pDistance)
{
	BOOL hit = false;
	DWORD faceIndex;
	(*pDistance) = FLT_MAX;
	FLOAT baryU, baryV; // 当たったポリゴンの位置

	// モデルの情報の取得
	const CModelManager::ModelInfo modelInfo = CObjectX::GetModelInfo();

	// レイの原点
	D3DXVECTOR3 rayOrigin, rayDir;

	// レイの取得
	math::GetMouseRay(&rayOrigin, &rayDir);

	// モデルのワールド行列
	D3DXMATRIX matWorld = CObjectX::GetMatrix();

	// 逆行列
	D3DXMATRIX matInvWorld;
	D3DXMatrixInverse(&matInvWorld, NULL, &matWorld);

	// レイをローカル空間に変換
	D3DXVec3TransformCoord(&rayOrigin, &rayOrigin, &matInvWorld);
	D3DXVec3TransformNormal(&rayDir, &rayDir, &matInvWorld);
	D3DXVec3Normalize(&rayDir, &rayDir);

	// レイとポリゴンの当たり判定
	D3DXIntersect(
		modelInfo.pMesh,
		&rayOrigin,
		&rayDir,
		&hit,
		&faceIndex,
		&baryU,
		&baryV,
		pDistance,
		nullptr,
		nullptr);

	if (hit)
	{
		return true;
	}

	return false;
}

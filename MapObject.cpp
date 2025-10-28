//===================================================
//
// マップに配置するオブジェクトのクラス [MapObject.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "MapObject.h"
#include "modelManager.h"
#include "manager.h"
#include "renderer.h"
#include "textureManager.h"
#include "math.h"
#include "imgui.h"
#include "input.h"
#include "MapObjectManager.h"
#include "camera.h"
#include "meshfield.h"
#include "transform.h"
#include "MapObjectList.h"
#include "Collider.h"

//***************************************************
// 定数宣言
//***************************************************
namespace MAPOBJ_INFO
{
	// オブジェクトの状態
	const char* TYPE[CMapObject::TYPE_MAX] =
	{
		"static",
		"collision"
	};
};

//===================================================
// コンストラクタ
//===================================================
CMapObject::CMapObject()
{
	m_fMove = MOVE_VALUE;
	m_bMouseUp = true;
	m_fAlv = 1.0f;
	m_nModelIdx = -1;
	m_aType = MAPOBJ_INFO::TYPE[CMapObject::TYPE_STATIC];
}

//===================================================
// デストラクタ
//===================================================
CMapObject::~CMapObject()
{
	m_aModelPath.clear();
}

//===================================================
// 生成処理
//===================================================
CMapObject* CMapObject::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,const char *pModelFileName,const char *pType)
{
	CMapObject* pMapObject = new CMapObject;

	// 番号の登録
	pMapObject->Register(pModelFileName);
	pMapObject->m_aType = pType;

	// 初期化処理
	if (FAILED(pMapObject->Init()))
	{
		pMapObject->Uninit();
		pMapObject = nullptr;
		return nullptr;
	}

	// 空間情報の取得
	CTransform* pTransform = pMapObject->GetTransform();

	if (pTransform == nullptr)
	{
		assert(false && "空間情報無し");
	}

	// 情報の取得
	CTransform::Info info = pTransform->GetInfo();

	info.pos = pos;
	info.rot = rot;
	
	// 情報の設定
	pTransform->SetInfo(info);

	return pMapObject;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CMapObject::Init(void)
{
	// 初期化処理
	if (FAILED(CCollisionObject3D::Init()))
	{
		return E_FAIL;
	}

	// モデルがないなら
	if (m_nModelIdx == -1)
	{
		return E_FAIL;
	}

	// モデルマネージャークラスの取得
	CModelManager* pModelManager = CManager::GetModel();

	// モデルの情報の取得
	CModelManager::ModelInfo modelInfo = pModelManager->GetModelInfo(m_nModelIdx);

	// 空間情報の取得
	CTransform *pTransform = CCollisionObject3D::GetTransform();

	CTransform::Info transform = pTransform->GetInfo();

	transform.Size = modelInfo.Size;

	pTransform->SetInfo(transform);

	// コライダーの追加
	CCollisionObject3D::AddCollider(CColliderOBB::Create(), "mapObject");

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CMapObject::Uninit(void)
{
	// 終了処理
	CCollisionObject3D::Uninit();
}

//===================================================
// 更新処理
//===================================================
void CMapObject::Update(void)
{
	// メッシュフィールドの取得
	CMeshField* pMeshField = CManager::GetMeshField();

	// 取得できなかったら処理しない
	if (pMeshField == nullptr) return;

	float fHeight = 0.0f;

	// 更新処理
	CCollisionObject3D::Update();

	// 空間情報の取得
	CTransform* pTransform = CCollisionObject3D::GetTransform();

	// nullだったら処理を飛ばす
	if (pTransform == nullptr) return;

	// 情報の取得
	CTransform::Info info = pTransform->GetInfo();

	// フィールドの当たり判定
	if (pMeshField->Collision(info.pos, &fHeight))
	{
		info.pos.y = fHeight;
	}

	if (m_nModelIdx == -1) return;

	// モデルマネージャークラスの取得
	CModelManager* pModelManager = CManager::GetModel();

	// モデルの情報の取得
	CModelManager::ModelInfo modelInfo = pModelManager->GetModelInfo(m_nModelIdx);

	info.Size = modelInfo.Size;

	// 空間情報の設定
	pTransform->SetInfo(info);
}

//===================================================
// 描画処理
//===================================================
void CMapObject::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// テクスチャクラスの取得
	CTextureManager* pTexture = CManager::GetTexture();

	// モデルクラスの取得
	CModelManager* pModel = CManager::GetModel();

	//計算用のマトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScal, mtxParent;

	D3DMATERIAL9 matDef;//現在のマテリアル保存用

	D3DXMATERIAL* pMat;//マテリアルデータへのポインタ

	// 更新処理
	CCollisionObject3D::Draw();

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	if (m_nModelIdx == -1)
	{
		//保存していたマテリアルを元に戻す
		pDevice->SetMaterial(&matDef);

		return;
	}

	// モデルの情報の取得
	CModelManager::ModelInfo modelInfo = pModel->GetModelInfo(m_nModelIdx);

	//マテリアルのデータへのポインタを取得
	pMat = (D3DXMATERIAL*)modelInfo.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)modelInfo.dwNumMat; nCntMat++)
	{
		D3DXMATERIAL Mat = pMat[nCntMat];

		Mat.MatD3D.Diffuse.a = m_fAlv;

		// マテリアルの設定
		pDevice->SetMaterial(&Mat.MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, pTexture->GetAdress(modelInfo.nTextureIdx[nCntMat]));

		// モデル(パーツ)の描画
		modelInfo.pMesh->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを元に戻す
	pDevice->SetMaterial(&matDef);
}

//===================================================
// モデルの番号の登録
//===================================================
void CMapObject::Register(const char* pModelFileName)
{
	// モデルクラスの取得
	CModelManager* pModel = CManager::GetModel();

	// インデックスの登録
	m_nModelIdx = pModel->Register(pModelFileName);

	// モデルのパスの設定
	m_aModelPath = pModelFileName;
}

//===================================================
// 移動処理
//===================================================
void CMapObject::UpdateMove(void)
{
	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// カメラの取得
	CCamera* pCamera = CManager::GetCamera();

	// 空間情報の取得
	CTransform* pTransform = CCollisionObject3D::GetTransform();

	// nullだったら処理を飛ばす
	if (pTransform == nullptr) return;

	// 情報の取得
	CTransform::Info info = pTransform->GetInfo();

	// カメラを取得出来なかったら処理しない
	if (pCamera == nullptr) return;

	// カメラの角度の取得
	float fCameraAngle = pCamera->GetRotaition().y;

	if (pKeyboard->GetPress(DIK_LEFT))
	{
		if (pKeyboard->GetPress(DIK_UP))
		{
			info.pos.x += sinf(fCameraAngle - D3DX_PI * 0.25f) * m_fMove;
			info.pos.z += cosf(fCameraAngle - D3DX_PI * 0.25f) * m_fMove;
		}
		else if (pKeyboard->GetPress(DIK_DOWN))
		{
			info.pos.x += sinf(fCameraAngle - D3DX_PI * 0.75f) * m_fMove;
			info.pos.z += cosf(fCameraAngle - D3DX_PI * 0.75f) * m_fMove;
		}
		else
		{
			info.pos.x += sinf(fCameraAngle - D3DX_PI * 0.5f) * m_fMove;
			info.pos.z += cosf(fCameraAngle - D3DX_PI * 0.5f) * m_fMove;
		}
	}
	else if (pKeyboard->GetPress(DIK_RIGHT))
	{
		if (pKeyboard->GetPress(DIK_UP))
		{
			info.pos.x += sinf(fCameraAngle + D3DX_PI * 0.25f) * m_fMove;
			info.pos.z += cosf(fCameraAngle + D3DX_PI * 0.25f) * m_fMove;
		}
		else if (pKeyboard->GetPress(DIK_DOWN))
		{
			info.pos.x += sinf(fCameraAngle + D3DX_PI * 0.75f) * m_fMove;
			info.pos.z += cosf(fCameraAngle + D3DX_PI * 0.75f) * m_fMove;
		}
		else
		{
			info.pos.x += sinf(fCameraAngle + D3DX_PI * 0.5f) * m_fMove;
			info.pos.z += cosf(fCameraAngle + D3DX_PI * 0.5f) * m_fMove;
		}
	}
	else if (pKeyboard->GetPress(DIK_UP))
	{
		info.pos.x += sinf(fCameraAngle) * m_fMove;
		info.pos.z += cosf(fCameraAngle) * m_fMove;
	}
	else if (pKeyboard->GetPress(DIK_DOWN))
	{
		info.pos.x += sinf(fCameraAngle + D3DX_PI) * m_fMove;
		info.pos.z += cosf(fCameraAngle + D3DX_PI) * m_fMove;
	}

	// 情報の設定
	pTransform->SetInfo(info);
}

//===================================================
// マウスの当たり判定
//===================================================
bool CMapObject::CollisionMouse(float* pDistance)
{
	BOOL hit = false;
	DWORD faceIndex;
	(*pDistance) = FLT_MAX;
	FLOAT baryU, baryV; // 当たったポリゴンの位置

	// モデルマネージャーの取得
	CModelManager *pModelManager = CManager::GetModel();

	// モデルがないなら
	if (m_nModelIdx == -1)
	{
		return false;
	}

	// モデルの情報の取得
	CModelManager::ModelInfo modelInfo = pModelManager->GetModelInfo(m_nModelIdx);

	// レイの原点
	D3DXVECTOR3 rayOrigin,rayDir;

	// レイの取得
	math::GetMouseRay(&rayOrigin, &rayDir);

	// 空間情報の取得
	CTransform* pTransform = CCollisionObject3D::GetTransform();

	// nullだったら処理を飛ばす
	if (pTransform == nullptr) return false;

	// 情報の取得
	CTransform::Info info = pTransform->GetInfo();

	// モデルのワールド行列
	D3DXMATRIX matWorld = info.mtxWorld;

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

//===================================================
// 情報の設定
//===================================================
void CMapObject::SetInfo(void)
{
	// 空間情報の取得
	CTransform* pTransform = CCollisionObject3D::GetTransform();

	// nullだったら処理を飛ばす
	if (pTransform == nullptr) return;

	// 情報の取得
	CTransform::Info info = pTransform->GetInfo();

	// 情報の設定
	ImGui::DragFloat3(u8"位置", info.pos, 0.5f);

	D3DXVECTOR3 rot = D3DXToDegree(info.rot);

	// 情報の設定
	if (ImGui::DragFloat3(u8"向き", rot, 0.5f, D3DXToDegree(-D3DX_PI), D3DXToDegree(D3DX_PI)))
	{
		// 範囲内をループ
		rot.x = Wrap(rot.x, D3DXToDegree(-D3DX_PI), D3DXToDegree(D3DX_PI));
		rot.y = Wrap(rot.y, D3DXToDegree(-D3DX_PI), D3DXToDegree(D3DX_PI));
		rot.z = Wrap(rot.z, D3DXToDegree(-D3DX_PI), D3DXToDegree(D3DX_PI));
	}
	
	static int nSelect = 0;

	if (ImGui::BeginCombo(u8"###state", m_aType.c_str()))
	{
		int nCnt = 0;

		for (auto& list : MAPOBJ_INFO::TYPE)
		{
			bool bSelect = (nSelect == nCnt);

			if (ImGui::Selectable(list, bSelect)) // 項目をクリックしたとき
			{
				nSelect = nCnt; // 選択を更新
				m_aType = list;
			}
			if (bSelect)
			{
				ImGui::SetItemDefaultFocus(); // デフォルトフォーカス設定（視認性向上）
			}
			nCnt++;
		}
		ImGui::EndCombo();
	}

	// 角度を設定
	info.rot = D3DXToRadian(rot);

	// 情報の設定
	pTransform->SetInfo(info);
}

//===================================================
// マウスのドラッグの設定
//===================================================
void CMapObject::SetMouseDrag(void)
{
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

	// 空間情報の取得
	CTransform* pTransform = CCollisionObject3D::GetTransform();

	// nullだったら処理を飛ばす
	if (pTransform == nullptr) return;

	// 情報の取得
	CTransform::Info info = pTransform->GetInfo();

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

		// ドラッグの状態が平面移動だったら
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
		info.pos += move;
	}

	// 情報の設定
	pTransform->SetInfo(info);
}

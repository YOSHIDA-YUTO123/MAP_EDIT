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

//===================================================
// コンストラクタ
//===================================================
CMapObject::CMapObject()
{
	m_bMouseUp = true;
	m_fAlv = 1.0f;
	m_pos = Const::VEC3_NULL;
	m_rot = Const::VEC3_NULL;
	D3DXMatrixIdentity(&m_mtxWorld);
	m_nModelIdx = -1;
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
CMapObject* CMapObject::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,const char *pModelFileName)
{
	CMapObject* pMapObject = new CMapObject;

	pMapObject->m_pos = pos;
	pMapObject->m_rot = rot;

	// 番号の登録
	pMapObject->Register(pModelFileName);

	// 初期化処理
	if (FAILED(pMapObject->Init()))
	{
		pMapObject->Uninit();
		pMapObject = nullptr;
		return nullptr;
	}

	return pMapObject;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CMapObject::Init(void)
{
	// モデルがないなら
	if (m_nModelIdx == -1)
	{
		return E_FAIL;
	}

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CMapObject::Uninit(void)
{
	// 終了処理
	CObject::Release();
}

//===================================================
// 更新処理
//===================================================
void CMapObject::Update(void)
{
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

	// モデルのワールド行列
	D3DXMATRIX matWorld = m_mtxWorld;

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
	// 情報の設定
	ImGui::DragFloat3(u8"位置", m_pos, 0.5f);

	D3DXVECTOR3 rot = D3DXToDegree(m_rot);

	// 情報の設定
	if (ImGui::DragFloat3(u8"向き", rot, 0.5f, D3DXToDegree(-D3DX_PI), D3DXToDegree(D3DX_PI)))
	{
		// 範囲内をループ
		rot.x = Wrap(rot.x, D3DXToDegree(-D3DX_PI), D3DXToDegree(D3DX_PI));
		rot.y = Wrap(rot.y, D3DXToDegree(-D3DX_PI), D3DXToDegree(D3DX_PI));
		rot.z = Wrap(rot.z, D3DXToDegree(-D3DX_PI), D3DXToDegree(D3DX_PI));
	}
	
	
	// 角度を設定
	m_rot = D3DXToRadian(rot);
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

		// 移動量の設定
		D3DXVECTOR3 move = camRight * MouseMove.x + camUp * -MouseMove.y;

		// 位置の更新
		m_pos += move;
	}
}

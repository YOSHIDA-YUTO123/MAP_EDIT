//===================================================
//
// モデルのアイコンの作成 [IconModel.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "IconModel.h"
#include "MapObjectManager.h"
#include "MapObject.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "modelManager.h"
#include "textureManager.h"
#include "TextureMTManager.h"

//===================================================
// 描画の設定処理
//===================================================
void CIconModel::SetDraw(void)
{
}

//===================================================
// コンストラクタ
//===================================================
CIconModel::CIconModel()
{
	m_pos = Const::VEC3_NULL;
	m_rot = Const::VEC3_NULL;
	D3DXMatrixIdentity(&m_mtxWorld);
}

//===================================================
// デストラクタ
//===================================================
CIconModel::~CIconModel()
{
}

//===================================================
// 初期化処理
//===================================================
HRESULT CIconModel::Init(void)
{
	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CIconModel::Uninit(void)
{
	// 自分自身の破棄
	CObject::Release();
}

//===================================================
// 更新処理
//===================================================
void CIconModel::Update(void)
{
}

//===================================================
// 描画処理
//===================================================
void CIconModel::Draw(void)
{
	// マップオブジェクトのマネージャーの生成
	CModelManager* pModelManager = CManager::GetModel();

	// 取得できなかったら処理しない
	if (pModelManager == nullptr) return;

	LPDIRECT3DSURFACE9 pRenderDef, pZBuffer;
	D3DVIEWPORT9 viepowtDef;
	D3DXMATRIX mtxViewDef, mtxProjectionDef;
	
	// マップオブジェクトのリストを調べる
	for (auto& modelInfo : pModelManager->GetList())
	{
		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		// 現在のレンダーターゲットの取得
		pDevice->GetRenderTarget(0, &pRenderDef);

		// 現在のZバッファの取得
		pDevice->GetDepthStencilSurface(&pZBuffer);

		// 現在のビューポートの取得
		pDevice->GetViewport(&viepowtDef);

		// 現在のビューマトリックスの取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxViewDef);

		// 現在のプロジェクションマトリックスの取得
		pDevice->GetTransform(D3DTS_PROJECTION, &mtxProjectionDef);

		D3DXVECTOR3 posV, posR, vecU;

		// サイズの取得
		D3DXVECTOR3 modelSize = modelInfo.Size;

		float fRadius = D3DXVec3Length(&modelSize);

		posV = D3DXVECTOR3(0.0f, fRadius, fRadius);

		posR = D3DXVECTOR3(0.0f, fRadius * 0.25f, 0.0f);

		vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// レンダリングターゲットの変更
		CManager::GetTextureMT()->ChangeTarget(posV, posR, vecU, modelInfo.filepath);

		// レンダリングターゲット用テクスチャのクリア
		pDevice->Clear(0,
			NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
			D3DCOLOR_RGBA(100, 100, 100, 255), 1.0f, 0);

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

		//マテリアルのデータへのポインタを取得
		pMat = (D3DXMATERIAL*)modelInfo.pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)modelInfo.dwNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, pTexture->GetAdress(modelInfo.nTextureIdx[nCntMat]));

			// モデル(パーツ)の描画
			modelInfo.pMesh->DrawSubset(nCntMat);
		}

		// レンダーターゲットをもとに戻す
		pDevice->SetRenderTarget(0, pRenderDef);

		// Zバッファをもとに戻す
		pDevice->SetDepthStencilSurface(pZBuffer);

		pRenderDef->Release();
		pZBuffer->Release();

		// ビューポートをもとに戻す
		pDevice->SetViewport(&viepowtDef);

		pDevice->SetTransform(D3DTS_VIEW, &mtxViewDef);

		pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjectionDef);

		//保存していたマテリアルを元に戻す
		pDevice->SetMaterial(&matDef);
	}
}

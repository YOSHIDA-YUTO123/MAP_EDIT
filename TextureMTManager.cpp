//===================================================
//
// レンダーターゲット用テクスチャを管理するマネージャー [TextureMTManager.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "TextureMTManager.h"
#include "manager.h"
#include "renderer.h"

//===================================================
// コンストラクタ
//===================================================
CTextureMTManager::CTextureMTManager()
{
}

//===================================================
// デストラクタ
//===================================================
CTextureMTManager::~CTextureMTManager()
{
}

//===================================================
// 終了処理
//===================================================
void CTextureMTManager::Uninit(void)
{
	// 要素数分回す
	for (auto& list : m_aInfo)
	{
		// レンダーターゲット用インターフェースの破棄
		if (list.pRenderMT != nullptr)
		{
			list.pRenderMT->Release();
			list.pRenderMT = nullptr;
		}

		// レンダーターゲット用テクスチャの破棄
		if (list.pTextureMT != nullptr)
		{
			list.pTextureMT->Release();
			list.pTextureMT = nullptr;
		}
		
		// Zバッファの破棄
		if (list.pZBuffMT != nullptr)
		{
			list.pZBuffMT->Release();
			list.pZBuffMT = nullptr;
		}

		// 名前の破棄
		list.aName.clear();
	}

	// コンテナの破棄
	m_aInfo.clear();
}

//===================================================
// アドレスの取得
//===================================================
LPDIRECT3DTEXTURE9 CTextureMTManager::GetAddress(const char* pName)
{
	// 要素数分調べる
	for (auto& list : m_aInfo)
	{
		// すでに作成済みだったら
		if (list.aName == pName)
		{
			return list.pTextureMT;
		}
	}

	// 生成する
	CreateRender(pName);

	// 要素数分調べる
	for (auto& list : m_aInfo)
	{
		// すでに作成済みだったら
		if (list.aName == pName)
		{
			return list.pTextureMT;
		}
	}

	return nullptr;
}

//===================================================
// レンダーターゲットの変更
//===================================================
void CTextureMTManager::ChangeTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU,const char *pName)
{
	if (m_aInfo.empty())
	{
		GetAddress(pName);
		return;
	}

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (auto& list : m_aInfo)
	{
		if (list.aName == pName)
		{
			D3DXMATRIX mtxView, mtxProjection; // ビューマトリックス、プロジェクションマトリックス
			float fAspect;

			// レンダリングターゲットを生成したテクスチャに設定
			pDevice->SetRenderTarget(0, list.pRenderMT);

			// 生成したZバッファに設定
			pDevice->SetDepthStencilSurface(list.pZBuffMT);

			// テクスチャレンダリング用ビューポートの設定
			pDevice->SetViewport(&list.viewport);

			// ビューマトリックスの初期化
			D3DXMatrixIdentity(&mtxView);

			// プロジェクションマトリックスの初期化
			D3DXMatrixIdentity(&mtxProjection);

			// ビューマトリックスの作成
			D3DXMatrixLookAtLH(
				&mtxView,
				&posV,
				&posR,
				&vecU);

			// ビューマトリックスの設定
			pDevice->SetTransform(D3DTS_VIEW, &mtxView);

			fAspect = (float)list.viewport.Width / (float)list.viewport.Height;

			// プロジェクションマトリックスの作成
			D3DXMatrixPerspectiveFovLH(&mtxProjection,
				D3DXToRadian(45.0f),
				fAspect,
				1.0f,
				100000.0f);

			// プロジェクションマトリックスの設定
			pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);
		}
		else
		{
			GetAddress(pName);
		}
	}

	// レンダリングターゲット用テクスチャのクリア
	pDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
		D3DCOLOR_RGBA(100, 100, 100, 255), 1.0f, 0);
}

//===================================================
// 生成処理
//===================================================
void CTextureMTManager::CreateRender(const char *pName)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	LPDIRECT3DSURFACE9 pRenderDef, pZBufferDef;

	CTextureMTManager::Info info = {};

	// レンダラーターゲット用テクスチャの生成
	pDevice->CreateTexture(
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&info.pTextureMT,
		NULL);

	// テクスチャのレンダリングターゲット用インターフェースの生成
	info.pTextureMT->GetSurfaceLevel(0, &info.pRenderMT);

	// テクスチャレンダリング用Zバッファの生成
	pDevice->CreateDepthStencilSurface(
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&info.pZBuffMT,
		NULL);

	// 現在のレンダリングターゲットを取得(保存)
	pDevice->GetRenderTarget(0, &pRenderDef);

	// 現在のZバッファの取得(保存)
	pDevice->GetDepthStencilSurface(&pZBufferDef);

	// レンダリングターゲットを生成したテクスチャに設定
	pDevice->SetRenderTarget(0, info.pRenderMT);

	// Zバッファを生成したZバッファの設定
	pDevice->SetDepthStencilSurface(info.pZBuffMT);

	// レンダリングターゲット用テクスチャのクリア
	pDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	// レンダーターゲットをもとに戻す
	pDevice->SetRenderTarget(0, pRenderDef);

	// Zバッファをもとに戻す
	pDevice->SetDepthStencilSurface(pZBufferDef);
	
	// テクスチャレンダリング用ビューポートの生成
	info.viewport.X = 0;
	info.viewport.Y = 0;
	info.viewport.Width = SCREEN_WIDTH;
	info.viewport.Height = SCREEN_HEIGHT;
	info.viewport.MinZ = 0.0f;
	info.viewport.MaxZ = 1.0f;

	info.aName = pName;
	m_aInfo.push_back(info);

	info.pTextureMT->Release();
	pRenderDef->Release();
	pZBufferDef->Release();
}

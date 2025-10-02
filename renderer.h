//===================================================
//
// ポリゴンの描画処理 [renderer.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _RENDERER_H_
#define _RENDERER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "main.h"
#include "debugproc.h"

//***************************************************
// 前方宣言
//***************************************************
class CImGuiManager;

//***************************************************
// レンダラークラスの定義
//***************************************************
class CRenderer
{
public:
	static constexpr int NUM_TEXTUREMT = 2; // テクスチャの数

	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(const int fps);
	static CImGuiManager* GetImGui(void) { return m_pImGui; }

	void ChangeTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU); // レンダリングターゲットの変更

	LPDIRECT3DDEVICE9 GetDevice(void);

	void onWireFrame();
	void offWireFrame();
	void onEffect(const float fLevel); // ブラーのオン
	void offEffect(void);
	void ResetDevice(void);
	HRESULT GetDeviceLost(void);
	void SetReSize(const UINT Width, const UINT Height);

private:
	static CImGuiManager* m_pImGui;						// IMGUIクラスへのポインタ
	static CDebugProc* m_pDebug;						// デバッグフォントへのポインタ
	LPDIRECT3DTEXTURE9 m_pTextureMT[NUM_TEXTUREMT];		// レンダリングターゲット用テクスチャ
	LPDIRECT3DSURFACE9 m_pRenderMT[NUM_TEXTUREMT];		// レンダリングターゲット用インターフェース
	LPDIRECT3DSURFACE9 m_pZBuffMT;						// レンダリングターゲット用Zバッファ
	D3DVIEWPORT9 m_viewport;							// テクスチャレンダリング用ビューポート
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffMT;				// フィードバッグ用ポリゴンの頂点バッファ
	LPDIRECT3D9 m_pD3D;									// Directxデバイスへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;						// Directxデバイスへのポインタ
	UINT m_ResizeWidth, m_ResizeHeight;					// 画面の大きさ変更時の大きさ
	D3DPRESENT_PARAMETERS m_d3dpp;						// プレゼンテーションパラメータ
	float m_fALv;										// A値
	bool m_bDeviceLost;									// デバイスのロスト判定
	bool m_bEffect;
};

#endif
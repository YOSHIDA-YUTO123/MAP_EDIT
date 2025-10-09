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

//***************************************************
// 前方宣言
//***************************************************
class CImGuiManager;
class CDebugProc;

//***************************************************
// レンダラークラスの定義
//***************************************************
class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(const int fps);
	static CImGuiManager* GetImGui(void) { return m_pImGui; }

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
	LPDIRECT3D9 m_pD3D;									// Directxデバイスへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;						// Directxデバイスへのポインタ
	UINT m_ResizeWidth, m_ResizeHeight;					// 画面の大きさ変更時の大きさ
	D3DPRESENT_PARAMETERS m_d3dpp;						// プレゼンテーションパラメータ
	float m_fALv;										// A値
	bool m_bDeviceLost;									// デバイスのロスト判定
	bool m_bEffect;
};

#endif
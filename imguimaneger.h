//===================================================
//
// imguiマネージャー [imguimanager.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _IMGUIMANEGER_H_
#define _IMGUIMANEGER_H_

//***************************************************
// インクルードファイル
//***************************************************
#include "main.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include <vector>
#include <string>

//***************************************************
// IMGUIマネージャークラスの定義
//***************************************************
class CImGuiManager
{
public:
	typedef enum
	{
		TYPE_DEFOULT = 0,
		TYPE_NOMOVE,
		TYPE_NOMOVESIZE,
		TYPE_NOMOVEANDSIZE,
		TYPE_MAX
	}TYPE;

	CImGuiManager();
	~CImGuiManager();

	void Init(HWND hWnd, LPDIRECT3DDEVICE9 pDevice);
	void Uninit(void);
	void SetPosition(ImVec2 pos);
	void SetSize(ImVec2 Size);
	void Start(const char* WindowName, TYPE type);
	void EndRenderer(void);
	void NewFrame(void);
	void EndFrame(void) { ImGui::EndFrame(); }
	void End(void) { ImGui::End(); }

	/// <summary>
	/// IMGUIのウィンドウを操作しているか
	/// </summary>
	/// <param name=""></param>
	/// <returns>判定結果</returns>
	bool GetActiveWindow(void);

	/// <summary>
	/// ドラッグ元(テクスチャアイコンの表示)
	/// </summary>
	/// <param name="texture"></param>
	/// <param name="pModelType"></param>
	void ShowTextureIcon(const LPDIRECT3DTEXTURE9 texture, const char* pModelType);
private:
	static bool m_bDraggingModel; // モデルをドラッグしているか
	static std::string m_DraggingModelType; // ドラッグしているモデルの種類
};

#endif // !_IMGUIMANEGER_H_
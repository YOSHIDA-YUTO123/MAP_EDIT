//===================================================
//
// デバッグログ [DebugLog.cpp]
// Author:YOSHIDA YUUTO
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "DebugLog.h"
#include "imguimaneger.h"
#include "manager.h"
#include "renderer.h"
#include "imgui.h"

//===================================================
// コンストラクタ
//===================================================
CDebugLog::CDebugLog()
{
}

//===================================================
// デストラクタ
//===================================================
CDebugLog::~CDebugLog()
{

}

//===================================================
// 生成処理
//===================================================
CDebugLog* CDebugLog::Create(void)
{
	return new CDebugLog;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CDebugLog::Init(void)
{

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CDebugLog::Uninit(void)
{
	// 自分自身の破棄
	CObject::Release();
}

//===================================================
// 更新処理
//===================================================
void CDebugLog::Update(void)
{
	// Imguiの取得
	CImGuiManager* pImgui = CManager::GetRenderer()->GetImGui();

	// nullだったら処理しない
	if (pImgui == nullptr) return;

	pImgui->SetSize(ImVec2(200.0f, 200.0f));
	pImgui->SetPosition(ImVec2(1080.0f, 0.0f));

	pImgui->Start("Log", CImGuiManager::TYPE_NOMOVESIZE);

	// 要素を調べる
	for (auto it = m_Info.begin(); it != m_Info.end(); it++)
	{
		// デバッグログの設定
		ImGui::Text(it->aMessage.c_str());

		it->nTime--;

		// 使用時間が終了したら
		if (it->nTime <= 0)
		{
			it = m_Info.erase(it);
		}

		if (it == m_Info.end()) break;
	}

	pImgui->End();
}

//===================================================
// 描画処理
//===================================================
void CDebugLog::Draw(void)
{
}

//===================================================
// 設定処理
//===================================================
void CDebugLog::Set(const char* pMessage, const int nTime)
{
	CDebugLog::Info info = {};

	info.aMessage = pMessage;
	info.nTime = nTime;

	// リストに設定
	m_Info.push_back(info);
}

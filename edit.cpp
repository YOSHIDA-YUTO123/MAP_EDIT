//===================================================
//
// 編集モード中のImguiの処理をまとめるクラス [edit.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "edit.h"
#include "MapObjectManager.h"
#include "imguimaneger.h"
#include "manager.h"
#include "renderer.h"
#include "MeshFieldManager.h"

//===================================================
// コンストラクタ
//===================================================
CEdit::CEdit()
{
}

//===================================================
// デストラクタ
//===================================================
CEdit::~CEdit()
{
}

//===================================================
// 生成処理
//===================================================
CEdit* CEdit::Create(void)
{
	CEdit* pEdit = new CEdit;

	// 初期化処理
	if (FAILED(pEdit->Init()))
	{
		pEdit->Uninit();
		pEdit = nullptr;
		return nullptr;
	}

	return pEdit;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CEdit::Init(void)
{

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CEdit::Uninit(void)
{
	// 自分自身の破棄
	CObject::Release();
}

//===================================================
// 更新処理
//===================================================
void CEdit::Update(void)
{
	// インスタンスの取得
	CMapObjectManager* pMapObjManager = CMapObjectManager::GetInstance();

	CMeshFieldManager* pMeshFieldManager = CMeshFieldManager::GetInstance();

	// 取得できなかったら処理しない
	if (pMapObjManager == nullptr) return;
	if (pMeshFieldManager == nullptr) return;

	// Imguiの取得
	CImGuiManager* pImgui = CManager::GetRenderer()->GetImGui();

	// 取得できなかったら処理しない
	if (pImgui == nullptr) return;

	// ウィンドウの設定
	pImgui->SetPosition(ImVec2(0.0f, 0.0f));
	pImgui->SetSize(ImVec2(400.0f, 500.0f));
	pImgui->Start(u8"インスペクター", CImGuiManager::TYPE_NOMOVEANDSIZE);

	if (ImGui::BeginTabBar("test000"))
	{
		if (ImGui::BeginTabItem(u8"モデル配置"))
		{
			// 更新処理
			pMapObjManager->Update();

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(u8"フィールド編集"))
		{
			// 更新処理
			pMeshFieldManager->Update();

			ImGui::EndTabItem();
		}
		else
		{
			// 更新処理
			pMeshFieldManager->SetShow(false);
		}

		// タブバーの終了処理
		ImGui::EndTabBar();
	}

	// 終了処理
	pImgui->End();

	// ウィンドウの設定
	pImgui->SetPosition(ImVec2(0.0f, 500.0f));

	ImVec2 window = ImGui::GetWindowSize();
	float fHeight = SCREEN_HEIGHT - 500.0f;

	pImgui->SetSize(ImVec2(400.0f, fHeight));
	pImgui->Start("Asset", CImGuiManager::TYPE_NOMOVEANDSIZE);

	// アイコンの設定処理
	pMapObjManager->SetTextureIcon(pImgui);

	pImgui->End();

}

//===================================================
// 描画処理
//===================================================
void CEdit::Draw(void)
{
}

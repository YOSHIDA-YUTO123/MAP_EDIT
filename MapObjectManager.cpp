//===================================================
//
// マップオブジェクトのマネージャー [MapObjectManager.cpp]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// インクルードファイル
//***************************************************
#include "MapObjectManager.h"
#include "imguimaneger.h"
#include "manager.h"
#include "renderer.h"
#include "MapObject.h"
#include "modelManager.h"
#include "input.h"
#include "EditMapObject.h"

//***************************************************
// 定数宣言
//***************************************************
constexpr int MAX_WORD = 256; // 最大の文字数

//***************************************************
// 静的メンバ変数
//***************************************************
std::unique_ptr< CMapObjectManager> CMapObjectManager::m_pInstance = nullptr;	// 自分のインスタンス
std::vector<CMapObject*> CMapObjectManager::m_pMapObjectList = {};				// マップオブジェクトのリスト
std::vector<std::string> CMapObjectManager::m_aModelPath = {};					// モデルのパスのリスト

//===================================================
// インスタンスの取得
//===================================================
CMapObjectManager* CMapObjectManager::GetInstance(void)
{
	// 自分が生成されていなかったら
	if (m_pInstance == nullptr)
	{
		// インスタンスの生成
		m_pInstance.reset(new CMapObjectManager);
		m_pInstance->Init();
	}

	return m_pInstance.get();
}

//===================================================
// マップオブジェクトの生成処理
//===================================================
CMapObject* CMapObjectManager::Create(const D3DXVECTOR3 pos, const char* pModelFileName)
{
	// マップオブジェクトの生成
	CMapObject* pMapObject = CMapObject::Create(pos, pModelFileName);

	if (pMapObject == nullptr)
	{
		MessageBox(NULL, "モデルがありません", "モデルを登録してください", MB_OK | MB_ICONWARNING);
		return nullptr;
	}

	// リストに追加
	m_pMapObjectList.push_back(pMapObject);

	return pMapObject;
}

//===================================================
// 初期化処理
//===================================================
HRESULT CMapObjectManager::Init(void)
{
	// モデルのマネージャーの取得
	CModelManager* pModelManager = CManager::GetModel();

	// 取得できなかったら処理しない
	if (pModelManager == nullptr)
	{
		assert(false && "モデルマネージャー取得失敗");

		return E_FAIL;
	}

	// モデルの情報の取得
	CModelManager::ModelInfo modelInfo = pModelManager->GetModelInfo(0);

	// 編集用マップオブジェクトの生成
	m_pEditMapObj = CEditMapObject::Create();

	//// マップオブジェクトの生成
	//m_pSelect = CMapObjectManager::Create({ -500.0f,0.0f,0.0f }, modelInfo.filepath);

	return S_OK;
}

//===================================================
// 更新処理
//===================================================
void CMapObjectManager::Update(void)
{
	// Imguiの取得
	CImGuiManager* pImgui = CManager::GetRenderer()->GetImGui();

	// 取得できなかったら処理しない
	if (pImgui == nullptr) return;

	// ウィンドウの設定
	pImgui->SetPosition(ImVec2(0.0f, 0.0f));
	pImgui->SetSize(ImVec2(300.0f, 200.0f));
	pImgui->Start("aaa", CImGuiManager::TYPE_NOMOVEANDSIZE);

	// モデルのパスの取得
	GetModelPath();

	// モデルのパスの登録
	Register();

	// モデルのパスのリストの表示
	SetModelPathList();

	if (ImGui::BeginTabBar("test0000"))
	{
		if (ImGui::BeginTabItem(u8"配置"))
		{
			if (m_pEditMapObj != nullptr)
			{
				// 表示する
				m_pEditMapObj->SetShow(true);
			}

			// 編集用オブジェとの更新処理
			UpdateEditMapObj();

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem(u8"インスペクター"))
		{
			if (m_pEditMapObj != nullptr)
			{
				// 表示しない
				m_pEditMapObj->SetShow(false);
			}

			// 選択中のモデルの処理
			SetSelectObj();

			// モデルのインスペクター
			SetInspector();

			ImGui::EndTabItem();
		}

		// タブバーの終了処理
		ImGui::EndTabBar();
	}

	// キーボードの取得
	CInputMouse* pMouse = CManager::GetInputMouse();

	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	ImGuiIO& io = ImGui::GetIO();
	
	// IMGUIのウィンドウを操作していないなら
	if (!io.WantCaptureMouse && ImGui::IsMouseClicked(0))
	{
		// 左クリックを押したら&&視点操作をしていないなら
		if (pMouse->OnMouseTriggerDown(CInputMouse::LEFT) && !pKeyboard->GetPress(DIK_LALT))
		{
			// マウスとの当たり判定
			CollisionMouse();
		}
	}

	// 終了処理
	pImgui->End();
}

//===================================================
// コンストラクタ
//===================================================
CMapObjectManager::CMapObjectManager()
{
	m_pSelect = nullptr;
	m_nType = 0;
	m_pEditMapObj = nullptr;
}

//===================================================
// モデルのパスの取得
//===================================================
void CMapObjectManager::GetModelPath(void)
{
	// モデルのマネージャーの取得
	CModelManager* pModelManager = CManager::GetModel();

	// 取得できなかったら処理しない
	if (pModelManager == nullptr) return;

	// モデルの総数分調べる
	for (auto itr = pModelManager->GetList().begin(); itr != pModelManager->GetList().end(); itr++)
	{
		// ファイルパスの取得
		std::string modelpath = itr->filepath;

		// パスのリストを調べるp
		for (auto pathList = m_aModelPath.begin(); pathList != m_aModelPath.end(); pathList++)
		{
			// すでに登録済みだったら
			if (pathList->find(modelpath) != std::string::npos)
			{
				// 関数を抜ける
				return;
			}
		}

		m_aModelPath.push_back(modelpath);
	}
}

//===================================================
// モデルのパスのリスト
//===================================================
void CMapObjectManager::SetModelPathList(void)
{
	if (m_aModelPath.empty()) return;

	if (ImGui::BeginCombo(u8"モデルのリスト", m_aModelPath[m_nType].c_str()))
	{
		int nCnt = 0;

		// パスのリストを調べるp
		for (auto pathList = m_aModelPath.begin(); pathList != m_aModelPath.end(); pathList++)
		{
			// 選択しているものか
			const bool bSelect = m_nType == nCnt;

			if (ImGui::Selectable(pathList->c_str(), bSelect))
			{
				m_nType = nCnt;
			}

			if (bSelect)
			{
				ImGui::SetItemDefaultFocus();
			}

			nCnt++;
		}
		ImGui::EndCombo();
	}
}

//===================================================
// 選択中のモデルの処理
//===================================================
void CMapObjectManager::SetSelectObj(void)
{
	// 要素を調べる
	for (auto itr = m_pMapObjectList.begin(); itr != m_pMapObjectList.end(); itr++)
	{
		// nullだったら処理を飛ばす
		if ((*itr) == nullptr) continue;

		// 選択中だったら
		if (m_pSelect == (*itr) && m_pSelect != nullptr)
		{
			m_pSelect->SetALv(0.5f);
		}
		// 選択されていないなら
		else
		{
			(*itr)->SetALv(1.0f);
		}
	}

	// 選択されていないなら処理しない
	if (m_pSelect == nullptr) return;

	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// 視点操作をしていないなら
	if (!pKeyboard->GetPress(DIK_LALT))
	{
		m_pSelect->SetMouseDrag();
	}
}

//===================================================
// インスペクターの設定
//===================================================
void CMapObjectManager::SetInspector(void)
{
	if (m_pSelect != nullptr)
	{
		m_pSelect->SetInfo();
	}
}

//===================================================
// 編集用のマップオブジェクトの更新処理
//===================================================
void CMapObjectManager::UpdateEditMapObj(void)
{
	// nullなら処理しない
	if (m_pEditMapObj == nullptr) return;

	// 編集用モデルの位置の取得
	D3DXVECTOR3 pos = m_pEditMapObj->GetPosition();

	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	if (ImGui::Button(u8"生成") || pKeyboard->GetTrigger(DIK_RETURN))
	{
		// モデルの生成
		Create(pos, m_aModelPath[m_nType].c_str());
	}

	// 位置の情報
	ImGui::DragFloat3("", pos, 0.5f);

	// 位置の設定
	m_pEditMapObj->SetPosition(pos);

	// モデルの種類の設定
	m_pEditMapObj->LoadModel(m_aModelPath[m_nType].c_str());
}

//===================================================
// マウスの当たり判定
//===================================================
bool CMapObjectManager::CollisionMouse(void)
{
	// いちばん近い距離
	float fDistanceNear = 0.0f;

	// 最初に代入したかどうか
	bool bFirst = false;
	bool bHit = false;

	// 要素を調べる
	for (auto itr = m_pMapObjectList.begin(); itr != m_pMapObjectList.end(); itr++)
	{
		// nullだったら処理を飛ばす
		if ((*itr) == nullptr) continue;

		float fDistance = 0.0f;

		// 当たったら
		if ((*itr)->CollisionMouse(&fDistance))
		{
			bHit = true;
		}
		// 最初の代入が終わっていないなら
		if (!bFirst)
		{
			// 近い距離の設定
			fDistanceNear = fDistance;

			// 選択中のオブジェクトのポインタの設定
			m_pSelect = (*itr);

			bFirst = true;
		}
		else if(fDistanceNear > fDistance)
		{
			// 近い距離の更新
			fDistanceNear = fDistance;

			// 選択中のオブジェクトのポインタの設定
			m_pSelect = (*itr);
		}
	}

	if (!bHit)
	{
		// 一度もあたらないなら
		m_pSelect = nullptr;
	}

	return bHit;
}

//===================================================
// マップオブジェクトの登録
//===================================================
HRESULT CMapObjectManager::Register(void)
{
	// モデルのマネージャーの取得
	CModelManager* pModelManager = CManager::GetModel();

	// 取得できなかったら処理しない
	if (pModelManager == nullptr) return E_FAIL;

	// 文字列
	static char pModelName[MAX_WORD] = {};

	ImGui::SetNextItemWidth(200); // 入力欄の幅を指定

	if (ImGui::InputText(u8"モデルの登録", pModelName, sizeof(pModelName), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		// 省略用パス
		std::string modelPath = "data/MODEL/";
		std::string findPath = pModelName;

		// 含まれていたら
		if (findPath.find("data/MODEL/") != std::string::npos)
		{
			// パスの設定
			modelPath = pModelName;
		}
		else
		{
			// 連結する
			modelPath += pModelName;
		}

		// パスのリストを調べる
		for (auto pathList = m_aModelPath.begin(); pathList != m_aModelPath.end(); pathList++)
		{
			// すでに登録済みだったら
			if (pathList->find(modelPath) != std::string::npos)
			{
				MessageBox(NULL, modelPath.c_str(), "すでに登録されています", MB_OK);
				return E_FAIL;
			}
		}
	}

	return S_OK;
}

//===================================================
// デストラクタ
//===================================================
CMapObjectManager::~CMapObjectManager()
{
}

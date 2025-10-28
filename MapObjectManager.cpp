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
#include "json.hpp"
#include <fstream>
#include "math.h"
#include "imgui_internal.h"
#include "DebugLog.h"
#include "camera.h"
#include "textureManager.h"
#include "IconModel.h"
#include "object3D.h"
#include "TextureMTManager.h"
#include "transform.h"
#include"MapObjectList.h"

// jsonの使用
using json = nlohmann::json;

//***************************************************
// 定数宣言
//***************************************************
constexpr int MAX_WORD = 256; // 最大の文字数
const char* JSON_FILE = "data/stage000.json"; // 使用するjsonファイル

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
CMapObject* CMapObjectManager::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const char* pModelFileName, const char* pType)
{
	// マップオブジェクトの生成
	CMapObject* pMapObject = CMapObject::Create(pos, rot, pModelFileName, pType);

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

	// アイコンの生成
	CIconModel::Create();

	LoadFileName();

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

	// キーボードの取得
	CInputMouse* pMouse = CManager::GetInputMouse();

	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// モデルのパスの取得
	GetModelPath();

	// モデルのパスの登録
	Register();

	// ファイルパスの設定
	SetFilePath();

	// モデルのパスのリストの表示
	SetModelPathList();

	// 配置モードかどうか
	static bool bSetMode = true;

	// カメラのフォーカス処理
	SetCamerafocus();

	CopyAndPaste(pKeyboard);

	if (ImGui::Checkbox(u8"オブジェクトを平面で移動させる", &m_bDragMoveXZ))
	{
		//// 状態の切り替え
		//m_bDragMoveXZ = m_bDragMoveXZ ? false : true;
	}
	else if (pKeyboard->GetPress(DIK_LSHIFT))
	{
		m_bDragMoveXZ = true;
	}
	else if(pKeyboard->GetRelease(DIK_LSHIFT))
	{
		m_bDragMoveXZ = false;
	}

	if (ImGui::BeginTabBar("test000"))
	{
		if (ImGui::BeginTabItem(u8"配置オブジェクト設定"))
		{
			bSetMode = false;

			// モデルのインスペクター
			SetInspector();

			// 選択中のオブジェクトの処理
			UpdateSelectObj();

			ImGui::EndTabItem();
		}

		// タブバーの終了処理
		ImGui::EndTabBar();
	}

	// 選択中のモデルの処理
	SetSelectObjAlv();
	
	// IMGUIのウィンドウを操作していないなら
	if (!pImgui->GetActiveWindow())
	{
		// 左クリックを押したら&&視点操作をしていないなら
		if (pMouse->OnMouseTriggerDown(CInputMouse::LEFT) && !pKeyboard->GetPress(DIK_LALT))
		{
			// マウスとの当たり判定
			CollisionMouse();
		}
	}
	
	SetLoadFile();

	if (ImGui::Button(u8"セーブする", ImVec2(300.0, 0)))
	{
		int nID = MessageBox(NULL, "配置情報のセーブ", "セーブしますか ?", MB_YESNO);

		if (nID == IDYES)
		{
			// デバッグログの取得
			CDebugLog *pDebugLog = CManager::GetDebugLog();

			if (pDebugLog != nullptr)
			{
				pDebugLog->Set(u8"セーブ完了!!!", 120);
			}
			// セーブ
			Save();
		}
	}

	if (ImGui::Button(u8"ロード", ImVec2(300.0, 0)))
	{
		int nID = MessageBox(NULL, "ステージのロード", "ロードしますか ?", MB_YESNO);

		if (nID == IDYES)
		{
			// デバッグログの取得
			CDebugLog* pDebugLog = CManager::GetDebugLog();

			if (pDebugLog != nullptr)
			{
				pDebugLog->Set(u8"ロード完了!!!", 120);
			}

			// ロード
			Load();
		}
	}
}

//===================================================
// コンストラクタ
//===================================================
CMapObjectManager::CMapObjectManager()
{
	m_pCopyObj = nullptr;
	m_fMove = MOVE_VALUE;
	m_bDragMoveXZ = false;
	m_pSelect = nullptr;
	m_nType = 0;
}

//===================================================
// モデルのパスの取得
//===================================================
void CMapObjectManager::GetModelPath(void)
{
	// マップのオブジェクトのリストの取得
	CMapObjectList* pMapObjectList = CManager::GetMapObjectList();

	// 取得できなかったら処理しない
	if (pMapObjectList == nullptr) return;

	// モデルの総数分調べる
	for (auto &list : pMapObjectList->GetList())
	{
		// ファイルパスの取得
		std::string modelpath = list.filepath;

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

	if (ImGui::BeginCombo("##", m_aModelPath[m_nType].c_str()))
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
void CMapObjectManager::SetSelectObjAlv(void)
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
}

//===================================================
// インスペクターの設定
//===================================================
void CMapObjectManager::SetInspector(void)
{
	if (m_pSelect != nullptr)
	{
		// 情報の設定
		m_pSelect->SetInfo();
	}
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

	// マップのオブジェクトのリストの取得
	CMapObjectList* pMapObjectList = CManager::GetMapObjectList();

	// 取得できなかったら処理しない
	if (pMapObjectList == nullptr) return E_FAIL;

	// 文字列
	static char pModelName[MAX_WORD] = {};

	ImGui::SetNextItemWidth(200); // 入力欄の幅を指定

	if (ImGui::InputText(u8"モデルの登録", pModelName, sizeof(pModelName), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		// 省略用パス
		std::string modelPath = "data/MODEL/MapObjectFile/";
		std::string findPath = pModelName;

		// 含まれていたら
		if (findPath.find("data/MODEL/MapObjectFile/") != std::string::npos)
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
		for (auto& pathList : m_aModelPath)
		{
			// すでに登録済みだったら
			if (pathList.find(modelPath) != std::string::npos)
			{
				MessageBox(NULL, modelPath.c_str(), "すでに登録されています", MB_OK);
				return E_FAIL;
			}
		}

		int nIdx = pModelManager->Register(modelPath.c_str());

		if (nIdx == -1) return E_FAIL;

		m_aModelPath.push_back(modelPath);

		pMapObjectList->Register();
	}

	return S_OK;
}

//===================================================
// 選択中のオブジェクトの更新処理
//===================================================
void CMapObjectManager::UpdateSelectObj(void)
{
	// 選択されていないなら処理しない
	if (m_pSelect == nullptr) return;

	// 要素がないなら処理しない
	if (m_aModelPath.empty()) return;

	// キーボードの取得
	CInputKeyboard* pKeyboard = CManager::GetInputKeyboard();

	// Imguiの取得処理
	CImGuiManager* pImgui = CManager::GetRenderer()->GetImGui();

	// 視点操作をしていないなら
	if (!pKeyboard->GetPress(DIK_LALT) && m_pSelect != nullptr)
	{
		if (pImgui != nullptr && !pImgui->GetActiveWindow())
		{
			// マウスのドラッグ処理
			m_pSelect->SetMouseDrag();
		}
	}

	// 移動の設定
	m_pSelect->SetMove(m_fMove);
	m_pSelect->UpdateMove();

	if (ImGui::Button(u8"種類の適応", ImVec2(300.0, 0)))
	{
		// 種類の変更
		m_pSelect->Register(m_aModelPath[m_nType].c_str());
	}

	if (ImGui::Button(u8"向きのリセット", ImVec2(300.0, 0)))
	{
		// 空間情報の取得
		CTransform* pTransform = m_pSelect->GetTransform();

		// 取得出来たら
		if (pTransform != nullptr)
		{
			// 情報の取得
			CTransform::Info transformInfo = pTransform->GetInfo();

			transformInfo.rot = Const::VEC3_NULL;

			// 向きの設定
			pTransform->SetInfo(transformInfo);
		}
	}

	if (pKeyboard->GetTrigger(DIK_DELETE) || ImGui::Button(u8"消去", ImVec2(300.0, 0)))
	{
		// リストから消去
		Erase(m_pSelect);
		m_pSelect = nullptr;
	}
}

//===================================================
// セーブ処理
//===================================================
void CMapObjectManager::Save(void)
{
	json config;

	// 要素を調べる
	for (auto itr = m_pMapObjectList.begin(); itr != m_pMapObjectList.end(); itr++)
	{
		// nullだったら処理しない
		if ((*itr) == nullptr) continue;

		// 空間情報の取得
		CTransform* pTransform = (*itr)->GetTransform();

		// 取得出来たら
		if (pTransform == nullptr) continue;
		
		// 情報の取得
		CTransform::Info transformInfo = pTransform->GetInfo();
	
		// 位置の取得
		D3DXVECTOR3 pos = transformInfo.pos;

		// 向きの取得
		D3DXVECTOR3 rot = transformInfo.rot;

		json obj =
		{
			{"file_path",(*itr)->GetPath()},
			{"position",{{"x",pos.x},{"y",pos.y},{"z",pos.z}}},
			{"rotation",{{"x",rot.x},{"y",rot.y},{"z",rot.z}}},
			{"type",(*itr)->GetType()},
		};

		config["MODEL_INFO"].push_back(obj);
	}

	// ファイルを開く
	std::ofstream file(m_aLoadFileName);

	// ファイルが開けたら
	if (file.is_open())
	{
		// ファイルに書き出し
		file << config.dump(4);

		file.clear();
		file.close();
	}
	else
	{
		MessageBox(NULL, "ファイルが開けませんでした", m_aLoadFileName.c_str(), MB_OK);
	}
}

//===================================================
// ロード処理
//===================================================
void CMapObjectManager::Load(void)
{
	json config;

	// ファイルを開く
	std::ifstream file(m_aLoadFileName);

	if (file.is_open())
	{
		file >> config;
		file.clear();
		file.close();
	}
	else
	{
		MessageBox(NULL, "ファイルが開けませんでした", m_aLoadFileName.c_str(), MB_OK);
		return;
	}

	// 要素分調べる
	for (auto& obj : config["MODEL_INFO"])
	{
		std::string filepath = obj["file_path"];
		float posX = obj["position"]["x"];
		float posY = obj["position"]["y"];
		float posZ = obj["position"]["z"];

		float rotX = obj["rotation"]["x"];
		float rotY = obj["rotation"]["y"];
		float rotZ = obj["rotation"]["z"];

		std::string type = obj["type"];

		// モデルの生成
		Create(D3DXVECTOR3(posX, posY, posZ), D3DXVECTOR3(rotX, rotY, rotZ), filepath.c_str(), type.c_str());
	}
}

//===================================================
// リストの連結の解除
//===================================================
void CMapObjectManager::Erase(CMapObject* pObj)
{
	// 要素を調べる
	for (auto itr = m_pMapObjectList.begin(); itr != m_pMapObjectList.end(); itr++)
	{
		// nullだったら処理しない
		if ((*itr) == nullptr) continue;

		if ((*itr) == pObj)
		{
			// 終了処理
			(*itr)->Uninit();

			// 間を埋める
			itr = m_pMapObjectList.erase(itr);

			pObj = nullptr;
		}

		// 最後尾だったら抜ける
		if (itr == m_pMapObjectList.end()) break;
	}
}

//===================================================
// ファイルパスの設定処理
//===================================================
void CMapObjectManager::SetFilePath(void)
{
	// マップオブジェクトのマネージャークラスの取得
	CMapObjectList* pMapObjectList = CManager::GetMapObjectList();

	// 取得できなかったら処理しない
	if (pMapObjectList == nullptr) return;
	
	// 登録処理
	pMapObjectList->Register();

	for (auto& modellist : pMapObjectList->GetList())
	{
		// すでにリストに登録されているか調べる
		auto it = std::find(m_aModelPath.begin(), m_aModelPath.end(), modellist.filepath);

		// 見つからないなら
		if (it == m_aModelPath.end())
		{
			m_aModelPath.push_back(modellist.filepath);
		}
	}
}

//===================================================
// カメラのフォーカス
//===================================================
void CMapObjectManager::SetCamerafocus(void)
{
	if (ImGui::Button(u8"カメラを選択モデルにフォーカス"))
	{
		D3DXVECTOR3 selectPos = Const::VEC3_NULL;

		// 空間情報の取得
		CTransform* pTransform = m_pSelect->GetTransform();

		if (m_pSelect != nullptr && pTransform != nullptr)
		{
			// 情報の取得
			CTransform::Info transformInfo = pTransform->GetInfo();

			// 位置の取得
			selectPos = transformInfo.pos;
		}

		// カメラの取得
		CCamera* pCamera = CManager::GetCamera();

		// nullだったら処理しない
		if (pCamera == nullptr)return;

		pCamera->SetPosV(selectPos);
		pCamera->SetPosR(selectPos);
		pCamera->UpdatePositionV();
		pCamera->UpdatePositionR();
	}
}

//===================================================
// テクスチャのアイコンの設定
//===================================================
void CMapObjectManager::SetTextureIcon(CImGuiManager* pImgui)
{
	// モデルパスのリストが無いなら
	if (m_aModelPath.empty()) return;

	// テクスチャのマネージャーの取得
	CTextureManager* pTexture = CManager::GetTexture();

	// 取得できなかったら処理しない
	if (pTexture == nullptr) return;

	// マップのオブジェクトのリストの取得
	CMapObjectList* pMapObjectList = CManager::GetMapObjectList();

	// 取得できなかったら処理しない
	if (pMapObjectList == nullptr) return;

	// テクスチャMTの取得
	CTextureMTManager* pTextureMT = CManager::GetTextureMT();

	// 取得できなかったら処理しない
	if (pTextureMT == nullptr) return;

	for (auto& modelList : pMapObjectList->GetList())
	{
		// パスの登録
		pTextureMT->GetAddress(modelList.filepath);
	}

	int nCnt = 0;

	for (auto& modelList : pTextureMT->GetList())
	{
		// モデルの名前の取得
		const char* pModelName = modelList.aName.c_str();

		// テクスチャのアイコンの表示
		pImgui->ShowTextureIcon(pTextureMT->GetAddress(pModelName), pModelName);

		// 3つ表示していないなら
		if (nCnt != 3)
		{
			ImGui::SameLine();
			nCnt++;
		}
		else
		{
			nCnt = 0;
		}
	}

	// 改行
	ImGui::NewLine();
}

//===================================================
// コピーペースト処理
//===================================================
void CMapObjectManager::CopyAndPaste(CInputKeyboard *pKeyboard)
{
	// 選択されていなかったら処理しない
	if (m_pCopyObj != nullptr && pKeyboard->GetPress(DIK_LCONTROL) && pKeyboard->GetTrigger(DIK_V))
	{
		// 空間情報の取得
		CTransform* pTransform = m_pCopyObj->GetTransform();

		if (m_pSelect != nullptr && pTransform != nullptr)
		{
			// 情報の取得
			CTransform::Info transformInfo = pTransform->GetInfo();

			// 位置の取得
			D3DXVECTOR3 pos = transformInfo.pos;
			D3DXVECTOR3 rot = transformInfo.rot;

			// パスの取得
			const char* pModelPath = m_pCopyObj->GetPath();

			// 生成
			Create(pos, rot, pModelPath, m_pCopyObj->GetType());
		}
	}

	// ctrl + c
	if (pKeyboard->GetPress(DIK_LCONTROL) && pKeyboard->GetTrigger(DIK_C))
	{
		m_pCopyObj = m_pSelect;
	}
}

//===================================================
// ファイル名のロード
//===================================================
void CMapObjectManager::LoadFileName(void)
{
	std::fstream file("data/system.ini");
	std::string line;

	std::string stageFile;

	if (file.is_open())
	{
		// ファイルを一行づつ読み取る
		while (std::getline(file, line))
		{
			if (line.find("STAGE_JSONFILE") != std::string::npos)
			{
				// = の位置を求める
				size_t pos = line.find('=');

				stageFile = line.substr(pos + 1);

				stageFile.erase(0, stageFile.find_first_not_of(" \t\"")); // 前の空白の除去
				stageFile.erase(stageFile.find_last_not_of(" \t\"") + 1); // 後ろの空白の除去
			}
		}

		file.clear();
		file.close();
	}

	m_aLoadFileName = stageFile;
}

//===================================================
// 読み込むファイルの設定
//===================================================
void CMapObjectManager::SetLoadFile(void)
{
	// 文字列
	static char pModelName[MAX_WORD] = {};

	ImGui::Text(u8"現在のファイル名 [ %s ]", m_aLoadFileName.c_str());

	ImGui::SetNextItemWidth(200); // 入力欄の幅を指定

	if (ImGui::InputText(u8"ファイル設定", pModelName, sizeof(pModelName), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		m_aLoadFileName = pModelName;
	}
}

//===================================================
// デストラクタ
//===================================================
CMapObjectManager::~CMapObjectManager()
{
}
